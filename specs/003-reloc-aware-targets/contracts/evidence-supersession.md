# Contract: evidence attribution + supersession

## Attribution echo (node-visible; ships in the 003 toolkit rebuild)

- **Submit side** (all three cell producers — `pipeline/matrix.py` submit,
  `pipeline/corpus.py` submit, `pipeline/lock.py` verify): each manifest cell
  target entry gains `"target_o_sha": <sha256 hex of the bundled .o bytes>`.
  The sha MUST be of the exact bytes placed in the bundle (for matrix/corpus
  that is `n64_target.target_o_sha` by definition; for lock verify likewise).
- **Node side** (`jobs/compile_score.py`): every result cell copies
  `target.get("target_o_sha")` through verbatim when present. No other node
  behavior changes. Failed-compile cells carry it too (attribution is about the
  pairing attempt, not success).
- **Ingest side** (`pipeline/matrix.py` cmd_ingest): store
  `cell.get("target_o_sha")` into `matrix_entry.target_o_sha`. Old blobs without
  the key ingest as NULL — never rejected.

## Supersession purge (Pi-local, `pipeline/targets.py`)

Trigger: `populate()` computes a target object whose sha differs from the stored
`n64_target.target_o_sha` (including NULL→sha on first build).

Action, atomically with the `n64_target` row update:

```sql
DELETE FROM matrix_entry WHERE target_id = :target_id
```

- All toolkits, all candidate origins. (Old-toolkit rows were already dead for
  rankings; they go too so the attribution invariant is unconditional.)
- Never touch `work_unit`, result blobs, or `function_status` here.
- Count and report: `superseded: <n> targets, <m> evidence rows purged`.
- First-build case (NULL→sha) purges zero rows by construction but MUST follow
  the same code path (no special casing).

## Post-conditions (tests assert these)

1. **Reschedule**: after a purge, `corpus submit --dry-run` /
   `matrix submit --dry-run` count the purged target's pairings as new work;
   unchanged targets contribute zero new cells.
2. **No stale reads**: `rankings_for`, `corpus report`, `_corpus_best`,
   `_recompute_reloc_flags`, and `update_rankings` operate on `matrix_entry`
   only — after purge they cannot see stale evidence because it does not exist.
   No new filters needed; tests still assert the visible behavior (flag drops /
   ranking empties after purge, repopulates after re-ingest).
3. **Attribution invariant (SC-006)**: after re-ingest, every surviving
   `matrix_entry` row for a target has `target_o_sha = n64_target.target_o_sha`.
   `corpus report` prints a one-line check:
   `attribution: <n> cells checked, <k> mismatched (expect 0)` — nonzero is a
   loud warning, not a crash.
4. **Cache correctness**: identical resubmission after re-ingest is 100%
   `cached_result` (manifest unchanged); resubmission after a target change is
   fresh compute (manifest's `input_shas` changed). Both already fall out of
   001's content addressing — tests confirm, not implement.

## Flag interplay (002 boundary)

`_recompute_reloc_flags` requires **no changes**: after a purge it sees no
corpus evidence for the target (condition unsatisfiable → flag clears per 002
rules); after re-score it sees only current-object evidence (flag upgrades to
the promotion path on true-0, or re-stamps honestly). Task-level test: flagged
target → purge → recompute clears → re-ingest true-0 cell → normal
candidate_identified path (and artifact regeneration per 002 stays consistent).
