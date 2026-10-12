#!/bin/sh
# mk.sh BODY.c OUT.c : prepend the controller_poll group prelude (current tree, up to its draft) to a body-only candidate
R=$(cd "$(dirname "$0")/../../../../.." && pwd)
G="$R/src/blob/groups/controller_poll/group.c"
n=$(grep -n '^/\* read the pads' "$G" | head -1 | cut -d: -f1)
head -$((n-1)) "$G" | sed 's/^s8 D_80149B64\[4\];/extern s8 D_80149B64[4];/; s/^s8 D_80149B74\[4\];/extern s8 D_80149B74[4];/' > "$2"
cat "$1" >> "$2"
