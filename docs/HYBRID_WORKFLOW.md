# Hybrid Function Matching Workflow

## Overview

This project uses a **hybrid workflow** that combines the best aspects of traditional decomp approaches with a modern per-function structure optimized for parallel work.

### Why Hybrid?

| Traditional Approach | Per-Function Approach | Hybrid (This Project) |
|---------------------|----------------------|----------------------|
| Functions grouped in .c files | Each function in its own dir | Work in isolation, integrate when done |
| One person owns a file | No conflicts possible | Best of both worlds |
| Hard to parallelize | Easy to parallelize | Parallel work + proper integration |
| Context preserved | Context lost | Context in src/, isolation in work/ |

## Directory Structure

```
rush2049-decomp/
├── src/                    # FINAL integrated source (the goal)
│   ├── game/
│   │   ├── game.c         # Contains matched functions
│   │   └── checkpoint.c
│   ├── libc/
│   │   └── string.c
│   └── libultra/
│       └── os_message.c
│
├── work/                   # PER-FUNCTION scratch space
│   ├── libc/
│   │   ├── strlen/
│   │   │   ├── base.c     # Your implementation
│   │   │   ├── target.s   # Target assembly to match
│   │   │   ├── STATUS     # MATCHING, TODO, WIP, etc.
│   │   │   └── info.txt   # Metadata (address, flags)
│   │   └── memcpy/
│   │       ├── base.c
│   │       ├── target.s
│   │       └── STATUS
│   ├── libultra/
│   │   └── os/
│   │       └── osCreateMesgQueue/
│   └── game/
│       ├── render/
│       ├── physics/
│       └── ai/
│
└── asm/us/                 # Original disassembly (reference)
    ├── 7600.s
    ├── 8800.s
    └── ...
```

## Status Values

Each function directory contains a `STATUS` file with one of:

| Status | Meaning |
|--------|---------|
| `TODO` | Not started, available to claim |
| `WIP` | Work in progress (add your name/session) |
| `MATCHING` | Byte-for-byte match achieved |
| `CLOSE` | Compiles, functionally correct, minor asm differences |
| `STUB` | Placeholder only, needs real implementation |
| `ASM` | Requires inline assembly, cannot match with pure C |
| `BLOCKED` | Waiting on dependency or external info |

## Workflow for AI Sessions

### 1. Claim a Function

```bash
# Find available functions
find work -name "STATUS" -exec grep -l "^TODO$" {} \; | head -20

# Or find by category
find work/libc -name "STATUS" -exec grep -l "^TODO$" {} \;

# Claim it by updating STATUS
echo "WIP - Session $(date +%H%M)" > work/libc/memcpy/STATUS
```

### 2. Understand the Target

```bash
# Read target assembly
cat work/libc/memcpy/target.s

# Read metadata
cat work/libc/memcpy/info.txt

# Look for arcade reference
grep -r "memcpy" reference/repos/rushtherock/
```

### 3. Implement and Test

```bash
# Edit base.c with your implementation
vim work/libc/memcpy/base.c

# Compile on watchman
ssh watchman
cd /home/cburnes/projects/rush2049-decomp

# Get compiler flags from info.txt
cat work/libc/memcpy/info.txt
# compiler_flags: -g0 -O2 -mips2 -G 0 -non_shared

# Compile
tools/ido-static-recomp/build/out/cc -g0 -O2 -mips2 -G 0 -non_shared \
    -c -o /tmp/test.o work/libc/memcpy/base.c

# Compare
mips-linux-gnu-objdump -d /tmp/test.o
```

### 4. Use asmdiff.py for Visual Comparison

```bash
# Quick side-by-side diff
python3 tools/asmdiff.py work/libc/memcpy

# Watch mode (auto-refresh on save)
python3 tools/asmdiff.py work/libc/memcpy --watch
```

### 5. Update Status and Commit

```bash
# When matched
echo "MATCHING" > work/libc/memcpy/STATUS
git add work/libc/memcpy/
git commit -m "Match memcpy function"

# Or if close but not perfect
echo "CLOSE - register allocation differs" > work/libc/memcpy/STATUS
```

### 6. Integration (Optional)

Once matched, the function can be integrated into src/:

```bash
# Copy implementation to proper source file
# (This is done by project maintainers)
```

## Compiler Flags Quick Reference

| Category | Flags | Reason |
|----------|-------|--------|
| libc | `-g0 -O2 -mips2 -G 0 -non_shared` | Verified via strlen match |
| libm | `-g0 -O2 -mips2 -G 0 -non_shared` | Same as libc |
| libultra/os | `-g0 -O1 -mips2 -G 0 -non_shared` | Verified via osCreateMesgQueue |
| libultra/gu | `-g0 -O2 -mips2 -G 0 -non_shared` | Verified via guMtxIdentF |
| game/* | `-g0 -O2 -mips2 -G 0 -non_shared` | Assumed based on branch-likely usage |

## Parallel Work Guidelines

### For Humans

1. Check `STATUS` before starting - don't work on `WIP` functions
2. Update `STATUS` immediately when you start
3. Commit frequently so others see your progress
4. If stuck, mark as `BLOCKED` with notes

### For AI Sessions

1. Each session should claim a **category** or **address range**
2. Update `STATUS` files atomically
3. Commit after each function match
4. Don't modify files outside your claimed area

### Example Session Assignments

```
Session 1: work/libc/*
Session 2: work/libultra/os/*
Session 3: work/libultra/gu/*
Session 4: work/game/render/*
Session 5: work/game/physics/*
```

## Progress Tracking

### Quick Stats

```bash
# Count by status
echo "MATCHING: $(grep -r '^MATCHING$' work --include=STATUS | wc -l)"
echo "WIP: $(grep -r '^WIP' work --include=STATUS | wc -l)"
echo "TODO: $(grep -r '^TODO$' work --include=STATUS | wc -l)"

# Progress by category
for cat in libc libm libgcc libultra game; do
    echo -n "$cat: "
    grep -r '^MATCHING$' work/$cat --include=STATUS 2>/dev/null | wc -l
done
```

### Detailed Report

```bash
# Generate full report
python3 tools/progress_report.py
```

## Game Functions Note

Functions in the `work/game/` directory (address range 0x80086A50+) come from compressed ROM data that's decompressed at runtime. Their target assembly may not be in `asm/us/*.s`.

To get target assembly for game functions:
1. Check if `build/game_code_disasm.txt` exists
2. If not, run the ROM and extract the decompressed code
3. Or reference the existing `src/game/game.c` implementations

## Files in Each Function Directory

| File | Purpose |
|------|---------|
| `base.c` | Your C implementation (edit this) |
| `target.s` | Target assembly to match |
| `STATUS` | Current status (MATCHING, TODO, etc.) |
| `info.txt` | Metadata (address, compiler flags, notes) |

## FAQ

**Q: What if two people edit the same function?**
A: Check STATUS first. If it's WIP, pick a different function.

**Q: What if target.s is empty or missing?**
A: Look up the address in `asm/us/*.s` files or `symbol_addrs.us.txt`.

**Q: How do I know when I'm done?**
A: When `asmdiff.py` shows 0 differences and score is 0.

**Q: Should I use decomp-permuter?**
A: For stubborn functions, yes. Create a permuter directory and run it.

---

Last updated: 2026-01-02
