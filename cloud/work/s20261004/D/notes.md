# Session s20261004 / agent D

Scoring: `tools/cloud/score.py` (a private copy on watchman2 under `~/rush2049/scratch/s20261004_D`.
The only changes are that it shows more diff rows and keeps the object). IDO 5.3 comes from toolkit
`796ae99a…/ido`. Extents come from `build/m2c_asm/<fn>.json` (closure/scanner), which equal the
`.text.<fn>` sections in `asm/us/blob`.

| function | result |
|---|---|
| func_800E7DD0 (116 w) | **MATCH**, true 0: `func_800E7DD0.c`, `-g0 -O2 -mips2 -G 0 -non_shared` (proof in `.json`) |
| func_800E7C2C (54 w) | best: 1 missing instruction (37/54 positional words). Group `best_audio_heap_800E7C2C/` (= `src/blob/groups/audio_heap`) |
| func_800E2D18 (122 w) | best 104/122 positional (frame + register allocation). `best_func_800E2D18.c` |
| func_800D9058 (82 w) | not attempted (blocked, see below) |
| func_800EF288 (202 w) | not attempted (blocked) |
| func_800DD0C0 (227 w) | not attempted (blocked) |

## func_800E7DD0 (matched)
This function is the race-end / standings check. Its first branch runs only when D_80142760, at least 2
active players and D_80143F10 are all set: it runs a timer `D_801525F4` against the frame delta
`D_8002EB94`. The second branch scans the D_80152744 cars (1 if `gameplay_mode == 2`) through
`D_8014A250[i].unk7C6` (the car index for that position) for a type-2 car whose speed is above 5.0.
`gameplay_mode == 4` returns `D_8015273A`. For register allocation the source must index
`D_8014A250[idx]` through an `s16 idx` local. A `Car *car` pointer local shifts the whole t-ring by one.

## func_800E7C2C (IPA -O3, audio_heap group)
Retail puts `heap ? heap : D_801527C8` into `v0` and then does `move a2,v0`. Every variant puts it straight
into `a2`: direct ternary, `if`, a `!heap`/reversed ternary, `void *`/`u32` casts, a temp, and a static
inline helper. Reusing one variable (`h = ternary; h = audio_helper(.., h, ..)`) keeps the extra move, but the
phi goes to `t3` and the remaining ring is shifted, so 19 words differ. Reusing `heap` itself is much worse.
`audio_dma_sync` has the same residual, so it is one open problem shared by both.

## func_800E2D18 (arcade `enginetorque` tail, drivetra.c)
This is the arcade torque lookup with RPM_SCALE 1150. It interpolates a `short[10][12]` curve over
`rindex = rpm/1150` and `tindex = throttle/14`, then returns
`(s16)(right * (m->unk4->unk8 * D_801110C4[m->unkC][m->unkB]))`. The `(s16)1150` divisor gives the
retail break/overflow checks, while `/14` and `/1149` have none. The structure is right. The residual is
register allocation: retail keeps `m` in a0 and `torquecurve` in a3 with no stack frame. Every candidate
spills one or both and gets a 32–40 byte frame. Retail colours rindex→t0, rrem→v1, tindex/left→a1,
row(tindex*12)→a2, trem→t1. Nothing helped: declaration order (permutations), pointer vs array vs row
pointer, a `tindex*12` local, interp as a macro, -O1/-O3. `&torquecurve[...]` gets closest. workbench
also flags operand order (`addu t9,a3,t8`, and `mul.s f16,f8` meaning (float)right first).

## Blocked: slot_state_setup IPA callers
D9058, EF288 and DD0C0 all call `slot_state_setup` with its argument in `s2` (`jal; li s2,N`) and then do
the dead `move s0,v0`. None of the 667 locked functions calls slot_state_setup, so this is the
project-wide open problem (see cloud/work/bigfish, game_C104). D9058 and DD0C0 also write s4/s5/s7
without saving them, so they are IPA-internal statics of larger groups (callers: func_800D91A0 and
control_settings). They cannot be matched alone.
