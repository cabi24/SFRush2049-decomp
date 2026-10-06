#!/bin/sh
# usage: gfd.sh GROUPDIR NAME -- aligned diff of NAME from a single-file group dir (group.c + group.json keep)
k=$(python3 -c "import json,sys;print(','.join(json.load(open('$1/group.json'))['keep']))")
"$(dirname $0)/fd.sh" "$1/group.c" "$2" --keep $k
