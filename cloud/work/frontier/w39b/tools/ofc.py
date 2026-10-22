#!/usr/bin/env python3
"""Builder-side (from the scratch root): ofc.py SRC.c OUT.o [flags] -- compile one file like score.py fn."""
import sys
sys.path.insert(0, 'tools/cloud')
import score
flags = sys.argv[3] if len(sys.argv) > 3 else '-g0 -O3 -mips2 -G 0 -non_shared'
score.compile_single(sys.argv[1], flags, sys.argv[2])
