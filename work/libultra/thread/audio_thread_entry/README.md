# audio_thread_entry

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800024FC` |
| **Category** | `libultra/thread` |
| **Status** | `TODO` |
| **Instructions** | ~16 |

## Description

audio thread entry point (priority 5)

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/thread` category.

## Files in This Directory

| File | Purpose |
|------|---------|
| `README.md` | This file - all context needed to work on this function |
| `base.c` | Your C implementation (edit this!) |
| `target.s` | Target assembly to match |
| `types.h` | Common type definitions |
| `compile.sh` | Script to compile and compare |
| `STATUS` | Current status (TODO/WIP/MATCHING) |

## How to Match This Function

### Step 1: Understand the Target

Look at `target.s` - this is the assembly your C code must produce.

Key things to note:
- Function prologue/epilogue (stack frame size)
- Register usage patterns
- Branch instructions (beq vs bne vs beql)
- Memory access patterns

### Step 2: Write Your Implementation

Edit `base.c` with your C implementation. Include `types.h` for common types.

```c
#include "types.h"

// Your implementation here
```

### Step 3: Compile and Compare

```bash
# On watchman (x86 build machine):
cd /home/cburnes/projects/rush2049-decomp
./work/libultra/thread/audio_thread_entry/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/thread/audio_thread_entry

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/thread/audio_thread_entry --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Check `reference/repos/rushtherock/game/audio.c`

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 2CF0.s
# Address: 0x800024FC

glabel func_800024FC
    /* 30FC 800024FC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 3100 80002500 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 3104 80002504 AFA40018 */  sw         $a0, 0x18($sp)
    /* 3108 80002508 3C040100 */  lui        $a0, (0x1000E00 >> 16)
    /* 310C 8000250C 0C00127C */  jal        func_800049F0
    /* 3110 80002510 34840E00 */   ori       $a0, $a0, (0x1000E00 & 0xFFFF)
  .L80002514:
    /* 3114 80002514 0C03F519 */  jal        func_800FD464
    /* 3118 80002518 00000000 */   nop
    /* 311C 8000251C 1000FFFD */  b          .L80002514
    /* 3120 80002520 00000000 */   nop
    /* 3124 80002524 10000001 */  b          .L8000252C
    /* 3128 80002528 00000000 */   nop
  .L8000252C:
    /* 312C 8000252C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 3130 80002530 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 3134 80002534 03E00008 */  jr         $ra
    /* 3138 80002538 00000000 */   nop
```

## Tips for This Category

