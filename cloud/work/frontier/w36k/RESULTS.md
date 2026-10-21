# w36k — controlscr.c (CONTROLS screen), game code (whole-program -O3 unit)

Lane started 2026-10-20 on master e68aea6e1. Builder scratch: watchman2:~/rush2049/scratch/frontier/w36k.

## Status

**Group `w36k_controlscr` (cloud/work/frontier/w36k/groups/controlscr/) MATCHES: 8,556 new bytes** (D91A0 3860 +
D816C 2796 + initButtons 1024 + D9058 328 + cleanup 196 + validControl 220 + uninitButtons 132; validOption 24 B is a
re-typed supersede of a locked single). `score.py group ... --claims` exit 0 (groups/controlscr_score_claims.txt);
blob_unit 8/8 EQUAL, `locked bodies that differ in this unit: 0` (groups/controlscr_blob_unit.txt).

| function | GC identity (MAT3 controlscr.c) | bytes | state | best file |
|---|---|---:|---|---|
| func_800D91A0 | updateControlScreen | 3860 | **MATCH** (group, kept) + unit EQUAL | groups/controlscr/caller.c |
| func_800D816C | updateButtons | 2796 | **MATCH** (group, internal) + unit EQUAL | groups/controlscr/target.c (Astra #322) |
| drone_throttle_calc | initButtons | 1024 | code identical, NOT VERIFIED two-region own .rodata (member, not claimed; blob_group proof) + unit EQUAL | groups/controlscr/dtc.c (w16f) |
| func_800D9058 | calcControlMenuMaxSize | 328 | **MATCH** (group, internal) + unit EQUAL | groups/controlscr/d9058.c |
| render_results_screen | cleanupControlScreen | 196 | **MATCH** (kept) + unit EQUAL | groups/controlscr/rows.c |
| func_800D8078 | validControl | 220 | **MATCH** (internal) + unit EQUAL | groups/controlscr/predicate.c |
| render_replay_ui | uninitButtons | 132 | **MATCH** (internal) + unit EQUAL | groups/controlscr/rows.c |
| func_800D8154 | validOption | 24 | **MATCH** (internal; supersedes locked single) + unit EQUAL | groups/controlscr/d8154.c |
| controls_animText | animText | 1576 | not matched: 151 rows (anim/best.c), 389-392 of 394 words, frame 352/360 | anim/best.c |
| controls_animButtonText | animButtonText | 1916 | not matched: **49 rows** (29 with sp offsets normalised), 478/479 words, frame 440/544 | anim/best.c |

## Notes

- 2026-10-20 ~1h in: MAT3 direct port of updateControlScreen (groups/controlscr/caller.c) on Astra's #322 group.
  Levers found: (1) PS_PakRumble/PS_PakInserted are the LOCKED kept getters func_80094FC4 / func_800A1A3C
  (menu_save_options), inlined by umerge (frame 232 -> 240 with them; Astra's func_800DA0B4 getter not needed);
  (2) the clear loops' two statements on separate lines (as1 store order); (3) memcmp count as size_t (u32);
  (4) func_800D9058 = calcControlMenuMaxSize must be in the unit as an INTERNAL function: retail's version uses
  s2/s4/s5 unsaved, and that clobber set moves const 1 to s6, 21 to s5, &gNumWindows to s7, &inited to s8
  (force.sh oracle confirmed, then the real body reproduced it); (5) `j` (dead after the duplicate scan) carries
  gPlayers[p].edges in both the 0x1000/0x2000 else-if chain and the &5 test (gives retail's a0 web).
  blob_unit: `EQUAL func_800D91A0: 965 words (kept, c_j5b.c)` (with D816C, initButtons, rows, D8078, D8154 EQUAL).
- Coordinator note (2026-10-21): D9058, D8078, render_results_screen, render_replay_ui assigned to w36k (w36n map
  proves them). validOption = func_800D8154: Astra's `if (0) { var_v1 = 1; }` replaced by a one-case `switch`
  (same 6 words; keeps D8154 out of D91A0 but inlined in D816C/D9058; the if(0) form broke D9058's `i+k==8` folding).
  Inline-budget probes (unit): plain body / `player;` / `item;` / switch-without-break -> inlined into D91A0 (wrong);
  `var_v1 = 1;` twice, if(0) after the if, switch+break -> not inlined (right).
- score.py group cand/controlscr (2026-10-21): func_800D91A0 MATCH (own .rodata verified 0x80124218..0x80124224),
  func_800D816C MATCH, render_replay_ui/render_results_screen/func_800D8078/func_800D8154 MATCH, drone_throttle_calc
  NOT VERIFIED (20 two-region own-.rodata relocations; code identical; wave-16 rule), func_800D9058 28/82
  (24/82 in the unit; colour-only per force.sh: i a0 / count a1 / constants one register up).
- func_800D9058 (calcControlMenuMaxSize) residual, traced: structure/frame exact (82 words, frame 24) with
  `rectHeight -= 24; size = rectHeight / 16` (retail reuses s4), font set = inlined lock/SetFont/unlock helpers
  (font_set_CS etc., non-static, internal), validOption inlined (switch form folds `i+k == 8` to `i == 8-k` as retail).
  Remaining 24 words are pure colouring: retail i=a0/count=a1 (ours count a0, i a1: count save 10.375 > i 10.14) and
  retail's loop constants start at a3 (ours a2): retail has one more loop-live web in a2 with no instruction of its
  own. Probes: `if (i) {}` in the loop gives i first (14 words); a quotient holder gets a2 but then the quotient
  itself is computed in a2 (retail t8); `validOption(...) == 1` creates exactly such a constant-1 web in a2 with the
  constants shifted to retail's a3..t3 (but adds xor/move code). ~35 other spellings (types, loop forms, statement
  order, declaration order, font-set forms, holders) stay at 24. Hypothesis: retail's constant-1 is a coloured web
  whose uses were all rematerialised (`li v1,1`); source form unknown.
- D9058 more probes (t1-t3 unsigned types, n1-n6 condition forms, m1-m3): m1 (`== 1`) traces as const-1 web
  w85 tot 40 / 7 blocks -> a2, constants a3,t0,t1,t2,t3, &size t4 = exactly retail's constant colouring (plus xor
  code). So retail most likely carries a loop-wide constant-1 web whose uses ugen emitted as immediates/li.
  Source for it still unknown. Variants under func_800D9058/v/ (batch*.out).
- 2026-10-21 (after usage reset) D9058 holder sweep 1 (36 variants, func_800D9058/sw/sweep1.out): a compiled-out
  `do {} while (0);` right after the counting loop (sw/dw_11.c) gives retail's i=a0 / count=a1 (16 words left:
  only the loop constants one register low, i.e. the missing a2 web). Same 16 with the holder before the loop or
  `if (count|rectHeight|i) {}` there; `if (i) {}` in the loop 14. Sweep 2 (second holder on dw_11) running.
- Sweep 2 (50 variants on dw_11): `if (D_80151AD0) {}` (gNumWindows) inside the counting loop -> `EQUAL func_800D9058:
  82 words (internal, c_hn_09.c)` and `EQUAL func_800D91A0: 965 words`. The read keeps the &gNumWindows address web
  live through the loop (the missing a2 web); the do-while after the loop gives i=a0/count=a1. Both compiled out.
- **2026-10-21: the whole controlscr group MATCHes.** d9058.c = hn_09 with the font-set helpers defined AFTER
  func_800D9058 (group compile put `lw s4` in the osRecvMesg delay slot when they were defined before: as1 line
  tie; unit was EQUAL either way). `score.py group cand/controlscr`: func_800D91A0 MATCH (own .rodata verified
  0x80124218..0x80124224), func_800D816C MATCH (own .rodata verified 0x801241E8..0x801241FC), func_800D9058 MATCH,
  render_replay_ui / render_results_screen / func_800D8078 / func_800D8154 MATCH, drone_throttle_calc NOT VERIFIED
  (20 two-region own-.rodata relocations, code identical: wave-16 rule, blob_group proof).

## Integration notes (group `w36k_controlscr`, new group)

- Dir: `cloud/work/frontier/w36k/groups/controlscr/` (group.json + 10 files). Flags `-g0 -O3 -mips2 -G 0 -non_shared`.
- members: func_800D91A0, func_800D816C, drone_throttle_calc, func_800D9058, render_replay_ui, render_results_screen,
  func_800D8078, func_800D8154. claims: all but drone_throttle_calc (two-region own .rodata: strings 0x80120254..,
  floats 0x801241BC..; code identical; needs the blob_group window-by-window proof as w16f/w32b precedents).
- keep: func_800D91A0, render_results_screen (+ context keeps func_8008B32C, func_800B5898, func_800B5940,
  func_800A1A3C, func_80094FC4, func_800B669C). Internal by derivation: func_800D816C, drone_throttle_calc,
  func_800D9058, render_replay_ui, func_800D8078, func_800D8154.
- force_internal += gfx_lock_CS, gfx_unlock_CS, font_set_CS (non-static inlined helpers in d9058.c; deleted stubs
  carry symbols, w17j).
- func_800D8154: the locked single `src/blob/func_800D8154.c` (`s32 func_800D8154(s32)`) is superseded by the
  group member `(s8 player, s32 item)` with the switch body (same 6 words) -> `--revert-single`. The re-typing is
  load-bearing: D91A0 passes the player in the 0(sp) slot and item in a0 (private ABI), and the switch body is the
  size at which umerge inlines it in D816C/D9058/animText but not in D91A0.
- func_800DA0B4 stays the locked empty single (Astra's packet gave it a getter body; not needed: the real
  getters are the locked func_80094FC4/func_800A1A3C).
- Context files: helpers.c (= locked singles' text, PR #322), ctx_pak.c (function text byte-identical to
  menu_save_options/group.c), ctx_gr3_b.c (byte-identical credits_scroll_grp/gr3_b.c; slot_state_setup's IPA s2
  parameter). In the group compile slot_state_setup context shows 2/58 and object_create is deleted (informational).
- Prototype note: caller.c declares func_800A1910 (memcmp) with a u32 count (locked declaration says s32): with s32
  the constant 10 joins the t1 web (measured).
- drone_throttle_calc keeps w16f's disclosed unused locals k/u0/u1/u2 (frame 112 vs 96).

## Identities proven (for w36n/w36y)

- func_800D9058 = calcControlMenuMaxSize@controlscr.c (MATCH with MAT3 body; GC 0x8006CD08).
- func_800D8154 = validOption@controlscr.c, N64 body rejects option 8 (MATCH).
- func_80094FC4 = PS_PakRumble (pak byte 6) and func_800A1A3C = PS_PakInserted (pak byte 1): inlined by
  updateControlScreen at the GC call sites (MATCH evidence).
- render_results_screen = cleanupControlScreen, render_replay_ui = uninitButtons@controlscr.c, func_800D8078 =
  validControl, drone_throttle_calc = initButtons@controlscr.c, func_800D816C = updateButtons@controlscr.c (MATCH).
- Data (controlscr.c statics/globals): D_80113ED4 inited (s8), D_80113ED8 blit, D_80113EDC exitControlScreen,
  D_80113EE0 gPlayerRects[4][4] (x,y,w,h,cx,cy), D_8015418C controlScrActive[4], D_8014A10C prevControlScrActive[4],
  D_8014A0F8 gControlSelect[4], D_8014A100 gControlFirst[4], D_8014A10A gControlMenuSize, D_80149DA8 gConflict[4][10],
  D_80149DD4 gConflictExists[4], D_8013C068 gNewControls[4][10], D_80146150 gJustPlayers[4], D_80139300
  gControlsSet[4], D_80157244 gSoftReset, D_80112A98..AB4 gButtonZoom, gButtonScale, gButtonSpace, buttonX, maxY, minY,
  buttonZ, cursorZ, D_80112AB8/D_8011361C/D_80113910 buttonsFrontEnd/buttonsPauseSingle/buttonsPauseMulti,
  D_80144030 pak channel array (0x304 stride), D_80111998 buttonInfo[4][17], D_80154618 buttonsInited[4],
  D_80154630 buttonsJustInited[4], D_80154648 cursorSpeed[4], D_80154FC0 scrollSpeed[4]. Callees: sound_control =
  NewMultiBlit, ambient_sound_set = InitPane, entity_flags_apply = AudPlayEffect, func_800C77C4 = SetDefaultControls,
  resource_type_select = PlayStartABSound, func_800A1910 = memcmp, drone_target_update = SetControls,
  sound_handles_clear = RemovePanes, func_800B4FB0 = setPauseState(-like), slot_state_setup = HAL font core.

## What generalises

- An IPA-internal callee's clobber set is part of its caller's colouring: D91A0's constants and address webs moved
  to retail's registers only when its internal callee calcControlMenuMaxSize (unsaved s2/s4/s5) was in the unit.
  Unmatched internal callees of a near-match are colouring blockers even when called by plain jal.
- umerge's inline budget picks the source size of a tiny shared helper: validOption needed a form big enough not to
  inline into the 3.9 KB caller but small enough for the others (one-case switch; `x = 1;` twice also works).
  The `if (0) {...}` size lever changes the inlined IR (stopped uopt folding `i + k == 8` in an unrolled loop).
- Colour-only residual with an "invisible" register: a compiled-out read of a global (`if (gNumWindows) {}`) keeps
  its address web live and occupies the missing register; found by a holder sweep at every statement boundary.
- Group vs unit as1 differences: helpers defined before their caller can schedule differently in the group compile
  (line ties); defining them after the caller fixed it.

## controls_animText / controls_animButtonText (not matched; notes for the next lane)

- Base: w32b `controls/best.c`; anim/v/r1.c = w32b with the REAL getters (func_800D8154 validOption with the w36k
  switch body, func_80094FC4 PS_PakRumble, locked func_800BEA3C TB_FlashTextColors declared not redefined).
  The w32b `if (player) {}` validOption copy is gone (the real validOption is a different function).
- r2/r3: MAT3 structure for animText (if/else on buttonsInited, nested if for controller/active, single loop tail;
  rumble text order `if (!PakRumble) off else names`): 366 -> 168 rows, body 389/394, ops rows 31; retail's single
  shared p-loop tail is reproduced (the w32b `continue` form PRE'd p+1 onto every edge). Sweep of `do {} while (0);`
  at 49 statement boundaries (anim/sw/sweep.out): best 151 rows (before `y = ...`, anim/best.c).
- Frame: retail 360, ours 352 in every if/else form, 360 only in the continue form (x4: pak/active checks as
  `continue`); top locals (i -4, p -8, x -12, y -16, str -20, str1 -84, pName -100, itemsDrawn -104) are identical to
  retail, so the 8 bytes are inline-area/spill-home bytes. An extra unused local does not move it (x5). Using the
  real TB_CenterText/TB_RightText (sound_pitch_diff_halved/_calc, locked, kept: their bodies have a local) gives
  368; the real TB_SetJust (func_800B669C) at the scroll-box site gives 384 (retail stores directly there).
- Colour residual (ctrace a3): retail gives s8 to the constant 10 (both `i == 10` compares, loop-wide, rematerialised
  after the animButtonText call) and spills p*2 (136(sp)); ours gives s8 to the p*2 offset web (save 12.0) and splits
  the constant 10 (save 11.11, tied with an address web and the 0x304 stride constant). The PakRumble inline also
  forms its address differently (ours CSEs a base, retail `lui 0x8014` + lb 16438).
- Identities found for this pair (w36n map, proven): object_create = HAL_SetFont (real function, inlined with the old
  value kept in s0), sound_pitch_diff_halved/_calc = TB_CenterText/TB_RightText, music_fade = TB_DrawEllipseText
  (animButtonText inlines it: buf[0x80] at sp+232 and the s16 x/y conversions), func_800B669C = TB_SetJust.
  Next: animButtonText with music_fade + object_create-shaped HAL_SetFont at its 7 font sites (frame 520 -> 544).
- animButtonText (anim/v/b1-b4): MAT3 structure (if/else, single exit) + the real TB_DrawEllipseText
  (`music_fade(x, y, gNumWindows == 1 ? 150 : 69, name + 20)`, inlined: buf at frame-top-312 exactly as retail) +
  direct `func_800BEA3C(flashColors[0], flashColors[1])`: 234 -> 49 rows (b3), 29 rows with --nosp, 478 vs 479
  words. Remaining = frame 440 vs 544: retail has 68 more bytes of cfe locals ABOVE screenPos (screenPos at
  top-88; a diagnostic `char str1[64]; s32 u0;` before it reproduces the top layout exactly, b4diag.c, frame 512 —
  NOT a deliverable: pad array) and 32 more bytes in the bottom inline/spill region (retail stores the two
  TB_FlashTextColors by-value parameters at sp+88/92 and sp+104/108, i.e. memory-resident inline parameter homes,
  plus one more word: `lw a3` name carrier). Block-scoped TextColor locals (b1/b2) add 16 bytes but generate
  struct-copy code retail does not have. Source of the 68 top bytes unknown (GC DWARF lists only i, x, y, str,
  screenPos, gTextPosScale).
