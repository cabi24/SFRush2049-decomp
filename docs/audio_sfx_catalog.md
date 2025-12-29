# Audio SFX Catalog - Rush 2049 N64 Decompilation

This document catalogs all sound effect IDs from the Rush The Rock arcade source code for use in the Rush 2049 N64 decompilation project.

## Overview

The arcade audio system uses a command-based interface where sound IDs >= 0x8000 are GUTS system commands, and sound effects use the `SOUND()` macro to trigger playback. The arcade audio hardware (CAGE board) processes these commands via a queue system.

### Arcade Sound System Architecture

- **MOOB (Motion Object OBject)**: Positional audio system for 3D sound effects
- **CAGE**: Atari's arcade audio hardware that processes sound commands
- **APM Engine**: RPM-based engine audio synthesis with multiple engine types
- **GUTS Commands**: System-level audio controls (0x8000+ range)

### Source Files Referenced

| File | Purpose |
|------|---------|
| `reference/repos/rushtherock/game/sounds.h` | Main sound definitions (rel_102) |
| `reference/repos/rushtherock/game/ssounds.h` | Original sound definitions (RushUp v5.3) |
| `reference/repos/rushtherock/game/sounds.82.h` | v8.2 sound definitions |
| `reference/repos/rushtherock/game/sounds.90.h` | v9.0 sound definitions |
| `reference/repos/rushtherock/game/carsnd.h` | Car sound constants and structures |
| `reference/repos/rushtherock/game/sounds.c` | Sound system implementation |
| `reference/repos/rushtherock/game/carsnd.c` | Car-specific sound handling |
| `reference/repos/rushtherock/game/moobdefs.h` | MOOB definitions for positional audio |

---

## Sound Categories

### 1. GUTS System Commands (0x8000-0x802F)

System-level audio controls for the CAGE audio hardware.

| ID | Name | Description | N64 Equivalent |
|----|------|-------------|----------------|
| 0x8000 | S_STOP_ALL_SNDS | Stop all sounds | sound_stop_all() |
| 0x8001 | S_ATTRACT_MODE | Select attract volume mode | attract_mode_sound = 1 |
| 0x8002 | S_GAME_MODE | Select game volume mode | attract_mode_sound = 0 |
| 0x8003 | S_VOLUME_TEST | Volume test: Jefferson Airplane | (diagnostic only) |
| 0x8004 | S_SMAX_CALLS | Query max sound calls | (diagnostic only) |
| 0x8005 | S_DO_DIAGNOSTICS | Start audio diagnostics | (diagnostic only) |
| 0x8006 | S_SET_GAME_VOL | Set game mode volume | sound_set_sfx_volume() |
| 0x8007 | S_SET_ATTR_VOL | Set attract mode volume | (N/A) |
| 0x8008 | S_MOS_VER | Query MOS version | (diagnostic only) |
| 0x8009 | S_PROJECT_VER | Query project version | (diagnostic only) |
| 0x800A | S_ERROR_CNT | Query error count | (diagnostic only) |
| 0x800B | S_CLEAR_ERROR_LOG | Clear error log | (diagnostic only) |
| 0x800C | S_RETURN_ERROR_LOG | Return error log | (diagnostic only) |
| 0x800D | S_REPORT_CHECKSUMS | Report checksums | (diagnostic only) |
| 0x800E | S_PEEK | Read 32-bit at address | (diagnostic only) |
| 0x800F | S_SPEAKER_TEST | Quad L/R speaker test | (diagnostic only) |
| 0x8010 | S_SINE_1K | 1kHz sine wave test | (diagnostic only) |
| 0x8011 | S_KILL_SINE | Stop sine wave | (diagnostic only) |
| 0x8012 | S_TEST2 | Second test | (diagnostic only) |
| 0x8013 | S_KILL_TEST2 | Stop second test | (diagnostic only) |
| 0x8014 | S_LOAD_BANK | Load sound bank from disk | (handled by ROM) |
| 0x8015 | S_UNLOAD_BANK | Unload sound bank | (handled by ROM) |
| 0x8016 | S_BANK_REPORT | Report loaded banks | (diagnostic only) |
| 0x8019 | S_MOOB_LISTENER | Set MOOB listener position | audio_listener_pos[] |
| 0x801A | S_MOOB_KILL_ALL | Kill all motion objects | sound_stop_all() |
| 0x801B | S_MOOB_KILL_OBJECT | Kill motion object by handle | sound_stop(handle) |
| 0x801C | S_MOOB_CHANGE_POSITION | Change MOOB position | audio_3d_position() |
| 0x801D | S_MOOB_CHANGE_VELOCITY | Change MOOB velocity | audio_doppler_effect() |
| 0x801E | S_MOOB_CHANGE_PITCH | Change MOOB pitch | audio_pitch_control() |
| 0x801F | S_MOOB_CHANGE_FILTER | Change MOOB filter | (N64: N/A) |
| 0x8022 | S_ENGINE_STOP | Stop listener engine | sound_stop_engine() |
| 0x8023 | S_ENGINE_CHANGE_PF | Change engine pitch/filter | sound_update_engine() |
| 0x8024 | S_ENGINE_CHANGE_PFV | Change engine pitch/filter/volume | sound_update_engine() |
| 0x8025 | S_ENGINE_CHANGE_AUTO | Change auto engine speed | sound_update_engine() |
| 0x8026 | S_ENGINE_CRASH | Engine crash (quick speed change) | collision_sound_play() |

---

### 2. Engine Sounds (0x8030-0x8036)

RPM-based engine audio synthesis with different engine types per car.

| ID | Name | Description | Parameters |
|----|------|-------------|------------|
| 0x8030 | S_MULTIDUMMY | Doppler MOOB multi-idle | (internal) |
| 0x8031 | S_M_MULTIIDLE | Multi-idle for other car's engine | 7 args: rpm, load, handle, priority, x, y, velocity, angle |
| 0x8032 | S_MULTIIDLE | Multi-idle for player engine | 2 args: rpm, load |
| 0x8033 | S_AUDREY | APM engine "Audrey" (VW-style) | 2 args: rpm, load |
| 0x8034 | S_NSX / S_CHEVELLE | APM engine NSX or Chevelle | 2 args: rpm, load |
| 0x8035 | S_GHR | APM engine "GHR" (standard) | 2 args: rpm, load |
| 0x8036 | S_TPM | APM engine "TPM" | 2 args: rpm, load |

**Engine Type Mapping (4 types in arcade):**
```c
const U16 own_engine[NUM_ENGINES][2] = {
    {S_AUDREY, 0},  /* Type 0: VW-style */
    {S_GHR, 0},     /* Type 1: Standard */
    {S_NSX, 0},     /* Type 2: Sports */
    {S_TPM, 0},     /* Type 3: Performance */
};
```

**N64 Implementation:**
```c
void sound_update_engine(s32 car_index, f32 rpm, f32 throttle);
void sound_start_engine(s32 car_index);
void sound_stop_engine(s32 car_index);
```

---

### 3. Reverb/Effects Commands (0x8037-0x803B)

Environmental audio effects for tunnels and enclosed spaces.

| ID | Name | Description | N64 Equivalent |
|----|------|-------------|----------------|
| 0x8037 | S_REVERB_SEND_VOL | Set reverb send level | audio_reverb_setup() |
| 0x8038 | S_REVERB_RETURN_VOL | Set reverb return level | audio_reverb_setup() |
| 0x8039 | S_REVERB_FEEDBACK | Set reverb feedback | audio_reverb_setup() |
| 0x803A | S_REVERB_SIZE | Set reverb size (width, height) | audio_reverb_setup() |
| 0x803B | S_REVERB_PARMS | Set reverb parameters | audio_reverb_setup() |

---

### 4. Music/Ambient Tracks (0x803C-0x80FF)

Background music and ambient sound loops.

#### Winner/Celebration
| ID | Name | Description | Kill Command |
|----|------|-------------|--------------|
| 0x803C | S_WINNER | Winner music RPM | S_KWINNER (0x803D) |
| 0x803E | S_SEAGULLS | Seagulls ambient | S_KSEAGULLS (0x803F) |
| 0x8040 | S_EXPLO | Explosion music RPM | S_KEXPLO (0x8041) |
| 0x8042 | S_WHATSYERNAME | "What's Your Name" song | S_KWHATSYERNAME (0x8043) |
| 0x8044 | S_PARKBIRD | Park birds ambient | S_KPARKBIRD (0x8045) |

#### Selection Music
| ID | Name | Description | Kill Command |
|----|------|-------------|--------------|
| 0x8046 | S_SELECT | Track selection music | S_KSELECT (0x8047) |
| 0x8048 | S_FANFARE2 | 3-2-1 countdown fanfare | - |
| 0x8049 | S_GARAGEAMB | Garage ambience RPM | S_KGARAGEAMB (0x804A) |
| 0x804B | S_CARSELECT | Car selection music | S_KCARSELECT (0x804C) |
| 0x804D | S_FOGHORN | Foghorn ambient | S_KFOGHORN (0x804E) |
| 0x804F | S_LOWERVOL | Lower selection music volume | - |

#### Attract Music (Radio Stations)
| ID | Name | Description | Kill Command |
|----|------|-------------|--------------|
| 0x8050 | S_AUG8ATT | August 8 attract | S_KAUG8ATT (0x8051) |
| 0x8052 | S_PULPATT | Pulp attract | S_KPULPATT (0x8053) |
| 0x8054 | S_STLATT | STL attract | S_KSTLATT (0x8055) |
| 0x8056 | S_ZETHNOATT | Zethno attract | S_KZETHNOATT (0x8057) |
| 0x8058 | S_RAVERUSHATT | Rave Rush attract | S_KRAVERUSHATT (0x8059) |
| 0x805A | S_BLUESATT | Blues attract | S_KBLUESATT (0x805B) |
| 0x805C | S_PLAYITAGAIN | "Play It Again" RPM | S_KPLAYITAGAIN (0x805D) |
| 0x805E | S_WHATSTAG | Goto tag for What's Your Name | - |

**Radio Station Arrays (from sounds.c):**
```c
const S16 demo_radio_off[MAX_RADIO] = {
    S_KPULPATT, S_KAUG8ATT, S_KZETHNOATT, S_KTHEROCK,
    S_KSTLATT, S_KRAVERUSHATT, S_KBLUESATT
};

const S16 demo_radio_on[MAX_RADIO] = {
    S_PULPATT, S_AUG8ATT, S_ZETHNOATT, S_THEROCK,
    S_STLATT, S_RAVERUSHATT, S_BLUESATT
};
```

---

### 5. Announcer/Voice Clips (0x805F-0x80AA)

Voice announcements for game events.

#### Announcer Clips
| ID | Name | Description |
|----|------|-------------|
| 0x805F | S_ANN1 | Announcer clip 1 |
| 0x8060 | S_ANN2 | Announcer clip 2 |
| 0x8061 | S_ANN3 | Announcer clip 3 |
| 0x8062 | S_ANN4 | Announcer clip 4 |
| 0x8063 | S_ANN5 | Announcer clip 5 |
| 0x8064 | S_ANN6 | Announcer clip 6 |
| 0x8065 | S_FINISHMOOB | Crowd finish celebration |
| 0x809C | S_START1 | Starting announcer 1 |
| 0x809E | S_START2 | Starting announcer 2 |
| 0x80A0 | S_START3 | Starting announcer 3 |

#### Lap Announcements
| ID | Name | Description |
|----|------|-------------|
| 0x80A2 | S_FINALL | "Final lap!" |
| 0x80A3 | S_2L | "2 laps to go" |
| 0x80A4 | S_3L | "3 laps to go" |
| 0x80A5 | S_4L | "4 laps to go" |
| 0x80A6 | S_5L | "5 laps to go" |
| 0x80A7 | S_6L | "6 laps to go" |
| 0x80A8 | S_7L | "7 laps to go" |
| 0x80A9 | S_8L | "8 laps to go" |
| 0x80AA | S_9L | "9 laps to go" |

#### Exclamations
| ID | Name | Description |
|----|------|-------------|
| 0x80EE | S_YELL | Yell exclamation |
| 0x80EF | S_ITSFAT | "It's Fat!" exclamation |
| 0x80F0 | S_CHIPS | "All that and a bag o' chips" |
| 0x80F1 | S_YOUGOTTA | "I can't do anymore..." |
| 0x80F2 | S_ITSINSANE | "It's insane!" |
| 0x80F3 | S_LOOKOUT | "Lookout!" |
| 0x80F4 | S_BADLICIOUS | "It's Badlicious!" |

---

### 6. Tire/Road Sounds (0x806C-0x8074)

Continuous looping sounds tied to car physics.

| ID | Name | Description | Parameters |
|----|------|-------------|------------|
| 0x806C | S_SKID_CUSTOM1 | Left tire skid | handle, pitch, vol |
| 0x806D | S_SKID_CUSTOM2 | Right tire skid | handle, pitch, vol |
| 0x806E | S_WINDNOISE | Wind at 30+ MPH | handle, pitch, vol |
| 0x806F | S_ROADNOISE | Pavement road noise | handle, pitch, vol |
| 0x8070 | S_GRAVELNOISE | Gravel/dirt road noise | handle, pitch, vol |
| 0x8071 | S_WATERROAD | Water/puddle splash | handle, pitch, vol |
| 0x8072 | S_PEELOUT | Tire peelout sound | handle, pitch, vol |
| 0x8073 | S_SQUEAL_LEFT | Left tire squeal | - |
| 0x8074 | S_SQUEAL_RIGHT | Right tire squeal | - |

**Tire Sound Array:**
```c
const U16 tire_squeal[4] = {
    S_SQUEAL_LEFT,   /* 0: Left side */
    S_SQUEAL_RIGHT,  /* 1: Right side */
    S_SKID_CUSTOM1,  /* 2: Front tires */
    S_PEELOUT        /* 3: Rear tires */
};
```

**Tire Sound Constants (carsnd.h):**
```c
#define WIND_PITCH          0x400
#define WIND_MAX_VOL        0x0150
#define ROAD_MAX_PITCH      0x03ff
#define ROAD_MIN_PITCH      0x0100
#define ROAD_MAX_VOL        0x0380
#define GRAVEL_MAX_VOL      0x00A0
```

---

### 7. Collision Sounds (0x8075-0x8082)

Impact and crash sound effects triggered by physics.

| ID | Name | Description | Trigger Condition |
|----|------|-------------|-------------------|
| 0x8075 | S_CARBUMP | Small collision | Car-world impact < threshold |
| 0x8076 | S_CARSCRAPE | High-speed scraping | Sustained side contact |
| 0x8077 | S_CARSMASH | Medium collision | Car-car collision |
| 0x8078 | S_CURBWHUMP | Curb hit | Tire hits curb |
| 0x8079 | S_BOTTOMOUT | Bottom out | Suspension compression |
| 0x807A | S_CONES | Cone hit | Collision with traffic cone |
| 0x807B | S_GLASS | Glass break | Collision with window |
| 0x807C | S_PMETER | Parking meter hit | Collision with meter |
| 0x807D | S_SCRAPELOOP | Looping scrape | Continuous scraping |
| 0x807E | S_KSCRAPELOOP | Kill scrape loop | End scraping state |
| 0x807F | S_LIGHTPOLE | Light pole hit | Collision with pole |
| 0x8080 | S_TREE | Tree hit | Collision with tree |
| 0x8081 | S_FENCE | Fence hit | Collision with fence |
| 0x8082 | S_BOOM | Seat-shaking boom | High-impact collision |

**Target Sound Array (carsnd.c):**
```c
typedef enum { CONE, GLASS, PMETER, BUSH, LIGHTPOLE, TREE, FENCE, NUM_TARGETS } target_types;

const S16 target_snd[] = {
    S_CONES, S_GLASS, S_PMETER, S_BUSH, S_LIGHTPOLE, S_TREE, S_FENCE
};

const S16 target_volume[] = { 255, 255, 255, 255, 255, 230 };
```

---

### 8. Game Event Sounds (0x8083-0x809B)

One-shot sounds for game events.

| ID | Name | Description | N64 Function |
|----|------|-------------|--------------|
| 0x8083 | S_CAR_LANDS | Car lands after jump | sound_car_lands() |
| 0x8084 | S_CHKPNTSTATIC | "Checkpoint!" voice | sound_checkpoint() |
| 0x8085 | S_BEEP1 | Time warning beep | - |
| 0x8086 | S_BOG | Engine bogging | S_KBOG (0x8087) |
| 0x8088 | S_LEADERLIGHT | Leader light chord | sound_first_place() |
| 0x808A | S_TRACKBROWSE | Track browse SFX | sound_menu_browse() |
| 0x808B | S_TRANSELECT | Transmission select SFX | sound_menu_select() |
| 0x808C | S_LIGHTSOUND | Car browse SFX | - |
| 0x808D | S_CROWDSCARE | Crowd scare reaction | - |
| 0x808E | S_TURNTABLE | Turntable loop | S_KTURNTABLE (0x808F) |
| 0x8090 | S_COIN1 | Partial credit coin | sound_coin(0) |
| 0x8091 | S_COIN2 | Full credit coin | sound_coin(1) |
| 0x8092 | S_THREE | Countdown "3" | sound_race_start(3) |
| 0x8093 | S_TWO | Countdown "2" | sound_race_start(2) |
| 0x8094 | S_ONE | Countdown "1" | sound_race_start(1) |
| 0x8095 | S_RUSH | Countdown "RUSH!" | sound_race_start(0) |
| 0x8096 | S_SPLASH | Splash in water | - |
| 0x8097 | S_DANGEROUS | "It's Dangerous!" | - |
| 0x8098 | S_KLAXON | Warning horn (start) | S_KKLAXON (0x8099) |
| 0x809A | S_SPUTTER | Engine sputter (time out) | S_KSPUTTER (0x809B) |

---

### 9. Ambient/Environment Sounds (0x8066-0x806B, 0x80D2-0x80E4)

Positional ambient sounds placed in the world.

| ID | Name | Description |
|----|------|-------------|
| 0x8066 | S_CABLECARBELL | Cable car bell |
| 0x8067 | S_FIRECRACKER | Firecracker explosion |
| 0x8068 | S_SMALLHOOT | Small crowd yell |
| 0x8069 | S_SMALLCLAP | Small crowd clap |
| 0x806A | S_PRISONMOOB | Prison siren MOOB |
| 0x806B | S_BLANK2 | Placeholder/placebo |
| 0x80D2 | S_BUSH | Hit a bush |
| 0x80D3 | S_RUSHWHISPER | "Rush" whisper |
| 0x80D4 | S_CROWDRIGHT | Crowd on right |
| 0x80D6 | S_CROWDLEFT | Crowd on left |
| 0x80D8 | S_THEROCK | Alcatraz theme | S_KTHEROCK (0x80D9) |
| 0x80DA | S_PRISONSIREN | Prison siren | S_KPRISONSIREN (0x80DB) |

#### Track-Specific Waiting Music
| ID | Name | Description |
|----|------|-------------|
| 0x80DC | S_WAIT1 | Waiting music track 1 |
| 0x80DD | S_WAIT2 | Waiting music track 2 |
| 0x80DE | S_WAIT3 | Waiting music track 3 |
| 0x80DF | S_WAIT4 | Waiting music track 4 |
| 0x80E0 | S_WAIT5 | Waiting music track 5 |
| 0x80E1 | S_WAIT6 | Waiting music track 6 |
| 0x80E2 | S_WAIT7 | Waiting music track 7 |
| 0x80E3 | S_WAIT8 | Waiting music track 8 |
| 0x80E4 | S_WAITROCK | Waiting music - The Rock |

#### Track Selection Announcements
| ID | Name | Description |
|----|------|-------------|
| 0x80E5 | S_YOU1 | "You have selected track 1" |
| 0x80E6 | S_YOU2 | "You have selected track 2" |
| 0x80E7 | S_YOU3 | "You have selected track 3" |
| 0x80E8 | S_YOU4 | "You have selected track 4" |
| 0x80E9 | S_YOU5 | "You have selected track 5" |
| 0x80EA | S_YOU6 | "You have selected track 6" |
| 0x80EB | S_YOU7 | "You have selected track 7" |
| 0x80EC | S_YOU8 | "You have selected track 8" |
| 0x80ED | S_YOUROCK | "You have selected The Rock" |

---

### 10. UI/Menu Sounds (0x80B5-0x80D1)

User interface and menu navigation sounds.

| ID | Name | Description |
|----|------|-------------|
| 0x80B5 | S_BEEPCAR | Car selection beep |
| 0x80B6 | S_BEEPWARN1 | Beginner car warning beep |
| 0x80B8 | S_BEEPWARN2 | Advanced car warning beep |
| 0x80BA | S_BEEPWARN3 | Expert car warning beep |
| 0x80C2 | S_8WAY | 8-way link exclamation |
| 0x80C3-0x80C9 | S_2CARS-S_8CARS | 2-8 cars linked sounds |
| 0x80CA-0x80D0 | S_K8CARS-S_K2CARS | Kill linked sounds |
| 0x80D1 | S_LINKED | 2-way link exclamation |

---

## N64 Sound Call Analysis

### Sound Functions in game.c

From analysis of `/home/cburnes/projects/rush2049-decomp/src/game/game.c`:

#### Audio System Functions (High-level)
| Function | Description | Lines |
|----------|-------------|-------|
| audio_init() | Initialize audio system | 22410 |
| audio_shutdown() | Shutdown audio system | 22446 |
| audio_frame_update() | Per-frame audio update | 22473 |
| audio_queue_process() | Process audio command queue | 6863, 21729 |
| audio_sync() | Synchronize audio state | 1086, 1186, 1190, 4378, 4689, 4713 |

#### Sound Playback Functions
| Function | Signature | Purpose |
|----------|-----------|---------|
| sfx_play() | `sfx_play(s32 soundId, f32 volume, f32 pan)` | Play sound effect |
| sound_play() | `sound_play(soundId, volume, pan)` | General sound playback |
| sound_play_menu() | `sound_play_menu(s32 soundId)` | Menu sound effects |
| voice_play() | `voice_play(s32 voiceId)` | Play voice announcement |
| collision_sound_play() | `collision_sound_play(void *car, f32 intensity)` | Play collision impact |

#### Music Functions
| Function | Purpose |
|----------|---------|
| audio_music_play(trackId) | Start music track |
| audio_stream_start(streamId, data) | Start audio stream |
| audio_stream_update() | Update streaming audio |

### Menu Sound IDs (from game.c usage)
| ID | Usage Context |
|----|---------------|
| 10 | Menu confirm/select |
| 11 | Menu back/cancel |
| 12 | Menu move/navigate |
| 13 | Error/locked sound |
| 14 | Toggle/checkbox sound |
| 15 | Slider tick, pause sound |
| 16 | Dialog open |
| 17 | Dialog close, quit |
| 20 | Unlock jingle, achievement |
| 25 | Jump sound |
| 30 | Countdown beep |

### Voice IDs (from game.c usage)
| ID | Voice Clip |
|----|------------|
| 3 | "First!" |
| 4 | "Second!" |
| 5 | "Third!" |
| 6 | "Final lap!" |
| 8 | "Checkpoint!" |
| 9 | "New record!" |

### In-Game Sound IDs
| ID | Sound Effect |
|----|--------------|
| 40 | Coin collect |
| 41 | Time bonus |
| 42 | Key collect |
| 50-54 | Various pickups |
| 55 | Generic pickup |
| 20 | Nitro activate |
| 21 | Nitro sustain |
| 22 | Nitro burst |
| 23 | Unknown |
| 30 | Countdown beep |
| 31 | Best lap |

---

## Sound Trigger Conditions

### Physics-Based Triggers

| Trigger | Condition | Sound |
|---------|-----------|-------|
| Tire squeal | sideforce > threshold | S_SQUEAL_LEFT/RIGHT |
| Tire skid | On dirt + moving | S_GRAVELNOISE |
| Wind noise | mph > 30 | S_WINDNOISE |
| Road noise | On pavement + moving | S_ROADNOISE |
| Water splash | Entering water | S_SPLASH |
| Engine bog | throttle + low rpm | S_BOG |

### Collision Force Thresholds (from carsnd.c)

| Force Level | Sound | Volume |
|-------------|-------|--------|
| > 5000 | S_CARBUMP | 150-235 |
| Sustained | S_CARSCRAPE | 165 |
| High-speed | S_CARSMASH | 190-255 |

### Tire Squeal Thresholds (from skid_data)

| Tire Position | Force Thresholds | Pitch Range | Volume Range |
|---------------|------------------|-------------|--------------|
| Left side | 1500-2600 | 800-1240 | 1-255 |
| Right side | 1500-2600 | 800-1240 | 1-255 |
| Front tires | 800-1500 | 800-1340 | 1-255 |
| Rear tires | 1700-2400 | 800-1140 | 1-255 |

---

## N64-Specific Sound IDs

From `/home/cburnes/projects/rush2049-decomp/include/game/sound.h`:

### SFX Categories
```c
/* Engine */
#define SFX_ENGINE_IDLE     0x00
#define SFX_ENGINE_REV      0x01
#define SFX_ENGINE_ACCEL    0x02

/* Tires */
#define SFX_TIRE_SCREECH    0x10
#define SFX_TIRE_SKID       0x11

/* Collisions */
#define SFX_CRASH_LIGHT     0x20
#define SFX_CRASH_HEAVY     0x21
#define SFX_CRASH_WALL      0x22

/* Race Events */
#define SFX_CHECKPOINT      0x30
#define SFX_LAP_COMPLETE    0x31
#define SFX_RACE_START      0x32
#define SFX_RACE_FINISH     0x33

/* Countdown */
#define SFX_COUNTDOWN_3     0x40
#define SFX_COUNTDOWN_2     0x41
#define SFX_COUNTDOWN_1     0x42
#define SFX_COUNTDOWN_GO    0x43

/* UI/Gameplay */
#define SFX_WRONG_WAY       0x50
#define SFX_MENU_SELECT     0x60
#define SFX_MENU_MOVE       0x61
#define SFX_MENU_BACK       0x62
#define SFX_BOOST           0x70
#define SFX_WING_EXTEND     0x71
```

---

## Arcade to N64 Sound Mapping

| Arcade Command | N64 Equivalent | Notes |
|----------------|----------------|-------|
| SOUND(cmd) | sound_play(cmd) | Direct translation |
| sndDoSkid() | sound_tire_screech() | Simplified |
| sndRoadNoise() | (inline in physics) | Per-surface handling |
| sndWindNoise() | (inline in physics) | Speed-based |
| sndStartManualEngine() | sound_start_engine() | Simplified RPM |
| sndUpdateManualEngine() | sound_update_engine() | Pitch only |
| sndPositionSound() | sound_play_3d() | 3D positioning |
| sndListenerUpdate() | audio_spatialize() | Camera tracking |
| music() | music_play()/stop() | Mode-aware |

---

## References

- Arcade source: `reference/repos/rushtherock/game/sounds.c` (1558 lines)
- Arcade source: `reference/repos/rushtherock/game/carsnd.c` (1309 lines)
- N64 implementation: `src/game/sound.c` (820 lines)
- N64 header: `include/game/sound.h` (193 lines)
- N64 game.c audio calls: 400+ sound function references

---

*Document generated for Rush 2049 N64 Decompilation Project*
*Last updated: 2025-12-29*
