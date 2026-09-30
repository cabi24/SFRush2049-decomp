# countdown (0x800FBF88) and its ABI callees, round 3 precursor pass

Scored with `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/countdown --as1=-r4300_mul`.

| function | words | result |
|---|---|---|
| `func_800B61A8` | 21 | **MATCH** (also `cloud/matches/func_800B61A8.c`, `score.py fn` MATCH at -O2) |
| `func_800FBF2C` | 23 | **MATCH** (also `cloud/matches/func_800FBF2C.c`) |
| `func_800FBE30` | 12 | **MATCH** (already in `cloud/matches`) |
| `func_800F7F3C` | 349 | size exact (349), 273/349 differ: register allocation only (see below) |
| `countdown` | 668 | draft emits 675 (660 body + s-register saves); not a match |

`claims` = the three MATCH members only.

## Key finding: countdown is NOT ABI-shaped

The scout note called it ABI. It is not. Its prologue is `addiu sp,-24; sw ra,20(sp)` and nothing else,
yet it uses s0-s6 and $f20/$f22. The unsaved callee-saved registers are the IPA signature: its only caller
`game_loop` (0x800FD464, 176 words) saves `s0-s8` and `$f20/$f22` itself (`sw s8,72(sp) ... sdc1 $f20,24(sp)`)
although game_loop's own body only touches v0/v1/t-regs. So the registers are saved once at the root for the
whole visible call tree. Also `func_800D6160` (76 words) takes an argument in `t0` (`move t0,zero` in the `jal`
delay slot) and uses s4/s5 unsaved; it has 5 other callers (`physics_init_mode0/1`, `drone_pathfind_main`,
`func_800D7634`, `RaceStateMachine_Update`). A group that can match `countdown` needs root `game_loop`
plus the rest of its tree (`countdown_handler`, `UpdateActiveObjects`, `PhysicsObjectList_Update`,
`Effects_UpdateEmitters`, `Input_ProcessGameplayPad`, `RaceStateMachine_Update`, `playgame_state_change`,
`func_800D6160` and its callers). That is the closure gap; not attempted.
The callees `func_800F7F3C`, `func_800B61A8`, `func_800FBF2C`, `func_800FBE30` DO save their own s-regs / are
leaves, so they were compiled separately at -O2 (ABI), which is why they match alone.

## func_800F7F3C (player ranking, 349 words)

Bubble sort of `D_80143F54[]` (player order) by a mode-dependent key, then flags ties with the leader in
`D_80150B68[]` and counts them in `D_80150B60`. Modes: `D_8014A110 == 4` key `D_80152038[rec.idx].key`;
`== 6` first sort by car byte `+0x3A3`, second by `D_8015256C[D_8012E77C[idx]]`, ties on `+0x3A3`; else car byte `+0xEE`.
Near miss source: `cloud/work/r5_d/func_800F7F3C.near.c` (also the body in `group.c`).
Learned, all needed to get the 349-word shape:
- Loop bounds must be the **global** (`i < D_8014A108 - 1`), not a local copy: a local trip count makes IDO unroll
  every loop (664 words); with the global it does not (init loop and sorts then match the target shape).
- Index loops `for (j...) a = o[j]; b = o[j+1]` (IDO turns them into the pointer loops the target has).
- `ia = rec[a].idx; ib = rec[b].idx` as named locals gives the target's `lh`-before-`multu` order.
- In the tie loop the leader index must be re-read **inside** the loop (`f = rec[o[0]].idx;` as the first statement of the loop body),
  which reproduces the target's hoisted `lh` with the multiply and `lw` kept in the loop.
Remaining difference: register choice (target: a/b=v1,a0, idx temps=a3,t0, i=t1, n-1=t2, n=t3, bases a1,a2,s0,s1;
ours: bases take v0,v1,a1,s0 first). 720 declaration permutations, 256 statement-order/operand mutations: no change.

## countdown draft

`cloud/work/r5_d/countdown_draft.c` (and in `group.c`): clean hand rewrite with typed player/car/input structs and
plain loops (not the m2c goto form). -O2 single compile: 660 words, opcode-aligned 558/668 (84%), exact aligned
240/668 (36%). The state machine (bits 0x40000, 0x80000, 0x200000, 0x100000, 0x400000, 0x1000000, 0x2000000, 0x800000,
then the `& 0x600000` tail loop) follows the target block for block. Known differences: target does not hoist
`&D_80114 6F0` style addresses (uses `lui at` per access), `func_800D6160` needs its `t0` argument, prologue/saves.

Note (round 5): the three matching callees are claimed as single functions (cloud/matches/ and src/blob/), so claims is empty here to avoid a double splice. countdown itself is IPA-shaped and needs game_loop as root of a larger closure.
