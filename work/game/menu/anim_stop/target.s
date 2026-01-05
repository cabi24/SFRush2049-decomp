# Source: game_code.bin (decompressed)
# Address: 0x800EA200

glabel anim_stop
    /* 800EA200 4602D682 */  .word 0x4602D682
    /* 800EA204 0085082B */  sltu $at, $a0, $a1
    /* 800EA208 24420004 */  addiu $v0, $v0, 4
    /* 800EA20C 461C0702 */  .word 0x461C0702
    /* 800EA210 24630004 */  addiu $v1, $v1, 4
    /* 800EA214 461AE680 */  .word 0x461AE680
    /* 800EA218 1420FFF4 */  bne $at, $zero, 0x800EA1EC
    /* 800EA21C E45A0080 */  swc1 $f26, 128($v0)
    /* 800EA220 00C01025 */  or $v0, $a2, $zero
    /* 800EA224 01001825 */  or $v1, $t0, $zero
    /* 800EA228 27A40094 */  addiu $a0, $sp, 148
    /* 800EA22C 27A500A0 */  addiu $a1, $sp, 160
    /* 800EA230 24840004 */  addiu $a0, $a0, 4
    /* 800EA234 C45E0084 */  lwc1 $f30, 132($v0)
    /* 800EA238 10850009 */  beq $a0, $a1, 0x800EA260
    /* 800EA23C C47C0000 */  lwc1 $f28, 0($v1)
    /* 800EA240 461EE681 */  .word 0x461EE681
    /* 800EA244 C45E0088 */  lwc1 $f30, 136($v0)
    /* 800EA248 24840004 */  addiu $a0, $a0, 4
    /* 800EA24C 24420004 */  addiu $v0, $v0, 4
    /* 800EA250 E49AFFF8 */  swc1 $f26, -8($a0)
    /* 800EA254 C47C0004 */  lwc1 $f28, 4($v1)
    /* 800EA258 1485FFF9 */  bne $a0, $a1, 0x800EA240
    /* 800EA25C 24630004 */  addiu $v1, $v1, 4
    /* 800EA260 461EE681 */  .word 0x461EE681
    /* 800EA264 24420004 */  addiu $v0, $v0, 4
    /* 800EA268 24630004 */  addiu $v1, $v1, 4
    /* 800EA26C E49AFFFC */  swc1 $f26, -4($a0)
    /* 800EA270 0009C080 */  sll $t8, $t1, 2
    /* 800EA274 0309C021 */  addu $t8, $t8, $t1
    /* 800EA278 0018C080 */  sll $t8, $t8, 2
    /* 800EA27C 0309C023 */  subu $t8, $t8, $t1
    /* 800EA280 0018C0C0 */  sll $t8, $t8, 3
    /* 800EA284 3C0E8015 */  lui $t6, 0x8015
    /* 800EA288 25CE0B70 */  addiu $t6, $t6, 2928
    /* 800EA28C 27190060 */  addiu $t9, $t8, 96
    /* 800EA290 032E2821 */  addu $a1, $t9, $t6
    /* 800EA294 0C022D31 */  jal 0x8008B4C4
    /* 800EA298 27A40094 */  addiu $a0, $sp, 148
    /* 800EA29C 8FBF0064 */  lw $ra, 100($sp)
    /* 800EA2A0 D7B40018 */  .word 0xD7B40018
    /* 800EA2A4 D7B60020 */  .word 0xD7B60020
    /* 800EA2A8 D7B80028 */  .word 0xD7B80028
    /* 800EA2AC D7BA0030 */  .word 0xD7BA0030
    /* 800EA2B0 D7BC0038 */  .word 0xD7BC0038
    /* 800EA2B4 D7BE0040 */  .word 0xD7BE0040
    /* 800EA2B8 8FB00048 */  lw $s0, 72($sp)
    /* 800EA2BC 8FB1004C */  lw $s1, 76($sp)
    /* 800EA2C0 8FB20050 */  lw $s2, 80($sp)
    /* 800EA2C4 8FB30054 */  lw $s3, 84($sp)
    /* 800EA2C8 8FB40058 */  lw $s4, 88($sp)
    /* 800EA2CC 8FB5005C */  lw $s5, 92($sp)
    /* 800EA2D0 8FB60060 */  lw $s6, 96($sp)
    /* 800EA2D4 03E00008 */  jr $ra
    /* 800EA2D8 27BD00A8 */  addiu $sp, $sp, 168
