# w36q — screens (wave 36), IDO 5.3 -O3, game code (whole-program unit)

Status: FINAL (wave 36). **New coverage ready to install: 4,056 B in three group packets** (drone_pathfind_main 1,124 +
particle_collision 1,012 + particle_render 448 + func_800B4FB0 1,472). Builder scratch `watchman2:~/rush2049/scratch/frontier/w36q` (from base e68aea6e1). Unit tag `w36q`.

| function | GC identity | bytes | state | best file |
|---|---|---:|---|---|
| func_8010F218 | animText@videoscr.c (+drawColorBars) | 2,444 | not reworked (w33b 324 rows); unroll analysis below | cloud/work/frontier/w33b/videoscr/best.c |
| func_8010EA08 | animButtonText@videoscr.c | 2,056 | not reworked (w32b 287 rows); private callee of F218, lands only with it | cloud/work/frontier/w33b/videoscr/best.c |
| replay_save_prompt | **initScreen@optionscr.c** (proven, w36n) | 1,540 | unit: 7 words, ops 0, frame exact: initButtons locals 84 B too high (umerge area order) + 1 temp register (a3 vs v0) | option_init/best.c |
| func_800B4FB0 | **setPauseState@pausescr.c** (+ updatePauseStateCreate inlined; proven by the match) | 1,472 | **MATCH in group** (new `frontier_pausescr_state`); unit EQUAL | groups/frontier_pausescr_state/pause.c |
| drone_pathfind_main | initScreen@audioscr.c (proven; Astra #411) | 1,124 | **MATCH in group** (superseding `audio_options_menu`); unit EQUAL 281 words, 0 locked bodies differ | groups/audio_options_menu/init_w36q.c |
| particle_collision | initScreen@videoscr.c (proven) | 1,012 | **MATCH in group** (superseding `physics_sym`; code identical, two-region own .rodata NOT VERIFIED, splice-proof replay image-identical); unit EQUAL | groups/physics_sym/video_init.c |
| drone_collision_avoid | calcMenuRect@audioscr.c (proven; Astra #411) | 852 | unit: 5 words, ops 0, frame exact: the menuIdx*4 IV home at sp+28 vs retail sp+32 (inline-area alignment) | audio_init/rect_best.c |
| particle_render (coordinator add) | calcMenuRect@videoscr.c (w36n) | 448 | **MATCH in group** (`physics_sym`; own .rodata verified); unit EQUAL | groups/physics_sym/video_init.c |
| particle_lifetime_set | **createBG@front.c** (proven) | 732 | ops 0 (structure identical), 65 strict words: s0-s2 priority + temp ring; D_801391E4 address tie (best.c stores through an alias symbol: diagnostic, not a candidate) | particle_lifetime_set/best.c |

## All three packets together in the unit
`blob_unit --tag w36q score drone_pathfind_main particle_collision particle_render func_800B4FB0 func_800D7634 physics_sym
--with groups/audio_options_menu/init_w36q.c --with groups/physics_sym/video_init.c --with
groups/frontier_pausescr_state/pause.c --internal drone_pathfind_main --internal di_setButtonColor --internal di_post_type1
--internal di_ProcessCommand --internal particle_collision --internal particle_render --internal vi_gfx_lock --internal
vi_gfx_unlock --internal vi_SetFont --internal vi_setButtonColor --internal ps_gfx_lock --internal ps_gfx_unlock --internal
ps_font_set --internal ps_SetFont --internal ps_updatePauseStateCreate --neighbours` -> all six `EQUAL` (drone_pathfind_main
281, particle_collision 253, particle_render 112, func_800B4FB0 368, func_800D7634 451, physics_sym 313), `locked bodies
that differ in this unit: 0`, `blob_unit score: 6/6 equal`.

## Identities proven
- func_800B4FB0 (0x800B4FB0) = GC setPauseState@pausescr.c (MAT3 0x80078F08) with updatePauseStateCreate (0x800788A4)
  inlined; func_800B4E68 = updatePauseStateCleanup; voice_stop = _TitlePane@pausescr.c; func_80094FE8 = animRumblePane.
  Data: gPauseState D_801170FC, titlePos D_80117280 (s16 x,y), DimMultiBlit D_80117284, TitleMultiBlit D_801172A8,
  PauseMultiBlit D_801172CC, SureMultiBlit D_801172F0, RumbleMultiBlit D_80117314, DimBlit D_80117338, TitleBlit
  D_8011733C, PauseBlit D_80117340, SureBlit D_80117344, RumbleBlit D_80117348, gYes D_80117354 (s8), gPlayerRects
  D_80117100 [n][4] x 0x18 (cx +16, cy +20), gNumPlayers D_8014A108 (s16). Proven by EQUAL/MATCH.
- particle_collision = initScreen@videoscr.c, particle_render = calcMenuRect@videoscr.c, replay_save_prompt =
  initScreen@optionscr.c, drone_pathfind_main = initScreen@audioscr.c (all with initButtons inlined), proven by
  EQUAL/near-EQUAL; func_800B59E8 = validOption@videoscr.c getter (physics_sym group, already named there).
- particle_lifetime_set (0x800B8374) = GC createBG@front.c (MAT3 front.c 0x80013C00): 4-poly loop, "BACKGROUND_SFX",
  gBackCamera|0x1210, ZOID_ShowPoly inlined (flags &= 0x7FFF), offsetsValid tc copy, gGameState&0x80 picks
  trackBack/frontBack for NewMultiBlit (= sound_control, N64 passes nblits 2). N64 data: gBackRects D_80114264
  [4][4] x 0x38, gBackCamera D_801145E4 [4][4] s32, backBlit D_80114624, backPolys D_80114628[4], N64-only
  background texture D_80114638 (= MBOX_FindTexture_WORK's result, read by updateBG = func_800FC9F8), offsetsValid
  D_8011463C[4], backColor template D_8011464C {0,0,0xC0,0xFF}, frontBack D_801141D4 / trackBack D_8011421C (36-B N64
  blit defs), N64-only D_801391E4 (s32, set to 1025 here, converted to float as GC BG_DIST). func_800B24EC takes
  (char *name, u16 *idx, s8 first, s8 last, s32 flag) = MBOX_FindTexture_WORK; func_800A78BC = ZOID_NewPoly.


## Notes in progress
- umerge inlining is a GLOBAL knapsack (`f_pack_knapsack` in umerge): adding a call site of a shared kept helper
  (object_create, object_type1_create) anywhere changes which sites get inlined elsewhere (adding Astra's rect with
  `object_create(10)` breaks PrintScores' inlined font set; a helper-inlined initButtons drops out of drone_pathfind_main
  when the rect's helpers are added). Local non-static copies (du_/di_ helpers, house style of frontier_audioscr) and
  initButtons written in place avoid it: locked bodies that differ = 0.

## drone_pathfind_main — MATCH in group (packet `groups/audio_options_menu/`, supersedes in place)
- Builder `score.py group cand/audio_options_menu` (and `--claims`): `drone_pathfind_main: MATCH`, `own .data verified at
  0x80110660 (4 bytes ...)`, `own .rodata verified at 0x801241C0..0x801241D4`; old members unchanged: func_800D690C MATCH,
  func_800D7634 MATCH (own .rodata verified at 0x801241D4..0x801241E8), func_800D6914 the known `NOT VERIFIED (12
  section-relative relocations unverified ... 0x8011F70C ... 0x80124194 ...)` line (identical in the installed group),
  context func_800D6160 `65/76 words differ` (identical in the installed group); new context sign_extend_call MATCH,
  func_800B8810 MATCH.
- Unit (Pi): `blob_unit --tag w36q score drone_pathfind_main func_800D7634 func_800D6914 --with
  cloud/work/frontier/w36q/groups/audio_options_menu/init_w36q.c --internal drone_pathfind_main --internal di_setButtonColor
  --internal di_post_type1 --internal di_ProcessCommand --neighbours` ->
  `EQUAL drone_pathfind_main: 281 words (internal, c_init_w36q.c)`, `EQUAL func_800D7634: 451 words (kept, ...)`,
  `EQUAL func_800D6914: 315 words (internal, ...)`, `locked bodies that differ in this unit: 0`, `blob_unit score: 3/3 equal`.
- Integration: install_group.py cloud/work/frontier/w36q/groups/audio_options_menu audio_options_menu (supersede in place).
  members += drone_pathfind_main; claims = [drone_pathfind_main]; files += init_w36q.c, ctx_sign_extend.c (byte copy of
  src/blob/groups/dot_sign_extend/group.c), ctx_func_800B8810.c (byte copy of src/blob/func_800B8810.c); context/keep +=
  sign_extend_call, func_800B8810 (both locked, inlined into drone_pathfind_main as in retail). unit_overrides
  force_internal (if not derived): drone_pathfind_main, di_setButtonColor, di_post_type1, di_ProcessCommand.
- Devices (disclosed in the header): one `do {} while (0);` (address-web tie: &gNumModels must win s8 over unitmatrix),
  unused u0..u3 = exact frame residual (168 vs 152, 4 slots, all at the frame top; no sp-relative access changes).
- Credit: body from Astra's PR #411 init.c (strict in their 48-member research group). The unit needed: initButtons in
  place, local AudKillSong helpers instead of object_type1_create, local validOption/font helpers (umerge's global
  knapsack), the block break and the frame locals.

## particle_collision + particle_render — MATCH in group (packet `groups/physics_sym/`, supersedes in place)
- Builder `score.py group cand/physics_sym`: `particle_render: MATCH` (`own .rodata verified at 0x8012267C..0x8012268C`);
  `particle_collision: NOT VERIFIED (16 section-relative relocations unverified ...; own .rodata: this function's
  references disagree on the section's image address (0x80122650 for +0x3c/+0x44/+0x48, 0x80123D7C for
  +0x68/+0x6c/+0x70/+0x74): the literals are not laid out as in retail)` + `own .data verified at 0x8011AD88 (4 bytes
  ...)` — strings (0x8012268C "CURSOR", "B1", "BUTTON_TITLE") and floats (0x80123DE4..) are two retail regions, the
  func_800B59F0 precedent in this same group. Old members unchanged (func_800B55F4/func_800B5688/func_800B59E8/
  physics_sym/TitlePane MATCH, func_800B59F0 its known NOT VERIFIED line; context slot_state_setup 2/58,
  object_create 28/28, func_800B669C 5/5 differ exactly as in the installed group); new context sign_extend_call,
  func_800B8810 MATCH.
- Splice-proof replay (`tools/gproof.py`, read-only copy of w33b's; `groups/proof_physics_sym.log`): `group_bodies OK:
  TitlePane, func_800B55F4, func_800B5688, func_800B59E8, func_800B59F0, particle_collision, particle_render,
  physics_sym`; every member `0 differ from the image`; `8/8 members image-identical`. audio packet:
  `groups/proof_audio_options_menu.log` -> `4/4 members image-identical` (drone_pathfind_main 281 words, 0 differ).
- Unit (Pi): `blob_unit --tag w36q score particle_collision particle_render physics_sym func_800B59F0 func_800B5688
  --with cloud/work/frontier/w36q/video_init/v/a3.c --internal particle_collision --internal particle_render --internal
  vi_gfx_lock --internal vi_gfx_unlock --internal vi_SetFont --internal vi_setButtonColor --neighbours` ->
  `EQUAL particle_collision: 253 words (internal, c_a3.c)`, `EQUAL particle_render: 112 words (internal, c_a3.c)`,
  `EQUAL physics_sym: 313 words (kept ...)`, `EQUAL func_800B59F0: 343 words`, `EQUAL func_800B5688: 49 words`,
  `locked bodies that differ in this unit: 0`, `blob_unit score: 5/5 equal` (video_init.c = a3.c + header comment).
- Integration: install_group.py cloud/work/frontier/w36q/groups/physics_sym physics_sym (supersede in place); members +=
  particle_collision, particle_render; claims = both; files += video_init.c, ctx_sign_extend.c, ctx_func_800B8810.c;
  context/keep += sign_extend_call, func_800B8810; unit_overrides force_internal (if not derived): particle_collision,
  particle_render, vi_gfx_lock, vi_gfx_unlock, vi_SetFont, vi_setButtonColor. The TitlePane inline_blockers entry stays.
- Device: unused u0..u3 in particle_collision (frame 136 vs 120: same 16 B as drone_pathfind_main). particle_render
  needed nothing: first typed port EQUAL. Fixes found by the unit's own-data check: B1/arrow y is 55.2f (not GC 59.2)
  and the third texture is "BUTTON_TITLE" (N64), not GC "BUTTON".

## replay_save_prompt — 7 words (GC initScreen@optionscr.c + calcOptionMenuMaxSize + initButtons in place)
- `option_init/best.c` (= v/a3.c). Unit: `FAIL replay_save_prompt: 7 of 385 words differ | words 23 ops 0 | frame 256/256`.
- Residuals: (1) buttonColor/texIdx at sp+240/234 vs retail 156/150: same frame, retail has ~96 B of locals/inline areas
  ABOVE initButtons' locals (calcOptionMenuMaxSize's area and its two HAL_SetFont areas come first in retail's umerge
  inline order; in-place block locals are cfe locals and sit at the top). calcOptionMenuMaxSize as a helper is not
  inlined by the unit's umerge knapsack (v/a2.c, v/d2.c: `jal op_calcOptionMenuMaxSize`). (2) gControllerRemoved's
  byte load in a3 (retail v0). `volatile s8 attached` (as physics_sym's Pad16) fixed the `move a1,zero` schedule.
- Data: validOption D_80116D14[14] s32, gMenuCount D_80149D9C, gOptionMenuSize D_80149B84, gSelectedOption D_80116D9C,
  gFirstOption D_80149DA2 (s16), exitOptionScreen D_80116DA0, blit D_80116D98, frontEndBlit D_80116D4C / pauseBlit
  D_80116D70 (1 entry each), gControllerRemoved D_80116DA8, inited D_80116D94, buttonInfo D_8011650C[32], buttonNames
  D_801164E8, buttonColor template D_80116DAC; N64 height base 210 (GC 214); _TitlePane = time_result_display.

## func_800B4FB0 — MATCH in group (packet `groups/frontier_pausescr_state/`, NEW group)
- Builder `score.py group cand/frontier_pausescr_state` (and `--claims`): `func_800B4FB0: MATCH`; context slot_state_setup
  `2/58`, object_create `28/28`, func_800B669C `5/5 words differ` (the ctx_gr3_b.c context lines every group using that
  copy shows, e.g. physics_sym). Standalone `score.py fn` is not applicable (`368/368 words differ (unresolved symbols:
  ps_updatePauseStateCreate ...)`: the helper must be inlined and slot_state_setup's s2 IPA needs its body).
- Splice-proof replay (`groups/proof_frontier_pausescr_state.log`): `group_bodies OK: func_800B4FB0`, `func_800B4FB0: 368
  words, 0 differ from the image`, `1/1 members image-identical`.
- Unit (Pi): `blob_unit --tag w36q score func_800B4FB0 physics_sym --with cloud/work/frontier/w36q/func_800B4FB0/v/d1.c
  --internal ps_gfx_lock --internal ps_gfx_unlock --internal ps_font_set --internal ps_SetFont --internal
  ps_updatePauseStateCreate --neighbours` -> `EQUAL func_800B4FB0: 368 words (kept, c_d1.c)`, `EQUAL physics_sym: 313
  words`, `locked bodies that differ in this unit: 0`, `blob_unit score: 2/2 equal` (pause.c = d1.c + header comment).
- Integration: install_group.py cloud/work/frontier/w36q/groups/frontier_pausescr_state frontier_pausescr_state (new group;
  func_800B4FB0 not locked before). unit_overrides force_internal (if not derived): ps_gfx_lock, ps_gfx_unlock,
  ps_font_set, ps_SetFont, ps_updatePauseStateCreate.
- Path: a1 (GC `width /= 2` statements, `max` reused as the rumble loop counter) 158 rows -> b1/b2 (halving inside the
  InitPane arguments; gNumPlayers is D_8014A108) 8 words -> d1 (rumble loop counter `i`: GC's single `max` variable was
  one web in s1 for both uses; retail keeps case-1 max in s2 and the loop counter in s1) EQUAL. No devices.

## drone_collision_avoid — 5 words (calcMenuRect@audioscr.c; Astra #411 rect.c + unit adaptation)
- `audio_init/rect_best.c` (= r/m2.c, the rect part after init). Unit: `FAIL drone_collision_avoid: 5 of 213 words differ |
  words 9 ops 0 | frame 144/144`; the 4 code rows are the menuIdx*4 IV home (`sw/lw ...,32(sp)` retail vs 28) + 1 more;
  the other rows are own-.rodata "10" relocations (unverified in the unit).
- Levers found: (1) locked func_800D6E74 is the EMPTY validOption stub (`void f(void) {}`), so calling it leaves a test on
  garbage v0; a local `du_validOption(s32) { return 1; }` (house: au_validOption) fixes the structure (k1: 30 words, all
  stack offsets = Astra's residual). (2) du_ lock/font helpers instead of object_create (object_create's extra site
  breaks PrintScores' inlining in the unit). (3) An unused `u0` between menuIdx and menuCount puts every local on its
  retail home (retail has a slot at -8 between them).
- Residual: retail's merged locals end at -60 and the inline areas follow unaligned (Udef 108); ours are 8-aligned after
  -60 (Udef 112), so both spill homes sit 4 lower. Tried: u0 as s16/s8, K&R helper definitions, validOption without /
  with s8 / s32-return parameter, `old`-less font helper, unused locals below str (no slot), decl permutations — none
  gives Udef 108. Unused locals below the last memory local cost nothing; between memory locals they cost a slot.
- It can join `audio_options_menu` later; drone_pathfind_main does NOT need it (EQUAL with drone_collision_avoid absent).

## func_8010F218 / func_8010EA08 — not reworked; uopt unroll model (for the next lane)
- uopt's loop-unroll size limit is in its own units: with gDPSetPrimColor = P and gDPFillRectangle = R commands on the
  D_80149438++ cursor, P8 and R4 and 2P+3R and 3P+2R unroll by 4 at default flags, P9, R5, 3P+3R, P+4R do not: P = 8,
  R = 16 units, limit L in [64, 71] (`-Wo,-unrolllimit,N` moves it; t7/t5/u1 probes in my scratchpad). Trip counts 4..7
  unroll fully (factor 4 + remainder copies), trip < 4 never; a branchy body unrolled fully at trip 4 gets its
  counter-dependent branches folded (`P; if (i < 3) P` -> 7 straight P's). Retail's colour section is a fully unrolled
  4-trip shape {W(x); B(x); C(x+32..x+80) for i < 3} (W/B = P+R pairs), i.e. 3P+3R+branch per copy = 72+ units > L:
  with these macros it cannot unroll at default flags (it does at `-unrolllimit 1000`, giving retail's copy shape with
  folded colours). So retail's source for this section is cheaper in uopt's measure or differently built; not found.
- umerge (not uopt) facts used above: per-edge inline threshold = min(bmax, (bmax/8)/sqrt(small/large)) with bmax 500
  by default (`-bmax`), checked against caller size + callee size, with the caller growing as sites are inlined; sites
  are processed from the end of the source (w20g), single-caller callees inline directly when they pass the threshold,
  multi-caller ones go through a knapsack (`-space_time`, default 3.0). This is why big single-call helpers
  (initButtons, calcOptionMenuMaxSize) stop being inlined once later sites in the same caller are inlined.

## What generalises
- **initScreen + initButtons family**: write GC initButtons in place (block with GC's locals), use local non-static
  helpers for HAL_SetFont / ZOID_SetColor / AudKillSong, and expect a 16-B frame residual at the frame top (4 unused
  locals) where retail had initButtons' umerge inline area. Same recipe closed audioscr and videoscr; optionscr is
  blocked because there retail's inline areas sit ABOVE initButtons' locals (calcOptionMenuMaxSize precedes it).
- **umerge inlining is per edge with a growing caller** (bmax 500 threshold, sites from the end of the source) and a
  knapsack for multi-caller callees: a big single-call helper stops inlining as soon as later sites in the same caller
  inline; adding a call site of a shared kept helper (object_create, object_type1_create) changes other functions'
  inlining. Prefer local helper copies, one per site, as the installed audio/optionscr groups already do.
- **One variable = one web across switch cases**: GC reusing `max` as a later loop counter forces one register; the
  N64 source used a second variable (setPauseState closed on that alone).
- **GC `w /= 2` statements are often N64 `w / 2` in the call arguments** (retail divides after an argument call, and
  inside loops the half is hoisted into the preheader).
- **Locked empty stubs are not the function**: func_800D6E74 (`void f(void) {}`) is validOption's remains; calling it
  as the predicate tests garbage v0. Use a local `return 1` helper.
- **Unused locals cost a slot only between memory-resident locals**; after the last one they cost nothing.
- The unit's own-data check catches N64 literal changes cheaply (55.2f vs GC 59.2f, "BUTTON_TITLE" vs GC "BUTTON").

## Permission denials
None.
