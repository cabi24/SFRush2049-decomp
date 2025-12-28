# Arcade Function Mapping (Rush The Rock)

This document catalogs arcade-side functions to help match N64 decomp `func_*` names to known arcade equivalents.

## 1. Game State Machine

### State Diagram

```
ATTRACT
  -> TRKSEL -> CARSEL -> PREPLAY -> COUNTDOWN -> PREPLAY2 -> PLAYGAME
                                             |                  |
                                             |                  v
                                             |               ENDGAME -> GAMEOVER -> HISCORE
                                             |                                   |
                                             +-----------------------------------+
                                                            back to ATTRACT
```

### Main Loop

- `void game_init(void)`
  - Initializes options, difficulty, attract audio settings, HUD, and globals.
  - Source: `reference/repos/rushtherock/game/game.c`
- `void game(void)`
  - Main frame loop. Handles input, debug, volume adjustment, and the `gstate` switch.
  - Source: `reference/repos/rushtherock/game/game.c`
- `BOOL preplay(BOOL useHud, BOOL canAbort)`
  - Loads track, initializes cars/drones, HUD, audio, checkpoints, and environment.
  - Source: `reference/repos/rushtherock/game/game.c`
- `void playgame(void)`
  - Core gameplay tick: physics update, drones, checkpoints, crashes, visuals, targets.
  - Source: `reference/repos/rushtherock/game/game.c`

### State Handlers

| Function | GState | Purpose | Signature | Source |
|----------|--------|---------|-----------|--------|
| `attract()` | `ATTRACT` | Attract/demo flow and high score entry gating | `attract(void)` | `reference/repos/rushtherock/game/attract.c` |
| `TrackSel()` | `TRKSEL` | Track select UI + transition to car select | `TrackSel(void)` | `reference/repos/rushtherock/game/select.c` |
| `CarSel()` | `CARSEL` | Car select UI + transition to preplay | `CarSel(void)` | `reference/repos/rushtherock/game/select.c` |
| `preplay()` | `PREPLAY` | One-time per-race setup | `BOOL preplay(BOOL useHud, BOOL canAbort)` | `reference/repos/rushtherock/game/game.c` |
| `CountDown()` | `COUNTDOWN` | Countdown timing + pre-start car setup | `CountDown(void)` | `reference/repos/rushtherock/game/*` (called in `game.c`) |
| `PLAYGAME` loop | `PLAYGAME` | Active race logic | `void playgame(void)` | `reference/repos/rushtherock/game/game.c` |
| `do_endgame()` | `ENDGAME` | Stop model, stop engine sound, set GAMEOVER timer | `static void do_endgame(void)` | `reference/repos/rushtherock/game/game.c` |
| `do_gameover()` | `GAMEOVER` | Waits for timeout or abort, transitions to HISCORE | `static void do_gameover(void)` | `reference/repos/rushtherock/game/game.c` |
| `HISCORE` block | `HISCORE` | Inserts score and resets to ATTRACT | in-line in `game()` | `reference/repos/rushtherock/game/game.c` |

Other helpers used in state transitions:
- `void demo_endgame(void)` and `void CleanupAfterGame(void)` in `reference/repos/rushtherock/game/game.c`.
- `void do_car_setup(void)` in `reference/repos/rushtherock/game/game.c`.

## 2. Physics System Functions

### Car/Model Update

- `void Init_MDrive(S16 mode, S16 drone_index)`
  - Initializes car model state, camera, and resurrection data.
  - Source: `reference/repos/rushtherock/game/mdrive.c`
- `void Update_MDrive(void)`
  - Reads controls, updates models, syncs `MODELDAT` into game task state.
  - Source: `reference/repos/rushtherock/game/mdrive.c`
- `void End_MDrive(S16 drone_index)`
  - Shuts down model for a car (used at endgame / crash).
  - Source: `reference/repos/rushtherock/game/mdrive.c`
- `void CheckCrash(S16 drone_index)`
  - Detects crash state and manages crash timers/respawn.
  - Source: `reference/repos/rushtherock/game/resurrect.c`

### Tire Model (tires.c)

- `void calctireuv(F32 v[3], F32 w[3], F32 r[3], F32 steer, struct uvect *caruvs, struct uvect *roaduvs, struct uvect *tireuvs, F32 tirev[3])`
  - Builds tire coordinate basis and computes tire velocity in wheel coords.
- `void dotireforce(MODELDAT *m, F32 tirev[3], F32 ottirev[3], struct uvect *tireuvs, struct tiredes *tire, F32 torque, F32 forcevec[3], F32 suscomp, F32 otsuscomp, F32 springrate, F32 arspringrate, F32 cdamping, F32 rdamping, int poortract, F32 airfact)`
  - Computes suspension force + friction circle lateral/traction forces for a tire.
- `void frictioncircle(MODELDAT *m, F32 tirev[3], F32 normalforce, F32 torque, struct tiredes *tire, F32 *sfp, F32 *trp)`
  - Combines lateral + longitudinal forces within a friction circle; handles slip.
- `F32 calcalpha(F32 tirev[3])`
  - Computes slip angle proxy from tire velocity components.
- `void make_tire_road_uvs(struct uvect *caruvs, F32 steer, struct uvect *roaduvs, struct uvect *truvs)`
  - Generates wheel unit vectors aligned to road for contact calculations.

### Physics Data Tables (cars.c)

- `reference/repos/rushtherock/game/cars.c` primarily defines car parameter tables:
  - Torque curves, gear ratios, suspension constants, tire setup, collision sizes.
  - These data tables drive the physics model in `MODELDAT`.

### Collision (collision.c)

- `void init_collision(MODELDAT *m)`
  - Initializes per-car collision forces and last terrain pointers.
- `void collision(MODELDAT *m)`
  - Broad-phase sphere test, then corner-in-body tests to resolve car-car impacts.
- `BOOL PointInBody(MODELDAT *m, F32 pt[3])`
  - Bounding box test in car body space.
- `void setFBCollisionForce(MODELDAT *m, MODELDAT *m1, MODELDAT *m2, F32 dir[3], F32 pos[3])`
  - Applies force-based collision response (front/back emphasis).
- `void setCollisionDamage(MODELDAT *m)`
  - Calculates damage from collision forces.
- `void setCenterForce(MODELDAT *m, MODELDAT *m1, MODELDAT *m2, F32 dir[3], F32 pos[3])`
  - Applies impact to center-of-mass forces.
- `F32 collForceNormal(MODELDAT *m, U32 coll_alg, F32 din, F32 vin)`
  - Collision normal force calculation by algorithm type.
- `void CollForceMineIn(...)` / `void CollForceOtherIn(...)`
  - Handles which car “owns” the collision response.

### Physics Core (drivsym.c)

- `void syminit(MODELDAT *m)`
  - Initialize model state, road, positions, and communication.
- `void sym(MODELDAT *m)`
  - Main physics tick: controls, drivetrain, forces, steering feedback, and comms.
- `void regular(MODELDAT *m)`
  - Runs road sampling, forces, torques, and integration steps.
- `void antispin(MODELDAT *m)`
  - Computes anti-spin center moment for stability.
- `void forces(MODELDAT *m)` / `void forces1(MODELDAT *m)` / `void forces2(MODELDAT *m)`
  - Tire forces, drag, gravity, and body forces aggregation.
- `void torques(MODELDAT *m)` / `void accelerations(MODELDAT *m)` / `void velocities(MODELDAT *m)` / `void positions(MODELDAT *m)`
  - Torque and integration pipeline for rigid-body updates.
- `void checkok(MODELDAT *m)`
  - Crash/thump detection, force thresholds, and safety clamps.

### Catchup / Rubber-Band (model.c)

- `void set_catchup(void)`
  - Adjusts catchup multiplier based on player distance to leader.
- `void no_catchup(void)`
  - Disables catchup (e.g., tourney mode).

## 3. AI / Drone System Functions (drones.c)

- `void InitDrones(void)`
  - Initializes maxpath data and per-slot drone scaling.
- `void DoDrones(void)`
  - Main AI loop: catchup scaling, maxpath updates, assigns drone behavior.
- `void EndDrones(void)`
  - Placeholder for cleanup.
- `void assign_drones(void)`
  - Assigns drone personalities, difficulty, and time-boost based on race state.
- `F32 linear_interp(F32 in_bound1, F32 in_bound2, F32 out_bound1, F32 out_bound2, F32 input)`
  - Utility for mapping values across ranges (used by AI tuning).

### Maxpath / Route-Following (maxpath.c)

Core AI pathing and hint system used by drones:

- `void InitMaxPath(S32 record)`
  - Initializes path tables and optional recording.
- `void MaxPath(MODELDAT *m)`
  - Updates maxpath control for a car (target point, steering, speed).
- `void assign_default_paths(S16 num_drones, S16 *drones)`
  - Assigns default paths for active drones.
- `void place_cars_in_order(S16 *cars_in_order, S16 *humans, S16 *drones, S16 *num_humans, S16 *num_drones)`
  - Determines race order and splits human vs drone lists.
- `void MP_TargetSpeed(MODELDAT *m, MPCTL *cp)`
  - Computes target speed from path metadata and hints.
- `void MP_TargetSteerPos(MODELDAT *m, MPCTL *cp)`
  - Computes target steering position toward next path point.
- `void AdjustSpeed(MODELDAT *m, F32 tspd)`
  - Applies speed control (brake/throttle) to match target speed.
- `void AdjustSteer(MODELDAT *m, F32 *pos)`
  - Applies steering control to match target position.
- `void avoid_areas(S16 drone_index)`
  - Avoidance logic for restricted or blocked areas.
- `void link_maxpath_and_pathpoints(void)`
  - Cross-links maxpath entries with checkpoint/path center data.

## 4. Rendering Functions (visuals.c)

### Core Rendering/Visual Update

- `void InitVisuals(BOOL for_game)`
  - Initializes dynamic objects, textures, and visual effect pools.
- `void UpdateVisuals(S16 slot)`
  - Per-car visual update: effects, body parts, skids, shadows, smoke.
- `void InitDynamicObjs(BOOL for_game)`
  - Preloads object handles for cars/track/environment.
- `void CreateCar(S16 slot, BOOL for_game)`
  - Allocates car object hierarchy for a slot.
- `void SetupCar(S16 slot, S16 type, BOOL for_game)`
  - Applies body/texture variants and damage state.
- `void StartEnvironment(void)` / `void UpdateEnvirons(void)` / `void RemoveEnvirons(void)`
  - Manage environment visuals (targets, track overlays, etc.).

### Visual Effects (selected)

- `void StartSparks(S16 slot)`
  - Spawns spark effects on a car.
- `void StartFrame(S16 slot)` / `void ShowHood(S16 slot, BOOL show)`
  - Controls hood/frame animation and visibility.
- `void InitSkids(void)`
  - Initializes skid mark pool.

## 5. Audio Functions

### Global Sound Control (sounds.c)

- `SOUND(S32 sound_command)`
  - Sends sound commands (play/stop SFX, mode changes).
- `void sndUpdate(void)`
  - Processes queued sound commands each tick.
- `void music(S16 mode, U8 radio_station)`
  - Controls music/radio playback modes.
- `void sndListenerUpdate(S32 x, S32 y, U16 velocity, U16 vel_angle, U16 facing_angle)`
  - Updates listener position for 3D sound.
- `sndQStore(S16 *parms, S32 cnt)` / `sndCmds(S16 *parms, S32 cnt)`
  - Enqueue and dispatch low-level sound commands.
- `sndStart*` / `sndChange*`
  - Start or modify static/doppler/positional sound effects.
- `sndStartEngine*` / `sndUpdate*Engine` / `sndStopEngine`
  - Engine sound control (auto/manual variants).
- `sndRoadNoise` / `sndGravelNoise` / `sndWindNoise` / `sndSplashNoise`
  - Surface and environment noise loops.
- `sndDoSkid`
  - Tire skid audio.

### Car + Engine Audio (carsnd.c)

- `void InitCarSnds(S16 drone_index)`
  - Initializes per-car sound state/handles.
- `void DoCarSounds(S16 update_car_sounds, S8 skids_only)`
  - Main car sound update loop.
- `void StartEngineSound(void)` / `void DoEngineSound(void)` / `void StopEngineSound(void)`
  - Engine sound lifecycle.
- `void StartRadio(U8 radio_station)` / `void StopRadio(U8 radio_station)`
  - Radio station control.
- `void DoTireSqueals(S16 drone_index)`
  - Tire squeal/skid audio.
- `void init_bump_sounds(void)` / `void do_bump_sounds(S16 update_car_sounds)`
  - Impact/bump audio from collision forces.
- `void target_sound(Target *t, S16 slot)`
  - Trigger sound effects for target interactions.
- `void init_reverb(void)` / `void handle_reverb(void)`
  - Per-environment reverb setup and updates.

## 6. Key Data Structures

### Vehicle + Physics

- `MODELDAT` (`reference/repos/rushtherock/game/modeldat.h`)
  - Full physics state: forces, velocities, tire data, drivetrain, collision, inputs.
  - Holds per-car simulation state used by `Update_MDrive` and tire model.
- `CAR_DATA` (`reference/repos/rushtherock/game/modeldat.h`)
  - Game-task view of car state for rendering, UI, and logic.
  - Includes dead-reckoned position, visual overlays, score, lap, mpath info.
- `tiredes` (`reference/repos/rushtherock/game/tiredes.h`)
  - Tire parameters and state (stiffness, friction, slip flags, angvel).
- `MPCTL` (`reference/repos/rushtherock/game/modeldat.h`)
  - Maxpath control data for drone routing and target speed.

### Rendering

- `Visual`, `Skid`, `NewSkid` (`reference/repos/rushtherock/game/visuals.h`)
  - Visual effect nodes, skid mark pooling, and skid line state.

## 7. Global Variables Reference (globals.h)

- `GState gstate` — current game state.
- `MODELDAT model[NCARS]` — per-car physics model state.
- `CAR_DATA game_car[NCARS]` — per-car game task data (visuals, score, etc.).
- `S8 num_active_cars` — number of active cars in race.
- `U8 gThisNode` — slot index for the local player.
- `S16 trackno` — current track number.
- `S8 difficulty[MAX_TRACKS]`, `S8 track_len[MAX_TRACKS]` — per-track difficulty/lap settings.
- `BOOL attract_effects`, `BOOL attract_music` — attract mode audio toggles.
- `S16 demo_game` — demo run state.
- `S16 gSWforce`, `S16 gSWpos` — steering force/position.
- `F32 gCamPos[3]`, `F32 gCamUvs[3][3]` — camera position + orientation.
- `U32 edges`, `U32 levels` (+ debug variants) — input edge and level state.

## Likely Shared With N64 Version

These core systems are likely to have N64 analogs due to fundamental behavior:
- `Update_MDrive`, `Init_MDrive`, `End_MDrive` (physics lifecycle)
- `calctireuv`, `dotireforce`, `frictioncircle`, `calcalpha` (tire model)
- `InitDrones`, `DoDrones`, `assign_drones` (AI/catchup logic)
- `InitVisuals`, `UpdateVisuals`, `CreateCar`, `SetupCar` (rendering + car assembly)
- `SOUND`, `music`, `StartRadio`, `StopRadio`, `DoEngineSound` (audio control)
- `InitCPS`, `CheckCPs`, `PassedCP` (checkpoint + lap logic)
- `CheckCrash`, `resurrect_car` (crash/respawn lifecycle)
- `InitHUD`, `ShowHUD`, `EnableRadioHUD` (HUD system)
- `UpdateCam`, `SetMCamera`, `SelectCam` (camera system)

## 8. Checkpoints, Laps, and Race Progress (checkpoint.c)

- `void init_cp_data(void)`
  - Initializes checkpoint tables and per-track data.
- `void InitCPS(void)`
  - Resets checkpoint system at race start.
- `void init_cp_time(void)`
  - Resets checkpoint timing and first-place timers.
- `U32 get_next_checkpoint(U32 cur_checkpoint)`
  - Calculates next checkpoint index (wraps to loop).
- `S16 get_next_center(S16 t_index, S16 t_direction)`
  - Gets next path center point for direction of travel.
- `void CheckCPs(void)`
  - Per-frame checkpoint crossing detection for all cars.
- `void PassedCP(MODELDAT *m, S8 flag)`
  - Handles checkpoint crossing for a single car.
- `void FPutCheckpoints(S16 track)`
  - Places checkpoint objects for current track.
- `void update_lap_counter(S16 mode, S16 laps_left)`
  - HUD update for lap counter.

## 9. Crash + Resurrection (resurrect.c)

- `void resurrect_car(MODELDAT *m)`
  - Places car on a valid respawn location with orientation.
- `void set_resurrection_data(S16 mode, MODELDAT *m)`
  - Saves last known state for respawn calculations.
- `void CheckCrash(S16 drone_index)`
  - Detects crash events and triggers respawn sequence.
- `void check_if_finished_resurrecting(void)`
  - Tracks cars in respawn sequence and restores control.
- `void update_resurrecting_car(MODELDAT *m)`
  - Per-frame updates while car is reappearing.
- `void detect_drone_abort(void)`
  - AI-specific abort/respawn detection.
- `BOOL are_we_dead_or_dying(S16 node)`
  - Reports death/resurrection state.

## 10. HUD / UI (hud.c)

- `void InitHUD(void)`
  - Initializes HUD blit lists and state.
- `void ShowHUD(void)`
  - Enables HUD rendering for gameplay.
- `void RemoveHUD(void)`
  - Hides HUD (used for menus/attract).
- `void EnableRadioHUD(void)`
  - Enables radio station indicator on HUD.
- `void ShowCheckpointTime(S32 delta)`
  - Shows checkpoint time bonus text.
- `void ShowFinishFlag(F32 pos[3], BOOL hide)`
  - Toggles finish flag indicator.
- `void update_lap_counter(S16 mode, S16 laps_left)`
  - Updates lap counter display (also in checkpoint.c).

## 11. Camera + View Control (camera.c)

- `void init_view(void)` / `void init_view3(void)`
  - Initializes camera modes and default view state.
- `void SelectCam(void)`
  - Selects active camera mode.
- `void UpdateCam(void)`
  - Per-frame camera update (tracking, smoothing).
- `void SetMCamera(S16 mode)`
  - Applies camera settings for the current mode.
- `void CheckCameraView(void)`
  - Updates camera when view inputs change.
- `void update_car_object(F32 pos[3], F32 uvs[3][3])`
  - Syncs render objects with car physics state.
- `void DeathCam(F32 pos[3], F32 uvs[3][3])`
  - Special camera behavior during crash/death.
- `void circle_camera_around_car(F32 pos[3], F32 uvs[3][3])`
  - Orbit camera for replay/attract.
- `void init_camera_on_track(void)`
  - Sets initial camera placement on current track.
- `void init_maxpath_cam(void)` / `void maxpath_cam(S16 mode, F32 pos[3], F32 uvs[3][3])`
  - Maxpath-driven camera for flythrough/debug.

## 12. Controls / Input (controls.c)

- `void controls(MODELDAT *m)`
  - Reads inputs and applies steering/throttle/brake to model.
- `void initcontrols(MODELDAT *m)`
  - Initializes control state at race start or respawn.

## 13. Track / Road / World (road.c, world.c)

- `void init_road(MODELDAT *m)` / `void road(MODELDAT *m)`
  - Road surface sampling and per-tire contact setup.
- `void uvinterp(...)` / `void vecinterp(...)`
  - Interpolate orientation vectors and positions between road segments.
- `void LoadWorld(void)`
  - Loads world/track geometry data (world header + objects).

## 14. Audio System Cross-Reference (arcade sounds.c / carsnd.c)

Likely arcade equivalents for N64 audio control:

- `SOUND(S32 sound_command)` (arcade `sounds.c`)
  - Main sound command dispatch; likely maps to N64 `func_800B37E8` (`src/game/audio.c`).
- `sndStart/Stop*`, `sndChange*` (arcade `sounds.c`)
  - Low-level sound start/stop/param controls; likely map to N64 `sound_play_*`, `sound_stop`, and engine channel updates in `src/game/sound.c`.
- `StartRadio`, `StopRadio` (arcade `carsnd.c`)
  - Music/radio toggles; likely map to N64 `music_play`, `music_stop` in `src/game/sound.c`.
- `StartEngineSound`, `DoEngineSound`, `StopEngineSound` (arcade `carsnd.c`)
  - Engine sound lifecycle; likely maps to N64 engine channel updates in `src/game/sound.c`.

## 15. N64 Cross-References (func_* -> arcade equivalents)

These mappings come from comments and module intent in the N64 tree and should be treated as likely matches.

| N64 Function | N64 Context | Arcade Equivalent | Notes |
|--------------|-------------|-------------------|-------|
| `func_800FD464` | `src/game/game.c` main loop | `game()` | Top-level per-frame state dispatch. |
| `func_800DB81C` | `src/game/state.c` attract handler | `attract()` | Attract/demo state logic. |
| `func_800CA3B4` | `src/game/state.c` playgame handler | `playgame()` | Main race loop for active gameplay. |
| `func_800FBC30` | `src/game/countdown.c` | `CountDown()` | Countdown timer and transition. |
| `func_800FBF88` | `src/game/state.c` / `src/game/game.c` | `EnterHighScore()` / HISCORE state | High score entry/transition. |
| `func_800B0868` | `src/game/physics.c` | `Update_MDrive()` + drivsym loop | Per-frame physics update. |
| `func_800B811C` | `src/game/physics.c` | `UpdateVisuals()` / effects update | Particle/visual emitter updates. |
| `func_800A04C4` | `src/game/camera.c` | `DrawBlits()` + ZOID render path | Main scene render. |
| `drone_init()` | `src/game/drone.c` | `InitDrones()` | Initialize drone AI and maxpath. |
| `drone_update_all()` | `src/game/drone.c` | `DoDrones()` | Per-frame AI update. |
| `maxpath_init()` | `src/game/maxpath.c` | `InitMaxPath()` | Load/init maxpath tables. |
| `maxpath_update()` | `src/game/maxpath.c` | `MaxPath()` | Path following and target updates. |
| `tire_constants()` | `src/game/tire.c` | `tire_constants()` | Same math (arcade `initiali.c`). |
| `tire_friction_circle()` | `src/game/tire.c` | `frictioncircle()` | Friction circle forces. |
| `tire_calc_alpha()` | `src/game/tire.c` | `calcalpha()` | Slip angle proxy. |
| `InitCPS()` | `src/game/checkpoint.c` | `InitCPS()` | Race checkpoint reset. |
| `CheckCPs()` | `src/game/checkpoint.c` | `CheckCPs()` | Per-frame checkpoint test. |
| `set_resurrection_data()` | `src/game/resurrect.c` | `set_resurrection_data()` | Save respawn data. |
| `CheckCrash()` | `src/game/resurrect.c` | `CheckCrash()` | Crash detection/respawn. |
| `hud_init()` | `src/game/hud.c` | `InitHUD()` | HUD setup. |
| `hud_update()` | `src/game/hud.c` | HUD animation functions (`Animate*`) | Speed/lap/timer updates. |
| `func_800A0BAC` | `src/game/camera.c` | `UpdateCam()` | Camera state update. |
| `func_800A0F74` | `src/game/camera.c` | `init_view()` / `init_view3()` | Camera initialization. |
| `func_800A0FDC` | `src/game/camera.c` | `SelectCam()` / `SetMCamera()` | Camera mode switching. |
| `controls_read_pad()` | `src/game/input.c` | `controls()` | Input sampling and steering/brake/throttle. |
| `controls_apply_to_physics()` | `src/game/input.c` | `controls()` / `initcontrols()` | Applies inputs to model state. |
| `func_800B37E8` | `src/game/audio.c` | `SOUND()` | Main audio dispatcher. |
| `func_800B358C` | `src/game/audio.c` | `sndKill*` / stop functions | Stop sound by channel/handle. |
| `sound_play()` / `sound_stop()` | `src/game/sound.c` | `sndStart*` / `sndStop*` | SFX start/stop calls. |
| `music_play()` / `music_stop()` | `src/game/sound.c` | `music()` / `StartRadio` / `StopRadio` | Music/radio control. |
| `effects_update()` | `src/game/effects.c` | `UpdateVisuals()` | Visual effects update (smoke/sparks/skids). |
| `particles_update()` | `src/game/particles.c` | `UpdateVisuals()` | Particle system tick. |

## 16. N64 Rename Candidates (by module)

These are candidate renames inferred from comments and module intent; confirm with behavior before renaming.

### Audio

- `src/game/audio.c`: `func_800B37E8` -> `sound_control` (arcade `SOUND`)
- `src/game/audio.c`: `func_800B358C` -> `sound_stop`
- `src/game/audio.c`: `func_8008A710` -> `audio_task_sync`

### Camera / Rendering

- `src/game/camera.c`: `func_800A04C4` -> `render_scene`
- `src/game/camera.c`: `func_800A0BAC` -> `camera_update`
- `src/game/camera.c`: `func_800A0F74` -> `camera_init`
- `src/game/camera.c`: `func_800A0FDC` -> `camera_mode_switch`
- `src/game/render.c`: `func_80099BFC` -> `render_object`
- `src/game/render.c`: `func_80087A08` -> `render_large`
- `src/game/render.c`: `func_800B65B4` -> `render_helper`
- `src/game/display.c`: `func_800015F0` -> `display_update`
- `src/game/display.c`: `func_80001B44` -> `viewport_setup`
- `src/game/display.c`: `func_80001D60` -> `display_process`
- `src/game/display.c`: `func_80001DFC` -> `get_tv_offset`
- `src/game/display.c`: `func_80001E58` -> `display_set_priority`
- `src/game/display.c`: `func_80001E84` -> `get_viewport_pos`
- `src/game/display.c`: `func_80001ECC` -> `get_viewport_offset`
- `src/game/display.c`: `func_80001F2C` -> `update_viewport`
- `src/game/display.c` (external): `func_800A7508` -> `viewport_scale`

### Physics / Effects

- `src/game/physics.c`: `func_800B0868` -> `PhysicsObjectList_Update`
- `src/game/physics.c`: `func_800B811C` -> `Effects_UpdateEmitters`

### Game / State

- `src/game/game.c`: `func_800FD464` -> `game_loop`
- `src/game/state.c`: `func_800DB81C` -> `attract_handler`
- `src/game/state.c`: `func_800FBC30` -> `countdown_handler`
- `src/game/game.c`: `func_800F43D4` -> `lod_distance_calc`
- `src/game/game.c`: `func_800FD8DC` -> `rand_int`
- `src/game/game.c`: `func_800FD9F8` -> `rand_float`
- `src/game/game.c`: `func_800FDF94` -> `rand_range`
- `src/game/game.c`: `func_800FE08C` -> `timer_start`
- `src/game/game.c`: `func_800FE4BC` -> `timer_stop`
- `src/game/game.c`: `func_800FE520` -> `timer_reset`
- `src/game/game.c`: `func_800FE5B0` -> `timer_get_elapsed_cs`
- `src/game/game.c`: `func_800FE7A4` -> `timer_pause`
- `src/game/game.c`: `func_800FE848` -> `timer_resume`
- `src/game/game.c`: `func_800FE924` -> `timer_lap`
- `src/game/game.c`: `func_800FEA08` -> `race_timer_update`
- `src/game/game.c`: `func_800FECA4` -> `countdown_display`
- `src/game/game.c`: `func_800FEE04` -> `split_time_display`
- `src/game/game.c`: `func_800FF298` -> `best_lap_check`
- `src/game/game.c`: `func_800FF724` -> `record_save`
- `src/game/game.c`: `func_800FFDF8` -> `hiscore_entry`
- `src/game/game.c`: `func_80100564` -> `leaderboard_display`
- `src/game/game.c`: `func_80100B8C` -> `score_calculate`
- `src/game/game.c`: `func_80100E60` -> `stunt_score`
- `src/game/game.c`: `func_8010190C` -> `combo_multiplier`
- `src/game/game.c`: `func_80101D8C` -> `trick_detect`
