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

## Hail-mary pass (stage log)
Tooling (scratchpad, not in repo): compile+relocate via score.py internals, aligned diff (difflib on register-normalised
disassembly), fine metric = aligned words weighted by register agreement.

Findings that fixed structure (682 words now reached, 267/682 words exact at first checkpoint):
- The "175 extra words in /10 section" note was stale; frame was already 64.
- `a=0;b=0;c=0;d=0;` (separate statements, in that order) must be at the top of the per-player iteration, BEFORE the
  `input_rec0[i].x->y->z != 0` test (target does `move s6/s4/s7/s5,zero` before the z test). `a = b = c = d = 0` gives
  the wrong register order (d,c,b,a).
- The constant 1 must be a real local (`s32 one = 1;`, compare and every store use `one`): only then is it a whole
  function web in s3 and `bne s3,t6` is used for the mode compares (with literal 1 the compare is `li at,1`).
- The a/b and c/d sum loops: `p = chain->S + k + 140; f(*(u16*)(p+92)&0xff); f(*(u16*)(p+92)&0xff00)` (and `+1292`/`+60`)
  reproduces target's `addiu s1,s1,140 ; lhu 92(s1)` rebasing exactly (a pointer temp defined in the loop body, S local
  is NOT used there; chain re-evaluated each iteration).
- Section-4 32-bit sum: `p = S + 1292; for (n=0;n<4;n++) v1 += *(s32*)(p+12+n*64);` gives the unrolled `addiu a1,v0,1292`
  form, but ONLY with a fresh loop variable `n` (re-using `k` prevents IDO's unroll).
- Section-2 12-term sum: write it with the chain expression directly (no `S` local); then the induction pointer is the
  `lw` result itself (no extra `move`), matching target incl. the `div` block.
- Section-2 store order (asm order) EB8[0],[1],[2], ED8[3], EB8[3] (=v>=200 NOT 250) ... see base.c.
Open: `active_player_count` (hoisted load, reloaded after calls) lands in a t-reg (t1/t2) in ours; target keeps it in s1
(sharing s1 with the p temp), which changes which constants in the section-2 loop spill to s-regs and cascades into
scheduling of the store block. Loop-form (for/while/do) and decl-order permutations do not change it.

### Stage 2 findings (register allocation)
- Instruction count now equals the target (682); structure aligned except for register naming (strict score: 289/682
  words differ from best permuter output; from 674 at start of this pass).
- The remaining mismatch is one cascade: the hoisted global `active_player_count` load (killed by the sum-loop calls
  and reloaded) lands in a caller-saved reg (t1/t2/a0) in ours but in s1 in the target (sharing s1 with the loop pointer
  temp `p`). Evidence: with all calls removed it becomes s0; any single call in the sec-1 loop makes it a t-reg. The
  constants hoisted in the section-2 loop (li 5/6/9/10/12, four `lui/addiu` bases) then take t0..t5/ra/a2 in the target
  and count is allocated after them; in ours count is allocated 2nd (right after const 6), which pushes constants 5 and
  F40 base into s0/s1 and changes hoisting of `li 76`/`&input_rec0` in the section-2 mode-1 arm and store scheduling.
- Named locals for the count (`cnt = active_player_count;` assigned once before section 2 and used by the section 2..4
  loop bounds) make uopt allocate cnt as a named web (s2) and the section-2 constants then get exactly the target's
  registers; but k/p swap (s1/s0) and sec-1 count becomes a0 (still not s1). base.c holds a permuter descendant of
  that variant.
- Things that do NOT change the count register: decl order of locals/globals, loop form (for/while/do, 6561 combos),
  types of count/one/cnt, `static`/defined/extern, struct/array wrapping of the global, `#pragma no side effects`,
  visible callee body at -O2/-O3, group -O3 build, -O1/-O3, K&R prototype.
- decomp-permuter (cloned to scratchpad, custom compile.sh that resolves relocations and re-assembles the function as flat
  .word text so its mnemonic/regalloc scorer works against target words) drives score 4355 -> 2360 quickly, then stalls.

### Stage 3 (end of this pass): state, levers, what is left
Saved: `net_state_validate/base.c` = best structural variant (permuter descendant of the `cnt` lever variant):
684 words emitted vs 682 target, **585 of 682 target words reproduced exactly in order (LCS)** (first pass: 171),
positional `score.py fn` is useless for it (661/682) because two extra words near the top (`move a0,s1`, `li a3,13`)
shift everything. `base_strict289.c` = an earlier variant with the best positional score (289/682 words differ, 684 emitted).
Not a MATCH; no cloud/matches entry.

What fixed things (all reproducible, see base.c):
1. Variable identity is part of the allocation: IDO allocates per VARIABLE (union of its webs), not per def-use web.
   Give each section's sum its own variable (`w` for the section-3b u16 sum, `v0` only for section 2b) and sec-2b gets
   sum=v0 / induction pointer=v1 exactly like the target; re-using `k` or `v0` across sections flips them.
2. Fresh loop variable for the 4-term sum (`n`) is required for IDO's unroll; sec-2b counter wants another variable so it is
   a temp (a0) not an s-reg.
3. The hoisted `active_player_count` load only leaves the t-regs (-> s1, like the target) if one section's loop bound
   goes through a different web: `cnt = active_player_count; for (i = 0; i < cnt; i++)` in ONE loop (section 1a, or 2b),
   or `active_player_count * 1` in section 1a. All such levers cost two extra words (a `move` copy and a `li a3,13`
   hoist into the section-1b preheader) that the target does not have; that is the remaining structural blocker.
4. Flag `D_801164C2`: a named local loaded at section-2 start gets s2 like the target but loads early; target hoists only the
   `lui`. Not solved.
Left over register diffs in best variant: sec-1 k/p (s0/s1 swapped), sec-2b chain temps a3/a1, sec-3/4 hoisted constants
(`li s0,19` vs ra etc.), flag s2 vs s0, sec-3b temps. All of these are downstream of the count/pool ordering.
Permuter (decomp-permuter with a flat-word compile wrapper) saturates around score 1475 from these bases.
Status: clearly stuck at structure+alloc coupling; not a MATCH.

## Round 5: callee func_800B78A4 matched
`func_800B78A4(u32 x, u8 n)` = popcount of the low `n` bits of `x` (stops early when `x` runs out):
`while (n != 0 && x != 0) { if (x & 1) c++; n--; x >>= 1; } return c;` (u8 parameter gives the
`sw a1,4(sp)` spill and `andi 0xff`). Strict MATCH at `-O2`, see `cloud/matches/func_800B78A4.c`.
Use prototype `s32 func_800B78A4(u32 x, u8 n)` in net_state_validate (it was called `(x, 16)`).
