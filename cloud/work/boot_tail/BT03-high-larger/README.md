# BT03 high larger packet

Source base `301d9e7552ad4fd7f54a38796db84671e1000d35`. Parent activation
2026-10-04 01:06 UTC, centrally persisted at `9795ae13`, covers six fresh bodies
/ **1,568 B**. Branch `dot/boot-tail-bt03-high-larger` reuses the clean existing
medium-tail worktree; previous medium-tail head `c618e421` remains frozen on its
original branch. No earlier packet is changed.

## Results

| Function | Bytes | Retained O2 | O1 control |
| --- | ---: | --- | --- |
| 8001B29C | 260 | MATCH | 65/65 + 11 extras |
| 8001B3A0 | 260 | MATCH | 65/65 + 11 extras |
| 8001B7C0 | 260 | MATCH | 65/65 + 11 extras |
| 8001F6EC | 256 | COMPLETE-NONMATCH, 30/64 | 63/64 + 15 extras; unpaired HI16 |
| 8001D660 | 260 | COMPLETE-NONMATCH, 23/65 | 64/65 + 7 extras |
| 80019AD4 | 272 | COMPLETE-NONMATCH, 18/68 | 68/68 + 12 extras |

Three full relocated matching bodies / **780 B**, with zero masks, nonzero
excess words, unresolved/unverified references or errors. Three complete bounded
research residuals / **788 B** have no match credit. Rejected O1 HI16 errors are
preserved exactly, including initial chain controls; proof rules are not relaxed.

## Native operations, ABI and domains

The three matching setters take a genuine word key and a byte value. Actual
`1EDF4(u32)` supplies the linked voice identifier or FFFFFFFF. Each iteration
uses its low byte as the slot, validates the full word against the packed voice
identifier+96, and stops at the first stale member. A valid member changes the
result from-1 to0 and calls the real four-byte-input `20610` with command10,
131 or7, slot, channel and value. Flag bit2 at+36 chooses channel+85 rather than
+75. The next identifier+16 is read **after** that call, so callback changes to
the chain are live. Registered slots lie within the allocated416-byte voice
array; valid chains terminate without cycles. No malformed-identifier safety is
claimed. All four byte widths agree with `20610` entry masks; no extra formal or
stubbed callee is used. Native packed fields and the full416-byte stride are also
checked by host compile assertions because this view contains no pointers.

`1F6EC` first calls actual one-pointer `1EE9C`, then reads the possibly changed
identifier. It clears the command word+0 and channel byte+46, appends an inactive
slot to the free queue, adjusts the external/internal byte count according to
byte+76, and invalidates the identifier. The free queue is a distinct32-entry
array at50440 of4-byte records (previous byte, next byte, active halfword); head,
tail and the two counters are neighboring separate byte globals504C0..504C3.
Valid registered low-byte slots and queue neighbors are0..31 orFF sentinel,
with a valid nonempty tail. Byte decrement deliberately retains modulo256,
including native underflow behavior. Only the real unlink helper is declared.

`1D660` has **six real inputs**: spatial-state pointer, position/velocity/forward
vector pointers, up-vector pointer and a byte level. The24-byte native frame
reads the fifth pointer at new sp+40 (incoming+16) and byte level at new sp+47
(incoming+23). After the enable gate and genuine no-input synchronization call,
it copies three real three-float vector objects to+12/+24/+36, negates the fourth
into up+60, calls matched one-pointer `1D5C0` to derive side and inverse matrix,
then sets gain+132 to level/127.0f and exits synchronization. The matrix helper's
actual view places side+48, up+60 and12 inverse floats+72..119, agreeing with this
source. Whole-object/self-copy vector semantics apply; arbitrary partially
overlapping object views are not covered. No fake floating argument is added to
change allocation.

`19AD4` accepts a packed ramp-state pointer and an unsigned target word. Flag
0x800 enables the update. It captures end+144 and start+140, computes the logical
low32 interval difference shifted8, and skips zero duration. Previous current+148
is retained for unsigned crossing checks. The step uses the low32 product of the
global rate and the signed interpretation of wrapped target-current shifted
arithmetically8, then signed division by the positive duration. It stores low32
of previous plus that quotient. A still-incomplete ramp returns the new current
and advances start by the global rate; otherwise start becomes captured end and
the original target is returned. All additions/subtractions/multiplication that
can wrap are unsigned; the cast/right-shift/division interpretation follows the
N64/IDO two's-complement and truncation convention, not a portable-C89 promise.
The divisor is in1..FFFFFF when used, excluding zero and INT_MIN/-1 traps.
The host independent wider-word model covers high-bit/wrap/crossing boundaries.

## Diagnosis and bounded source forms

Every first form used O2 then O1. All six initial residuals were diagnosed with
the unchanged workbench before refinement, and the improved free-list/ramp
checkpoints were diagnosed again before their final controls. Sanitized summaries
bind source hashes in `diagnosis.json` and `diagnosis_improved.json`; temporary
canonical listings/objects are not published. Strict relocated comparison is
authoritative over workbench relocation-layout heuristics.

- Three setters: initial24/65 each. A natural guarded valid-member block with an
  explicit stale-member return supplies the original loop/return structure and
  closes all three. Two complete source forms each, not synthesized callers.
- Free queue: a genuine named link pointer improves58/64 to30/64. Fresh diagnosis
  shows the remaining mask value copy and temporary/schedule fallout. A byte
  index control worsens to52/64 and is rejected. Three forms, then stop.
- Spatial wrapper: the first complete source is23/65; opcode geometry and frame
  agree, but ordinary aggregate copies use AT and different temporary registers.
  This is the already measured vector-copy pattern from the prior complete
  `1D0F0` residual. No repeated declaration/packing/volatile sweep is attempted.
- Ramp: real captured start/end locals improve67/68 to18/68. Fresh diagnosis
  isolates logical-shift value coalescing and later register fallout. Separating
  the low-word subtraction and logical shift leaves18/68, so it stops. Three
  forms; signed-overflow or arithmetic-duration shortcuts are not substituted.

Twelve final rows and fourteen rejected-control rows reproduce all retained and
archived sources. No fake formals, keepers, padding locals, assembly, callee bodies,
volatile tricks or target/flag changes are used. Unknown record spans represent
actual untouched storage, not stack-size manipulation.

## Semantic and repository verification

Six strict-C89 ASan/UBSan groups compile the actual sources. Each matching setter
has2,304 valid/stale-chain/channel/value/live-link cases plus a missing-key case.
Free-queue tests cover108 active/head/external/count combinations and a live
post-unlink identifier. The spatial wrapper has512 enable/byte-level cases plus
a disabled null-pointer gate. The ramp uses33,614 flag, interval, previous,
target and rate combinations, with a separate wider-word reference calculation.
Synthetic external helpers test caller contracts and mutation visibility; they
do not claim to implement original algorithms. LeakSanitizer alone is disabled
under ptrace. These tests do not turn the three residuals into matches.

```sh
python3 cloud/work/boot_tail/BT03-high-larger/verify.py
python3 cloud/work/boot_tail/BT03-high-larger/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-larger -p 'test_*.py' -v
```

Use the pinned shared compiler via IDO_DIR. Fresh setup/preflight verifies the
protected target manifests,439 exact extents/99,120 B and existing getter. Only
three matching submissions and this owned packet change. Protected paths,
central ledgers/D10, prior sources, targets/scorer, symbols/layout/locks, runtime
image/farm and restricted helper work remain unchanged. No ROM, native assembly
dumps, objects, credentials or unrelated private material is included. Paired
source/ABI review and exact aggregate CI precede independent-checker merging.

Independent paired review PASS at source commit
`6bab1d3bd150e8aa4dab156783253b30e865dd3b`, tree
`f87bc0585aa204682ae6f59e78b6afd3646b137b`. All twelve final rows, fourteen
archived controls, immutable source hashes and six sanitizer groups reproduced.
Live post-callback chain reads, post-unlink identifier use, six genuine spatial
inputs and low-word/signed-positive-divisor ramp arithmetic passed the actual
source/native review. The documented index/vector domains and rejected O1 HI16
failures remain explicit. `independent_review.json` binds three matching bodies
/780 B and three complete residuals /788 B; sources are unchanged.
