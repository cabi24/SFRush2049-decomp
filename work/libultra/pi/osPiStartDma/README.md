# osPiStartDma

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000DCD0` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~50 |

## Description

start PI DMA transfer

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
./work/libultra/pi/osPiStartDma/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/osPiStartDma

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/osPiStartDma --watch
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
# Source: E8D0.s
# Address: 0x8000DCD0

glabel func_8000DCD0
    /* E8D0 8000DCD0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E8D4 8000DCD4 3C03A460 */  lui        $v1, %hi(PI_STATUS_REG)
    /* E8D8 8000DCD8 34630010 */  ori        $v1, $v1, %lo(PI_STATUS_REG)
    /* E8DC 8000DCDC AFBF0014 */  sw         $ra, 0x14($sp)
    /* E8E0 8000DCE0 AFA40018 */  sw         $a0, 0x18($sp)
    /* E8E4 8000DCE4 AFA5001C */  sw         $a1, 0x1C($sp)
    /* E8E8 8000DCE8 8C620000 */  lw         $v0, 0x0($v1)
    /* E8EC 8000DCEC 00C02025 */  or         $a0, $a2, $zero
    /* E8F0 8000DCF0 304E0003 */  andi       $t6, $v0, 0x3
    /* E8F4 8000DCF4 11C00005 */  beqz       $t6, .L8000DD0C
    /* E8F8 8000DCF8 00000000 */   nop
    /* E8FC 8000DCFC 8C620000 */  lw         $v0, 0x0($v1)
  .L8000DD00:
    /* E900 8000DD00 304F0003 */  andi       $t7, $v0, 0x3
    /* E904 8000DD04 55E0FFFE */  bnel       $t7, $zero, .L8000DD00
    /* E908 8000DD08 8C620000 */   lw        $v0, 0x0($v1)
  .L8000DD0C:
    /* E90C 8000DD0C 0C003570 */  jal        func_8000D5C0
    /* E910 8000DD10 AFA70024 */   sw        $a3, 0x24($sp)
    /* E914 8000DD14 8FA30018 */  lw         $v1, 0x18($sp)
    /* E918 8000DD18 8FA70024 */  lw         $a3, 0x24($sp)
    /* E91C 8000DD1C 3C18A460 */  lui        $t8, %hi(PI_DRAM_ADDR_REG)
    /* E920 8000DD20 AF020000 */  sw         $v0, %lo(PI_DRAM_ADDR_REG)($t8)
    /* E924 8000DD24 3C198000 */  lui        $t9, %hi(osRomBase)
    /* E928 8000DD28 8F390308 */  lw         $t9, %lo(osRomBase)($t9)
    /* E92C 8000DD2C 8FA8001C */  lw         $t0, 0x1C($sp)
    /* E930 8000DD30 3C011FFF */  lui        $at, (0x1FFFFFFF >> 16)
    /* E934 8000DD34 3421FFFF */  ori        $at, $at, (0x1FFFFFFF & 0xFFFF)
    /* E938 8000DD38 03284825 */  or         $t1, $t9, $t0
    /* E93C 8000DD3C 01215024 */  and        $t2, $t1, $at
    /* E940 8000DD40 3C0BA460 */  lui        $t3, %hi(PI_CART_ADDR_REG)
    /* E944 8000DD44 10600006 */  beqz       $v1, .L8000DD60
    /* E948 8000DD48 AD6A0004 */   sw        $t2, %lo(PI_CART_ADDR_REG)($t3)
    /* E94C 8000DD4C 24010001 */  addiu      $at, $zero, 0x1
    /* E950 8000DD50 10610007 */  beq        $v1, $at, .L8000DD70
    /* E954 8000DD54 24EEFFFF */   addiu     $t6, $a3, -0x1
    /* E958 8000DD58 1000000B */  b          .L8000DD88
    /* E95C 8000DD5C 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000DD60:
    /* E960 8000DD60 24ECFFFF */  addiu      $t4, $a3, -0x1
    /* E964 8000DD64 3C0DA460 */  lui        $t5, %hi(PI_WR_LEN_REG)
    /* E968 8000DD68 10000006 */  b          .L8000DD84
    /* E96C 8000DD6C ADAC000C */   sw        $t4, %lo(PI_WR_LEN_REG)($t5)
  .L8000DD70:
    /* E970 8000DD70 3C0FA460 */  lui        $t7, %hi(PI_RD_LEN_REG)
    /* E974 8000DD74 10000003 */  b          .L8000DD84
    /* E978 8000DD78 ADEE0008 */   sw        $t6, %lo(PI_RD_LEN_REG)($t7)
    /* E97C 8000DD7C 10000002 */  b          .L8000DD88
    /* E980 8000DD80 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000DD84:
    /* E984 8000DD84 00001025 */  or         $v0, $zero, $zero
  .L8000DD88:
    /* E988 8000DD88 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E98C 8000DD8C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E990 8000DD90 03E00008 */  jr         $ra
    /* E994 8000DD94 00000000 */   nop
```

## Tips for This Category

