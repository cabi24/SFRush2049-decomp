# func_800E4300

**BUILDS** (cloud pass 2026-09-29) with minimal fixes to the m2c seed; not
matching. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
func_800E4300  135/135   (size 293/135, 149 extra words)
func_800E451C  393/397   (size 430/397, 29 extra words)
```

## What this is

`func_800E4300` finds the path point nearest to a position: it scans a
window of `2*max(|lag|,5)+1` points around an index, wrapping at the
path's length, and returns the index with the smallest squared distance.
`func_800E451C` is its only caller; it converts the car position at
`ipa_s5+0x794` to three `s16`s on the stack and calls it once per loop
iteration.

## Build fixes

- **Parameter order.** The seed ordered the IPA parameters by register,
  which left `arg3`/`arg4` undefined and scrambled the call. The home-slot
  stores in the prologue give the real order: `sp+0,4,8,12,16` hold
  `$a2, $t2, $t5, $a1, $t0`. So the signature is
  `s16 func_800E4300(s16 *pos, s16 i28, s16 i24, s16 player, s16 i)`, and
  the caller passes `(&sp60, s6->0x28, s6->0x24, s5->0x7E2, s3)`. This
  matches the caller's `jal` setup (`a2=s8`, `t2`/`t5` from `0x28`/`0x24`
  of `$s6`, `a1` from `0x7E2` of `$s5`, `t0` = the loop counter). All five
  parameters are `s16` except `pos` (`sll`/`sra` after each home store).
- `sp60`/`sp62`/`sp64` became `s16 sp60[3]`, since a pointer to them is
  passed.
- `fabsf`/`sqrtf` are declared as IDO intrinsics (the seed left them
  undeclared).

## Remaining

- `func_800E4300` compiles to more than twice its size. The seed's counted
  `do { ... } while (var_a3 != temp_t1)` loop is probably unrolled by `-O3`,
  and the original is a plain `for` loop over the window. Rewrite it by
  hand before scoring the rest.
- Closure: `func_800E4B58` and `func_800E398C` are part of this cluster and
  link it to `func_800E56F8`. They are not in the unit.

## Relocations

Seed-level only; not reviewed.
