# camera_play_script (0x800C5644)

Feasibility: **low / blocked (about 3 %)**. Effort: 2+ weeks, needs the shared IPA cluster.

## Facts
- **880 words**, frame 608 (largest frame), saves ra, s0-s8, f20-f30. Not spliced.
- Callers: itself (recursion, 2 `jal`) and `camera_victory` (0x800C66DC: `a0=s4, a1=s0, a2=sp+204`,
  ABI). Entry `(a0,a1,a2)` is ABI; prologue reads no non-ABI registers.
- 15 `jal`: `func_800A61B0` x3 (matched in `cloud/matches`), `math_utility` x2, `func_800C54F0` x2
  (locked), `func_800C36A0` x2 (**268 w, reads `s1`,`s2` as IPA inputs**: the caller sets
  `addiu a0,sp,280; addiu a1,s2,4` and keeps `s2`), `func_800AD650`/`func_800AD5D0` (the
  `func_800AD4C8` cluster; `AD5D0` already has a strict match, `AD650` is hand-written there),
  `entity_flags_apply`. 248 FP instructions, 63 branches (26 likely), 14 mult/div (the 8-byte `PV`
  table `D_8015201C` seen in the cluster STATUS notes).
- `func_800C36A0` is also called by `camera_victory`: same cluster as `input_deadzone_apply`
  (`func_800AD4C8` group closure gaps list `camera_play_script` and `func_800C36A0` by name).
- Name is a label; behaviour is a scripted camera path/collision walk.

## First pass
- m2c seed compiles after 2 mechanical fixes (a `f32`/`f32 *` shared-slot confusion at `sp94`):
  964 words vs 880; **opcode-aligned 57.7 %**, opcode+regs 22.3 %, exact-word LCS 99 (the highest exact
  count, from float constant loads).

## Recommended approach
- Blocked behind the `func_800AD4C8` cluster (`func_800C36A0`, `camera_victory`,
  `camera_trigger_check`, `entity_update`, `input_process_controller`, `func_800C3AD0`). Not a
  hail-mary candidate.
