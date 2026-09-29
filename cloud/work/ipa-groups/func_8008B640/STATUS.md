# func_8008B640

**BUILDS** under IDO -O3. Not matching; the group is incomplete by
construction (below).

```
$ python3 tools/cloud/score.py group cloud/work/ipa-groups/func_8008B640
func_8008B640:                 17/23 words differ
physics_velocity_integrate_a:  176/178 words differ
```

## Build fixes to the seed

- `block_15:` sat directly before `}` (cfe syntax error): now `block_15:;`.
- Dropped `arg6 = arg0;` (m2c's name for the `sw a0,64(sp)` home-slot spill).
- `saved_reg_f20/22/24` became parameters `ipa_f20/22/24` of
  `physics_velocity_integrate_a`, passed through to `func_8008B640`.
- `model_bounds_calc(M2C_ERROR(unset $a0), flag)` became
  `model_bounds_calc(flag, ipa_s1)` (it is unprototyped in the prelude).
- `func_8008B640` rewritten from the assembly:
  `(s16 idx, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz)`. It stores
  `x/y/z` at `v[9..11]` of `*(f32 **)(D_8012E708 + idx*0x44)`, calls
  `vector_normalize_length(&v[9], v)`, then stores `nx/ny/nz`. The seed had
  the index stored as a float and the arguments shuffled.

## Why it cannot match as seeded

The IPA register choices come from callers and callees outside this group:

1. `func_8008B640` reads `$f20/$f22/$f24` that its only caller,
   `physics_velocity_integrate_a`, never sets. They pass through from
   `_a`'s callers.
2. `physics_velocity_integrate_a` reads `$s1`/`$s2` on entry, and is called
   (`jal`) from `physics_velocity_integrate_b`, `_c`, `_d`, `_e` and `_f`.
   None of those is in the group. Each is a normal-ABI root: it saves
   `$s0-$s2` and `$f20-$f24` and sets `$s1`, `$f20`, `$f22`, `$f24` before
   the call. Their addresses are taken in `func_800AC8D4` (a function-pointer
   table), which is why they keep the standard ABI.
3. `physics_velocity_integrate_a` calls `model_bounds_calc` with the object
   in `$a3` and a flag in `$s0`. `model_bounds_calc` (0x8008B26C, 18 words)
   is itself an IPA callee with four other callers: `matrix_scale_apply`,
   `anim_state_update`, `entity_anim_texture` and `buffer_swap`.

So the real unit is at least `{func_8008B640, _a, _b.._f}` plus
`model_bounds_calc` and its other callers (as context). The group generator
(`build/ipa_groups.json`) missed the `_b.._f` callers, probably because they
are only reached via `jal` from roots that are otherwise address-taken.
Suggest regenerating this group from that closure before more hand work.

## Relocations (for review until F2 lands)

- `func_8008B640`: `D_8012E708` (%hi/%lo), `jal vector_normalize_length`.
- `physics_velocity_integrate_a`: `player_array`, `state_word_a`,
  `D_8014A640`, `D_8012E708`, `D_8011735C`, `D_801427C0`, `D_80161368`;
  calls `func_8008B640`, `func_8008B32C`, `model_bounds_calc`,
  `func_8008B000`, `model_data_load`.
