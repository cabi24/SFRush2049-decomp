# func_800E681C (per-player control loop) - not assigned, worked because it is the only caller of func_800E6460

`best.c` is the group source. 16/179 words, all one register permutation.

Residual lane: coloured pool. After `jal func_800E6460` three loop invariants are rematerialised:
retail `a2 = 2`, `a3 = &D_8014A110`, `t0 = &D_8013FED0`; ours `a2 = &D_8013FED0`, `a3 = 2`, `t0 = &D_8014A110`.
Everything else (frame, s0/s1, t1-t5, all code) is identical.

Tried without movement (about 45 variants): `2 != D_8014A110` (gives FED0/A110/2), unsigned `2U` or `u32 D_8014A110`
(stops sharing the constant, 169 words), extern declaration order, an earlier function referencing D_8014A110,
dead reads of D_8014A110 (not code-free here), nested-if and De Morgan forms of the two button tests, `!D_8013FECB`,
`continue` forms, for-header forms, speed-chain order, `>> 2` spellings, `s16 speed`, signedness of map/D_8013FED0/D_801403C0,
a third argument to func_800E627C.

Best next hypothesis: the three webs tie on priority and are ordered by web creation; retail creates the constant-2 web
first. Something in the retail source mentions the constant 2 (same signed-int web as `st->gear = 2`) before the
reverse-button test, or the gear chain is not a goto-shared block. The `goto pick` in best.c is our construction: retail
reaches the speed table from two places, and a different way of writing that join is the most likely place for the
ordering difference.
