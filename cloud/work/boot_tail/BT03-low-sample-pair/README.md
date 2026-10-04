# BT03 low sample descriptor and playback pair

Central claim `b5daa44a` assigns two fresh bodies: `800146B4` (488 B) and
`80016CF0` (336 B). Branch `dot/boot-tail-bt03-low-sample-pair` reuses the existing
isolated repository, explicitly stacked on frozen controller-pair head
`7d17ea9c94310e70bf393dcde84297b5fe01870c`. Only one named matching submission and
this owned packet change.

## Result

- `146B4`: **MATCH**, all122 relocated words /488 B, exact488-byte ELF function
  symbol, no masks, unresolved/unverified references, errors or nonzero excess.
- `16CF0`: **COMPLETE-NONMATCH**,80/84 words differ,332-byte ELF body versus336
  native. Zero nonzero section excess does not establish correct body extent.

One new exact body /488 B; one complete research nonmatch /336 B. Both O1 controls
are rejected. No cartridge, application-wide or maintainer acceptance is claimed.

## Actual ABI, layout and semantics

`146B4` takes a genuine word slot, packed descriptor pointer and byte reset flag.
It calls actual `11C1C(u16,u16)` with the narrowed slot and zero. That helper's
full source/ABI is independently available in the approved BT02-medium packet.
The subsequent indexed playback record still uses the original word slot. Valid
registered slots must address the runtime array and agree with the helper's
halfword index. No formal is narrowed merely to change compiler allocation.

The source uses the actual naturally aligned104-byte runtime record, including
an aligned double at+8, integer positions+16/+20, reset halfwords+32/+34, float+36,
steps halfword+40, sample pointer+44, length/loop words+48/+52/+56/+60 and format
byte+93. The global runtime-base pointer is read live after initialization and
between the native stores. Reset zero leaves the four reset fields unchanged;
nonzero reset writes0/0/1.0f/20. The unsigned offset converts exactly to double,
including offsets at or above80000000, then supplies both integer position fields.

The **chained assignment** is substantive source evidence: native code reads
that offset once for both integer fields. Writing the two C assignments
separately reloads the packed offset, adds two instructions and changes register
allocation. The ordinary chain preserves the single computed value and closes
the entire body without fake parameters or padding.

Tail arithmetic is unsigned low-word `length - loopLength - loopStart`; zero
loop length or tail below10 clears the tail. The sample pointer is also an opaque
native address/tag word. Its high nibble80000000 suppresses setting flag1; other
high nibbles set it. The source uses an explicit pointer-to-unsigned-long bit
view, a32-bit unsigned word under the verified N64 O32 ABI. This is target
pointer-representation evidence, not a claim of portable pointer tagging on
arbitrary ABIs. No sample-data pointer is dereferenced by this routine.

`16CF0` takes the actual halfword sample key and output descriptor pointer. It
writes the real key global, then iterates the **live** signed registered-bank
count. The independently approved C07 service/registry sources at `b5142352` and
`f05f9eeb` establish the28-byte `SampleRecord` and12-byte `RegisteredSamples`
layouts: identifier/references, offset, data pointer,16-byte descriptor; then
record pointer, base pointer, count halfword and final halfword. The descriptor
is modeled as its actual packed four-word payload rather than fabricated bytes.

The genuine five-input `1E864` contract is key pointer, record-array pointer,
count, record size28 and a real comparator pointer in old sp+16. The complete
C11-numeric reconstruction at `42c094cb` corroborates it. Actual `16CE0` reads
one unsigned halfword from each pointer and returns their signed integer
difference. No search or comparator implementation is inserted into the scored
source. Banks and records must remain valid allocated objects, registered bank
count0..8, and the original search's sorted-key/count requirements apply.

On success, the routine copies frequency and data pointer, sets offset zero,
extracts low24-bit length/high8-bit format, copies loop start/length, publishes
the exact payload and record pointers, and returns0. A failed visited bank
publishes a null found-record pointer; failure leaves the old payload pointer
and output unchanged. With zero banks, neither found pointer changes. These
native stores and the live count reload are retained, with no invented fallback.

Both routines use the genuine packed25-byte descriptor. Pointer-bearing sizes
are N64-only; LP64 host fixtures exercise logical members, not native offsetof
or sizeof. Tests do not dereference synthetic pointer-tag values or claim that
empty/malformed resources are safe for downstream hardware playback.

## Bounded diagnosis and controls

O2 precedes O1. Unchanged workbench diagnosis runs on initial and retained forms;
`diagnosis_summary.json` contains only hashes and classification counts. Native
listings and objects stay temporary.

- Playback starts91/122 with496-byte ELF extent. Its one directed shared-position
  assignment closes all122 words and the exact488-byte function extent. Frozen.
- Lookup starts80/84 with332-byte extent. A natural explicit trailing-payload
  address produces the same numerical result; the direct member form is retained.
  An earlier `offsetof` spelling failed because the pinned standalone compiler
  has no `stddef.h` include path. That source and failure are explicitly archived
  and replayed; no toolchain/header or scoring change was made. Two successful
  source forms and one compile failure, then stop on the known key/payload
  allocation difference. No fake ABI, opaque keeper or declaration sweep.

| Function | Retained O2 / ELF size | O1 control / ELF size |
|---|---:|---:|
|146B4|MATCH /488 B|120/122 +104 excess /920 B|
|16CF0|80/84 /332 B|83/84 +11 excess; unpaired HI16 /384 B|

The lookup O1 control retains its actual unpaired `D_800385A0` HI16 error and
never receives matching credit. Four final rows, eight scored control rows and
one known compile failure bind exact source hashes. Accepted bodies require one
correctly named STT_FUNC at zero, exact native function size and full relocated
word equality independently of zero section alignment. There is no padding,
assembly, volatile trick, flag sweep or target-boundary adjustment.

## Tests and gates

Two actual-source strict-C89 ASan/UBSan groups pass:

-17,280 playback combinations exercise all three real inputs, reset fields,
unsigned32-to-double conversion, wrapping tails, pointer-tag branches, unchanged
bytes and a synthetic initializer that switches the runtime base before return.
-270 search/result/payload patterns plus live bank-count growth/shrink cases
check all five helper inputs, the actual comparator, packed descriptor members,
visited-bank null publication and zero-bank/failed-output preservation.

Synthetic helpers express external contracts, not original algorithms. Only
LeakSanitizer is disabled under ptrace. Pointer-bit fixtures establish this
routine's tag logic, not real sample allocation or hardware safety. No heap is
allocated by the harnesses.

Run `verify.py`, `verify_controls.py`, and unittest discovery for `test_*.py`.
Use the existing pinned compiler through `IDO_DIR`. Fresh preflight validates
all439 target extents/99,120 B, protected manifests and the existing getter.
The canonical changed-submission gate,161 static locks and whitespace pass.
Shared ledgers/D10, earlier packet sources, targets/scorer/compiler/symbol/layout/
lock/spec inputs, runtime image/farm, production gates, accepted800D1248 and
restricted helper work remain unchanged. No ROM, raw native dump, object,
credential or unrelated private data is published. Independent paired review
precedes central integration and exact-head CI; the user's checker alone merges.

Independent paired review PASS binds source commit
`475faac7a10c4477c3ccf832ea1914f448ce0d54`, tree
`6d1add1fb9a3e39e1fe4a2dd82245bc67f09cb63`. Fresh immutable Git copies reproduce
all four final rows, eight controls, the explicit header failure and both
sanitizer groups. Independent IDO layout assertions verify25-byte SampleInfo,
104-byte AudioState,28-byte SampleRecord,12-byte bank and O32 pointer/unsigned-long
widths. Full native/source inspection confirms both real ABIs, shared offset
read, live runtime base, pointer-tag semantics and exact search/global publication
order. `independent_review.json` records one strict488 B body and one complete
336 B nonmatch. Matching source hashes remain frozen.
