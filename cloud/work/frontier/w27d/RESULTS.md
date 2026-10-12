# w27d RESULTS (wave 27, game misc pool, IDO 5.3)

Lane w27d final (wave27 HEAD 08183a3d). Sections below per function; variants live under each `<fn>/v/`.

| Function | Bytes | State | Flags | Scorer line |
|---|---:|---|---|---|
| func_800A5A40 | 252 | **MATCH** (cloud/matches) | -O3 | `MATCH`; unit `EQUAL func_800A5A40: 63 words`, 0 locked differ |
| save_settings | 584 | **MATCH** (cloud/matches) | -O3 | `MATCH`; unit `EQUAL save_settings: 146 words`, 0 locked differ |
| menu_controller_remap | 1,548 | **MATCH** (cloud/matches) | -O3 | `MATCH` + `own .rodata verified at 0x80124124..0x80124128`; unit `EQUAL … 387 words`, 0 locked differ |
| func_800D5A04 | 428 | **unit EQUAL** (group dir) | -O3 | unit `EQUAL func_800D5A04: 107 words`, 0 locked differ; score.py fn/group `13/107` (temp ring) |
| func_8010B5D0 | 556 | draft, EQUIVALENT | -O3 | unit `20 of 139`, rows 7, ops 3, frame 152/152 |
| func_8010B7FC | 460 | draft, EQUIVALENT | -O3 | unit `42 of 115`, ops 5, frame 56/56 |
| func_8010B9C8 | 700 | draft, EQUIVALENT | -O3 | unit `108 of 175`, rows 63, ops 3, frame 152/152 |
| debug_collision + debug_stats | 532+532 | draft pair, EQUIVALENT | -O3 | unit 3 rows (ops 3) + ops 18 (`--internal debug_stats`) |
| func_800C4F68 + camera_follow_path | 1,416+1,484 | draft pair | -O3 | unit 13 rows (ops 2) + 33 rows (ops 0) (`--internal camera_follow_path`) |
| music_seq_load | 1,428 | draft (ops 42) | -O3 | unit `323 of 357`, ops 42, frame 192/192; draft_check host crash |
| func_800DE45C | 1,020 | draft (ops 72–80) | -O3 | unit `220 of 255`; unrolled-loop head unresolved |
| fp_call_wrapper + brake_force_apply | 104+196 | blocked (w27e callees) | -O3 | unit `0/2 equal` |

## func_800A5A40 (MATCH)
`cloud/matches/func_800A5A40.c` (= `w27d/func_800A5A40/best.c`). Viewport-0 setup: `arb_rate_set(0, &D_8011EA30, D_80149870,
D_80154188, 0.0f, w, h, w/2, h/2)` then entry 0 of D_8017A510 re-bound and rect D_80149870 = {0,0,(u16)w,(u16)h}.
Unit: `blob_unit --tag w27d score func_800A5A40 --with .../v/v30.c --neighbours` -> `EQUAL func_800A5A40: 63 words (kept, c_v30.c)`,
`locked bodies that differ in this unit: 0`, `1/1 equal`.
Devices (disclosed in header): (1) arb_rate_set declared returning `ArbEntry *` (retail arb_rate_set leaves
&D_8017A510[idx] in v0 at `jr ra`), result kept in `e` and read by compiled-out `if (e) {}` at the end: blocks v0
(h/w colour a0/v1) and ends the block so `lw ra` stays after the stores (`jr ra; nop`). Without it 10 words.
(2) `w = D_8002AFC0; h = D_8002AFC4;` on one source line: as1 breaks aftercycles ties by `.loc` line; two lines
leave the lui/lhu pair or the store pair swapped (4 words). Path: 19 -> 15 -> 8 -> 4 -> 2 -> 0 words (as1t trace
showed the line-number tie). Integration: single (splice_singles). arb_rate_set's prototype elsewhere says `void`;
no locked file needs changing for this TU.

## save_settings (MATCH)
`cloud/matches/save_settings.c`. Modal settings dialog runner: sound_control → display_settings(a0, mode, 10, 10, 300, 180)
→ save + override the per-viewport fog words (func_800A7428 / func_800A7480 on D_8017A510[i]+0x40) → loop
control_settings(a0, arg4, arg5, &done, arg6) until done (game_mode_handler on thread 7; func_800C813C(0,0) unless
mode is 10–13) → sound_stop → restore. Draft 1 was 18 words (stack layout only); declaring `done`, `snd` before the
arrays (retail slot order 151/144/136/128/124/120/116/112) gave `MATCH`. No devices.
Unit: `EQUAL save_settings: 146 words (kept, c_v2.c)`, `locked bodies that differ in this unit: 0`. Integration: single.
Prototypes chosen (callees unmatched): func_800A7480(s16, s16, u8, u8, u8, u8, s32) (callee reads args 5/6 with lbu),
control_settings 5th arg is a function pointer (callee does `jalr` on it).

## menu_controller_remap (MATCH, 1,548 B)
`cloud/matches/menu_controller_remap.c`. Arcade ancestor: `detect_drone_abort()` in `game/resurrect.c` (stuck timer
→ resurrection, then help-arrow angle per player viewport). `score.py fn` → `MATCH`, `own .rodata verified at
0x80124124..0x80124128`. Unit: `EQUAL menu_controller_remap: 387 words (kept, c_v16.c)`, `locked bodies that differ in
this unit: 0`. Path: 331 → 97 ops (Vec3s path points `s16 (*)[3]`, arcade locals) → 29 (`delta[k]` written per branch,
no point local) → 4 (player node read twice from D_8014A118 instead of a `node` local: kills the -1 hoist and puts
node in s4) → 0 ops (player loop counts with `node`, the 5-step walk with `i`) → 12 words sp offsets → EQUAL with the
arcade's full declaration list. Disclosed: unused arcade locals index1, index2, dist1, dist2, pos[3], angle = exact
frame residual (168 vs 144 without them). Integration: single. Struct offsets recovered (MODELDAT 0x808: magvel 0x3F0,
time 0x714, slot 0x7C6, we_control 0x7CA, stuck_time 0x7D8, please_abort_me 0x7E0, drone_scale 0x7EC; CAR_DATA 0x3B8:
dr_pos 0x08, dr_uvs 0x2C, place_locked 0xEF, mpi/seg 0xFA/0xFC, distance/last 0x100/0x104, fwd_progress_t 0x340,
we_died 0x358, stray angle 0x364).

## music_seq_load (draft, 42 ops rows; logic known, shaping left)
`w27d/music_seq_load/best.c`. Car-object setup (arcade svisuals.c CreateCar/SetupCar shape; N64 layout differs).
Unit `us.sh`: `FAIL music_seq_load: 323 of 357 words differ ... | words 189 ops 42 norm 42 | frame 192/192`;
standalone `326/357 words differ`. Residual class: logic known, compiler shaping left (ops rows are register/order).
Frame: retail 192 vs 96 for the plain draft; a static 7-parameter wrapper around save_slot_valid inlined at exactly
the three body/hood/brake sites gives 192 (each inline site of an N-parameter helper costs 4·(N+1) B here: 2 sites of a
3-param helper +32, 12 sites of the 7-param one +384) — a hypothesis, not proven. Open: (1) retail narrows the tint
object handle twice from one `lw 20(s0)` (two sll/sra pairs) — an s32 value passed to something s16 per use, not
`lh 22(s0)`; an inline `set_obj_colors(s16/s32 obj, s32 *a, s32 *b)` was worse (52/67 rows); (2) body flags in a3
with `move a3,v1; ori t5,a3,0x18; move a3,t5` (parameter-like variable); (3) tyre flags: retail ORs `refl` inside
both arms of `if (!for_game)` (v2 form, 59 → 42 rows). Next: find what the 3 inline sites really are (frame says
3×7-param or equivalent), then the handle narrowing.

## fp_call_wrapper + brake_force_apply (blocked on w27e)
Bodies in `w27d/fp_call_wrapper/best.c` (same as the audio_frame_sync research group). Both only call w27e's
register-parameter callees func_80096CA8 ($fp slot, $t0 first) / func_80097164; retail saves s0–s8 because the callees
clobber them. `blob_unit score fp_call_wrapper brake_force_apply` → `0/2 equal` (callees are not C in the unit).
Integration: hand both bodies to w27e's component-104 unit; nothing to do standalone.
draft_check (w27r): `music_seq_load` → every script `ERROR oracle: host exited -11` (replacement host crashes; race
script crashes after frame 700, no call log written). Not diagnosed; the draft's data views were re-checked against the
retail loads. Treat the draft as unverified behaviour.

## func_800C4F68 + camera_follow_path (component 13, draft: 13 + 33 words, ops 2 + 0)
`w27d/func_800C4F68/best.c` (members only; started from `cloud/work/ipa-groups/codex_camera_reset_b135/group.c`).
Unit with `--internal camera_follow_path` (matrix arrives in s0; func_800C4F68 saves s0/f20/f22 for it):
`FAIL func_800C4F68: 13 of 354 words differ | words 13 ops 2 norm 2 | frame 104/104`;
`FAIL camera_follow_path: 33 of 371 words differ | words 33 ops 0 norm 0 | frame 104/112`.
Was 347/354 (28 ops) and 208/371 (77 ops) for the research draft in this unit. Levers that worked: `switch (mode)`
(case 0 / case 2), `state->current == count - 1` (no CSE of current+1), delta computed inside both arms of each
end-of-path test (retail duplicates them), the locked prototype `f32 func_8009C3F8(f32 x, s32 flag)` (the draft had it
swapped; that alone took camera_follow_path from 4 ops to 0), arrays declared after `angle`. Residual class: colour +
frame. Left: camera_follow_path's argument registers (retail pathMode a0 / point a1; ours swapped — decided by the
callee's colouring), FP colouring (retail heading f22, sine/vertical f20), `addu v0,a0,t6` operand order (descriptor base
first), local area (retail: 8 B above `angle`, a 4 B slot between angle and surface; our 5 pointer locals give 20 B
above angle; with 3 pointers the frame is 104 but arrays sit 8 B low; declaration order of the pointers has no effect),
and in func_800C4F68 the cross-product FP colouring.
draft_check: `DIVERGES` on attract_pak/backward/circuit/two_player, `EQUIVALENT 11 calls` on stunt_end, `ERROR -11` on
attract. First divergence attract_pak frame 614 call #1: matrix words differ in sign — expected from the oracle build:
func_8009C3F8 takes its input in f16 (IPA) and the standalone draft calls it o32 (`--allow-ipa` warning). A fair check
needs a static copy of locked func_8009C3F8 (frontier_particle_knot group) in an oracle source (brief item 4); not done.
Integration (when it closes): new group with both members, `--internal camera_follow_path`; menu_controller_remap
(already matched) is its caller.

## func_800D5A04 (unit EQUAL, 428 B)
`w27d/groups/func_800D5A04/` (group.c = the function; six byte-identical locked context copies). Body from
`dot_transition_d5a04_research_20261006` with `osJamMesg` → `osSendMesg` (retail jal 0x800075E0; renamed symbol) —
that was the only change. Unit: `blob_unit --tag w27d score func_800D5A04 --with groups/func_800D5A04/group.c
--neighbours` → `EQUAL func_800D5A04: 107 words (kept, c_group.c)`, `locked bodies that differ in this unit: 0`.
`score.py fn` and `score.py group` → `13/107 words differ`, all ugen temp-ring names (`lb t1` vs `lb t7`, …): the ring
position comes from procedures emitted before it in the unit (records_screen, func_800D58C4, player_state_clear …);
context copies (all MATCH in the group) and file order did not reproduce it; dropping them from `keep` inlines them.
Not in cloud/matches. Integration: install through the whole-program unit gate (no unit_overrides needed in the unit
run); coordinator call on packaging. draft_check: not run (census: no script reaches func_800D5A04).

## drone_collision_avoid, func_800DD0C0 (skipped: component 104)
Both sit in register-passing component 104 with slot_state_setup / audio_frame_sync (w27e's area; census also says
func_800DD0C0 is unreached). Not attempted.

## func_800DE45C (draft, logic complete; 220/255, ops 80)
`w27d/func_800DE45C/best.c`. Settings/race reset (player records, 41 option bytes via func_800CCB40, two IPA
`speed_set(blend f20, amount f22, first s1, second s2)` calls with 0.05f, option bytes copied to ~35 globals, flags
cleared). Unit: `FAIL func_800DE45C: 220 of 255 words differ ... | words 88 ops 80` (the v2 form, D_80146108[120..123]
instead of the separate D_80146180 bytes, is ops 72 but wrong on the tail). Residual class: shaping (one block).
Retail's first 50 words are a fully unrolled 4-player loop (per-store `lui at` and fresh `li` from the t6–t9 ring, no
base/constant CSE); explicit statements CSE them, and no loop form tried unrolls (s32/s16/s8 index, do-while, `!=`,
volatile array). Next: find what makes uopt unroll (w14k/w15i/w16c RESULTS mention unroll-by-4 cases) — or an inlined
helper with the loop. Also `slti at,s0,41` kept by retail in the 41-loop. draft_check: not run (speed_set takes IPA
register parameters; an o32 standalone call would diverge — needs an oracle source with a static speed_set copy).

## debug_collision + debug_stats (draft pair; 3 words + ops 18; draft_check EQUIVALENT)
`w27d/debug_collision/best.c` (both functions). debug_stats is internal (callers `move s0,zero` = its second, s16
parameter reused as the loop counter; it saves no s-registers). Unit with `--internal debug_stats`:
`FAIL debug_collision: 3 of 133 words differ | words 3 ops 3 norm 3 | frame 48/48` (the three `move s0,zero` come
out `sw zero,4(sp)`), `FAIL debug_stats: 117 of 133 words differ | words 60 ops 18 norm 18 | frame 24/24` (we colour
&D_8011415C in v0 and the constant 1 in s0, which also costs `i` its s0; retail keeps both as ring temporaries).
`volatile` on D_8011415C: no change; `s32 i`: worse (46 ops). draft_check (w27r):
`VERDICT: EQUIVALENT over 7716 calls / 1 scripts (attract 7716); never called in: attract_pak` — logic right, colour
left. Next: make the then-branch constant 1 / D_8011415C address non-candidates (block split? inlined helper for the
first-entry block).

## 0x80106B3C–0x8010F218 block (component 104 handlers)
All twelve are menu/HUD callbacks that call slot_state_setup (font selection in s2). slot_state_setup's C body is in
the whole-program unit via credits_scroll_grp, so they can be unit-scored directly; the credits_scroll_grp
gfx_lock/gfx_unlock/font_set statics are the idiom.

### func_8010B7FC (draft, 42/115, ops 5; draft_check EQUIVALENT)
`w27d/func_8010B7FC/best.c`. `us.sh`: `FAIL func_8010B7FC: 42 of 115 words differ ... | words 18 ops 5 norm 5 | frame
56/56` (draft 1: 17 ops). Levers that worked: x/y as s32 (17 → 8), `(s16)x` inside the state_utility argument (retail
hoists the narrowing of x out of the loop; 8 → 5), an inlined `item_refresh(item)` that returns the dirty flag (retail
keeps the flag in v1). Left: argument-narrowing temp order at the state_utility call (retail `subu t7; sll a0; sll a1;
sra t6; sra t8; move a0; move a1`), s5–s7 assignment of the loop invariants. A static `text_at` wrapper changed nothing.
draft_check (`--callee 's32 slot_state_setup(s32 sel)=s2/s0,s1,s3'`): `VERDICT: EQUIVALENT over 855 calls / 6 scripts
(saves 240, audio_options 195, stunt_end 150, race 90, race_track2 90, race_track3 90)`.

### func_8010B5D0 (draft, 20/139 words, 7 rows, ops 3; draft_check EQUIVALENT)
`w27d/func_8010B5D0/best.c`. `us.sh`: `FAIL func_8010B5D0: 20 of 139 words differ; compiled body is 138 words, target
139 | words 7 ops 3 norm 3 | frame 152/152`. Same family (one centred text line: fcvt of the selection, a language
string for modes 7/8, empty for 6, or the player-name line). Disclosed devices: `item_refresh` returning the flag;
nested static `font_select` inside `font_set` (puts `old` at retail's 68(sp): each inlined site costs 8 B below it);
gfx_lock/gfx_unlock defined after the function (as1 line-number tie: moves `sw v0,68(sp)` out of the jal delay slot);
unused `s32 i` = frame residual (152; 144 without it, sp offsets off, 23 rows). Left (2 rows + one word): the first
state_utility argument narrowing (retail `subu t9; sll a0,t9; sra t6,a0; move a0,t6`) and `sw v0,68` vs `addiu a0`
order. draft_check: `VERDICT: EQUIVALENT over 960 calls / 6 scripts (saves 345, audio_options 195, stunt_end 150, race
90, race_track2 90, race_track3 90)`.
Family lesson: the narrowing pattern at state_utility calls (into temps, then `move a0`) is the same in both and is not
a source-visible inline wrapper (`text_at(s16,s16|s32,…)` adds 16 B of frame per site); next: check how locked callers
of state_utility in credits_scroll_grp/game_results_* emit it.

### func_8010B9C8 (draft, 63 rows, ops 3; draft_check EQUIVALENT)
`w27d/func_8010B9C8/best.c`. `us.sh`: `FAIL func_8010B9C8: 108 of 175 words differ ... | words 63 ops 3 norm 3 | frame
152/152`. Disabled when D_801170FC >= 7: retail's refresh is `if (flag != item->dirty) { item->dirty = flag; refresh }`,
so the family helper is probably `item_disable(item, flag)` returning the flag (B5D0/B7FC pass 0; using it there is
code-identical). Header string centred, two option strings at x ∓ width/4, selection D_80117354 highlighted. Left:
inlined font_set `old` slots (retail 108/84 = 24 B apart; with nested font_select ours are 32 apart, without it 24 but
the named block moves) — a 30-variant unused-local sweep did not place both; the narrowing temp order. draft_check:
`VERDICT: EQUIVALENT over 105 calls / 1 scripts (saves 105)`.

## Not reached this wave
func_800C7EC8 (IPA 3-fn unit, unreached by scripts), track_texture_load, stunt_combo_display (group c13),
func_80106B3C, func_80107EDC, func_80108154, func_80108AB0, func_8010A53C, func_8010BC84, func_8010DCFC, func_8010E8B4,
func_8010F218. The family template (`w27d/func_8010B5D0/best.c`: lock/font_set statics, item_disable, `(s16)x`
narrowing, s32 x/y) should apply to func_8010BC84 and the other slot_state_setup callers; score them with `us.sh`
(slot_state_setup's body is in the unit) and draft_check with `--callee 's32 slot_state_setup(s32 sel)=s2/s0,s1,s3'`.
