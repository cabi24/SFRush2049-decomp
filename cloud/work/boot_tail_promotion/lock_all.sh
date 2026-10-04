#!/bin/sh
# Re-prove every cloud/matches/boot_tail body at true score 0 against the
# splat-derived reloc-aware static targets (pool compile_score via `lock add`),
# using the exact flags on the body's line 1. Results: lock_results.tsv.
cd "$(git rev-parse --show-toplevel)" || exit 1
out=cloud/work/boot_tail_promotion/lock_results.tsv
: > "$out"
for f in cloud/matches/boot_tail/func_*.c; do
  fn=$(basename "$f" .c)
  flags=$(sed -n '1s|^/\* flags: \(.*\) \*/$|\1|p' "$f")
  res=$(python3 -m tools.conveyor.pipeline.lock add "$f:$fn" --flags "$flags" 2>&1 | tail -1)
  printf '%s\t%s\t%s\n' "$fn" "$flags" "$res" >> "$out"
done
