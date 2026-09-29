# menu_back (suspicious: not a work item)

Investigated 2026-09-29; not forced.

**Finding: the callers exist; the group generator missed them.**

- `menu_back` (0x800CBE08) reads `$s1` on entry (`lw t6,0(s1)`) and writes
  `$s0` without saving it, so it is an IPA callee.
- It is called by `jal` twice from `func_800CBF2C` (0x800CBF2C, 69 words),
  under `if (s0 != 0)`, with `$s1` live.
- `func_800CBF2C` also reads `$s1` on entry (`lw s2,0(s1)`), so it is an
  IPA callee too. Its callers are `func_800CC040`, `menu_item_value_get` and
  `props_render`, which presumably set `$s1` (not checked further).
- `menu_transition` (0x800CBE8C), also called from `func_800CBF2C`, uses the
  standard ABI.

So `menu_back` is not a function-pointer target or an extent problem. The
right group is at least `{menu_back, func_800CBF2C}` plus whichever of
`func_800CC040`/`menu_item_value_get`/`props_render` set `$s1`. Same
pattern as `func_8008B640` (callers `physics_velocity_integrate_b.._f`
missing): the generator's caller discovery drops real `jal` callers in some
cases. Worth checking `blob_group seed` before reseeding.
