nonmatching func_8001C1D8, 0x1B0

glabel func_8001C1D8
    /* 1CDD8 8001C1D8 3C018005 */  lui        $at, %hi(D_8004F800)
    /* 1CDDC 8001C1DC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1CDE0 8001C1E0 AC24F800 */  sw         $a0, %lo(D_8004F800)($at)
    /* 1CDE4 8001C1E4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1CDE8 8001C1E8 3C018005 */  lui        $at, %hi(D_8004F804)
    /* 1CDEC 8001C1EC 240E2800 */  addiu      $t6, $zero, 0x2800
    /* 1CDF0 8001C1F0 AC2EF804 */  sw         $t6, %lo(D_8004F804)($at)
    /* 1CDF4 8001C1F4 24040078 */  addiu      $a0, $zero, 0x78
    /* 1CDF8 8001C1F8 0C006698 */  jal        func_80019A60
    /* 1CDFC 8001C1FC 240500FF */   addiu     $a1, $zero, 0xFF
    /* 1CE00 8001C200 3C018005 */  lui        $at, %hi(D_8004F2F8)
    /* 1CE04 8001C204 3C028005 */  lui        $v0, %hi(D_8004BEB8)
    /* 1CE08 8001C208 3C098005 */  lui        $t1, %hi(D_8004BEB8 + 0x3400)
    /* 1CE0C 8001C20C A020F2F8 */  sb         $zero, %lo(D_8004F2F8)($at)
    /* 1CE10 8001C210 2529F2B8 */  addiu      $t1, $t1, %lo(D_8004BEB8 + 0x3400)
    /* 1CE14 8001C214 2442BEB8 */  addiu      $v0, $v0, %lo(D_8004BEB8)
    /* 1CE18 8001C218 24080064 */  addiu      $t0, $zero, 0x64
    /* 1CE1C 8001C21C 24070017 */  addiu      $a3, $zero, 0x17
    /* 1CE20 8001C220 3C06003F */  lui        $a2, (0x3F0000 >> 16)
    /* 1CE24 8001C224 24050080 */  addiu      $a1, $zero, 0x80
    /* 1CE28 8001C228 240400FF */  addiu      $a0, $zero, 0xFF
    /* 1CE2C 8001C22C 2403FFFF */  addiu      $v1, $zero, -0x1
  .L8001C230:
    /* 1CE30 8001C230 00000A02 */  srl        $at, $zero, 8
    /* 1CE34 8001C234 244201A0 */  addiu      $v0, $v0, 0x1A0
    /* 1CE38 8001C238 A041FEC8 */  sb         $at, -0x138($v0)
    /* 1CE3C 8001C23C A041FFD0 */  sb         $at, -0x30($v0)
    /* 1CE40 8001C240 A041FFDC */  sb         $at, -0x24($v0)
    /* 1CE44 8001C244 0049082B */  sltu       $at, $v0, $t1
    /* 1CE48 8001C248 A843FEC0 */  swl        $v1, -0x140($v0)
    /* 1CE4C 8001C24C A840FE60 */  swl        $zero, -0x1A0($v0)
    /* 1CE50 8001C250 A840FE84 */  swl        $zero, -0x17C($v0)
    /* 1CE54 8001C254 A840FE88 */  swl        $zero, -0x178($v0)
    /* 1CE58 8001C258 A840FE90 */  swl        $zero, -0x170($v0)
    /* 1CE5C 8001C25C A846FE98 */  swl        $a2, -0x168($v0)
    /* 1CE60 8001C260 A840FFCC */  swl        $zero, -0x34($v0)
    /* 1CE64 8001C264 A840FFD8 */  swl        $zero, -0x28($v0)
    /* 1CE68 8001C268 A848FEEC */  swl        $t0, -0x114($v0)
    /* 1CE6C 8001C26C B843FEC3 */  swr        $v1, -0x13D($v0)
    /* 1CE70 8001C270 B840FE63 */  swr        $zero, -0x19D($v0)
    /* 1CE74 8001C274 B840FE87 */  swr        $zero, -0x179($v0)
    /* 1CE78 8001C278 B840FE8B */  swr        $zero, -0x175($v0)
    /* 1CE7C 8001C27C A040FE8E */  sb         $zero, -0x172($v0)
    /* 1CE80 8001C280 A040FEC9 */  sb         $zero, -0x137($v0)
    /* 1CE84 8001C284 A044FEAA */  sb         $a0, -0x156($v0)
    /* 1CE88 8001C288 B840FE93 */  swr        $zero, -0x16D($v0)
    /* 1CE8C 8001C28C A045FEF9 */  sb         $a1, -0x107($v0)
    /* 1CE90 8001C290 A040FEFA */  sb         $zero, -0x106($v0)
    /* 1CE94 8001C294 B846FE9B */  swr        $a2, -0x165($v0)
    /* 1CE98 8001C298 AC40FEA0 */  sw         $zero, -0x160($v0)
    /* 1CE9C 8001C29C AC40FEA4 */  sw         $zero, -0x15C($v0)
    /* 1CEA0 8001C2A0 A040FEA8 */  sb         $zero, -0x158($v0)
    /* 1CEA4 8001C2A4 A040FEA9 */  sb         $zero, -0x157($v0)
    /* 1CEA8 8001C2A8 A040FF1D */  sb         $zero, -0xE3($v0)
    /* 1CEAC 8001C2AC A047FF1E */  sb         $a3, -0xE2($v0)
    /* 1CEB0 8001C2B0 B840FFCF */  swr        $zero, -0x31($v0)
    /* 1CEB4 8001C2B4 A040FFD1 */  sb         $zero, -0x2F($v0)
    /* 1CEB8 8001C2B8 B840FFDB */  swr        $zero, -0x25($v0)
    /* 1CEBC 8001C2BC A040FFDD */  sb         $zero, -0x23($v0)
    /* 1CEC0 8001C2C0 B848FEEF */  swr        $t0, -0x111($v0)
    /* 1CEC4 8001C2C4 1420FFDA */  bnez       $at, .L8001C230
    /* 1CEC8 8001C2C8 A040FEF8 */   sb        $zero, -0x108($v0)
    /* 1CECC 8001C2CC 3C028005 */  lui        $v0, %hi(D_8004F300)
    /* 1CED0 8001C2D0 3C048005 */  lui        $a0, %hi(D_8004F300 + 0x500)
    /* 1CED4 8001C2D4 2484F800 */  addiu      $a0, $a0, %lo(D_8004F300 + 0x500)
    /* 1CED8 8001C2D8 2442F300 */  addiu      $v0, $v0, %lo(D_8004F300)
    /* 1CEDC 8001C2DC 3C05007F */  lui        $a1, (0x7F0000 >> 16)
    /* 1CEE0 8001C2E0 24030004 */  addiu      $v1, $zero, 0x4
  .L8001C2E4:
    /* 1CEE4 8001C2E4 24420028 */  addiu      $v0, $v0, 0x28
    /* 1CEE8 8001C2E8 0044082B */  sltu       $at, $v0, $a0
    /* 1CEEC 8001C2EC AC40FFD8 */  sw         $zero, -0x28($v0)
    /* 1CEF0 8001C2F0 AC40FFDC */  sw         $zero, -0x24($v0)
    /* 1CEF4 8001C2F4 AC40FFE4 */  sw         $zero, -0x1C($v0)
    /* 1CEF8 8001C2F8 A043FFEC */  sb         $v1, -0x14($v0)
    /* 1CEFC 8001C2FC 1420FFF9 */  bnez       $at, .L8001C2E4
    /* 1CF00 8001C300 AC45FFF0 */   sw        $a1, -0x10($v0)
    /* 1CF04 8001C304 3C048005 */  lui        $a0, %hi(D_8004F300)
    /* 1CF08 8001C308 2484F300 */  addiu      $a0, $a0, %lo(D_8004F300)
    /* 1CF0C 8001C30C 240F0001 */  addiu      $t7, $zero, 0x1
    /* 1CF10 8001C310 3C028005 */  lui        $v0, %hi(D_8004F300)
    /* 1CF14 8001C314 3C038005 */  lui        $v1, %hi(D_8004F300 + 0x140)
    /* 1CF18 8001C318 A08F04EC */  sb         $t7, 0x4EC($a0)
    /* 1CF1C 8001C31C 2463F440 */  addiu      $v1, $v1, %lo(D_8004F300 + 0x140)
    /* 1CF20 8001C320 2442F300 */  addiu      $v0, $v0, %lo(D_8004F300)
  .L8001C324:
    /* 1CF24 8001C324 24420028 */  addiu      $v0, $v0, 0x28
    /* 1CF28 8001C328 0043082B */  sltu       $at, $v0, $v1
    /* 1CF2C 8001C32C 1420FFFD */  bnez       $at, .L8001C324
    /* 1CF30 8001C330 A0400384 */   sb        $zero, 0x384($v0)
    /* 1CF34 8001C334 AC850348 */  sw         $a1, 0x348($a0)
    /* 1CF38 8001C338 0C007A6C */  jal        func_8001E9B0
    /* 1CF3C 8001C33C AC850370 */   sw        $a1, 0x370($a0)
    /* 1CF40 8001C340 0C007E19 */  jal        func_8001F864
    /* 1CF44 8001C344 00000000 */   nop
    /* 1CF48 8001C348 3C028005 */  lui        $v0, %hi(D_8004BE98)
    /* 1CF4C 8001C34C 3C038005 */  lui        $v1, %hi(D_8004BE98 + 0x20)
    /* 1CF50 8001C350 2463BEB8 */  addiu      $v1, $v1, %lo(D_8004BE98 + 0x20)
    /* 1CF54 8001C354 2442BE98 */  addiu      $v0, $v0, %lo(D_8004BE98)
  .L8001C358:
    /* 1CF58 8001C358 24420008 */  addiu      $v0, $v0, 0x8
    /* 1CF5C 8001C35C A440FFFA */  sh         $zero, -0x6($v0)
    /* 1CF60 8001C360 A440FFFC */  sh         $zero, -0x4($v0)
    /* 1CF64 8001C364 A440FFFE */  sh         $zero, -0x2($v0)
    /* 1CF68 8001C368 1443FFFB */  bne        $v0, $v1, .L8001C358
    /* 1CF6C 8001C36C A440FFF8 */   sh        $zero, -0x8($v0)
    /* 1CF70 8001C370 0C008633 */  jal        func_800218CC
    /* 1CF74 8001C374 00000000 */   nop
    /* 1CF78 8001C378 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1CF7C 8001C37C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1CF80 8001C380 03E00008 */  jr         $ra
    /* 1CF84 8001C384 00000000 */   nop
endlabel func_8001C1D8
    /* 1CF88 8001C388 00000000 */  nop
    /* 1CF8C 8001C38C 00000000 */  nop
