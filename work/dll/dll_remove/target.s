# Source: CC50.s
# Address: 0x8000C050

glabel func_8000C050
    /* CC50 8000C050 00803025 */  or         $a2, $a0, $zero
    /* CC54 8000C054 8CC70000 */  lw         $a3, 0x0($a2)
    /* CC58 8000C058 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* CC5C 8000C05C 10E0000A */  beqz       $a3, .L8000C088
    /* CC60 8000C060 00000000 */   nop
  .L8000C064:
    /* CC64 8000C064 14E50004 */  bne        $a3, $a1, .L8000C078
    /* CC68 8000C068 00000000 */   nop
    /* CC6C 8000C06C 8CAE0000 */  lw         $t6, 0x0($a1)
    /* CC70 8000C070 10000005 */  b          .L8000C088
    /* CC74 8000C074 ACCE0000 */   sw        $t6, 0x0($a2)
  .L8000C078:
    /* CC78 8000C078 00E03025 */  or         $a2, $a3, $zero
    /* CC7C 8000C07C 8CC70000 */  lw         $a3, 0x0($a2)
    /* CC80 8000C080 14E0FFF8 */  bnez       $a3, .L8000C064
    /* CC84 8000C084 00000000 */   nop
  .L8000C088:
    /* CC88 8000C088 03E00008 */  jr         $ra
    /* CC8C 8000C08C 27BD0008 */   addiu     $sp, $sp, 0x8