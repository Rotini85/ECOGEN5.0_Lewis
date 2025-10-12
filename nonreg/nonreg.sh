#!/bin/bash
#set -e # Exit with nonzero exit code if anything fails

# -------------------------------------------------------------------------
#
# Configuration for non regression tests
#
# Arguments: see error handling
#
# Assigns global variables:
#   TEST_OUTPUTS - output filename for `run.sh` script (terminal test outputs)
#   TEST_STATUSES - report filename for `run.sh` script (test exit statuses)
#   REPORT_DIR - name of folder for report storage
#   MAIN_WRKTREE_DIR - path of the main worktree
#
# Creates:
#   REPORT_DIR - folder to store reports
#
# Moves:
#   results -> results_save folder
#
# Deletes:
#   nonreg/results_reference - reference results from a previous non regression run
#   nonreg/results_validation - validation results from a previous non regression run
#
# Returns:  None
#
# Examples:
#   configure_nonreg libTests/tests.list origin/devel origin/feature/my-new-feature
#
# -------------------------------------------------------------------------
configure_nonreg() {

  local n=3

  if [ $# != $n ]
  then
    echo ""
    echo "build_branch: Missing arguments."
    echo "    Expects $n args:"
    echo "         <test_list> - name of file containing non regression tests"
    echo "         <reference_branch> - branch to use as reference"
    echo "         <validation_branch> - branch to validate"
    echo ""
    return 1
  fi

  local test_list reference_branch validation_branch
  test_list=$1
  reference_branch=$2
  validation_branch=$3

  #Settings
  #--------
  TEST_OUTPUTS="mainOutput.out"
  TEST_STATUSES="report.out"

  echo "Branch to validate: $validation_branch"
  echo "Validation against branch: $reference_branch"

  local validationBranchSimplified
  validationBranchSimplified=$(echo "$validation_branch" | sed -e "s/\//_/g")

  # Stores the absolute path of the main worktree directory
  MAIN_WRKTREE_DIR=$(pwd)

  #Creating folder for report storage
  #----------------------------------
  # Use absolute path for REPORT_DIR to ensure to save all outputs in the main
  # worktree
  REPORT_DIR="$(pwd)/nonreg/reports/$(date +%Y%m%d_%H%M)_$validationBranchSimplified"
  mkdir -p "$REPORT_DIR"

  #Cleaning results
  #----------------
  run_cmd rm -Rf "$REPORT_DIR"/results_reference "$REPORT_DIR"/results_validation
  run_cmd echo "nonreg/results_reference and nonreg/results_validation deleted"

  # Moves results directory to preserve any pre-existing results (otherwise move
  # the new empty 'results' directory)
  if [ -d results ]
  then
    if ! mv -v results results_save
    then
      return 1
    fi
  fi

  #Check if Gmsh is installed
  #--------------------------

  #On Ubuntu Gmsh redirects to stdout while on MacOS
  #it redirects to stderr (even if success)
  if ! gmsh --version &> /dev/null
  then
    echo "WARNING: Gmsh is not installed or not avalaible in your PATH."
    echo "All unstructured nonreg test cases will not tested."
  fi
}

# -------------------------------------------------------------------------
#
# Build current branch.
#
# Arguments: see error handling
#
# Uses global variables:
#   REPORT_DIR - name of directory to save regression results
#
# Assigns:
#   error - set error to 1 if build fails
#
# Returns:  0 for success, 1 for failure
#
# Examples:
#   > build_branch test_statuses_file.out my_branch_name
#
# -------------------------------------------------------------------------
build_branch() {

  local n=2

  if [ $# != $n ]
  then
    echo ""
    echo "build_branch: Missing arguments."
    echo "    Expects $n args:"
    echo "         <output_file> - name of output compilation report file"
    echo "         <branch> - name of current branch (used for output messages only)"
    echo ""
    return 1
  fi

  local output_file branch
  output_file="$1"
  branch="$2"

  echo "Cleaning and compiling using Makefile..."
  make clean > "$REPORT_DIR/$output_file"

  if make -j 4 >> "$REPORT_DIR/$output_file" 2>&1
  then
    echo "Compilation OK"
    return 0
  else
    local sha
    sha="$(git rev-parse --short HEAD)"
    echo "Compilation failed. Running on $branch (commit $sha) abort."
    error=1
    return $error
  fi
}

# -------------------------------------------------------------------------
#
# Test current branch.
#
# Arguments: see error handling
#
# Uses global variables:
#   TEST_OUTPUTS - output filename for `run.sh` script (terminal test outputs)
#   TEST_STATUSES - report filename for `run.sh` script (test exit statuses)
#   REPORT_DIR - name of folder for report storage
#
# Moves:
#   results -> <REPORT_DIR>/<results_dir>
#
# Assigns:
#   error - set error to 1 if run.sh fails (i.e. contains at least 1 failing test)
#
# Returns:  0 for success, 1 for failure
#
# Examples:
#   test_branch my_test_list.list my_result_dir branch_name
#
# -------------------------------------------------------------------------
test_branch() {

  local n=3
  if [ $# != $n ]
  then
    echo ""
    echo "test_branch: Missing arguments."
    echo "    Expects $n args:"
    echo "          <test_list> - file with the list of non regression tests"
    echo "          <results_dir> - name of results directory"
    echo "          <branch> - name of branch"
    echo ""
    return 1
  fi

  local sha
  sha="$(git rev-parse --short HEAD)"

  local test_list results_dir branch

  test_list=$1
  results_dir=$2
  branch=$3

  echo "Running non-regression tests for $branch branch (commit $sha)... More details in main output file ./results/$TEST_OUTPUTS and report file ./results/$TEST_STATUSES"
  chmod u+x ./scripts/run.sh

  # Mark the script as "failing"
  local err=0
  if ! ./scripts/run.sh -l "$test_list" -o ./results/"$TEST_OUTPUTS" -r ./results/"$TEST_STATUSES"
  then
    echo "Error: some of the non regression tests are failing..."
    printf '\n%s\n' "-- Step status: Failed"
    error=1
    err=1
  else
   printf '\n%s\n' "-- Step status: Succeed"
  fi

  mv -v results "$REPORT_DIR"/"$results_dir"

  return "$err"
}

# -------------------------------------------------------------------------
#
# Eval differences in git tree based on git diff arguments stored in
# <diff_args> variable and prune the output to keep only added or
# deleted lines depending on the sign ('+' or '-') passed as argument.
#
# Examples:
#   prune_diff '+'
#   prune_diff '-'
#
# -------------------------------------------------------------------------
prune_diff() {
  local n=1
  if [ $# != $n ]
  then
    echo ""
    echo "prune_diff: Missing arguments."
    echo "    Expects $n args:"
    echo "          <sign> - '+' or '-' to search for added or removed diffs"
    echo ""
    return 1
  fi

  local sign="$1"

  eval git diff "$diff_args" | grep "^$sign"  | grep -E '[[:alnum:]]' | grep -vE "^\\$sign\\$sign\\$sign"
}


# -------------------------------------------------------------------------
#
# Count diff lines based on git diff arguments stored in <diff_args>
# variable and the given sign ('+' or '-') passed as argument.
#
# Examples:
#   count_line_diff '+'
#   count_line_diff '-'
#
# -------------------------------------------------------------------------
count_line_diffs() {
  local n=1
  if [ $# != $n ]
  then
    echo ""
    echo "count_line_diff: Missing arguments."
    echo "    Expects $n args:"
    echo "          <sign> - '+' or '-' to search for added or removed diffs"
    echo ""
    return 1
  fi

  prune_diff "$1" | wc -l
}

# -------------------------------------------------------------------------
#
# Store diff lines based on git diff arguments stored in <diff_args>
# variable and the given sign ('+' or '-') passed as argument.
#
# Examples:
#   store_diff_list '+'
#   store_diff_list '-'
#
# -------------------------------------------------------------------------
store_diff_list() {
  local n=1
  if [ $# != $n ]
  then
    echo ""
    echo "store_diff_list: Missing arguments."
    echo "    Expects $n args:"
    echo "          <sign> - '+' or '-' to search for added or removed diffs"
    echo ""
    return 1
  fi

  local sign="$1"
  prune_diff "$sign" | sed s/'^'"$sign"// | awk NF=1 | sort | uniq
}

# -------------------------------------------------------------------------
#
# Write in EXCLUDE_LIST the command line arguments that have to be passed
# to the diff command to ignore:
#   - the output directories of:
#      - tests whose inputs have been modified
#      - tests added in the non regression test list
#      - tests deleted from the non regression test list
#   - the lines containing tests paths added/deleted from the non-reg test
#     list (only if the second argument is 1)
#
# The list of test cases paths has to be passed as first argument. If the
# second argument is 0, ignore only the ouptut test directories, if the
# second argument is 1, ignore both the output test directories and lines
# containing test paths.
#
# Modifies global variable:
#   EXCLUDE_LIST: concatenation of the diff exclusion commands
#
# Example:
#   exclude_from_diff "./dir1/dir2/ dir3/dir4" 0
#   exclude_from_diff "./dir5" 1
#
# -------------------------------------------------------------------------
exclude_from_diff() {
  local n=2
  if [ $# != $n ]
  then
    echo ""
    echo "exclude_from_diff: Missing arguments."
    echo "    Expects $n args:"
    echo "          <test_cases_paths> - list of test cases paths"
    echo "          <ignore_test_paths> - 1 to ignore lines containing"
    echo "                                the test paths"
    echo ""
    return 1
  fi

  while IFS= read -r filepath
  do
    # Find the main.xml file in the given path and extract the run names
    if [ -d "$filepath" ]
    then
      while IFS= read -r run_name
      do
        EXCLUDE_LIST+=" -x $run_name"
        if [ "$2" -eq "1" ]
        then
          EXCLUDE_LIST+=" --ignore-matching-lines=$filepath"
        fi
      done < <(find "$filepath" -type f -name "main.xml" -exec sed -n 's:.*<run>\(.*\)</run>.*:\1:p' {} +)
    fi
  done <<< "$1"
}

# -------------------------------------------------------------------------
#
# Check non regression test cases modifications.
# Ignore whitespace changes and blank lines.
#
# Arguments: see error handling
#
# Assigns global variables:
#   INPUT_DIFFS: list of non regression test cases with modified inputs
#   ADDED_LIST: list of non regression tests added in <test_list>
#   REMOVED_LIST: list of non regression tests deleted from <test_list>
#   EXCLUDE_LIST: computed diff exclusion arguments
#
# Returns:
#    0 for success (no diff, news tests added to the test list or modification
#      of the inputs of existing tests),
#    1 if tests have been deleted from the test list
#    2 for unexpected failure
#
# Examples:
#   eval_diff my_test_list.list origin/devel ""
#   eval_diff my_test_list.list origin/devel my_branch
#
#
# -------------------------------------------------------------------------
eval_diff() {

  local n=3
  if [ $# != $n ]
  then
    echo ""
    echo "count_diff_lines: Missing arguments."
    echo "    Expects $n args:"
    echo "          <test_list> - file with the list of non-regression tests"
    echo "          <reference_branch> - name of reference branch"
    echo "          <validation_branch> - name of validation branch"
    echo "                                An empty string can be passed to check uncommited changes in validation branch."
    echo ""
    return 2
  fi

  local test_list reference_branch validation_branch
  test_list="$1"
  reference_branch="$2"
  validation_branch="$3"

  # First: check for modifications in existing test cases inputs (directory add or removal are ignored)
  local err=0

  local files
  files=$(git diff --ignore-blank-lines --ignore-space-change --diff-filter=M --stat "$reference_branch"..${validation_branch:+"$validation_branch"} -- nonreg/nonregTests | sed '$d' | awk '{print $1}' )

  if [ -n "$files" ]
  then
    echo "Warning: Modified test cases ↵"

    INPUT_DIFFS=$(echo "$files" | xargs -n 1 dirname | sort | uniq)

    while IFS= read -r line; do
      echo "         $line"
    done <<< "$INPUT_DIFFS"
  fi

  # For now we still want to check the diffs in the output results of
  # modified tests. Uncomment the next line to change this behaviour.
  #exclude_from_diff "$INPUT_DIFFS" 0

  # Second: check for added/removed test cases in the non regression test list
  local diff_args="--ignore-blank-lines --ignore-space-change $reference_branch..$validation_branch -- $test_list"

  # The 'git diff --numstat' command allow to find easily stat infos
  # on number of lines changed between commits but it counts blank
  # lines added just before a new line (it is view as a unique
  # modification).  Thus we cannot directly use its output to give
  # informations on added and deleted lines.
  #
  # Filtering the git diff output works as soon as 'git diff' is not
  # empty.  In consequence, we check for empty diff with 'git diff
  # --numstat', then we treat non empty diffs with combinations of
  # diff and grep commands.
  local has_diff
  has_diff=$(eval git diff --numstat "$diff_args" | wc -l)

  if [[ "$has_diff" -eq "0" ]]
  then
    return "$err"

  elif [[ "$has_diff" -eq "1" ]]
  then
    echo "Warning: $test_list file contains modifications."

    diff_args="-U0 $diff_args"

    local added
    added="$(count_line_diffs '+')"
    if [[ "$added" -gt "0"  ]]
    then
      echo "         Added   $added line(s) of test case(s)."
      ADDED_LIST="$(store_diff_list '+')"
      exclude_from_diff "$ADDED_LIST" 1

    fi

    local removed
    removed="$(count_line_diffs '-')"
    if [[ "$removed" -gt "0"  ]]
    then
      echo "         Removed $removed line(s) of test case(s)."
      echo "         => pipeline marked as failed!"
      REMOVED_LIST="$(store_diff_list '-')"
      exclude_from_diff "$REMOVED_LIST" 1
      err=1
    fi

    return "$err"

  else
    echo "count_diff_lines: Unexpected value for 'has_diff'. Check implementation."
    return 2
  fi

}

# -------------------------------------------------------------------------
#
# Formatted print of the branch name passed as argument
#
# -------------------------------------------------------------------------
print_message() {
  mess="$1"

  echo ""
  printf '%*s\n' ${#mess} | tr ' ' '*'
  echo "$mess"
  printf '%*s\n' ${#mess} | tr ' ' '*'
}

# -------------------------------------------------------------------------
#
# Formatted print of the branch name passed as argument of the non
# regression message
#
# -------------------------------------------------------------------------
print_nonreg_message() {

  print_message "------- RUNNING $1 BRANCH NONREG TESTS -------"

}


# -------------------------------------------------------------------------
#
# Clean worktree at exit
#
# -------------------------------------------------------------------------
function cleanup {
  echo ""
  echo "Exit. Clean temporary worktree."
  git worktree remove -f ecogen_reference 2>/dev/null
  git branch -D reference 2>/dev/null
}

# -------------------------------------------------------------------------
#
# Check differences between reference and validation branch
#
# Moves:
#   results_save -> results folder (if exist)
#
# Uses global variables:
#   TEST_OUTPUTS - output filename for `run.sh` script (terminal test outputs)
#   REPORT_DIR - name of folder for report storage
#   EXCLUDE_LIST - diff exclusion arguments
#
# Deletes:
#   nonreg/results_reference - reference results from a previous non regression run
#   nonreg/results_validation - validation results from a previous non regression run
#
# Returns: 0 for success, 1 for failure
#
# Example:
#   check_and_clean
#
# -------------------------------------------------------------------------
check_and_clean() {

  echo ""
  echo "************************************************************************"
  echo "--------   FINISHING NONREG TESTS (CLEANING/COMPUTING DIFF)  -----------"
  echo "************************************************************************"
  echo "Cleaning..."

  cd "$MAIN_WRKTREE_DIR" ||
    (echo "Unable to restore results directory and compute diffs" && return 1 )

  if [ -d results_save ]
  then
    mv -v results_save results
  fi

  echo "-----------------------------------------"
  echo "Computing differences between reference and validation branches..."

  echo "Exclude list: $EXCLUDE_LIST"

  cmd="diff --ignore-blank-lines --ignore-space-change "$EXCLUDE_LIST" -x "$TEST_OUTPUTS" -qr -x 'infoCalcul.out' -x '.DS_Store' "$REPORT_DIR"/results_reference "$REPORT_DIR"/results_validation > $REPORT_DIR/diff_nonreg.out 2>&1 || true"

  # Here the run_cmd script fails for normal mode due to the combination of output redirection and
  # protected arguments passed to 'eval' ('\$*')
  if [ "$DRY_RUN" = "true" ]; then
    echo "dry-run: $cmd"
  else
    eval "$cmd"
  fi

  if [ -s "$REPORT_DIR"/diff_nonreg.out ]
  then
    echo "Differences exist: details available in $REPORT_DIR/diff_nonreg.out file."
    printf '\n%s\n' "-- Step status: Failed"
    error=1
  else
    echo "Non-regression tests complete: tests common to both branches present no difference!"
    printf '\n%s\n' "-- Step status: Succeed"
  fi
}

# -------------------------------------------------------------------------
#
# Dry-run mode: echo commands in dry-run mode,
# run them otherwise
#
# -------------------------------------------------------------------------
run_cmd() {
  if [ "$DRY_RUN" = "true" ]; then
    echo "dry-run: $*"
  else
    # $* is escaped to avoid special char evaluation before command execution which may lead to missing argument
    # when the name of evaluation branch starts by # for example (# is evaluated as a comment). See PR #119.
    eval "\$*"
  fi
}

# -------------------------------------------------------------------------
#
# Print help for main program.
#
# -------------------------------------------------------------------------
usage() {
  echo ""
  echo "Usage: $(basename "$0") [-d] [-r reference_branch] [-l test_list]"
  echo ""
  echo "    Build ecogen, run non-regression tests and validate results"\
       " against a reference branch."
  echo ""
  echo "        -d dry-run mode."
  echo "        -r <reference_branch> branch to use as reference branch."\
       " Defaults to 'origin/devel'."
  echo "        -l <test_lists> path and name of file with test list."\
       " Defaults to './nonreg/ECOGEN_nonReg_full.list'."

  echo ""
}

# -------------------------------------------------------------------------
#
# Main non regression script when called from command line.
#
# Arguments:
#   [-r <reference_branch>] - name of branch used as reference
#   [-l <test_list>] - file containing the list of tests to run
#
# -------------------------------------------------------------------------
main() {
  trap cleanup EXIT # Execute cleanup function at exit command

  error=0
  DRY_RUN=false

  # Parse command line options
  while getopts ':hdr:l:' OPTION; do
    case "$OPTION" in
      l)
        test_list_file=$OPTARG
        ;;
      r)
        reference_branch_name=$OPTARG
        ;;
      d)
        DRY_RUN=true
        ;;
      h)
        usage
        exit 0
        ;;
      \?)
        echo ""
        echo "Error: Illegal option -$OPTARG"
        usage
        exit 1
        ;;
      :)
        echo ""
        echo "Error: Option -$OPTARG requires an argument"
        usage
        ;;
    esac
  done

  # Assign default values if parameters are unset
  test_list_file=${test_list_file:="./nonreg/ECOGEN_nonReg_full.list"}
  reference_branch_name=${reference_branch_name:=origin/devel}

  validation_branch_name="$(git rev-parse --abbrev-ref HEAD)"

  # Parse command line options and configure non regression tests.
  # Defines global variables for output dirs and files and creates
  # report directories
  if ! configure_nonreg "$test_list_file" "$reference_branch_name" "$validation_branch_name"
  then
    # Either the function is called with wrong arguments, or old
    # 'results' directory cannot be moved in 'results_save'. Quit the
    # process to avoid loting results
    echo "Error when configuring non-regression checks."
    exit 1
  fi


  print_message "------- EVALUATING TEST CASES DIFFERENCES -------"
  # Check if non regression tests has been modified. Here
  # we don't want to pass the name of the validation branch because it
  # will ignore possible uncommited modifications.
  if ! eval_diff "$test_list_file" "$reference_branch_name" ""
  then
    if [ "$?" -eq "2" ]
    then
      echo "Error when checking modification in non regression test list."
      exit 1
    else
      # $? == 1 => Error: test deletion in the test list!
      error=1
      printf '\n%s\n' "-- Step status: Failed"
    fi
  else
    printf '\n%s\n' "-- Step status: Succeed"
  fi

  # Build and test the branch to validate
  print_nonreg_message "$validation_branch_name"
  if run_cmd build_branch "output_compile.out" "$validation_branch_name"
  then
    run_cmd test_branch "$test_list_file" "results_validation" "$validation_branch_name"
  fi

  # Clone, build and test the reference branch
  print_nonreg_message "$reference_branch_name"

  echo "Fetching and checking out $reference_branch_name branch..."

  if ! git fetch > "$REPORT_DIR"/output_fetchAndCheckout.out
  then
    echo "Warning: unable to fetch the remote repository."
    echo "Attempt to run using last commit locally available on $reference_branch_name."
  fi

  count=$(git worktree list | \grep  -c ecogen_reference)
  if [ "$count" ]
  then
    # unclean worktree: delete the old tree
    git worktree remove -f ecogen_reference >> "$REPORT_DIR"/output_fetchAndCheckout.out 2>&1
    git branch -D reference >> "$REPORT_DIR"/output_fetchAndCheckout.out 2>&1
  fi

  # Checkout the reference branch in a new tree
  if ! git worktree add ../ecogen_reference -B reference "$reference_branch_name" >> "$REPORT_DIR"/output_fetchAndCheckout.out
  then
    echo "Error during checkout on $reference_branch_name. Running on $reference_branch_name abort."
    error=1
  else
    cd ../ecogen_reference ||
      (echo "Unable to move in 'ecogen_reference' directory. Exit." && exit 1)
    if run_cmd build_branch "output_compile_ref.out" "$reference_branch_name"
    then
      run_cmd test_branch "$test_list_file" "results_reference" "$reference_branch_name"
    fi
  fi

  cd "$MAIN_WRKTREE_DIR" ||
    (echo "Unable to move in '$MAIN_WRKTREE_DIR' directory. Next step may fail.")

  # Check test differences and final cleaning
  if ! check_and_clean
  then
    error=1
  fi

  # Sending error code if something went wrong
  exit "$error"
}

# -------------------------------------------------------------------------
#
# Definition of two usages for current script file:
#
#     - the file can be sourced to load the defined functions and allow
#       individual function call
#
#     - the file can be executed as a script to run the non regression
#       pipeline
#
# Examples:
#   source "./nonreg/nonreg.sh"
#   source "./nonreg/nonreg.sh" && usage
#   ./nonreg/nonreg.sh
#   ./nonreg/nonreg.sh -h
#   ./nonreg/nonreg.sh -r branch_toto -l ./libTests/tests.list
#
# -------------------------------------------------------------------------

# Do not call the main function when we source the .sh file
if [ "$0" = "${BASH_SOURCE[0]}" ]
then
  # Script call from command line: execute the main func
  main "$@"
fi
