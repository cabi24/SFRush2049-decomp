# N64 Decompilation Projects Reference

A curated list of N64 decompilation projects for research and reference.

## Game Decompilations

| Game | Repository | Status |
|------|------------|--------|
| Aidyn Chronicles | https://github.com/blackgamma7/Aidyn | Unknown |
| Banjo Kazooie | https://github.com/n64decomp/banjo-kazooie | **100% Complete** |
| Blast Corps | https://github.com/retroplastic/blastcorps | Unknown |
| Body Harvest | https://github.com/jaytheham/body-harvest-decompilation | Unknown |
| Bomberman 64 | https://github.com/MarioMaster96/bomberman-64 | Unknown |
| Conker's Bad Fur Day | https://github.com/mkst/conker | Unknown |
| Dinosaur Planet | https://github.com/zestydevy/dinosaur-planet | Unknown |
| Goldeneye 007 | https://github.com/n64decomp/007 | ~71% |
| Kirby 64: The Crystal Shards | https://github.com/farisawan-2000/kirby64 | Unknown |
| Mario Kart 64 | https://github.com/n64decomp/mk64 | Active (660 commits) |
| Mario Party 3 | https://github.com/PartyPlanner64/mp3 | Unknown |
| Mischief Makers | https://github.com/Drahsid/mischief-makers | Unknown |
| Mystical Ninja | https://github.com/Fluvian/mnsg | Unknown |
| Neon Genesis Evangelion | https://github.com/farisawan-2000/evangelion | Unknown |
| Paper Mario | https://github.com/pmret/papermario | Unknown |
| Perfect Dark | https://github.com/n64decomp/perfect_dark | **Complete** (matching) |
| Pokemon Snap | https://github.com/ethteck/pokemonsnap | Unknown |
| Pokemon Stadium | https://github.com/ethteck/pokemonstadium | Unknown |
| Quest 64 | https://github.com/mallos31/quest64 | ~43% |
| Rocket: Robot on Wheels | https://github.com/RocketRet/Rocket-Robot-On-Wheels | Unknown |
| Snowboard Kids | https://github.com/tenry92/sbk-decomp | Unknown |
| Space Station Silicon Valley | https://github.com/mkst/sssv | Unknown |
| Super Mario 64 | https://github.com/n64decomp/sm64 | **Complete** (5 regions) |
| The Legend of Zelda: Majora's Mask | https://github.com/zeldaret/mm | **Complete** |
| The Legend of Zelda: Ocarina of Time | https://github.com/zeldaret/oot | **Complete** |
| Turok 3 | https://github.com/Drahsid/turok3 | Unknown |
| Ura Zelda | https://github.com/Decompollaborate/n64dd | Unknown |
| Yoshi's Story | https://github.com/decompals/yoshis-story | Unknown |

## Graphics Microcodes

| Microcode | Repository | Notes |
|-----------|------------|-------|
| F3DEX2 | https://github.com/Mr-Wiseguy/f3dex2 | Standard N64 graphics microcode |
| Turbo3D | https://github.com/farisawan-2000/turbo3d | Alternative microcode |

---

## Rush 2049 Specific Resources

### San Francisco Rush: The Rock - ORIGINAL SOURCE CODE

| Resource | URL | Notes |
|----------|-----|-------|
| Rush The Rock Source | https://github.com/historicalsource/rushtherock | **ARCADE SOURCE CODE** - C (95.4%), Assembly (3.8%) |
| Project R (PC Port) | https://t3hd0gg.com/project-r/ | Native PC port using static recompilation |

### Key Findings

**Rush The Rock Source Code Structure:**
- `GUTS/` - Core game logic/engine
- `LIB/` - Library files
- `MB/` - Motherboard/main binary components
- `OS/` - Operating system/platform-specific code
- `game/` - Game-specific assets and implementations

**Why This Matters:**
- Rush 2049 shares the same engine as Rush: The Rock
- Having actual source code (not decompiled) is a MASSIVE advantage
- We can study the arcade codebase to understand N64 port differences
- Function names, data structures, game logic already documented

**Project R Technical Stack:**
- Uses Vulkan 1.2 for graphics
- SDL3 for cross-platform support
- Static recompilation approach (not decompilation)
- Supports Rush: The Rock AND Rush 2049

---

## Toolchain & Build Systems (from research)

### Common Tools Across Projects

| Tool | Purpose |
|------|---------|
| Make | Build system (all projects use this) |
| mips-linux-gnu-gcc | Cross-compiler for MIPS |
| binutils-mips-linux-gnu | MIPS assembler/linker |
| Python 3.6+ | Build scripts, asset extraction |
| splat | ROM splitting tool |
| asm-differ | Compare compiled output to original |
| armips | MIPS assembler |

### Build Pattern (from SM64/MK64/Perfect Dark)

```bash
# 1. Place base ROM
cp baserom.z64 ./

# 2. Extract assets
make extract

# 3. Build
make -j

# 4. Verify match
make test
```

### Mario Kart 64 - Racing Game Patterns

Useful structure for Rush 2049:
- `/src/actors/` - Individual game entities
- `/courses/` - Track data, display lists, ghosts
- Multi-version support via SHA-1 verification
- Debug flags: `DEBUG=1`, `AVOID_UB=1`

---

## Research Queue

Priority order for Rush 2049 N64 decomp:

1. [x] Rush The Rock source code - **FOUND! This is gold**
2. [ ] Project R - Study how they used the arcade source
3. [ ] Mario Kart 64 - Racing game decomp patterns
4. [ ] Perfect Dark - CI/CD and matching workflow
5. [ ] SM64 - Best documented, toolchain reference
6. [ ] F3DEX2 microcode - Graphics pipeline understanding

---

## Community Resources

- SM64 Discord: discord.gg/DuYH3Fh
- N64 decomp organization: https://github.com/n64decomp
- decomp.me - Online collaborative decompilation
- Retro Reversing: https://www.retroreversing.com/n64-decompiling
