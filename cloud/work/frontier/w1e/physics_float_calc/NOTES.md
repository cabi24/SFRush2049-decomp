# physics_float_calc (0x8009EBC0, 294 words) — not matched

Best: `best.c`, `-g0 -O3 -mips2 -G 0 -non_shared`:
`210/294 words differ (12 extra words (nonzero beyond target length); 2 section-relative relocations unverified: .rodata+0x0 at +0x1e0, .rodata+0x0 at +0x1e8)`.
Aligned by instruction (`adiff.py`): 176 retail words without a counterpart. It also owns a 19-entry jump table at
0x80123B14, so even identical code would score "unverified" until the scorer owns rodata.

## What retail shows (tested unless marked)

1. **Saved `ra` and `s0` with no `jal`.** Only two functions in the image save `ra` without a call
   (`net_session_update`, which uses `ra` as a general register, and this one).
   Reproduced: a call guarded by a condition that only uopt can fold (`s32 dbg = 0; ... if (dbg) foo(obj);`, `k2.c`)
   disappears from the output but leaves `sw ra`, the callee-saved register for the call's argument (`obj` -> `s0`),
   the homed-and-reloaded `arg2`, un-hoisted loop constants (`lui at,0x7fff` in the loop) and a loop that is not
   unrolled. A literal `if (0)` is removed by cfe and leaves nothing; an inlined empty callee leaves nothing; a
   non-inlined empty static leaves real `jal`s.
2. **`count`, `index`, `bestDist`, `best` live on the stack and `D_80149B80` is re-read after each store to them.**
   Reproduced by making the zone search a `static` function that writes through pointer parameters and is inlined
   (`c.c`): the first loop is then retail instruction for instruction. A local struct keeps the four in memory but
   does not reload the global; `volatile` does not reproduce the load pattern.
3. **`func_8009EBB8` (the `jr ra; nop` stub directly before the function, no callers)** is the deleted body of that
   inlined static (inferred from 2 and from callee-before-caller emission).
4. Loop 2 in retail has two dead induction variables (`a2 += 4` with the mask word pointer, `a3 += 68` with the object
   pointer) and computes the object base after the `blez` guard: the source indexes `D_8012E700[D_80149D90 + i]`
   and `mask[w]` (tested: `keep/loop2_indexed_qi_fo_A.c` reproduces `s0`, `a3` and the base computation).
5. The value passed to the dead call gets the callee-saved register (tested with `obj`, the indexed element address,
   and `bit`), so the dead call takes the object pointer.

## Residual (what did not move in about 800 scripted variants)

- **`bit <<= 1` placement.** Retail shifts at the bottom of the loop (`sll t5,v0,1; move v0,t5; bne`). Every variant
  with the dead call inside loop 2 shifts right after the mask test. Without a dead call the shift is at the bottom
  but the loop is unrolled by four. `bit += bit`, `bit *= 2`, the `for` increment slot, `s32` vs `u32`: no change.
- **`mask` across the switch.** Retail keeps `mask` in `t2` through the 19 cases (with one `lw t2,100(sp)` for the
  uninitialised default path) and copies it to `t0` before the loop. Every variant stores it to its home slot in each
  case and reloads it before the loop.
- **Temp ring.** Retail's ring is `t3`-`t9` (uopt used `t0`-`t2`); the variants' ring is `t0`-`t9`.
- **Callee stack layout.** Retail: `d[3]` at sp+60 with seven word slots below it (32..56) and seven between it and
  `mask` (72..96), four between `mask` and `best` (104..116). `best.c`: `d` at sp+36.

## Best next hypothesis

The dead call is not inside loop 2. It sits where `arg2` and the object pointer are live across it but `bit`, `i`,
`n` and `mask` are not, and it is itself part of the inlined static (so that the static's parameter that is a
constant 0 at the single call site is the folded condition). Concretely: make the whole body the static
`func_8009EBB8(arg0, pos, arg2, debug)` called once with `debug = 0` from a thin `physics_float_calc`, and put
`if (debug) print(...)` between the switch and loop 2, then look for what keeps loop 2 from unrolling. Untested.
The four slots between `best` and `mask` and the seven below `d` are then that static's parameters and locals.
