# Frontier batch, agent B (2026-10-04)

Scored on `watchman2` in `~/rush2049/scratch/frontier/agentB` only. Scorer prefix for every command below:
`cd ~/rush2049/scratch/frontier/agentB && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a…/ido`.
Helpers here: `sc.sh` (strict fn score), `scg.sh` (strict group score), `dis.sh` (candidate disassembly),
`batch.sh` / `batchg.sh` (aligned / group batch scoring). Only `MATCH` from `tools/cloud/score.py` is counted.

| function | result | flags | how scored |
|---|---|---|---|
| func_8008A148 | **MATCH** | `-g0 -O2 -mips2 -G 0 -non_shared` | `fn`, alone |
| entity_name_copy | **MATCH** | `-g0 -O2 -mips2 -G 0 -non_shared` | `fn`, alone (one unused local) |
| func_8008C768 | **MATCH** | `-g0 -O3 -mips2 -G 0 -non_shared` | `fn`, alone; 22 words off at `-O2` |
| Input_ApplyPadConfig | **MATCH** | `-g0 -O3 -mips2 -G 0 -non_shared` | `fn` and real 2-member `group`; needs `Input_InitPadHandlers` in the same file |
| func_80099B30 | 20/51 alone; MATCH only in a stand-in `-O3` group (lead) | | IPA-internal |
| model_data_load | 3/93 | `-O2` | allocation |
| func_80091B00 | 18/42 alone; 11/42 in a stand-in `-O3` group | | IPA-internal + as1 schedule |

## Strict matches

- **func_8008A148** -> `cloud/matches/func_8008A148.c`
  `python3 tools/cloud/score.py fn cand/func_8008A148.c func_8008A148` -> `func_8008A148:` / `  MATCH`
  Plain SDK GBI macros on the global display-list pointer `D_80149438`: `gDPLoadTLUT_pal256`,
  `gDPLoadTLUT_pal16`, `gDPSetPrimColor` + `gDPSetEnvColor`, cached on `D_8012E6D0`. No quirks. Matched first try.
- **entity_name_copy** -> `cloud/matches/entity_name_copy.c`
  `python3 tools/cloud/score.py fn cand/entity_name_copy.c entity_name_copy` -> `entity_name_copy:` / `  MATCH`
  It is a libc `bsearch(key, base, nmemb, size, compar)`. What closed it: no `half` variable (`n / 2` written three
  times, the CSE temp takes `s1`), `void *base` with `(char *) base + n * size` (gives `addu base,prod`), and
  `adjust = (n & 1) ? 0 : 1; n = n / 2 - adjust;`.
  **Quirk:** one unused local (`s32 unused;`) is needed for the 72-byte frame (6 words off without it).
- **func_8008C768** -> `cloud/matches/func_8008C768.c` (an `atan2f(y, x)` over `func_8008C680`/`func_8008C720`)
  `python3 tools/cloud/score.py fn cand/func_8008C768.c func_8008C768 --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `func_8008C768:` / `  MATCH`
  At the default `-O2` the same source scores `22/67 words differ` (`y` goes to `$f20` with a 32-byte frame; retail keeps it in `$f16`).
  Its callee `func_8008C680` is locked at `-O3` (group `sol_high_c680`), so `-O3` for this file is plausible.
  **Quirk:** literal types. Two zeros are int literals (`return 0;` and the last `y > 0`), the other four are `0.0f`;
  this splits the zero-constant webs as in retail (`$f0` x4, then `$f18`). Three literal mixes match; all-`0.0f` does not.
  The four constants are `extern const f32 D_801238F4/F8/FC/D_80123900` (retail rodata), not local literals.
- **Input_ApplyPadConfig** -> `cloud/matches/Input_ApplyPadConfig.c`
  `python3 tools/cloud/score.py fn cand/Input_ApplyPadConfig.c Input_ApplyPadConfig --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `Input_ApplyPadConfig:` / `  MATCH`
  (same file, `Input_InitPadHandlers` -> `MATCH`; at `-O2` the file gives `48/48 words differ`.)
  `python3 tools/cloud/score.py group groups/padcfg` (= `Input_ApplyPadConfig/group/`) -> both members `MATCH`.
  The first 20 words are `Input_InitPadHandlers(index, h8, w4, w0, hA, hC, h10, h12)` **inlined** (arguments loaded
  last-to-first into `ra,t5..t0`); that function has no `jal` callers in the image. So this is a real `-O3` unit of
  two real bodies, no stand-ins, no quirks. The frontier tool's "not an IPA member" is wrong here: it does not see inlined callees.

## Not matched

- **func_80099B30** — `func_80099B30/best.c`: `20/51 words differ` alone at `-O2`. Residual: temp ring only.
  Source is exactly `gDPLoadTLUT(gfx++, tex->last - tex->first + 1, 0x100 + tex->first, tex->palette)` on a local
  `gfx = *gfxp`, guarded by `D_8017A4B0 != tex->palette || D_80161430`. Pool registers all agree; retail's temps wrap
  `t9 -> t6`, ours use `t5..t9`.
  `func_80099B30/group_standin/` (same body + two stand-in callers `zz_a`/`zz_b`, `-O3`):
  `python3 tools/cloud/score.py group groups/f99B30` -> `func_80099B30:` / `  MATCH`. **Lead only, not spliceable.**
  Tried alone (~300 variants): hand-written stores, block-scoped `_g`, 40 dead-read placements, 187 dead-local variants,
  13 natural extra-local forms; none reserves `t5`.
  Next hypothesis: it is IPA-internal to its callers (`render_display_list`, `particle_system`); it will match when it is
  put, unexported, in the `-O3` group that holds both real callers.
- **model_data_load** — `model_data_load/best.c`: `3/93 words differ`. Residual: allocation, mode-3 loop only
  (`lh a3,22(s0); beq a3,s4; move a0,a3` vs retail `lh a0,22(s0); beql a0,s4; lh a0,24(s0)`).
  Recovered: third parameter is `u32` (fixes the three `or` operand orders), one `s32 t` shared by the flags read and the
  mode-2 child, read index `(s16) a`, write index `a`, mode 2 is a self tail call, entry size 0x44 (`flags@0, child@0x16, sibling@0x18`).
  Tried (~600 variants in 10 batches): child via `c`/`t`/direct, `Ent *e` forms, separate loop variable, tail-recursive
  sibling, types, declaration orders, switch vs if-chain.
  What is known: `a = e->child; if (a != -1) f(a, 3, v); a = e->sibling;` with a pointer local gives exactly retail's three
  words, but then the pointer becomes a split web (`v1` in mode 2, `s3` in mode 3) where retail has one `s0` in both.
  Next hypothesis: uopt's interference is per basic block, so the child must be assigned to `a` itself; find a form where
  the entry pointer is a single web across mode 2 and mode 3 (assigned once on a path common to both).
- **func_80091B00** — `func_80091B00/best.c`: `18/42 words differ` alone; `func_80091B00/group_standin/`: `11/42 words differ`.
  Two separate residuals. (1) Ring: retail's temps are a 4-wide `t6..t9` ring, reproduced only as an IPA-internal `-O3`
  member (23 real callers). (2) Schedule: retail keeps `sb t,3(v1); li t,-1; sh t,0(v1)` in each unrolled copy; ours has
  `li -1` before the `sb`. `cc -S` shows ugen already emits retail's order, so the swap is as1's reorganiser.
  Tried in group mode (~90 variants): 48 line layouts, one-line/macro bodies, pointer forms, basic-block splits
  (`if (1)`, `do/while(0)`, goto, inlined helpers for either store). Best stays 11.
  Next hypothesis: in retail the `li 1` reached as1 already above the `bnez` (so nothing in the block needed moving);
  look for a source form that evaluates the `1` before the test, and score it only inside a group with real callers.

## Generalisable notes

- A temp ring that never leaves `t6..t9` in a function whose pool does not reach `t5` means IPA-internal `-O3` member:
  check this before any variant search (two of seven here).
- Loads of several fields into `ra, t5, t4, ...` in descending order followed by all the stores = an `-O3` inlined call.
  Look for an already matched neighbour with no `jal` callers and put it in the same file.
- A GBI block that allocates `v0 = *ptr; *ptr = v0 + 8` per command is the SDK macro on a global pointer; write the macro, not the words.
- Ring start is set by the highest pool register uopt reserved, including webs that later vanish (the per-macro `_g` copies).
- `u32` vs `s32` on a shifted operand flips `or` operand order; `void *` + `(char *)` cast flips `addu` for pointer + product.
- uopt keeps a value out of an argument register if the parameter living there is live anywhere in the same basic block.
