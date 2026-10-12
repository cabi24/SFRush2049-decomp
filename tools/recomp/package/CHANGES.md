# CHANGES (for the tester)

Newest first. Every package version adds a section here (tools/recomp/package/CHANGES.md in the repo).

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
