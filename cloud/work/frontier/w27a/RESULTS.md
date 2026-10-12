# w27a — game render pool (wave 27)

Base: branch `wave27` = `8a54d6d2`. Builder scratch `watchman2:~/rush2049/scratch/frontier/w27a`, toolkit reused from wtk.
Flags unless stated: `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3).

Status: final. 3 strict MATCHes (camera_position_update, car_render_full, wheel_render_full: 4,540 B), 8 complete
drafts (7 draft_check EQUIVALENT; particle_render not reachable by any script), props_render not drafted.

Integration notes (coordinator):
- cloud/matches/camera_position_update.c: standalone MATCH, unit EQUAL; no overrides.
- cloud/matches/car_render_full.c + cloud/matches/wheel_render_full.c: standalone MATCH (own .rodata verified),
  unit EQUAL together with camera_position_update (`3/3 equal`, 0 locked bodies differ) ONLY with
  **src/blob/unit_overrides.json force_internal: `view_bind`, `view_bind_w`** (non-static inlined viewport-bind
  helpers; as `static` the deleted-procedure stub leaves 2 extra words after car_render_full that retail does not
  have). The two helpers have distinct names to avoid a duplicate symbol. No data-symbol changes.
- Shared idiom found this wave: the D_801461D0 font lock (`osRecvMesg; slot_state_setup(sel) [s2]; osSendMesg`) is
  inlined in every HUD caller; the locked object_create is kept and never inlined, so drafts carry nested
  static `font_lock`/`font_unlock`/`font_select` copies (nesting is needed for umerge to inline at 2+ sites, and
  the inline frame areas then match retail). Retail stubs func_800B41B8/func_800B42E8 (locked) may be those helpers.
- draft_check needs `--callee 's32 slot_state_setup(s32 sel)=s2/s0,s1,s3'` for these HUD functions (an o32 call
  crashes the host) and `--callee 'void sound_update_channel(s32 force)=t0'` where the bank getters are inlined.

| Function | Bytes | State | Scorer line |
|---|---:|---|---|
| camera_position_update | 452 | **MATCH** (standalone + unit) | `camera_position_update: MATCH` |
| wheel_render_full | 2072 | **MATCH** (standalone + unit), own rodata verified | `wheel_render_full: MATCH` / `own .rodata verified at 0x80123BEC..0x80123BFC` |
| render_thread_entry | 268 | NONMATCH 62/67 words (structure right; register/frame residual) | `FAIL render_thread_entry: 62 of 67 words differ | words 37 ops 10 norm 10 | frame 64/56` |
| hud_setup | 1048 | NONMATCH 32/262 (colour + one address web) | `FAIL hud_setup: 32 of 262 words differ | words 32 ops 3 norm 3 | frame 64/64`; draft_check EQUIVALENT (6 scripts) |
| dust_cloud_effect | 600 | NONMATCH 19/150 (13 with a disclosed 40-byte unused-local frame probe) | `FAIL dust_cloud_effect: 19 of 150 words differ | words 15 ops 5 norm 5 | frame 144/104` |
| particle_render | 448 | PROVISIONAL (stand-in caller) 9/112 words, colour-only proven | `FAIL particle_render: 9 of 112 words differ | words 15 ops 0 norm 0 | frame 128/128` |
| render_viewport_init | 924 | NONMATCH 176/231 (structure close, frame 192 vs 112) | `FAIL render_viewport_init: 176 of 231 words differ | words 151 ops 75 norm 75 | frame 192/112` |
| skid_mark_render | 1396 | NONMATCH 185/349 rows, ops 15 (registers/schedule) | `FAIL skid_mark_render: 185 of 349 words differ | words 140 ops 15 norm 15 | frame 64/64` |
| explosion_effect | 1460 | NONMATCH 282/365 (ops 64; frame 288/280) | `FAIL explosion_effect: 282 of 365 words differ; compiled body is 367 words, target 365; +0x148: .rodata+0x7e4: | words 247 ops 64 norm 64 | frame 288/280` |
| particles_update | 1464 | NONMATCH 153/366 (ops 7; frame ok) | `FAIL particles_update: 153 of 366 words differ; +0xf0: .rodata+0x460: retail words encode 0xb0254000, outside  | words 195 ops 7 norm 7 | frame 256/256` |
| props_render | 2180 | not drafted (analysis only) | - |
| car_render_full | 2016 | **MATCH** (standalone + unit), own rodata verified | `car_render_full: MATCH` / `own .rodata verified at 0x80123BDC..0x80123BEC` |

## camera_position_update (0x800AEB54, 452 B) — MATCH

Arcade counterpart: none found (N64 multiplayer player-pair visibility matrix). Start: near_miss_B70 rows draft (26/113).
- `score.py fn cloud/matches/camera_position_update.c camera_position_update` -> `camera_position_update:\n  MATCH`
- `blob_unit --tag w27a score camera_position_update --with cloud/matches/camera_position_update.c --neighbours` ->
  `EQUAL camera_position_update: 113 words (kept, c_camera_position_update.c)` / `locked bodies that differ in this unit: 0` / `blob_unit score: 1/1 equal`
- Levers (in order): matrix `s8` not `u8` (1 constant web shared with j = 1: `move s1,s3`); `v[1] += 5.0f` after the three
  copies (retail's $f0 web); clear loop with its own index `k` declared after `i, j` (frame 168 -> 176 and the 12 B above
  first[]; cfe gives every named local a slot); the clear loop on three lines (as1 delay-slot fill of the unrolled sb).
- No shaping devices, no unused locals. Deliverable: `cloud/matches/camera_position_update.c`. No unit_overrides.

## car_render_full (0x800A6404, 2016 B) — MATCH

N64 split-screen viewport layout (1 / 2 / 3-4 players); no arcade counterpart. Case 1 = matched sibling func_800A5A40
(cloud/matches), which supplied the idioms (u16 w/h locals -> lhu narrowing, arb_rate_set returns ArbEntry *).
- First complete draft (direct field stores, s16 Rect): `471 of 504 words differ`, frame 64/72.
- u16 block locals + f32 fov: 478/504 but structure right (case 1 v0/v1).
- Inlined static `view_bind(k, vp, rect)` returning `&D_8017A510[k]` (w15d lever: the unused return value is the
  dead `move v0,t0` / `lui v0,..` before each next call; also stops &D_8017A510 being held in s2): 426/504 rows but
  `words 190` and frame 64/64.
- Store order per pane h, w, x, y (y first when nonzero), then view_bind: `15 of 504` -> `9 of 504` -> EQUAL.
- Final: `score.py fn cloud/matches/car_render_full.c car_render_full` -> `car_render_full:\n  MATCH\n    own .rodata verified at 0x80123BDC..0x80123BEC`;
  `blob_unit --tag w27a score car_render_full --with cloud/matches/car_render_full.c --neighbours` ->
  `EQUAL car_render_full: 504 words (kept, c_car_render_full.c)` / `locked bodies that differ in this unit: 0`.
- Integration (IMPORTANT): as a `static`, view_bind leaves a deleted-procedure stub: with both files in the unit
  blob_unit printed `EQUAL car_render_full: 504 words (kept, ...); stub tail: 2 words of deleted-procedure stubs follow
  (NOT as in the image)`. The delivered files therefore define the helper NON-static (`view_bind` in car_render_full.c,
  `view_bind_w` in wheel_render_full.c, distinct names to avoid a duplicate symbol), and the unit needs
  **unit_overrides force_internal: view_bind, view_bind_w** (reason: inlined viewport-bind helper, no retail body/stub).
  Proof: `blob_unit --tag w27a score car_render_full wheel_render_full camera_position_update --with cloud/matches/car_render_full.c
  --with cloud/matches/wheel_render_full.c --with cloud/matches/camera_position_update.c --internal view_bind --internal view_bind_w --neighbours` ->
  `EQUAL car_render_full: 504 words` / `EQUAL wheel_render_full: 518 words` / `EQUAL camera_position_update: 113 words` /
  `locked bodies that differ in this unit: 0` / `blob_unit score: 3/3 equal` (no stub tail). Standalone `score.py fn` MATCH
  for both forms. Helper identity is a disclosed hypothesis (retail has no stub for it, so it was not a static of this TU).

## wheel_render_full (0x800A6BE4, 2072 B) — MATCH

Twin of car_render_full (2-player panes are 3/4 width at x = 2, unscaled fov). Started from the car match:
first draft `3 of 518 words differ` (one store order), then EQUAL with pane-2 order h, x, w, y.
- `score.py fn cloud/matches/wheel_render_full.c wheel_render_full` -> `wheel_render_full:\n  MATCH\n    own .rodata verified at 0x80123BEC..0x80123BFC`
- unit: see the car_render_full line above (3/3 equal, 0 locked bodies differ). Needs force_internal view_bind_w.

## render_thread_entry (0x800E7808, 268 B) — NONMATCH, best `render_thread_entry/best.c`

N64 scheduler-client thread (sibling of the matched dma_wait_complete/audio_queue_process threads): __setfpcsr,
osCreateMesgQueue(&D_8017A498, D_8017A4B8, 8), func_800E7710(), D_801525F0 = 0, func_800E762C(0),
osScAddClient(D_8002E8E8, &client, &q), then forever: recv BLOCK, drain NOBLOCK until -1, and
`if (func_800E73D8() && D_801525F0) func_800E6AF8();`. No arcade counterpart.
- first draft (while-drain): `65 of 67 words differ`; `do {} while (recv(..., 0) != -1)`: `FAIL render_thread_entry: 62 of 67
  words differ | words 37 ops 10 norm 10 | frame 64/56` (unit, us.sh). Every op matches; residual = registers + frame.
- Residual class: allocation. Retail colours SIX s-registers: s0 &msg, **s1 = constant 1** (`move a2,s1` for
  OS_MESG_BLOCK), **s2 saved but never used**, s3 = -1, s4 &queue, s5 &D_801525F0; ours has 4 webs (ctrace: w17 s0,
  w18 -1, w12 &q, w14 &flag; the constant 1 is not even a candidate). Retail also uses t7 (not t6) for the flag load,
  i.e. one more ugen temp was taken earlier in the loop. Frame 64 vs 56 is the two extra saves only.
- Tried (no strict gain, all disclosed as non-natural): `running` loop flag (47-48/67 rows but code differs), `ret`/`ok`
  locals (62), unreachable tail code (62), block/noblock variables (59, devices). `score.py fn`: 66/67 words.
- draft_check (w27r): `VERDICT: EQUIVALENT over 6 calls / 6 scripts (attract 1, attract_pak 1, audio_options 1, backward 1, battle 1, circuit 1)` -> logic right, compiler shaping only.
- Next hypothesis: the body contained something that IDO removed after colouring (an s2 web plus a second use of 1),
  e.g. an inlined static (check umerge-inlined helpers: func_800E7710/func_800E762C are locked neighbours) or a
  debug block compiled out; or func_800E6AF8/func_800E73D8 are IPA-internal and clobber s2 (frontier shows no
  unsaved regs for them, but both are unmatched - re-check once func_800E6AF8 (w27b) lands). Infinite-loop epilogue
  alignment (`.align 5`, see dma_wait_complete) will also need an `align` unit override.

## hud_setup (0x800C8B8C, 1048 B) — NONMATCH 32/262, best `hud_setup/best.c`

Historical label; really the N64 sound-system init (sibling of the matched hud_speed_display = list/pool init, whose
`list_init` static + func_800A370C idiom it reuses). (u8 cfg, u8 voices, u8 rate, numA, numB, f32, f32, s32): run-once
flag D_8011023C (if set and the two settings are unchanged return, else func_800C8918() shutdown), store settings,
clear D_80142DD8[128].active, create/prime the slot-queue lock D_80142728 once (D_80110274), car_shadow_render(cfg,
voices, rate), start the audio thread (osCreateThread(&D_80034F00, 10, func_800992AC, 0, stack, 10)) on first init,
voice count = clamp(2*voices, 64) then max(voices + numB), allocate the 68-byte voice pool and a 12-byte array,
three empty lists, handler D_80143F58, link every voice into D_80143FF8, D_801460F8 = func_800C8738(count) (+1 if
1<<n < count), mask D_80146104, 6 channels of 60 B into D_80149808, then hud_speed_display(numA, numB, f, f, s32).
- first draft: `250 of 262 words differ` (frame 56); car_shadow_render taking ints + max on numB: 136/262;
  `i` declared before `reinit` (memory-resident flag at 56(sp)): 133; `D_8011024C = D_80110250 = 0;` (retail keeps
  &D_80110250 in a0): `FAIL hud_setup: 32 of 262 words differ | words 32 ops 3 norm 3 | frame 64/64` (unit).
- Residual: (1) retail re-forms `lui` for each access of the once-flag D_80110274, ours shares one address web
  (v0); a function-local `static s8` reproduces it (`best_static_flag.c`: 24/262) but then the flag is our .bss
  (`+0xac: .bss+0x39f0: retail words encode 0x80110274`), so not usable without a data-placement answer;
  (2) colour: retail's D_80146200 reload is v1 / doubled v0 / count v0 / sum a0, ours one register "lower"
  (v0/v1/a0/v1); the count web is v0 in retail (then `move a0,v0` for func_800C8738) and a0 in ours. ctrace taken
  (`ctrace.sh hud_setup h`), not resolved; next: force.sh on the w47/w5 webs to see whether colour alone closes it.
- draft_check: `VERDICT: EQUIVALENT over 6 calls / 6 scripts (attract 1, attract_pak 1, audio_options 1, backward 1, battle 1, circuit 1)`.

## dust_cloud_effect (0x800EFEA8, 600 B) — NONMATCH 19/150, best `dust_cloud_effect/best_nounused.c` (and `best.c`, 13/150)

HUD sprite callback (PadConfig-style record): kind = packed & 15, mode = (packed >> 4) & 15; disabled =
(mode == 1 && players >= 2) || (mode == 2 && players < 2) || kind >= players; on change set +26 and refresh
(Input_ApplyPadConfig); disabled -> active = 0. Then hidden unless D_8015F72C and car mode != 1; font select under
the D_801461D0 lock (font 4 for one player, else 1), height = camera_shake_update(56), place x/y from the per-player-count
table D_80115A68[players-1][kind] (prior draft codex_dust_a141 used D_80115B68: wrong address), half18 = 100, status.
Start: cloud/work/ipa-groups/codex_dust_a141/group.c (127/150).
- whole-program unit (slot_state_setup's real body is in the unit via credits_scroll_grp): the locked object_create is
  never inlined (w17h rule), a one-level static `font_select` is not inlined at 2 sites (`refers to func`); with
  nested static `font_lock`/`font_unlock` (osRecvMesg/osSendMesg on D_801461D0) inside `font_select(sel)` umerge
  inlines all of it and the inlined return value lands in retail's 76(sp)/52(sp) homes.
- `||` chain for `disabled`: `FAIL dust_cloud_effect: 19 of 150 words differ | words 15 ops 5 norm 5 | frame 144/104`.
- Frame: retail 144 has 40 more bytes of NAMED locals (4 above `kind`, 32 below the four ints). Probe with unused
  `s32 t0_;` first and `s32 t1_[8];` after: `FAIL dust_cloud_effect: 13 of 150 words differ | words 9 ops 5 norm 5 |
  frame 144/144` (all sp offsets now match) -> the layout guess is right, but the 32-byte block is unknown structure
  (not acceptable as a pad array; candidate: a real local struct/array the original used and IDO deleted).
- Remaining 13: tail `y = table[..][1] + (players == 1)`: retail is an if/else (`li v0,1` / `move v0,zero` arms,
  `move a0,s4` hoisted), ours a default-0 + set; plus as1 order of the `half18 = 100` / status stores. Tried ternary,
  if/else into a local, operand orders, one-line layouts (all 13).
- draft_check (best_nounused.c, `--callee 's32 slot_state_setup(s32 sel)=s2/s0,s1,s3'`; without it the o32 call crashed the
  host): `VERDICT: EQUIVALENT over 19647 calls / 6 scripts (stunt_end 9341, two_player 2197, backward 2031, ghost_race 2031, practice 2031, other_car 2016)`.
- Integration note: three static helpers (font_lock/font_unlock/font_select) -> retail has caller-less stubs
  func_800B41B8 / func_800B42E8 near slot_state_setup/object_create (both locked); the helper identity is open.

## particle_render (0x800B8650, 448 B) — PROVISIONAL near-miss, best `particle_render/best.c`

Internal (IPA) function: saves no s-registers; its only caller particle_collision (0x800B8820, unmatched, itself
saving nothing) does. Menu box layout: font select 11 under the D_801461D0 lock, then for the 4 rows with
D_8011AD40[i] == 1: rows++, label width = MAX(width(strings[text->first + i])) (MAX macro: width called twice),
row 2 value = sprintf(buf, "%s  %s", strings[text->value], strings[text->value + 1]) else the literal "000" (the
macro duplicates the literal: retail strings at 0x80122684 and 0x80122688); box w = label + value + 10,
h = font height (object_bytes_sum_global) * rows, x = 160 - w/2, x2 = x + label + 5 into D_8011AD70.
(object_manager_update = string width, fcvt_wrapper = sprintf, by use.)
- Kept in the unit: saves inflate the frame (`107 of 112`). `--internal particle_render` alone empties it (no caller in
  the unit); a one-call stand-in gets it inlined; a stand-in with three call sites
  (`__standin_particle_render`, appended in best.c, UARGS `--internal particle_render --keep __standin_particle_render`)
  keeps it a real internal body.
- void `font_set(11)` built from nested static font_lock/font_unlock (frame 120 -> 128), `s8 flag` local, and the
  sprintf arguments as `(&strings[value])[0]`, `[1]`: `FAIL particle_render: 9 of 112 words differ | words 15 ops 0 norm 0 | frame 128/128`.
- `force.sh pr4 particle_render p1:w17=c19,p1:w78=c18,p1:w15=c2` -> only the 6 own-rodata string rows remain:
  colour-only residual (buf address web w78 wants s4, the strength-reduced flag pointer w17 s5, flag w15 v1).
  `if (buf) {}` levers tried (x1-x3): no gain. Next: priority lever on w78 (one more weighted use of buf).
- PROVISIONAL: needs particle_collision (unmatched) as the real caller. draft_check: census says no script reaches it.

## render_viewport_init (0x800FA9B4, 924 B) — NONMATCH 176/231, best `render_viewport_init/best.c`

Historical label; really the per-frame race update tail: race_init_helper, mode dispatch (4: camera_scene_manager,
6: the two image-A routines 0x8038FCE0/0x80390F60, else 5: func_800D169C or render_large_objects, then
func_800F8EC8), hud_render, camera_position_update (matched this wave), then per car (player_array / D_8014A250 game
records): on a "just finished" flag (+237 < 0): bump D_8014A118[slot].finishes unless D_801174B4 & 8, save the car's
camera position (D_80150B70[cam] +36 -> +132) and switch camera mode to 4, save_write_data(&slot, 1, 1.f, 0),
func_800D5524(g), the finish jingle (type 2: entity_flags_apply(45, slot, 1, 1) unless a 2-player ghost check;
else camera_target_track(g+556, D_801141B0, 400, 0, 1, 0, 45, slot, 0, 128)), func_800C3578(slot) in mode 4;
on +237 > 0 the reverse; cpak_read(slot); race_setup_2(slot) for state 0/6 entries; then race_setup_1,
menu_controller_remap, skid_mark_render unless D_80114650.
- first draft `230 of 231` (frame 192/120); `1.f` for save_write_data's 1.0 (retail does not share it with the
  camera_target_track 1.0f: two spellings) 176; type==2 arms in retail order: `FAIL render_viewport_init: 176 of 231 words
  differ | words 151 ops 75 norm 75 | frame 192/112`.
- Residual: frame 192 vs 112 (retail `slot` home at 186(sp): ~80 more bytes of locals/inline areas, unknown), the
  s-register assignment (retail s5 = 4, s6 = &gameplay_mode, s4 = &D_8010FFC0), and the loop-head schedule.
  Next: find what owns the 80 bytes (an inlined helper with a local array? spcensus), then the s-reg order.
- draft_check: `VERDICT: EQUIVALENT over 30717 calls / 6 scripts (stunt_end 9082, attract_pak 8191, attract 7681, backward 1921, ghost_race 1921, practice 1921)`.

## skid_mark_render (0x800F0100, 1396 B) — NONMATCH (ops 15), best `skid_mark_render/best.c`

Historical label; really the in-race tip/hint HUD (counterpart of records_screen's teardown): on first call clear the
three per-player float tables and the 4 record rows' a/b ids (-1), create the HUD objects (sound_control = object
create: D_80154198 always, D_801541A4 for 2/3 players, D_801541A0 unless image-A mode 4/6 which calls 0x803925D0).
In attract/demo (D_801174B4 & 8) with D_80138660 set: every D_8015462C seconds (viScheduleTick(90) /
viGetTimeToDeadline() / 13 on creation) pick the next tip, a random corner (libc rand() = func_8008B2B4 inlined,
*4/32768), measure the title and the NULL-terminated line table D_80114A0C[tip][11] (MAX macro: width twice), place
the box in the corner and draw it (sound_handles_clear(0); ambient_sound_set(x, y, x+w+20, y+lines*10+32, 192, 0,0,0)).
- draft a (u32 seed via func_800D50E4): 178/349; with `volatile f32 D_8002EB90` (as frontier_mode_select) and libc
  rand: `FAIL skid_mark_render: 185 of 349 words differ | words 140 ops 15 norm 15 | frame 64/64`.
- Residual: ours keeps &D_80154644 in a0 (retail re-forms `lui at` per access and keeps the value in $f2 from the
  compare to the add), which shifts the FP and temp registers of the whole tip block; volatile (c), a struct view (d)
  and statement orders (e-g) did not fix it. Next: ctrace on the address web; check whether retail's D_80154644 is
  defined in the unit (w15d).
- draft_check (`--callee 's32 slot_state_setup(s32 sel)=s2/s0,s1,s3'`): `VERDICT: EQUIVALENT over 30729 calls / 6 scripts (stunt_end 9083, attract_pak 8192, attract 7688, backward 1922, ghost_race 1922, practice 1922)`.

## explosion_effect (0x800EF8F4, 1460 B) — NONMATCH, best `explosion_effect/best.c`

Historical label; really the per-player 7-segment speedometer: if D_8015F72C, render_helper(0), pair D_80118E20 =
{1, 3} (inlined `func_800B669C`-style pair setter, retail keeps &D_80118E20 in v1), font 4/1, char width =
camera_shake_update(56) + 1; per player (skip if player_array[g->slot].mode == 1): speed = (car.speed >> 2) * 1.2f
(* 1.61f for km/h, D_801613AA), abs, three digits `(s32)(speed/100) % 10 + '0'` ...; each digit drawn twice
(a dim '8' background with dispatch_handler(0) unless ':' or '.', then the digit with colour 14) via
state_utility(x, y, str); unit label y += (s16)(bank byte2 + byte3) - 4 (object_type_byte2_get /
object_type_byte3_get inlined: sound_update_channel(0) with t0 = 0, IPA keeps byte2 in t1), font 12, shadow + label
from D_8017A4E4->+808 (mph) / +812 (km/h); finally pair = {0, 3}, render_helper(-1).
- First complete draft with static font_lock/font_unlock/font_select, pair_set, bank_byte2/3 copies (the locked
  credits-group getters are kept and are never inlined into a new caller, w17h):
  `FAIL explosion_effect: 282 of 365 words differ; ... | words 247 ops 64 norm 64 | frame 288/280` (str[4] same).
- Residual class: logic close, register/spill layout far: retail spills g (80(sp)), i (256), y (264, read back as
  lh 266), kind*8 (76) and keeps '.'/':' in s8/s7, str at 276, digits 280..283; ours colours differently and the
  frame is 8 short. Next: declaration order and which locals are memory-resident (spcensus), then the digit loop.
- draft_check (`--callee 's32 slot_state_setup(s32 sel)=s2/s0,s1,s3' --callee 'void sound_update_channel(s32 force)=t0'`):
  `VERDICT: EQUIVALENT over 19361 calls / 6 scripts (stunt_end 9336, backward 2026, ghost_race 2026, practice 2026, other_car 2011, circuit 1936)`.

## particles_update (0x800B7A40, 1464 B) — NONMATCH 153/366 (ops 7), best `particles_update/best.c`

Historical label; really the per-player coin-counter HUD (2 counters per player: models "GOLDCOING_COIN" /
"SILVERCOINS_COI" in D_801541A8 rows, created on demand via string_copy_format + func_8008E26C, model_data_load,
scale 10; placed with func_8008B32C (0.03f/0.04f by player count), car_lights_render(i, &pos, cam, 2.0f, obj);
with D_80146113 and a non-ghost car: model_transform_setup + sprintf(buf, "%d/8", func_800B78F0(i, j+1)) drawn with
shadow; else the model is hidden). Position tables D_80115E28 / image-A D_80394050 (mode 4).
- a: 326/366 (frame ok); names array in the inner block + u8 D_80140BDC: 317; both mode arms written out in the
  text branch (no table pointer): `FAIL particles_update: 153 of 366 words differ; ... | words 195 ops 7 norm 7 | frame 256/256`.
- Residual: the named-local block is 8 bytes too big (retail: pos 252, buf 244, 4 bytes, block `names` at 232;
  ours names at 224, spill homes 116/124 vs 108/120): two of {i, j}, {x, y}, rec are not separate named locals in
  retail (dropping rec: frame 248; text coordinates through pos: worse). Then the mode-4 pos arm order.
- draft_check (slot_state_setup shim): `VERDICT: EQUIVALENT over 19361 calls / 6 scripts (stunt_end 9336, backward 2026, ghost_race 2026, practice 2026, other_car 2011, circuit 1936)`.
- Own data: the `names` initializer is compiler-emitted .data (retail 0x801161BC = {0x80120E0C, 0x80120E1C}).

## props_render (0x800F6AB8, 2180 B) — not drafted

Group recipe with func_800CBF2C (register parameters s0, s1, s3; unsaved s2; unmatched, blockers draw_ui_element,
drone_set_catchup, menu_back). 545 words: player-session setup for mode 2 (`D_80156994 ? 4 : 2` slots, pointer
tables D_801409F0/D_80142500 strided 48/88, memcpy of names/records from D_8014A118[i] (+72 chain), func_800CDDE8,
audio_dma_sync(0, n*3*4) record pools, then a func_800CBF2C(...) loop over D_80152698[] with list compaction).
Prior evidence: cloud/work/ipa-groups/codex_menu_entry_b126, codex_menu_value_b126, src/blob/groups/menu_cc040_20261004.
Not started this wave (time went to the 11 others); next lane should start from func_800CBF2C's register contract
(regmap_verify) before drafting.
