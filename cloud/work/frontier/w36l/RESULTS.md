# w36l — wave 36, visuals.c (MAT3 GC donor), game code

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36l (copied from base, master e68aea6e1).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| drone_ai_update (0x80093B20) | AnimateFrame@visuals.c (confirmed) | 3280 | **MATCH** (group, score.py group strict + blob_unit EQUAL) | groups/codex_visual_tire_module_20261002/rocket.c |
| entity_tick_main (0x800930A4) | AnimateRocketFlame@visuals.c (confirmed) | 2684 | **MATCH** (group, strict + unit EQUAL; 6 disclosed frame-residual locals) | groups/codex_visual_tire_module_20261002/rocket.c |
| entity_update_callback (0x80090FEC) | AnimateNewBlast@visuals.c (confirmed) | 2184 | **MATCH** standalone (score.py fn) + unit EQUAL; 4 disclosed frame-residual locals | cloud/matches/entity_update_callback.c |
| music_seq_load (0x800B15B4) | CreateCar@visuals.c (confirmed) | 1428 | 34/357 words (temp-ring names only; norm 0, frame exact) | music_seq_load/best.c |
| save_write_data (0x800AF06C) | StartBlast@visuals.c (confirmed) | 1200 | 169/300 (norm 42) | groups/w36l_blast/blast.c (research, no claims) |
| func_80090308 | StartNewBlast@visuals.c (confirmed; internal, slot via caller's 0(sp)) | 1128 | 169/282 (norm 8, frame exact) | groups/w36l_blast/blast.c |
| (partner) entity_physics_update (0x800908A0) | AnimateBlast@visuals.c | 712 | MATCH in group w36l_blast + unit EQUAL — PROVISIONAL (needs StartNewBlast) | groups/w36l_blast/blast.c |
| music_control (0x800B1B48) | StartEnvironment@visuals.c (confirmed; StartShadow+StartArrow inlined) | 992 | **MATCH** standalone + unit EQUAL; 8 disclosed frame-residual locals | cloud/matches/music_control.c |

## Permission denials
none

## Identities proven (for w36n/w36y)
Functions (N64 -> MAT3, evidence = matching or the call/field/constant pattern noted):
- drone_ai_update = AnimateFrame@visuals.c (MATCH); entity_tick_main = AnimateRocketFlame@visuals.c (MATCH)
- entity_update_callback = AnimateNewBlast@visuals.c (MATCH); music_control = StartEnvironment@visuals.c (MATCH)
- music_seq_load = CreateCar@visuals.c (34/357, structure exact); save_write_data = StartBlast@visuals.c;
  func_80090308 = StartNewBlast@visuals.c; entity_physics_update = AnimateBlast@visuals.c (MATCH, provisional)
- entity_collision_detect = AnimateNewBlastRing@visuals.c (callback 0x80090B68 installed by the inlined
  StartNewBlastRing in StartBlast); entity_process_main = AnimateShadow@visuals.c (visuals[11] callback set by
  StartShadow inside StartEnvironment); audio_frame_update = StartCar@visuals.c (called after CreateCar)
- save_slot_valid = InitCarObj@visuals.c (7 args); sound_bank_unload = SetupCarColor@visuals.c (5 args on N64);
  func_80092FE0 = SetFrameColor; func_80092BF4 = SetSheenColor; func_80092B80 = PickCar (int return)
- func_8008E06C = ZOID_SetColor; func_80092BC8 = ZOID_SetEnvColor; func_8008B3A0 = ZOID_FindMatrix;
  func_8008AE64 = ZOID_ObjectFlags; func_8008AE48 = ZOID_SetObjectFlags; func_80090770 = ZOID_SetObjectDef;
  func_8008D870 = ZOID_SetTexture; func_800A78BC = ZOID_NewPoly; entity_spawn_callback = _RemoveObjectTree
  (zoid.c; called (objnum, 0, 0) through the inlined ZOID_RemoveObject -> ZOID_RemoveObjectTree chain);
  func_8008E26C = the callee of the inlined MBOX_NewObject (4 args); entity_transform_apply = ReleaseEnviron;
  func_80090284 = GrabEnvEntry; func_80090F44 = PitchUV; func_80090E9C = YawUV; math_utility = CopyUV;
  func_8008B32C = ScaleUV; entity_spawn_init = StartSmoke; camera_target_track = EmitGameSound;
  model_data_load = MBOX_HideObject; model_transform_setup = MBOX_ShowObject
Data: D_80154660 gBlastObjs (tBlastStruct 0x190); D_80154FD8 gBlastRingObjs (0x3C); D_8013C238 gBlastMats;
D_8013C094 gBlastMatCnt (s16); D_801391F0 gEnvList; D_8012E700 ObjInstanceList (0x44); D_80139320 gCarParts
(N64 0x40: frame, extraparts[3], hood 0x10, sheen 0x14, weap, tires[4] 0x1C, lf/lr/rf/rr/bSpark 0x2C..0x3C);
D_801427C0 gObjList; D_8011AF90 gTrackColors; D_8011B468 gTrackBaseColors (s32[19]); D_8011ADC0 SheenColors
[track][2]; D_8011AE58 Track1Colors; D_8014978C gTrackNum; D_801543CC gModelTime; D_8002EB94 gFrameSeconds
(volatile); D_801170FC gPauseState; D_8011735C rand seed; D_8011418C unitmatrix (D_801141B0 = unitmatrix[3]);
D_8015B268 PolyInstanceList (0x58); D_8011AD8C gShadowColor; D_8011AD90 gDummyVtx; D_80161368 gTexList;
D_8011B558 arrowColors[5]; D_8012E67C gCarTeam; D_8014A110 gRace; D_80151AD0 gNumWindows; active_player_count
gNumPlayers; D_80153E88 gLink (8 B: car +1, owner +7); D_8011157C gTireRims[][13]; D_80143F74 gWheelTextures;
D_80140418 gInvisibleCar; D_8010FFC0 the EmitGameSound enable flag; D_8011B550/B554/B574/B578 the clr
initialisers of StartNewBlast/StartNewBlastRing/StartArrow/AnimateArrow; tCarData (player_array, 952 B): pal[4]
0x344, IRclr 0x34C, prevIRclr 0x350, sheenAlpha 0x360, visuals[22] at 0xF8; tModelData (0x808): crashflag 0x640,
moving_state 0x6C4, in_game 0x7C8.

## Notes (running)

### entity_tick_main = AnimateRocketFlame (builds on Astra #391, refs/pr/391 mat3_rocket_frame_20261017)
- #391 group re-scored in the current tree: entity_tick_main 47/671 (stack immediates only), frame 352 vs 368.
- Retail frame map (top = 368): m/car/mat slots 364/360/356, flames[3] 344..352, clr 340, scale1 336, pos 324..332.
  There is NO slot for the GC's unreferenced `float r` (the GC DWARF lists it; the N64 frame does not).
- Removing `r` gives frame 344 with every named offset 24 B low; six more 4-byte slots below `b` give
  `entity_tick_main: MATCH | ALIGNED rows differ 0 ... frame -368/-368` (gscore, group = #391 context).
- Per-site cost measured: one inlined ZOID_SetColor (2 params) = 8 B (2 slots). The 6 slots are below pos/mph/b
  (inline area): candidates are 3 more inlined 2-parameter helper sites; not identified yet.
- Whole-program unit: the locked singles func_8008B3A0 (&D_8012E708 base) and func_8008E06C (&D_8012E73C base)
  give separate table-base webs; with D_8012E700-based helper forms in --with, the unit reproduces the group
  (entity_tick_main 47/671 in the unit with the #391 body).
- In the unit the locked func_80092BF4 (SetSheenColor) is NOT inlined into drone_ai_update (retail inlines it);
  with the MAT3 form (car local + ZOID_SetColor + SetEnvColor calls) it is (unit/ctx_helpers.c).

### drone_ai_update = AnimateFrame: MATCH in the #391 group context (2026-10-21, rocket/m1/)
Levers, in the order they were found (each measured with gscore in the group):
1. Frame map: retail 400; named offsets hulk 396, slot 392 (s16 at -8: a 2-byte slot before it), def 380, hobj 372,
   sobj 360, clr 356, sclr1 352, sclr2 348, nextclr 332; inline GetCarsIRColor clr 308, sclr2 300, rgb 296.
2. GetCarsIRColor: no `colors` local (the table expression D_8011AF90[gTrackNum] is one CSE web, retail a3);
   a named sum variable for each channel (retail v0); index variables declared before the blend a/b
   (505 -> 304 words).
3. SetEnvColor (func_80092BC8) writes through an object pointer local: `o = &ObjInstanceList[objnum]; o->env = c`.
   This is the locked single func_80092BF4's "second store through a ModelSlot * local" form; it gives the
   second inlined SetSheenColor store its base-register form and the frame its real inline slots (130 words,
   then 49 after giving back 4 filler slots).
4. PickCar (func_80092B80) declared `int` (GC: int PickCar) -> `beq v0,def` operand order (47).
5. `tex = ObjInstanceList[hobj].index` named (retail keeps it in v1 before the branch) (30).
6. A compiled-out read `if (car->lightIdx) {}` before `flags = ZOID_ObjectFlags(hobj)`: uopt PRE then places the
   camera load before the index test and reloads it after ZOID_SetObjectDef's store (retail shape), which makes
   the single camera web interfere with tex (v1) and colours it a3; t0/t1 follow (MATCH).
   Forced-colour oracle first: p1:w505=c6 alone left only this PRE/schedule difference (14 rows).

### DELIVERABLE: group codex_visual_tire_module_20261002 (supersedes the installed group IN PLACE)
Dir: cloud/work/frontier/w36l/groups/codex_visual_tire_module_20261002/ . Old members (matrix_scale_apply,
anim_state_update, buffer_swap, entity_anim_texture) and their 7 files are byte-identical to src/blob; new files
rocket.c, func_80092BF4.c, func_80092BC8.c. claims: entity_tick_main, drone_ai_update. New context:
func_8008E06C, func_80092BF4, func_80092BC8, func_8008AE64, func_8008AE48 (all kept). GetCarsIRColor and
ZOID_SetObjectDef are internal (defined in the group, not kept; deleted stubs carry symbols).

`score.py group cand/grp_tire` (builder, w36l scratch):
```
matrix_scale_apply: MATCH / anim_state_update: MATCH / buffer_swap: MATCH / entity_anim_texture: MATCH
entity_tick_main:
  MATCH
    own .data verified at 0x8011B464 (4 bytes; .data is laid out per translation unit)
    own .rodata verified at 0x80123A00..0x80123A60
drone_ai_update:
  MATCH
    own .rodata verified at 0x80123A60..0x80123A64
exit=0       (and `--claims`: both claims MATCH, exit=0)
```
Context (informational): func_8008B3A0 2/10 (the installed entity_anim_texture.c one-line copy), func_8008B2B4
12/12 and func_80091B00 11/42 (as in the installed group), the five new helpers MATCH.

Whole-program unit (Pi): `blob_unit --tag w36l score drone_ai_update entity_tick_main matrix_scale_apply
anim_state_update buffer_swap entity_anim_texture func_8008E06C func_80092BC8 func_8008B3A0 func_8008AE64
func_8008AE48 func_80092BF4 --with G/rocket.c --with G/func_80092BF4.c --with G/func_80092BC8.c
--with singles/func_8008B3A0.c --internal entity_tick_main --internal GetCarsIRColor --internal ZOID_SetObjectDef
--neighbours`:
```
  EQUAL drone_ai_update: 820 words (kept, c_rocket.c)
  EQUAL entity_tick_main: 671 words (internal, c_rocket.c)
  ... (matrix_scale_apply, anim_state_update, buffer_swap, entity_anim_texture, and the six helpers EQUAL)
  locked bodies that differ in this unit: 0
blob_unit score: 12/12 equal
```

### INTEGRATION NOTE (required): replace six locked singles' sources
The unit keeps one definition per name and the locked singles win. Four of them are m2c forms whose inlined
copies use other table bases (&D_8012E708 node, &D_8012E73C colour, D_8012E700 + `arg0 * 0x44`), and the
locked func_80092BF4 (direct stores) is not inlined by umerge into AnimateFrame. Replacement sources (struct
form against D_8012E700, one statement per line; as1 schedules by source line) are in
cloud/work/frontier/w36l/singles/: func_8008B3A0.c, func_8008AE64.c, func_8008AE48.c, func_8008E06C.c,
func_80092BC8.c (each `score.py fn ... --flags "-g0 -O2 ..."`: MATCH, the lock flagset) and func_80092BF4.c
(-O3, lock flagset; repeats the two accessors so umerge inlines them: MATCH). With them in the unit:
`blob_unit ... score func_8008E06C func_80092BC8 func_8008B3A0 func_8008AE64 func_8008AE48 func_80092BF4
--with unit/helpers.c --neighbours` -> 6/6 EQUAL, "locked bodies that differ in this unit: 0".
Without the replacements the unit gives entity_tick_main 603/671 and drone_ai_update ~720/820.
Also: rocket.c declares PickCar `int func_80092B80(s16, u8)` (the locked single defines u16; MAT3 `int PickCar`).

### entity_update_callback = AnimateNewBlast: MATCH (cloud/matches/entity_update_callback.c)
Identity: gPauseState test, lifeTime (0x188) -= gFrameSeconds, moving_state/crashflag, 4 x 0x54 blast objects,
glowIdx 0x150, alpha/alphaCnt 0x184/0x185, incTime 0x18C = 0.0333333f, Pitch/YawUV calls: GC AnimateNewBlast.
Path: typed MAT3 port (nb1, 511 words, frame 192) -> volatile gFrameSeconds (473) -> forced-colour oracle:
p1:w257=c3,p1:w245=c21,p1:w256=c22 leaves 0 code rows (gBlastObjs base must take s7, player_array s8, the 952
stride a0) -> vbatch b1 (300 variants, 8 axes): every strict-0 file has `if (b->mbIdx == -1) continue;` in
the update loop; no other axis matters. Frame: GC ZOID_RemoveObject -> ZOID_RemoveObjectTree -> _RemoveObjectTree
inlined (4 slots per site, 3 sites) replaces 12 of 16 filler slots; u0..u3 remain (disclosed in the header).
`score.py fn cand/entity_update_callback.c entity_update_callback --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
```
entity_update_callback:
  MATCH
    own .rodata verified at 0x801239E0..0x801239F0
```
`blob_unit --tag w36l score entity_update_callback --with blast/final.c --neighbours`:
`EQUAL entity_update_callback: 546 words (kept, c_final.c)`, `locked bodies that differ in this unit: 0`.

### music_seq_load = CreateCar: 34/357 (music_seq_load/best.c)
`fs.sh ... music_seq_load` (gscore = score.compare): `34/357 words differ | ALIGNED rows differ 36 (norm 0) frame -192/-192`;
blob_unit (with the file's helper forms): `FAIL music_seq_load: 34 of 357 words differ`, 0 locked bodies differ.
From w27d's 323/357 (arcade-shaped draft). See the file header for the levers and the residual (ugen temp free
list at the ZOID_SetTexture line: retail allocates t1,t2,t3,t8,t9 where ours allocates t8,t9,t1,t2,t3; all
registers before it match). vbatch b1 (300) + b2 (128) + prototypes: no change.

### StartNewBlast / StartBlast / AnimateBlast group (blast3/g9, group "w36l_blast", members func_80090308,
entity_physics_update, save_write_data; context func_80090770 = ZOID_SetObjectDef)
- entity_physics_update (0x800908A0, 712 B) = AnimateBlast: **MATCH in the group and EQUAL in the unit**
  (`blob_unit ... score func_80090308 entity_physics_update save_write_data --with blast3/b1.c --internal
  func_80090308` -> `EQUAL entity_physics_update: 178 words (kept, c_b1.c)`). Not on my list but it is the
  caller of func_80090308 (w26c had it at 21 words, colour only). PROVISIONAL: its prologue saves s0-s8 because
  the internal StartNewBlast clobbers them, so it can only be claimed once func_80090308 lands with it.
- func_80090308 = StartNewBlast (GC 0x80057998): 169/282, normalised 8 rows; frame 272 exact.
- save_write_data = StartBlast (GC 0x800585E4): 169/300, normalised 42 rows (body 288/300); frame 352 exact.
Frame findings (they explain w26c's 72/104 B gaps): (1) StartNewBlast and the inlined StartNewBlastRing share
one local template `tVisual *v; int i; tUVMatrix mat; tRGBA clr;` (retail v -4, clr -60 in both; the 48-byte
`mat` is unused: disclosed filler of a real type); (2) MBOX_NewObject is an inline 4-parameter wrapper around
func_8008E26C (4 slots per site); (3) ZOID_RemoveObject -> ZOID_RemoveObjectTree two-level chain; (4) the
ring and glow colour stores go through an int-index colour setter (no s16 narrowing in retail), the blast
pieces through ZOID_SetColor(s16) (lh of the low half); (5) EmitGameSound guard is an inline 10-parameter
wrapper (`if (!D_8010FFC0) return; if (snd == -1) return;`); (6) StartBlast declares `v, snd, mat`.
Open: StartNewBlast seed reload before the 4th Random (`sw t9,0(s1); lw t6,0(s1)`; w26c saw it too) and an
s2/s3 swap; StartBlast hoists MBOX_NewObject's constant arguments above `if (linkToCar)` in retail (ours after),
which also lets retail multiply by 952 with shifts (no free register for a 952 web); 2 slots short in StartBlast
with the wrapper-without-local form.

### music_control = StartEnvironment: MATCH (cloud/matches/music_control.c)
Identity: two 0..5 loops over model[i].in_game (0x7C8): CreateCar(i, 15, 1) + StartCar(i); then StartShadow
(visuals[11] at gGameCar+0x200, func entity_process_main, gShadowColor (0x8011AD8C) alpha 192, ZOID_NewPoly
(func_800A78BC) with gTexList[6]) and StartArrow (visuals[12..13], AnimateArrow = entity_render_setup,
arrowColors D_8011B558 by gCarTeam in battle, 0x3200/0x1200 flags, clr initialiser at 0x8011B574).
`score.py fn cand/music_control.c music_control --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
```
music_control:
  MATCH
    own .data verified at 0x8011B574 (4 bytes; .data is laid out per translation unit)
```
`blob_unit --tag w36l score music_control --with startenv/se7.c --neighbours`: `EQUAL music_control: 248 words
(kept, c_se7.c)`, `locked bodies that differ in this unit: 0`. (final.c = se7.c + header comment.)
Identities: audio_frame_update = StartCar, entity_process_main = AnimateShadow (callback of visuals[11]).
