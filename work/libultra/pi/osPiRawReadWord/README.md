# osPiRawReadWord

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800081D0` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~15 |

## Description

synchronized ROM word read with PI access control

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pi` category.

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
./work/libultra/pi/osPiRawReadWord/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/osPiRawReadWord

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/osPiRawReadWord --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Standard N64 libultra function - check other decomp projects (sm64, oot)

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 8DD0.s
# Address: 0x800081D0

glabel func_800081D0
    /* 8DD0 800081D0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 8DD4 800081D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8DD8 800081D8 AFA40020 */  sw         $a0, 0x20($sp)
    /* 8DDC 800081DC 0C003704 */  jal        func_8000DC10
    /* 8DE0 800081E0 AFA50024 */   sw        $a1, 0x24($sp)
    /* 8DE4 800081E4 8FA40020 */  lw         $a0, 0x20($sp)
    /* 8DE8 800081E8 0C003720 */  jal        func_8000DC80
    /* 8DEC 800081EC 8FA50024 */   lw        $a1, 0x24($sp)
    /* 8DF0 800081F0 0C003715 */  jal        func_8000DC54
    /* 8DF4 800081F4 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 8DF8 800081F8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 8DFC 800081FC 8FA2001C */  lw         $v0, 0x1C($sp)
    /* 8E00 80008200 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 8E04 80008204 03E00008 */  jr         $ra
    /* 8E08 80008208 00000000 */   nop
```

## Tips for This Category

