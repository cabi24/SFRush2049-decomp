# __scHandleRetrace

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000880` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~52 |

## Description

VI retrace handler, sends msgs to clients

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
./work/scheduler/__scHandleRetrace/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scHandleRetrace

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scHandleRetrace --watch
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
# Address: 0x80000880

glabel func_80000880
    /* 1480 80000880 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1484 80000884 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1488 80000888 AFA40030 */  sw         $a0, 0x30($sp)
    /* 148C 8000088C AFA00020 */  sw         $zero, 0x20($sp)
    /* 1490 80000890 AFA0001C */  sw         $zero, 0x1C($sp)
    /* 1494 80000894 8FAE0030 */  lw         $t6, 0x30($sp)
    /* 1498 80000898 8DCF027C */  lw         $t7, 0x27C($t6)
    /* 149C 8000089C 25F80001 */  addiu      $t8, $t7, 0x1
    /* 14A0 800008A0 ADD8027C */  sw         $t8, 0x27C($t6)
    /* 14A4 800008A4 3C198003 */  lui        $t9, %hi(D_8002AFA4)
    /* 14A8 800008A8 8F39AFA4 */  lw         $t9, %lo(D_8002AFA4)($t9)
    /* 14AC 800008AC 13200015 */  beqz       $t9, .L80000904
    /* 14B0 800008B0 00000000 */   nop
    /* 14B4 800008B4 8FA80030 */  lw         $t0, 0x30($sp)
    /* 14B8 800008B8 3C0A8003 */  lui        $t2, %hi(D_8002AFA8)
    /* 14BC 800008BC 8D4AAFA8 */  lw         $t2, %lo(D_8002AFA8)($t2)
    /* 14C0 800008C0 8D09027C */  lw         $t1, 0x27C($t0)
    /* 14C4 800008C4 012A5823 */  subu       $t3, $t1, $t2
    /* 14C8 800008C8 2D610002 */  sltiu      $at, $t3, 0x2
    /* 14CC 800008CC 1420000D */  bnez       $at, .L80000904
    /* 14D0 800008D0 00000000 */   nop
    /* 14D4 800008D4 3C048003 */  lui        $a0, %hi(D_8002AFA4)
    /* 14D8 800008D8 0C001D64 */  jal        func_80007590
    /* 14DC 800008DC 8C84AFA4 */   lw        $a0, %lo(D_8002AFA4)($a0)
    /* 14E0 800008E0 0C000758 */  jal        func_80001D60
    /* 14E4 800008E4 00000000 */   nop
    /* 14E8 800008E8 8FAC0030 */  lw         $t4, 0x30($sp)
    /* 14EC 800008EC 3C018003 */  lui        $at, %hi(D_8002AFA8)
    /* 14F0 800008F0 8D8D027C */  lw         $t5, 0x27C($t4)
    /* 14F4 800008F4 AC2DAFA8 */  sw         $t5, %lo(D_8002AFA8)($at)
    /* 14F8 800008F8 3C018003 */  lui        $at, %hi(D_8002AFA4)
    /* 14FC 800008FC 10000001 */  b          .L80000904
    /* 1500 80000900 AC20AFA4 */   sw        $zero, %lo(D_8002AFA4)($at)
  .L80000904:
    /* 1504 80000904 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 1508 80000908 8DF80260 */  lw         $t8, 0x260($t7)
    /* 150C 8000090C 1300000A */  beqz       $t8, .L80000938
    /* 1510 80000910 AFB80028 */   sw        $t8, 0x28($sp)
  .L80000914:
    /* 1514 80000914 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 1518 80000918 8FA50030 */  lw         $a1, 0x30($sp)
    /* 151C 8000091C 00003025 */  or         $a2, $zero, $zero
    /* 1520 80000920 0C001D78 */  jal        func_800075E0
    /* 1524 80000924 8DC40004 */   lw        $a0, 0x4($t6)
    /* 1528 80000928 8FB90028 */  lw         $t9, 0x28($sp)
    /* 152C 8000092C 8F280000 */  lw         $t0, 0x0($t9)
    /* 1530 80000930 1500FFF8 */  bnez       $t0, .L80000914
    /* 1534 80000934 AFA80028 */   sw        $t0, 0x28($sp)
  .L80000938:
    /* 1538 80000938 10000001 */  b          .L80000940
    /* 153C 8000093C 00000000 */   nop
  .L80000940:
    /* 1540 80000940 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1544 80000944 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 1548 80000948 03E00008 */  jr         $ra
    /* 154C 8000094C 00000000 */   nop
```

## Tips for This Category

