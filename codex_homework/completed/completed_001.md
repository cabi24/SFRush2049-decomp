# Completed: Homework 001

## Summary
Re-ran the top-50 D_80 global scan against the current `src/game/game.c`, analyzed the top 30 (all 4 refs), and produced `docs/globals_remaining_analysis.md` with proposed names, evidence, confidence, arcade cross-references, and a next-batch rename list.

## Findings
- Most of the top 30 globals are UI/menu state or animation frame counters (title, menu, results, trophy, achievements).
- Audio listener vectors (`D_801601A4`, `D_80160198`) are well-scoped for rename.
- A few globals are dual-purpose or ambiguous:
  - `D_80159E04` is used as a stunt-menu selection and a debug collision toggle.
  - `D_80159A14` is used both for finish-time recording and as a ghost-mode flag in time attack.

## Files Created/Modified
- `docs/globals_remaining_analysis.md`

## Notes for Claude
- The current top-50 list all have 4 references, so the “High Priority (6+ refs)” section is empty; Medium Priority covers 30+ entries.
- Consider splitting `D_80159A14` into two concepts once the ghost/finish-time ownership is confirmed.
- If you want me to expand beyond the top 30, I can continue with the next 20 in the top-50 list.

## Time Spent
~1.5 hours
