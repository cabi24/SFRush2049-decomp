# CHANGES (for the tester)

Newest first. Every package version adds a section here (tools/recomp/package/CHANGES.md in the repo).

## v3 (2026-10-12, lane w26r; this package: @@PACKAGE@@)

From the v2 report (thank you: 0 hangs, good sound, and the list of small things below).

- **Controller pak: the game now saves.** The program plugs a 32 KB controller pak into controller 1, kept in
  `--home` as `rush2049.mpk` (the standard `.mpk` layout emulators use) and written after every change. So:
  - **No boot notice any more**: "NO CONTROLLER PAK DETECTED / CONTINUE WITHOUT SAVING" is gone; the Midway
    logo goes straight to the title, and with no input the attract demos start by themselves (~8 s).
  - **Players and records are saved**: START GAME -> PLAYER 1 now offers CREATE PLAYER (name entry), your
    player, DELETE PLAYER; the game writes the player (9 pages, note "RUSH2049" + name) and its statistics, and
    they are back after closing and restarting the program (RECORDS lists the players too). Log lines `pak:`.
  - **"INSERT RUMBLE PAK NOW" at the start of a race is expected**: the pak in the controller is a controller pak,
    not a rumble pak, and the game invites you to swap (as on a console with a controller pak plugged in). It goes
    away after a few seconds; the pause menu has a RUMBLE PAK item. There is no rumble in this build.
  - `--no-pak` gives the v2 behaviour (the notice); `--pak FILE` uses another image. With `--clock frames` (T8)
    the pak is a fresh image in memory that is never written (identical every run), and the T8 script runs
    without a pak as before, so T8's expected lines are unchanged.
- **Controller polish**: the `controller -> player N` line is printed once per pad (no repeat for the
  "device added" event SDL sends at start). The left-stick dead zone (0.15) is now rescaled: past the dead zone
  the value starts near 0 and still reaches 1.00 at the edge (v2: it jumped from 0 to 0.15). Back, Guide and the
  stick clicks (L3/R3) stay **unmapped on purpose**: the N64 pad has no such buttons and every N64 input already
  has a control.
- **Modes nobody had played are scripted and checked** (coordinator, RT64 on a software GPU): PRACTICE, GHOST
  RACE, CIRCUIT (BEGINNER CIRCUIT 8 RACES), BACKWARD, another car, RECORDS, BATTLE (2 players), saves. No hang,
  no new busy-wait. Scripts: `practice`, `ghost_race`, `circuit`, `backward`, `other_car`, `records`,
  `battle`, `saves`, `saves_reload`, `race_pak`, `attract_pak` (`--press-file rush\tools\recomp\scripts\NAME.press`).
- **Lockstep fix** (found on the way): with `--clock frames --renderer rt64` the game used to go on after 10 s if
  RT64 had not taken a frame, which could crash RT64 on a very slow GPU; it now waits (log
  `lockstep: still waiting for the renderer ...` every 10 s).
- **The "lockstep makes ~82 % of the audio" item was a measurement error**: the count came from the last
  once-per-second status line before the run stopped; the full output is 100.0 % of 22050 Hz (a new `final`
  line and a `lockstep: ... audio frames = 100.0% of 22050 Hz` line print it). Nothing to listen for.
- New status fields `pak_writes=` and `max_burst_ms=` (the longest time a game thread ran without a message
  call: a check that no thread starves the others in wall clock); options `--pak`, `--no-pak`, `--fb-every`.
- What to test: **T1 (no notice), T2 (attract with no START), T3b saves (create a player, race, quit, close,
  restart: the player is still there), T4b the new modes (practice, ghost race, circuit, battle with the pad as
  player 2, backward, another car, records), T7a the stick near the centre (small values now), sound
  everywhere** (listen again in the new modes), then T8.

## v2 (2026-10-12, lane w25r; this package: @@PACKAGE@@)

Fixes from the v1 report (thank you: it found the boot hang, the A5 target and the ROM copy).

- **Wall-clock boot hang fixed.** v1 hung on a black window in the default mode (13/13 runs): the game waits
  for the graphics scheduler in a loop with no system call (`game_loop`, 0x800FD500), and the runtime only
  switches threads inside system calls, so RT64's "frame done" never reached the game. The runtime now yields
  in that loop and in the 13 other loops of the same kind found in the game (log: `busy-wait at 0x...:
  yielding`, status field `spin_yields=`). Two of them are reached later (front end 0x8038BA20 at ~20 s, race
  start 0x80096184) and can hang the same way. Checked by the coordinator with RT64 itself on a
  software GPU: boots, menus, race. **Retest: T1-T4 in the default (wall-clock) mode.**
- **Sound.** The game's audio microcode (its own, not Nintendo's standard one) is recompiled with RSPRecomp
  (INSTALL A5/A6 build and run it), and the program plays the game's audio through SDL (`audio:` log line,
  `--volume`, `--no-sound`). A second fix makes the game produce all of its audio: it hands the audio
  hardware one short buffer per audio interrupt, which the runtime sent only once per screen refresh (half the
  sound). Checked only as data (a file, not speakers). **Retest: sound in every step; T6 changes each audio
  option and listens.**
- **Lockstep + RT64 crash fixed** (found by the coordinator, latent in v1): with `--clock frames` the game could
  rebuild a display list while RT64 was still reading it (crash at the race start on a slow GPU).
- **T8**: the RT64 self-check's differences are now explained (RT64 writes the depth buffer at 0x8017AEC0 and
  the framebuffers back into memory) and `oracle.py compare --mask-renderer` judges everything else.
- **Privacy**: the program deletes its copy of the ROM from `--home` once the game has loaded it; T9 still
  excludes `*.z64` and says how.
- **Docs**: INSTALL A5 builds `N64RecompCLI` (and `RSPRecomp`), on Linux too; TEST_PROCESS T6 keys corrected
  (UP/DOWN = row, LEFT/RIGHT = value); new T2 (5 min attract, wall clock), T6b (stunt mode), T7a (pad and
  keyboard mapping with `--log-input`); the default command is wall clock, `--clock frames` the fallback.
- **Attract mode exists** (headless evidence): v1's idle runs never left the boot notice "NO CONTROLLER PAK
  DETECTED ... CONTINUE WITHOUT SAVING"; after one START the game cycles title (5 s) and demo races (30 s) on
  the four tracks. T2 now presses START once, then idles; the A9 smoke test does the same.
- New: `--log-input`, `--wav PATH`, `--volume`, `--no-sound`, `--keep-rom-copy`; scripts `stunt.press`.
- What to retest: **wall-clock boot (T1, D3D12 and Vulkan), attract (T2), race/tracks (T3, T4), stunt (T6b),
  pad mapping (T7a), sound everywhere**, then T8.

## v1 (2026-10-12, lane w24r; this package: @@PACKAGE@@)

First gameplay-test package. Target tester OS: Windows 11 (the owner's gaming laptop).

- What you get: the stock recompiled game (no decompiled or replaced functions inside: the code is the
  N64Recomp translation of the original game code), built as a native program with a window (SDL2),
  the RT64 renderer and live keyboard/controller input. Earlier phases ran only headless with scripted
  input; nobody has seen the picture yet.
- Windows support is new and **unverified on Windows by the coordinator** (no Windows machine; the
  sources were checked against Windows headers only). Changes made for it:
  - The test-mode call interception (lockstep, virtual clock) no longer uses the GNU linker's `--wrap`
    (which MSVC's linker lacks); the build renames the calls in the recompiled C instead. Same behaviour,
    re-verified on Linux.
  - Per-call capture (`--call-log`, a coordinator tool) uses POSIX memory protection and is switched off
    on Windows (`RUSH_CAPTURE=OFF`, printed at configure time). The gameplay test does not need it;
    `--call-log` on Windows exits with a message saying so.
  - The headless (null) renderer is still available in the same program: `--renderer null`.
- New input scripts for the scripted replays: `race_track3.press`, `race_track4.press` (the track selector
  cycles four tracks) and `attract.press` (5 minutes idle); `--press-file` runs any of them from the program.
- Known limits: no sound (audio microcode not recompiled), no saves/controller pak, default RT64 settings,
  lockstep mode with the RT64 window is new (T8 second comparison is informative only).
- Pins: N64Recomp `@@N64RECOMP_COMMIT@@`, N64ModernRuntime `@@RUNTIME_COMMIT@@`, RT64 `@@RT64_COMMIT@@`.
