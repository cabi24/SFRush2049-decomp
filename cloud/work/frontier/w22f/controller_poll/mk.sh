#!/bin/sh
# mk.sh BODY.c OUT.c : prepend the controller_poll group prelude (current tree) to a body-only candidate
R=$(cd "$(dirname "$0")/../../../../.." && pwd)
head -3720 "$R/src/blob/groups/controller_poll/group.c" | sed 's/^s8 D_80149B64\[4\];/extern s8 D_80149B64[4];/; s/^s8 D_80149B74\[4\];/extern s8 D_80149B74[4];/' > "$2"
cat "$1" >> "$2"
