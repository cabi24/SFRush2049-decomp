# BT03-high: voice-record control helpers

Four strict O2 matches, **392 B / 98 words**. Two complete NONMATCHs, **176 B**,
remain research only. Independent paired review passed; aggregate exact-head CI remains
required. No cartridge coverage or promotion is claimed.

- Branch `dot/boot-tail-bt03-high-control`, fresh-master source base
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central activation `da3681ed`: `18B8C+96`, `18BEC+64`, `18EB4+108`,
  `193C8+88`, `19420+112`, `199F4+100`, all prefixed `0x800`.
- These remain in BT03-high, outside C11. Earlier packets are frozen. No central
  generated status, claims or D10 file is changed. No source dependency on earlier
  matching drafts or unmerged research is introduced.
- Protected manifests, all 439 starts/sizes (99,120 B), and the existing getter
  replay pass with the unchanged pinned compiler. No protected input is repaired.

## Shared record evidence

All sources consistently model records rooted at `D_80043EB8`, with **4088-byte
stride**. Native index arithmetic multiplies by 511 then8; the initializer's
base/end pair and loop cover exactly eight such records. Its stores identify
bytes +4032/+4033. Other members access byte +4036, byte +4037 and unsigned
halfword +4038. The setter's byte-selector window starts at +1320; its genuine
byte selector gives the modeled 256-byte addressable window, without asserting
that every selector has a meaningful public API role.

`VoiceState` uses unknown object-byte ranges for the intervening storage. These
are data-layout gaps, not stack padding or invented live locals. The resulting
size is 4088 and all accessed offsets agree with the native words. Full original
type names, the unmodeled fields and middleware release remain unknown; no
production/shared header or symbol map is changed.

## Matching bodies

| Function | Bytes | Native operation |
|---|---:|---|
| `80018B8C` | 96 | When enabled, translate the word identifier, reject -1 and a set high tag bit, return halfword +4038 or zero |
| `80018EB4` | 108 | Same identifier/tag guards; store logical-not of the genuine byte argument at +4037, return one or zero |
| `800193C8` | 88 | When enabled and translated index is not -1, return byte +4036; otherwise zero |
| `800199F4` | 100 | Initialize eight records' bytes +4032/+4033 to0/1, then call `800171C0` and `800175A8` |

The translator `80017644` takes a real identifier word and returns an index/tag
word or all-ones. The two guarded APIs expressly test the high bit. The byte getter and archived
selector setter intentionally accept tagged rows, as the native bodies do.
Peer review rejected their initial direct tagged-array subscripts: those are
out of range in abstract C, even though the original 32-bit compiled addresses
happened to compare exactly. The final sources instead compute the byte offset
using unsigned 32-bit multiplication/addition, then convert the resulting address
back to a record pointer under the N64 32-bit pointer ABI. This arithmetic is
defined modulo 2^32: `(tag | row) * 4088` loses bit 31 and equals `row * 4088` for
every actual translator result (validated row 0..7 plus optional bit 31).
All 16 row/tag cases were independently checked arithmetically. The recovered
address is in the eight-record object; no out-of-range C pointer arithmetic is
performed. Pointer/integer conversion relies on the actual N64 ABI, not a wider
host. The getter remains a strict match and the setter remains 14/28 NONMATCH.
The initializer's two actual callees use globals and need no incoming argument.
`175A8` is the separately matched word clearer; no callee definition is inserted.

`193C8` and `199F4` matched their first natural sources. The latter's ordinary
8-iteration loop lets IDO emit the native fourfold store grouping and two loop
iterations, with no manual padding or unroll directive.

## Directed controls and nonmatches

Initial O2 followed by O1 controls are in `initial_controls.json`. Before every
non-exact refinement, unmodified workbench diagnosis ran against temporary
canonical-word targets and initial candidates. `diagnosis.json` binds each
source hash, flags, exact strict residual, timestamp and verdict; raw words,
assembly and objects are not published.

- `18B8C` initially differed in 7/24 words and `18EB4` in 10/27. A signed-index
  comparison reproduced value behavior but changed branch-likely selection and
  omitted the native high-bit-test instruction form. One genuine source change
  per function, testing `(index & 0x80000000) == 0`, produced strict whole-body
  MATCHes. This expresses the observed tag-bit guard; it is not an artificial
  shift or instruction insertion.
- `18BEC` is a full enabled/valid-identifier boolean wrapper. O2 remains 3/16,
  diagnosed as register-only; O1 is 11/16 plus two extras. One meaningful result
  local retained the same residual and was rejected. The natural initial source
  is archived. Next: authentic boolean-result coalescing context, not keepers.
- `19420` is a full identifier lookup followed by a byte-selector write and
  `1C19C(channel,2)`. O2 remains 14/28; O1 is 25/28 plus two extras. Native narrows
  its real byte formal into its original argument register; current output uses
  a temporary and shifts the surrounding allocation. This same bounded plateau
  was already established in BT03/C13 work, so ineffective declaration sweeps
  were not repeated. Next: original narrow-formal/argument-lowering evidence.

Peer-review domain repair also tested an explicit high-bit removal in both
unguarded members. Those defined array-subscript forms worsened the getter to
14/22 plus two extras and the setter to 24/28 plus two extras. The final unsigned
word-address form matches the actual 32-bit addressing semantics and restores
0/22 and14/28 respectively. No keeper or invented input is involved. Historical
raw-index seed results are explicitly marked rejected for source semantics in
`initial_controls.json`; score zero alone was not accepted.

No member used more than three natural forms. The original helper ABI, exact
boundaries and flag levels were preserved. No fake input, padding local,
keeper, dummy call, volatile trick, inline assembly or protected-tool change
was used. Both archives remain COMPLETE-NONMATCH with no matching credit.

## Flags and verification

Accepted headers use `-g0 -O2 -mips2 -G 0 -non_shared`; the scorer adds
`-Wab,-r4300_mul`. The final O1 controls differ in 20/24+3 extras (`18B8C`),
25/27+7 (`18EB4`),18/22+2 (`193C8`) and24/25+4 (`199F4`). Native branch forms,
allocation and loop grouping support O2 for these members.

`verification.json` binds twelve final O2/O1 rows to source SHA-256 hashes.
Four accepted bodies give 98 fully relocated words with zero differences,
nonzero extras, unresolved symbols, unverified relocations or errors. No local
rodata, switch table, external source body or unauthenticated arcade identity
is used.

```sh
python3 cloud/work/boot_tail/BT03-high-control/verify.py
```

`status_delta.csv` is for the sole central writer. Only this packet directory and
four submission sources change. No target, scorer/compiler, symbol, lock,
layout, runtime image, farm, forbidden helper, ROM/raw instruction dump, object,
credential or production gate is modified or published. Independent review and
exact-head CI precede checker-owned merging.

## Independent review

The BT05/BT07 reviewer replayed all twelve rows and final source hashes at
`ff0b887e`, reviewed all complete source/native operations and layout offsets,
and independently validated the semantic repair for tagged rows. All sixteen
row/tag combinations recover the exact in-bounds record, and all 4,096 selector
addresses are within those records. `independent_review.json` explicitly limits
pointer/integer conversion to the N64 32-bit ABI. It approves four strict bodies
and two honest nonmatches, with the earlier raw-subscript source rejected.
