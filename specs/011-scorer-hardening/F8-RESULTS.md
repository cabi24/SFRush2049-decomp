# F8 validation — 2026-09-29

`tools/cloud/rank_near_miss.py` runs the strict `score.py fn` command for each
INDEX entry's `base.c`, using its recorded flags. It rewrites the index with
both historical pipeline scores and strict word-difference counts, sorted
ascending with deterministic name-based ties. Nonzero excess instructions
count as differences; zero padding does not. The scorer's explanation remains
visible beside each count.

Only exit-zero plain MATCH results leave the ranked worklist. Unverified or
unresolved references, relocation errors, and excess instructions never become
free matches, even if the compared-word difference count is zero. Expected
compile/missing-target failures remain as unscored entries at the end. Tool or
integrity failures abort without rewriting the index. Missing sources, malformed
rows, duplicate names, and concurrent index/manifest changes also abort.

Strict matches are reported in a separate table, retaining their flags and
source provenance. Subsequent runs rescore both tables, so matches are not lost
from the report and changed sources can return to the worklist. Source
directories and locked files are untouched.

## Results on the current targets

All **104** original candidates compiled and scored on watchman2. None had
unverified relocations or unresolved symbols in this run.

- **103** remain in the ranked worklist, with **1–100** strict word differences.
- **39** have nonzero instructions past the target extent; those instructions
  contribute to the strict count.
- **1** plain MATCH was removed from the worklist: **`func_800C54F0`**, using
  `-g0 -O2 -mips2 -G 0 -non_shared`. It is already present in
  `blob_matched.lock.json`, so this identifies stale work rather than new ROM
  coverage. Its source and flags remain in the separate match table.
- Four source candidates are one word away: `audio_start`, `func_800B1F30`,
  `func_800DD45C`, and `func_800FBE30`. `audio_start` is already locked using its
  promoted source; the other three are not locked. The older near-miss source
  for `audio_start` remains because it does not itself strictly match.

## Validation

- Two full real-IDO ranking runs produced identical table contents and order,
  including rescoring the reported match on the second run. All 104 original
  names, flags, pipeline scores, and source-provenance labels were preserved.
- The 24 new tests cover strict result parsing, recorded flags, extra words,
  unverified/unresolved/error results, unscored compile failures, sorting,
  metadata preservation, repeatability, and preserving the index on failure
  or concurrent modification.
- Full Conveyor suite with `-m 'not node_required'`:
  **569 passed, 13 skipped, 5 deselected** on x86;
  **450 passed, 132 skipped, 5 deselected** on the Pi.
  The five deselected tests need live compute nodes.
- `git diff --check`: passed.

Ranking and compiler tests used the isolated checkout at
`watchman2:~/rush2049/tmp/f6-ci-ya7TJQfU/repo`, with the F6 target hashes and
the target revision from `d41dbb8`. Raw run logs remain at the scratch root
as `f8-ranking.log` and `f8-reranking.log`. The shared builder checkout and
pre-existing dirty m2c submodule were not changed.
