STATUS (2026-10-14): DONE (jobs 1-5). wave32 13219eb6 (w32i installs + w32n renames) merged. Recomp Phase 12 (decomp feedback tooling), branch w32r (worktree, from wave32 d1c53ab2). Not a matching
lane: no src/, asm/, lock or coverage change. Docs: tools/recomp/README.md "Phase 12"; plan §21. Test:
tests/cloud/test_recomp_phase12.py. Rocky service tree `~/rush2049/recomp/dc/repo` = this branch (rsynced).

# 1. control_settings pak_nopak (func_800DD0C0 dialog): HARNESS, fixed; the drafts were right

- Fault PC first: the report had no N64 PC because clang tail-merged the identical `b join; lb t6,0(s0)` delay-slot
  copies of control_settings' switch cases into one line-0 block (addr2line `funcs_40.c:?`). Fix: the host build
  compiles RecompiledFuncs with `-mllvm -enable-tail-merge=false -mllvm -simplifycfg-sink-common=false` (CMake option
  `RUSH_EXACT_FAULT_LINES`, default ON; GCC -fno-crossjumping); a line-0 frame still names its function. Report now:
  `N64 PC 0x80800C14 `lb $t6, 0x0($s0)` = control_settings+0x368 (draft)`, s0 = 0x0000000B.
- Cause: 0x80800C14 is right after `jal func_800DD0C0` (the IPA out-shim to retail). Retail func_800DD0C0 writes
  s0 (`move s0,v0` = the font call's 11; `addiu s0,sp,0xAC`), s1 and s3 without saving them; it also stores them to
  the stack (s1/s3 spilled at 0x1AC/0x1B4 around a call, s0 passed as a stack argument at 0x18), and
  regmap_verify.Interproc counted those stores as prologue saves (the w31r rule: "a save if the body reloads the
  slot or writes the register"). The ipa_callees.json entry (hand-written from the frontier facts) had clobbers
  s2,s4-s8 only, so the shim let the draft's s0 die. Neither the (row, col) sp+0/sp+4 convention nor the draft.
- Fix (general): a stack store of a register is a save only if the body has not written that register at a lower
  address. Over all 1,216 game functions 24 unsaved sets grow; spot-checked (camera_look_at_point `swc1 f22,0x10(sp)`
  float stack args, track_data_decompress s7/s8, countdown_handler s0/s1, func_80097164 s0/s2/s4): all genuine
  writes. Table entries corrected: func_800DD0C0 -> s0-s8, func_80097164 -> s0-s7, func_80104704 +s5. draft_check
  also widens every table entry the draft names with the retail set (`NOTE ...: clobbers widened`). CACHE_EPOCH
  w32r-1 (all cached --all-lanes rows rerun).
- Verdicts (rocky, `--scripts pak_nopak,pak_corrupt,pak_repair,ghost_delete_fail`):
  w30d control_settings: `VERDICT: EQUIVALENT over 360 calls / 4 scripts (pak_nopak 90, pak_corrupt 90, pak_repair 90, ghost_delete_fail 90)`
  w29c control_settings: `VERDICT: EQUIVALENT over 360 calls / 4 scripts (pak_nopak 90, pak_corrupt 90, pak_repair 90, ghost_delete_fail 90)`
  w31d control_settings (group TU, func_800DD0C0 as C): `VERDICT: EQUIVALENT over 360 calls / 4 scripts (...)`
  w31d func_800DD0C0 alone (`draft_check func_800DD0C0 w31d/control_settings/best.c --scripts pak_nopak`, inbound
  shim sp+0/sp+4): `VERDICT: EQUIVALENT over 90 calls / 1 scripts (pak_nopak 90)`
  For w32d: nothing to fix in the drafts' logic; w31d's dc.args (`--scripts ghost_delete_fail`) can now name all
  four scripts.

# 2. Stock-host fault on `pak fail read 1 3` (player_reload, frame 120): REAL RETAIL BEHAVIOUR (game bug)

gdb on the stock capture host (breakpoints per function, a watchpoint on the buffer handle 0x801460BC):
SaveServer -> func_800C7EC8 -> drone_set_catchup (the player-note read; record 0x80146074, buffer handle +0x48)
-> osPfsReadWriteFile fails (ERR 3) -> func_800A1E94 -> the dialog pointer 0x80144008 = save_settings ->
control_settings "THIS CONTROLLER PAK IS CORRUPTED. PRESS THE A BUTTON TO FIX THE DATA." save_settings pumps
SaveServer each dialog frame (backtrace: pak_delete_file_retry <- draw_speedometer <- func_800C7EC8 <- SaveServer
<- save_settings <- drone_set_catchup <- func_800C7EC8 <- SaveServer <- input_aux_handler <- game_loop): the nested
SaveServer re-enters the same note's read, the player code deletes the note (pak_delete_file_retry) and clears +0x48.
When the dialog closes (START at 4 s = frame 120) the suspended read retries: `lw t8,0x48(s6)` = 0, `lw t9,0(t8)` at
0x800A2B10 = read of 0x00000000: a TLB exception on the console (only 0xC0000000 is TLB-mapped, __osTlbInit). All
retail code; the host only injected the error. Sweep read N ERR (N 1-2, ERR 1-12): read 1 ERR 1,2,3,4,5,7,8,9,12
crash (frame 120/210); 6 no dialog; 10/11 repair dialog, no crash in 600 frames; read 2: nothing (one boot read).
`pak fail` now refuses `read 1 ERR` (ERR not 6/10/11) in a `pak from` script, with the cause in the message;
`RUSH_PAK_FAIL_ALLOW_CRASH=1` overrides. All committed scripts still parse.

# 4. draft_check of overlay drafts: WORKS end to end

draft_check refused overlay functions; now `NAME` = `ovl_a:NAME` / `ovl_b:NAME` / `ovl_a_NAME` (or a plain name
found in one image; func_8038A400 is in both and must be qualified), census scripts from census/overlays.json,
trees/run dirs `ovl_a_NAME`, --all-lanes MATCHED from ovl_matched.lock.json. Rocky:
`python3 -m tools.recomp.draft_check ovl_a:func_80396F44 src/ovl_a/func_80396F44.c` ->
`VERDICT: EQUIVALENT over 2047 calls / 6 scripts (controls_frontend 412, pak_corrupt 327, pak_nopak 327, pak_repair 327, player_finish 327, saves 327)`;
injected bug (`return found + 1`): `DIVERGES ... controls_frontend, frame 210, func_80396F44 (draft) call #1: return
value differs (v0: retail 00000000 | draft 00000001)`; image B `ovl_b:func_8038A8CC src/ovl_b/func_8038A8CC.c` ->
`VERDICT: EQUIVALENT over 16 calls / 6 scripts`. (`found = 2` instead of 1 stayed EQUIVALENT: no script makes it
true; the return-value bug is the proof.)

# 3. draft_check --all-lanes after the w32n renames: 108 drafts: 47 EQUIVALENT, 56 MATCHED, 5 ERROR, no divergence

Service tree = w32r with wave32 13219eb6 merged; build/frontier.json + build/ipa_members.json copied from the Pi main
checkout's post-merge `ipa scan` / `frontier scan` (15:06; rocky has no Conveyor DB: `ipa scan` there scanned 0
targets, so the old files were restored first). CACHE_EPOCH w32r-2, so every row was recomputed ("(cached)" rows are
byte-identical drafts in two lane dirs, computed earlier in the same run).
`ALL LANES w2[8-9]*,w3*: 108 drafts: 47 EQUIVALENT, 5 ERROR, 56 MATCHED` (log/json: rocky
`~/rush2049/recomp/dc/lanes/w32r-all.{log,json}`).
- DIVERGES / FAULT: none. The three DIVERGES of w31r are gone: w28b SaveServer is MATCHED now; w29c/w30d
  control_settings EQUIVALENT over 360 calls / 4 scripts (job 1).
- ERROR (5): w31a `audio`, `f5ea8`, `hs`, w32a `hs`, w32b `audio`: unit directories with no dc.args (NAME = the
  directory name). Not rename damage: each best.c is byte-identical to a named twin that ran (w31a f5ea8 =
  func_80105EA8 -> animResults EQUIVALENT 26,166 calls; w31a/w32a hs = func_80105480 -> highscore_animText EQUIVALENT
  4,921 calls) or drafts a matched pair (audio_animText/audio_animButtonText). By hand: `draft_check --unit
  highscore_animText,DrawName,DrawGhost,DrawUnlocked w32a/hs/best.c --scripts ghost_pak,unlock,player_finish,race_finish`
  -> `VERDICT: EQUIVALENT over 2473 calls / 4 scripts` = w32a's pre-rename result. Lanes wanting the row: add
  `--unit ...` dc.args to those dirs.
- Rename invariance: every renamed draft's verdict and call count equals w31r's (e.g. DrawUnlocked 98, func_80100E58
  3,405, highscore_animText 4,921, CreatePlayer 5, LoadTrack 41, SetupWaveModel 282); no stale-dc.args ERROR.

Reconciliations asked by the coordinator:
- control_cell_text2's s8: confirmed undetected by my spill rule (the stack-argument store comes BEFORE its first
  write). Fixed here: a save also needs a reload of its slot (IDO always restores; a call-free body keeps the old
  rule). Interproc now gives control_cell_text2 +s8; 7 more functions grow (audio_doppler_calc s0,
  best_times_display s2, entity_tick_main s5, func_800B0A88 s1,s2, func_800E398C s7, render_replay_ui s4,
  save_validate s3; spot-checked: an sp store before the write, never reloaded). The conveyor's
  `frontier.unsaved_callee_writes` needs the same clause (not touched here).
- func_80097164 s0-s4 (conveyor) vs s0-s7 (table): both right for what they measure. The scan lists the function's
  own unsaved writes; Interproc is transitive and func_80097164 calls SetupWaveModel (was func_80096CA8, `jal
  0x80096CA8`), which writes s5-s7 unsaved. A shim around a call must save the transitive set: keep s0-s7.

# 5. Census refresh

wave32 13219eb6, 39 scripts, 150 unmatched: **147 reached (164,724 B), 3 never (632 B)**: world_effect_update,
fp_call_wrapper, object_bytes23_sum (no static reference). Was 163 of 166 at wave31. Overlays (`--overlays`):
image A 99 of 193 reached (unmatched 45 of 112), image B 35 of 49 (unmatched 19 of 28), unchanged.

# Other

- `RUSH_DC_BUILD_JOBS=N`: ninja -j per build side (two sides build concurrently; default 8 each). This lane ran
  with 4 (8 cores).

# Checks

(see below; filled at the end)

## Permission denials
None.
