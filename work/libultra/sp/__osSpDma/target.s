# Source: E280.s
# Address: 0x8000D680

glabel func_8000D680
    /* E280 8000D680 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E284 8000D684 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E288 8000D688 AFA40018 */  sw         $a0, 0x18($sp)
    /* E28C 8000D68C AFA5001C */  sw         $a1, 0x1C($sp)
    /* E290 8000D690 AFA60020 */  sw         $a2, 0x20($sp)
    /* E294 8000D694 0C0035C4 */  jal        func_8000D710
    /* E298 8000D698 AFA70024 */   sw        $a3, 0x24($sp)
    /* E29C 8000D69C 10400003 */  beqz       $v0, .L8000D6AC
    /* E2A0 8000D6A0 8FAE001C */   lw        $t6, 0x1C($sp)
    /* E2A4 8000D6A4 10000013 */  b          .L8000D6F4
    /* E2A8 8000D6A8 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000D6AC:
    /* E2AC 8000D6AC 3C0FA404 */  lui        $t7, %hi(SP_MEM_ADDR_REG)
    /* E2B0 8000D6B0 ADEE0000 */  sw         $t6, %lo(SP_MEM_ADDR_REG)($t7)
    /* E2B4 8000D6B4 0C003570 */  jal        func_8000D5C0
    /* E2B8 8000D6B8 8FA40020 */   lw        $a0, 0x20($sp)
    /* E2BC 8000D6BC 3C18A404 */  lui        $t8, %hi(SP_DRAM_ADDR_REG)
    /* E2C0 8000D6C0 AF020004 */  sw         $v0, %lo(SP_DRAM_ADDR_REG)($t8)
    /* E2C4 8000D6C4 8FB90018 */  lw         $t9, 0x18($sp)
    /* E2C8 8000D6C8 8FAB0024 */  lw         $t3, 0x24($sp)
    /* E2CC 8000D6CC 00001025 */  or         $v0, $zero, $zero
    /* E2D0 8000D6D0 17200006 */  bnez       $t9, .L8000D6EC
    /* E2D4 8000D6D4 256CFFFF */   addiu     $t4, $t3, -0x1
    /* E2D8 8000D6D8 8FA80024 */  lw         $t0, 0x24($sp)
    /* E2DC 8000D6DC 3C0AA404 */  lui        $t2, %hi(SP_WR_LEN_REG)
    /* E2E0 8000D6E0 2509FFFF */  addiu      $t1, $t0, -0x1
    /* E2E4 8000D6E4 10000003 */  b          .L8000D6F4
    /* E2E8 8000D6E8 AD49000C */   sw        $t1, %lo(SP_WR_LEN_REG)($t2)
  .L8000D6EC:
    /* E2EC 8000D6EC 3C0DA404 */  lui        $t5, %hi(SP_RD_LEN_REG)
    /* E2F0 8000D6F0 ADAC0008 */  sw         $t4, %lo(SP_RD_LEN_REG)($t5)
  .L8000D6F4:
    /* E2F4 8000D6F4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E2F8 8000D6F8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E2FC 8000D6FC 03E00008 */  jr         $ra
    /* E300 8000D700 00000000 */   nop