# Source: game_code.bin (decompressed)
# Address: 0x800AB70C

glabel engine_rpm_calc
    /* 800AB70C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800AB710 00064C00 */  sll $t1, $a2, 16
    /* 800AB714 00097403 */  sra $t6, $t1, 16
    /* 800AB718 AFA60020 */  sw $a2, 32($sp)
    /* 800AB71C 8FAA0028 */  lw $t2, 40($sp)
    /* 800AB720 00073400 */  sll $a2, $a3, 16
    /* 800AB724 000EC200 */  sll $t8, $t6, 8
    /* 800AB728 AFBF0014 */  sw $ra, 20($sp)
    /* 800AB72C 3B190F00 */  xori $t9, $t8, 0x0F00
    /* 800AB730 00067C03 */  sra $t7, $a2, 16
    /* 800AB734 01E03025 */  or $a2, $t7, $zero
    /* 800AB738 0C02389B */  jal 0x8008E26C
    /* 800AB73C 032A3825 */  or $a3, $t9, $t2
    /* 800AB740 8FBF0014 */  lw $ra, 20($sp)
    /* 800AB744 27BD0018 */  addiu $sp, $sp, 24
    /* 800AB748 03E00008 */  jr $ra
    /* 800AB74C 00000000 */  nop
