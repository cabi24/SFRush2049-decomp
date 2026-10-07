# audio_fade_control: four-word research residual

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `audio_fade_control`, `[0x800953CC, 0x800954A0)`, **212 bytes / 53 words**.

Observed canonical O3 group output:

```text
audio_fade_control:
  4/53 words differ
```

Zero nonzero excess words, unresolved symbols, unverified relocations or errors.
This is **NONMATCH research**, with no matching or accepted cartridge claim.
The archived A158 source freshly scored 44 / 53 with the same real list bodies.
Capturing the existing entry pointer before the kind guard reduces that to 4 / 53.
The entry reload after the two list callbacks is preserved. Removing that reload
is not the submitted source or a valid improvement.

All four residual words are at the entry: retail materializes the constant 2 in
an argument register and loads the entry only after the kind tests; the candidate
loads the entry earlier and materializes 2 in the assembler scratch register.
The remaining body, frame and extent compare exactly under the canonical scorer.
Ten subsequent natural type/operand/condition spellings did not move this residual.
No padding, extra inputs, unused storage, artificial volatile or assembly is used.

## Actual source and context

`group.c` starts from the complete genuine
`cloud/work/ipa-groups/codex_audio_value_a158/group.c` at the base above. Only the
cleanup's entry-binding spelling is changed. Its six callers remain unchanged:
`gfx_setup_e700`, `audio_timing_sync`, `audio_bus_route`, `sound_priority_set`,
`audio_stream_control`, `audio_buffer_manage`, plus their actual `func_800956BC`
lookup helper. These are unclaimed context, not synthetic callers.

`lists.c` supplies the real `func_80091FBC` insertion and `func_8009211C` removal
bodies from the base's `codex_effect_typed_b132/group.c`. Adding those bodies alone
left the old 44-word residual unchanged; their successful context scores earn no
new claim. `group.json` records the actual files, exported roots and empty claims.
It is not a proposed whole-group production replacement.

## Reproduce

From the repository root, with the documented IDO 5.3 toolchain:

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_fade_control_20261006
```

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`; mandatory `-r4300_mul` is added by the
unchanged canonical group backend. The list file's historic header includes the
corresponding `-Wab,-r4300_mul` spelling; the group recipe is the actual compile.

The source assumes native O32 layouts and valid node/list storage. The early
snapshot reads a valid node's pointer field without dereferencing that pointer
until the original kind/flag path needs it. Native post-callback refresh and
list traversal order remain explicit. Arbitrary corruption, external aliasing
and gameplay behavior are not independently tested.

This packet contains only complete C, required real context, the compile recipe
and these notes. No test harness, receipts, independent verification, full tests,
CI wait, image/ROM integration or merge was performed. The independent checker
owns acceptance and merging.
