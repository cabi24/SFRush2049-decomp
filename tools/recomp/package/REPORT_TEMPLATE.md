# Rush 2049 recomp gameplay-test report (@@VERSION@@)

Fill every field; write "n/a" with a reason when a step was skipped. Paste log excerpts as text, not images.

## 1. Tested version and machine

| Field | Value |
|---|---|
| package | @@PACKAGE@@ (`VERSION.txt`: repo commit @@REPO_COMMIT@@) |
| pins built | N64Recomp @@N64RECOMP_COMMIT@@, N64ModernRuntime @@RUNTIME_COMMIT@@, RT64 (`git -C src\rt64 rev-parse HEAD`): ... |
| MANIFEST check (A4) | OK / failed: ... |
| OS + build number (`winver`) | Windows 11 ... |
| CPU, RAM | ... |
| GPU + driver version | ... |
| display resolution / refresh rate | ... |
| compiler (`clang-cl --version`), CMake, Ninja, Python | ... |
| build path used | Windows native / Linux / WSL2 / prebuilt Linux binary |
| graphics API RT64 chose (`RT64 setup:` log line) | D3D12 / Vulkan |
| controllers used | keyboard / model of each pad |

## 2. Build result per step

| Step | Result (OK / FAILED) | Duration | Error lines if failed |
|---|---|---|---|
| A1 tools | | | |
| A3 ROM SHA-1 | | | |
| A5 N64Recomp + RSPRecomp (both .exe present) | | | |
| A6 make_rom + recompile (function count; `rsp\rush_audio_ucode.cpp` written) | | | |
| A7 RT64 clone (HEAD) | | | |
| A8 configure (both `Rush 2049 host:` lines) | | | |
| A8 build | | | |
| null-renderer fallback build (only if A8 failed) | | | |

## 3. Scenarios

Mode: **wall** = the default command (no `--clock`), **frames** = with `--clock frames`. T1-T4 must have a
wall-clock result (pass or fail); add a frames row only if wall clock failed.

| Step | Mode | Pass / Partial / Fail | Symptoms (what you saw vs expected) | Sound (what you heard) | FPS (gfx_tasks/s) | Evidence files |
|---|---|---|---|---|---|---|
| T0 headless smoke (`final` line, `audio_nonzero=`) | null | | | n/a | | |
| T1 boot to title (D3D12) | wall | | | | | |
| T1 boot to title (`--gfx-api vulkan`) | wall | | | | | |
| T2 attract mode (5 min idle) | wall | | | | | |
| T3 race, default track | wall | | | | | |
| T4 second track (+ tracks 3, 4) | wall | | | | | |
| T5 two players | wall | | | | | |
| T6 audio options (each row changes the sound?) | wall | | | | | |
| T6b stunt mode | wall | | | | | |
| T7a pad / keyboard mapping (table in section 5) | wall | | | n/a | | |
| T8 lockstep self-check (a vs b; a vs c; a vs c --mask-renderer first lines) | frames | | | n/a | | |

Per screen seen (T1-T6b), one line each: screen name/title -> rendering correct? (geometry, textures, colours,
text, HUD) -> input works? -> sound?

## 4. Crashes and freezes

For each: step, what you did just before, whether the window froze or closed, and the **last 40 lines of the
log** (and the Windows Event Viewer entry for `rush2049_recomp.exe` if it crashed).

```
(paste)
```

## 5. Controller mapping (T7a)

| Physical control (pad model: ...) | N64 name printed by `--log-input` | OK / wrong / nothing |
|---|---|---|
| A / B / X / Y | | |
| LB / RB / LT / RT | | |
| Start / Back | | |
| D-pad up/down/left/right | | |
| left stick (4 directions) | | |
| right stick (4 directions) | | |
| keyboard keys (table in TEST_PROCESS) | | |

Which N64 buttons did not work or felt wrong (menus via stick vs D-pad, accelerate/brake, camera/C buttons,
second player), and what mapping would be better.

## 5b. Sound

The `audio:` log line (driver, device rate); `audio_nonzero=` at the end of T3; what plays where (title music,
menu sounds, engine, effects, music track changes, stereo vs mono); problems (crackle, gaps, too fast/slow,
pitch, delay against the picture, volume balance) and in which mode/step.

## 6. Performance

Typical and worst FPS in menus and in a race; stutter; input lag; CPU/GPU load if visible (Task Manager).

## 7. Free-text gameplay notes

What playing it was like: what works, what is broken, anything that looks different from the real N64 game
(if the owner knows it), and anything the coordinator should look at first.

## 8. Attachments

List of files in the zipped `run\` folder.
