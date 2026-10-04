# BT03 high medium-tail packet

Source base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
Central claim: `976c3e0d`; exact six bodies / 1,364 bytes.
Earlier packets remain frozen; central integration owns shared status and draft CI.

## Results

| Address | Bytes | O2 result | O1 control |
| --- | ---: | --- | --- |
| 80018C2C | 212 | MATCH | 52/53, 12 extras |
| 80018D40 | 236 | MATCH | 59/59, 11 extras |
| 8001E9B0 | 240 | MATCH | 56/60 |
| 8001EE9C | 240 | MATCH | 56/60, 16 extras |
| 8001D0F0 | 208 | COMPLETE-NONMATCH, 17/52 | 50/52, 4 extras |
| 800180A0 | 228 | COMPLETE-NONMATCH, 26/57 | 57/57 |

Four unique complete relocated matches / **928 B**. Two complete research
nonmatches / **436 B** receive no match credit. Every accepted O2 row has zero
extra words, masks, unresolved or unverified references and relocation errors.
Alignment zero padding is not part of a function body or additional match credit.
`1E9B0` starts exactly at the exclusive end of C11 and is outside its claim.

## Actual ABI, layout and domain

`18C2C` and `18D40` take one real word identifier. Actual `17644` returns either
FFFFFFFF or a validated row0..7 plus the original bit31 tag. The tagged branch
removes that tag before any subscripting; the untagged branch is already bounded.
Both records are naturally aligned, 4,088 bytes. Active/inactive bytes are at
+4032/+4033, flags at+4078 and pending word at+4080. Calls to actual one-pointer
`1734C` and `1729C` happen after clearing active. The former walks both linked
heads and the latter concatenates those heads into the free list and clears them.
The same selected row survives their calls; `18D40` writes inactive after the
callbacks regardless of changes to that field. Reusing the now-dead incoming
identifier to hold the real translated result is ordinary source, not a synthetic
ABI or padding variable.

`1E9B0` resets the count and used head, points the free head at a fixed32-node
array and links all nodes in both directions, preserving their key/value words.
The packed node has native next+0, previous+4, key+8 and value+12; size16 is proved
by the 32-bit IDO/native replay. The host test checks logical pointers and payloads,
not a false claim that an LP64 pointer-containing node has the N64 offsets.

`1EE9C` consumes an actual packed voice prefix: channel byte+46 and identifier
word+96. It unlinks the selected active channel entry from a byte-indexed list;
when it was the sole member, it also unlinks its group from a halfword-indexed
list. Channel entries are4 bytes and the allocated channel table has32 entries;
group heads and entries have256 slots. Valid registered channel identifiers have
low byte0..31, byte neighbors are allocated channels orFF, and group neighbors
are0..255 orFFFF. The source does not add malformed-list safety absent in native.
The target body uses packed loads for the voice identifier; the two link tables
are naturally aligned. No helper body or data table is fabricated.

`1D0F0` has an emitter pointer, two actual vector pointers and a byte level. It
checks the global enable byte, invokes genuine no-argument synchronization calls,
copies three-float position and velocity values to+12/+24, sets gain+40 to
level/127.0f and decreases current+44 only when the gain is lower. The unknown
word+36 remains untouched by this body. Vector sources are valid complete objects;
whole-object/self-copy semantics apply, not arbitrary partially overlapping views.
The current complete reconstruction is semantically tested but not a byte match.

`180A0` processes64 context tracks, each40 bytes, beginning+0x568. Any of the
three activity words+12/+16/+20 enables the update. It adds low32 fractions,
retains low16, interprets the wrapped sum as signed32 for arithmetic carry>>16,
and stores low32 of the whole-word sum. Unsigned additions deliberately avoid
signed-overflow UB. Unsigned-to-signed conversion and negative right shift use
IDO/N64 two's-complement conventions; strict C89 does not promise these values
on every implementation. The pointer-free track layout is also host-checked.
Native reloads the global context between field stores; source expressions do too.

## Bounded diagnosis and refinements

All six first forms were compiled at O2 then O1. The unchanged workbench diagnosed
all four initial residuals before refinement; sanitized summaries and source
hashes are in `diagnosis.json`. Temporary native listings and objects stay out of
this packet. Workbench object relocation-layout warnings are not strict proof;
full relocated comparisons decide every acceptance.

- `1E9B0` and `1EE9C`: first-form strict matches.
- `18C2C`: initial2/53 consists only of the saved pointer's stack-home displacement.
  A genuine named record pointer did not change it. Reusing the consumed input as
  the translated identifier closes it. Three source forms total.
- `18D40`: initial49/59 used a spilled index rather than native's saved register.
  An assignment guard and an ordinary local `register` declaration did not help.
  Reusing the consumed identifier closes it. Four forms total. No register-binding
  extension or fake incoming parameter is used.
- `1D0F0`: initial17/52 is the struct-copy temporary/constant scheduling region;
  an equivalent three-element-array vector representation and a real calculated
  gain local both leave17/52. Three forms; stopped without changing packing or ABI
  just to manipulate compiler temporaries.
- `180A0`: initial26/57 has exact opcode/control-flow geometry but the signed carry
  remains in a temporary instead of native's original value register, shifting
  the following temporary allocation. Explicit carry assignment does not help.
  Two forms; stopped rather than use signed-overflow arithmetic to force a match.

Twelve final rows and sixteen archived rejected-control rows bind every retained
source hash. No fake formal, keeper, padding local, assembly, volatile trick or
callee implementation is introduced. Unknown byte spans describe actual untouched
record storage. The eight rejected controls remain research only.

## Verification

Six strict-C89 ASan/UBSan groups exercise both tagged and untagged registry paths
across every validated row, sentinel rejection, callback mutations and final
inactive writes; complete32-node initialization and untouched payloads;432 list
unlink topologies/activity cases;2,560 emitter gate/gain cases; and125 complete
64-track contexts including wrap/high-bit/carry boundaries. Tests use actual
source files and synthetic external contracts. LeakSanitizer alone is disabled
under ptrace. Host tests supplement, rather than replace, native ABI/layout proof.

```sh
python3 cloud/work/boot_tail/BT03-high-medium-tail/verify.py
python3 cloud/work/boot_tail/BT03-high-medium-tail/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-medium-tail -p 'test_*.py' -v
```

Set IDO_DIR to the pinned shared compiler. Fresh setup/preflight verifies the
protected manifests, all439 starts/99,120 B and the existing getter. Only the four
matching submissions and this packet are changed. Sparse-checkout lock/storage
proof dependencies are restored from the unchanged base when needed. No central
ledger, D10, targets/scorer, symbols/layout/locks, runtime image/farm, forbidden
helper work, ROM, assembly dumps, objects or private material is changed/published.
Independent paired review and exact aggregate-head CI precede checker-owned merging.

Independent paired review PASS at source commit
`e222c1b06266c524712a5ec5fffee971d3e6da48`, tree
`56bad2656da88e870a10d551990991daa05c7eb2`. All twelve final rows, sixteen
rejected controls, immutable source hashes and six sanitizer groups reproduced.
Actual source/native ABI, tag bounds, callback ordering, N64-only pointer layout,
valid list domains and safe carry arithmetic passed. The four strict matches
remain 928 B and the two complete residuals remain 436 B. The source-bound review
is recorded in `independent_review.json`; source files were not changed.
