# func_8008B640 (model/texture velocity, `physics_velocity_integrate_*`)

Rewritten by hand from the assembly (cloud pass 2, 2026-09-29); the m2c seed is
superseded. **Builds; 1 of 8 members matches** with `-r4300_mul`
(`cloud/work/tools/zbuild.py --as1=-r4300_mul`).

```
model_bounds_calc                 18/18   MATCH   (new member)
func_8008B640                      9/23   words differ (size 23/23)
physics_velocity_integrate_a      64/178  (size 178/178)
physics_velocity_integrate_b       9/72   (size 72/72; zbuild reports 73 only because it is last in the object)
physics_velocity_integrate_c       9/66   (size 66/66)
physics_velocity_integrate_d       9/66   (size 66/66)
physics_velocity_integrate_e       9/66   (size 66/66)
physics_velocity_integrate_f       9/66   (size 66/66)
```

Every remaining difference is register naming or the order of two independent
moves; instruction sequences, sizes and frames match.

## Closure (now complete)

The old group was `{func_8008B640, _a}` with `_a` kept as an ABI root, which
cannot reproduce the ROM: `_a` takes `$s1/$s2/$f20/$f22/$f24` (IPA registers)
because its five callers `physics_velocity_integrate_b..f` are in the same
IPA unit. They are now group members (roots: address-taken in the function
table at `func_800AC8D4`, so they stay in `keep` and use the normal ABI), and
`_a` is **not** in `keep`. `model_bounds_calc` (0x8008B26C, 18 words, IPA
callee: flag in `$s0`, object in `$a3`) is a member with a stand-in caller.
Its four other callers (`matrix_scale_apply`, `anim_state_update`,
`entity_anim_texture`, `buffer_swap`) are not needed for it to match.

## What the code is

`_b.._f` are per-player wrappers: they read the car (`player_array`, 0x3B8
bytes) and the per-player record (`D_8014A250`, 0x808 bytes), compute a flag
(`level >= N && car flag`) and three float parameters, and call `_a(obj, flag,
194, f20, f22, f24, arg1)`. `_a` steps the object's state (`obj->state`, s16 at
+4; -1 = idle), calls `func_8008B640(model, x, y, z, nx, ny, nz)` to seed the
model's vector, scales it by the record's level, and finishes through
`model_bounds_calc`.

## Findings that mattered

- `_a`'s home-slot store is `sw a0,64(sp)` (frame 40): the `s16` flag is the
  **seventh** parameter (`sp+24` of the entry frame), so it is declared last.
  The others' order (IPA registers) does not change codegen; `s1` first makes
  the `move s1`/`move a1` order match `_b`.
- `!(state < 0)` must be written `state >= 0`; the `(x < 0) ^ 1` spelling adds
  a temp and a `move s0`.
- `x * 2.0f` compiles to `add.s x,x`; the target has `mul.s` with `2.0f`
  loaded into a register. A local `f32 two = 2.0f;` reproduces it (the
  original probably had a named constant).
- Declare `car` before `rec` in `_b.._f` (address-computation order).
- `_b`: `flag = lvl >= 13 && (t != 0 || (car->flags & 0x800) != 0);` gives the
  `sltu t2,zero,t0; bnez t2` chain.
- `func_8008B640`'s index parameter must be `s32` with an inner `(s16)` cast:
  an `s16` parameter adds a home-slot store the target does not have.

## Blockers

- `func_8008B640`: the target takes `idx` in `$a0` and holds the model pointer
  in `$a1` (`lw a1,..; move s0,a1` around the call); ours puts `idx` in `$a2`
  and the pointer only in `$s0`. The same `$a1` pointer pattern appears in
  `_a` before `func_8008B32C(m, m, s)`. Reloading, a second local, a struct
  array (`D_8012E708[idx].m`) and parameter reorders did not change it.
- `_a`: `t` (the `(s16)(s32)` of the record's float) lives in `$v1` in the
  target, `$v0` in ours; every later temp then differs.
- `_c.._f`: the `(s16)` compare temps rotate (`t8,t9,v1` vs `v1,t8,t9`) and
  the load of `$f24` sits before the `move`s in the target, in the `jal` delay
  slot in ours.
