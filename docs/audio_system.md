# Rush 2049 N64 Audio System Documentation

## Overview

The Rush 2049 N64 audio system is a comprehensive sound engine that handles sound effects (SFX), music playback, engine sounds, environmental audio, and 3D positional audio. The system is derived from the arcade version's CAGE (Custom Audio Game Engine) system but adapted for the N64's audio hardware capabilities.

The audio system uses the N64's Audio Interface (AI) for sample playback and the standard libultra audio library (AL) for sound management.

## Architecture

### System Layers

```
+------------------------------------------+
|           Game Logic Layer               |
|  (game.c, sound.c - high-level API)      |
+------------------------------------------+
|         Audio Manager Layer              |
|  (audio.c - message queue, scheduling)   |
+------------------------------------------+
|        N64 Audio Library (AL)            |
|  (alSndp*, alSeqp* - sound/sequence)     |
+------------------------------------------+
|     Low-Level OS Interface               |
|  (osAi* - DMA, sample rate control)      |
+------------------------------------------+
|         N64 Audio Hardware               |
|  (AI registers at 0xA4500000)            |
+------------------------------------------+
```

### Thread Model

The N64 version uses a dedicated audio thread (Thread 8) for sound processing:

| Thread | ID | Priority | Purpose |
|--------|-----|----------|---------|
| Audio Thread | 8 | 3 | Audio buffer management, sound mixing |
| Game Thread | 7 | 2 | Main game logic |
| Render Thread | 5 | 1 | Graphics processing |

The audio thread is created at address `0x800AC75C` with a stack size of 0x960 bytes.

## Key Source Files

### N64 Implementation

| File | Purpose |
|------|---------|
| `src/game/sound.c` | Main sound API, channel management, arcade compatibility layer |
| `src/game/audio.c` | Audio thread interface, message queue handling |
| `src/libultra/os_ai.c` | AI hardware interface, DMA and sample rate |
| `src/game/game.c` | Sound integration with game logic, active sound updates |
| `include/game/sound.h` | Sound effect IDs, data structures, function prototypes |

### Arcade Reference

| File | Purpose |
|------|---------|
| `reference/repos/rushtherock/game/sounds.c` | CAGE audio command queue, sound dispatch |
| `reference/repos/rushtherock/game/moobs.c` | Motion Object Oriented Broadcast System (3D audio) |
| `reference/repos/rushtherock/game/carsnd.c` | Car-specific sounds (engine, tires, collisions) |
| `reference/repos/rushtherock/game/sounds.h` | Sound command IDs and constants |
| `reference/repos/rushtherock/game/carsnd.h` | Car sound parameters and structures |

## Data Structures

### SoundChannel (src/game/sound.c)

```c
typedef struct SoundChannel {
    u16     sound_id;           /* Current sound playing */
    u8      priority;           /* Channel priority (0-2) */
    u8      volume;             /* Volume (0-127) */
    s8      pan;                /* Pan (-64 to 63) */
    u8      playing;            /* Is channel active? */
    u8      looping;            /* Is sound looping? */
    u8      pad;
} SoundChannel;
```

### EngineSoundState (src/game/sound.c)

Per-car engine sound state for RPM-based pitch modulation:

```c
typedef struct EngineSoundState {
    f32     rpm;                /* Current RPM */
    f32     pitch;              /* Current pitch modifier */
    u8      channel;            /* Audio channel assigned */
    u8      active;             /* Is engine sound active? */
    u8      pad[2];
} EngineSoundState;
```

### N64SoundHandle (src/game/sound.c)

Internal sound handle structure from ROM analysis:

```c
typedef struct N64SoundHandle {
    struct N64SoundHandle *next;  /* 0x00: Next in chain */
    void *field_04;               /* 0x04 */
    u8   pad_08[0x2C];            /* 0x08-0x33 */
    u16  state_index;             /* 0x34: Index into state array */
    u8   pad_36[6];               /* 0x36-0x3B */
    struct N64SoundHandle *linked;/* 0x3C: Linked sound handle */
} N64SoundHandle;
```

### Arcade snd_state (reference/repos/rushtherock/game/carsnd.h)

```c
typedef struct snd_state {
    S16  volume;
    S16  pitch;
    U16  state_var;
    BOOL on;
} snd_state;
```

### Arcade skid_data (reference/repos/rushtherock/game/carsnd.h)

```c
typedef struct skid_data {
    S32  force_tab[5];
    S32  pitch_h;
    S32  pitch_l;
    S32  vol_h;
    S32  vol_l;
    S16  gr_skid_thresh;
    S16  gr_smoke_thresh;
} skid_data;
```

## Global Variables

### Sound State

| Variable | Address | Type | Description |
|----------|---------|------|-------------|
| `sound_channels[]` | - | SoundChannel[16] | SFX channel states |
| `engine_sounds[]` | - | EngineSoundState[8] | Per-car engine sounds |
| `music_volume` | - | u8 | Music volume (0-100) |
| `sfx_volume` | - | u8 | SFX volume (0-100) |
| `sound_enabled` | - | u8 | Sound enable flag |
| `current_music` | - | u16 | Current music track ID |
| `music_playing` | - | u8 | Music playing flag |
| `active_sound_count` | - | s32 | Number of active sounds |
| `active_sound_list[]` | - | N64SoundHandle*[] | Active sound handles |
| `sound_state_array[]` | - | N64SoundState[] | Sound state array |

### Audio System (from game.c externs)

| Variable | Description |
|----------|-------------|
| `sound_handles[]` | Sound handle array |
| `audio_effect_handles[]` | Effect handle array |
| `audio_priority_list[]` | Sound priority list |
| `audio_duck_targets[5]` | Audio ducking targets |
| `speed_of_sound` | Speed of sound for doppler |
| `audio_zone_type` | Current audio zone type |
| `channel_volumes[16]` | Per-channel volumes |
| `channel_pans[16]` | Per-channel pans |
| `channel_pitches[16]` | Per-channel pitches |
| `audio_channel_states[16]` | Channel state flags |
| `current_music_track` | Current playing track |
| `audio_pause_flag` | Pause flag |
| `audio_buffer_ptr` | Audio buffer pointer |

## Sound Effect IDs

### N64 Sound Effects (include/game/sound.h)

```c
/* Engine sounds */
#define SFX_ENGINE_IDLE     0x00
#define SFX_ENGINE_REV      0x01
#define SFX_ENGINE_ACCEL    0x02

/* Tire sounds */
#define SFX_TIRE_SCREECH    0x10
#define SFX_TIRE_SKID       0x11

/* Collision sounds */
#define SFX_CRASH_LIGHT     0x20
#define SFX_CRASH_HEAVY     0x21
#define SFX_CRASH_WALL      0x22

/* Race sounds */
#define SFX_CHECKPOINT      0x30
#define SFX_LAP_COMPLETE    0x31
#define SFX_RACE_START      0x32
#define SFX_RACE_FINISH     0x33

/* Countdown */
#define SFX_COUNTDOWN_3     0x40
#define SFX_COUNTDOWN_2     0x41
#define SFX_COUNTDOWN_1     0x42
#define SFX_COUNTDOWN_GO    0x43

/* Other */
#define SFX_WRONG_WAY       0x50
#define SFX_MENU_SELECT     0x60
#define SFX_MENU_MOVE       0x61
#define SFX_MENU_BACK       0x62
#define SFX_BOOST           0x70
#define SFX_WING_EXTEND     0x71
```

### Arcade GUTS System Commands (reference/repos/rushtherock/game/sounds.h)

Commands with values >= 0x8000 are system commands:

| Command | Value | Description |
|---------|-------|-------------|
| S_STOP_ALL_SNDS | 0x8000 | Stop all sounds |
| S_ATTRACT_MODE | 0x8001 | Set attract volume mode |
| S_GAME_MODE | 0x8002 | Set game volume mode |
| S_SET_GAME_VOL | 0x8006 | Set game mode volume |
| S_SET_ATTR_VOL | 0x8007 | Set attract mode volume |
| S_MOOB_LISTENER | 0x8019 | Set MOOB listener position |
| S_MOOB_KILL_ALL | 0x801A | Kill all motion objects |
| S_MOOB_KILL_OBJECT | 0x801B | Kill specific motion object |
| S_ENGINE_STOP | 0x8022 | Stop engine sound |
| S_ENGINE_CHANGE_PF | 0x8023 | Change engine pitch/filter |
| S_ENGINE_CHANGE_PFV | 0x8024 | Change engine pitch/filter/volume |
| S_ENGINE_CHANGE_AUTO | 0x8025 | Change automatic engine speed |
| S_ENGINE_CRASH | 0x8026 | Engine crash sound |

### Arcade Engine Types

```c
const U16 own_engine[NUM_ENGINES][2] = {
    {S_AUDREY, 0},    /* v1.06 vw */
    {S_GHR, 0},       /* v1.06 std */
    {S_NSX, 0},       /* NSX engine */
    {S_TPM, 0},       /* TPM engine */
};
```

### Arcade Tire Sounds

```c
const U16 tire_squeal[4] = {
    S_SQUEAL_LEFT,    /* Left tire squeal */
    S_SQUEAL_RIGHT,   /* Right tire squeal */
    S_SKID_CUSTOM1,   /* Custom skid 1 */
    S_PEELOUT         /* Peelout sound */
};
```

### Arcade Music/Radio Stations

7 radio stations defined in arcade source:

```c
const S16 demo_radio_on[MAX_RADIO] = {
    S_PULPATT, S_AUG8ATT, S_ZETHNOATT, S_THEROCK,
    S_STLATT, S_RAVERUSHATT, S_BLUESATT
};
```

## Key Functions

### Initialization

| Function | File | Description |
|----------|------|-------------|
| `sound_init()` | sound.c | Initialize sound channels and engine sounds |
| `sound_reset()` | sound.c | Reset sound system (stop all sounds) |
| `audio_start()` | init.c | Start audio thread and playback |
| `audio_msg_send()` | audio.c | Send message to audio thread |

### Sound Effect Playback

| Function | File | Description |
|----------|------|-------------|
| `sound_play(sound_id)` | sound.c | Play sound at default volume |
| `sound_play_vol(sound_id, volume)` | sound.c | Play sound with specific volume |
| `sound_play_3d(sound_id, pos[3])` | sound.c | Play positional 3D sound |
| `sound_stop(sound_id)` | sound.c | Stop specific sound |
| `sound_stop_all()` | sound.c | Stop all sound effects |
| `SOUND(cmd)` | sound.c | Arcade-compatible sound dispatch |

### Music Control

| Function | File | Description |
|----------|------|-------------|
| `music_play(track_id)` | sound.c | Start music track |
| `music_stop()` | sound.c | Stop current music |
| `music_pause()` | sound.c | Pause music |
| `music_resume()` | sound.c | Resume music |
| `music_fade_out(frames)` | sound.c | Fade out music over frames |
| `music_set_volume(volume)` | sound.c | Set music volume |

### Engine Sounds

| Function | File | Description |
|----------|------|-------------|
| `sound_start_engine(car_index)` | sound.c | Start engine sound for car |
| `sound_stop_engine(car_index)` | sound.c | Stop engine sound for car |
| `sound_update_engine(car_index, rpm, throttle)` | sound.c | Update engine pitch based on RPM |
| `engine_sound_update(car)` | game.c | Per-frame engine sound update |

### Collision/Impact Sounds

| Function | File | Description |
|----------|------|-------------|
| `sound_collision(car_index, force)` | sound.c | Play collision based on impact force |
| `sound_wall_hit(car_index, force)` | sound.c | Play wall impact sound |
| `sound_tire_screech(car_index, slip)` | sound.c | Play tire screech based on slip |
| `sound_tire_skid(car_index, slip)` | sound.c | Play skid sound (dirt/grass) |
| `sound_car_lands(car_index)` | sound.c | Play landing sound after jump |

### Game Event Sounds

| Function | File | Description |
|----------|------|-------------|
| `sound_checkpoint()` | sound.c | Play checkpoint sound |
| `sound_lap_complete()` | sound.c | Play lap completion sound |
| `sound_race_start(countdown)` | sound.c | Play countdown (3, 2, 1, GO) |
| `sound_first_place()` | sound.c | Play first place indicator |
| `sound_winner()` | sound.c | Play winner music |
| `sound_coin(full)` | sound.c | Play coin insert sound |
| `sound_menu_select()` | sound.c | Play menu selection |
| `sound_menu_browse()` | sound.c | Play menu browse sound |

### Per-Frame Updates

| Function | File | Description |
|----------|------|-------------|
| `sound_update()` | sound.c | Main per-frame sound update |
| `active_sounds_update()` | game.c | Update active sound objects |
| `update_game_systems(sound_update, do_physics)` | game.c | Main update dispatcher |

## N64 Audio Hardware Interface

### AI Register Map (os_ai.c)

| Register | Address | Description |
|----------|---------|-------------|
| AI_DRAM_ADDR_REG | 0xA4500000 | DRAM address for DMA |
| AI_LEN_REG | 0xA4500004 | DMA transfer length |
| AI_CONTROL_REG | 0xA4500008 | AI control register |
| AI_STATUS_REG | 0xA450000C | AI status register |
| AI_DACRATE_REG | 0xA4500010 | DAC sample rate divider |
| AI_BITRATE_REG | 0xA4500014 | Bit rate divider |

### osAiSetNextBuffer (os_ai.c)

Queues an audio buffer for DMA to the Audio Interface:

```c
s32 osAiSetNextBuffer(void *addr, u32 size);
```

- Uses double buffering with 0x2000 byte alignment
- Returns 0 on success, -1 if AI DMA queue is full

### osAiSetFrequency (os_ai.c)

Configures the AI sample rate:

```c
s32 osAiSetFrequency(u32 frequency);
```

- Formula: dacRate = osClockRate / frequency
- N64 clock rate: 46.875 MHz
- Minimum DAC rate: 0x84
- Maximum bit rate: 16
- Returns actual output frequency

## Arcade MOOB System (Motion Object Oriented Broadcast)

The arcade version uses a sophisticated 3D audio system called MOOB:

### Coordinate Transforms

```c
#define X_COORD_TRANS(x)   ((S16)((x*3)/10))  /* feet to meters */
#define Y_COORD_TRANS(y)   ((S16)((y*3)/10))  /* feet to meters */
#define MOOB_VELOCITY_XFORM(v) ((S16)((v*447)/120))  /* mph to mm/frame */
```

### MOOB Functions (reference/repos/rushtherock/game/sounds.c)

| Function | Description |
|----------|-------------|
| `sndListenerUpdate(x, y, vel, vel_angle, facing)` | Update listener position |
| `sndStartStaticUnpitched(objID, handle, priority, x, y)` | Start static sound |
| `sndStartStaticPitched(objID, handle, priority, x, y, pitch, filter, Q)` | Start pitched sound |
| `sndStartDopplerUnpitched(...)` | Start doppler sound (no pitch) |
| `sndStartDopplerPitched(...)` | Start full doppler sound |
| `sndDoSkid(tire, pitch, volume)` | Tire skid sound |
| `sndRoadNoise(pitch, volume)` | Road noise |
| `sndGravelNoise(pitch, volume)` | Gravel noise |
| `sndWindNoise(pitch, volume)` | Wind noise |
| `sndSplashNoise(pitch, volume)` | Water splash |
| `sndStartEngine(engineID, pitch, filter, Q)` | Start engine |
| `sndUpdateManualEngine(rpm, torque)` | Update engine from RPM |
| `sndUpdateMoobEngine(...)` | Update other car's engine (doppler) |
| `sndKillMoob(handle)` | Kill specific motion object |
| `sndKillAllMoobs()` | Kill all motion objects |

### Sound Queue System

The arcade uses a command queue to buffer sound commands:

```c
typedef struct sndEl {
   S16 d[16];
   U8  cnt;
} sndEl;

typedef struct sndQueue {
    U32 head;
    U32 tail;
   sndEl  q[Q_SIZE];  /* Q_SIZE = 4096 */
} sndQueue;
```

Key functions:
- `sndQXfer()` - Transfer queued commands to audio hardware
- `sndQStore()` - Store command in queue
- `sndCmds()` - Send commands (direct or queued)
- `sndUpdate()` - Called once per frame from model task

## N64 vs Arcade Mapping

### Function Mapping

| N64 Function | Address | Arcade Equivalent | Notes |
|--------------|---------|-------------------|-------|
| `sound_handle_stop` | 0x800B358C | `sndKillMoob` | Stop and remove sound |
| `sound_play` | - | `SOUND()` | General sound trigger |
| `sound_update_engine` | - | `sndUpdateManualEngine` | RPM-based pitch |
| `music_play` | - | `music(Do_it, station)` | Start music track |
| `music_stop` | - | `music(Undo_it, station)` | Stop music |
| `active_sounds_update` | 0x800F7344 | `UpdateListener` | Per-frame update |
| `audio_msg_send` | 0x8008A6D0 | `aud_put` | Queue audio command |

### Arcade Sound Types to N64

| Arcade Sound Type | N64 Equivalent |
|-------------------|----------------|
| ENGINESOUND | Engine channel (0-7) |
| ROADSOUNDS | SFX channels |
| BUMPSOUNDS | SFX channels |
| LISTENUPDATE | Automatic (per frame) |
| OTHERCARS | SFX channels (doppler) |
| MISCSOUNDS | Ambient SFX channels |

## Sound Priority System

### Priority Levels

```c
#define SND_PRIORITY_LOW    0
#define SND_PRIORITY_NORMAL 1
#define SND_PRIORITY_HIGH   2
```

### Channel Allocation

When all channels are in use, the system:
1. Searches for free channel
2. If none free, finds lowest priority channel
3. Preempts lowest priority sound with new sound

### Arcade Priority Constants

```c
#define DRONE_PRIORITY      /* Used for other car engines */
#define HIGHEST_PRIORITY    /* Used for critical sounds */
```

## Engine Sound Pitch Calculation

Engine pitch is modulated based on RPM:

```c
/* Pitch formula (sound.c) */
pitch = 0.5f + (rpm / 8000.0f) * 1.5f;

/* Clamp to valid range */
if (pitch < 0.5f) pitch = 0.5f;
if (pitch > 2.0f) pitch = 2.0f;
```

Arcade version uses more sophisticated model:

```c
/* RPM scaling (carsnd.c) */
#define MAX_ENG_RPM_SCL  0x1E00  /* Max RPM scale */

/* RPM is clamped to 0x1700 to prevent CAGE overload */
/* Torque scale default = 0.5 */
```

## Environmental Audio

### Arcade Reverb System

```c
/* Reverb commands (sounds.h) */
#define S_REVERB_SEND_VOL   0x8037
#define S_REVERB_RETURN_VOL 0x8038
#define S_REVERB_FEEDBACK   0x8039
#define S_REVERB_SIZE       0x803A
#define S_REVERB_PARMS      0x803B
```

### Tunnel Detection

Arcade tracks "in_tunnel" state per car for reverb adjustments.

### Miscellaneous Environmental Sounds

Triggered by proximity to specific world positions:

```c
const MISC_SOUND misc_sound_tab[] = {
    {S_SEAGULLS,   S_KSEAGULLS, 0, {-1623, 152, 4080}, 1000*1000},
    {S_PARKBIRD,   S_KPARKBIRD, 0, {-2003, 0, -1796}, 550*550},
    /* ... etc ... */
};
```

## Attract Mode

Special handling for attract mode sound restrictions:

```c
static u8 attract_mode_sound = 0;
static u8 attract_effects = 1;
static u8 attract_music = 1;

void set_attract_sound(s32 effects, s32 music);
```

Commands:
- `S_ATTRACT_MODE` (0x8001) - Enter attract mode
- `S_GAME_MODE` (0x8002) - Enter game mode

## Integration Notes

### Game Loop Integration

Sound updates are called from the main game update:

```c
void update_game_systems(s32 sound_update, s32 do_physics) {
    if (sound_update != 0) {
        objects_update();  /* Calls active_sounds_update */
    }
    /* ... physics, effects, render ... */
}
```

### State-Based Sound Control

Different game states trigger different music:

| Game State | Music Action |
|------------|--------------|
| ATTRACT | start_attract_music() |
| TRKSEL | start_select_music() |
| CARSEL | start_car_select_music() |
| PLAYGAME | start_track_music() |
| ENDGAME | stop_track_music() |
| GAMEOVER | stop_track_music() |
| HISCORE | sound_winner() |

## Known Limitations

1. **Channel Count**: N64 version limited to 16 SFX channels and 8 engine channels vs arcade's more sophisticated system
2. **3D Audio**: N64 has simplified 3D audio compared to arcade's full MOOB system
3. **Reverb**: N64 may have limited or no reverb compared to arcade
4. **Sample Quality**: N64 samples likely lower quality due to cartridge space constraints

## Further Research Needed

1. Complete mapping of all sound effect IDs from ROM
2. Music track organization and playback system
3. Audio sample bank structure
4. DMA buffer management details
5. Complete libultra AL function decompilation
6. Sound effect priority and ducking behavior

## Related Documentation

- `CLAUDE.md` - Project overview and current status
- `docs/PROGRESS_REPORT.md` - Decompilation progress
- `reference/repos/rushtherock/` - Complete arcade source code
- `symbol_addrs.us.txt` - Function addresses and symbols
