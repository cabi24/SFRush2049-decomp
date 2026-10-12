# w26r: recomp Phase 6 (controller pak, pad polish, mode scripts, lockstep audio, cheaper capture, package v3) — tooling lane, branch w26r

Status: done (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w26r` (copy of this branch's tracked files + w25r's `b.sh`/`xvfb`; helper scripts
`sc.sh`, `modes.sh`, `rtp.sh`, `wsweep.sh`, `sanity/run.sh` there; nothing ROM-derived committed).
Inputs: `tools/recomp/reports/v2/REPORT.md`, plan §13/§14. Code commit `5919f821`.

## 1. Saves / controller pak — landed

Cause: N64ModernRuntime cdf5abbd `librecomp/src/pak.cpp` returns PFS_ERR_NOPACK from every osPfs* call. The game
calls osPfsInitPak, FreeBlocks, AllocateFile, DeleteFile, FileState, ReadWriteFile, RepairId (and osMotorInit);
no osEeprom*/SRAM/flash call exists in the recompiled code.

Fix: `tools/recomp/runtime/src/pak.cpp` implements those (plus FindFile, NumFiles, Checker) with libultra's
return codes over a 32 KiB image in the standard `.mpk` layout (ID block + 3 copies with checksums, inode table +
backup with checksum, 16 notes, 123 pages). CMake `RUSH_PAK_RENAMED` renames the calls in RecompiledFuncs to
`rush_osPfs*_recomp`. Controller 1 reports a non-rumble pak (status CONT_CARD_ON, osMotorInit PFS_ERR_DEVICE).
Wall clock: `--home/rush2049.mpk`, created formatted, rewritten (temp + rename) after each change. Lockstep: fresh
in-memory image, never written (deterministic), `--pak FILE` / `--pak-write` for save tests. `--no-pak` = v2.

What the game does (RT64 pixels, lockstep): no boot notice (Midway logo -> title ~3 s -> attract demos by
itself, frame 242); PLAYER 1 = JUST PLAY / CONTROLLER PAK 1 / players / CREATE PLAYER (/ DELETE PLAYER);
CREATE PLAYER -> ENTER YOUR NAME grid -> note "RUSH2049AAAA" (company 0x3544, game 0x4E525545, 9 pages = the
notice's "1 NOTE AND 9 PAGES"); "AAAA PAUSED"; GAME OVER table with AAAA. With a controller pak every race
starts with "INSERT RUMBLE PAK NOW" (swap prompt; expected) and the pause menu has RUMBLE PAK.

Verification:

    lockstep saves.press --pak p.mpk --pak-write:
      pak: allocated note 0 "RUSH2049AAAA" (company 0x3544 game 0x4E525545), 9 pages
      pak: write note 0, 2304 bytes at 0 / 64 bytes at 1888 / 32 bytes at 2016 / 128 bytes at 128 (quit)
    lockstep saves_reload.press --pak p.mpk:
      pak: controller 1, loaded .../p.mpk (1 notes used, 114 free pages), not saved (fixed image)
      pak: read note 0, 2304 bytes at 0   (boot, and again when AAAA is selected); file sha256 unchanged
    RT64 lockstep pixels with that pak: PLAYER 1 lists AAAA (+ DELETE PLAYER), car screen titled "AAAA",
      RECORDS lists AAAA.
    wall clock headless, same --home twice (saves.press 112 s, then saves_reload.press 45 s):
      run 1: pak: controller 1, new runs/wsv/home/rush2049.mpk ... allocated note 0 ... write x4 (t=24.0-91.5 s)
      run 2: pak: controller 1, loaded runs/wsv/home/rush2049.mpk (1 notes used, 114 free pages)
             pak: read note 0, 2304 bytes at 0 (t=0.42 s, t=13.02 s)
    RT64 wall clock (xvfb, fresh package build, the saved pak in --home):
      pak: controller 1, loaded .../run/rt/rush2049.mpk (1 notes used, 114 free pages); pak: read note 0 ... (4.43 s)
    No pak (--no-pak): the notice NO CONTROLLER PAK DETECTED (pixels), as v2.

Lockstep determinism with the pak: two null runs of a pak-flow script `EQUIVALENT over 700 frames (judged: all
frames, 1 baseline run(s))`. RT64 wall clock on lavapipe (~3 gfx tasks/s) is too slow to follow a wall-timed
script, so the scripted RT64 save checks are lockstep; the wall path is the headless one plus the RT64 load above.

## 2. Controller polish — landed (sdl_frontend.cpp; rt64 build only, not exercised without a pad here)

- `controller -> player N` printed only when a slot gets a different joystick instance (SDL's start-up
  DEVICEADDED re-open no longer prints); a removed pad prints `none`.
- Dead zone 0.15 rescaled: |v| < 0.15 -> 0, else (|v| - 0.15) / 0.85 (triggers/right-stick thresholds apply to it).
- Back, Guide, L3/R3 unmapped on purpose (commented; TEST_PROCESS T7a and CHANGES say so).

## 3. Scripted coverage of untested modes — landed

`.press` scripts may say `pak on|off` (host and oracle.py); all 8 old scripts now say `pak off` (frames and
verdicts unchanged); `oracle.py run` with `--press`/default runs `--no-pak`. New pak-flow scripts and what they
reach (null lockstep `modes.sh` + RT64 lockstep pixels `rtp.sh`):

| script | image B frame | race data | pixels |
|---|---:|---|---|
| race_pak | 580 | 0x43FE80 | race |
| attract_pak | 242, 1292, ... | tracks 1-4 cycle | title + demos |
| practice | 670 | 0x43FE80 | alone on track 1, no position/lap |
| ghost_race | 670 | 0x43FE80 | "PLAYER 1 / GHOST 1", 3:40 countdown |
| circuit | 760 | 0x43FE80 | CIRCUIT: NEW CIRCUIT / BEGINNER / ENTER CODE, "BEGINNER CIRCUIT 8 RACES" |
| backward | 670 | 0x43FE80 | BACKWARD ON (MIRROR, FOG rows appear), reversed route |
| other_car | 685 | 0x43FE80 | another car model |
| battle (2P) | 685 | arena (none of the 5 track streams) | SELECT TRACK BATTLE 1, POINTS 10, split screen |
| records | - | - | TOTALS (TRACK 1 BACKWARD, TRACK 2, STATS: LAP TIMES, AVERAGE TIMES) |
| saves / saves_reload | 1090 / 670 | 0x43FE80 | see §1 |

Wall-clock sweep (`wsweep.sh`: null renderer, `--null-dl-delay 25`, all 16 scripts in parallel, 70-95 s): every
race loads, busy-wait sites seen only the known hooked ones (0x800FD500, 0x8038BA20, 0x80096184, 0x8038B4E8,
0x8038B3DC, 0x8008AB3C), no hang. Long call-free computations: new run-burst census (osRecvMesg/osSendMesg added
to RUSH_WRAPPED; the time a game thread runs between message calls or busy-wait yields, after the first 2 s):
`max_burst_ms=` 0.3-0.8 for 15 scripts, 1.1 (race_track2). (A first version counted busy-wait spinning as a
burst: 26 ms at the title — waiting, not computing.)

Found and fixed: RT64 lockstep crashed 3x in parallel lavapipe runs (`renderer did not take gfx task 677 within
10 s`, then `Unable to find a matching GBI`, SIGSEGV): the mirror wait now has no timeout (warning every 10 s);
afterwards practice, ghost_race, battle, backward, other_car all completed.

Not done: the GAME OVER menu did not react to DOWN in saves.press (QUIT there not reached; the statistics write
happens at the in-race QUIT anyway).

## 4. Lockstep audio shortfall — explained (measurement artifact)

Lockstep runs stopped at `--frames` without a final status line, so the "~82 %" came from the last per-second
status line. Full output: race.press 1,322,688 frames in 3600 virtual VIs; w25r's binary `--wav`: 22050 Hz,
1322688 frames, 59.9859 s; this branch identical. A frame stop now prints `final` and
`lockstep: 3600 virtual VIs (60.00 s), 1322688 audio frames = 100.0% of 22050 Hz` (attract: 18000 VIs, 100.0 %).

## 5. register_params callees / cheaper capture — capture landed, IPA open

Cheaper capture (`capture.cpp`): rdram protected once; each window re-protects only pages written since (runs
per mprotect); no unprotect at the last exit. matched10 capture build, race.press, 333,252 calls: 21.0 s -> 15.5 s
(no `--call-log`: 2.7 s); `cmp` of the two call logs: identical. ess draft regression (cap-ess vs repl-ess):

    race / race_track2 / two_player / audio_options: EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / per-call: all 3599 captured calls identical
    saves:  EQUIVALENT over 4500 frames (judged: all frames, 2 baseline run(s)) / all 8999 captured calls identical
    battle: EQUIVALENT over 2700 frames (judged: all frames, 2 baseline run(s)) / all 5399 captured calls identical

IPA: not done. Each remaining `register_params` callee needs a prototype and an o32->register mapping proven
from a retail call site (`ipa_callees.json` "evidence"); the detector gives registers, not which parameter is
which. Phase 7 item (do them as drafts need them).

## 6. Package v3

- `rocky:~/rush2049/recomp/dist/rush2049-recomp-test-v3.tar.gz` = Pi `build/recomp/dist/rush2049-recomp-test-v3.tar.gz`
  in this worktree (git-ignored): 36,290,633 bytes, sha256 `e8905b4f1d1d17533c7a8fa8eb0365165a188a047222292c596b4b5bc08d08d0`,
  2192 files, repo commit 5919f821, 0 `.z64` members.
- Private, rocky only: `rush2049-recomp-test-v3-with-rom.tar.gz`, 47,723,226 bytes, sha256
  `ccd874c014f97b12e3836b50131d27631518046a7d721437a15294fb4562b5c7`.
- Sanity (`~/rush2049/recomp/w26r/sanity/run.sh`, log `sanity/log.txt`, fresh extract, INSTALL B): MANIFEST OK;
  prebuilt + built hosts headless 30 s, no input: `pak: controller 1, new run/home/rush2049.mpk`, non-silent audio
  3.21 s, `load ovl_b` 8.15 s (attract without START), `final ... gfx_tasks=899 ... image=ovl_b ... max_burst_ms=0.4`;
  saves/saves_reload as §1; lockstep a vs b `EQUIVALENT over 1800 frames (judged: all frames, 1 baseline run(s))`;
  RT64 container: wall 60 s loads the saved pak, demos at 9.7/44.7 s; a vs c `DIFFERENT at frame 2: 71 rdram
  page(s)`, `--mask-renderer` `EQUIVALENT except renderer-written pages over 1800 frames (judged: all frames, 1
  baseline run(s))`; 0 `.z64` left under run/.
- Docs: CHANGES v3, VERSIONS package_version 3, INSTALL A9 (no press; `pak:` lines), TEST_PROCESS (header, T1 no
  notice, T2 no START, T3 path + rumble prompt, new T3b saves, new T4b modes table, T6b path, T7a dead zone, T9
  `.mpk` ok), REPORT_TEMPLATE rows.

## Checks

- Pi `python3 -m pytest tests/recomp -p no:cacheprovider`: `146 passed, 3 skipped`. New tests: pak directive
  parsing, `host_command` `--no-pak`, every committed script's pak mode, battle has 2 controllers.
- rocky (civenv, private inputs RUSH2049_ROM / RUSH2049_RECOMP_HOST / RUSH2049_RECOMP_ROM / RUSH_N64RECOMP_INCLUDE
  from build/stock): `147 passed, 2 skipped` (game_code.bin, libubsan).
- Pi `make test-fast JOBS=4` in this worktree: tests/conveyor `40 failed, 1506 passed, 806 skipped, 2 errors`,
  tests/cloud `770 passed, 179 skipped` (exit 2). **Identical on a fresh worktree of the base 46780d3e** (same
  counts): the conveyor failures/errors are environmental (fresh worktree without build artifacts;
  `No module named 'src.scorer'`, IDO-dependent tests), not from this lane, which touches only tools/recomp and
  tests/recomp.

## Laptop prompt (owner -> Claude on the Windows laptop)

> You are testing package v3 of the Rush 2049 N64 recompilation on this Windows 11 laptop
> (rush2049-recomp-test-v3.tar.gz; the ROM is in the -with-rom variant or I will give you rom\baserom.us.z64).
> Extract it to C:\rush2049\, read README.md, CHANGES.md (v3 section) and INSTALL.md, and build it by following
> INSTALL section A exactly (same steps as v2). Then follow TEST_PROCESS.md in order, wall clock (no --clock)
> unless a step says otherwise, always with --home run\home so saves carry over: T0 smoke (no START needed now),
> T1 boot on D3D12 and Vulkan (there should be no "NO CONTROLLER PAK DETECTED" notice), T2 attract with no input,
> T3 race (note "INSERT RUMBLE PAK NOW" at the start: expected), T3b saves (I will type a player name; create it,
> race, quit, close, restart and check the player is still there and in RECORDS), T4-T6b briefly, T4b the new
> modes (practice, ghost race, circuit, backward, another car, battle with my pad as player 2, records), T7a pad
> mapping with --log-input including slow stick movement near the centre, T7 free play with me, T8 lockstep
> self-check (all three compare lines). Describe what you see and what I hear at every step. Fill
> REPORT_TEMPLATE.md, save it with the logs under run\, delete any *.z64 under run\ as T9 shows (the .mpk save
> file may stay), zip run\ (no ROM, no --wav files) and give me the report and the zip.

## Open items for Phase 7

v3 tester report (saves by hand, new modes, dead zone by feel); remaining register_params IPA entries; rumble
(the host reports a controller pak and has no rumble output; a "pak swap" model or a rumble-only option); paks on
controllers 2-4 (only controller 1 has one); GAME OVER menu navigation in scripts; capture cost beyond faults
(log formatting); Windows per-call capture; pad changes unverified on hardware (no pad on rocky).

## Permission denials

None.
