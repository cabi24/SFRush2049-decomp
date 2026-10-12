# CHANGES (for the tester)

Newest first. Every package version adds a section here (tools/recomp/package/CHANGES.md in the repo).

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
