# audio_frame_update

**BUILDS** (cloud pass 2026-09-29). Not tuned.

```
audio_frame_update:  147/150 words differ
func_800B08FC:        84/99 words differ
func_800B0A88:       101/112 words differ
```

## Build fixes to the seed

- `arg0 = (s32) ipa_t0; arg1 = ipa_t3;` (in `func_800B08FC`) and
  `unksp48 = ipa_s1; unksp4C = ipa_s2;` (in `func_800B0A88`) were m2c's view
  of the callee storing its IPA register parameters to stack home slots.
  Removed; if the target really spills them, the original probably took their
  address or used them after a call.
- `unksp38/3C/40` in `func_800B0A88` are one `f32 vec[3]` at `sp+0x38`,
  passed as `func_8008D6FC(idx, vec, NULL)`. Now a local array.

## Relocations (for review until F2 lands)

Seed globals unchanged (`gameplay_mode`, `D_8014A250`, `D_80152AEC`,
`D_80152ABC`, `player_array`, `D_801427C0`, `D_80111299`, `D_8012E708`,
`D_80123C1C`, `D_8011B4B8`, `D_80139320`, `D_801543CC`); calls
`save_slot_valid`, `model_data_load`, `func_8008D6FC`, `func_80092484`.
