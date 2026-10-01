# Lane 1 worker B — bounded residual probes (2026-10-01)

No strict matches. All five packet targets were absent from `blob_matched.lock.json` at start.
Private work: Rocky `~/agents/B/scratch/codex_B`; scorer repo `~/agents/B/wt`.
Flags throughout: `-g0 -O2 -mips2 -G 0 -non_shared`. `tools/cloud/score.py` supplies
`-Wab,-r4300_mul` itself. No integration, lock/source/layout changes, commits, or pushes.

Read the handoff, CLAUDE.md, cloud playbook/toolkit handoff, workbench START_HERE,
near-miss verdicts, hand notes, and pilot C1/C2/W1. Each baseline was run through
`~/agents/wb/loop.sh` before probing. Avoided known exhausted line reflows,
parameter/declaration permutations, generic dead-read grids, and autopilot reruns.
The cloud toolkit's measured low remaining yield argues for bounded mechanism probes.

| Target | Original strict baseline | Best strict result | New variants | Result |
|---|---:|---:|---:|---|
| `func_800CCE5C` | 15/40 | 2/40 | 6 | not matched |
| `func_8008A704` | 2/28 | 2/28 | 4 | not matched |
| `state_update_global` | 7/28 | 3/28 | 24 | not matched |
| `func_8008C680` | 4/40 | 4/40 | 9 | not matched |
| `func_800D18D8` | 10/41 | 8/41 | 19 | not matched |

The new-variant counts include rebuilding the improved pilot baselines where applicable.
Everything was compiled and scored serially; no more than one compute subprocess ran.

## func_800CCE5C

Start diagnose: frame -48 versus -40, mixed 13 displacement/constants and 2 register
words. Pool identical 18/18; final temp target t6 versus candidate t0.
Reproduced the pilot's removal of pad, pad2, and named sp1C; repeating sp24+0x3C
makes the frame and homes exact: 15 -> 2 strict words.
Five alternatives for the final M2C_FIELD: parentheses, pointer +0, pointer cast
through u32 and redundant full mask, byte-pointer dereference, indexed pointer
view. All remain 2. No new improvement.

Tool: frame guidance correctly moves the baseline. Temp-ring diagnosis should
be read as a width reservation difference, consistent with W1's prior group
experiments; these five spelling probes do not change that reservation.
Existing best full TU: `cloud/work/workbench_pilot_W1/func_800CCE5C/best_2words_single_file.c`.

## func_8008A704

Start diagnose: schedule mismatch, 2 words, identical pool/temp lanes.
Only early li t7,1 versus sw ra ordering remains. Prior 1,024-layout sweep and
23 source shapes are documented in pilot C1, so did not repeat them.
New TU probes: define D_8011194C, define volatile, declare extern volatile,
explicit initialized definition. All 2/28: this class of context change does
not put the immediate in the predecessor block.
Tool correctly identifies scheduling; -g0 is already active. The useful prior
L79 node-count explanation remains unchanged. Stop rather than repeat reflows.

## state_update_global

Start diagnose: 7 register words, pool v0/v1 swap and identical temp lane.
Rebuilt pilot C1's assignment-in-condition shape: 3/28, where the raw global
load gets a1 instead of target v0. Saved full TU in
`near_miss_B/state_update_global_best.c`, explicitly a NONMATCH.

Six condition variants: reversed assignments 7; subtraction 7; xor 6;
reversed double-negation 7; unsigned >0 26; ternary bool 25 plus 3 extra words.
Six dead-cost uses before the assignment condition: v&255/v+1/v==0 all 9;
t+1/t==0 both 13; arg0 guard inert at 3.
Eight t/v type probes: t u8=27, v u8=5, t s8=26 plus 1 extra, v s8=27 plus
1 extra; u32/s32 remain 3 for both variables.
Define the global instead of extern: 3, inert; defined/extern volatile: 27.
No improvement on the previously known 3-word form.

Tool: correct lane/mechanism family, but assembly alone still does not expose
what would reduce a1 to v0 while retaining the correct temp web. Not claiming
a fundamental impossibility. These bounded probes exhausted only these families.

## func_8008C680

Start diagnose: 4 register words, fp ring f4 f6 f8 f10 f16 f18 in candidate
versus four-wide target f4 f6 f8 f10 f4 f6. This is the pilot's measured ring
width problem; avoided its already exhausted phase, literal, and flag sweeps.
Nine TU probes: define each of E4/E8/F0 as f32 (all 4), f64 (39/28/10+1 extra),
or volatile f32 (35+1 extra/36/11). Context definition does not withdraw
f16/f18 without changing emitted instructions.
Tool's generic phase lever is insufficient for this width difference; prior
cc -K evidence establishes that f16/f18 here are ugen temps, despite lane labels.

## func_800D18D8

Start loop strict 10/41; diagnose reports 8 aligned/raw words plus FOUR differing
relocation symbols. Always used the strict scorer's 10, not the masked number.
Drop pass-through sp1C: strict 8/41; spill is still 24(sp) versus target 28(sp),
and address webs for D_801460F8/D_80146104 are crossed.
Nine individual global-definition/type probes (each of F8,104,258 as s32/u32/
volatile s32): all 8 except volatile 258=30.
All three defined together: s32/u32 both 8, volatile s32=31 plus 1 extra.
Six unused pad declarations before temp_a1 (u8/u16/s32/u32/s64/f32): all 10.
No improvement; definitions alone do not explain the address-web ordering.
Tool's relocation warning is valuable. Do not conflate its 8-word masked baseline
with the strict baseline; after dropping sp1C strict also becomes 8.

## Artifacts and next use

`near_miss_B/probe.py` and `probe2.py` recreate the bounded variant grids on Rocky
using the documented private paths. `final_probes.json` records the second grid;
first-grid counts/results are captured above. Scripts are research artifacts,
not pipeline tooling. Neither requests parallel work nor invokes generic search.
All remote sources remain in `~/agents/B/scratch/codex_B`.

A real context group or calibrated allocator reservation evidence looks more
promising for the width targets than more repeated single-function spellings.
No image or ROM verification is appropriate here because no strict match exists.
