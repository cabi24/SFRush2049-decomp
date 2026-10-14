# func_80104B14 = GC DrawGhost@highscorescr.c (w31a fork, unit tag w31ag)

`best.c` = `hs/best_h1.c` + the DrawGhost body (+ helpers hs_SetJust, gh_font_set, gh_SetFont) in place of the
`extern void func_80104B14(void)`. Unit args: `$(cat cloud/work/frontier/w30a/uargs5480.txt) --internal func_80104704
--internal hs_SetFont --internal func_80104B14 --internal hs_SetJust --internal gh_SetFont --internal gh_font_set`.

Final unit (`blob_unit --tag w31ag score func_80104B14 func_80105480 func_80104704 func_80103D28 --with hs/ghost/best.c ... --neighbours`):
```
  FAIL func_80104B14: 17 of 601 words differ        (us.sh: words 23 ops 0 norm 0 | frame 576/576)
  FAIL func_80105480: 27 of 445 words differ        (udiff: want 445 got 445; differing rows 33; frame 584/704)
  EQUAL func_80104704: 260 words (internal, c_best.c)
  FAIL func_80103D28: 575 of 629 words differ; compiled body is 614 words, target 629
  locked bodies that differ in this unit: 0
```
func_80105480 afterwards: body length now exact (445/445) and its `sdc1 $f20/$f22` saves now appear (ra at 76 as
retail): DrawGhost's 60.0f/1000.0f in $f20/$f22 were the clobber. Remaining 5480 residual = frame only (704 vs 584,
all sp offsets shifted; the inline-area question from w30a).

func_80104B14 residual (17 words, 6 of them the unverified own-.rodata rows: two "%01d:%02d.%03d" literals at
0x80120654/0x80120664 and the jump table 0x80124880): colour only, ops 0. The gGhostStatus[pak] PRE web (pretrace
bit 276) gets v1 and pak*4 (bit 131) t0; retail v0 / v1. force.sh `p1:w276=c1` is declined (forbidden v0 = a
precoloured interferer, not the ghost-pointer web w134: forcing w134 elsewhere does not free v0). Not traced further.

Levers (in order): `str = ghost->name;` BEFORE the time sprintf in the status 1/2 case (GC order differs; 42 -> 33
rows incl. +1 word); `y = object_bytes_sum_global();` before the SetJust stores in the confirm branch, then `y * 4`
(retail calls the line height before the stores) -> ops 0; frame 440 -> 576: TB_SetJust as an inlined helper
hs_SetJust(h, v) at its 6 sites (+48 B), HAL_SetFont as a void wrapper gh_SetFont over a SEPARATE copy gh_font_set
at the 10 font sites (+80 B; wrapping the shared hs_font_set instead stops umerge inlining it: body 488 words), and
one unused `s16 gFreePages` declared after `str` (GC DrawGhost local; exact frame residual: 568 without, 576 with,
one slot) -- disclose as an unused local.
No-effect variants: time case first in the switch (worse, 115 rows), explicit default/brace forms (same).

GC-derived names (all from GC DrawGhost@highscorescr.c unless noted): gSavedGhost D_801461A8 (Ghost **),
gPakGhosts D_80146198 (Ghost **[pak]), gGhostPak D_801460C0 (s8), gGhostMenuConflict D_801460F0, gGhostMenuConfirm
D_801460FC, gGhostMenuNotify D_80146148, gGhostMenuSelect D_80144DA0, gGhostStatus D_801461C0[pak],
gPS_PakName D_8011EAD0 ("1".."4"); D_80146131 = the two-option confirm selection (GC gGhostConfirmMenuSelect is a
different shape: N64 draws two options at x 145/175); Ghost layout name +10 (GC +0xe), time f32 +32 (GC +0x24);
TB_DrawTextScrollBox = camera_auto_follow(160, 110, 130, h*6 / h*4, -1, 0, text); status switch cases
-4 fixed[165], -3 fixed[169], -2 fixed[166], -1 fixed[168], -5 fixed[160], 0 strings[h[46]+i], 1/2 ghost name+time.
