# Audio System Mapping: N64 vs Arcade

This document maps the N64 Rush 2049 audio system to the arcade Rush The Rock source code.

## Overview

### N64 Audio Architecture

The N64 audio system uses a dedicated audio thread that communicates with the main game thread via message queues. The hardware components include:

| Component | Address | Description |
|-----------|---------|-------------|
| AI_DRAM_ADDR | 0xA4500000 | DRAM address for DMA |
| AI_LEN | 0xA4500004 | Length of DMA transfer |
| AI_CONTROL | 0xA4500008 | AI control register |
| AI_STATUS | 0xA450000C | AI status register |
| AI_DACRATE | 0xA4500010 | DAC sample rate divider |
| AI_BITRATE | 0xA4500014 | Bit rate divider |

**Key N64 Audio Libraries:**
- **libultra Audio Interface (AI)**: Low-level DMA and sample rate control
- **Audio Library (AL)**: High-level sound playback (sequences, samples)

### Arcade Audio Architecture (CAGE)

The arcade uses the CAGE (Custom Audio Game Engine) DSP board for audio processing:
- Dedicated DSP for 3D positional audio (MOOBs - Motion Object Audio)
- Software sound queue with hardware command interface
- 4KB command queue for audio commands
- Multiple engine sound models (Audrey, NSX, GHR, TPM)

## Function Mapping Table

### Core Sound Control

| N64 Function | Address | Arcade Function | File | Description |
|--------------|---------|-----------------|------|-------------|
| `sound_handle_stop` | 0x800B358C | `sndKillMoob` | sounds.c | Stop a sound by handle |
| `audio_control_update` | 0x800B37E8 | `sndUpdate` | sounds.c | Per-frame audio update |
| `audio_msg_send` | 0x8008A6D0 | N/A (HW direct) | sounds.c | Send audio thread message |
| `sound_init` | N64 only | `init_car_sound_handles` | carsnd.c | Initialize sound system |
| `sound_reset` | N64 only | `InitCarSnds` | carsnd.c | Reset sound state |
| `sound_play` | N64 only | `SOUND` | sounds.c | Play single sound |
| `sound_play_vol` | N64 only | `sndPositionSound` | sounds.c | Play sound with volume/angle |
| `sound_stop` | N64 | `sndKillMoob` | sounds.c | Stop specific sound |
| `sound_stop_all` | N64 | `sndKillAllMoobs` | sounds.c | Stop all sounds |
| `SOUND` | N64 wrapper | `SOUND` | sounds.c | Arcade-compatible interface |
| `SOUNDS` | N64 wrapper | `sound_wparms` | sounds.c | Multi-parameter sound call |

### Engine Sound System

| N64 Function | Address | Arcade Function | File | Description |
|--------------|---------|-----------------|------|-------------|
| `sound_start_engine` | N64 | `StartEngineSound` | carsnd.c | Start engine loop |
| `sound_stop_engine` | N64 | `StopEngineSound` | carsnd.c | Stop engine loop |
| `sound_update_engine` | N64 | `DoEngineSound` | carsnd.c | Update engine pitch/vol |
| N/A | N/A | `sndStartManualEngine` | sounds.c | Start RPM/torque engine |
| N/A | N/A | `sndUpdateManualEngine` | sounds.c | Update RPM/torque |
| N/A | N/A | `sndStartEngineWithVolume` | sounds.c | Engine with initial vol |
| N/A | N/A | `sndChangeEngine_PF` | sounds.c | Change pitch/filter |
| N/A | N/A | `sndChangeEngineWithVolume` | sounds.c | Change pitch/filter/vol |

### Tire/Road Sound System

| N64 Function | Address | Arcade Function | File | Description |
|--------------|---------|-----------------|------|-------------|
| `sound_tire_screech` | N64 | `DoTireSqueals` | carsnd.c | Tire squeal sounds |
| `sound_tire_skid` | N64 | `sndDoSkid` | sounds.c | Individual tire skid |
| N/A | N/A | `sndRoadNoise` | sounds.c | Pavement road noise |
| N/A | N/A | `sndGravelNoise` | sounds.c | Off-road gravel |
| N/A | N/A | `sndWindNoise` | sounds.c | Wind noise (speed-based) |
| N/A | N/A | `sndSplashNoise` | sounds.c | Water driving sound |

### Collision Sound System

| N64 Function | Address | Arcade Function | File | Description |
|--------------|---------|-----------------|------|-------------|
| `sound_collision` | N64 | `do_bump_sounds` | carsnd.c | Car collision sounds |
| `sound_wall_hit` | N64 | `sndPositionSound(S_CARBUMP)` | carsnd.c | Wall/barrier hit |
| N/A | N/A | `check_forces_on_car` | carsnd.c | Calculate collision force |
| N/A | N/A | `get_force_and_peak` | carsnd.c | Peak force detection |
| N/A | N/A | `kill_scrape_sound` | carsnd.c | Stop scraping loop |

### Music System

| N64 Function | Address | Arcade Function | File | Description |
|--------------|---------|-----------------|------|-------------|
| `music_play` | N64 | `music` | sounds.c | Start music track |
| `music_stop` | N64 | `music(Undo_it)` | sounds.c | Stop music |
| `music_fade_out` | N64 | N/A | N/A | Fade out music |
| `start_track_music` | N64 | `StartRadio` | carsnd.c | Start race music |
| `stop_track_music` | N64 | `StopRadio` | carsnd.c | Stop race music |
| `start_attract_music` | N64 | `music(Do_it)` | sounds.c | Attract mode music |
| `start_select_music` | N64 | `SOUND(S_SELECT)` | sounds.c | Selection screen |
| `start_car_select_music` | N64 | `SOUND(S_CARSELECT)` | sounds.c | Car select music |

### 3D Positional Audio

| N64 Function | Address | Arcade Function | File | Description |
|--------------|---------|-----------------|------|-------------|
| `sound_play_3d` | N64 | `sndListenerUpdate` | sounds.c | Set listener position |
| N/A | N/A | `sndStartStaticUnpitched` | sounds.c | Static positioned sound |
| N/A | N/A | `sndStartStaticPitched` | sounds.c | Static with pitch control |
| N/A | N/A | `sndStartDopplerUnpitched` | sounds.c | Doppler sound (no pitch) |
| N/A | N/A | `sndStartDopplerPitched` | sounds.c | Full Doppler with pitch |
| N/A | N/A | `sndUpdateMoobEngine` | sounds.c | Update drone engine pos |
| N/A | N/A | `sndChangePosition` | sounds.c | Move sound source |
| N/A | N/A | `sndChangeVelocity` | sounds.c | Change source velocity |

## Sound Effect IDs (GUTS Commands)

### System Commands (0x8000+)

| ID | N64 Define | Arcade Define | Description |
|----|------------|---------------|-------------|
| 0x8000 | `S_STOP_ALL` | `S_STOP_ALL_SNDS` | Stop all sounds |
| 0x8001 | `S_ATTRACT_MODE` | `S_ATTRACT_MODE` | Attract volume mode |
| 0x8002 | `S_GAME_MODE` | `S_GAME_MODE` | Game volume mode |
| 0x8006 | `S_SET_GAME_VOL` | `S_SET_GAME_VOL` | Set game volume |
| 0x8007 | `S_SET_ATTR_VOL` | `S_SET_ATTR_VOL` | Set attract volume |
| 0x8019 | N/A | `S_MOOB_LISTENER` | Set listener position |
| 0x801A | N/A | `S_MOOB_KILL_ALL` | Kill all MOOBs |
| 0x801B | N/A | `S_MOOB_KILL_OBJECT` | Kill MOOB by handle |
| 0x8022 | N/A | `S_ENGINE_STOP` | Stop engine sound |
| 0x8023 | N/A | `S_ENGINE_CHANGE_PF` | Engine pitch/filter |
| 0x8024 | N/A | `S_ENGINE_CHANGE_PFV` | Engine pitch/filter/vol |
| 0x8025 | N/A | `S_ENGINE_CHANGE_AUTO` | Auto engine speed |
| 0x8026 | N/A | `S_ENGINE_CRASH` | Engine crash sound |

### Engine Types (APM Models)

| ID | Define | Description |
|----|--------|-------------|
| 0x8033 | `S_AUDREY` | VW-style engine |
| 0x8034 | `S_NSX` | NSX-style engine |
| 0x8035 | `S_GHR` | Standard high-load |
| 0x8036 | `S_TPM` | TPM engine model |
| 0x8031 | `S_M_MULTIIDLE` | Multi-idle (drones) |

### Game Sounds

| ID | Define | Description |
|----|--------|-------------|
| 0x803C | `S_WINNER` | Winner music |
| 0x803D | `S_KWINNER` | Kill winner music |
| 0x8040 | `S_EXPLO` | Explosion |
| 0x8041 | `S_KEXPLO` | Kill explosion |
| 0x8046 | `S_SELECT` | Track select music |
| 0x8047 | `S_KSELECT` | Kill select music |
| 0x804B | `S_CARSELECT` | Car select music |
| 0x804C | `S_KCARSELECT` | Kill car select |
| 0x8083 | `S_CAR_LANDS` | Car landing sound |
| 0x8084 | `S_CHKPNTSTATIC` | Checkpoint sound |
| 0x8086 | `S_BOG` | Engine bog |
| 0x8087 | `S_KBOG` | Kill engine bog |
| 0x8088 | `S_LEADERLIGHT` | First place sound |
| 0x8089 | `S_KLEADERLIGHT` | Kill first place |
| 0x808E | `S_TURNTABLE` | Car turntable loop |
| 0x808F | `S_KTURNTABLE` | Kill turntable |
| 0x8090 | `S_COIN1` | Partial credit |
| 0x8091 | `S_COIN2` | Full credit |
| 0x8092 | `S_THREE` | Countdown 3 |
| 0x8093 | `S_TWO` | Countdown 2 |
| 0x8094 | `S_ONE` | Countdown 1 |
| 0x8095 | `S_RUSH`/`S_GO` | Countdown GO |

### Tire/Road Sounds

| ID | Define | Description |
|----|--------|-------------|
| 0x806C | `S_SKID_CUSTOM1` | Custom skid sound |
| 0x806D | `S_SKID_CUSTOM2` | Custom skid sound 2 |
| 0x806E | `S_WINDNOISE` | Wind noise loop |
| 0x806F | `S_ROADNOISE` | Road noise loop |
| 0x8070 | `S_GRAVELNOISE` | Gravel/dirt noise |
| 0x8071 | `S_WATERROAD` | Water driving |
| 0x8072 | `S_PEELOUT` | Tire peelout |
| 0x8073 | `S_SQUEAL_LEFT` | Left tire squeal |
| 0x8074 | `S_SQUEAL_RIGHT` | Right tire squeal |

### Collision Sounds

| ID | Define | Description |
|----|--------|-------------|
| 0x8075 | `S_CARBUMP` | Small collision |
| 0x8076 | `S_CARSCRAPE` | Scraping sound |
| 0x8077 | `S_CARSMASH` | Medium collision |
| 0x8078 | `S_CURBWHUMP` | Curb hit |
| 0x8079 | `S_BOTTOMOUT` | Bottoming out |
| 0x807D | `S_SCRAPELOOP` | Scrape loop |
| 0x807E | `S_KSCRAPELOOP` | Kill scrape loop |
| 0x8082 | `S_BOOM` | Bass boom (subwoofer) |

## N64 Audio Hardware Registers

### Audio Interface (AI) - 0xA4500000

```c
#define AI_DRAM_ADDR_REG    (*(vu32 *)0xA4500000)  /* DMA source address */
#define AI_LEN_REG          (*(vu32 *)0xA4500004)  /* DMA length */
#define AI_CONTROL_REG      (*(vu32 *)0xA4500008)  /* Control register */
#define AI_STATUS_REG       (*(vu32 *)0xA450000C)  /* Status register */
#define AI_DACRATE_REG      (*(vu32 *)0xA4500010)  /* DAC rate divider */
#define AI_BITRATE_REG      (*(vu32 *)0xA4500014)  /* Bit rate divider */
```

### DAC Rate Calculation

```c
/* osAiSetFrequency calculation */
dacRate = osClockRate / frequency;  /* Clock = 46.875 MHz */
bitRate = dacRate / 66;             /* Max 16 */

AI_DACRATE_REG = dacRate - 1;
AI_BITRATE_REG = bitRate - 1;

/* Actual frequency = osClockRate / dacRate */
```

### N64 Audio Thread

The audio thread runs separately from the main game thread:

```c
/* Audio thread priority typically 12 (higher than game at 10) */
/* Communication via message queues */
extern void *audio_msg_queue;  /* 0x801597D0 */

void audio_msg_send(void) {
    osJamMesg(audio_msg_queue, NULL, 0);  /* OS_MESG_NOBLOCK */
}
```

## Engine Sound Generation

### N64 Implementation

```c
typedef struct EngineSoundState {
    f32     rpm;        /* Current RPM */
    f32     pitch;      /* Pitch modifier (0.5 - 2.0) */
    u8      channel;    /* Audio channel */
    u8      active;     /* Sound active flag */
} EngineSoundState;

void sound_update_engine(s32 car_index, f32 rpm, f32 throttle) {
    /* Calculate pitch from RPM (idle ~1000, redline ~8000) */
    pitch = 0.5f + (rpm / 8000.0f) * 1.5f;

    /* Clamp pitch */
    if (pitch < 0.5f) pitch = 0.5f;
    if (pitch > 2.0f) pitch = 2.0f;

    /* Volume from throttle (80-127 range) */
    volume = 80.0f + throttle * 47.0f;

    /* Apply via N64 audio library */
    /* alSndpSetPitch(channel, pitch); */
    /* alSndpSetVol(channel, (s16)volume); */
}
```

### Arcade Implementation (carsnd.c)

```c
/* Engine types indexed by which_engine variable */
const U16 own_engine[NUM_ENGINES][2] = {
    {S_AUDREY, 0},   /* VW engine */
    {S_GHR, 0},      /* Standard engine */
    {S_NSX, 0},      /* NSX engine */
    {S_TPM, 0},      /* TPM engine */
};

void DoEngineSound(void) {
    U16 clip_rpm;
    S16 clip_torque;

    /* Scale RPM with clipping */
    clip_rpm = (abs(rpm) * 0.8);  /* 80% scale */
    if (clip_rpm > 9000) clip_rpm = 9000;

    /* Get torque from physics */
    clip_torque = model[this_node].engtorque;

    /* Send to CAGE DSP */
    if (coast_flag)
        sndUpdateManualEngine(0, 0);
    else
        sndUpdateManualEngine(clip_rpm, clip_torque);
}
```

## Arcade Sound Queue System

The arcade uses a command queue for audio:

```c
#define Q_SIZE 4096
#define Q_MASK (Q_SIZE-1)

typedef struct sndEl {
    S16 d[16];    /* Up to 16 words per command */
    U8  cnt;      /* Word count */
} sndEl;

typedef struct sndQueue {
    U32 head;
    U32 tail;
    sndEl q[Q_SIZE];
} sndQueue;

/* Transfer commands to hardware when ready */
#define AUDIO_HW_READY ( (aud_qlen()) < (0xd000) )

void sndQXfer(void) {
    while (q.tail != q.head && AUDIO_HW_READY) {
        sndEl *el = &q.q[q.tail];
        AUDIO_HW_SEND(el->d, el->cnt);
        q.tail = (q.tail + 1) % Q_SIZE;
    }
}
```

## Data Structures

### N64 Sound Handle (Decompiled)

```c
typedef struct N64SoundHandle {
    struct N64SoundHandle *next;   /* 0x00: Chain pointer */
    void *field_04;                /* 0x04: Unknown */
    u8   pad_08[0x2C];             /* 0x08-0x33: Padding */
    u16  state_index;              /* 0x34: State array index */
    u8   pad_36[6];                /* 0x36-0x3B: Padding */
    struct N64SoundHandle *linked; /* 0x3C: Linked sounds */
} N64SoundHandle;  /* 0x40 bytes */

typedef struct N64SoundState {
    u8   pad[22];    /* 0x00-0x15 */
    u8   status;     /* 0x16: 0=free, 1=playing, 2=stopped */
    u8   pad2[9];    /* 0x17-0x1F */
} N64SoundState;     /* 0x20 bytes per entry */
```

### Arcade Sound State

```c
typedef struct snd_state {
    S16  volume;
    S16  pitch;
    U16  state_var;
    BOOL on;
} snd_state;

typedef struct bump_state {
    F32  peak;
    F32  peak_vec[3];
    S32  time;
    BOOL bump_it;
} bump_state;

typedef struct skid_data {
    S32  force_tab[5];    /* Force thresholds */
    S32  pitch_h, pitch_l; /* Pitch range */
    S32  vol_h, vol_l;     /* Volume range */
    S16  gr_skid_thresh;   /* Skid mark threshold */
    S16  gr_smoke_thresh;  /* Smoke threshold */
} skid_data;
```

## Key Differences

### Platform Differences

| Aspect | N64 | Arcade |
|--------|-----|--------|
| Audio Hardware | AI DMA + RSP | CAGE DSP board |
| Sample Rate | 22.05/44.1 kHz | Variable |
| Channels | 16 software | 32+ hardware |
| 3D Audio | Software pan | Hardware MOOB |
| Command Interface | Message queues | Direct memory |
| Engine Model | Simple pitch | APM synthesis |

### Implementation Notes

1. **N64 Simplifications**:
   - No MOOB (Motion Object) system - uses simple pan
   - Simpler engine sound (pitch only, no torque modulation)
   - No hardware reverb - software approximation
   - Reduced concurrent sounds (16 vs 32+)

2. **Arcade Features Not in N64**:
   - Full Doppler effect with velocity
   - Hardware reverb with configurable parameters
   - Multiple engine sound models (APM)
   - Per-tire individual skid sounds
   - 3D positional audio with distance attenuation

3. **N64-Specific Features**:
   - Audio thread with message queue synchronization
   - Double-buffered DMA for audio output
   - Integration with libultra audio library

## Decompilation Progress

### Completed Functions

| Function | Address | Status |
|----------|---------|--------|
| `sound_handle_stop` | 0x800B358C | Decompiled |
| `osAiSetNextBuffer` | 0x8000BE70 | Decompiled |
| `osAiSetFrequency` | 0x8000BF00 | Decompiled |
| `audio_msg_send` | 0x8008A6D0 | Decompiled |
| `sound_init` | N64 | Stub complete |
| `sound_play` | N64 | Stub complete |
| `sound_stop` | N64 | Stub complete |
| `sound_update_engine` | N64 | Stub complete |
| `SOUND` | N64 | Wrapper complete |

### TODO Functions

| Function | Address | Priority |
|----------|---------|----------|
| `audio_control_update` | 0x800B37E8 | High |
| `audio_task_sync` | 0x8008A710 | Medium |
| `sound_start` | Unknown | High |
| `audio_sync` | Unknown | Medium |
| Engine sound integration | Various | Medium |

## References

### Source Files

**N64:**
- `/home/cburnes/projects/rush2049-decomp/src/game/sound.c` - Sound system implementation
- `/home/cburnes/projects/rush2049-decomp/src/game/audio.c` - Audio thread interface
- `/home/cburnes/projects/rush2049-decomp/include/game/sound.h` - Sound definitions
- `/home/cburnes/projects/rush2049-decomp/src/libultra/os_ai.c` - AI hardware interface

**Arcade Reference:**
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/sounds.c` - Main sound system
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/carsnd.c` - Car sound effects
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/sounds.h` - Sound command IDs
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/carsnd.h` - Car sound structures
