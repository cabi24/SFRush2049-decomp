# net_session_update (0x800F34D8) : hail-mary pick

Feasibility: **medium-low (about 25 %)**, the best of the five. Effort: 2-4 focused days.

## Facts
- **892 words** (3,568 bytes). Not spliced (no `src/blob/net_session_update.c`; `work/game/race/net_session_update/base.c` is a stub).
  `src/blob/func_800F42C8.c` only *declares* it (`void net_session_update(void)`).
- **Pure leaf: zero `jal`/`jalr`, no callees at all.** No floating point. Integer only.
  Frame 312 bytes, saves ra, s0-s8; the body uses `ra` as a temporary (register pressure).
- **ABI / not IPA:** no arguments (`void`), no prologue reads of non-ABI registers, callers
  (`func_800F42C8` x1, `func_800F45F8`, `stat_race_start`) set nothing before `jal`. There is
  nothing to close: no IPA group is needed. It is a compile-alone `-O2` function (IDO unrolls
  simple `for` loops at `-O2`; the `n & 3` prologue loops in the ROM are that unrolling).
  Use `-Wab,-r4300_mul` (54 mult/div words) unless `score.py` already adds it (it now does).
- 106 branches (18 branch-likely), 54 mult/div (the `LCG % k` sequences), 4 distinct `lui` bases.
- **Globals (few):** `input_rec0[D_801543D4]` (stride 0x4C, chain `rec->0x48 -> *->0x2C -> *` gives
  a session block; info struct at +0x6F4: `+8 s8 mode`, `+9 s8 limit`, `+0xC u32 seed`),
  `D_80154658` (rand seed, the ANSI LCG `x*1103515245+12345`, `(x>>16)&0x7FFF`),
  `D_80154450[]` (0x4C-byte player/AI records, `active_player_count` is an `s16`),
  `D_80154640/28`, `D_80154FD0[]`, `D_801543D8..`, `D_801117C4` (s32 [6][12] table),
  `D_80111784` (table indexed by mode), `D_80156994`.
- **Nature:** game code (session/opponent setup: unique random ids `% 55`, Fisher-Yates style
  xor-swap shuffles, table copies, a per-slot random walk). Not library code, not repetitive; it is
  a sequence of about six phases, each a small loop. No arcade counterpart identified.

## First pass (scripts in `tools/`)
- Seed from m2c (own asm via `tools/mkasm.py`, `tools/seedgen.py`, `tools/autofix.py`): compiles after
  3 mechanical fixes but is gotos-only ("flowgraph not reducible"): 948 words vs 892;
  instruction-aligned closeness 63.5 % by opcode, 11.5 % opcode+registers (regs differ everywhere).
- **Hand skeleton of phases 1-4** (`../net_session_update_partial.c`, natural `for`/`do-while`
  loops, LCG as a macro): it compiles to 428 words (includes the whole epilogue) against the ROM's
  first ~388 words: **75.5 % opcode-aligned**, and the first ~50 words are the same instruction
  sequence as the ROM (only register names and the placement of `lui` differ). Register naming differs
  because the remaining ~500 words (not yet written) add live ranges (the ROM keeps `&seed` in `s2`,
  the LCG multiplier in `s5`, `55` in `a3`, current seed in `t5`).
  The ROM also hoists `lui/addiu D_80154640` inside the branch and reuses it: minor shape trims.
- This is real evidence that natural source reproduces the shape; the blocker is volume and
  register allocation across a whole 892-word body, not an unknown mechanism.

## Recommended approach
1. Finish the natural rewrite of the remaining phases (from `block_56` on: the `D_801543D8`
   per-slot loop with `rand % (n-1+1)`, the `D_80154FD0[.] == 4` skip loop, and the tail). The m2c seed
   at `SCRATCH/seed/net_session_update.fix.c` (regenerate with the tools) shows each phase.
2. Use the R2 findings: typed struct arrays (`Rec[]`, 0x4C), drop m2c temporaries, launder
   pointers with `(u32)`, `volatile` only where the ROM refuses CSE, permute local declaration order
   (stack slots and `s` register order).
3. Score by phase: cut the function into `#if` blocks and compare prefixes (`PREFIX=n close.py`).
4. Fallback if a phase will not match: none in-tree (a function is all-or-nothing), so budget for
   permuter (`decomp-permuter`) on the last 1-2 phases.

## Risks
- Unreduced flow graph in the ROM (m2c fell back to gotos) hints at a `goto retry` or a
  `break`/`continue` mix in the original; expect several structural trials per phase.
- 106 branches: a single wrong loop form shifts every later word (but the scorer localises it).


## Session log: full natural rewrite + search (2026-09-30, hail-mary agent)

Files: `net_session_update/base.c` (best single source so far), `net_session_update/tmpl*.txt` +
`srch3.py` + `opts*.json` (template + hill-climb search), `ev.py` (in-process scorer: exact/norm/shape),
`sbs2.py` (shape-aligned side-by-side), `tv.py` (variant tester).

### Structure facts established (all confirmed by compiling and diffing against the ROM)
- Whole function now written in natural C (phases: seed setup, zero table, unique ids, xor-shuffle,
  table copy, 12-column xor shuffle, slot table (mode==3 branch), per-player loop, sort by score, rank).
  Word count reaches exactly 892; frame 312 bytes; all sp offsets (84,92,248,264,288) equal the ROM.
- Frame: locals are laid out **first declared = highest address**; declaring extra dead locals adds stack.
  ROM layout = 6 scalars, `idx[6]` (@264), 3 scalars, `info` (@248), then ~120-124 bytes of other locals
  (modelled here by `u8 pad[124]`, contents unknown). Declaration ORDER of scalars has no effect on allocation.
- `p = &input_rec0[D_801543D4]` must NOT be a named local (it is an unnamed CSE temp spilled at sp+92);
  use `input_rec0[D_801543D4].a` / `.b1` directly and access `D_80154450[i]` by direct indexing (not via a
  pointer local), otherwise uopt reloads `D_801543D4` in the loops and 92(sp) never appears.
- Random number macro: `r = RAND(); r = (u16)((u32)r % ((u16)(max)+1))` with ONE signed int `r` (lives in v0);
  reproduces the dead `move v0,t7` / `mfhi v0` / `andi ffff` / `move v0,t8` pattern exactly. Storing inside the
  same statement (`x.id = RR(54)`) puts the `sb` before the seed store like the ROM.
- Retry loops are `goto again` (phase 2 unique ids) and `for(;;){ for(j..){..break;} if (j==i) break; }`
  (phase 5); `do{ x = RR(..);}while(k==x)` for the shuffles (x = a0 in the ROM, separate variable).
- Constant-bound loops in the ROM use `!=` tests in some places (`for (j = 0; j != 12; j++)` for the 12-column loop
  changed allocation to match s7=n, s8=apc, p spill).
- `D_80156994` is `s8` (lb). `D_80154628`/`D_80154640` s8 globals, `lim`,`mode` are s8 fields, `Rec` = 0x4C:
  id@0, b1@1, h2@2 (u16), b4[24]@4, b28[16]@28, h[16]@44.

### Scores (ev.py, vs ROM 892 words)
- natural draft: 872 words, exact 126, norm(opcode+imm) 701
- with frame/spill/RNG fixes: 892 words, exact 295, norm 744
- best searched mapping of loop variables: 892 words, exact 423, norm 761 (still not a match; register
  allocation differs: ROM keeps `3` in s0, base in s1, &seed in s2, counter in s3, mult in s5, cnt s6, n s7,
  apc s8, info in ra; uopt allocation is per-variable so loop variable roles matter).

### Final state of this session
- `net_session_update/base.c`: 892 words (same as ROM), frame 312 with all sp offsets identical, `score.py fn` = 692/892 words
  differ (200 strict-equal); near.py: aligned exact 584 (65 %), aligned opcode shape 837 (94 %), opcode+immediate 801 (90 %).
  After optimally renaming s-registers (only s7/s8 swapped: ROM n=s7, apc=s8; ours the reverse) aligned exact = 603.
- Everything left is register naming of uopt webs / ugen temps; instruction sequence is essentially the ROM's.

### Most useful findings for whoever continues
1. **`cc -c ... -K` keeps `a.s`, the ugen listing BEFORE as1 scheduling** (with the final register names, macros like
   `mul`, `remu`, `ble` unexpanded, `sw`/`lw` pairs before as1 forwards them to `move`). Comparing it with the ROM shows
   which choices are ugen's (temps, order) and which are as1's (hoisting into mult/div shadows, delay slots).
   Use it: `cd tmpdir; cc -c -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul -K a.c` (needs the same IDO env as score.py).
2. as1 hoists independent instructions into multu/divu shadows, so the ROM's "chain before LCG" is only a register-conflict
   effect: ROM's `(u16)(n-1)+1` chain uses v1,t9,v1, ours uses t9,t8,t6 (t6 is also the seed temp, so it cannot float).
   ugen picks temps from a rotating pool; the pool phase depends on how many temps were used earlier in the function,
   so one early difference shifts every later temp. The first real (non s-register) divergence is in phase 2:
   ROM keeps the `(u8)r` value in v0, the scan pointer in a1 and the constant 55 in a3; ours gets a1/a3/t1.
3. `x = RR(..)` copies of `r` (the shuffle partner index, ROM keeps it in a0 separately from r=v0) are removed by uopt's copy
   propagation in every form tried (types u8/u16/s16, chain assignment, for/do/goto). The ROM has a distinct web for it.
4. uopt allocation is **per named variable** and the s-register order is a priority order (ROM: s0=3, s1=&D_80154450,
   s2=&seed, s3=loop counter, s4=consts, s5=LCG multiplier, s6=cnt, s7=n, s8=apc, ra=info). Using `apc + e` inside the
   `D_80154450[..]` index (form 0) puts n=s7/apc=s8 like the ROM but hoists `apc*76+base` out of the loops (wrong);
   `e + apc` (form 1) matches the loop bodies but flips s7/s8. Unresolved.
5. `r` typed u32 (with a separate signed temp for `% 4`) reproduces the ROM's chain registers (v1,t9,v1) in the shuffle but
   removes 4+ words (signed div checks) elsewhere and moves r to a0 (x merges into r). Combine with (3) to make progress.
6. Search infrastructure (all in `net_session_update/`): `tmplN.txt` (template with role tokens and option slots),
   `opts*.json` (alternative statement forms), `srch4.py` (annealing with restarts, 4 procs, ~0.3 s/eval), `ev.py`
   (exact / norm / reg / exact-after-s-renaming metrics), `canon.py` (diff after best s-reg renaming), `sbs2.py`, `blk.py`.
   Searches plateau at aligned-exact ~605 (n=892); they cannot fix (2)-(5) because those need new source forms, not
   different variable roles.
