# idle_thread_entry

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800021A4` |
| **Category** | `libultra/thread` |
| **Status** | `TODO` |
| **Instructions** | ~37 |

## Description

boot idle thread: inits PI manager, creates game thread, loops forever

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/thread` category.

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
./work/libultra/thread/idle_thread_entry/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/thread/idle_thread_entry

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/thread/idle_thread_entry --watch
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
# Source: 2CF0.s
# Address: 0x800021A4

glabel func_800021A4
    /* 2DA4 800021A4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 2DA8 800021A8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 2DAC 800021AC AFA40020 */  sw         $a0, 0x20($sp)
    /* 2DB0 800021B0 3C058003 */  lui        $a1, %hi(D_8002EE08)
    /* 2DB4 800021B4 3C068003 */  lui        $a2, %hi(D_8002EE20)
    /* 2DB8 800021B8 24C6EE20 */  addiu      $a2, $a2, %lo(D_8002EE20)
    /* 2DBC 800021BC 24A5EE08 */  addiu      $a1, $a1, %lo(D_8002EE08)
    /* 2DC0 800021C0 24040096 */  addiu      $a0, $zero, 0x96
    /* 2DC4 800021C4 0C002084 */  jal        func_80008210
    /* 2DC8 800021C8 240700C8 */   addiu     $a3, $zero, 0xC8
    /* 2DCC 800021CC 00002025 */  or         $a0, $zero, $zero
    /* 2DD0 800021D0 0C0020E0 */  jal        func_80008380
    /* 2DD4 800021D4 00002825 */   or        $a1, $zero, $zero
    /* 2DD8 800021D8 3C0E8003 */  lui        $t6, %hi(D_80030150)
    /* 2DDC 800021DC 25CE0150 */  addiu      $t6, $t6, %lo(D_80030150)
    /* 2DE0 800021E0 25CF0960 */  addiu      $t7, $t6, 0x960
    /* 2DE4 800021E4 3C048003 */  lui        $a0, %hi(D_80034D50)
    /* 2DE8 800021E8 3C068000 */  lui        $a2, %hi(func_80002238)
    /* 2DEC 800021EC 24180004 */  addiu      $t8, $zero, 0x4
    /* 2DF0 800021F0 AFB80014 */  sw         $t8, 0x14($sp)
    /* 2DF4 800021F4 24C62238 */  addiu      $a2, $a2, %lo(func_80002238)
    /* 2DF8 800021F8 24844D50 */  addiu      $a0, $a0, %lo(D_80034D50)
    /* 2DFC 800021FC AFAF0010 */  sw         $t7, 0x10($sp)
    /* 2E00 80002200 24050006 */  addiu      $a1, $zero, 0x6
    /* 2E04 80002204 0C001BCC */  jal        func_80006F30
    /* 2E08 80002208 8FA70020 */   lw        $a3, 0x20($sp)
    /* 2E0C 8000220C 3C048003 */  lui        $a0, %hi(D_80034D50)
    /* 2E10 80002210 0C001C20 */  jal        func_80007080
    /* 2E14 80002214 24844D50 */   addiu     $a0, $a0, %lo(D_80034D50)
  .L80002218:
    /* 2E18 80002218 1000FFFF */  b          .L80002218
    /* 2E1C 8000221C 00000000 */   nop
    /* 2E20 80002220 10000001 */  b          .L80002228
    /* 2E24 80002224 00000000 */   nop
  .L80002228:
    /* 2E28 80002228 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 2E2C 8000222C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 2E30 80002230 03E00008 */  jr         $ra
    /* 2E34 80002234 00000000 */   nop
```

## Tips for This Category

