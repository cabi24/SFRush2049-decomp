#!/bin/bash
# Compile and test this function
# Run from this directory or pass the directory as argument

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

# Compiler settings (verified for this category)
CC="tools/ido-static-recomp/build/out/cc"
FLAGS="-g0 -O1 -mips2 -G 0 -non_shared"

# Check if we're on the build machine
if [ ! -f "${DIR}/../../../../$CC" ]; then
    echo "ERROR: Must run on watchman (x86 build machine)"
    echo "Run: ssh watchman"
    echo "Then: cd /home/cburnes/projects/rush2049-decomp && $0"
    exit 1
fi

cd "${DIR}/../../../.."

# Compile
echo "Compiling base.c..."
$CC $FLAGS -c -o /tmp/func_test.o "$DIR/base.c" 2>&1

if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

# Disassemble
echo ""
echo "=== COMPILED OUTPUT ==="
mips-linux-gnu-objdump -d /tmp/func_test.o

echo ""
echo "=== TARGET ASSEMBLY ==="
cat "$DIR/target.s"

echo ""
echo "Use tools/asmdiff.py for side-by-side comparison:"
echo "  python3 tools/asmdiff.py $DIR"
