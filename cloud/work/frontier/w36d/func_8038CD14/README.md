# func_8038CD14 = buildTrackMaps (MAT3 trackscr.c) — not matched

best_unit.c = the whole trackscr unit (same file as groups/trackscr/group.c at the time of writing); CD14 and its
inlined statics are func_8038CD14, func_8038CCF4 (buildRaceTrackMaps), func_8038CD0C (stunt), func_8038CD04 (battle),
func_8038CCFC (obstacle), func_8038CCEC (loadPathData), func_8038CCE4 (unloadPathData). Score with
`score.py group groups/trackscr --targets asm/us/ovl_a`: 1005/1206 positional (3 words short), ~31 register-normalised
rows, frame 488 and every variable home equal to retail. See ../RESULTS.md "func_8038CD14".
