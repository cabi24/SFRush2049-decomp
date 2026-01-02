# dma_queue_init

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002540` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~22 |

## Description

initialize DMA message queue

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `boot` category.

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
./work/boot/dma_queue_init/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/dma_queue_init

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/dma_queue_init --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

No specific reference available.

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 3140.s
# Address: 0x80002540

glabel func_80002540
    /* 3140 80002540 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 3144 80002544 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 3148 80002548 240E0001 */  addiu      $t6, $zero, 0x1
    /* 314C 8000254C 3C018003 */  lui        $at, %hi(D_8002B030)
    /* 3150 80002550 A02EB030 */  sb         $t6, %lo(D_8002B030)($at)
    /* 3154 80002554 3C048003 */  lui        $a0, %hi(D_8002F190)
    /* 3158 80002558 3C058003 */  lui        $a1, %hi(D_8002F1A8)
    /* 315C 8000255C 24A5F1A8 */  addiu      $a1, $a1, %lo(D_8002F1A8)
    /* 3160 80002560 2484F190 */  addiu      $a0, $a0, %lo(D_8002F190)
    /* 3164 80002564 0C001A80 */  jal        func_80006A00
    /* 3168 80002568 24060001 */   addiu     $a2, $zero, 0x1
    /* 316C 8000256C 3C048003 */  lui        $a0, %hi(D_8002F190)
    /* 3170 80002570 2484F190 */  addiu      $a0, $a0, %lo(D_8002F190)
    /* 3174 80002574 00002825 */  or         $a1, $zero, $zero
    /* 3178 80002578 0C001D78 */  jal        func_800075E0
    /* 317C 8000257C 00003025 */   or        $a2, $zero, $zero
    /* 3180 80002580 10000001 */  b          .L80002588
    /* 3184 80002584 00000000 */   nop
  .L80002588:
    /* 3188 80002588 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 318C 8000258C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 3190 80002590 03E00008 */  jr         $ra
    /* 3194 80002594 00000000 */   nop
```

## Tips for This Category

