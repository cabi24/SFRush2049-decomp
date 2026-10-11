# Wave 14, lane w14ob: wheel_torque_apply (MATCH), func_800DA0BC (provisional), func_800E7A98

Written by the coordinator on 2026-10-10 from the lane's files; the lane's session ended before it reported.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| wheel_torque_apply | 140 | **MATCH** standalone; spliced 2026-10-10 | -g0 -O3 -mips2 -G 0 -non_shared | `score.py fn` (builder, 2026-10-10): `wheel_torque_apply: MATCH`; `splice_singles.py`: image_ok |
| func_800DA0BC | 184 | PROVISIONAL: EQUAL in the unit only with a stand-in caller (real caller func_800DB1E0 unlocked); added to provisional.json | -O3 | coordinator re-run: `EQUAL func_800DA0BC: 46 words (internal, c_best.c)` |
| func_800E7A98 | 172 | not matched; variants a1-a8, b1-b5 (unit-scored lane, no saved line) | -O3 | w14q baseline 13 words / 1 op row stands |

wheel_torque_apply (`cloud/matches/wheel_torque_apply.c` = `wheel_torque_apply/best.c`): progression b1 16 -> b8 17
-> b9 4 -> best 0. Key: the name lookup is a static helper that umerge inlines (no stub); its key local sits in the
inlined-call area (frame 64); the store goes through an out parameter so uopt keeps separate addresses (retail's
`lui at`/`lui a0` pair). No shaping constructs. func_800DA0BC's shaping: `slot = &D_8011650C; do {` on one line.
