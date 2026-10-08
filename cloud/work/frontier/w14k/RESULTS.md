# w14k results (race_position_update, func_800B9740)

No function matched (no strict 0 in any round). No cloud/matches file, group or splice was produced.
Stop rule applied: both functions have at least 3 rounds and 300 scored variants, and the last 3 rounds
on each gave no strict improvement.

| function | bytes | state | best strict line (bscore, default -O3 flags) | residual |
|---|---|---|---|---|
| race_position_update | 484 | not matched, structure close | `mnem-missing 24 aligned-missing 56 strict 114 size -13` (b1 v0002, b3, b4) | width web forced to a2 does not give 0 (see force) |
| func_800B9740 | 408 | not matched | `mnem-missing 24 aligned-missing 65 strict 83 size +2` (b4 v0003); size +0 at strict 83 in b6 v0129 and b7 v0033 | retail saves only s0; candidates still keep 3 values live across the loop |

Lower strict is better. The race prior best (w4c, 116 / -9) and the func prior best (w3c, 91 / +5) are the
re-score baselines.

## race_position_update force test (`tools/trace/force.sh`, unit snapshot `r0`)

Unit baseline (us.sh): `FAIL: 107 of 121 words differ | frame 120/120`. The unit frame already equals retail's
120 bytes.
- `p1:w36=c5` (a2): got 112, differing rows 115.
- `p1:w21=c5` (a2): got 119, differing rows 73. This is the best single force. It is the width web (save 10.8).
- `p1:w12=c5`: declined (forbidden mask).
- `p1:w65=c5`: got 112, differing rows 68.
- Pair search `p1:w21=c5` plus `p1:wN=cM` over 9 webs times 8 colours (`force_pairs.txt`, 69 runs): best
  72 differing rows (`w12 c15`). No pair reaches 0.
- Conclusion: forcing the width web alone does not give 0; the residual is structural plus other webs.

## Batch log

### race_position_update
- b1: template t1 (5 choice points: decl order, spare locals, index forms, `n`, cast). 32 variants. Best
  strict 114 / aligned 56 / size -13. Improved strict from 116.
- b2: template t2 (7 choice points: spare locals and width/room temps, p index, tail stores). 400 variants,
  0 compile fails. Best strict 116 / aligned 68 / size -9 (the prior baseline). No improvement.
- b3: template t3 (10 choice points: split temps, `n` forms, re-read of `dst[len]`, `len--` form, `p` forms,
  spare locals). 400 variants. Best strict 114 / aligned 56 / size -13. No improvement.
- b4: template t4 (14 choice points: `wide` forms, extra `q` locals, re-reads of return values). 400 variants.
  Best strict 114 / aligned 56 / size -13. No improvement.
- Stop: b2, b3, b4 gave no strict improvement after b1.

### func_800B9740
- b1: template t1 (9 choice points). 300 variants. Best strict 91 / aligned 61 / size +5 (= prior). Every
  row identical: the choice points did not change codegen.
- b2: template t2 (11 choice points: inner-loop for/indexed/`ranges + j` forms, hoisted ranges local, axis
  unrolled or if-form). 300 variants. Best strict 87 / aligned 60 / size +1 (v0103). Improved strict.
- b3: template t3 (byte-offset inner loop `off += 16` with `(u8*)` cast, re-read of range_count). 224 of 384
  compile failures (declaration options); best strict 109 / aligned 103. Worse; the forms are a regression.
- b4: template t4 built from b2 v0103 (decl forms, break on hit, byte-offset `j += 16`, unrolled axis).
  168 valid of 288 (120 compile failures). Best strict 83 / aligned 65 / size +2 (v0003). Improved strict.
- b5: template t5 from b4 v0003 (decl forms, eligible forms, vertex forms, axis form). 72 valid of 144.
  Best strict 83. No improvement.
- b6: template t6 (10 choice points: outer loop with pointer walk, vertex forms, else-block forms, axis
  reversed). 207 valid of 400. Best strict 83 (v0129 size +0; v0000 size +2). No improvement.
  Note: an earlier b6 run produced strict 77 but used empty options that dropped `vertex = ...`; those rows
  were invalid and discarded. The rerun above uses non-empty options only.
- b7: template t7 (t6 plus an inner-loop range-step form). 201 valid of 400. Best strict 83 (v0033 size +0).
  No improvement.
- Stop: b5, b6, b7 gave no strict improvement after b4.

## Retail facts (func_800B9740, tdis.py)
- Min/max update: min = (min < v) ? min : v; max = (v < max) ? max : v (axis loop over 3 s16).
- Inner range loop: byte-offset counter (t0 += 16) bounded by range_count*16, plus a separate range pointer
  (t1 += 16). Only s0 is saved; vertex base (D_801409E8) and range_count are re-read inside the outer loop.
  The candidates still hold more values across the loop.

## Next hypotheses (not tested)
- race_position_update: the a2 width web is not reachable with source choice alone. Test whether a second
  call-crossing web (the s5 web, saved but unused in retail) changes the cost ranking. Use `force.sh` with the
  s5 web as a non-callee colour first.
- func_800B9740: retail keeps the loop state in one callee-saved reg. Try a version with `vertex` as the only
  outer-loop state and the range loop as a single pointer `end` compare.

## Permission denials
None.
