#!/bin/bash
# ar.sh FILE.c FUNC : area of FUNC in a single-file toy
cd "$(dirname "$0")/.."
export TAG=w35a SCR=rush2049/scratch/frontier/w35a
T=../tools/trace
L=ar$$
$T/snap.sh $L $1 $2 >/dev/null 2>&1 || { echo "stage failed"; exit 1; }
$T/spill.sh $L $2 2>&1 | grep "^AREA f_readnxt"
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w35a/st_$L"
