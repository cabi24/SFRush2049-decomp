# func_8008E408 (0x8008E408, 1544 B) — w9c

All five callees (func_8008E0B8, func_8008E26C, func_8008E3C0, math_utility,
vector_normalize_length) are locked, so this was scored in the real whole-program unit
(`blob_unit --tag w9c score func_8008E408 --with best.c`); no stand-ins, no group needed.
`frontier` labels it `group` only for `preserved: a1`, which looks like a false positive
(caller entity_spawn_init is plain ABI). Prior seed cloud/work/ipa-groups/codex_effect_basis_a159
was 383/386 off.

## Progress (unit score, words differ of 386)
| step | change | result |
|---|---|---|
| cand.c | natural typed rewrite | 366 (+19 short) |
| v2 | `snap = D_80117498` + compiled-out check | dead `sw t7,168(sp)` reproduced; o leaves s0 |
| d_snapmid | check before 2nd alloc | 267, 382 words |
| base2 | `func_8008B2E4(1.0f) - 0.5f` (Random, inlined) | keeps the mul by 1.0 |
| e_h86lit | first lookup `D_8014295A[0]` | 220 |
| i_u32 | `u32 type` | switch uses `li at,3` (no shared t0=3) |
| j_012_pv | `pos[i] = pos[i] + vel[i]`, i=0,1,2 | 84, size exact; only car t3<->seed t4 |
| v_typ | `if (type > 3) DEBUG_PRINT` before switch | registers exact |
| u_9 padding | 22 padding slots placed as retail | locals exact, frame 256 |
| best.c | | **22 of 386**: spill slots only |

## Residual (lane: ugen spill-temp area)
Retail spill temps are at 28 (wheel ptr t2), 40 (&o->m t5), 44 (flags), 48 (car); ours at
24/36/40/44. Same allocation order (car first, top-down), so retail's temp area is one
slot (4 B) bigger and the locals start at 52, not 48. Every local is at the retail offset.
Padding the locals cannot move it: the locals block is rounded to 8 and the temp area
stays 24..47. Tried with no movement (~70 variants): debug checks after each call site and at
the end (car/type/idx/flags/o/scale/speed/r), flags as named/expression/pre-switch,
Random as a local static without `rannum`, inlined empty debug functions with 0-6 args,
h86 order, `r` split, `NULL`, `fabsf(speed)`, s16 pads.

## Traced allocator facts (copy of w3a instrumented uopt, proc 91 in the w9c unit)
car `&D_80152818[idx]` (w5) and the inlined rand seed address (w247) both had save=1.0.
Ties go to the lower web number (strict `<` in the candidate loop), so car took t3.
Retail needs car < 1.0: one more live block without a use. `if (type > 3) DEBUG_PRINT`
provides that block (n_velfirst showed the same with nocs 13 / tot 12).

## Best next hypothesis
Retail has one more ugen temp (a 7th slot at 24, never referenced in the final code), or an
8-aligned FP temp that 8-aligns the temp area (unused slots 32/36 in retail, 28/32 here).
Look at ugen's temp allocation for this proc: run ugen on the w9c unit stage with the w3a
force.sh recipe, and compare the temp count against a variant that keeps one more
caller-saved value across a call. The ~22 extra named slots retail carries (frame
256 vs 168 natural) suggest more source, such as inlined helpers or debug locals, that may
also own the extra temp.

## Structs recovered
Car (D_80152818, stride 0x3B8): +0x14 f32 dir[3], +0x2C f32 vel[3], +0x74 f32 wheel[4][3],
+0xF8 s16 (speed<<2). Effect object (pool D_8013F1E0 node): +0x04 f32 m[3][3], +0x28 pos[3],
+0x34 s32 render handle, +0x40 f32 (0.0333333), +0x44 vel[3], +0x50 u32 flags,
+0x54 s16 car idx, +0x56 s16 model sel. D_801392D8[]: per-car u32 latch bits 0x100<<wheel.
Own rodata 0x80123950..5C = 0.0333333f, 0.85f, 0.15f, 0.0333333f (verified against image bytes).
