# Donor-backed target-overlap predicates

**Two strict matches, 640 bytes total. No accepted-byte or ROM-coverage gain.**

- `func_8010C448`: `[0x8010C448, 0x8010C588)`, 320 bytes / 80 words.
- `func_8010C588`: `[0x8010C588, 0x8010C6C8)`, 320 bytes / 80 words.

Scouted against master `264381e9`; neither range had an accepted lock or a pending
submission in open PRs 98–108. The neighboring active B55FC lane was excluded.

Both match the complete manifest-protected target under ordinary IDO O3 and O2,
including all four relocations. Neither owns literal/data bytes or has alignment
bytes outside its ELF extent. GNU linking independently reproduces every byte.
No production source, lock, protected target, scorer, compiler, keep list or
build recipe is changed. Publication, integration and merging are separate.

## New source evidence

Pinned arcade source is `rushtherock` revision
`845329d7b36f5a384c5625ed9a0aef584ab46139`:

- [`game/targets.c`, OverlapTarget, lines 673–693](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/targets.c#L673-L693)
- [`game/vecmath.h`, mvecsub/mveccopy, lines 20–21](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.h#L20-L21)

The donor supplies the model-collidable gate, selected car position, vector
subtraction, squared distance, radius inflation, optional gap output, and the
`dsq, gap` declaration before the vector. It also supplies the crucial operation
order: compute squared distance first, then enlarge radius, then calculate gap.

N64 differences are authoritative native observations: selector/radius are
passed by pointer; origin is snapshotted; the sphere becomes an X/Z cylinder;
the radius increment is 3.5 rather than arcade 11.2; vertical bounds are strict
(-2, 18) or (-2, 4). The final explicit true/false branch preserves native NaN
comparison behavior. The donor is an algorithmic ancestor, not a claim that
these exact original N64 source files have been recovered.

Earlier work had no arcade checkout and reported 45/80 and 43/80 differences,
with deficient 24-byte stack frames. All historical source and proof files are
unchanged. The new source uses the donor declaration and operation order, the
natural X-before-Z source sum, and native branch shape. Both 32-byte frames and
all 80 words now match. Every local has a real use. No dummy storage, dead reads,
empty conditions, forced volatile, extra parameters, synthetic helpers, or
stand-ins are introduced. The pinned vector macros themselves did not improve
the prior source; they are not presented as the matching cause.

The initial C448 donor spelling gave 42/80 with the correct frame. Restoring
X-before-Z reduced it to 24/80; the native true/false branch reduced it to 16/80;
the donor distance-before-radius order closed the residual. The final receipt
repeats four fixed ablations independently for each target. These controls
measure this source combination, not uniqueness of the original spelling.

## Scope and proof

`verify.py` checks final unchanged source, complete ELF function sizes, every
relocation, independent GNU linking, O32 layout assertions, and a genuine five-body
O3 regression with both candidates and three unchanged accepted adjacent callback
bodies (`func_8010C02C`, `func_8010C2E4`, `func_8010C7CC`). That is a neighboring
binary regression with separate existing type views, not a unified type model
or full-game shadow compilation.

The native executor is adapted from the already-reviewed cylinder research,
with instruction coverage, stack canaries and FP callee-save checks added. The
host harness includes the actual final candidate, unchanged, and is compiled as
C89 with UBSan. A separately written binary32 oracle checks arithmetic and bounds.
Native words, GNU-linked words, oracle and host compare return values and all
input/output memory, including nine output arrangements (null, separate, three
origin aliases, radius alias, three selected-position aliases). Disabled-path
native reads, output-store order and full selected-record canaries are checked.
The final corpus contains 16,092 cases per target: 32,184 cases and 64,368
native/linked executions in total. Eight focused regression tests pass.
Five compiled wrong-contract controls per target test radial equality, both vertical bounds,
radius expansion and plane selection. All must fail behavior comparison.

The bounded corpus includes signed-zero, subnormals, extreme finite values,
infinities, quiet NaNs, threshold neighbors and deterministic random values.
NaNs are compared by class; hardware FCSR/exception behavior and signaling-NaN
payload identity are outside the claim. Both functions execute 79 of 80 offsets.
The duplicate constant-load at offset 0xfc is unreachable after branch-likely
copying; it is still included in the full-byte proof. A valid readable selector,
radius, selected records and enabled-path origin are preconditions. Eight records
are test allocation capacity, not a recovered game population count.

No real-game callers are invented. Protected direct-call/function-address scans
found no direct JAL or simple LUI/ADDIU pointer materialization references to these
two bodies. Their callback role is supported by the adjacent accepted callback
family and pointer-shaped native signatures; runtime dispatch is not proven.
No gameplay, whole-image, compressed-stream, full-ROM SHA-1 or CI pass is claimed.

## Reproduce

With pinned IDO and GNU MIPS binutils available:

```sh
python3 cloud/work/frontier/dot_target_overlap_20261005/verify.py \
  --donor /path/to/rushtherock \
  --output /tmp/target-overlap-verification.json
python3 -m pytest tests/conveyor/test_dot_target_overlap.py
python3 tools/cloud/score.py fn cloud/matches/func_8010C448.c func_8010C448 \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 tools/cloud/score.py fn cloud/matches/func_8010C588.c func_8010C588 \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

The donor option additionally verifies HEAD and exact checked-out file bytes
against the pinned Git objects. It is optional for compiler/behavior replay.
Receipts contain source hashes, counts and verified identities; raw target words,
assembly dumps, objects and donor checkouts are not included in this packet.
