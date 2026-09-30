# entity_spawn_init (0x8008EA10)

| item | value |
|---|---|
| words | **1386** (blob_8008d0c0.s) |
| spliced? | No. (`src/game/game.c:8631` has an unrelated old stub `entity_spawn_init(void*, s32)`, not compiled into the blob.) |
| ABI or IPA | **ABI.** Frame 728, saves s0-s8, ra, f20/f22, reads only a0-a3 (4 args: car index s16, type, flag, a3). No unsaved s-regs. Callers `func_80090308`, `buffer_swap`, `cpak_read` (callers.py names; symbol names are unreliable). |
| callees | 7 distinct, 9 jal: `func_8008E26C` x3 (75w, ABI, 15 other callers), `func_8008E0B8` (35w), `func_8008E3C0` (18w, **already spliced**), `func_8008D870` (26w, **already spliced**), `func_8008E408` (386w, only caller is this function), `model_transform_setup` (101w), `model_data_load` (93w) |
| globals | 25 lui targets. Car table (stride 0x3B8, `0x80152818`; `0x8014A250` stride 0x808) and 6 words copied from `0x8011747C..8011749x` into stack locals at entry (looks like an initialized local array). |
| shape | Car/entity **spawn initialization**: 549 FP instructions (many `mtc1` literals 0.5/2.0/0.75/...), a **jump-table switch** on the type (`sltiu at,t3,8; jr t3` at 0x8008F0E8, table `0x80123970`, 8 entries), mostly straight-line field initialization with scaling by `f22`. 32% repetition. Semantically arcade `InitCar`/`init_car_physics`-like, so the arcade source should give names and order. |

## First pass
- m2c **fails**: "Unable to determine jump table for jr at line 477". Needs the 8 jtbl targets (rodata is not in the repo; recover targets by reading the case bodies) or a hand seed. No compile, no closeness number.
- callee prerequisites: `func_8008E26C`, `func_8008E0B8`, `func_8008E408`, `model_*` unmatched.

## Feasibility: LOW-MEDIUM
For: ABI, callees mostly small and shared; arcade reference exists for the semantics; no IPA.
Against: a `switch` with a rodata jump table (table lives outside .text, only relocation-verifiable with `--allow-unverified`; IDO emits `sltiu`+`jr` only if the case set is dense and layout of cases must match); huge float initializer tail (dozens of literal loads whose order is the scheduler's); a 386-word private callee that also has to match; 6-word local array initializer from rodata/data; no seed yet.

## Recommended approach
1. Hand-seed: get the arcade car-init source (reference/repos/rushtherock, not in the cloud tree; a maintainer can supply the function), derive the case bodies from the asm.
2. Match `func_8008E408`(386w) and `func_8008E26C`(75w) separately first: they are independent single functions, likely -O2, so they may be Lane B material.
3. Then the body top-down; the switch last.

## Effort
4-7 days including the 386-word helper. Not recommended as the hail mary.
