# func_800E681C (control input)

**BUILDS** (cloud pass 2026-09-29), hand-written from the assembly; not
matching yet. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
func_800E627C   92/121
func_800E6460  149/239
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

## Known remaining differences

- `func_800E627C`: `x` and `prev` are in `$f0`/`$f2` the other way round,
  and the three-way branch for `steerSrc == 0x19` joins the tail
  differently. Declaration order does not change the FP registers.
- The other two are untuned.

## Relocations (for review; resolved by the strict scorer)

`input_rec0`, `D_8014A250`, `player_array`, `active_player_count`,
`state_word_a`, `D_801170FC`, `D_8013FECB`, `D_8013FED0`, `D_801403C0`,
`D_8014A110`, `D_80140620`, `D_80124498`..`D_801244AC`, `D_80151AD8`,
`D_80140A04`, `D_8013F1D9`, `D_8013F2FC`, `D_80156CF0`.
