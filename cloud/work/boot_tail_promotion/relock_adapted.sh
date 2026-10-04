#!/bin/sh
# Re-prove adapted sources (true score 0 via pool `lock add`) and retire the
# superseded cloud/matches lock entry for each. Usage: relock_adapted.sh fn...
cd "$(git rev-parse --show-toplevel)" || exit 1
out=cloud/work/boot_tail_promotion/relock_results.tsv
for fn in "$@"; do
  src=cloud/work/boot_tail_promotion/sources/$fn.c
  flags=$(sed -n '1s|^/\* flags: \(.*\) \*/$|\1|p' "$src")
  res=$(python3 -m tools.conveyor.pipeline.lock add "$src:$fn" --flags "$flags" 2>&1 | tail -1)
  case "$res" in
    locked*) python3 -m tools.conveyor.pipeline.lock remove "cloud/matches/boot_tail/$fn.c:$fn" >/dev/null 2>&1 ;;
  esac
  printf '%s\t%s\t%s\n' "$fn" "$flags" "$res" >> "$out"
done
