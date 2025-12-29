# Homework 003: Catalog Arcade Sound Effect IDs

## Objective
Create a complete catalog of sound effect IDs from the arcade source to map against N64 audio calls.

## Context
The N64 game.c has functions like `sound_play(id)`, `sfx_play(id)`, `voice_play(id)`. We need to know what each ID corresponds to.

## Tasks
1. Search arcade source for sound definitions:
   ```bash
   grep -rn "SND_\|SOUND_\|SFX_" reference/repos/rushtherock/game/*.h
   grep -rn "#define.*[Ss]ound" reference/repos/rushtherock/game/*.h
   ```

2. Analyze these files:
   - `reference/repos/rushtherock/game/sounds.c`
   - `reference/repos/rushtherock/game/carsnd.c`
   - Any `*.h` files with sound enums/defines

3. Categorize sounds:
   - Engine sounds (idle, rev, shift)
   - Collision sounds (impact, scrape, crash)
   - Tire sounds (squeal, screech, gravel)
   - UI sounds (menu, select, confirm)
   - Voice samples ("Checkpoint!", "Final Lap!")
   - Ambient (wind, crowd, environment)
   - Music/radio

4. Find N64 sound calls in game.c:
   ```bash
   grep -n "sound_play\|sfx_play\|voice_play" src/game/game.c | head -30
   ```

## Deliverables
Create `docs/audio_sfx_catalog.md`:

```markdown
# Sound Effect Catalog

## Engine Sounds
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x01 | SND_ENGINE_IDLE | Engine idle loop | carsnd.c:XXX |

## Collision Sounds
[same format]

## Tire Sounds
[same format]

## UI Sounds
[same format]

## Voice Samples
[same format]

## Ambient Sounds
[same format]

## N64 Sound Call Analysis
| game.c Line | Function | ID Used | Likely Sound |
|-------------|----------|---------|--------------|

## Sound Trigger Conditions
[When each category plays]
```

## Reference Files
- `reference/repos/rushtherock/game/sounds.c`
- `reference/repos/rushtherock/game/carsnd.c`
- `reference/repos/rushtherock/game/*.h` (for defines)
- `src/game/game.c` (N64 calls)

## Success Criteria
- At least 50 sound effects cataloged
- All major categories covered
- At least 10 N64 sound calls mapped to likely IDs
