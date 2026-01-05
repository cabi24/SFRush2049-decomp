# Source: game_code.bin (decompressed)
# Address: 0x800AC668

glabel wheel_torque_apply
    /* 800AC668 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800AC66C AFA50044 */  sw $a1, 68($sp)
    /* 800AC670 00802825 */  or $a1, $a0, $zero
    /* 800AC674 AFBF001C */  sw $ra, 28($sp)
    /* 800AC678 AFA40040 */  sw $a0, 64($sp)
    /* 800AC67C 240EFFFF */  addiu $t6, $zero, -1
    /* 800AC680 3C018015 */  lui $at, 0x8015
    /* 800AC684 27A40024 */  addiu $a0, $sp, 36
    /* 800AC688 0C0291CF */  jal 0x800A473C
    /* 800AC68C A42E9D90 */  sh $t6, -25200($at)
    /* 800AC690 3C0F8015 */  lui $t7, 0x8015
    /* 800AC694 8DEF97FC */  lw $t7, -26628($t7)
    /* 800AC698 3C188009 */  lui $t8, 0x8009
    /* 800AC69C 2718AD48 */  addiu $t8, $t8, -21176
    /* 800AC6A0 3C058015 */  lui $a1, 0x8015
    /* 800AC6A4 8DE60000 */  lw $a2, 0($t7)
    /* 800AC6A8 AFB80010 */  sw $t8, 16($sp)
    /* 800AC6AC 8CA59818 */  lw $a1, -26600($a1)
    /* 800AC6B0 27A40020 */  addiu $a0, $sp, 32
    /* 800AC6B4 0C024B16 */  jal 0x80092C58
    /* 800AC6B8 24070014 */  addiu $a3, $zero, 20
    /* 800AC6BC 8C590000 */  lw $t9, 0($v0)
    /* 800AC6C0 3C018015 */  lui $at, 0x8015
    /* 800AC6C4 3C048015 */  lui $a0, 0x8015
    /* 800AC6C8 AC399B80 */  sw $t9, -25728($at)
    /* 800AC6CC 8C849B80 */  lw $a0, -25728($a0)
    /* 800AC6D0 0C02B0F6 */  jal 0x800AC3D8
    /* 800AC6D4 87A50046 */  lh $a1, 70($sp)
    /* 800AC6D8 0C02AD8E */  jal 0x800AB638
    /* 800AC6DC 00000000 */  nop
    /* 800AC6E0 8FBF001C */  lw $ra, 28($sp)
    /* 800AC6E4 3C028015 */  lui $v0, 0x8015
    /* 800AC6E8 84429D90 */  lh $v0, -25200($v0)
    /* 800AC6EC 03E00008 */  jr $ra
    /* 800AC6F0 27BD0040 */  addiu $sp, $sp, 64
