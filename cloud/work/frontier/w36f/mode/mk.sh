#!/bin/bash
# mk.sh SRC.c OUTDIR
S=$1; O=$2; mkdir -p $O; cp $S $O/eligibility.c; cp g0/renderer.c $O/; cp g0/group.json $O/
