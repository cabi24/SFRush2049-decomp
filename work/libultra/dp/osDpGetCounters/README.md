# osDpGetCounters

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800099B0` |
| **Category** | `libultra/dp` |
| **Status** | `TODO` |
| **Instructions** | ~14 |

## Description

get RDP performance counters

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/dp` category.

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
./work/libultra/dp/osDpGetCounters/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/dp/osDpGetCounters

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/dp/osDpGetCounters --watch
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
# Source: A5B0.s
# Address: 0x800099B0

glabel func_800099B0
    /* A5B0 800099B0 3C0EA410 */  lui        $t6, %hi(DPC_CLOCK_REG)
    /* A5B4 800099B4 8DCF0010 */  lw         $t7, %lo(DPC_CLOCK_REG)($t6)
    /* A5B8 800099B8 3C18A410 */  lui        $t8, %hi(DPC_BUFBUSY_REG)
    /* A5BC 800099BC 3C08A410 */  lui        $t0, %hi(DPC_PIPEBUSY_REG)
    /* A5C0 800099C0 AC8F0000 */  sw         $t7, 0x0($a0)
    /* A5C4 800099C4 8F190014 */  lw         $t9, %lo(DPC_BUFBUSY_REG)($t8)
    /* A5C8 800099C8 3C0AA410 */  lui        $t2, %hi(DPC_TMEM_REG)
    /* A5CC 800099CC 2484000C */  addiu      $a0, $a0, 0xC
    /* A5D0 800099D0 AC99FFF8 */  sw         $t9, -0x8($a0)
    /* A5D4 800099D4 8D090018 */  lw         $t1, %lo(DPC_PIPEBUSY_REG)($t0)
    /* A5D8 800099D8 AC89FFFC */  sw         $t1, -0x4($a0)
    /* A5DC 800099DC 8D4B001C */  lw         $t3, %lo(DPC_TMEM_REG)($t2)
    /* A5E0 800099E0 03E00008 */  jr         $ra
    /* A5E4 800099E4 AC8B0000 */   sw        $t3, 0x0($a0)
```

## Tips for This Category

