# Source: game_code.bin (decompressed)
# Address: 0x800F8B70

glabel finish_state_alt
    /* 800F8B70 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800F8B74 3C0E8011 */  lui $t6, 0x8011
    /* 800F8B78 81CE4728 */  lb $t6, 18216($t6)
    /* 800F8B7C AFBF0034 */  sw $ra, 52($sp)
    /* 800F8B80 AFB70030 */  sw $s7, 48($sp)
    /* 800F8B84 AFB6002C */  sw $s6, 44($sp)
    /* 800F8B88 AFB50028 */  sw $s5, 40($sp)
    /* 800F8B8C AFB40024 */  sw $s4, 36($sp)
    /* 800F8B90 AFB30020 */  sw $s3, 32($sp)
    /* 800F8B94 AFB2001C */  sw $s2, 28($sp)
    /* 800F8B98 AFB10018 */  sw $s1, 24($sp)
    /* 800F8B9C 15C00020 */  bne $t6, $zero, 0x800F8C20
    /* 800F8BA0 AFB00014 */  sw $s0, 20($sp)
    /* 800F8BA4 0C03E15F */  jal 0x800F857C
    /* 800F8BA8 24100001 */  addiu $s0, $zero, 1
    /* 800F8BAC 3C078014 */  lui $a3, 0x8014
    /* 800F8BB0 24E791E0 */  addiu $a3, $a3, -28192
    /* 800F8BB4 ACE00000 */  sw $zero, 0($a3)
    /* 800F8BB8 0C03E12C */  jal 0x800F84B0
    /* 800F8BBC 00002025 */  or $a0, $zero, $zero
    /* 800F8BC0 1440000C */  bne $v0, $zero, 0x800F8BF4
    /* 800F8BC4 00000000 */  nop
    /* 800F8BC8 8CEF0000 */  lw $t7, 0($a3)
    /* 800F8BCC 25E40001 */  addiu $a0, $t7, 1
    /* 800F8BD0 28810007 */  slti $at, $a0, 7
    /* 800F8BD4 14200003 */  bne $at, $zero, 0x800F8BE4
    /* 800F8BD8 ACE40000 */  sw $a0, 0($a3)
    /* 800F8BDC 10000005 */  beq $zero, $zero, 0x800F8BF4
    /* 800F8BE0 ACE00000 */  sw $zero, 0($a3)
    /* 800F8BE4 0C03E12C */  jal 0x800F84B0
    /* 800F8BE8 00000000 */  nop
    /* 800F8BEC 5040FFF7 */  .word 0x5040FFF7
    /* 800F8BF0 8CEF0000 */  lw $t7, 0($a3)
    /* 800F8BF4 3C068011 */  lui $a2, 0x8011
    /* 800F8BF8 24C64700 */  addiu $a2, $a2, 18176
    /* 800F8BFC 00002025 */  or $a0, $zero, $zero
    /* 800F8C00 00002825 */  or $a1, $zero, $zero
    /* 800F8C04 0C02CDFA */  jal 0x800B37E8
    /* 800F8C08 24070001 */  addiu $a3, $zero, 1
    /* 800F8C0C 3C018011 */  lui $at, 0x8011
    /* 800F8C10 AC22472C */  sw $v0, 18220($at)
    /* 800F8C14 3C018011 */  lui $at, 0x8011
    /* 800F8C18 24190001 */  addiu $t9, $zero, 1
    /* 800F8C1C A0394728 */  sb $t9, 18216($at)
    /* 800F8C20 0C033F2A */  jal 0x800CFCA8
    /* 800F8C24 00000000 */  nop
    /* 800F8C28 3C0E8015 */  lui $t6, 0x8015
    /* 800F8C2C 81CE7244 */  lb $t6, 29252($t6)
    /* 800F8C30 3C048014 */  lui $a0, 0x8014
    /* 800F8C34 24842728 */  addiu $a0, $a0, 10024
    /* 800F8C38 11C00039 */  beq $t6, $zero, 0x800F8D20
    /* 800F8C3C 3C028014 */  lui $v0, 0x8014
    /* 800F8C40 00002825 */  or $a1, $zero, $zero
    /* 800F8C44 0C001C9C */  jal 0x80007270
    /* 800F8C48 24060001 */  addiu $a2, $zero, 1
    /* 800F8C4C 0C0246C0 */  jal 0x80091B00
    /* 800F8C50 00000000 */  nop
    /* 800F8C54 240F0001 */  addiu $t7, $zero, 1
    /* 800F8C58 3C048014 */  lui $a0, 0x8014
    /* 800F8C5C 24842728 */  addiu $a0, $a0, 10024
    /* 800F8C60 A04F0002 */  sb $t7, 2($v0)
    /* 800F8C64 00408025 */  or $s0, $v0, $zero
    /* 800F8C68 00002825 */  or $a1, $zero, $zero
    /* 800F8C6C 0C001D78 */  jal 0x800075E0
    /* 800F8C70 00003025 */  or $a2, $zero, $zero
    /* 800F8C74 3C048014 */  lui $a0, 0x8014
    /* 800F8C78 248427A8 */  addiu $a0, $a0, 10152
    /* 800F8C7C 02002825 */  or $a1, $s0, $zero
    /* 800F8C80 0C001D78 */  jal 0x800075E0
    /* 800F8C84 00003025 */  or $a2, $zero, $zero
    /* 800F8C88 3C048015 */  lui $a0, 0x8015
    /* 800F8C8C 0C02E44B */  jal 0x800B912C
    /* 800F8C90 8084978C */  lb $a0, -26740($a0)
    /* 800F8C94 3C108014 */  lui $s0, 0x8014
    /* 800F8C98 8E1091F0 */  lw $s0, -28176($s0)
    /* 800F8C9C 12000007 */  beq $s0, $zero, 0x800F8CBC
    /* 800F8CA0 02002025 */  or $a0, $s0, $zero
    /* 800F8CA4 0C0241E7 */  jal 0x8009079C
    /* 800F8CA8 24050001 */  addiu $a1, $zero, 1
    /* 800F8CAC 3C108014 */  lui $s0, 0x8014
    /* 800F8CB0 8E1091F0 */  lw $s0, -28176($s0)
    /* 800F8CB4 5600FFFB */  .word 0x5600FFFB
    /* 800F8CB8 02002025 */  or $a0, $s0, $zero
    /* 800F8CBC 0C02C160 */  jal 0x800B0580
    /* 800F8CC0 00000000 */  nop
    /* 800F8CC4 24180001 */  addiu $t8, $zero, 1
    /* 800F8CC8 3C018015 */  lui $at, 0x8015
    /* 800F8CCC A4381AD0 */  sh $t8, 6864($at)
    /* 800F8CD0 0C029AF9 */  jal 0x800A6BE4
    /* 800F8CD4 24040001 */  addiu $a0, $zero, 1
    /* 800F8CD8 3C108015 */  lui $s0, 0x8015
    /* 800F8CDC 261041A4 */  addiu $s0, $s0, 16804
    /* 800F8CE0 8E040000 */  lw $a0, 0($s0)
    /* 800F8CE4 10800004 */  beq $a0, $zero, 0x800F8CF8
    /* 800F8CE8 00000000 */  nop
    /* 800F8CEC 0C02CD63 */  jal 0x800B358C
    /* 800F8CF0 00000000 */  nop
    /* 800F8CF4 AE000000 */  sw $zero, 0($s0)
    /* 800F8CF8 0C03DD12 */  jal 0x800F7448
    /* 800F8CFC 24040001 */  addiu $a0, $zero, 1
    /* 800F8D00 24190001 */  addiu $t9, $zero, 1
    /* 800F8D04 3C018011 */  lui $at, 0x8011
    /* 800F8D08 0C03E1D5 */  jal 0x800F8754
    /* 800F8D0C AC3974BC */  sw $t9, 29884($at)
    /* 800F8D10 240E0004 */  addiu $t6, $zero, 4
    /* 800F8D14 3C018011 */  lui $at, 0x8011
    /* 800F8D18 10000015 */  beq $zero, $zero, 0x800F8D70
    /* 800F8D1C AC2E74B8 */  sw $t6, 29880($at)
    /* 800F8D20 8C428870 */  lw $v0, -30608($v0)
    /* 800F8D24 24010001 */  addiu $at, $zero, 1
    /* 800F8D28 10410005 */  beq $v0, $at, 0x800F8D40
    /* 800F8D2C 24010002 */  addiu $at, $zero, 2
    /* 800F8D30 10410007 */  beq $v0, $at, 0x800F8D50
    /* 800F8D34 3C0F8015 */  lui $t7, 0x8015
    /* 800F8D38 1000000E */  beq $zero, $zero, 0x800F8D74
    /* 800F8D3C 8FBF0034 */  lw $ra, 52($sp)
    /* 800F8D40 0C03E1E8 */  jal 0x800F87A0
    /* 800F8D44 00000000 */  nop
    /* 800F8D48 1000000A */  beq $zero, $zero, 0x800F8D74
    /* 800F8D4C 8FBF0034 */  lw $ra, 52($sp)
    /* 800F8D50 8DEF694C */  lw $t7, 26956($t7)
    /* 800F8D54 31F80007 */  andi $t8, $t7, 0x0007
    /* 800F8D58 53000006 */  .word 0x53000006
    /* 800F8D5C 8FBF0034 */  lw $ra, 52($sp)
    /* 800F8D60 0C02D7F1 */  jal 0x800B5FC4
    /* 800F8D64 24040004 */  addiu $a0, $zero, 4
    /* 800F8D68 0C03E15F */  jal 0x800F857C
    /* 800F8D6C 24100001 */  addiu $s0, $zero, 1
    /* 800F8D70 8FBF0034 */  lw $ra, 52($sp)
    /* 800F8D74 8FB00014 */  lw $s0, 20($sp)
    /* 800F8D78 8FB10018 */  lw $s1, 24($sp)
    /* 800F8D7C 8FB2001C */  lw $s2, 28($sp)
    /* 800F8D80 8FB30020 */  lw $s3, 32($sp)
    /* 800F8D84 8FB40024 */  lw $s4, 36($sp)
    /* 800F8D88 8FB50028 */  lw $s5, 40($sp)
    /* 800F8D8C 8FB6002C */  lw $s6, 44($sp)
    /* 800F8D90 8FB70030 */  lw $s7, 48($sp)
    /* 800F8D94 03E00008 */  jr $ra
    /* 800F8D98 27BD0038 */  addiu $sp, $sp, 56
