# Source: AB20.s
# Address: 0x8000A2F0

glabel func_8000A2F0
    /* AEF0 8000A2F0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* AEF4 8000A2F4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* AEF8 8000A2F8 8C8E0000 */  lw         $t6, 0x0($a0)
    /* AEFC 8000A2FC 24030005 */  addiu      $v1, $zero, 0x5
    /* AF00 8000A300 31CF0005 */  andi       $t7, $t6, 0x5
    /* AF04 8000A304 11E0000B */  beqz       $t7, .L8000A334
    /* AF08 8000A308 00000000 */   nop
    /* AF0C 8000A30C 0C003C49 */  jal        func_8000F124
    /* AF10 8000A310 AFA40018 */   sw        $a0, 0x18($sp)
    /* AF14 8000A314 8FA40018 */  lw         $a0, 0x18($sp)
    /* AF18 8000A318 14400006 */  bnez       $v0, .L8000A334
    /* AF1C 8000A31C 00401825 */   or        $v1, $v0, $zero
    /* AF20 8000A320 8C980000 */  lw         $t8, 0x0($a0)
    /* AF24 8000A324 2401FFFB */  addiu      $at, $zero, -0x5
    /* AF28 8000A328 0301C824 */  and        $t9, $t8, $at
    /* AF2C 8000A32C 10000001 */  b          .L8000A334
    /* AF30 8000A330 AC990000 */   sw        $t9, 0x0($a0)
  .L8000A334:
    /* AF34 8000A334 8FBF0014 */  lw         $ra, 0x14($sp)
    /* AF38 8000A338 27BD0018 */  addiu      $sp, $sp, 0x18
    /* AF3C 8000A33C 00601025 */  or         $v0, $v1, $zero
    /* AF40 8000A340 03E00008 */  jr         $ra
    /* AF44 8000A344 00000000 */   nop