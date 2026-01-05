# Source: game_code.bin (decompressed)
# Address: 0x800E7100

glabel object_alloc_slot
    /* 800E7100 90780000 */  lbu $t8, 0($v1)
    /* 800E7104 24020080 */  addiu $v0, $zero, 128
    /* 800E7108 14580004 */  bne $v0, $t8, 0x800E711C
    /* 800E710C 00000000 */  nop
    /* 800E7110 90790000 */  lbu $t9, 0($v1)
    /* 800E7114 5059FFFF */  .word 0x5059FFFF
    /* 800E7118 90790000 */  lbu $t9, 0($v1)
    /* 800E711C 0C032577 */  jal 0x800C95DC
    /* 800E7120 00000000 */  nop
    /* 800E7124 8FBF0014 */  lw $ra, 20($sp)
    /* 800E7128 27BD0018 */  addiu $sp, $sp, 24
    /* 800E712C 03E00008 */  jr $ra
    /* 800E7130 00000000 */  nop
