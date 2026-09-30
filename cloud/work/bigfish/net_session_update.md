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
