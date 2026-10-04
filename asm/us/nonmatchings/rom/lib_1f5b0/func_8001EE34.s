nonmatching func_8001EE34, 0x68

glabel func_8001EE34
    /* 1FA34 8001EE34 3C038005 */  lui        $v1, %hi(D_8004FA18)
    /* 1FA38 8001EE38 9063FA18 */  lbu        $v1, %lo(D_8004FA18)($v1)
    /* 1FA3C 8001EE3C 00001025 */  or         $v0, $zero, $zero
    /* 1FA40 8001EE40 3C048005 */  lui        $a0, %hi(D_800504C8)
    /* 1FA44 8001EE44 10600007 */  beqz       $v1, .L8001EE64
    /* 1FA48 8001EE48 340EFFFF */   ori       $t6, $zero, 0xFFFF
    /* 1FA4C 8001EE4C 248404C8 */  addiu      $a0, $a0, %lo(D_800504C8)
  .L8001EE50:
    /* 1FA50 8001EE50 24420001 */  addiu      $v0, $v0, 0x1
    /* 1FA54 8001EE54 0043082B */  sltu       $at, $v0, $v1
    /* 1FA58 8001EE58 24840004 */  addiu      $a0, $a0, 0x4
    /* 1FA5C 8001EE5C 1420FFFC */  bnez       $at, .L8001EE50
    /* 1FA60 8001EE60 A480FFFE */   sh        $zero, -0x2($a0)
  .L8001EE64:
    /* 1FA64 8001EE64 3C028005 */  lui        $v0, %hi(D_80050548)
    /* 1FA68 8001EE68 3C048005 */  lui        $a0, %hi(D_80050648)
    /* 1FA6C 8001EE6C 24840648 */  addiu      $a0, $a0, %lo(D_80050648)
    /* 1FA70 8001EE70 24420548 */  addiu      $v0, $v0, %lo(D_80050548)
    /* 1FA74 8001EE74 240300FF */  addiu      $v1, $zero, 0xFF
  .L8001EE78:
    /* 1FA78 8001EE78 24420004 */  addiu      $v0, $v0, 0x4
    /* 1FA7C 8001EE7C A043FFFD */  sb         $v1, -0x3($v0)
    /* 1FA80 8001EE80 A043FFFE */  sb         $v1, -0x2($v0)
    /* 1FA84 8001EE84 A043FFFF */  sb         $v1, -0x1($v0)
    /* 1FA88 8001EE88 1444FFFB */  bne        $v0, $a0, .L8001EE78
    /* 1FA8C 8001EE8C A043FFFC */   sb        $v1, -0x4($v0)
    /* 1FA90 8001EE90 3C018005 */  lui        $at, %hi(D_80050A48)
    /* 1FA94 8001EE94 03E00008 */  jr         $ra
    /* 1FA98 8001EE98 A42E0A48 */   sh        $t6, %lo(D_80050A48)($at)
endlabel func_8001EE34
