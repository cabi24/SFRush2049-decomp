# Source: E7C0.s
# Address: 0x8000DC80

glabel func_8000DC80
    /* E880 8000DC80 3C03A460 */  lui        $v1, %hi(PI_STATUS_REG)
    /* E884 8000DC84 34630010 */  ori        $v1, $v1, %lo(PI_STATUS_REG)
    /* E888 8000DC88 8C620000 */  lw         $v0, 0x0($v1)
    /* E88C 8000DC8C 3C188000 */  lui        $t8, %hi(osRomBase)
    /* E890 8000DC90 304E0003 */  andi       $t6, $v0, 0x3
    /* E894 8000DC94 11C00005 */  beqz       $t6, .L8000DCAC
    /* E898 8000DC98 00000000 */   nop
    /* E89C 8000DC9C 8C620000 */  lw         $v0, 0x0($v1)
  .L8000DCA0:
    /* E8A0 8000DCA0 304F0003 */  andi       $t7, $v0, 0x3
    /* E8A4 8000DCA4 55E0FFFE */  bnel       $t7, $zero, .L8000DCA0
    /* E8A8 8000DCA8 8C620000 */   lw        $v0, 0x0($v1)
  .L8000DCAC:
    /* E8AC 8000DCAC 8F180308 */  lw         $t8, %lo(osRomBase)($t8)
    /* E8B0 8000DCB0 3C01A000 */  lui        $at, (0xA0000000 >> 16)
    /* E8B4 8000DCB4 00001025 */  or         $v0, $zero, $zero
    /* E8B8 8000DCB8 0304C825 */  or         $t9, $t8, $a0
    /* E8BC 8000DCBC 03214025 */  or         $t0, $t9, $at
    /* E8C0 8000DCC0 8D090000 */  lw         $t1, 0x0($t0)
    /* E8C4 8000DCC4 03E00008 */  jr         $ra
    /* E8C8 8000DCC8 ACA90000 */   sw        $t1, 0x0($a1)