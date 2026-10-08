# w14p results (matching wave 14, lane w14p: default-block placement and return merge)

Builder scratch: `~/rush2049/scratch/frontier/w14p` (copied from base; src/blob, include, tools/cloud, asm/us/blob and
blob_matched.lock.json synced from the Pi). Flags for all runs: `-g0 -O3 -mips2 -G 0 -non_shared`.
No function reached MATCH. No `cloud/matches/` files, no groups, no overrides, nothing to splice.
Scores are standalone bscore (strict = differing words); no `score.py fn` or whole-program unit line was run, so no
MATCH/EQUAL claim is made.

## Results

| function | bytes | state | best draft | standalone line (verbatim, vbatch/bscore) | residual |
|---|---|---|---|---|---|
| func_8010D3C0 | 704 | 61/176 strict words differ, 8 own-rodata relocs unverified (not a match) | `func_8010D3C0/best.c` (= `start.c`, default first) | `strict  61  mnem-missing   4  aligned-missing  23  size +0  v0000.c unverified=8` | default-block constant hoist (see below) |
| func_800A1BB4 | 184 | 28/46 strict words differ (not a match) | `func_800A1BB4/best.c` (= `b1/v0000_e1_p1_l1_f1_t0.c`) | `strict  28  mnem-missing   6  aligned-missing  23  size +0  v0000.c` | early-exit return merge (see below) |

Diagnose (`python3 -m tools.conveyor.pipeline.diagnose one NAME --source BEST.c`):
- func_8010D3C0: `verdict: mixed(constant:32, structural:14)`, `lever: none-known`. Note diagnose reports
  `strict_words_differing: 80` / `words_differing: 56` for the same file that bscore scores 61 (different
  measurement; bscore is the number used here).
- func_800A1BB4: `lever: none-known`, `strict_words_differing: 28`, 46 words.

Workbench guide: `guide laws ido53` has no entry on early-return duplication or tail merging. The field-guide
levers that were checked (1, 7, 8, 10, 14, 25, 33, 36) concern constants and as1 scheduling; none names a
return-duplication lever. The lever for the constant hoist below is the constant-web sharing rule only as
observed here (not in the guide).

## Round log

### func_8010D3C0 (start 61 = w14j best; standalone bscore)
| round | template / generator | variants | best strict | best rows |
|---|---|---|---|---|
| 0 | control (start.c) reproduced | 1 | 61 | `v0000.c` 61 (control reproduces) |
| 1 | `gen1.py`: placement {start,end,goto-after-switch} x default load {direct, ptr, volatile, addr-cast} x default kind {1, 1U} | 24 | 61 | start 61 (all 6 non-volatile start rows); end/goto 72; volatile 131-132 |
| 2 | `gen2.py`: round 1 x default kind {1,1U,(s8)1,(u8)1,(s16)1} x case-351 kind (same five) | 75 | 61 | start 61 (25 rows); end/goto 72 (50 rows); constant spelling does not move the score |
| 3 | `gen3.py`: default statement order x `if(amount){}` copies (0/1/2) before or after the compare x placement | 36 | 61 | start 61 (7 rows); end/goto 72 or 78 |
| 4 | `gen4.py`: break the `kind=1` web (`kind=0; kind+=1;`, `kind=2-1;`, `kind=kind`, `kind++`, `if(amount){}`) in default and case 351 | 54 | 61 | start 61 (15 rows); 71/72/82 otherwise |
| 5 | `gen5.py`: default outside the switch behind `if((u32)(effect->effect-350)>10u)` | 3 | 144 | worse (explicit range test adds structure) |
| w | `wbgen.py start.c g1` (control + 347 one-edit neighbours) | 348 | 61 | 61 at 10 rows; no strict improvement |

Total about 540 scored variants. Three consecutive rounds (2, 3, 4) gave no improvement over 61. Stopped.

Retail vs ours (`sbs.py`, instruction diff against the retail words of `asm/us/blob/blob_80100d24.s`):
- Retail's default block is `lui v1,D_80121D64@hi; li a0,1; lw v1,D_80121D64@lo` placed after the case-360 block and
  falls into the join. Each case block carries its own `li a0,1` (case 351) before its `b`.
- Ours hoists `li a0,1` and `lui v1,D_80121D64@hi` into the switch head, before the `sltiu`/`beqz at` dispatch
  (2 extra instructions, so every later branch target is shifted by 8 bytes). The `lw` stays in the blocks. That is the
  whole 61-word difference apart from the 8 unverified rodata relocations.
- Placing the default last or after the switch (`goto` form) keeps the hoist and moves the block to the retail spot;
  that gives 72, which is worse because the block layout then has more rows out of place.

Residual hypothesis: the `li a0,1` and the D_80121D64 hi-half are a constant/address web shared by the default and
case 351, and IDO places that web at the switch dispatch block. Spelling the constant or the load differently does not
break the web (rounds 2 and 4). Not tried: `trace/force.sh` on the constant web, and `if (x);` splits on the kind
store (lever 10), which needs a trace to know the web number first.

### func_800A1BB4 (start 28 = w14g b3 best)
| round | template / generator | variants | best strict | notes |
|---|---|---|---|---|
| 1 | `gen1.py` (`b1`): early exit {if-return x3 spellings} x p-init {with/without early p==0 return} x loop {while(1)+break, while(p)} x final clear {4 forms} x trailing shaper {0,1,2} | 144 | 28 | 24 rows at 28; the p-init early return is the 28 form (`p1`), `p2` (no early return) gives 32 |
| 2 | `gen2.py` (`b2`): body wrapped in `if (active != 0)` with no early return, three loop bodies | 9 | 32 | size -2 (the inline retail return is still merged) |
| w | `wbgen.py best.c g1` (control + 8 one-edit neighbours) | 9 | 28 | control reproduces 28; no neighbour below 28 |

Total about 160 scored variants. Two rounds plus the wbgen climb gave no improvement below 28. Stopped.

Retail vs ours (`func_800A1BB4/o0.dis` vs `retail_a.raw`):
- Retail: `bnez t7,0x3c` at 0x2c falls into an inline `addu v1; jr ra; nop` (the `active==0` return with its own
  `jr`), and the final exit is a separate `jr ra; nop` at 0xb0 after `sb zero`.
- Ours: `beqz t7,0xb0` at 0x2c branches to the single shared `jr ra` at 0xb0, so IDO merges the early exit into the final
  tail. That is the 4-word (2 return pairs) gap (ours 42 words, retail 46 in the best form).
- Tried and not moved: return written as `return;`, `{return;}`, `!active`; final clear as `if(p==0)`, `if(p==0){...}`,
  `if(p!=0) return;`, the `return` inside the clear, and trailing `if(index){}` / `if(data){}`. Every variant with the
  early exit ends in the same shared `jr`.
- Retail's loop also loads `data->next` before the `enabled` test into `a1`, and ours loads it into `a1` later
  (`lw a1,0(v1)` at 0x54 vs `lw a1,0(a2)` at 0x54); the loop-variable order is a separate residual, not tried.

Next hypothesis: the early return must be a different block from the final return (retail has two `jr ra` blocks). A
trace of ugen's block merge (`trace/ugt.sh` for the return blocks) is the next step. Not run.

## Integration
Nothing to integrate. No match, no group, no unit override, no supersession. Do not promote either `best.c`.

## Permission denials
None.

## What generalises
- Retail duplicating a return is a source-shape question only if the early-exit block and the final block differ. In this
  function, every spelling that keeps the early exit as `return;` merges into the final `jr ra`.
- A constant or address web shared by a switch default and a case is placed at the dispatch block, which shifts every
  later branch in the function by the hoisted instructions. Changing the spelling of the constant does not split the web.
