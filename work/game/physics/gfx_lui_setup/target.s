# Source: game_code.bin (decompressed)
# Address: 0x800A12D4

glabel gfx_lui_setup
    /* 800A12D4 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800A12D8 AFBF001C */  sw $ra, 28($sp)
    /* 800A12DC AFB10018 */  sw $s1, 24($sp)
    /* 800A12E0 11C00011 */  beq $t6, $zero, 0x800A1328
    /* 800A12E4 AFB00014 */  sw $s0, 20($sp)
    /* 800A12E8 3C048015 */  lui $a0, 0x8015
    /* 800A12EC 24842770 */  addiu $a0, $a0, 10096
    /* 800A12F0 00002825 */  or $a1, $zero, $zero
    /* 800A12F4 0C001C9C */  jal 0x80007270
    /* 800A12F8 24060001 */  addiu $a2, $zero, 1
    /* 800A12FC 3C058039 */  lui $a1, 0x8039
    /* 800A1300 24A5A400 */  addiu $a1, $a1, -23552
    /* 800A1304 0C0257F6 */  jal 0x80095FD8
    /* 800A1308 00003025 */  or $a2, $zero, $zero
    /* 800A130C 3C048015 */  lui $a0, 0x8015
    /* 800A1310 24842770 */  addiu $a0, $a0, 10096
    /* 800A1314 00002825 */  or $a1, $zero, $zero
    /* 800A1318 0C001D78 */  jal 0x800075E0
    /* 800A131C 00003025 */  or $a2, $zero, $zero
    /* 800A1320 3C018012 */  lui $at, 0x8012
    /* 800A1324 A020ED00 */  sb $zero, -4864($at)
    /* 800A1328 8FBF001C */  lw $ra, 28($sp)
    /* 800A132C 8FB00014 */  lw $s0, 20($sp)
    /* 800A1330 8FB10018 */  lw $s1, 24($sp)
    /* 800A1334 03E00008 */  jr $ra
    /* 800A1338 27BD0020 */  addiu $sp, $sp, 32
