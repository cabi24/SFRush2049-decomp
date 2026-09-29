# func_800D2FA8

**BUILDS** (cloud pass 2026-09-29). Not tuned.

```
func_800D2FA8:       286/290 words differ
split_time_display:   60/60 words differ
```

## Build fixes to the seed

- `*(D_801407FC + (temp_t6 * 0x10)) != 0` dereferenced an `s32`: now
  `*(s8 *) (D_801407FC + ...)`. `D_801407FC` is the base of a 16-byte node
  table (fields at +1 `s8` next index, +2/+6 `u16`, +4 `s8`, +0xA `u16`).
- The call's `M2C_ERROR(unset $t4)`: `split_time_display`'s `ipa_t4`
  parameter is that same node table (it indexes `ipa_t4 + idx*0x10`), so the
  caller passes `D_801407FC` in `$t4`. The call is now
  `split_time_display(sp50, sp4C, ..., arg2, D_801407FC, arg3)`.

`func_800D2FA8` is recursive (it calls itself with `arg5 + 1`) and walks the
node table; `D_80124F88` is a visited list.

## Relocations (for review until F2 lands)

`D_80124F88`, `D_801407FC`, `D_801407F0`, `D_80151CE8`; calls
`func_800D2FA8`, `time_of_day_select`, `minimap_render`,
`split_time_display`.
