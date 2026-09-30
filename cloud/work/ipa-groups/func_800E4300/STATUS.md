# func_800E4300 (nearest path point to a position) -- cloud pass 2, 2026-09-29

## Current status (Rescored 2026-09-30 (Round 2 addendum), master d0891f3.)

**Builds; no member matches (0/532 words).** Scored with `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/func_800E4300 --as1=-r4300_mul`:

```
func_800E4300  80/135 words differ  size 135/135
func_800E451C 388/397 words differ  size 401/397 (3 extra words nonzero beyond target)
```

Both members are now hand-written (`func_800E451C` was rewritten in Pass 3; the "still the m2c seed" remarks
below are history). Blockers: `func_800E4300` register naming only (loop invariants take `$t3..$t5/$s0/$s1` in a
different order); `func_800E451C` IPA parameter registers (ROM `car=$s5, nav=$s6`, ours `$s4/$s5`), frame 232
reached only with `volatile` padding.
Closure gaps (open): `func_800E4B58` (567 words, the only caller of `func_800E451C`, stand-ins used) and
`func_800E398C` (599 words); both link to `func_800E56F8`'s group, consider one merged group.

Builds. With `-r4300_mul` (`cloud/work/tools/zbuild.py --as1=-r4300_mul`):

```
func_800E4300  80/135 words differ   size 135/135   (was 135/135 differ, size 293)
func_800E451C  392/397 words differ  size 396/397   (was 393/397, size 430)   [with stand-ins, see below]
```

The callee now has the ROM's **size and instruction sequence**; only register
naming differs (see "Blockers"). `func_800E451C` is still the m2c seed body.

## What the code is

`func_800E4300(pos, A, B, player, C)` finds the path point nearest to `pos` on
track `C`: it takes a window of `2*max(|lag|,5)+1` points centred on point
`B + d` (wrapping at the track's point count), where `d` is the difference of
two section start points and `lag` is the same difference measured the other
way round the loop, and returns the index of the point with the smallest squared
distance. Types (in `group.c`): `Section` (0x50 bytes; `last` at +2 and `count`
at +8 valid in element 0, `start[16]` at +0x30 -- the same array as
`func_800B9B64`'s `D_80151CE8`), `Track` (8 bytes: `u16 numPoints; TrackPt
*points`, array `D_8012E5E8`), `TrackPt` (8 bytes: `s16 x,y,z; u8 flag`).
The seed's `D_8012E668` was an m2c mis-read: that word is not used; the
"other player" branch reads `ent[1].start[...]` (offset 0x80 from the section
base).

## What made the size match

- **Plain loop instead of the seed's counted `do/while`.** `for (k = 0; k <
  cnt; k++)` (any natural spelling) is unrolled twice by `-O3` (293 words
  instead of 135). Written as
  `k = 0; if (cnt > 0) for (;;) { ...; if (++k == cnt) break; }` the unroller
  leaves it alone and LFTR still turns the test into `bne`. (Other things that
  also stop the unroll but give worse code: an `s16` loop counter, a `while
  (k++ < cnt)`, a separate `s32` copy of the bound.)
- Index `idx` and the wrap must be `s32` (no `sll/sra` per step), the point
  count `u16` (so `n - 1` is hoisted as `addiu s1,v0,-1`), `win`/`cnt`/`diff`
  `s16`.
- Address arithmetic through **arrays of structs** (`D_80151CE8[player]`); an
  explicit `player * 0x50` compiles to `li 80; multu` instead of the shifts.
- The IPA parameters are ordered by home slot: `(pos, A, B, player, C)`.

## Stand-ins for a missing caller (new)

`func_800E451C` in the ROM is an IPA function: it takes `$s5/$s6` as
parameters and never saves `$s0-$s8`. Its only caller (`func_800E4B58`, 567
words) is not in the unit, and while `func_800E451C` was in `keep` IDO
compiled it as an ABI root (426 words, saves everything). It is now **not**
in `keep` and has two stand-in call sites (`__standin_func_800E451C_a/_b`):
two call sites stop the inliner and IDO gives it the IPA convention (size
430 -> 396 of 397). Use this for any group whose real callers are missing; it
cannot substitute for a missing *callee* (its clobber set is unknown).

## Blockers

- `func_800E4300` registers (pure naming): the ROM assigns the loop invariants
  in body order (`points` `$t3`, `pos[0..2]` `$t4,$t5,$s0`, `n-1` `$s1`) and
  keeps the loop bound in `$t1` (the register `win` had); ours hoists `points`
  last and assigns `pos[0..2]` `$t3..$t5`, `n-1` `$s0`, `points` `$s1`, bound
  `$s0`; the parameter `B` lands in `$s0` instead of `$t5`. Loop-body
  statement orders, the wrap as `?:`, `dist` operand orders, named pointer
  locals for the two section entries, integer widths (one gives 78/135), and a
  few hundred decomp-permuter mutations did not change it.
- `func_800E451C` (397 words) is still the seed: named scalar locals each take
  a stack slot (see `func_800D2FA8`), so its ~50 `temp_*` locals inflate the
  frame (256 vs 232); it needs a hand rewrite with the ROM's slot layout
  (`sp3C..spE4` in the seed's names) before its registers can be compared.
  Its callee `func_800E4B58`/`func_800E398C` cluster (link to
  `func_800E56F8`) is still missing from the unit.

## Pass 3: func_800E451C rewritten by hand

Typed (`D_8014A250_Record` = the 0x808-byte car with `vel`, `pos[3]`, `unk7C6`, `unk7CA`, `unk7E2`;
`Nav` = the 0x2C-byte per-car path cursor: `a,b,c,d` outputs, `tm`, `pt`, `flag`, `sel`, `tk`; `Track`/`TrackPt`
from `func_800E4300`). The 50 `temp_*` scalars are gone; locals are `best/bestScore/cx/cz/dir[3]/tm/pp[3]/v[2]/tot`.
`(f32)(u32)flag` gives the `bgez`/0x4F800000 fix-up; `func_800B9338` next-point calls kept.

```
func_800E451C  388/397 differ  size 401/397  frame 232 (was 392/397, size 396, frame 256 with real callers)
```

Frame 232 comes from `volatile s32 padv[12]` declared before `bestScore`. The ROM's locals: spE4 (228)
`best`, spBC (188) `bestScore`, spB0/spAC (176/172) `cx`/`cz`, sp74..7C `dir[3]`, sp6C `tm`, sp60 `pp[3]`,
sp38/3C `v[2]`, sp30 `tot`; the ROM also reserves about 35 unused 4-byte slots between them (declaration-order
holes of the original named locals), so exact slot addresses cannot be reproduced without reproducing that
declaration list. Not matched: the IPA parameter registers (ROM `car=$s5, nav=$s6`, ours `$s4/$s5`; the ROM
strength-reduces the track walk into `$s4`, ours does not even with an explicit `Track *tr; tr++`).
