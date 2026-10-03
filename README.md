# San Francisco Rush 2049 (N64) Decompilation

A work-in-progress decompilation of San Francisco Rush 2049 for the Nintendo 64.

## Status

Verified cartridge coverage after integrating PRs #24 and #40:

| Component | Matching functions | Matching native bytes |
|-----------|-------------------:|----------------------:|
| Game code | 666 / 1,216 | 100,656 / 647,072 (15.56%) |
| Static code | 158 / 230 | 44,808 / 61,440 (72.93%) |

Combined byte tracker: **20.53%**. The source-built image, original compressed stream and complete ROM hash pass. Use `make progress` for current derived coverage.

Nonmatching reconstructions, behavioral evidence and compiler/context audits are preserved on master under `cloud/work/`. See the [research progress index](cloud/RESEARCH_INDEX.md) and [helper work queue](dot_handoff.md). Research progress earns matching coverage only after native-code and cartridge integration gates pass.

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
