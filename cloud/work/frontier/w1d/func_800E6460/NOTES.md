# func_800E6460 (throttle/brake input)

`best.c` is the three-member group source (same file as `../groups/control_input_E681C/group.c`).

- State: strict `MATCH` as a member of the real group (real caller `func_800E681C` kept, no stand-ins).
  The caller is 16/179 words off, so this is provisional, not a claim.
- Alone at `-O3` it is 94/239 (`alone_control.c`): internal function, four-wide `t6`-`t9` ring.
- The 32-word residual of the earlier attempt was the operand order of `in->reverse & D_8013FED0[in->pad]`.
  Fix: the record has an array `u32 map[13]` at +0x18 and the test is `D_8013FED0[in->pad] & in->map[5]`.
