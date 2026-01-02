# osVirtualToPhysical

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D5C0` |
| **Category** | `libultra/vi` |
| **Status** | `TODO` |
| **Instructions** | ~31 |

## Description

convert virtual address to physical

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/vi` category.

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
./work/libultra/vi/osVirtualToPhysical/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/vi/osVirtualToPhysical

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/vi/osVirtualToPhysical --watch
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
# Source: E1C0.s
# Address: 0x8000D5C0

glabel func_8000D5C0
    /* E1C0 8000D5C0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E1C4 8000D5C4 AFA40018 */  sw         $a0, 0x18($sp)
    /* E1C8 8000D5C8 8FAE0018 */  lw         $t6, 0x18($sp)
    /* E1CC 8000D5CC 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* E1D0 8000D5D0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E1D4 8000D5D4 01C1082B */  sltu       $at, $t6, $at
    /* E1D8 8000D5D8 14200007 */  bnez       $at, .L8000D5F8
    /* E1DC 8000D5DC 3C01A000 */   lui       $at, (0xA0000000 >> 16)
    /* E1E0 8000D5E0 01C1082B */  sltu       $at, $t6, $at
    /* E1E4 8000D5E4 10200004 */  beqz       $at, .L8000D5F8
    /* E1E8 8000D5E8 3C011FFF */   lui       $at, (0x1FFFFFFF >> 16)
    /* E1EC 8000D5EC 3421FFFF */  ori        $at, $at, (0x1FFFFFFF & 0xFFFF)
    /* E1F0 8000D5F0 1000000E */  b          .L8000D62C
    /* E1F4 8000D5F4 01C11024 */   and       $v0, $t6, $at
  .L8000D5F8:
    /* E1F8 8000D5F8 8FAF0018 */  lw         $t7, 0x18($sp)
    /* E1FC 8000D5FC 3C01A000 */  lui        $at, (0xA0000000 >> 16)
    /* E200 8000D600 01E1082B */  sltu       $at, $t7, $at
    /* E204 8000D604 14200007 */  bnez       $at, .L8000D624
    /* E208 8000D608 3C01C000 */   lui       $at, (0xC0000000 >> 16)
    /* E20C 8000D60C 01E1082B */  sltu       $at, $t7, $at
    /* E210 8000D610 10200004 */  beqz       $at, .L8000D624
    /* E214 8000D614 3C011FFF */   lui       $at, (0x1FFFFFFF >> 16)
    /* E218 8000D618 3421FFFF */  ori        $at, $at, (0x1FFFFFFF & 0xFFFF)
    /* E21C 8000D61C 10000003 */  b          .L8000D62C
    /* E220 8000D620 01E11024 */   and       $v0, $t7, $at
  .L8000D624:
    /* E224 8000D624 0C003F2C */  jal        func_8000FCB0
    /* E228 8000D628 8FA40018 */   lw        $a0, 0x18($sp)
  .L8000D62C:
    /* E22C 8000D62C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E230 8000D630 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E234 8000D634 03E00008 */  jr         $ra
    /* E238 8000D638 00000000 */   nop
```

## Tips for This Category

