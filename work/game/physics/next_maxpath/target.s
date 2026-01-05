# Source: game_code.bin (decompressed)
# Address: 0x800A1080

glabel next_maxpath
    /* 800A1080 0107082B */  sltu $at, $t0, $a3
    /* 800A1084 24E90020 */  addiu $t1, $a3, 32
    /* 800A1088 1109002B */  beq $t0, $t1, 0x800A1138
    /* 800A108C 00002825 */  or $a1, $zero, $zero
    /* 800A1090 8CF80004 */  lw $t8, 4($a3)
    /* 800A1094 2503FFE0 */  addiu $v1, $t0, -32
    /* 800A1098 24190001 */  addiu $t9, $zero, 1
    /* 800A109C 13000003 */  beq $t8, $zero, 0x800A10AC
    /* 800A10A0 AC780004 */  sw $t8, 4($v1)
    /* 800A10A4 10000002 */  beq $zero, $zero, 0x800A10B0
    /* 800A10A8 AF030008 */  sw $v1, 8($t8)
    /* 800A10AC AC43000C */  sw $v1, 12($v0)
    /* 800A10B0 3C0FFEDC */  lui $t7, 0xFEDC
    /* 800A10B4 35EFBA98 */  ori $t7, $t7, 0xBA98
    /* 800A10B8 AC600010 */  sw $zero, 16($v1)
    /* 800A10BC A0790014 */  sb $t9, 20($v1)
    /* 800A10C0 A0600015 */  sb $zero, 21($v1)
    /* 800A10C4 A0600016 */  sb $zero, 22($v1)
    /* 800A10C8 AC6F0000 */  sw $t7, 0($v1)
    /* 800A10CC 8CEE000C */  lw $t6, 12($a3)
    /* 800A10D0 01072023 */  subu $a0, $t0, $a3
    /* 800A10D4 2484FFE0 */  addiu $a0, $a0, -32
    /* 800A10D8 2C810040 */  sltiu $at, $a0, 64
    /* 800A10DC 01C4C023 */  subu $t8, $t6, $a0
    /* 800A10E0 14200009 */  bne $at, $zero, 0x800A1108
    /* 800A10E4 AC78000C */  sw $t8, 12($v1)
    /* 800A10E8 AC670008 */  sw $a3, 8($v1)
    /* 800A10EC ACE30004 */  sw $v1, 4($a3)
    /* 800A10F0 8C6F000C */  lw $t7, 12($v1)
    /* 800A10F4 8CF9000C */  lw $t9, 12($a3)
    /* 800A10F8 032F7023 */  subu $t6, $t9, $t7
    /* 800A10FC 25D8FFE0 */  addiu $t8, $t6, -32
    /* 800A1100 1000000B */  beq $zero, $zero, 0x800A1130
    /* 800A1104 ACF8000C */  sw $t8, 12($a3)
    /* 800A1108 8CE40008 */  lw $a0, 8($a3)
    /* 800A110C AC640008 */  sw $a0, 8($v1)
    /* 800A1110 8CF90008 */  lw $t9, 8($a3)
    /* 800A1114 AF230004 */  sw $v1, 4($t9)
    /* 800A1118 8CE40008 */  lw $a0, 8($a3)
    /* 800A111C 8C8F000C */  lw $t7, 12($a0)
    /* 800A1120 01E87021 */  addu $t6, $t7, $t0
    /* 800A1124 01C7C023 */  subu $t8, $t6, $a3
    /* 800A1128 2719FFE0 */  addiu $t9, $t8, -32
    /* 800A112C AC99000C */  sw $t9, 12($a0)
    /* 800A1130 00603825 */  or $a3, $v1, $zero
    /* 800A1134 24690020 */  addiu $t1, $v1, 32
    /* 800A1138 8CEF000C */  lw $t7, 12($a3)
    /* 800A113C 00003025 */  or $a2, $zero, $zero
    /* 800A1140 01EA7023 */  subu $t6, $t7, $t2
    /* 800A1144 2DC10040 */  sltiu $at, $t6, 64
    /* 800A1148 5420001A */  .word 0x5420001A
    /* 800A114C 240E0001 */  addiu $t6, $zero, 1
    /* 800A1150 8CF80004 */  lw $t8, 4($a3)
    /* 800A1154 00EA2021 */  addu $a0, $a3, $t2
    /* 800A1158 24830020 */  addiu $v1, $a0, 32
    /* 800A115C 13000005 */  beq $t8, $zero, 0x800A1174
    /* 800A1160 AC980024 */  sw $t8, 36($a0)
    /* 800A1164 24830020 */  addiu $v1, $a0, 32
    /* 800A1168 8C6F0004 */  lw $t7, 4($v1)
    /* 800A116C 10000002 */  beq $zero, $zero, 0x800A1178
    /* 800A1170 ADE30008 */  sw $v1, 8($t7)
    /* 800A1174 AC43000C */  sw $v1, 12($v0)
    /* 800A1178 AC670008 */  sw $a3, 8($v1)
    /* 800A117C 8CEE000C */  lw $t6, 12($a3)
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
