# Twelve-function source-contract repair integration

Base: `abc0f256e8c6b5e2004662af09aca7c5bf915204`.

This integrates reviewed repairs for 12 already identified candidates, totaling
540 target bytes. It does not promote a function, migrate a lock, establish a
new match, or claim additional cartridge coverage. The actual ROM build and
SHA-1 transaction remain with the maintainer and independent checker.

## Narrow source changes

- Eight 44-byte macro wrappers use the shared opaque state/control and known
  command types already accepted by `lib_22300.c`. New adapted source paths
  preserve the original individually locked sources. No production slot changes.
- Three SDK queue wrappers use the genuine `OSMesgQueue_s` tag. Their existing
  normal-CI-selected source paths, function bodies and flags are retained.
- The declaration of `func_8001EDF4(u32)` in `lib_207b0.c` returns `int`, agreeing
  with its accepted definition and unchanged 48-byte identifier-wrapper source.
  The candidate's production assembly passthrough is retained.

No protected targets, symbol addresses, pinned contexts, lock files, generated
layouts, compiler settings, or scoring code change. All 383 static body locks
remain intact. No builder or coordinator was contacted.

## Final-tree evidence

Each packet was independently peer reviewed and then replayed from the combined
integration checkout. Each fresh structured result is exactly equal to the
corresponding checked-in evidence, so the evidence binds the final integrated
sources rather than only the isolated author branches.

- [Macro proof](../macro_wrapper_contracts/README.md): 8 candidates and all 15
  previously accepted C bodies strictly match after relocation. Actual baseline
  and overlay TUs have all 66 function offsets and all 13,552 raw text bytes
  unchanged. Allocated non-text bytes are unchanged. Negative controls and the
  36-case host forwarding harness pass.
- [Queue proof](../queue_wrappers/README.md): 3 candidates and all 5 accepted
  C bodies match in the actual combined TU; all 21 TU slots, totaling 3,836
  protected target bytes, match after strict relocation. The independent review
  additionally confirmed exact C symbol extents and identical whole-TU GNU-linked
  text. The 4 final alignment bytes are outside those target slots.
- [Identifier proof](../identifier_contract/README.md): all 17 accepted C bodies
  plus the candidate match exact extents and full relocated words. Independent
  GNU linking checks all 24 TU slots; the full 2,656-byte linked text is identical
  for the original, repaired and overlaid source, including 12 alignment bytes.

Together these verify 49 candidate/existing C bodies across three real TUs,
including all 37 previously accepted C bodies in those TUs. The figures above
describe different checks: the macro packet does not claim GNU-linked whole-TU
target verification for every assembly passthrough. All raw bytes, objects and
assembly used by local checks stay out of this publication.

The integration record in `verification.json` gives aggregate test results,
source/evidence hashes, and commands. The original per-packet documents retain
their isolated test counts as historical results.

## Explicit CI coverage

`check_submissions.py` selects the three queue sources and reports 3/3 matches.
It does not select the adapted macro sources or the production identifier TU.
The standard CI suites now explicitly execute all three real-TU regressions:

- `tests/cloud/test_macro_wrapper_contracts.py` collects packet source/extent and
  post-promotion lifecycle tests and runs the full macro verifier in an isolated
  subprocess.
- `tests/cloud/test_identifier_contract.py` runs the identifier actual-TU proof.
- `tests/conveyor/test_queue_wrapper_promotion.py` runs the queue actual-TU proof.

With `REQUIRE_TOOLCHAIN=1`, the new explicit regressions fail on missing
toolchain dependencies. The local aggregate run still skips nine older
cross-GCC checks because that compiler is unavailable in this recovered local
environment. The remaining 32 skips require private/generated inputs. These
skips are not reported as passes; exact-head CI installs cross-GCC separately.

The macro and queue proofs keep fixed historical baselines and negative controls,
but verify the actual current TU and overlay only candidates still represented
by assembly. Temporary partial/full-promotion fixtures exercise migrated locks.
Missing current locks and an incorrect promoted body with a freshly recomputed
temporary lock are rejected. These fixtures do not edit production source or
real locks, and CI continues checking the repaired sources after promotion.

Reproduce from the repository root with the pinned IDO compiler, MIPS binutils
and pinned submodules available:

```sh
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor tests/cloud -q -rs -m 'not node_required'
python3 cloud/work/boot_tail_promotion/macro_wrapper_contracts/verify.py > /tmp/macro.json
python3 cloud/work/boot_tail_promotion/queue_wrappers/verify.py --output /tmp/queue.json
python3 cloud/work/boot_tail_promotion/identifier_contract/verify.py --out /tmp/identifier
python3 -m tools.cloud.guard_paths --base abc0f256 --head HEAD --lock-revision abc0f256
python3 tools/cloud/check_submissions.py --base abc0f256 --head HEAD
make check-matched
git diff --check
```

## Remaining maintainer transaction

Refresh candidate/context evidence for the repaired paths; the existing pinned
context records are deliberately unchanged. Adapted macro source paths require
the correct gated lock replacement, not duplicate source-lock entries. Use the
usual narrowly scoped promotion transaction, force fresh objects, verify the
actual complete cartridge SHA-1, and migrate locks only through that gate.
Do not run the legacy batch drivers against unrelated refused candidates.
The draft PR leaves merging to the independent checker.
