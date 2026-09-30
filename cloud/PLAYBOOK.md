# Cloud matching playbook (one page)

What worked in cloud rounds 1-3 (single functions and IPA groups). Setup and rules are in
[CloudHandoff.md](../CloudHandoff.md); group state and open blockers are in
[CloudHandoffV2.md](../CloudHandoffV2.md). Everything here was verified by strict scoring.

## The six rules

1. **Score strictly on the real bytes, every time.** `python3 tools/cloud/score.py fn|group ...`
   (it adds `-r4300_mul` itself). A compile plus strict compare takes about 0.3 s, so search
   instead of reasoning about one variant. While working, also compare *aligned by instruction*
   (`cloud/work/bigfish/near.py`): a positional score is misleading, because one extra or missing
   word shifts every word after it. "0 differing" without true strict MATCH is only a lead.
2. **Decide ABI or IPA first.** If the function reads non-ABI registers on entry (`t0`-`t3`,
   `s*`, `f16`+) or keeps values in caller-save registers across calls, no source change matches
   it alone. Build a small closed group with its callers/callees (`cloud/work/tools/closure.py`,
   `callers.py`). `func_800EA3F4` only matched as an `-O3` group. Functions with ABI-only callees
   are the best targets.
3. **Write natural source, not m2c-shaped source.** Typed struct arrays, plain `for` loops and
   real element types on externs fixed about half the single functions. Drop m2c's `spNN` spill
   locals and goto-loops unless the target needs them.
4. **Search, don't ponder.** When a function is structurally right and only register names or
   instruction order differ, script a mutation search (declaration order, temporaries, casts,
   loop forms) scored by aligned words. Keep the multi-line source layout: line layout changes
   IDO's scheduling.
5. **Scout before committing to a big function.** Check ABI vs IPA, callee count, globals, and
   how close a hand-written first pass gets. A truncated prefix cannot be scored strictly
   (frame and registers depend on the whole function), so a go/no-go on a 2,500-word function
   costs the whole function.
6. **Know when to stop.** About 50 variants with no movement on the same difference means the
   cause is in IDO/`as1`, not the source. Write it up in the group's `STATUS.md` and move on.

## Quirks that worked repeatedly

- **Frame size counts named locals.** IDO reserves a stack slot for every named scalar local,
  even one held in a register. Use expression style; an unused local or `volatile s32 pad[n]`
  reproduces an exact frame. Locals are laid out in declaration order, first at the highest address.
- **`do/while` vs `for`** compile differently (peeling, unrolling, `bnezl`); swapping fixed several functions.
  A known trip count invites unrolling: use a goto loop or an `s16` counter to stop it, or an `s32`
  counter to get the peel-2/unroll-4 shape.
- **Shared exits:** write every early exit as `result = N;` with one final `return result;`.
- **Parameter types:** an `s16` parameter adds `sll/sra` and a spill; use `s32`. A K&R callee
  prototype (`s32 f();`) removes `lh` narrowing of arguments.
- **One `lui at` for several stores:** `as1` merges them only when the symbol is *defined* in the
  same unit, not `extern` (and only for one array, in loop form).
- **`(u32)` laundering:** passing a pointer through a `(u32)` cast stops uopt hoisting loads above
  stores (`c = (PCar *)(u32)&player_array[i];`).
- **`volatile`** fields/globals stop reuse where the target does not reuse (`D_8002EB94` in
  `func_800EA3F4`).
- **Literal types:** int literals (`0`, `1 / len`) and float literals (`0.0f`) form different
  constant webs; a `Nf` where the target uses an int constant, or the reverse, rotates registers.
- **Compound forms:** `x *= 2` vs `x <<= 1` swap `sll`/`srl` order; scale-then-add
  (`v *= 1 - b; v += w * b;`) and `(u32) slot * 0x18` (no reuse of a hoisted `li 24`) matched FP/index code.
- **Stand-ins:** a missing callee can be stood in for by a function that clobbers the right
  registers; keep roots in `keep`, give each IPA function two call sites or `-O3` inlines it, and a
  dead `if (0) { switch ... }` in a callee stops inlining without changing words. A stand-in can
  never be spliced.
- **Debug listing:** `cc -K` leaves `a.s`, the listing before `as1` scheduling, which separates
  ugen's register choices from `as1`'s hoisting.

## Honesty notes

- Some matches depend on compile-affecting quirks (`volatile`, unused locals, literal types). They
  match the bytes but may not be the original source; say so in the PR.
- Claim only functions that score strict MATCH: put them in `group.json` `"claims"`. Keep scorer
  scripts in `cloud/work/tools/`, not in group dirs. Hand back matched singles in `cloud/matches/`.
