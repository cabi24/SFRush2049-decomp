# Source: game_code.bin (decompressed)
# Address: 0x800B80C8

glabel particles_spawn_emitter
    /* 800B80C8 3C0F8012 */  lui $t7, 0x8012
    /* 800B80CC 25EF4FD0 */  addiu $t7, $t7, 20432
    /* 800B80D0 00047080 */  sll $t6, $a0, 2
    /* 800B80D4 01CF1021 */  addu $v0, $t6, $t7
    /* 800B80D8 8C430000 */  lw $v1, 0($v0)
    /* 800B80DC 0004C080 */  sll $t8, $a0, 2
    /* 800B80E0 0304C021 */  addu $t8, $t8, $a0
    /* 800B80E4 1060000B */  beq $v1, $zero, 0x800B8114
    /* 800B80E8 0018C080 */  sll $t8, $t8, 2
    /* 800B80EC 0304C023 */  subu $t8, $t8, $a0
    /* 800B80F0 3C198015 */  lui $t9, 0x8015
    /* 800B80F4 27390B70 */  addiu $t9, $t9, 2928
    /* 800B80F8 0018C0C0 */  sll $t8, $t8, 3
    /* 800B80FC 03192821 */  addu $a1, $t8, $t9
    /* 800B8100 C4A40024 */  lwc1 $f4, 36($a1)
    /* 800B8104 E4640024 */  swc1 $f4, 36($v1)
    /* 800B8108 8C480000 */  lw $t0, 0($v0)
    /* 800B810C C4A6002C */  lwc1 $f6, 44($a1)
    /* 800B8110 E506002C */  swc1 $f6, 44($t0)
    /* 800B8114 03E00008 */  jr $ra
    /* 800B8118 00000000 */  nop
