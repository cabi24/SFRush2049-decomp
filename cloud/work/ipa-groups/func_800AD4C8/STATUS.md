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

## Round 3: closure callees added as context

`func_800AD650` (57 words, nine `s16 * 2^-14` conversions, hand-written) and `func_800AD5D0` (the strict single-function
match from `cloud/matches/func_800AD5D0.c`, reused verbatim) are now defined in `group.c` and listed in `keep`
(both have callers outside the group: `camera_first_person`, `camera_play_script`). Emitted sizes are the ROM's
(57 and 32 words; 31 words when `func_800AD5D0` is left out of `keep`, which makes it IPA, so keep it).

```
func_800AD4C8             45/66   differ  size  66/66   (unchanged)
func_800C3AD0            334/362  differ  size 356/362  (unchanged count, but see below)
input_process_controller 340/363  differ  size 362/363
```

Effect: with the callees' clobber sets known, `func_800C3AD0` now keeps `poly` in `$s4` (ROM: `s4`, `pt` param is
`a3` only in `input_process_controller`), `outIdx=$s5`, `mat=$s7`, and its first ~30 words agree with the ROM
except for frame offsets. What still shifts every stack offset by 4 (and hence most of the differing words):
the ROM's `idx[20]` is at sp+56 and `f1/f2` are at sp+24/28, ours put `idx` at sp+60 and `f1/f2` at sp+52/56
(declaration-order permutations of the eight locals did not fix it: best 333). The ROM's `zmin` constant is in
`$f22`, ours `$f20`. `input_process_controller`: the ROM keeps `mat` in `$s8` (IPA), ours still leaves it on
the stack (`lw a0,220(sp)`, frame 192 vs 184); IDO never gave the 8th integer IPA parameter a register here.
Not MATCH yet. Verify: `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/func_800AD4C8 --as1=-r4300_mul`.

## Round 5 (r5_g): three matches, and what they taught

```
func_800AD650    57/57   MATCH   (was hand-written, 30/57 off as a kept function)
func_800AD5D0    32/32   MATCH   (unchanged)
handbrake_apply  54/54   MATCH   (new member; needs func_800AC9BC in the same unit)
func_800C36A0    268/268 MATCH   (new member, hand-written; see the end of this section)
func_800AC9BC    56 words emitted, 52/56 differ (quadtree lookup; see below)
func_800AD4C8    44/66 differ     func_800C3AD0 317/362 (size 356)     input_process_controller 308/363 (size 361)
```

Verify: `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/func_800AD4C8 --as1=-r4300_mul`
(also `python3 tools/cloud/score.py group ...`, which now reports the same).

**`func_800AD650` must not be in `keep`.** As a kept function it gets the ABI-caller view and
IDO allocates `f16/f18/t0-t2` in it (30 words off). Left out of `keep` (it still has two call sites,
from `func_800C3AD0` and `input_process_controller`, so it is not inlined) it is emitted exactly as in
the ROM: `t6-t9` and `f4-f10` rotating. Its source stays the nine straight-line `(f32) p[i] * 2^-14`
statements (a `for` loop is partly unrolled to 70 words). The same change makes the callers' registers
agree with the ROM: `input_process_controller` now keeps `poly` in `$a3` across the `func_800AD650`
call and `mat` in `$s8` (ROM: `a3`/`s8`), and its prologue (`li s4,1`) matches. Leave `func_800AD5D0`
in `keep` (31 words otherwise).

**`handbrake_apply` (0x800ACA9C, 54 words)** is a point-in-rectangle walk over an array of 20-byte quadtree
nodes at `D_80124EEC` (`QNode`: `s16 next; u8 pad; u8 mask; s16 x0,x1,y0,y1; u16 child[4]`). It returns 0
when it runs out of parents, otherwise it calls `func_800AC9BC(node, x, y, out)`. The `a3 -> t1` move
is not an IPA detail: with `func_800AC9BC` defined in the same -O3 unit (and both in `keep`) IDO
allocates the out pointer to `t1`; compiled alone at -O2 the pointer is spilled (20 words off). The test is
`x >= x1 || x < x0 || y >= y1 || y < y0` held in an `s16`, then `while (1) { outside = ...; if (!outside) break; ... }`.
`func_800AC9BC` (the callee) is the same structure but still differs in `y` handling: the ROM copies the `s16` `y`
into `t0` (`sll/sra` into `t8`, `move t0,t8`) and reuses `a2` for `q`; none of ~60 variants (`yy` copies, `int q`,
ternary, `do/while`, `for(;;)`) reproduced it. Rotated-loop layout (found-return after the loop) is right.

**`func_800AD4C8`:** `D_801141B0` is element 3 of a `Vec3f` array based at `D_8011418C` (the ROM addresses
`D_8011418C + 36/44`), now written that way. What is left is float register naming, which depends on the
callers' float registers (ROM: zero in `$f20`, radius in `$f18`); it will only settle with matching callers.

## func_800C36A0 (r5_g/c36): collision response, nearly matched

See `cloud/work/r5_g/c36/` (a group of its own plus two stand-in callers). Hand-written from the assembly:
`s1` = car (`CCar`), `s2` = collision record (`CCol`: `poly*` at 0, a 9-float matrix at 4 whose row at +16..24 is the
normal, `s16` wheel-table index at 40, `f32` impulse at 44). It applies the impulse to the car (`+556`, `+1940`, and the
two four-element `[4][3]` arrays at `+580`/`+628`), builds a torque-ish vector from `A244[idx] x v76 + v64`, calls
`func_8009E820`/`func_800A61B0` twice, scales by `D_80123F54 / f1472`, `f1592` and `D_80123F58..6C`, and adds the
result to `A196[idx]` (`+300` too when type is 5 or 6, `w1548[idx] == 8` and both components are below 4000).
Findings that generalise (in this file's own words): **(1)** IPA inputs get `s` registers in order *after* the function's
own locals: the loop counter and the polygon type share `s0` (one variable `i`), so `car` and `col` land in `s1`/`s2`
exactly when `i` is used for both. **(2)** Every *named* pointer local that is live across a call reserves a frame slot
even in a register: `u = car->A244[idx]` and `q = ...` cost 8 bytes; written as `car->A244[idx][n]` the frame is 96.
**(3)** A local declared *after* the other scalars gets the low slot: `idx` (spilled to `56(sp)` in the ROM) needs to be
declared after `i`, `f12`, `k`. **(4)** Operand order of commutative float ops (`a + b` vs `b + a`) moves the whole
float temp rotation: flipping two sums took the function from 181 to 6 differing rows.

### Frame layout rules learned (used to fix `input_process_controller` and `func_800C3AD0`)

IDO lays locals out bottom-up in *reverse* declaration order: the last declared local is lowest (just above the
spill/implicit area that starts at `sp+24`), the first declared is highest. Consequences that reproduced the ROM frames:

- The ROM's `f1/f2` at `24(sp)/28(sp)` are not declared locals: they are ugen's own float spills. The earlier `volatile f32 f1, f2`
  hack put them 16-24 bytes too high. Remove them (write `va[0] = vb[0] - ve[0]` and reuse `va[0]`/`ve[0]`) and the spills
  appear at `24/28` by themselves.
- A named scalar takes a frame word when it is live across a call (`res`, `n`; `poly` when it lives in an `s` register), and so
  does a named pointer such as `PV *e`; a float temp that only lives between calls (`t = vp[1]/d`) does too in
  `input_process_controller`. Dropping `t` (expression-inline `vp[1] / d`) took the frame 192 -> 184.
- Declaring a scalar (`res`) *first* puts its word at the very top (above the first array); this is the ROM's 4 spare bytes
  between `idx` and `vp`. In `input_process_controller` the order `idx, res, vp, vq, va, ve, v0, vprev, k, n, d, e, vd`
  (with `PV *e` and the `e = &D_8015201C[i]; DECODE(v, e)` form) gives frame 184, `idx` at 144, `vprev` at 68, `vd` at 40:
  identical to the ROM. `func_800C3AD0` with `res, va, vb, vc, vd, ve, idx, k, n` gives `idx` at 56, `ve` at 96, frame 160, spills at 24/28.
- Operand order of commutative float sums (`a + b` vs `b + a`) re-rolls the whole float-temp rotation (`f4/f6/f8/f10`): a two-statement flip took
  `func_800C36A0` from 181 to 6 differing rows. Use a toggle/hill-climb over such flips scored by aligned differing rows (see
  `cloud/work/r5_g/toggle.py`, `autoclimb.py`, `gsbs.py`); positional counts are misleading.

Still open in these two: the 8th IPA float parameter (`zmin`/`rad2`) sits in `$f22`/`$f24` where the ROM has `f20`/`f24`
(`func_800C3AD0`: constant `0.03125` in ROM `f22`, `zmin` in `f20`; ours the other way round), the ROM emits
`addiu a1,poly,4; jal func_800AD650; move a0,mat` (delay slot gets the `mat` move) where ours emits `move a0; jal; addiu a1`
(none of casts, locals or `+0` changed it), and a couple of decode load orders. `func_800AD4C8`'s callers decide its `f18/f20` choice,
so it will settle together with them.

Note (round 5): func_800AD5D0 is also a single-function match in cloud/matches/ and is claimed there, so it is not claimed here (no double splice).
