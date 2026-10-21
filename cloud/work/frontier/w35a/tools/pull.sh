#!/bin/bash
# pull.sh <cand dir name>  -> out/<name>.out
cd "$(dirname "$0")/.."
rm -rf out/$1.out; mkdir -p out
scp -q -r watchman2:rush2049/scratch/frontier/w35a/cand/$1.out out/
