# Implementation Plan: Rush 2049 N64 Decompilation

**Branch**: `main` | **Date**: 2025-12-06 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/spec.md`

## Summary

Decompile San Francisco Rush 2049 for Nintendo 64 into matching C source code, leveraging the Rush The Rock arcade source as primary reference. The project will produce a build system that compiles C code into a byte-identical ROM, with comprehensive documentation linking N64 code to arcade equivalents.

## Technical Context

**Language/Version**: C89/C90 (IDO compiler compatible), Python 3.6+ (build scripts)
**Primary Dependencies**: IDO 5.3/7.1 compiler, MIPS binutils, splat, asm-processor
**Storage**: File-based (ROM, extracted assets, source code)
**Testing**: SHA-1 hash verification, asm-differ for function matching
**Target Platform**: Nintendo 64 (MIPS R4300i, 4MB RAM, Reality Co-Processor)
**Project Type**: Single project (decompilation)
**Performance Goals**: Build in <5 minutes, match 100% of functions
**Constraints**: Must use original compiler quirks, must match byte-for-byte
**Scale/Scope**: ~25,000-35,000 lines of game code (estimated from arcade source)

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

| Principle | Status | Notes |
|-----------|--------|-------|
| I. Matching First | ✅ PASS | Build system designed for matching verification |
| II. Arcade Source as Rosetta Stone | ✅ PASS | Reference repos cloned, cross-referencing planned |
| III. Progressive Disclosure | ✅ PASS | NON_MATCHING mode supported, GLOBAL_ASM for stubs |
| IV. Platform-Aware Portability | ✅ PASS | Three-tier categorization documented in plan |
| V. Documentation as Artifact | ✅ PASS | Comment requirements in code style guide |

## Project Structure

### Documentation

```text
.specify/
├── memory/
│   └── constitution.md      # Project principles
├── specs/
│   ├── spec.md              # Feature specification
│   ├── plan.md              # This implementation plan
│   └── tasks.md             # Task breakdown (next step)
└── templates/               # Spec Kit templates

reference/
├── n64-decomp-projects.md   # N64 decomp project list
├── lessons-learned.md       # Patterns from other projects
└── repos/                   # Cloned reference projects
    ├── rushtherock/         # ARCADE SOURCE (primary reference)
    ├── sm64/                # Build system reference
    ├── perfect_dark/        # CI/CD reference
    ├── mk64/                # Racing game patterns
    └── banjo-kazooie/       # 100% complete reference
```

### Source Code (Target Structure)

```text
rush2049/
├── src/
│   ├── racing/              # Vehicle physics, AI, paths
│   │   ├── cars.c           # Vehicle dynamics (arcade: cars.c)
│   │   ├── checkpoint.c     # Lap tracking (arcade: checkpoint.c)
│   │   ├── collision.c      # Collision detection (arcade: collision.c)
│   │   ├── drones.c         # AI behavior (arcade: drones.c)
│   │   ├── maxpath.c        # Racing lines (arcade: maxpath.c)
│   │   └── tires.c          # Tire physics (arcade: tires.c)
│   ├── game/                # Game state machine
│   │   ├── game.c           # Main loop (arcade: game.c)
│   │   ├── menu.c           # UI/menus
│   │   ├── attract.c        # Attract mode (arcade: attract.c)
│   │   └── hiscore.c        # High scores (arcade: hiscore.c)
│   ├── camera/              # Camera system (arcade: camera.c)
│   ├── audio/               # Sound system (N64-specific)
│   ├── rendering/           # Graphics (N64-specific, RDP)
│   └── os/                  # Platform layer
├── asm/
│   ├── entry.s              # ROM header, entrypoint
│   └── nonmatchingfunc.s    # GLOBAL_ASM stubs
├── courses/                 # Per-track data
│   ├── track_1/
│   ├── track_2/
│   └── ...
├── include/
│   ├── types.h              # Common type definitions
│   ├── macros.h             # GLOBAL_ASM, alignment macros
│   └── variables.h          # Global variable declarations
├── lib/                     # libultra SDK code
├── assets/                  # Extracted textures, models, audio
├── data/                    # Binary data segments
├── sound/                   # Audio sequences, instruments
├── tools/
│   ├── asm-processor/       # GLOBAL_ASM support
│   ├── ido-recomp/          # IDO compiler
│   ├── splat/               # ROM extraction
│   └── scripts/             # Build helpers
├── ld/
│   └── rush2049.ld          # Linker script
├── Makefile                 # Main build system
├── Makefile.split           # Splat integration
├── .clang-format            # Code style
├── progress.py              # Progress tracking
├── diff.py                  # Function diff tool
└── *.sha1                   # ROM hash verification
```

**Structure Decision**: Follows MK64/SM64 conventions with racing-specific organization. The `src/racing/` directory mirrors arcade source structure for easy cross-referencing.

## Phased Implementation

### Phase 0: Infrastructure (Week 1-2)

**Goal**: Build system that produces a valid (non-matching) ROM skeleton.

**Deliverables**:
1. Makefile with VERSION support (copy from MK64)
2. ROM extraction via splat (config.yaml)
3. Linker script for memory layout
4. GLOBAL_ASM support via asm-processor
5. Progress tracking script
6. CI/CD pipeline (GitHub Actions)

**Verification**: `make VERSION=us NON_MATCHING=1` produces bootable ROM

### Phase 1: Core Analysis (Week 3-4)

**Goal**: Map ROM structure, identify all functions, categorize by arcade equivalent.

**Deliverables**:
1. Complete ROM segment map
2. Function list with arcade cross-references
3. Data structure documentation
4. Compression format analysis
5. Asset extraction for all tracks

**Verification**: Documentation covers 100% of ROM address space

### Phase 2: Game State Machine (Week 5-6)

**Goal**: Decompile game loop and state transitions (most portable code).

**Target Files**:
- `src/game/game.c` (arcade: game.c - 1,687 LOC)
- `src/game/globals.c` (arcade: globals.c)
- State machine enum and transitions

**Verification**: Game boots to menu, state transitions work

### Phase 3: Checkpoint & Race Logic (Week 7-8)

**Goal**: Decompile racing rules, lap tracking, timing.

**Target Files**:
- `src/racing/checkpoint.c` (arcade: checkpoint.c - 1,694 LOC)
- `src/racing/cpinit.c` (arcade: cpinit.c)
- Track data loading

**Verification**: Laps count correctly, checkpoints trigger

### Phase 4: AI System (Week 9-10)

**Goal**: Decompile drone behavior and racing lines.

**Target Files**:
- `src/racing/drones.c` (arcade: drones.c - ~24K LOC)
- `src/racing/maxpath.c` (arcade: maxpath.c - 3,730 LOC)

**Verification**: AI cars race, follow paths, vary by difficulty

### Phase 5: Vehicle Physics (Week 11-14)

**Goal**: Decompile vehicle dynamics (most complex).

**Target Files**:
- `src/racing/cars.c` (arcade: cars.c - 1,892 LOC)
- `src/racing/tires.c` (arcade: tires.c - 528 LOC)
- `src/racing/collision.c` (arcade: collision.c - 18K LOC)

**Verification**: Cars handle realistically, physics match arcade behavior

### Phase 6: Camera System (Week 15-16)

**Goal**: Decompile camera modes and transitions.

**Target Files**:
- `src/camera/camera.c` (arcade: camera.c - 2,043 LOC)

**Verification**: All camera modes work correctly

### Phase 7: Audio & Rendering (Week 17-20)

**Goal**: Decompile N64-specific systems.

**Target Files**:
- `src/audio/*` (N64-specific, reference libultra)
- `src/rendering/*` (N64-specific, RDP commands)

**Verification**: Sound plays, graphics render correctly

### Phase 8: Matching & Polish (Week 21+)

**Goal**: Achieve 100% matching.

**Activities**:
- Replace all GLOBAL_ASM with matching C
- Verify all versions (US, EU, JP if applicable)
- Complete documentation
- Final CI/CD verification

**Verification**: `make test` passes for all versions

## Key Technical Decisions

### Decision 1: IDO Compiler via Static Recompilation

**Choice**: Use ido-static-recomp from SM64 project
**Rationale**: Required for byte-matching; GCC produces different assembly
**Alternative Rejected**: Native IRIX (unavailable on modern systems)

### Decision 2: splat for ROM Extraction

**Choice**: Use splat library for ROM analysis and splitting
**Rationale**: Industry standard for N64 decomp, generates Makefile integration
**Alternative Rejected**: Manual extraction (error-prone, not reproducible)

### Decision 3: MK64 as Primary Structural Reference

**Choice**: Mirror MK64 project structure and patterns
**Rationale**: Same genre (racing), proven approach, similar complexity
**Alternative Rejected**: SM64 structure (different game type)

### Decision 4: Three-Tier Portability Classification

**Choice**: Categorize all code as Portable/Adaptation/Rewrite
**Rationale**: Focuses effort on what matters, leverages arcade source
**Alternative Rejected**: Treat all code equally (wastes effort on rewrites)

## Risks & Mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| Physics doesn't match | High | High | Start with arcade, use permuter, accept GLOBAL_ASM |
| Unknown compression | Medium | Medium | Study MK64 mio0, reverse-engineer if needed |
| N64 version heavily modified | Medium | High | Accept differences, document thoroughly |
| Contributor burnout | Medium | Medium | Track progress, celebrate milestones |
| IDO compiler issues | Low | Critical | Use proven ido-static-recomp, test early |

## Toolchain Summary

| Tool | Source | Purpose |
|------|--------|---------|
| ido-static-recomp | SM64 repo | Run IDO compiler on Linux/Mac |
| asm-processor | SM64 repo | GLOBAL_ASM support |
| splat | ethteck/splat | ROM extraction |
| asm-differ | simonlindholm | Function diff |
| armips | ARM9/armips | MIPS assembler |
| progress.py | MK64 repo (adapt) | Progress tracking |
| diff.py | SM64 repo | Interactive diff |

## Next Steps

1. Run `/speckit.tasks` to generate detailed task breakdown
2. Set up GitHub repository with initial structure
3. Obtain Rush 2049 N64 ROM for analysis
4. Begin Phase 0: Infrastructure
