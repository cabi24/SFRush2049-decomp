nonmatching func_8001BDB8, 0x5C

glabel func_8001BDB8
    /* 1C9B8 8001BDB8 AFA40000 */  sw         $a0, 0x0($sp)
    /* 1C9BC 8001BDBC 308400FF */  andi       $a0, $a0, 0xFF
    /* 1C9C0 8001BDC0 00047080 */  sll        $t6, $a0, 2
    /* 1C9C4 8001BDC4 01C47021 */  addu       $t6, $t6, $a0
    /* 1C9C8 8001BDC8 3C0F8005 */  lui        $t7, %hi(D_8004F300)
    /* 1C9CC 8001BDCC 25EFF300 */  addiu      $t7, $t7, %lo(D_8004F300)
    /* 1C9D0 8001BDD0 000E70C0 */  sll        $t6, $t6, 3
    /* 1C9D4 8001BDD4 01CF1021 */  addu       $v0, $t6, $t7
    /* 1C9D8 8001BDD8 90580014 */  lbu        $t8, 0x14($v0)
    /* 1C9DC 8001BDDC 24010004 */  addiu      $at, $zero, 0x4
    /* 1C9E0 8001BDE0 5301000A */  beql       $t8, $at, .L8001BE0C
    /* 1C9E4 8001BDE4 00001025 */   or        $v0, $zero, $zero
    /* 1C9E8 8001BDE8 8C59000C */  lw         $t9, 0xC($v0)
    /* 1C9EC 8001BDEC 53200007 */  beql       $t9, $zero, .L8001BE0C
    /* 1C9F0 8001BDF0 00001025 */   or        $v0, $zero, $zero
    /* 1C9F4 8001BDF4 8C480008 */  lw         $t0, 0x8($v0)
    /* 1C9F8 8001BDF8 05030004 */  bgezl      $t0, .L8001BE0C
    /* 1C9FC 8001BDFC 00001025 */   or        $v0, $zero, $zero
    /* 1CA00 8001BE00 03E00008 */  jr         $ra
    /* 1CA04 8001BE04 24020001 */   addiu     $v0, $zero, 0x1
    /* 1CA08 8001BE08 00001025 */  or         $v0, $zero, $zero
  .L8001BE0C:
    /* 1CA0C 8001BE0C 03E00008 */  jr         $ra
    /* 1CA10 8001BE10 00000000 */   nop
endlabel func_8001BDB8
