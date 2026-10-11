# w14zb results

Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`. Standalone scoring via `vbatch.sh` on builder scratch
`~/rush2049/scratch/frontier/w14zb`. No unit (blob_unit) runs. No match, no group, nothing to splice.

| function | bytes | state | flags | exact scorer line |
|---|---|---|---|---|
| particle_lifetime_set | 732 | 59 of 183 words off (plateau, 3 rounds, ~1,000 variants) | -O3 | `strict 59 mnem-missing 0 aligned-missing 58 size +0 v0000.c` |
| func_800A1BB4 | 184 | 28 of 46 words off (plateau, 2 rounds, ~80 variants) | -O3 | `strict 28 mnem-missing 6 aligned-missing 23 size +0 v0000.c` |

## particle_lifetime_set

Start: `particle_lifetime_set/best.c` (copy of w14r best = w11d/w10b line, 59). Control reproduced 59 in every round.

- Round 1 (`b1`, `t1.c`, 10 choice points, 300 of 1024): index-operand forms, `verts` assignment grouping, `D_801145D4`
  operand order, `scale` load forms, `k`/`i` loop init. Best 59 (6 variants). Distribution: 59 (14), 62 (17), 66, 69,
  then 164-182 (the rest). The 59 floor is not moved by any option here.
- Round 2 (`b2`, `t2.c`, 14 choice points, 400 variants): adds `idx` local, `scale * rect` operand swap, the
  `flags &=` spelling, sound branch sense. Best 59 (11 variants), next 61.
- Round 3 (`b3`, `t3.c`, 16 choice points, 400 variants): adds the `D_801391E4` reload spellings, a block-local
  `fl` for D_801145D4, a `u32 zz` unused local. Best 59 (4 variants), next 61.
- Stopped under the 3-rounds-no-improvement rule. Diagnose was not rerun this lane; the prior verdict (w14r) is
  register-class (`mixed(constant:2, structural:3, register:57)`), temp ring and as1 macro temp (see w11d).
  Options that changed the temp order only moved the count upward; none crossed 59.

## func_800A1BB4 (rerun per coordinator: 6 rounds, 315 scored variants)

Still 28 of 46 words off (`strict 28 mnem-missing 6 aligned-missing 23 size +0`). Not a match. Start: `best.c` (w14p best).

Diagnose (`python3 -m tools.conveyor.pipeline.diagnose one func_800A1BB4 --source best.c --flags "-g0 -O3 -mips2 -G 0 -non_shared"`):
verdict `mixed(structural:10, register:16)`, lever `none-known`, lanes pool slot 6, temp slot 8, shared slot 1,
`strict_words_differing 28`.

Return-block trace (snapshot `b0` of best.c, `ugt.sh`, and the `b3/v0000` snapshot for comparison):
- ugen emits the early `active==0` test as `beq $15,0,$36`, and `$36` is the single shared `j $31` at function end.
  Retail has an inline `jr ra` on that path (`bnez t7` over `addu v1; jr ra`), so ugen does not replicate the
  return. Only that early exit differs; the rest of the listing was compared to retail by hand:
  the `p==0` store-and-return is inline in both, the loop bottom test (`beqz t9`, `bnez a1`) and the break-to-test
  are the same shape as retail once the loop is written as `if (p != 0) do {...} while (p != 0);` (b3 listing).
- Retail computes `&D_80144D68[index]` in the bnez delay slot before the test (`lui v1; addu v1,v1,t8`); ours does not
  (b5 tried the pointer-first forms; no change).

Rounds (all standalone `vbatch.sh`, `--flags "-g0 -O3 -mips2 -G 0 -non_shared"`):
| round | template | variants | best strict | notes |
|---|---|---|---|---|
| b1 (earlier) | `t1.c` goto/return forms | 16 | 28 | goto exits merge into the same exit |
| b2 (earlier) | `t2.c` Rec pointer, duplicated early return | 64 | 28 | |
| b3 | `t3.c` do/while guarded loop, wrap `if (active != 0)` vs early return, enabled-test forms, `0 == p` | 64 | 32 | wrap and early return both 32 with the do/while loop; 44 compile failures from mismatched open/close pairs |
| b4 | `t4.c` `{return;}` forms, explicit trailing `return;`, p==0 block | 32 | 28 | trailing return does not split the exit |
| b5 | `t5.c` `ModelList *l` before the test, `!active`, `l->head` | 64 | 28 | |
| b6 | `t6.c` body-as-fall-through (`if (active != 0) {`), closing brace choice | 64 | 28 | |
| g1 | `wbgen.py best.c g1` (9 one-edit neighbours, absolute path) | 11 | 28 (control) | no neighbour below 28; best neighbour 33 |

Three consecutive rounds (b4, b5, b6) with no strict gain, 315 variants total. Stopped.
Not tried (and why): a value-returning function (`return 0`), because retail is void and a value return adds
`li v0`; the do/while(0) wrapper (equivalent to b3's do/while form for this shape).
Next hypothesis (not run): the early exit needs a block that IDO does not merge, i.e. the early `return` must be
the fall-through of a branch whose target is the rest of the body. b6 tried that shape; the distinction is in
ugen's block layout, and `trace/ugt.sh` on b6's best would be the next step.

## Integration
Nothing to integrate. Do not promote either best.c.

## Permission denials
None.
