# w14w results (wave 14): ghost_race_setup, func_800CC50C, entity_anim_texture

No function reached a strict MATCH. Nothing is deliverable to cloud/matches or as a group with claims.
No splice, lock, or src/ change was made. Builder scratch: ~/rush2049/scratch/frontier/w14w (base copy,
src/blob, include, tools/cloud, asm/us/blob and blob_matched.lock.json synced from the Pi).

## Table

| Function | Bytes | State | Flags | Scorer line (this session) |
|---|---|---|---|---|
| ghost_race_setup | 440 | 91/110 words differ (group with menu_transition); prior A119 notes agree | -g0 -O3 -mips2 -G 0 -non_shared | `score.py group cand/ghost_grp` -> `ghost_race_setup: ... 91/110 words differ`; `menu_transition: 4/40 words differ` |
| func_800CC50C | 760 | no real draft; only the m2c context body in ipa-groups/codex_func_800C7200 (wrong signature, see below) | same | `score.py group cand/c7200` -> `func_800CC50C: 162/190 words differ` |
| entity_anim_texture | 636 | best value-preserving draft 131/159 differ; not matched | same | `vbatch` rows (below); best `strict 131 mnem-missing 27` |

## entity_anim_texture (the only function with a real search)

Retail facts recovered from `tdis.py`: record is in s1 (copied from a0 after `sw a1,84(sp)`), enabled is in s0,
player is spilled at 36(sp), side*24 at 32(sp), view at 72(sp), pose at 68(sp); frame is 80 bytes. Model68 has pose at
+8, Side24 flags at player+668, player flags at +232, pose at +896, stride 952. The draft's semantics match these.
The draft is structurally close (mnemonic-missing 24-27 of 159) but the prologue is wrong: the draft keeps record in
s0 and saves only s0/ra (frame 64-72).

Rounds (vbatch, `--flags "-g0 -O3 -mips2 -G 0 -non_shared"`; score.txt in each dir):
- r1 `eat/b1` (8 choice points, 300 of 4374): best `strict 156 mnem-missing 27 aligned-missing 102 size -7`, control also 156.
- r2 `eat/b2` (10 choice points incl. spare locals, 300 of 26244): best `strict 156 mnem-missing 27`. No improvement.
  Note: a spare local moves the frame (+8 per s32), but the strict count stayed 156 because the offset shift dominates.
- r3 `eat/b3` (12 choice points incl. view/enabled hoist, pose temp): best `strict 140 mnem-missing 27 size +0`
  (`b3/v0093.c`, choices 0 2 0 1 1 2 1 1 2 2 1 2).
- r4 `eat/b4` (frame-filler named locals on best_r3): best `strict 138` (`b4/v0001.c`, saved as `eat/best_r4.c`).
- climb `eat/g1` (wbgen on best_r4, 173 variants): best `strict 126` (v0063, v0064, v0065). REJECTED: the one-line
  diff is `enabled = pose;` / `if(pose==0) enabled=0;`, which clobbers the `enabled` value that model_bounds_calc
  reads. Other climb winners were also semantics-breaking hoists into live locals (v0023 overwrites `side`,
  v0100 passes an f32 `scale` as the s32 model_data_load argument). `wbgen` hoists are not checked for liveness;
  every climb winner must be reviewed before adoption (`experiment review-mutation` reports them UNREVIEWED).
- Best value-preserving file: `eat/best_valid.c` = g1/v0030 (adds `ff2 = D_80140418;` copy into a declared local).
  Score: `strict 131`, but the shaping device (unused `ff1`, copy local `ff2`) is not justified as the original, so it
  is a lead only and not a match.

Diagnose (`tools.conveyor.pipeline.diagnose one entity_anim_texture --source eat/best_r4.c --flags ...`):
verdict `mixed(constant:2, structural:71, register:63)`, frame_delta 8, lever `drop-a-declared-local`. Workbench
levers 26 (frame residual with exact registers: preserve the live-range topology, move homes with a split local)
and 40 (de-declare a value so it takes a compiler-temp home) are the next things to test. The register residual
(s1 record, s0 enabled) is the blocker; the frame residual is 8 bytes.

Stop state: 3 batch rounds done (r1-r4 with about 1,080 scored variants including the climb), best valid strict 131.
Stopped because the remaining residual is the register/frame topology of the prologue, which the vbatch templates
did not move. Next: lever 40 on `pose`/`view` (de-declare to compiler temps), and an explicit `ModelObj *self = record;`
alias to get s1 for the record.

## ghost_race_setup

Prior frozen A119 group (`cloud/work/ipa-groups/codex_ghost_a119/group.json`, members ghost_race_setup and
menu_transition, claims empty) reproduced exactly in this session: `ghost_race_setup 91/110`, `menu_transition 4/40`.
No new variants were run; the prior notes say the cursor/IPA residual is far from a match. Not deliverable.

## func_800CC50C

No draft exists beyond the m2c context body in `cloud/work/ipa-groups/codex_func_800C7200/group.c`
(`s32 func_800CC50C(s32 *arg0, s32 arg1)`). The C7200 notes say the second parameter is a signed-byte output pointer and
the body reads a float from D_80111754[index*4]. Scored as the real-caller group `codex_func_800C7200_2`:
`func_800CC50C: 162/190 words differ`. The function needs a new source written from the disassembly (not started).

## Integration notes
- No overrides, no superseded groups, no claims.
- Nothing belongs in cloud/matches.

## Permission denials
None.
