# osSetGlobalIntMask

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800071D0` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~40 |

## Description

set global interrupt mask

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/os` category.

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
./work/libultra/os/osSetGlobalIntMask/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osSetGlobalIntMask

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osSetGlobalIntMask --watch
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
# Source: 7C80.s
# Address: 0x800071D0

glabel func_800071D0
    /* 7DD0 800071D0 400C6000 */  mfc0       $t4, $12 /* handwritten instruction */
    /* 7DD4 800071D4 3182FF01 */  andi       $v0, $t4, 0xFF01
    /* 7DD8 800071D8 3C088003 */  lui        $t0, %hi(D_8002C370)
    /* 7DDC 800071DC 2508C370 */  addiu      $t0, $t0, %lo(D_8002C370)
    /* 7DE0 800071E0 8D0B0000 */  lw         $t3, 0x0($t0)
    /* 7DE4 800071E4 2401FFFF */  addiu      $at, $zero, -0x1
    /* 7DE8 800071E8 01614026 */  xor        $t0, $t3, $at
    /* 7DEC 800071EC 3108FF00 */  andi       $t0, $t0, 0xFF00
    /* 7DF0 800071F0 00481025 */  or         $v0, $v0, $t0
    /* 7DF4 800071F4 3C0AA430 */  lui        $t2, %hi(MI_INTR_MASK_REG)
    /* 7DF8 800071F8 8D4A000C */  lw         $t2, %lo(MI_INTR_MASK_REG)($t2)
    /* 7DFC 800071FC 11400005 */  beqz       $t2, .L80007214
    /* 7E00 80007200 000B4C02 */   srl       $t1, $t3, 16
    /* 7E04 80007204 2401FFFF */  addiu      $at, $zero, -0x1
    /* 7E08 80007208 01214826 */  xor        $t1, $t1, $at
    /* 7E0C 8000720C 3129003F */  andi       $t1, $t1, 0x3F
    /* 7E10 80007210 01495025 */  or         $t2, $t2, $t1
  .L80007214:
    /* 7E14 80007214 000A5400 */  sll        $t2, $t2, 16
    /* 7E18 80007218 004A1025 */  or         $v0, $v0, $t2
    /* 7E1C 8000721C 3C01003F */  lui        $at, (0x3F0000 >> 16)
    /* 7E20 80007220 00814024 */  and        $t0, $a0, $at
    /* 7E24 80007224 010B4024 */  and        $t0, $t0, $t3
    /* 7E28 80007228 000843C2 */  srl        $t0, $t0, 15
    /* 7E2C 8000722C 3C0A8003 */  lui        $t2, %hi(D_8002D6D0)
    /* 7E30 80007230 01485021 */  addu       $t2, $t2, $t0
    /* 7E34 80007234 954AD6D0 */  lhu        $t2, %lo(D_8002D6D0)($t2)
    /* 7E38 80007238 3C01A430 */  lui        $at, %hi(MI_INTR_MASK_REG)
    /* 7E3C 8000723C AC2A000C */  sw         $t2, %lo(MI_INTR_MASK_REG)($at)
    /* 7E40 80007240 3088FF01 */  andi       $t0, $a0, 0xFF01
    /* 7E44 80007244 3169FF00 */  andi       $t1, $t3, 0xFF00
    /* 7E48 80007248 01094024 */  and        $t0, $t0, $t1
    /* 7E4C 8000724C 3C01FFFF */  lui        $at, (0xFFFF00FF >> 16)
    /* 7E50 80007250 342100FF */  ori        $at, $at, (0xFFFF00FF & 0xFFFF)
    /* 7E54 80007254 01816024 */  and        $t4, $t4, $at
    /* 7E58 80007258 01886025 */  or         $t4, $t4, $t0
    /* 7E5C 8000725C 408C6000 */  mtc0       $t4, $12 /* handwritten instruction */
    /* 7E60 80007260 00000000 */  nop
    /* 7E64 80007264 00000000 */  nop
    /* 7E68 80007268 03E00008 */  jr         $ra
    /* 7E6C 8000726C 00000000 */   nop
```

## Tips for This Category

