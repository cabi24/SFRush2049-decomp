# func_800EA3F4

## Current status (Round 3, hail-mary agent, 2026-09-30)

**MATCH, 762/762 words**, strict, under the `-O3` IPA group pipeline (`score.py group ... --claims`):

```
python3 tools/cloud/score.py group cloud/work/ipa-groups/func_800EA3F4 --claims
python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/func_800EA3F4 --as1=-r4300_mul
```

It does **not** match with `score.py fn ... -O2` (do not add it to `cloud/matches/`; CI would rescore it
single-function and fail). Reason: the retail code keeps the blend factor in `$f12` across calls and spills
it only around them (IPA/-O3 caller-save allocation, no `$f20+`).

The group contains the function plus **stand-in bodies for its four callees**
(`func_8008B3C8`, `vector_copy_scale`, `func_800CFDEC`, `func_800E8CB8`); all four stand-ins are themselves
byte-exact retail matches (listed as context). The callees are kept (`keep` list) so they stay out of line.
`__standin_func_800EA3F4` keeps the function out of line (its retail caller is `func_800EB028`).

## What made it match (each step verified by compile)
1. Group build with callee stand-ins (frame 160 and the first 43 words appear at once).
2. `extern volatile f32 D_8002EB94;` (frame delta time): gives the `lui a0/addiu a0` address form and reloads.
3. Int-typed literals are different uopt constant webs than `Nf`: `f12 = 0.0;`, `CF(0xAC) < 0`,
   `f12 > 0`, `q == 0`, second `func_800CFDEC` lo-argument `0`, and `1 / len`, `-1 / len`, `1 - f12` are int
   literals; clamp compares/stores, `p == 0.0f`, `1.0f + f2`, the `sp5C` normalise `1.0f / len`
   stay float. (This fixed constant register assignment: len=$f14, 1.0f=$f18, 0.0f=$f16.)
4. Vector blends are written scale-then-add (`v[i] *= 1 - b; v[i] += w[i] * b;`), and `sp44` likewise
   (`sp44 = sp5C * sp94; sp44 += sp50 * sp90;`). This is what makes uopt hoist/keep the components in
   `$f2/$f14/$f16` and `$f14/$f16/$f18`.
5. `mode == 2 || mode == 3` is `if (m == 2) {A} else if (m == 3) {A}` (block compiled twice in retail).
6. Frame: 8 function-level scalar slots (`idx`, `len`, `sp94`, `sp90`, `i`, one unused local, `sp84`, `f2`)
   plus block-level `f12` (blend, home slot 64). The retail frame has one slot the compiler never uses,
   which shifts the `&D_80155210[idx]` spill from 48 to 44; an unused `s32 unused;` local reproduces it.
   (`s8 st = D_801525F8[idx]` was tried; unnamed direct `D_801525F8[idx]` is what matches.)
7. `as1` must get `-r4300_mul` directly (the `-Wab,` form is for `cc` only).

## Caveats for splicing
- Uses the volatile `D_8002EB94` declaration, int/double-typed literals, and one unused local; all are needed.
- Stand-ins for `func_800E8CB8` reference `math_utility`, `func_8008D6FC`, `D_801613AB`, `D_801106C0`.
