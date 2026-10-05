#!/bin/sh
# Wave 3: re-prove the PR #82/#83 adapted source paths at true score 0 (pool
# `lock add`, flags from the source's line 1) and retire every superseded
# non-ROM lock entry for the same function, so each function keeps exactly one
# candidate lock. Usage: relock_wave3.sh fn=path [fn=path ...]
cd "$(git rev-parse --show-toplevel)" || exit 1
out=cloud/work/boot_tail_promotion/relock_wave3_results.tsv
for arg in "$@"; do
  fn=${arg%%=*}; src=${arg#*=}
  flags=$(sed -n '1s|^/\* flags: \(.*\) \*/$|\1|p' "$src")
  res=$(python3 -m tools.conveyor.pipeline.lock add "$src:$fn" --flags "$flags" 2>&1 | tail -1)
  case "$res" in
    locked*)
      python3 - "$src:$fn" "$fn" <<'PY'
import sys
from tools.conveyor.pipeline import lock
keep, fn = sys.argv[1], sys.argv[2]
e = lock.load_lock()
for k in [k for k in e if k.endswith(":" + fn) and k != keep and not k.startswith("src/")]:
    del e[k]
    print("retired", k)
lock.save_lock(e)
PY
      ;;
  esac
  printf '%s\t%s\t%s\t%s\n' "$fn" "$src" "$flags" "$res" >> "$out"
done
