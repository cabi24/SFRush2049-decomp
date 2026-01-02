# inflate_loop

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006678` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~23 |

## Description

main inflate decompression loop

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `inflate` category.

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
./work/inflate/inflate_loop/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_loop

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_loop --watch
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
# Source: 5610.s
# Address: 0x80006678

glabel func_80006678
    /* 7278 80006678 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 727C 8000667C 3C018003 */  lui        $at, %hi(D_800354C0)
    /* 7280 80006680 AC2054C0 */  sw         $zero, %lo(D_800354C0)($at)
    /* 7284 80006684 3C018003 */  lui        $at, %hi(D_800354BC)
    /* 7288 80006688 AFB00018 */  sw         $s0, 0x18($sp)
    /* 728C 8000668C 27B00024 */  addiu      $s0, $sp, 0x24
    /* 7290 80006690 AC2054BC */  sw         $zero, %lo(D_800354BC)($at)
    /* 7294 80006694 AFBF001C */  sw         $ra, 0x1C($sp)
  .L80006698:
    /* 7298 80006698 0C001935 */  jal        func_800064D4
    /* 729C 8000669C 02002025 */   or        $a0, $s0, $zero
    /* 72A0 800066A0 10400003 */  beqz       $v0, .L800066B0
    /* 72A4 800066A4 00000000 */   nop
    /* 72A8 800066A8 10000005 */  b          .L800066C0
    /* 72AC 800066AC 24020001 */   addiu     $v0, $zero, 0x1
  .L800066B0:
    /* 72B0 800066B0 8FAE0024 */  lw         $t6, 0x24($sp)
    /* 72B4 800066B4 11C0FFF8 */  beqz       $t6, .L80006698
    /* 72B8 800066B8 00000000 */   nop
    /* 72BC 800066BC 00001025 */  or         $v0, $zero, $zero
  .L800066C0:
    /* 72C0 800066C0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 72C4 800066C4 8FB00018 */  lw         $s0, 0x18($sp)
    /* 72C8 800066C8 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 72CC 800066CC 03E00008 */  jr         $ra
    /* 72D0 800066D0 00000000 */   nop
```

## Tips for This Category

