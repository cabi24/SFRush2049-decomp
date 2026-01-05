# Source: game_code.bin (decompressed)
# Address: 0x800FECA4

glabel game_timer_display
    /* 800FECA4 27BDFF88 */  addiu $sp, $sp, -120
    /* 800FECA8 3C0E8011 */  lui $t6, 0x8011
    /* 800FECAC 81CE0634 */  lb $t6, 1588($t6)
    /* 800FECB0 AFBF003C */  sw $ra, 60($sp)
    /* 800FECB4 AFB40038 */  sw $s4, 56($sp)
    /* 800FECB8 AFB30034 */  sw $s3, 52($sp)
    /* 800FECBC AFB20030 */  sw $s2, 48($sp)
    /* 800FECC0 AFB1002C */  sw $s1, 44($sp)
    /* 800FECC4 AFB00028 */  sw $s0, 40($sp)
    /* 800FECC8 15C00045 */  bne $t6, $zero, 0x800FEDE0
    /* 800FECCC AFA40078 */  sw $a0, 120($sp)
    /* 800FECD0 3C0F8011 */  lui $t7, 0x8011
    /* 800FECD4 8DEF74B4 */  lw $t7, 29876($t7)
    /* 800FECD8 3C017C03 */  lui $at, 0x7C03
    /* 800FECDC 3421FFFE */  ori $at, $at, 0xFFFE
    /* 800FECE0 01E1C024 */  and $t8, $t7, $at
    /* 800FECE4 1700003B */  bne $t8, $zero, 0x800FEDD4
    /* 800FECE8 3C048014 */  lui $a0, 0x8014
    /* 800FECEC 248461D0 */  addiu $a0, $a0, 25040
    /* 800FECF0 00002825 */  or $a1, $zero, $zero
    /* 800FECF4 0C001C9C */  jal 0x80007270
    /* 800FECF8 24060001 */  addiu $a2, $zero, 1
    /* 800FECFC 0C02D080 */  jal 0x800B4200
    /* 800FED00 2412000D */  addiu $s2, $zero, 13
    /* 800FED04 3C048014 */  lui $a0, 0x8014
    /* 800FED08 248461D0 */  addiu $a0, $a0, 25040
    /* 800FED0C AFA2005C */  sw $v0, 92($sp)
    /* 800FED10 00002825 */  or $a1, $zero, $zero
    /* 800FED14 0C001D78 */  jal 0x800075E0
    /* 800FED18 00003025 */  or $a2, $zero, $zero
    /* 800FED1C 3C198015 */  lui $t9, 0x8015
    /* 800FED20 8F39698C */  lw $t9, 27020($t9)
    /* 800FED24 3C028015 */  lui $v0, 0x8015
    /* 800FED28 3C018011 */  lui $at, 0x8011
    /* 800FED2C 00197100 */  sll $t6, $t9, 4
    /* 800FED30 004E1021 */  addu $v0, $v0, $t6
    /* 800FED34 80426CF0 */  lb $v0, 27888($v0)
    /* 800FED38 2C4F0001 */  sltiu $t7, $v0, 1
    /* 800FED3C 0C0359F0 */  jal 0x800D67C0
    /* 800FED40 A02F064C */  sb $t7, 1612($at)
    /* 800FED44 0C02CFD4 */  jal 0x800B3F50
    /* 800FED48 00000000 */  nop
    /* 800FED4C 0002C040 */  sll $t8, $v0, 1
    /* 800FED50 2719000A */  addiu $t9, $t8, 10
    /* 800FED54 3C048014 */  lui $a0, 0x8014
    /* 800FED58 A7B90074 */  sh $t9, 116($sp)
    /* 800FED5C 248461D0 */  addiu $a0, $a0, 25040
    /* 800FED60 00002825 */  or $a1, $zero, $zero
    /* 800FED64 0C001C9C */  jal 0x80007270
    /* 800FED68 24060001 */  addiu $a2, $zero, 1
    /* 800FED6C 0C02D080 */  jal 0x800B4200
    /* 800FED70 2412000A */  addiu $s2, $zero, 10
    /* 800FED74 3C048014 */  lui $a0, 0x8014
    /* 800FED78 248461D0 */  addiu $a0, $a0, 25040
    /* 800FED7C AFA20044 */  sw $v0, 68($sp)
    /* 800FED80 00002825 */  or $a1, $zero, $zero
    /* 800FED84 0C001D78 */  jal 0x800075E0
    /* 800FED88 00003025 */  or $a2, $zero, $zero
    /* 800FED8C 3C038011 */  lui $v1, 0x8011
    /* 800FED90 24630650 */  addiu $v1, $v1, 1616
    /* 800FED94 87A80074 */  lh $t0, 116($sp)
    /* 800FED98 8C620000 */  lw $v0, 0($v1)
    /* 800FED9C 8C6E0004 */  lw $t6, 4($v1)
    /* 800FEDA0 8C6F0008 */  lw $t7, 8($v1)
    /* 800FEDA4 241800B0 */  addiu $t8, $zero, 176
    /* 800FEDA8 004E3021 */  addu $a2, $v0, $t6
    /* 800FEDAC 010F3821 */  addu $a3, $t0, $t7
    /* 800FEDB0 24E70004 */  addiu $a3, $a3, 4
    /* 800FEDB4 24C60008 */  addiu $a2, $a2, 8
    /* 800FEDB8 AFB80010 */  sw $t8, 16($sp)
    /* 800FEDBC AFA00014 */  sw $zero, 20($sp)
    /* 800FEDC0 AFA00018 */  sw $zero, 24($sp)
    /* 800FEDC4 AFA0001C */  sw $zero, 28($sp)
    /* 800FEDC8 2505FFFC */  addiu $a1, $t0, -4
    /* 800FEDCC 0C02CED3 */  jal 0x800B3B4C
    /* 800FEDD0 2444FFF8 */  addiu $a0, $v0, -8
    /* 800FEDD4 24190001 */  addiu $t9, $zero, 1
    /* 800FEDD8 3C018011 */  lui $at, 0x8011
    /* 800FEDDC A0390634 */  sb $t9, 1588($at)
    /* 800FEDE0 8FBF003C */  lw $ra, 60($sp)
    /* 800FEDE4 8FB00028 */  lw $s0, 40($sp)
    /* 800FEDE8 8FB1002C */  lw $s1, 44($sp)
    /* 800FEDEC 8FB20030 */  lw $s2, 48($sp)
    /* 800FEDF0 8FB30034 */  lw $s3, 52($sp)
    /* 800FEDF4 8FB40038 */  lw $s4, 56($sp)
    /* 800FEDF8 27BD0078 */  addiu $sp, $sp, 120
    /* 800FEDFC 03E00008 */  jr $ra
    /* 800FEE00 24020001 */  addiu $v0, $zero, 1
