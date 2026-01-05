# Source: game_code.bin (decompressed)
# Address: 0x800B9130

glabel physics_step
    /* 800B9130 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B9134 AC209818 */  sw $zero, -26600($at)
    /* 800B9138 AFBF0014 */  sw $ra, 20($sp)
    /* 800B913C 3C018015 */  lui $at, 0x8015
    /* 800B9140 AFA40018 */  sw $a0, 24($sp)
    /* 800B9144 0C02E43E */  jal 0x800B90F8
    /* 800B9148 AC209B80 */  sw $zero, -25728($at)
    /* 800B914C 3C048014 */  lui $a0, 0x8014
    /* 800B9150 0C02584C */  jal 0x80096130
    /* 800B9154 8C843A18 */  lw $a0, 14872($a0)
    /* 800B9158 3C048014 */  lui $a0, 0x8014
    /* 800B915C 0C02584C */  jal 0x80096130
    /* 800B9160 8C843A20 */  lw $a0, 14880($a0)
    /* 800B9164 3C048014 */  lui $a0, 0x8014
    /* 800B9168 0C02584C */  jal 0x80096130
    /* 800B916C 8C843A28 */  lw $a0, 14888($a0)
    /* 800B9170 3C048014 */  lui $a0, 0x8014
    /* 800B9174 24843A10 */  addiu $a0, $a0, 14864
    /* 800B9178 00002825 */  or $a1, $zero, $zero
    /* 800B917C 0C0009E4 */  jal 0x80002790
    /* 800B9180 2406002C */  addiu $a2, $zero, 44
    /* 800B9184 8FBF0014 */  lw $ra, 20($sp)
    /* 800B9188 27BD0018 */  addiu $sp, $sp, 24
    /* 800B918C 03E00008 */  jr $ra
    /* 800B9190 00000000 */  nop
