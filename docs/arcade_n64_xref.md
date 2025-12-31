# Arcade to N64 Cross-Reference Table

This document maps Rush The Rock arcade source files to Rush 2049 N64 decompiled sources.

## Overview

- **Arcade Source**: `reference/repos/rushtherock/game/` (79 C files, ~97K lines)
- **N64 Source**: `src/game/` (47 C files, ~42K lines)
- **Symbol File**: `symbol_addrs.us.txt` (~1950 symbols)

### Key Differences

1. **Graphics API**: Arcade uses 3dfx ZOID; N64 uses RDP/RSP microcode
2. **Sound System**: Arcade uses BSMT2000/DCS; N64 uses N64 audio API
3. **Memory Model**: Arcade has flat memory; N64 uses compressed ROM with runtime decompression
4. **Input**: Arcade uses steering wheel/pedals; N64 uses controller

---

## 1. Arcade File to N64 Source Mapping

### Core Game Logic

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `game.c` | 1000 | `src/game/game.c` | Mapped | Main game loop, state machine |
| `game.h` | 123 | `include/game/game.h` | Mapped | GState enum, constants |
| `init.c` | 500 | `src/game/init.c` | Partial | OS bootstrap differs significantly |
| `globals.c` | 200 | (in game.c) | Merged | Global variables inlined |
| `globals.h` | 400 | (in game.c) | Merged | Struct definitions |

### Racing/Physics

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `cars.c` | 1892 | `src/game/car.c` | Mapped | Car parameters, Milliken model |
| `tires.c` | 500 | `src/game/tire.c` | Mapped | Tire physics, friction circle |
| `collision.c` | 600 | `src/game/collision.c` | Mapped | Car-car collision |
| `mdrive.c` | 1200 | `src/game/drivetrain.c` | Mapped | Car controls, Update_MDrive |
| `drivetra.c` | 400 | `src/game/drivetrain.c` | Merged | Drivetrain simulation |
| `drivsym.c` | 900 | `src/game/physics.c` | Mapped | Force/torque calculations |

### AI/Pathfinding

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `drones.c` | 800 | `src/game/drone.c` | Mapped | AI drone control |
| `maxpath.c` | 3730 | `src/game/maxpath.c` | Mapped | Path following, AI steering |

### Checkpoints/Race Logic

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `checkpoint.c` | 1700 | `src/game/checkpoint.c` | Mapped | Checkpoint detection, timing |

### Camera

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `camera.c` | 1000 | `src/game/camera.c` | Mapped | View modes 1/2/3 |
| `camera.v3.c` | 2050 | `src/game/camera.c` | Merged | View 3 implementation |

### UI/Menus

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `attract.c` | 2412 | `src/game/attract.c` | Mapped | Attract mode, title screen |
| `hiscore.c` | 1500 | `src/game/hiscore.c` | Mapped | High score entry/display |
| `hud.c` | 1500 | `src/game/hud.c` | Mapped | HUD elements |
| `sselect.c` | 2808 | `src/game/select.c` | Mapped | Track/car selection |
| `select.c` | 3159 | `src/game/carsel.c` | Split | Car selection UI |

### Audio

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `sounds.c` | 1100 | `src/game/sound.c` | Mapped | Sound effects |
| `carsnd.c` | 1000 | `src/game/audio.c` | Mapped | Engine/tire sounds |

### Visual Effects

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `visuals.c` | 2495 | `src/game/effects.c` | Mapped | Skid marks, sparks, shadows |
| `targets.c` | 2048 | `src/game/particles.c` | Partial | Particle effects |

### Track/World

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `road.c` | 600 | `src/game/road.c` | Mapped | Road geometry |
| `world.c` | 200 | `src/game/track.c` | Mapped | World/environment setup |

### Math/Utilities

| Arcade File | Lines | N64 File | Status | Notes |
|-------------|-------|----------|--------|-------|
| `vecmath.c` | 200 | `src/game/vecmath.c` | Mapped | Vector math |
| `mathsubs.c` | 300 | `src/game/vecmath.c` | Merged | Math helpers |
| `d3math.c` | 150 | `src/game/vecmath.c` | Merged | 3D math |
| `cksum.c` | 100 | `src/util/checksum.c` | Mapped | Checksum functions |

### Not Ported (Arcade-Specific)

| Arcade File | Lines | Reason |
|-------------|-------|--------|
| `dis.c` | 2865 | 3dfx display list - replaced by RDP |
| `sdis.c` | 2865 | San Francisco Rush variant |
| `model.c` | 1200 | 3dfx model loading |
| `moobs.c` | 1000 | Motion objects (3dfx) |
| `pdu.c` | 1100 | Network PDU (arcade link) |
| `gamenet.c` | 700 | Network play |
| `fswheel.c` | 600 | Force feedback steering wheel |
| `tourney.c` | 1100 | Tournament mode (arcade cabinets) |
| `debug.c` | 2210 | Debug menus (different on N64) |

---

## 2. Key Function Mappings

### Game State Machine

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `game()` | 0x800FD464 | `game_loop` | High |
| `game_init()` | - | (different purpose) | Low |
| `attract()` | 0x800DB81C | `attract_mode` | High |
| `TrackSel()` | 0x800D000C | `track_select_handler` | High |
| `CarSel()` | 0x800D138C | `car_select_handler` | High |
| `preplay()` | 0x800FBC30 | `preplay_setup` | High |
| `CountDown()` | 0x800FBC30 | `countdown_handler` | High |
| `playgame()` | 0x800CA3B4 | `playgame_state_change` | High |
| `do_endgame()` | 0x800FEE04 | `gameover_handler` | Medium |
| `do_gameover()` | 0x800FEE04 | `gameover_handler` | Medium |
| `EnterHighScore()` | 0x800FBF88 | `hiscore_entry_handler` | High |

### Physics/Driving

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `Update_MDrive()` | 0x800AB544 | `drivetrain_update` | High |
| `Init_MDrive()` | 0x800AB70C | `engine_rpm_calc` | Medium |
| `dotireforce()` | 0x800A7E00 | `tire_force_calc` | High |
| `forces1()` | 0x80095240 | `physics_forces1` | High |
| `forces2()` | 0x800953C0 | `physics_forces2` | High |
| `torques()` | 0x80095840 | `physics_torques` | High |

### AI/Drones

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `InitDrones()` | 0x800A1244 | `InitMaxPath` | High |
| `DoDrones()` | 0x80093B20 | `drone_ai_update` | High |
| `assign_drones()` | 0x800F4FEC | `assign_drones` | High |
| `place_cars_in_order()` | 0x800F54C0 | `place_cars_in_order` | High |
| `linear_interp()` | 0x800F7C28 | `linear_interp` | High |
| `AdjustSteer()` | 0x800A25C0 | `AdjustSteer` | High |
| `AdjustSpeed()` | 0x800A2680 | `AdjustSpeed` | High |
| `MP_TargetSteerPos()` | 0x80097AFC | `MP_TargetSteerPos` | High |
| `MP_TargetSpeed()` | 0x800A0F74 | `MP_TargetSpeed` | High |
| `NextMaxPath()` | 0x800A0FDC | `NextMaxPath` | High |
| `PrevMaxPath()` | 0x800A11E4 | `PrevMaxPath` | High |
| `mp_interval_pos()` | 0x800979A8 | `mp_interval_pos` | High |
| `update_mpath_index()` | 0x800F8588 | `update_mpath_index` | High |
| `find_maxpath_intervals()` | 0x800F87AC | `find_maxpath_intervals` | High |
| `set_catchup()` | 0x800A2CE4 | `set_catchup` | High |
| `no_catchup()` | 0x800A2D0C | `no_catchup` | High |

### Checkpoints

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `CheckCPs()` | - | (in game.c) | Medium |
| `InitCPS()` | - | (in game.c) | Medium |
| `FPutCheckpoints()` | - | (in game.c) | Medium |

### Camera

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `init_view()` | 0x800BB69C | `camera_reset` | High |
| `update_camera()` | 0x800C0AC0 | `camera_update` | High |
| `update_rear_camera()` | 0x800BFBEC | `camera_third_person` | Medium |
| `flyaround()` | 0x800C00E0 | `camera_free_look` | Medium |

### Audio

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `SOUND()` | 0x800B24EC | `sfx_play` | High |
| `DoEngineSound()` | 0x800B2DF8 | `engine_sound_update` | High |
| `DoTireSqueals()` | 0x800B338C | `tire_sound_update` | High |
| `InitCarSnds()` | 0x800B0904 | `audio_init` | Medium |
| `StartRadio()` | 0x800B1B48 | `music_control` | Medium |

### Rendering

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `UpdateVisuals()` | 0x800B55FC | `visual_objects_update` | High |
| `DoSkid()` | 0x800A4E60 | `tire_skid_mark` | High |
| `StartSmoke()` | 0x800A5744 | `exhaust_smoke_effect` | Medium |

### HUD

| Arcade Function | N64 Address | N64 Symbol | Confidence |
|-----------------|-------------|------------|------------|
| `InitHUD()` | - | (in game.c) | Low |
| `ShowHUD()` | 0x80095600 | `hud_update` | Medium |
| `update_lap_counter()` | - | (in game.c) | Low |

---

## 3. Unmapped Functions Needing Investigation

### High Priority (Core Game Logic)

| Arcade Function | File | Purpose | Investigation Notes |
|-----------------|------|---------|---------------------|
| `ProcessPDUs()` | game.c | Network packet processing | N64 may have simplified version for local play |
| `get_switches()` | controls.c | Read arcade switches | Replaced with controller input |
| `check_buttons()` | game.c | Button press handling | Need to find N64 input handler |
| `get_credits()` | game.c | Coin handling | Not applicable to N64 |
| `munge_gLink_data()` | game.c | Link play data | May be stubbed |
| `ResetGameLink()` | game.c | Link reset | May be stubbed |

### Medium Priority (Physics)

| Arcade Function | File | Purpose | Investigation Notes |
|-----------------|------|---------|---------------------|
| `calctireuv()` | tires.c | Tire unit vector calc | May be inlined |
| `bodtorw()` | vecmath.c | Body to real-world coords | Look for matrix functions |
| `crossprod()` | vecmath.c | Cross product | Likely in vecmath.c |
| `vecadd()` | vecmath.c | Vector add | Likely in vecmath.c |

### Low Priority (UI/Visual)

| Arcade Function | File | Purpose | Investigation Notes |
|-----------------|------|---------|---------------------|
| `DrawBlits()` | attract.c | 2D sprite rendering | Different system on N64 |
| `txt_str()` | - | Text rendering | Different font system |
| `MBOX_ScreenDump()` | game.c | Debug screenshot | Debug feature |
| `ShowTransit()` | attract.c | Loading transition | May be different |

---

## 4. Structural Differences

### Game State Machine

**Arcade** (enum-based switch):
```c
enum GState { ATTRACT, TRKSEL, CARSEL, PLAYGAME, ENDGAME, GAMEOVER, HISCORE, PREPLAY, PREPLAY2, COUNTDOWN };
switch (gstate) {
    case ATTRACT: attract(); break;
    case TRKSEL: TrackSel(); break;
    // ...
}
```

**N64** (bitmask-based ladder):
```c
// gstate at 0x801174B4 uses bitmask encoding
// 0x00040000 = GSTATE_INIT
// 0x00080000 = GSTATE_SETUP
// 0x00100000 = GSTATE_MENU
// 0x00200000 = GSTATE_CAMERA
// 0x00400000 = GSTATE_READY
// 0x00800000 = GSTATE_PLAY
// 0x01000000 = GSTATE_CARS
// 0x02000000 = GSTATE_FINISH
if (gstate & 0x00040000) state_init_handler();
else if (gstate & 0x00080000) state_setup_handler();
// ...
```

### Car Data Structure

**Arcade** (game_car[] array):
- Each car ~200 bytes
- Contains pos, vel, uvs, flags
- 8 cars max (MAX_LINKS)

**N64** (game_car at 0x80152818):
- Each car 952 bytes (stride 0x3B8)
- Extended for N64-specific data
- 6 entries in practice

### Memory Layout

**Arcade**:
- Flat memory model
- Direct hardware access
- ~16MB addressable

**N64**:
- Compressed ROM with runtime decompression
- Game code at 0x80086A50 (632KB decompressed)
- TLB-mapped memory regions

### Input System

**Arcade**:
- Steering wheel (analog position)
- Gas/brake/clutch pedals (8-bit ADC)
- Shifter (digital)
- Buttons (view, abort, music)

**N64**:
- Analog stick for steering
- A/B buttons for gas/brake
- Trigger for additional functions
- D-pad for menu navigation

---

## 5. Recommended Investigation Order

1. **game_loop (0x800FD464)** - Verify state machine matches arcade game()
2. **playgame_state_change (0x800CA3B4)** - Main gameplay handler
3. **drivetrain_update (0x800AB544)** - Core physics, compare to Update_MDrive
4. **drone_ai_update (0x80093B20)** - AI behavior, compare to DoDrones
5. **tire_force_calc (0x800A7E00)** - Tire physics, compare to dotireforce
6. **camera_update (0x800C0AC0)** - Camera system, compare to update_camera

---

## 6. Symbol Statistics

| Category | Arcade | N64 Named | N64 Total |
|----------|--------|-----------|-----------|
| Game Logic | ~50 | ~40 | ~50 |
| Physics | ~30 | ~25 | ~35 |
| AI/Drones | ~20 | ~18 | ~20 |
| Audio | ~25 | ~45 | ~60 |
| Camera | ~15 | ~30 | ~35 |
| Rendering | ~40 | ~60 | ~80 |
| UI/Menu | ~30 | ~25 | ~30 |
| **Total** | ~210 | ~243 | ~310 |

Note: N64 has more symbols due to OS/hardware abstraction layers and N64-specific features.

---

## Appendix: File Size Comparison

| Category | Arcade LOC | N64 LOC | Ratio |
|----------|------------|---------|-------|
| Core Game | 1,123 | 4,309 | 3.8x |
| Physics | 3,192 | 2,568 | 0.8x |
| AI | 4,530 | 1,331 | 0.3x |
| Camera | 3,050 | 423 | 0.1x |
| Audio | 2,100 | 1,300 | 0.6x |
| UI/Menu | 6,000 | 3,200 | 0.5x |
| **Total Game** | ~20,000 | ~13,000 | 0.65x |

The N64 version is more compact due to:
- Simpler UI (no coin handling, link play)
- Shared library code (libultra)
- Removed debug features
- Optimized for cartridge space

---

*Last Updated: 2025-12-31*
*Generated from analysis of rushtherock/game/ and symbol_addrs.us.txt*
