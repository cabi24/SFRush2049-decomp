#!/bin/sh
# usage: am.sh CAND.c [dump] -- score audio_mixer_main (+ internal audio_priority_find) in the unit (tag w9g)
c=$(realpath "$1"); cd /home/cburnes/projects/rush2049-decomp

python3 -m tools.conveyor.pipeline.blob_unit --tag w9g score audio_mixer_main audio_priority_find --with "$c" --internal audio_priority_find --keep audio_mixer_main 2>&1 | grep -E "EQUAL|FAIL|error|Error" | head -3
if [ -n "$2" ]; then
  mips-linux-gnu-objdump -d --no-show-raw-insn build/blob_unit/w9g/unit.o | awk '/<audio_mixer_main>:/{p=1} p&&/^$/{exit} p' | sed 's/^ *[0-9a-f]*:\t//' 
fi
