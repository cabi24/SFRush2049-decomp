# Image A menu color helper: genuine caller-context match candidate

Frozen input base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Follow-on to [draft #288](https://github.com/cabi24/SFRush2049-decomp/pull/288), extending #266.

**New A:8039ED20, 480 bytes, 120/120 exact words**, with zero uncertainty,
errors, unresolved symbols or extra words. The complete actual EF08 caller
provides its private save/clobber context. Both real four-byte addressed color
objects and all fourteen branch-specific color calls are retained.

**A:8039D494 retains its prior 528-byte, 132/132 exact candidate.**
There is no duplicate credit for that previous claim.
Its complete natural `(item, player)` source already existed at
`cloud/work/frontier/dot_runtime_a_availability_boundary_20261006/candidate.c`.
The new E3BC list builder and D6A4 renderer are genuine callers. Together they
prevent inappropriate single-caller inlining and recover the native private
register reservations. No dummy parameters, retention barriers or fake callers.

Seven newly reconstructed complete low bodies now provide 10,164 bytes:
D05C cleanup (676), D6A4 option animation (2,908), E200 record navigation (444),
E3BC available-item list (136), FF04 slot creation (1,428), ED20 color update
(480), and EF08 car animation (4,092). Relative to #288, ED20 and EF08 are new. The three prior
high bodies and old D300 product remain context, without duplicate credit.

| Body | Differing words | Extra | Unverified | Errors |
|---|---:|---:|---:|---:|
| ED20 | 0/120 | 0 | 0 | 0 |
| EF08 | 991/1023 | 0 | 50 | 0 |
| D494 | 0/132 | 0 | 0 | 0 |
| E3BC | 5/34 | 0 | 0 | 0 |
| E200 | 99/111 | 1 | 0 | 0 |
| D05C | 121/169 | 0 | 0 | 0 |
| FF04 | 331/357 | 0 | 20 | 0 |
| D6A4 | 696/727 | 0 | 10 | 1 |
| D300 | 101/101 | 0 | 0 | 0 |
| A0498 | 154/720 | 0 | 0 | 0 |
| A0FD8 | 875/949 | 0 | 14 | 0 |
| A1EAC | 1710/1776 | 0 | 12 | 0 |

Every body has zero unresolved symbols. Own-data differences in the large
renderers remain explicit; they are not accepted instruction matches. The
high root's score is worse than #266, so this packet does not supersede its
best high-source baseline as matching evidence. The new claim is ED20 only.
E444 and 90DCC remain real external services; the complete private context is
still incomplete. The latter's nine natural source inputs follow the prior
complete #263 definition rather than invented argument slots.

## Reproduce

```
python3 cloud/work/lean/runtime_a_car_select_20261006/reproduce.py
```

Actual IDO 5.3 recipe: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical group
pipeline and `as1 -r4300_mul`. Only the real A1EAC root is kept. `fabsf` uses
the stock IDO intrinsic pragma, as in existing matching source. The small
helper authenticates the frozen asset and image A in memory and binds the
unchanged scorer's owned-data checks. Only compile/scoring was performed.

## Required context and assumptions

The complete source preserves the real 64-byte animated slots, 44 slots per
player, 52-byte cleanup records, record handle lists, distinct persistent and
cached record chains, availability gates, projected selection animation,
alpha/color updates, model creation, resource teardown and repeated calls.
The two new four-byte color locals are real copied/addressed objects; their
alpha byte is changed and passed to the existing color helper. No padding,
artificial volatile, unused pressure local or fabricated formal is added.

D6A4/FF04/EF08 constants are decoded values from authenticated image A,
including tables 0x803B9610..0x803B9624 and 0x803B9674..0x803B971C.
EF08 supplies actual type evidence for the car slot's float selection weight
at +0 and signed car ID at +9, plus the record's +40 associated-car handle.
It preserves the main and secondary previews, ghost-record setting reloads,
angle wrapping, selection fades and one-player color/overlay calls.
No native bytes or raw assembly are published. Record field +36 is retained
separately from the prior +40 active field. E200's source uses two real inputs,
a record handle and signed-byte direction, rather than invented private-ABI
argument slots. Source names, full object types and whole-TU visibility remain
hypotheses. Valid player/selector ranges, acyclic handles, valid model/string
assets, finite arithmetic and nonzero animation scales are required. Color
alpha conversion requires a representable nonnegative value; inherited high
buffer-capacity assumptions remain unproven. No group here is a drop-in ABI
replacement until the checker resolves its complete native context.

Observed match candidate is not accepted coverage. Independent checker owns
acceptance and ROM integration; no additional review, test suite or CI wait.
