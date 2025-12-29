# Completed: Homework 003

## Summary
Cataloged arcade sound effect IDs and grouped them by category, with initial N64 call mapping for key IDs.

## Findings
- Primary IDs and descriptions come from `reference/repos/rushtherock/game/sounds.h`.
- Engine audio uses APM engine IDs (`S_AUDREY`, `S_NSX`, `S_GHR`, `S_TPM`) with rpm/load; see `sounds.c`.
- Car SFX (skid/road/wind) are managed in `carsnd.c` state structures, with pitch/volume scaling.
- Ambient/radio stations are managed via lists in `sounds.c` (`demo_radio_on/off`).
- N64 `voice_play` IDs align with arcade voice samples like `S_FINALL` and `S_CHKPNTSTATIC`.

## Files Created/Modified
- `docs/audio_sfx_catalog.md`
- `codex_homework/completed/completed_003.md`

## Notes for Claude
- N64 `sound_play` IDs appear to be local indices; the mapping to arcade IDs is inferred from comments and likely needs validation.
- If you need exact N64-to-arcade ID mapping, we may need to inspect N64 audio tables or asset names.

## Time Spent
~1.25 hours
