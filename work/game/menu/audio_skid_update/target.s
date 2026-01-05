# Source: game_code.bin (decompressed)
# Address: 0x800E6200

glabel audio_skid_update
    /* 800E6200 31AD0078 */  andi $t5, $t5, 0x0078
    /* 800E6204 15A00005 */  bne $t5, $zero, 0x800E621C
    /* 800E6208 00000000 */  nop
    /* 800E620C 440D8000 */  .word 0x440D8000
    /* 800E6210 3C018000 */  lui $at, 0x8000
    /* 800E6214 10000007 */  beq $zero, $zero, 0x800E6234
    /* 800E6218 01A16825 */  or $t5, $t5, $at
    /* 800E621C 10000005 */  beq $zero, $zero, 0x800E6234
    /* 800E6220 240DFFFF */  addiu $t5, $zero, -1
    /* 800E6224 440D8000 */  .word 0x440D8000
    /* 800E6228 00000000 */  nop
    /* 800E622C 05A0FFFB */  bltz $t5, 0x800E621C
    /* 800E6230 00000000 */  nop
    /* 800E6234 8C6E004C */  lw $t6, 76($v1)
    /* 800E6238 8C680050 */  lw $t0, 80($v1)
    /* 800E623C 01A07825 */  or $t7, $t5, $zero
    /* 800E6240 000FC900 */  sll $t9, $t7, 4
    /* 800E6244 01C85021 */  addu $t2, $t6, $t0
    /* 800E6248 01454821 */  addu $t1, $t2, $a1
    /* 800E624C 44CCF800 */  .word 0x44CCF800
    /* 800E6250 01256021 */  addu $t4, $t1, $a1
    /* 800E6254 0179C025 */  or $t8, $t3, $t9
    /* 800E6258 A1980000 */  sb $t8, 0($t4)
    /* 800E625C 8C6D0050 */  lw $t5, 80($v1)
    /* 800E6260 00001025 */  or $v0, $zero, $zero
    /* 800E6264 25AF0001 */  addiu $t7, $t5, 1
    /* 800E6268 AC6F0050 */  sw $t7, 80($v1)
    /* 800E626C 03E00008 */  jr $ra
    /* 800E6270 00000000 */  nop
