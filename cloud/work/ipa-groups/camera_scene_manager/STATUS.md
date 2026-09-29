# camera_scene_manager

**BUILDS** (cloud pass 2026-09-29) with minimal fixes to the m2c seed; not
matching. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
camera_scene_manager  605/614   (size 629/614, 12 extra words)
func_800C2430         158/165   (size 161/165)
func_800C26C4         155/160   (size 157/160)
func_800C2944         161/167   (size 165/167)
```

## Build fixes

- `fabsf` was called but never declared, so cfe compiled it as an
  implicit-`int` external call (unresolved `fabsf` in the scorer, and about
  30 extra words). It is now declared and `#pragma intrinsic (fabsf)`, which
  emits `abs.s` as in the ROM. The same seed bug was in `func_800E56F8`
  and `func_800E92C8`.
- The flag test on `D_80152900` cast the address to `GameCar` and masked
  the struct; both uses now read the flag word as `*(s32 *)`.

## Closure gap

`func_800C1B60`, `func_800C2004`, `func_800C220C` (`unprototyped` in
`group.json`) and `func_800C3578` are called but not in the unit. The
members' register choices around those calls (`$a0` vs `$s2` in
`func_800C2944`) will not match without them.

## Relocations

Seed-level only; not reviewed.
