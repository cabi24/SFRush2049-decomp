# Source: game_code.bin (decompressed)
# Address: 0x8008ABF4

glabel message_send_async
    /* 8008ABF4 27BDFFD8 */  addiu $sp, $sp, -40
    /* 8008ABF8 AFBF0024 */  sw $ra, 36($sp)
    /* 8008ABFC 010E082A */  slt $at, $t0, $t6
    /* 8008AC00 10200018 */  beq $at, $zero, 0x8008AC64
    /* 8008AC04 00001025 */  or $v0, $zero, $zero
    /* 8008AC08 250F0001 */  addiu $t7, $t0, 1
    /* 8008AC0C 31F9FFFF */  andi $t9, $t7, 0xFFFF
    /* 8008AC10 8C780008 */  lw $t8, 8($v1)
    /* 8008AC14 00197080 */  sll $t6, $t9, 2
    /* 8008AC18 01D97023 */  subu $t6, $t6, $t9
    /* 8008AC1C 000E7080 */  sll $t6, $t6, 2
    /* 8008AC20 A46F0004 */  sh $t7, 4($v1)
    /* 8008AC24 030E1021 */  addu $v0, $t8, $t6
    /* 8008AC28 8C4FFFFC */  lw $t7, -4($v0)
    /* 8008AC2C 8C47FFF8 */  lw $a3, -8($v0)
    /* 8008AC30 3C188015 */  lui $t8, 0x8015
    /* 8008AC34 AFAF0010 */  sw $t7, 16($sp)
    /* 8008AC38 8C59FFF4 */  lw $t9, -12($v0)
    /* 8008AC3C 27183E68 */  addiu $t8, $t8, 15976
    /* 8008AC40 3C048016 */  lui $a0, 0x8016
    /* 8008AC44 24841438 */  addiu $a0, $a0, 5176
    /* 8008AC48 AFB80018 */  sw $t8, 24($sp)
    /* 8008AC4C 24050001 */  addiu $a1, $zero, 1
    /* 8008AC50 00003025 */  or $a2, $zero, $zero
    /* 8008AC54 0C00218C */  jal 0x80008630
    /* 8008AC58 AFB90014 */  sw $t9, 20($sp)
    /* 8008AC5C 10000001 */  beq $zero, $zero, 0x8008AC64
    /* 8008AC60 24020001 */  addiu $v0, $zero, 1
    /* 8008AC64 8FBF0024 */  lw $ra, 36($sp)
    /* 8008AC68 27BD0028 */  addiu $sp, $sp, 40
    /* 8008AC6C 03E00008 */  jr $ra
    /* 8008AC70 00000000 */  nop
