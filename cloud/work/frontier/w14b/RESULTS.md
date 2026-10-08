# w14b results

No functions matched. No cloud/matches or group files were written; nothing was committed or spliced.
Builder scratch: `~/rush2049/scratch/frontier/w14b` (copied from base, src/blob, include, tools/cloud, asm/us/blob and the lock file synced).

## drone_target_update (0x800D7E88, 496 B, 124 words) — no match

Scorer line, whole-program unit (`TAG=w14b`), best draft `drone_target_update/best.c` (do-while, `end` computed after the selector block):

    FAIL drone_target_update: 110 of 124 words differ; compiled body is 120 words, target 124 | words 97 ops 16 norm 16 | frame 64/64

About 30 variants were scored (`drone_target_update/cand*.c`, `v_*.c`):
- for-loop form (the near-miss native source), do-while with bound at the top or bottom, index loop (`cand3`, 91/124, worse), unsigned mode, selector ternary, if/else, and ordering of the `end` assignment. The best family reaches 16 mnemonic-level differing rows.
- Record-copy variants (separate `rec`/`bp` pointers, `k1`-`k3`) are worse (44+ rows).
- Re-indexing the count (`&input_rec0[count]`, `(s16)` cast) and address micro-changes made no difference.

Residual (from `us.sh` `DIFF=1 UD=--all --ops`): the draft keeps `&active_player_count` in a callee-saved register (`addiu s6,s6,-24312`, `lh t0,0(s7)`), where retail rematerialises `lui`/`lh` at each read. Retail also has `move s4,s5; move s6,s5` record copies and a 2-element peel before the unrolled 8-element bindings loop; ours is 4 words short (120 vs 124). Source-level tweaks have not moved this. Next hypothesis: force the colour of the address web (force.sh on the `&active_player_count` web) before any more variants.

## func_800AC9BC (0x800AC9BC, 224 B, 56 words) — no movement

`func_800AC9BC/best.c` is the w9d draft unchanged. Scorer (whole-program unit, `a1.c` = w9d draft in the unit):

    FAIL func_800AC9BC: 32 of 56 words differ | words 36 ops 18 norm 18 | frame 0/0

Variants `a1`-`a10` were generated (`func_800AC9BC/`). Scored so far (`batch2.txt`):

    a10: 32 of 56 words differ (ops 18)
    a2 (s32 x,y params): 50 of 56 words differ (ops 24)
    a3 (s32 q, mx, my): 49 of 56 words differ, 54 words (ops 20)
    a4 (q, my, mx declared in that order): 32 of 56 words differ (ops 18), no change from a10
    a5 (q = (x >= mx) + 2*(y < my)): 50 of 56 words differ, 53 words (ops 24)
    a6 (bit test as ((leafMask >> q) & 1) == 0): 40 of 56 words differ (ops 26)
    a7 (mx computed before my): 52 of 56 words differ, 55 words (ops 17)
    a8 (one-line if forms for q): 32 of 56 words differ (ops 18), same as a10
    a9 (D_80124EEC + child[q] pointer form): 32 of 56 words differ (ops 18), same as a10

The 32/56 level is not beaten. All of a1-a10 are now scored (a7 has the fewest mnemonic differences, 17, but 52 word differences). Each run took about 10 min on the loaded Pi.

Retail `frontier show` lists no callees and no blockers; the group `src/blob/groups/frontier_entity_update` only inlines func_800AC9BC as context (`context` in group.json), so the standalone score is the one that matters.

## Permission denials
None.
