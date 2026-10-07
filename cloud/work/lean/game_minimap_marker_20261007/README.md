# Minimap marker callback: lean research

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_80109F54`, `[0x80109F54,0x8010A534)`, 1504 bytes / 376 words.
Canonical O3 comparison improves **363/376 to 322/376 differing words**.
Candidate extent: 1516 bytes, three nonzero excess words, no unresolved symbols,
unverified relocations or errors. **NONMATCH research; claims are empty.**

## Source and corrections

Both frozen source indexes contain only the registered-head seed for this target:
`cloud/work/registered-heads/seeds/func_80109F54/group.c`, blob
`73fbde37b2a18d9439fec2655f4da5fa0c8f37da`. It is incomplete reconstruction evidence,
not a behavior oracle. This packet reconstructs the full native callback and:

- Uses the actual 0x80151CE8 record base, selected signed halfword at +4, 80-byte
  stride, and float coordinates at +12 and +20. The seed starts from +4 instead.
- Restores separate render-state words at 0x80118E20 and +4; the seed aliases both
  to the second word.
- Reuses the genuinely computed x/y values for the second text draw instead of
  the seed's uninitialized halfword aliases.
- Gives render_helper its actual float contract, including the -1.0f reset.
- Represents the real object widths, coordinates, visibility, frame and timer
  fields directly, preserving both visibility updates, aspect-ratio paths,
  mirrored map calculation, timer decrement and unsigned five-frame wrap.
- Reuses the existing returning font_set wrapper for fonts 6/7. Its inlined
  return value explains the native slot-return stores without dummy operations.

The compact genuine slot_state_setup and real corrected PR220 countdown callback
supply the out-of-line font context. Neither is a new claim. Countdown source
SHA256: `d41ecea76cddbccd58af80410a26666fa283e45beb5bd0a6d21cd712b1b43837`.
The returning wrapper derives from frozen `cloud/work/s20261004/E/src/helper.h`.
No seed stand-in caller, dummy pressure, artificial padding local, inline assembly,
or new volatile qualifier is retained.

## Focused controls and reproduction

All controls use the same real slot/countdown context:
- Frozen seed callback: 363/376, 1524-byte extent, five nonzero excess words and
  two unverified local-data references caused by its incorrect numeric float reset.
- Corrected body with direct queue/slot/queue calls: 325/376, 1508-byte extent,
  four canonical excess words; no relocation diagnostics.
- Corrected returning-wrapper body: 322/376, 1516-byte extent, three nonzero
  excess words; no relocation diagnostics.

Context slot_state_setup stays 18/58 with a 232-byte own extent and two canonical
next-symbol excess words. Existing countdown stays 105/150, own extent 604 bytes,
with four canonical next-symbol excess words in the candidate (one in controls).
Those context results are informational, not claimed improvements.

```
python3 tools/cloud/score.py group cloud/work/lean/game_minimap_marker_20261007
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`; unchanged canonical scorer also
supplies mandatory assembler `-r4300_mul`. The baseline is reproduced by using
the frozen callback and its required seed header, while replacing the seed slot
body and stand-in with this packet's real context. Direct-call control replaces
font_set(6/7) with the equivalent osRecvMesg/slot_state_setup/osJamMesg sequence.

Assumes native O32, valid object and map records, supported player/map indices,
nonzero map dimensions on this drawing path, and ordinary text/queue contracts.
Names describe observed behavior; original declarations and translation-unit
ownership remain unproved. No asynchronous-mutation or gameplay-safety proof is
claimed. Frame, allocation and scheduling differences remain.

Only source, actual context, flags and these observed notes are published. No
behavior harness, broad suite, CI wait, independent review, image/ROM acceptance,
integration or merge was performed. Independent checker owns acceptance and
integration. Protected targets/scorer and accepted production files are unchanged.
No ROM bytes, raw assembly, binaries, credentials or unrelated private data.
