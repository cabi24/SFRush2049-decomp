# func_800E56F8 (per-player update: car state, queue, scoring)

## Current status (Rescored 2026-09-30 (Round 2 addendum), master d0891f3.)

**Builds; no member matches (0/693 words).** Scored with `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/func_800E56F8 --as1=-r4300_mul`:

```
func_800E56F8  115/359 words differ  size 359/359
func_800E6AF8  313/334 words differ  size 332/334
```

`func_800E56F8` has the ROM's size and instruction sequence; every remaining difference is register naming from
the hoisted `li s6,2` (see below). `func_800E6AF8` was rewritten in Pass 3 (the "still close to the m2c seed"
remark and 300/334 below are history).
Closure gap (open): `func_800E4B58` (567 words, `func_800E56F8`'s IPA callee) and, behind it, `func_800E398C`
(599) and `func_800E451C`; chain `func_800E6AF8 -> func_800E56F8 -> func_800E4B58 -> {func_800E398C, func_800E451C ->
func_800E4300}`. `closure.py` also lists `func_800D0424`, `func_800D11BC`, `menu_control_settings`,
`menu_video_settings`, `object_update_full`, `track_select_handler` (approximate, not checked here).

Cloud pass 2 (2026-09-29). Builds. With `-r4300_mul`
(`cloud/work/tools/zbuild.py --as1=-r4300_mul`):

```
func_800E56F8  115/359 words differ   size 359/359   (was 356/359 differ, size 376)
func_800E6AF8  300/334 words differ   size 336/334   (was 316/334, size 337)
```

`func_800E56F8`'s **size and instruction sequence now match the ROM**
(one `lui` is scheduled a slot differently); every remaining difference is
register naming from one cause (below). `func_800E6AF8` is still close to the
m2c seed (loops as `goto`, several `s32` globals that are `u16`/`s16`).

## Fixes to the seed (func_800E56F8)

- `ipa_s4` is `s32`, not `s16` (the ROM uses it unextended); `var_v1` `s32`
  (an `s8` adds `sll/sra 24` per step); `temp_a0` `s32`.
- The shift-down loop is an index loop
  `idx = D_80153F40; var_a0 = D_8015274C - 1; for (; idx < var_a0; idx++)
  D_80152808[idx] = D_80152808[idx + 1];` with `D_80152808` an `s16` array
  (the seed's pointer walk compiled with a spurious copy). Reusing `var_a0`
  (an existing `s32`) instead of a fresh temp, and a fresh `idx`, put the two
  loop values in `$a0`/`$a1` as in the ROM.
- `func_800E543C(2, ipa_s4)`: two arguments (the seed's third,
  `M2C_ERROR(unset $a2)`, made the build emit `move a2,zero`).
- Float fields copied as `f32` (`M2C_BITWISE(f32, s32 read)` and `= 0`
  compile to `lw/sw`; the ROM has `lwc1/swc1` and `mtc1 zero`).
- `temp_f0 = (0x400 * 0x7F0) * (((f32)s16 * D_8012447C) + 1.0f)`: the product
  must be the *left* operand for the ROM's load order (any other spelling
  loads `0x7F0/0x400` before the first `mul.s`, 250+ words differ).
- `fabsf` sum as `(|x10| + |x14|) + |x18|`.
- `var_a0 /= 2; if (...) var_a0 += 0x186A0; else {...; var_a0 += temp_lo/800;}`
  keeps the divide-by-two before the branch (as a separate `temp_a0_2` it is
  sunk into both arms: 161 -> 250 differing words).

## Blocker (the only kind of difference left)

The constant `2` is the first definition of a register hoisted to `$s6` in the
ROM (`li s6,2` before the first branch, also used by `sb s6,857(s2)`); the build
puts that first definition in a temp (`li t8,2`), and the temp rotation then
shifts every later `$t6..$t9` by two. Tried: a named local `two`, statement
orders, reusing other locals. The `lui` of the `D_80143FF4` word is one slot
later than the ROM's.

## Closure

`func_800E4B58` (567 words) is still not in the unit: it is `func_800E56F8`'s
IPA callee, so its clobber set is unknown here. Merging with `func_800E4300`'s
group would need `func_800E4B58`, `func_800E398C` (599) and `func_800E451C`
(397) written from the assembly: the chain is
`func_800E6AF8 (root) -> func_800E56F8 -> func_800E4B58 -> {func_800E398C,
func_800E451C -> func_800E4300}`. Not attempted; see the stand-in-caller
technique in `func_800E4300/STATUS.md` for how to give an IPA callee its
register convention when its caller is missing.

## Pass 3: func_800E6AF8 rewritten by hand

Typed: `D_8014A250_Record` (speed at 0x3F0, `lastTick/tickTime/dt` at 0x710/714/718, vel at 0x788, `s7CA`,
`unk7C6`), `Inp` (76-byte input records, byte 0 = car index), `GC2` (player_array stride 0x3B8 with `w380`,
`b358`, `b359`, `bEF`), `Row3` (`D_80120E74` is `s16[][3]`, `D_80153E84` is `s16`). The goto loops are plain
loops (`for(;;)` re-entry replaces `goto loop_10`).

```
func_800E6AF8  313/334 differ  size 332/334  (was 300/334 differ, size 336)
func_800E56F8  115/359 differ  size 359/359  (unchanged)
```

The instruction sequence now follows the ROM almost everywhere (mnemonic-level diff is a handful of
scheduling/hoisting differences); the word count is dominated by temp-register rotation from the very first
instructions (the ROM hoists `&D_80143FF4` into `$t0` before the `D_801525F0` test; ours does not).
Frame 160 needs `volatile s32 padv[3]` before `fz,fx,fy` and `padw[3]` after `n,k`; `t`/`r` then sit at
sp+112/116 as in the ROM. The float temporaries must be named (`fz,fx,fy`, in that declaration order) for the
`sqrt` sum to load `$f12,$f14,$f2`.

## Round 3: merged with func_800E4300's group; func_800E4B58 and func_800E398C hand-written

This directory is now the merged group for the whole chain
`func_800E6AF8 -> func_800E56F8 -> func_800E4B58 -> {func_800E398C, func_800E451C -> func_800E4300}`.
Members: `func_800E56F8`, `func_800E6AF8`, `func_800E4B58`, `func_800E398C`, `func_800E451C`, `func_800E4300`
(the last two copied from `../func_800E4300/group.c`, whose own group is unchanged and now redundant).
`keep`: `func_800E6AF8`, `func_800E4B58`, `__standin_func_800E56F8`.
`D_8014A250_Record` was extended with the fields all six functions use (0x220 `vx/vy/vz`, 0x634/0x638,
0x720/0x728/0x72C, 0x7CC/0x7DE/0x7E0/0x7EC ...) and `Nav` (0x2C bytes at `player_array[n]+0x314`) gained `spd`
(0x10) and `tgt[3]` (0x14).

```
func_800E56F8   115/359 differ  size 359/359   (unchanged: the callees' clobber sets do not change it)
func_800E6AF8   313/334 differ  size 332/334   (unchanged)
func_800E4B58   528/567 differ  size 533/567   (new, first draft)
func_800E398C   592/599 differ  size 567/599   (new, first draft; IPA parameter `n` in $s7)
func_800E451C   388/397 differ  size 401/397
func_800E4300    80/135 differ  size 135/135
```

Findings:
- **The premise did not hold.** With `func_800E4B58` in the unit the diff of `func_800E56F8` is byte-identical to
  before (115/359, the hoisted `li s6,2`). `func_800E4B58` is an ABI function in the ROM: it takes `car` in `$a0`
  and saves `$s0-$s8` and `$f20-$f30` (frame 304), so it must be in `keep`; not in `keep` IDO makes it IPA
  (param in `$s6`, frame 32) and `func_800E56F8` grows to 366 words with stack spills. `func_800E398C` and
  `func_800E451C` are the IPA functions (`n` in `$s7`; `car=$s5, nav=$s6`), each called only from `func_800E4B58`.
  The blocker for `func_800E56F8` is therefore not the closure.
- `func_800E4B58`: follows the nav cursor along the path (`nav->pt`, `nav->sel`, `a/b/c/d` progress values),
  interpolates target speed `spd` from the points' flag bytes with a lateral-offset damping curve, walks 80 units of
  path to get the look-ahead point `tgt`, calls `func_800E398C(car->unk7C6)`, then derives the throttle/brake
  smoothing floats in `D_80153F68/F48/F28[n]` and `car->f720/f728/f72C`, `car->s7E0`.
- `func_800E398C`: transforms the target into the car frame (`func_800A61B0`), handles the stopped / slow /
  sideways cases, scans the other cars (`D_80152744` of them) for one ahead within 8 units laterally to scale
  `sc` (via `camera_blend_between`), then blends the steering vector `sv` by the path point's type byte
  (`[7]` = 1..5) into `nav->tgt`, `D_80153F88[n]`, `D_80154138[n]`.
- Both are first drafts written from `tdis.py` (`cloud/work/tools/tdis.py func_800E4B58 func_800E398C`); the
  differing-word counts are dominated by frame size (ours 216/288 vs 304/288) and float register naming
  (the ROM's `f20-f30` constants in `func_800E398C` are hoisted; ours match in count but not in order).
