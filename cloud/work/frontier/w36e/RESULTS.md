# w36e RESULTS (wave 36, runtime image A: trackscr/startscr rest)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36e (from base, master e68aea6e1).

## Coordination with w36d (ONE trackscr unit revision)

**Deliver this revision of the trackscr unit:** `cloud/work/frontier/w36e/groups/dot_runtime_a_c93c_closure_20261012/`
(same-name supersession of the installed C93C group). It is w36d's packet file of 2026-10-21 00:51
(w36d/groups/dot_runtime_a_c93c_closure_20261012/group.c, byte-for-byte except four bodies) with w36e's final
buildTrackMap (AEB0), animTrackMap (B3EC), showTrackSelect (C5EC) and animTrackMaps (A820, context) spliced in by
`w36e/merge.py THEIRS MINE OUT`. Claims (12, all strict MATCH): C93C, BBB4, DFEC, ACB0, B834, A634, AE50, B388,
B940, AEB0, B3EC, C5EC. w36d: if you produce a newer revision (E5DC / CD14), rebuild it from THIS file (or re-run
merge.py with your file as THEIRS and w36e/groups/best_ts/group.c as MINE) so the delivered unit keeps all 12
claims; then say in your RESULTS which file supersedes this one. w36e's own groups/w36e_trackscr (9 claims) is
superseded by the merged packet and should NOT be installed.
**Confirmed by w36d** (w36d/RESULTS.md "Deliverable", 2026-10-21): w36e's merged packet is THE trackscr deliverable;
w36d deleted its 9-claim copy, so only this revision exists. If w36d later matches E5DC it will say so in its
RESULTS and reuse these bodies verbatim.

Note (coordinator): cloud/matches/ovl_a/func_80390BC0.c makes tests/cloud/test_runtime_a_resource_cache.py fail
(it asserts the file is absent) - expected; the integrator handles that research receipt.

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_80390DCC | UpdateSelectCar@selectcarload.c (CONFIRMED: MAT3 statement order; N64 9 args r,g,b tint; CreateSelectCar inlined (stub 0x80390BB8)) | 1700 | 33/425 in its group (superseding joint_transforms with SetSelectTireSizes): every remaining word is a spill-home offset (+4: retail homes 92..112, ours 88..108). Code identical with `CreateSelectCar(s16, s16, s32 cam)` (the s32 param is the lever that stops the cam-load CSE web), FSCLS = D_801106C4[gFrameScale - 1], gTireRims = D_8011157C[p+1][idx], wheelTex before rims store. Frame: retail locals area 56 B larger than MAT3 locals; 12 unused s32 + 1 between cam and alphaCol reproduce frame 232 and alphaCol/cam/newuvs offsets; the 4-byte home shift suggests one more coloured expression web in retail | func_80390DCC/group_best (padded diagnostic: group_padded) |
| func_8038AEB0 | buildTrackMap@trackscr.c with loadTrackScreen inlined (stub 0x8038AEA8) (CONFIRMED) | 1240 | MATCH in trackscr unit (). Levers: SDK GBI macros; `vtxSize`/`dlSize` locals computed before each memcpy; `(n * 5 + 7) * sizeof(Gfx)`; volatile gNumModels; compiled-out holder `if (vertex) {}` in the last-section branch (colour priority); disclosed frame residual: unused `Vtx *vtx` + `s32 trackMBIdx` (168 -> 176) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038A820 | animTrackMaps@trackscr.c (CONFIRMED; strip helper inlined, N64 Vtx.cn colouring via a 4-byte colour union) | 1168 | 61/292 in trackscr unit: one extra `sw v0,92(sp)` trackIdx spill at the loop head (uopt live-range split region; retail reloads only on the point-loop exit) + as1 order of the mapPos/pos block; frame and code otherwise identical. Tried: continue form, do-while, mapIdx placement, condition forms, source-line layouts, pStrip/trackPS2 local shapes (the `pStrip = verts; pStrip += pointIdx * 4;` two-statement form and colour union fixed the colour/vertex code) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038B3EC | animTrackMap@trackscr.c (CONFIRMED; clearTrackMap inlined, buildTrackMap called) | 1096 | MATCH in trackscr unit (; MAT3 function-local statics cityPos/mapPos/rotAngle; N64 blend operand orders) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038FFD8 | initScreen@startscr.c (CONFIRMED: MAT3 initScreen + inlined initButtons (stub 0x8038FFD0) + inlined eggs.c InitEgg(23)) | 1088 | **MATCH in group** w35d_start_screen (superseded in place; old members F79C/80390418 unchanged and MATCH). Lever: MAT3 file order (as1 breaks ties by source line; initButtons must follow initScreen) | groups/w35d_start_screen |
| func_8038C5EC | showTrackSelect@trackscr.c (CONFIRMED) | 840 | MATCH in trackscr unit () with disclosed frame residual: unused `UVMatrix fogMat` (48 B, above uv) + `f32 fogY, fogDelta` (8 B); without them 3 words differ (frame 120 vs 176, uv at sp+84 vs sp+92). Statics angle/fogPos, MAT3 literal spellings | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038B940 | _UninitScreen@trackscr.c (uninitButtons, clearTrackMap, clearTrackMaps inlined) | 628 | MATCH in trackscr unit (calls clearTrackMap(), `def` pointer local) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038A400 | updateWarningScreen@warningscr.c (CONFIRMED by GC tail: RemoveBlit, PS_GetBlocks(0x810), setGameState(gPakScr ? 0x4000000 : 4), SetControllerActive/StickEdges(-1, 1); N64 adds the controller/pak scan) | 556 | 50/139 standalone (normal ABI, game-called). Residual = two branch layouts: retail controller scan has `beqzl -> inc; b exit` (ours folds to `bnez exit`), and retail pak scan goes `bnez v0, done` (ours `beqz -> inc; b done+8`). Levers found: one reused variable for freeBlocks / the gPakScr toggle / the setGameState argument (retail s1), the controller scan as a static helper `return i < 4` (post-loop LFTR compare). ~25 loop/goto shapes tried (var/) | func_8038A400/var/n.c |
| func_8038A634 | SetFogIntensity@trackscr.c (N64 blit/palette tail); four-wide t6-t9 ring = internal | 492 | MATCH in trackscr unit (dot_runtime_a_color_smoothing_20261006 body) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038ACB0 | OptionValid@trackscr.c (register param in a1, index*4 LFTR) | 416 | MATCH in trackscr unit (plain MAT3 OptionValid + 2 N64 tests, index arrives in a1 by IPA) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_80390BC0 | LoadSelectCar@selectcarload.c (CONFIRMED: MAT3 loop/assign order; N64 index+88, extra func_800BB02C) | 364 | **MATCH standalone** (`score.py fn ... --targets asm/us/ovl_a`: `MATCH`, no unverified relocations) | cloud/matches/ovl_a/func_80390BC0.c |
| func_8038B834 | TrackValid@trackscr.c (s0 unsaved) | 252 | MATCH in trackscr unit (plain MAT3 TrackValid) | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038B388 | clearTrackMap@trackscr.c (+ N64 wait on volatile D_8002EB70) | 100 | MATCH in trackscr unit  | groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038AE50 | unloadTrackScreen@trackscr.c (two blits 0x803B8288/0x803B828C) | 88 | MATCH in trackscr unit (form with `Blit *b` local keeps it out of line) | groups/dot_runtime_a_c93c_closure_20261012 |

## Deliverables (summary)

1. **groups/dot_runtime_a_c93c_closure_20261012/** (RECOMMENDED trackscr packet): same-name supersession of the
   installed C93C group = w36d's whole-unit packet (cloud/work/frontier/w36d/groups/dot_runtime_a_c93c_closure_20261012,
   copied, not edited there) + w36e's final AEB0/B3EC/C5EC/A820 bodies (merge.py). 12 claims, all strict MATCH:
   C93C (old member, unchanged), BBB4, DFEC (w36d), ACB0, B834, A634, AE50, B388, B940, AEB0, B3EC, C5EC.
   Context: E5DC 412/926 (w36d: one instruction), CD14 1085/1206, A820 61/292. Exit 0 with --claims.
   Own .rodata hand-verified (rocheck.py on the merged object): 23 sites, 0 bad, single base 0x803B92C8.
   Own .data windows (dcheck.py): B3EC 0x803B8338/44/50, C5EC 0x803B8290, all equal. .bss: BBB4/DFEC statics
   (w36d), C5EC fogPos 0x803B9A90 (image A .bss).
   If w36d's own packet is installed instead, add AEB0/B3EC/C5EC from this one (they need w36d's MAT3 file order
   and my holder/unused-local disclosures; see each body).
2. **groups/w35d_start_screen/**: same-name supersession of the installed startscr group; claims F79C, 80390418
   (unchanged) + FFD8 (new). Exit 0 with --claims; .rodata 27 sites OK (strings base 0x803B8CE0, floats 0x803B9360,
   as in the installed group); .data template 0x803B6A74 = FFFFFFFF.
3. **cloud/matches/ovl_a/func_80390BC0.c**: LoadSelectCar, standalone strict MATCH.
4. groups/w36e_trackscr/: my earlier standalone packet of the same unit (9 claims) - superseded by (1).

## Deliverable: group packet groups/w36e_trackscr (9 members claimed; superseded by the merged packet above)

`score.py group lane/groups/w36e_trackscr --targets asm/us/ovl_a --claims` (builder scratch, exit 0):
func_8038A634 MATCH; func_8038ACB0 MATCH; func_8038AE50 MATCH; func_8038AEB0 MATCH (4 section-relative relocations
unverified: .rodata+0x8/+0xc); func_8038B388 MATCH; func_8038B3EC MATCH (20 section-relative relocations unverified:
.data+0x0/+0xc/+0x18, .rodata+0x10..0x18); func_8038B834 MATCH; func_8038B940 MATCH; func_8038C5EC MATCH (24
section-relative relocations unverified: .data+0x1c, .rodata+0x34..0x40, .bss+0x0). Context: E5DC 744/926, DFEC
162/380, BBB4 563/654, CD14 1203/1206 (w36d stand-ins), A820 61/292, C93C MATCH.
The scratch scorer cannot verify overlay own data ("no retail data available" / ".bss not in a known game range"):
expected; owndata checks .data windows independently and image-A .bss 0x803B9A50..0x803BAE20 at the splice.
Integration: new group `w36e_trackscr`, keep [func_8038E5DC], members=claims as listed, context as listed,
"allow_unverified": true. It overlaps the installed dot_runtime_a_c93c_closure_20261012 only through context
(C93C member there/context here; ACB0, B834 context there/members here). No overrides. When w36d's real
E5DC/DFEC/BBB4/CD14 bodies land, rescore this packet with them as context (the members' code depends only on
the unit's IPA facts, which the stand-ins already reproduce: all nine match with stand-ins).

## Identities proven

- func_80390BC0 = LoadSelectCar@selectcarload.c (GC 0x8003b97c): three slot-search loops, LoadWave(index+88), gCarModelNums write. MATCH.
- audio_frame_sync = LoadWave (call shape LoadWave(index+N,1,1,0,0)); D_80142B08 = gCarModelNums; D_803BA230 = gSelectCarWavePtrs[16] (12-byte {wave, loading, slot}).
- sfx_volume_set = InitCarDynamicObjs(slot, idx, gCarModelNums[slot]) (called from UpdateSelectCar when !ready).
- func_80390DCC = UpdateSelectCar (N64 9 args); func_80390A28 = SetSelectTireSizes (installed); func_80390D2C = UnloadSelectCar (installed).
- func_8038FFD8 = initScreen@startscr.c with initButtons and eggs.c InitEgg(23) inlined: D_803B4278 = gEgg, D_803B427C = gNextTimeOut (f32), D_803B4280 = gEggIdx, D_803B41D4 = &gEggs[23].
- func_8038A400 = updateWarningScreen@warningscr.c (N64 body); D_803B839C = gPakScr, D_803BA7EB = gBlocksNeeded, D_803B83C8 = sBlit.
- func_8038A634 = SetFogIntensity@trackscr.c; D_803BA1E0 = fogColor[2], D_803BA1E8 = n64FogPal (anon_name_start_18, palBase at +0x14), D_803B828C = trackFogPoly (an N64 Blit), D_80114658 = gFogColors[][3], D_801427A0 = gFogColor, D_8013F1D8 = gFogIntensity; Input_ApplyPadConfig = UpdateBlit.

## What generalises

- **IDO umerge inline limit is tiny and size-based**: two `if (p) { f(p); p = 0; }` blocks are exactly at the
  limit; one more ucode statement (even dead `if (0) {...}`, an empty `if (x) {}`, a `Blit *b` temp) keeps a callee
  out of line. Callers with ~100+ basic blocks never receive inlines (partial inlining = big callers keep calls).
  Experiments: w36e/inl/ (gen*.py). Hand-inlined copies of a static helper are wrong when retail inlined it: call it.
- **Function-local statics vs externs**: a MAT3 function-local static whose address is never taken is NOT reloaded
  around calls/stores and its address is not CSE'd into a register; declaring it as an extern D_ symbol changes the
  code completely (animTrackMap 271 -> 0, showTrackSelect 167 -> 0). owndata checks .data windows independently,
  so scattered statics are fine.
- **Arguments computed into locals before a call** (`size = ...; memcpy(dst, src, size);`) reproduce retail's
  argument evaluation order / temp ring when the inline expression form does not (buildTrackMap 76 -> 12).
- **`(n * 5 + 7) * sizeof(Gfx)`** keeps `n*5` as a multiply that shares a register constant (multu by s2=5); the
  `* 8` spelling lets the 5 fold away.
- **as1 breaks scheduling ties by source line** (w36d): define an inlined helper AFTER its caller (MAT3 file order);
  initScreen went 7 -> 0 just by reordering functions.
- **Parameter type of an inlined helper matters**: `CreateSelectCar(s16, s16, s32 cam)` stops uopt building a
  cam-load CSE web across calls (UpdateSelectCar 247 -> 33).
- **A struct-typed colour local updated per branch** (SetFogIntensity's `tRGBA col` promotion) explains dead
  stack stores sunk to the end of both if/else arms.
- Frames in these N64 front-end functions repeatedly need disclosed unused locals (C5EC, AEB0, BBB4, F79C, w35f).

## Notes / permission denials

none yet

## trackscr cluster: inlining law found (for w36d too)

IDO umerge inlines a callee only if its ucode is at or below a tiny size (two `if (p) { call(p); p = 0; }` blocks
is exactly at the limit: ONE more statement, even dead `if (0) {...}` or an empty `if (x) {}`, blocks it;
experiments in inl/), and never into a caller with ~100+ basic blocks (that is why clearTrackMap is inlined into
animTrackMap/_UninitScreen but stays a call in updateTrackScreen). Retail keeps unloadTrackScreen (AE50) out of
line, so its source is one ucode statement bigger than the plain form. With
`Blit *b; if ((b = D_803B8288)) { RemoveBlit(b); D_803B8288 = 0; } ...` AE50 stays out of line and
AE50, B388 (clearTrackMap), and B940 (_UninitScreen calling clearTrackMap(), with a `def` pointer local for the
gRomObjDefPtrs lod restore) all MATCH in the unit (groups/ts2). B3EC/B940 must CALL clearTrackMap() and AEB0 must
call loadTrackScreen() (as static/non-kept functions) instead of hand-inlined copies.

## Key structural finding (trackscr)

Every trackscr function except the kept root updateTrackScreen (func_8038E5DC, w36d) is IPA-internal: unsaved
callee-saved registers (A820, AE50, B388, B834, ...), register parameters (OptionValid takes its index in a1), four-wide
t6-t9 temp ring (SetFogIntensity). None can match standalone; they match only inside the whole trackscr unit with the
real root. w36d is building that unit (cloud/work/frontier/w36d/groups/trackscr); w36e uses a copy (groups/ts*) with
w36d's root/BBB4/DFEC/CD14 drafts as context and replaces the w36e bodies.
