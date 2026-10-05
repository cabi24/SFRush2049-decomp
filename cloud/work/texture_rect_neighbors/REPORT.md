# Matched graphics neighbors and SDK packet lifetimes

## Result

The new sibling-packet-scope hypothesis is rejected for the frozen rectangle body.
Both bounded ordinary-C controls produce the exact same linked 1,780-byte body
as the baseline: **4 differing words out of 445**, no extras, no unresolved or
unverified relocations. The original scheduling residual is preserved.

The result localizes more strongly than identical final bytes. After ignoring
only source comments and `.file`/`.loc` directives, all **495** retained ugen
listing lines agree across baseline and both controls, including the complete
alias/no-alias metadata. The implicated alias close remains after the preceding
arm's exit. No transformed listing is supplied to the compiler or assembler.

Each of the three compiled sources passes **6,926 behavior cases**: 6,828 cases
through the existing native/compiled/scalar/defined-host-C verifier and 98
additional native/compiled/scalar cases from the independent behavior-first
packet. The extra cases are not claimed as defined host-C checks for this
older signed-arithmetic source. All 445 native
instructions are exercised. GNU-linked function bytes agree with the unchanged
project relocator across the complete extent. This is research, not a match,
image splice, ROM hash claim, or hardware graphics test.

## What was genuinely new

The prior authentic-SDK test changed the header context while leaving the
`gSPTextureRectangle` call topology intact. This test changes the topology of
the real packet pointer scopes:

- The SDK rectangle macro declares a pointer to the first packet in an outer
  scope and shadows it in the two nested `gImmp1` scopes.
- A sequence of existing SDK primitives, `gDPLoadTileGeneric` with `G_TEXRECT`
  followed by two `gImmp1` calls, emits the same three packets using sibling
  scopes. The coordinate pairs supplied to the generic primitive are reversed
  to preserve the texture-rectangle layout.

Only two source controls were tested, as recorded beforehand in `DESIGN.md`:
`sdk_sibling_both.c` changes the preceding stretched both-flags emitter;
`sdk_sibling_all.c` applies the same construction to all eight emitter sites.
The formulas, conditions, declarations, reads, command count, compiler and
flags remain unchanged. No new helper, loop, dummy expression, pressure
variable, assembly emission, or compiler-control change is introduced.

The locked `object_render` source supplies the existing generic macro. Its
whitespace-independent token sequence also agrees with `gDPLoadTileGeneric`
in the previously pinned [libreultra 2.0I SDK header](https://github.com/n64decomp/libreultra/blob/master/include/2.0I/PR/gbi.h),
Git blob `b418e0321f48acd86187a066a873211f0e0ff9cb`.
This supports authentic primitive provenance; it does **not** establish that
Rush originally assembled texture rectangles this way.

## Matched-neighbor calibration

Rebuilding the unchanged `src/blob/groups/gfx_modes` with its recorded
whole-program recipe gives strict zero-difference comparisons for all six
members, with no unresolved, unverified or erroneous references:

- `func_80086A50`: 387 words
- `func_8008705C`: 45 words
- `func_800878E0`: 74 words
- `func_8008A148`: 145 words
- `func_8008A46C`: 118 words
- `sound_init`: 100 words

`func_8008A46C` independently corroborates the same signed clip box, strict
inversion rejection and inclusive lower-right coordinate adjustment.
`func_8008A148` demonstrates ordinary sibling packet scopes across palette
selection arms. `object_render` demonstrates game-local composition of SDK
primitives. None of these sources establishes a drawing-body `do/while(0)`
wrapper for the target function. Source shapes that reproduce other functions
are supporting context, not proof of the original target source.

## Interpretation and stopping condition

Changing lexical nesting of these genuine packet temporaries is eliminated
before scheduling. It neither removes the metadata predecessor nor introduces
the lifetime divisor change seen with the separate drawing-body wrapper.
The earlier wrapper result therefore cannot be explained just by splitting
the SDK's packet-pointer scopes. This lane stops after the two predeclared
controls; no spelling, whitespace or identifier sweep follows.

The workbench was run before modifying either candidate. Its first comparison
to an unrelocated object included expected symbol-site cautions; diagnosis of
the fully GNU-linked baseline reports the pure scheduling residual, 445
instructions, a 104-byte frame and zero relocation-symbol disagreements.
The independent full linker proof, not a masked workbench score, controls the
numerical result.

## Reproduce

Use the pinned IDO/binutils environment, then run:

    python cloud/work/texture_rect_neighbors/verify.py
    python -m pytest -q tests/cloud/test_texture_rect_neighbors.py

The script validates each complete control against the exact declared source
transformation before compiling. It recompiles the unchanged graphics group,
replays all three rectangle sources, checks relocation completion and behavior,
and checks the recorded negative result. Receipts contain source/tool hashes,
comparison counts and behavior coverage. Objects, protected bytes, listings,
and raw disassembly remain in ignored `build/87110_neighbors/`.
