# osAiSetNextBuffer

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000BE70` |
| **Category** | `libultra/ai` |
| **Status** | `TODO` |
| **Instructions** | ~36 |

## Description

set AI DMA buffer

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/ai` category.

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
./work/libultra/ai/osAiSetNextBuffer/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/ai/osAiSetNextBuffer

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/ai/osAiSetNextBuffer --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Check `reference/repos/rushtherock/game/drones.c`

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: CA70.s
# Address: 0x8000BE70

glabel func_8000BE70
    /* CA70 8000BE70 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* CA74 8000BE74 AFBF0014 */  sw         $ra, 0x14($sp)
    /* CA78 8000BE78 AFA5001C */  sw         $a1, 0x1C($sp)
    /* CA7C 8000BE7C 0C003ED8 */  jal        func_8000FB60
    /* CA80 8000BE80 AFA40018 */   sw        $a0, 0x18($sp)
    /* CA84 8000BE84 10400003 */  beqz       $v0, .L8000BE94
    /* CA88 8000BE88 8FA40018 */   lw        $a0, 0x18($sp)
    /* CA8C 8000BE8C 10000018 */  b          .L8000BEF0
    /* CA90 8000BE90 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000BE94:
    /* CA94 8000BE94 3C0E8003 */  lui        $t6, %hi(D_8002C3C0)
    /* CA98 8000BE98 91CEC3C0 */  lbu        $t6, %lo(D_8002C3C0)($t6)
    /* CA9C 8000BE9C 8FAF001C */  lw         $t7, 0x1C($sp)
    /* CAA0 8000BEA0 00802825 */  or         $a1, $a0, $zero
    /* CAA4 8000BEA4 11C00002 */  beqz       $t6, .L8000BEB0
    /* CAA8 8000BEA8 008FC021 */   addu      $t8, $a0, $t7
    /* CAAC 8000BEAC 2485E000 */  addiu      $a1, $a0, -0x2000
  .L8000BEB0:
    /* CAB0 8000BEB0 33191FFF */  andi       $t9, $t8, 0x1FFF
    /* CAB4 8000BEB4 17200005 */  bnez       $t9, .L8000BECC
    /* CAB8 8000BEB8 3C018003 */   lui       $at, %hi(D_8002C3C0)
    /* CABC 8000BEBC 24080001 */  addiu      $t0, $zero, 0x1
    /* CAC0 8000BEC0 3C018003 */  lui        $at, %hi(D_8002C3C0)
    /* CAC4 8000BEC4 10000002 */  b          .L8000BED0
    /* CAC8 8000BEC8 A028C3C0 */   sb        $t0, %lo(D_8002C3C0)($at)
  .L8000BECC:
    /* CACC 8000BECC A020C3C0 */  sb         $zero, %lo(D_8002C3C0)($at)
  .L8000BED0:
    /* CAD0 8000BED0 0C003570 */  jal        func_8000D5C0
    /* CAD4 8000BED4 00A02025 */   or        $a0, $a1, $zero
    /* CAD8 8000BED8 3C09A450 */  lui        $t1, %hi(AI_DRAM_ADDR_REG)
    /* CADC 8000BEDC AD220000 */  sw         $v0, %lo(AI_DRAM_ADDR_REG)($t1)
    /* CAE0 8000BEE0 8FAA001C */  lw         $t2, 0x1C($sp)
    /* CAE4 8000BEE4 3C0BA450 */  lui        $t3, %hi(AI_LEN_REG)
    /* CAE8 8000BEE8 00001025 */  or         $v0, $zero, $zero
    /* CAEC 8000BEEC AD6A0004 */  sw         $t2, %lo(AI_LEN_REG)($t3)
  .L8000BEF0:
    /* CAF0 8000BEF0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* CAF4 8000BEF4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* CAF8 8000BEF8 03E00008 */  jr         $ra
    /* CAFC 8000BEFC 00000000 */   nop
```

## Tips for This Category

