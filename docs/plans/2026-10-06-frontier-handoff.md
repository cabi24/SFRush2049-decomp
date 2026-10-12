# Frontier handoff: 2026-10-06 (after wave 12), updated 2026-10-11 (after wave 15)

## 0. Wave 13–19 update (2026-10-12, after wave 19; read the checklist at the end of §0 first)

Read this section first; §1–§10 below are the wave-12 text and still describe the method. Section-by-section
deltas are noted inline where they matter.

| | Functions | Bytes | % |
|---|---|---|---|
| Game code (compressed image) | 973 / 1216 | 310,928 / 647,072 | 48.05 |
| Static code (boot/library) | 550 / 668 | 103,172 / 160,720 | 64.19 |

Frontier: 244 unmatched game functions (251 KB) after wave 17. Provisional: 20 functions (`provisional.json`).

**What landed since wave 12 (+19.9 KB game):**
- Wave 13: records_screen, func_800E762C, differential_output (+1.2 KB); wave 14: graphics_chunk,
  place_cars_in_order (+1.1 KB).
- Wave 14 tail (lanes whose sessions ended before reporting; recovered 2026-10-10): entity_lod_select (716),
  wheel_torque_apply (140).
- **Mode-select cluster** (w13a bodies, w15i packaging): frontier_mode_select, 10 functions, 8,400 B. The group
  needed the unit's IPA context (codex_transform_b109's group.c with the duplicate `entity_hierarchy_update`
  reduced to a prototype, plus byte-identical func_8008B2E4.c and player_conditional_check.c) — see WAVE_BRIEF
  "Group packaging". No unit_overrides were needed.
- Wave 15: frontier_billboard_render (billboard_render + provisional func_800F207C/func_800F1930, 3,640 B;
  prefer_definition func_800F1210), audio_channel_setup (332), func_800A79F4 (240), func_800AC9BC (224),
  func_800E7A98 group (172), physics_sym group (2,836).

**Method change (wave 15): trace, don't sweep.** Seven of eight matching lanes closed something after wave 14's
28 mass-variant lanes closed two. The lane rules file now says so; the lessons are in WAVE_BRIEF "Wave 15".

**physics_sym group (landed 2026-10-11, +2,836 B):** physics_sym (1,252), func_800B5688 (196), func_800B59F0 (1,372)
and the stubs func_800B55F4/func_800B59E8, all with real callers, **no implicit int** (§6 item 2 is moot for it).
func_800B59F0's own .rodata lies in two retail regions (strings 0x801226A8, floats 0x80123DC4); w15j added a
fail-closed `_independent_regions` proof to `blob_group.py` (whole-window byte evidence inside non-function image
runs, no overlaps) that only replaces owndata's one-base refusal. `owndata.py`/`score.py` are unchanged because
two receipts pin owndata's hash, so `score.py` still prints NOT VERIFIED for it; the alternative diff is in
`w15j/owndata_alternative_not_applied.diff`. w6d's pause_quit is probably the same case (untested).

**Open near-misses after wave 15** (replaces the §6 table where names overlap):

| Target | Bytes | Gap | Cause and best source | Lands with it |
|---|---:|---|---|---|
| camera_update | 2876 | 5 words (was 17) | two do-loop compares: retail loads `sc->count` before forming `idx+1`; uopt canonicalises by operand kind; retail probably keeps `ctl->idx` in v0. `w15a/RESULTS.md`, `w15a/groups/camera_aspect_ratio/` (work base, claims []). | camera_free_look, camera_track_spline (EQUAL), ≈4.2 KB |
| entity_spawn_init | 5544 | 61 words (was 1356), frame 728 ok | function top: `surface` only in memory, model address in t1 before the loop; two spill slots 4 B off. `w15e/entity_spawn_init/best.c`. Disclosed ~52 words of unused locals not yet proven exact. | unlocks 3 / 3.6 KB |
| camera_play_script | 3520 | 48 normalised rows (was 133), frame 600 vs 608 | -1 constant ties at 30.0 benefit vs cost (t5); one temp slot missing; 0.0f reloads. `w15h/camera_play_script/best.c`. | |
| camera_victory | 1468 | 250/367 words, frame ok | new full draft; `car` needs s4 across func_800C3AD0/func_800C36A0. Would supersede locked func_800AD4C8 group. `w15f/camera_victory/best.c`. | func_800C3AD0 (1,448 B provisional) |
| entity_process_main | 1424 | 2 words | ugen temp order shift, load, NOT — no C expression reaches ugen in that order. `w15c`, `w14o`, `w12b`. | |
| func_80096130 | 264 | 3 words | retail splits `slot`'s register lifetime once (explains `.alias $3,$sp` and the 32(sp) slot). Needs a "split once" force mode to prove. `w15c`. | |
| func_8010FBE0 | 128 | 6 rows | shared `lui at` only when `D_80155238` is defined in the unit; address web must be split. `w15d`. | |
| input_deadzone_apply | 3580 | 122 words | ugen double compile, w12a. Untouched since. | input_process_controller |
| func_800E847C + func_800E7FA0 | 2100 + 1244 | far | w11g | |
| func_800AC9BC | — | **landed** w15d | | |
| audio_channel_setup, func_800A79F4, func_800E7A98, differential_output | — | **landed** | | |

**Provisional unblockers, ranked by bytes closed** (w15f `prov_callers.py` replays this): func_800E4B58 (2,268 B,
closes 2,128 but is 488/567 off), func_800D91A0 (3,860 B, closes 1,572, 603 rows off), camera_victory (above),
audio_mixer_main (31/183 across four waves). The F857C / audio_update_d / finish_state_alt trio does NOT close via
func_800F87A0 alone; finish_state_alt's real caller is `countdown` (needs game_loop's tree).

**Static side (w15g survey, nothing promoted):** ~4.8 KB recoverable as C from vendored ultralib/libgcc at the right
flags (libgcc needs `-mips3 -32`; libultra is the "J or later" variant; several static labels sit on the wrong code,
e.g. `osEPiRawStartDma` is really `__osEPiRawWriteIo`). Another 4.2 KB is byte-identical hand asm (policy needed).
Table in `cloud/work/frontier/w15g/RESULTS.md`.

**Owner decisions, 2026-10-11 (the owner delegated the open judgment calls to the coordinator):**
- Merged remote branches: deleted (213 branches, `--merged origin/master`, open PR heads excluded). 172 remain.
- `.git/gc.log`: cleared; `git prune` + `git gc` run (0 loose objects, 1 pack). Builder scratch for waves 3–12
  removed (10 GB freed; `~/rush2049/scratch/frontier` keeps base, wtk, unit, wholeprog and w13+).
- Unused locals: **no numeric cap**. The standing rule stands (exact frame residual, each disclosed with the frame
  arithmetic). billboard_render's eight stay.
- `owndata.py` alternative (w15j): **not applied**. The blob_group proof is fail-closed and tested; `score.py fn`
  printing NOT VERIFIED for split-rodata own data is expected and documented in the wave-16 rules. Revisit only if
  a single-function splice (not a group) ever needs it.
- PR #163 and the new drafts #305–#308 (texture rectangle, camera_transform research) are Astra's; untouched.
  **camera_transform is added to the keep-off list** (§4).
- Implicit int: still open; no landed function needs it.

**Wave 16 (2026-10-11, eight Opus matching/tooling lanes plus w16i tooling, trace-first). Landed:**
- **Game +9,308 B:** camera_victory (1,468) with provisional func_800C3AD0 (1,448) in the superseding `func_800AD4C8`
  group (w16d; levers: loop-2 inner test as `continue` so uopt's split start block skips the call block and `car` keeps
  s4; one `j` across three loops; sibling idiom `cnt = *q++` from camera_trigger_check; no shaping devices);
  camera_update (2,876) with camera_free_look (436) and camera_track_spline (860) in the
  superseding `camera_aspect_ratio` group (w16a; lever: uopt orders `==` operands by kind, so `idx * 1 + 1 ==
  sc->count` puts count first; disclosed, plus w15a's two compiled-out reads; `D_80123E8C` is the literal 0.0425f);
  singles func_80108F40 (1,320; three unused locals = exact frame residual, disclosed) and func_800F45F8 (900) (w16h).
- **Static +4,248 B, 31 functions (w15g survey → w16g sources → w16i tooling):** `__osPfsRWInode`, `__osContRamWrite`,
  `__osEPiRawWriteIo`, `__osEPiRawReadIo`, `__osContAddressCrc`, `__osContDataCrc`, `osMotorStop` (= osPfsRepairId),
  `__osSiDeviceBusy`, `func_800103A0`, `func_80014650`, `func_8002517C`, `__osResetGlobalIntMask`,
  `__osSetGlobalIntMask`, `osYieldThread`, `osDestroyThread`, and the 16 libgcc-named functions, which are ultralib
  `libc/ll.c` + `llcvt.c` verbatim at `-g0 -O1 -mips3 -32`. Ten segments converted. New tooling:
  `tools/conveyor/pipeline/targets_rename.py` (inventory rows re-registered under current layout names; applied to
  the live DB, 18 actions, backup in the coordinator's scratchpad), `write_opt_overrides` `-mips3 -32` rule and
  `tools/conveyor/pipeline/o32abi.py` (EF_MIPS_32BITMODE in the ROM-TU Makefile rule). Static promotion is run with
  `cloud/work/boot_tail_promotion/promote_batch.py CTX.jsonl SEG… --only fn,…` after `lock add` + `layout convert`
  (lock first: the segment flagset derives from the lock). Still open on the static side: `osAiSetNextBuffer` (needs a
  16-byte owned `.data` slot), `__fixunsdfdi`/`__fixunssfdi` (owned `.rodata` + reloc-aware targets), two stale
  inventory rows (`__osGetCause` → osCauseGet, `__osContAddressCrc` insn_count), the hand-asm policy question
  (`w16g/RESULTS.md`), and the wrong labels listed there (notably `__osActiveQueue`/`__osRunQueue` swapped).
- **Provisional:** pause_quit (= drone_throttle_calc) passes the w15j own-data gate (w16f dry run) but its only caller
  func_800D91A0 is unmatched; init_state_continue is code-identical with asm-only callers (w16f).

**Open near-misses after wave 16** (replaces the wave-15 table where names overlap):

| Target | Bytes | Gap | Cause and best source | Lands with it |
|---|---:|---|---|---|
| camera_play_script | 3520 | 6 normalised rows (was 48), frame 600 vs 608 | -1 constant tie broken by block count (18 → 19 blocks flips float rounding of benefit = cost); zero spelled two ways; `old` after `n`. Left: corner-loop register (a1 blocked by an unknown web), k-loop denominator in $f22 without merging `u`, two spill slots. `w16c/camera_play_script/best.c`. | |
| entity_spawn_init | 5544 | 61 words, frame ok | decision order: the model address ties a constant web at priority 0.6667 and is decided too early, so its top piece is never coloured; `surface` memory-only; a missing early expression value (two spill slots 4 B off). Split logger `w16b/tools/splitpatch.py`. | 3 / 3.6 KB |
| anim_state_update | 1840 | 39/460 words (21 rows), frame ok | `lucent` start in a0 (retail) vs ra; context copy inside locked `codex_visual_tire_module_20261002` (replace under the same name). `w16h/anim_state_update/best.c`. | |
| entity_render_setup | 2048 | 394/512, frame 184 vs 200 | camera pointer substituted away; palette base and loop pointer timing. `w16h/entity_render_setup/best.c`. | |
| func_800E4B58 | 2268 | 503/567 words (106 → 70 rows) | arcade MaxPathControls with five helpers inlined; blocked by func_800E398C's convention (parameter in s7, clobbers s0–s8/$f20–$f30 unsaved). `w16d/func_800E4B58/best.c`. | func_800E451C, func_800E4300 (2,128 B) |
| audio_frame_update | 600 | 21 words | unknown web holds v1 over the final loop; `D_801543CC` hoisting. `w16f/audio_frame_update/best.c`. | |
| func_80096130 | 264 | 3 words (needs one disclosed `if (slot) {}`) | retail's `slot` tail is a register piece after osJamMesg; any extra use crosses the split threshold. Needs uopt `f_split` reading or a "split once" force mode. `w16e`. | |
| func_8010FBE0 | 128 | 6 rows | defining `OSScTask D_80155238;` in the unit is supported (func_800B2BDC precedent) but the symbol address then lives in a3/v0. `w16e`. | |
| entity_process_main | 1424 | 2 words | ugen shift/load/NOT order; no expression tree reaches it. | |
| input_deadzone_apply | 3580 | 122 words | ugen double compile, w12a. Untouched. | input_process_controller |
| func_800E847C + func_800E7FA0 | 2100 + 1244 | far | w11g | |

Provisional unblockers: func_800E4B58 (above), func_800D91A0 (closes 1,572 B + pause_quit 1,024, 603 rows), audio_mixer_main.
Wave 16 lesson (w16d, w16b): uopt's live-range split start block follows source-visible block order; a private
instrumented uopt with skip/reject oracles proves the lever before searching for the source form.

**Wave 17 (2026-10-11 afternoon/evening, nine Opus lanes + w17j packaging + w17k tooling). Landed, game +9,524 B:**
- **frontier_maxpath_controls** (w17b, 6,792 B): func_800E4B58 MaxPathControls, func_800E398C avoid_areas, the
  former provisionals func_800E451C and func_800E4300, six inlined arcade helpers (locked stubs reverted, now
  members). Levers: float-literal spelling is a constant-web key (`0`, `0.0f`, `0.f` are three constants), arcade
  integer literals, one-line vector macros for store order, definition order for as1 ties. The blamed callee
  convention was not the blocker (a correct-convention stand-in changed nothing).
- **anim_state_update** (w17c, 1,840 B) in the superseding codex_visual_tire_module_20261002 group: `lucent` is a
  memory variable fed from a reused `show` local; model_bounds_calc's real parameter order is the arcade
  `CheckVisible(v, on)` (the func_8008B640 group superseded under the same name, prototype/definition/call only).
- **codex_heap_release_a25** extended (w17h + w17i, merged by w17j, 892 B): func_800C8918 (slot-queue lock/unlock
  stubs func_80091AF0/AF8 given bodies) and func_80096130 (early return ends uopt's not-the-stack marker). Packaging
  lesson: an inlined `static` helper's deleted-procedure stub has no symbol and lands after a member → "extra words";
  make the helper a non-static internal function (not on `keep`) so its stub carries a symbol.
- **blob_unit fix:** the unit-wide own-.bss extent rule was link-order dependent (locking anim_state_update moved a
  file in the link and func_800F0674's statics then reported an overlap); the extent is now the access width capped
  by the gap. One receipt (dot_billboard_helpers_20261005) rebound.
- **Provisional:** physics_response joined (MATCH in its group; caller audio_effect_apply unmatched).

**Static: the Rush boot tail is IDO 7.1 (w17g finding, w17k tooling, landed 2026-10-11 evening, +9,912 B / 58
functions).** The Rush-specific boot-tail segments (lib_11050 … lib_26ab0, 18 ROM TUs) were compiled with IDO 7.1;
libultra and the 435 earlier locks are 5.3 and unaffected (397/435 re-score exact under 7.1, the 38 that differ are
libultra; all 18 TUs build byte-identical under both, proven by the switch-only ROM gate before any promotion).
Tooling now on master: `tools/conveyor/compilers.py` (pinned 5.3/7.1 builds with fingerprints; `install ido7.1`),
`build_toolkit --swap-ido-from`, `publish-toolkit --no-pin` (7.1 toolkit `4586d230…`, uploaded unpinned),
`lock add --toolkit SHA` (records `compiler: ido7.1`; `lock verify` re-proves with it), `rom_compilers.json`
(segment → compiler; `write_opt_overrides` emits `CC := $(IDO71_CC)`; the Makefile stops if 7.1 is missing, never
falls back), `promote_batch --dry-run/--splice-out/--lockfile` and compiler-mismatch refusals, docs in BUILDING.md
and COMPILER_SETTINGS.md. The builder has 7.1 at `~/rush2049/repo/tools/ido-static-recomp/build71/out` (CI builds
no ROM and needs nothing). **Packets:** eight boot-tail research packets rebuild their ROM TU; they now take the compiler from the pin via `compilers.tu_cc` (receipts rebound), and `tools/cloud/setup.sh` installs 7.1 beside 5.3 for CI. **Gotcha fixed:** `blob_rom rom` synced only the Makefile and blob to the builder, so a
stale `rom_compilers.json`/`opt_overrides.mk` there built every TU with 5.3 and still matched; it now syncs both.
Still open on the static side: osAiSetNextBuffer (owned `.data` slot prepared in `w17g/`), `__fixunsdfdi`/
`__fixunssfdi` (owned `.rodata` + `.set gp=64` for reloc-aware targets), two stale inventory rows (`osCauseGet`,
`__osContAddressCrc`), 9 w17g sources exact under 7.1 but conflicting with TU declarations (`w17g/lockonly/`,
2,532 B), the sched (`__sc*`) older-build family, inflate/lzss (~8.2 KB, no vendored source), the hand-asm policy
question, and the remaining ~115 boot-tail `func_*` near-misses now that the right compiler is known
(`w17g/inv_table.md`: many at true=10–40 were never examined under 7.1).

**Open near-misses after wave 17** (replaces the wave-16 table where names overlap):

| Target | Bytes | Gap | Cause and best source | Lands with it |
|---|---:|---|---|---|
| func_800D91A0 | 3860 | 8 mnemonic rows, 212 word rows, frame 240 exact (w17e, from 603 rows) | fresh reconstruction (N64-only name-entry screen); needs func_800D816C (2,796 B, Astra draft 627/699) in the same group (IPA partner: parameter in t2, clobbers s0–s8); func_800D8154 is `(s8 player, s32 item)` with the player in the stack slot; caller-less func_800DA0B4 is an inlined getter. Remaining: constants 21/1 coloured in the other order (s5/s6). `w17e/d91a0/best.c`, `w17e/base/`. | drone_throttle_calc (pause_quit), func_800D8078, render_results_screen, render_replay_ui (1,572 B) |
| camera_play_script | 3520 | 27 rows, frame 608 exact (w17a, from 48) | `i = idx[0]` through an int variable in an existing slot; `c1` was a fake local; zero spellings; `u` assigned before the compare. Left: corner-loop pointer a1 vs t0 (register only), t0/t1 priority swap, one spill slot 144 vs 148, one mul operand order. `w17a/camera_play_script/best.c`, `w17a/tools/patch_lbb.py` (uopt block dump). | |
| entity_render_setup | 2048 | 68 rows (from 171), frame 184 exact (w17d) | `op;` reproduces the entry `sw a1`; `angle` reused as the world scale; laundered `view` pointer. Left: palette base load placement, `&delta`/`&quad` before the mode branch, draw-loop as1 schedule, world-loop FP temps. `w17d/entity_render_setup/best.c`. | |
| entity_spawn_init | 5544 | 61 words (w16b/w17f: oracle proves 45 of them are two decisions) | the model address ties the constant 5 at priority 0.6667 and wins by number; `surface` must be address-taken (`*(s16 *)&surface` form in `w17f/entity_spawn_init/base_alias.c` costs no statement); the inlining budget is exactly one statement. Needs a code-neutral form removing 2–4 blocks where both are live. `w17f/tools/prio_patch.py`. | 3 / 3.6 KB |
| audio_frame_update | 600 | 22 words (w17i) | shared base via `(player_array + slot)->…` and `cb` before `handle` fixed the loop; retail holds one more value in v1 across the loop (an eliminated second counter?). `w17i/audio_frame_update/best.c`. Supersede group audio_frame_update, keep func_800B0A88. | |
| engine_sound_sync | 1236 | 286/309, frame 376 vs 368 (w17h) | takes the same slot-queue lock as func_800C8918: a frame 8 short likely means a missing nested inline level (caller-less stubs next to func_80091B00). `w17h/engine_sound_sync/best.c`. | |
| func_800D6914 | 1260 | 283/315, stand-in callers (w17h) | provisional lane; `w17h/func_800D6914/best.c`. | |
| func_8010FBE0 | 128 | 6 rows | defining `OSScTask D_80155238;` in the unit is supported; symbol address then held in a3/v0. `w16e`. | |
| entity_process_main | 1424 | 2 words | ugen shift/load/NOT order. | |
| input_deadzone_apply | 3580 | 122 words | ugen double compile, w12a. Untouched. | input_process_controller |
| func_800E847C + func_800E7FA0 | 2100 + 1244 | far | w11g | |

**New diagnostic tools (wave 17):** `w17f/tools/prio_patch.py` (priority oracle: decide one candidate earlier/later),
`w17i/boostpatch.py` (force a value to be considered earlier), `w17a/tools/patch_lbb.py` (uopt block dump),
`w17c/tools/uproc.py`; w17i's RESULTS decodes how uopt ranks register candidates (blocks counted in groups of
four, so exact ties are common).

**Wave 18 (2026-10-11 night):** w18c matched **39 more boot-tail functions under IDO 7.1 (7,080 B)**, including the
nine `w17g/lockonly/` sources, via a `sequence_context.h` naming patch and declaration edits in six TUs (gated alone
first); six use relocation-label views (`docs/reloc_reconciliation.md`, func_8001C19C precedent) and three a disclosed
`& 0xFF`/`& 0xFFFF` mask. Static 489 → 528/668 (59.01%). Not matched: ~20 listed in `w18c/RESULTS.md` with causes
(u8 parameters reloaded from the stack, copy-then-narrow, six that differ only in relocation names the compiler
synthesises). w18d: camera_play_script stays 27 rows; the corner pointer is not a bare argument copy and `pts[ka]`
is weighted about twice `pts[kb]` (w18d/RESULTS.md). The overlay path (w18a/w18b/w18e) is described below.

**Wave 19 (2026-10-12):** static 528 → 550/668 (64.19%): w19b closed four IDO 7.1 idiom cases (copy-then-narrow,
s8 in a coloured register, nested-ternary clamp, switch with a shared tail) and showed six lib_16320 functions were
code-identical and blocked only by relocation names on compiler-made addresses → `reloc_addrs.us.txt` entries (20)
plus a `reloc_split` negative-addend fix, `make extract`, target refresh, promoted (1,928 B); w19a matched nine
larger boot-tail functions (5,448 B; lever: pass `x & 255` to a u8 parameter for 7.1's copy-then-narrow, one result
variable instead of per-path returns, typed globals); w19c landed osAiSetNextBuffer and `__fixunsdfdi`/`__fixunssfdi`
through owned `.data`/`.rodata` slots (`owned_data prepare`, co-owners, `promote_batch` slot promotion) and made
static targets assemble with `.set gp=64` (18 more reloc-aware). Overlay A: Astra's #309/#310 merged, six bodies
spliced (42/193, 14,364 B). Game: w19e matched func_800CF06C (548 B, arcade road(); two unused locals = exact frame
residual 192 vs 168, disclosed) and left three traced near-misses (world_effect_update 12 words = two missing block
boundaries proven by relabelling our own listing; assign_drones 29 words; race_position_update needs two extra
register values); it also corrected three names: world_effect_update is the graphics/heap init, assign_drones the
end-of-race records update, race_position_update a text-width helper. Overlays are IDO 5.3 (0 in-place narrowings vs 147 in the boot tail). Open static
items: func_80021548 and func_80023BDC need owned jump-table slots (entries drafted in w19b §C / w19a); six w19a
relocation/target cases (`w19a/relocname/`, `ownrodata/`); the near-misses in w19a/w19b RESULTS (scheduling-only
residuals under 7.1 remain unexplained).

**Test loop (w19d, 2026-10-12):** `make test-fast JOBS=N` runs both suites under pytest-xdist (strace-derived
concurrency groups in `tests/conftest.py`; the one real hazard is IDO -O3 writing `<source>.u` into the cwd) and
`tools/ci_equiv.sh HOST BUNDLE` (with `CI_FAST=N`) replaces the hand-typed bundle/fetch/run sequence; rocky is set
up as a second CI-equivalent host (both suites in ~2 min; watchman2's ciclone6 remains the fallback). Pi: 3 min
instead of 6–7; rocky: 31 s + 83 s instead of 2:15 + 6:15. GitHub CI is unchanged (xdist there is an owner call).

**Astra / cloud:** see [2026-10-11-astra-cloud-plan.md](2026-10-11-astra-cloud-plan.md). Workstream A landed the
same night (w18a layout/splice/lock, w18b exact stream round-trip + compose, w18e merge): runtime images A/B are a
third and fourth population with their own gates (`ovl_layout build` image gate, `ovl_splice check`,
`ovl_matched.lock.json`, `src/ovl_{a,b}/`, `ovl_rom rom` composing both streams into every ROM build behind the
SHA-1). First splice: overlay A 36/193 (12,856 B), overlay B 8/49 (1,640 B) from the 25 `cloud/matches/ovl_*`
singles and the 12 claimed packet groups. `make progress` reports them as separate rows. Landing an overlay
match: `ovl_splice revert`/`revert-group` if superseding → `ovl_splice single|group` → `ovl_layout build` +
`ovl_splice check` → `ovl_rom rom` (w18e RESULTS has the sequence; a fresh checkout runs `ovl_image extract;
ovl_splice rebuild; ovl_layout build` first). Workstream B (brief rewrite, `cloud/work/CLAIMS.md`) is done;
C (packet protocol v2) is in the brief for Astra to adopt.

**Coordinator checklist for wave 20 (written 2026-10-12 before a context compaction):**
1. State: master `0b3daa0c` (or later), CI green. Static 550/668 (103,172 B, 64.19%); game 973/1216 (310,928 B,
   48.05%); overlay A 42/193 (14,364 B); overlay B 8/49 (1,640 B). Lockfiles: `matched.lock.json` (554 incl. 112 IDO
   7.1 entries), `blob_matched.lock.json` (973), `ovl_matched.lock.json` (50). Conveyor DB inventory renamed
   twice today (targets_rename; backups in the session scratchpad only, which is gone after compaction: take a
   fresh `cp ~/.conveyor/conveyor.db` before any further `targets_rename apply`).
2. Session start: `git status` (expect only the `tools/mips_to_c` submodule mod), check untracked
   `cloud/work/frontier/w*/` for unreported results, `python3 -m tools.conveyor.pipeline.frontier scan && … report`,
   `make progress`, read `cloud/work/CLAIMS.md`, `gh pr list` (Astra: #311 open, #163/#305–#308 drafts).
3. Integration recipe (fast loop since w19d): game single → `cloud/work/frontier/tools/splice_singles.py NAME`
   then `git add src/blob/NAME.c`; game group → `install_group.py DIR NAME [--revert-single STUB…]` (same name
   supersedes); overlay → `ovl_splice group|splice --targets asm/us/ovl_x DIR` → `ovl_layout build` →
   `ovl_splice check` → `ovl_rom rom`; static → `lock add FILE:FN --flags … [--toolkit 4586d230…]` (7.1 segments per
   `rom_compilers.json`) → `promote_batch.py CTX.jsonl SEG… --only … --prefix-dedup rom/lib_…` (rewrite its
   hard-coded Opus trailer). Gates: `blob_unit check`, `blob_group check`, `blob_splice check`, `lock check`,
   `ovl_splice check`, `blob_rom rom`. Tests: `make test-fast JOBS=4` (Pi, ~3 min) and
   `CI_FAST=16 bash tools/ci_equiv.sh rocky <bundle>` (~2 min; `git bundle create B HEAD --remotes=origin`);
   watchman2 `~/rush2049/scratch/ciclone6` is the fallback. Push only after both pass. Lane dirs: commit
   RESULTS.md/best.c/groups/tools, never `v/`, `e9/`, `b*/` (ignored). Lanes' commits carry an Opus trailer:
   rewrite with `git filter-branch --msg-filter` before pushing.
4. Known traps: a research packet receipt pins tool hashes → rebind (§5) after any tooling change; a packet that
   rebuilds a ROM TU must use `compilers.tu_cc`; promoted neighbours can break packets that assume exact slot
   extents (macro_wrapper_contracts fixed 2026-10-12, others may follow); `blob_rom rom` needs
   `build/ovl/ovl_{a,b}.deflate` present; `splice_singles` leaves `src/blob/NAME.c` untracked; `score.py fn`
   says NOT VERIFIED for split-rodata own data (install via blob_group).
5. Wave 20 candidates, by expected bytes per lane: (a) boot tail under 7.1, the w19a/w19b not-matched lists and
   the untried larger functions (`w17g/inv_table.md`); two owned jump tables (func_80021548 w19b §C,
   func_80023BDC w19a) via `owned_data prepare`; (b) game: `frontier next` fresh singles (w19e closed 1 of 4;
   world_effect_update is two block boundaries from done), func_800D816C stays Astra's; (c) overlays: Astra's
   #311 frame-only residuals once Astra converts them; coordinator lanes keep off overlays unless CLAIMS says
   otherwise; (d) recomp spike per `2026-10-12-recomp-port-plan.md` Phase 0–1 on rocky (owner/coordinator, needs
   the ROM). Optional tooling: xdist in GitHub CI (owner call), score.py `--compiler` (hook-protected).
6. Astra: brief is `CloudHandoff.md` (rewritten 2026-10-11); protocol v2 adopted in #309/#310 (both merged
   2026-10-11); claims in `cloud/work/CLAIMS.md`. Integrate ready PRs within a day; comment the result.

**Wave 20 outcome (2026-10-12, master `9ee4f494`):** static 550 → 568/668 (111,748 B, 69.53%): w20a 2 (func_8001824C,
func_80017D38), w20b 8 (lib_15a10/17dc0/1cf90/22300; line layout decides 7.1 as1 scheduling; function-local statics
for lib_16320), w20c 7 (reloc_addrs fixes for splat mis-pairings via `reloc_split derive`; owned jump-table .rodata
slots incl. a co-owned pair in lib_1a660; `owned_data.section_fits`), PR #314's func_8001F954 promoted from a
TU-spelled copy. Game +2 (func_800D11BC w20f, assign_drones w20d; 975/1216, 48.26%). Overlay A 42 → 48 (Astra #315/#316/#318; `ovl_splice group --claims`
and the supersede guard). Recomp Phase 0 landed (`tools/recomp/`, plan §8): N64Recomp emits C for every game/overlay
function and 578 static ones from a synthetic ROM with the inflated images appended. Packet verifiers now accept
registered owned slots through `cloud/work/boot_tail_promotion/owned_slots.py` (w20p). Near-misses moved without
landing: entity_spawn_init 61 → 7 words (empty-block tie lever, w20g), engine_sound_sync 107 → 14 rows (group with
audio_effect_setup), func_800D6914 3 rows (provisional), entity_render_setup 20 rows (data-alignment decision: define
D_8011AD90 at a 16-byte section offset, w20h), audio_fade_control unit 6/8 (w20f). Open static: w20b `ownbss/` trio
needs an owned .bss block for lib_16320; w20a scheduling-only residuals need a 7.1 trace (`cc -S` is the best proxy).
Wave 21 candidates: `CLAIMS.md` open rows; recomp Phase 1 on rocky (runtime pinned to N64Recomp 81213c18; overlay
load hook; ~20 libultra internals to shim).

**Wave 21 outcome (2026-10-12, master after `46c45bd2`):** static 568 → 580/668 (118,488 B, 73.72%): w21d owned `.bss`
slot kind (lib_16320 trio), w21b 3 (loop-end reloc_addrs, store read-back lever), w21a 3 (`-Wab,-r4300_mul` was
missing from the 7.1 lock recipe and explained two "scheduling-only" residuals; now a neutral flagset token in
layout/promote_batch), w21c 1 (func_80012730; identities for the other six large ones in its RESULTS), w21i 2 via
owned `.data` and a 4-byte `logical_kind: constants` `.rodata` slot. Game 975 → 979/1216 (318,752 B, 49.26%):
entity_spawn_init (5,544 B, group frontier_entity_spawn_init: two inlined file statics get_car/get_mph defined after
the function, one disclosed `if (type) {}`), func_800D63EC, func_800B3704 (NewBlit), func_800D6530 (codex_vsync_a145
superseded). blob_unit `data_align` override landed (entity_render_setup 20 → 12 rows). Recomp Phase 1 landed
(`tools/recomp/runtime/`, plan §9): headless boot to the front end and image B under scripted input; 43 libultra
inventory names corrected only in `tools/recomp/identity.json` (owner decision: rename the inventory). Open: CLAIMS
`open` rows; func_80090308/buffer_swap/cpak_read now ready; engine_sound_sync 8 rows, func_800D7634 15 words
(w21h); w21c's five identified large functions; recomp Phase 2 (function replacement, rdram view, frame oracle) and
RT64 on a machine with a display.

**Wave 22 outcome (2026-10-12, master after `2ae36670`):** static 581 → 586/668 (124,448 B, 77.43%): seqStartPlay
(w22a; index reused as return value), the 2,796-B macro interpreter + func_80022874 via a grown co-owned .rodata slot
and a .bss block (w22b/w22i: `promote_batch --replace-slot`, composed companions do not work because asm-processor
puts GLOBAL_ASM .rodata first), func_80010E80 + func_8001A270 from the new instrumented IDO 7.1 toolkit
(`cloud/work/frontier/tools/trace71/`, w22c: callee-saved cost = max(4, blocks/4), ties go caller-saved; one C
variable = one allocation unit). Game 979 → 982/1216 (322,664 B, 49.87%): audio_options_menu group (func_800D7634 +
func_800D6914 + stub, w22e: a precoloured call result forbids v0 to every web in its uopt block), cpak_read (w22d;
explicit calls to the libgcc-named ll.c statics, rename pending). Libultra rename landed (w22g: 43 static names to
ultralib VERSION_J, `static_rename` + `targets_rename rename`, DB renamed in place; game callers still use the old
names through `game_static_legacy.us.txt`). Recomp Phase 2 landed (w22r, plan §10): replacement by manifest with the
project's IDO, lockstep clock, frame oracle; ten matched functions EQUIVALENT over 1800 frames, injected bug caught at
frame 3, w21h's engine_sound_sync draft DIFFERENT at frame 706. Open: CLAIMS `open` rows (buffer_swap 43 words,
func_80090308 group, entity_render_setup 8 rows, func_8001F13C 2420, func_8001A658 7725, func_80012D18 12735);
game-side rename + ll.c family rename; recomp Phase 3 (per-call capture) and RT64 on a display. Astra's #324/#326/#327/#328/#329 landed
via lane w22p (overlay A 53/193, B 10/49; four 20261006 groups migrated into dot_runtime_a_text_helpers_20261006).

**Wave 23 outcome (2026-10-12, master after `6fe4eeaa`):** static 586 → 600/668 (130,500 B, 81.20%): func_80018634 (one
u32 variable spanning two switch arms forbids its register in the default case), w23c's 13 code-identical functions
(5 plain, func_8001E0E0 via a constants slot, two via reloc names + TU hoist, three via a `.bss` block grown at its
front, osPfsIsPlug/__osPfsRequestData via new inventory rows; lib_16320 and lib_10d60 are all C). Game 982 →
985/1216 (325,760 B, 50.34%): buffer_swap (group member; group-vs-unit line markers), engine_sound_sync +
audio_effect_setup + stub (the Phase 2 frame-706 DIFFERENT was the standalone build ignoring $f22–$f28 IPA
arguments; Phase 3's per-call capture named that call). Renames: 18 ll.c/llcvt.c helpers + 6 libultra suspects on the
static side and every game caller (w23g, merged by w23m; `game_static_legacy.us.txt` empty; cpak_read natural u64
form). Recomp Phase 3 landed (plan §11): per-call capture, 4 input scripts, static binding, OSThread word mask;
all 689 matched game files replaced at once = EQUIVALENT, 4.2 M calls identical. Traps: the builder gate repo
(`~/rush2049/repo`) goes stale whenever an owned slot changes ("owned ROM slot address changed" on a sound tree):
`rm -rf build/us && make COMPILER=ido -j16 && make test` there; after a rename, `build/blob/obj` objects are stale
(`blob_rom rom` refuses on provenance): `cloud/work/frontier/w23m/rebuild_stale.py`. Near-misses moved: controller_poll
92 → 11, synthHandle 7725 → 2770, voiceAllocate 2420 → 2045, entity_render_setup cause 3 proven (live range needs block
17 AND a split). Ceiling table for the boot tail in w23c/RESULTS.md (0x26F60 segment 5,232 B needs conversion;
pre-tail boot code 13,152 B untriaged; 480 B hand asm).

**Wave 24 PR batch (2026-10-12, lane w24p, master after `b37643ee`):** Astra's 14 open PRs triaged in one lane
(`cloud/work/frontier/w24p/RESULTS.md`). Landed #331 (func_803990D0, superseding `dot_runtime_a_menu_state_20261006`,
4 members), #332 (func_803A4340 single), #333 (func_8039BB80 in `dot_runtime_a_text_helpers_20261006`, now 12
members, all claimed), #334 (func_8038D3A4 + owned 0.2f literal, image B): overlay A 53 → 56/193 (25,196 B, 12.98%),
overlay B 10 → 11/49 (3,608 B, 8.22%). #322 imported as research: func_800D816C is a private-ABI callee of
func_800D91A0 (691/699 standalone with a normal prologue; 570/699 made internal, and the unit then inlines locked
func_800DA0B4) so it lands only with a matching D91A0. #330 unchanged since w22p; #325 still awaiting the
`replace.py` native-mode rework; #323, #317, #314, #313, #312, #311, #163 superseded (reasons in RESULTS.md).
Static and game unchanged.

**Wave 25 PR batch (2026-10-12, lane w25p, wave24 after `7eb1ae51`):** Astra's 18 PRs #335–#352 in one lane
(`cloud/work/frontier/w25p/RESULTS.md`). 16 landed: five same-name group supersessions (#335 text_helpers → 13 members,
#336 availability_refresh → 2, #337 option_eligibility → 3, #340 seven_option_group → 2, #346 menu_renderers → 4), two
new-group chains installed in order with a gate after each step (#341 → #344 → #349 → #352 `runtime_a_stats_menu_20261006`,
now 9 members incl. three never-locked exact panels; #345 → #350 `runtime_a_profile_menu_20261006`, 3 members), singles
#348/#351 (settings bar/values) and image-B #338/#343/#347 (position initializer, nearest-target steering, trail callback
with owned data). Overlay A 56 → 75/193 (43,112 B, 22.21%), overlay B 11 → 14/49 (5,964 B, 13.59%), +20,272 B. #339
(B 80392894, 2/468) and #342 (A 803A5358, 58/192) imported as research. Lessons: a packet's `verify.py` must pass both
before and after installation (#340/#346 compared against the pre-install group source and had to be patched, receipts
re-recorded with `--deep`); running packet tests under `pytest -n` needs `--dist loadgroup`. Static and game unchanged.
Package v1 laptop report: `tools/recomp/reports/v1/` and the recomp plan "Package v1 laptop report" bullet.

**Wave 25 outcome (2026-10-12, wave25 after `4f59607f`):** static 614/667 → 617/667 (139,624 B, 86.87%): __scMain/__scExec
(684 B, w25a) through the new `OWNED-OFFSETS` builder gate (`tools/conveyor/pipeline/owned_offsets.py`: every named
.bss/.data/.rodata object of a ROM TU checked against its retail address after `make test`; cause of the wrong order
was IDO cfe numbering symbols by first sight and sorting .bss by that number mod 256 — `w25a/tools/bssorder.py`
predicts it; `promote_batch --bss-at-companion`; headroom 6 declarations in `rom_tu.h`), and voiceAllocate
(func_8001F13C, 1,456 B, w25f: the 96-row register rename was one code-free ugen temp pop; NOT the `& 0xFF`
narrowing — as1 deletes a narrowing only inside one block — but a label between two equal loads that stops ugen's CSE,
spelled `do {} while (0);`, plus the search loop written without `continue` so the extra block does not flip the
num/voice tie). Six libultra renames proven byte-identical and applied (w25a: __osCurrentTime, __osBaseCounter,
__osViIntrCount, __osCheckPackId, __osRepairPackId, __osCheckId; `gViTimeAccumLo` stays as the low-word asm label).
Game 986 → 997/1216 (329,104 B, 50.86%): the audio_fade_control unit as a 12-member group (w25c: a `do {} while (0);`
in the inlined node_alloc makes the `&D_80146138` web span two blocks so a0 is forbidden and it takes a2; four locked
empty stubs absorbed as the real inlined helpers via force_internal + prefer_definition), wheel_setup_initial +
func_800A4B40 (codeless `if (type == 64) {}` flips the three-way tie), func_800EC190 (`& 0xFF` on a byte store =
one hidden ugen temp, deleted by as1 — the 5.3 form of the mechanism), func_8008AD6C (w25e: read every value through
`*gfx`, no word local). Near-misses pinned to mechanisms, no score change: entity_render_setup (w25d: the h_d/SPL
route is dead; two separate candidates hypothesis), func_80090308 group (helper route refuted; inlined colour setter
reproduces constant sharing; frame gap probably a 12-word local shared with save_write_data), synthHandle (two
independent store kills; midi tests on the struct field), func_80018448 325 → 310 true (first half matches),
func_80012D18 (double + two u32 locals), func_800A1BB4 is INTERNAL (t6–t9 only; 9 rows with a stand-in caller),
ghost_race_setup residual is as1-stage. Inflate block: IDO 5.3 `cc -O2 -S` output hand-assembled at as1 -O1 (needs
a TU recipe, w25a). Recomp Phase 5 (w25r): 14 libultra-free busy-waits hooked with `rush_hook_spin` (the laptop's
wall-clock hang reproduced headless with `--null-dl-delay` and under RT64 in an Xvfb container, fixed), RT64
self-check pages named (z-buffer 0x8017AEC0, framebuffers 0x807BB3C0/0x807DDA00; `oracle.py compare
--mask-renderer` → "EQUIVALENT except renderer-written pages"), lockstep+RT64 display-list race fixed, Rush's own
audio microcode recompiled with RSPRecomp (0xF40 B at ROM 0x2AC50; AI interrupts paced to the game's 192-frame
buffers; sound through SDL and `--wav`), package v2 (`build/recomp/dist/rush2049-recomp-test-v2.tar.gz`, sha256
99b57e31…, with-ROM copy on rocky) with attract/stunt/pad steps and the ROM copy deleted from `--home`. Astra's
#353–#358 (w25q, end of wave): five landed (+3,976 B; overlay A 75 → 77/193, 44,700 B, 23.03%; overlay B 14 → 20/49,
8,352 B, 19.03%), #357 (A9088 15/917) research only. Traps: the
builder's disk hit 100% mid-promotion (finished-lane scratch copies; now cleaned); the Makefile depends on
`cloud/work/frontier/<lane>/rename/rebind/*.json`, so the builder gate repo needs those dirs rsynced too; `ci_equiv`
bundles must be based on a commit the rocky clone already has. Lessons for wave 26: the two code-free temp-pop
mechanisms (as1-deleted narrowing within a block; label-broken CSE) are now the first thing to try on any "pure
register rename" residual on either compiler; an empty block adds one to the block count of every value live across
it and can flip a near-tied priority; retail using only t6–t9 temporaries means an internal function; a value still
live at a `return` is denied v0; as1 fills a load delay slot from the branch target even past a label.

**Wave 24 outcome (2026-10-12, master after `3bec9ce3`):** static 600/668 → 614/667 (137,484 B, 85.54%): the 0x26F60
segment is a libgsm GSM 06.10 decoder (w24a, 4 fns 5,232 B, `layout convert` + 7.1 pin; the checked-in
`rush2049.us.ld` had to be regenerated, now automatic via `layout ldsync`); the pre-tail is IDO 5.3 `-g1 -O1`, not 7.1
(w24b: 11 fns 1,752 B — rmonPrintf, 4 sched.c, game_init, 7 vi* — promoted by w24s after refreshing inventory targets
that still carried pre-rename callee names; the "viStub" slot was viDeadlinePassed's trailing `jr ra; nop` pairs and
is retired with the new `targets_rename apply --absorbed`; the rest of the pre-tail is 2,640 B ultralib hand asm and an
8,104 B inflate/lzss block). __scMain/__scExec are code-identical but their TU's `.bss` order is wrong in a way the
full-ROM gate cannot see (syms.ld overrides definitions): needs a `.bss`-offset gate before promotion. Game 985 →
986/1216 (326,688 B, 50.49%): controller_poll (w24d; volatile pad views fix load order, `if (b == 0) continue;` flips
the t2/t3 priority tie; group member with `keep` + defined D_80156958 + `extern_data` override). Tooling (w24t): builder
gates clean `build/us` themselves on an owned-slot-input stamp, `blob_splice rebuild-stale` runs inside `blob_rom
rom`, `replace.py` refuses IPA register-parameter callees without `--allow-ipa` (`ipa_detect.py`), timer_services
renamed to ultralib J (dll_* → __osTimerServicesInit/__osTimerInterrupt/__osSetTimerIntr; 0x80009E18 →
__osPfsCheckRamArea), `opt_overrides.mk` equals its generator. Recomp Phase 4 (w24r, merged with w24t by w24m):
engine_sound_sync EQUIVALENT on all four scripts via `ipa_callees.json` register shims; RT64 is a CMake renderer
option (null default, pinned, builds on rocky); seven input scripts; gameplay-test package v1
(`tools/recomp/package.py`, `build/recomp/dist/rush2049-recomp-test-v1.tar.gz`, Windows 11 native INSTALL first) for
the owner's laptop — every later recomp phase regenerates it. Near-misses: voiceAllocate 2045 → 605 (7.1 PRE oracle
`w24c/tools/pre71.py`: no critical-edge splitting, spill-home order = expression-web bit order), synthHandle's span/midi
proven store-/load-PRE, entity_physics_update 21 words without a device, func_80090308 198 rows with the 112 B frame
gap decomposed, wheel_setup_initial 4 words (inlined helper from a locked sibling's predicate), audio_fade_control unit
7/8. Astra PRs #331–#334 landed (w24p). Lessons for the wave-25 rules: a retail constant reused across stores into a
stack struct means a pointer (inlined helper parameter); a chained assignment costs one frame temp per link; 7.1 frame
4·N bytes large = N missing named locals; if retail's schedule is worse than as1's choice, a dependency edge is
missing (volatile); old drafts must say osSendMesg not osJamMesg after the renames; inventory targets may still carry
pre-rename callee names (refresh_split_targets.py).

**For the next coordinating session.** This covers where the frontier matching effort stands, how a wave is run end to end, what the owner has decided, and every open item. Read it with `CLAUDE.md`, `cloud/work/frontier/WAVE_BRIEF.md` (the brief every lane agent reads) and `docs/plans/2026-10-04-frontier-plan.md` (the original method).

## 1. Where things stand (master `4d81f8dd`, CI green)

| | Functions | Bytes | % |
|---|---|---|---|
| Game code (compressed image) | 929 / 1216 | 271,600 / 647,072 | 41.97 |
| Static code (boot/library) | 399 / 668 | 73,292 / 160,720 | 45.60 |
| Combined bytes | | 344,892 / 807,792 | ≈42.7 |

`make progress` prints the first two rows; combined is just the sum. Report game and static separately, never as one denominator (CLAUDE.md).

**Frontier** (`python3 -m tools.conveyor.pipeline.frontier report`):
- **Remaining:** 287 unmatched game functions, 289 KB.
- **Ready now:** 29 singles (24 KB), 33 group functions (32 KB) and 5 unit functions (3 KB). "Ready" means every callee is already locked.
- **Provisional:** 17 functions (13 KB) are recorded in `cloud/work/frontier/provisional.json`. Their bodies are proven only with stand-in callers, so they are not ROM coverage.

**Yield per wave (game code):**

| Wave | Added | Kind of work |
|---|---|---|
| 9 | 15.4 KB | fresh large functions |
| 10 | 6.3 KB | |
| 11 | 10.0 KB | |
| 12 | 4.9 KB | near-miss closing |

The easy large functions are gone. What is left is near-misses, clusters that have to land together, and large functions whose callers are still unmatched.

## 2. How a wave is run

1. **Choose targets.** Use these commands:
   - `frontier next --limit 200`: ready functions with recipe single, group or unit.
   - `frontier show NAME`: callees, callers, register facts (IPA), blockers and prior source paths.
   - Also read the previous wave's `cloud/work/frontier/w<N><lane>/RESULTS.md` files for near-misses and their traced causes.
2. **Size the wave.** Use 5–9 lanes, each with 1–6 functions grouped by subsystem or call cluster. Keep within the session's agent guideline (under about 10).
3. **Write a rules file** in the scratchpad (the last one was `w12common.txt`; text in §8) and launch one background `Agent` per lane.
   - Each lane prompt names its lane id, local dir `cloud/work/frontier/w<N><x>/` and builder scratch `~/rush2049/scratch/frontier/w<N><x>`.
   - It lists the targets with byte sizes, the current residual and where the best prior source is.
   - It ends with "read the shared rules file and follow it". Template in §8.
4. **Agents write only to their lane dir**, plus `cloud/matches/NAME.c` for functions that match standalone. They never splice, commit or edit `src/`, `asm/`, locks or `tools/`.
5. **Integrate as lanes report.** Do it on a branch, then push to master. The full sequence is in §3.
6. **Report to the owner.** Give each lane's result in a few lines, a running tally, and an updated coverage table after every push.

The shared tracing toolkit lives in `cloud/work/frontier/tools/trace/`; read its `README.md`. Lanes run `install.sh <their scratch>` or `--reuse ~/rush2049/scratch/frontier/wtk`. It provides:
- `us.sh`: unit score plus row counts.
- `udiff.py --ops/--norm`: aligned and opcode diffs.
- `ctrace.sh` / `sum.sh`: register-colouring decisions.
- `force.sh`: the forced-colour oracle.
- `spill.sh` / `spcensus.sh`: frame and spill layout.
- `ugt.sh`: code generator (ugen) evaluation order.
- `as1t.sh`: assembler (as1) scheduling.
- `asm.sh`: reassemble a hand-edited listing.

Every traced binary is checked to give output identical to the stock compiler.

## 3. Integration procedure (do not skip steps)

Run on the Pi from the repo root, on a branch such as `git checkout -b wave13`.

1. **Single functions** (the file in `cloud/matches/NAME.c` has a bare `/* flags: … */` on line 1):
   `PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py NAME …`
   - It compiles each function standalone and runs the image gate.
   - A refusal like "compiled body is shorter than the extent" or "first difference at …" means the match depends on whole-program inlining or IPA. Package it as a group (step 2); w12l did this for three functions.
2. **Groups:**
   `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py <lane>/groups/<g> <name> [--revert-group OLD] [--revert-single STUB] [--provenance TEXT]`
   - **Name:** use the same name as the locked group the new one supersedes. Tests and research packets look groups up by name. Renaming broke `compare_group` once; renaming back fixed it.
   - **Members:** the group's `members` must keep every old member, plus every locked stub it now gives a real body. If it doesn't, the install fails "functions locked before are unlocked now". The stubs are reverted with `--revert-single`.
   - **Overrides:** stubs that become real inlined helpers need entries in `src/blob/unit_overrides.json`:
     - `prefer_definition` pointing at the group file;
     - `force_internal`.
     Add them **before** installing. If `blob_unit` complains that an override points at a missing group file, the group isn't installed yet.
   - **Missing objects:** if an install fails and a later one reports "object missing build/blob/obj/X.o", rebuild the object:
     `PYTHONPATH=. python3 -c "from pathlib import Path; from tools.conveyor.pipeline import blob_splice; blob_splice.compile_on_builder({'X': Path('src/blob/X.c')}, '<flagset from lock>')"`
     The rollback now recompiles reverted singles too.
3. **Gates. All must pass:**
   - `python3 -m tools.conveyor.pipeline.blob_unit check` → N locked, N equal, 0 differ.
   - `python3 -m tools.conveyor.pipeline.blob_group check` → 0 problems.
   - `python3 -m tools.conveyor.pipeline.blob_splice check` → 0 problems.
   - `python3 -m tools.conveyor.pipeline.lock check` → all 403 static locks intact.
   - `python3 -m tools.conveyor.pipeline.blob_rom rom` → `MAKE=0 TEST=0` (full-ROM SHA-1).
   - `make progress` → the new numbers.
4. **If a match closes a provisional entry,** remove that entry from `provisional.json`. Keep the file's 1-space JSON indent so the diff is only the removed lines. `tests/conveyor/test_frontier.py` checks the seeded entries.
5. **Commit**, ending the message with the co-author line. Include the lane dirs.
6. **Tests, both environments, before pushing:**
   - **Pi:** `python3 -m pytest tests/conveyor tests/cloud -q -m "not node_required" -p no:cacheprovider`. The Pi has no IDO, so compiler tests skip there.
   - **Builder (CI-equivalent):**
     1. On the Pi, bundle HEAD **plus all remote branches**: `git bundle create X.bundle HEAD --remotes=origin`.
     2. scp it to watchman2.
     3. In `~/rush2049/scratch/ciclone6` (a disposable clone; submodules are copied from `~/rush2049/scratch/ci`):
        `git checkout -q . && git clean -qfd cloud tests && git fetch -q ../X.bundle HEAD "refs/remotes/origin/*:refs/remotes/origin/*" && git checkout -q FETCH_HEAD`
     4. Run `REQUIRE_TOOLCHAIN=1 ~/rush2049/scratch/civenv/bin/python -m pytest tests/conveyor` and the same for `tests/cloud`, both with `-m "not node_required"`.

     Steps 2-4 in one command (also on the second host `rocky`; `CI_FAST=N` for the parallel split): `tools/ci_equiv.sh watchman2 X.bundle` ([BUILDING.md](../BUILDING.md#ci-equivalent-test-hosts)).

     Remote branches are needed because some research packets `git show` pre-purge commits that exist only on old unmerged branches such as `origin/dot/palette-steering-20261005`. **Never delete unmerged remote branches.**
7. **Push** with `git push origin HEAD:master`, then `git branch -f master HEAD`. Watch GitHub CI with `gh run watch <id>` until it passes.
   - CI's "Rescore changed cloud submissions" step rescores every changed `cloud/matches/*.c` standalone.
   - A function that is EQUAL only in a group or the unit must **not** be in `cloud/matches/`; its source lives in the group dir.

**Research-packet breakage** (Astra's `cloud/work/**/verify.py` plus `tests/cloud/test_*`) is the usual post-splice test failure. Fix it in the established ways only (§5).

## 4. Owner decisions and standing rules

- **Astra's functions:** do not work on `func_80087110`, `stat_race_update`, `func_800FE5B0` or `camera_transform` (PRs #305–#308, 2026-10-07). Astra (GitHub `cabi24`, PRs from `dot/*`, `work/*` and `research/*` branches) owns them.
- **Unused local declarations** are acceptable only when they are the **exact frame residual** and are disclosed in the file header. The owner approved this in wave 11. Measured rule: every function-level named local takes a slot, used or not.
  Pad arrays standing in for unknown structure elsewhere are not acceptable.
- **Shaping devices** must always be disclosed in the header, and a match needing them is accepted with that disclosure:
  - compiled-out reads such as `if (x) {}`;
  - compiled-out debug switches (`func_800CCB40`'s `DEBUG_FORCE_PAL`, which the owner approved);
  - shaping helpers of unknown identity (`eu_nop`, `attach`).
- **Not acceptable:**
  - a false prototype, such as declaring a void function as returning `s32`;
  - pinning stand-ins as matches.
  Implicit int (no declaration in scope) is **an open decision**; see §6.
- **Provisional rule:** a body that matches only with stand-in callers goes in `provisional.json` and is not spliced. It becomes a match when its real callers match in the same group.
- **Pushing:** push gate-passing, test-passing work to master as it lands; the owner wants master current. Force-push only with explicit owner approval (one history purge was approved).
- **Hook-protected files:** do not edit `tools/cloud/score.py` directly. Changes go through a reviewed patch with owner approval.
- **Permission denials:** if the permission classifier denies an action, do not retry it another way. Surface it to the owner.
  - Lanes are told the same; two wave-9 lanes broke this with `rm`.
  - The owner's blanket approval did not override a "Git Destructive" denial for branch deletion; see §6.

## 5. Research-packet fixes: the established patterns

Astra's verification packets bind receipts to hashes. Integration breaks them when it changes something they pin. These fixes are owner-approved (the "test fix" class):
- **Whole-manifest or lock pins.** Hashes of `asm/us/blob/SHA256SUMS`, per-function `asm/us/blob/*.s` text (each splice adds a `/* compiled from … */` line), or `blob_matched.lock.json`: drop those keys from the comparison, inside the test or the packet's `--check`. Selected native bodies stay bound.
- **Superseded context group.** Copy the old group byte-identically into `cloud/work/frontier/superseded/<group>/`, with a README naming the packet. Make the packet resolve the old path there, keeping the receipt's recorded paths unchanged.
  - Examples: `dot_directional_sound`, `dot_audio_force`, `dot_object_sound` (via `context_path()`), `dot_quadtree_donor` and `dot_heap_max`.
  - Update the packet's own `verify.py` hash in its receipt.
- **"Function not yet locked" assertions.** Allow the promoted state: `NAME not in locks or locks[NAME]['source'] == 'src/blob/NAME.c'`. Applied to `masked_random` and `nearest_point_donor`.
- **Missing IDO** is handled centrally in `tests/conftest.py`: it becomes a toolchain skip, which in turn fails under `REQUIRE_TOOLCHAIN=1`.

## 6. Open items: what needs communicating, and to whom

**To the owner:**
1. **Merged remote branches can be deleted by the owner.** This was blocked for Claude as "Git Destructive". 70 merged remote branches can go; deleting them loses nothing:
   ```
   ! open=$(gh pr list --state open --json headRefName -q '.[].headRefName'); br=$(git branch -r --merged origin/master | grep '^  origin/' | grep -v 'origin/master\|HEAD' | sed 's|  origin/||'); for b in $open; do br=$(echo "$br" | grep -vx "$b"); done; git push origin --delete $br
   ```
   It must stay restricted to `--merged`; unmerged branches hold commits that research tests need.
2. **Decision: implicit int.** `func_800B59F0` (1,372 B) compiles identically with no false prototype by leaving the void callee `func_8008D870` undeclared (C89 implicit int), plus `(s16)` casts (w12h). This is the same mechanism as the rejected false prototype, arising from a missing header. The function is provisional until its caller `physics_sym` matches, so the decision isn't urgent.
3. **Attribution doubt on `func_800AC660`.** It is in the locked `frontier_level_objects` group as `transmission_ratio_get`'s inlined getter, a naming hypothesis. w12d found that a static getter defined after `differential_output` leaves its stub exactly at that address, so it is probably `differential_output`'s flags getter. The bytes are unaffected (`jr ra; nop`). Settle it when `differential_output` lands: the stub should then move to that group.
4. **Cluster-landing decisions to expect:** both near-complete clusters (below) carry disclosed shaping.
   - Mode-select: two unused locals, the `for(i=0,n=4;i!=n;i++)` form, and a `func_800E0048` camera-wrapper identity hypothesis.
   - Camera: the empty-then `if (flags) {} else {…}` form.
   These fall within the standing rules; mention them when landing.

**To Astra:**
5. **PR #163 (draft, 443 files).** It merges cleanly but fails five of its own replays with pinned IDO on x86. Hold it until Astra rebases. The note was given to the owner to forward; if it wasn't sent:
   > #163 merges cleanly onto master but these fail with pinned IDO on an x86 Ubuntu builder (REQUIRE_TOOLCHAIN=1): test_runtime_a_flags::test_fresh_o3_replay pins gnu_ld/host_cc version strings (drop them from the compared receipt); late_packet_portability[153/156/160] (event_ring, lives_text, e114_parent) report frozen/source-bound receipt drift; runtime_b_closure reports "negative control drift". Please rebase on current master (waves 9–12 spliced ~45 functions, superseded frontier_list_alloc_sound / camera_scene_manager / frontier_traction_control / func_800AD4C8 / codex_vsync_a145 / audio_heap / camera_aspect_ratio / func_8008B640, and changed func_800B61A8's source) and rerun the full matrix with IDO before un-drafting.

   #163 also carries `func_8001F954`'s production-compatible adaptation (`cloud/matches/voice_unblock_production/`), a static promotion worth 124 B. Promote it with `.claude/skills/promote-match` once #163 lands. That command needs a clean tree and commits on success.
6. **Astra's own items** (keep off them): `func_80087110`, `stat_race_update` / `func_800FE5B0`.

**Matching work. Open near-misses**, all with a traced cause. Paths are under `cloud/work/frontier/`.

| Target | Bytes | Gap | Cause and best source | Lands with it |
|---|---:|---|---|---|
| func_800E05F0 | 1328 | 30 word rows | One `.alias $8,$sp` that uopt places after `jal player_conditional_call`. Moving it in the listing leaves only the unverified 0.6f/0.1f words. `w12i/RESULTS.md`, `w12i/comp/mode.c`, traced alias patch `w12i/tools/uopt_alias_patch.py`. | **Mode-select cluster, ≈8.4 KB.** 9 bodies already EQUAL with real callers (`w12i/comp/run.sh`): mode_select_handler, func_800E0050, func_800D5E64, func_800DFBA0, best_times_display, func_800DED78, mode_select_input, func_800DEF60, func_800E0048. Landing recipe in `w12c/RESULTS.md` plus `w12i/RESULTS.md`. |
| camera_update | 2876 | 17 words | 8 words and 4 words are each a register that one forced colour fixes (an "invisible holder" in retail); 5 words are compare operand order. `w12f/RESULTS.md`, `w12f/camera_update/best.c`, group `w12f/groups/camera_aspect_ratio/`. | camera_free_look, camera_track_spline (both EQUAL), ≈4.2 KB |
| entity_process_main | 1424 | 2 words | as1 operand order shift, load, not. `w11a`, `w12b/RESULTS.md`. | |
| func_80096130 | 264 | 3 words | One `.alias $3,$sp`; the hand-edited listing gives 0 rows. `w12e`. | |
| audio_channel_setup | 332 | 3 words | as1 copy coalescing into an argument register. `w12e`. | |
| differential_output | 648 | 13 words | Register only: forcing 2 colours gives 0. `w12d`. | settles `func_800AC660` |
| func_800E7A98 | 172 | 14 words | `w11a`, `w12e` | |
| object_bytes_sum_global / object_bytes23_sum | 84 / 72 | 8 / 4 | Not `func_80096288`'s body (refuted). `w12e`. | |
| audio_channel_priority | 468 | 11 words | Register only (6 forced colours give 0). `w12e/audio_channel_priority/stride.c`. | |
| audio_doppler_full | 952 | 39 words | One statement makes 3 fewer ugen temp allocations. `w12g`. | |
| camera_look_at_point | 796 | 38 words | `w12f`, `w11c` | |
| physics_sym | 1252 | 4 rows with 8 forced colours | New draft, `w12h/physics_sym/` | func_800B59F0 (see item 2) |
| input_deadzone_apply | 3580 | 122 words | ugen compiles it twice; its float free-list carries over from the first pass. `w12a`. | input_process_controller (provisional) |
| func_800E847C + func_800E7FA0 | 2100 + 1244 | 232 / 35 | Web numbering: retail's late values must be compiler-created. `w11g`. | |
| race tail / drone pair | | far | drone_ai_update + entity_tick_main frames are ≈300 B unexplained. `w11e/drone`. | |

Newly unblocked and untouched: **entity_spawn_init**, after func_8008E408 matched.

## 6a. PR integration #164–#304 (2026-10-06 evening, master `69966534`)

- All 141 PRs merged (research packets plus runtime-image A/B candidates as evidence). The 7 stacked PRs were closed with a comment because GitHub can't mark them merged automatically. **#163 is still held** (§6 item 5; not rebased).
- #164's `records_screen` clashed with the w13f splice. The spliced source stays in `cloud/matches/`; Astra's copy moved to its packet dir, and its verify.py/test now point there.
- **Spliced (game, +1,020 B):** `func_800C7578`, `func_800CD8EC`, `func_800CCE5C` and `func_800CD798` as groups `dot_setting_c7578`, `dot_records_cd8ec`, `dot_filename_cce5c` and `dot_range_cd798`. Each takes the accepted hash as internal context, Astra's disclosed visibility hypothesis.
- **Promoted (static, +304 B):** `func_8001D944` (#177). The candidate was -O3, but it is also 0/76 at the segment's -O2. The production copy is `cloud/work/boot_tail_promotion/sources/func_8001D944.c` (shared `StateNode` contract), with context row `context_pr177.jsonl`, landed via `promote_batch.py`.
- **Provisional (strict MATCH, real context not yet matched):** audio_effect_setup, MaxPathZeroControls, audio_update_d, finish_state_alt, func_800F857C, render_results_screen, render_replay_ui. The func_800E451C and func_800D8078 entries were refreshed. The F857C/audio_update_d/finish_state_alt trio plus the D91A0/D816C results cluster are the next landings once those callers close.
- **Worktree gotchas:** `promote_batch.py` rsyncs the repo to `~/rush2049/repo` without excluding `.git` (a file in a worktree), and it copies symlinks verbatim. A symlinked `baserom.us.z64` overwrote the builder's ROM (restored; SHA-1 checked). In a worktree, copy the baserom and exclude `.git`.

## 7. Techniques that close functions (cumulative, highest yield first)

1. **Inlined locked helpers.** Big functions inline already-locked kept functions: rand `func_8008B2B4`/`func_8008B2E4`, LCG `func_800D50E4`, `func_800B930C`, `func_800B61A8`. Paste the definitions into the candidate, or put the locked file in the group as context.
2. **Caller-less `jr ra; nop` stubs are deleted inlined statics.** The number of stubs caps how many helpers a file can have. Name the helper after its stub (non-static, made internal through overrides); otherwise the scorer reports "1 extra words".
3. **Frame layout is fully determined:**
   - Named locals take slots top-down in declaration order, used or not.
   - Block-scoped locals come next.
   - Each inlined call's area follows, 8-aligned, in call order.
   - Spill temps come last: one slot per coloured expression web, in first-appearance order.
   - So a +4 shift of the temp area means the last inlined area has an odd word count (add or move an inlined call), not a missing variable.
   - Measure with `Udef Mmt` (`w12h/tools/mu.sh`) or `spill.sh`.
4. **Register allocation works in two phases:**
   - Phase 1 colours webs with at least 22 interferences, by priority.
   - Phase 2 assigns the rest in web-number order (first appearance in source); copy-propagated values are numbered after all named ones.
   - Each procedure has one callee-saved cost; a web whose total saving is below it gets split.
   - Run `force.sh` first: if forcing reaches 0 rows, the residual is register colour only.
5. **Levers for colour ties:**
   - A compiled-out `if (x) {}` adds a use with no code; `if (a|b|c) {}` raises several webs at once.
   - `t = a; t -= b;` priority splits.
   - Reusing one variable across loops.
   - A dead `x = y = 0` renumbering.
   - Operand order of `==` and commutative operators.
   - Unsigned casts set multiply operand order.
   - Each dead read raises the callee-saved cost, so don't stack them blindly (w12d).
6. **Directive and assembler residuals:**
   - uopt (not ugen) emits `.noalias`/`.alias`.
   - A `.alias` between an unconditional `b` and the next label blocks as1's cross-block hoist.
   - A pointer to a global assigned early in another block gets no `.noalias`.
   - Prove these by editing the listing and reassembling with `asm.sh`; w12b's debug as1 is `tools/build_as1dbg.sh`.
7. **Source idioms:**
   - Index global arrays directly rather than through pointer locals; stores through a pointer alias every global.
   - Write natural float literals rather than `extern f32 D_8012xxxx`; the scorer verifies own rodata.
   - Use `volatile` where retail reloads.
   - A function-local static for run-once flags.
   - Line layout changes as1 scheduling: write multi-line wrappers.
   - An empty-then `if (c) {} else {…}` forces a reload.
   - Recursion where a small tree walk gets the wrong saved registers.
   - The register-parameter order of a callee follows the caller's argument set-up order.
8. **Re-score old drafts in the unit** before writing new source; two wave-10 matches came from early drafts that later lanes had replaced with worse ones. `blob_unit score NAME --internal NAME` can show that a "single" label is wrong.
9. **Check older drafts' global addresses** against the retail disassembly; w9e found `D_801493D0` where the real global is `D_801392D0`.

## 8. Templates

**Shared rules file** (scratchpad `w<N>common.txt`; lanes substitute their name for `<lane>`):
```
First read `cloud/work/frontier/WAVE_BRIEF.md` in full and follow it — especially "Wave 9 techniques",
"Shared tracing toolkit", "Wave 12 rules" and "Permission denials" — plus what it points to. Run
`python3 -m tools.conveyor.pipeline.frontier show NAME` and grep cloud/work for prior attempts first.
Hard rules:
- Do NOT work on func_80087110, stat_race_update, or func_800FE5B0 (another contributor's), nor on other lanes' functions.
- Do not commit, splice, push, or edit src/, asm/, *.lock.json, tools/, include/, tools/cloud/score.py,
  src/blob/unit_overrides.json, provisional.json, or other agents' dirs. Deliverables: cloud/matches/NAME.c ONLY
  for standalone `score.py fn` matches; group dirs under your lane dir with "claims" and a complete "members"
  list (same name and all old members of any locked group superseded; locked stubs given bodies are members);
  RESULTS.md with exact integration notes (overrides needed, groups superseded).
- Confirm every match in the whole-program unit (`blob_unit --tag <lane> score ... --neighbours`, 0 locked
  bodies differing) and quote the output. A match with stand-in callers is provisional.
- Verification scripts must replay against the current tree (no whole-manifest/asm-text/lock pins, no
  `git show` of non-ancestor commits).
- Builder: at most 2 cores, only your own scratch. Breadth over depth: ~50 variants without movement → write up, move on.
- If a permission check denies anything, do not retry it in another form; note it and continue.
Use the shared toolkit cloud/work/frontier/tools/trace/ (README.md; install.sh into your scratch, or --reuse ~/rush2049/scratch/frontier/wtk).
Final message: RESULTS table (function, bytes, state, flags, exact scorer output line), integration notes, what generalises.
```

**Lane prompt:**
```
You are matching-wave agent **w13x** in the Rush 2049 N64 decompilation at /home/cburnes/projects/rush2049-decomp
(Raspberry Pi coordinator; IDO only on builder `watchman2`). Lane dir `cloud/work/frontier/w13x/`; builder scratch
`~/rush2049/scratch/frontier/w13x` (copy from `~/rush2049/scratch/frontier/base`, sync src/blob, include,
tools/cloud, asm/us/blob and the lock from the Pi first).
**Assignment:** <functions with bytes, current gap, traced cause, best prior source path, what lands with it>.
Read the shared rules file `<scratchpad>/w13common.txt` and follow it (substitute w13x for <lane>).
```

## 9. Suggested wave 13 (small, focused)

| Lane | Work |
|---|---|
| a, b | Two different approaches on `func_800E05F0`'s `.alias` placement. Use `w12i/tools/uopt_alias_patch.py` to find what keeps the pointer's live range open across the camera and set paths. Each lane works in its own copy of `w12i/comp/`. **Payoff: ≈8.4 KB cluster.** |
| c | `camera_update`'s invisible-holder registers and compare order. **Payoff: ≈4.2 KB with free_look and track_spline.** |
| d | The 2–3-word residuals (entity_process_main, func_80096130, audio_channel_setup), plus differential_output (settles func_800AC660). |
| e | `physics_sym`, which would land func_800B59F0 if the owner accepts implicit int, plus the fresh `entity_spawn_init`. |
| f (optional) | Ready singles and groups nobody has touched: `frontier next`, minus everything listed in §6. |

## 10. Pointers

- Wave lane results: `cloud/work/frontier/w9a` … `w12l/RESULTS.md`. The newest are the most useful.
- Superseded groups kept for packets: `cloud/work/frontier/superseded/`.
- Tools: `cloud/work/frontier/tools/` (`splice_singles.py`, `install_group.py`, `trace/`).
- Brief that every lane reads: `cloud/work/frontier/WAVE_BRIEF.md`; add each wave's lessons there.
- Builder: `ssh watchman2`. The toolkit IDO is at `~/rush2049/cache/toolkits/796ae99a…bfbf5/ido`. Never touch `~/rush2049/repo` or the shared `/tmp/blobsplice` and `/tmp/blobgroup`.
- **Not part of this work:** the `tools/mips_to_c` submodule has uncommitted local edits (`m2c/arch_mips.py`, `m2c/translate.py`, a test output file and a new `m2c/rush_ipa.py`), dated 2026-09-29. They predate waves 9–12 and were never committed or used by them. They are most likely the m2c IPA patch work from late September. Leave them alone unless the owner says otherwise.
- **Two Claude profiles on the Pi:** `~/.claude-work/` and `~/.claude/` keep separate memory directories for this repo. A note saved in one is invisible to a session using the other, so this handoff (linked from CLAUDE.md) is the shared source of truth.
