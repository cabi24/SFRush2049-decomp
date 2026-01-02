# __osPfsSelectBank

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000E850` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~30 |

## Description

select controller pak bank

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pfs` category.

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
./work/libultra/pfs/__osPfsSelectBank/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/__osPfsSelectBank

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/__osPfsSelectBank --watch
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
# Source: F450.s
# Address: 0x8000E850

glabel func_8000E850
    /* F450 8000E850 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* F454 8000E854 AFB00020 */  sw         $s0, 0x20($sp)
    /* F458 8000E858 30B000FF */  andi       $s0, $a1, 0xFF
    /* F45C 8000E85C AFBF0024 */  sw         $ra, 0x24($sp)
    /* F460 8000E860 AFA40058 */  sw         $a0, 0x58($sp)
    /* F464 8000E864 AFA5005C */  sw         $a1, 0x5C($sp)
    /* F468 8000E868 27A20038 */  addiu      $v0, $sp, 0x38
    /* F46C 8000E86C 27A30058 */  addiu      $v1, $sp, 0x58
  .L8000E870:
    /* F470 8000E870 24420004 */  addiu      $v0, $v0, 0x4
    /* F474 8000E874 A050FFFD */  sb         $s0, -0x3($v0)
    /* F478 8000E878 A050FFFE */  sb         $s0, -0x2($v0)
    /* F47C 8000E87C A050FFFF */  sb         $s0, -0x1($v0)
    /* F480 8000E880 1443FFFB */  bne        $v0, $v1, .L8000E870
    /* F484 8000E884 A050FFFC */   sb        $s0, -0x4($v0)
    /* F488 8000E888 8FAE0058 */  lw         $t6, 0x58($sp)
    /* F48C 8000E88C 24060400 */  addiu      $a2, $zero, 0x400
    /* F490 8000E890 27A70038 */  addiu      $a3, $sp, 0x38
    /* F494 8000E894 8DC40004 */  lw         $a0, 0x4($t6)
    /* F498 8000E898 8DC50008 */  lw         $a1, 0x8($t6)
    /* F49C 8000E89C 0C003DA0 */  jal        func_8000F680
    /* F4A0 8000E8A0 AFA00010 */   sw        $zero, 0x10($sp)
    /* F4A4 8000E8A4 14400003 */  bnez       $v0, .L8000E8B4
    /* F4A8 8000E8A8 00401825 */   or        $v1, $v0, $zero
    /* F4AC 8000E8AC 8FAF0058 */  lw         $t7, 0x58($sp)
    /* F4B0 8000E8B0 A1F00065 */  sb         $s0, 0x65($t7)
  .L8000E8B4:
    /* F4B4 8000E8B4 8FBF0024 */  lw         $ra, 0x24($sp)
    /* F4B8 8000E8B8 8FB00020 */  lw         $s0, 0x20($sp)
    /* F4BC 8000E8BC 27BD0058 */  addiu      $sp, $sp, 0x58
    /* F4C0 8000E8C0 03E00008 */  jr         $ra
    /* F4C4 8000E8C4 00601025 */   or        $v0, $v1, $zero
```

## Tips for This Category

