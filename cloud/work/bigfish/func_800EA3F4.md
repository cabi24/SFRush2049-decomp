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

---

## Round 3 log (hail mary agent, 2026-09-30)

### KEY FINDING: this function must be scored as an IPA/-O3 group, not with `score.py fn -O2`
The target's blend factor (`$f12`) is kept in a caller-saved register across calls and spilled only
around them (`swc1 $f12,64(sp)` before `jal`), no `$f20+` callee-saved use. Single-function `-O2` never
produces that (toy test: `-O2` and `-O3` single-file put such a value in `$f20`, or in a home slot).
Compiling the function **together with stand-in bodies of its four callees** through the group pipeline
(`score.compile_group` steps: cc -j, uld -kp, usplit, umerge, uopt, ugen, as1 `-r4300_mul`, flags `-O3`)
reproduces: frame 160, the first 43 words byte-identical, `f12` blend register + call-time spills, and the
`lui a0/addiu a0` address form for `D_8002EB94`. The stand-in callees compile to exactly the retail sizes
(func_8008B3C8 11, vector_copy_scale 20, func_800CFDEC 34, func_800E8CB8 38 words).
Group harness: `cloud/work/bigfish/func_800EA3F4/group/` (group.c + group.json, `zbuild.py`-compatible;
`python3 cloud/work/tools/zbuild.py cloud/work/bigfish/func_800EA3F4/group --as1=-r4300_mul`).

### Other facts established
- Every named scalar local reserves a 4-byte frame slot even when promoted; block-scoped locals in
  sibling blocks share slots. Target has exactly 8 function-level scalar slots (0x80..0x9C) in the order
  idx, ?, sp94, sp90, ?, ?, sp84, ? (the `?` are promoted, never accessed).
- The `mode == 2 || mode == 3` add block is compiled twice (source is `if (m == 2) {A} else if (m == 3) {A}`).
- `sp44` stores are z,y,x order; second vector (sp50) values live in `$f14/$f16/$f18` between blocks.

### Progress (group build, `-O3` IPA harness, as1 `-r4300_mul`)
Findings that moved the score (each verified by compile):
1. Group build with stand-in callees: frame 160, first 43 words identical (see above).
2. `extern volatile f32 D_8002EB94;` (frame delta time): reproduces the `lui a0/addiu a0` address-CSE form
   and per-branch reloads. 60 -> 61% aligned.
3. Integer/double-typed literals matter: `f12 = 0.0;` (not `0.0f`) makes the blend zero a direct
   `mtc1 zero,$f12` instead of a hoisted shared zero temp. Likewise `CF(0xAC) < 0`, `f12 > 0`, `q == 0`,
   and the second `func_800CFDEC` lo-argument `0` use int-typed zero (local rematerialisation), while the
   clamp compares/stores and `p == 0.0f` and the first cfdec's `0.0f` argument stay float (hoisted `$f16`).
   Pipeline lesson: an int literal in a float context is a different uopt constant web than `Nf`.
4. Frame layout: 8 function-level scalar slots (`idx`, 4 promoted-unused names, `sp94`, `sp90`, `sp84`);
   the blend factor is a *block-level* local below the arrays (its home is slot 64, only written before calls).
   Best named set: function-level {len, i, st, f2(=1+X scale)} + post-array {f12}. slotdiff to target = 3-9.
5. `as1` must be given `-r4300_mul` directly (not `-Wab,`); wrong flag hid the nop padding.
Tools (scratch, not committed): aligned-word LCS score with relocation masks; single-flip hill climb over
operand order and int-vs-float literal per site.

### RESULT: STRICT MATCH 762/762 (group mode), 2026-09-30
Source: `cloud/work/bigfish/func_800EA3F4/base.c` (= `group/group.c` = `cloud/work/ipa-groups/func_800EA3F4/group.c`).
Verified: `python3 tools/cloud/score.py group cloud/work/ipa-groups/func_800EA3F4 --claims` -> func_800EA3F4 MATCH
(and all four stand-in callees MATCH as context); `zbuild.py ... --as1=-r4300_mul` -> 762/762 MATCH.
NOT reproducible with `score.py fn -O2` (needs IPA caller-save allocation); so it lives in `ipa-groups/`, not `cloud/matches/`.
Last steps that closed the gap: scale-then-add vector idiom (634 -> 754 words), operand flips on 4 cross-product
muls (758), then an unused `s32` local to reproduce the retail frame's spare slot (spill 48 -> 44): 762.
Full list of findings: `cloud/work/ipa-groups/func_800EA3F4/STATUS.md`.
