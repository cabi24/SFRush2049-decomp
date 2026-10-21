# f3 RESULTS (fast 20-minute dig, 2026-10-21)

| function | identity | bytes | state | best file |
|---|---|---|---|---|
| func_803A44FC | animText@carscr.c (w36b) | 3668 | NONMATCH 2/917, unchanged (as1 xbb moves `sll t7,s3,2` into the header's lw stall; retail has it in the bne delay slot) | w36b/func_803A44FC/best.c (unchanged); f3 tries in A44FC/v1-v11.c |
| func_8039842C | updatePlayerScreen@playerscr.c (w36c) | 1804 | NONMATCH 2/451, unchanged (reset-loop pointer spill at sp+120, retail 124). Same score with less shaping: f3/9842C/t2_7 | installed group (unchanged); f3/9842C/t2_7/group.c |

Nothing to deliver. No MATCH, so no group revision and nothing in cloud/matches/.
Permission note: a cleanup that would have pruned the variant dirs under f3/9842C/ was denied by the rm safety
check. It was not retried, so all variant dirs are still there. Only t2_7 (and t1_6/t1_8 as evidence) are worth
keeping. The rest can be deleted.

## func_803A44FC (standalone `score.py fn ... --targets asm/us/ovl_a`)
- v0 = w36b best.c: `2/917 words differ` (+0x094/+0x098 bne/sll swap), reproduced.
- The w36w2 noalias lever is already active here. The loop already indexes the global directly (`D_803B9B90[i]`).
  ugen turns that index into a pointer in $2 and opens `.noalias $2,$sp` at the top of the loop header. The region
  closes after the first `jal render_helper` inside the inlined animButtonText, not after a block-ending `b`. So it
  never gives the first inlined block (bb6) a predecessor with no incoming edges.
- These stay at 2/917: v1 `*(D_803B9B90+i)`, v2 explicit guard plus do-while, v5/v9 `continue` placement, v6 while
  loop, v7/v8 `do {} while (0);` after/before the if, v10/v11 an unused `D_803B9B90[p];` read at the start of
  animButtonText. These are worse: v3 switch (19), v4 inverted if (532).
- as1 detail read this session (as1.c func_428758): the "+1" weight rule means the predecessor P has a successor
  outside the current region bit-set. The region list (global 0x100235d0) is built in func_428a2c/func_428fc4
  (not decoded; regions are likely loops). If regions are loops, the loop header is a region entry (w=1) and
  bb6 = 4. Then neither the preheader nor the guard can change the decision; only bb6's own flags/dominance can.

## func_8039842C (group `score.py group`, installed dot_runtime_a_availability_refresh_20261007 as base)
- Base: all 9 members MATCH. Context 9842C `2/451` (+0x5ec/+0x5f8 `sw/lw v0,124(sp)` vs ours 120).
- t2_7: same 2/451, all members MATCH, with less shaping. It sets `q = playerIdx;` once at the top of the main loop
  instead of in every case. A simpler base for whoever comes next.
- Frame findings: retail homes are D_803BACE0 ptr 116, reset ptr 124, nothing at 120. Ours always puts the two
  in adjacent temps (temp0 reset, temp1 D_803BACE0). With 8 unused locals everything moves down 4 (t2_8: 10 words). With 6
  unused, frame is 184 and D_803BACE0 = 116 matches, but reset is 120 and the playerIdx home is 180 (t1_6: 14 words).
  So retail has one temp (120) between them. Most likely it holds the first-loop pointer web (web 16, no visible
  spill code), which must also conflict with web 53 (the D_803BACE0 pointer) so that 53 drops to 116.
- Worse (all members still MATCH): first loop on playerIdx/p (412-421), shared function-scope `Owner *player`
  (142-161), MAT3 block-local `i` in purge/allReady loops with 5-8 unused locals (46-204), function-scope
  reset/case-2 counter k (14/22 with 5/6 unused; 2 with 7), `do {} while (0);` in the reset loop body (13). These
  stay at 2: comma expression, moving u6 into a block, `do {} while (0);` at four other boundaries.
