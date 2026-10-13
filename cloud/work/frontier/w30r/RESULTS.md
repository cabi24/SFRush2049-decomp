STATUS (2026-10-13): DONE. Recomp Phase 10 (decomp feedback tooling), branch w30r (worktree, from wave30 1b34f797).
Not a matching lane: no src/, asm/, lock or coverage change. Docs: tools/recomp/README.md "Phase 10"; plan §19.

# 1. A ghost file on the controller pak: DONE (it was gated by a developer id)

N64 high-score state machine = `finish_state_normal` 0x800F64D4 (GC highscorescr.c updateHighScoreScreen; state
word gHighScoreState 0x80140B20; SetHighScoreState 0x800F1210 takes its state in s2). State 3 (after the name
entry), gRace 0x8014A110 == 2 (ghost race) and the N64 gGhostToSave 0x80140800 != 0: UnloadWaveAll
(wheel_setup_initial), gSavedGhost 0x801461A8 = CompressGhost (func_800CC50C), gGhostToSave = 0, then
- byte **0x801148E4** (N64 gSaveGhostFiles) set: SetHighScoreState(4) = SAVE GHOST (FindGhosts, func_80104B14,
  func_800F1930 = ControlGhost);
- else SaveGhost (func_800CC040)(gGhostPak 0x801460C0, gGhostPlayer 0x8014A160, gSavedGhost, limbo 1): the ghost
  stays in memory (GC SaveGhost(..., limbo) -> SaveGhostInLimbo). That is what w29r's ghost_save saw.
Only func_800CCEFC writes 0x801148E4: when strcmp(player id, "000F1CC5 EJK" at 0x801211CC) == 0 it sets 0x801164A1,
the halfwords 0x801164C0/C2/C4 (unlock everything) and 0x801148E4. GC is identical (_ReadPlayer sets
gSaveGhostFiles for that id; updateHighScoreScreen saves to the card only `if (gSaveGhostFile && gSaveGhostFiles)`).
Retail players never get ghost files; the scripts poke the flag (both sides of a comparison alike).

Scripts (README "Phase 10"): `ghost_pak` (ghost_save + `poke 28 801148E4 1 1`, gas released at 127 s; SAVE GHOST
3836, DOWN + A = SAVE, GHOST SAVED ON CONTROLLER PAK 1 at 3990, note RUSH2049AAAA1F 256 B), `ghost_reload` (`pak from
ghost_pak`: boot ghost read = draw_ui_element; GHOST 1 TIME 1:38.858 loaded), `ghost_resave` (save again: the old
file is deleted first = pak_delete_file_retry). The pak is cached by the harness per script chain (`pak from`).

# 2. Unlock state: DONE

`_InitScreen` = track_texture_load 0x800F5F90: memcpy of the unlock tables 0x80150DD8.. -> 0x801426A0.., then
net_state_validate 0x800F2A28 (= GC CalcUnlockedStuff) recomputes them, func_800A1910 (memcmp) per player and table;
a difference -> SetHighScoreState(1) -> ControlUnlocked (func_800F2718) -> DrawUnlocked (func_80103D28). With the
developer halfword 0x801164C0 = 1 CalcUnlockedStuff unlocks all 13 cars. Script `unlock` (race_finish + `poke 30
801164C0 1 2`): CONGRATULATIONS! YOU JUST UNLOCKED: LOCUST LX | GX-2 | MINI XS | VENOM | CRUSHER | EURO LX |
PANTHER at frame 3742; func_80103D28 98 calls. (Poking a byte of the source table, 0x80150E3C, also triggers the
screen, with an empty list.)

# 3. A failing pak delete: DONE (host fault lever)

pak_delete_file_retry (0x800A2680) = PS_DeleteFile: osPfsDeleteFile; on an error it calls the pak error callback
(D_80144008) and loops while the user picks RETRY. Reached at all by ghost_resave (successful delete). The error
path needs a failing delete: new host option `--pak-fail OP:N[:ERR]` (pak.cpp; OP delete/read/write/alloc/state,
the Nth call returns ERR, default PFS_ERR_CONTRFAIL, image unchanged), script line `pak fail OP N [ERR]`, passed by
every harness (`oracle.lever_args`). Script `ghost_delete_fail` (ghost_resave + `pak fail delete 1`): CONTROLLER 1 |
UNKNOWN ERROR. MAKE SURE THE CONTROLLER AND THE CONTROLLER PAK ARE CONNECTED PROPERLY. | CONTINUE WITHOUT SAVING |
RETRY (3991), A = RETRY, second delete succeeds, GHOST SAVED (4080). Bonus: that dialog is drawn by
**control_settings + control_cell_text2** (90 calls each): the first script reaching them (w30d's target).

# 4. `--statics` binds initialised function-local statics: DONE

Cause: IDO 5.3 writes NO symbol for any function-local static (initialised or not) nor for a file `static`
(rocky probe objects: only the .data/.bss section symbols + offsets), so replace.static_bindings had no name to
match unless the code matched retail. New `tools/recomp/static_layout.py`: layout from the draft preprocessed by
IDO's own `cc -E` (source order, file-scope and function-local definitions interleaved; initialised -> .data,
uninitialised static -> .bss; each aligned to max(4, natural); an unreferenced static is dropped with no gap; all
checked against IDO objects on rocky), verified against the object (symbols' offsets, section size, integer
initialisers); refused layouts leave the statics private with a `NOTE static layout:` line. `--statics NAME=ADDR`
or `FUNCTION:NAME=ADDR`; all unit members. Result: w29d `func_800C813C/best.c` (SaveServer, no dc.c):
`VERDICT: EQUIVALENT over 3490 calls / 3 scripts (saves 1568, ghost_reload 825, ghost_resave 1097)` with
`--statics sInitCountdown=0x8011742C,sLastPak=0x80117430,sReadingPak=0x80117434` (the ghost branch now included);
without: `DIVERGES ... word 0x8011742C: retail 03000000->02000000 | draft unchanged` and the NOTE names
`.data+0x0 sInitCountdown, .data+0x4 sLastPak, .data+0x8 sReadingPak`.

# 5. Census and the rename check

Census (rocky, 34 scripts, 175 unmatched at wave30): **168 reached (184,796 B), 7 never (5,260 B)** (w29r: 178/192,
14 never). Newly reached: func_80104B14, draw_ui_element, func_80103D28, pak_delete_file_retry, control_settings,
control_cell_text2 (FindGhosts was matched in wave 29). Never: func_8010C974 (mine pickup in battle),
func_800DD0C0, control_cell_text3, control_cell_text (other control_settings cell kinds; probably image A's
controller-settings screen, unverified), world_effect_update, fp_call_wrapper, object_bytes23_sum (no static ref).

Rename findings (harness, fixed): (a) build/frontier.json facts were looked up by name: 22 renamed functions had
lost register_params/unsaved facts (no refusal for FindGhosts s8 / SetupWaveModel s8,t0 drafts; no clobber shim
around drawMain, PrintScores, TitlePane, control_cell_text...): now keyed through each fact's address
(`replace.frontier_facts`). (b) `--lane` used the directory name (old names: w29d/func_800C813C holds SaveServer):
now mapped (`replace.stale_function_names`, NOTE line). (c) w29n's text replace had renamed the SCRIPT race_finish
to `animPlace` in tools/recomp/census/*.json (draft_check default scripts pointed at a missing script): census
regenerated.

## draft_check --lane after the rename (rocky, service tree = this branch)

Run: `--lane w29d`, `--lane w29a`, w29b (5 of 8 drafts, then stopped for time), and the 21 remaining RENAMED
drafts of w28a/b/c/d, w29b, w29c (the 31 best.c files w29n's replay touched; 8 non-renamed w28c drafts, w28d
func_800E543C, w29b func_8010DCFC/game_results_input not run: time). Each renamed directory printed `NOTE ...
renamed since the draft was saved: OLD -> NEW`.

**No rename-caused divergence.** Every renamed draft that ran is EQUIVALENT, or diverges/errs for a reason that
is not the rename:

| lane/dir (now) | verdict | why |
|---|---|---|
| w29d func_80096CA8 (SetupWaveModel), w28d func_80096CA8 | EQUIVALENT 282 / 637 calls | |
| w29d func_800CD104 (CreatePlayer), w28b func_800CD104 | EQUIVALENT 2 calls | |
| w29d func_800C7EC8, func_800BB9B0 (not renamed) | EQUIVALENT 1 / 41 calls | |
| w29d func_800C813C (SaveServer) | DIVERGES attract frame 2, word 0x8011742C | function-local statics private (no dc.args): with `--statics` EQUIVALENT 3490 calls (job 4) |
| w29d draw_ui_element | ERROR: register params | first check ever: `--params ghosth=t0,data=t2,repair=t1` -> **EQUIVALENT over 11 calls / 3 scripts (ghost_delete_fail 4, ghost_resave 4, ghost_reload 3)** |
| w29a func_80103D28 | **EQUIVALENT over 98 calls / 1 scripts (unlock 98)** | first check ever (unlock.press) |
| w29a func_80100E58 / func_80102448 / func_80102980 | EQUIVALENT 3405 / 390 / 945 calls | |
| w29a, w28a func_800F0F44 (FindGhosts, matched) | ERROR: register params s8 | correct refusal; after the rename (before this lane's fix) the stale facts lookup would have built it o32 silently |
| w28a func_800F1210 (SetHighScoreState, matched) | ERROR: register params s2 | same |
| w28a func_800F0914 (PrintScores) / func_8010221C (drawMain) / func_80102F30 | EQUIVALENT 9 / 4709 / 6674 calls | |
| w29b func_80106B3C, func_8010B5D0, func_8010B7FC, func_8010B9C8, func_8010BC84, func_8010E8B4 | EQUIVALENT 73380 / 1890 / 2235 / 105 / 90 / 571 | |
| w28c audio_doppler_full (ShowCountdown) / func_80108154 (animLapTime) / func_8010E8B4 / sound_loop_set (TitlePane) | EQUIVALENT 126253 / 62941 / 571 / 1 | |
| w28d, w29c race_finish (animPlace) | EQUIVALENT 19962 calls / 6 scripts | |
| w28d attract_*_handler (control_cell_text, _text2, _text3), func_800E56F8 | ERROR: register params (s5,s6[,s7,s8]; s4) | the drafts have no dc.args (never checkable before; same refusal under the old names) |
| w28b AdjustSpeed (pak_delete_file_retry) | DIVERGES: draft FAULTED, ghost_delete_fail/ghost_resave frame 3990 | first check ever: the draft calls `audio_reverb_update(address, 0)`; retail passes the handle in **a1** (`move a1,s0; jal audio_reverb_update; move a2,zero`; the callee starts `move a0,a1`): a 3-parameter callee, first unused. Draft bug (w30c) |
| w28b func_800C813C (SaveServer) | DIVERGES: FAULTED attract_pak frame 4 | the known w28b +8 field bug (w29r) |
| w29c control_settings | ERROR `best.c has no definition` -> fixed (function-pointer parameter `s32 (*poll)(void)` in find_prototype/parse_prototype); now **DIVERGES: FAULTED** ghost_delete_fail frame 3991, `lb $a0, 0x1($s0)` s0 = 0 at control_settings+0x4F0, called by save_settings 0x800DE39C (the pak error dialog). w30d's newer draft: same fault at +0x4F4. First check ever (w30d) |


# Checks

- Branch w30r = wave30 e4e580f3 merged (wave30 moved by three lane-results commits, no tool/src changes).
- Pi worktree `make test-fast JOBS=4`: tests/cloud `820 passed, 204 skipped`; tests/conveyor `40 failed, 1511
  passed, 819 skipped, 2 errors` = the known fresh-worktree environment (no tools/mips_to_c submodule; failures in
  test_group_search.py / unit/test_reloc_blind.py). tests/conveyor in the MAIN checkout (wave30, with the submodule;
  this lane touches no conveyor code): `1623 passed, 780 skipped`.
- Pi `python3 -m pytest tests/recomp tests/cloud/test_recomp_phase8.py tests/cloud/test_recomp_phase9.py
  tests/cloud/test_recomp_phase10.py`: all pass (e2e skipped off rocky).
- rocky clone `~/rush2049/scratch/w30r` (w30r f1b3d2f7, origin/master = 1b34f797, submodules populated, CI env
  `../w28s.env`): `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: tests/conveyor `2342 passed, 52 skipped`;
  tests/cloud `1019 passed, 5 skipped, 8 warnings, 27 subtests passed` (the two Phase 10 e2e tests run there).

## Permission denials
None.
