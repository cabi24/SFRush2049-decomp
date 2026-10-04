# BT03-high: first bounded 64–127-byte packet

Two strict matching bodies, **140 B / 35 words**. Four complete NONMATCH
reconstructions, **328 B**, remain research only. Independent paired review passed; aggregate
publication-head CI remains required. No cartridge coverage or promotion is claimed.

- Branch `dot/boot-tail-bt03-high-medium`, fresh-master source base
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Six exact claims activated centrally at `a5e5948b`: `1785C+84`, `18AEC+80`,
  `19A60+72`, `1BDB8+92`, `1EAA0+76`, `1EDF4+64`; all addresses begin `800`.
- All targets lie in the assigned BT03-high partition, outside the whole C11
  interval. Both preceding BT03-high packets remain frozen. No shared status,
  claims, generated cluster/research file or D10 entry is edited here.
- This source branch has no dependency on unmerged research PR #59 or earlier
  matching drafts. Canonical manifests and the unchanged pinned compiler are
  shared with completed Packet 2; the existing getter and 439-start/size census
  are freshly checked, totaling 99,120 B. The packet edits no protected input.

## Matching packed-list pair

`8001EAA0` is a 76-byte sorted-list lookup. It reads the head `D_80050C50`,
compares a genuine unsigned key against packed node word +8, returns an equal
node, stops early if the query is smaller, and otherwise follows packed next
pointer +0. Missing keys return null.

`8001EDF4` is a 64-byte lookup/value wrapper. A -1 key bypasses lookup. Otherwise
it calls `8001EAA0` and returns packed node word +12, or -1 for no node. Its caller
`800201D0`, independently matched in the preceding packet, passes the word
identifier and checks the -1 result. Other native callers use the same one-word
input/result convention. No extra input or cross-function body is introduced.

Both sources use the same packed `SequenceNode` prefix: next at +0, unknown word
+4, unsigned key +8, signed result word +12. `#pragma pack(1)` is evidenced by the
native paired unaligned loads at these offsets, and is reset afterward. This is
a minimum accessed prefix, not a claim about the complete original object size.
The unknown member represents untouched object bytes, not a stack-padding local.
The only reconstructed callee is the separately matched pair member; the wrapper
contains an external declaration, not a callee definition.

Both initial natural forms matched at `-g0 -O2 -mips2 -G 0 -non_shared` plus
mandatory `-Wab,-r4300_mul`. Branch-likely list traversal, the wrapper's 24-byte
frame and packed accesses are all included in strict relocated equality. The O1 controls differ in 19/19 words plus eight extras for `1EAA0`,
and 16/16 plus four extras for `1EDF4`; they are not accepted. There
are zero extra words, unresolved symbols, unverified relocations or errors.
No switch table or local-rodata assumption is needed.

## Complete nonmatching research

| Function | Bytes | Frozen O2 residual | O1 control | Whole native operation |
|---|---:|---|---|---|
| `8001785C` | 84 | 17/21; 1 extra word | 21/21; 9 extra | Fill a 128-byte map with 255, then map 8-byte-record keys to wrapping byte indices until sentinel key 255 |
| `80018AEC` | 80 | 6/20 | 19/20; 4 extra | If enabled, add low increment, store low 16 bits, and add carry plus high increment |
| `80019A60` | 72 | 6/18 | 6/18 | Remap byte selector 255 to 8 and store unsigned scaled value in word table |
| `8001BDB8` | 92 | 15/23 | 23/23; 1 extra | Test three fields of a byte-selected 40-byte channel record |

`1785C`'s record prefix models the native eight-byte stride and key at +5. Its
byte index intentionally wraps. `18AEC` models observed 32-bit slots through the
global pointer, reloading that pointer after the low-word store. Arithmetic is
unsigned, including the carry. `19A60` retains the native unsigned overflow and
division semantics; algebraic cancellation is not used. `1BDB8`'s 40-byte object
layout contains only genuine accessed offsets and unknown object ranges.

These sources are full behavior reconstructions but are not submitted matches.
Original field names, middleware release and complete types remain unproved.
No external source body or unauthenticated arcade reference was copied.

## Bounded diagnostics

Initial O2 then O1 controls are in `initial_controls.json`. Before refinement,
`tools/workbench.py diagnose` ran on temporary objects built from unchanged
canonical words. The source-bound, timestamped `diagnosis.json` records this pre-refinement
step. Raw diagnostic words, assembly and objects are not published.
It found structural/register mixtures for `1785C`, `19A60`, `1BDB8`, and an
allocation/scheduling difference for `18AEC`; frame mismatch was not the issue.

Only three directed natural refinements were attempted, one per applicable
hypothesis, in `refinement_controls.json`:

- `1785C`: an explicitly byte-truncated update in a word-sized counter retained
  the same residual. Native reuses its index register; compiler adds temporary
  copies. Next: authenticate original narrow-local update lowering.
- `18AEC`: exchange commutative low-sum operands and reuse the meaningful sum
  variable for carry. Residual improved 7/20 to 6/20; this complete best source
  is archived. Next: recover authentic carry expression/type context for the
  remaining return-register and addition-order differences.
- `19A60`: separate the initial scale into a meaningful local. It worsened 6/18
  to 15/18. The original is retained. Next: recover both original narrow-formal
  coalescing and staged scaling context before further experiments.
- `1BDB8`: the whole condition already agrees, while narrow-formal normalization
  shifts homes and temporary registers. The same entry plateau was independently
  established in preceding BT03/C13 packets; redundant declaration sweeps were
  not repeated. Next: authentic compiler-context evidence for the byte formal.

No hypothesis approaches the twenty-variant bound. There is no permission to
add fake formals, keepers, redundant operations, inline assembly, padding locals,
or change the compiler/target/scorer to close a residual.

## Replay and integration

```sh
python3 cloud/work/boot_tail/BT03-high-medium/verify.py
```

`verification.json` binds twelve fresh controls to exact source hashes and strict
counts. Only two rows are matching submissions; the four archive sources remain
COMPLETE-NONMATCH. `status_delta.csv` is the central sole writer's integration
input. Publication/CI and merging remain the central lead/checker's work.

Only this packet directory and the two matching C files are changed. No ROM,
raw disassembly, object, secret, target, scorer, symbols, layout, lock, runtime
image, farm, forbidden helper work or production gate is changed or published.

## Independent paired review

The C13 reviewer independently reproduced all twelve O2/O1 rows on immutable
source commit `47243f5b`, confirmed both complete relocated bodies, and reviewed
the actual packed-node ABI and honest NONMATCH archives. Its receipt is
`independent_review.json`. Strict C89 host tests also passed 84 key/list queries,
including empty, duplicate, ordered, high-bit and sentinel cases. Host pointers
are 64-bit, so those tests cover behavior only; native +0/+8/+12 field offsets
are proven by the independent 32-bit IDO/native equality, not host offsetof.
