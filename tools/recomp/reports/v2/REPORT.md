# Rush 2049 recomp gameplay-test report (v2)

Filled in by the operator (Claude Code in Claude Desktop, running commands on the owner's laptop) with the owner listening
and playing, 2026-10-12. Every command ran in a Visual Studio x64 developer PowerShell. The operator drove with synthetic
keyboard input sent to the game window; the owner pressed START on the boot notice in T1, played player 2 in T5 and did
T7a with an Xbox 360 pad, and judged all sound by ear. Screenshots are captured from the game window only
(PrintWindow), one every 4-10 s.

**Summary for the coordinator**

1. **The v2 fixes work on Windows.** Every wall-clock run booted and played: 0 hangs and 0 crashes in 17 windowed runs,
   steady 30 gfx tasks/s (27-31). All five busy-wait sites the runtime yields at were seen: 0x800FD500 (boot),
   0x8038BA20 (front end, ~20 s), **0x8038B4E8** (track selector, every time the track was changed), 0x80096184 (race start).
   `--clock frames` was never needed except for T8.
2. **Sound works and the owner rated it highly** ("sounded awesome", "audio performed as expected"): title music,
   countdown, engine, race music, split-screen sound, stunt mode; stereo, in sync with the picture, no pops. All four audio
   options change the sound as intended (also measured: MONO makes left and right sample-identical). The recorded output
   runs at real-time speed with no gaps except the boot notice and the pause screen (both silent by design).
3. T2 attract mode, T6b stunt mode, T7a pad mapping and T8 (`--mask-renderer`: `EQUIVALENT except renderer-written pages`)
   all match the headless expectations.
4. Small findings: the `controller -> player N` line is printed twice for one pad; the left-stick dead zone jumps from 0
   to 0.15 (no rescaling); Back and the stick clicks are unmapped; on the boot notice A works as well as START. T7 free play
   was skipped by the owner this round.

## 1. Tested version and machine

| Field | Value |
|---|---|
| package | rush2049-recomp-test-v2 (`VERSION.txt`: repo commit c2b81dc6bbd5033b9bdc03c367c0d4af494e0f04) |
| pins built | N64Recomp ffb39cdad1da5de07eaaa48bd1db4a89a7986771, N64ModernRuntime cdf5abbd5026fef5c364c676e4667c45e42b6863, RT64 (`git -C src\rt64 rev-parse HEAD`): 23cab603c4f9f4a8b369b38e036f1aa484603878 |
| MANIFEST check (A4) | OK (`MANIFEST OK`) |
| OS + build number (`winver`) | Windows 11 Home 25H2, build 26200.9457 |
| CPU, RAM | 13th Gen Intel Core i9-13900H (14 cores / 20 threads), 39.6 GB |
| GPU + driver version | NVIDIA GeForce RTX 3050 6GB Laptop GPU, driver 32.0.16.1714 (RT64 runs on it: the only adapter with dedicated VRAM in use); also Intel Iris Xe, driver 31.0.101.4146 |
| display resolution / refresh rate | 2880x1620 @ 120 Hz (single display); game window 1280x960 client area |
| compiler (`clang-cl --version`), CMake, Ninja, Python | clang-cl 19.1.5 (VS 2022 Build Tools 17.14, x86_64-pc-windows-msvc), CMake 3.31.6-msvc6 and Ninja 1.12.1 (bundled with VS), Python 3.12.10, Git 2.55.0.windows.3, Windows SDK 10.0.26100 |
| build path used | Windows native |
| graphics API RT64 chose (`RT64 setup:` log line) | D3D12 (`RT64 setup: ok, graphics API D3D12`); Vulkan tested in T1 (`RT64 setup: ok, graphics API Vulkan`) |
| controllers used | keyboard (operator, synthetic key events); Xbox 360 pad via "Xbox 360 Wireless Receiver for Windows" (owner: T5 player 2, T7a) |

## 2. Build result per step

| Step | Result (OK / FAILED) | Duration | Error lines if failed |
|---|---|---|---|
| A1 tools | OK (already installed from v1; VS-bundled CMake/Ninja used; dev shell prints a harmless `'vswhere.exe' is not recognized`) | n/a | |
| A3 ROM SHA-1 | OK `3F99351D7BB61656614BDB2AA1A90CFE55D1922C` | <1 s | |
| A5 N64Recomp + RSPRecomp (both .exe present) | OK: `N64Recomp.exe` (890,368 B) and `RSPRecomp.exe` (472,064 B); the documented `--target N64RecompCLI RSPRecomp` works as written | 12 s | |
| A6 make_rom + recompile (function count; `rsp\rush_audio_ucode.cpp` written) | OK: synthetic ROM sha256 `120270514798â€¦0e2ec` = manifest; `Function count: 2135`; 41 `funcs_*.c` + `recomp_overlays.inl` + `lookup.cpp`; RSPRecomp printed nothing and wrote `config\rsp\rush_audio_ucode.cpp` (2,548 lines). N64Recomp also prints `[Info] Ambiguous jal target 0x8038A400 in function input_aux_handler, falling back to function lookup` (info only) | 2 s | |
| A7 RT64 clone (HEAD) | OK `23cab603c4f9f4a8b369b38e036f1aa484603878` | 66 s | |
| A8 configure (both `Rush 2049 host:` lines) | OK `-- Rush 2049 host: renderer rt64, capture OFF`, `-- Rush 2049 host: recompiled audio ucode`; `check_contract: 75 shims and 4 hooks defined` | 11 s | only CMake deprecation warnings |
| A8 build | OK: 645/645 steps, 0 errors, 18 warnings (same benign set as v1: macro redefinitions, unused zstd functions, `osDestroyThread` exception spec, nodiscard); `rush2049_recomp.exe` (11.96 MB) + `SDL2.dll`, `dxil.dll`, `dxcompiler.dll` | 79 s | |
| null-renderer fallback build (only if A8 failed) | n/a: A8 succeeded | | |

## 3. Scenarios

Mode: **wall** = the default command (no `--clock`), **frames** = with `--clock frames`. No wall-clock step failed, so
there are no frames rows except T8.

| Step | Mode | Pass / Partial / Fail | Symptoms (what you saw vs expected) | Sound (what you heard) | FPS (gfx_tasks/s) | Evidence files |
|---|---|---|---|---|---|---|
| T0 headless smoke (`final` line, `audio_nonzero=`) | null | Pass | `removed the ROM copy`, `load game: stream ROM 0xB0CB10 -> 0x80086A50`, `load ovl_a` 0.16 s, `audio ucode: recompiled (rush_audio_ucode, RSPRecomp)`, `first non-silent audio buffer` 5.10 s, `load ovl_b` 10.02 s; `final t=30.3s vi=1811 gfx_tasks=905 audio_tasks=1798 other_tasks=0 swaps=1811 origin=0x007BB640 fb_nonzero=4 fb_distinct=1 last_dl=0x80062390(8136 B) audio_samples=1329792 audio_nonzero=1111610 audio_peak=32768 image=ovl_b spin_yields=0`; no `.z64` left in `run\home` | n/a | ~30 | logs\smoke.txt |
| T1 boot to title (D3D12) | wall | Pass | Window after 0.37 s (1280x960, 4:3). First picture at ~2 s: Midway logo + legal text; ~5 s: "NO CONTROLLER PAK DETECTED / RUSH 2049 REQUIRES 1 NOTE AND 9 PAGES / CONTINUE WITHOUT SAVING" (waits there). The owner pressed START at ~59 s -> title screen ("PRESS START", (c)1999 Midway), correct colours, no garbage. Log: `RT64 setup: ok, graphics API D3D12`, `load ovl_a` 1.36 s, `first gfx task` 1.39 s, `busy-wait at 0x800FD500: yielding` 1.40 s, `audio: SDL driver wasapi, device 48000 Hz stereo float`, `first non-silent audio buffer` 59.30 s (after the START) | Silent on the boot notice (owner: as expected); title music after START, fine | 30 (23 in the first second) | logs\T1_boot.txt; shots\T1_boot_* |
| T1 boot to title (`--gfx-api vulkan`) | wall | Pass | Same boot. The owner pressed START at ~20 s -> title, then the attract demo by itself (`load ovl_b` 26.1 s, title again 56.1 s, next demo 61.1 s); `busy-wait at 0x80096184: yielding` 56.09 s. Screenshots of this run were taken while another window covered the game, so they showed that window's content and were deleted (capture switched to window-only PrintWindow afterwards) | Music after START, fine | 30 | logs\T1_boot_vulkan.txt |
| T2 attract mode (5 min idle) | wall | Pass | START at 5.3 s, then idle 5 min: title (~5 s) and a demo race (~30 s), repeated 8 times (`load ovl_b` at 10, 45, 80, 115, 150, 185, 223 s). The demos are real races: AI cars driving themselves, changing cameras (chase, bumper with the yellow hood, in-car), a car tumbling after a crash (160 s), "PRESS START" blinking; all four tracks appear. From ~223 s the demo shows the **staff credits** over the race (PROGRAMMERS, PRODUCER, MUSIC Barry Leitch, PRODUCT MANAGER, TEST MANAGER, LEAD TESTER, PRODUCT ANALYSTS, TESTERS). It loops. No video (screenshots every 10 s instead) | Owner: music played fine, no pops or quality issues, stereo works | 27-31 | logs\T2_attract.txt; shots\T2_attract_01..32 |
| T3 race, default track | wall | Pass | Keyboard by hand: title -> START GAME (PLAYERS 1) -> SELECT MODE (SINGLE RACE) -> car select (stats bars TOP SPEED / ACCELERATION / CONTROL / STRENGTH, CAR ROCKET ZX, AUTOMATIC, NORMAL) -> SELECT TRACK (TRACK 1, LAPS 3, BACKWARD OFF) -> race (`load ovl_b` 23.26 s; busy-waits yielded at 0x8038BA20 19.95 s and 0x80096184 23.25 s). Start gantry, opponents, 3-2-1, full HUD (MPH, odometer, position, LAP, coin counters, minimap, timer). Drove 2.5 min: full throttle, steering, grass, a jump (airborne 75 s), car flipped (80 s), crash smoke (110 s), braking, LAP 2/3 from 145 s, pause menu and resume. No freeze | Owner: countdown, engine and music sounded right and stayed in sync. Recorded output: 173.8 s of audio for a 175 s run (real time), silent only on the boot notice (0-3.8 s) and while paused (151.8-156.8 s), no other gaps, true stereo | 30 (one 28, three 31) | logs\T3_race.txt; shots\T3_race_01..34 |
| T4 second track (+ tracks 3, 4) | wall | Pass | SELECT TRACK: stick RIGHT x1 -> "TRACK 2", D-pad RIGHT x2 -> "TRACK 3", stick LEFT x1 -> "TRACK 4" (names are just "TRACK n"); each has its own preview, map route and a visibly different race (track 2 wooded park; track 3 sunset cliffs + glass barrier; track 4 night city, chevron walls), different minimaps. New busy-wait site yielded at the selector: `busy-wait at 0x8038B4E8: yielding` (~18.3 s, all three runs) | Owner: "sounded awesome" | 28-31 | logs\T4_track2.txt, T4_track3.txt, T4_track4.txt; shots\T4_track*_* |
| T5 two players | wall | Pass | `--keyboard-alone`: keyboard = player 1 (operator), Xbox 360 pad = player 2 (owner); log `controller -> player 2: Controller (Xbox 360 Wireless Receiver for Windows)` (printed twice). START GAME PLAYERS 2 -> SELECT MODE now lists SINGLE RACE, PRACTICE, STUNT, BATTLE -> car select for PLAYER 1 and PLAYER 2 -> horizontal split screen (P1 top, P2 bottom, each with its own HUD) + shared right panel (minimap, timer, RUSH 2049 logo). The owner's car reached 211 MPH, 2nd-3rd of 6, lap 2/3; P1 got "WRONG WAY". Both inputs work | Owner: "test was awesome, sounded great" | 30 (27-31) | logs\T5_two_player.txt; shots\T5_two_player_01..31 |
| T6 audio options (each row changes the sound?) | wall | Pass | Four races, each changing one row from defaults (pause -> DOWN x3 -> A -> AUDIO; UP/DOWN = row, LEFT/RIGHT = value, as corrected in v2), then B and START to resume at ~49 s: a) EFFECTS VOLUME 10 -> 0 (LEFT x10), b) MUSIC VOLUME 10 -> 0, c) MUSIC TRACK DEFAULTS -> **BATTLE2** (LEFT x1), d) SPEAKERS STEREO -> MONO. The screen shows each new value | Owner: "all tests successful, audio performed as expected". Measured (level 30-35 s vs 52-74 s): a -11.7 -> -14.2 dBFS, b -11.6 -> -14.0 dBFS (one component removed, sound continues), c -11.7 -> -6.3 dBFS (another tune), d left/right difference 1.28 -> **0.00** (exact mono). The pause/options screens are silent | 30 | logs\T6_audio_a_effects0.txt, _b_music0, _c_musictrack, _d_mono; shots\T6_audio_* |
| T6b stunt mode | wall | Pass | SELECT MODE DOWN x4 -> STUNT -> car select -> arena (`inflate src 0x812BE0`, then 0x89F220, 0xA680B0, 0xAEA800 at 23.3 s, as expected): a bowl arena with ramps and banners; 3-2-1; trick score at the bottom (0 -> 540 -> 597 -> 603), trick text "BONUS X 3 / TRIPLE BONUS X10 / NO WINGS BONUS X7", combo icons (x2, x3), crash explosions. Pause -> CONTROLS page "PLAYER 1 CONTROLS": GAS A, BRAKE B, STEERING stick, SHIFT UP R, SHIFT DOWN C-down, REVERSE C-left, ABORT C-up, VIEW L, WINGS/SHOOT Z, RUMBLE OFF, RESET CONTROLS | Owner: "sounded good and looked good" | 30-31 | logs\T6b_stunt.txt; shots\T6b_stunt_01..42 |
| T7a pad / keyboard mapping (table in section 5) | wall | Pass | Every control gives the N64 input in the TEST_PROCESS table (section 5). Then the owner raced a lap with the pad (118-218 s in the log) | n/a | 30 | logs\T7a_pad.txt, T7a_keyboard.txt |
| T7 free play | wall | n/a | Skipped by the owner this round ("all other tests were great"); T7_free.txt is only the launch, closed on the boot notice | n/a | | logs\T7_free.txt |
| T8 lockstep self-check (a vs b; a vs c; a vs c --mask-renderer first lines) | frames | Pass | a vs b: `EQUIVALENT over 1800 frames (judged: all frames, 1 baseline run(s))`. a vs c: `DIFFERENT at frame 2: 71 rdram page(s)` (first pages 0x8017A000-0x80181FFF, "... 63 more pages"). a vs c `--mask-renderer`: `EQUIVALENT except renderer-written pages over 1800 frames (judged: all frames, 1 baseline run(s))`, `renderer mask: 105 page(s) not judged (RDP color/depth images)`. Run c: RT64 D3D12, race at 24.6 s, no crash (v2 lockstep fix confirmed on a real GPU) | n/a | 30 | self\a.log, b.log, c.log, *.err; logs\T8_compare_ab.txt, T8_compare_ac.txt, T8_compare_ac_mask.txt, T8_c_rt64.txt; shots\T8_c_rt64_* |

Per screen seen (T1-T6b, all wall clock): screen -> rendering -> input -> sound

- Midway logo + legal text -> correct -> none needed -> silent
- NO CONTROLLER PAK DETECTED / CONTINUE WITHOUT SAVING -> correct (option blinks) -> START and also pad A pass it -> silent (by design)
- Title "PRESS START" -> correct, animated -> START works -> title music
- Attract demo races + credits demo -> correct (AI cars, cameras, HUD-less with blinking PRESS START) -> START returns -> music/engine
- START GAME (PLAYERS 1-4, RECORDS, OPTIONS, AUDIO, VIDEO) -> correct -> stick and D-pad, RIGHT sets player count -> menu music
- SELECT MODE (1P: SINGLE RACE, PRACTICE, GHOST RACE, CIRCUIT, STUNT; 2P: SINGLE RACE, PRACTICE, STUNT, BATTLE) -> correct -> DOWN/A -> music
- Car select (stat bars, CAR/TRANSMISSION/HANDLING; 2P: two panels) -> correct -> A -> music
- SELECT TRACK (TRACK n, LAPS, BACKWARD, preview, city map) -> correct -> LEFT/RIGHT (stick or D-pad) -> music
- Race (countdown, HUD, opponents, effects) -> correct -> A/B/stick/R -> countdown, engine, music, in sync
- Split-screen race -> correct -> both players -> sound fine
- Stunt arena (score, bonus text) -> correct -> works -> sound fine
- PLAYER 1 PAUSED menu, AUDIO options, PLAYER 1 CONTROLS page -> correct (selected row blinks) -> works -> silent while paused

## 4. Crashes and freezes

None. All 18 windowed runs (17 wall clock + T8 run c) and 3 headless runs ended normally with a `final` status line (exit code 0); no hang, no
black window, `gfx_tasks=` grew ~30/s in every wall-clock run. Windows Event Viewer has no Application entry for
`rush2049_recomp.exe` since the v2 build started (v1 had one `RADAR_PRE_LEAK_64` entry; not seen this time).

```
(nothing to paste: no crash or freeze)
```

## 5. Controller mapping (T7a)

| Physical control (pad model: Xbox 360 controller via "Xbox 360 Wireless Receiver for Windows") | N64 name printed by `--log-input` | OK / wrong / nothing |
|---|---|---|
| A / B / X / Y | A / C-DOWN / B / C-UP | OK (as in the table) |
| LB / RB / LT / RT | L / R / Z / R | OK |
| Start / Back | START / nothing | OK / nothing (Back is not mapped; expected) |
| D-pad up/down/left/right | D-UP / D-DOWN / D-LEFT / D-RIGHT | OK |
| left stick (4 directions) | analog stick, x and y up to +-1.00 (up = y +1) | OK; dead zone: values below 0.15 read 0, the first non-zero value is 0.15 (no rescaling, so a small step) |
| right stick (4 directions) | C-LEFT / C-RIGHT / C-UP / C-DOWN (diagonals give two C buttons, e.g. C-UP+C-RIGHT) | OK |
| stick clicks (L3 / R3) | nothing | nothing (not mapped; expected) |
| keyboard keys (table in TEST_PROCESS) | X=A, C=B, Z=Z, Q=L, E=R, W/S/A/D=D-UP/DOWN/LEFT/RIGHT, I/K/J/L=C-UP/DOWN/LEFT/RIGHT, arrows=stick +-1.00, Enter=START | OK, all 18 checked |

The owner raced a lap with the pad (and played player 2 in T5): the mapping made sense, no control felt wrong. In the
game's own terms (pause -> CONTROLS) that means GAS = pad A, BRAKE = pad X, SHIFT UP = RB/RT, SHIFT DOWN = pad B,
REVERSE = right stick left, ABORT = pad Y, VIEW = LB, WINGS/SHOOT = LT. Menus work with both the stick and the D-pad.
Cosmetic: the `controller -> player N: ...` line is printed twice at start for one pad (open + device-added reopen?), and an
idle stick prints `y=-0.00`.

## 5b. Sound

- `audio: SDL driver wasapi, device 48000 Hz stereo float`; the `--wav` output is 22050 Hz stereo 16-bit.
- `audio_nonzero=` at the end of T3: 7286070 (of `audio_samples=7666560`).
- What plays (owner, by ear): silence on the boot notice (expected); title music after START; music in the attract
  demos; countdown, engine (follows the speed) and race music in races; sound in split screen and in stunt mode. All rated
  good: "no pops or obvious quality issues", "stereo sound worked", "stayed in sync", "sounded awesome".
- Options (T6): effects volume 0, music volume 0, music track (BATTLE2) and mono all change the sound as expected (owner
  and measurement, see T6).
- Measurement of the T3 recording: plays at real-time speed (173.8 s of audio in a 175 s run), no gaps except boot
  notice and pause, left and right differ (true stereo).
- Problems: none found, in any step (all wall clock).

## 6. Performance

- Menus and races, 1P and 2P, stunt: 30 gfx tasks/s typical; worst 27 (one second each in T2 and T5, at a load) in wall
  clock; first second of each run 22-25 (boot). No stutter seen in screenshots or reported by the owner.
- Input lag: the owner noticed none (pad, T5 and T7a).
- Load in a wall-clock race (T7_perf.txt): CPU 0.55 cores (2.7% of 20 logical CPUs), working set 452 MB; RTX 3050
  3D engine 12.5%, copy engine 5.5%, 236 MB dedicated VRAM.

## 7. Free-text gameplay notes

- v2 fixes the two v1 blockers on this laptop: the default command boots and plays, and there is good sound. The
  owner's verdict across T1-T7a: everything looked and sounded right.
- Busy-wait sites seen (all yielded, none hung): 0x800FD500, 0x8038BA20, 0x8038B4E8 (track selector, not listed in
  CHANGES.md's examples), 0x80096184. `spin_yields=` per run 16-93.
- The boot notice accepts A as well as START (T7a: the owner's first pad A press passed it).
- T7 free play was skipped by the owner this round; Practice, Ghost Race, Circuit and Battle modes, other cars,
  BACKWARD and RECORDS remain untested by a human.
- The attract mode includes a credits demo (~223 s after START) that headless runs may not have reached.

## 8. Attachments

`rush2049-recomp-test-v2-run.zip`, made from `run\` after `Get-ChildItem run -Recurse -Filter *.z64` found no file (no ROM
copy was ever left behind in v2). No `--wav` recordings are inside (they were written outside `run\`). Contents: this
report; `logs\` (smoke, T1-T8 and T7_perf logs, `logs\build\` A1-A8 outputs); `self\` (a/b/c `.log` and `.err`; the
home folders h1-h3, which hold only `mods.json`); `shots\` (game-window screenshots per step); `home\` (the runtime's `mods.json`). No video.
