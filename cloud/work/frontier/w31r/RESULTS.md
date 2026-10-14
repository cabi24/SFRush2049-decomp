STATUS (2026-10-14): DONE. Recomp Phase 11 (decomp feedback tooling), branch w31r (worktree, from wave31 880e729e,
wave31 882c9f77 merged). Not a matching lane: no src/, asm/, lock or coverage change. Docs: tools/recomp/README.md
"Phase 11"; plan §20. Test: tests/cloud/test_recomp_phase11.py.

# 1. Battle mine pickup: DONE (func_8010C974 reached)

func_8010C974 = GC _AnimateMine (targets.c): the animate callback of target type WPR_MINE (targetInfo entry
0x80118BB0: names WPR_MINE / WPR_MINEBASE, start func_8010C7F4 = _StartMine, animate func_8010C974). A mine is a
shot that comes to rest (GC UpdateShots: shot type 3 -> SetupTarget), so the player must FIRE a mine, not pick one up.
Weapon slot from the N64 pick-up code func_8010D3C0 (= GC _StartWeaponIcon): gGameCar 0x80152818, stride 0x3B8 (the
`((i*16-i)*8-i)*8` sequence), type byte car+0x384, ammo car+0x385 (ammo table 0x80121D60, 8 words) = GC
tCarData.battle.weapon {type, ammoCnt} at 0x39C. Lever: `poke 26 80152B9C 0309 2` (type 3, 9 rounds) + Z (B/L/R do
not fire). Sweep over types 0-7 (same presses): only type 3 reaches func_8010C974. Script `battle_mine.press`
[1500]: first call frame 839, 573 calls, _StartMine 2 calls (a car hits a mine).
Side result for Astra (image B), script `battle_weapons.press` [1800]: type 0 + Z -> ovl_b func_8038A95C (GC
_RemoveCannMFlash) frame 1080; type 1 -> func_8038AA14 (_RemoveGattMFlash) 1380; type 7 -> func_8038D498 (84 calls).
Types 2/4/5/6 reach none of image B's then-unreached functions.

# 2. Overlay-A reach: YES, the host runs overlay code; the three "image-A only" functions were not image-A only

- The game loads images A and B through inflate_entry; the host's hook load_overlays-es the recompiled section and
  calls into 0x8038A400.. dispatch by load state, so overlay code runs whenever the game has it loaded. New:
  `script_probe --functions ovl_a | ovl_b | ovl_a:NAME` and `census --overlays` (probes on all 242 overlay functions).
  Image A = front end (and stays loaded during a normal race: 69 image-A functions in `race`); image B = race-time
  image (1-2 functions in a normal race, ~25 in battle: weapons).
- control_cell_text / control_cell_text3 / func_800DD0C0 are cell kinds of control_settings = the modal controller
  pak dialog. Callers of control_settings: save_settings 0x800DE398 (game image; runs it for every pak error) and image
  A's func_803990D0 at 0x803991B4 (when its flag 0x803B46B4 is set; no script reaches func_803990D0, a poke of the flag
  on the PLAYER screen did nothing: it is not the PLAYER screen, which is func_8039842C). The image-A label came from
  the ovl_a call site; the cells are reached through the game-image caller by a pak fault during CREATE PLAYER:
  sweep `pak fail OP 1 ERR` (OP delete/read/write/alloc/state, ERR 1-12) on saves.press: write and alloc failures
  open the dialog at frame 721, read/delete/state none. ERR -> dialog -> cell table: README "Phase 11". New scripts:
  `pak_nopak` (ERR 1, func_800DD0C0), `pak_repair` (ERR 10, control_cell_text3), `pak_corrupt` (ERR 3,
  control_cell_text).
- Overlay census (side result for Astra; `tools/recomp/census/OVERLAYS.md`, `overlays.json`, tree c49bc16b, 39
  scripts): image A 99 of 193 reached (unmatched 45 of 112, 92,504 of 101,892 B); image B 35 of 49 (unmatched 19 of
  28, 28,696 of 29,688 B). Never reached, unmatched, larger than a stub: image A func_803A9088 (3,668), func_80395E8C
  (2,604), func_8039C910 (1,844), func_803A5358 (768) + 63 eight-byte stubs; image B func_8038F938 (928) + 8 stubs.
- Observed, not pursued: `pak fail read 1 3` on player_reload (boot read of player AAAA fails) makes the STOCK host
  fault at frame 120 (N64 read of 0x00000000, thread game7, no replaced call open).

# 3. dc.args for the --lane ERROR drafts: DONE (all three cells EQUIVALENT)

Two harness bugs had to be fixed first (both general):
- ipa_detect: a callee-saved register stored to the stack counted as a prologue save. Retail control_cell_text passes
  its s7 parameter as a stack argument (`jal camera_auto_follow; sw s7,24(sp)`); s7 was missing from the live-in
  set (regmap_verify REFUTED `s7=text`, --derive printed s5,s6). Rule now: a save only if the body reloads that slot,
  writes that register, or makes no call. Over all 1,216 game functions this changes control_cell_text (+s7) and
  control_cell_text2 (+s8) only. build/frontier.json (tools/conveyor's scanner, not touched here) has the old sets.
- replace.ipa_shim (inbound): a retail stack parameter at sp+N whose o32 slot is also sp+N was skipped as a no-op
  (`sp+16 -> sp+16`; source = caller frame, destination = the shim's outgoing area). control_cell_text3's s2/s3
  arrived as garbage: draft FAULTED in state_utility (read 0x3). Fixed (caller-stack sources named csp+N).

dc.args written (w28d; the directories keep the old attract_* names, mapped by the Phase 10 rename table):
| draft | dc.args | verdict |
|---|---|---|
| attract_mode_handler (control_cell_text) | `--params row=s5,col=s6,text=s7 --scripts pak_corrupt,pak_repair,pak_nopak` | EQUIVALENT 90 calls (pak_corrupt) |
| attract_video_handler (control_cell_text2) | `--params sel=s5,row=s6,col=s7,text=s8 --scripts ghost_delete_fail,pak_nopak,pak_corrupt` | EQUIVALENT 90 calls (ghost_delete_fail) |
| attract_demo_handler (control_cell_text3) | `--params sel1=s5,row=s6,text=sp+8,s1=sp+12,s2=sp+16,s3=sp+20,col=s7,sel2=s8 --scripts pak_repair` | EQUIVALENT 90 calls (pak_repair) |
| func_800E56F8 | `--params ipa_s4=s4` | matched since (lock): not run |
Evidence: call sites in control_settings (`lb s5,1(s1)` sel1, `lb s8,2(s1)` sel2, `move s6,s3; move s7,s4` row/col,
`sw t8,8(sp)` .. `sw t8,20(sp)` the four string arguments of text3; text3's 56-byte frame reads 64/68/72/76(sp));
regmap_verify --derive after the fix. FindGhosts (s8) and SetHighScoreState (s2) are matched: no draft check needed;
--all-lanes lists matched functions as MATCHED without running them. w30a's unit directories got dc.args with the
new `--name` (audio_options -> game_results_render, controls -> func_80100564; scripts as w30a's RESULTS ran them).

# 4. draft_check --all-lanes: 76 drafts: 33 EQUIVALENT, 3 DIVERGES, 40 MATCHED (not run)

`python3 -m tools.recomp.draft_check --all-lanes` (default globs `w2[8-9]*,w3*`: 29 lane directories incl. w31b and
the wave-3 w3a-d; rocky service tree = this branch + wave31 882c9f77), final aggregate all from the cache
(`~/rush2049/recomp/dc/lanes/all-final.{log,json}`; the rows were computed by four concurrent `--all-lanes` workers over
disjoint globs, --jobs 2 each). Four HARNESS gaps were found on the way and fixed, after which every affected row was
rerun (rows whose drafts name a changed callee were dropped from the cache, not re-keyed):
- audio_reverb_update takes (address, tag) in a1/a2 (IPA; regmap_verify CONSISTENT). It was the cause of the w28b
  pak_delete_file_retry FAULT that wave 30 called a draft bug, and of the w3b sound_bank_unload / w3c display_enable
  FAULTs: all three EQUIVALENT with the ipa_callees.json entry. **w31c: the draft's `audio_reverb_update(address, 0)`
  is right; do not "fix" it.**
- func_80104704 takes p in s5: w30a func_80105480 FAULTED (o32 call) -> EQUIVALENT 4,921 calls / 6 scripts.
- matched IPA callees with unsaved callee-saved writes had no clobber shim (frontier facts cover only scanned
  functions): func_800DCD58 writes s0,s1 -> the control_settings `lb a0,1(s0)`, s0 = 0 fault (wave 30 "draft bug").
  draft_check now derives them from the retail bodies.
- func_800DD0C0 takes (row, col) at the caller's sp+0/sp+4 (entry added; the pak_nopak fault below remains).
Stale args fixed: SaveServer drafts (w29d func_800C813C, w30c SaveServer) had no `--statics` -> dc.args -> EQUIVALENT
19,422 calls / 6 scripts each.

DIVERGES / FAULT with cause:
| draft | first divergence | cause |
|---|---|---|
| w28b SaveServer (func_800C813C) | attract_pak frame 4, FAULT, N64 read of 0x01010000 | DRAFT bug (known since w29r): reads the per-pak file list head at +8 of D_80144D68's 16-byte entries; retail +0 (w29d's later draft is EQUIVALENT) |
| w29c control_settings | pak_nopak frame 721, FAULT, N64 read of 0x00000008 | OPEN (cell type 2 = func_800DD0C0 path only; ghost_delete_fail/pak_corrupt/pak_repair EQUIVALENT after the clobber fix). Suspected harness (func_800DD0C0's stack-parameter convention, entry sp+0/sp+4 added, still faults; the report has no N64 PC) or the draft's type-2 branch; next: `--keep` and read the faulting host pc in RecompiledFuncs |
| w30d control_settings | same | same |

| lane | function | verdict | detail |
|---|---|---|---|
| w28a | PrintScores | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28a | FindGhosts | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28a | SetHighScoreState | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28a | drawMain | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28a | func_80102F30 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28b | pak_delete_file_retry | EQUIVALENT | over 2 calls / 2 scripts (ghost_delete_fail 1, ghost_resave 1) |
| w28b | SaveServer | DIVERGES | script attract_pak, frame 4, SaveServer (draft) call #3 (global seq 3): the draft FAULTED (host signal; retail did not) / SIGSEGV, N64 read of 0x01010000 at frame 4 (thread game7) |
| w28b | CreatePlayer | EQUIVALENT | over 5 calls / 5 scripts (pak_corrupt 1, pak_nopak 1, pak_repair 1, player_finish 1, saves 1) |
| w28c | ShowCountdown | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_80106B3C | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_80107EDC | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | animLapTime | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_80108AB0 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_8010A53C | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_8010AEAC | EQUIVALENT | over 1649 calls / 2 scripts (options_menus 960, pause_menus 689) |
| w28c | func_8010BC84 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_8010DCFC | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | func_8010E8B4 | EQUIVALENT | over 571 calls / 2 scripts (options_menus 331, pause_menus 240) |
| w28c | animControls | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28c | TitlePane | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28d | control_cell_text3 | EQUIVALENT | over 90 calls / 1 scripts (pak_repair 90) |
| w28d | control_cell_text | EQUIVALENT | over 90 calls / 1 scripts (pak_corrupt 90); never called in: pak_repair, pak_nopak |
| w28d | control_cell_text2 | EQUIVALENT | over 90 calls / 1 scripts (ghost_delete_fail 90); never called in: pak_nopak, pak_corrupt |
| w28d | SetupWaveModel | EQUIVALENT | over 637 calls / 6 scripts (attract 203, attract_pak 75, ghost_save 116, two_player 83, two_player_finish 81, audio_options 79) |
| w28d | func_800E543C | EQUIVALENT | over 126 calls / 6 scripts (attract 42, attract_pak 25, two_player_finish 17, race_finish 15, unlock 15, pause_menus 12) |
| w28d | func_800E56F8 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w28d | animPlace | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29a | FindGhosts | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29a | func_80100E58 | EQUIVALENT | over 3405 calls / 6 scripts (saves 1395, race_finish 480, ghost_pak 420, ghost_resave 420, unlock 420, player_finish 270) |
| w29a | func_80102448 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29a | func_80102980 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29a | func_80103D28 | EQUIVALENT | over 98 calls / 1 scripts (unlock 98) |
| w29b | func_80106B3C | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29b | func_8010B5D0 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29b | func_8010B7FC | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29b | func_8010B9C8 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29b | func_8010BC84 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29b | func_8010DCFC | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29b | func_8010E8B4 | EQUIVALENT | over 571 calls / 2 scripts (options_menus 331, pause_menus 240) |
| w29b | animControls | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29c | control_settings | DIVERGES | script pak_nopak, frame 721, control_settings (draft) call #1 (global seq 1): the draft FAULTED (host signal; retail did not) / SIGSEGV, N64 read of 0x00000008 at frame 721 (thread game7) |
| w29c | animPlace | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29d | draw_ui_element | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29d | SetupWaveModel | EQUIVALENT | over 282 calls / 2 scripts (race 79, attract 203) |
| w29d | LoadTrack | EQUIVALENT | over 41 calls / 2 scripts (race 6, attract 35) |
| w29d | func_800C7EC8 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w29d | SaveServer | EQUIVALENT | over 19422 calls / 6 scripts (attract 9014, attract_pak 908, pause_menus 3691, options_menus 2498, controls_frontend 1743, saves 1568) |
| w29d | CreatePlayer | EQUIVALENT | over 2 calls / 2 scripts (saves 1, player_finish 1) |
| w30a | game_results_render | EQUIVALENT | over 1273 calls / 3 scripts (audio_options 434, options_menus 540, pause_menus 299) |
| w30a | func_80100564 | EQUIVALENT | over 1949 calls / 2 scripts (controls_frontend 1110, pause_menus 839) |
| w30a | func_80100E58 | EQUIVALENT | over 3405 calls / 6 scripts (saves 1395, race_finish 480, ghost_pak 420, ghost_resave 420, unlock 420, player_finish 270) |
| w30a | func_80105480 | EQUIVALENT | over 4921 calls / 6 scripts (ghost_pak 1144, ghost_resave 1144, ghost_save 900, player_finish 863, two_player_finish 496, ghost_delete_fail 374) |
| w30b | func_800DB1E0,func_800DA2C0 | EQUIVALENT | over 961 calls / 1 scripts (options_menus 961) |
| w30b | func_80108AB0 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w30b | func_8010AEAC,func_8010A8D0,func_8010A7A4 | EQUIVALENT | over 1649 calls / 2 scripts (pause_menus 689, options_menus 960) |
| w30b | func_8010B5D0 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w30b | func_8010DCFC | MATCHED | in blob_matched.lock.json: no draft check needed |
| w30b | func_8010E8B4 | EQUIVALENT | over 571 calls / 2 scripts (options_menus 331, pause_menus 240) |
| w30c | CreatePlayer | EQUIVALENT | over 5 calls / 5 scripts (pak_corrupt 1, pak_nopak 1, pak_repair 1, player_finish 1, saves 1) |
| w30c | SaveServer | EQUIVALENT | over 19422 calls / 6 scripts (attract 9014, attract_pak 908, pause_menus 3691, options_menus 2498, controls_frontend 1743, saves 1568) |
| w30d | LoadTrack | EQUIVALENT | over 41 calls / 2 scripts (race 6, attract 35) |
| w30d | animPlace | MATCHED | in blob_matched.lock.json: no draft check needed |
| w30d | control_settings | DIVERGES | script pak_nopak, frame 721, control_settings (draft) call #1 (global seq 1): the draft FAULTED (host signal; retail did not) / SIGSEGV, N64 read of 0x00000008 at frame 721 (thread game7) |
| w31b | func_800DB1E0,func_800DA2C0 | EQUIVALENT | over 961 calls / 1 scripts (options_menus 961) |
| w31b | func_8010E8B4 | EQUIVALENT | over 571 calls / 2 scripts (options_menus 331, pause_menus 240) |
| w3b | audio_mixer_main | EQUIVALENT | over 36 calls / 6 scripts (attract 7, attract_pak 5, ghost_save 6, pause_menus 6, race_finish 6, two_player_finish 6) |
| w3b | sound_bank_unload | EQUIVALENT | over 108 calls / 6 scripts (attract 42, attract_pak 8, two_player_finish 16, pause_menus 14, race_finish 14, unlock 14) |
| w3c | display_enable | EQUIVALENT | over 42 calls / 6 scripts (attract 17, attract_pak 4, ghost_save 6, pause_menus 5, race_finish 5, stunt_short 5) |
| w3c | func_800B9740 | EQUIVALENT | over 36 calls / 6 scripts (attract 7, attract_pak 5, ghost_save 6, pause_menus 6, race_finish 6, two_player_finish 6) |
| w3d | arb_rate_set | EQUIVALENT | over 232 calls / 6 scripts (ghost_save 48, two_player_finish 40, battle 36, battle_end 36, battle_mine 36, battle_weapons 36) |
| w3d | camera_shake_start | MATCHED | in blob_matched.lock.json: no draft check needed |
| w3d | func_800A4CB8 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w3d | func_800E8D50 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w3d | func_800F6928 | MATCHED | in blob_matched.lock.json: no draft check needed |
| w3d | model_transform_setup | MATCHED | in blob_matched.lock.json: no draft check needed |
| w3d | steering_sensitivity | MATCHED | in blob_matched.lock.json: no draft check needed |

ALL LANES w2[8-9]*,w3*: 76 drafts: 3 DIVERGES, 33 EQUIVALENT, 40 MATCHED (EQUIVALENT is behaviour only, never a match)

(The 40 MATCHED rows are drafts of functions in blob_matched.lock.json at wave31: listed, not run.)

# 5. Census refresh: DONE

Tree c49bc16b (= wave31 880e729e lock, 166 unmatched), 39 scripts: **163 reached (182,996 B), 3 never (632 B)**:
world_effect_update (456), fp_call_wrapper (104), object_bytes23_sum (72) — no jal, pointer or lui/addiu reference
in any image (dead or computed-jalr only; w28r). Newly reached: func_8010C974 (battle_mine 573 calls),
control_cell_text (pak_corrupt 90), control_cell_text3 (pak_repair 90), func_800DD0C0 (pak_nopak 90); control_settings
now has 4 scripts. Was 159 of 166 (w30r).

# Checks

- Branch w31r = wave31 b90ca2c8 merged (92337388; no conflicts), head 4da575fe.
- Pi worktree `make test-fast JOBS=4` (92337388): tests/cloud `841 passed, 205 skipped`; tests/conveyor in the worktree
  `40 failed, 1511 passed, 814 skipped, 2 errors` = the known fresh-worktree environment (no tools/mips_to_c
  submodule); tests/conveyor in the MAIN checkout (wave31 b90ca2c8, with the submodule; this lane touches no conveyor
  code): `1621 passed, 771 skipped`. 4da575fe changes only the rocky-only e2e test (Pi: phase 11 file 22 passed, 1 skipped).
- rocky clone `~/rush2049/scratch/w31r` (w31r 4da575fe, origin/master = 880e729e, submodules populated, IDO toolchains
  copied from the w30r clone, CI env `../w28s.env`): `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: tests/conveyor
  `2340 passed, 52 skipped`; tests/cloud `1041 passed, 5 skipped, 8 warnings, 27 subtests passed` (the Phase 11 e2e
  test runs there).
- `python3 -m pytest tests/recomp -q`: all pass (Pi).

## Permission denials
None.
