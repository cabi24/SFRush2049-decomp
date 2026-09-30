# input_deadzone_apply (0x800ADD58)

Feasibility: **low / blocked (about 3 %)**. Effort: 2+ weeks and needs maintainer closure work.

## Facts
- **895 words**, frame 400, saves ra, s0-s8, f20-f30. Not spliced.
- **The name is wrong** (`docs/arcade_n64_function_map.md` says "Low" confidence). Body: calls
  `handbrake_apply` 14 times, `steering_sensitivity`, `traction_control`, `input_process_controller`,
  `func_8008E0B8`, `func_800ADCE0`, `math_utility`, and `func_800ACA9C` variants (the m2c seed's arity
  errors, 13 sites). 286 FP instructions, no mult/div, 99 branches (33 likely), 5 `lui` bases.
- **ABI at the entry** (`a0..a3` plus stack args at 16/20(sp), and a float in the constant-pool
  register: callers `camera_position_update` (0x800C69F4: `lui a3,0x3f00` = 0.5f) and `entity_iterate`
  set a0-a3 and 16/20(sp)); a `v0` return.
- **IPA-entangled:** it keeps `t0` live across 14 calls to `handbrake_apply` (54 w), `t7`/`t5`
  across others, and calls `input_process_controller` (363 w), which is the known unmatched
  `func_800AD4C8` cluster (`cloud/work/ipa-groups/func_800AD4C8/STATUS.md`: two passes, still 340/363
  words differ because of a missing `func_800AD650` clobber set). Callers `camera_trigger_check`,
  `camera_victory`, `entity_update` share `handbrake_apply` / `steering_sensitivity` with it.
- Cluster words: this 895 + `input_process_controller` 363 + `func_800C3AD0` 362 + `camera_trigger_check` 308 +
  `camera_victory` 367 + `entity_update` 391 + `steering_sensitivity` 232 + `traction_control` 248 + smaller.

## First pass
- m2c seed compiles after 1 mechanical fix (arity K&R decls): 916 words vs 895,
  **opcode-aligned 54.7 %**, opcode+regs 14.2 %, exact-word LCS 60. This says nothing about IPA
  register choice, which is where it fails.

## Recommended approach
- Do not start here. Revisit only after `input_process_controller`/`func_800C3AD0` group matches.
