# audio_bus_route: typed validation and current lookup context

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `audio_bus_route`, `[0x8009570C, 0x800957F8)`, **236 bytes / 59 words**.

Observed canonical O3 group result: **35 / 59 words differ**, zero nonzero
excess words, unresolved symbols, unverified relocations or comparison errors.
The earlier complete A158 form freshly scored 39 / 59. This is a modest research
improvement, not a match, independently verified result or accepted coverage.

The validation now reads the actual `Entry[handle & 255].handle` field through
its typed 24-byte record. The lookup declaration and call use the real accepted
32-bit key interface explicitly. All native invalid-handle/no-existing-node,
node allocation/removal, sentinel initialization, post-callback table reread,
queue submission and existing-node direct-write paths remain present.

The old A158 lookup body differed in all 20 words; `lookup.c` is the unchanged
current accepted `src/blob/func_800956BC.c` from the immutable base. Its historical
volatile-list read is disclosed by its original header, not newly introduced by
this candidate. This context now prints MATCH, with no new credit. Updating the
lookup alone left the caller at 39 / 59; the typed validation accounts for the
reported reduction to 35. Natural index/table/local-view alternatives did not
improve it further.

## Actual group and reproduction

`group.c` preserves the real A158 callers and the four-word queue-helper research
spelling published in PR #191. `lists.c` contains the real B132 insertion/removal
bodies. `lookup.c` is the accepted lookup above. All files/exports are declared
in `group.json`; only `audio_bus_route` is a research member and `claims` is empty.
Other callers and helpers are unclaimed context and must not replace accepted
production bodies wholesale.

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_bus_route_20261006
```

Run from the repository root with the documented IDO 5.3 toolchain. Exact group
flags are `-g0 -O3 -mips2 -G 0 -non_shared`; the unchanged backend supplies the
mandatory assembler `-r4300_mul` option. No target or scorer changes.

The remaining gap includes an eight-byte non-save frame deficit (64 versus 72),
slot/offset allocation and argument-home scheduling. No extra locals or padding
are added merely to fill it. The source assumes native O32 pointers/record views
and valid list storage; a pointer is converted explicitly to its actual 32-bit
lookup key. Arbitrary corruption/aliasing and gameplay behavior are untested.

This lean publication contains only source, required real context, compile
recipe and notes. No test harness, receipts, independent verification, full tests,
CI wait, image/ROM integration or merge was performed. Acceptance and merging
remain with the independent checker.
