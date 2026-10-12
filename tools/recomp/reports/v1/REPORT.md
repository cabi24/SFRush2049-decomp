# Rush 2049 recomp gameplay-test report (v1)

Filled in by the operator (Claude Code in Claude Desktop, running commands directly on the owner's laptop), 2026-10-12.
Every command was run in a Visual Studio x64 developer PowerShell (Launch-VsDevShell.ps1 -Arch amd64 -HostArch amd64).
Gameplay input came from synthetic keyboard events sent to the game window (the operator cannot hold a controller);
no human free play yet, see section 7.

**Summary for the coordinator (read this first)**

1. The Windows build works end to end (one doc fix is needed in A5, see 2).
2. **In the default wall-clock mode with RT64 the game hangs at boot, on a black window, every time** (13/13 runs, both D3D12 and
   Vulkan): `gfx_tasks` stops at 2 and never grows. The game thread spins at 100% of one core in `game_loop`
   (`funcs_33.c:15043`, MIPS `0x800FD500`): a `bnel` busy-wait on the halfword at `0x8002EB70`, waiting for it to become 0
   (code path `L_800FD4F0`, taken when the words at `0x801074B4` and `0x801074B8` differ). Nothing clears it, because the spinning
   thread never yields to whatever should clear it. The headless null renderer does not hit this (906 gfx tasks in 30 s).
3. **With `--clock frames` (lockstep) the RT64 window works**: correct picture, live keyboard input, steady 30 gfx tasks/s,
   race, 2 players, tracks 2-4, pause and audio menus all fine. All gameplay results below come from that mode.
4. Privacy: the runtime copies the private synthetic ROM into every `--home` directory (`run\home\rush2049.n64.us.z64`,
   `run\self\h1..h3\...z64`, 13.5 MB each), so zipping `run\` as T9 says would include a ROM. The attached zip excludes them.

## 1. Tested version and machine

| Field | Value |
|---|---|
| package | rush2049-recomp-test-v1 (`VERSION.txt`: repo commit 8b8072e58f73dbbbea999f0ea56e2061e72655be) |
| pins built | N64Recomp ffb39cdad1da5de07eaaa48bd1db4a89a7986771, N64ModernRuntime cdf5abbd5026fef5c364c676e4667c45e42b6863, RT64 (`git -C src\rt64 rev-parse HEAD`): 23cab603c4f9f4a8b369b38e036f1aa484603878 |
| MANIFEST check (A4) | OK (`MANIFEST OK`) |
| OS + build number (`winver`) | Windows 11 Home 25H2, build 26200.9457 |
| CPU, RAM | 13th Gen Intel Core i9-13900H (14 cores / 20 threads), 39.6 GB visible |
| GPU + driver version | NVIDIA GeForce RTX 3050 6GB Laptop GPU, driver 32.0.16.1714; Intel Iris Xe Graphics, driver 31.0.101.4146 (hybrid laptop). RT64 ran on the NVIDIA GPU (the only adapter with dedicated VRAM in use: 410 MB during a race) |
| display resolution / refresh rate | 2880x1620 @ 120 Hz (single display); the game window was 1280x960 client area |
| compiler (`clang-cl --version`), CMake, Ninja, Python | clang-cl 19.1.5 (VS 2022 Build Tools 17.14, target x86_64-pc-windows-msvc), CMake 3.31.6-msvc6 and Ninja 1.12.1 (both bundled with VS), Python 3.12.10, Git 2.55.0.windows.3, Windows SDK 10.0.26100 |
| build path used | Windows native |
| graphics API RT64 chose (`RT64 setup:` log line) | D3D12 (`[rush] RT64 setup: ok, graphics API D3D12`); Vulkan also tested with `--gfx-api vulkan` (`RT64 setup: ok, graphics API Vulkan`) |
| controllers used | operator runs: keyboard (synthetic key events), no controller connected (no `controller ->` lines in those logs); owner free play (T7): Xbox 360 controller |

## 2. Build result per step

| Step | Result (OK / FAILED) | Duration | Error lines if failed |
|---|---|---|---|
| A1 tools | OK (already installed: VS 2022 Build Tools 17.14 with clang-cl, Git, Python 3.12; winget not run). Standalone Kitware CMake / Ninja not installed: the copies bundled with VS are on PATH in the dev shell and were used | n/a | Dev shell prints a harmless `'vswhere.exe' is not recognized as an internal or external command` at start |
| A3 ROM SHA-1 | OK `3F99351D7BB61656614BDB2AA1A90CFE55D1922C` | <1 s | |
| A5 N64Recomp | OK with a **doc fix**: as written, `cmake --build build\N64Recomp --target N64Recomp` exits 0 but builds only the static library `N64Recomp.lib`, so no `N64Recomp.exe`. On Windows the target is `N64RecompCLI` (CMakeLists.txt:117/129, `OUTPUT_NAME N64Recomp`); `cmake --build build\N64Recomp --target N64RecompCLI` produced `build\N64Recomp\N64Recomp.exe` | 8 s + 3 s | Last lines of the documented command: `[49/49] Linking CXX static library N64Recomp.lib` / exit 0 / no exe. Suggest `--target N64RecompCLI` in INSTALL A5 (probably for Linux too) |
| A6 make_rom + recompile (function count) | OK: `make_rom: wrote config\rom\rush2049.recomp.z64 (13500416 bytes, sha256 120270514798d1c08811c56f8ff1b0984aae063576cbfd642a6a6a1056b0e2ec) = manifest synthetic ROM`; `Function count: 2136`; 41 `funcs_*.c` + `recomp_overlays.inl` + `lookup.cpp` | 1 s | |
| A7 RT64 clone (HEAD) | OK `23cab603c4f9f4a8b369b38e036f1aa484603878` (clone, checkout, submodules all exit 0) | 82 s | |
| A8 configure (`Rush 2049 host:` line) | OK `-- Rush 2049 host: renderer rt64, capture OFF`; `check_contract: 75 shims and 3 hooks defined` | 13 s | Only CMake deprecation warnings (e.g. rt64 `nativefiledialog-extended/CMakeLists.txt:1 (cmake_minimum_required)`) |
| A8 build | OK: 643/643 steps; `build\host\rush2049_recomp.exe` (11.8 MB) + `SDL2.dll`, `dxil.dll`, `dxcompiler.dll` | 89 s | 18 warnings, none relevant: 9x `'_CRT_SECURE_NO_WARNINGS' macro redefined`, 2x `CONST`, 2x `UNUSED`, 1x `HLSL_CPU` macro redefined, 2x unused zstd functions, 1x `'osDestroyThread' has a non-throwing exception specification but can still throw`, 1x nodiscard ignored |
| null-renderer fallback build (only if A8 failed) | n/a: A8 succeeded (`--renderer null` of the same exe used for T0/T8) | | |

## 3. Scenarios

Two modes were used. **Wall clock** = the commands exactly as in TEST_PROCESS.md. **Lockstep** = the same command plus
`--clock frames` (the workaround found in T8). Wall-clock runs that used scripted input are listed by script name.

| Step | Pass / Partial / Fail | Symptoms (what you saw vs expected) | FPS (gfx_tasks/s) | Evidence files |
|---|---|---|---|---|
| T0 headless smoke (`final` line) | Pass | `load game: stream ROM 0xB0CB10 -> 0x80086A50`, `load ovl_a: stream ROM 0xB5C534 -> 0x8038A400`, `final t=30.4s vi=1814 gfx_tasks=906 audio_tasks=1792 other_tasks=0 swaps=1814 origin=0x007DDC80 fb_nonzero=4 fb_distinct=1 last_dl=0x80073D50(344 B) audio_samples=696576 image=ovl_a` | ~30 | logs\smoke.txt |
| T1 boot to title | **Fail** (wall clock, D3D12 and Vulkan); Pass in lockstep | Wall clock: window appears after 0.4 s, 1280x960, then **stays black for the whole 60 s** (screenshots 0% non-black). Log: RT64 setup ok, `load ovl_a` at 0.84 s, `first gfx task` at 0.87 s, then `gfx_tasks=2` forever while vi keeps counting. Same with `--gfx-api vulkan`. Lockstep (T8 run c, T3L): Midway logo + legal text with "MEMORY EXPANSION PAK ENHANCED" at ~3 s, "NO CONTROLLER PAK DETECTED / RUSH 2049 REQUIRES 1 NOTE AND 9 PAGES / CONTINUE WITHOUT SAVING" at ~4 s, then the title (SF skyline, cars, "PRESS START", "(c)1999 MIDWAY GAMES WEST INC.") by 8 s; correct colours, no garbage, 4:3, animates | wall: 0 after t=1 s; lockstep: 30 | logs\T1_boot.txt, T1_boot_vulkan.txt, T1_probe.txt, T1_stall_probe.txt, T1_stall_threads.txt, T1_stall_stacks.txt; shots\T1_boot_*, T1_boot_vulkan_*, T8_c_rt64_01/02 |
| T2 attract mode | **Fail** (wall clock) | Black window for 3 min, `gfx_tasks=2` throughout, no `load ovl_b`. Not repeated in lockstep (time) | 0 | logs\T2_attract.txt, shots\T2_attract_* |
| T3 race, default track | **Fail** wall clock (by hand and `race.press`); **Pass** lockstep by hand | Wall clock: black, the key presses change nothing, `gfx_tasks=2`. Lockstep by hand (keyboard, 170 s): START GAME (PLAYERS 1 selected) -> SELECT MODE (SINGLE RACE selected) -> SELECT TRACK (TRACK 1, LAPS 3, BACKWARD OFF, preview picture + city map) -> race loads (`load ovl_b` at 23.3 s) -> countdown 3-2-1 with 5 opponent cars (red sports cars, green pickup, yellow and silver cars), "INSERT RUMBLE PAK NOW" banner, checkered start gantry -> racing. HUD all present and updating: speed (0-148 MPH), odometer, position 6/6, LAP 1/3, two coin counters 0/8, track minimap with player dot, race timer. Drove 2 min: accelerate (X), steer (arrows), brake (C) all work; ran onto grass, scraped walls (sparks), crashed (smoke, slowed to 13 MPH), timer kept running; pause (Enter) shows "PLAYER 1 PAUSED" menu (CONTINUE / OPTIONS / CONTROLS / AUDIO / VIDEO / RUMBLE PAK / RESTART / QUIT), Enter again resumes. No freeze. Stayed 6/6 (the operator drives badly) | lockstep: 30 (31 three times), steady in menus and race | logs\T3_race.txt, T3_script.txt, T3_race_lockstep.txt; shots\T3_race_* (black), T3_race_lockstep_01..33 |
| T4 second track | **Fail** wall clock (`race_track2/3/4.press`, black, `gfx_tasks=2`); **Pass** lockstep by hand | Lockstep: on SELECT TRACK one RIGHT (stick/arrow) -> "TRACK 2", two D-pad RIGHT (D key) -> "TRACK 3", one LEFT -> "TRACK 4"; each shows its own preview picture and highlighted route on the city map, and the race is a visibly different track with a different minimap (track 2: wooded park; track 3: sunset with cliffs and a glass barrier; track 4: night city with yellow chevron walls). Names shown are only "TRACK 1..4" | lockstep: 30 | logs\T4_track2.txt, T4_track3.txt, T4_track4.txt (these are the lockstep runs; the wall-clock script logs of the same name were overwritten, their final lines were `gfx_tasks=2` like T3_script); shots\T4_track*_* |
| T5 two players | Partial: no game controller, so no by-hand test; **Pass** with `two_player.press` in lockstep | START GAME with PLAYERS 2 selected -> car select for PLAYER 1 and PLAYER 2 (CAR ROCKET ZX, TRANS. AUTOMATIC, HANDLING NORMAL) -> race in **horizontal split screen** (P1 top, P2 bottom, both 3D views rendered, each with its own speed/position/lap/coins HUD, "INSERT RUMBLE PAK NOW" on both) plus a shared panel on the right with the minimap, race timer and a RUSH 2049 logo. P2 moved independently (24 MPH, 5/6 while P1 6/6) | lockstep: 30 | logs\T5_two_player.txt (wall clock, `gfx_tasks=2`), T5_two_player_script.txt; shots\T5_two_player_script_* |
| T6 audio options | **Fail** wall clock (`audio_options.press`, black); **Pass** lockstep by hand | Pause -> DOWN x3 -> AUDIO -> A opens "AUDIO": EFFECTS VOLUME 10, MUSIC VOLUME 10, MUSIC TRACK DEFAULTS, SPEAKERS STEREO. Every row changed: EFFECTS 10->0, MUSIC 10->8, MUSIC TRACK DEFAULTS->OFF, SPEAKERS STEREO->MONO; B returns to the pause menu, START resumes the race. **Doc mismatch:** UP/DOWN move between rows and LEFT/RIGHT change the value (TEST_PROCESS T6 says the opposite). The selected row blinks (that is also why CONTINUE looks dark in pause-menu screenshots). Silent as expected | lockstep: 30 | logs\T6_audio.txt (lockstep; the wall-clock script log was overwritten, its final line was `gfx_tasks=2`); shots\T6_audio_* |
| T8 lockstep self-check (a vs b; a vs c first lines) | Pass / informative | a vs b: `EQUIVALENT over 1800 frames (judged: all frames, 1 baseline run(s))` (second line: thread-stack contents differ from frame 1, masked). a vs c (RT64 window): `DIFFERENT at frame 2: 71 rdram page(s)` (first pages 0x8017A000-0x80181FFF, "... 63 more pages"). Run c showed the full picture at normal speed: title, menus, race (`load ovl_b` at 24.1 s), 1781 gfx tasks in 60 s | c: 30 | self\a.log, b.log, c.log, a.err, b.err, c.err; logs\T8_compare_ab.txt, T8_compare_ac.txt, T8_c_rt64.txt; shots\T8_c_rt64_* |

Per screen seen (lockstep runs; wall clock shows only a black window):

- Midway logo + legal text + "MEMORY EXPANSION PAK ENHANCED" -> correct (text crisp, colours right) -> no input needed
- NO CONTROLLER PAK DETECTED / CONTINUE WITHOUT SAVING -> correct (option text blinks) -> passed automatically / with START
- Title "PRESS START" -> correct, animated -> START (Enter) works
- START GAME (PLAYERS 1 2 3 4, RECORDS, OPTIONS, AUDIO, VIDEO) -> correct -> stick and D-pad move; RIGHT changes player count
- SELECT MODE (SINGLE RACE, PRACTICE, GHOST RACE, CIRCUIT, STUNT) -> correct -> A accepts
- SELECT TRACK (TRACK n, LAPS 3, BACKWARD OFF, preview + map) -> correct -> stick and D-pad LEFT/RIGHT change track
- Car select (1P and 2P: CAR ROCKET ZX, TRANS. AUTOMATIC, HANDLING NORMAL) -> correct -> A accepts
- Race countdown + race HUD -> correct geometry, textures, transparency (exhaust glow, smoke, sparks), HUD -> accelerate/steer/brake work
- PLAYER 1 PAUSED menu -> correct (selected row blinks) -> START, DOWN, A, B work
- AUDIO options -> correct -> UP/DOWN = row, LEFT/RIGHT = value, B = back
- VIDEO options (VERTICAL CENTER, HORIZONTAL CENTER, RESET TO DEFAULTS + grey ramp / RGB test pattern) -> correct -> LEFT/RIGHT changed VERTICAL CENTER 0 -> -4 -> +1
- Split screen 2P race -> correct (see T5)

## 4. Crashes and freezes

No crash in any run (all exits code 0, Escape and window close work, `final` line always written). Windows Event Viewer has
one Windows Error Reporting entry for `rush2049_recomp.exe` at 09:13: event 1001, `RADAR_PRE_LEAK_64` (Windows' heuristic
memory-leak detector, probably reacting to the large recomp heap reservation `0x1F000000`); not a crash.

**Freeze, every wall-clock RT64 run** (T1 x3 incl. one probe run, T1 Vulkan, T1 stall probe, T2, T3 by hand, and the race / race_track2/3/4 / two_player / audio_options scripts: 0 of 13 runs got past it).
What was done before: nothing; it happens about 0.9 s after start, before any input. The window stays open and responsive
(Escape quits), the picture stays black, VI and swaps keep counting, `gfx_tasks` stays at 2 and `audio_tasks` stops at a
varying small number (1, 2, 41, 47 in different runs), which suggests a race. Log (T1_boot.txt, beginning and end; the
lines in between are identical status lines):

```
[rush] renderer rt64, clock wall, live input
[rush] SDL video driver: windows
[rush] RT64 setup: ok, graphics API D3D12
[rush] t=0.82s audio frequency 48000
[rush] t=0.82s first VI
[rush] t=0.82s first VI screen update, origin 0x00000000
[rush] t=0.83s shim osPiReadIo_recomp first called
[rush] t=0.83s load game: stream ROM 0xB0CB10 -> 0x80086A50 (load #1)
[rush] t=0.84s inflated game image check: 161761/161768 words equal the compiled image
[rush] t=0.84s inflate src 0x036300 -> 0x8019D500 (data, no code image)
[rush] t=0.84s inflate src 0x02F4E0 -> 0x8019D500 (data, no code image)
[rush] t=0.84s audio frequency 22050
[rush] t=0.84s load ovl_a: stream ROM 0xB5C534 -> 0x8038A400 (load #2)
[rush] t=0.84s idle thread parked (pause_self)
[rush] t=0.85s shim osStopThread_rx first called
[rush] t=0.87s largest display list so far: 160 B at 0x80062390 (gfx task 0)
[rush] t=0.87s first gfx task: ucode 0x80028CC0 data_ptr 0x80062390 size 160
[rush] t=0.89s first audio RSP task (completed without ucode)
[rush] status t=1.1s vi=21 gfx_tasks=2 audio_tasks=1 other_tasks=0 swaps=21 origin=0x007DDC80 fb_nonzero=0 fb_distinct=0 last_dl=0x80073D50(160 B) audio_samples=1536 image=ovl_a
[rush] status t=2.1s vi=81 gfx_tasks=2 audio_tasks=1 other_tasks=0 swaps=81 origin=0x007DDC80 fb_nonzero=0 fb_distinct=0 last_dl=0x80073D50(160 B) audio_samples=1536 image=ovl_a
...
[rush] status t=61.2s vi=3623 gfx_tasks=2 audio_tasks=1 other_tasks=0 swaps=3623 origin=0x007DDC80 fb_nonzero=0 fb_distinct=0 last_dl=0x80073D50(160 B) audio_samples=1536 image=ovl_a
[rush] final t=62.0s vi=3671 gfx_tasks=2 audio_tasks=1 other_tasks=0 swaps=3671 origin=0x007DDC80 fb_nonzero=0 fb_distinct=0 last_dl=0x80073D50(160 B) audio_samples=1536 image=ovl_a
```

Diagnosis done on the stalled process (logs\T1_stall_threads.txt, T1_stall_stacks.txt):

- Of 85 threads, one is Running at 100% of a core (3.0 s CPU in 3 s); all others are waiting.
- Sampling that thread's instruction pointer 10 times (SuspendThread/GetThreadContext, symbolised with llvm-symbolizer and
  the RelWithDebInfo PDB): 10/10 in `game_loop` at `config\RecompiledFuncs\funcs_33.c:15043`.
- That spot is the MIPS loop `0x800FD500: bnel $t8,$zero,0x800FD500 / lh $t8,0($v0)` with `$v0 = 0x8002EB70`, i.e. the
  game busy-waits until the halfword at `0x8002EB70` becomes 0. It is reached through `L_800FD4F0` when the words at
  `0x801074B4` and `0x801074B8` differ (otherwise `game_mode_handler` is called).
- On hardware an interrupt or another thread clears the flag while this spins. Under ultramodern the game threads are not
  preempted, so the spinning thread starves whatever should clear it. With the null renderer, or with `--clock frames`
  (lockstep delivers VI/SP/DP events itself), the flag is apparently already clear when the check runs. Suggested fix: a
  yield inside this loop (a patch/hook at 0x800FD500, like Zelda64Recomp does for busy-waits), or find out which event
  handler clears 0x8002EB70 and why it has not run after gfx task 2 with RT64's asynchronous completion.
- `lldb` from the VS LLVM toolset would not start (exit 0xC0000135, missing DLL), so there is no full backtrace.

## 5. Controller mapping issues

**Owner, Xbox 360 controller (T7, lockstep):** the button mapping made sense and the game played smoothly; no mapping problems reported.

Operator, keyboard (player 1), lockstep mode:

- Menus: both the stick (arrow keys) and the D-pad (W/A/S/D) work (track selector tested with both).
- A = X (accept / accelerate), B = C (back / brake), START = Enter (accept / pause): all fine.
- Steering with the arrow keys is digital, full lock: usable but twitchy; an analog pad will be better.
- L/R (Q/E), Z and the C buttons (I/J/K/L) were sent but never during a race (the attempt ended in the VIDEO menu), so their
  in-race effect (camera, etc.) is untested.
- TEST_PROCESS T6 has the audio-options keys the wrong way round (rows are UP/DOWN, values LEFT/RIGHT).

## 6. Performance

- Wall clock + RT64: 0 gfx tasks/s after the 1 s boot (frozen, see 4).
- Lockstep + RT64: exactly 30 gfx tasks/s in menus and in races (1P and 2P split screen); the per-second deltas over 170 s
  were all 30 except three 31s. No visible stutter in the screenshots (taken every 4-5 s).
- Load during a lockstep race: CPU 0.36 cores (about 2% of 20 logical CPUs), working set 448 MB; GPU (RTX 3050) 3D engine
  about 15%, copy engine about 6%, 410 MB dedicated VRAM. The frozen wall-clock state uses 1 full core (the busy-wait).
- Owner (T7, Xbox 360 controller): the game "played smoothly"; no stutter or input lag reported.

## 7. Free-text gameplay notes

- **The game itself looks right under RT64** (lockstep mode): title, all front-end menus, four tracks, cars, opponent AI,
  HUD, transparency effects (exhaust flame, smoke, sparks), sky gradients, Golden Gate Bridge and the city draw without
  visible errors, garbage textures or wrong colours; aspect 4:3. Text is crisp at 1280x960.
- Coordinator should look first at: (1) the wall-clock busy-wait at 0x800FD500 (makes the default command unusable);
  (2) INSTALL A5 target name; (3) ROM copies in `--home` vs the T9 "zip run\" instruction.
- Lockstep with the RT64 window is the playable configuration for now:
  `build\host\rush2049_recomp.exe --rom config\rom\rush2049.recomp.z64 --home run\home --clock frames`
  (without `--frames` it runs until the window is closed; live keyboard input works).
- **T7 owner free play (lockstep command above, run by the owner):** "the game seems whole, and working". They used an Xbox 360 controller and raced one lap of the first track with the 8-ball car; the button mapping made sense and it played smoothly. The only missing thing was the
  audio, which is completely absent. No crash or freeze was reported. That run had no `2>` redirect, so there is no log
  file for it. Before that the operator did an automated pass through the menus: logs\T7_stunt.txt (it reached
  START GAME -> OPTIONS/VIDEO instead of STUNT, because DOWN on START GAME moves through its rows) and logs\T7_perf.txt (perf run).
- No sound at all, as documented (audio microcode not recompiled); confirmed by the owner.

## 8. Attachments

`rush2049-recomp-test-v1-run.zip` (made from `run\`; everything except the ROM copies `run\home\*.z64` and
`run\self\h*\*.z64` and the runtime's `mods.json*`): this report, all logs in `logs\` and `logs\build\`, `self\*.log` and
`self\*.err`, and the screenshots in `shots\` (window client area only). No video: Game Bar recording was not used, since
the wall-clock runs show only a black window and the lockstep runs are documented with timed screenshots.
