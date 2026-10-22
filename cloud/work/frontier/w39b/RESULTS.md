# w39b RESULTS (wave 39, MAT3-faithful ports of unmatched functions with a MAT3 identity)

Branch wave37 @ 6e5265bc4. Builder scratch `watchman2:~/rush2049/scratch/frontier/w39b` (copied from `base`, then
src/blob, src/ovl_a, src/ovl_b, include, asm/us/{blob,blob_data,ovl_a,ovl_b} and the lock files rsynced from this
checkout, because `base` predates the w38 installs; trace toolkit reused from wtk). Lane dir only; no git operations.

Status: complete for this session. **No new MATCH.** Every target on the list has a scored MAT3-faithful port (new or
rescored); four new ports moved well past the prior best (_AnimateMine 165 -> 66 rows, updateWarningScreen 50 words -> 10
rows, controlscr animButtonText 49 -> 29 rows, drawDeletePlayer: a MAT3 port reaches the installed 15/917 with ops 0).

Scoring conventions. Game: unit score on the Pi, `TAG=w39b SCR=rush2049/scratch/frontier/w39b sh
cloud/work/frontier/tools/trace/us.sh NAME FILE` (= `python3 -m tools.conveyor.pipeline.blob_unit --tag w39b score NAME --with
FILE $UARGS` + aligned diff); the line quoted is `<blob_unit verdict> | words N ops N norm N | frame want/got`. "words N"
counts aligned differing rows and includes the function's own-.rodata relocation rows (jump tables, float literals), which the
unit cannot resolve. Overlay A: `tools/og.sh ovl_a DIR` (= `score.py group cand/<dir> --targets asm/us/ovl_a` on the builder),
`tools/of.sh ovl_a FILE FN` (= `score.py fn ... --targets asm/us/ovl_a`), aligned rows from `tools/ogd.sh` / `tools/ofd.sh`
(udiff.py logic against the overlay targets, `tools/oudiff.py`).

## Ranked table (closest first)

| # | function | MAT3 identity | best port file (w39b unless noted) | scorer (exact) | inlined helpers | what differs | best next step |
|---|---|---|---|---|---|---|---|
| 1 | ghost_race_setup | GhostEnd@ghost.c | w36w2 best = `ghost/base.c` (straight MAT3 = `ghost/straight.c`) | base: `FAIL ghost_race_setup: 3 of 110 words differ \| words 3 ops 2 \| frame 64/64`; straight: `54 of 110 words differ; compiled body is 112 words \| words 14 ops 4` | DeallocateMemory = func_800960CC | as1 copy-propagates the ghost copy into the header load: retail's ugen emits &gGameCar[i] in the main block (w36w2 listing oracle) | find why uopt computes the car address before the flag test (k1-k3 `ghost` defined at the test: 19) |
| 2 | object_bytes_sum_global / object_bytes23_sum | TB_GetFontLineHeight / TB_GetFontHeight | w36w text / w38a `tb/d1.c` (straight) | `4 of 21 words differ \| words 4 ops 0` / `4 of 18 words differ \| words 4 ops 0` | HAL_GetFontAscent/Descent (locked getters) | &gFontHeader web t2 (ours) vs t4: ctrace (`tbh`): value web t1, address web sees t2..t5 at cost 0; retail has t2/t3 taken by two codeless webs | same phantom-web class as TB_DrawText and TB_EllipseText's unused s5 (all TB_text.c); see What generalises |
| 3 | state_utility | TB_DrawText@TB_text.c | w38a `tbdraw/t1.c` (straight) | `14 of 99 words differ \| words 10 ops 4 \| frame 24/24` | TB_GetFontHeight (unit body), HAL_SetTextPos | ctrace (`tbd`): address webs w101 (`ldaS(99)`, save 0.67) coloured t4, w100 (`ldaS(1058)`) split; retail rematerialises both | one cause with #2 |
| 4 | func_8038A400 (ovl A) | updateWarningScreen@warningscr.c | `A400/dc2la.c` (Dreamcast text + N64 additions, DC statics) | `of.sh`: `118/139 words differ (24 section-relative relocations unverified ...)` (body 2 words short, own .data unplaceable); `ofd.sh`: 10 non-relocation rows | none (w38b: no stub, none possible) | controller scan: retail `beqzl t8,inc; addiu; b exit; nop` + dead `addiu`, ours `bnez t8,exit` (uopt; ugen listing already merged) | (w36e best 50 words) ~40 loop/compare/block forms tried, all 10 |
| 5 | func_803A9088 (ovl A) | drawDeletePlayer@playerscr.c | `A9088/best_body.c` (o8) in a copy of runtime_a_profile_menu_20261006 | `15/917 words differ (8 section-relative relocations unverified ...)`; straight MAT3 m1: `283/917`, aligned 261, ops 25 | - | spill homes only ((s16)cx 72 vs 56, IV/a3 swapped); straight MAT3 is ops 0 after the N64 abs order | home-order oracle on f_spilltemps (which web shares home 0 in retail) |
| 6 | controls_animButtonText | animButtonText@controlscr.c | `ctl/best.c` (= pe3) | `39 of 479 words differ; compiled body is 478 words \| words 29 ops 3 \| frame 544/480` (w36k b3: 49 rows) | ct_SetJust (TB_SetJust), ct_DrawEllipseText (after the caller), ct_font_set (HAL_SetFont), PS_PakRumble | 64 B of frame above screenPos (probe: 22 rows) + two webs retail colours a3 (force both: 13 rows = 6 reloc + 7 schedule) | find the 64 B and the a3 preference of the inlined `text` parameter |
| 7 | func_8010D680 | _AnimateWeaponIcon@targets.c | w36w best (straight = minus the disclosed `pad`) | straight: `86 of 119 words differ ... \| words 26 ops 12`; w36w: `words 22 ops 12` | ZOID_ObjectFlags | op narrowed into a1 (retail a coloured web), as1 xbb hoists | (w36w notes) |
| 8 | func_8010D3C0 | _StartWeaponIcon@targets.c | w36w best (already straight) | `72 of 176 words differ ... \| words 23 ops 4 \| frame 32/32` (8 rows own-rodata) | - | as1 xbb hoist into the jump-table dispatch (w36w `.livereg` oracle) | (w36w notes) |
| 9 | explosion_effect | animSpeedText@hud.c | w38a `ee/x2.c` (straight = `ee/straight.c`) | x2: `282 of 365 words differ; compiled body is 366 words \| words 33 ops 4`; straight: `words 73 ops 25` | 7 `*_Speed` helpers internal | &gGameCar split piece t0 vs v0, constants 1/10 | (w36x3 notes) |
| 10 | physics_float_calc | WorldCullObjects@world.c | w36x3 best (straight = `wco/straight.c`) | best: `52 of 294 \| words 39 ops 10`; straight: `213 of 294; compiled body is 312 words \| words 241 ops 53` | WorldDistance | holders needed for ptr (save 0); inline area 8 B | (w37c notes) |
| 11 | particle_lifetime_set | createBG@front.c | w37b best (straight = `cbg/dc1.c`) | w37b: `51 of 183 \| words 58 ops 0`; dc1: `52 of 183 \| words 59 ops 0` | ZOID_ShowPoly | ugen ring one pop behind from the gBackCamera load | ring-pop source (mask/cast) |
| 12 | race_position_update | TB_EllipseText@TB_text.c | w36w best (straight) | `107 of 121 words differ; compiled body is 114 words \| words 60 ops 16 \| frame 120/112` | HAL_GetFontSpacing | -1 kept in s4 and narrowed per iteration, phantom s5 save, dbl in s6 (w38a) | phantom-web class (#2) |
| 13 | func_8010C974 | _AnimateMine@targets.c | `mine/best.c` (= f1) | `68 of 659 words differ \| words 66 ops 5 \| frame 392/376` (w36s: 167 words, 165 rows; straight MAT3: 383 rows) | MBOX_NewObject, ZOID_* (w36s helpers) | 16 B inline area (probe p2: 49 rows), j/pointer register rotation in three loops, mat[1] add schedule | find the 16 B inline chain; then j as one web |
| 14 | CreatePlayer | CreatePlayer@save.c | w32c `lead_f2.c` | `256 of 272 words differ; compiled body is 271 words \| words 120 ops 43 \| frame 224/224` | (N64 unique-name helper, `_Pow`) | MAT3 text covers only the tail; the N64 head is a filename builder | identify the two stub helpers (w38a) |
| 15 | controls_animText | animText@controlscr.c | w36k best | `365 of 394 words differ; compiled body is 390 words \| words 151 ops 35 \| frame 360/352` (TB_SetJust helper `ctl/t1.c`: ops 33, frame 368) | ct_* | (w36k notes) | - |
| 16 | pak_delete_file_retry | PS_DeleteFile@paksysdc.c | w36v best | `168 of 196 words differ; compiled body is 169 words \| words 177 ops 41 \| frame 232/208` | 9 helpers internal | MAT3 body is the PS2 card version; draft drifted with the installed paksys group | rebase on the installed w37a_paksys packet |
| 17 | func_8010EA08 | animButtonText@videoscr.c | w33b best | `484 of 514 words differ; compiled body is 511 words \| words 287 ops 86` | vs_* | (w33b notes) | - |
| 18 | func_8010F218 | animText@videoscr.c | w33b best | `423 of 611 words differ; compiled body is 518 words \| words 324 ops 113 \| frame 456/384` | vs_drawColorBars etc. | unrolled bar loop (w33b) | - |
| 19 | display_list_flush | PrePlay@game.c | w36p best | `499 of 589 words differ \| words 395 ops 55 \| frame 256/88` | InitStunt etc. | spill-driven frame (w37c) | - |
| 20 | track_render_process | PS_PakServer@paksysdc.c | w37a group.c | `752 of 800 words differ; compiled body is 771 words \| words 603 ops 83 \| frame 432/424` | 10 internals | N64 osPfs body; MAT3 is PS2 card logic | (w37a notes) |

Not attempted (per the brief): func_80087110, camera_transform, car_shadow_render, debug_stats, debug_collision,
time_result_display, updateTrackScreen, animTrackMaps.

## Matches
None. No `cloud/matches/` file and no group packet were written; nothing in this lane is spliceable.

## Per-function notes

### func_803A9088 = drawDeletePlayer@playerscr.c (overlay A, 3,668 B): MAT3 port = 15/917 (same as installed)
- Port files: `A9088/m1_body.c` (straight MAT3), `A9088/best_body.c` (= o8; MAT3 + three N64 facts), spliced into a copy
  of the installed `runtime_a_profile_menu_20261006` group by `A9088/mk.sh BODY DIR`.
- Scores (`tools/og.sh ovl_a DIR` = `score.py group cand/<dir> --targets asm/us/ovl_a`, context line for func_803A9088;
  aligned rows from `tools/ogd.sh`):
  - installed body (native-led): `15/917 words differ (8 section-relative relocations unverified ...)`; aligned 23 rows
    (8 of them the own-rodata relocation rows), frame 272/272.
  - m1 straight MAT3 (`abs` as `x<0 ? -x : x`, label passed inline): `283/917 words differ`; aligned 261 rows, ops 25,
    frame 272/264.
  - m2 = m1 + N64 `abs` order `(x) >= 0 ? (x) : -(x)` + the menuItem==1 label assigned to `str` first: ops **0**, 86 rows,
    frame 272/264 (structure identical; only the frame/homes differ).
  - o5 = m2 + `i` declared between menuItem and screenPos (screenPos lands at retail's 112) + an N64 `f32 delta` local
    for the case-2 offset: frame 272/272, 29 rows.
  - **o8 = o5 + N64 multiply order `45.0f * gTextPosScale * 300.0f / buttonZ`** (MAT3 writes `300.0f * (45.0f * s)`;
    retail `mul.s f4,f10,f24`): `15/917 words differ` = the installed body's residual exactly.
- Residual (both bodies): spill homes only: (s16)cx at 72 vs retail 56, the button IV at 52 vs 48, &gSelectedPlayer[p]
  at 48 vs 52. `spill.sh` (snapshot a9o8): cfe area 196 = retail's; homes are given in web-bit order and (s16)cx (bit 38)
  reuses home 0 next to the gNumPlayers load web (bit 4); retail puts it in home 4, so in retail that web conflicts with
  home 0's occupant, and &gSelectedPlayer[p] precedes the IV. Not a source-order lever found (declaration orders, `lines`
  local s16/s32 at three places: no gain or worse).
- Conclusion: the MAT3 text is the original (ops 0); the N64 differences are `abs` arm order, one extra `delta` local,
  `i` declared before screenPos, the operand order of the scale products and the label-first statement. Next: the same
  "one more interfering web" residual as 9842C/90DCC; a home-order oracle (patch f_spilltemps order) would say which web.

### func_8038A400 = updateWarningScreen@warningscr.c (overlay A, 556 B): DC port = 10 real rows (from 50 words)
- Port: `A400/dc2la.c` = the Dreamcast warningscr.c updateWarningScreen text (MAT3 keeps only the tail) with the N64
  additions read from retail (4 paks, PS_GetBlocks(0x810), gActiveEdges 0xC00 PlayUDSound + gPakScr toggle, `& 3` start
  test), DC statics (gPakScr, the pak count byte, function-local `static Blit *sBlit`), PS_GetFreeFiles returning
  `unsigned int` (MAT3 prototype; `>= 1` then compiles to retail's `bnez`).
- Scores: `tools/of.sh ovl_a A400/dc2la.c func_8038A400` (= `score.py fn ... --targets asm/us/ovl_a`): `118/139 words
  differ (24 section-relative relocations unverified ...)` because the body is 2 words short, so the own .data cannot be
  placed; aligned diff (`tools/ofd.sh`): only 10 rows that are not own-.data relocations: the controller-scan break
  (retail `beqzl t8,inc; addiu; b exit; nop` + a dead `addiu`, ours `bnez t8,exit`, 2 words) and the `move s2,zero` order
  that follows from it. Everything else (s1 reused for the free-block count / gPakScr toggle / setGameState argument, the
  `lw t5; move a0,t5` RemoveBlit path) comes out of the plain DC text with the statics. w36e's best (`var/n.c`, externs,
  helper) = `50/139 words differ`, 35 aligned rows. Extern instead of static globals: 24 rows (done-block registers).
- Tried for the break shape (all 10 rows; uopt normalises them): for/while/do/goto/continue forms, `!= 0`, else arms,
  compiled-out blocks before/after/inside the loop, pointer-walk loops (e1 33), -O2. ugen's listing (`as1t.sh wsla`)
  already has `bne $24,0,$34`, so retail's `beq; b` comes from uopt, not as1.

### func_8010C974 = _AnimateMine@targets.c (2,636 B): MAT3 port 66 rows (from w36s 165)
- Files: `mine/mat3.c` (straight MAT3 text on w36s's N64 types/zoid helpers), `mine/mat3b.c`..`mat3e.c`, `mine/f1.c`
  (= `mine/best.c`, header documents the shape).
- Scores (unit, `TAG=w39b us.sh func_8010C974 FILE`):
  - w36s best (native-led): `167 of 659 words differ | words 165 ops 8 | frame 392/376`.
  - mat3.c straight MAT3: `503 of 659 words differ; compiled body is 687 words | words 383 ops 50 | frame 392/368`.
  - mat3b = MAT3 with the glow-scale loop as nested 3x3 loops (MAT3's flat `j < 9` loop is a CodeWarrior device per its
    own comment): `words 137 ops 16 | frame 392/376`.
  - mat3c (+ team colour through `clrPtr = &clr; clrPtr->c[i] = ...`, retail `addiu v1,sp,368; sb ..,0(v1)`): 126 rows.
  - mat3d (+ `pos = pos + uv * 0.75f`, retail operand order): `words 68 ops 8`.
  - mat3e (+ init stores alpha, step, scale, elapsed in retail order): `words 66 ops 8`.
  - **f1 = mat3e with the scale loop's outer index a third variable `k` and inner `j`**: `FAIL func_8010C974: 68 of 659
    words differ | words 66 ops 5 norm 5 | frame 392/376` (body length now exact, 659).
- Residual: (1) frame 16 B short. The named locals sit at retail's frame-relative offsets (clr top-24, opos top-40, uvs
  top-76; retail has five 4-byte slots above clr, so i, j, k, t, m), so the 16 B are inline area: a probe `p2.c` with a
  3-deep empty static inline chain gives `frame 392/392`, `words 49 ops 5` (probe only; two flat empty inlined calls add
  only 8, `p1.c`). Deeper wrapper chains for MBOX_NewObject / ZOID_RemoveObject overshoot (q1 440, q2 424). (2) the
  oldest-mine and uvs-copy loops have the index and pointer registers swapped (retail j = a1 in all three loops). (3) the
  mat[1] position adds are scheduled differently.
- Next: find the N64 inline chain that is 16 B deeper (umerge.log of the unit build), then the j web.

### particle_lifetime_set = createBG@front.c (732 B): straight port = 59 rows (w37b 58)
- `cbg/dc1.c` = w37b best with MAT3/DC function-level `backPoly`, `vertIdx`: `FAIL particle_lifetime_set: 52 of 183 words
  differ ... | words 59 ops 0 norm 0 | frame 184/184` (backColor home moves 124 -> 116). w37b best (block-local) rescored:
  `51 of 183 words differ ... | words 58 ops 0`. Residual unchanged (ugen ring one pop behind from the gBackCamera load).

### physics_float_calc = WorldCullObjects@world.c (1,176 B): straight 241 rows; w36x3 best 39 rows
- `wco/straight.c` (w36x3 best without the two `if (ptr) {}` holders, MAT3 declaration order): `213 of 294 words differ;
  compiled body is 312 words | words 241 ops 53 | frame 136/136`. w36x3 best rescored: `52 of 294 words differ | words 39
  ops 10 | frame 136/136`. The holders stay the lever (ptr's save is 0 without a loop use).

### explosion_effect = animSpeedText@hud.c (1,460 B): straight 73 rows; w38a x2 33 rows
- `ee/straight.c` (w38a x2 without `if (y) {}`): `150 of 365 words differ; compiled body is 364 words | words 73 ops 25`;
  `ee/x2.c` rescored: `282 of 365 words differ; compiled body is 366 words | words 33 ops 4 | frame 288/288` (UARGS: the seven
  `*_Speed` helpers internal).

### ghost_race_setup = GhostEnd@ghost.c (440 B): straight 14 rows; best 3 words (unchanged)
- `ghost/straight.c` (pure MAT3, no compiled-out join block): `54 of 110 words differ; compiled body is 112 words | words 14
  ops 4`. w36w2 best (`ghost/base.c`) rescored: `FAIL ghost_race_setup: 3 of 110 words differ | words 3 ops 2 | frame 64/64`.

### func_8010D680 / func_8010D3C0 = _AnimateWeaponIcon / _StartWeaponIcon (476 / 704 B): prior ports are straight MAT3
- D680 straight (`wicon/d680_straight.c` = w36w best without the disclosed `s32 pad`): `86 of 119 words differ ... | words 26
  ops 12 | frame 80/80`; with pad (w36w best): `82 of 119 ... | words 22 ops 12`.
- D3C0 (w36w best, already a straight port with MAT3's `wep`): `72 of 176 words differ ... | words 23 ops 4 | frame 32/32`
  (8 of the rows are own-rodata jump-table relocations).

### controls_animButtonText = animButtonText@controlscr.c (1,916 B): MAT3-shaped port 29 rows (from 49), 7 with forced colours
- Base: w36k `anim/v/b3.c` (GC-based; the baseline needs the ct_* helpers internal: `UARGS="--internal ct_CenterText
  --internal ct_RightText --internal ct_font_set --internal ct_gfx_lock --internal ct_gfx_unlock --internal ct_SetFont
  --internal controls_animButtonText"`): `58 of 479 words differ; compiled body is 478 words | words 49 ops 7 | frame 544/440`.
- MAT3 call shapes, each measured (unit, same UARGS + the new helpers internal):
  - `ctl/j2.c`: MAT3 `TB_SetJust(h, v)` calls as a private inlined helper `ct_SetJust` (retail inlines TB_SetJust here: `li s7,1;
    sw s7,0(t0); sw s7,4(t0)`; the locked func_800B669C is forced kept in the unit): `54 of 479 | words 44 ops 7 | frame 544/480`;
    every inline-area home below the top now sits at retail's sp offset (colour homes 88/92/104/108, ellipse buffer 232).
  - `ctl/pr3.c` = j2 with `gTextPosScale` declared before `screenPos` (screenPos then at retail's 456): `words 33 ops 7`.
  - **`ctl/pe3.c` (= `ctl/best.c`)** = pr3 + MAT3 TB_DrawEllipseText as a private helper `ct_DrawEllipseText` defined AFTER the
    caller (MAT3/N64 body `u8 str[128]; TB_EllipseText(str, text, w); TB_DrawText(x, y, str);`): `FAIL controls_animButtonText:
    39 of 479 words differ; compiled body is 478 words, target 479 | words 29 ops 3 norm 3 | frame 544/480`.
- Residual, measured: (1) frame 64 B: retail has 64 unexplained bytes above screenPos (nothing stored there; w37b census).
  Probe `ctl/best_probe64.c` (pe3 + `u8 probe[64]` before gTextPosScale, a device, not deliverable): `33 of 479 | words 22
  ops 3 | frame 544/544`. (2) two webs that retail colours a3: the rumble-row `str` (retail `lw a3; ... move a2,a3` in the jal
  delay slot, ours loads a2: one word short) and the inlined TB_DrawEllipseText `text` parameter (retail `lw a3,0(t6); addiu
  a1,a3,20`). `force.sh cab controls_animButtonText p1:w223=c6,p1:w85=c6` on the probe build: `want 479 words, got 479;
  differing rows 13` = 6 own-.rodata relocation rows (unit artefact) + 7 schedule rows around the ellipse call. So the whole
  residual is frame (64 B) + those two a3 colours.
- Tried for the a3 colours: u8*/void* types and `(char *)(u32)` laundering on both arguments (no change), MAT3 rumble-arm order
  (77 rows), str reused for the name (`pv1.c`: both webs a3 and body length exact, 21 rows, but the load moves above the `?:`),
  `str = *save` + `str + 20` (`pv2.c`, 25), the ellipse helper `static` (`pe4.c`) or with a `(char *)` cast on `text`
  (`pe5.c`): both 22 (with the probe), unchanged. MAT3 block-local colour copies for TB_FlashTextColors: 166 rows.
- Next: find the 64 B (the inlined areas of HAL_SetFont/PS_PakRumble are already there; candidates: an N64 local array) and
  why retail's inlined `text` parameter keeps the 4th-argument register.

### Prior MAT3-shaped ports rescored in the current unit (no new port: the prior file already is the MAT3 text)
| function | identity | file | UARGS | scorer line (us.sh) |
|---|---|---|---|---|
| object_bytes_sum_global | TB_GetFontLineHeight | w36w/text/best_object_bytes_sum_global.c | - | `4 of 21 words differ | words 4 ops 0 | frame 24/24` |
| object_bytes23_sum | TB_GetFontHeight | w38a/tb/d1.c | - | `4 of 18 words differ | words 4 ops 0 | frame 24/24` |
| state_utility | TB_DrawText | w38a/tbdraw/t1.c | - | `14 of 99 words differ | words 10 ops 4 | frame 24/24` |
| race_position_update | TB_EllipseText | w36w/race_position_update/best.c | - | `107 of 121 words differ; compiled body is 114 words | words 60 ops 16 | frame 120/112` |
| display_list_flush | PrePlay | w36p/display_list_flush/best.c | `--internal InitBattlePoints` | `499 of 589 words differ | words 395 ops 55 | frame 256/88` |
| controls_animText | animText@controlscr | w36k/anim/best.c | ct_* + controls_animButtonText internal | `365 of 394 words differ; compiled body is 390 words | words 151 ops 35 | frame 360/352` |
| func_8010F218 | animText@videoscr | w33b/videoscr/best.c | vs_* + func_8010EA08 internal | `423 of 611 words differ; compiled body is 518 words | words 324 ops 113 | frame 456/384` |
| func_8010EA08 | animButtonText@videoscr | w33b/videoscr/best.c | same | `484 of 514 words differ; compiled body is 511 words | words 287 ops 86 | frame 312/312` |
| CreatePlayer | CreatePlayer@save.c | w32c/CreatePlayer/lead_f2.c | - | `256 of 272 words differ; compiled body is 271 words | words 120 ops 43 | frame 224/224` |
| pak_delete_file_retry | PS_DeleteFile@paksysdc.c | w36v/pak_delete_file_retry/best.c | func_800A1E94 + 8 pdf_/paksys_ helpers | `168 of 196 words differ; compiled body is 169 words | words 177 ops 41 | frame 232/208` (w36v reported 63 rows; the unit has changed under it since w37a/w38a installed the paksys group) |
| track_render_process | PS_PakServer@paksysdc.c | w37a/groups/w37a_paksys/group.c | w37a's 10 internals | `752 of 800 words differ; compiled body is 771 words | words 603 ops 83 | frame 432/424` (w37a: 706/800 with its full packet) |
- Why no new straight port for these: CreatePlayer's N64 head is a unique-filename builder (name filter, `_Pow` digits; two
  caller-less stubs before it) that MAT3's `_GetUniqueExtension(pak, "RUSH2049", "P", ext)` call does not describe; MAT3
  paksysdc.c PS_DeleteFile/PS_PakServer are the PS2/GC memory-card bodies (PS2MC* calls), the N64 bodies are osPfs; the
  videoscr/controlscr animText drafts are already GC-text ports whose residuals are N64 loop/unroll shapes (w33b/w36k notes).

## What generalises
1. **MAT3 shaping devices are CodeWarrior-specific and must be undone for IDO.** MAT3 comments flag them; _AnimateMine's
   "flat 9-element loop" (a CodeWarrior device) is a nested 3x3 loop on the N64 (383 -> 137 rows by itself), and the
   GC's `300.0f * (45.0f * s)` products are `45.0f * s * 300.0f` on the N64 (drawDeletePlayer). Read MAT3's comments as
   "N64 shape unknown here".
2. **The Dreamcast text is the better base when MAT3 keeps only a stub** (updateWarningScreen: MAT3 has the 10-line tail, DC
   has the whole controller/pak scan): the plain DC text with its statics produced retail's odd register reuse (one s1 for
   three values, `lw t5; move a0,t5`) that the native-led draft had needed a reused variable for.
3. **MAT3 prototypes carry signedness that decides branches**: PS_GetFreeFiles returns `unsigned int`, so DC's `>= 1` is
   retail's `bnez`; with `s32` it is `bgtz` plus a duplicated tail.
4. **Inline-call shape is visible in the frame**: writing MAT3's `TB_SetJust(h, v)` calls as an inlined helper (retail
   inlines TB_SetJust although its out-of-line copy is kept) put every lower home of controlscr animButtonText at retail's sp
   offset; an inlined helper defined after its caller fixed the call's schedule (as1 line tie, as in w38a). Frame residuals
   left after that are a top-of-frame block (64 B here, 16 B in _AnimateMine) that probes can size exactly.
5. **force.sh before variants pays off on far functions too**: controlscr animButtonText looked like 49 rows of frame+schedule;
   forcing two webs to a3 (plus a 64 B probe) leaves 7 schedule rows, so the problem is two colour preferences, not structure.
6. **Phantom webs in TB_text.c**: TB_GetFontHeight/LineHeight (t2/t3 unavailable), TB_DrawText (address webs rematerialised)
   and TB_EllipseText (s5 saved, unused) all behave as if extra code-free webs were live. They share HAL_text.c callees
   inlined across files. Tested and rejected here: a private copy of the whole _SetupFontData/getter chain with gFontHeader
   as a file `static` (MAT3's linkage) vs `extern` (`tbs/s1.c`, `tbs/s2.c`): both `6 of 18 words differ | words 6 ops 0`
   with the address web still in t2 (the 2 extra rows are the private callee's jal). Retail's TB_DrawText keeps x/y in t2/t3
   across _SetupFontData, so the callee's clobber summary is not the cause either. Still open.

## Permission denials
None.
