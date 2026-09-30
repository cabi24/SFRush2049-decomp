#!/bin/bash
# var.sh GROUPDIR SRC.c : score SRC.c as group.c of GROUPDIR in a temp dir
T=$(mktemp -d); cp $1/group.json $T/; cp $2 $T/group.c
cd /home/user/SFRush2049-decomp; python3 tools/cloud/score.py group $T 2>&1 | grep -v "^    +"
rm -rf $T
