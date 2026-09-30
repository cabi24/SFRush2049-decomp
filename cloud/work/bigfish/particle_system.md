# particle_system (0x8009C8F0), scout report

Feasibility: **LOW** (IPA-blocked; caller is a 678-word function). Effort: 3+ days.

## Facts
- 725 words, `asm/us/blob/blob_800966d8.s`. Not spliced. Frame 232, saves only `ra` (44(sp)) while clobbering `s0..s8`: it does not save callee-saved registers at all.
- **IPA callee.** Reads `s7` at entry (`lhu v0,20(s7)`, `lw v1,0(s7)`: an object pointer the caller keeps in `s7`), `t0` (caller does `move t0,s2`), and `a1..a3` plus stack words 0(sp) and 24(sp), 232(sp) (`lw t6,232(sp)`: a pointer-to-pointer, which the function reads at entry and writes back at exit). The sole caller `track_collision_wall` (678 words, 0x8009E5xx) sets these up and reloads `s1`,`s7` from its own stack afterwards. A group needs `track_collision_wall` too (real callers of that: more), so 1,400+ words minimum.
- 6 callees (`camera_update_d`, `func_8008E0B8`, `func_8008C768`, `func_8009C3F8`, `func_80099B30` x2, `render_display_list`), 11 distinct globals, 65 branches, 4 loops, display-list building constants (`0xE7000000`, `0xD9FFFFFF`, `0xDC08...`, `0xF2...`), 1 multiply. The name is a guess; it is a display-list emitter for effects.
- m2c cannot type it (parameters land in `saved_reg_s7`, stack args mis-attributed).

## First pass
`seeds/particle_system.c` (m2c plus hacks so it compiles; parameters are wrong): 632 words vs 725, aligned shape 61%, aligned exact 9%. Not meaningful beyond "same size class".

## Approach
None recommended before `track_collision_wall`'s IPA group exists. Revisit after Lane A tooling can regenerate closure.
