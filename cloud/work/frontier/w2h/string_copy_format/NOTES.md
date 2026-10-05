# string_copy_format (0x80092E2C, 436 bytes) - 36/109 positional, one residual

Semantics (names are labels): look an 88-byte record up by name across a range of sorted tables.
`key` is a local 88-byte record; the name is copied in with `func_80092DCC` (strncpy, n=16) or, when the
name is NULL/empty, an inline 11-byte copy of "AAANULLOBJ" (retail keeps the string in .data at 0x80120E68;
`strcpy` intrinsic with a literal reproduces the lwl/lwr sequence, a struct copy from an extern gives aligned `lw`).
Then the same clamp/loop/bsearch as the matched sibling `func_800B24EC` (cloud/matches), over
`{Rec *base; u32 count;} D_801161F4[]`, element size 88, comparator `validate_and_call` (0x80095120).
Returns `(found - base) | (i << 10)` or -1. Fourth parameter is narrow (homed, never read); callers pass 1.

## Residual (the only one)
Retail keeps `name` in a0 for the two tests, copies it to **t0** (`move t0,a0` as the last prologue word, no a0
home) and passes t0 to strncpy. a3 is homed (`sw a3,172(sp)`) but never overwritten.
- `b.c` (plain source): uopt moves `name` to a3 and homes a0 and a3 (aligned 31 rows, size +1).
- `best.c` (`p = name; name = key.name;`): an un-merged copy at the right place, no a0 home, same length, but the
  copy is coloured a3 instead of t0; everything else is the one-step temp-ring shift that follows (36 words).
Whenever a3 is made unavailable (4th parameter live, 4-argument call, name live across the call) uopt picks s0,
never t0. The matched `menu_load_options` shows uopt does use t0/t1 for parameters when a0-a3 are taken and *no
callee-saved register is in use yet*; here s0-s5 are all in use.

## Tried (about 110 variants, no movement on the register)
4th parameter type (s8/u8/s16/s32/pointer/absent) and dead uses of it in every block; K&R definition; name as
s32/u32/void*/Rec*; local copies in every test/call combination; inlined helper (group, not kept); internal with
stand-in callers (completely different code: it is a kept ABI function); `blob_unit score` in the real unit
(same words as standalone); -O2 (same); if/else forms (goto, nested, ternary, swapped); found initialiser position;
4-argument strncpy call; struct-copy default.

## Next hypothesis
The colour is t0 only if both a3 and every already-paid callee-saved register (s0 = i, s3 = table base) are
unavailable to the name web, or the name web is coloured before the loop webs. Needs the instrumented-uopt
colouring trace (workbench `instrument`) on best.c to read the forbidden set of the copy web.

## Re-check (w2h takeover, 2026-10-05)
Rescored after refreshing the builder copy: `score.py fn` -> `36/109 words differ (2 section-relative
relocations unverified ...)`; in the unit `FAIL string_copy_format: 36 of 109 words differ`.  Unchanged; no
further variants (already ~110 with no movement).
