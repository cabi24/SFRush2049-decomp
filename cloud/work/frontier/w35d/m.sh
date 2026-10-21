#!/bin/bash
# usage: m.sh groups/DIR FUNC  -> raw differing words (strict) and number of structural hunks (register-normalised)
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35d || exit 1
raw=$(./cnt.sh "$1" | tr ' ' '\n' | grep "^$2:" | head -1)
h=$(SBS_ARGS=--norm ./sbs.sh group "$1" "$2" 2>/dev/null | grep -c '^---')
echo "$1 $raw hunks=$h"
