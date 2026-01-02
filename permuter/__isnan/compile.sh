#!/bin/bash
# Compile using IDO 5.3 on watchman
# Permuter calls: ./compile.sh input.c -o output.o

INPUT=""
OUTPUT=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -o)
            OUTPUT="$2"
            shift 2
            ;;
        *)
            INPUT="$1"
            shift
            ;;
    esac
done

PROJ="/home/cburnes/projects/rush2049-decomp"
IDO="$PROJ/tools/ido-static-recomp/build/out/cc"
FLAGS="-O2 -mips2 -g0 -G 0 -non_shared"

# Copy source to watchman, compile, copy object back
scp -q "$INPUT" watchman:/tmp/permuter_in.c
ssh -q watchman "$IDO $FLAGS -c -o /tmp/permuter_out.o /tmp/permuter_in.c" 2>/dev/null
scp -q watchman:/tmp/permuter_out.o "$OUTPUT"
