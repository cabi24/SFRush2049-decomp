# func_8008B640 (model/texture velocity, `physics_velocity_integrate_*`)

Rewritten by hand from the assembly (cloud pass 2, 2026-09-29); the m2c seed is
superseded. **Builds; 6 of 8 members match** (cloud pass 4; was 1 of 8) with `-r4300_mul`
(`cloud/work/tools/zbuild.py --as1=-r4300_mul`).

```
model_bounds_calc                 18/18   MATCH
func_8008B640                      9/23   words differ (size 23/23)
physics_velocity_integrate_a      64/178  (size 178/178)
physics_velocity_integrate_b      MATCH   (zbuild reports 73/72: it is last in the object, trailing pad nop)
physics_velocity_integrate_c..f   MATCH   (66 words each)
```

## Cloud pass 4: what made `_b.._f` match

- `s16 lvl` **local** instead of `s32 lvl` + `(s16) lvl` casts: the cast form
  puts the `sll/sra` temps in a rotated register order (`v1,t8,t9` vs `t8,t9,v1`).
- `_c.._f`: the last float argument as a named local assigned just before the call
  (`f24 = M2C_FIELD(car, f32 *, 0xD0);`) puts the `lwc1 $f24` where the target has it
  (otherwise it drifts into the `jal` delay slot).
- `_b`: no named `rec` pointer and no named `flag`: every use spells
  `&((Rec808 *) &D_8014A250)[arg0->player]` and `flag` is the call argument itself
  (`lvl >= 13 && (t != 0 || ...)`); `t` is `s32`. Named locals become uopt webs that get
  their registers in a different order from expression CSE temps (found by the
  permuter, see below).
- The remaining blocker is unchanged and is now understood better: `func_8008B640`'s
  IPA parameter register. The IPA picks `idx`'s register as the first register above the
  callee's outgoing argument count: with the call `vector_normalize_length(&v[9], v)` (2 args)
  it gives `$a2`, with one argument `$a0`, with three `$a3` (tested by editing the prototype
  and call). The target has `$a0` with two arguments and `v` living in `$a1` (`lw a1; move s0,a1`).
  Not reproduced: named/unnamed `v`, `register`, struct array, copies of `v`, prototypes.
  `_a` (idx `$a2` vs `$a0`, `t` `$v0` vs `$v1`) depends on it.


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
- `_b.._f`: solved in cloud pass 4 (see above).
