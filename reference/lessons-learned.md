# N64 Decompilation Lessons Learned

Compiled from studying SM64, Perfect Dark, Mario Kart 64, Banjo-Kazooie, and the Rush The Rock arcade source.

---

## 1. Build System Essentials

### Must-Have Components
- **Make-based build system** (all major projects use this)
- **Multi-version support** from day one (US, EU, JP variants)
- **Environment variables** for version selection (e.g., `ROMID`, `VERSION`)
- **SHA-1/MD5 verification** of output ROMs
- **Asset extraction step** before compilation

### Build Pipeline Pattern
```bash
# Standard N64 decomp build flow
1. make extract    # Extract assets from original ROM
2. make tools      # Build custom tools
3. make assets     # Process extracted assets
4. make -j         # Compile source
5. make test       # Verify ROM matches original
```

### Compiler Considerations
- **IDO compiler** (5.3 or 7.1) required for matching builds
- **GCC fallback** for non-matching/functional builds
- **Per-file optimization flags** - different files need different -O levels
- **GLOBAL_ASM macro** for functions not yet decompiled

```c
// Pattern: Undecompiled functions use GLOBAL_ASM
#ifdef NON_MATCHING
void my_function(void) {
    // Functional C implementation
}
#else
GLOBAL_ASM("asm/my_function.s")
#endif
```

---

## 2. Directory Structure Pattern

All successful N64 decomps follow similar organization:

```
project/
├── src/              # Game C source (organized by subsystem)
│   ├── audio/
│   ├── engine/
│   ├── game/
│   └── [subsystems]/
├── asm/              # Handwritten assembly, ROM header
├── assets/           # Extracted assets (per-version)
├── bin/              # Segment ordering files
├── data/             # Binary data, scripts
├── include/          # Header files
├── lib/              # SDK code (libultra)
├── levels/ or courses/  # Level/track data
├── sound/            # Audio sequences, samples
├── textures/         # Texture assets
├── tools/            # Build tools and utilities
├── docs/             # Documentation
└── ld/               # Linker scripts (version-specific)
```

---

## 3. Matching Strategy

### The Matching Problem
Goal: Compiled C must produce **byte-identical** assembly to original ROM.

### Key Techniques

**1. GLOBAL_ASM for Stubborn Functions**
- Embed original MIPS assembly directly
- Use `asm-processor` tool to handle this
- Gradually replace with matching C

**2. Compiler Quirk Handling**
- IDO has specific optimization behaviors
- Loop unrolling control: `-Wo,-loopunroll,0`
- Inline control: `-sopt,-inline=function_name`
- Float constants need `.late_rodata` section handling

**3. Undefined Behavior**
- Original code often has UB that IDO handles consistently
- Use `AVOID_UB` flag for GCC builds
- Document UB instances for future reference

**4. Per-File Optimization**
From SM64 Makefile:
```makefile
# Different optimization per file
$(BUILD_DIR)/src/audio/%.o: OPT_FLAGS := -O2 -Wo,-loopunroll,0
$(BUILD_DIR)/lib/src/math/%.o: OPT_FLAGS := -O3
$(BUILD_DIR)/src/goddard/%.o: OPT_FLAGS := -g  # No optimization
```

---

## 4. Tools You'll Need

### Essential Custom Tools

| Tool | Purpose | Source |
|------|---------|--------|
| `asm-processor` | Embeds MIPS ASM in C via GLOBAL_ASM | SM64 |
| `ido-static-recomp` | Runs IRIX IDO compiler on Linux/Mac | SM64 |
| `mio0` / compression tools | ROM segment compression | All projects |
| `n64cksum` | ROM checksum calculation | All projects |
| `n64graphics` | PNG to N64 texture conversion | SM64 |
| `armips` | MIPS/RSP assembler | All projects |
| `diff.py` / `asm-differ` | Compare compiled output to original | SM64/MK64 |
| `first-diff.py` | Find first instruction mismatch | MK64 |
| `progress.py` | Track decompilation progress | MK64 |

### Diff Tool Workflow
```bash
# Iterative matching workflow
./diff.py function_name -m -w
# -m: auto-rebuild on changes
# -w: watch mode for live updates
# Iterate until assembly matches
```

---

## 5. Racing Game Specific Patterns (from MK64)

### Course/Track Data Structure
```c
struct CourseTable {
    u8* dlRomStart/End;      // Display list data (compressed)
    u8* vertexRomStart/End;  // Vertex positions (compressed)
    CourseVtx* vertexStart;  // Decompressed pointer
    u32 vertexCount;
    u32* textures;           // Texture pointer table
};
```

### Track Section System
- Tracks partitioned into sections for:
  - Level of detail (LOD) rendering
  - Direction-based culling
  - Collision mesh lookup
  - Split-screen optimization

### Path System (Critical for AI)
```c
// Multiple racing lines per track
struct PathPoint {
    Vec3f position;
    f32 width;
    u16 flags;
};

// AI uses paths for:
// - Navigation
// - Camera targeting
// - Lap detection
// - Ghost playback
```

### Physics Considerations
- Surface types affect traction
- Fixed-point math common for physics
- Collision is triangle mesh-based

---

## 6. Rush Arcade Source Insights

### Codebase Size
- ~97,000 lines of C in game/ directory
- ~6,500 lines of headers
- Professional arcade-quality codebase

### Key Subsystems (Files to Study)

| System | Files | Notes |
|--------|-------|-------|
| Physics | `cars.c`, `mdrive.c`, `tires.c` | Detailed vehicle dynamics |
| Collision | `collision.c` | Car-to-car, car-to-world |
| Camera | `camera.c` | Multiple view modes |
| Game State | `game.c`, `game.h` | State machine pattern |
| AI | `drones.c`, `maxpath.c` | Racing line computation |
| Checkpoints | `checkpoint.c` | Lap tracking, timing |
| Rendering | `dis.c`, `visuals.c` | 3D display, effects |
| Audio | `sounds.c`, `carsnd.c` | Sound management |

### Game State Machine
```c
typedef enum {
    ATTRACT,    // Demo/attract mode
    TRKSEL,     // Track selection
    CARSEL,     // Car selection
    PLAYGAME,   // Active race
    ENDGAME,    // Race finished
    GAMEOVER,   // Game over
    HISCORE,    // High score entry
} GState;
```

### Portable vs Platform-Specific

**Likely Portable to N64:**
- Game state machine (`game.c`)
- Checkpoint logic (`checkpoint.c`)
- AI/racing lines (`drones.c`, `maxpath.c`)
- Math utilities (`math.c`, `vecmath.c`)
- Camera system (`camera.c`)

**Needs Major Adaptation:**
- Rendering (`dis.c`, `visuals.c`) - 3dfx → N64 RCP
- Physics (`mdrive.c`) - Mathbox → CPU
- Audio (`sounds.c`) - Different format
- Controls (`controls.c`) - Arcade → Controller

---

## 7. Version Handling

### Multi-Version Support Pattern
```c
// Version constants
#define VERSION_US    0
#define VERSION_EU    1
#define VERSION_JP    2

// Conditional compilation
#if VERSION == VERSION_JP
    // Japan-specific code
#elif VERSION == VERSION_EU
    // Europe-specific code
#else
    // US default
#endif
```

### Linker Script Templating
Use C preprocessor on linker scripts:
```
#if VERSION >= VERSION_JP
  #include "segments.jp.inc"
#else
  #include "segments.us.inc"
#endif
```

---

## 8. Testing & Verification

### Segment-Level Matching
```bash
# Perfect Dark approach
make test
# Extracts segments, compares MD5 against known good
# Empty output = perfect match
```

### Progress Tracking
```bash
# MK64 progress.py output
$ python3 progress.py
src/racing:     45.2% [████████░░░░░░░░░░░░] 12,345/27,300 bytes
src/audio:      78.1% [███████████████░░░░░] 8,901/11,400 bytes
Total:          52.3%
```

---

## 9. Common Pitfalls

### Gotchas to Avoid

1. **Path length limit** - Keep repo paths under 255 characters
2. **Uninitialized memory** - Old compilers had consistent UB handling
3. **Compression magic values** - Original compression may use uninitialized data
4. **Float constant handling** - `.late_rodata` section critical for matching
5. **Piracy checks** - Some games have anti-tamper that needs preservation
6. **Segment memory layout** - Must match exactly or linker errors
7. **Toolchain versions** - IDO 5.3 vs 7.1 produce different output

### Memory Segmentation
N64 uses 16 segments (0-15) for address translation:
```c
#define SEGMENT_NUMBER(addr) ((u32)(addr) >> 24)
#define SEGMENT_OFFSET(addr) ((u32)(addr) & 0xFFFFFF)
#define VIRTUAL_TO_PHYSICAL(addr) \
    (gSegmentTable[SEGMENT_NUMBER(addr)] + SEGMENT_OFFSET(addr))
```

---

## 10. Documentation Best Practices

### What to Document

1. **Build options** and what each flag does
2. **Per-file compiler flags** (copy SM64's approach)
3. **Compiler quirks** specific to IDO
4. **Why code is written oddly** when matching original quirks
5. **Version differences** and why they exist
6. **Asset pipeline** from source to ROM

### Technical Deep Dives
Perfect Dark has excellent examples:
- `docs/ailists.md` - AI scripting system
- `docs/piracychecks.md` - Anti-piracy breakdown
- `docs/chrs.md` - Character system design

---

## 11. Recommended Approach for Rush 2049

### Phase 1: Infrastructure
1. Set up build system with version support
2. Create ROM extraction tools
3. Implement progress tracking
4. Set up diff tools for matching

### Phase 2: Analysis
1. Compare N64 ROM structure to arcade source
2. Identify shared functions between arcade and N64
3. Map N64 memory segments
4. Identify compression schemes used

### Phase 3: Core Systems
1. Start with game state machine (most portable)
2. Decompile checkpoint/race logic
3. Work on camera system
4. Tackle physics (hardest)

### Phase 4: Graphics & Audio
1. Understand N64 display list format
2. Adapt rendering from arcade
3. Handle audio format differences

### Leverage Points
- **Arcade source is GOLD** - Use it to name functions, understand logic
- **MK64 patterns** - Similar racing game structure
- **Project R work** - They've already mapped arcade ↔ binary

---

## Quick Reference Commands

```bash
# Clone essential tools
git clone https://github.com/simonlindholm/asm-differ
git clone https://github.com/ethteck/splat  # ROM splitting

# Build with matching
make VERSION=us NON_MATCHING=0 COMPARE=1 -j

# Build functional (faster iteration)
make VERSION=us NON_MATCHING=1 -j

# Check progress
python3 progress.py

# Diff a function
./diff.py function_name -m -w

# Find first mismatch
python3 first-diff.py build/us/rom.z64 baserom.us.z64
```

---

## Resources

- **decomp.me** - Online collaborative decompilation
- **SM64 Discord** - discord.gg/DuYH3Fh
- **N64 decomp org** - github.com/n64decomp
- **Retro Reversing** - retroreversing.com/n64-decompiling
- **N64brew Wiki** - n64brew.dev
