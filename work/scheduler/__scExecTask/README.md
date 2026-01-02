# __scExecTask

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000C54` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~70 |

## Description

process task msgQ, handle watchdog

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `scheduler` category.

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
./work/scheduler/__scExecTask/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scExecTask

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scExecTask --watch
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
# Source: 1050.s
# Address: 0x80000C54

glabel func_80000C54
    /* 1854 80000C54 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1858 80000C58 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 185C 80000C5C AFA40020 */  sw         $a0, 0x20($sp)
    /* 1860 80000C60 AFA50024 */  sw         $a1, 0x24($sp)
    /* 1864 80000C64 8FAE0024 */  lw         $t6, 0x24($sp)
    /* 1868 80000C68 8DCF0004 */  lw         $t7, 0x4($t6)
    /* 186C 80000C6C 31F80003 */  andi       $t8, $t7, 0x3
    /* 1870 80000C70 17000036 */  bnez       $t8, .L80000D4C
    /* 1874 80000C74 00000000 */   nop
    /* 1878 80000C78 8FB90024 */  lw         $t9, 0x24($sp)
    /* 187C 80000C7C 8F280050 */  lw         $t0, 0x50($t9)
    /* 1880 80000C80 15000001 */  bnez       $t0, .L80000C88
    /* 1884 80000C84 00000000 */   nop
  .L80000C88:
    /* 1888 80000C88 8FA90024 */  lw         $t1, 0x24($sp)
    /* 188C 80000C8C 24060001 */  addiu      $a2, $zero, 0x1
    /* 1890 80000C90 8D240050 */  lw         $a0, 0x50($t1)
    /* 1894 80000C94 0C001D78 */  jal        func_800075E0
    /* 1898 80000C98 8D250054 */   lw        $a1, 0x54($t1)
    /* 189C 80000C9C AFA2001C */  sw         $v0, 0x1C($sp)
    /* 18A0 80000CA0 8FAA0024 */  lw         $t2, 0x24($sp)
    /* 18A4 80000CA4 24010001 */  addiu      $at, $zero, 0x1
    /* 18A8 80000CA8 8D4B0010 */  lw         $t3, 0x10($t2)
    /* 18AC 80000CAC 15610025 */  bne        $t3, $at, .L80000D44
    /* 18B0 80000CB0 00000000 */   nop
    /* 18B4 80000CB4 3C0C8003 */  lui        $t4, %hi(D_8002EB70)
    /* 18B8 80000CB8 858CEB70 */  lh         $t4, %lo(D_8002EB70)($t4)
    /* 18BC 80000CBC 3C018003 */  lui        $at, %hi(D_8002EB70)
    /* 18C0 80000CC0 258DFFFF */  addiu      $t5, $t4, -0x1
    /* 18C4 80000CC4 A42DEB70 */  sh         $t5, %lo(D_8002EB70)($at)
    /* 18C8 80000CC8 8FAE0024 */  lw         $t6, 0x24($sp)
    /* 18CC 80000CCC 8DCF0008 */  lw         $t7, 0x8($t6)
    /* 18D0 80000CD0 31F80040 */  andi       $t8, $t7, 0x40
    /* 18D4 80000CD4 1300001B */  beqz       $t8, .L80000D44
    /* 18D8 80000CD8 00000000 */   nop
    /* 18DC 80000CDC 31F90020 */  andi       $t9, $t7, 0x20
    /* 18E0 80000CE0 13200018 */  beqz       $t9, .L80000D44
    /* 18E4 80000CE4 00000000 */   nop
    /* 18E8 80000CE8 8FA80020 */  lw         $t0, 0x20($sp)
    /* 18EC 80000CEC 3C0A8003 */  lui        $t2, %hi(D_8002AFA8)
    /* 18F0 80000CF0 8D4AAFA8 */  lw         $t2, %lo(D_8002AFA8)($t2)
    /* 18F4 80000CF4 8D09027C */  lw         $t1, 0x27C($t0)
    /* 18F8 80000CF8 012A5823 */  subu       $t3, $t1, $t2
    /* 18FC 80000CFC 2D610002 */  sltiu      $at, $t3, 0x2
    /* 1900 80000D00 1420000C */  bnez       $at, .L80000D34
    /* 1904 80000D04 00000000 */   nop
    /* 1908 80000D08 8FAC0020 */  lw         $t4, 0x20($sp)
    /* 190C 80000D0C 3C018003 */  lui        $at, %hi(D_8002AFA8)
    /* 1910 80000D10 8D8D027C */  lw         $t5, 0x27C($t4)
    /* 1914 80000D14 AC2DAFA8 */  sw         $t5, %lo(D_8002AFA8)($at)
    /* 1918 80000D18 8FAE0024 */  lw         $t6, 0x24($sp)
    /* 191C 80000D1C 0C001D64 */  jal        func_80007590
    /* 1920 80000D20 8DC4000C */   lw        $a0, 0xC($t6)
    /* 1924 80000D24 0C000758 */  jal        func_80001D60
    /* 1928 80000D28 00000000 */   nop
    /* 192C 80000D2C 10000005 */  b          .L80000D44
    /* 1930 80000D30 00000000 */   nop
  .L80000D34:
    /* 1934 80000D34 8FB80024 */  lw         $t8, 0x24($sp)
    /* 1938 80000D38 3C018003 */  lui        $at, %hi(D_8002AFA4)
    /* 193C 80000D3C 8F0F000C */  lw         $t7, 0xC($t8)
    /* 1940 80000D40 AC2FAFA4 */  sw         $t7, %lo(D_8002AFA4)($at)
  .L80000D44:
    /* 1944 80000D44 10000005 */  b          .L80000D5C
    /* 1948 80000D48 24020001 */   addiu     $v0, $zero, 0x1
  .L80000D4C:
    /* 194C 80000D4C 10000003 */  b          .L80000D5C
    /* 1950 80000D50 00001025 */   or        $v0, $zero, $zero
    /* 1954 80000D54 10000001 */  b          .L80000D5C
    /* 1958 80000D58 00000000 */   nop
  .L80000D5C:
    /* 195C 80000D5C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1960 80000D60 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1964 80000D64 03E00008 */  jr         $ra
    /* 1968 80000D68 00000000 */   nop
```

## Tips for This Category

