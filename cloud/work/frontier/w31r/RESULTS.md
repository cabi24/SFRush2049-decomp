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

# 4. draft_check --all-lanes: see "All-lanes table" below

# 5. Census refresh: DONE

Tree c49bc16b (= wave31 880e729e lock, 166 unmatched), 39 scripts: **163 reached (182,996 B), 3 never (632 B)**:
world_effect_update (456), fp_call_wrapper (104), object_bytes23_sum (72) — no jal, pointer or lui/addiu reference
in any image (dead or computed-jalr only; w28r). Newly reached: func_8010C974 (battle_mine 573 calls),
control_cell_text (pak_corrupt 90), control_cell_text3 (pak_repair 90), func_800DD0C0 (pak_nopak 90); control_settings
now has 4 scripts. Was 159 of 166 (w30r).
