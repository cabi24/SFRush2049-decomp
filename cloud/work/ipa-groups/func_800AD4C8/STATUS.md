# func_800AD4C8 (segment / point closest-approach test)

Cloud pass 2 (2026-09-29). Builds; not matching.

```
func_800AD4C8              45/66   (size 66/66)      was 63/66
func_800C3AD0             353/362  (size 263/362)    seed, code missing
input_process_controller  356/363  (size 282/363)    seed, code missing
```

## func_800AD4C8

A leaf: `t = (a.z*p.z + p.x*a.x) / (a.z*a.z + a.x*a.x)` clamps to `[0,1]` and
writes the vector from the closest point on the segment to `p` into `out`
(`out.y = 0`), returning 1 when its squared length is below the IPA float
parameter `$f18` (radius squared). The seed's `D_801141B0` accesses were wrong
(`+0x2C` from the wrong base, and an `s32` load with `cvt.s.w`);
`D_801141B0` is a `Vec3f` and the code reads its x and z. With that fixed the
instruction sequence and size match; what remains is floating-point register
naming: the ROM keeps `0.0f` in `$f20` and the radius in `$f18`, and the local
webs (`a.z,a.x,p.x,t`) in `$f2,$f12,$f16,$f14`; the build uses `$f18` for the
zero, `$f16` for the radius and lower registers for the webs.

## Closure / IPA notes

`func_800C3AD0` and `input_process_controller` are IPA functions in the ROM
(register parameters) whose callers are outside the unit
(`camera_trigger_check`, `camera_victory`, `entity_update`,
`input_deadzone_apply`, all 300-900 words). Instead of leaving them in `keep`
(which forces the ABI and makes them save every callee-saved register), each
now has two stand-in call sites and is not in `keep`: two call sites stop the
inliner and IDO then gives them the IPA convention. This is the way to stand
in for a missing *caller*; a missing *callee* cannot be faked the same way
(its clobber set is unknown). See `func_800E4300`'s STATUS for the same trick
on `func_800E451C`.

`func_800C3AD0` and `input_process_controller` still come out 100-100 words
short: the m2c seed dropped the code that addressed stack arrays through `sp`
(see the earlier note in this file's history); they need a hand rewrite
from the assembly before their registers can be judged.
