# func_800AD4C8 (segment / point closest-approach test)

## Current status (Rescored 2026-09-30 (Round 2 addendum), master d0891f3.)

**Builds; no member matches (0/791 words).** Scored with `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/func_800AD4C8 --as1=-r4300_mul`:

```
func_800AD4C8              45/66   words differ  size  66/66
func_800C3AD0             334/362  words differ  size 356/362
input_process_controller  340/363  words differ  size 362/363
```

Blockers: `func_800AD4C8` is float register naming only (`0.0f` in `$f20`, radius in `$f18`). The two big
members are hand-written but the ROM keeps `poly` in `$a3` across `func_800AD650` (IPA knows that clobber set),
so ours promotes it to an `s` register.
Closure gaps (still open): callers `camera_play_script`, `camera_trigger_check`, `camera_victory`, `entity_update`,
`func_800C36A0`, `input_deadzone_apply` (stand-ins used) and callees `func_800AD650`, `func_800AD5D0`. The
headline block below ("Cloud pass 2": 353/362, 356/363 "seed, code missing") is history; see "Pass 3".

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

## Pass 3: func_800C3AD0 and input_process_controller rewritten by hand

The dropped code was a compressed-vertex decode (`DECODE`: `(s16<<5 + ((w & mask) >> shift)) * 0.03125f` from
the 8-byte `PV` table `D_8015201C`) inlined ten times, a `u16 idx[20]` stack array filled by `func_800AD5D0`
(the seed lost it, hence 100 missing words), and a convex-polygon edge test (`cross < 0` then
`func_800AD4C8` closest-point test). Both are now typed (`Poly`, `PV`, `DECODE` macro, `va/vb/vc/vd/ve` vectors).

```
func_800C3AD0             334/362 differ  size 356/362  (was 353/362, size 263)
input_process_controller  340/363 differ  size 362/363  (was 356/363, size 282)
func_800AD4C8              45/66  differ  size  66/66   (unchanged)
```

`input_process_controller`'s real parameter order is positional `(p1 s1, p2 s2, out s3, poly a3, outIdx s5,
flag s6, vcOut s7, mat s8/fp, rad2 f24)`: the seed listed `arg3` first only because m2c sorts by register.
The two spilled floats at sp+24/28 are reproduced with `volatile f32 f1, f2` (`va[0] = f2 = ...`).
Left: the ROM keeps `poly` in `$a3` across the first `func_800AD650` call (IPA knows that callee's clobber set),
so ours promotes `poly` to an `s` register and, in `input_process_controller`, leaves the 8th argument on the
stack (`lw a0,220(sp)`). `func_800AD650`/`func_800AD5D0` are the missing closure. Also `f32 zmin` lands in
`$f22` (ROM `$f20`, the 0.03125 constant takes `$f22`), and the locals area starts at sp+52 instead of sp+24.
