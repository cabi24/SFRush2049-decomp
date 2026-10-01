# Workbench pilot W1 (agent C): register-rotation wall, 4 functions

Setup: Rocky, `~/agents/wb/loop.sh` for single-file `-O2` runs; for the group experiments a small harness
(`tools/cloud/score.py group` plus `decomp_workbench diagnose` on the group object). Sources and best variants are in
`cloud/work/workbench_pilot_W1/<function>/`.

## Headline finding (not in the guide, and it contradicts the diagnosis for this wall)

The `t6`-`t9` ring wall is **not a source-level temp-FIFO phase problem**. In every one of the four functions the retail
temp lane is a 4-wide ring (`t6 t7 t8 t9`, wrapping `t9 -> t6`), while a single-TU `-O2` compile gives a 10-wide ring
(`t6..t9, t0..t5`). `diagnose` reads this as "one pop short / phase-shift" and points at levers 14-16, but no phantom pop can
make `t9` wrap to `t6` while `t0`-`t5` are still in the ring. The cause is the uopt-to-ugen register reservation range
(`Uregs`, see `cloud/work/register-ring/README.md`), which is raised when the function is **IPA-internal** in an `-O3`
`uld`/`umerge` whole-program group. Compiling the function as a non-kept member of a group with two call sites (stand-in
callers `zz_a`/`zz_b`, callee not in `keep`) reproduces the 4-wide ring with plain C and no hacks:

* `input_aux_handler`: 16 words (`-O2` single file) -> **group MATCH** (also needed: drop the m2c `0x400 & 0xFFFFFFFFFFFFFFFF`
  and the `new_var`/dummy-label/`& 0xFFFFu` noise; it matches as the clean if-else chain).
* `func_800C7200`: 40 words -> 4 words (ring and pool lanes then identical, residual is `move v1,zero` placement and a trailing `li v0,64`).
* `func_8008ABE4`: 23 words -> 6 words (ring identical 11/11; residual is the base pointer colouring, `t1` vs `v1`).
* `func_800CCE5C`: does not combine: as an IPA-internal member it gets the IPA argument convention (`s`-registers, frame -24), and
  as an address-taken/exported member (ABI args) the reservation reverts to the 10-wide ring (2 words). Best: 2 words single-file.

Caveats: group matches use stand-in callers (two sites so the callee is not inlined; one site inlines it at `-O3`). The real
groups need the real callers (`game_loop`, `task_complete_signal`/`struct_init_and_call`, `func_800C813C`/`func_800CC50C`) and are
not splicable as-is. Whether retail really had these functions IPA-internal is inferred (the ring matches with no other change).

## func_800CCE5C

Start: seed `-O2`, `allocation-mismatch`, 15/40 (frame -48 vs -40, `sp1C`/`sp24` homes, ring `t6->t0` x2).

| # | lever | change | words |
|---|---|---|---|
| 1 | 26 (frame, drop locals) | drop `pad2` | 15 -> 12 |
| 2 | 26 | drop `pad` too (the task note's "5") | 12 -> 5 (frame 40; `sp1C` home 24 vs 28 + ring) |
| 3 | 26 (types, order, `register`) | decl order swap (7), `u8*` types, `register`, extra dummy locals of every size/order (all 12: frame 48) | no gain |
| 4 | 26 "ablate one local at a time" | remove local `sp1C` entirely, inline `(u8*)sp24 + 0x3C` | 5 -> **2** (frame and homes exact) |
| 5 | 14 | hoist `slot_state_lookup` arg into local `q` | 5 -> 12 (frame grows) |
| 6 | 15 | `if ((x == NULL) != 0) return;` guards | 5 -> 40 / 19 (codegen explodes) |
| 7 | 16 | redundant `& 0xFFFFFFFF` on call args | 5 -> 5 (no effect at all) |
| 8 | flags | `-O1` 40, `-O3` 2, `+r4300_mul` 2, `-Wo,-regr,10/12` 33 | no gain |
| 9 | IPA stand-in callee with 12 live values, `-O3` and group mode | raises nothing | 5 -> 5 |
| 10 | IPA group (internal, 2 callers) | ring right but ABI becomes `s`-regs | 2 -> 39; address-taken form back to 2 |

Result: best **2/40** (`func_800CCE5C/best_2words_single_file.c`): only `lw t6,0(t9)`/`lw a0,8(t6)` (`t0` vs `t6`). The thing that moved it most
was removing the named local `sp1C` (frame and spill homes exact), which the guide's ablation advice (lever 26) found; the
ring was never moved by 14-16.
Tool verdict: frame diagnosis and `frame evidence` split (save slots equal, non-save differs) were correct and useful; the ring part
was misleading (`one pop short`), because the real cause is ring width, not phase. Lane view (`t7 t8 t9 t6` vs `t7 t8 t9 -`) did show it
if you read it as a wrap.

## func_800C7200

Start: seed `-O2`, `structure-mismatch`, 40/64 (65 vs 64 insns; ring `t0..` from the second block; `move v1,zero` and `li v0,64`).

| # | lever | change | words |
|---|---|---|---|
| 1 | rewrite natural (not a lever) | typed index loop with explicit 4-way body, `-O2` | 40 -> 43 (worse) |
| 2 | 14-16 not applicable | the ring lane was constant-shifted, not phase | - |
| 3 | IPA group (internal, two stand-in callers, `-O3`) | seed body unchanged | 40 -> **4** |
| 4 | variants on the return/limit (`return var_v1`, local `n=0x40`, `e` pointer, `>=`, `!=` goto form) | | 4 -> 4 (all same) |
| 5 | swap init order | `var_a0` first | 4 -> 32 (pool colours swap) |
| 6 | `while`/`for`/`do` forms | | 5-7 and extra words (unrolled) |

Result: best **4/64**, group only. Residual: target hoists `move v1,zero` after `addiu a0` (ours between `lui` and `addiu`) and has no
`li v0,64` after the loop (the loop-bound constant web in `v0` doubles as the return value; ours rematerialises it). One thing that
moved it: group mode. Tool: `diagnose` said `structure-mismatch` (correct for the real 1-word delta) and the lanes showed the
target never leaves `t6-t9`; the `webs` line (`w1 t6->t0 ...`) was the useful part. No lever applied to the last 4 words
(`lever: none-known`).

## input_aux_handler

Start: `allocation-mismatch` / `register-ring-only`, 16/121 at `-O2`. The tool's verdict `register-ring-only` was exactly right and
the signature `prefix-exact@24 ... temp:9` located the first bad row.

| # | lever | change | words |
|---|---|---|---|
| 1 | IPA group `-O3`, internal, 2 stand-in callers, m2c seed unchanged | | 16 -> 51 (structure: different) but ring lane identical to row 78 |
| 2 | 1 (audit the earliest immediate) | `0x400 & 0xFFFFFFFFFFFFFFFF` -> `0x400` (64-bit and emitted `beqzl`+extra `andi`) | 51 -> 8 |
| 3 | m2c cleanup | remove `new_var` and the assignment in the condition | 8 -> **MATCH** |
| 4 | m2c cleanup | remove dummy labels and the 12x `& 0xFFFFu` chain | still **MATCH** |

Result: **MATCH as a group** (`cloud/work/workbench_pilot_W1/input_aux_handler/`, `score.py group --claims` re-scored MATCH), not as a single-file
`-O2` compile (16 words), so no `cloud/matches` file was written. Flags `-g0 -O3 -mips2 -G 0 -non_shared`, callee not kept, two stand-in call sites.
Tool: correct (ring-only) but the lever list (14-16) would not have solved it; the late-hunk `structural` verdict with the
relocation-masked "floor" caution was useful to see that remaining words were real code.

## func_8008ABE4

Start: seed `-O2`, `structure-mismatch`, 23/36 (pool v1 vs t1, ring from row 4).

| # | lever | change | words |
|---|---|---|---|
| 1 | IPA group, seed unchanged | | 23 -> 12 (ring identical 11/11) |
| 2 | rewrite natural | early `return 1`/`return 0` instead of result variable (target has `move v0,zero` in the delay slot of the branch) | 12 -> **6** |
| 3 | struct-typed `extern ABQ D_80153F10`, `->cur++` forms | | 11 / 16 / 33 (worse) |
| 4 | named base pointer local (`u8 *B`), `register` | | 33 (frame grows) |
| 5 | operand/compare orders, `s32` temp, `!(a<b)` early return | | 6 or syntax errors |

Result: best **6/36**, group only. All 6 words are one allocation: target keeps the `D_80153F10` base in `v1` and `cur` in `t0`; ours puts
the base in `t1` (v0/v1 free in our lane, so this is a colouring-order effect, not interference). No lever in the guide addresses it.
Tool: lanes (`temp identical 11/11`) tell you the ring is solved and the rest is pool colouring: good. It cannot say why the base
web is coloured last.

## Summary

| function | start | best | how |
|---|---|---|---|
| func_800C7200 | 40/64 | 4/64 (group) | IPA-internal group fixes the ring |
| input_aux_handler | 16/121 | MATCH (group) | IPA-internal group + removing m2c 64-bit mask and noise |
| func_8008ABE4 | 23/36 | 6/36 (group) | IPA group + early-return form |
| func_800CCE5C | 15/40 | 2/40 (single) | drop `sp1C`/pad locals; ring not reachable alongside ABI args |

Where `diagnose` helped: the lanes view (pool vs temp, `identical N/N` on the temp lane) quickly showed which part of a residual was
solved; `register-ring-only`, the frame evidence split, `prefix-exact@N`, and the webs line were all accurate; the strict relocation caution was honest. Where it
did not: for this wall it prescribes levers 14-16 (phase perturbation) but the real cause is the ring width set by IPA internal reservation, which
no source phantom pop can change, so "one pop short" is misleading; it never mentions IPA/group context as a lever; and for the residual last
words (`none-known`) it has nothing. Not in the guide: (a) compiling the function as a non-exported member of an `-O3` group with two call sites
reproduces the 4-wide `t6-t9` ring (a diagnosis the guide should add: "ring never leaves t6-t9 and wraps t9->t6 => IPA-internal reservation"); (b) a
named local that is only ever used as a pass-through (`sp1C = sp24 + 0x3C`) adds a spill home and moves a neighbouring spill slot; deleting it fixed the
frame (lever 26's ablation covers this, but the exact slot behaviour (spill temps land at the bottom of the locals region) is worth noting); (c) m2c
`x & (0x400 & 0xFFFFFFFFFFFFFFFF)` makes a 64-bit and, adding a `beqzl` and an extra `andi`.
