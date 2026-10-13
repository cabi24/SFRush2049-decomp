# w29b RESULTS (wave 29, menus / options / controls, IDO 5.3 -O3)

Status: FINAL (wave 29). Builder scratch `watchman2:~/rush2049/scratch/frontier/w29b`; unit tag `w29b`.
Starting drafts: `w28c/<fn>/best.c`, `w27d/func_8010B5D0|B7FC|B9C8/best.c`.

| Function | Bytes | Score before → after | draft_check | Residual | Notes |
|---|---:|---|---|---|---|
| game_results_input (= GC animControls) | 1,732 | unit 157/433 (ops 6 in this tree) → **MATCH** (`score.py fn`) + unit EQUAL | EQUIVALENT (w28c) | none | `cloud/matches/game_results_input.c` |
| sound_loop_set (= GC _TitlePane) | 332 | unit EQUAL only with func_8010E8B4 as C (w28c) → **MATCH in group** physics_sym (supersede) + unit EQUAL without E8B4 | n/a | none | packet `w29b/groups/physics_sym/` |
| func_80106B3C (HUD title text) | 600 | unit 50/150 (ops 16) → **MATCH in group** `func_80106B3C` + unit EQUAL | EQUIVALENT (w28c) | none | packet `w29b/groups/func_80106B3C/` |
| func_8010BC84 (pause controller status) | 928 | unit 160/232 (ops 29) → **MATCH in group** `func_8010BC84` + unit EQUAL | EQUIVALENT (w28c) | none | packet `w29b/groups/func_8010BC84/` |
| func_8010B7FC (language list page) | 460 | unit 42/115 (w27d) → **MATCH in group** `func_8010B7FC` + unit EQUAL | EQUIVALENT (w27d) | none | packet `w29b/groups/func_8010B7FC/` |
| func_8010B9C8 (two-choice page) | 700 | unit 108/175 (w27d) → **MATCH in group** `func_8010B9C8` + unit EQUAL | EQUIVALENT (w27d) | none | packet `w29b/groups/func_8010B9C8/` |
| func_8010B5D0 (centred line page) | 556 | unit 20/139 (w27d) → 2/139 (one unused local) | EQUIVALENT (w27d) | as1/ugen order of font_set's `old` spill store | `w29b/func_8010B5D0/best.c` |
| func_8010DCFC (= GC _StartCone) | 660 | 145/165, rows 99, ops 11, frame 136/104 → 147/165, rows 88, ops 10, frame 136/136 | EQUIVALENT 4 calls (attract 1, race_track4 3) after fixing the list-head address | colour + one branch shape; frame closed by 3 hypothesised inline helpers | `w29b/func_8010DCFC/best.c` |
| func_80108AB0 (= GC animWrongWay) | 760 | 184/190 ops 46 (unchanged) | EQUIVALENT (w28c) | FP constants hoisted into $f20-$f26 (callee cost 9.5 = 38 blocks); 72 B frame gap | w28c drafts |
| func_8010AEAC/A8D0/A7A4 unit | 1,652+1,500+300 | AEAC 357/413 ops 30 → 290/413 ops 28 (frame 240/240); A8D0 325/375 ops 29 → 241/375 ops 26 (frame 160/152); A7A4 16/75 | EQUIVALENT (coordinator: dc_static.c, 1,649 calls, options_menus + pause_menus) | TB_CenterText applies; logic of A8D0 still to shape | `w29b/func_8010AEAC/ct_centertext.c` |
| func_800DA2C0+func_800DB1E0, func_8010EA08+func_8010F218 | 2,316+1,400, 2,056+2,444 | not attempted (time) | — | — | — |
| func_8010E8B4 (initScreen arm) | 340 | 21/85, frame 96/80 → 3/85, frame 96/96 (one unused local) | EQUIVALENT (w28c v2 body) | s16 narrowing through ring temps (family residual) | `w29b/func_8010E8B4/best.c` |

## game_results_input = animControls — MATCH (1,732 B)

`cloud/matches/game_results_input.c` (= `w29b/game_results_input/best.c`).
- Builder: `score.py fn cand/game_results_input.c game_results_input --flags "-g0 -O3 -mips2 -G 0 -non_shared"` →
  `MATCH` / `own .rodata verified at 0x80120234..0x80120240`.
- Unit: `blob_unit --tag w29b score game_results_input --with cloud/work/frontier/w29b/game_results_input/best.c --neighbours`
  → `EQUAL game_results_input: 433 words (kept, c_best.c)`, `locked bodies that differ in this unit: 0`, `1/1 equal`.
- Integration: single (splice_singles); no unit_overrides, no data-symbol changes. Called only via function pointer.
- The lever ("why is a3 unavailable to every web"): ctrace showed `first` (= D_8014A100[p], hoisted expression web
  w37, save 3.0) taking a3 in ours; in retail a3 is held by a web with NO code that interferes with every
  entry-block variable. The GC call site gives it: GC animControls calls `validOption(extsb p, gControlFirst[p] + j)`,
  i.e. validOption has a player parameter (GC DWARF lost it: "static int validOption(void)"). With
  `validOption(u8 player, s32 item)` the hoisted `(u8)p` is a loop-invariant web (`and $7,$12,255` in the ugen listing,
  deleted as dead by as1 — no code). `s8 player`/`s16`/`s32` do not work (205/205/157 words); u8 → 18 words.
  Its priority must beat first's (tie → lower web number, first wins): `if ((u8)p) {}` before `gTextPosScale = ...`
  and after the loop give it 13/5 against first's 12/5 → a3; then first t0, blt t1, id t2, action t3, p t4,
  conflict t5, uninited ra (as retail). Path: 157 → 18 (u8 player param + `if (player) {}`) → 15 (two outer reads)
  → EQUAL (field extraction order conflict, uninited, p, id, val: retail's ugen ring t6,t7,t8,t9,t6,t7).
- Shaping devices (disclosed in header): inlined validOption/Hidden statics; named `valid` (frame 120; 112 without);
  three compiled-out reads `if (player) {}` / `if ((u8)p) {}` ×2 (each is needed: dropping any one → 41 or 133 words;
  dropping both outer ones → 5 words).
- GC-derived names: game_results_input → animControls (controlscr.c), validOption(player, item) (controlscr.c call
  site in animControls), Hidden (hud.c); globals as in w28c (gControlFirst D_8014A100, gControlMenuSize D_8014A10A,
  gConflict D_80149DA8, controlScrActive D_8015418C, buttonsInited D_80154618, buttonNames D_80111998, gButtonScale
  D_80112A9C, buttonZ D_80112AB0, gNewControls D_8013C068).

## sound_loop_set packaging (owner-delegated) — group `w29b/groups/physics_sym/` (MATCH)

Delivered packaging = **physics_sym group superseded**: members func_800B55F4, func_800B5688, func_800B59E8, func_800B59F0,
physics_sym (all five old members, `group.c` byte-identical to src/blob/groups/physics_sym/group.c) + sound_loop_set
(`slp.c`); claims = sound_loop_set; context = slot_state_setup, object_create, func_800B669C from `ctx_gr3_b.c` (byte-identical
copy of credits_scroll_grp/gr3_b.c, the same context file frontier_billboard_render carries); keep = physics_sym.
- Builder: `score.py group cand/groups/physics_sym` → `func_800B55F4: MATCH`, `func_800B5688: MATCH`, `func_800B59E8: MATCH`,
  `func_800B59F0: NOT VERIFIED (12 section-relative relocations unverified … own .rodata: this function's references disagree
  on the section's image address (0x801226A8 for +0x0/+0x10, 0x80123D98 for +0x2c/+0x30/+0x34/+0x38) …)` (the locked group
  prints the same line — baseline run of src/blob/groups/physics_sym: `NOT VERIFIED … 0x80123DAC for +0x18/+0x1c/+0x20/+0x24`,
  exit 1 — the known two-region own-rodata case, installed through blob_group's independent-regions proof),
  `physics_sym: MATCH`, `sound_loop_set: MATCH` + `own .rodata verified at 0x80122614..0x80122624`; exit 1 (only that
  NOT VERIFIED). `score.py group --claims cand/groups/physics_sym` → `sound_loop_set: MATCH`, exit 0.
- Unit: `blob_unit --tag w29b score sound_loop_set physics_sym func_800B5688 func_800B59F0 --with
  cloud/work/frontier/w29b/groups/physics_sym/slp.c --internal sound_loop_set --block sound_loop_set --internal gfx_lock_slp
  --internal gfx_unlock_slp --internal font_select_slp --internal font_set_slp --neighbours` → `EQUAL sound_loop_set: 83 words
  (internal, c_slp.c)`, `EQUAL physics_sym: 313 words`, `EQUAL func_800B5688: 49 words`, `EQUAL func_800B59F0: 343 words`,
  `locked bodies that differ in this unit: 0`, `4/4 equal`.
- **unit_overrides needed**: `inline_blockers` += sound_loop_set (retail calls it by jal; with func_8010E8B4 still asm,
  physics_sym is its only C caller and umerge inlines it: the run without `--block` gives `umerge inlined into it:
  func_800B59E8, sound_loop_set`, physics_sym fails); `force_internal` += sound_loop_set (internal callee, unsaved s0/s1/s2/s4;
  as a kept name it compiles o32), gfx_lock_slp, gfx_unlock_slp, font_select_slp, font_set_slp (inlined helpers, non-static
  so their deleted stubs carry symbols). func_800B669C is already force_keep (w28a). No data-symbol changes.
- Attempts, in order (each scored with `score.py group` on the builder):
  1. physics_sym + sound_loop_set (w28c body, helpers renamed non-static) + context = copies of gr3.c and gr3_b.c with all
     context names on keep → sound_loop_set 8/83 (slot_state_setup kept = o32, wrong convention).
  2. same, keep as credits_scroll_grp (slot_state_setup, sound_update_channel, func_80096288 internal) → 4/83.
  3. credits_scroll_grp superseded (all 7 members) + physics_sym's 5 + sound_loop_set, credits flags → every old member MATCH,
     sound_loop_set 4/83 — so the context was not the cause.
  4. Unit: the 4 words reproduce with `--block sound_loop_set` and NO func_8010E8B4 in C: they are an as1 delay-slot tie
     (jal osSendMesg takes `move s0,v0` instead of `move a2,zero`). ugen listings differ only in `.loc` records: with ONE
     inlined call site umerge keeps font_set/gfx_lock/gfx_unlock's own line numbers; with E8B4 also C (two sites) the inlined
     code carries none. Writing the four helpers on one source line → unit EQUAL without E8B4.
  5. Group 1 with the one-line static helpers → sound_loop_set MATCH, func_800B5688 `MISMATCH (1 extra words …)` (static
     helpers' symbol-less stubs, w17j) → non-static `*_slp` → all MATCH.
  6. No context at all → sound_loop_set 5/83 (slot_state_setup o32: font in a0, &D_801461D0 in s1 not s4).
  7. Context = gr3_b.c only (slot_state_setup + object_create + func_800B669C) → MATCH: **smallest clean packaging, delivered**.
- Shaping disclosed in slp.c: non-static uniquely named helpers; the four helpers on one source line (as1 line tie).
- When func_8010E8B4 lands it joins this group (it is sound_loop_set's other caller) and the blocker can go.

## func_8010E8B4 (draft, 3/85)
`w29b/func_8010E8B4/best.c` (also defines sound_loop_set). `us.sh` (`--internal sound_loop_set`): `FAIL func_8010E8B4: 3 of 85
words differ | words 3 ops 0 norm 0 | frame 96/96`. From w28c 21/85 (frame 96/80): named `s8 c` for the controller byte
(retail keeps it in v0; 21 → 7), operand order `D_8011AD70[1] + D_8011AD70[0] + 8` (→ 4), `s32 h` for the unnarrowed line
height + one unused local `u0` (frame 96; 88 without u0) → 3. Residual: retail `sll t8,t0,16; sra t9,t8,16` and uses t9 (t0 keeps
the unnarrowed value); ours `sra t0,t8,16` (ugen `sra $25,$8,16; move $8,$25`, merged by as1). Tried: s32 y with `(s16)y` at the
uses, `y = (s16)(…)`, an inlined `optPane(s16 y[, …])` helper (frame stays 80), split assignments, `if (y) {}`: none. Same
narrowing-through-temps residual as the B5D0/B7FC/BC84 family. GC: initScreen@optionscr.c (locals: `y` s16 only), so the
unused local is a frame stand-in, not a GC-attested variable.

## func_80106B3C — MATCH in group (600 B)
Packet `w29b/groups/func_80106B3C/`: members/claims/keep = func_80106B3C (`f6b3c.c`), context slot_state_setup, object_create,
func_800B669C from `ctx_gr3_b.c` (byte-identical copy of credits_scroll_grp/gr3_b.c). Draft copy with static helpers:
`w29b/func_80106B3C/best.c`.
- Builder: `score.py group cand/groups/func_80106B3C` → `func_80106B3C: MATCH`, exit 0; `--claims` exit 0.
- Unit: `blob_unit --tag w29b score func_80106B3C --with cloud/work/frontier/w29b/groups/func_80106B3C/f6b3c.c --internal
  Hidden_6B3C --internal TB_SetJust_6B3C --internal getB6_6B3C --internal gfx_lock_6B3C --internal gfx_unlock_6B3C --internal
  font_set_6B3C --neighbours` → `EQUAL func_80106B3C: 150 words (kept, c_f6b3c.c)`, `locked bodies that differ in this unit: 0`.
  Same with best.c (static helpers, no --internal): EQUAL, 0 differ.
- Integration: install the new group (blob_group); unit_overrides `force_internal` += the six `*_6B3C` helpers. Standalone
  `score.py fn` is not possible (slot_state_setup takes its font in s2 only with its body in the program).
- Path: 50 → 43 words (`ctl` named local: loop invariants s5/s6/s7 as retail) → 8 (b6 into a named local; retail keeps
  ctl/b6/nw in v1/v0/a1) → 4 (gfx_lock/gfx_unlock/font_set defined AFTER the function: the TB_SetJust stores precede the
  osRecvMesg argument moves — as1 line-number tie) → EQUAL (b6 through an inlined getter `getB6(ctl)`: its value return
  takes v0; the named local got v1 because it interfered with the &D_80115FA8 row web).
- Devices: inlined helpers (Hidden, TB_SetJust = locked func_800B669C's body, getB6, gfx_lock, gfx_unlock, font_set), helper
  placement after the function. GC: no confident pair (w28c).

## func_8010BC84 — MATCH in group (928 B)
Packet `w29b/groups/func_8010BC84/` (members/claims/keep func_8010BC84 in `fbc84.c`; context slot_state_setup, object_create,
func_800B669C from `ctx_gr3_b.c`, byte-identical copy of credits_scroll_grp/gr3_b.c). Static-helper draft: `w29b/func_8010BC84/best.c`.
- Builder: `score.py group cand/groups/func_8010BC84` → `func_8010BC84: MATCH`, exit 0.
- Unit: `blob_unit --tag w29b score func_8010BC84 --with cloud/work/frontier/w29b/groups/func_8010BC84/fbc84.c --internal Hidden_BC84
  --internal getB6_BC84 --internal TB_CenterText_BC84 --internal gfx_lock_BC84 --internal gfx_unlock_BC84 --internal font_set_BC84
  --neighbours` → `EQUAL func_8010BC84: 232 words (kept, c_fbc84.c)`, `locked bodies that differ in this unit: 0`; best.c (static
  helpers): EQUAL, 0 differ.
- Integration: new group (blob_group); unit_overrides force_internal += the six `*_BC84` helpers.
- **The family lever (`x - (w >> 1)` narrowing through a temp)**: it is GC `s16 TB_CenterText(void *text, s16 centerX)`
  (TB_text.c: `return centerX - (HAL_GetTextWidth(text, -1) >> 1)`, extsh at the return) inlined; its s16 result is narrowed
  again for TB_DrawText's (= state_utility) s16 x, giving retail's `subu tN; sll a0,tN; sra tM,a0; move a0,tM`. 160 → 99 words.
  Then the 106B3C levers: `ctl` named local + b6 through an inlined getter in both loops → EQUAL.
- GC names: state_utility = TB_DrawText (w28c), object_manager_update = HAL_GetTextWidth, TB_CenterText (TB_text.c).

## func_8010B7FC, func_8010B9C8 — MATCH in group; func_8010B5D0 2 words
Packets `w29b/groups/func_8010B7FC/` (`fb7fc.c`) and `w29b/groups/func_8010B9C8/` (`fb9c8.c`), each members/claims/keep = the
function, context slot_state_setup/object_create/func_800B669C from `ctx_gr3_b.c` (byte-identical gr3_b.c copy).
- Builder: `score.py group cand/groups/func_8010B7FC` → `func_8010B7FC: MATCH`, exit 0; `… func_8010B9C8` → `func_8010B9C8: MATCH`, exit 0.
- Unit: `blob_unit --tag w29b score func_8010B7FC --with …/groups/func_8010B7FC/fb7fc.c --internal gfx_lock_B7FC --internal
  gfx_unlock_B7FC --internal font_set_B7FC --internal item_refresh_B7FC --internal TB_CenterText_B7FC --neighbours` → `EQUAL
  func_8010B7FC: 115 words`, 0 locked differ; same for B9C8 (`item_disable_B9C8`) → `EQUAL func_8010B9C8: 175 words`, 0 differ;
  best.c drafts (static helpers): EQUAL, 0 differ.
- Integration: two new groups; force_internal += `gfx_lock_X, gfx_unlock_X, font_set_X, item_refresh_B7FC / item_disable_B9C8,
  TB_CenterText_X` (X = B7FC, B9C8).
- Path: w27d drafts + TB_CenterText inlined: B7FC 42 → 2, B9C8 108 → 30 (frame 176: w27d's frame devices now overshoot),
  B5D0 20 → 18 (frame 160). Dropping w27d's nested font_select (and B9C8's unused `i`): B9C8 3, B5D0 2. `y = D_80117280[1] +
  object_bytes_sum_global() * 2` (retail evaluates the shift first: ring t6) → B7FC and B9C8 EQUAL.
- B5D0 residual (2 words, `sw v0,68(sp)` of font_set's old before vs after `addiu a0,…` for osSendMesg) and its unused `s32 i`
  are in the best.c header with what was tried. Next: find the source form where `old` is stored after gfx_unlock's arguments
  (ugen emits the spill store immediately after `jal slot_state_setup`).

## func_8010DCFC = GC _StartCone (draft)
`w29b/func_8010DCFC/best.c`. The correlation listed `_StartCone` [targets.c] as a weak 2nd candidate; the GC disassembly has the
same structure step by step (see header): N64 is `_StartCone` with target ids 236/339/237 (GC 0x226/0x28D/0x227). GC locals: v,
tmat (matrix at sp+8), vel, angV, car, tInfo. Retail stack: node home 132, tmat 84..131, spill temps 48 (tInfo), 60 (car),
64 (v), 68 (vel). Declaration order of the pointer locals has NO effect (24 permutations, identical output) — they are
copy-propagated. Frame 104 → 136 with three inlined helpers (AddToEnvList(e), GrabEnvEntry(), Flag1600(car); each +8 B), giving
retail's node/tmat homes; which helpers are real is a hypothesis. `us.sh`: `FAIL func_8010DCFC: 147 of 165 words differ | words 89
ops 10 norm 10 | frame 136/136`. Next: tInfo/car colouring at entry (retail t0/a3) and the second id-236 branch shape.

## func_80108AB0 = GC animWrongWay (no change)
ctrace on w28c's gc_named.c: the hoisted FP constants (±π/2, 2.0, 255.0) are phase-1 webs and the procedure's callee-saved cost is
9.5 (38 blocks); retail rematerialises them per iteration, so its callee cost must exceed their benefit (≈ 80+ blocks) — consistent
with retail's 72 B larger frame (more inlined helper sites). Loop as if/else or goto: no change (185/190). Next: find the inlined
helpers (frame and block count together), e.g. GC TB_SetTextColor/HAL_SetFont/TB_GetFlashTextCycle shapes.

## func_8010AEAC / func_8010A8D0 / func_8010A7A4 (unit draft)
TB_CenterText (inlined, 4 sites: A8D0's `x - w/2` and AEAC's two `160 - w/2` titles) → AEAC 357 → 290 words (ops 30 → 28, frame
240/240), A8D0 325 → 241 (ops 29 → 26, frame 152 → 160 over), A7A4 unchanged 16/75. `w29b/func_8010AEAC/ct_centertext.c`
(`blob_unit … --internal func_8010A7A4 --internal func_8010A8D0`). Coordinator note (wave 29): the options_menus host crash was a
host bug (static func_8010A8D0 → slot_state_setup bypassing the register shim), fixed by w29r; w28c's dc_static.c is EQUIVALENT
over 1,649 calls (options_menus 960, pause_menus 689). Next: the A8D0 frame (+8 now: drop one of w28c's frame devices) and the
flash-colour helper's param homes.

## draft_check (`python3 -m tools.recomp.draft_check --remote rocky --lane w29b`, after w29r's host fix)
```
func_80106B3C       EQUIVALENT over 25581 calls / 6 scripts (stunt_end 9336, pause_menus 3736, two_player_finish 3390, ghost_save 3268, race_finish 3218, player_finish 2633)
func_8010B5D0       EQUIVALENT over 1890 calls / 6 scripts (pause_menus 810, saves 345, player_reload 300, audio_options 195, stunt_end 150, race 90)
func_8010B7FC       EQUIVALENT over 1695 calls / 6 scripts (pause_menus 720, player_reload 300, saves 240, audio_options 195, stunt_end 150, race 90)
func_8010B9C8       EQUIVALENT over 105 calls / 1 scripts (saves 105)
func_8010BC84       EQUIVALENT over 90 calls / 1 scripts (pause_menus 90)
func_8010DCFC       DIVERGES script race_track4, frame 1376, func_8010DCFC (draft) call #1 (global seq 1): memory written differs
  -> word 0x80138FF0 written by the draft, 0x801391F0 by retail: the env-list head is D_801391F0 (retail lui 0x8014 / addiu -28176);
     w28c's draft had D_80138FF0 (same divergence on w28c best.c). Fixed: `draft_check … func_8010DCFC w29b/func_8010DCFC/best.c
     --scripts attract,race_track4` -> VERDICT: EQUIVALENT over 4 calls / 2 scripts (attract 1, race_track4 3) (all other scripts NEVER CALLED).
func_8010E8B4       EQUIVALENT over 571 calls / 2 scripts (options_menus 331, pause_menus 240)
game_results_input  EQUIVALENT over 105680 calls / 2 scripts (controls_frontend 88880, pause_menus 16800)
LANE w29b: 1 DIVERGES, 7 EQUIVALENT (EQUIVALENT is behaviour only, never a match)
```
(The matched functions are listed for completeness; their evidence is score.py/blob_unit above.)

## Permission denials
None so far.
