# Fresh matching batch: three complete object matches

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b` (2026-10-05).

## Scope

Three new complete strict object matches are submitted:

- `func_800ACFF8`: 152 bytes, rotates matrix rows 1 and 2.
- `func_800AD090`: 152 bytes, rotates matrix rows 0 and 1.
- `func_800BE744`: 120 bytes, counts byte or marker-prefixed two-byte characters.

The total is **424 bytes of object-match evidence**, not newly integrated ROM
coverage. See `../dot_matrix_rows/README.md` and
`../dot_fresh_string/README.md` for the native semantics, ABI, complete extents,
flags, independently replayable proofs, and source-form search histories.

Two explicitly nonmatching research packets are kept outside `cloud/matches/`:

- `../dot_fresh_masked_rng/RESULTS.md`: 13/67 differing words at an exact
  268-byte ELF extent after 41 bounded compiler controls. No match claimed.
- `../task_enqueue_20261005/`: typed scheduler-task initialization and enqueue
  research. The emitted function is 124 bytes against a 128-byte native target;
  the shortened extent is an explicit failure, even if a scorer ignores padding.

The research packets retain useful semantic tests and failed hypotheses so
future passes do not repeat blind source permutations. Neither adds accepted
bytes or changes a production source, target, symbol, context, lock, scorer,
compiler setting, build gate, or existing caller source. PR #82 and PR #83
contract-repair work is separate and not included.

## Integration validation

The final source tree is validated with the unchanged protected-path guard,
changed-submission scorer, scorer sanity checks, static-lock check, both packet
verifiers, focused semantic/negative controls, and the repository test suites.
The Conveyor portion reports **1,656 passed, 15 failed, 41 skipped** and 9
node-required cases deselected. Its 15 failures exactly reproduce the current
baseline's unchanged locked-single section-relative relocation refusals. The
corrected-environment Cloud suite reports **277 passed, 2 failed, 25 subtests
passed**, with no skips. Both failing builder subtests exactly reproduce the
baseline's existing prose-in-flags parsing problem. No unrelated fixes or gate
changes were made.

The first aggregate run found 18 existing Cloud cases unable to see IDO because
they test the conventional `tools/cloud/ido` path rather than `IDO_DIR`. The
unchanged pinned binaries were materialized at that ignored path, and the
entire Cloud suite was rerun. The integration receipt preserves the first run
and corrected replay separately. The 41 remaining skips are 9 missing MIPS
cross-GCC cases and 32 missing SDK/ROM/generated/private-input cases. This is
not a green CI claim.

Independent source-bound reviews are required before draft publication. The
matrix proof includes the genuine traction-control caller group without changing
any of its five existing bodies; its `steering_sensitivity` nonmatch remains a
nonmatch. The string kept-single-source probe is a compiler control, not a
whole-program shadow gate.

The string single-file whole-object hashes can vary with source worktree paths:
independent replay localized this to `.mdebug` metadata. Executable sections,
relocations, all complete-body comparisons, and the kept-group proof agree.
Whole-object hash equality across worktrees is not claimed.

## Acceptance boundary

No blob splice, production lock, accepted-source migration, source-image build,
compressed-stream check, full-ROM hash check, or merge occurs in this batch.
The normal whole-program shadow and source-built image/ROM acceptance gates
remain for the independent checker. Native object files, raw target/assembly
dumps, ROM data, credentials, and private execution logs are not submitted.
