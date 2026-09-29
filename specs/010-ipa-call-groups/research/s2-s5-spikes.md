# Spikes S2-S5 (2026-09-29)

Follow-ups to [S1](s1-poc.md). Group: `func_80097694`, `resource_slot_clear`,
`resource_slots_clear_multiple`. Tools are in [tools/](tools/) (`wplink.sh`,
`romcmp.py`, `grouplink.py`), and the source files are in [s3-kp/](s3-kp/).

## S3 — how the original was built: whole-program `uld -kp` (RESOLVED)

- **Layout fingerprint:** callees sit at lower addresses than their callers
  for **243/243** IPA call edges and **1,883/1,895 (99.4%)** of all other call
  edges. That is IDO -O3 bottom-up emission order across the entire image.
  The game code was optimized as **one whole-program unit**.
- **Mechanism:** `cc -O3 -c a.c b.c` links ucode with `uld -preserve_dead_code`,
  which keeps every global external, so no IPA happens across files (S1's failed
  two-file attempt). Running the same stages by hand (`tools/wplink.sh`,
  byte-identical to `cc` for the baseline) with **`uld -kp <file>`** instead,
  where the file lists the procedures that stay external, makes every other
  global internal to the program. The two-file build (`s3-kp/k_a.c` +
  `s3-kp/k_b.c`, `func_80097694` an ordinary global in its own file, keep list
  `s3-kp/keep.txt`) then matches the ROM for all three functions.
  **No unity build is needed: sources can keep a normal file structure.**
- Other options tried, with no effect: `-hides`, `-hidden_symbol`,
  `-exported_symbol`, `-e`; dropping `-preserve_dead_code` without `-kp` emptied
  every body (nothing was reachable).

## S2 — sensitivity to neighbours (FAVOURABLE, one group)

- The stand-in callers of the leaf were replaced twice: once by a caller keeping
  five values live across two calls, once by two callers that only pass
  constants. **All three members still match the ROM** each time. So member code
  did not depend on the stand-ins' bodies, only on their existence, which keeps
  the leaf from being inlined.
- The members' external callees (`audio_frame_sync`, `display_list_alloc`) were
  left undefined/external and the members still matched, because those calls
  happen after the IPA-carried value's last use. That is not true in general: a
  group whose IPA value lives across a call to an external function needs that
  callee inside the unit.
- **Implication:** a group = the IPA members + every function they call while an
  IPA value is live + at least the caller shape needed to prevent inlining.
  Unknown callers can be stand-ins.

## S4 — placing group members at their own addresses (RESOLVED)

`tools/grouplink.py` takes the whole-program object and applies its `.text`
relocations per slice:
- R_MIPS_26 targets inside the object map to the member slice's image address;
- HI16/LO16 pairs are applied REL-style;
- externals resolve through `blob_splice.image_symbols`.

Result: **all three relocated bodies are byte-identical to `build/game_code.bin`
(no masking)**, and `blob_splice.build_with(existing 121 bodies + these 3)`
**passes the image gate** (sha256 bf7da3fa…). The lock and the region assembly
were not changed (rebuilt from the lock afterwards; the tree is clean).
The ROM SHA-1 was not run: that needs group support in the splice lock
(Phase 4). Not yet handled: relocations in `.rodata`/`.data`, such as jump
tables (R_MIPS_32), and GP-relative relocations (the game uses `-G 0`, so
none are expected).

## S5 — seeds for group members (FEASIBLE, needs an m2c extension)

Current m2c output for the group:
- `resource_slot_clear`: correct apart from `M2C_ERROR(unset $t0)` where the
  parameter belongs;
- `resource_slots_clear_multiple`: calls `resource_slot_clear()` with **no
  arguments**, because the `li $t0,N` in the delay slots is invisible to m2c;
- `func_80097694`: the 4x-unrolled loop comes out literally (a normal
  permuter/hand job; unrelated to IPA).

Deterministic fix, using the Phase 0 detector's data: a per-function
*register-parameter map* (for example `resource_slot_clear: [t0]`) given to m2c
so that (a) the callee gets a parameter per live-in register, and (b) calls to
that function take their arguments from those registers. Group TUs then
declare ordinary C parameters, and `-O3` IPA reassigns the registers itself,
as S1 showed.

## Conclusions for the spec

1. Build model: **separate C files, whole-program -O3, `uld -kp keep-list`**,
   staged by hand (`wplink.sh`), not the `cc` driver's link step.
2. Matching unit: an IPA call group plus the callees live across IPA values,
   with stand-ins allowed for unknown callers.
3. Splice: per-slice relocation (`grouplink.py`) feeding the existing image gate.
   The lock needs a group record (members, sources, keep list, flags).
4. Seeds: m2c register-parameter map (S5), driven by `build/ipa_members.json`.
5. Still open: how group size scales (the 125-caller `state_utility`
   cluster), the emission-order rule (not needed for splicing), and data-section
   relocations.
