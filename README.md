# San Francisco Rush 2049 (N64) Decompilation

A work-in-progress decompilation of San Francisco Rush 2049 for the Nintendo 64.

## Status

🚧 **Early Development** - Infrastructure setup in progress

| Component | Status |
|-----------|--------|
| Build System | 🔴 Not Started |
| ROM Extraction | 🔴 Not Started |
| Game Loop | 🔴 Not Started |
| Physics | 🔴 Not Started |
| Overall Matching | 0% |

## Project Goals

1. **Matching Decompilation**: Produce C source code that compiles to a byte-identical ROM
2. **Documentation**: Thoroughly document game systems with arcade source cross-references
3. **Moddability**: Enable game modifications through readable source code

## Unique Advantage

This project leverages the **Rush The Rock arcade source code** (available at `reference/repos/rushtherock/`) as a Rosetta Stone. The N64 version shares significant code with the arcade, allowing us to:
- Identify function purposes without guesswork
- Use original variable and function names
- Understand algorithm intent directly

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

- [Constitution](.specify/memory/constitution.md) - Project principles
- [Specification](.specify/specs/spec.md) - Goals and requirements
- [Plan](.specify/specs/plan.md) - Implementation phases
- [Tasks](.specify/specs/tasks.md) - Detailed task breakdown
- [Lessons Learned](reference/lessons-learned.md) - Patterns from other decomps

## ROM Information

| Version | SHA-1 | Status |
|---------|-------|--------|
| US | `f79223f8060a530d0dc8683a923c3c60615aa0a0` | Primary target |
| EU | Unknown | Not planned |
| JP | Unknown | Not planned |

## References

- [Rush The Rock Arcade Source](reference/repos/rushtherock/) - Original game code
- [Super Mario 64 Decomp](https://github.com/n64decomp/sm64) - Build system reference
- [Mario Kart 64 Decomp](https://github.com/n64decomp/mk64) - Racing game patterns
- [Perfect Dark Decomp](https://github.com/n64decomp/perfect_dark) - CI/CD reference

## Legal

This project does not include copyrighted game assets or ROM files. You must provide your own legally obtained copy of the game.

## Credits

- Original game by Atari Games / Midway
- Decompilation tooling from the N64 decomp community
- Arcade source preservation by historicalsource
