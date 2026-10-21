#!/bin/bash
# usage: g.sh NAME ROOT.c  -- build groups/NAME from ROOT.c and print summary of gd diff for func_803AD57C (and keep full diff in $S/NAME.txt)
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/4b393cd0-4116-4767-90e0-4ddf67d9ff98/scratchpad
D=$(dirname $0)/..
python3 $D/tools/mkgrp.py $1 $2 && $D/tools/gd.sh group a $D/groups/$1 func_803AD57C > $S/$1.txt 2>&1; tail -1 $S/$1.txt
