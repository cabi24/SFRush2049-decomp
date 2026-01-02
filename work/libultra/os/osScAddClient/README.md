# osScAddClient

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800005D4` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~26 |

## Description

link client into scheduler clientList

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
./work/libultra/os/osScAddClient/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osScAddClient

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osScAddClient --watch
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
# Source: 1050.s
# Address: 0x800005D4

glabel func_800005D4
    /* 11D4 800005D4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 11D8 800005D8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 11DC 800005DC AFA40020 */  sw         $a0, 0x20($sp)
    /* 11E0 800005E0 AFA50024 */  sw         $a1, 0x24($sp)
    /* 11E4 800005E4 AFA60028 */  sw         $a2, 0x28($sp)
    /* 11E8 800005E8 0C001C74 */  jal        func_800071D0
    /* 11EC 800005EC 24040001 */   addiu     $a0, $zero, 0x1
    /* 11F0 800005F0 AFA2001C */  sw         $v0, 0x1C($sp)
    /* 11F4 800005F4 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 11F8 800005F8 8FAF0024 */  lw         $t7, 0x24($sp)
    /* 11FC 800005FC ADEE0004 */  sw         $t6, 0x4($t7)
    /* 1200 80000600 8FB80020 */  lw         $t8, 0x20($sp)
    /* 1204 80000604 8FA80024 */  lw         $t0, 0x24($sp)
    /* 1208 80000608 8F190260 */  lw         $t9, 0x260($t8)
    /* 120C 8000060C AD190000 */  sw         $t9, 0x0($t0)
    /* 1210 80000610 8FA90024 */  lw         $t1, 0x24($sp)
    /* 1214 80000614 8FAA0020 */  lw         $t2, 0x20($sp)
    /* 1218 80000618 AD490260 */  sw         $t1, 0x260($t2)
    /* 121C 8000061C 0C001C74 */  jal        func_800071D0
    /* 1220 80000620 8FA4001C */   lw        $a0, 0x1C($sp)
    /* 1224 80000624 10000001 */  b          .L8000062C
    /* 1228 80000628 00000000 */   nop
  .L8000062C:
    /* 122C 8000062C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1230 80000630 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1234 80000634 03E00008 */  jr         $ra
    /* 1238 80000638 00000000 */   nop
```

## Tips for This Category

