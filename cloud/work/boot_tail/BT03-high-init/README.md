# BT03-high initialization and reset packet

Four strict O2 matches, **416 B / 104 words**, and two complete nonmatches,
**200 B**. Claimed exclusively at central `fc53ae59`, based on unchanged master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only the six named targets and this
packet are edited; preceding sources and central ledgers stay frozen.

| Function | Bytes | Final O2 | Final O1 |
|---|---:|---|---|
| 8001C508 | 120 | MATCH | 30/30 + 1 extra |
| 8001EE34 | 104 | MATCH | 26/26 + 8 extra |
| 8001F7EC | 120 | MATCH | 30/30 + 21 extra |
| 8001F954 | 124 | 2/31 | 30/31 + 1 extra |
| 8001F9D0 | 72 | MATCH | 15/18 + 2 extra |
| 80020598 | 76 | 16/19 | 16/19 |

## Complete behavior and ABI

- `1C508` walks 24-byte sample records, calls real two-input `14C60` only for
  mode one, forwarding the pointer at +8 and full-word sample count at +12.
  The global count is live across each call; the source does not cache it.
  The host has wider pointers, so this native pointer layout is established by
  IDO equality, not by host sizeof. The callee sequentially copies four samples
  and flushes eight bytes; no implementation is inserted here.
- `1EE34` clears halfword +2 in each configured four-byte channel record,
  fills the 256-byte map with FF and sets the final halfword sentinel to FFFF.
  All unexamined halfwords are preserved.
- `1F7EC` builds four-byte previous/next/flags entries and head/tail bytes.
  Its intended configuration domain is **1 through 32 channels**. The native
  body itself writes before the link table if the count is zero; the source is
  not offered as a safe implementation for zero or malformed counts. The tests
  cover every supported count and preserve the unused tail. The table extent
  follows the native neighboring globals at 50440 and 504C0. This is a declared
  native configuration contract, not an independently proven universal caller
  precondition.
- `1F954` preserves the -1 sentinel, queries the actual byte-valued predicate
  `1467C`, optionally invokes `14AF0`, sets the packed 416-byte state's full
  identifier at +96, calls `1F6EC`, then clears +189. Valid slot indices are
  assumed. The genuine predicate computes exactly zero or one; the retained
  declaration returns u8. The two displaced pointer-home words remain NONMATCH.
- `1F9D0` forwards one real state pointer to `1EB10`, then clears flag bits0/1
  at packed +36 and the full word at +40 before forwarding it to `1F6EC`.
  The first callee can mutate these fields, so their post-call loads matter.
- `20598` copies a real 32-byte callback record. Its opaque eight-word view
  preserves all bits without inventing callable signatures. Actual consumers
  use indirect calls at these eight slots; exact signature reconstruction is
  outside this copy routine. Aggregate-copy temporaries remain NONMATCH.

Both state sources share the same packed 416-byte view. Unknown byte arrays
represent actual unexamined object ranges, never stack padding. All callees
remain real external declarations; no fake argument, keeper, padding local,
volatile trick, forced register, inserted assembly or callee body is used.

## Bounded diagnosis and refinement

`diagnosis.json` records the workbench diagnosis before any refinement. The
workbench reports raw object relocation-layout differences too; strict relocated
scoring remains authoritative. Temporary native dumps/objects are not committed.

- `1C508` and `1F9D0`: first-form O2 match.
- `1EE34`: initial four-byte-record fill unrolled four records per iteration.
  Pointer iteration reduced it to a register/order residual; an inequality
  control retained unrolling. The natural flat 256-byte fill gives the native
  four-byte unroll and exact equality. Four forms total.
- `1F7EC`: explicit cached count unrolled the loop and produced an unresolved
  HI16 in the rejected O2 object. A pointer/while spelling also unrolled.
  Keeping the global bound in ordinary source gives the native single-record
  loop and exact equality. Three forms total; final relocation proof is clean.
- `1F954`: initial two-word residual is solely a pointer stack-home displacement,
  sp+24 versus native sp+28, with the same 32-byte frame. A meaningful predicate
  local shifts that home but expands the frame, so it is rejected. Inner scope,
  explicit index copy and genuine byte predicate-local controls do not close it.
  Final exact byte-return declaration retains the best 2/31. Six forms total,
  then stop; no artificial local was added to force a frame.
- `20598`: aggregate copy, real destination pointer, function-pointer opaque
  representation, explicit member copy and ordinary memcpy controls do not close
  the temporary-register residual. The uniform function-pointer representation
  was only a rejected copy experiment, not verified callback signatures. The
  simple opaque-word aggregate copy is retained. Five forms total, then stop.

`verification.json` binds twelve final O2/O1 rows. `controls_verification.json`
separately binds the rejected controls, including honest unresolved evidence;
none receives match credit. No control broadens an accepted function boundary.
All accepted bodies have full relocated equality and zero nonzero excess,
unresolved symbols, unverified references or errors.

## Reproduction and bounds

```sh
python3 cloud/work/boot_tail/BT03-high-init/verify.py
python3 cloud/work/boot_tail/BT03-high-init/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-init -p 'test_*.py' -v
```

Fresh setup/manifests, all 439 extents / 99,120 B and the existing getter pass.
Six C89 ASan/UBSan tests cover valid sample records including live count mutation,
all 0..32 reset counts, all 1..32 link counts, packed state layout and external
call ordering, valid/sentinel slot dispatch, and ordinary/self callback copies.
Test helpers are synthetic contracts, not original implementations. No malformed
resource or cartridge behavior is claimed. LeakSanitizer alone is disabled under
ptrace; these harnesses allocate no heap. Matching source/native ABI review and
exact aggregate-head CI precede checker-owned merging.

Independent paired review PASS at source checkpoint
`72c07a3cfe74946bcba43e469f7ae988ae214ea9`, tree
`f5d4c105ef1704d84874ec5e7cac5ff3fbab0c11`. The reviewer reproduced all twelve
final rows and thirty rejected controls from a separate checkout, ran all six
sanitizer tests, audited actual sources/callees, and additionally checked live
count growth/future mode mutation and full untouched-tail preservation.
`independent_review.json` binds this four-match / 416-byte result to exact hashes.
