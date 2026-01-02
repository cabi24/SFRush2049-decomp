# Source: E7C0.s
# Address: 0x8000DBC0

glabel func_8000DBC0
    /* E7C0 8000DBC0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E7C4 8000DBC4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E7C8 8000DBC8 240E0001 */  addiu      $t6, $zero, 0x1
    /* E7CC 8000DBCC 3C018003 */  lui        $at, %hi(D_8002C4A0)
    /* E7D0 8000DBD0 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E7D4 8000DBD4 3C058003 */  lui        $a1, %hi(D_80037C70)
    /* E7D8 8000DBD8 AC2EC4A0 */  sw         $t6, %lo(D_8002C4A0)($at)
    /* E7DC 8000DBDC 24A57C70 */  addiu      $a1, $a1, %lo(D_80037C70)
    /* E7E0 8000DBE0 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E7E4 8000DBE4 0C001A80 */  jal        func_80006A00
    /* E7E8 8000DBE8 24060001 */   addiu     $a2, $zero, 0x1
    /* E7EC 8000DBEC 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E7F0 8000DBF0 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E7F4 8000DBF4 00002825 */  or         $a1, $zero, $zero
    /* E7F8 8000DBF8 0C001D78 */  jal        func_800075E0
    /* E7FC 8000DBFC 00003025 */   or        $a2, $zero, $zero
    /* E800 8000DC00 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E804 8000DC04 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E808 8000DC08 03E00008 */  jr         $ra
    /* E80C 8000DC0C 00000000 */   nop