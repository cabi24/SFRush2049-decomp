#!/bin/sh
# usage: aq.sh file.c NAME  -- score at -O3 and -O2
./sc.sh "$1" "$2" --flags "'-g0 -O3 -mips2 -G 0 -non_shared'" 2>&1 | head -${3:-40}
./sc.sh "$1" "$2" --flags "'-g0 -O2 -mips2 -G 0 -non_shared'" 2>&1 | head -3
