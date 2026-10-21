# func_8038CD14 = buildTrackMaps (MAT3 trackscr.c, GC 0x8008ad10) - lead, NOT matched

best_unit.c = w36d's newest whole trackscr unit (cloud/work/frontier/w36d/func_8038CD14/best_unit.c, GBI-macro CD14)
with ONE change in the inlined buildRaceTrackMaps (func_8038CCF4): the two allocation sizes live in a local ARRAY
`s32 size[2]` (vtx = size[1] at sp+300, dl = size[0] at sp+296, dl written `(D_803B7CD8 * 5 + 7) * sizeof(Gfx)`).
uopt keeps stores to constant-indexed local array elements (memory variables, "mvarbits") while still colouring the
value, which is retail's "dead" `sw a2,300(sp)` / `sw v1,296(sp)` with `size + 7` computed from the register.
The dl half is now exact (`addiu v1,t9,56; addiu a1,v1,7; jal; sw v1,296(sp)`); positional score 1005 -> 404 of 1206
(aligned 537 -> 440 rows, normalised hunks 21 -> 14). A struct {dl; vtx} is scalarised (no change).
Open: the vtx element is not a register candidate here (retail colours it a2 and moves trackRomObjDef to a3);
end-of-race-DL `_g` in a0 (retail) vs folded offsets; the constant-72 web's split (multu vs shift sequences swap
places); temp ring start. All 12 packet claims still MATCH with this file (w36x2 vrun c4).
Score with: score.py group <dir with best_unit.c as group.c + group.json> --targets asm/us/ovl_a.
