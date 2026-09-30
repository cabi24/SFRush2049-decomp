# render_display_list (0x80099BFC)

| item | value |
|---|---|
| words | **2559** (blob_800966d8.s) |
| spliced? | No. |
| ABI or IPA | **IPA-dependent, heavily.** Prologue saves only `ra` (frame 608) yet the body uses s0-s5 and reads **s0, s1, s3, s4 before any write** (m2c shows them as `saved_reg_s0/s1/s3/s4`): a rect pointer (s0: 4 x u16), an object/texture descriptor (s1: flags at +0x1C, u8 fmt at +0x14, size at +0x15, u16 w/h at +0x10/+0x12, tex ptr at +0x18), s4 = `Gfx **` cursor, s3 a fourth input; it also clobbers s2/s3/s5 unsaved and uses `ra` as a scratch (`lui ra,0xf510`). It cannot be compiled alone. Callers: `particle_system` (725w) and `func_8009F058` (1307w, itself IPA). |
| closure | Belongs to the 129-function / 32,859-word `audio_doppler_calc` closure (see `closure.py`: `render_display_list`, `func_8009F058`, `particle_system`, `track_collision_wall`, ... all listed missing). |
| callees | 2 distinct: `func_80087804` (spliced), `func_80099B30` (51w, 2 args ABI-looking) |
| globals | 29 lui targets; `0x8017A638` flag, `0x8017A..` state, cursor in stack slot 596(sp) |
| shape | Same RDP command emitter family as `object_render` (SETTIMG 0xFD, SETTILE 0xF5, LOADTILE, 0xF2, 0xE6/E7/E3), but driven from a struct instead of arguments. **96% of words are in repeated 12-word shape windows**; 148 branches (6%); no FP. Pure table/state-machine code. |

## First pass
- m2c seed with s0/s1/s3/s4 promoted to parameters and the usual `void *`-deref sed compiles (`seeds/render_display_list.c`): **2632 words vs 2559 (+2.9%)**; -O2: opcode-shape 47.4%, opcode+reg 14.7%, exact 9.6%. -O3: shape 49.8%, reg 8.5%, exact 5.2%. Registers are meaningless until the IPA group exists (target's operand regs come from IPA).

## Feasibility: LOW (blocked by IPA)
The register file at entry (s0,s1,s3,s4 inputs; s2,s3,s5 unsaved) and its callers `particle_system`/`func_8009F058` cannot be reproduced without compiling them together, and their closure is the 33k-word mega group. Only worth it if the maintainers build the full `audio_doppler_calc` closure. Structurally it would be very tractable (same PUSH-macro tower as `object_render`), so the IPA closure is the only real obstacle.

## Recommended approach
Do nothing until `object_render` shows the macro approach works; then reuse its macros here and attempt only if a group containing `particle_system`, `func_8009F058` and their callers exists.

## Effort
3-5 days of C plus the (unscoped) IPA closure work. Not for the hail mary.
