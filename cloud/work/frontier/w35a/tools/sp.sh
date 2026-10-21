#!/bin/bash
# sp.sh <ours.txt> : summary of stack offsets
grep -o "[-0-9]*(sp)\|addiu sp,sp,-[0-9]*\|addiu [a-z0-9]*,sp,[0-9]*" $1 | sort -t'(' -k1 -n | uniq -c | tr '\n' ';'; echo
