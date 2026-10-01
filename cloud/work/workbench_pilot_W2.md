# Workbench pilot W2 (agent D): register-rotation-wall functions

Scored with `tools/cloud/score.py` (strict) plus `decomp_workbench diagnose` against `~/agents/wb/targets/<fn>.o`.
Default flags `-g0 -O2 -mips2 -G 0 -non_shared`; `-O1/-O3/-r4300_mul` were not needed. Roughly 45-60 variants per function,
run as batches through a small scorer script (strict score + the diagnose verdict line) rather than 40 hand-read `loop.sh` runs;
every lever below was read from a full diagnose report at least once.

## func_800966D8 - MATCH (flags `-g0 -O2 -mips2 -G 0 -non_shared`)
- Start: `structure-mismatch`, 7/23 words (seed). Diagnose: lane `temp` target t6 t7 t8 vs candidate t7 t8 t9 (rotation +1), pool identical, `lever: none-known`.
  The "structure" part was only the 16-byte padding nop of the target (24 vs 23 instructions); the real residual was one ring pop plus one coalescing choice.
- Levers tried:
  - L14/L16 style source edits did not move it. Removing `new_var2` (the constant-6 local): ring right (t6 t7 t8) but pool wrong, 7 -> 13 (that edit also re-read through `temp_a0`).
  - Literal 6 in the re-read (keeps the loop structure of the target): ring right, but uopt now hoists `move a2,a1` (arg1 copy) and puts the `lh` value in a1: aligned 10.
  - Drop the in-expression `new_var =` assignment: aligned 6; the lh value then lands in a ring temp (t9) instead of pool a2.
  - L7/L9 dead web `if (arg1) {}` before the first test (zero code): pins arg1 in a1; 22 -> 3 words (pool lane now identical).
  - Restore the in-expression `new_var =` local (pool a2) with the dead read: 6 words, ring wrong again (the named local costs a pop, L76 direction).
  - L16/L65 redundant mask `((temp_a0_2 << 5) - new_var) & 0xFFFF` stored to an s16 field: 6 -> 1 word (supplies the missing phantom pop between sll t8 and subu, so subu takes t0 and the final andi t1).
  - Last word: `andi a0,v1,0x1ff` vs target `andi a0,a0,0x1ff`. ~25 variants (swap which of temp_a0/keep feeds the mask, decl order, `|0`, casts): no change. Changing `temp_a0`/`keep` from `u16` to `s32` fixed it (1 -> MATCH).
- Final: **MATCH**. Most-moving single change: the zero-code dead read `if (arg1) {}` (22 -> 3), then the redundant `& 0xFFFF` (6 -> 1).
- Verdict on the tool: diagnosis correct but incomplete. The lane view located the problem exactly (ring phase +1, pool identical), and lever 16 ended up being the right lever, but
  the tool offered `none-known` for this function and its `structure-mismatch` verdict was misled by the 16-byte pad nop (it pointed at "structure-buckets" levers 1,4,24,5,6, none useful).
  The useful information came from the lane output and from reading L64/L65/L76 laws, not from the "lever:" line.

## func_8008A644 - MATCH (flags `-g0 -O2 -mips2 -G 0 -non_shared`)
- Start: `structure-mismatch`, 18/24 words, relocation-symbol caution (6 sites). `lever: none`.
  The sites were not a real symbol difference; the seed (the `do{}while(0)` p16/pg form) simply did not hoist the address of D_8012E67A across the branch.
- Levers tried:
  - Plain natural source (`if (arg0 != (u16) D_8012E67A) {...}`): 18 -> 14; adding the `| 0` on the shift (earlier agent's trick): 7 (address registers a1/a2 now hoisted like the target).
  - Reverse compare operands, `if ((u16) D_8012E67A != arg0)`: beq operand order fixed, 7 -> 5/6; store order of the two words (`[0]` first in source) moved the `lui 0xee00` before the `sll`: 5.
  - Verdict became `allocation-mismatch` / pool lane: pointer in v0 instead of target v1. `lever: none-known` ("register-role-audit" playbook gives no source change).
    Lever 8 (expression dead read `if (arg0 << 0x10) {}`) before the first store pushed the pointer to v1: 5 -> 3.
  - Remaining t0 vs t1 on the sll result: L16/L65 phantom pop (`((u32) arg0 & 0xFFFF) << 0x10`, a redundant mask): 3 -> 2, verdict `schedule-mismatch` (only the two stores swapped).
  - Lever 23/25 idea (statement line boundaries are scheduling barriers at -g0): put the two stores on one source line: **2 -> MATCH**.
- Final: **MATCH**; the file keeps the stores on one line (a second variant with only the `if (arg0 << 0x10) {}` statement also on that line matches too). Most-moving: the one-line store pair (L23/25 family), after the dead read (L8) and the redundant mask (L16).
- Verdict on the tool: good. Diagnose's classes tracked the progress honestly (structure -> allocation -> schedule) and the field guide had a lever for each stage
  (8, 16, 23/25). Only the first stage (hoisting of the address registers) was not named; the diagnose's relocation-symbol caution was a false alarm for this case.

## func_8008B000 - NOT matched, best strict 15/54 (aligned residual 7-9)
- Start: `structure-mismatch`, 53/54 words (seed). The seed is simply wrong source: m2c scaled `(&D_801161F4) + idx*8` as an `s32 *` (shift by 5 instead of 3), the params are s32 where the
  target spills u16/s16 homes, and the `if (arg1 >= 0)` branch falls into the loop instead of returning. Diagnose could not say any of this (`structure-mismatch`, 8 gaps, lever 1/4/24/5/6 generic).
- Rewrite from the target asm (typed table `{Ent *base; s32 x;} D_801161F4[]`, `Ent{pad[0x16]; s16 count; Slot slot[4];}` 0x58 bytes, params `(u16, s16, u16)`, if/else with the loop in the else): 53 -> 40.
  Both a u8 byte-offset version and the struct version gave the same 40.
- Levers tried:
  - Declaration order, loop forms (for / do-while / while, pointer walk, `i++` vs `i += 1`), param types (s16 arg2: loop right but prologue `sll/sra`; s16 `i`: first half matches exactly, loop wrong): 40 -> 31..17 positional, none strict-clean.
  - L7/L8/L9 dead reads: `if (arg2) {}` and variants are inert on a parameter (L8 note: expression form needed). `if (arg2 + 1) {}` in the else branch shifted the early pool webs (the t3/t4/t0/t1/t2 lane) onto the target exactly: aligned 35 -> 9, the whole function up to the loop is then byte-identical.
  - Residual after that: the loop has `move a0,a2` (a copy of arg2) and the counter in a2 where the target keeps arg2 in a2 and the counter in a0 (pool lane a0/a3 swap); ~30 variants (dead reads of arg2/arg1/e->count/i in other positions, 1-3 refs per statement per L39, `(s16)` casts on the stored value, local copies, loop forms) did not remove it.
- Final: best strict 15/54 (aligned_total 11), best aligned 7 (the `(arg2+1)&&(arg2+1)&&(arg2+1)` variant). Not a match; no `cloud/matches/` file.
- Verdict on the tool: its pool-lane output was accurate and led to the dead-read lever that fixed 40+ words of the function in one step, which is real value. It did not help with the final loop webs (no lever for "param web split into a loop copy"),
  and for the initial seed it was no help at all: a wrong-element-size source (m2c artifact) reads as `structure-mismatch` with no hint to re-derive from the asm.

## func_800A79F4 - NOT matched, best strict 44/60 (aligned_total 41)
- Start: `structure-mismatch`, 58/60 words, 17 relocation-symbol differences (wrong address hoisting, not different symbols), `lever: none`.
- Facts established from the asm: the params are all plain s32 (target has no sign-extension and no `sw a0/a3` homes); the `unk8 = arg0` store appears once at the end; arg0 is copied into t0 (`move t0,a0`) because a0 is the pad pointer.
  Using s32 params instead of s16 (seed): 58 -> 57; the loop and the n/i webs are structurally the same as the target.
- Levers tried:
  - `var_v1 = 0` before `var_v0 = D_801613AC` (first-occurrence order, the L39 tie-break): 57 -> 51 (aligned 55 -> 42).
  - Use the local `var_v0` instead of re-reading the global in the compares (so the n web is one web in v0, not a t0 load plus a `move v0,t0` copy): 51 -> 45 (aligned 41); the loop now matches the target word for word except `li t0,2` vs `li t2,2`.
  - Extra references to arg0 (dead reads `if (arg0 + 1) {}`, duplicate early store of `unk8`) to raise its priority so it gets t0 instead of an `sw a0,0(sp)` home: no help or worse (44-62).
  - Extra reads of n at the end (`if (var_v0 + 1) {}`) to keep v0 busy so arg5/arg6/arg4 loads go to t1/t2/t4 instead of v0/t0: inert.
- Remaining: arg0 is spilled (`sw a0,0(sp)`/`lw t3,0(sp)`) instead of `move t0,a0`; the three stack-arg values take v0/t0/t... instead of t1/t2/t4; stores of arg5/arg6 order. All pool-position issues for which the guide has levers 7-13 but none moved this.
- Final: best strict 44/60. Not a match.
- Verdict on the tool: lanes were correct and showed the pool problem (n in t0 vs v0) early, but there was no applicable lever; the "pool-position" levers 7-9 (dead reads on a parameter or a global-derived local) were inert here, and the verdict stayed `structure-mismatch` throughout
  because the real structure differences (address hoisting, arg spill vs copy) are allocation decisions.

## Summary

| function | start | best | result | key move |
|---|---|---|---|---|
| func_800966D8 | 7/23 | 0 | MATCH `-g0 -O2 -mips2 -G 0 -non_shared` | dead read `if (arg1) {}` (L7/9) + redundant `& 0xFFFF` (L16/65) + `s32` for the two u16 temps |
| func_8008A644 | 18/24 | 0 | MATCH `-g0 -O2 -mips2 -G 0 -non_shared` | `| 0` + dead read `if (arg0 << 16) {}` (L8) + redundant mask (L16) + both stores on one source line (L23/25) |
| func_8008B000 | 53/54 | 15/54 | not matched | re-derived source from asm (40), then `if (arg2 + 1) {}` pool dial (first half identical); loop webs unresolved |
| func_800A79F4 | 58/60 | 44/60 | not matched | s32 params, `var_v1 = 0` first, one n web via the local; arg0/stack-arg pool webs unresolved |

Where `diagnose` helped: the temp / pool / shared lane split told, for 800966D8 and 8008A644, exactly which population (ring phase versus colored pool) differed, so levers could be chosen instead of searched;
the verdict changing from `structure-mismatch` to `allocation-mismatch` to `schedule-mismatch` was a reliable progress meter, and levers 8, 16 and 23/25 are what actually closed the two matches.
Where it did not: on the initial seeds the verdict was `structure-mismatch` with generic bucket levers (the pad nop made 800966D8's "gap" and the relocation-symbol cautions on 8008A644/800A79F4 were artifacts of wrong address hoisting, not a different variable); `lever: none-known` appeared at the decisive steps; and the pool-position levers 7-9 are
sensitive to what the dead read is applied to (inert on a parameter and on a plain local, effective as `x + 1`), which the guide only mentions in lever 8.
Found beyond the guide: (1) a plain `if (param) {}` is inert but the expression form `if (arg + 1) {}` is a code-free web that shifted an entire early pool lane (8008B000) - lever 8's phrasing covers it but not the "parameter" case;
(2) the ring phase can also be fixed by a *named local's type*: `u16 temp/keep` versus `s32` changed which of two copies feeds a mask (800966D8, last word) - a coalescing direction that none of levers 14-18 names;
(3) statement line boundaries as schedule barriers (L23) work for plain `cc` sources too: putting two adjacent stores on one physical line swapped their scheduled order without any preprocessor.
