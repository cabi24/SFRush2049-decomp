# net_state_validate (0x800F2A28) - bigfish scout

## Facts
- 682 words (asm/us/blob/blob_800ef5b0.s), frame 64, saves ra + s0-s8. Not spliced, no cloud/matches entry.
- ABI: no argument or non-ABI register is read at entry; `void f(void)`. Only caller is `track_texture_load` (bare `jal`). Single callee `func_800B78A4(x, 16)` (4 calls, ABI a0/a1, returns v0). It sits in near-miss with 18/19 words wrong, so a clean seed could be missing, but it does not need IPA here.
- Globals: 18 distinct symbols, all plain data: `active_player_count` (s16), `input_rec0` (76-byte records, ptr chain `rec->x->y->z->S` at +72, +0, +44, +0), `D_801164C0/C2/C4` (s16 mode flags == 1), `D_80156994` (s8), `D_80150F7C[]` (s8), and 7 byte-flag tables: `D_80150E30` (13/player), `D_80150DD8` (19), `D_80150E88` (4), `D_80150EB8` (8), `D_80150ED8` (9), `D_80150F00` (5), `D_80150F40` (6). Symbol sizes are unknown to symbols.json; row strides were derived from the address math.
- Despite the name it is not a network validator: it is a per-player "unlock / achievement flag evaluator". Four sections. Each is `if (mode == 1) set every flag; else compute from the stats blob S` (thresholds: sums of 6/4 `func_800B78A4` results >= 48/24/36/32/16/24, int sums `/10` against 100..2000, sums against 100000..1000000, u16 nonzero tests at S+1674..1734).
- Shape: highly regular (nested short `for (s16 j...)` fill loops, chains of `(x >= K)` stored as `slti; xori 1; sb`, `multu i,K` for 13/19/9/5/6 strides). No fp. 14 mul/div, 83 branches. No library code; no arcade counterpart identified (achievement/unlock tables are N64-original).
- A quirk to reproduce: in the mode-C2==1 branch of the 4-byte table the tail `if (!D_80156994) D_80150E88[i][2] = 0` runs with `i` left at `active_player_count` (original bug, indexes one past the last player).

## First pass (hand written whole function)
`cloud/work/bigfish/net_state_validate/base.c`, flags `-g0 -O2 -mips2 -G 0 -non_shared` (the standard game flags; no -r4300_mul needed although the target has `multu` without `mul;nop;mul` pairs). Compiles under IDO without errors.
- 688 emitted words vs 682 target (+6). `score.py fn`: 675/682 words differ (raw), i.e. essentially unaligned.
- Opcode-only LCS 93% (632/682) by my aligner (`scratchpad/scout1/close.py`). The structure is right; what differs is register allocation and a few block orders.
- Known differences: (1) frame 112 vs 64 and prologue saves s5..s7 differently; target holds the constant 1 in s3, `active_player_count` in s1 (reloaded after each byte store because the store may alias it), `&input_rec0[i]` in s2, i in s8, sums in s4/s5/s6/s7. Mine spills the record pointer (`sw a3,68(sp)`). (2) Target re-evaluates the pointer chain inside the two sum loops; done in the current base.c (+1%). (3) the `/10` average loop and the 4-branch tail sit at different offsets, giving ~175 extra words near want[258]/got[275..451] (check for unrolling of the 12-term sum: target is a 3-trip loop of 4 loads stride 384, so write it as `for (m=0;m<3;m++) v0 += S32(228+384m)+S32(324+384m)+S32(420+384m)+S32(516+384m)` and similarly the 2-trip u16 loop of 4 loads stride 48).

## Feasibility: MEDIUM-HIGH (best of the five)
Nothing here is IPA-dependent, the layout is fully understood, and every section is simple. Risk is only uopt register pressure (9 callee-saved regs live), which is what decomp-permuter is for.

## Approach / effort
1. Rewrite the two sum loops as the unrolled-by-4 bodies above (0.5 h), rescore.
2. Split into static helper blocks? No: target is one function; keep one body but try `register`-free permutations of local declaration order and of the (i, j, k) types (s16 vs int) (1-2 h).
3. Run decomp-permuter on the function with the final skeleton (needs `-r4300_mul` not required); expected several permuter hours.
4. Check `D_80150E88[i][2]` quirk and `func_800B78A4` prototype (`s32 f(s32,s32)`) once that callee is spliced.
Estimate: 4-8 hours agent time to a strict MATCH, probability ~50%. Payoff: 682 words = 1 function, the same as any other, but 0.11% of game words.
