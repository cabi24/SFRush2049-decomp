# Source: game_code.bin (decompressed)
# Address: 0x800A1180

glabel prev_maxpath
    /* 800A1180 3C0FFEDC */  lui $t7, 0xFEDC
    /* 800A1184 35EFBA98 */  ori $t7, $t7, 0xBA98
    /* 800A1188 01CAC023 */  subu $t8, $t6, $t2
    /* 800A118C 2719FFE0 */  addiu $t9, $t8, -32
    /* 800A1190 AC79000C */  sw $t9, 12($v1)
    /* 800A1194 AC600010 */  sw $zero, 16($v1)
    /* 800A1198 A0600014 */  sb $zero, 20($v1)
    /* 800A119C A0600015 */  sb $zero, 21($v1)
    /* 800A11A0 A0600016 */  sb $zero, 22($v1)
    /* 800A11A4 AC6F0000 */  sw $t7, 0($v1)
    /* 800A11A8 ACE30004 */  sw $v1, 4($a3)
    /* 800A11AC ACEA000C */  sw $t2, 12($a3)
    /* 800A11B0 240E0001 */  addiu $t6, $zero, 1
    /* 800A11B4 ACE00010 */  sw $zero, 16($a3)
    /* 800A11B8 A0EE0014 */  sb $t6, 20($a3)
    /* 800A11BC A0E00015 */  sb $zero, 21($a3)
    /* 800A11C0 3C048015 */  lui $a0, 0x8015
    /* 800A11C4 24842770 */  addiu $a0, $a0, 10096
    /* 800A11C8 0C001D78 */  jal 0x800075E0
    /* 800A11CC AFA9001C */  sw $t1, 28($sp)
    /* 800A11D0 8FBF0014 */  lw $ra, 20($sp)
    /* 800A11D4 8FA2001C */  lw $v0, 28($sp)
    /* 800A11D8 27BD0048 */  addiu $sp, $sp, 72
    /* 800A11DC 03E00008 */  jr $ra
    /* 800A11E0 00000000 */  nop
