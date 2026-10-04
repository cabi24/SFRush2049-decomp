#!/bin/bash
# usage: var.sh <basegroup> <varname> <python-edit-script-file>
B=$1; V=$2; P=$3
rm -rf vars/$V; mkdir -p vars; cp -r $B vars/$V
python3 $P vars/$V/group.c
./run.sh vars/$V 2>&1 | grep -m1 "words differ\|MATCH" | sed "s/^/$V: /"
