# Source: game_code.bin (decompressed)
# Address: 0x800FBC30

glabel game_mode_post
    /* 800FBC30 3C0E8015 */  lui $t6, 0x8015
    /* 800FBC34 8DCEA110 */  lw $t6, -24304($t6)
    /* 800FBC38 27BDFEA8 */  addiu $sp, $sp, -344
    /* 800FBC3C 24010002 */  addiu $at, $zero, 2
    /* 800FBC40 15C10003 */  bne $t6, $at, 0x800FBC50
    /* 800FBC44 AFBF002C */  sw $ra, 44($sp)
    /* 800FBC48 10000003 */  beq $zero, $zero, 0x800FBC58
    /* 800FBC4C 24100001 */  addiu $s0, $zero, 1
    /* 800FBC50 3C108015 */  lui $s0, 0x8015
    /* 800FBC54 8610A108 */  lh $s0, -24312($s0)
    /* 800FBC58 1A00000B */  blez $s0, 0x800FBC88
    /* 800FBC5C 00001825 */  or $v1, $zero, $zero
    /* 800FBC60 3C028015 */  lui $v0, 0x8015
    /* 800FBC64 24426CF0 */  addiu $v0, $v0, 27888
    /* 800FBC68 804F0000 */  lb $t7, 0($v0)
    /* 800FBC6C 55E00004 */  .word 0x55E00004
    /* 800FBC70 24630001 */  addiu $v1, $v1, 1
    /* 800FBC74 10000005 */  beq $zero, $zero, 0x800FBC8C
    /* 800FBC78 0070082A */  slt $at, $v1, $s0
    /* 800FBC7C 24630001 */  addiu $v1, $v1, 1
    /* 800FBC80 1470FFF9 */  bne $v1, $s0, 0x800FBC68
    /* 800FBC84 24420010 */  addiu $v0, $v0, 16
    /* 800FBC88 0070082A */  slt $at, $v1, $s0
    /* 800FBC8C 14200007 */  bne $at, $zero, 0x800FBCAC
    /* 800FBC90 3C058012 */  lui $a1, 0x8012
    /* 800FBC94 3C018016 */  lui $at, 0x8016
    /* 800FBC98 AC2313B0 */  sw $v1, 5040($at)
    /* 800FBC9C 24100140 */  addiu $s0, $zero, 320
    /* 800FBCA0 3C018011 */  lui $at, 0x8011
    /* 800FBCA4 1000005A */  beq $zero, $zero, 0x800FBE10
    /* 800FBCA8 AC3046F8 */  sw $s0, 18168($at)
    /* 800FBCAC 27B10050 */  addiu $s1, $sp, 80
    /* 800FBCB0 3C018016 */  lui $at, 0x8016
    /* 800FBCB4 AC2313B0 */  sw $v1, 5040($at)
    /* 800FBCB8 02202025 */  or $a0, $s1, $zero
    /* 800FBCBC 24A50308 */  addiu $a1, $a1, 776
    /* 800FBCC0 0C001264 */  jal 0x80004990
    /* 800FBCC4 24660001 */  addiu $a2, $v1, 1
    /* 800FBCC8 3C128018 */  lui $s2, 0x8018
    /* 800FBCCC 2652A4E0 */  addiu $s2, $s2, -23328
    /* 800FBCD0 8E430004 */  lw $v1, 4($s2)
    /* 800FBCD4 27B40058 */  addiu $s4, $sp, 88
    /* 800FBCD8 3C058012 */  lui $a1, 0x8012
    /* 800FBCDC 8C7801F4 */  lw $t8, 500($v1)
    /* 800FBCE0 8C6601F0 */  lw $a2, 496($v1)
    /* 800FBCE4 AFB10014 */  sw $s1, 20($sp)
    /* 800FBCE8 AFB80010 */  sw $t8, 16($sp)
    /* 800FBCEC 8C7901F8 */  lw $t9, 504($v1)
    /* 800FBCF0 AFB0001C */  sw $s0, 28($sp)
    /* 800FBCF4 24A5030C */  addiu $a1, $a1, 780
    /* 800FBCF8 AFB90018 */  sw $t9, 24($sp)
    /* 800FBCFC 8C6E01FC */  lw $t6, 508($v1)
    /* 800FBD00 02802025 */  or $a0, $s4, $zero
    /* 800FBD04 02203825 */  or $a3, $s1, $zero
    /* 800FBD08 0C001264 */  jal 0x80004990
    /* 800FBD0C AFAE0020 */  sw $t6, 32($sp)
    /* 800FBD10 8E4F0004 */  lw $t7, 4($s2)
    /* 800FBD14 02802025 */  or $a0, $s4, $zero
    /* 800FBD18 0C0291DC */  jal 0x800A4770
    /* 800FBD1C 8DE50200 */  lw $a1, 512($t7)
    /* 800FBD20 3C158014 */  lui $s5, 0x8014
    /* 800FBD24 26B561D0 */  addiu $s5, $s5, 25040
    /* 800FBD28 02A02025 */  or $a0, $s5, $zero
    /* 800FBD2C 00002825 */  or $a1, $zero, $zero
    /* 800FBD30 0C001C9C */  jal 0x80007270
    /* 800FBD34 24060001 */  addiu $a2, $zero, 1
    /* 800FBD38 0C02D080 */  jal 0x800B4200
    /* 800FBD3C 2412000B */  addiu $s2, $zero, 11
    /* 800FBD40 00408025 */  or $s0, $v0, $zero
    /* 800FBD44 02A02025 */  or $a0, $s5, $zero
    /* 800FBD48 00002825 */  or $a1, $zero, $zero
    /* 800FBD4C 0C001D78 */  jal 0x800075E0
    /* 800FBD50 00003025 */  or $a2, $zero, $zero
    /* 800FBD54 0C02DD28 */  jal 0x800B74A0
    /* 800FBD58 24040016 */  addiu $a0, $zero, 22
    /* 800FBD5C 3C028011 */  lui $v0, 0x8011
    /* 800FBD60 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FBD64 30580001 */  andi $t8, $v0, 0x0001
    /* 800FBD68 1700001A */  bne $t8, $zero, 0x800FBDD4
    /* 800FBD6C 30590002 */  andi $t9, $v0, 0x0002
    /* 800FBD70 17200018 */  bne $t9, $zero, 0x800FBDD4
    /* 800FBD74 00027040 */  sll $t6, $v0, 1
    /* 800FBD78 05C00005 */  bltz $t6, 0x800FBD90
    /* 800FBD7C 304F0004 */  andi $t7, $v0, 0x0004
    /* 800FBD80 15E00003 */  bne $t7, $zero, 0x800FBD90
    /* 800FBD84 30580008 */  andi $t8, $v0, 0x0008
    /* 800FBD88 1300000B */  beq $t8, $zero, 0x800FBDB8
    /* 800FBD8C 3C108011 */  lui $s0, 0x8011
    /* 800FBD90 3C108011 */  lui $s0, 0x8011
    /* 800FBD94 8E1046F8 */  lw $s0, 18168($s0)
    /* 800FBD98 24050008 */  addiu $a1, $zero, 8
    /* 800FBD9C 02803025 */  or $a2, $s4, $zero
    /* 800FBDA0 00102400 */  sll $a0, $s0, 16
    /* 800FBDA4 0004CC03 */  sra $t9, $a0, 16
    /* 800FBDA8 0C02DC75 */  jal 0x800B71D4
    /* 800FBDAC 03202025 */  or $a0, $t9, $zero
    /* 800FBDB0 10000008 */  beq $zero, $zero, 0x800FBDD4
    /* 800FBDB4 00000000 */  nop
    /* 800FBDB8 8E1046F8 */  lw $s0, 18168($s0)
    /* 800FBDBC 240500CD */  addiu $a1, $zero, 205
    /* 800FBDC0 02803025 */  or $a2, $s4, $zero
    /* 800FBDC4 00102400 */  sll $a0, $s0, 16
    /* 800FBDC8 00047403 */  sra $t6, $a0, 16
    /* 800FBDCC 0C02DC75 */  jal 0x800B71D4
    /* 800FBDD0 01C02025 */  or $a0, $t6, $zero
    /* 800FBDD4 3C108011 */  lui $s0, 0x8011
    /* 800FBDD8 8E1046F8 */  lw $s0, 18168($s0)
    /* 800FBDDC 02802025 */  or $a0, $s4, $zero
    /* 800FBDE0 2405FFFF */  addiu $a1, $zero, -1
    /* 800FBDE4 0C02CFE9 */  jal 0x800B3FA4
    /* 800FBDE8 2610FFFF */  addiu $s0, $s0, -1
    /* 800FBDEC 3C018011 */  lui $at, 0x8011
    /* 800FBDF0 AC3046F8 */  sw $s0, 18168($at)
    /* 800FBDF4 00027823 */  subu $t7, $zero, $v0
    /* 800FBDF8 020F082A */  slt $at, $s0, $t7
    /* 800FBDFC 50200005 */  .word 0x50200005
    /* 800FBE00 8FBF002C */  lw $ra, 44($sp)
    /* 800FBE04 24100140 */  addiu $s0, $zero, 320
    /* 800FBE08 3C018011 */  lui $at, 0x8011
    /* 800FBE0C AC3046F8 */  sw $s0, 18168($at)
    /* 800FBE10 8FBF002C */  lw $ra, 44($sp)
    /* 800FBE14 27BD0158 */  addiu $sp, $sp, 344
    /* 800FBE18 03E00008 */  jr $ra
    /* 800FBE1C 00000000 */  nop
