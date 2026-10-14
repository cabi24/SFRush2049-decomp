# w32b — screen units + menus (wave 32), IDO 5.3 -O3

Status: FINAL (wave 32). Unit tag `w32b`; builder scratch `watchman2:~/rush2049/scratch/frontier/w32b` (deleted at
the end). Tree: branch `wave32` at `d1c53ab2` (src/blob + lock synced to the builder at start).

| function | bytes | start | end (unit / group) | draft_check | residual |
|---|---:|---|---|---|---|
| game_results_render (GC animText@audioscr.c) | 1,164 | 14 rows (w31a) | **MATCH in group** `frontier_audioscr`; unit `EQUAL ... 291 words (kept)` | EQUIVALENT (w31a, 1,273 calls) | none |
| game_results_update (GC animButtonText@audioscr.c) | 1,172 | 23 rows | **MATCH in group** (code 0 diffs; `NOT VERIFIED` two-region own .rodata, splice-proof replay image-identical); unit `EQUAL ... 293 words (internal)` | covered | none |
| func_800BEA3C (TB_FlashTextColors, GC union) | 28 | locked single | member of `frontier_audioscr` (same 7 words, kept) | — | — |
| func_800DB1E0 (GC updateOptionScreen@optionscr.c) | 1,400 | unit EQUAL (blocked) | **MATCH in group** `frontier_optionscr_buttons` (kept) | EQUIVALENT (w31b) | none |
| func_800DA2C0 (GC updateButtons@optionscr.c) | 2,316 | 202/579, frame 120/152 | **MATCH in group** (code 0 diffs; `NOT VERIFIED` two-region own .rodata + own .bss statics; splice-proof replay image-identical); unit `EQUAL ... 579 words (internal)` | EQUIVALENT (w31b, 961 calls) | none |
| func_80100564 (GC animText@controlscr.c) | 1,576 | 378 rows, frame 352/360 | 253 rows, frame 360/360 exact | **EQUIVALENT** 1,949 calls / 2 scripts | p coloured (s8) vs retail memory; constant-10 web; p+1 PRE |
| game_results_exit (GC animButtonText@controlscr.c) | 1,916 | 234 rows | 234 rows (not reworked) | covered by 0564 run | `goto end` tail duplicated; frame 520/544 |
| func_8010E8B4 (GC initScreen arm) | 340 | 3/85 | 3/85 (12 more variants) | EQUIVALENT (w29b) | y must be a coloured web != h's register; no source form |
| func_8010F218 (GC animText@videoscr.c + drawColorBars) | 2,444 | not drafted | NEW draft: 536/611 words, frame 392/456 | **EQUIVALENT** 569 calls / 2 scripts (unit) | uopt loop unrolling of the colour bars not reproduced |
| func_8010EA08 (GC animButtonText@videoscr.c) | 2,056 | not drafted | NEW draft: 511/514 words, 287 rows, frame 312/312 exact | exercised inside F218 | FlashTextColors argument-copy idiom; colour |

**New coverage ready to install: 6,052 B** (audio pair 2,336 B; button pair 3,716 B), two packets.

## 1. Audio-options pair — MATCH (lever 1 closed)

Lever: the toupper loop. With the w31a form (`for (...; *charPtr; ...) if (lower) *charPtr = *charPtr ^ 0x20; else
*charPtr = *charPtr;`) uopt keeps `cvt.J(ilod *charPtr)` (pretrace bit 140) as its own PRE web beside the
loop-carried load (bit 139): both live in the compare/xor blocks, so they interfere and the cvt web costs a `move
v1,v0`. Writing GC's shape — an explicit `!= 0` loop test and ONE unconditional ternary store (GC stores
`stb r3,0(r4)` in both cases) — makes the loop test, the compares, the xor and the store all read the cvt value, so
the plain load web only lives up to the cvt and both share v0:
```c
for (charPtr = str; *charPtr != 0; charPtr++) {
    *charPtr = (*charPtr >= 'a' && *charPtr <= 'z') ? *charPtr ^ 0x20 : *charPtr;
}
```
(Equivalent forms that also close it: `!= 0` test + `else *charPtr = (s32)*charPtr;`, or `+ 0`.) The SetJust store
order residual of w31a was gone already in this tree (update's 23 rows were 14 unverified .rodata rows + the move).
Variants: `audio/v/` (b*: loop/compare spellings — `(u32)` compares close the move but emit `sltiu`; d*: else-store
forms alone; e*: the closing combinations).

Packet `groups/frontier_audioscr/` (NEW group): `audio.c` (= `audio/best.c`), `ctx_gr3_b.c` (byte-identical copy of
`src/blob/groups/credits_scroll_grp/gr3_b.c`, = physics_sym's copy; slot_state_setup takes its font in s2 only with
its body in the program); members/claims = game_results_render, game_results_update, func_800BEA3C; keep =
game_results_render, func_800BEA3C; context slot_state_setup, object_create, func_800B669C.
- Unit (Pi): `blob_unit --tag w32b score game_results_render game_results_update func_800BEA3C --with
  cloud/work/frontier/w32b/groups/frontier_audioscr/audio.c $(cat cloud/work/frontier/w32b/audio/uargs.txt) --neighbours`
  ```
  EQUAL game_results_render: 291 words (kept, c_audio.c)
  EQUAL game_results_update: 293 words (internal, c_audio.c)
  EQUAL func_800BEA3C: 7 words (kept, c_audio.c)
  locked bodies that differ in this unit: 0
  blob_unit score: 3/3 equal
  ```
- Builder `score.py group cand/frontier_audioscr` (and `--claims`): `game_results_render: MATCH` (`own .rodata
  verified at 0x8011F6C4..0x8011F6D4`, `0x8011F6D8..0x8011F6E0`), `func_800BEA3C: MATCH`, `game_results_update: NOT
  VERIFIED (14 section-relative relocations unverified: ... own .rodata: this function's references disagree on the
  section's image address (0x8011F6E0 for +0x0/+0x4, 0x80124804 for +0x24/+0x28/+0x2c/+0x30/+0x34): the literals are
  not laid out as in retail)` — the known two-region case (strings + float literals), exit 1 only for that line.
- Splice-proof replay (`tools/gproof.py GROUP_DIR`: builds the object with `blob_group.builder_script` in my builder
  scratch, then runs `blob_group.group_bodies` — the exact relocation/own-data/independent-regions proof
  `install_group` runs — read-only, nothing under tools/ or src/ touched): `group_bodies OK: func_800BEA3C,
  game_results_render, game_results_update`; each member `0 differ from the image`; `3/3 members image-identical`
  (`groups/proof_audioscr.log`).
- **Integration**: `install_group.py cloud/work/frontier/w32b/groups/frontier_audioscr frontier_audioscr
  --revert-single func_800BEA3C` (the locked Color4 single moves into the group with the same 7 words; it stays KEPT —
  other callers: save_load_data.c, func_8010E828.c, several groups). unit_overrides `force_internal` (only if the
  group derivation does not already make non-keep names internal): game_results_update, au_gfx_lock, au_gfx_unlock,
  au_font_set, au_CenterText, au_RightText, au_SetJust, au_SetFont, au_validOption. No data-symbol changes.
- Shaping devices (disclosed in the header): inlined non-static helpers au_* (GC HAL lock/font, TB_CenterText,
  TB_RightText, TB_SetJust, HAL_SetFont void wrapper, validOption `return 1`). No unused locals.

## 2. Options button pair — MATCH (pool item 3 closed)

`groups/frontier_optionscr_buttons/buttons.c` (= `func_800DA2C0/best.c`). Path (unit `--internal func_800DA2C0
--internal ZOID_SetColor_da --internal ZOID_SetColorP_da`, true words / rows):
| step | change | result |
|---|---|---|
| a0 | w31b best | 202/579, 88 rows, frame 120/152 |
| f2 | GC `ZOID_SetColor(s16 objnum, union *col)`@zoidtexture.c as an inlined helper (GC calls it twice; N64 had the stores written out) | 85, frame 136 |
| g3 | one nested helper level (outer passes `col` on) — the 32 B below `color` were 2 sites x 2 levels x 8 B | 77, frame 152/152 exact |
| g4 | keep `u0` (slot placement: texIdx/color at 102/96 as retail) | 74 (56 rows) |
| j2 | yi holder moved OUT of the loop (`if (yi) {}` after it): yi 22/14 = 1.571 -> s8, pointer webs 2.07 -> s6/s7 as retail | 67 |
| k1/l2 | `do {} while (0);` after `color.c[3] = alpha` (byte store and helper load in separate blocks: &color no longer CSE'd into a3; retail `sb v1,99(sp); lw v0,96(sp)`); a second post-loop `if (yi) {}` (yi had dropped to 1.467 vs &D_80116D10's 1.476) | 10 words: one FP tie |
| r1 | `do {} while (0);` after the pre-loop `func_800B5898(1.5707964f, ...)`: one more live block for the hoisted 1.5707964f only, breaking its exact 30/16 tie with 40.0f (force oracle `p1:w283=c34,p1:w305=c33` -> 0 code rows) | **EQUAL** |
- Unit (Pi): `blob_unit --tag w32b score func_800DB1E0 func_800DA2C0 --with
  cloud/work/frontier/w32b/groups/frontier_optionscr_buttons/buttons.c --internal func_800DA2C0 --internal
  ZOID_SetColor_da --internal ZOID_SetColorP_da --internal ZOID_GetInst_da --internal ZOID_SetColorW_da --neighbours`
  ```
  EQUAL func_800DB1E0: 350 words (kept, c_buttons.c)
  EQUAL func_800DA2C0: 579 words (internal, c_buttons.c); own zero-initialised data (addresses consistent, inside game .bss, no overlaps; nothing to compare)
  locked bodies that differ in this unit: 0
  blob_unit score: 2/2 equal
  ```
- Builder `score.py group cand/frontier_optionscr_buttons` (and `--claims`): `func_800DB1E0: MATCH` (`own .rodata
  verified at 0x80124248..0x80124280`); `func_800DA2C0: NOT VERIFIED (32 section-relative relocations unverified ...;
  own .rodata: this function's references disagree on the section's image address (0x80121048 for +0x0/+0x10,
  0x8012420C for +0x18/+0x1c/+0x20/+0x24/+0x28): the literals are not laid out as in retail)`, `own .bss placed at
  0x8015273C (4 bytes ...)`, `own .bss placed at 0x80152740 (4 bytes ...)` — strings + float literals, the
  physics_sym/func_800B59F0 precedent.
- Splice-proof replay (`tools/gproof.py`): `group_bodies OK: func_800DA2C0, func_800DB1E0`, `func_800DA2C0: 579
  words, 0 differ from the image`, `func_800DB1E0: 350 words, 0 differ from the image`, `2/2 members
  image-identical` (`groups/proof_buttons.log`).
- **Integration**: new group, `install_group.py cloud/work/frontier/w32b/groups/frontier_optionscr_buttons
  frontier_optionscr_buttons` (no revert: neither function was locked). members/claims func_800DB1E0 (keep),
  func_800DA2C0; no context needed. unit_overrides `force_internal` (if not derived): func_800DA2C0,
  ZOID_SetColor_da, ZOID_SetColorP_da. Own .bss: the function-local statics D_8015273C / D_80152740 (GC
  cursorSpeed$472 / scrollSpeed$473) become the TU's .bss at their retail addresses; no src/blob extern-data
  definition of them exists. func_800D816C / func_800D91A0 (Astra's) untouched.
- Shaping devices (disclosed in the header): two compiled-out `if (yi) {}` after the button loop; two `do {} while
  (0);` block breaks (after the colour byte store, after the pre-loop 1.5707964f call); one unused named local `u0`
  (frame 152 either way by 8-B rounding; without it texIdx/color sit 4 B higher: 106/100 vs retail 102/96 — GC has a
  `tex` pointer local the N64 code never uses); inlined non-static ZOID_SetColor helpers; GC statics named by their
  retail addresses.

## 3. Controls pair — improved, not closed (lever 2, one lane-hour)

`controls/best.c` (from w30a `controls/best.c`). func_80100564: 378 -> 261 rows with GC's `validOption(player, i)`
— the locked animControls.c body (same GC source file, controlscr.c; GC's call is `validOption(extsb p, i)`),
including its `if (player) {}` and named `valid` — then frame 352 -> 360 exact with a void `ct_SetFont(11)`
wrapper (GC HAL_SetFont) at the per-player font site: **253 rows** (`365 of 394 words differ; compiled body is 410
words`). Traced residual (ctrace): p (w0, save 12.83 = 231/18) takes s8; retail gives s8 to the constant 10 (save
10.53, both `i == 10` compares) and keeps p in its home (352) with every use a `lw`. The `if (player) {}` use adds
p's inner-loop use (131/8 -> 231/18). Also p+1 is PRE'd between the sprintf and the increment (insertions on every
`continue` edge); retail recomputes it from memory at one shared tail. Tried: `volatile p` (502 rows), address-taken
p via an inlined `ct_touch(&p)` (317: kills the strength-reduced IVs retail has), player type s8/s16/s32/u16 (no
change), removing the holder (366). game_results_exit unchanged (234 rows; `goto end` tail duplicated per early exit,
frame 520/544 — probably three more 8-B helper areas: GC's HAL_SetFont at three of its seven font sites).
Next lever: lower p's priority below 10.53 without dropping its inner-loop liveness (the holder in a weight-1 place
that still keeps p live across the inner loop), then the p+1 PRE (retail behaves as if p is reloaded at the tail).

## 4. func_8010E8B4 — unchanged 3/85 (lever 4, ~40 min)

Force oracle (`force.sh e0 func_8010E8B4 "p1:w36=c8"`): colouring the cvtl web elsewhere gives 11 rows — the residual
is not a colour choice of y; retail's y is not a coloured web at all: h's unnarrowed expression is coloured (t0)
and the narrowed value is a ugen ring temp (t9) used twice. Variants (`func_8010E8B4/v/`): inlined `pane(s16 y)` /
`pane(s32 h, s16 y)` helpers with h live or not (b1-b6: same 3 code words, frame 88 — the helper's area replaces
`u0`), a `narrow(s32) -> s16` return-web helper (c1-c4: 20-23; y becomes a ring temp but h goes to v0 and the base
to t0), holders `if (h) {}` around it (d1-d4: 12-22). Best stays w31b's (`func_8010E8B4/best.c`, copied unchanged).
Next: make h's expression a coloured web (two uses) while y stays an uncoloured CSE temp — e.g. h used once more in
the same block in a code-free way before the narrowing; `SCHED=1 ugt.sh` on c1 shows the ring order.
physics_sym packet: not delivered (E8B4 not matched; the `inline_blockers` TitlePane entry stays).

## 5. Video-options pair — NEW complete draft (pool item 5)

`videoscr/best.c` (= `videoscr/v/u1.c` + header): func_8010F218 = GC animText@videoscr.c with GC **drawColorBars
inlined at its head** (w31b's note "minus drawColorBars" was wrong: F218's first 0x5E8 bytes are the colour bars as
GBI words on D_80149438 — pipe sync, combine 0xFCFFFFFF/0xFFFDF6FB, prim depth (16,0), white tick
(24,140)-(32,160), 32 grey PrimColor bars x = 32.., c = 0..248 step 8, a black bar, the white/black/red/green/blue
reference blocks at y 160..200, pipe sync), then GC animText minus the frequency row (N64 options 0 and 1 only:
"%s%d" with "+"/"" for gVertCenter/gHorzCenter). func_8010EA08 = GC animButtonText (buttons, then the test pane
and the confirm pane with yes/no; widths D_80156988 / D_801569B0 set to 160 at entry, heights D_8011AD80 /
D_8011AD84). Unit (`--internal` list in `videoscr/uargs.txt`): `FAIL func_8010F218: 520 of 611 words differ;
compiled body is 536 words, target 611 | words 529 ops 147 | frame 456/392`; `FAIL func_8010EA08: 484 of 514 words
differ; compiled body is 511 words, target 514 | words 287 ops 86 | frame 312/312`.
Residuals: (a) retail's 32-bar loop is unrolled by 4 (colour IV copies s0/s1/s2/t4, exit `t4 != 280`) and the
three colour blocks are straight-line on an x induction register (a 3-trip loop fully unrolled; immediates for the
colours) — none of 8 loop spellings (`v/b1-b4`, `c1-c3`) made uopt unroll here; that also explains the 64-B frame
gap only if the unrolled copies need spill homes (unchecked); (b) EA08's FlashTextColors sites: retail copies the
first colour through a stack temp with an `at` word copy and stores the second to its parameter home — neither the
locked Color4 body (315 rows) nor the union body (287) gives it.
GC-derived names (videoscr.c): buttonsInited D_8011AC94, validOption[4] D_8011AD40 (s8 array, not GC's function),
gVideoMenuSelect D_8011AD44, gVertCenter D_8011AD4C, gHorzCenter D_8011AD50, gTestFrequency D_8011AD54,
gConfirmFrequency D_8011AD58, gYesFrequency D_8011AD5C, gTestPane D_8011AD60, gConfirmPane D_8011AD64,
gControllerRemoved D_8011AD6C, menuRect.x D_8011AD7C, height$637 D_8011AD80, height$646 D_8011AD84, width$636
D_80156988, width$645 D_801569B0, buttonInfo D_8011A994 (64-B entries; title position +0x2F0 = D_8011AC84);
proposed function names video_animText (F218) / video_animButtonText (EA08); TB_GetTextNumLines = camera_zoom_fov,
InitPane = ambient_sound_set, TB_DrawTextScrollBox = camera_auto_follow, RemovePane = sound_handles_array_clear
(w31b map confirmed by the draft's behaviour).

## draft_check
- videoscr, first draft (`v/a1.c`, locked Color4 FlashTextColors): `python3 -m tools.recomp.draft_check --remote
  rocky --unit func_8010F218,func_8010EA08 cloud/work/frontier/w32b/videoscr/best.c --scripts options_menus,pause_menus`
  -> `VERDICT: EQUIVALENT over 569 calls / 2 scripts (options_menus 330, pause_menus 239)`; `unit members (outermost
  calls compared): func_8010F218 569, func_8010EA08 0`; `NOTE never called from outside the unit: func_8010EA08
  (exercised only through other members, inside their compared calls)`; ipa shim slot_state_setup (s2).
- videoscr final (`best.c`, union FlashTextColors body, draft sha256 4ee4dbcf65014c71): same command -> `VERDICT:
  EQUIVALENT over 569 calls / 2 scripts (options_menus 330, pause_menus 239)`, same unit note (`videoscr/dc.log`).
  Limit: func_8010EA08 is reached only inside F218 when buttonsInited == 1; whether these scripts take that path is
  not shown by the verdict (no separate EA08 call count).
- controls (`controls/best.c`): `python3 -m tools.recomp.draft_check --remote rocky func_80100564
  cloud/work/frontier/w32b/controls/best.c --scripts controls_frontend,pause_menus` -> `VERDICT: EQUIVALENT over 1949
  calls / 2 scripts (controls_frontend 1110, pause_menus 839)` (`controls/dc.log`).

## Proposed names (GC-derived)
game_results_render -> audio_animText, game_results_update -> audio_animButtonText (audioscr.c, as w31a);
func_8010F218 -> video_animText, func_8010EA08 -> video_animButtonText (videoscr.c); func_800DB1E0 ->
updateOptionScreen, func_800DA2C0 -> option_updateButtons (optionscr.c updateButtons); ZOID_SetColor (zoidtexture.c)
is inlined (no N64 symbol).

## What generalises
- **A loop-carried byte load and its int conversion are two PRE webs unless every in-loop use reads the conversion**:
  make the loop test explicit (`*p != 0`) and store through one ternary so the else arm stores the converted value;
  then the load web ends at the cvt and both share a register (closed the audioscr pair).
- **"N bytes of frame with no stack traffic below a local" = inline areas of a GC helper the N64 draft wrote out**:
  GC's call list (ZOID_SetColor x2) named it; one nested level per site (8 B each) made the frame exact.
- **Holder strength is placement**: an `if (x) {}` inside a loop adds 10x the weight of one after it; when a holder
  over-raises a web, move it out of the loop (and add a second weight-1 one) to land between two competitors.
- **`do {} while (0);` as a block break**: (a) separates a store from a load so a local's address is not CSE'd into a
  register; (b) after a pre-loop use of a hoisted FP constant it adds a live block to that constant only, breaking an
  exact priority tie with another hoisted constant (force oracle first).
- **Read-only splice-proof replay** (`tools/gproof.py`): building the group object with `blob_group.builder_script`
  and running `blob_group.group_bodies` locally gives the install verdict for two-region NOT VERIFIED members
  before the integration lane touches anything.

## Permission denials
None.
