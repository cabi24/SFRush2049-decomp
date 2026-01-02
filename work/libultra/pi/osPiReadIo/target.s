# Source: E390.s
# Address: 0x8000D7A0

glabel func_8000D7A0
    /* E3A0 8000D7A0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E3A4 8000D7A4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E3A8 8000D7A8 AFA40018 */  sw         $a0, 0x18($sp)
    /* E3AC 8000D7AC 0C003F5C */  jal        func_8000FD70
    /* E3B0 8000D7B0 AFA5001C */   sw        $a1, 0x1C($sp)
    /* E3B4 8000D7B4 10400003 */  beqz       $v0, .L8000D7C4
    /* E3B8 8000D7B8 8FAE0018 */   lw        $t6, 0x18($sp)
    /* E3BC 8000D7BC 10000007 */  b          .L8000D7DC
    /* E3C0 8000D7C0 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000D7C4:
    /* E3C4 8000D7C4 3C01A000 */  lui        $at, %hi(D_A0000000)
    /* E3C8 8000D7C8 01C17825 */  or         $t7, $t6, $at
    /* E3CC 8000D7CC 8DF80000 */  lw         $t8, %lo(D_A0000000)($t7)
    /* E3D0 8000D7D0 8FB9001C */  lw         $t9, 0x1C($sp)
    /* E3D4 8000D7D4 00001025 */  or         $v0, $zero, $zero
    /* E3D8 8000D7D8 AF380000 */  sw         $t8, 0x0($t9)
  .L8000D7DC:
    /* E3DC 8000D7DC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E3E0 8000D7E0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E3E4 8000D7E4 03E00008 */  jr         $ra
    /* E3E8 8000D7E8 00000000 */   nop