# Function Matching Progress Tracker

## How to Work in Parallel

Each AI session claims a **single file** to work on. This prevents merge conflicts since each file has one owner.

### Rules
1. Before starting, check this file for unclaimed files
2. Edit this file to claim your file (change status to `IN_PROGRESS`)
3. Work only on your claimed file
4. When done, update status to `DONE` and commit
5. If stuck, mark as `BLOCKED` with notes

### Starting a Session

Tell the AI:
```
I want to work on function matching for Rush 2049.
Claim file: src/game/[filename].c
Follow docs/FUNCTION_MATCHING.md protocol.
```

---

## File Status

### Priority 1: Library Functions (easiest to match)

| File | Status | Owner | Notes |
|------|--------|-------|-------|
| src/libc/string.c | DONE | - | strlen matches -O2 |
| src/libc/memory.c | AVAILABLE | - | bcopy, memcpy |
| src/libm/math.c | AVAILABLE | - | sinf, cosf, sqrtf |
| src/libultra/os_message.c | PARTIAL | - | osCreateMesgQueue matches -O1 |
| src/libultra/gu.c | PARTIAL | - | guMtxIdentF matches -O2 |

### Priority 2: Game Core (has arcade reference)

| File | Status | Owner | Notes |
|------|--------|-------|-------|
| src/game/game.c | AVAILABLE | - | Main game loop, 4300+ lines |
| src/game/checkpoint.c | AVAILABLE | - | Has arcade equivalent |
| src/game/drone.c | AVAILABLE | - | AI racing, has arcade ref |
| src/game/camera.c | AVAILABLE | - | Camera system |
| src/game/car.c | AVAILABLE | - | Car physics |
| src/game/drivetrain.c | AVAILABLE | - | Engine/transmission |
| src/game/tire.c | AVAILABLE | - | Tire physics |
| src/game/collision.c | AVAILABLE | - | Collision detection |

### Priority 3: Game Systems

| File | Status | Owner | Notes |
|------|--------|-------|-------|
| src/game/menu.c | AVAILABLE | - | Menu system |
| src/game/hiscore.c | AVAILABLE | - | High scores |
| src/game/attract.c | AVAILABLE | - | Attract mode |
| src/game/select.c | AVAILABLE | - | Track selection |
| src/game/carsel.c | AVAILABLE | - | Car selection |
| src/game/effects.c | AVAILABLE | - | Particle effects |
| src/game/sound.c | AVAILABLE | - | Sound system |
| src/game/replay.c | AVAILABLE | - | Replay system |

### Priority 4: Support Code

| File | Status | Owner | Notes |
|------|--------|-------|-------|
| src/game/vecmath.c | AVAILABLE | - | Vector/matrix math |
| src/game/road.c | AVAILABLE | - | Track geometry |
| src/game/state.c | AVAILABLE | - | Game state machine |
| src/os/dll.c | AVAILABLE | - | DLL/timer system |
| src/inflate/inflate.c | AVAILABLE | - | Decompression |

---

## Workflow for Each File

### Step 1: Analyze Current State
```bash
# Count functions in file
grep -c "^[a-zA-Z].*(" src/game/[file].c

# Find stub functions (NON_MATCHING or empty)
grep -B2 "NON_MATCHING\|// TODO\|return;" src/game/[file].c
```

### Step 2: Find Target Assembly
```bash
# Look up function address in symbol_addrs.us.txt
grep "function_name" symbol_addrs.us.txt

# Find assembly in asm/us/*.s files
grep -l "function_name\|0x80XXXXXX" asm/us/*.s
```

### Step 3: Compile and Compare
```bash
# On watchman machine
ssh watchman
cd /home/cburnes/projects/rush2049-decomp

# Compile single file
tools/ido-static-recomp/build/out/cc -g0 -O2 -mips2 -G 0 -non_shared \
  -c -o /tmp/test.o src/game/[file].c

# Disassemble and compare
mips-linux-gnu-objdump -d /tmp/test.o > /tmp/compiled.txt
```

### Step 4: Use asmdiff.py for quick comparison
```bash
# Create permuter directory for function
mkdir -p permuter/[func_name]
# Add target.s and base.c
python3 tools/asmdiff.py permuter/[func_name]
```

---

## Compiler Settings Quick Reference

| Module | Flags |
|--------|-------|
| libc | `-g0 -O2 -mips2 -G 0 -non_shared` |
| libultra/os | `-g0 -O1 -mips2 -G 0 -non_shared` |
| libultra/gu | `-g0 -O2 -mips2 -G 0 -non_shared` |
| game code | `-g0 -O2 -mips2 -G 0 -non_shared` (likely) |

---

## Last Updated
2026-01-02
