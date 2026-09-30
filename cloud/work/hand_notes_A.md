# Hand-matching notes, agent A (list ~/agents/A/list.txt on Rocky, 37 functions)

Deliverables (strict MATCH, files in cloud/matches/, each re-scored as a bare MATCH from the repo copy):
- func_800AB750   flags `-O3` (amatch search found it: the ONLY change vs the seed is `-O3`; seed at -O2 was 12/32).
- func_800BB7F4   already a strict MATCH at -O2 from the seed, and cloud/matches/func_800BB7F4.c already existed (not new).
- func_800A5488   plain `for (i = 0; i < 200; i++) (&D_80149450)[i] = (s32)((&D_80146210) + (i << 6));`. The m2c goto loop stepping a
                  pointer by 0x10 and storing at +4/+8/+12 is just the loop IDO unrolls x4 itself.
- input_init_flag_get  `do { if (*p != 0) (*p)(); p++; } while (p != end);` over a function-pointer table, return type void.
                  QUIRK: matches ONLY with the whole body on one source line; the multi-line layout swaps two `lui`s.
- Effects_UpdateEmitters  rewritten from the retail asm, not from the seed: two arrays of 152-byte records
                  (`(EmitA *)&D_80150B70 + i`, `(EmitB *)&D_80150BA0 + i`), plain for loop over `(s16) D_80151AD0`. The seed's
                  offsets were simply wrong (it was 45/64).
- func_800FBE60   rewritten from asm: index loop over (s8)D_80152744 with `extern u8 D_80152818[]` (address-spelled symbol, resolves via
                  the scorer fallback). QUIRKS: `D_80152018 = 0;` (int literal) while the in-loop store is `= 0.0f` (float literal):
                  two different constant webs give the two distinct `mtc1 zero` registers (f4 and f0) of the target.

Lessons that paid off
- If the seed is 40%+ off, rewrite from the retail asm (`python3 cloud/work/tools/tdis.py FN`): m2c seeds here mis-scale `s32 *` arithmetic
  (+0x10 bytes where the asm has +4) and invent `new_var`/`temp` locals. Two functions went from ~45 diffs to MATCH this way.
- Unrolled-by-4 loops in the asm: write a simple counted loop, do not copy m2c's pointer walk.
- Parameter homes: target stores a0..a2 to the stack and narrows with andi/sll+sra => the params are u16/s16 (func_8008B000, func_8008A644).
- A `| 0` on an unsigned shift (`((u32) arg0 << 16) | 0`) changed which registers uopt gives the address pointers (func_8008A644 13 -> 5 diffs).

Register "ring" symptom (blocked about 8 functions): target temps cycle t6,t7,t8,t9,t6,... and skip t0-t5 (or skip exactly one), while our
compiles rotate t6..t9,t0..t5. Tried and ruled out: -O1/-O3/-mips1, per-function -O3 group pipeline (uld/umerge/uopt, score.py group with a
single member), extern vs defined symbols, declaration order, dead `x = x;` statements (all folded away before ugen), `| 0`/`+ 0` on most
expressions. I could not reproduce it from a single TU. Affected: func_800C7200, input_aux_handler, func_8008ABE4 (tail), func_800CCE5C (2 diffs, t6 vs t0),
func_800966D8 (t6 vs t7 start), func_8008B000, func_8008A644, func_800A79F4.

Per function (tried / why stopped)
- func_8008A704 (2 diffs): only the order `li t7,1` / `sw ra,20(sp)` around the first `bnez` (target: li before the branch, sw ra in the delay slot;
  ours the reverse). ~30 forms (negated test, goto, comma expr, local `one`, OSMesg typing, -O3/-O1, one-line body) and a 1500-eval amatch run; no movement.
- func_800D0B14 (4): float regs: target t in f2 (sub.s f2) then mul f10; ours sub->f8, mul->f2. Split/merge of the expression, locals, statement order,
  `*1.0f`/`+0.0f` dead-ops grid: no match. 1500-eval search no movement.
- func_800966D8 (7): loop structure and everything right, only t6/t7/t8 vs t7/t8/t9 and `andi a0,a0` vs `andi a0,v1`; ~20 edits of the m2c locals (new_var, new_var2, keep) no movement.
- func_800EAFDC (4 with `f32 d = D_80124518;` first): d and t swap f2/f12. Without the local the registers match but the `lwc1 D` schedules after `add.s`. ~25 forms.
- func_800D18D8 (8): stack slot fixed by dropping `sp1C`; remaining diff is which of D_801460F8/D_80146104 gets the first address register (first-use order). `u32` externs, temps, statement/operand orders: no.
- func_800D1248 (14): target keeps arg0 in a2 for the whole first half (`move a2,a0` hoisted); ours uses a0 until the call. Local copy, param types, flags: no.
- func_800CCE5C (2): only `lw t6,0(t9); lw a0,8(t6)` vs t0 (ring). amatch found the 2-diff form (best in partial dir).
- input_aux_handler (16-26): the m2c `& 0xFFFF` chains / dummy labels are the permuter's register shifters; the clean if-chain is 26 diffs, ring symptom.
- func_8008A644 (5): pointers a1/a2 now match (see `| 0` lesson); remaining: temp_v1 lands in v0 (target v1) and `lui t9 / sll t1` vs `sll t9 / lui t0`.
- func_800B73E4 (15): loop body matches as `for (i = 0; i < 8; i += 4)` with stores ordered [1],[2],[3],[0]; prologue differs (flag address reg v0 vs a0, `lui at` shared by the two globals in the target).
- func_8008ABE4 (11): with a typed struct for D_80153F10 and `(s32) ents + idx * 0xC` the code is right apart from temp numbering.
- func_800ADCE0 (9): natural nested while loops; arg3 allocated to s0 (target t0 copy), `v` copy (v1 vs t1).
- func_800B0EA0 (30): `inv` must be s32 (sra); remaining is the scheduler hoisting the `andi` masks above the first mflo (target keeps them after). No source change moved it.
- func_800C1A00 (20), random_seed_init (11), save_slot_valid (27+), func_800A7480 (14), func_800C7200 (40), func_8008B000 (40), func_800A79F4, func_800D2054 (20),
  func_8008C768, func_800E7038, display_list_traverse (22 with `q = p + 1` pointer), sound_control (~36 from search; regs spill arg0/arg1 to 66/70(sp) vs ours s1): structure recovered in cloud/work/hand_partial_A/, not matched.
- IPA-bound (not singly solvable): mode_byte2_set and object_type_byte2_get (callee sound_update_channel reads t0; target `move t0,zero` vs ours `move a0,zero`/nop),
  func_800B5688 and func_800DA0BC (use s0-s3 without saving them: caller-saved s regs).
- func_800E7038 also shows 8 separate `li 70` (t0..t5,t8,t9) and one shared `lui at`: looks like -O1-style constant handling inside an -O2 function; not reproduced.
- graphics_chunk (106/132): seed too far; not attempted beyond reading it.
- physics_forces2: no `.text.physics_forces2` section in asm/us/blob, nothing to score.
