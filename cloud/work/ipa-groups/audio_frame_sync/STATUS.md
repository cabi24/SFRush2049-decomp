# audio_frame_sync (asset loader, not audio)

**BUILDS** (cloud pass 2026-09-29) with minimal fixes to the m2c seed; not
matching. With `-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
audio_frame_sync    123/130
brake_force_apply    14/49
fp_call_wrapper       2/26
func_80096CA8       295/301
func_80097164        49/86
suspension_setup     59/60
```

## What this is

Loads a resource block and fixes up its internal pointers: chunks are
found by FourCC (`OBHD`, `TXHD`, `PLHD` via `lookup_with_output`), then
offsets in object (0x58-byte), texture and palette records are relocated
against either the loaded copy or the source base.

## Build fixes (minimal; the body is still the m2c seed)

- Three pointer-plus-pointer relocations (`var_a3 + field`) now add the
  field as `u32` to a `u8 *` base.
- `sp50` (read after `func_80096C28(&sp64, ...)`) is declared; it is filled
  through `&sp64`, which the seed does not model.
- `func_80096CA8` takes two IPA parameters, `x` in `$s8` and a flag in
  `$t0`: `fp_call_wrapper` passes `(arg0, 0)` and `func_80097164` passes
  `(value, 1)`. The seed had a spurious third parameter and scrambled the
  call sites. In the seed body `x` is almost unused, so IDO still puts the
  flag in `$s8`; that resolves only with a real rewrite of
  `func_80096CA8`.
- Remaining `M2C_ERROR(unset $t0/$t1)` in `func_80096CA8` are compiled as
  written and are wrong: they are the flag and an index held in
  caller-saved registers.

## Relocations

Seed-level only; not reviewed.
