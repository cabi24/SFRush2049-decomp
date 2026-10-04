# BT03 high chain and buffer callers

Activation `16763d4b` covers six fresh whole bodies / **1,864 B** on branch
`dot/boot-tail-bt03-high-chains`, from master source base
`301d9e7552ad4fd7f54a38796db84671e1000d35`. The existing sparse worktree is reused;
prior larger head `41085407` remains frozen on its own branch. Central integration
alone owns the shared ledger, aggregate draft and CI.

## Strict results

| Function | Bytes | O2 | O1 control |
| --- | ---: | --- | --- |
| 8001FD2C | 300 | MATCH | 75/75 + 9 extras |
| 80020048 | 300 | MATCH | 75/75 + 9 extras |
| 8001C3CC | 316 | MATCH | 78/79 + 42 extras |
| 8001ECE0 | 276 | COMPLETE-NONMATCH, 8/69 | 69/69 + 21 extras |
| 8001B4A4 | 336 | COMPLETE-NONMATCH, 61/84 + 2 extras | 82/84 + 25 extras |
| 8001B5F4 | 336 | COMPLETE-NONMATCH, 61/84 + 2 extras | 82/84 + 25 extras |

Three first-form relocated matches / **916 B**, with zero masks, unverified or
unresolved references, errors or nonzero excess words. Three complete research
nonmatches / **948 B** receive no matching credit. Semantic tests and an8-word
near-match do not authorize masking differences or changing strict acceptance.

## Actual source and ABI

`1FD2C` and `20048` are synchronized chain setters for commands91 and64. They
check the global enabled byte once, enter the actual no-input synchronization
helper, look up the word key using `1EDF4`, and walk registered416-byte voice
records. Each full identifier is checked against packed+96. A stale member exits
synchronization before returning the accumulated result; a sentinel-ended chain
also exits once. Valid members invoke the real four-byte-input `20610`, choosing
channel+85 or+75 from flag2 at+36, then reread the next identifier+16 after the
call. Those calls do not use fabricated arguments or helper implementations.
Valid low-byte slots must address the allocated voice array and valid chains must
terminate. Callback changes to links are visible; later enable changes do not
change the already-entered synchronization scope.

`1B4A4` and `1B5F4` use the same genuine key plus a **u16** value, sending two
controller components with command pairs132/133 and128/129. The first is the low
byte of value>>7, the second value&127. Bit15 is discarded by the actual byte
argument conversion. A real byte slot local is reused by both calls. The channel
category is chosen once before the pair, but the chosen channel field is read
again for the second call; changing flags during the first call does not change
which field the second call uses. The next identifier is reread after both calls.
These complete bodies still have narrow derived-value coalescing differences;
no narrower/wider fake formal is introduced to hide them.

`1C3CC` consumes no inputs. It iterates the live configured voice count and only
mode-one sample buffers. The naturally aligned native descriptor is24 bytes:
mode+0, callback pointer+4, short-buffer pointer+8, sample capacity+12, last
position+16 and an opaque word context+20. The genuine callback takes five inputs:
first short span pointer/count, optional wrapped second span pointer/count, and
that context word at incoming sp+16. This is a caller-visible word contract, not
an invented callee implementation or pointer meaning for the opaque token.
Actual `14C18(int)` reads the current word position from the selected104-byte
runtime record. Actual `14C40(short*,u32)` forwards the buffer and doubled byte
count to the cache operation. No hidden floating argument exists.

For forward progress, the callback sees old..new; on wrap it sees old..capacity
and0..new. A true callback result flushes those regions using the **live** buffer,
old-position and capacity fields after the callback. The saved new position is
then installed. The second wrap flush reloads the base after the first helper.
The global count and later modes are read live. Configured count remains within
the allocated descriptor array, positions/capacities remain within allocated
short buffers, and callback mutations preserve those bounds. Pointer/function
pointer widths make the24-byte layout native-only evidence; host semantic tests
do not assert LP64 descriptor offsets equal N64 offsets.

`1ECE0` obtains a key from genuine no-argument `1EAEC`, scans the used packed node
list, advances the key again on collision and remembers the preceding node. It
then pops the free head, fixes both directions of the used/free links, stores key
and the live voice identifier, and installs the node pointer in voice+24. If the
free list is empty it returns FFFFFFFF without modifying the used list, while the
already-consumed key generation remains visible. The node is native16 bytes
(next+0, previous+4, key+8, value+12); voice identifier is native+96. Host pointer
widths differ and are not layout proof. Valid allocated lists and the key helper's
actual skip-FFFFFFFF counter contract are retained. The implementation's local
ordering behavior is reconstructed; global uniqueness/order under counter wrap
or malformed/cyclic lists is not newly promised.

## Diagnosis and bounded controls

O2 precedes O1. The unchanged workbench diagnosed each initial residual before
refinement, and repeated diagnosis at the improved8/69 node-list checkpoint.
Metadata and source hashes are in `diagnosis*.json`; native listings and objects
remain temporary and are not published. Strict relocated scoring is authoritative.

- The two synchronized setters and buffer callback loop match on their first forms.
- `1ECE0` starts52/69 plus4 excess words. Moving the key-order stop into an
  explicit loop break reduces it to8/69 and zero excess. The remaining three
  pointer-copy schedule words and five allocation-load/branch operands remain.
  A guarded allocation block worsens to37/69; a natural linked assignment and an
  allocation assignment guard both remain8/69. Five total source forms, then stop.
- `1B4A4` starts61/84 plus2 excess; a real named high component and arithmetic-width
  low component worsen to62/84 plus2. The original is retained. The equivalent
  `1B5F4` seed has the exact same measured lowering pattern, so no redundant type
  or declaration sweep is applied. These are two forms and one form respectively.

Twelve final rows and ten rejected-control rows are source-hash bound. Unknown
record byte arrays describe actual untouched fields. No fake formal, keeper,
local padding, assembly, volatile trick, artificial callback or flag sweep is used.

## Tests and gates

Six actual-source strict-C89 ASan/UBSan groups pass. Each synchronized setter
checks2,304 valid/stale/channel/value/live-link cases plus missing-key and disabled
guards. Each halfword pair checks5,376 cases plus a missing key, including channel
and flag mutations between its two real calls. Node insertion covers108 list/key/
free-pool combinations plus the helper's skipped-FFFFFFFF counter case. Buffer
callbacks cover324 old/new/result/mutation combinations plus live count growth,
future-mode mutation and an empty count. Synthetic helpers express external
contracts, not original algorithms; LeakSanitizer alone is disabled under ptrace.

```sh
python3 cloud/work/boot_tail/BT03-high-chains/verify.py
python3 cloud/work/boot_tail/BT03-high-chains/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-chains -p 'test_*.py' -v
```

Use the pinned shared IDO via IDO_DIR. Fresh preflight verifies all target
manifests,439 starts/99,120 B and the existing getter. Only the three submissions
and this owned packet change. All baseline lock dependencies are materialized
unchanged for the sparse checkout. Central ledgers/D10, prior sources, protected
inputs/scorer/symbols/layout/locks, runtime image/farm and forbidden helpers remain
untouched. No ROM, native dumps, objects, credentials or unrelated private data is
included. Independent source/ABI review and exact aggregate CI precede merging by
the user's independent checker.

Independent paired review PASS at source commit
`05958acb4586943b3d73514992dd6f3feec952d9`, tree
`e61443581004f75169782e0a3b1c81e4f364e250`. All22 final/control rows and exact
Git source hashes reproduced, and all six sanitizer groups passed independently.
Actual source/native review confirmed synchronized early/stale exits, live links,
fixed channel selection with second-call reload, the real five-input callback and
live descriptor/count behavior, and qualified N64 node layouts. The source-bound
`independent_review.json` records three matches /916 B and three complete residuals
/948 B. No source changed after this review.
