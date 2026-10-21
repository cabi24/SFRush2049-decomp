# w36o RESULTS (wave 36, game hud 0x800EExxx-0x800F8xxx)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36o (from base, master e68aea6e1). Trace toolkit installed (--reuse wtk).
Helpers: `tools/sc.sh` (standalone score), `tools/gsc.sh` (group score), `tools/gbuild.sh` (group build keeping intermediates).

## Status

| function | bytes | GC identity | state | best file |
|---|---|---|---|---|
| skid_mark_render (0x800F0100) | 1396 | ShowHUD@hud.c (confirmed) | **MATCH** (group w36o_showhud MATCH, unit EQUAL) | `groups/w36o_showhud/` |
| explosion_effect (0x800EF8F4) | 1460 | animSpeedText@hud.c (confirmed) | NONMATCH unit 150/365 words, 73 rows (ops 25), frame 288/288 (w27a: 282/365) | `explosion_effect/best.c` (group form `explosion_effect/group_wip/`) |
| func_800F7F3C | 1396 | FindWinner@gamerecords.c (confirmed) | 117 words, ops 0 (colour only; prior w1c/w10b/w11d best stands) | `cloud/work/frontier/w11d/func_800F7F3C/best.c` |
| emitter_update (0x800EEDB0) | 1240 | animCountdownTimer@hud.c (confirmed) | **MATCH** (group w36o_countdown MATCH; unit EQUAL with the extern_data override) | `groups/w36o_countdown/` |
| func_800F87A0 | 968 | N64 pause/quit menu handler (no MAT3 counterpart found; unsaved s0-s2 = private IPA inside finish_state_alt) | not attempted (needs the unmatched 4-function cluster) | - |
| game_late_init (0x800EEA7C) | 820 | InitSystem@Init.c (confirmed) | **MATCH** (group codex_heap_release_a25 superseded in place, all 21 members MATCH; unit EQUAL) | `groups/codex_heap_release_a25/` |
| func_800EF288 | 808 | animGear@hud.c (confirmed) | **MATCH** (group w36o_animgear MATCH, unit EQUAL) | `groups/w36o_animgear/` |
| world_velocity_integrate (0x800ED804) | 712 | HAL_FinishText@HAL_text.c (confirmed) | not attempted: provisional at best (IPA partner audio_doppler_calc = _DrawText is unmatched, lane w36t) | `cloud/work/frontier/w27b/world_velocity_integrate/best.c` |

## skid_mark_render = ShowHUD -- MATCH (group + unit)

- Identity: MAT3 ShowHUD@hud.c (GC 0x8001C980): 4-slot timer/coin clear, NewMultiBlit (= sound_control) sets, ShowHUDExtra
  (image B 0x803925D0) for gRace 6/4, credits cycling with SetCountdownTimer/GetCountdownTimeRemaining/Random/HAL_SetFont.
- Base: Astra's research packet PR #414 `dot_showhud_research_20261017/group/candidate.c` (strict in its own 32-member graph).
  In the whole-program unit it was `FAIL skid_mark_render: 335 of 349 words differ; compiled body is 340 words` because
  `object_create(11)` (the locked HAL_SetFont) stays a jal there, while retail inlines it.
- Lever: inline a non-static copy of HAL_SetFont (`HAL_SetFont_ShowHUD` + `_GetSemaphore_ShowHUD`/`_ReleaseSemaphore_ShowHUD`,
  MAT3 HAL_text.c shape) -> unit `EQUAL skid_mark_render: 349 words (kept, c_v1.c); own zero-initialised data (addresses
  consistent, inside game .bss, no overlaps; nothing to compare)` / `locked bodies that differ in this unit: 0`.
- Group packaging (blob_group compiles the group alone): first 39/349 (font-queue address hoisted above the credits branch)
  until func_8008B2E4 went on keep; then 2/349 (as1 swap `addiu a0,a0,25040` / `move s0,v0`). Traced with as1t.sh: in the
  group compile the inlined helper bodies keep their own `.loc` lines and as1 breaks ready-list ties by lower line; in the
  unit every inlined instruction carries the call-site line (138) and the tie goes by besttime. Writing
  `_ReleaseSemaphore_ShowHUD` and `HAL_SetFont_ShowHUD` on ONE source line (disclosed in the header) gives retail's order in
  both builds.
- Scorer output (builder, `score.py group cand/groups/w36o_showhud` and with `--claims`):
  `skid_mark_render:` / `MATCH` / `own .bss placed at 0x8015462C (4 bytes; zero-initialised: verified by address only, nothing to compare)` /
  `own .bss placed at 0x80154644 (4 bytes; ...)`.
  Unit: `blob_unit --tag w36o score skid_mark_render --with cloud/work/frontier/w36o/groups/w36o_showhud/showhud.c --neighbours` ->
  `EQUAL skid_mark_render: 349 words (kept, c_showhud.c); own zero-initialised data (...)` / `locked bodies that differ in this unit: 0`.
- Integration: new group `w36o_showhud` (members/claims [skid_mark_render]; context slot_state_setup, sound_update_channel,
  func_8008B2E4 as byte-identical copies; keep listed in group.json). Unit install: force_internal += HAL_SetFont_ShowHUD,
  _GetSemaphore_ShowHUD, _ReleaseSemaphore_ShowHUD. Standalone `score.py fn` cannot match (slot_state_setup s2 convention).
- Note on Astra's "volatile clock" caveat: MAT3 rush_protos.h declares `extern volatile float gSeconds;` (line 1607), the
  wave-35 proven lever; D_8002EB90 is gSeconds.

## func_800EF288 = animGear -- MATCH (group + unit)

- Identity: MAT3 animGear@hud.c (GC 0x8001E144): fonts 10/11/12 by window count, per slot gear = model[slot].mainin.gear
  (+0x730), skip on slot >= gNumWindows / !gShowShifter (D_80161394) / autotrans (+0xA) / place_locked, "GEAR" label
  right-aligned with TB_RightText (inlined, object_manager_update = HAL_GetTextWidth) at gearPos (D_80115BE8) - 4, gear
  char 'R'/'N'/digit at +4.
- First direct port (MAT3 locals `u8 str[2]; s32 slot; s16 x, y; s8 gear;`): unit `EQUAL func_800EF288: 202 words (kept,
  c_v1.c)` | words 0 ops 0 norm 0 | frame 184/184. N64 differs from GC only in the missing early return.
- Packaged like ShowHUD (non-static helpers after the caller, one-line HAL_SetFont/_ReleaseSemaphore):
  `score.py group cand/groups/w36o_animgear` -> `func_800EF288:` / `MATCH` (also with --claims);
  unit `EQUAL func_800EF288: 202 words (kept, c_animgear.c)` / `locked bodies that differ in this unit: 0`.
  Standalone `score.py fn`: 61/202 (slot_state_setup s2 convention; group-only).
- Integration: new group `w36o_animgear` (members/claims [func_800EF288]; context slot_state_setup, sound_update_channel;
  keep in group.json). Unit install: force_internal += _GetSemaphore_Gear, _ReleaseSemaphore_Gear, HAL_SetFont_Gear,
  TB_RightText_Gear.

## emitter_update = animCountdownTimer -- MATCH (group; unit EQUAL with extern_data)

- Identity: MAT3 animCountdownTimer@hud.c (GC 0x8001F048): gGameState bit 22 / gShowRemaining (D_8015B24C) / gRace
  (battle mode D_80140B08 == 0, stunt mode D_801407BC == 1, race 0/1/3) gate; t = GetCountdownTimeRemaining
  (viGetTimeToDeadline); statics sec (0x80124F74) / nextBeep = 30 (0x801161B8); PlayGameSound (gate D_8010FFC0,
  entity_flags_apply = AudPlayEffect, id 2); blink on sc.frameCount & 8 (volatile D_8002E8E8 +0x27C);
  countdownTimerPos D_80115C68 / mapPos D_801160A8; "M:SS" with fonts 9/8/5.
- v1 MAT3 port: 114 rows, frame 216/224 (PlayGameSound inlined in the matched sibling's 1-test form).
  v5 MAT3's exact PlayGameSound (`if (!enabled) return -1; if (fxid == -1) return -1;`): frame 216/216, 70 rows.
  v6 `str[0] != '0'` (retail re-masks the stored byte, `andi t6,t8,0xff`): words 33 ops 15.
- Residual traced (ctrace): &gRace (w219) and the constant 4 (w221) tie at save 0.125 (tot 1, nocs 8); the lower web
  (&gRace) takes s3, retail has `li s3,4`. force.sh `p1:w219=s,p1:w221=c17` -> 12 rows (only the 12 own-data
  relocations). Separate `if (...) return 1;` statements give the constant nocs 7 (it wins), but separate return
  blocks (24 rows); four `if (...) goto skip;` sharing one `return 1` (disclosed) -> unit EQUAL.
- Group packaging: like ShowHUD, plus gJust (D_80118E20) defined in the file (tentative definition, as
  credits_scroll_grp/gr3_d.c): with an extern the group compile gives the TB_SetJust(0, 3) tail stores two `lui at`.
- Scorer output: `score.py group cand/groups/w36o_countdown` (and --claims) -> `emitter_update:` / `MATCH` /
  `own .bss placed at 0x80124F74 (4 bytes; zero-initialised: verified by address only, nothing to compare)` /
  `own .data verified at 0x801161B8 (4 bytes; .data is laid out per translation unit)`.
  Unit: with the definition the shadow unit refuses (`uld saw a name defined twice ... D_80118E20: multiply defined`,
  credits_scroll_grp/gr3_d.c defines it); with the definition turned extern (what an extern_data override does):
  `EQUAL emitter_update: 310 words (kept, c_countdown.c); own zero-initialised data (...)` / `locked bodies that differ in this unit: 0`.
- Integration: new group `w36o_countdown` (members/claims [emitter_update]; context slot_state_setup,
  sound_update_channel). unit_overrides: `extern_data` += {name D_80118E20, file
  src/blob/groups/w36o_countdown/countdown.c, reason: the two TB_SetJust tail stores share one lui, which needs a
  definition in the group's program; in the shadow unit credits_scroll_grp/gr3_d.c is the one definition};
  force_internal += _GetSemaphore_Countdown, _ReleaseSemaphore_Countdown, HAL_SetFont_Countdown,
  TB_SetJust_Countdown, PlayGameSound_Countdown.

## explosion_effect = animSpeedText -- in progress

- Identity confirmed against MAT3 animSpeedText (GC 0x8001DB6C): gShowSpeed (D_8015F72C) test, HAL_SetTextDepth(0)
  (render_helper), TB_SetJust(1,3) (D_80118E20 pair), HAL_SetFont 4/1, HAL_GetCharWidth('8')+1 (camera_shake_update),
  rwmph>>2 * 1.2f (* 1.61f metric D_801613AA), speedPos D_80115A68[4][4][2], dim '8' behind digits unless ':'/'.',
  TB_GetFontHeight inlined (two _SetupFontData = sound_update_channel(t0) getters), gText.single[202/203] (MPH/KPH).
- Prior: w27a best 282/365 (frame 288/280). MAT3 typed port v1 (MAT3 declaration list `f32 mph; u8 digits[3]; u8 str[2];
  s32 digitW, x, y, digitIdx, slot;` reproduces the retail homes: y 264, slot 256, str 276, digits 280):
  unit `FAIL explosion_effect: 152 of 365 words differ` | words 77 ops 29 norm 29 | frame 288/288.
- Residual: loop-top colouring (retail slot=a1, const 1=a3, gGameCar base=v0, model pointer fully spilled at 80(sp); ours
  slot=a3, 1=t0, base=a1, model ptr t1 for the loop-top piece). Traced (ctrace e1): slot's split piece (w33) is
  coloured first and finds v0..a2 forbidden because in ours its piece runs into the HAL_SetFont arms (spill
  `sw a3,256(sp)` in the osRecvMesg delay slot, a0-a2 are call arguments there); retail spills slot inside the
  digits block (`sw a1,256(sp)` at +0x1e4) before the else-arm arguments (`lui a0/addiu a0/move a1,zero` at +0x1e8,
  emitted in the digits block, the taken arm recomputes them). Forces on the pieces are declined (interference).
  Batches (group compile, tools/gbatch.sh): b1 120 variants (continue holder, mph forms, 0.0f/0.f/0, x/y order,
  font-height forms, digitW holder) and b2 18 (font-set arm order, digit expression forms): best 153/365 = v1.
- Non-static helpers (call-site lines in the unit): `FAIL explosion_effect: 150 of 365 words differ` | words 73 ops 25.
  The early `move a1,zero`/`lui a0` placements are as1 global hoisting (the ugen listing keeps every argument in
  the arms) and follow from colouring: slot's register piece ends with its home store in the arm, after the
  argument setup, so a0-a2 interfere (ours a3); retail's piece ends before the arguments (store in the digits block)
  and slot takes a1. No source form found that moves the piece end (b1/b2/b3 batches, arm order, continue form).
- Split oracle (own build of w16d's patch_spl.py: builder `w36o/bin/uopt_spl`, `tools/oracle.sh`): slot (sym 33)
  is split from start bb14; the BFS adds bb15..bb22 and then the two HAL_SetFont arm blocks bb23/bb28 (call blocks,
  added but not expanded), which forbid a0-a2 (f0 7c007e00). `SPL_REJ=23,28 SPL_SYM=33` (keep the arms out of the
  piece) reproduces retail's fully spilled model pointer (`sw t9,80(sp)` at loop entry, `lw t6,80(sp)` at the loop
  top) but slot then takes a0 (slot*8, retail a0, stays uncoloured) and the rest cascades (158 rows; with forces
  on slot/&gGameCar/1/952 153 rows). So retail's slot piece stops before the arms; the source reason is not found.
- Next: why retail's piece excludes bb23/bb28 (addadjacents rejection rule, or a CFG difference before the arms),
  then the slot*8 web (retail colours it a0 between the rwmph and digits blocks).

## game_late_init = InitSystem -- MATCH (codex_heap_release_a25 superseded in place; unit EQUAL)

- Identity: MAT3 InitSystem@Init.c (N64 form; see the file header for the callee map). The frees are MAT3
  DeallocateMemory (memsys.c) = the heap group's func_80095CF4 / audio_reverb_update(ptr, 0) / func_80095CFC with
  audio_reverb_update's internal IPA convention (a1/a2, a0 unset; s0/s1 left to the caller) -> must compile inside
  codex_heap_release_a25 (w27b's "unknown convention" blocker).
- Path: w27b draft 184/205 (frame 128/56) -> inlined DeallocateMemory + HAL_InitText/_InitSemaphore + InitGraphics
  chain (static) frame 112, 66 rows -> D_8006A090 (draft had 0x80069090) -> as1t trace of the osTvType block: the FOV
  store reaches the branch delay slot only when the store and the wait loop share a source line (tie on line,
  then besttime) -> buffer read-back form (`D_8015B250 = rec[0].buf`, `+1728`, `+0x9CC0` from the kept register) ->
  free-block and AudioInit wrapper inlines: frame 128/128, 31 rows -> one-line _InitGfx body: 16 -> sInited
  function-local static + one-line _InitSemaphore: 15 -> record store order (heaps first, buf88 before buf216): 5
  rows (4 own-data relocations + 1 extra word) -> own DeallocateMemory copy instead of the group's func_800960CC
  (the latter pulls an unnamed deleted stub behind game_late_init): MATCH.
- Scorer output: `score.py group cand/groups/codex_heap_release_a25` -> all 21 members `MATCH`, context MATCH;
  with --claims: `game_late_init:` / `MATCH` / `own .data verified at 0x801147C0 (4 bytes, all zero; .data is laid out
  per translation unit)`. Unit: `blob_unit --tag w36o score game_late_init --with
  cloud/work/frontier/w36o/groups/codex_heap_release_a25/initsystem.c --neighbours` -> `EQUAL game_late_init: 205
  words (kept, c_initsystem.c)` / `locked bodies that differ in this unit: 0`.
- Integration: supersede src/blob/groups/codex_heap_release_a25 IN PLACE with the lane copy (all 11 old files
  byte-identical, members/keep/files appended with game_late_init / initsystem.c, `claims: [game_late_init]`).
  Unit install: force_internal += HAL_InitText_Init, DeallocateMemory_Init.
- Disclosed shaping: one-line bodies for the inlined _InitGfx and _InitSemaphore; wrapper inlines sizing the frame.

## func_800F7F3C = FindWinner -- 117 words, colour only

- Identity: MAT3 FindWinner@gamerecords.c (GC 0x80015E30): gRankedPlayer D_80143F54 filled 0..gNumWindows-1, bubble sorts
  by gRace (4 = kStunt: gStuntPlayer D_80152038[].totalPoints (+0x14, stride 0x78); 6 = kBattle: gGameCar[].battle.Points
  (+0x3A3) then gTeamPoints D_8015256C[gCarTeam D_8012E67C[]]; else place (+0xEE)), ties D_80150B60 / tied D_80150B68.
  N64 differences: no `ties = 0; tied[] = 0` prologue, last arm a plain else (GC `gRace != kPractice`).
- The MAT3 expression form (slot loads inside the compare) is structurally wrong in IDO (`330 of 349`): retail loads
  both slots before both keys, i.e. the prior lanes' ia/ib locals form. Re-scored prior best: unit `117 of 349 words
  differ | words 137 ops 0 norm 0 | frame 16/16`.
- Batch b1 (24 variants: a/b/ia/ib declaration scope, MAT3 temp swap vs a/b swap, s16/s32, s8/s32): all >= 117.
- ctrace (fw): phase-2 colouring (web-number order, lowest free register): the arm-1 end pointer (w60) takes t4,
  &gStuntPlayer (w201) t5, 120 (w202) s0; retail end pointer t5, &gStuntPlayer s0, 120 s1 (t4 unused in retail's arm 1:
  some earlier web holds it). Not resolved; same stop as w14t.

## Identities proven

| N64 | MAT3 | evidence |
|---|---|---|
| skid_mark_render | ShowHUD@hud.c | unit EQUAL / group MATCH with the MAT3 body |
| explosion_effect | animSpeedText@hud.c | body structure, constants 1.2f/1.61f, '8'/':'/'.', speedPos |
| func_800F7F3C | FindWinner@gamerecords.c | gRankedPlayer D_80143F54, ties D_80150B60, tied D_80150B68, bubble sorts by gRace |
| slot_state_setup | _HAL_SetFont (static, HAL_text.c) | called between _GetSemaphore/_ReleaseSemaphore (osRecvMesg/osSendMesg on D_801461D0 = font semaphore) |
| object_create | HAL_SetFont@HAL_text.c | gr3_b.c body = HAL_SetFont |
| sound_update_channel | _SetupFontData (static, HAL_text.c) | called by the HAL_GetFontAscent/Descent getters (object_type_byte2/3_get) |
| object_type_byte2_get / object_type_byte3_get | HAL_GetFontAscent / HAL_GetFontDescent | `_SetupFontData(0); return gFontHeader->ascent/descent` (N64 without /2.0f) |
| D_801497F0 | gFontHeader | as above |
| render_helper | HAL_SetTextDepth | animSpeedText 0.0f / -1.0f |
| camera_shake_update | HAL_GetCharWidth | animSpeedText digitW |
| dispatch_handler | TB_SetTextColor | animSpeedText colours 0/14/1 |
| state_utility | TB_DrawText | (x, y, str) |
| func_800B669C / D_80118E20 | TB_SetJust / gJust | {h, v} pair |
| D_8015F72C / D_801613AA / D_80115A68 | gShowSpeed / gShowMetric / speedPos | animSpeedText |
| sound_control / ambient_sound_set / sound_handles_clear | NewMultiBlit / InitPane / RemovePanes | ShowHUD |
| viScheduleTick / viGetTimeToDeadline | SetCountdownTimer / GetCountdownTimeRemaining | ShowHUD credits |
| 0x803925D0 (image B) | ShowHUDExtra | ShowHUD gRace 6/4 arm |
| D_8002EB90 | gSeconds (volatile) | ShowHUD |
| emitter_update | animCountdownTimer@hud.c | group MATCH |
| func_800EF288 | animGear@hud.c | group MATCH |
| game_late_init | InitSystem@Init.c | AudioInit = hud_setup(32,16,16,5,64,1.0f,200.0f,0), SetControllerActive(-1,0) = player_state_set, HAL_InitText (_InitSemaphore + gFontIndex D_80149DA0 = -1) inlined last |
| world_velocity_integrate | HAL_FinishText@HAL_text.c | s8 end arg, gBufferInited D_8011474C, gFinished, _SetDefaults = func_800B6748 (7 pointer args in a0-a3/t0-t2), switch on gBuffer D_80149DD8[i++] |
| audio_doppler_calc | _DrawText (static, HAL_text.c) | called by HAL_FinishText with (s2 = &gBuffer[i], s0 = length) |
| func_800B6748 | _SetDefaults (static, HAL_text.c) | HAL_FinishText |
| D_80149DA0 | gFontIndex | slot_state_setup = _HAL_SetFont stores it |
| func_800F7F3C | FindWinner@gamerecords.c | see above |
| D_80143F54 / D_80150B60 / D_80150B68 | gRankedPlayer / ties / tied | FindWinner |
| D_8015F72C / D_80161394 / D_8015B24C | gShowSpeed / gShowShifter / gShowRemaining | anim* gates |
| D_80115BE8 / D_80115C68 / D_801160A8 / D_80115A68 | gearPos / countdownTimerPos / mapPos / speedPos | hud.c tables |
| D_8010FFC0 / entity_flags_apply | gGameSoundsEnabled / AudPlayEffect | PlayGameSound inline |
| D_80140B08 / D_801407BC | gBattleMode / gStuntMode | animCountdownTimer |
| func_800960CC (codex_heap_release_a25) | DeallocateMemory@memsys.c (func_80095CF4/CFC = _GetSemaphore/_ReleaseSemaphore, audio_reverb_update = _DeallocateMemory) | InitSystem frees |
| func_800E7D0C | InitMemory(50, 10)? (lead) | InitSystem's first call |

## What generalises

- **Inlined-helper source lines decide as1 ties.** as1 breaks ready-list ties by source line (then besttime). In the
  whole-program unit, NON-static helpers inlined by umerge carry the call-site line; static helpers and every helper
  in a `score.py group` / blob_group compile keep their own lines. Retail behaves like "all inlined code on one
  line" (cross-file inlining). Lever: write the inlined helper body (or the two statements in a tie) on one source
  line. ShowHUD/animGear/countdown (`_ReleaseSemaphore` + `HAL_SetFont` on one line) and InitSystem (one-line
  `_InitGfx` and `_InitSemaphore` bodies: 31 -> 5 rows) all needed it for the group compile.
- **A unit EQUAL is not a group MATCH**: the group compile also lacks (a) kept-ness of locked context (put
  func_8008B2E4 on keep, else PRE hoists differently), (b) data definitions in the unit: two stores to one symbol
  share a `lui at` only if the symbol is defined in the program -> tentative definition in the group file plus an
  `extern_data` override for the unit (precedent: D_80156958 controller_poll, gr3_d.c's D_80118E20).
- **Locked HAL_SetFont (object_create) stays a jal in the unit**: inline a local non-static copy (MAT3 HAL_text.c
  shape) in every HUD caller (PR #414's ShowHUD went from 335/349 to EQUAL with only this).
- **MAT3 exact inline helpers size the frame**: PlayGameSound's `fxid == -1` test (folded away) still reserves
  inline space: frame 224 -> 216 exact; each inlined wrapper level = 8 bytes (InitSystem: AudioInit and free-block
  wrappers 112 -> 128).
- **Exact save ties between a constant and an address web** (animCountdownTimer: constant 4 vs &gRace, both 0.125)
  are broken by block structure: separate `if` statements make the constant's live range one block shorter.
- **Deleted-procedure stubs follow the call graph**: calling another file's inlined helper (func_800960CC) let umerge
  place game_late_init between that helper and its unnamed stub (1 extra word); a local copy of the helper avoids it.
- HUD/HAL identities (table above) cover most text helpers used by every hud.c/screen function.

## Permission denials

(none)
