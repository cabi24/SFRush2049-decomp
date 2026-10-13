#!/bin/sh
# w28s (Pi, repo root): compare every object prove.sh built (copied to cloud/work/frontier/w28s/ceiling/obj/,
# git-ignored) with the retail ROM at the address in vram.txt (w24t compare.py: whole .text, relocations by value).
C=cloud/work/frontier/w28s/ceiling
while read n v; do
  printf '%-20s %s  ' "$n" "$v"
  python3 cloud/work/frontier/w24t/rename/compare.py $C/obj/$n.o $v | grep RESULT
done < $C/vram.txt
