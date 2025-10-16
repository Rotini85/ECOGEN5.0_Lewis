#!/bin/bash
# run_rt.sh — Run Rayleigh–Taylor ECOGEN case from rundirectory

# --- Config ---
RUNDIR="./rundirectory/RT/"
ECOGEN_EXEC="ECOGEN"
NP=14  # Number of MPI processes

# 1. Extract run name from main.xml
RUNNAME=$(grep -oP '(?<=<run>).*?(?=</run>)' "$RUNDIR/libTests/main.xml")

if [ -z "$RUNNAME" ]; then
    echo "Error: Could not extract <run> name from $RUNDIR/libTests/main.xml"
    exit 1
fi

# --- Timestamp (YYYY-MM-DD_HH-MM-SS) ---
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")

echo "=== Starting ECOGEN simulation: $RUNNAME ==="
echo "=== Timestamp: $TIMESTAMP ==="

# --- Directories ---
RESULTS_BASE="/home/sdcfd/ECOGEN-5_0/results"
RESULTS_DIR="$RESULTS_BASE/$RUNNAME"
EXTERNAL_BASE="/home/sdcfd/SimulationLogs/RT"
EXTERNAL_DIR="$EXTERNAL_BASE/${RUNNAME}_$TIMESTAMP"

# Create necessary folders
mkdir -p "$RESULTS_DIR"
mkdir -p "$EXTERNAL_DIR"

# --- Copy geometry source files immediately ---
cp src/Geometries/GDEntireDomainWithParticularities.* "$EXTERNAL_DIR/"
echo "=== Source files copied to $EXTERNAL_DIR ==="

# --- Define cleanup function (runs on finish, crash, or Ctrl+C) ---
cleanup() {
    echo "=== Copying results to SimulationLogs (cleanup triggered) ==="
    if [ -d "$RESULTS_DIR" ]; then
        mkdir -p "$EXTERNAL_DIR"
        cp -r "$RESULTS_DIR/"* "$EXTERNAL_DIR/" 2>/dev/null
        echo "=== Results successfully copied to $EXTERNAL_DIR ==="
    else
        echo "Warning: Results directory not found ($RESULTS_DIR)"
    fi
}
trap cleanup EXIT INT TERM

# --- Run ECOGEN in background ---
nohup mpirun -np $NP --use-hwthread-cpus "$ECOGEN_EXEC" > "$RESULTS_DIR/output.log" 2>&1 &
SIM_PID=$!

echo "Simulation started in background (PID=$SIM_PID). Output is in $RESULTS_DIR/output.log"
echo "Press Ctrl+C to stop watching (simulation keeps running)."

# --- Follow live output ---
tail -f "$RESULTS_DIR/output.log"
