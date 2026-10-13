STATUS (2026-10-13): DONE. Recomp Phase 9 (decomp feedback tooling), branch w29r. Not a matching lane: no
src/, asm/, lock or coverage change. Docs: tools/recomp/README.md "Phase 9"; plan §18.

# 1. The host crash (-11): two causes, one host bug and one draft bug

## func_8010AEAC (w28c `func_8010AEAC/dc_static.c`, options_menus): HOST BUG, fixed

Reproduced on rocky (exit -11 after "replacement image loaded"). gdb: SIGSEGV in ultramodern `do_send` <-
osSendMesg_recomp <- `static_4_80800154` (the draft's static func_8010A8D0, recompiled from the .repl chunk) <-
repl__draft__func_8010AEAC. The recompiled helper calls `jal 0x800B4200` = slot_state_setup DIRECTLY, not through
the IPA shim: slot_state_setup takes s2 and clobbers s0,s1,s3 unsaved; s3 held &D_801461D0 (the gfx message
queue), so the next `osSendMesg(s3)` got garbage. Cause: IDO -O3 writes NO symbol for a static function, and
replace.py retargeted `jal callee` -> shim only inside named functions (`defined_functions`: FUNC symbols with a
size). The draft's 0x6E0 bytes of static helpers (printOption/animButtonText/font_set...) were never scanned
("ipa shim slot_state_setup: 2 call sites" = only AEAC's own). Fix (`replace.code_ranges`): every code range
(shim retargeting, `$zero`-load nops, self-call retargets outside a unit) is the named functions + the object's
whole `.text`. Now "ipa shim slot_state_setup: 4 call site(s)".

Verdicts after the fix (rocky):
- dc_static.c: `VERDICT: EQUIVALENT over 1649 calls / 2 scripts (options_menus 960, pause_menus 689)`
- best.c as a unit (`--unit func_8010AEAC,func_8010A8D0,func_8010A7A4 --params
  func_8010A7A4:x=s1,y=s2,item=a0,name=s3,value=s4`, the header's mapping): `VERDICT: EQUIVALENT over 1649 calls /
  2 scripts (options_menus 960, pause_menus 689)`; func_8010A8D0/func_8010A7A4 are called only from inside the
  unit (0 outer calls; exercised inside AEAC's compared calls). The unit object has no static code (the three
  members cover .text), so the unit form was never affected; w28c's crash run was dc_static.c.

## func_800C813C (w28b `func_800C813C/best.c`, saves + attract_pak): DRAFT BUG, now reported

gdb: SIGSEGV inside repl__draft__func_800C813C at N64 0x8080013C `lw $v0, 0x0($s2)`, s2 = 0x01010000. s2 comes
from `fileh = (PakHandle *)D_80144D68[pak].head` = `lw s2, 0x4D70(s2)` (field +8); retail reads `lw s2, 0x4D68(s2)`
(word 0x8E524D68 in func_800C813C): the per-pak file list head is at +0 of the 16-byte D_80144D68 entries (the
ghost list D_80152020 does use +8: retail `lw s0, 8(s7)`). On the console 0x01010000 is a TLB exception too:
genuine draft bug. With that one field changed (`.head` -> `.unk0`, local copy only, not delivered):
`VERDICT: EQUIVALENT over 3393 calls / 3 scripts (saves 1568, attract_pak 908, race 917)` (same --callee options
as w28b). The ghost branch (draw_ui_element) is still not exercised by these scripts.

What draft_check prints now for the unfixed draft (host fault report, below):

    saves          DIVERGES     3 calls of func_800C813C - draft FAULTED at frame 4  (4500 frames, 8 s)
    FIRST DIVERGENCE: script saves, frame 4, func_800C813C (draft) call #3 (global seq 3): the draft FAULTED (host signal; retail did not)
      fault: SIGSEGV, N64 read of 0x01010000 at frame 4 (thread game7)
      N64 PC 0x8080013C `lw $v0, 0x0($s2)` = func_800C813C+0x13C (draft)
      registers: v0=FFFFFFFF s2=01010000 sp=80031C90 ra=00000000 (thread context; may lag the host registers)
      N64 call chain: func_800C813C (draft) <- func_800C813C <- input_aux_handler @0x800FD408 <- game_loop @0x800FD60C <- audio_thread_entry @0x80002518
      faulting call: func_800C813C (draft) call #3 (global seq 3, entered at frame 4)

## Second harness finding: argument home slots (call log v3)

Before the fault, w28b's race/saves verdicts were "memory written differs: one extra word" at 0x80031CD8 =
sp_entry+0 (draft frame 72 B, sp 0x80031C90): the draft stores its a0 into the o32 argument home slot in the
CALLER's frame; retail does not. Those 16 bytes belong to the callee by the ABI and are not behaviour: the capture
now excludes [floor, floor+16) like the dead frames below (capture.cpp `excluded`, log header v3). Effect:
func_800C813C race DIVERGES -> EQUIVALENT (917 calls); w28b's func_800CD104 `dc.c` (saves, "one extra own-stack
word 0x80031BEC") -> `VERDICT: EQUIVALENT over 1 calls / 1 scripts (saves 1)`.

## Host fault report (runtime/src/fault.cpp, oracle.py, draft_check.py)

Host: SIGSEGV/SIGBUS/SIGFPE/SIGILL/SIGABRT handler (installed before capture's write-tracking handler, which
chains to it for faults outside rdram) prints `[rush] FAULT` lines: N64 address (host address - rdram base +
0x80000000), read/write, frame, thread, the replaced/probed call open on the faulting thread (generated hooks ->
rush_cap_enter/exit -> rush_fault_enter/exit, capture or not), host pc + backtrace, that call's recomp context
registers; then flushes the call log and the oracle log and dies as before (-11). oracle.py `run` tolerates a
signal death of the replaced host only (the stock runs must succeed), symbolises pc/backtrace with addr2line and
maps each recompiled-C line to the `// 0xADDR: insn` comment N64Recomp wrote above it, compares the call logs up to
the fault (an earlier differing call still comes first) and returns DIFFERENT with `crash`; draft_check reports a
DIVERGES with the fault lines above (exit 1), never ERROR.

# 2. Scripts for the unreached functions: 9 of 23 reached

New mechanisms (README "Phase 9"): script lines `poke TIME ADDR VALUE [BYTES]` (host `--poke`, applied when the
frame starts, both sides alike) and `pak from SCRIPT` (the harness runs SCRIPT once on the stock host and starts
from the pak image it leaves; cached per script chain). Every harness honours them (oracle.py run, draft_check,
census, script_probe, regmap_verify). Fixed: `script_probe --pak-write FILE` without `--pak` never wrote the image.

Levers found by dump diffs (host `--dump-at`, two runs that differ in one menu setting):
- race lap count = byte 0x80152735 (3 default/ghost, 1 after LAPS 1); a poke after race init (>= frame 735) makes a
  ghost race 1 lap; earlier pokes are overwritten at race start (~frame 700).
- battle points target = byte 0x80142513 (10; 5 with POINTS LEFT); 0 ends the battle at the next score check.

| script | frames | reaches (newly) |
|---|---:|---|
| player_reload (saves_reload + `pak from saves`) | 1500 | func_800C7EC8, drone_set_catchup, draw_speedometer (SaveServer reads player AAAA at frame 4) |
| ghost_save (`pak from saves`, AAAA ghost race, `poke 28 80152735 1 1`, RACE AGAIN) | 5000 | + func_800CC50C, func_800CC040 (BEST GHOSTS 3836), func_800CBF2C, menu_back (RACE AGAIN 4860: the in-memory ghost loads) |
| battle_end (battle.press + `poke 24 80142513 0 1`, A on SCOREBOARD) | 1800 | func_80101904 (GAME OVER standings 795), func_80102980 (SCOREBOARD 855) |

saves_reload itself stays "not runnable alone" (tests/recomp pins that; player_reload is the runnable copy).

Census (rocky, 30 scripts, 192 unmatched at wave29 8c74d8b6; `tools/recomp/census/`): **178 reached (194,028 B),
14 never (14,912 B)** (w28r: 183 of 206 / 23; 14 were matched in wave 28). Still never reached:
func_8010C974 (_AnimateMine: a mine pickup in battle), control_settings, func_800DD0C0, attract_mode_handler,
attract_demo_handler, attract_video_handler (image A only), func_80103D28 (DrawUnlocked: needs an unlock),
func_80104B14, func_800F0F44 (_FindGhosts: the ghost-save menu), draw_ui_element (SaveServer's ghost branch: a
ghost FILE on the pak), AdjustSpeed (pak file delete with retry), world_effect_update, fp_call_wrapper,
object_bytes23_sum (no static reference).

Ghost file not reached, what was tried: the 1-lap ghost finish enters BEST GHOSTS and the player record (128-byte
write) but writes no ghost note (GC updateHighScoreScreen: SaveGhost only when gGhostToSave is set; state 4 = the
save menu when ghost files already exist); JUST PLAY and pak-player flows alike; GAME OVER QUIT / NEW TRACK / RACE
AGAIN + a second 1-lap finish (1:35.864) also write none; lap-counter pokes (0x8014A647, 0x8014A980, 0x801E0ECB,
0x801F6178 := 3) did not shorten a 3-lap race. Next lever: find the N64 gGhostToSave (the store before the
high-score state switch in func_800F1930/func_800F1210) and poke it, or steer a real 3-lap finish.

# 3. draft_check --lane and the --gc check

`draft_check --lane LANE` (summary mode): every `cloud/work/frontier/LANE/<NAME>/best.c`, per-draft options in
`<NAME>/dc.args`, one table. `--gc` no longer needs a NAME word (it did since Phase 8's help said "give any word").
`--gc --dry-run` on rocky (dc 15.6 GB): 0.4 GB to free (trees #13/#14 by last use, one superseded retail cache).

# Checks
