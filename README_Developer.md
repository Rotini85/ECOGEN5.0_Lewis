Developer information - ECOGEN
===

Please be the most consistent possible with the current content of the code. This includes, at least, rules of development regarding variable and class names, standard Doxygen comments and developer personal comments (flags), which are detailed here.

## Code rules

### Code style guidelines

In order to minimize unnecessary conflicts during merge requests, please configure your text editor to use the following code style :
  - no tabulations
  - 2 spaces indentations
  - no trailing whitespaces
  - left alignment of pointers and addresses (`int* i; int& k;`)
  - the last line of a file has to be ended by a line feed character.

More generally, the project uses the `LLVM` style of `clang-format` with `Stroustrup` option for break insertion before braces. Short statements on one line are authorized only when the statement is not wrapped between braces. For example, a `if/else` block will take the following style:
```cpp
if (a) {
  baz(2);
}
else {
  baz(5);
}
// Same test
if (a) baz(2);
else baz(5);
```

***Remark:***  
*Some parts of the code do not respect these conventions: try to resist the temptation to change the formatting of these parts of code (it may create conflicts for the other developers). If you really wish to do so, please dedicate a commit and a merge request to the code formatting.*


### Variable and Class names:

1. Variable name should began with lowercase letter and be self-understandable. Each new word began with a uppercase letter.
\
**Examples**:
   ```cpp
     int myInteger;
     vector<double*> vectorOfDoublePointer;
     Etc.
    ```

2. Class attribute should began by `m_`.
\
**Examples**:
   ```cpp
   int m_myInteger;
   m_double* m_doublePointer;
   etc.
   ```

3. Class name should began with an uppercase Letter.
\
**Examples**: `class MyClass;`

### Standard Doxygen comments
To automate code documentation, comments should be inserted as follow in header files.

1. Before class definition:
   ```
   //! \class     class name
   //! \brief     brief description
   //! \details   Detailed description
   //!                  continue description
   ```

2.  Before function and method prototypes:
    ```
    //! \brief      brief description
    //! \details    Detailed description
    //!                  continue description
    //! \param      parameter name         description
    //! \param      parameter name         description
    //! \return     return description
    ```

3. Class member description:
   ```cpp
   type m_variable; //!< member description
   ```

### Developer personal comments - flags

Standard flags can be used to insert personal comments inside the code.
```
	//Developer//KeyWord// comments
```
**Example**: `"//FP//DEV// comment, description"`

**Keyword list**

| Keyword      | Meaning                       |
| ---          | ---                           |
| `//DEV//`    | in development                |
| `//Q//`      | question to dig               |
| `//TODO//`   | should be done in the future  |
| `//ERR//`    | error: to correct ASAP        |
| `//ID//`     | idea                          |
| `//HERE//`   | Stop development position     |
| `//VERIF//`  | to verify: is it needed?      |
| `//TEST//`   | test: To delete ASAP          |

## Commit rules
  - For each modification, a comment should be prepared to be included to the commit message for Git.
  - All commit messages must use the following syntax:
    ```
    XX_branchName: My commit message

    Detailed description of the commit.
    ```
