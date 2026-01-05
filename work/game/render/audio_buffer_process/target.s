# Source: game_code.bin (decompressed)
# Address: 0x8008C724

glabel audio_buffer_process
    /* 8008C724 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008C728 AFBF0014 */  sw $ra, 20($sp)
    /* 8008C72C 460C203C */  .word 0x460C203C
    /* 8008C730 46006386 */  .word 0x46006386
    /* 8008C734 45000005 */  .word 0x45000005
    /* 8008C738 00000000 */  nop
    /* 8008C73C 0C0231A0 */  jal 0x8008C680
    /* 8008C740 00000000 */  nop
    /* 8008C744 10000005 */  beq $zero, $zero, 0x8008C75C
    /* 8008C748 8FBF0014 */  lw $ra, 20($sp)
    /* 8008C74C 0C0231A0 */  jal 0x8008C680
    /* 8008C750 46007307 */  .word 0x46007307
    /* 8008C754 46000007 */  .word 0x46000007
    /* 8008C758 8FBF0014 */  lw $ra, 20($sp)
    /* 8008C75C 27BD0018 */  addiu $sp, $sp, 24
    /* 8008C760 03E00008 */  jr $ra
    /* 8008C764 00000000 */  nop
