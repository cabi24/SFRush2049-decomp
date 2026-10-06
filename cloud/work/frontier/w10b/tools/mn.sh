#!/bin/sh
# usage: mn.sh file.c NAME [keep] -- mnemonic-only aligned diff (structure lens), -O3 (group mode when keep given)
f="$1"; n="$2"; k="$3"
if [ -n "$k" ]; then ./full.sh "$f" "$n" --flags "\"-g0 -O3 -mips2 -G 0 -non_shared\"" --mn --keep "$k"; else ./full.sh "$f" "$n" --flags "\"-g0 -O3 -mips2 -G 0 -non_shared\"" --mn; fi
