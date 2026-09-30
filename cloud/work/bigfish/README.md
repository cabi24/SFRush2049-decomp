# bigfish: scouting large game functions for a full-match "hail mary"

Scouted 2026-09-30 (round 3). One note per function: `object_render.md`,
`render_large_objects.md`, `entity_spawn_init.md`, `render_display_list.md`,
`func_8009F058.md`.

Tools in `tools/`: `dis2s.py` (tdis output -> m2c-readable .s), `align.py`
(aligned closeness of a compiled function against the retail words),
`repetition.py` (how repetitive a function is). `seeds/` holds the compiling
(hand-patched) m2c seeds and the two small hand chunks.

Metrics used by `align.py` (all LCS, so insertions/deletions do not shift
everything): opcode-shape (opcode/funct only), opcode+regs (registers kept,
immediates ignored), exact word. Relocation fields are zero in the .o, so
`exact` undercounts lui/lw/jal words slightly. These are leads, not matches.

Method to spot IPA dependence: (a) a register read before written that is not
an ABI arg (s0-s8, t0-t3, f16+), (b) s-registers or f20+ used without a
prologue save. Both are true of `render_display_list` and `func_8009F058`.
