# func_800E681C (control input)

**1 of 3 members MATCH** (`func_800E627C`, 121 of 539 words), rescored
2026-09-30 (`zbuild.py --as1=-r4300_mul`). Hand-written from the assembly. Not spliced.

| function | target words | result |
|---|---|---|
| `func_800E627C` | 121 | **MATCH** |
| `func_800E6460` | 239 | 32 differ (size 239/239) |
| `func_800E681C` (`keep`) | 179 | 173 differ (emits 178) |

Closure: no gap known (seed's unset `$t0..$t5/$a2/$a3` were values
`func_800E681C` keeps in caller-saved registers across its two calls; written
as ordinary C, IDO recreates them).

## What this is

Per-player control input into each car's state record (`D_8014A250`, 0x808
bytes). `func_800E627C` steering (0x720: analog curve, quantized to 1/127,
dead-band against previous value, mirror flag); `func_800E6460`
throttle/brake (0x72C/0x728, analog, pad bytes or buttons, quantized to 1/15);
`func_800E681C` the per-player loop plus gear handling (0x730: reverse, up/down
with clamps, automatic pick from speed when negative) and button bytes
0x731/0x732. `input_rec0` is an array of 0x4C-byte input records.

## Techniques that worked

- `func_800E627C`: no named `t` (a named local is a uopt web and rotates the FP
  registers): `x = (f32) (s32) (q < 0.0f ? q - 0.5f : q + 0.5f) / 127;`. The
  steering offset `f32 c = D_8012449C;` is declared **inside** the
  `x < D_80124498` arm (at function scope its load is hoisted above the compare).
- `func_800E6460`: throttle and brake fields (0x728/0x72C) are **`volatile f32`**
  in `CarState`; the target does not CSE them (three `lwc1` + `mul.s` per field)
  and volatile reproduces that (size 231 -> 239, 149 -> 32 differing). Rounding
  is written with the field repeated (`st->throttle * 15.0f < 0.0f ? ...`),
  division by int constant `15` (reloads `15.0`).

## Remaining blockers

- `func_800E6460` (32 words): in the target `in->reverse & D_8013FED0[in->pad]`
  loads `reverse` (`lw t6,44(a1)`) between the `lbu` and the `sll` with operands
  `(reverse, array)`; ours loads the array first, operands `(array, reverse)`.
  Source operand order, operand types, `volatile InputRecord`, casts and extra
  earlier uses have no effect. Same for the `throttleSrc` and `brakeSrc` chains;
  the rest is t-register rotation.
- `func_800E681C`: target multiplies `car * 0x3B8` with shifts
  (`sll 4; subu; sll 3; subu; sll 3`) and rebuilds `&player_array` in the loop;
  ours hoists `952` into `$a3` and uses `multu` (`player_array + st->car`, byte
  pointers, `sizeof(Car)`, `(u32)`/`(u16)` index all keep `li a3,952`). Target `$a2` is the constant 2 and `$a3` a pointer; frame
  32 vs our 40 (we use `s0..s3`, target `s0`,`s1`). With `--st->gear <= 0` the
  target reuses the sign-extended value (`bgtz v0`), ours reloads.

## Relocations (resolved by the strict scorer)

`input_rec0`, `D_8014A250`, `player_array`, `active_player_count`,
`state_word_a`, `D_801170FC`, `D_8013FECB`, `D_8013FED0`, `D_801403C0`,
`D_8014A110`, `D_80140620`, `D_80124498`..`D_801244AC`, `D_80151AD8`,
`D_80140A04`, `D_8013F1D9`, `D_8013F2FC`, `D_80156CF0`.
