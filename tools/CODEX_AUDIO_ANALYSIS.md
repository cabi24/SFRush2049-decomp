# Codex Task: Audio System Deep Analysis

## Objective
Map the N64 audio system to the arcade audio implementation, focusing on engine sounds, positional audio, and music/radio systems.

## Background
The N64 uses the N64 Audio Library (libaudio) while the arcade uses custom 3dfx audio. Despite different hardware, the high-level audio logic should be similar.

---

## Task 1: Engine Sound System

### Arcade Files to Analyze
- `reference/repos/rushtherock/game/carsnd.c` (33,232 bytes) - Car audio
- `reference/repos/rushtherock/game/sounds.c` - Sound command dispatch

### Key Arcade Functions
```c
void InitCarSnds(S16 drone_index)    // Init per-car sounds
void DoCarSounds(S16 update, S8 skids_only)  // Update car audio
void StartEngineSound(void)          // Start engine loop
void DoEngineSound(void)             // Update engine pitch/volume
void StopEngineSound(void)           // Stop engine
void DoTireSqueals(S16 drone_index)  // Tire squeal audio
```

### N64 Functions to Map
- `engine_sound_update`
- `tire_squeal` / `wind_sound`
- `sound_play` / `sound_stop`
- `sound_pitch_set` / `sound_volume_set`

### Deliverable
Create `docs/audio_engine_mapping.md` with:
1. Engine RPM to pitch conversion
2. Load-based volume modulation
3. Multi-car engine sound mixing
4. Doppler effect implementation (if any)

---

## Task 2: Positional Audio System

### Arcade Files
- `reference/repos/rushtherock/game/sounds.c` - 3D audio

### Key Functions
```c
void sndListenerUpdate(S32 x, S32 y, U16 vel, U16 vel_angle, U16 facing)
void sndStartPosn(...)              // Start positional sound
void sndChangePosn(...)             // Update sound position
void sndDoDopp(...)                 // Doppler calculation
```

### N64 Functions
- `sound_position_set` / `sound_z_set`
- `sound_start_3d` / `sound_distance`
- `sound_attenuation` / `sound_reverb_set`

### Deliverable
Create `docs/audio_positional_mapping.md` with:
1. Listener position updates
2. Distance attenuation curve
3. Pan calculation
4. Reverb zone system

---

## Task 3: Music/Radio System

### Arcade Functions
```c
void music(S16 mode, U8 radio_station)
void StartRadio(U8 station)
void StopRadio(U8 station)
```

### N64 Functions
- `music_start` / `music_stop` / `music_volume`
- Audio streaming system

### Deliverable
Create `docs/audio_music_mapping.md` with:
1. Radio station system
2. Music state machine
3. Streaming vs sequenced music
4. Volume ducking during gameplay

---

## Task 4: Sound Effect Categories

### Analyze and Categorize
From arcade `sounds.c`, identify sound effect categories:
- Collision sounds (impacts, scrapes)
- Checkpoint/lap sounds
- Menu sounds
- Ambient sounds (wind, gravel, water)
- Voice samples ("Final lap!", "Checkpoint!")

### N64 Functions
- `sfx_play` / `sfx_stop`
- `voice_play` / `voice_stop`
- `sound_effect_play`

### Deliverable
Create `docs/audio_sfx_catalog.md` with:
1. Complete sound effect ID list
2. Category mapping
3. Trigger conditions
4. Priority system

---

## Task 5: Audio Architecture Comparison

### Compare Overall Systems
- Arcade: Custom 3dfx audio middleware
- N64: Nintendo Audio Library (libaudio/libmus)

### Document
- Message queue architecture (`audio_msg_queue`)
- Audio thread synchronization
- Buffer management
- Voice/channel allocation

### Deliverable
Create `docs/audio_architecture.md` with:
1. Audio thread diagram
2. Message types and handlers
3. Voice allocation strategy
4. Sync with video frame

---

## Analysis Commands

```bash
# Find audio functions in game.c
grep -n "sound_\|audio_\|music_\|voice_\|sfx_" src/game/game.c | head -50

# Check audio.c and sound.c
cat src/game/audio.c
cat src/game/sound.c

# Find audio-related externs
grep -n "extern.*sound\|extern.*audio\|extern.*music" src/game/game.c
```

## Success Criteria
- Engine sound algorithm fully documented
- At least 30 sound effect IDs cataloged
- Positional audio math documented
- Audio thread architecture diagram
- N64 libaudio integration points identified
