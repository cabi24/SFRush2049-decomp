# Feature Specification: Rush 2049 N64 Decompilation

**Feature Branch**: `main`
**Created**: 2025-12-06
**Status**: Draft
**Input**: Decompile San Francisco Rush 2049 for Nintendo 64, leveraging arcade source code as reference

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Build Matching ROM (Priority: P1)

A contributor clones the repository, provides the original Rush 2049 N64 ROM, runs the build process, and produces a byte-identical ROM that can be verified against the original SHA-1 hash.

**Why this priority**: This is the core deliverable. Without a matching ROM, the decompilation is incomplete. Everything else depends on this foundation.

**Independent Test**: Can be tested by running `make VERSION=us && make test` and verifying SHA-1 match.

**Acceptance Scenarios**:

1. **Given** a fresh clone with baserom.us.z64 provided, **When** running `make VERSION=us`, **Then** build completes without errors
2. **Given** a completed build, **When** running `make test`, **Then** SHA-1 hash matches original ROM exactly
3. **Given** the matching ROM, **When** loaded in N64 emulator, **Then** game plays identically to original

---

### User Story 2 - Understand Game Logic via Readable Code (Priority: P1)

A researcher or modder reads the decompiled C source code to understand how the game's physics, AI, or rendering works, with clear comments linking to arcade source equivalents.

**Why this priority**: Equal to matching — the code must be understandable, not just correct. Undocumented matching code has limited value.

**Independent Test**: A new contributor can read `src/racing/cars.c` and understand vehicle physics without external documentation.

**Acceptance Scenarios**:

1. **Given** decompiled source code, **When** reading any function, **Then** comments explain purpose and link to arcade equivalent
2. **Given** a question about game mechanics, **When** searching codebase, **Then** relevant code is findable and comprehensible
3. **Given** arcade source reference, **When** comparing to decompiled N64 code, **Then** structural similarities are evident

---

### User Story 3 - Track Decompilation Progress (Priority: P2)

A project maintainer or contributor runs a progress command to see what percentage of the game is decompiled, which subsystems are complete, and what remains.

**Why this priority**: Progress visibility keeps contributors motivated and helps prioritize work.

**Independent Test**: Running `make progress` produces accurate statistics.

**Acceptance Scenarios**:

1. **Given** partial decompilation, **When** running `make progress`, **Then** percentage and byte counts displayed per subsystem
2. **Given** a newly matched function, **When** progress recalculated, **Then** percentage increases appropriately
3. **Given** GLOBAL_ASM stubs remaining, **When** viewing progress, **Then** stub count is visible

---

### User Story 4 - Modify and Rebuild Game (Priority: P2)

A modder modifies game code (e.g., changes physics parameters, adds features) and rebuilds a working ROM with their changes.

**Why this priority**: Modding capability is a key outcome of decompilation projects.

**Independent Test**: Change a constant in physics code, rebuild with `NON_MATCHING=1`, verify change in-game.

**Acceptance Scenarios**:

1. **Given** source code modification, **When** building with `NON_MATCHING=1`, **Then** ROM builds successfully
2. **Given** modified ROM, **When** running in emulator, **Then** modification is reflected in gameplay
3. **Given** multiple modifications, **When** rebuilding, **Then** all changes are incorporated

---

### User Story 5 - Contribute Matching Code (Priority: P2)

A contributor decompiles a new function, uses diff tools to achieve matching, documents it, and submits a pull request that passes CI.

**Why this priority**: Community contributions are essential for project completion.

**Independent Test**: Submit a PR with one newly matched function; CI passes; maintainers can review.

**Acceptance Scenarios**:

1. **Given** a function stub in GLOBAL_ASM, **When** contributor writes matching C, **Then** diff.py shows 0 differences
2. **Given** matching PR submitted, **When** CI runs, **Then** build succeeds and test passes
3. **Given** PR merged, **When** progress rechecked, **Then** percentage increased

---

### User Story 6 - Cross-Reference with Arcade Source (Priority: P3)

A researcher compares N64 code to arcade source to understand what was changed, simplified, or rewritten for the console port.

**Why this priority**: Understanding platform differences is valuable for preservation and education.

**Independent Test**: Documentation exists mapping N64 modules to arcade equivalents.

**Acceptance Scenarios**:

1. **Given** any N64 source file, **When** checking header comments, **Then** arcade equivalent is referenced
2. **Given** a platform-specific module, **When** reading documentation, **Then** differences from arcade are explained
3. **Given** a portable module, **When** comparing code, **Then** logic matches arcade closely

---

### Edge Cases

- What happens when building without baserom? → Clear error message with instructions
- What happens when IDO compiler unavailable? → Fallback to GCC with NON_MATCHING=1
- How does system handle partial asset extraction? → Build fails gracefully with specific error
- What happens when contributing a non-matching function? → CI flags it; contributor must use GLOBAL_ASM or iterate

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: Build system MUST produce byte-identical ROM when using IDO compiler with correct flags
- **FR-002**: Build system MUST support at least US version, with framework for EU/JP if they exist
- **FR-003**: All decompiled functions MUST be documented with arcade source reference
- **FR-004**: Progress tracking MUST report accurate percentages by subsystem
- **FR-005**: GLOBAL_ASM MUST be supported for unmatched functions
- **FR-006**: CI pipeline MUST verify ROM hash on every PR
- **FR-007**: Asset extraction MUST reproducibly extract all game assets from original ROM
- **FR-008**: Build MUST complete in under 10 minutes on modern hardware
- **FR-009**: Non-matching builds MUST produce playable ROMs
- **FR-010**: All source files MUST follow consistent code style (clang-format)

### Key Entities

- **Function**: A decompiled C function with arcade equivalent, matching status, and compiler flags
- **Module**: A logical grouping of functions (e.g., racing, game, audio) with aggregate progress
- **ROM Segment**: A memory region containing code or data, with compression and layout info
- **Checkpoint**: In-game race checkpoint with position, radius, timing data
- **Vehicle**: Car entity with physics state, position, orientation, and control inputs
- **Track/Course**: Racing environment with collision mesh, checkpoints, AI paths

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 100% of game code decompiled to matching C (ultimate goal)
- **SC-002**: ROM SHA-1 matches original for all supported versions
- **SC-003**: Build completes in under 5 minutes with parallel compilation
- **SC-004**: Every function has arcade source cross-reference in comments
- **SC-005**: Progress can be tracked and reported automatically
- **SC-006**: New contributors can build ROM within 30 minutes of cloning
- **SC-007**: At least 3 contributors beyond original author
- **SC-008**: Modders can create and share game modifications

### Milestones

1. **Infrastructure Complete**: Build system, extraction, progress tracking functional
2. **Boot to Menu**: Game boots and displays menu (estimated 20% matching)
3. **Playable Race**: Complete a race with functional physics/AI (estimated 50% matching)
4. **Feature Complete**: All game modes playable (estimated 80% matching)
5. **100% Matching**: Every function matched, ROM verified identical

## Assumptions

- Original Rush 2049 N64 ROM (US version) is available for extraction
- Arcade source code at `reference/repos/rushtherock/` is accurate and relevant
- IDO compiler can be run via static recompilation on Linux/macOS
- N64 version shares significant code with arcade version (not a complete rewrite)
- Community interest exists in contributing to this project

## Dependencies

- IDO 5.3/7.1 compiler (via ido-static-recomp from SM64)
- splat for ROM extraction
- asm-processor for GLOBAL_ASM support
- MIPS cross-compiler toolchain (binutils-mips-linux-gnu)
- Python 3.6+ for build scripts
- Reference N64 decomp projects (SM64, MK64, Perfect Dark)

## Out of Scope

- Porting game to other platforms (PC, modern consoles)
- Adding new features not in original game
- Decompiling Dreamcast or Game Boy Color versions
- Creating ROM distribution mechanisms
- Emulator development or fixes
