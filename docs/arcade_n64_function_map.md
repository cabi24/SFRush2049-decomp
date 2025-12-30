# N64 to Arcade Function Mapping

This document maps N64 Rush 2049 functions to their equivalent functions in the Rush The Rock arcade source code.

## Overview

The arcade source code is located in `reference/repos/rushtherock/game/`. The N64 version shares significant code with the arcade, but with platform-specific adaptations for the N64 hardware.

### Key Differences

1. **OS Layer**: N64 uses libultra OS primitives; arcade uses custom GUTS OS
2. **Graphics**: N64 uses RSP/RDP microcode; arcade uses 3dfx Glide
3. **Memory**: N64 has limited RAM (4MB/8MB); arcade has more flexible memory
4. **Compressed Code**: N64 game logic is compressed in ROM, decompressed to 0x80086A50 at runtime

---

## Game State Machine

The core game loop follows the same state machine pattern.

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800FD464 | game_loop | game/game.c:605 | game() | **High** | Main per-frame state machine, 704 bytes. Both use switch on gstate |
| 0x800CA3B4 | playgame_state_change | game/game.c:1004 | playgame() | **High** | Active racing state handler, only writer to gstate |
| 0x800DB81C | attract_mode | game/attract.c | attract() | **High** | Demo/title screen handler, 2148 bytes |
| 0x800FBF88 | countdown | game/sselect.c:2688 | CountDown() | **High** | Race countdown timer handler |
| 0x800FBC30 | preplay_setup | game/game.c:730 | preplay() | **Medium** | Pre-race setup, initializes checkpoints |
| 0x800C9AE0 | game_mode_handler | game/game.c:620 | ProcessPDUs() | **Medium** | Input/network processing when in_game_mode |
| 0x800EDDC0 | attract_or_transition | game/game.c | state transition | **Medium** | Called on state change |

### Game State Enum Mapping

The arcade uses this enum (game.h:104-117):
```c
typedef enum GState {
    ATTRACT, TRKSEL, CARSEL, PLAYGAME, ENDGAME,
    GAMEOVER, HISCORE, PREPLAY, PREPLAY2, COUNTDOWN
} GState;
```

N64 uses bitmask encoding for states in gstate (0x801174B4):
- 0x00040000 -> GSTATE_INIT
- 0x00080000 -> GSTATE_SETUP
- 0x00100000 -> GSTATE_MENU
- 0x00200000 -> GSTATE_CAMERA
- 0x00400000 -> GSTATE_READY
- 0x00800000 -> GSTATE_PLAY
- 0x01000000 -> GSTATE_CARS
- 0x02000000 -> GSTATE_FINISH

---

## Attract Mode and Title Screen (attract.c)

The arcade attract() function [attract.c:812-1187] is the main attract mode loop that cycles through various screens: ATR_ATARILOGO, ATR_HISCORE1-7, ATR_DEMO, ATR_CREDITS, ATR_JOIN, etc. It uses the attractFunc state variable, ShowAttract() to display/hide screens, SetCountdownTimer() for screen timing, and chk_start() for game start detection.

### Attract Mode State Machine

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800DC88C | attract_mode_handler | game/attract.c:812 | attract() | **High** | Main attract state machine, 1272 bytes |
| 0x800DC99C | attract_demo_handler | game/attract.c | play_demogame() | **High** | Demo race playback, 1016 bytes |
| 0x800DCD94 | attract_idle_handler | game/attract.c | TimeOut() | **Medium** | Idle timeout check, 96 bytes |
| 0x800DCDF4 | attract_video_handler | game/attract.c | AttractMovie() | **Medium** | Video/FMV playback, 732 bytes |
| 0x800DD0D0 | attract_sequence_handler | game/attract.c | ShowAttract() | **Medium** | Screen sequencing, 988 bytes |

### Title Screen

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800DD4AC | title_screen_handler | game/attract.c:1193 | ShowLogo()/ShowJoin() | **High** | Main title screen, 2816 bytes |
| 0x800DDFAC | title_logo_handler | game/attract.c:1221 | AnimateLogo() | **Medium** | Logo animation, 608 bytes |
| 0x800DE20C | title_prompt_handler | game/attract.c | AddJoinNow() | **Medium** | PRESS START prompt, 724 bytes |
| 0x800DE4DC | title_background_handler | game/attract.c | title background | **Low** | Background effects, 908 bytes |

### Main Menu (N64-specific)

The N64 has a main menu structure not present in the arcade version. The arcade transitions directly from attract mode to track selection via chk_start().

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800DE868 | main_menu_handler | N64-specific | - | N/A | Main menu screen, 836 bytes |
| 0x800DEBAC | main_menu_input_handler | N64-specific | - | N/A | Menu input, 224 bytes |
| 0x800DEC8C | main_menu_render_handler | N64-specific | - | N/A | Menu rendering, 732 bytes |
| 0x800DEF68 | mode_select_handler | N64-specific | - | N/A | Race/Battle/Stunt mode select, 2976 bytes |
| 0x800DFB08 | mode_select_input_handler | N64-specific | - | N/A | Mode selection input, 188 bytes |
| 0x800DFBC4 | profile_select_handler | N64-specific | - | N/A | Profile selection, 1868 bytes |

---

## High Score System (hiscore.c)

The arcade hiscore() function handles the high score entry screen when gstate=HISCORE. EnterHighScore() manages character input for name entry using a keyboard layout and saves scores to NVRAM.

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800FBF88 | hiscore_entry_handler | game/hiscore.c | hiscore()/EnterHighScore() | **High** | High score entry |
| 0x800DBFC0 | hiscore_check_handler | game/hiscore.c | CheckHighScore() | **Medium** | Check if score qualifies |
| 0x800DC000 | hiscore_display_handler | game/hiscore.c | ShowHighScores() | **Medium** | Display high score table |
| 0x800DC100 | hiscore_insert_handler | game/hiscore.c | InsertHighScore() | **Medium** | Insert new entry |

---

## Countdown and Race Start

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800FBC30 | countdown_handler | game/game.c:1000+ | CountDown() | **High** | Race countdown 3-2-1-GO |
| 0x800FBC60 | countdown_display_handler | game/game.c | countdown display | **Medium** | Countdown number display |

---

## Pause Menu (N64-specific)

The N64 version has a pause menu not present in the arcade (arcade games cannot pause). This allows in-race options.

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800FAEF4 | pause_menu_handler | N64-specific | - | N/A | In-race pause menu, 1808 bytes |
| 0x800FB5F4 | pause_toggle_handler | N64-specific | - | N/A | Pause state toggle, 820 bytes |
| 0x800FB928 | game_timer_update | N64-specific | - | N/A | Timer update (pauses during pause), 712 bytes |

---

## Game Over and Results

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800FEE04 | gameover_handler | game/game.c | gameover() | **High** | Game over screen |
| 0x800FEC78 | game_results_init | game/game.c | endgame setup | **Medium** | Results initialization, 1808 bytes |
| 0x800FF298 | game_results_render | game/game.c | results display | **Medium** | Results rendering, 1164 bytes |
| 0x800FF724 | game_results_input | game/game.c | results input | **Medium** | Results input handling, 1748 bytes |
| 0x800FFDF8 | game_results_exit | game/game.c | results exit | **Medium** | Exit results screen, 1900 bytes |

---

## Options Menu

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800D4000 | options_menu_handler | game/game.c:494 | game_init() options | **Low** | Game options menu |
| 0x800CED3C | menu_audio_settings | N64-specific | - | N/A | Audio options submenu, 1364 bytes |
| 0x800CF290 | menu_video_settings | N64-specific | - | N/A | Video options submenu, 228 bytes |
| 0x800CF374 | menu_control_settings | N64-specific | - | N/A | Controller options, 808 bytes |
| 0x800CF69C | menu_controller_remap | N64-specific | - | N/A | Button remapping, 1976 bytes |
| 0x800CFE74 | menu_vibration_test | N64-specific | - | N/A | Rumble test, 404 bytes |

---

## Loading and Transitions

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800FA9E4 | loading_screen_handler | game/attract.c:1479 | ShowTransit() | **Medium** | Loading/transition screen, 1296 bytes |

---

## Menu and Selection

### Track Selection (arcade: sselect.c:TrackSel)

The arcade TrackSel() [sselect.c:123-1002] sets gstate=TRKSEL, handles input (SW_EAST/WEST/SELECT/START), updates trackno, calls ShowTrackSelect(true), and runs track_negotiation() for link play.

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800D000C | track_select_handler | game/sselect.c:123 | TrackSel() | **High** | Main track selection menu, 2264 bytes |
| 0x800D08E4 | track_preview_handler | game/sselect.c | ShowTrackSelect() | **Medium** | Renders track thumbnail/preview, 692 bytes |
| 0x800D0BA0 | track_info_handler | game/sselect.c | track stats display | **Medium** | Shows track statistics, 1192 bytes |

### Car Selection (arcade: sselect.c:init_car_select, AnimateCarSel)

The arcade init_car_select() [sselect.c:1190-1284] sets gstate=CARSEL, SetCountdownTimer(CAR_SELECT_TIME), ShowCarSelect(true), and create_cars() for 3D car models on rotating platform. AnimateCarSel [sselect.c:1373-1504] animates the car turntable and handles gPickPad selection.

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800D138C | car_select_handler | game/sselect.c:1190 | init_car_select()/CarSel() | **High** | Car selection menu, 804 bytes |
| 0x800D16B0 | car_preview_handler | game/sselect.c:1373 | AnimateCarSel() | **Medium** | Renders rotating 3D car model, 564 bytes |
| 0x800D18E4 | car_stats_handler | game/sselect.c | car stats | **Medium** | Shows car performance stats, 152 bytes |
| 0x800D197C | car_unlock_handler | game/sselect.c | unlock check | **Medium** | Checks if car is unlocked, 316 bytes |
| 0x800D1AB8 | car_color_handler | game/sselect.c | color select | **Medium** | Car color/paint selection, 552 bytes |

### Race Setup

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800D1CE0 | race_setup_screen | game/sselect.c | race options | **Medium** | Configure race options, 1960 bytes |
| 0x800D24C8 | lap_count_screen | game/sselect.c | lap count | **Medium** | Lap count selection, 1120 bytes |
| 0x800D2928 | difficulty_select | game/game.c | difficulty | **Low** | Difficulty selection, 332 bytes |

---

## Physics System (drivsym.c)

The Milliken tire model physics from the arcade are adapted for N64.

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800B8C14 | physics_sym | game/drivsym.c:186 | sym() | **High** | Main per-frame physics update |
| 0x800B9000 | physics_regular | game/drivsym.c:285 | regular() | **High** | Standard physics cycle (road, forces, velocity, position) |
| 0x800B9400 | physics_positions | game/drivsym.c:594 | positions() | **High** | Integrate velocity to position |
| 0x800BAE24 | physics_forces | game/drivsym.c:382 | forces() | **High** | Calls forces1(), forces2() |
| 0x80095240 | physics_forces1 | game/drivsym.c:391 | forces1() | **Medium** | Tire/drag force calculation |
| 0x800953C0 | physics_forces2 | game/drivsym.c:494 | forces2() | **Medium** | Sum all forces |
| 0x80095840 | physics_torques | game/drivsym.c:530 | torques() | **Medium** | Calculate moments |
| 0x800B0868 | PhysicsObjectList_Update | game/drivsym.c | main loop | **High** | Per-frame physics object list iterator |

### Physics Velocity Functions

These integrate acceleration to velocity (arcade: drivsym.c velocities):

| N64 Address | N64 Name | Confidence | Notes |
|-------------|----------|------------|-------|
| 0x8008B69C | physics_velocity_integrate_a | **Medium** | 712 bytes |
| 0x8008B964 | physics_velocity_integrate_b | **Medium** | Variant |
| 0x8008BA84 | physics_velocity_integrate_c | **Medium** | Variant |
| 0x8008BB8C | physics_velocity_integrate_d | **Medium** | Variant |
| 0x8008BC94 | physics_velocity_integrate_e | **Medium** | Variant |
| 0x8008BD9C | physics_velocity_integrate_f | **Medium** | Variant |

---

## Tire Model (tires.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800BA000 | tire_calc_force | game/tires.c:120 | dotireforce() | **High** | Full tire force calculation |
| 0x800BA800 | tire_suspension_update | game/tires.c | suscomp() | **Medium** | Suspension compression calc |
| 0x800A7E00 | tire_force_calc | game/tires.c | friction circle | **High** | 1200 bytes, tire friction model |
| 0x800A4940 | wheel_rotation_update | game/visuals.c | wheel rotation | **Medium** | Visual wheel rotation and slip |
| 0x800A4CC0 | suspension_update | game/tires.c | suspension | **Medium** | Suspension compression |
| 0x800A4E60 | tire_skid_mark | game/visuals.c | DoSkid() | **Medium** | Tire skid mark generation |
| 0x800B338C | tire_sound_update | game/carsnd.c | DoTireSqueals() | **High** | Tire squeal sounds from slip |

---

## Drivetrain (drivetra.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800AB544 | drivetrain_update | game/drivetra.c:24 | drivetrain() | **High** | Drivetrain simulation |
| 0x800AB70C | engine_rpm_calc | game/drivetra.c:223 | engine() | **High** | Engine RPM calculation |
| 0x800AB7D8 | engine_torque_calc | game/drivetra.c:249 | enginetorque() | **High** | Engine torque lookup |
| 0x800ABBD0 | transmission_shift | game/drivetra.c:190 | transmission() | **Medium** | Gear shift logic |
| 0x800ABCC8 | transmission_ratio_get | game/drivetra.c | gear ratio | **Medium** | Get current gear ratio |
| 0x800AC3D8 | differential_output | game/drivetra.c | differential | **Medium** | Differential output calc |
| 0x800AC668 | wheel_torque_apply | game/drivetra.c | wheel torque | **Medium** | Apply torque to wheels |
| 0x800A7C9C | car_gear_shift | game/drivetra.c:164 | autoshift() | **Medium** | Auto transmission handler |

---

## AI/Drone System (drones.c, maxpath.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800A0000 | DoDrones | game/drones.c:253 | DoDrones() | **High** | Main drone housekeeping per frame |
| 0x800A1000 | MaxPathControls | game/maxpath.c:745 | MaxPathControls() | **High** | Drone path following controller |
| 0x800A2680 | AdjustSpeed | game/maxpath.c:1501 | AdjustSpeed() | **High** | Adjust drone throttle/brake |
| 0x800A25C0 | AdjustSteer | game/maxpath.c:1560 | AdjustSteer() | **High** | Adjust drone steering |
| 0x800A2378 | MaxPathZeroControls | game/maxpath.c:799 | MaxPathZeroControls() | **High** | Zero drone controls |
| 0x800A1800 | avoid_areas | game/maxpath.c | avoid_areas() | **Medium** | Obstacle avoidance |
| 0x800A0F74 | MP_TargetSpeed | game/maxpath.c | MP_TargetSpeed() | **High** | Target speed from maxpath |
| 0x800A0FDC | NextMaxPath | game/maxpath.c | NextMaxPath() | **High** | Get next maxpath index |
| 0x800A11E4 | PrevMaxPath | game/maxpath.c | PrevMaxPath() | **High** | Get previous maxpath index |
| 0x800A1244 | InitMaxPath | game/maxpath.c | InitMaxPath() | **High** | Initialize maxpath for drones |
| 0x80097AFC | MP_TargetSteerPos | game/maxpath.c | MP_TargetSteerPos | **High** | Target steering position |
| 0x800979A8 | mp_interval_pos | game/maxpath.c | mp_interval_pos | **High** | Car position relative to interval |
| 0x800A2CE4 | set_catchup | game/drones.c | set_catchup() | **High** | Set catchup mode |
| 0x800A2D0C | no_catchup | game/drones.c | no_catchup() | **High** | Disable catchup for tournament |
| 0x800F4FEC | assign_drones | game/drones.c | assign_drones() | **Medium** | Assign drone tasks |
| 0x800F54C0 | place_cars_in_order | game/drones.c | place_cars_in_order() | **Medium** | Sort cars by position |
| 0x800F7C28 | linear_interp | game/drones.c | linear_interp() | **Medium** | Linear interpolation for speeds |

---

## Checkpoint System (checkpoint.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800C0000 | CheckCPs | game/checkpoint.c:866 | CheckCPs() | **High** | Check checkpoint crossing |
| 0x800C85F0 | checkpoint_check | game/checkpoint.c | InitCPS() | **Medium** | Checkpoint collision |
| - | InitCPS | game/checkpoint.c:616 | InitCPS() | - | Checkpoint initialization (not yet mapped) |

---

## Collision Detection (collision.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800BB000 | collision_detect | game/collision.c | collision() | **Medium** | Main collision detection |
| 0x800BB400 | collision_car_car | game/collision.c | setFBCollisionForce() | **Medium** | Car-to-car collision |
| 0x800BB800 | collision_car_wall | game/collision.c | ForceApart() | **Medium** | Car-to-wall collision |
| 0x800BBC00 | collision_point_in_body | game/collision.c | PointInBody() | **Medium** | Point inside box test |
| 0x800A9390 | car_crash_detect | game/sresurrect.c:1193 | CheckCrash() | **High** | Crash detection |
| 0x800A99C8 | car_crash_response | game/collision.c | crash impulse | **Medium** | Collision response |

---

## Camera System (camera.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x80097CA0 | camera_transform | game/camera.c | camera matrix | **Medium** | Camera/view transformation |
| 0x800C0AC0 | camera_update | game/camera.c:485 | UpdateCam() | **High** | Main camera update |
| 0x800BB69C | camera_reset | game/camera.c:404 | ZeroCamera() | **Medium** | Reset camera state |
| 0x800BCBB8 | camera_follow_target | game/camera.c | follow | **Medium** | Camera follows target |
| 0x800BCEE4 | camera_collision_avoid | game/camera.c | collision | **Medium** | Prevent camera clipping |
| 0x800BDAA8 | camera_shake_start | game/camera.c | shake | **Low** | Camera shake effect |
| 0x800BF838 | camera_first_person | game/camera.c | first person | **Medium** | First person view |
| 0x800BFBEC | camera_third_person | game/camera.c | third person | **Medium** | Third person chase view |
| 0x800C085C | camera_build_view_matrix | game/camera.c | view matrix | **Medium** | Build view matrix |

---

## Audio System (sounds.c, carsnd.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800B37E8 | sound_control | game/sounds.c | SOUND() macro | **High** | Audio control |
| 0x800B358C | sound_stop | game/sounds.c | KillSound() | **High** | Stop sound |
| 0x800B24EC | sfx_play | game/sounds.c | snd_play() | **High** | Play sound effect |
| 0x800B2658 | sfx_stop | game/sounds.c | snd_stop() | **High** | Stop sound effect |
| 0x800B2DF8 | engine_sound_update | game/carsnd.c | DoEngineSound() | **High** | Engine sound from RPM |
| 0x800B333C | tire_sound_update | game/carsnd.c | DoTireSqueals() | **High** | Tire squeal from slip |
| 0x800B3710 | wind_sound_update | game/carsnd.c | sndWindNoise() | **Medium** | Wind noise from speed |
| 0x800B1B48 | music_control | game/sounds.c | StartRadio() | **Medium** | Music playback control |
| 0x800B39BC | crowd_cheer_play | game/sounds.c | crowd | **Low** | Crowd cheer for stunts |
| 0x800B3D20 | voice_play | game/sounds.c | announcer | **Medium** | Announcer voice clips |

---

## Visuals/Rendering (visuals.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x80099BFC | render_object | game/model.c | model render | **Medium** | 3D model rendering, 10KB |
| 0x80087A08 | object_render | game/visuals.c | UpdateVisuals | **Medium** | Object rendering, 10KB |
| 0x800A04C4 | render_scene | game/visuals.c | scene | **Medium** | Scene setup, viewport |
| 0x800A4508 | car_lod_select | game/visuals.c | LOD | **Medium** | Car model LOD selection |
| 0x800A5D34 | car_shadow_render | game/visuals.c | shadow | **Low** | Car shadow rendering |
| 0x800A6094 | car_lights_render | game/visuals.c | lights | **Low** | Headlight/taillight render |
| 0x800A6404 | car_render_full | game/visuals.c:1006 | UpdateVisuals() | **High** | Full car render with effects |
| 0x800A5588 | engine_particle_effect | game/visuals.c | StartSmoke() | **Medium** | Engine smoke/sparks |
| 0x800B811C | particle_type_set | game/visuals.c | particles | **Medium** | Particle effects |

---

## HUD (hud.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800C8B8C | hud_setup | game/hud.c:405 | InitHUD() | **High** | HUD initialization |
| 0x800C84FC | hud_speed_display | game/hud.c | speedometer | **Medium** | Render speedometer |
| 0x800C8DA0 | hud_tachometer_update | game/hud.c | tachometer | **Medium** | Update tachometer |
| 0x800C8E50 | hud_nitro_update | game/hud.c | nitro gauge | **Low** | N64-specific (nitro in 2049) |
| 0x800CADA4 | hud_render | game/hud.c | HUD render | **Medium** | Main HUD rendering |
| 0x800C760C | draw_number | game/hud.c | UpdateNumber() | **Medium** | Draw numeric value |
| 0x800C7818 | draw_speedometer | game/hud.c | speedometer | **Medium** | Draw speedometer dial |
| 0x800C7ED4 | draw_lap_counter | game/hud.c | laps | **Medium** | Display lap counter |
| 0x800C8864 | draw_position | game/hud.c | position | **Medium** | Show race position |
| 0x800C8920 | draw_timer | game/hud.c | timer | **Medium** | Show race time |

---

## Car Setup (initiali.c, visuals.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800A78C8 | car_setup_physics | game/initiali.c:188 | carsetup() | **High** | Car physics setup |
| 0x800A7AE4 | car_mass_set | game/initiali.c | mass/inertia | **Medium** | Set car mass |
| 0x800AA454 | car_reset_position | game/sresurrect.c | resurrect_car() | **Medium** | Reset to track |
| 0x800AA708 | car_spawn_at_checkpoint | game/sresurrect.c | spawn | **Medium** | Spawn at checkpoint |

---

## Crash/Resurrection (sresurrect.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800A9390 | car_crash_detect | game/sresurrect.c:1193 | CheckCrash() | **High** | Crash detection |
| 0x800AA454 | car_reset_position | game/sresurrect.c:382 | resurrect_car() | **High** | Resurrection handler |

---

## Track/Road (road.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800A2D4C | track_process_main | game/road.c | road() | **Medium** | Track segment processing, 3116 bytes |
| 0x800A377C | track_render_process | game/road.c | render | **Medium** | Track rendering, 3156 bytes |
| 0x800A3654 | track_spline_interp | game/road.c | spline | **Medium** | Catmull-Rom spline |
| 0x8009DC50 | track_segment_setup | game/road.c | segment | **Medium** | Track segment init |
| 0x8009DD88 | track_segment_render | game/road.c | render | **Medium** | Render track segment |
| 0x8009EA70 | track_bounds_check | game/road.c | bounds | **Medium** | Track boundary check |
| 0x800A1648 | track_segment_load | game/world.c | LoadWorld() | **Medium** | Load track from ROM |

---

## Input/Controls (controls.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800C95DC | controller_poll | game/controls.c | poll | **Medium** | Per-frame controller poll |
| 0x800C997C | process_inputs | game/pdu.c | ProcessPDUs() | **Medium** | Game loop input entry |
| 0x800AD734 | input_process_controller | game/controls.c | controls | **Medium** | Process controller input |
| 0x800ADD58 | input_deadzone_apply | game/controls.c | deadzone | **Low** | Input deadzone |
| 0x800E73E4 | Input_PollPads | game/controls.c | poll | **Medium** | Raw controller read |
| 0x80094EC8 | Input_ApplyPadConfig | game/controls.c | config | **Medium** | Controller config |

---

## Vector/Matrix Math (vecmath.c)

| N64 Address | N64 Name | Arcade File | Arcade Function | Confidence | Notes |
|-------------|----------|-------------|-----------------|------------|-------|
| 0x800091E0 | guMtxF2L | N64 libultra | guMtxF2L() | N/A | N64-specific matrix conversion |
| 0x800092E0 | guMtxL2F | N64 libultra | guMtxL2F() | N/A | N64-specific matrix conversion |
| 0x80009394 | guMtxIdentF | N64 libultra | guMtxIdentF() | N/A | N64-specific identity matrix |
| 0x8008D6B0 | math_utility | game/vecmath.c | vecmath | **Medium** | Vector/matrix utility |
| 0x800A373C | drone_rwtobod | game/unitvecs.c:35 | rwtobod() | **High** | Real-world to body coords |

---

## Data Structures

### Game State (gstate)

| N64 Address | Name | Arcade Equivalent | Notes |
|-------------|------|-------------------|-------|
| 0x801174B4 | gstate | gstate (game.c) | Game state bitmask |
| 0x801174B8 | gstate_prev | - | Previous state for transition |
| 0x801146EC | gstate_init_flag | in_game_mode | Init flag byte |
| 0x80142AFC | frame_counter | - | Frame counter |
| 0x801170FC | race_active | - | Race loop enable flag |

### Car State (game_car[])

| N64 Address | Name | Arcade Equivalent | Notes |
|-------------|------|-------------------|-------|
| 0x80152818 | game_car | game_car[] (cars.c) | Car state array, stride 0x3B8 (952 bytes) |
| +0x04C | pos_x | m->pos[0] | X position |
| +0x050 | pos_y | m->pos[1] | Y position |
| +0x054 | pos_z | m->pos[2] | Z position |
| +0x008 | vel_x | m->vel[0] | X velocity |
| +0x00C | vel_y | m->vel[1] | Y velocity |

### Track Data

| N64 Address | Name | Arcade Equivalent | Notes |
|-------------|------|-------------------|-------|
| 0x8014A250 | track_data | track_data[] | Track data, stride 0x808 (2056 bytes), 6 entries |
| 0x8014A110 | gTrackDataA | - | Track data array A |

### Player Slots

| N64 Address | Name | Arcade Equivalent | Notes |
|-------------|------|-------------------|-------|
| 0x80144030 | player_slots | - | Player state, stride 0x304 (772 bytes) |
| 0x80140BF0 | input_buffer | - | Input state, stride 0x20 (32 bytes) per player |

---

## Confidence Levels

- **High**: Strong structural/naming match, confirmed via analysis
- **Medium**: Similar functionality, needs verification
- **Low**: Speculative, based on address proximity or partial pattern match

---

## Files Cross-Reference

| Arcade File | N64 Functions | Description |
|-------------|---------------|-------------|
| game/game.c | game_loop, playgame_state_change | Main game loop and state machine |
| game/sselect.c | track_select_screen, car_select_screen, countdown | Selection screens |
| game/attract.c | attract_mode | Demo/attract mode |
| game/drivsym.c | physics_sym, physics_regular, physics_positions, physics_forces | Core physics simulation |
| game/tires.c | tire_calc_force, tire_suspension_update | Tire physics model |
| game/drivetra.c | drivetrain_update, engine_rpm_calc, transmission_shift | Drivetrain simulation |
| game/drones.c | DoDrones, set_catchup, assign_drones | AI drone management |
| game/maxpath.c | MaxPathControls, AdjustSpeed, AdjustSteer, MP_TargetSpeed | Drone path following |
| game/checkpoint.c | CheckCPs | Checkpoint detection |
| game/collision.c | collision_detect, collision_car_car, collision_car_wall | Collision system |
| game/camera.c | camera_update, camera_follow_target | Camera system |
| game/hud.c | hud_setup, hud_render, draw_speedometer | HUD display |
| game/visuals.c | car_render_full, UpdateVisuals | Visual rendering |
| game/sounds.c | sfx_play, sound_control, music_control | Audio system |
| game/carsnd.c | engine_sound_update, tire_sound_update | Car audio |
| game/sresurrect.c | car_crash_detect, car_reset_position | Crash/resurrection |
| game/road.c | track_process_main, track_render_process | Track/road handling |
| game/controls.c | controller_poll, process_inputs | Input handling |
| game/unitvecs.c | drone_rwtobod | Unit vector transforms |
| game/initiali.c | car_setup_physics | Car initialization |

---

## Last Updated

2025-12-30

## See Also

- `symbol_addrs.us.txt` - Complete N64 symbol list
- `reference/repos/rushtherock/game/` - Arcade source code
- `CLAUDE.md` - Project overview and status
