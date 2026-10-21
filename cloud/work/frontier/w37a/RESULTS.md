# w37a — Controller Pak system (MAT3 paksysdc.c), wave 37

Lane started 2026-10-21. Builder scratch: watchman2:~/rush2049/scratch/frontier/w37a (from base @ c12837976).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| track_render_process | PS_PakServer@paksysdc.c (N64 osPfs form; confirmed: gInited, changed[4], gErrPak, dup/same, ClearLL, _RemovePak/_ClearPak, GC statement order) | 3200 | NOT MATCHED: unit 706/800 words differ, 794 vs 800 words, frame 432 exact, every call/lock slot aligned (with a disclosed 3-statement umerge-budget device); residual colour/split/schedule | groups/w37a_paksys/group.c (body also pak/server_best.c) |
| PS_CreateFile | PS_CreateFile@paksysdc.c | 1752 | PROVISIONAL: unit EQUAL 438 words with the REAL callers (no stand-in); waits on PS_PakServer | groups/w37a_paksys |
| no_catchup | one-parameter InsertLLE helper | 56 | PROVISIONAL: unit EQUAL 14 words (internal), 2 real callers | groups/w37a_paksys |
| func_800A3640 | _RemovePak@paksysdc.c | 204 | PROVISIONAL: unit EQUAL 51 words (internal) with real caller; group compile 29/51 (IPA register set differs) | groups/w37a_paksys |
| func_800A3724 | _ClearPak@paksysdc.c | 88 | PROVISIONAL: unit EQUAL 22 words (internal) with both real callers; score.py group MATCH | groups/w37a_paksys |
| car_lod_select | PS_InitPakSys@paksysdc.c | 324 | NOT MATCHED: 19/81 words (list/end address webs s3/s4 swapped + head load in the jal delay slot) | groups/w37a_paksys (pak/initpak_v2.c) |
| pak_delete_file_retry | PS_DeleteFile | 784 | not attempted this wave (w36v best: 63 rows) | w36v/pak_delete_file_retry/best.c |
| drone_set_catchup | PS_ReadPartialFile | 852 | not attempted this wave (w36v best: 114 rows) | w36v/drone_set_catchup/best.c |

## Log

- v1/v2 (pak/v/v1.c, v2.c): one file = w36v PS_CreateFile group (types, helpers, PS_CreateFile unchanged) + _RemovePak
  (w8b body, osSendMesg) + _ClearPak + PS_PakServer (PR #410 skeleton, w36v types, MAT3 names/order) and no stand-ins.
  `blob_unit --tag w37a score track_render_process PS_CreateFile no_catchup func_800A3640 func_800A3724 --with
  cloud/work/frontier/w37a/pak/v/v2.c --internal func_800A1E94 --internal no_catchup --internal paksys_queue_init
  --internal paksys_lock --internal paksys_unlock --internal pfs_get_node --internal func_800A3640 --internal
  func_800A3724 --internal _FileChanged` -> `FAIL track_render_process: 771 of 800 words differ; compiled body is 811
  words`, `EQUAL PS_CreateFile: 438 words`, `EQUAL no_catchup: 14 words`, `EQUAL func_800A3640: 51 words`,
  `EQUAL func_800A3724: 22 words`.
  Finding: retail's first lock in PS_PakServer is a `jal func_8008A704` (the public pak lock, frontier_rumble_thread);
  the other six locks are inlined copies. Ours inlines it (umerge budget not exhausted: our caller is smaller).

- Levers found for PS_PakServer (in order; each measured in the unit):
  1. No `pak`/`other` pointer locals: every record access `D_80144030[i].x` / `D_80144030[j].x` (volatile array, as
     PS_CreateFile): frame 440 -> 432 equal, &changed[i] placement exact (v3).
  2. `if (!flushing) { bitmap loop; same = 1; goto flush; } continue;` gives retail's dead `b next` block after the goto
     and the direct `bnez -> next` branch; GC-style positive `if (i != j && inited && !memcmp(...)) { ... move: ... break; }`
     for the other-port loop; `if (file_size) {...}` in the directory loop (v5).
  3. GC `ClearLL(&gPS_Files[j])` = locked func_800A370C, inlined (retail's move block stores head/tail/count) (v7).
  4. The first rumble-clear (rescan branch) is an inlined one-site helper (`_StopRumble(i)`); the second is inline
     code: this gives retail's +8 inline area above lock1 (all six lock message slots then equal retail) (v9).
  5. `s8 status = 0; s8 retry = 0;` initialisers (prologue store order), `dup = 0; same = 0; memcpy(...)` on one
     line (as1 line tie: spill stores before the two byte stores) (v10/v11c).
  Open: (a) umerge still inlines retail's first `jal func_8008A704`: retail's caller is 3 compiled-out statements
  bigger at the IR level (probe `if (forceUpdate) {}` x3 stops the inline: frame/locks/calls then align);
  (b) rescan branch: retail reloads &changed[i] on each edge into the `damaged = 0` block (4 words);
  (c) same-pak loop: psfileh t0 / bitmap pointer a3 swapped; (d) t-ring rotations in the move block.

## Deliverable: groups/w37a_paksys (PROVISIONAL, claims empty)

Files: group.c (types + func_800A1E94 + lock helpers + no_catchup + pfs_get_node + PS_CreateFile + _RemovePak + _ClearPak
+ _FileChanged + _StopRumble + PS_PakServer + PS_InitPakSys), ctx_ll.c (byte copies of locked func_800A370C ClearLL /
func_800A44E8 InitLL), ctx_lock.c (byte copy of locked func_8008A704 + its static init).

Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w37a score track_render_process PS_CreateFile no_catchup
func_800A3640 func_800A3724 car_lod_select --with cloud/work/frontier/w37a/groups/w37a_paksys/group.c --internal
func_800A1E94 --internal no_catchup --internal paksys_queue_init --internal paksys_lock --internal paksys_unlock
--internal pfs_get_node --internal func_800A3640 --internal func_800A3724 --internal _FileChanged --internal
_StopRumble --neighbours` ->
```
  FAIL track_render_process: 706 of 800 words differ; compiled body is 794 words, target 800
  EQUAL PS_CreateFile: 438 words (kept, c_group.c)
  EQUAL no_catchup: 14 words (internal, c_group.c)
  EQUAL func_800A3640: 51 words (internal, c_group.c)
  EQUAL func_800A3724: 22 words (internal, c_group.c)
  FAIL car_lod_select: 19 of 81 words differ
  locked bodies that differ in this unit: 0
```
Builder `score.py group cand/groups/w37a_paksys`: PS_CreateFile MATCH, no_catchup MATCH, func_800A3724 MATCH,
func_800A3640 29/51 (s3-s7 vs s1-s6: the group compile colours PS_PakServer differently from the unit, 779/800 there,
so IPA hands _RemovePak another register set), track_render_process 779/800, car_lod_select 19/81; context
func_800A1E94 / func_800A370C / func_800A44E8 / func_8008A704 MATCH. To install as a bare group later, the group compile
needs more of the unit's IPA context (PS_PakServer's internal callees in the unit: func_80091FBC, func_8009211C,
func_800A1910, func_800A1644, PS_FlushPak ...) or the unit-based install path.

Not spliceable yet: PS_PakServer (the real caller that keeps no_catchup out of line and fixes _RemovePak/_ClearPak's
private registers) is not matched. Owner decision possible: accept the PS_PakServer reconstruction as unmatched
caller context (input_process_controller precedent) -> claims = PS_CreateFile, no_catchup, func_800A3640,
func_800A3724 (2,100 B) with overrides force_internal func_800A1E94 / no_catchup / func_800A3640 / func_800A3724,
prefer_definition no_catchup (one-parameter body replaces the locked two-parameter single; same 14 words), revert
single no_catchup, retire provisional.json `func_800A3640`.

## PS_PakServer residual (for the next lane)

With the 3-statement device (umerge budget) every call, lock-message slot, spill slot and the frame (432) equal
retail; 794/800 words. Left:
1. Rescan branch: retail reloads &changed[i] (124(sp)) on each edge into the `damaged = 0; changed[i] = 1;` block
   (`b 0x1ac; lw t6,124(sp)` edge blocks, 4 words); ours reloads inside the block. Condition spellings (&&, !(||),
   nested ifs, changed/damaged order, else-if vs if after continue) do not change it: a live-range split decision.
2. Goto-move path (+0xa58..0xab0) and directory loop (+0xb20..0xb68): PRE/schedule placement of the move-block
   address spills and of the name/ext pointer spills (2 words).
3. Same-pak bitmap loop: psfileh t0 / bitmap pointer a3 swapped (ours a3/t0).
4. j-loop and move block: t6-t9 ring one phase off from +0x754 (after the dead `continue` block).
5. The umerge budget: without the device umerge also inlines retail's first `jal func_8008A704`. Probes: dead stores
   (`X = 0` x12) do not count; compiled-out `if` blocks do; probes in another function do not (per-caller budget;
   umerge has estimate_inline/threshold/pack_knapsack). Retail's PS_PakServer has some branch structure we lack.
6. `s8 X` (sp+311) is unreferenced in retail; its identity is unknown (disclosed).

## Identities proven
- track_render_process = PS_PakServer@paksysdc.c (N64 form): two params (forceUpdate unused, rumbleOnly =
  suppress the motor-init error); callees func_8008A704 = public pak/SI lock, func_800A3640 = _RemovePak,
  func_800A3724 = _ClearPak, func_800A370C = ClearLL (inlined, move block), func_800A1910 = memcmp (_SamePak test),
  PS_FlushPak, func_800A1E94 = osPfs error -> kPS_Err conversion.
- car_lod_select = PS_InitPakSys: func_800A44E8 = InitLL(list, char handle, char dbl) with UNSIGNED char params
  (MAT3 `char`; an s8 prototype shares gInited's constant 1 with the InitLL web and drops retail's `li t8,1`).
- gPaks (D_80144030) N64 772-byte record: +0 changed, +1 inserted, +2 present, +3 wasRemoved, +4 inited, +5 damaged,
  +6 rumble, +7 flushing, +8 flushed, +9 original, +10 locked, +11 flush, +12 OSPfs, +116 error, +120 FreeSpace,
  +124 motorOn, +125 request, +126 level, +128 phase (s16), +132 files[16] (40 B: flush, flag1, flag2, mapBytes +4,
  OSPfsState +8).
- Small pak getters (singles, for the namers): func_800A361C = PS_PakChanged(pak) (+0), func_800A35F8 = damaged (+5),
  func_800A3598 = FreeSpace, func_800A35BC = (FreeSpace+255)>>8, func_800A3508 = PS_GetBlocks (n+255)>>8,
  func_800A3518 = PS_GetFreeFiles (count of empty file slots), func_800A44D0 = gErrorFunc setter, func_800A44DC =
  gInited = 0 (PS_ExitPakSys?).

## What generalises
- A locked public function called by `jal` from a big caller while its other sites are inlined copies is the umerge
  per-caller budget running out at the FIRST call site (umerge processes sites last to first). If our caller
  inlines it, our caller is smaller than retail at the IR level; dead stores do not count, compiled-out `if`s do.
- Frame-top-relative comparison of every inline-area local (the lock messages here) tells which region lacks an
  inlined site: here +8 above the first lock (a one-site helper) and +8 between locks 4 and 5 (GC's ClearLL call).
- `if (c) continue;` vs `if (!c) {...}`: IDO emits a separate `b next` block for a mid-body continue; retail's direct
  branch to the loop increment means a positive if (or the continue placed after a goto, which leaves a dead
  `b next` block that as1 threads to).
- MAT3 `char` parameters are unsigned in IDO: constant webs for the arguments do not share with s8 stores.

## Permission denials
none
