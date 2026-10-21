# w36u RESULTS (wave 36, game render/world lane)

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_8009F058 | N64 per-window scene setup (Camera = tWindowInfo ZoidWinInfo D_8017A510); no GC body | 5228 | **MATCH (group, own .rodata verified) + unit EQUAL** | groups/frontier_particle_knot/ (supersedes installed group in place) |
| func_800E847C | update_game_data@mdrive.c (GC 0x8002B064) — CONFIRMED | 2100 | 524/525 words, 178 rows: only the s-register shift that follows from the callee | update_game_data/best.c |
| particles_update | animCoins@hud.c (GC 0x8001FC2C; the N64 version still contains animCoinsText) — CONFIRMED | 1464 | unit 365/366 words, 188 rows; frame/homes exact only with a probe wrapper (lead) | particles_update/lead_area128.c |
| func_800E7FA0 | _UpdateTireAppearance@mdrive.c (GC 0x8002B6CC) — CONFIRMED | 1244 | MAT3 port: 312/311 words, 37 rows, colour-only (force oracle: 9 rows) | update_game_data/best.c |
| physics_float_calc | WorldCullObjects@world.c (GC 0x8005E2F4) + inlined static WorldDistance — CONFIRMED | 1176 | 86 rows off (same size, 294/294 words), parked (declaration order and `register` have no effect) | physics_float_calc/best.c |
| car_shadow_render | AudioInitSys@audiosysps2.c (N64 platform version; AudioSfxInit tail) — CONFIRMED | 836 | **MATCH (group)** + unit EQUAL | groups/w36u_audio_init_sys/ |

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36u (copied from base, master e68aea6e1).

## Identities proven
- car_shadow_render = AudioInitSys (MAT3 audiosysps2.c, GC 0x8009ED90; N64 platform body, shared tail
  AudioSfxInit(maxSfx) = func_800A4CB8). Callees: LoadWave = audio_frame_sync, LockWaveInstance =
  display_list_alloc, _ValidateWaveInstance = func_80096288 (3 args, `beqz a2`), GetWaveInstanceAddr (waves.c)
  umerge-inlined, gWaveInstance = D_80156D38 (20-byte records, ptr at +0xC); the two threads are
  task_complete_signal (id 3, pri 10) and dma_wait_complete (id 9, pri 9).
- func_800E847C = update_game_data (MAT3 mdrive.c, GC 0x8002B064). N64 tModelData = GC layout exactly (0x808);
  N64 tCarData = GC layout up to 0x110, visuals[21] (GC 22) so later fields are -0x18 (collidable 0x308, solid
  0x309, collide_time 0x30C, collide_count/state 0x310/0x311, sound_flags 0x344, dead 0x359, in_tunnel 0x35A, stuntW
  0x368), size 0x3B8. model=D_8014A250, gGameCar=D_80152818, gLink=D_80153E88 (8-byte records, owner at +7),
  gModelTime=D_801543CC, carSuspensions=D_80111560, gCarSuspensions=D_8011156C, game_reckon_all=battle_mode_setup,
  CopyUV=math_utility, PitchUV=func_80090F44, RollUV=func_8009EA68, WorldVector=func_8009E820; the N64 brackets
  the body with osStopThread/osStartThread(D_80034840) (model thread hold).
- func_800E7FA0 = _UpdateTireAppearance (MAT3 mdrive.c, GC 0x8002B6CC), static callee of update_game_data
  (w2e's disassembly-derived body has the same logic: sviscode/sound_flags, sSmoke/sSmoke2/sSkid/sSkidData tables =
  D_80120EBC/ECC/EDC/EEC, gCoastFlag = D_8013FECB, gRace = D_8014A110 (kBattle = 6), tune->skidk at +0x20).
- particles_update = animCoins (MAT3 hud.c, GC 0x8001FC2C) + animCoinsText merged (the GC split them):
  gPlayerCoin = D_801541A8, ZOID_WindowTo3D = car_lights_render, ScaleUV = func_8008B32C, YawUV = func_80090E9C,
  hudObjAngle = D_80116178, MBOX_FindObject_WORK = string_copy_format, MBOX_NewObject = func_8008E26C,
  MBOX_HideObject = model_data_load, MBOX_ShowObject = model_transform_setup, whichCam = D_80114C48,
  NumCoinsFound = func_800B78F0, TB_Sprintf = fcvt_wrapper, TB_SetTextColor = dispatch_handler,
  TB_DrawText = state_utility, HAL_SetTextDepth = render_helper, TB_SetJust = (inlined) D_80118E20 pair,
  HAL_SetFont = inlined font lock + slot_state_setup (= _HAL_SetFont), coinPos = D_80115E28,
  coinStuntPos = image-A D_80394050, gRace == kStunt is `gameplay_mode == 4`.
- physics_float_calc = WorldCullObjects (MAT3 world.c, GC 0x8005E2F4) with the static WorldDistance (GC 0x8005E0FC)
  inlined; the caller-less `jr ra; nop` at 0x8009EBB8 is WorldDistance's deleted body. Evidence: 0x68-byte
  tModelPart records (pos 0x34, next 0x44, min 0x50, max 0x5C), 0.75f * max[1] test, sqrtf(dx*dx+dz*dz), 19-case
  per-track parts switch, trackParts u8[19] and showAllParts u32[4], bit 31 of ObjInstanceList flags. N64 globals:
  WorldBase=D_80149B80, gTrackNum=D_80149B8C, Worlddlidx=D_80149D90 (s16), gInvisibleTrack=D_8014061A (s8),
  ObjInstanceList=D_8012E700 (0x44-byte records on the N64, 0x60 on GC), trackParts=D_8011E748, showAllParts=D_8011E75C,
  track1Parts..obstacle1Parts = D_8011B898, D_8011BFE8, D_8011C738, D_8011CE88, D_8011D618, D_8011DC88, D_8011E428,
  D_8011E438, D_8011E448, D_8011E458, D_8011E468, D_8011E548, D_8011E558, D_8011E568, D_8011E578, D_8011E588,
  D_8011E598, D_8011E5A8, D_8011E5B8 (switch order 0..18 = track1..6, dm1..8, stunt1..4, obstacle1).

## Notes per function

### car_shadow_render (AudioInitSys) — MATCH as group `w36u_audio_init_sys`
- `score.py group cand/grp_ais` (= `groups/w36u_audio_init_sys`, with and without `--claims`):
  `Members:` / `car_shadow_render:` / `  MATCH` (context func_80096288 .. func_800A4CB8 MATCH except
  `display_list_alloc: MISMATCH (1 extra words (nonzero beyond target length))`, informational, excluded from exit status).
- Unit: `blob_unit --tag w36u score car_shadow_render --with groups/w36u_audio_init_sys/car_shadow_render.c` ->
  `EQUAL car_shadow_render: 209 words (kept, c_car_shadow_render.c)` / `blob_unit score: 1/1 equal`.
  With `--neighbours`: `locked bodies that differ in this unit: 2: dma_wait_complete, task_complete_signal` and
  `still unaligned after 4 builds (align / data_align): task_complete_signal` — the documented position-dependent
  32-byte epilogue padding of the two thread entries (unit_overrides `align` reasons); the same two appear with
  any car_shadow_render body (also with the 2-word-off variant), so it is the shadow-unit layout, not this body.
- Standalone `score.py fn`: 41/209 — the locked callees func_80096288/display_list_alloc must be in the compile
  (their IPA register facts keep the thread/stack constants in ugen temps), hence the group with byte-identical
  context copies of src/blob/groups/slot_sound/{group.c, func_800A4E58.c, entity_render_mode.c} and
  src/blob/func_800A4CB8.c (keep = car_shadow_render + slot_sound's keep list + func_800A4CB8; func_80096288 stays
  internal as slot_sound requires).
- Levers from A142's 72/209: GetWaveInstanceAddr inlined (31 words -> 10 rows), static-lib prototypes from src/rom
  (func_80020274 takes no argument: the 22050 in the beqz delay slot is the else path's), a separate counter for
  the 4-entry instrument loop (end address folds to &D_8011F0A0[4]), and the ROM-offset symbol D_00039B40 for
  the last 2 words (lui 0x4 / addiu -0x64C0 = symbol address). Coordinator: D_00039B40 is a new absolute symbol
  name (asset ROM offset 0x39B40, inside data_ROM 0x10000..0xC00000); it needs a definition/approval before splice.
- With another lane's new cloud/matches/tire_compound_set.c (= InitWaves) in the same unit:
  `blob_unit --tag w36u score car_shadow_render tire_compound_set --with groups/w36u_audio_init_sys/car_shadow_render.c
  --with cloud/matches/tire_compound_set.c` -> `EQUAL car_shadow_render: 209 words` / `EQUAL tire_compound_set: 93 words`
  / `2/2 equal` -- the two land independently.
- Frontier listed audio_frame_sync (register param v0) and tire_compound_set as blockers; both are unlocked, so the
  unit compiles them as external ABI calls and the body is still EQUAL. If audio_frame_sync is matched later its
  IPA facts must be rechecked against this body.

### physics_float_calc (WorldCullObjects) — not matched, parked after round 1
Scorer (standalone, full.py aligned rows): best.c = 294/294 words, 86 differing rows (bscore strict 80).
Retail facts the C must reproduce (from the disassembly):
- frame 136, ra+s0 saved although there is no call: a call that uopt deletes (a constant-0 debug flag) is in the
  parts loop; without it the loop unrolls by 2 (379 words) and the frame is 80. `if (0)` is folded by cfe (no
  effect); a local `dbg = 0` works. The w14f draft found the same.
- loop 2 has FOUR induction variables: a3 (step 68, init 0, dead, incremented at the loop top), s0 (ObjInstance
  pointer, init &ObjInstanceList[Worlddlidx] computed after the loop guard), a2 (step 4, init 0, dead) and t0 (ptr
  walk, init = ptr copied from t2). ptr is a REGISTER web through the switch (`lw t2,100(sp)` on the undefined
  path), and show is reloaded from its home every pass.
- The MAT3 form (`ptr[w]`, `ObjInstanceList[Worlddlidx + i]`) gives the right IV count only with the
  `ptr_no_clip ? mask & ptr_no_clip[w] : 0` clause present (ptr_no_clip = 0 folds it but leaves the w*4 IV = a2);
  but with `ptr[w]` uopt keeps ptr in MEMORY (every case stores it). `ptr++` keeps ptr in a register but loses the
  t2->t0 copy and the a2 IV. Nine sweeps (b1-b9, ~2,000 variants: walk form, ptr_no_clip forms, object
  addressing, dead-call position/arguments, default case) — best b7/v0172 (= best.c).
- Next hypothesis: find what gives `ptr` more register weight in the `ptr[w]` form (the remaining structural
  difference), then the frame slot order (retail ptr home 100, dx/dy/dz at 60/64/68).

### update_game_data + _UpdateTireAppearance (func_800E847C + func_800E7FA0) — one group, not matched yet
Group compile (`full.py SRC NAME --keep func_800E847C`, = blob_unit with `--internal func_800E7FA0`):
`func_800E7FA0: want 311 words, got 312; differing rows 37`, `func_800E847C: want 525 words, got 525; differing rows 230`.
Unit: `blob_unit --tag w36u score func_800E847C func_800E7FA0 --with update_game_data/v/u4.c --internal func_800E7FA0`
-> `FAIL func_800E847C: 229 of 525 words differ` / `FAIL func_800E7FA0: 40 of 311 words differ` (u4 = w2e callee).
Levers found (MAT3 port -> these):
- zero constants: update_game_data's angle compares/`angle = 0`/collide_time are written with int `0` (they share
  retail's $f24 web with `if (m->brake)`), stuntW zeroing with `0.0f` (its own $f28 web); 20.0f in $f26.
- MAT3 operand order of the force sums (`tf[0]+tf[1]`, `tf[2]+sum`, `tf[3]+sum`) on a 3-float stack array
  `force[3]` declared between gc and angle (retail frame: i 182 .. same_count 164, m 160, gc 156, force 144, angle
  140, susp 136, tsf 134 — exact); `gc->dr_tirepos[j][k] = gc->dr_pos[k] + gc->dr_tirepos[j][k]`;
  `collide_state = -1; collide_count = -1;` as two statements.
- callee: forceLow declared before force (force's home at 0(sp)); `skidData = &sSkidData[side]` BEFORE the switch
  (skidData IV t0 / force t1 as retail); `if ((m->skidIntensity[side] = ...) > skidThreshold)`; every zero in the
  main loop the same spelling (`= 0`, `<= 0`, implicit `if (skidIntensity)`).
Residual: callee colouring. Forcing p2 w166(forceLow)=t4, w260(skidIntensity IV)=t5, w285(&forceTable[index])=s0,
w288(forceTable[index+1])=s1, consts 3/6/&sSmoke/4 = s2..s5 leaves 9 rows (`force.sh g11 ...`). In retail some
web holding t3 interferes with forceLow and the IV, so the index pointer is pushed into s0 and the callee clobbers
s0-s5; ours clobbers s0-s4, which is exactly why update_game_data colours m/gc/tsf/12 one register lower
(retail m s6, gc s7, tsf s8, 12 s3, &same_count s4, 8 s5). Fix the callee first; the caller should follow.
Caller round 2 (best.c = v/u16.c): the loop body is `if (m->in_game && gc->dead < 2) { ... }` (not `continue`):
retail's loop-top reload of i (`lh v0,182(sp)`) appears; `gc->dr_tirepos[j][k] = gc->dr_tirepos[j][k] +
gc->dr_pos[k]` gives retail's load order. Group: `func_800E847C: want 525 words, got 524; differing rows 178`,
callee `want 311 words, got 312; differing rows 37`; unit (`--internal func_800E7FA0`): `FAIL func_800E847C:
208 of 525 words differ; compiled body is 524 words` / `FAIL func_800E7FA0: 116 of 311 words differ` (positional).
Every remaining caller row is the s-register shift (m/gc/tsf and the three constants re-materialised after the
call: retail li s3,12 / addiu s4,sp,164 / li s5,8; ours keeps 12 in s8, so it is one word short) -- i.e. the caller
is colour-complete once the callee clobbers s0-s5. Callee sweeps (b3-b6: decl order/types, loop forms, explicit
`ft` pointer, intensity temp, compare spellings; ~450 variants) found no source form for the t3-holder.

### particles_update (animCoins + animCoinsText) — not matched
- Unit (`us.sh`, whole program): w27a best re-scored `FAIL particles_update: 266 of 366 words differ; compiled body
  is 365 words, target 366 ... | words 194 ops 6 norm 6 | frame 256/256`.
- Retail frame decoded: pos 252, buf 244, player/coin shorts 240..243, block `names` 232, homes at 120/108 =
  home1/home4 => retail cfe named area (uopt `AREA f_readnxtinst`) is 128; ours 116 (rec removed, x/y in the text
  block). Measured (spill.sh): nested inlined lock/unlock calls cost ~10 B each, a pointer local in an inlined
  helper ~6 B/site, int params/int locals of inlined helpers 0. A 2-site float-parameter wrapper around
  render_helper (`static void text_depth(f32)`, NOT a natural source -- render_helper is HAL_SetTextDepth itself)
  gives exactly 128 and retail's homes (`lead_area128.c`: `words 188 ... frame 256/256`). The real +12 B source is
  still unknown.
- With the frame right, the remaining structural difference is ONE missing word: retail loads gNumModels as
  `lui t9,0x8014; addiu t9,t9,3036; ... lbu a2,0(t9)` (address as a rematerialised address web, as in
  camera_shake_start where &D_80140BDC is a register candidate) where ours folds `lui a2; lbu a2,3036(a2)`;
  everything after it is shifted by one word (the arm order w27a suspected is fine). Tried: array/`*(u8 *)&`
  declarations, compiled-out `if (D_80140BDC) {}` holders (one variant reshuffled the init block), an inlined
  pointer-parameter MBOX_FindObject wrapper (umerge inlined it; no change). Next: find the construct that makes
  &gNumModels an address candidate (a second use in the same loop nest).

### func_8009F058 — MATCH, superseding group `frontier_particle_knot` (claims: func_8009F058)
- `score.py group cand/pk --claims` (= groups/frontier_particle_knot): `Members:` / `func_8009F058:` / `  MATCH` /
  `    own .rodata verified at 0x80123B60..0x80123B68`; context render_display_list, func_80099B30, func_8009C3F8,
  camera_update_c, select_screen_update, particle_system, track_collision_wall, Input_ProcessGameplayPad,
  func_800BFD8C all `MATCH`; exit 0 (also without --claims).
- Unit: `blob_unit --tag w36u score func_8009F058 render_display_list particle_system track_collision_wall
  Input_ProcessGameplayPad --with groups/frontier_particle_knot/group.c --internal func_8009F058 --internal
  render_display_list --internal func_80099B30 --internal particle_system --internal track_collision_wall
  --internal func_8009C3F8 --neighbours` -> `EQUAL func_8009F058: 1307 words (internal, c_group.c)` ... 5/5 equal,
  `locked bodies that differ in this unit: 0`.
- The fix (diff vs installed group.c is 3 lines, all in func_8009F058): declare `Camera *cam = &D_8017A510[view];`
  BEFORE `Vp *vp = &D_8015B250->vp[view];`, and replace the ortho arm's unused `s32 pad1;` with the natural
  `Mtx *m = &D_8015B250->proj[view];` passed to guOrtho (identical code, same slot, so perspNorm stays at 322).
- Mechanism (spill.sh): f_spilltemps gives every coloured expression web a 4-byte home in itable
  (first-occurrence) order, reusing a home only when the webs share no block. With vp declared first,
  view*16 (s3) is web 5 -> home 0 and the spilled &gCamera[view] (t0, web 201) conflicts with it -> home 1 = 68(sp).
  With cam first, &ZoidWinInfo[view] takes home 0; web 201 does not conflict with it -> home 0 = 72(sp) = retail.
  Breaking the vp/vtx sharing of view*16 also gave home 0 but changed the code (frame 392).
- Integration: supersede src/blob/groups/frontier_particle_knot in place (same name, same members + func_8009F058
  as a member and the only claim, context emptied); func_8009F058 must stay internal (unsaved s0-s8).

## What generalises
- **Spill-home slots follow the declaration order of pointer locals initialised at their declaration.**
  f_spilltemps homes every coloured expression web in itable (first-occurrence) order; the initialiser of the
  first-declared pointer is the first expression. func_8009F058's 22-word "one slot off" residual (three waves,
  w6a-w8a) closed by swapping `Vp *vp = ...; Camera *cam = ...;` -> `Camera *cam = ...; Vp *vp = ...;`. Check
  with `spill.sh LABEL NAME` (HOME bit=... home=N) before scoring. The same mechanism probably explains the
  "unexplained frame filler" pads kept in particle_system/track_collision_wall.
- **A cross-file helper umerge-inlines in the real unit** (GetWaveInstanceAddr from waves.c into AudioInitSys):
  its return value lands in v0, which is what made car_shadow_render look like it fed audio_frame_sync a v0
  register parameter. Define the helper (non-static, off keep) in the candidate.
- **Static-library prototypes come from src/rom** (func_80020274 takes no argument); a wrong argument puts a
  constant in the call's delay slot.
- **An address that retail builds with lui/addiu-negative-low is a symbol address, not an integer** (IDO builds
  integers with lui/ori): car_shadow_render's 0x39B40 is the ROM offset of an asset (D_00039B40).
- **Locked callees with known register usage must be in the group compile** (car_shadow_render: slot_sound's
  func_80096288/display_list_alloc keep thread/stack constants in ugen temps); standalone gave 41/209, group MATCH.
- `if (cond) { body }` instead of `if (!cond) continue;` decides whether a memory-resident loop counter is
  reloaded at the loop top (update_game_data's i).
- Zero spellings split FP constant webs: `if (m->brake)` shares the int-`0` constant web, not `0.0f`.

## Permission denials
none
