# Source: game_code.bin (decompressed)
# Address: 0x800D9060

glabel stat_results_display
    /* 800D9060 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800D9064 3C158014 */  lui $s5, 0x8014
    /* 800D9068 000E7880 */  sll $t7, $t6, 2
    /* 800D906C 01EE7823 */  subu $t7, $t7, $t6
    /* 800D9070 000F7940 */  sll $t7, $t7, 5
    /* 800D9074 3C148011 */  lui $s4, 0x8011
    /* 800D9078 26B561D0 */  addiu $s5, $s5, 25040
    /* 800D907C AFBF0014 */  sw $ra, 20($sp)
    /* 800D9080 028FA021 */  addu $s4, $s4, $t7
    /* 800D9084 8E943E8C */  lw $s4, 16012($s4)
    /* 800D9088 02A02025 */  or $a0, $s5, $zero
    /* 800D908C 00002825 */  or $a1, $zero, $zero
    /* 800D9090 0C001C9C */  jal 0x80007270
    /* 800D9094 24060001 */  addiu $a2, $zero, 1
    /* 800D9098 0C02D080 */  jal 0x800B4200
    /* 800D909C 2412000B */  addiu $s2, $zero, 11
    /* 800D90A0 02A02025 */  or $a0, $s5, $zero
    /* 800D90A4 00002825 */  or $a1, $zero, $zero
    /* 800D90A8 0C001D78 */  jal 0x800075E0
    /* 800D90AC 00003025 */  or $a2, $zero, $zero
    /* 800D90B0 2694FFE8 */  addiu $s4, $s4, -24
    /* 800D90B4 3C0C8015 */  lui $t4, 0x8015
    /* 800D90B8 258CA10A */  addiu $t4, $t4, -24310
    /* 800D90BC 06810003 */  bgez $s4, 0x800D90CC
    /* 800D90C0 0014C103 */  sra $t8, $s4, 4
    /* 800D90C4 2681000F */  addiu $at, $s4, 15
    /* 800D90C8 0001C103 */  sra $t8, $at, 4
    /* 800D90CC A5980000 */  sh $t8, 0($t4)
    /* 800D90D0 24050002 */  addiu $a1, $zero, 2
    /* 800D90D4 00002025 */  or $a0, $zero, $zero
    /* 800D90D8 240B000C */  addiu $t3, $zero, 12
    /* 800D90DC 240A0005 */  addiu $t2, $zero, 5
    /* 800D90E0 24090006 */  addiu $t1, $zero, 6
    /* 800D90E4 24080007 */  addiu $t0, $zero, 7
    /* 800D90E8 24070008 */  addiu $a3, $zero, 8
    /* 800D90EC 14870002 */  bne $a0, $a3, 0x800D90F8
    /* 800D90F0 24030001 */  addiu $v1, $zero, 1
    /* 800D90F4 00001825 */  or $v1, $zero, $zero
    /* 800D90F8 0003102B */  sltu $v0, $zero, $v1
    /* 800D90FC 10400002 */  beq $v0, $zero, 0x800D9108
    /* 800D9100 24030001 */  addiu $v1, $zero, 1
    /* 800D9104 24A50001 */  addiu $a1, $a1, 1
    /* 800D9108 54880003 */  .word 0x54880003
    /* 800D910C 0003102B */  sltu $v0, $zero, $v1
    /* 800D9110 00001825 */  or $v1, $zero, $zero
    /* 800D9114 0003102B */  sltu $v0, $zero, $v1
    /* 800D9118 10400002 */  beq $v0, $zero, 0x800D9124
    /* 800D911C 24030001 */  addiu $v1, $zero, 1
    /* 800D9120 24A50001 */  addiu $a1, $a1, 1
    /* 800D9124 54890003 */  .word 0x54890003
    /* 800D9128 0003102B */  sltu $v0, $zero, $v1
    /* 800D912C 00001825 */  or $v1, $zero, $zero
    /* 800D9130 0003102B */  sltu $v0, $zero, $v1
    /* 800D9134 10400002 */  beq $v0, $zero, 0x800D9140
    /* 800D9138 24030001 */  addiu $v1, $zero, 1
    /* 800D913C 24A50001 */  addiu $a1, $a1, 1
    /* 800D9140 148A0002 */  bne $a0, $t2, 0x800D914C
    /* 800D9144 00000000 */  nop
    /* 800D9148 00001825 */  or $v1, $zero, $zero
    /* 800D914C 10600002 */  beq $v1, $zero, 0x800D9158
    /* 800D9150 24840004 */  addiu $a0, $a0, 4
    /* 800D9154 24A50001 */  addiu $a1, $a1, 1
    /* 800D9158 148BFFE4 */  bne $a0, $t3, 0x800D90EC
    /* 800D915C 00000000 */  nop
    /* 800D9160 85990000 */  lh $t9, 0($t4)
    /* 800D9164 3C0E8011 */  lui $t6, 0x8011
    /* 800D9168 00B9082A */  slt $at, $a1, $t9
    /* 800D916C 54200008 */  .word 0x54200008
    /* 800D9170 A5850000 */  sh $a1, 0($t4)
    /* 800D9174 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800D9178 3C017C03 */  lui $at, 0x7C03
    /* 800D917C 3421FFFE */  ori $at, $at, 0xFFFE
    /* 800D9180 01C17824 */  and $t7, $t6, $at
    /* 800D9184 51E00003 */  .word 0x51E00003
    /* 800D9188 8FBF0014 */  lw $ra, 20($sp)
    /* 800D918C A5850000 */  sh $a1, 0($t4)
    /* 800D9190 8FBF0014 */  lw $ra, 20($sp)
    /* 800D9194 27BD0018 */  addiu $sp, $sp, 24
    /* 800D9198 03E00008 */  jr $ra
    /* 800D919C 00000000 */  nop
