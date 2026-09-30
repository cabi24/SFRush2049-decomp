# func_800E681C (control input)

**BUILDS** (cloud pass 2026-09-29), hand-written from the assembly; not
matching yet. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
func_800E627C   MATCH    (cloud pass 4; was 15/121)
func_800E6460   32/239   (cloud pass 4; was 149/239)
func_800E681C  173/179
```

## What this is

Per-player control input into each car's state record (`D_8014A250`,
0x808 bytes): `func_800E627C` steering (0x720, analog curve then
quantized to 1/127, with a dead-band against the previous value and a
mirror flag), `func_800E6460` throttle/brake (0x72C/0x728, analog,
pad bytes or buttons, quantized to 1/15), `func_800E681C` the per-player
loop plus gear handling (0x730: reverse, up/down with clamps, automatic
pick from speed when negative) and two button bytes (0x731, 0x732).
`input_rec0` is an array of 0x4C-byte input records.

The seed's `M2C_ERROR(unset $t0/$t1/$t2/$t3/$t5/$a2/$a3)` were values
`func_800E681C` keeps in caller-saved registers across its two calls:
IPA knows the leaves do not clobber them. Written as ordinary C, IDO
recreates that itself.

## Cloud pass 4

- `func_800E627C` **MATCH**. Two changes: (1) no named `t`: the rounding is
  `x = (f32) (s32) (q < 0.0f ? q - 0.5f : q + 0.5f) / 127;` (a named `t` is a uopt web
  and rotates `t/r/d` in the FP registers; removing it fixed all of that); (2) the
  steering offset local `f32 c = D_8012449C;` is declared **inside** the `x < D_80124498`
  arm (at function scope its load is hoisted above the compare).
- `func_800E6460` 149 -> 32: the throttle and brake fields are **`volatile f32`** in
  `CarState` (0x728/0x72C). The target does not CSE them (three `lwc1` + `mul.s` per
  field, no reload-forwarding), and volatile reproduces exactly that (size 231 -> 239).
  The rounding is written with the field repeated (`st->throttle * 15.0f < 0.0f ? ...`),
  divisions by the int constant `15` (reloads `15.0` like the target); a `q` local makes no difference. FP register naming of `0.0/1.0/255.0` now matches.
  Left (32 words): in the target the `and` for `in->reverse & D_8013FED0[in->pad]`
  loads `reverse` (`lw t6,44(a1)`) between the `lbu` and the `sll` and has operands
  `(reverse, array)`; ours loads the array first and has `(array, reverse)`. Operand
  order in the source has no effect (IDO canonicalises), nor do types of either operand,
  `volatile InputRecord`, casts, or extra earlier uses. The same holds for the
  `throttleSrc` and `brakeSrc` chains. Everything after is a t-register rotation.
- `func_800E681C` unchanged. With `st->gear` as `--st->gear <= 0` the target reuses the
  sign-extended value (`sll/sra`, `bgtz v0`) and ours reloads; neither the `assign` nor
  `+= 1` forms change it. The 952 multiply: forms `player_array + st->car`, byte
  pointers, `sizeof(Car)`, `(u32)`/`(u16)` index all keep `li a3,952` hoisted.

## Known remaining differences (older notes, superseded above for 627C/6460)

- `func_800E681C`: the target multiplies `car * 0x3B8` with shifts
  (`sll 4; subu; sll 3; subu; sll 3`) and rebuilds `&player_array` inside the
  loop; ours hoists `952` into `$a3` and uses `multu`. The target's `$a2` is
  the constant 2 and `$a3` a pointer, so the constant/pointer register
  assignment (IPA) differs; the frame is 32 bytes against our 40 (we use
  `s0..s3`, target `s0`,`s1`). Untuned.

## Relocations (for review; resolved by the strict scorer)

`input_rec0`, `D_8014A250`, `player_array`, `active_player_count`,
`state_word_a`, `D_801170FC`, `D_8013FECB`, `D_8013FED0`, `D_801403C0`,
`D_8014A110`, `D_80140620`, `D_80124498`..`D_801244AC`, `D_80151AD8`,
`D_80140A04`, `D_8013F1D9`, `D_8013F2FC`, `D_80156CF0`.
