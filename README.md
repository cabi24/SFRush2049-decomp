# San Francisco Rush 2049 (N64) Decompilation

For the current state and how matching waves are run, start with [CLAUDE.md](CLAUDE.md) and the [frontier handoff](docs/plans/2026-10-06-frontier-handoff.md) §0. [PROJECT_PLAN.md](PROJECT_PLAN.md) is the 2026-10-03 audit plan and work packages; its figures are a dated snapshot.

A work-in-progress decompilation of San Francisco Rush 2049 for the Nintendo 64.

## Status

Cartridge coverage at master `7be1d67b` (2026-10-21). Every figure is gated by the built ROM's SHA-1, and
the four code populations are reported separately, never summed. Run `make progress` for live numbers.

| Population | Functions | Code bytes | Whole image |
|---|---:|---:|---:|
| Game code (DEFLATE image at ROM `0xB0CB10`, inflates to `0x80086A50`) | 1,165 / 1,216 | 509,680 / 561,860 (90.71%) | 78.77% of 647,072 B |
| Static boot/library code | 677 / 677 (634 C + 43 assembly) | 162,188 / 162,284 (99.94%; from C 95.12%) | — |
| Runtime image A (ROM `0xB5C534`, loads at `0x8038A400`) | 127 / 193 | 128,060 / 152,952 (83.73%) | 65.97% of 194,128 B |
| Runtime image B (ROM `0xB6FEC4`, loads at `0x8038A400`) | 41 / 49 | 36,272 / 38,708 (93.71%) | 82.65% of 43,888 B |

Static counts functions that were assembly in the original build as built from assembly source, apart from C ([static asm ceiling](docs/static-asm-ceiling.md)), including 10 hand-written routines the build composes into the ROM data segment (`pipeline.boot_data`). Code bytes leave out each image's `.data`/`.rodata` tail, which is proven to hold no code (see the
[memory map](docs/memory_map.md) §1.2 and `tests/conveyor/test_blob_data_tail.py`, `test_ovl_data_tail.py`).
The 51 unmatched game functions are listed in [docs/dot_handoff_targets.csv](docs/dot_handoff_targets.csv).

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
