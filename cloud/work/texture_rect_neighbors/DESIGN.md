# Matched graphics neighbors: packet-scope hypothesis

This test is recorded before compiling either new rectangle control.

## Independent source evidence

The locked graphics group uses ordinary SDK macros on `D_80149438++`:

- `src/blob/groups/gfx_modes/rect.c`: `func_8008A46C` clips against the same four globals, rejects only strict inversion, and adjusts the lower-right coordinates by one for drawing.
- `src/blob/groups/gfx_modes/func_8008A148.c`: the palette-selection arms compose six sibling packet scopes, then rejoin to update the texture cache.
- `src/blob/groups/gfx_modes/object_render.c`: real custom texture-loading wrappers compose SDK primitive macros. Its `gDPLoadTileGeneric` writes the same coordinate-field shape needed for the first texture-rectangle packet, with the coordinate pairs reversed at the call.

The neighbors do not establish that this rectangle used a custom wrapper. They also provide no evidence for a `do/while(0)` drawing wrapper. Macro provenance must not be confused with proof of original source identity.

## Distinguishing prediction

The baseline SDK rectangle keeps the first packet pointer's lexical scope around two nested `gImmp1` scopes. Three genuine SDK primitive calls can emit the same three packets with sibling scopes: `gDPLoadTileGeneric` using `G_TEXRECT`, followed by `gImmp1` for each texture half. There is no new helper, added runtime read, dummy expression, or loop.

If nesting of genuine packet lifetimes causes the residual alias predecessor, sibling scopes should remove that predecessor without adding an executable CFG block or changing height/offset live-block membership. Test only two complete sources:

1. Decompose the preceding stretched both-flags emitter, the arm whose alias close was implicated by the prior diagnostic.
2. Apply the same decomposition consistently at all eight real emitter sites.

Hold flags, arithmetic, clipping, declarations, conditions, target, compiler, and scoring fixed. Stop this hypothesis after those two controls; do not vary macro spelling, identifier names, or source whitespace to seek a score.

## Verification

Replay the frozen four-word baseline; run the workbench diagnosis. Compile the locked graphics group as an independent calibration, disclosing any section-relative references. For each control require independent GNU linking and agreement with the unchanged project relocator across the whole 1,780-byte extent. Check native, IDO, host defined-C and independent scalar packet behavior if compilation succeeds. All binary/disassembly artifacts stay under ignored `build/`.
