# func_8008B640 (model/texture velocity, `physics_velocity_integrate_*`)

**6 of 8 members MATCH** (354 of 555 words), rescored 2026-09-30
(`zbuild.py --as1=-r4300_mul`). Hand-written from the assembly. Not spliced.

| function | target words | result |
|---|---|---|
| `model_bounds_calc` | 18 | **MATCH** |
| `physics_velocity_integrate_b` | 72 | **MATCH** (zbuild prints 73/72: trailing pad nop, it is last in the object) |
| `physics_velocity_integrate_c`, `_d`, `_e`, `_f` | 66 each | **MATCH** |
| `func_8008B640` | 23 | 9 differ (size 23/23) |
| `physics_velocity_integrate_a` | 178 | 64 differ (size 178/178) |

## Closure: complete

`_a` takes `$s1/$s2/$f20/$f22/$f24` (IPA registers) because its five callers
`_b.._f` are in the unit; they are roots (address-taken in the function table
at `func_800AC8D4`), so they are in `keep` and use the normal ABI, and `_a` is
not in `keep`. `model_bounds_calc` (0x8008B26C, IPA callee: flag in `$s0`,
object in `$a3`) has a stand-in caller; its four other callers are not needed.

## What the code is

`_b.._f` are per-player wrappers: read the car (`player_array`, 0x3B8 bytes)
and per-player record (`D_8014A250`, 0x808 bytes), compute a flag
(`level >= N && car flag`) and three float parameters, call
`_a(obj, flag, 194, f20, f22, f24, arg1)`. `_a` steps the object's state
(s16 at +4; -1 idle), calls `func_8008B640(model, x, y, z, nx, ny, nz)` to seed
the model's vector, scales it by the record's level, finishes via
`model_bounds_calc`.

## Techniques that worked

- `_b.._f`: `s16 lvl` **local** instead of `s32 lvl` + `(s16)` casts (casts
  rotate the `sll/sra` temps to `v1,t8,t9`); `car` declared before `rec`;
  `_c.._f` last float argument as a named local assigned just before the call
  (`f24 = M2C_FIELD(car, f32 *, 0xD0);`) keeps `lwc1 $f24` out of the `jal`
  delay slot.
- `_b`: no named `rec` or `flag`: every use spells
  `&((Rec808 *) &D_8014A250)[arg0->player]`, `flag` is the call argument
  (`lvl >= 13 && (t != 0 || ...)`), `t` is `s32`. Named locals become uopt webs
  with registers in a different order from expression CSE temps.
- `_a`: home-slot store `sw a0,64(sp)` (frame 40) means the `s16` flag is the
  **seventh** parameter, declared last. `!(state < 0)` must be `state >= 0`.
  `x * 2.0f` compiles to `add.s`; the target has `mul.s` with `2.0f` in a
  register, so `f32 two = 2.0f;`.
- `func_8008B640`'s index parameter is `s32` with an inner `(s16)` cast (an
  `s16` parameter adds a home-slot store the target lacks).

## Blockers

- `func_8008B640` IPA parameter register. IPA gives `idx` the first register
  above the callee's outgoing argument count: `vector_normalize_length(&v[9], v)`
  (2 args) gives `$a2`, one argument `$a0`, three `$a3` (tested by editing the
  prototype). The target has `$a0` with two arguments and the model pointer in
  `$a1` (`lw a1; move s0,a1` around the call). Not reproduced (named/unnamed
  `v`, `register`, struct array `D_8012E708[idx].m`, copies, prototypes, param
  reorders). The same `$a1` pointer pattern precedes `func_8008B32C(m, m, s)` in `_a`.
- `_a` (depends on the above): idx `$a2` vs `$a0`; `t` (the `(s16)(s32)` of the
  record's float) is `$v1` in the target, `$v0` here; every later temp differs.
