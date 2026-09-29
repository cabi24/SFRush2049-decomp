# func_800AD4C8

**BUILDS** (cloud pass 2026-09-29) with placeholder declarations only; not
matching and not expected to match as is.

```
func_800AD4C8              63/66
func_800C3AD0             362/362
input_process_controller  361/363
```

## Build fixes

m2c addressed stack arrays through a bare `sp` (`(u8 *) sp + i*2 + 0x36`)
and left several stack slots (`sp3A`, `sp78`, `sp7C`, `sp92`, ...)
undeclared. `group.c` declares `u8 sp[0x100]` plus those slots as locals in
`func_800C3AD0` and `input_process_controller`, marked `/* cloud: ... */`.
That makes it compile; it does not model the real frame. The real code
keeps small arrays of `u16` indices into `D_8015201C` (8-byte packed
vertices: s16 x/y/z with 5-bit fractions in the fourth half-word).

## Group closure

The approximate IPA closure check (members plus every caller or callee that
passes values in non-ABI registers) adds six functions that are not in this
group: `camera_play_script`, `camera_trigger_check`, `camera_victory`,
`entity_update`, `func_800C36A0`, `input_deadzone_apply`. Until those are
included, the IPA register choices cannot be reproduced.

## Relocations

Seed-level only; not reviewed.
