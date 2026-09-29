# func_800E681C (control input)

**BUILDS** (cloud pass 2026-09-29), hand-written from the assembly; not
matching yet. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
func_800E627C   15/121   (was 92/121; cloud pass 3)
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

- `func_800E627C` (**15/121**, all but the relocation-masked words are FP
  register naming). What fixed 77 words:
  - `x = 0.0;` (a **double** literal; also `(f32) 0`) instead of `x = 0.0f;`:
    with `0.0f` uopt copies the shared zero constant (`mov.s`), with the
    double it emits the target's separate `mtc1 zero,$f2` and the extra
    words (size 117 -> 121);
  - `/ 127` (int constant) instead of `/ 127.0f` reloads the 127.0 constant
    like the target instead of CSE'ing it, and also swaps `x`/`prev` into
    `$f2`/`$f0` (the swap the old note asked about);
  - the dead-band as `if (A < d) prev = r; else if (d < B) prev = r;` (two
    assignments, not `||`);
  - `D_80151AD8` is `s8` (`lb`, not `lbu`);
  - a block-local `f32 c = D_8012449C; x += c;` for the steering offset moves
    that load into `$f12` like the target (22 -> 15 words).
  Left: the target has `t = q +- 0.5` in `$f14`, `r` in `$f2` (reusing `x`'s
  register) and `d` in `$f12`; we get `t` `$f2`, `r` `$f12`, `d` `$f14`
  (a rotation). It reads as if `x` were still live when `t` is defined. Variable
  reuse (`x`/`q` in place of `t`/`r`), an `int n = (s32) t`, `q +- h` with a
  phi'd `h`, ternary and local copies of the other constants did not help
  (the register choice is per web, not per name).
- `func_800E6460`: the target does **not** CSE `st->throttle`/`st->brake`
  across the `if (q < 0)` arms (three separate `lwc1`+`mul.s 15.0f`, the tails
  of the source chain fall through into the rounding code with no reload),
  and holds `0.0`/`1.0`/`255.0` in `$f2`/`$f0`/`$f12` (ours `$f12`/`$f2`/`$f14`).
  We emit 231 words against 239: our chain ends each arm with `b; lwc1 $f0,...`
  (one hoisted load). Rewriting the rounding with the field re-read in each arm
  and as a ternary does not change the word count (142-149/239).
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
