# __osContBuildPacket

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009450` |
| **Category** | `libultra/cont` |
| **Status** | `TODO` |
| **Instructions** | ~87 |

## Description

build controller pak read/write command

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/cont` category.

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
./work/libultra/cont/__osContBuildPacket/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cont/__osContBuildPacket

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cont/__osContBuildPacket --watch
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
# Source: A050.s
# Address: 0x80009450

glabel func_80009450
    /* A050 80009450 3C028003 */  lui        $v0, %hi(D_8002C3B0)
    /* A054 80009454 2442C3B0 */  addiu      $v0, $v0, %lo(D_8002C3B0)
    /* A058 80009458 8C4E0000 */  lw         $t6, 0x0($v0)
    /* A05C 8000945C 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* A060 80009460 AFBF0024 */  sw         $ra, 0x24($sp)
    /* A064 80009464 AFA40070 */  sw         $a0, 0x70($sp)
    /* A068 80009468 AFA50074 */  sw         $a1, 0x74($sp)
    /* A06C 8000946C 11C00003 */  beqz       $t6, .L8000947C
    /* A070 80009470 AFA60078 */   sw        $a2, 0x78($sp)
    /* A074 80009474 10000049 */  b          .L8000959C
    /* A078 80009478 00001025 */   or        $v0, $zero, $zero
  .L8000947C:
    /* A07C 8000947C 240F0001 */  addiu      $t7, $zero, 0x1
    /* A080 80009480 0C001CEC */  jal        func_800073B0
    /* A084 80009484 AC4F0000 */   sw        $t7, 0x0($v0)
    /* A088 80009488 AFA20060 */  sw         $v0, 0x60($sp)
    /* A08C 8000948C 14400020 */  bnez       $v0, .L80009510
    /* A090 80009490 AFA30064 */   sw        $v1, 0x64($sp)
    /* A094 80009494 3C010165 */  lui        $at, (0x165A0BC >> 16)
    /* A098 80009498 3421A0BC */  ori        $at, $at, (0x165A0BC & 0xFFFF)
    /* A09C 8000949C 0061082B */  sltu       $at, $v1, $at
    /* A0A0 800094A0 1020001B */  beqz       $at, .L80009510
    /* A0A4 800094A4 27A40028 */   addiu     $a0, $sp, 0x28
    /* A0A8 800094A8 27A5006C */  addiu      $a1, $sp, 0x6C
    /* A0AC 800094AC 0C001A80 */  jal        func_80006A00
    /* A0B0 800094B0 24060001 */   addiu     $a2, $zero, 0x1
    /* A0B4 800094B4 8FB80060 */  lw         $t8, 0x60($sp)
    /* A0B8 800094B8 8FB90064 */  lw         $t9, 0x64($sp)
    /* A0BC 800094BC 3C090165 */  lui        $t1, (0x165A0BC >> 16)
    /* A0C0 800094C0 3529A0BC */  ori        $t1, $t1, (0x165A0BC & 0xFFFF)
    /* A0C4 800094C4 24080000 */  addiu      $t0, $zero, 0x0
    /* A0C8 800094C8 240A0000 */  addiu      $t2, $zero, 0x0
    /* A0CC 800094CC 240B0000 */  addiu      $t3, $zero, 0x0
    /* A0D0 800094D0 27AC0028 */  addiu      $t4, $sp, 0x28
    /* A0D4 800094D4 27AD006C */  addiu      $t5, $sp, 0x6C
    /* A0D8 800094D8 01183023 */  subu       $a2, $t0, $t8
    /* A0DC 800094DC 0139082B */  sltu       $at, $t1, $t9
    /* A0E0 800094E0 00C13023 */  subu       $a2, $a2, $at
    /* A0E4 800094E4 AFAD001C */  sw         $t5, 0x1C($sp)
    /* A0E8 800094E8 AFAC0018 */  sw         $t4, 0x18($sp)
    /* A0EC 800094EC AFAB0014 */  sw         $t3, 0x14($sp)
    /* A0F0 800094F0 AFAA0010 */  sw         $t2, 0x10($sp)
    /* A0F4 800094F4 27A40040 */  addiu      $a0, $sp, 0x40
    /* A0F8 800094F8 0C0038F4 */  jal        func_8000E3D0
    /* A0FC 800094FC 01393823 */   subu      $a3, $t1, $t9
    /* A100 80009500 27A40028 */  addiu      $a0, $sp, 0x28
    /* A104 80009504 27A5006C */  addiu      $a1, $sp, 0x6C
    /* A108 80009508 0C001C9C */  jal        func_80007270
    /* A10C 8000950C 24060001 */   addiu     $a2, $zero, 0x1
  .L80009510:
    /* A110 80009510 240E0004 */  addiu      $t6, $zero, 0x4
    /* A114 80009514 3C018003 */  lui        $at, %hi(D_80037AE1)
    /* A118 80009518 A02E7AE1 */  sb         $t6, %lo(D_80037AE1)($at)
    /* A11C 8000951C 0C002596 */  jal        func_80009658
    /* A120 80009520 00002025 */   or        $a0, $zero, $zero
    /* A124 80009524 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A128 80009528 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A12C 8000952C 0C00392C */  jal        func_8000E4B0
    /* A130 80009530 24040001 */   addiu     $a0, $zero, 0x1
    /* A134 80009534 8FA40070 */  lw         $a0, 0x70($sp)
    /* A138 80009538 27A5006C */  addiu      $a1, $sp, 0x6C
    /* A13C 8000953C 0C001C9C */  jal        func_80007270
    /* A140 80009540 24060001 */   addiu     $a2, $zero, 0x1
    /* A144 80009544 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A148 80009548 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A14C 8000954C 0C00392C */  jal        func_8000E4B0
    /* A150 80009550 00002025 */   or        $a0, $zero, $zero
    /* A154 80009554 AFA20068 */  sw         $v0, 0x68($sp)
    /* A158 80009558 8FA40070 */  lw         $a0, 0x70($sp)
    /* A15C 8000955C 27A5006C */  addiu      $a1, $sp, 0x6C
    /* A160 80009560 0C001C9C */  jal        func_80007270
    /* A164 80009564 24060001 */   addiu     $a2, $zero, 0x1
    /* A168 80009568 8FA40074 */  lw         $a0, 0x74($sp)
    /* A16C 8000956C 0C00256B */  jal        func_800095AC
    /* A170 80009570 8FA50078 */   lw        $a1, 0x78($sp)
    /* A174 80009574 3C018003 */  lui        $at, %hi(D_80037AE0)
    /* A178 80009578 0C003958 */  jal        func_8000E560
    /* A17C 8000957C A0207AE0 */   sb        $zero, %lo(D_80037AE0)($at)
    /* A180 80009580 3C048003 */  lui        $a0, %hi(D_80037B08)
    /* A184 80009584 3C058003 */  lui        $a1, %hi(D_80037B20)
    /* A188 80009588 24A57B20 */  addiu      $a1, $a1, %lo(D_80037B20)
    /* A18C 8000958C 24847B08 */  addiu      $a0, $a0, %lo(D_80037B08)
    /* A190 80009590 0C001A80 */  jal        func_80006A00
    /* A194 80009594 24060001 */   addiu     $a2, $zero, 0x1
    /* A198 80009598 8FA20068 */  lw         $v0, 0x68($sp)
  .L8000959C:
    /* A19C 8000959C 8FBF0024 */  lw         $ra, 0x24($sp)
    /* A1A0 800095A0 27BD0070 */  addiu      $sp, $sp, 0x70
    /* A1A4 800095A4 03E00008 */  jr         $ra
    /* A1A8 800095A8 00000000 */   nop
```

## Tips for This Category

