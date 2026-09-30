# func_800EA3F4 (0x800EA3F4), scout report

Feasibility: **MEDIUM** (best of the five scouted). Effort: 4-8 agent-hours of frame/spill trial and error.

## Facts
- 762 words in `asm/us/blob/blob_800e7030.s`. Not spliced, no C in `src/blob/` or `cloud/matches/` (only prototypes elsewhere).
- **ABI, not IPA.** Prologue reads only `a0`,`a1` (both home-slot spilled: `sw a1,164(sp)`, `sw a0,160(sp)`); no `s` regs saved or used; frame 160 (0xA0), `ra` at 36. Only caller `func_800EB028` passes `a0=t0`, `a1=&s0->36` with nothing else set up. All four callees are ABI (checked with `ipa_signals.py`: no entry reads of `t*`/`s*`, no unsaved `s*` writes).
- 4 distinct callees: `func_800E8CB8` (3 args!), `func_8008B3C8` (`float len(f32 *)`, 11 words), `func_800CFDEC` (vector lerp), `vector_copy_scale` (normalize `src` into `dst`, 2 args). ~24 distinct globals (tables indexed by a `s8` slot at `car+0x35C`: `D_801526F8`, `D_801526E0`, `D_80152574`, `D_801525F8`, `D_80152680`, `D_801551D8`, `D_80155210`, `D_80155178`, `D_801551A8`, `D_80150B70` (stride 0x98), plus about 11 float constants at 0x801244EC..514).
- Shape: straight FP/vector math (about 254 `lwc1/swc1`), 2 small loops, no jump tables, no multiply-by-constant tricks. Not a library. It is a per-wheel/car orientation update: builds two smoothed direction vectors (`D_80155178[idx]`, `D_801551A8[idx]`), a right vector by cross product, and normalises.
- Correct prototypes (m2c got them wrong):
  - `func_800E8CB8(void *car, void *vel, void *mat)`: caller does `addiu a2,a0,80` (`car+0x50`), a0/a1 pass through unchanged.
  - `func_800CFDEC(f32 *a, f32 *b, s16 n, f32 lo, f32 hi, f32 t, f32 *out)` (float 4th arg travels in `a3` via `mfc1`; `hi`,`t` at 16/20(sp), `out` at 24(sp)). Calls are `(sp68, sp74, 3, 0.0f, 1.0f, *p, sp5C)`.
  - `vector_copy_scale(f32 *src, void *dst)`.

## First pass
`seeds/func_800EA3F4.c` (hand-typed from m2c, `-O2`): 732 words vs 762, **aligned shape 80%, aligned exact 28%**, first ~60 words identical including register allocation and the `bc1f` layout. Measured with `near.py`.
Key findings from the pass:
1. Use `arg0`/`arg1` directly (macro `#define car ((u8 *)arg0)`); a local alias `car = arg0` makes IDO allocate `s0` and adds a save. The target reloads `lw t1,160(sp)` after each call.
2. K&R prototypes make floats promote to double and inflate the outgoing arg area; use real prototypes.
3. **Every named scalar local reserves a stack slot even when promoted to a register** (removing 6 block locals shrank the frame by exactly 24 bytes). The target frame is 160, with named scalars only at 0x84, 0x90, 0x94, 0x9C (plus `a0`,`a1` home) and five uopt spill temps at 0x2C,0x34,0x38,0x3C,0x40. My seed has about 18 named scalars, so the frame is 248. Rewrite with expression-style code (repeat `D_xxx[idx]`, `obj->f`) and only `idx`, `sp94`, `sp90`, `sp84` named, plus the five vec3 arrays at 0x44,0x50,0x5C,0x68,0x74.
4. Unresolved: the 5 spill temps are `idx*4`, `&D_80155210[idx]`, `f12` (the blend factor, spilled only around calls), `obj` pointer, `idx*12`.

## Approach
Rewrite `seeds/func_800EA3F4.c` body minimising named locals; drive with `python3 cloud/work/bigfish/sbs.py <src> func_800EA3F4 "<flags>" a b` (aligned side by side) and `slots.py` (stack slot histogram vs target). Then permute expression order. Try `score.py fn` with `-O2` (INDEX flag) and, if the spill geometry will not fit, test whether a `-O1` mix helps (unlikely).

## Risks
About 250 FP instructions: scheduler order swaps (`mul.s`/`add.s` operand order) are numerous but usually cheap. Frame geometry is the gate.
