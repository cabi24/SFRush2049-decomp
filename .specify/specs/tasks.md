# Task Breakdown: Rush 2049 N64 Decompilation

**Plan**: [plan.md](./plan.md) | **Spec**: [spec.md](./spec.md) | **Date**: 2025-12-06

---

## Phase 0: Infrastructure

### P0.1 - Build System Setup
- [ ] **P0.1.1** Copy and adapt Makefile from MK64 reference
- [ ] **P0.1.2** Create util.mk with helper functions from SM64
- [ ] **P0.1.3** Set up VERSION variable support (us, eu, jp)
- [ ] **P0.1.4** Configure COMPILER selection (ido vs gcc)
- [ ] **P0.1.5** Add NON_MATCHING build mode
- [ ] **P0.1.6** Create COMPARE target for SHA-1 verification

### P0.2 - Tool Integration
- [ ] **P0.2.1** Copy ido-static-recomp from SM64 to tools/
- [ ] **P0.2.2** Copy asm-processor from SM64 to tools/
- [ ] **P0.2.3** Set up splat submodule or copy
- [ ] **P0.2.4** Create armips build target
- [ ] **P0.2.5** Copy/adapt diff.py from SM64
- [ ] **P0.2.6** Copy/adapt progress.py from MK64

### P0.3 - ROM Analysis
- [ ] **P0.3.1** Obtain Rush 2049 N64 ROM (US version)
- [ ] **P0.3.2** Calculate and document SHA-1 hash
- [ ] **P0.3.3** Create splat config.yaml for ROM
- [ ] **P0.3.4** Run initial splat extraction
- [ ] **P0.3.5** Identify memory segments and layout
- [ ] **P0.3.6** Document ROM header structure

### P0.4 - Project Skeleton
- [ ] **P0.4.1** Create directory structure (src/, asm/, include/, etc.)
- [ ] **P0.4.2** Create linker script rush2049.ld
- [ ] **P0.4.3** Set up .clang-format from MK64
- [ ] **P0.4.4** Create include/types.h with base types
- [ ] **P0.4.5** Create include/macros.h with GLOBAL_ASM
- [ ] **P0.4.6** Create Makefile.split for splat integration

### P0.5 - CI/CD Pipeline
- [ ] **P0.5.1** Create .github/workflows/build.yml
- [ ] **P0.5.2** Set up ROM storage (encrypted, private repo)
- [ ] **P0.5.3** Add asset extraction step to CI
- [ ] **P0.5.4** Add build verification step
- [ ] **P0.5.5** Add progress reporting step
- [ ] **P0.5.6** Test pipeline end-to-end

### P0 Milestone
- [ ] `make VERSION=us NON_MATCHING=1` produces bootable ROM skeleton

---

## Phase 1: Core Analysis

### P1.1 - ROM Segment Mapping
- [ ] **P1.1.1** Document all ROM segments (code, data, assets)
- [ ] **P1.1.2** Identify compression formats used
- [ ] **P1.1.3** Map segment addresses to N64 memory
- [ ] **P1.1.4** Document overlay loading if present
- [ ] **P1.1.5** Create segment size table

### P1.2 - Function Identification
- [ ] **P1.2.1** Extract all function addresses from ROM
- [ ] **P1.2.2** Create function list with sizes
- [ ] **P1.2.3** Cross-reference with arcade source by algorithm
- [ ] **P1.2.4** Categorize functions by subsystem
- [ ] **P1.2.5** Identify libultra SDK functions
- [ ] **P1.2.6** Document unknown/unique N64 functions

### P1.3 - Data Structure Analysis
- [ ] **P1.3.1** Identify vehicle data structures
- [ ] **P1.3.2** Identify track/checkpoint structures
- [ ] **P1.3.3** Identify AI path structures
- [ ] **P1.3.4** Document global variable layout
- [ ] **P1.3.5** Map arcade structs to N64 equivalents

### P1.4 - Asset Extraction
- [ ] **P1.4.1** Extract all texture assets
- [ ] **P1.4.2** Extract 3D model data
- [ ] **P1.4.3** Extract audio samples/sequences
- [ ] **P1.4.4** Extract track collision meshes
- [ ] **P1.4.5** Document asset formats
- [ ] **P1.4.6** Create asset rebuild pipeline

### P1 Milestone
- [ ] Complete ROM documentation covering 100% of address space

---

## Phase 2: Game State Machine

### P2.1 - Main Game Loop
- [ ] **P2.1.1** Locate main() and game loop in ROM
- [ ] **P2.1.2** Compare to arcade game.c structure
- [ ] **P2.1.3** Decompile game state enum
- [ ] **P2.1.4** Decompile main loop function
- [ ] **P2.1.5** Match or GLOBAL_ASM main loop

### P2.2 - State Transitions
- [ ] **P2.2.1** Decompile ATTRACT state handling
- [ ] **P2.2.2** Decompile TRKSEL (track select) state
- [ ] **P2.2.3** Decompile CARSEL (car select) state
- [ ] **P2.2.4** Decompile PLAYGAME state
- [ ] **P2.2.5** Decompile ENDGAME/GAMEOVER states
- [ ] **P2.2.6** Decompile HISCORE state

### P2.3 - Global Variables
- [ ] **P2.3.1** Identify all global variables
- [ ] **P2.3.2** Create globals.h declarations
- [ ] **P2.3.3** Create globals.c definitions
- [ ] **P2.3.4** Document variable purposes

### P2 Milestone
- [ ] Game boots and transitions between states correctly

---

## Phase 3: Checkpoint & Race Logic

### P3.1 - Checkpoint System
- [ ] **P3.1.1** Compare to arcade checkpoint.c
- [ ] **P3.1.2** Decompile CheckPoint struct
- [ ] **P3.1.3** Decompile checkpoint initialization
- [ ] **P3.1.4** Decompile checkpoint detection (CheckCPs)
- [ ] **P3.1.5** Decompile lap counting logic
- [ ] **P3.1.6** Match checkpoint functions

### P3.2 - Race Timing
- [ ] **P3.2.1** Decompile race timer system
- [ ] **P3.2.2** Decompile time bonus calculation
- [ ] **P3.2.3** Decompile finish detection
- [ ] **P3.2.4** Decompile position tracking

### P3.3 - Track Data
- [ ] **P3.3.1** Extract checkpoint data per track
- [ ] **P3.3.2** Create courses/ directory structure
- [ ] **P3.3.3** Decompile track loading functions
- [ ] **P3.3.4** Document track data formats

### P3 Milestone
- [ ] Checkpoints trigger, laps count, race can complete

---

## Phase 4: AI System

### P4.1 - Racing Lines
- [ ] **P4.1.1** Compare to arcade maxpath.c
- [ ] **P4.1.2** Decompile path point structures
- [ ] **P4.1.3** Decompile path following algorithm
- [ ] **P4.1.4** Extract racing line data per track
- [ ] **P4.1.5** Match maxpath functions

### P4.2 - Drone Behavior
- [ ] **P4.2.1** Compare to arcade drones.c
- [ ] **P4.2.2** Decompile drone state machine
- [ ] **P4.2.3** Decompile difficulty scaling
- [ ] **P4.2.4** Decompile catchup/rubberbanding
- [ ] **P4.2.5** Decompile collision avoidance
- [ ] **P4.2.6** Match drone functions

### P4 Milestone
- [ ] AI cars race on all tracks, difficulty affects behavior

---

## Phase 5: Vehicle Physics

### P5.1 - Vehicle Dynamics
- [ ] **P5.1.1** Compare to arcade cars.c
- [ ] **P5.1.2** Decompile vehicle data structures
- [ ] **P5.1.3** Decompile suspension system
- [ ] **P5.1.4** Decompile engine/transmission
- [ ] **P5.1.5** Match vehicle parameter loading

### P5.2 - Tire Physics
- [ ] **P5.2.1** Compare to arcade tires.c
- [ ] **P5.2.2** Decompile tire force calculation
- [ ] **P5.2.3** Decompile friction circle model
- [ ] **P5.2.4** Decompile slip angle computation
- [ ] **P5.2.5** Match tire functions

### P5.3 - Collision System
- [ ] **P5.3.1** Compare to arcade collision.c
- [ ] **P5.3.2** Decompile car-to-car collision
- [ ] **P5.3.3** Decompile car-to-world collision
- [ ] **P5.3.4** Decompile damage system
- [ ] **P5.3.5** Decompile crash/respawn logic
- [ ] **P5.3.6** Match collision functions

### P5 Milestone
- [ ] Cars handle realistically, collisions work, physics feel correct

---

## Phase 6: Camera System

### P6.1 - Camera Modes
- [ ] **P6.1.1** Compare to arcade camera.c
- [ ] **P6.1.2** Decompile camera state structure
- [ ] **P6.1.3** Decompile chase camera mode
- [ ] **P6.1.4** Decompile cockpit camera mode (if present)
- [ ] **P6.1.5** Decompile replay camera (if present)
- [ ] **P6.1.6** Match camera functions

### P6 Milestone
- [ ] All camera modes work correctly

---

## Phase 7: Audio & Rendering

### P7.1 - Audio System
- [ ] **P7.1.1** Identify N64 audio library usage
- [ ] **P7.1.2** Decompile audio initialization
- [ ] **P7.1.3** Decompile sound effect playback
- [ ] **P7.1.4** Decompile music playback
- [ ] **P7.1.5** Decompile engine sound synthesis (if present)
- [ ] **P7.1.6** Match audio functions

### P7.2 - Rendering System
- [ ] **P7.2.1** Identify RSP microcode used
- [ ] **P7.2.2** Decompile display list generation
- [ ] **P7.2.3** Decompile car model rendering
- [ ] **P7.2.4** Decompile track rendering
- [ ] **P7.2.5** Decompile HUD rendering
- [ ] **P7.2.6** Decompile visual effects (skids, sparks)
- [ ] **P7.2.7** Match rendering functions

### P7 Milestone
- [ ] Game is fully playable with correct audio and graphics

---

## Phase 8: Matching & Polish

### P8.1 - Function Matching
- [ ] **P8.1.1** List all remaining GLOBAL_ASM stubs
- [ ] **P8.1.2** Prioritize by complexity
- [ ] **P8.1.3** Match high-priority functions
- [ ] **P8.1.4** Use decomp-permuter for stubborn cases
- [ ] **P8.1.5** Document unmatchable functions (if any)

### P8.2 - Version Support
- [ ] **P8.2.1** Obtain EU version ROM (if exists)
- [ ] **P8.2.2** Obtain JP version ROM (if exists)
- [ ] **P8.2.3** Document version differences
- [ ] **P8.2.4** Add conditional compilation for versions
- [ ] **P8.2.5** Verify matching for all versions

### P8.3 - Documentation
- [ ] **P8.3.1** Complete function documentation
- [ ] **P8.3.2** Complete data structure documentation
- [ ] **P8.3.3** Write contributor guide
- [ ] **P8.3.4** Write building instructions
- [ ] **P8.3.5** Create modding guide

### P8 Milestone
- [ ] 100% matching for all versions, comprehensive documentation

---

## Summary Statistics

| Phase | Task Count | Estimated Effort |
|-------|------------|------------------|
| P0: Infrastructure | 30 tasks | 2 weeks |
| P1: Core Analysis | 22 tasks | 2 weeks |
| P2: Game State | 14 tasks | 2 weeks |
| P3: Checkpoint | 14 tasks | 2 weeks |
| P4: AI System | 12 tasks | 2 weeks |
| P5: Vehicle Physics | 17 tasks | 4 weeks |
| P6: Camera | 7 tasks | 2 weeks |
| P7: Audio & Rendering | 14 tasks | 4 weeks |
| P8: Matching & Polish | 14 tasks | 4+ weeks |
| **Total** | **144 tasks** | **24+ weeks** |

---

## Priority Order

**Critical Path** (blocks everything):
1. P0.3 ROM Analysis - Must have ROM
2. P0.1 Build System - Must be able to build
3. P0.4 Project Skeleton - Must have structure
4. P1.2 Function Identification - Must know what to decompile

**High Value** (enables testing):
1. P2 Game State Machine - Get game running
2. P3 Checkpoint Logic - Enable race completion
3. P5 Vehicle Physics - Core gameplay

**Parallel Work** (can be done alongside):
1. P4 AI System - Independent of physics matching
2. P6 Camera - Independent subsystem
3. P7 Audio/Rendering - Independent subsystems

---

## Getting Started

To begin work:

1. **Obtain ROM**: Get Rush 2049 N64 (US) ROM file
2. **Clone repo**: `git clone [repo-url] && cd rush2049-decomp`
3. **Read docs**: Review constitution.md and plan.md
4. **Start P0.3.1**: Place ROM as `baserom.us.z64`
5. **Run splat**: Begin ROM analysis

First contributor task: **P0.3 - ROM Analysis**
