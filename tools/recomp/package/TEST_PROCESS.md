# TEST PROCESS (@@VERSION@@)

Do the steps in order after `INSTALL.md`. Times are wall-clock estimates. Everything below uses the Windows
paths from INSTALL.md (`C:\rush2049\@@PACKAGE@@`, a developer PowerShell there); on Linux replace
`build\host\rush2049_recomp.exe` with `build/host/rush2049_recomp` and drop the `cmd /c "..."` wrapper.
Put every log, screenshot and video under `run\` and name them after the step (`run\logs\T3_race.txt`,
`run\shots\T3_race_01.png`, ...). Fill `REPORT_TEMPLATE.md` as you go.

What the coordinator already knows (from headless runs, no pixels): the game boots to its front end
(runtime image A) in ~1 s; the presses below reach a race (runtime image B loads ~23 s after start);
two-player and a second track are reachable; the audio-options screen is reachable from the pause menu.
**What nobody has seen yet is the picture.** The most valuable output of this test is a precise
description of what is on screen at each step, with screenshots.

## Controls

| N64 | Game controller (XInput/SDL) | Keyboard (player 1) |
|---|---|---|
| analog stick | left stick | arrow keys |
| A | A (bottom) | X |
| B | X (left) | C |
| Z | left trigger | Z |
| START | Start/Menu | Enter |
| L / R | LB / RB (or right trigger) | Q / E |
| D-pad | D-pad | W / A / S / D |
| C buttons | right stick (also B = C-down, Y = C-up) | I / J / K / L |
| quit | - | Escape (or close the window) |

Controllers must be connected before starting the program (hot-plug is handled but untested). The keyboard
is always player 1; game controllers are players 1, 2, ... in connection order (with `--keyboard-alone` they
start at player 2). In Rush 2049 A is usually accelerate and B brake;
the menus react to the stick and/or the D-pad: try both and note which works.

## Capturing evidence

- Logs: every run below redirects stderr to a file (`2> run\logs\NAME.txt`). The program prints one
  `[rush] status ...` line per second and event lines (`load game`, `load ovl_a`, `load ovl_b`, first gfx task,
  RT64 setup, controller names). Keep the whole file.
- Screenshots: Win+Shift+S (Snipping Tool) or Win+PrtScn (saved in Pictures\Screenshots). At least one per
  step, more when something looks wrong.
- Video: Xbox Game Bar, Win+Alt+R to start/stop (saved in Videos\Captures). Record steps T2-T6.
- FPS: the game's frame rate = the increase of `gfx_tasks=` between two status lines one second apart
  (the N64 game targets 30 per second in races; menus may differ); the RT64 window rate is the monitor's.
  Run this on a log to print per-second deltas:
  `python -c "import re,sys;v=[int(m.group(1)) for m in re.finditer(r'gfx_tasks=(\d+)',open(sys.argv[1]).read())];print([b-a for a,b in zip(v,v[1:])])" run\logs\T3_race.txt`

## T0. Headless smoke test (1 min)

INSTALL.md A9. Record pass/fail and the `final` line.

## T1. Boot to title (3 min)

```powershell
cmd /c "build\host\rush2049_recomp.exe --rom config\rom\rush2049.recomp.z64 --home run\home 2> run\logs\T1_boot.txt"
```

Do not touch anything for 60 seconds. Note: time until the window appears; time until the first non-black
picture; what it shows (logos, title screen, "press start"); whether it animates; any flicker, wrong colours,
missing or stretched elements, garbage textures; window size/aspect. Screenshot every distinct screen.
Then close the window. In the log: the `RT64 setup:` line (graphics API), `first gfx task`, `load ovl_a`.

## T2. Attract mode (4 min)

Start as in T1 (log `T2_attract.txt`) and leave it idle for 3 minutes: arcade-style games usually cycle
title, demo race, high scores. Note every screen and whether a demo race appears (`load ovl_b` in the log
means the race code was loaded; headless runs saw no `load ovl_b` in 5 minutes without input, so a demo race
here would be news). Record a video.

## T3. Race on the default track (by hand, 6 min)

Start as in T1 (log `T3_race.txt`). Reference path the coordinator's script takes (game seconds since
start; you do not need the exact timing, only the order): START at the title (5 s), START (8 s), A, A, A
(11-17 s), START (20 s), A, A, A, A (23-32 s: the race loads), then racing. Do it by hand: at each screen
press START or A to accept the default. Describe each menu screen (its title, what is selected) and take a
screenshot. Then drive for at least 2 minutes: accelerate, steer, brake, crash into a wall, drive off a
ramp, let the timer run. Note: track and car rendering, HUD (speed, lap, time, position), opponent cars,
frame rate (see "Capturing evidence"), input lag, controller response, anything that freezes.

Optional, if T3 by hand could not reach a race: let the script drive the menus while you watch:

```powershell
cmd /c "build\host\rush2049_recomp.exe --rom config\rom\rush2049.recomp.z64 --home run\home --press-file rush\tools\recomp\scripts\race.press --seconds 70 2> run\logs\T3_script.txt"
```

(the script presses only for the first 50 s; the pad/keyboard is ignored while a script runs.)

## T4. Second track (race_track2, 5 min)

Like T3 (log `T4_track2.txt`), but on the track selection screen (reached after ~18 s in the script: the
screen after START, START, A, A, A) press RIGHT once before accepting. Note the track name shown and whether
the race loads a different-looking track. Script equivalent: `--press-file rush\tools\recomp\scripts\race_track2.press`.
The selector cycles four tracks (headless evidence): if time allows, also race RIGHT x2 (`race_track3.press`)
and LEFT x1 (`race_track4.press`) and note each track's name.

## T5. Two players (two_player, 6 min; needs a game controller for player 2)

Player 1 and player 2 each need a device. With two game controllers: connect both before starting (log
`T5_two_player.txt`); the log shows `controller -> player 1: ...` and `controller -> player 2: ...` (the
keyboard also drives player 1). With one game controller: add `--keyboard-alone` to the command so the
keyboard is player 1 and the controller is player 2 (log: `controller -> player 2: ...`). On the player-count screen (~9.5 s in the
script: the one after the first START presses) press RIGHT to choose 2 players; player 2 presses A/START
when the game asks. Race for 2 minutes. Note: split-screen layout, both views rendering, both players'
input, frame rate. Script equivalent: `two_player.press` (drives both ports itself).

## T6. Audio options (audio_options, 4 min)

Start a race as in T3 (log `T6_audio.txt`). After a few seconds of racing press START (pause), then DOWN
three times and A: the audio-options screen. Change each row (UP/DOWN change values, LEFT/RIGHT change the
row: sfx volume, music volume, music track, stereo/mono), then B to leave and resume the race. Sound is
expected to be silent in this build; report only what is displayed and whether the menus respond.
Script equivalent: `audio_options.press`.

## T7. Free play (10-20 min)

Anything the owner likes: other tracks, cars, modes (stunt, battle, practice), options. Note what works and
what does not, and any crash. This is the free-text gameplay section of the report.

## T8. Lockstep self-check (3 min; needs Python)

The deterministic test mode the decompilation uses to compare builds. Two identical scripted runs with the
headless renderer must give identical frames:

```powershell
mkdir run\self -Force | Out-Null
cmd /c "build\host\rush2049_recomp.exe --renderer null --rom config\rom\rush2049.recomp.z64 --home run\self\h1 --clock frames --frames 1800 --press-file rush\tools\recomp\scripts\race.press --oracle-log run\self\a.log 2> run\self\a.err"
cmd /c "build\host\rush2049_recomp.exe --renderer null --rom config\rom\rush2049.recomp.z64 --home run\self\h2 --clock frames --frames 1800 --press-file rush\tools\recomp\scripts\race.press --oracle-log run\self\b.log 2> run\self\b.err"
python rush\tools\recomp\oracle.py compare run\self\a.log run\self\b.log
```

Expected: the first line is `EQUIVALENT over 1800 frames (judged: all frames, 1 baseline run(s))` (each run
takes a few seconds); a second line about thread-stack contents is informational (stacks are masked). Then the same
run with the RT64 window (the picture plays at normal speed for 60 s while the script drives):

```powershell
cmd /c "build\host\rush2049_recomp.exe --renderer rt64 --rom config\rom\rush2049.recomp.z64 --home run\self\h3 --clock frames --frames 1800 --press-file rush\tools\recomp\scripts\race.press --oracle-log run\self\c.log 2> run\self\c.err"
python rush\tools\recomp\oracle.py compare run\self\a.log run\self\c.log
```

Report both first lines. The second comparison is informative: `EQUIVALENT` means RT64 does not disturb the
game's memory; `DIFFERENT at frame F` is useful data, not a failure (RT64 can write rendered frames back into
the console's memory).

## T9. Collect

Zip `run\` (logs, `self\*.log`, screenshots, videos; NOT `config\rom\` and not `rom\`) and attach it with the
filled `REPORT_TEMPLATE.md`.
