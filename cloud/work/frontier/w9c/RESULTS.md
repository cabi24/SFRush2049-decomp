# w9c results

| function | state | flags | scorer |
|---|---|---|---|
| func_8008E408 | 22 words off (spill-slot offsets only; registers, schedule, locals, frame exact). Not a match. | -g0 -O3 -mips2 -G 0 -non_shared (whole-program unit) | `python3 -m tools.conveyor.pipeline.blob_unit --tag w9c --jobs 2 score func_8008E408 --with cloud/work/frontier/w9c/func_8008E408/best.c` -> `FAIL func_8008E408: 22 of 386 words differ` / `blob_unit score: 0/1 equal` |
| func_8008E0B8, func_8008E26C, func_8008E3C0, math_utility, vector_normalize_length | already locked; used unchanged as unit context | - | - |

Files: `func_8008E408/best.c` (header documents quirks), `func_8008E408/notes.md`
(residual, what was tried, allocator trace, structs), `func_8008E408/steps/` (intermediate
drafts), `tools/` (sc.sh / run.sh unit-score wrappers, udiff.sh, ctrace.sh = w3a's traced uopt
adapted to tag w9c; builder copy of the traced uopt in ~/rush2049/scratch/frontier/w9c/uopt).
No group directory: no group was needed, because every callee is locked and the body is
scored in the unit.
The standalone `score.py fn` result does not mean anything here: alone the body compiles to a
144-byte frame with a different shape. Only the unit score counts.

## Generalisable
- A function with *no* callee-saved registers and everything spilled to homes is a sign of
  compiled-out debug checks: an empty-bodied `if (x) DEBUG_PRINT(...)` lowers priorities
  enough that `o` leaves s0. A dead `lw global; sw N(sp)` at entry is `local = GLOBAL;` used
  only in such a check.
- `x * 1.0f` survives only through an inlined helper parameter (`Random(1.0f)`, func_8008B2E4).
- A `case 3:` that shares a `li t0,3` with a later loop bound means the switch operand is
  signed. `u32` gives retail's `li at,3`.
- Allocator ties (equal save) go to the lower web number. A debug check that adds one live
  block with no use flips them.
- Local offsets can be fitted exactly with padding locals, but the ugen spill-temp area
  (24..) is a separate count that local padding cannot move.
