# Source: game_code.bin (decompressed)
# Address: 0x800D5C90

glabel player_respawn
    /* 800D5C90 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800D5C94 AFA40028 */  sw $a0, 40($sp)
    /* 800D5C98 AFBF001C */  sw $ra, 28($sp)
    /* 800D5C9C AFA5002C */  sw $a1, 44($sp)
    /* 800D5CA0 AFA60030 */  sw $a2, 48($sp)
    /* 800D5CA4 3C048014 */  lui $a0, 0x8014
    /* 800D5CA8 AFB10018 */  sw $s1, 24($sp)
    /* 800D5CAC AFB00014 */  sw $s0, 20($sp)
    /* 800D5CB0 AFA70034 */  sw $a3, 52($sp)
    /* 800D5CB4 24842728 */  addiu $a0, $a0, 10024
    /* 800D5CB8 24060001 */  addiu $a2, $zero, 1
    /* 800D5CBC 0C001C9C */  jal 0x80007270
    /* 800D5CC0 00002825 */  or $a1, $zero, $zero
    /* 800D5CC4 0C0246C0 */  jal 0x80091B00
    /* 800D5CC8 00000000 */  nop
    /* 800D5CCC 240E0002 */  addiu $t6, $zero, 2
    /* 800D5CD0 A04E0002 */  sb $t6, 2($v0)
    /* 800D5CD4 8FAF0030 */  lw $t7, 48($sp)
    /* 800D5CD8 AC4F0004 */  sw $t7, 4($v0)
    /* 800D5CDC 0C02489E */  jal 0x80092278
    /* 800D5CE0 AFA20024 */  sw $v0, 36($sp)
    /* 800D5CE4 8FA70024 */  lw $a3, 36($sp)
    /* 800D5CE8 8FA30028 */  lw $v1, 40($sp)
    /* 800D5CEC C7A20038 */  lwc1 $f2, 56($sp)
    /* 800D5CF0 ACE20014 */  sw $v0, 20($a3)
    /* 800D5CF4 AC400014 */  sw $zero, 20($v0)
    /* 800D5CF8 8CF80014 */  lw $t8, 20($a3)
    /* 800D5CFC 44800000 */  .word 0x44800000
    /* 800D5D00 3C048014 */  lui $a0, 0x8014
    /* 800D5D04 AF030034 */  sw $v1, 52($t8)
    /* 800D5D08 8FB9002C */  lw $t9, 44($sp)
    /* 800D5D0C 8CEE0014 */  lw $t6, 20($a3)
    /* 800D5D10 4600103C */  .word 0x4600103C
    /* 800D5D14 3C013F80 */  lui $at, 0x3F80
    /* 800D5D18 ADD90038 */  sw $t9, 56($t6)
    /* 800D5D1C 93AF0037 */  lbu $t7, 55($sp)
    /* 800D5D20 8CF80014 */  lw $t8, 20($a3)
    /* 800D5D24 24190001 */  addiu $t9, $zero, 1
    /* 800D5D28 24842728 */  addiu $a0, $a0, 10024
    /* 800D5D2C A30F001B */  sb $t7, 27($t8)
    /* 800D5D30 8CEE0014 */  lw $t6, 20($a3)
    /* 800D5D34 24180001 */  addiu $t8, $zero, 1
    /* 800D5D38 ADD90010 */  sw $t9, 16($t6)
    /* 800D5D3C 8CEF0014 */  lw $t7, 20($a3)
    /* 800D5D40 3C0E8012 */  lui $t6, 0x8012
    /* 800D5D44 01C37021 */  addu $t6, $t6, $v1
    /* 800D5D48 A1E00018 */  sb $zero, 24($t7)
    /* 800D5D4C 8CF90014 */  lw $t9, 20($a3)
    /* 800D5D50 A338001A */  sb $t8, 26($t9)
    /* 800D5D54 91CEF5CC */  lbu $t6, -2612($t6)
    /* 800D5D58 8CEF0014 */  lw $t7, 20($a3)
    /* 800D5D5C 45000005 */  .word 0x45000005
    /* 800D5D60 A1EE0019 */  sb $t6, 25($t7)
    /* 800D5D64 3C013F80 */  lui $at, 0x3F80
    /* 800D5D68 44817000 */  .word 0x44817000
    /* 800D5D6C 1000000B */  beq $zero, $zero, 0x800D5D9C
    /* 800D5D70 E4E00008 */  swc1 $f0, 8($a3)
    /* 800D5D74 44817000 */  .word 0x44817000
    /* 800D5D78 00000000 */  nop
    /* 800D5D7C 4602703C */  .word 0x4602703C
    /* 800D5D80 00000000 */  nop
    /* 800D5D84 45020004 */  .word 0x45020004
    /* 800D5D88 46001006 */  .word 0x46001006
    /* 800D5D8C 10000002 */  beq $zero, $zero, 0x800D5D98
    /* 800D5D90 46007006 */  .word 0x46007006
    /* 800D5D94 46001006 */  .word 0x46001006
    /* 800D5D98 E4E00008 */  swc1 $f0, 8($a3)
    /* 800D5D9C 3C01BF80 */  lui $at, 0xBF80
    /* 800D5DA0 44816000 */  .word 0x44816000
    /* 800D5DA4 C7A2003C */  lwc1 $f2, 60($sp)
    /* 800D5DA8 460C103C */  .word 0x460C103C
    /* 800D5DAC 00000000 */  nop
    /* 800D5DB0 45020004 */  .word 0x45020004
    /* 800D5DB4 4602703C */  .word 0x4602703C
    /* 800D5DB8 10000009 */  beq $zero, $zero, 0x800D5DE0
    /* 800D5DBC E4EC000C */  swc1 $f12, 12($a3)
    /* 800D5DC0 4602703C */  .word 0x4602703C
    /* 800D5DC4 00000000 */  nop
    /* 800D5DC8 45020004 */  .word 0x45020004
    /* 800D5DCC 46001006 */  .word 0x46001006
    /* 800D5DD0 10000002 */  beq $zero, $zero, 0x800D5DDC
    /* 800D5DD4 46007006 */  .word 0x46007006
    /* 800D5DD8 46001006 */  .word 0x46001006
    /* 800D5DDC E4E0000C */  swc1 $f0, 12($a3)
    /* 800D5DE0 C7A20040 */  lwc1 $f2, 64($sp)
    /* 800D5DE4 460C103C */  .word 0x460C103C
    /* 800D5DE8 00000000 */  nop
    /* 800D5DEC 45020004 */  .word 0x45020004
    /* 800D5DF0 4602703C */  .word 0x4602703C
    /* 800D5DF4 10000009 */  beq $zero, $zero, 0x800D5E1C
    /* 800D5DF8 E4EC0010 */  swc1 $f12, 16($a3)
    /* 800D5DFC 4602703C */  .word 0x4602703C
    /* 800D5E00 00000000 */  nop
    /* 800D5E04 45020004 */  .word 0x45020004
    /* 800D5E08 46001006 */  .word 0x46001006
    /* 800D5E0C 10000002 */  beq $zero, $zero, 0x800D5E18
    /* 800D5E10 46007006 */  .word 0x46007006
    /* 800D5E14 46001006 */  .word 0x46001006
    /* 800D5E18 E4E00010 */  swc1 $f0, 16($a3)
    /* 800D5E1C 8CF80014 */  lw $t8, 20($a3)
    /* 800D5E20 00002825 */  or $a1, $zero, $zero
    /* 800D5E24 00003025 */  or $a2, $zero, $zero
    /* 800D5E28 8F19000C */  lw $t9, 12($t8)
    /* 800D5E2C AFA70024 */  sw $a3, 36($sp)
    /* 800D5E30 0C001D78 */  jal 0x800075E0
    /* 800D5E34 AFB90020 */  sw $t9, 32($sp)
    /* 800D5E38 3C048014 */  lui $a0, 0x8014
    /* 800D5E3C 8FA50024 */  lw $a1, 36($sp)
    /* 800D5E40 248427A8 */  addiu $a0, $a0, 10152
    /* 800D5E44 0C001D78 */  jal 0x800075E0
    /* 800D5E48 00003025 */  or $a2, $zero, $zero
    /* 800D5E4C 8FBF001C */  lw $ra, 28($sp)
    /* 800D5E50 8FA20020 */  lw $v0, 32($sp)
    /* 800D5E54 8FB00014 */  lw $s0, 20($sp)
    /* 800D5E58 8FB10018 */  lw $s1, 24($sp)
    /* 800D5E5C 03E00008 */  jr $ra
    /* 800D5E60 27BD0028 */  addiu $sp, $sp, 40
