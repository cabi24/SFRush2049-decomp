# w36d RESULTS (wave 36, runtime image A, trackscr big functions)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36d (copied from base, master e68aea6e1).

## Deliverable (coordinated with w36e: ONE trackscr unit revision)

**THE trackscr deliverable is w36e's merged packet `cloud/work/frontier/w36e/groups/dot_runtime_a_c93c_closure_20261012/`**
(same-name supersession of the installed C93C group; keep func_8038E5DC; 12 claims, all strict MATCH). It is w36d's
whole-unit packet plus w36e's final AEB0/B3EC/C5EC/A820 bodies. w36d's own earlier 9-claim copy of that packet was
deleted (2026-10-21) so only one revision exists. Rescored from w36d's builder scratch (copy of w36e's files):
`score.py group ... --targets asm/us/ovl_a --claims`: C93C, BBB4, DFEC, ACB0, B834, A634, AE50, B388, B940, AEB0, B3EC,
C5EC all `MATCH`; context E5DC 412/926, CD14 1085/1206, A820 61/292.

w36d's share of the claims: func_8038BBB4 updateButtons (2616 B) and func_8038DFEC _InitScreen (1520 B), plus the MAT3
ports of OptionValid/TrackValid (with w36e), the unit construction, root and file order. If w36d later matches E5DC
or CD14, w36d will publish a superset revision at `w36d/groups/dot_runtime_a_c93c_closure_20261012/` that contains
w36e's packet bodies verbatim and will say so HERE; otherwise w36e's copy is the one to install.
w36d's working tree for the unit (groups/trackscr/parts + build.sh) now holds w36e's final bodies too (12 strict) and
w36d's newest CD14 context (1005/1206 positional, ~31 register-normalised rows).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_8038CD14 | buildTrackMaps (+ inlined buildRace/Stunt/Battle/ObstacleTrackMaps, loadPathData, unloadPathData) | 4824 | NOT matched: full N64 port (display-list map builder decoded); frame 488 and every variable home = retail; 1005/1206 positional (3 words short), ~31 register-normalised rows; residual = colour/temp-ring (see below) | func_8038CD14/best_unit.c |
| func_8038E5DC | updateTrackScreen (kept root) | 3704 | ONE instruction off: case 14 store (see Notes) | func_8038E5DC/best_unit.c |
| func_8038BBB4 | updateButtons | 2616 | MATCH in the unit group (deliverable: w36e's merged packet) | w36e/groups/dot_runtime_a_c93c_closure_20261012 |
| func_8038DFEC | _InitScreen | 1520 | MATCH in the unit group (same) | same |

### func_8038CD14 (buildTrackMaps) write-up

Levers that took it from 1203/1206 to 1005 positional / ~31 normalised rows (each measured):
1. `if (!IsTrackUnlocked(gTrackNum)) continue;` in all three map loops (MAT3 `&&`): only this gives the separate
   fail-path block where PRE inserts the gTrackNum reload, so the latch reuses the loaded value.
2. Race offsets `-(gPathMin[i] + gPathMax[i]) / 2` (D_801407D4 = gPathMin first), scale `gPathMax - gPathMin`.
3. `/ 2` with an INT 2 in the 16 sectVerts lines (`/ 2.0f` becomes `* 0.5f`).
4. The 24 `*= 0.01f` scalings as explicit statements (a loop is only 2x unrolled; retail is fully unrolled, memory-resident).
5. SDK GBI macros (gSPTexture/gDPPipeSync/gDPSetRenderMode/gDPSetCombine/gSPVertex/gSP2Triangles/gSPEndDisplayList)
   for every display-list word: with them the race prologue hoists 0.1f/0.01f/16.0f into $f24/$f22/$f20 as retail.
6. `f32 sectVerts[8][3]` declared in the point-loop block (the GBI `_g` slots then land above it, as retail);
   dx0, dy0, len0, dx1, dy1, len1, dx2, dy2, len2 at function level in that order; volatile gNumModels.
7. Frame: retail 488; the local list reproduces every retail home (saveTrackNum 436, x0 408, y0 404, y3 380, len0 368,
   len1 356, offsets 336/332/328, trackMBIdx 316, sizes 300/296, sectVerts 176) only with 4 disclosed-type filler
   slots (`f32 u0` before xoffset, `s32 u1, u2` before trackMBIdx, `s32 u3` before sectVerts' block); their original
   variables are unknown (the N64 loop probably had a few more locals).
Open residual (all colour/schedule; force.sh oracle confirms the t3/t4 swap is colour-only):
- ugen's integer temp ring starts at $t9 here, at $t6 in retail (every t6-t9 in the function is rotated); traced
  ugen reports the first allocation as reg 14 although the listing emits $25, unexplained.
- colour order of &gTrackNum (t3 here, t4 retail) vs the constant 6 (t4 here, t3 retail): priorities 21.5 vs 21.1.
- retail keeps dead stores of the two allocation sizes (`vtxSize` at 300(sp), `dlSize` at 296(sp)) and computes
  `size + 7` from them (two addiu); IDO here folds `k*64+71`; volatile/address-taken/inlined-parameter forms tried.
- end of the race display list: retail `addiu a0,s3,8` for gSPEndDisplayList's pointer, here `8(s3)` offsets.
Not tried to completion: forcing every web to retail colours (needs the ring fixed first).

## Identities proven

The whole N64 trackscr.c is one IDO IPA unit with root func_8038E5DC (only it saves s0-s8/$f20-$f30):
| N64 | MAT3 (trackscr.c) | evidence |
|---|---|---|
| func_8038E5DC | updateTrackScreen | game_calls_here; 21-case jump table = 21 options, gOptions offsets/limits per case identical |
| func_8038DFEC | _InitScreen | OptionValid loop, buildTrackMaps, GetPlayerTrack x4 (+14/+18/+6), SetOptions/createBG/initButtons/NewMultiBlit order |
| func_8038BBB4 | updateButtons | MATCH (strict, in group) |
| func_8038CD14 | buildTrackMaps (+inlined buildRace/Stunt/BattleTrackMaps, loadPathData, unloadPathData) | gRace 4/6/5 dispatch, loops 14..17 / 6..13 / 0..5, trackMapsBuilt=1, pathPos=0 |
| func_8038C5EC | showTrackSelect | calls updateButtons first, OptionValid loop, 30*gFrameSeconds, gBackwards/gMirrorWaves |
| func_8038ACB0 | OptionValid | MATCH (strict, in group) as a direct MAT3 port (w36e region; context here) |
| func_8038B834 | TrackValid | MATCH (strict, in group) as the natural MAT3 port: IDO turns `track >= 14 && track <= 17` into pointer compares on &trackReady[track] (w36e region; context here) |
| func_8038A634 | SetFogIntensity | color_smoothing candidate MATCHES in this unit (w36e region) |
| func_8038AE50 | unloadTrackScreen | two RemoveBlit/clear pairs |
| func_8038AEB0 | buildTrackMap (+inlined loadTrackScreen) | |
| func_8038B388 | clearTrackMap | unloadTrackScreen + ZOID_RemoveObject(trackMapObj) |
| func_8038B3EC | animTrackMap | |
| func_8038B940 | _UninitScreen (+inlined uninitButtons, clearTrackMaps) | |
| func_8038A820 | animTrackMaps | |
| func_8038C93C | initButtons | installed member |
Game callees: func_800B5570 setGameState, func_800A361C (pak removed), resource_type_select PlayStartABSound,
audio_doppler PlayLRSound, entity_flags_apply AudPlayEffect, audio_bus_mix SavePlayerOption, reverb_setup
GetPlayerOption, menu_text_input SavePlayerTrack, func_800CDC3C GetPlayerTrack, init_state_begin SetOptions,
particle_lifetime_set createBG, sound_control NewMultiBlit, particle_position_set ZeroCamera, Effects_UpdateEmitters
UpdateCameras, entity_audio_update ClearRushControls, func_800F7644 IsTrackUnlocked, audio_frame_sync LoadWave,
slot_value_get GetWaveInstanceAddr, audio_dsp_process ReadCheckpointData, audio_effect_apply ReadPathData,
func_80096130 UnloadWaveInstance, audio_dma_sync AllocateMemoryHeap, func_800BA61C FindResPathIntersect.
Data: D_8014978C gTrackNum, D_8014A110 gRace, D_80146108 gOptions, D_801407B4/D_801407D4 gPathMin/gPathMax (s16[3]),
D_801407F0 gResPath, D_80151CE8 gCheckpoints (loopPt +2), D_803B7CE4 trackMap[20] (72 B), D_803B7738 trackMapObjNames,
D_803B7CD8 kNumMapSects, D_803B6B14 buttonInfo[48], D_803B7714.. buttonsInited/JustInited/gButtonScale/Space/buttonX/maxY/minY/buttonZ/cursorZ,
D_803BA898 gNumOptions, D_803BA85A gActiveOption, D_803BA878 gFirstOption, D_803BA8B0 gOpts, D_803BA918 gTracks[19].

## Notes

- Working tree groups/trackscr (2026-10-21, after the coordinator's restart): w36e's final AEB0/B3EC/C5EC/A820
  bodies imported verbatim, 12 strict members as in the deliverable; additionally _UninitScreen split into MAT3's
  inlined statics clearTrackMaps (stub 0x8038B930) and uninitButtons (stub 0x8038B938), still MATCH, so the compiled
  object now emits every function and every deleted stub at retail's relative order (A62C, A634, A820, ACB0, AE50,
  AEA8, AEB0, B388, B3EC, B834, B930, B938, B940, BBB4, C5EC, C934, C93C, CCE4..CD0C, CD14, DFEC, E5DC). Optional
  refinement for the installed packet; not needed for any claim.
- updateWarningScreen (func_8038A400, w36e) placed inside the trackscr unit as a second kept root changes nothing
  (50/139, CD14 unchanged): it is its own unit.

- E5DC case 14 (the only residual of the root): with the store written through a second symbol for the same address
  (diagnostic only: `D_8014611D[8] = 3`, D_8014611D = the image's name for &gOptions[21], the per-player option block)
  E5DC is a strict MATCH (diagnostic edit recorded in func_8038E5DC/README.md). So retail's store used a different base symbol/lda than the gOptions array;
  every spelling of gOptions[29] (pointer forms, casts, struct-less offsets) folds back to the v0 base. A forced-split
  sweep over all 59 colour webs (trace force.sh) never produces the absolute store, so it is not colouring. The genuine
  source spelling is unknown; not delivered with the synthetic alias.

- 2026-10-21: merged w36e's unit bodies (AE50 `Blit *b` form, B388, B940 calling clearTrackMap, loadTrackScreen
  helper AEA8 + AEB0 with SDK GBI macros) from cloud/work/frontier/w36e/groups/best_ts. In the merged unit:
  C93C, DFEC, BBB4, ACB0, B834, A634, AE50, B388, B940 MATCH; AEB0 12/310 (w36e: colour + frame); E5DC 1 insn.
- gNumModels (D_80140BDC) is `volatile u8` (buildTrackMaps' race loop materialises its address); C93C/BBB4/DFEC still match.
- Float `x / 2` with an INT 2 compiles to div.s by 2.0; `/ 2.0f` becomes `* 0.5f` (IDO reciprocal). The race-map
  vertex code uses `/ 2` (retail div.s).

- Emission order in the compiled unit puts IDO's deleted-procedure stubs where retail has them: setCamera = stub
  0x8038A62C (before SetFogIntensity), BlowFog = stub 0x8038C934 (before initButtons). The six stubs before
  buildTrackMaps come out as unloadPathData, loadPathData, race, obstacle, battle, stunt (0x8038CCE4.. in that order).
- buildTrackMaps loops: N64 writes `if (!IsTrackUnlocked(gTrackNum)) continue;` (MAT3 uses `&&`). Only the continue
  form gives the separate fail-path block in which uopt's PRE inserts the gTrackNum reload, so the loop latch reuses
  the loaded value (retail `b tail; lb s5` / `bnel ..; addiu t6,s5,1`). With `&&` the latch reloads (critical edge).
- The first four display-list words are written with constant offsets (dl[0..3], then dl += 4), the rest with dl++.

- Levers that closed DFEC (2026-10-21): TrackValid/OptionValid as natural MAT3 ports (their exact clobber sets
  decide DFEC's s-registers: a TrackValid that also used s1 pushed every DFEC loop variable up one register);
  GetPlayerOption loop uses `i` (not MAT3's `j`) and its body on its own line (as1 then fills the branch delay slot
  with the `sb v0,-1(s0)` store, as retail); no block-scoped `j` in the gTrack search; `mapTransition = 0` (int
  zero: a fresh `mtc1 zero`, not the 0.0f web); BlowFog's MAT3 static fogTileSize + `tile` pointer (shared `lui at`
  for uls/ult); `gNumOptions = 0;` on the same line as its for loop (as1 tie order); helpers in MAT3 file order
  (BlowFog/setCamera after _InitScreen) for as1's line-number tie breaks.

- Shared unit with w36e: the trackscr group (groups/trackscr) contains every trackscr function. It is meant to
  supersede `dot_runtime_a_c93c_closure_20261012` IN PLACE (same name; C93C stays a member; kept root moves from
  DFEC to the real root E5DC). w36e bodies are included as context; when w36e lands bodies they should be merged
  here (or w36e can take this file as its unit).
- updateButtons frame: retail 120; without three unused f32 locals declared first the frame is 112 and every local
  sits 4 bytes high. Same 3-slot phenomenon as w35d's startscr updateButtons.

