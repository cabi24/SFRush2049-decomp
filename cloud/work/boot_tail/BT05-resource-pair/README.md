# BT05 child-voice and sample-resource callers

**Two complete NONMATCHs, 912 B; zero matching bodies or verified bytes.**
Retained O2 scores are 95/113 and 103/115 words. Both complete natural sources
and their bounded controls remain research only.

Activation `ea50bee9` covers `80022160+452` and `80022874+460`. Branch
`dot/boot-tail-bt05-resource-pair` explicitly stacks on frozen reviewed timed
packet `6c4b7d31445ea485b913c485d7756463a91f249e`; the original master base is
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only this packet changes. No new clone
or compiler copy was created; previous sources remain untouched.

## Genuine native contracts

`22160` packs a real constructor word from the command, adds a signed-byte
interval to packed original note+0x4E, clamps it to0..127 and adds the external
bit when needed. It sets the actual active byte, invokes the constructor, clears
active and links a successful child into the existing parent/child chain. The
new child's parent uses the live parent identifier after construction. Existing
child links and the external-controller-copy condition are likewise read after
the call. The optional controller copy has two genuine state pointers.

The constructor `24988` has **eleven actual inputs**: a word, a u16, five u8
values, two u16 values and two u8 values. Its72-byte frame reads incoming slots
+16/+20/+24 as bytes, +28/+32 as halfwords and +36/+40 as bytes; the leading
register inputs are independently homed/read at the corresponding widths.
The zero tenth argument is genuinely consumed, not frame padding. The source
preserves the packed416-byte voice stride and observed fields, with no fabricated
local or formal. Registered constructor results and existing child IDs must
select allocated records. Arbitrary invalid indices, aliases or malformed chains
are not claimed safe.

`22874` calls `16CF0(u16, descriptor*)`, proceeding only on success. The actual
shared descriptor at `D_80056208` has six observed words at+0..20 and a byte at+24;
it is declared externally, without defining storage or changing its ownership.
The lookup writes those fields, and `146B4(word_index, descriptor*, u8)` reads
them. Its floating arithmetic does not create a float argument in this caller.
No fourth or later sample-helper argument is invented.

The handler selects direct, inverse-volume or volume-scaled offset, or zero for
other modes. Multiplication and subtraction retain native low32 unsigned
semantics; division is by127. It clamps offset against descriptor length and
calls playback with the real flag-controlled reset byte. It then reloads the
live descriptor words into state and sets flag0x20. Unlike the newer public
source family, native volume extraction is a full high word, without an extra
byte truncation. Zero length retains native unsigned `length-1` behavior; tests
of that arithmetic use a synthetic helper and do not assert real empty-sample
playback safe. Real calls require valid registered voice indices and resource
descriptors. No storage, local table, helper body or symbol is transplanted.

## Bounded diagnosis and results

O2 precedes O1. The unchanged workbench diagnosed both initial sources before
refinement using temporary relocated objects; no raw native listing is published.

- Child caller: initial110/113 and64-byte frame. A single meaningful packed-word
  expression improves to95/113 and restores the native72-byte frame. Reading the
  existing child index at full word width gives95/113 plus one extra; inlining
  the single-use descriptor returns110/113. The combined expression is retained.
- Sample caller: initial103/115 with the correct24-byte frame. Spelling the two
  mutually exclusive playback calls explicitly worsens to112/115. The original
  complete source is retained. Global descriptor access/allocation and argument
  lowering differ; matching is not forced by changing global-storage visibility.

Four/two source forms, then frozen. No declaration, register, K&R, artificial
frame object, padding, volatile, assembly or flag sweep. Native interfaces stay
unchanged. No matching submission is created.

| Function | Final O2 | Final O1 |
|---|---:|---:|
| 22160 |95/113|113/113 +39 extras; unpaired HI16|
| 22874 |103/115|109/115|

O2 rows have no nonzero extras or relocation uncertainty. The failed constructor
O1 control retains its unpaired `D_8004BEB8` HI16 error explicitly; it has no
matching credit. Initial and directed numerical receipts bind all controls.

Public CC0 [MusyX](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c)
provides PlayMacro/StartSample family context, not an authenticated N64 API or
layout. Revision `78d2e16e4905fc675952162d331c24d5198b2687`, source blob
`a3203f9e0032fa5e9d08fa5e51da80acf4cab0c0`, CC0 license blob
`0e259d42c996742e9e3cba14c677129b2c1b6311`. Native argument counts, fields, flags and
omitted modern operations remain authoritative.

## Verification

Two strict-C89 ASan/UBSan groups compile the actual retained sources. The child
protocol checks240 note/default/external/link/success cases, all eleven argument
widths, live constructor mutations and ordered controller copy. The sample group
checks1,280 mode/length/offset/volume/reset/failure cases, low-word arithmetic,
exact descriptor fields and live post-playback reloads. Helpers are synthetic
contracts, not original implementations. One fixture initially miscounted240
iterations as480; only that assertion was corrected, with no source change.
LeakSanitizer alone is disabled under ptrace; no heap is allocated.

Run `verify.py` and `python3 -m unittest discover` with this packet's `test_*.py`.
Use pinned setup or `IDO_DIR` for the existing verified compiler. Four final
rows/source hashes are bound in `verification.json`; preflight verifies protected
manifests, all439 extents/99,120 B and the existing getter. All161 static locks
and whitespace pass. No central ledger, target, compiler/scorer, symbol, layout,
lock, runtime image, farm, spec or production gate changes. Accepted800D1248 and
restricted helper work remain untouched. No ROM, raw assembly, object or private
data is published. Independent peer review precedes central integration; central
owns aggregate CI and the independent checker alone merges.

Independent paired review PASS binds source commit
`975139409292bf8fa18355d26eb02ceb46c6356b`, tree
`14b54bd281f3074009b953257c06f0ffb7151526`. Fresh immutable source copies reproduce
all four rows and both sanitizer groups. The complete native callers and three
resource helpers independently confirm all eleven constructor and three playback
inputs, descriptor fields through +24 and live post-call links/data. Valid-resource
and empty-sample qualifications remain explicit. `independent_review.json` records
912 bytes of complete nonmatching research with zero matching credit.
