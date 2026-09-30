# bigfish scout B (net_session_update, stunt_combo_display, func_800D91A0, input_deadzone_apply, camera_play_script)

One file per function; ranking table is in the agent report. `tools/` has the helpers used
(paths inside reference SCRATCH; adjust): `mkasm.py` (retail words -> labelled asm for m2c, with %hi/%lo),
`seedgen.py` (runs conveyor `m2c_seed`), `autofix.py` (mechanical cfe error fixes), `close.py`
(instruction-aligned closeness: opcode-only and opcode+regs LCS, `PREFIX=n` env for partial bodies),
`xcall.py` (caller-save registers read after calls: IPA smell test).
`net_session_update_partial.c` is the hand skeleton of the first four phases.
