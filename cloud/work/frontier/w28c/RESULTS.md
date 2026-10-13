# w28c RESULTS (wave 28, menu / options / controls / countdown pool, IDO 5.3 -O3)

Status: FINAL (wave 28). Builder scratch `watchman2:~/rush2049/scratch/frontier/w28c`; unit tag `w28c`.

Pool: game_results_input (*animControls), func_800DA2C0/func_800DB1E0 (*updateButtons/*updateOptionScreen),
func_8010A8D0/func_8010AEAC (animButtonText/animText), audio_doppler_full (*ShowCountdown, misnamed), and the
0x80106B3C–0x8010F218 block.

| Function | Bytes | State | Score before → after | draft_check | Residual | GC evidence |
|---|---:|---|---|---|---|---|
| game_results_input → **animControls** | 1,732 | draft, EQUIVALENT | never drafted at O3 here (prior -O2 baseline 408/433 at O3) → 157/433, ops 11, frame 120/120 | EQUIVALENT 105,680 calls (controls_frontend 88,880, pause_menus 16,800) | colour only (a3 unavailable in retail) | `*animControls` [controlscr.c] |
| func_8010E8B4 (pause title-pane init) | 340 | draft, EQUIVALENT | never attempted → 21/85, ops 0, frame 96/80 (with sound_loop_set C internal) | EQUIVALENT 571 calls (options_menus 331, pause_menus 240) | 16 B frame above `old` + ring position (colour/frame) | GC `initScreen@optionscr.c` else-arm (gControllerRemoved, _TitlePane, InitPane) |
| sound_loop_set = **_TitlePane** (blocker of E8B4, not in pool) | 332 | **unit EQUAL** (`--internal sound_loop_set`), physics_sym stays EQUAL | never attempted → EQUAL | n/a | packaging only (callers physics_sym group + E8B4) | `_TitlePane@optionscr.c`: width, str1[64], "CONTROLLER %d %s", ChangePaneSize/InitPane |
| func_8010BC84 (pause controller-status text) | 928 | draft, EQUIVALENT | never attempted → 160/232, ops 29, frame 64/64 | EQUIVALENT 90 calls (pause_menus) | &D_8014A108 coloured in s8 in retail (address form, not volatile); `x - (w>>1)` narrowing pattern; s0/s1 order | none (pausescr.c family) |
| func_8010A53C | 616 | **MATCH in group** credits_scroll_grp (supersede) + unit EQUAL | never attempted → EQUAL | not needed | none | `ShowHUD`/`animCreditText` [hud.c] credit-name table |
| func_80107EDC | 632 | **MATCH in group** + unit EQUAL | never attempted → EQUAL | EQUIVALENT (race 1,096, pause_menus 3,736 calls; earlier draft) | none | weak (hud.c) |
| func_80108154 = **animLapTime** | 896 | **MATCH in group** + unit EQUAL | never attempted → EQUAL | not run (code-identical) | none | `animLapTime` [hud.c]: locals w/digitW/colonW/win/show/fixedWidth, flt 192 |
| func_8010AEAC = **animText** (+ func_8010A8D0 = **animButtonText**, func_8010A7A4 = **printOption**) | 1,652 (+1,500, 300) | draft unit (3 fns) | never attempted → A7A4 16/75 (ops 7), A8D0 325/375, AEAC 357/413 | pause_menus EQUIVALENT 689 calls; options_menus host crash (-11, the A8D0 path, not diagnosed) | logic of A8D0 unverified; printOption colour | `animText`/`animButtonText`/`printOption` [optionscr.c] |
| func_8010DCFC | 660 | draft | never attempted → 145/165, ops 11, frame 104/136 | not run (attract 1 call) | frame/spill layout (inlined-helper params?) | weak (targets.c) |
| func_8010B5D0 / B7FC / B9C8 | 556/460/700 | w27d drafts re-scored, unchanged (20/139, 42/115, 108/175) | — | EQUIVALENT (w27d) | the `x - (w >> 1)` argument narrowing (see lessons) | B5D0: `_TitlePane@pausescr.c` weak |
| func_800DA2C0 / func_800DB1E0, func_8010EA08 / func_8010F218 | 2,316/1,400, 2,056/2,444 | not attempted (time) | — | — | DA2C0 and EA08 are internal (unsaved s0–s8) callees of DB1E0 / F218: pairs only | `*updateButtons`/`*updateOptionScreen` [optionscr.c]; template: locked physics_sym group (func_800B59F0 = the pause-menu updateButtons) |
| func_80106B3C | 600 | draft, EQUIVALENT | never attempted → 50/150, rows 52, ops 16, frame 64/64 | EQUIVALENT 4,832 calls (race 1,096, pause_menus 3,736) | colour (nw in a1, loop invariants s5–s7 order) | weak (hud.c) |
| func_80108AB0 (wrong-way text) | 760 | draft, EQUIVALENT | never attempted → 184/190, ops 46, frame 200/128 | EQUIVALENT 4,832 calls (race, pause_menus) | FP constants hoisted into $f20–$f26 in ours; 72 B frame gap (unknown locals/inlines) | GC `animWrongWay` [hud.c] by behaviour (not in correlation) |
| audio_doppler_full → **ShowCountdown** | 952 | **code-identical** (score.py fn: 0 word diffs, NOT VERIFIED = owndata in two .rodata regions); unit EQUAL | never attempted → EQUAL | not needed (code-identical) | none | `*ShowCountdown` [countdown.c] (strings CNTDWN3/CNTDWN%d, locals, statics) |

## audio_doppler_full = ShowCountdown (0x800B61FC, 952 B) — code-identical, unit EQUAL

Files: `w28c/audio_doppler_full/best.c` (= `w28c/groups/audio_doppler_full/audio_doppler_full.c`), group dir
`w28c/groups/audio_doppler_full/` (`claims`/`members`/`keep` = audio_doppler_full, no context).
- Unit: `blob_unit --tag w28c score audio_doppler_full --with cloud/work/frontier/w28c/audio_doppler_full/best.c --neighbours`
  → `EQUAL audio_doppler_full: 238 words (kept, c_best.c); own zero-initialised data (addresses consistent, inside game .bss, no overlaps; nothing to compare)`
  / `locked bodies that differ in this unit: 0`.
- `score.py fn` (and `score.py group`): no word diffs; verdict `NOT VERIFIED (28 section-relative relocations unverified: …;
  own .rodata: this function's references disagree on the section's image address (0x80120288 for +0x0/+0x8, 0x80123DC0
  for +0x14): the literals are not laid out as in retail)`, then `own .bss placed at 0x80154368 (48 bytes …)`, `own .bss placed
  at 0x80154398 (2 bytes …)`, `own .bss placed at 0x801543A4 (2 bytes …)`, `own .data verified at 0x801140F4 (4 bytes …)`,
  `own .data verified at 0x801140F8 (48 bytes …)`. The strings sit at 0x80120288/0x80120290 and the 0.001f literal at
  0x80123DD4: two retail .rodata regions (the known owndata one-base case) → install via blob_group.
- Name: propose **ShowCountdown** (GC countdown.c, `void ShowCountdown(int active)`); the N64 label "audio_doppler_full" is wrong.
- GC-derived names/layouts (from GC `ShowCountdown`): function-local statics poly[12] (0x80154368, `Poly *`), polyTidx
  (0x80154398, u16), lastCountDown (0x801543A4, u16), polyColor (0x801140F4, u8[4] = FF FF FF C0), polyXYZ (0x801140F8,
  f32[4][3] = (±1, ±1, 6)); globals gNumWindows = D_80151AD0 (s16), gCountDown = D_80152032 (s16), gCamera = D_80150B70
  (stride 0x98, pos at +0x24, the matrix WorldVector takes at +0x30), gNumModels = D_80140BDC; callees WorldVector =
  func_8008C544, MBOX_FindTexture_Sub = func_800B24EC (err 1), ZOID_NewPoly = func_800A78BC, ZOID_UpdatePoly =
  func_8008C074, ZOID_RemovePoly = func_8008D0C0, PlayGameSound = entity_flags_apply, GetCountdownTimeRemaining =
  viGetTimeToDeadline (0x80001578, label also suspect), ZOID_ShowPoly inlined (`flags &= 0x7FFF`, u16 at +2).
- Path: draft 1 (externs, plain ifs) 161/238 words, frame 200/256 → inlined PlayGameSound + ZOID_ShowPoly (frame 256) →
  early-return gate + volatile gNumModels (ops 16) → function-local statics (ops 0; as externs the &polyTidx web takes
  s7/s8 from polyColor because loads of an extern go through the coloured address) → flags as
  `(1 << winidx) | 0x8000 | 0x200 | 0x80 | 0x40` (the folded `0x82C0` leaves ugen's temp ring 3 registers behind) → EQUAL.
- Integration notes: data-symbol change — the five statics become this TU's own .bss/.data (0x80154368 48 B, 0x80154398
  2 B, 0x801543A4 2 B; .data 0x801140F4 4 B, 0x801140F8 48 B); remove any `D_80154368/D_80154398/D_801543A4/D_801140F4/
  D_801140F8` extern-data definitions the build carries for them if blob_group requires it. No unit_overrides.

## credits_scroll_grp superseded: func_8010A53C, func_80107EDC, func_80108154 (MATCH in group, EQUAL in unit)

Group dir `w28c/groups/credits_scroll_grp/` (same name, every old member kept: credits_scroll, func_80100B8C,
slot_state_setup, world_trigger_check; `claims` = the three new members; new files gr3_a53c.c, gr3_edc.c, gr3_8154.c).
- Builder: `python3 tools/cloud/score.py group cand/groups/credits_scroll_grp` → exit 0, Members: `credits_scroll: MATCH`,
  `func_80100B8C: MATCH`, `slot_state_setup: MATCH`, `world_trigger_check: MATCH`, `func_8010A53C: MATCH`,
  `func_80107EDC: MATCH`, `func_80108154: MATCH` (context bodies print their usual informational diffs);
  `score.py group --claims ...` → exit 0.
- Unit: `blob_unit --tag w28c score func_8010A53C func_80107EDC func_80108154 --with .../gr3_a53c.c --with .../gr3_edc.c
  --with .../gr3_8154.c --internal font_set_A53C --internal gfx_lock_EDC --internal gfx_unlock_EDC --internal font_set_EDC
  --internal gfx_lock_8154 --internal gfx_unlock_8154 --internal font_set_8154 --internal Hidden_8154 --neighbours` →
  `EQUAL func_8010A53C: 154 words`, `EQUAL func_80107EDC: 158 words`, `EQUAL func_80108154: 224 words`,
  `locked bodies that differ in this unit: 0`, `3/3 equal`. (Without the --internal flags the non-static helpers stay
  real calls in the unit and func_8010A53C fails; in the group, uld -kp internalises them since they are not on keep.)
- Standalone `score.py fn` is impossible (slot_state_setup takes its font in s2 by IPA; func_8010A53C standalone 100/154).
- Integration: install the group dir (blob_group). Shaping/data changes to disclose:
  * all three use the family's inlined helpers (gfx_lock/gfx_unlock/font_set, Hidden) as non-static, uniquely named
    functions not on keep (w17j); gr3_c.c and gr3_d.c's static gfx_lock/gfx_unlock/font_set were renamed to global
    *_g3c/*_g3d for the same reason (code unchanged): with the new files umerge otherwise places their three symbol-less
    stubs after world_trigger_check (`world_trigger_check: MISMATCH (3 extra words …)`). If the whole-program unit
    manifest needs it, those six g3c/g3d names and the eight new helper names are force_internal.
  * func_80108154: unused local `x` = exact frame residual (one 4-byte named slot; frame stays 152 without it but every
    named home moves 4 B: 10 words differ). Its GC twin animLapTime has locals w, digitW, colonW, win, show,
    fixedWidth; `x` is the N64 extra slot at the top.
  * func_8010A53C: declaration order `x` before `hide` places x's spill home at 60(sp); `i = 0; y += 15; for (; …)`
    gives retail's addiu order. func_80107EDC: two `continue` guards (nested ifs colour the strength-reduced lap pointer
    first; with continues the order is x, y, i, ptr = s0..s3 as retail).
- Names/semantics: func_8010A53C = credits page text (attract; GC hud.c credit-name table / ShowHUD);
  func_80107EDC = per-window lap counter ("%d/%d"-style text at StrTable header +0x338, drop-shadowed);
  func_80108154 = **animLapTime** (hud.c): width = 7 × width('8'+1) + 2 × width(':'+1) with fixed-width digits forced
  through object_byte9_set (GC fixedWidth), fade alpha = lapTimer × 192 / 3 under 3 s.

## Key lever found this wave (applies to the whole slot_state_setup family)
- The retail `move sN,v0` after slot_state_setup (the old font, kept in a callee-saved register although nothing reads
  it) is the **inlined font_set helper's return value** (w15d mechanism), not a source variable: written-out
  `osRecvMesg; old = slot_state_setup(n); osSendMesg;` with or without the `if (old) {}` holder (any placement, one line or
  not) loses the move in this unit; `font_set(n);` as an inlined helper (non-static, internal) restores it at zero
  frame cost. For two sites in one function the gfx_lock/gfx_unlock nesting is needed or umerge does not inline both.
- Countdown: function-local statics (as on GC) are not interchangeable with externs: a loaded extern goes through the
  coloured address web (`lhu a2,0(s7)`), a local static loads by `%lo` and leaves the address uncoloured.
- A folded constant (`| 0x82C0`) vs the OR of its flag constants (`| 0x8000 | 0x200 | 0x80 | 0x40`) moves ugen's temp
  ring by 3 registers with no code change — a fourth code-free ring lever after `(short)`, `>>=`, `do{}while(0)`.
- Unresolved family residual: retail narrows the first state_utility argument through a temp
  (`subu t7; sll a0,t7; sra t8,a0; move a0,t8`), ours narrows in place (`subu a0; sll t9,a0; sra a0,t9`). Tried in B5D0/B7FC:
  explicit casts, `(s16)(...)`, `/2`, an s16 local, s32-parameter prototype, inline text wrapper (+16 B frame per site):
  none. A53C's matched `sx` (s16 local passed on) produces the same 3-instruction shape, so the shape is "an s16-typed value
  narrowed again for an s16 parameter"; the source form that yields it without the variable narrowing is still unknown.

## game_results_input = animControls (draft, EQUIVALENT)
`w28c/game_results_input/best.c`. GC animControls (controlscr.c 0x8006BC2C; "CONTROLS_SM"; locals id, conflict,
uninited, p, val, action, i, j, screenPos[2], gTextPosScale) gives the whole structure: data-word nibbles, the
validOption skip loop (N64: item 8 invalid), `Hidden(blt, …)` inlined twice, two ZOID_ProjectWindow (= brake_light_update)
passes at 55·scale·300/buttonZ and 75·…, RenameBlit (= func_800EF5B0) "CONTROLS_SM" for split screen, UpdateBlit =
Input_ApplyPadConfig, SelectBlit = stat_race_update with button textures D_8011407C[gNewControls[ctl][action]] (+1 for
action 0/1 when 25). Unit: `FAIL game_results_input: 157 of 433 words differ | words 162 ops 11 | frame 120/120`.
Frame needs the named local `valid` inside the inlined validOption (120; 112 without). Residual (force.sh): colour only —
with first=t0, blt=t1, id=t2, action=t3, p=t4, conflict=t5 forced, 27 rows remain and uninited takes a3 where retail uses
ra: in retail a3 is unavailable to every web of this procedure (cause not found). draft_check: `VERDICT: EQUIVALENT over
105680 calls / 2 scripts (controls_frontend 88880, pause_menus 16800)`. Next lever: find what forbids a3 (ctrace
forbidden masks on uninited's web; an inlined helper whose parameter lives in a3?).

## Other drafts (headers in each best.c carry the details)
- func_80106B3C: `FAIL … 50 of 150 words differ | words 52 ops 16 | frame 64/64`; draft_check `EQUIVALENT over 4832 calls
  / 2 scripts (race 1096, pause_menus 3736)` (earlier body). Next: nw lives in a1 in retail across the loop.
- func_80108AB0 (= GC animWrongWay by behaviour): `184 of 190 … ops 46 … frame 200/128`; EQUIVALENT 4,832 calls.
  GC names in `func_80108AB0/gc_named.c` (wasStrayed function static, TB_SetJust/TB_GetFlashTextCycle inlined: frame 208).
  Next: why retail does not hoist 2.0f/255.0f/±π/2 into $f20+ (it reloads them per iteration).
- func_8010BC84: `160 of 232 … ops 29 … frame 64/64`; `EQUIVALENT over 90 calls / 1 scripts (pause_menus 90)`.
  Global D_8014A108 (player count) is the right address (draft 1 had 0x80149F08).
- func_8010E8B4 + sound_loop_set (= _TitlePane): sound_loop_set `EQUAL sound_loop_set: 83 words (internal, c_t4.c)` with
  `--internal sound_loop_set`, `EQUAL physics_sym: 313 words` in the same run. E8B4 `21 of 85 … ops 0 … frame 96/80`;
  draft_check (v2) `EQUIVALENT over 571 calls / 2 scripts (options_menus 331, pause_menus 240)`. Packaging of
  sound_loop_set needs a group holding its callers (physics_sym group) and slot_state_setup (credits_scroll_grp):
  coordinator decision (merge, or wait for E8B4).
- func_8010AEAC/A8D0/A7A4 (optionscr.c animText/animButtonText/printOption): register parameters s1–s4 appear with the
  GC prototype order printOption(x, y, item, name, value) and the function non-static + `--internal`. TB_FlashTextColors
  takes the two flash colours (two separate globals D_801146BC/D_801146C0, not an array) by value; their stack homes
  (40/44, 32/36 in printOption) need the params address-taken. AEAC also saves f20–f24 and s0–s8 for A8D0 (A8D0 must be C
  in the unit before AEAC's prologue can match). Draft crashes the recomp host on options_menus (A8D0 path).
- func_8010DCFC: `145 of 165 … ops 11 … frame 136/104`, see header.

## GC reference: what it bought
- Direct structure + names: ShowCountdown (→ code-identical), animLapTime (locals list closed the frame with one extra
  slot), _TitlePane (unit EQUAL), animControls (ops 325 → 11), animText/animButtonText/printOption (register-parameter
  prototype order, flash-colour helper), animWrongWay (names; no match).
- Not found on GC: func_80106B3C, func_80107EDC, func_8010BC84, func_8010DCFC have no confident pair; hud.c/pausescr.c
  family idioms (Hidden, TB_SetJust, HAL_SetFont) still transfer.
- GC-derived names (N64 symbol → GC name): audio_doppler_full → ShowCountdown; D_80151AD0 gNumWindows; D_80152032
  gCountDown; D_80150B70 gCamera; D_80140BDC gNumModels; func_8008C544 WorldVector; func_800B24EC MBOX_FindTexture_Sub;
  func_800A78BC ZOID_NewPoly; func_8008C074 ZOID_UpdatePoly; func_8008D0C0 ZOID_RemovePoly; entity_flags_apply
  PlayGameSound; viGetTimeToDeadline GetCountdownTimeRemaining; game_results_input animControls; Input_ApplyPadConfig
  UpdateBlit; func_800EF5B0 RenameBlit; stat_race_update SelectBlit; brake_light_update ZOID_ProjectWindow;
  render_helper HAL_SetTextDepth; func_800ED66C HAL_SetTextTranslucency; dispatch_handler TB_SetTextColor;
  state_utility TB_DrawText; object_manager_update HAL_GetTextWidth; object_bytes_sum_global TB_GetFontLineHeight;
  camera_shake_update (char width); object_byte9_set (fixed-width digits toggle); ambient_sound_set InitPane;
  crowd_cheer_play ChangePaneSize; sound_loop_set _TitlePane; func_80108154 animLapTime; func_80108AB0 animWrongWay;
  func_8010AEAC animText; func_8010A8D0 animButtonText; func_8010A7A4 printOption; D_80118E20 TB_SetJust pair;
  D_801174B4 gGameState; D_8014A110 gRace; D_8015698C playerInPause; D_8011AD6C gControllerRemoved; D_8011AD68
  gTitlePane; D_80116D0C buttonsInited (options); D_80116D9C gSelectedOption; D_80116D14 validOption[14];
  D_80146108 gOptions; D_801146BC/C0 flashColors; slot_state_setup HAL_SetFont.

## Permission denials
None.
