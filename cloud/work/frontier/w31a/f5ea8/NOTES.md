# func_80105EA8 = GC animResults@hud.c (w31a fork, unit tag w31ae) -- first draft by any lane

Files: `best.c` (= `d.c` -> `d2.c` -> `d3.c`), `retail.s` (tdis dump), variants in `v/`.
Unit: `python3 -m tools.conveyor.pipeline.blob_unit --tag w31ae score func_80105EA8 --with cloud/work/frontier/w31a/f5ea8/best.c --internal ar_font_set --internal ar_SetJust --internal ar_SetFont --neighbours`
```
  FAIL func_80105EA8: 562 of 627 words differ; +0x468: .rodata+0x18: retail words encode 0x0013d0d8, outside the image; ... (+3 more)
       defined by c_best.c (kept)
       umerge inlined into it: ar_SetFont, ar_SetJust
  locked bodies that differ in this unit: 0
```
us.sh: `FAIL func_80105EA8: 562 of 627 words differ | words 358 ops 41 norm 41 | frame 400/400` (body length and frame exact;
the 6 own-.rodata format strings are written as literals and verify as "unverified" rows).
Progression: first complete draft `d.c` 613 rows / ops 90 / frame 360; `d2.c` (GC local structure: no extra h/t locals,
place/min/sec/thou block-scoped in the default case) 360 rows / ops 41 / frame 368; `d3.c` (TB_SetJust as an inlined
helper at its 2 sites + a void HAL_SetFont wrapper at the font site) frame 400/400, 358 rows / ops 41.

draft_check (one run): `draft_check: ERROR: func_80105EA8 takes register parameters (t9, build/frontier.json facts): pass
--params PARAM=REG,... for every parameter of `s32 func_80105EA8(s32 arg0)` ...` -- the frontier fact `register_params t9`
looks spurious (retail homes a0 at 400(sp) like every other screen anim; the function is KEPT); needs
`--params arg0=a0` after regmap_verify, or a fact correction (w31r item).

Residual class: register colouring (callee-saved assignment), not logic. ctrace (snapshot `st_ea3` in
watchman2:~/rush2049/scratch/frontier/w31a, toolkit there): 9 webs take s0..s8 and `x` (w97, save 4.44, tot 40 < best 120)
is split, so x lives at its home 392 (`sw t7,392(sp)` / `lh a0,394(sp)`); retail keeps x in s8. The extra callee-saved
web in ours is `playerIdx*16` (pane-create 7th argument): ugen/uopt emit it BEFORE the font-height call
(object_bytes_sum_global) so it lives in s3 across the call; retail computes `sll t6,s6,4` after the call. Tried
without effect: `playerIdx << 4` (same placement), assignment inside the argument list, height split over two
statements, `y + h` operand order, the pane block as an inlined helper (frame +16). Forced colouring with guessed web
roles was declined (needs the exact retail web map). Other small rows: as1 places the first SetJust stores after the
osRecvMesg argument moves (retail before), the mfhi/ctc1 order in the two time conversions.
Next lever: find why playerIdx*16 is hoisted above the call (pretrace the pane block; try making the per-player pane
index a GC-style pane-slot expression or computing the args through the panes array element), then force.sh with the
retail map (retail: s0 = &D_80116148[p] / old font, s1 = &input_rec0[p], s2 = &panes[p], s3 = model ptr,
s4 = &countdown_state, s5 = y, s6 = playerIdx, s7 = str, s8 = x; head: s8 = &gNumWindows, s4 = numFinished, s5 = aFinished).

GC-derived names (all from GC animResults@hud.c): model = D_8014A250 (stride 2056: car index +1990 (GC 0x7c6), f1996
(GC 0x7cc, "winner" flag), lap +2024 (GC 0x7e8)); gGameCar = player_array (952 B; GC 0x3cc stride differs: place +238
(GC 0xee), finished f239 (GC 0xef), race time f32 +240 (GC 0xf0), f931 (GC 0x3b7, battle/other count)); gNumLaps =
D_80152734; gDeathMode = D_80142760; gRankedPlayer = D_80143F54; ties = D_80150B60; gLap = D_80144018[p]; gStuntPlayer =
D_8015204C (stride 120; GC +0x14 score); wrongWayPos = D_80115F28; gPlayers = input_rec0 (name ptr +72); gRace =
gameplay_mode; text ids fixed[198] place, [199] ranked player wins, [200] winner flag text, [201] ties, [207] DNF,
[208] time label, [130] stunt/points label, [214] lap-5 label; strings[h[55] + place] place names. N64-only (not in GC
animResults): the on-demand pane creation (D_80154350[p] / [4] via ambient_sound_set(x-69, y-h-4, x+69, h+y+4, 192, 0,
p*16, 1) + tournament_unlock_check(pane, &D_80116148[p].x, &.y)).
