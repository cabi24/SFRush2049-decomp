# Reference scripts (the coordinating Claude session's working scripts, unchanged)

Not part of the pipeline: copy, read, and turn the useful ones into tested tools under `tools/` if you keep using them.
Run from the repo root with `PYTHONPATH=.`.

- `splice_singles.py [NAME ...]`: splice the unlocked single-function files in `cloud/matches/` (all of them, or only the
  names given) one at a time behind the image gate; flags come from line 1 of each file; already-locked functions are skipped
  (an earlier version re-spliced locked ones and overwrote lock entries: never remove that skip).
- `splice_claims.py GROUP ...`: promote a cloud group (`cloud/work/ipa-groups/<g>`) into `src/blob/groups/<g>` with
  members = its `claims` (not already locked), everything else as context, then `blob_group.splice`. Skips groups that
  already exist under `src/blob/groups/`; for an existing group edit its `group.json` members by hand and run
  `python3 -m tools.conveyor.pipeline.blob_group splice <g>`.
- `reflow.py FN FILE.c OUTDIR [N]`: all newline layouts of one function body (token-identical), for the line-placement lever.
- `loop.sh FN SRC.c [flags]`: the Rocky loop (compile, strict score, workbench diagnose). Expects `~/agents/wb/` on Rocky
  (`targets/<fn>.o`, `base/<fn>.c`, `wb/src` = vendored workbench).

After any splice: `python3 -m tools.conveyor.pipeline.blob_rom rom` must print `SHA-1 EXACT`; `blob_splice check` and
`blob_group check` must report 0 problems; run pytest with its exit code captured; then commit the new `src/blob/**` files,
`git add -u asm/us/blob blob_matched.lock.json` (new region files need `git add -f`; after a layout change `git rm` region files
the layout no longer lists, then `blob_tu generate`), set the coordinator statuses to `matched`, push.
