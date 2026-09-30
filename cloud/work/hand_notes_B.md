# Hand-matching notes, agent B (list ~/agents/B/list.txt on Rocky)

Result so far: no function reached a strict MATCH; nothing written to `cloud/matches/`.
Method: every function got the amatch search (`search.py`, budget 400, seed 1, -j 4) from its near-miss seed, then
hand rewrites scored with `score.py` (flags `-g0 -O2 -mips2 -G 0 -non_shared`, plus `-Wab,-r4300_mul` via the
scorer). The catalog search never improved on the seed for any function (best_path empty everywhere), so the
notes below are about what the diffs actually are.

Helpers used (scratch only, on Rocky `~/agents/B/scratch/`): `tryv.py` (replace one function with N text variants and
score each), `perm.py` (permute a block of source lines, in-process scorer), `mkobj.py` + objdump on the Pi for a
side-by-side disassembly of retail vs ours. Diffs are listed "want / got".

## Functions that are an IPA/group problem, not a source problem

- **mode_byte_set**: natural rewrite (`if (arg0 < 0) { sound_update_channel(); D = D_801497F0->byte8; } else D = arg0;`)
  scores 1/20. The one differing word is `move t0,zero` in the `jal sound_update_channel` delay slot.
  `sound_update_channel` (0x800B3D18) itself reads `t0` (`bnez t0,...`), so it is an IPA-compiled callee with a hidden
  parameter in `t0`. Needs a group containing the callee; not matchable alone.
- **object_type_byte3_get**: same, one word (`move t0,zero`). Seed scored 1 word.
- **func_800B7438**: target stores `a0` AFTER `jal func_800B73E4` (`sw a0,0(v0)`), i.e. it relies on the callee not
  clobbering `a0`. Natural rewrite `void f(s32 a){ if(!D_801147C4) func_800B73E4(); for(p..10 words) if(!*p){*p=a;break;} }`
  compiles to the right loop but spills a0 around the call (11/24). IPA with callee func_800B73E4 needed. The m2c seed
  also has the wrong signature (it is void, takes one arg).
- **func_800DCCE0, func_800DCD58**: target uses `s0`/`s1` as scratch with no save/restore in the prologue (and
  `addiu sp` placed after the first loads). Callee-saved registers clobbered without saving means an IPA/shrink-wrapped
  group; our compile saves a0 to the stack instead. Not matchable alone.

## Register-allocation / scheduling walls (structure already right)

- **func_8008C680** (4 words): only FP register names differ. Target reuses `f4/f6` for `arg0+1.0f` and the
  `D_801238F0` load; ours takes `f16/f18`. 15 source variants (temps, operand order, else-if, result var, int
  literals, comparison direction) produce the identical 4 words. Looks like an FP allocator rotation difference.
- **state_update_global** (7; 5 with `u8 v`/`u32 t` locals): v0/v1 swapped (target keeps the raw global in `v0`,
  the loaded byte in `v1`). Tried t/v declaration order, init order, types s8/u8/u32, ternary, no-local forms.
  Only type combos changed it (u8 v gives 5). Not solved.
- **func_8008D0C0** (9): target uses `v1,t0,t1` for the three temps, ours `a3,a1,a2` (a1-a3 untouched by the
  target). Declared-but-unused extra params (a1..a3) only add spills (`sw a1,4(sp)`). -O1 / -O3 / -mips1 / loopunroll
  flags do not help. Likely a different original structure that uses a1-a3 for something removed.
- **camera_update_a** (14): target puts `arg2` in `s0` and the node in `s1`; ours the reverse. Modifying `arg1`
  directly, while/for forms: 12 words, still the same swap. Weight/priority issue.
- **func_80096BBC / func_80096C28** (11 / 23): same loop shape (list of 12-byte entries, counter + byte offset + element
  pointer). Natural typed-struct source gets BBC to 8/27; the remaining difference is that the target puts the byte
  offset in `v1` (reusing the dead `q` pointer) and the element pointer in `v0`, ours the reverse. C28 also has the
  `sll t9,t8,0` sign test that only the m2c `& 0x80000000` form reproduces.
- **func_800D11BC** (7): target keeps the store order in source order and leaves the `jal` delay slot for
  `move a0,s0`; ours hoists `li t6,1; sb` above the float stores and fills the slot with `swc1`. Volatile, typedef'd
  struct, type of the byte store, store reordering all give the same 7 words.
- **func_800D4D84** (6): same set of loads/stores, only as1's ordering of the FP loads/stores differs. Source-order
  permutations of the 9 stores (sampled 3000 plus a 15000 run) did not produce it.
- **func_800DC1AC** (15) : `v`/counter/address registers shifted (target `a3` for the shifted value, `a0` counter).
  `result`-variable, for-loop and `arg0 >>= 1` forms change the size and score worse.
- **task_complete_signal** (5): ours is one word longer: two nops before the (dead) epilogue, target has one.
  while(1), for(;;), do/while(1), trailing return, continue forms all give the same.
- **func_80091B00** (18): the 4-way unrolled slot scan. Target emits `sb flag; li -1; sh` per copy reusing
  t6..t9; ours loads both constants first and orders `sh` before `sb` in the copies. Source store order swap,
  pointer loop, index loop, volatile stores all tried; none changed it.
- **game_timer_pause** (16): the two address computations (`D_80110270 + off` for the load, `off + D_80110270` for the
  returned pointer) are not CSE'd in the target. The form `if (load != arg0) NULL else ptr` (else-branch assignment)
  gets 13, typed `TSlot*` forms lose the early `move a1/a2,zero` hoist and get 22+.
- **entity_name_copy** (bsearch): a clean rewrite with the unused-looking `s32 one = 1; s32 z = 0;` variables gets
  the correct frame (72) and structure, 30/74. Remaining: allocation order of `n/half/mid` (target s2/s1/s0, ours
  s0/s2/s1, plus the `mid` copy in s3). Declaration order (720 permutations) and operand order make no difference.
- **tire_sound_update** (16): changing the first loop's store to `*(var_s1 - 1) = ...` fixes the store offsets;
  what is left is the order of the address `lui/addiu` setups (12 words after permuting init and decl order). The
  m2c offset-accumulator (`var_s2`) structure hides the real arrays.
- **func_8008B32C** (3x3 scale): `for i for j arg1[j] = arg0[j]*arg2; arg0 += 3; arg1 += 3;` gets 10/29 (best). The
  target uses `a2=arg1`, `a3=arg0` as outer pointers and inner copies `a0/a1`; ours swaps them and adds a spurious
  `sw a0,0(sp)`. Other loop forms score 13-28.
- **func_800A7BF8** (list search, 68-byte entries): natural typed rewrite 24/41 (seed 31). Target keeps the `s16 arg0`
  sign-extended in place, returns `(s16)i` with `i` an s32, uses `move a3,a2` for the index copy. Not solved.
- **func_800D2C10** (nearest point): target uses a 32-byte frame with `sp6` and copies `arg0` to `a3`. Changing the
  m2c `s16 var_v0` to `s32` makes IDO unroll the loop (131 extra words); goto/while forms and padded frames
  did not fix the frame (41-43 words differ).
- **entity_cull_check**: m2c shape (with its `new_var` temporaries) is what reproduces the `addiu t0,v0,4`
  and the hoisted `lw 8(v0)`; a natural u32* rewrite is 63/64 different. The target saves s0-s2 for hoisted
  constants; needs the original macro form. Not solved (41 word diff seed).

## Not attempted beyond the automated search (seed diff >= 40 words, m2c-shaped, large)
credits_screen (72), func_800A7E10 (66), func_800D8078 (49), func_800E762C (47), func_800B3704 (42, size also
wrong), game_mode_handler (40), graphics_chunk_b (112), main_menu_render, records_screen, sound_stop (m2c goto
form; the two nested loops would need a full rewrite), func_800C9480 (31, memset/minimap array init loop: target
keeps `li a0,1` constant-index, ours builds it via counter),
func_800F7448 (31), object_pool_init (no retail target section: `score.py` cannot score it).

## Late additions

- **records_screen** (22 seed, 20 best): rewritten as a 104-byte-stride struct walk
  (`typedef struct { s16 a; char p[0x32]; s16 b; char q[0x32]; } R104;`, `p = (R104 *) &D_801541A8; ... p++`,
  loop body `t = p->a; if (-1 != t) { entity_spawn_callback(t,0,0); p->a = -1; } t = p->b; ...`) with **`s32 t`** (an
  s32 temp passed to the `s16` parameter reproduces the target's `sll/sra/move a0` narrowing; an `s16` temp or a
  direct `p->a` argument does not). Remaining 20 words are all one allocation difference: target keeps the
  `&D_80154198` base pointer in `s0` and reuses `s0` for the loop pointer `p` (and later `&D_801541A0`); ours puts the
  first pointer in `s1`. Decl-order permutations and explicit `s32 *g = &D_80154198` pointers do not move it.
- **main_menu_render** (25): the m2c `D_8014A15C`/volatile `new_var` is really a walk over a 76-byte player array
  based at `D_8014A15C - 0x44` (`p->id` at +0x44, `p->f0` u8 at +0, the loop also indexes `D_80150B70` with stride 0x98
  and passes `q + 0xC` as the 5th (stack) argument). Natural struct loop gets the body right (frame 64, same
  multiplies) but swaps the roles of `s0/s1` (target: counter `s1`, element pointer `s0`; ours the reverse). Not
  solved; decl order did not change it.
- **object_pool_init**: the harness has no retail section for it, so `score.py fn` cannot score it.
- **sound_stop** (35), **func_800C9480** (31), **func_800F7448** (31): only the automated search was run.
