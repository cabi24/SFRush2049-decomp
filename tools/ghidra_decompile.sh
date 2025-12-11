#!/bin/bash
# ghidra_decompile.sh - Run Ghidra headless decompilation
#
# Usage:
#   ./tools/ghidra_decompile.sh setup              # First-time setup (import ROM)
#   ./tools/ghidra_decompile.sh all                # Decompile all functions
#   ./tools/ghidra_decompile.sh func 0x800CA3B4    # Decompile single function
#   ./tools/ghidra_decompile.sh analyze            # Re-analyze (if needed)

set -e

# Configuration - EDIT THESE FOR YOUR SYSTEM
GHIDRA_HOME="${GHIDRA_HOME:-$HOME/ghidra}"
PROJECT_DIR="${PROJECT_DIR:-/tmp/ghidra_projects}"
PROJECT_NAME="rush2049"
ROM_PATH="$(dirname "$0")/../baserom.us.z64"
SCRIPT_DIR="$(dirname "$0")/ghidra_scripts"
OUTPUT_DIR="$(dirname "$0")/../ghidra_output"

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Check Ghidra installation
if [ ! -d "$GHIDRA_HOME" ]; then
    echo "ERROR: Ghidra not found at $GHIDRA_HOME"
    echo ""
    echo "Install Ghidra:"
    echo "  wget https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_11.2.1_build/ghidra_11.2.1_PUBLIC_20241105.zip"
    echo "  unzip ghidra_*.zip"
    echo "  export GHIDRA_HOME=\$PWD/ghidra_11.2.1_PUBLIC"
    exit 1
fi

ANALYZE="$GHIDRA_HOME/support/analyzeHeadless"

if [ ! -x "$ANALYZE" ]; then
    echo "ERROR: analyzeHeadless not found at $ANALYZE"
    exit 1
fi

case "$1" in
    setup)
        echo "=== Setting up Ghidra project ==="
        echo "ROM: $ROM_PATH"
        echo "Project: $PROJECT_DIR/$PROJECT_NAME"
        echo ""

        if [ ! -f "$ROM_PATH" ]; then
            echo "ERROR: ROM not found at $ROM_PATH"
            exit 1
        fi

        mkdir -p "$PROJECT_DIR"

        # Import ROM and analyze
        "$ANALYZE" "$PROJECT_DIR" "$PROJECT_NAME" \
            -import "$ROM_PATH" \
            -processor "MIPS:BE:32:default" \
            -cspec "default" \
            -loader N64LoaderFactory \
            -scriptPath "$SCRIPT_DIR" \
            2>&1 | tee "$OUTPUT_DIR/setup.log"

        echo ""
        echo "=== Setup complete ==="
        echo "Project created at: $PROJECT_DIR/$PROJECT_NAME"
        ;;

    all)
        echo "=== Decompiling all functions ==="
        echo "Output: $OUTPUT_DIR/all_functions.c"
        echo ""

        "$ANALYZE" "$PROJECT_DIR" "$PROJECT_NAME" \
            -process \
            -noanalysis \
            -scriptPath "$SCRIPT_DIR" \
            -postScript DecompileAllFunctions.java \
            > "$OUTPUT_DIR/all_functions.c" \
            2> "$OUTPUT_DIR/all_functions.log"

        echo ""
        echo "=== Done ==="
        echo "Output: $OUTPUT_DIR/all_functions.c"
        echo "Log: $OUTPUT_DIR/all_functions.log"
        wc -l "$OUTPUT_DIR/all_functions.c"
        ;;

    func)
        if [ -z "$2" ]; then
            echo "Usage: $0 func 0xADDRESS"
            exit 1
        fi
        ADDR="$2"
        echo "=== Decompiling function at $ADDR ==="

        "$ANALYZE" "$PROJECT_DIR" "$PROJECT_NAME" \
            -process \
            -noanalysis \
            -scriptPath "$SCRIPT_DIR" \
            -postScript DecompileFunction.java "$ADDR" \
            2>&1 | grep -v "^INFO\|^WARN" | tee "$OUTPUT_DIR/func_${ADDR}.c"
        ;;

    analyze)
        echo "=== Re-analyzing project ==="

        "$ANALYZE" "$PROJECT_DIR" "$PROJECT_NAME" \
            -process \
            -scriptPath "$SCRIPT_DIR" \
            2>&1 | tee "$OUTPUT_DIR/analyze.log"
        ;;

    *)
        echo "Ghidra Headless Decompilation Tool"
        echo ""
        echo "Usage:"
        echo "  $0 setup              First-time setup (import ROM, analyze)"
        echo "  $0 all                Decompile all functions to C"
        echo "  $0 func 0xADDRESS     Decompile single function"
        echo "  $0 analyze            Re-analyze project"
        echo ""
        echo "Environment variables:"
        echo "  GHIDRA_HOME     Path to Ghidra installation (default: ~/ghidra)"
        echo "  PROJECT_DIR     Path to store Ghidra projects (default: /tmp/ghidra_projects)"
        echo ""
        echo "Output directory: $OUTPUT_DIR"
        ;;
esac
