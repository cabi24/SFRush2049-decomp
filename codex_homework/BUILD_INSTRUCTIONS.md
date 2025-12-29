# Build Instructions for Codex

This document describes the build environment and tools available for the Rush 2049 N64 decompilation project.

---

## Quick Reference

### Build the ROM
```bash
# Standard build (20 cores available)
make VERSION=us NON_MATCHING=1 -j20

# Expected output on success:
# Verifying ROM hash...
# baserom.us.z64: OK
# ROM matches!
```

### Verify Build
```bash
# Check if build passes
make VERSION=us NON_MATCHING=1 -j20 2>&1 | tail -5
```

### Clean Build
```bash
make clean
make VERSION=us NON_MATCHING=1 -j20
```

---

## Installed Tools

### Cross-Compiler Toolchain
| Tool | Path | Version |
|------|------|---------|
| GCC | `/usr/bin/mips-linux-gnu-gcc` | 12.2.0 |
| Assembler | `/usr/bin/mips-linux-gnu-as` | 2.40 |
| Linker | `/usr/bin/mips-linux-gnu-ld` | 2.40 |
| Objdump | `/usr/bin/mips-linux-gnu-objdump` | 2.40 |

### Build System
| Tool | Path | Version |
|------|------|---------|
| Make | `/usr/bin/make` | 4.3 |
| Python | `/usr/bin/python3` | 3.11.2 |
| sha1sum | `/usr/bin/sha1sum` | coreutils |

### Decompilation Tools
| Tool | Location | Purpose |
|------|----------|---------|
| mips_to_c | `tools/mips_to_c/` | Auto-generate C from MIPS assembly |
| m2c.py | `tools/m2c.py` | Wrapper for mips_to_c |
| diff.py | `tools/diff.py` | Compare function output |
| Capstone | Python package | Disassembly library |

---

## Using mips_to_c

### Decompile a function by name
```bash
./tools/m2c.py func_80009387
```

### Decompile a function from assembly file
```bash
./tools/m2c.py asm/us/5610.s -f 0    # First function
./tools/m2c.py asm/us/5610.s         # All functions
```

### Compare your implementation
```bash
./tools/diff.py drone_update -m -w
```

---

## Project Structure

```
rush2049-decomp/
├── src/                    # C source files (edit these)
│   ├── game/
│   │   ├── game.c         # Main game code (~9,400 lines)
│   │   ├── drone.c        # AI/drone system (your target)
│   │   ├── physics.c      # Car physics
│   │   └── ...
│   ├── libc/              # Standard C library
│   ├── libm/              # Math library
│   └── libultra/          # N64 OS library
├── include/               # Header files
│   └── game/
│       ├── drone.h        # Drone system headers
│       ├── structs.h      # Game structures
│       └── ...
├── asm/us/                # Assembly files (reference)
├── reference/repos/       # Arcade source code
│   └── rushtherock/
│       └── game/
│           ├── drones.c   # Arcade drone code
│           ├── maxpath.c  # Arcade path system
│           └── ...
├── build/                 # Build output
├── tools/                 # Development tools
└── codex_homework/        # Your assignments
```

---

## Workflow

### 1. Read the assignment
```bash
cat codex_homework/CODEX_AI_DRONE_01.md
```

### 2. Read arcade reference code
```bash
cat reference/repos/rushtherock/game/drones.c | head -100
cat reference/repos/rushtherock/game/maxpath.c | head -100
```

### 3. Edit source files
```bash
# Edit the drone system
nano src/game/drone.c
nano include/game/drone.h
```

### 4. Build and verify
```bash
make VERSION=us NON_MATCHING=1 -j20
```

### 5. Fix any errors
- Compiler errors will show file:line
- Fix the issue and rebuild

### 6. Commit your changes
```bash
git add src/game/drone.c include/game/drone.h
git commit -m "Implement drone path structures (Part 1)"
```

---

## Common Build Errors

### Undefined reference
```
undefined reference to `drone_init_maxpath'
```
**Fix**: Add function declaration to header file, or implement the function.

### Implicit declaration
```
warning: implicit declaration of function 'sqrtf'
```
**Fix**: Add `extern f32 sqrtf(f32 x);` or include the right header.

### Type mismatch
```
error: incompatible types when assigning
```
**Fix**: Check your type definitions match (s32 vs int, f32 vs float).

### ROM doesn't match (non-matching build)
This is expected for NON_MATCHING builds. The important thing is that it compiles without errors.

---

## N64 Types

Use these types instead of standard C types:

| N64 Type | Standard C | Size |
|----------|-----------|------|
| `s8` | `signed char` | 1 byte |
| `u8` | `unsigned char` | 1 byte |
| `s16` | `signed short` | 2 bytes |
| `u16` | `unsigned short` | 2 bytes |
| `s32` | `signed int` | 4 bytes |
| `u32` | `unsigned int` | 4 bytes |
| `f32` | `float` | 4 bytes |
| `f64` | `double` | 8 bytes |

These are defined in `include/types.h`.

---

## Hardware Info

This machine has:
- **CPU**: 20 cores (use `-j20` for parallel builds)
- **RAM**: 31 GB
- **OS**: Linux (Debian-based)

Builds complete in ~10-15 seconds.

---

## Success Criteria

A successful build shows:
```
Verifying ROM hash...
baserom.us.z64: OK
ROM matches!
```

If you see errors, fix them before proceeding. The build must pass before committing.

---

## Getting Help

### Check existing code for patterns
```bash
grep -n "drone_" src/game/drone.c
grep -rn "MPATH" include/
```

### Read the arcade source
```bash
cat reference/repos/rushtherock/game/drones.c
cat reference/repos/rushtherock/game/maxpath.c
cat reference/repos/rushtherock/game/maxpath.h
```

### Check the documentation
```bash
cat docs/ai_drone_mapping.md
```

---

## Remember

1. **Build after every change** - Catch errors early
2. **Small commits** - One logical change per commit
3. **Reference the arcade source** - It's your guide
4. **Types matter** - Use N64 types (s32, f32, etc.)
5. **ROM matches is success** - That's the goal
