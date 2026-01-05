# Source: game_code.bin (decompressed)
# Address: 0x800B42F0

glabel object_create
    /* 800B42F0 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800B42F4 AFA40030 */  sw $a0, 48($sp)
    /* 800B42F8 AFBF0024 */  sw $ra, 36($sp)
    /* 800B42FC 3C048014 */  lui $a0, 0x8014
    /* 800B4300 AFB30020 */  sw $s3, 32($sp)
    /* 800B4304 AFB2001C */  sw $s2, 28($sp)
    /* 800B4308 AFB10018 */  sw $s1, 24($sp)
    /* 800B430C AFB00014 */  sw $s0, 20($sp)
    /* 800B4310 248461D0 */  addiu $a0, $a0, 25040
    /* 800B4314 00002825 */  or $a1, $zero, $zero
    /* 800B4318 0C001C9C */  jal 0x80007270
    /* 800B431C 24060001 */  addiu $a2, $zero, 1
    /* 800B4320 0C02D080 */  jal 0x800B4200
    /* 800B4324 8FB20030 */  lw $s2, 48($sp)
    /* 800B4328 3C048014 */  lui $a0, 0x8014
    /* 800B432C AFA2002C */  sw $v0, 44($sp)
    /* 800B4330 248461D0 */  addiu $a0, $a0, 25040
    /* 800B4334 00002825 */  or $a1, $zero, $zero
    /* 800B4338 0C001D78 */  jal 0x800075E0
    /* 800B433C 00003025 */  or $a2, $zero, $zero
    /* 800B4340 8FBF0024 */  lw $ra, 36($sp)
    /* 800B4344 8FA2002C */  lw $v0, 44($sp)
    /* 800B4348 8FB00014 */  lw $s0, 20($sp)
    /* 800B434C 8FB10018 */  lw $s1, 24($sp)
    /* 800B4350 8FB2001C */  lw $s2, 28($sp)
    /* 800B4354 8FB30020 */  lw $s3, 32($sp)
    /* 800B4358 03E00008 */  jr $ra
    /* 800B435C 27BD0030 */  addiu $sp, $sp, 48
