# Contract: the promotion transaction

`tools/conveyor/pipeline/promote.py` — library + CLI
(`promote run <segment>:<function> --from <path|candidate_id>`), also called by
the upgraded `verify_promote` job on the builder.

## Preconditions (all checked before touching anything; refusal names remedy)

1. Segment converted; function in `passthrough` state.
2. Evidence: a `matched.lock.json` entry for the body, OR a score-0
   `matrix_entry`/verify record for (function, flags), OR `--override` with a
   mandatory `--reason` recorded into the promotion record.
3. Segment flagset pinned (flag_registry / layout map); else refuse → flag
   sweep is the named remedy.
4. `git status` clean for the TU, lockfile, and yaml.
5. Real-compiler build available here (IDO present) or `--via-builder` (ssh /
   conveyor job); else refuse (FR-008). Never a gcc fallback.

## Transaction

1. **Splice**: replace the function's pragma with the body + provenance header
   (source path/origin/commit, flags, evidence scores, date). Body source: a
   repo path (reference src file or work artifact) or a corpus candidate id.
2. **Build + gate**: full `make` + SHA-1 verify (`make test`), COMPILER=ido.
   The hash is the only authority — no partial credit, no per-function diffing
   at this stage.
3. **On pass**: migrate the lock (add `src/rom/<name>.c:<func>` pinned entry;
   remove a superseded reference-path entry for the same target if present);
   insert `promotion_record` (function, tu, source, flags, evidence,
   commit-to-be); `git commit` TU + lockfile + record-adjacent files in one
   commit with the standard trailer.
4. **On any failure** (compile, link, hash, interrupt): `git checkout --` the
   spliced TU (clean at precondition 4, so restoration is exact), record the
   refusal (same schema, outcome=failed + first error), exit nonzero. The repo
   MUST be `git status`-clean and SHA-1-buildable after any outcome.

## Batch mode

`promote batch --locked` iterates the lockfile's entries whose functions are
passthrough slots in converted segments, one transaction each, stopping on
first failure (SC-004's driver). No batch-level shortcuts: N functions = N
full gates. (Slow is fine; correctness compounds.)

## Post-conditions (tests assert)

- Failed promotion leaves zero diff (`git status` clean, SHA-1 exact).
- Passed promotion: TU has the body, lock green at the new path, coverage
  increments by exactly that function's size, record row exists.
- Re-running a passed promotion refuses (`already promoted`).
- The conveyor job path and the CLI path execute the identical library code
  (one implementation; test by construction/import, not duplication).
