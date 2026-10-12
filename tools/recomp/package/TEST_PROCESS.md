# TEST PROCESS (@@VERSION@@)

Do the steps in order after `INSTALL.md`. Times are wall-clock estimates. Everything below uses the Windows
paths from INSTALL.md (`C:\rush2049\@@PACKAGE@@`, a developer PowerShell there); on Linux replace
`build\host\rush2049_recomp.exe` with `build/host/rush2049_recomp` and drop the `cmd /c "..."` wrapper.
Put every log, screenshot and video under `run\` and name them after the step (`run\logs\T3_race.txt`,
`run\shots\T3_race_01.png`, ...). Fill `REPORT_TEMPLATE.md` as you go.

What is known (v1 report, 2026-10-12): with `--clock frames` (lockstep) RT64 draws the game correctly (title,
menus, four tracks, 1P and 2P races, pause/audio/video menus) at 30 frames/s, and the owner raced a lap with an
Xbox 360 pad; the **default wall-clock mode hung at boot** on a black window (13/13 runs), and there was **no
sound**. v2 fixes both (`CHANGES.md`). So this round is mainly: **does the default command now boot and play
(T1-T4 in wall clock), does it sound right (T1-T7), do attract mode, stunt mode and the pad mapping work.**

Default command for every step below: wall clock (no `--clock`). If a wall-clock step fails (black window,
`gfx_tasks=` stops growing), write down the failure with the last 40 log lines, then repeat the step with
`--clock frames` added (the v1 mode) so the rest of the step still gets tested; say in the report which mode
each result comes from.

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

Controllers must be connected before starting the program (hot-plug is handled but untested). The table is
what the program sets; T7a checks it button by button. The keyboard
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
- Sound: describe what you hear at each step (music, engine, effects, menu sounds; crackle, gaps, wrong
  speed/pitch, one side only). The status lines carry `audio_nonzero=` (non-silent samples the game produced)
  and the log has an `audio: SDL driver ..., device ... Hz` line.
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

This is the v1 failure case: **wall clock** (no `--clock`). Do not touch anything for 60 seconds. Note: time
until the window appears; time until the first non-black picture; what it shows (logos, title screen, "press
start"); whether it animates; any flicker, wrong colours, missing or stretched elements, garbage textures;
window size/aspect; **what you hear** (music from about 5 s, when the title appears). Screenshot every distinct
screen. Then close the window. In the log: the `RT64 setup:` line (graphics API), `first gfx task`,
`load ovl_a`, `busy-wait at 0x800FD500: yielding` (the v2 fix at work; expected), `audio:` and
`first non-silent audio buffer`, and whether `gfx_tasks=` keeps growing (~30/s). Repeat once with
`--gfx-api vulkan` (log `T1_boot_vulkan.txt`).

## T2. Attract mode (4 min)

Start as in T1 (wall clock, log `T2_attract.txt`) and leave it idle for **5 minutes** without touching
anything: arcade-style games usually cycle title, demo race, high scores, credits. Note every screen in order
with its time, whether it loops, and whether a demo race appears (`load ovl_b` in the log means the race code
was loaded; headless runs saw no `load ovl_b` in 5 minutes without input, so a demo race here would be news).
Note the music over the 5 minutes. Record a video (it was never run in v1: the window was black).

## T3. Race on the default track (by hand, 6 min)

Start as in T1, wall clock (log `T3_race.txt`). Reference path the coordinator's script takes (game seconds since
start; you do not need the exact timing, only the order): START at the title (5 s), START (8 s), A, A, A
(11-17 s), START (20 s), A, A, A, A (23-32 s: the race loads), then racing. Do it by hand: at each screen
press START or A to accept the default. Describe each menu screen (its title, what is selected) and take a
screenshot. Then drive for at least 2 minutes: accelerate, steer, brake, crash into a wall, drive off a
ramp, let the timer run. Note: track and car rendering, HUD (speed, lap, time, position), opponent cars,
frame rate (see "Capturing evidence"), input lag, controller response, anything that freezes; sound: engine
pitch following the speed, music, crash/scrape effects, countdown voice/beeps, whether the sound stays in step
with the picture over the 2 minutes (no growing delay, no speed-up/slow-down).

Optional, if T3 by hand could not reach a race: let the script drive the menus while you watch:

```powershell
cmd /c "build\host\rush2049_recomp.exe --rom config\rom\rush2049.recomp.z64 --home run\home --press-file rush\tools\recomp\scripts\race.press --seconds 70 2> run\logs\T3_script.txt"
```

(the script presses only for the first 50 s; the pad/keyboard is ignored while a script runs.)

## T4. Second track (race_track2, 5 min)

Like T3, wall clock (log `T4_track2.txt`), but on the track selection screen (reached after ~18 s in the script: the
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
three times and A: the audio-options screen. **UP/DOWN move between the rows** (EFFECTS VOLUME, MUSIC VOLUME,
MUSIC TRACK, SPEAKERS) and **LEFT/RIGHT change the selected value** (v1 report). For each row change the value
and listen: effects volume 10 -> 0 should silence the engine/effects, music volume to 0 the music, MUSIC TRACK
should change the tune, SPEAKERS STEREO -> MONO should make left and right identical (headphones help). Then B
to leave and START to resume the race. Script equivalent: `audio_options.press`.

## T6b. Stunt mode (stunt, 5 min)

Wall clock (log `T6b_stunt.txt`). From the title: START, START, A (START GAME, 1 player), then on **SELECT MODE**
(SINGLE RACE, PRACTICE, GHOST RACE, CIRCUIT, STUNT) press DOWN four times to **STUNT** and A, then accept the
following screens (A / START) until it loads (`load ovl_b` in the log). Do some jumps and tricks for 2 minutes
(the stunt controls are in the pause menu's CONTROLS page: note what it says). Note the arena, the trick/score
display, sound, frame rate. Headless evidence: STUNT loads race data no track uses (stream 0x812BE0), i.e. a
stunt arena. Script equivalent (watch only): `--press-file rush\tools\recomp\scripts\stunt.press --seconds 70`.
(In v1 the scripted attempt ended in OPTIONS/VIDEO: DOWN on START GAME moves through that menu's rows, so press
DOWN only on SELECT MODE.)

## T7a. Pad and keyboard mapping check (5 min)

Start with the Xbox 360 pad (and any other pad) connected, wall clock, with `--log-input`:

```powershell
cmd /c "build\host\rush2049_recomp.exe --rom config\rom\rush2049.recomp.z64 --home run\home --log-input 2> run\logs\T7a_pad.txt"
```

The log names each pad (`controller -> player N: <name>`) and prints a line for every change:
`input player 1: A+START stick x=+0.00 y=+0.00`. On the title screen (nothing happens there on most buttons)
press, one at a time, every pad control in the Controls table: A, B, X, Y, LB, RB, LT, RT, Start, Back, each
D-pad direction, the left stick full left/right/up/down, the right stick in four directions, and the stick
buttons. Then the same for the keyboard keys of the table. Report a table: physical control -> the N64 name the
log printed (or "nothing"), and any control that felt wrong in the race (e.g. accelerate on A vs a trigger,
steering dead zone). Then race one lap with the pad and note whether you would change the mapping (the owner's
v1 note: "the button mapping made sense").

## T7. Free play (10-20 min)

Anything the owner likes, wall clock: other tracks, cars, modes (stunt, battle, practice, circuit), options.
Note what works and what does not, any crash, and **the sound** throughout. This is the free-text gameplay
section of the report.

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
python rush\tools\recomp\oracle.py compare --mask-renderer run\self\a.log run\self\c.log
```

Report the first lines of all three comparisons. Expected (coordinator, RT64 on a software GPU): plain
`DIFFERENT at frame 2: 71 rdram page(s)` (RT64 writes the rendered frame and the depth buffer back into the
console's memory: since v2 these are known, the z-buffer at 0x8017AEC0 and the framebuffers at 0x807BB3C0 /
0x807DDA00), and with `--mask-renderer`: `EQUIVALENT except renderer-written pages over 1800 frames ...`.
Anything else (another first frame, other pages, `DIFFERENT` with the mask) is the interesting result.

## T9. Collect

Zip `run\` (logs, `self\*.log`, screenshots, videos) **without any `*.z64` file and without `config\rom\` or
`rom\`**. Since v2 the program deletes its ROM copy from each `--home` folder once the game has loaded it, but a
run that crashed early can leave one behind, so check first and exclude them:

```powershell
Get-ChildItem run -Recurse -Filter *.z64 | Remove-Item -WhatIf    # lists any ROM copy; run again without -WhatIf to delete
Compress-Archive -Path run\* -DestinationPath rush2049-recomp-test-@@VERSION@@-run.zip
```

Also leave out any `--wav` recording (the game's audio). Attach the zip with the filled `REPORT_TEMPLATE.md`.
