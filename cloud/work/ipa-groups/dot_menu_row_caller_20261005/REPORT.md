# Real menu-row caller exposes a 300-byte helper

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

## Result and limits

`func_8010A7A4`, `0x8010A7A4..0x8010A8D0`, is an exact **300-byte / 75-word**
research match. Its ELF function size and distance to the next function are
both 300 bytes. Production `blob_group.relocate` and an independent GNU linker
resolve the entire extent to the protected target, with the same SHA-256 and
no remaining masked, unresolved, unverified, or extra words.

The ordinary scorer initially reports `MATCH (2 section-relative relocations
unverified ...)`: its own `1.57079637f` literal is independently verified at
`0x801248D4` by `owndata.verify`, then fully relocated by both link paths. This
packet does not label the ordinary scorer's masked result a strict proof by
itself. No splice, lock, shadow-unit, image-build, or ROM gate was run, and no
cartridge coverage is claimed.

The complete real caller `func_8010A8D0` is new source context. It has the full
14-case menu draw loop and both surrounding setup/cleanup paths, replacing
the old `caller_a`/`caller_b` substitutes in
`cloud/work/ipa-groups/func_8010A7A4`. It remains a **NONMATCH**: 367/375 differing
words, 1,480 emitted function bytes versus 1,500 native bytes, no nonzero extra
words. Its own literal and table references remain unverified and unclaimed.
The claimed helper receives no credit for any part of that caller.

## Native hypothesis and source evidence

No arcade donor is established. The reference source checkout is absent here.
The native functions are N64 menu-option rendering, despite historical network
labels in the old index.

- The helper selects render mode 1 or 22 from the current menu mode, the row's
  angle, and selected row. It then draws a label at `x - 105` and value at
  `x + 40`, with signed-halfword coordinates.
- The caller projects the header and each visible row, selects resource sets
  13 then 11 under the real message queue, draws the centered header, and walks
  fourteen 64-byte option records. Angle is at +8, position at +48, and alpha
  at +60. Rows strictly inside the angular interval or with zero alpha are
  skipped. The two boundaries remain visible.
- Case 0 uses the separate value table. Cases 1..13 use the actual signed
  option selector bytes at offsets 10, 0..9, 18, and 11, respectively. The
  header's unsigned halfword at +32 is added to the selector. Label entries are
  25, 13, 15..24, 26, and 14 in case order. The protected switch mapping was
  checked privately; no extracted jump-table or instruction words are included.
- Fourteen genuine calls provide the helper's five consumed formals. IDO places
  `index` in a0 and `x`, `y`, label, value in s1..s4, exactly as in the native
  helper. No register annotations, unused inputs, stand-ins, or dummy calls
  are needed.
- The helper's duplicate color stores to local homes are the inlined argument
  homes of the genuine two-color endpoint setter `func_800BEA3C`. The accepted
  consumer `dispatch_handler` already reads the four RGBA channels. A packed
  union provides both the channel and 32-bit word views. The setter copies the
  explicit packed members, preserving the accepted complete 28-byte body.
  This reproduces both inlined call sites without the prior volatile locals.
- The selected-row branch is one call with a conditional mode argument. Two
  separate calls have the same high-level behavior but do not reproduce the
  native branch and delay-slot structure.

The caller also has another real caller, `func_8010AEAC`, and the helper has
fourteen additional real calls there. That larger source is not reconstructed
here and is not replaced with a substitute. Retaining A8D0 as an ordinary
entry gives a complete genuine context for the helper, but cannot reproduce
A8D0's own unsaved-register behavior. Its downstream `slot_state_setup`
register-summary context is also not supplied. Those are the next source
boundaries if the caller itself is pursued; no register-pressure search was
used to hide them.

## Accepted context and isolation

`src/blob/func_800BEA3C.c` and every production source, symbol, target, lock,
scorer, and compiler recipe remain unchanged. Its accepted source hash remains
`ced1420bc1b0bb4bb8dd693d4d83b7d7d004562da1268871fa1ef9657eeed173`.

The research group contains the same two endpoint assignments through the
packed word view. This is an explicit source-contract refinement in the
research copy, not a silent change to the accepted source. Both the original
standalone source and the refined grouped definition reproduce all seven
native words. Only A7A4 is claimed. The existing 28-byte setter receives zero
new-function credit.

Three bounded controls are recreated by `verify.py`:

- Original byte-aggregate copying: helper 63/75 different, 18 extra words
- Packed aggregate copying: helper 63/75 different, 14 extra words
- Separate dispatch calls: helper 31/75 different, zero extra words

All three preserve the accepted setter's strict match. These demonstrate why
setter-only equality did not settle its inlined source contract. An aligned
outer wrapper, word-only aggregate, retained/internal setter, same/separate
translation units, and inline annotation were also examined privately before
identifying the explicit packed-member operation. No inline annotation remains.

## Verification

From the repository root with the pinned toolchain on PATH and IDO_DIR set:

```
python3 cloud/work/ipa-groups/dot_menu_row_caller_20261005/verify.py
python3 -m pytest -q tests/conveyor/test_dot_menu_row_caller.py
```

`verify.py` checks target integrity, the exact ELF extents, literal ownership,
production relocation, independent GNU linking, the original accepted setter,
nine O32 layout facts, and 1,769 source behavior cases under ASan/UBSan. Host
cases cover every row, every selected-row pairing, both render modes, exact
angle boundaries, zero-alpha filtering, all fourteen label/value selections,
projection results, and real queue/setup/cleanup calls. These are source
behavior tests, not a native execution emulator.

The production relocation is given a sparse in-memory research image assembled
only from integrity-checked target extents and the checked own-data artifact.
It is not a reconstructed ROM/image gate. Generated objects, raw local logs,
and linked byte streams stay under ignored `build/`. The published proof
contains only source, hashes, counts, and verification outcomes.

## Integration and independent review

`peer_review.json` records independent review of the original eight-file packet.
The reviewer checked all fourteen native case/argument mappings and reproduced
a wrong-literal control: a masked score of zero still failed strict own-data
verification. The final integration adds a normal pytest toolchain replay which
runs the entire verifier and compares fresh evidence with `verification.json`.
It is not a saved-report-only test; missing IDO or MIPS tools fail CI under
`REQUIRE_TOOLCHAIN=1` through the existing shared test policy.

The packed-word setter is confined to this research group. Before eventual
promotion, reconcile the canonical setter source and its shared type with this
inlined contract, then verify the entire relevant unit and image. Standalone
setter equality does not prove equal inlined behavior. The parent remains
unmatched, and this packet does not establish a production-ready unit.

See `integration.json` for the actual aggregate result, unchanged-baseline
comparison, protected-path checks, and toolchain setup. No unrelated baseline
failure is fixed or waived here.
