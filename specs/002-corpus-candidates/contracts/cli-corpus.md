# CLI Contract: `python3 -m tools.conveyor.pipeline.corpus`

Global options (same conventions as matrix/lock): `--data` (default `~/.conveyor`),
`--coordinator` (default `http://127.0.0.1:8323`), `--token`.

All commands are Pi-local except `submit` (talks to the coordinator) — none touch
the network beyond the LAN coordinator. Exit code 0 on success, 1 on any refusal or
failed invariant, with the reason on stderr.

## `corpus register <origin> <path> [--repo-url URL] [--include-dirs a,b]`

Records a corpus root. Refuses (exit 1) if: origin is `arcade`, contains `/` or
`.c`; path is not a git clone; origin already registered with a different path.
Re-registering identically is a no-op. Records `commit_sha` from
`git -C <path> rev-parse HEAD`.

Output: `registered <origin> at <path> @ <short-sha> (include_dirs=[...])`

## `corpus ingest [<origin>] [--allow-dirty]`

For each registered root (or just `<origin>`): re-check clone presence, commit, and
`git status --porcelain`. Refuse dirty/missing/moved-commit clones unless
`--allow-dirty` (which records `<sha>-dirty`). Extract every function with the
shared extractor; upsert candidates with corpus id grammar, origin, provenance.
Idempotent: unchanged root ⇒ `0 new, 0 changed`. A function whose body changed
(commit moved) gets a new `body_sha` — its future cells are new cache keys by
construction.

Output (per root): `<origin>: <N> functions from <M> files (<new> new, <upd> updated) @ <sha>`

## `corpus submit [--origin X] [--flagsets ...] [--dry-run]`

Pairs every `n64_target` whose `target_id` equals a corpus candidate's name (exact,
case-sensitive) with that candidate. For each pairing × flagset not already scored
under the pinned toolkit: build a `compile_score` job (reduced TU + resolved
headers + registered include_dirs + target .o from inventory), priority 50, submit
in batches. Skips (with counted reasons in output): target has no `.o` blob;
candidate body no longer extractable.

Output: `<P> name pairings; <J> jobs submitted (<C> cells), <K> cache hits, skipped: no_target_o=<a> unextractable=<b>`

`--dry-run`: print the counts, upload nothing (mirrors `matrix submit --dry-run`).

## `corpus ingest-results`

Thin wrapper over the shared `matrix ingest` machinery (corpus cells ARE matrix
cells) plus corpus post-processing:

1. Ingest DONE `compile_score` results (both scores) — shared code path.
2. Recompute `reloc_only_diff` flags from `matrix_entry` per data-model rules.
3. Write/refresh `corpus_match.c` artifacts for flagged targets.
4. Run `update_rankings` (shared) so true score-0 targets promote normally.

Output: standard ingest line + `reloc_only_diff: <n> flagged (<d> newly), <a> artifacts written`

## `corpus report`

Pi-local, no coordinator needed. Sections:

1. Roots: origin, commit, ingested_at, candidate count.
2. Pairing coverage: targets paired / skipped(+reasons) / scored under pinned toolkit.
3. Compile rate per origin (distinguishable in the failure-clustering report too).
4. Per-target best evidence table: `target  best_true  best_reloc_blind  candidate  flagset`,
   sorted by (reloc_blind, true), with `--target X` detail mode like `matrix report`.
5. Flag summary: counts of true-0 (promotion path) vs `reloc_only_diff` vs neither.

SC-005 acceptance: section 4/5 answer "what's blocked only on relocations, from
which source" in one command.

## Changes to existing commands

- `matrix ingest`: also reads `score_reloc_blind` from result cells when present
  (NULL otherwise). No behavior change for arcade-only use.
- `matrix report`: coverage lines split by origin when corpus candidates exist.
- `pipeline.lock add/verify`: unchanged interface; internally imports the moved
  `seeds/context.py` helpers.
