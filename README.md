# San Francisco Rush 2049 (N64) Decompilation

A work-in-progress decompilation of San Francisco Rush 2049 for the Nintendo 64.

## Status

| Component | Status |
|-----------|--------|
| Build System | Matching build achieved |
| ROM Extraction | Complete (88 code files) |
| Symbol Identification | 228 functions identified |
| libultra | ~70 functions decompiled |
| libm/libc | ~18 functions decompiled |
| Game Code | ~752 functions extracted |
| Overall | Early decompilation phase |

## Project Goals

1. **Matching Decompilation**: Produce C source code that compiles to a byte-identical ROM
2. **Documentation**: Thoroughly document game systems with arcade source cross-references
3. **Moddability**: Enable game modifications through readable source code

## Unique Advantage

This project leverages the **Rush The Rock arcade source code** as a Rosetta Stone. The N64 version shares significant code with the arcade, allowing us to:
- Identify function purposes without guesswork
- Use original variable and function names
- Understand algorithm intent directly

The arcade source contains ~97K lines of game code including physics, AI, and track logic that maps closely to the N64 version.

## Building

### Requirements

- MIPS cross-compiler (binutils-mips-linux-gnu)
- Python 3.6+
- Make

### Quick Start

```bash
# Place your legally obtained ROM
cp /path/to/rush2049.z64 baserom.us.z64

# Verify ROM hash
sha1sum -c us.sha1

# Extract assets (once build system is ready)
make extract

# Build (non-matching for now)
make VERSION=us NON_MATCHING=1

# Build matching (when ready)
make VERSION=us
```

## Project Structure

```
rush2049-decomp/
├── src/                    # Decompiled C source
│   ├── racing/            # Vehicle physics, AI, paths
│   ├── game/              # Game state, menus
│   ├── camera/            # Camera system
│   ├── audio/             # Sound (N64-specific)
│   └── rendering/         # Graphics (N64-specific)
├── asm/                    # Assembly stubs (GLOBAL_ASM)
├── include/                # Headers
├── courses/                # Per-track data
├── reference/              # Arcade source & other decomps
│   ├── repos/rushtherock/ # Arcade source code
│   ├── repos/mk64/        # Mario Kart 64 reference
│   └── lessons-learned.md # Decomp best practices
├── tools/                  # Build tools
└── .specify/               # Project planning docs
```

## Documentation

- [Lessons Learned](reference/lessons-learned.md) - Patterns from other decomps

## ROM Information

| Version | SHA-1 | Status |
|---------|-------|--------|
| US | `3f99351d7bb61656614bdb2aa1a90cfe55d1922c` | Primary target |
| EU | Unknown | Not planned |
| JP | Unknown | Not planned |

## References

- [Rush The Rock Arcade Source](https://github.com/historicalsource/rushtherock) - Original game code
- [Super Mario 64 Decomp](https://github.com/n64decomp/sm64) - Build system reference
- [Mario Kart 64 Decomp](https://github.com/n64decomp/mk64) - Racing game patterns
- [Perfect Dark Decomp](https://github.com/n64decomp/perfect_dark) - CI/CD reference

## Legal

This project does not include copyrighted game assets or ROM files. You must provide your own legally obtained copy of the game.

## Credits

- Original game by Atari Games / Midway
- Decompilation tooling from the N64 decomp community
- Arcade source preservation by historicalsource
