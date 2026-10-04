# BT03 voice allocation retry

**Local strict whole-function MATCH: one body / 276 bytes.** This reclassifies
a previously attempted complete nonmatch; it adds zero unique attempted functions.
It is source-matching evidence, not cartridge integration or ROM coverage.

Exclusive target: `func_8001ECE0`, native interval `8001ECE0–8001EDF4`.
Source base: `ef129c7c30c061e38e1675f188cfe6eadb0bd8ae`, on
`dot/boot-tail-bt03-voice-allocation-retry`. Central owns aggregate ledgers and
publication. Cut16 / draft PR76 and the entire earlier
[BT03-high-chains archive](../BT03-high-chains/README.md) remain immutable.
Independent source review is PASS; aggregate exact-head CI is still required.

## Two bounded, evidence-led repairs

The retained source and all five previous forms were inspected before editing.
Fresh stock IDO 5.3 O2 replay reproduces 8/69 differences and no excess words:
three traversal-copy scheduling words, followed by five allocation-load/branch
operands. All record field accesses retain the native packed layout.

1. The pinned [AxioDL/musyx vidMakeNew family source](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthvoice.c#L134)
   advances its global free head within an assignment-valued null test, reading
   next through that global. File blob `3ad906e217a82e77b649edc8935fd6c139d09415`
   was independently verified. The pinned repository [LICENSE](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
   is CC0-1.0, blob `0e259d42c996742e9e3cba14c677129b2c1b6311`,
   independently fetched by the reviewer. None of the old five forms used this topology;
   the old allocation-assignment control changed only the earlier pop guard.
   This one source delta removes all five allocation differences, leaving 3/69.
   The donor is a reference lead, not proof of the N64 ABI or compiler. Its
   newer second argument, master-list behavior and counter-wrap retry are absent
   from the native body and are deliberately not adopted. No donor file is vendored.
2. Unchanged workbench diagnosis and stock `-K` / `-Wa,-R` inspection identify the
   remaining pair of copies at separate statement lines. The assembler's ready
   nodes tie on time, critical path and latency. In the while form the preceding
   node assignment owns line32 and the packed-load address copy owns line33,
   so the preceding-node copy wins. One conventional for-loop traversal moves
   the existing next-node advance into the iteration clause. That clause now
   owns line28 and the body assignment line31. The packed-address copy wins,
   exactly matching the native order. The source operations, tests, locals and
   evaluation order are unchanged. Local initialization moves before the loop
   initializer, without any observable intervening operation.

No further candidate was tried after the second control. There is no manual
unrolling, pointer cast trick, artificial keeper, padding local, dummy input,
volatile qualifier, assembly insertion, flag sweep or compiler modification.
The stock traced and ordinary objects are byte-identical for both measured forms.
The verifier reproduces the exact source deltas and source-line ownership.

## Strict results

| Source | O2 full-word result | O2 ELF bytes | O1 rejected control | O1 ELF bytes |
| --- | --- | ---: | --- | ---: |
| Retained historical source | 8/69, zero excess | 276 | 69/69, 21 extra nonzero words | 368 |
| Global free-head assignment | 3/69, zero excess | 276 | 69/69, 22 extra nonzero words | 372 |
| Conventional for traversal | MATCH | 276 | 69/69, 21 extra nonzero words | 368 |

All six rows check relocations across the complete text section, with zero
masks, unresolved references, unverified sections or errors. The canonical
function has an exact 276-byte STT_FUNC extent and all 69 relocated words equal
the target. Only zero section-alignment padding follows the function; no
truncation or padding credit is used. O2/O1 each retain the established
`-g0 -mips2 -G 0 -non_shared -Wab,-r4300_mul` settings.

The workbench consumes a temporary literal-word target ELF, so its relocation
symbol warnings are diagnostic limitations, not evidence of a new source
reference. The unchanged strict scorer resolves every candidate reference to
its pinned native address, and its full-word result is authoritative.

## Native ABI and semantics

The actual callee reads one pointer saved from a0. Native callers 80019C8C,
80019F48 and 80024988 pass that voice pointer; there is no isMaster input read.
The function calls genuine no-input 8001EAEC, whose counter skips FFFFFFFF.
The native SequenceNode is packed16 bytes: next+0, previous+4, key+8, value+12.
The voice's node pointer is+24 and identifier+96. Nine IDO compile-time layout
checks establish the 32-bit layout; LP64 host tests are not native layout proof.

A key is consumed before testing pool availability. The scan stops at a larger
key, regenerates on an equal key, and remembers the preceding node. Allocation
advances the free list, clears its new head's backlink, connects both neighboring
used nodes, fills key/value and saves the new node in the supplied voice.
An empty pool returns FFFFFFFF while retaining consumed key-counter changes.
The for increment is skipped at the same explicit break as in the old while
form. All allocated lists and pointers must be valid, and traversed lists must
terminate. No new concurrency or malformed-list domain is claimed.

The final and intermediate source both pass the existing actual-source
108-case C89 ASan/UBSan fixture plus skipped-counter-sentinel case. A new
counter-wrap collision test explicitly preserves the native absence of the
newer donor's retry. It checks call count, counter state, exact neighboring
links, node contents and untouched voice bytes; it does not promise sorted
uniqueness across wrap. Fifteen separately compiled semantic mutations are all
rejected by the fixture's assertions. They cover key-consumption order,
collision/order handling, every used/free link, key/value stores, saved voice
node and return value. LeakSanitizer alone is disabled as in the frozen fixture.

## Reproduction

Use the pinned shared IDO environment, then run from the checkout root:

```sh
python3 cloud/work/boot_tail/BT03-voice-allocation-retry/verify.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-voice-allocation-retry -p 'test_*.py' -v
```

Input pins cover compiler files, scorer, manifest, extents, inventory, the
existing getter and the whole historical packet. Replay validates all439
starts /99,120 bytes, every boot-tail manifest entry and the getter. The packet
contains only reconstructed source, tests and source-free receipts. Temporary
raw native words/listings, pre-assembler output and objects are not committed.
No protected inputs, scorer, symbols, shared headers, layouts, locks, earlier
sources, restricted helpers or out-of-scope implementation bodies are changed.

Independent bounded review PASS binds source commit
`548a15fb7a835a0e74fc5d65ad799df3242cb38f`, tree
`837812c4393d7c22bed074216afec21aa7bfb74c`. The reviewer independently
reproduced all six rows, exact immutable-blob ELF/native/ABI checks, stock
trace ownership and packet tests. An additional810 list/wrap/whole-state cases
plus an exhausted-pool null-unused-state case passed for each of the historical,
intermediate and canonical forms. The source-bound receipt is
`independent_review.json`. This follow-up changes only documentation, provenance
and the review receipt; canonical/control/test hashes remain frozen. Aggregate
exact-head CI and the independent checker's merge decision remain central gates.
