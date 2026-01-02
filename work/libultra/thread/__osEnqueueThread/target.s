# Source: D580.s
# Address: 0x8000D0C4

glabel func_8000D0C4
    /* DCC4 8000D0C4 8C980000 */  lw         $t8, 0x0($a0)
    /* DCC8 8000D0C8 8CAF0004 */  lw         $t7, 0x4($a1)
    /* DCCC 8000D0CC 0080C825 */  or         $t9, $a0, $zero
    /* DCD0 8000D0D0 8F0E0004 */  lw         $t6, 0x4($t8)
    /* DCD4 8000D0D4 01CF082A */  slt        $at, $t6, $t7
    /* DCD8 8000D0D8 14200007 */  bnez       $at, .L8000D0F8
    /* DCDC 8000D0DC 00000000 */   nop
  .L8000D0E0:
    /* DCE0 8000D0E0 0300C825 */  or         $t9, $t8, $zero
    /* DCE4 8000D0E4 8F180000 */  lw         $t8, 0x0($t8)
    /* DCE8 8000D0E8 8F0E0004 */  lw         $t6, 0x4($t8)
    /* DCEC 8000D0EC 01CF082A */  slt        $at, $t6, $t7
    /* DCF0 8000D0F0 1020FFFB */  beqz       $at, .L8000D0E0
    /* DCF4 8000D0F4 00000000 */   nop
  .L8000D0F8:
    /* DCF8 8000D0F8 8F380000 */  lw         $t8, 0x0($t9)
    /* DCFC 8000D0FC ACB80000 */  sw         $t8, 0x0($a1)
    /* DD00 8000D100 AF250000 */  sw         $a1, 0x0($t9)
    /* DD04 8000D104 03E00008 */  jr         $ra
    /* DD08 8000D108 ACA40008 */   sw        $a0, 0x8($a1)