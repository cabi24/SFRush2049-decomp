# Fresh small-function probes, 2026-10-05

Research only. **No strict matches, no claims, zero accepted functions or bytes.**
The sources are full-body leads for independent review; neither belongs in the
accepted source tree. No protected targets, symbols, contexts, scorer, flags,
locks, or accepted source were changed.

Base: `f88dc3cb3807b8be3246b719a9b1e008210b477c` (freshly fetched master).
The two retained ordinary-ABI targets were selected outside the supplied active
worker ranges. Repository claim files and current locks were checked before edits.
Absence from those files is not proof that no unpublished worker has touched them.

## Retained leads

- `func_800DC628`, `[0x800DC628, 0x800DC720)`, 248 bytes:
  the saved B14 word-size baseline is **40/62 differing words** at O3;
  the retained candidate is **20/62**. Assigning through the genuine u16 global
  into a signed count preserves the observed narrowing and signed comparison,
  without an explicit mask carrier. Moving the buffer pointer assignment into
  the positive-count block removes an avoidable pointer copy. The inverse
  permutation loop itself was already right. The remaining differences concern
  the initializer's shared address and count/buffer register allocation.
- `func_800D4DFC`, `[0x800D4DFC, 0x800D4EF4)`, 248 bytes:
  the saved A62 baseline is **16/62**; the retained candidate is **12/62**.
  The coefficient is now a natural `2.72727275f`, with all four owned literal
  bytes verified independently. The product operands follow the observed loads,
  and the amount is stored directly instead of introducing a named register
  temporary. The remaining twelve differences are the load/store scheduling and
  FP temporary allocation of independent copied fields. The real accepted
  `math_utility` stays **MATCH**, 76 bytes, in a two-function O3 unit; the caller
  remains 12/62 there.

Both retained functions have exact ELF symbol extents. GNU MIPS linking
independently resolves the entire function and reproduces the scorer's differing
word offsets. Eight zero section-alignment bytes after each function are excluded
from the body, checked as zero, and reported separately. This is proof of the
remaining mismatch, not body equality.

## Bounded hypotheses and stop points

`func_800DC628`: O2/O3 unchanged; array/defined/static initializer declarations
unchanged; chained u16 assignment improved 40 to 22; guarded pointer creation
improved 22 to 20. Shared index, split increment and array flag forms stayed at
20 or worse. Reading the count global throughout the clear loop added accesses.
No padding, dead reads, stand-ins, or compiler changes were tried.

`func_800D4DFC`: natural literal alone stayed at 16; correcting the first product
operand order reached 14; direct amount storage reached 12. Typed field/array
and natural grouped-copy views were worse. A small set of line groupings based
on the vector-copy boundaries did not close the residual; no layout sweep was
run. Workbench diagnosis preceded these probes.

Two further unclaimed targets were stopped without retaining another source:
- `func_800A43FC`, `[0x800A43FC, 0x800A44D0)`, 212 bytes: the natural old loop
  remains 51/53, with a 124-byte candidate. Volatile flag/field and bitfield
  controls did not naturally recover the two-record native unroll. Explicit
  paired source remains nonmatching and was not adopted.
- `battle_mode_setup`, `[0x800D4EF4, 0x800D5050)`, 348 bytes: the old genuine
  three-vector scratch source remains 12/87. All-delta-first, compound stores,
  and three grouped-expression controls were worse. No source improvement.

Do not restart these residuals with a blind variable/order sweep. Useful new
input would be a genuine source/TU boundary or matching arcade declaration/copy
macro. No arcade reference checkout was available in this environment, so no
arcade ancestry is claimed.

## Verification

Run from the repository root with the pinned IDO and GNU MIPS toolchain:

```sh
python3 cloud/work/frontier/dot_fresh_small_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/conveyor/test_dot_fresh_small_20261005.py
```

The fresh verifier checks canonical target integrity, source hashes, whole ELF
extents, full relocated code, own literal content, and the unchanged accepted
callee in a real group. It additionally runs:
- 4,608 bounded packing cases: valid/invalid capacities, initialization on/off,
  shuffled permutations, u16 narrowing, and u32 wraparound boundaries
- 4,096 finite snapshot cases: the coefficient and direct narrowing, every copied
  field, the real basis-copy contract, and all untouched object bytes
- AddressSanitizer and UndefinedBehaviorSanitizer on that host harness
- Two semantic negative controls: wrong coefficient and wrong position reset;
  both must be rejected by assertions

Host checks compare the C with a separately written reference model. They do
not execute the native instructions and do not establish general runtime
behavior, NaN/out-of-range floating conversion behavior, or complete game
correctness. No game-image, whole-program shadow, compressed-stream, or ROM hash
gate was run. All candidates remain NONMATCH.

`verification.json` is a reproducible receipt, not a substitute for fresh
compilation. The tests rerun the full proof and compare the receipt exactly.
No native words, raw disassemblies, objects, or proprietary data dumps are stored
in this packet.

Scoped regression run: all 675 tests passed across the packet's three fresh
checks and `test_cloud_score.py`, `test_cloud_guard.py`, and
`test_cloud_submissions.py`. This is a scoped tooling suite, not the full
repository suite. The changed-submission checker checks zero submissions for
these research paths; the explicit verifier above is what recompiles them.
