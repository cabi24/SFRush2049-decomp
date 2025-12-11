#!/bin/bash
# batch_m2c.sh - Run mips_to_c on all assembly files
#
# Usage:
#   ./tools/batch_m2c.sh              # Decompile all .s files
#   ./tools/batch_m2c.sh 7600.s       # Decompile specific file
#   ./tools/batch_m2c.sh --parallel   # Run in parallel (faster on multi-core)
#
# Designed to run on the OptiPlex with 20 cores for fast batch processing

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
ASM_DIR="$PROJECT_DIR/asm/us"
OUTPUT_DIR="$PROJECT_DIR/m2c_output"
M2C="$SCRIPT_DIR/mips_to_c/m2c.py"
VENV="$SCRIPT_DIR/m2c_venv/bin/activate"

# Check if venv exists
if [ -f "$VENV" ]; then
    source "$VENV"
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Check m2c exists
if [ ! -f "$M2C" ]; then
    echo "ERROR: m2c not found at $M2C"
    echo "Run: cd $SCRIPT_DIR && git clone https://github.com/matt-kempster/mips_to_c.git mips_to_c"
    exit 1
fi

# Process a single file
decompile_file() {
    local asm_file="$1"
    local base_name=$(basename "$asm_file" .s)
    local output_file="$OUTPUT_DIR/${base_name}.c"

    echo "Decompiling $asm_file -> $output_file"

    # Run m2c (without context - types.h has preprocessor directives m2c can't handle)
    python3 "$M2C" "$asm_file" > "$output_file" 2>/dev/null || true

    # Report result
    if [ -s "$output_file" ]; then
        local funcs=$(grep -c "^void\|^s32\|^u32\|^f32\|^s16\|^u16\|^s8\|^u8\|^extern\|^\?" "$output_file" 2>/dev/null || echo 0)
        echo "  -> $funcs function(s)"
    else
        echo "  -> (empty or failed)"
        rm -f "$output_file"
    fi
}

# Export for parallel execution
export -f decompile_file
export PROJECT_DIR OUTPUT_DIR M2C

# Handle arguments
case "$1" in
    --parallel|-p)
        echo "=== Batch m2c decompilation (parallel mode) ==="
        echo "Output directory: $OUTPUT_DIR"
        echo ""

        # Count files
        total=$(ls -1 "$ASM_DIR"/*.s 2>/dev/null | wc -l)
        echo "Processing $total assembly files with $(nproc) cores..."
        echo ""

        # Use GNU parallel if available, otherwise xargs
        if command -v parallel &> /dev/null; then
            ls "$ASM_DIR"/*.s | parallel -j $(nproc) decompile_file {}
        else
            ls "$ASM_DIR"/*.s | xargs -n1 -P $(nproc) -I {} bash -c 'decompile_file "$@"' _ {}
        fi
        ;;

    "")
        echo "=== Batch m2c decompilation ==="
        echo "Output directory: $OUTPUT_DIR"
        echo ""

        total=$(ls -1 "$ASM_DIR"/*.s 2>/dev/null | wc -l)
        count=0

        for asm_file in "$ASM_DIR"/*.s; do
            if [ -f "$asm_file" ]; then
                count=$((count + 1))
                echo "[$count/$total] $asm_file"
                decompile_file "$asm_file"
            fi
        done
        ;;

    *)
        # Specific file
        if [ -f "$ASM_DIR/$1" ]; then
            decompile_file "$ASM_DIR/$1"
        elif [ -f "$1" ]; then
            decompile_file "$1"
        else
            echo "File not found: $1"
            exit 1
        fi
        ;;
esac

# Summary
echo ""
echo "=== Summary ==="
output_count=$(ls -1 "$OUTPUT_DIR"/*.c 2>/dev/null | wc -l)
total_lines=$(wc -l "$OUTPUT_DIR"/*.c 2>/dev/null | tail -1 | awk '{print $1}')
echo "Generated: $output_count C files"
echo "Total lines: $total_lines"
echo "Output: $OUTPUT_DIR/"
