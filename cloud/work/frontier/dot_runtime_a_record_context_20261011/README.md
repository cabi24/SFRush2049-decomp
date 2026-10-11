# Image A record navigation: 444-byte complete match candidate

The only new claim is **A:func_8039E200**, `[0x8039E200,0x8039E3BC)`,
444 bytes / 111 words. Stock IDO 5.3 whole-group `-g0 -O3 -mips2 -G 0
-non_shared`, with canonical `as1 -r4300_mul`, reproduces the complete native
body with all relocation fields resolved and no excess bytes or uncertainty.
This is matching evidence, **zero accepted-image or cartridge coverage gain**.

The complete twelve-function source group retains the genuine A1EAC root.
It is an alternative to `dot_runtime_a_available_list_20261011`; do not install
both overlapping groups. Existing D494 (528), ED20 (480) and E3BC (136) remain
exact, with no duplicate credit. Root/caller and renderer bodies remain
nonmatching; inherited owned-literal failures remain explicit in the receipt.

## Grounded source/context recovery

`D_80152028` and `D_8015202C` are the first/last handle fields at +8/+12 of
`D_80152020`. Existing `cloud/matches/audio_volume_pan.c` already describes this
16-byte indirect-list object, and
`src/blob/groups/menu_cc040_20261004/group.c` independently uses its +8 head.
The native E200 body forms the shared base and accesses both fields.

Replacing the two unrelated externs with the real list object changes the
99/111-word, two-instruction-longer baseline to 1/111. Referencing
`(*node)->file`, rather than the equivalent cached record pointer before the
predicate call, closes the remaining address-register word. The same explicit
handle dereference after the predicate call is retained because that external
service may update the handle. These are source-form recoveries, not proof of
the historical spelling or original names.

The function searches the persistent linked record list and three cached
records, in either direction, accepting a matching track/mode record that is
not already selected, passes its external-file predicate if needed, and has a
nonzero field at +36. Zero direction has a distinct cached-last termination
case and is tested separately. The actual caller supplies signed-byte changes;
the machine entry normalizes the low byte of its private argument.

No dummy arguments, synthetic callers, padding, instruction encodings,
artificial volatile accesses, compiler patches or protected scorer changes.
The arcade reference checkout is absent in this cloud tree, so no arcade
ancestry claim is made. Native image and repository source are the evidence.

## ABI, data and complete-body verification

- E200 takes the record handle in **s1** and direction in **s2**, returning v0.
  It intentionally clobbers s0 through s6 and writes its argument home at entry
  sp+4. It is **not** a standalone O32 replacement.
- All three generated and all three native A1EAC callsites are audited for s1,
  s2, v0 and the caller's s5/s6 save/restore pairs. The large caller is not
  executed end-to-end and remains nonmatching.
- The verifier checks authenticated image A, loader pointer, full native extent,
  exact ELF STT_FUNC size and next boundary, full strict scorer result, and all
  15 E200 relocations via a fresh independent GNU as/ld route.
- GNU re-sections each complete fresh compiled function and retains relocation
  records, translating local call addends through real ELF function ownership.
  This is an extracted-body link, not original contiguous-TU placement proof.
- A separate stock IDO layout probe checks all used record/list offsets and
  sizes, including the 16-byte list, 44-byte record, 76-byte player, 64-byte slot
  and 52-byte panel record. E200 owns no literal or storage data.
- 6,546 protected-native cases, 6,546 fresh GNU-linked cases and 6,546 extracted
  unchanged C89 UBSan/bounds cases agree with an independent ordered-list model.
  All 111 instructions execute, covering 60 branch outcomes, every signed-byte
  direction and 798 external-callback handle-mutation cases. Full native/GNU
  memory, register, read/write and call traces agree; guards stay intact.
- The real external predicate is stubbed with deterministic return values,
  optional handle rewrites and arbitrary O32 volatile-register clobbers. Its
  own implementation and full game behavior are not established by these tests.
- Four compiled controls reject the old source, cached-pointer spelling, wrong
  zero-direction boundary and wrong tail field. The decoder rejects unknown
  opcodes/unmapped memory. The verifier explicitly rejects optimized Python
  because its inherited independent ELF/GNU helper uses assertions.

## D05C cleanup research, no match claim

The complete 676-byte cleanup body improved from **121/169 to 18/169** differing
words. Directly indexing 52-byte records, separating two halfword stores,
using explicit fixed-bound inner-loop termination and an early continue,
and naming the actual reloaded first-loop player count recover the exact
instruction geometry. Its native frame remains 24 bytes. The remaining 18
words are one consistent permutation of four allocated registers, with zero
unresolved/unverified relocations, errors or excess words.

`reproduce_research.py` replays 25 distinct source forms, including the baseline
and the list-field and navigation stages. Attempts were chosen from observed structure,
reload, index-width and allocation evidence. Named bound/selected temporaries,
index width, separate counters and equivalent loop forms did not close the
remaining coloring; some regressed and are not in the retained source.
No blind permutation sweep or fake pressure statement was used.

Workbench diagnosis ran before source experiments and again after geometry
recovery. Its raw-object relocation warnings are not matching evidence.
Stock uopt listing mode hit the supplied binary's `wrapper_ecvt` assertion.
Stock ugen tree diagnostics succeeded; E200/D05C emitted bodies stayed identical
to the canonical build, but renderer bytes changed, so diagnostic whole-object
fidelity is not claimed and those objects are never used for match verification.
The diagnostic tree confirms that player, two loop limits and signed-byte
player copy arrive at ugen already assigned to the four differing registers.
The next useful step is authentic TU/allocator evidence rather than additional
unconstrained spelling tests.

## Reproduce and scope

Configure `IDO_DIR` and GNU MIPS tools, then from repository root:

```
python3 cloud/work/frontier/dot_runtime_a_record_context_20261011/verify.py
python3 cloud/work/frontier/dot_runtime_a_record_context_20261011/reproduce_research.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_record_context.py -q
```

Baseline: reviewed `4296906ea06f551540a29a2547ffdffe92e92104`. Existing merged
claims were searched; no earlier E200 claim appears in the cloud group set.
Pending PR #163's known A claims are disjoint. Final peer review and publication
are coordinated separately; the independent checker owns acceptance/merging.
No source-image, full-ROM SHA-1, compressed stream, splice, hardware or gameplay
claim. No ROM bytes, raw assembly dumps or private credentials are committed.

UTC phase observations: preflight began 13:35; E200 first exact at 13:37;
D05C reached 18 words at 13:40; full native/GNU/host E200 proof passed 13:46.
Fresh author verification took about 8 seconds; recorded per-build timings in
`research.json` are replay wall times, not compiler-performance benchmarks.

Author focused regression: all 11 tests across the new six-test packet and the
existing five-test available-list packet passed with REQUIRE_TOOLCHAIN=1. The
new packet was rerun after isolating its semantic-module import; all six passed.
The combined full suite and independent peer review are owned by integration.
