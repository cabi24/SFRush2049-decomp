#!/bin/sh
# score audio_mixer_main variants in the unit (tag w6b): ammv.sh FILE.c...
for f in "$@"; do
  p=$(realpath "$f")
  r=$(cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w6b score audio_mixer_main audio_priority_find --internal audio_priority_find --keep audio_mixer_main --with "$p" 2>&1 | grep -E "audio_mixer_main:|rror" | head -1)
  echo "$(basename $f): $r"
done
