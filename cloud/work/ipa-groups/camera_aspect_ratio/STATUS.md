# camera_aspect_ratio

**BUILDS** (cloud pass 2026-09-29) with minimal fixes to the m2c seed; not
matching. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
camera_aspect_ratio    27/50
camera_fov_control     28/60
camera_free_look      107/109   (size 103/109)
camera_look_at_point  191/199   (size 189/199)
camera_update         716/719   (size 667/719)
```

## Build fixes (minimal; the bodies are still the m2c seed)

- `camera_update`: `spB8`/`spBC` were used but never declared. With
  `spB4` they are a 3-float scale vector filled through `&spB4`, which the
  seed does not model. They are declared as plain locals so the unit builds.
- The pointer/integer warnings (`temp_t1 + temp_a0 * 0x44`) are left as is.

## Closure gap

`camera_build_view_matrix` is called but is not in the unit (listed as
`unprototyped` in `group.json`). `camera_process_input` also calls into this
cluster. The IPA register choices of the members depend on those callees, so
`camera_update`'s prologue (spilled `$s` set and frame size differ) will not
match until they are added as context.

## Relocations

Seed-level only; not reviewed.
