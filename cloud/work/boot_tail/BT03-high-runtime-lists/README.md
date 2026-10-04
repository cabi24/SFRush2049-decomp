# BT03 high runtime and list packet

Central activation `ca63e26b` assigns six whole bodies / **2,252 B** on branch
`dot/boot-tail-bt03-high-runtime-lists`, from unchanged source master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. The existing sparse worktree is reused.
Prior chains head `d3dc7037` remains frozen on its own branch; prior files and
central ledgers are unchanged.

## Result

| Function | Bytes | Retained O2 | O1 control |
| --- | ---: | --- | --- |
| 800198C8 | 300 | MATCH | 72/75 |
| 8001D944 | 304 | COMPLETE-NONMATCH, 53/76 | 76/76 + 46 extras |
| 8001FBB0 | 380 | COMPLETE-NONMATCH, 66/95 + 2 extras | 95/95 + 21 extras; HI16 error |
| 8001DA74 | 404 | COMPLETE-NONMATCH, 58/101 | 101/101 + 51 extras |
| 8001C1D8 | 432 | COMPLETE-NONMATCH, 4/108 | 107/108 + 246 extras |
| 8001EF8C | 432 | COMPLETE-NONMATCH, 88/108 | 108/108 + 50 extras |

One first-form strict body / **300 B**; five complete research residuals / **1,952 B**
receive zero matching credit. Accepted198C8 has exact300-byte ELF function-symbol
extent, full relocated equality, zero masks/nonzero excess/unverified or unresolved
references/errors. Zero section-alignment padding is not body or extra match credit.
Rejected O1 HI16 failures in final or archived forms remain explicitly unaccepted.

## Actual ABI, layouts and bounded behavior

`198C8` is a no-argument scheduler for exactly eight4,088-byte sequence contexts.
A global word gate is read once. Active byte+4032 enables a record; the source
publishes its actual pointer and index, queries the real byte-selected1BDB8,
and invokes ten real no-input helpers in native order. The byte results of18634
and17D38 and low byte of actual int-return18184 are retained across intervening
calls. If all three are zero, the selected record's active byte is cleared and
inactive+4033 is set. Future active flags are read live; a helper's mutation of
the global context pointer does not change which record's final flags are stored.
Return widths were checked against actual native epilogues. No keeper or fake
helper argument is added to obtain the native frame.

`1D944` has an object pointer plus **one real float**, transferred from a1 to f12
by native code. It finds/creates the object's word-keyed group (object+56), then
inserts a12-byte small node into the group's small list, stopping only when the
new distance is strictly less than the current node distance. Equal distances
stay after existing equals. The pool/group counts are bytes. This routine does
**not** check capacity: valid configurations must keep group count0..32, and a
new group needs count<32; the small-node count must be<32 before every insertion.
All traversed nodes belong to valid allocated acyclic lists. The caller1DDE0
ignores its incidental v0 scratch value, corroborating the void source interface.

`1DA74` has the object pointer plus **five genuine float inputs**. Native moves
first/second floats from a1/a2, homes a3 at old sp+12 and loads the final floats
from old sp+16/+20. It returns a real zero/one capacity result. It rejects creation
of group33 and node33; importantly, creating a new group can happen before a full
large-node pool is discovered, and that side effect is not rolled back. The
nonempty large list preserves its existing head as an anchor: traversal starts
at that head and insertion is always after a visited node, never ahead of an
existing head. The source does not replace this with a conventional globally
sorted insertion. Its28-byte node stores next, five floats and the object pointer.

Both list routines share the genuine12-byte group view: word key, large pointer,
small pointer. Native addresses independently establish32 group entries atFDA0,
32 large entries atFF28 and32 small entries at502B0, with adjacent byte counts.
These pointer-bearing sizes are N64-only; LP64 host fixtures test logical behavior,
not native sizeof. Float comparisons assume the ordinary engine FP state. Tests
include IEEE quiet NaN/infinity values and preserve native ordered-less-than
behavior, but do not claim arbitrary FCSR exception-enable/rounding-state safety
or preservation of exceptional NaN payloads across every platform.

`1FBB0` is the genuine word-key/u16-value synchronized chain setter for controller
pair1/33. It uses the same416-byte voice layout, validated full identifiers,
registered low-byte slots, chosen channel field and post-call next-link reload
as the reviewed chain packets. The channel category is fixed before its two calls,
but that chosen field is read live on the second call. Every enabled sentinel or
stale exit invokes the real no-input release helper exactly once. The derived
byte component coalescing remains nonmatching; no formal is widened or invented.

`1C1D8` accepts one real configuration word. It sets two globals, calls19A60 with
actual word120/byte255, resets selected fields in32 packed416-byte voices and32
naturally aligned40-byte channel records, applies the native special channel
banks, invokes the real list initializers, clears a16-halfword table, then invokes
218CC. Complete native caller/callee widths agree. Unexamined bytes are preserved.
The two words at voice+64/+68 are represented as an actual naturally aligned
word-array slice: native uses aligned stores there, while adjacent packed word
fields use partial stores. The fixed base and416-byte stride keep that slice
aligned. This is local observed record modeling, not a shared layout change.
The halfword table is signed in the retained declaration, corroborated by the
actual2021C signed getter/setter view; zeroing is independent of signedness.
Eight special channels23..30 are expressed as an eight-entry relative bank,
which matches the native loop addresses. The final16-halfword clear still has
four rotated unrolled store sites and remains a complete NONMATCH.

`1EF8C` takes an actual voice pointer and byte channel. It captures the low-byte
slot and its4-byte link **before** possible unlink, preserving them even if the
helper changes the voice identifier. An already active entry on the same channel
returns immediately. Otherwise it uses live post-unlink head/group tables,
prepends the channel link and, when the group was empty, inserts that group's
4-byte halfword link into the ascending group list. The list domains require
registered slots0..31, byte neighbors0..31 orFF, groups0..255 orFFFF, and valid
allocated acyclic sorted group links. The loop's preceding node is defined by
the preceding head/order guard; no dummy initialization is added for padding.
The final voice channel is stored after list updates. No helper body is supplied.

## Diagnosis and bounded controls

O2 precedes O1. The unchanged workbench diagnoses every initial residual before
refinement, and the initializer/group-insertion improved checkpoints before their
last controls. `diagnosis*.json` stores summaries and source hashes only; native
listings and objects stay temporary. Strict relocated scores decide acceptance.

- Scheduler198C8 matches on its first complete source.
- Small insertion1D944: one captured group-count/named-node form and one actual
  count-snapshot form both worsen53/76 to74/76. Three forms, original retained.
- Large insertion1DA74: real named head and inserted-node pointers improve62/101
  to58/101. Two forms, then stop without artificial float or padding parameters.
- Initializer1C1D8: relative channel-bank indexing improves7/108 to4/108. An
  unsigned loop counter and the evidence-supported signed halfword declaration
  leave4/108. Four forms, then stop on the unrolled store rotation. No fictitious
  four-field grouping is introduced solely to force that scheduling.
- Group insertion1EF8C: direct assignment to the genuine byte slot improves
  101/108+2 to88/108 with no excess. Explicit loop-stop spelling worsens to102/108.
  Three forms, better complete source retained.
- Synchronized halfword setter1FBB0: first66/95+2 has the exact already diagnosed
  derived-byte normalization pattern from1B4A4/1B5F4. No redundant type/flag sweep.

Twelve final and sixteen archived-control rows bind all source hashes. There are
no fake formals, keepers, local padding objects, register tricks, assembly,
volatile changes, callee implementations, target edits or altered flag families.

## Semantic checks and repository gates

Six actual-source C89 ASan/UBSan groups pass. They cover:

- 2,048 scheduler active/result patterns, plus live future activation and disabled
  entry, enforcing all eleven calls and captured-record behavior.
- Small-list insertion across five group configurations, four list lengths and
  seven IEEE float fixtures, preserving equal/unordered comparison behavior.
- Large-list head anchoring, group/pool capacity failures and the real five-float
  values, including new-group creation before pool exhaustion.
- Complete byte-by-byte initializer results across all32 voices/channels and
  all untouched bytes, with genuine callback order and later helper mutations.
- 2,048 group/channel/unlink cases including all256 channel values, same-channel
  early return and a synthetic unlink that changes the voice identifier.
- 5,376 synchronized component-pair cases plus sentinel and disabled entry,
  including channel/flag mutations between calls and live chain changes.

Synthetic helpers define external contracts, not copied original algorithms.
Only LeakSanitizer is disabled under ptrace. FP and pointer-width qualifications
above remain part of these bounded tests; none grants matching credit to residuals.

```sh
python3 cloud/work/boot_tail/BT03-high-runtime-lists/verify.py
python3 cloud/work/boot_tail/BT03-high-runtime-lists/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-runtime-lists -p 'test_*.py' -v
```

Fresh preflight validates the immutable manifests,439 starts/99,120 B and getter
with the pinned shared IDO. Only the single matching submission and this owned
packet change. Sparse static-lock dependencies are restored unchanged from base.
Central ledger/D10, earlier packets, targets/scorer/compiler/symbols/layout/locks,
runtime image/farm and restricted helper work remain untouched. No ROM, raw
assembly dump, object, credential or unrelated private information is published.
Paired review and exact aggregate CI precede checker-owned merging.

Independent paired review PASS at source commit
`23961be1c7414440658b6334407c326f6c152994`, tree
`6e35043b26a4bc70e6674f632f328acf7e6c6dec`. All28 final/control rows and immutable
Git source hashes reproduced; accepted198C8 has exactly300-byte STT_FUNC extent.
All six sanitizer groups passed independently. Actual native/source review confirmed
the small-list capacity domain, large-list mandatory head anchor, genuine float
argument slots, initializer untouched bytes/aligned word slice and captured slot
with live post-unlink tables. The README's pointer-width and ordinary-FP-state
qualifications are retained. `independent_review.json` binds one match /300 B and
five complete residuals /1,952 B; no C source changed after review.
