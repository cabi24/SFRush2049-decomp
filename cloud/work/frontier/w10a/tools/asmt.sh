#!/bin/sh
# usage: asmt.sh file.s NAME [--all|--mn] -- assemble hand-edited ugen listing on the builder, aligned diff vs retail
scp -q "$1" watchman2:rush2049/scratch/frontier/w10a/cand/_a.s && ssh watchman2 "cd ~/rush2049/scratch/frontier/w10a/cand && sh asm_remote.sh _a.s $2 $3"
