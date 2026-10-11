# w16c results (wave 16)

Assignment: camera_play_script (0x800C5644, 3,520 B, group recipe). Start: w15h best.c (48 norm rows, frame 600/608).

## Summary

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| camera_play_script | 3520 | not matched; **48 -> 6 register-normalised rows**, 880/880 words, frame 600 vs 608; -1 now t5 and old now s0 without forcing | `-g0 -O3 -mips2 -G 0 -non_shared` (whole-program unit) | `python3 -m tools.conveyor.pipeline.blob_unit --tag w16c --jobs 2 score camera_play_script --with cloud/work/frontier/w16c/camera_play_script/best.c --neighbours` -> `FAIL camera_play_script: 832 of 880 words differ; +0x208: .rodata+0x484: retail words encode 0x01284282, outside the image` / `locked bodies that differ in this unit: 0`; `tools/trace/us.sh` -> `words 625 ops 6 norm 6 \| frame 608/600` |

No match, so nothing to integrate (no cloud/matches file, no group, no overrides). The strict word count is
dominated by the 8-byte frame shift (every sp offset) and colour-only register names; compare drafts with
`us.sh` norm rows. `best.c` = `prefix.c` (w15h's, unchanged) + `body.c` (= `body_r3.c`).

## What moved it (three source levers, each traced first)

1. **Two spellings of float zero** (48 -> 33). `if (t < 0.f)` (line 148) and `if (!(-px * hz > 0.f))` (line 94); every
   other zero stays `0.0f`. ucode real constants are strings, so `0.f` is a different uopt constant (w15e's 0.5f/.5f
   lesson). With one spelling the 0.0 web kept two coloured pieces ($f2, $f22; tot 10 vs cost 0); retail
   rematerialises zero at every use (`mtc1 zero`). This is a literal-spelling choice, disclosed here as a shaping
   device; it does not add code.
2. **`conv = 1;` after the conversion loop** (33 -> 13): `if (conv == 0) { for (k = n - 1; k > 0; k--) {...} conv = 1; }`.
   Retail still emits the store before the loop guard. The edit adds live blocks to the -1 constant's web, which
   now colours t5 as in retail (see the -1 trace below).
3. **`old` assigned inside the non-null branch** (13 -> 6): `if (col->poly != NULL) { old = col->poly; if (col->k < best) ...`.
   The load becomes an expression web in v0 (retail `lw v0,0(s2); beqz v0`) and old's definition moves to a weight-1
   block. Its save drops from 10.5 to 1.0, so it is decided after n has taken s0. A callee-saved register that is
   already used costs 0 plus copy charges (1.1, against 2.1 for a3), so old gets s0: retail's `move s0,v0` and
   `sw s0,188(sp)` around func_800C36A0. The tail, the bc1f/bc1fl shape and the poly reload all follow.

## The -1 tie, traced exactly (why lever 2 works)

I built a private instrumented uopt in my builder scratch (`~/rush2049/scratch/frontier/w16c/bin/uopt_cup`). The patch
scripts are in `camera_play_script/uopt_patches/` and apply to the wtk `uopt.trace.c`. It logs `f_compute_save` per
live unit (`[SAVE]`, env `CUPWEB=<web>`), every `f_cupcosts` charge with the callee's IPA register info (`[CUP]`),
the web's block sets, and the block weights (`CUPDUMP=1`). Stock output is unaffected (it only adds stderr prints).

- net = gross = 3 compares x weight 10 (bb51/61/67). The loop-head unit's entry-load charge is exempt (canmoverlod).
  The effect_cleanup argument use is a remat occurrence and does not count.
- cost(t5) = 3 post-call blocks x 10: after func_800A61B0, after effect_cleanup's func_803914B4 and after the type-7
  func_800C54F0. All three callees have `flag12=0` (no IPA register info), so the charge is unconditional, matching
  retail's `li t5,-1` after each of these calls.
- The p1 test is `float(net/nocs) * nocs <= cost` (uopt.c 158777). With nocs = 18 the product is exactly 30.0, so the
  web is split. With nocs 19, 23, 25 or 27 it is 30.000002, so the web is coloured. nocs = (units + live-through
  blocks - 2)/4 + 2. Lever 2 took it from (4+65-2)/4+2 = 18 to 19. Every save stays below the 952 and
  &player_array webs, so the colour order is unchanged.
- **Generalises:** an exact tie `tot == best` in `ctrace` can be broken by single-precision rounding in
  `save*nocs`. Changing the live-block count by 1-4 is enough, with no new uses. Check the nocs table before hunting
  for a fourth use.

## Remaining residual (6 norm rows + frame), with causes

1. **Corner-loop IV** (+0x14c, retail `move a1,t0`). Colour-only. `force.sh t3 camera_play_script "p1:w81=c7"`
   gives 5 rows (only the k-loop and frame remain). Ours: the IV web w81 has cost a1 20.0 and t0 21.0 (the copy
   relation to the argument saves 1), and forbidden is v0/a0/a2/a3. Retail must forbid a1 as well, or price it
   higher, so some web coloured a1 overlaps the corner loop in retail. Not found.
2. **k-loop clip** (+0x914..0x93c). Retail computes `ex*dz - ez*dx` before the branch into **$f22** (a coloured
   expression web) and moves `px` ($f26 -> $f2) between compare and branch. `u = ex*dz - ez*dx; px = pv[k][0];
   if (u != 0.0f) u = (...)/u;` (`v/k3.c`) reproduces the k-loop exactly. But u's web grows to tot 550. The k-loop
   expression webs then take $f20/$f22, and t loses $f30 (t is split before p1 decides it; 32 rows). Reusing `ez` as
   the denominator gives no fold (`c.eq.s` against 0, 47 rows).
3. **Frame 600 vs 608.** The cfe locals area (448 + 4 from unrolling) and the named-local offsets relative to the
   frame top match retail. The first temp home (-456) matches too. Retail has **two more coloured expression-web
   homes** (area 492/496 vs 484; `spill.sh`). The two best candidates are both visible: retail's `lhu a2,568(sp)`
   (idx[0] load kept in a2 as a coloured web, a ring temp in ours) and the k-loop denominator in $f22 (item 2).
   Fixing item 2 the retail way probably gives one of the two slots.

**Best next hypothesis:** get item 2's retail shape without merging the k-loop u into the clip-wide u web. A
dedicated expression web (CSE of the difference used twice), with no named variable, would also give the missing
temp home. Then re-check item 1, because the a1/t0 choice depends on which FP/int webs exist in the preheader.
Use `force.sh "p1:w81=c7"` as the oracle.

## Files
`camera_play_script/best.c` (current best, full unit file), `body.c` (= `body_r3.c`), `body_r1.c` (zero spelling),
`body_r2.c` (+ conv placement), `prefix.c` (w15h), `q.sh` (unit score + -1 web line), `fq.sh` (q.sh + -1 forced to
t5), `uopt_patches/` (instrumented-uopt patches), `v/` (variants, traces).

Notes: `python3 -m tools.conveyor.pipeline.diagnose one` compiles standalone (858 words, frame +40), so it is not
meaningful for this unit-only function. `ctrace`, `force`, `spill` and the instrumented uopt were used instead.
Time: about 3 h of trace work (stop rule reached).

Permission denials: none.
