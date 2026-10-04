nonmatching func_80018C2C, 0xD4

glabel func_80018C2C
    /* 1982C 80018C2C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 19830 80018C30 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19834 80018C34 0C005D91 */  jal        func_80017644
    /* 19838 80018C38 00000000 */   nop
    /* 1983C 80018C3C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19840 80018C40 1041002B */  beq        $v0, $at, .L80018CF0
    /* 19844 80018C44 00402825 */   or        $a1, $v0, $zero
    /* 19848 80018C48 00027000 */  sll        $t6, $v0, 0
    /* 1984C 80018C4C 05C00017 */  bltz       $t6, .L80018CAC
    /* 19850 80018C50 24070FF8 */   addiu     $a3, $zero, 0xFF8
    /* 19854 80018C54 24070FF8 */  addiu      $a3, $zero, 0xFF8
    /* 19858 80018C58 00470019 */  multu      $v0, $a3
    /* 1985C 80018C5C 3C068004 */  lui        $a2, %hi(D_80043EB8)
    /* 19860 80018C60 24C63EB8 */  addiu      $a2, $a2, %lo(D_80043EB8)
    /* 19864 80018C64 00007812 */  mflo       $t7
    /* 19868 80018C68 00CF1821 */  addu       $v1, $a2, $t7
    /* 1986C 80018C6C 90780FC0 */  lbu        $t8, 0xFC0($v1)
    /* 19870 80018C70 53000020 */  beql       $t8, $zero, .L80018CF4
    /* 19874 80018C74 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 19878 80018C78 90790FC1 */  lbu        $t9, 0xFC1($v1)
    /* 1987C 80018C7C 5720001D */  bnel       $t9, $zero, .L80018CF4
    /* 19880 80018C80 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 19884 80018C84 00A70019 */  multu      $a1, $a3
    /* 19888 80018C88 00004012 */  mflo       $t0
    /* 1988C 80018C8C 00C82021 */  addu       $a0, $a2, $t0
    /* 19890 80018C90 A0800FC0 */  sb         $zero, 0xFC0($a0)
    /* 19894 80018C94 0C005CD3 */  jal        func_8001734C
    /* 19898 80018C98 AFA4001C */   sw        $a0, 0x1C($sp)
    /* 1989C 80018C9C 0C005CA7 */  jal        func_8001729C
    /* 198A0 80018CA0 8FA4001C */   lw        $a0, 0x1C($sp)
    /* 198A4 80018CA4 10000013 */  b          .L80018CF4
    /* 198A8 80018CA8 8FBF0014 */   lw        $ra, 0x14($sp)
  .L80018CAC:
    /* 198AC 80018CAC 3C017FFF */  lui        $at, (0x7FFFFFFF >> 16)
    /* 198B0 80018CB0 3421FFFF */  ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    /* 198B4 80018CB4 00412824 */  and        $a1, $v0, $at
    /* 198B8 80018CB8 00A70019 */  multu      $a1, $a3
    /* 198BC 80018CBC 3C068004 */  lui        $a2, %hi(D_80043EB8)
    /* 198C0 80018CC0 24C63EB8 */  addiu      $a2, $a2, %lo(D_80043EB8)
    /* 198C4 80018CC4 00004812 */  mflo       $t1
    /* 198C8 80018CC8 00C92021 */  addu       $a0, $a2, $t1
    /* 198CC 80018CCC 908A0FC0 */  lbu        $t2, 0xFC0($a0)
    /* 198D0 80018CD0 51400008 */  beql       $t2, $zero, .L80018CF4
    /* 198D4 80018CD4 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 198D8 80018CD8 908B0FC1 */  lbu        $t3, 0xFC1($a0)
    /* 198DC 80018CDC 55600005 */  bnel       $t3, $zero, .L80018CF4
    /* 198E0 80018CE0 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 198E4 80018CE4 908C0FEE */  lbu        $t4, 0xFEE($a0)
    /* 198E8 80018CE8 358D0008 */  ori        $t5, $t4, 0x8
    /* 198EC 80018CEC A08D0FEE */  sb         $t5, 0xFEE($a0)
  .L80018CF0:
    /* 198F0 80018CF0 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80018CF4:
    /* 198F4 80018CF4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 198F8 80018CF8 03E00008 */  jr         $ra
    /* 198FC 80018CFC 00000000 */   nop
endlabel func_80018C2C
