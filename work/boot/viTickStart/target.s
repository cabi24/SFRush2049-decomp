# Source: 1050.s
# Address: 0x80001350

glabel func_80001350
    /* 1F50 80001350 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 1F54 80001354 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1F58 80001358 F7B40010 */  sdc1       $fs0, 0x10($sp)
    /* 1F5C 8000135C 0C001C74 */  jal        func_800071D0
    /* 1F60 80001360 24040001 */   addiu     $a0, $zero, 0x1
    /* 1F64 80001364 AFA20024 */  sw         $v0, 0x24($sp)
    /* 1F68 80001368 3C0E8003 */  lui        $t6, %hi(D_8002EB64)
    /* 1F6C 8000136C 8DCEEB64 */  lw         $t6, %lo(D_8002EB64)($t6)
    /* 1F70 80001370 3C018003 */  lui        $at, %hi(D_8002EBA4)
    /* 1F74 80001374 AC2EEBA4 */  sw         $t6, %lo(D_8002EBA4)($at)
    /* 1F78 80001378 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* 1F7C 8000137C 44812000 */  mtc1       $at, $ft0
    /* 1F80 80001380 3C018003 */  lui        $at, %hi(D_8002AFB8)
    /* 1F84 80001384 C426AFB8 */  lwc1       $ft1, %lo(D_8002AFB8)($at)
    /* 1F88 80001388 3C018003 */  lui        $at, %hi(D_8002EB94)
    /* 1F8C 8000138C 46062502 */  mul.s      $fs0, $ft0, $ft1
    /* 1F90 80001390 E434EB94 */  swc1       $fs0, %lo(D_8002EB94)($at)
    /* 1F94 80001394 3C018003 */  lui        $at, %hi(D_8002EB90)
    /* 1F98 80001398 E434EB90 */  swc1       $fs0, %lo(D_8002EB90)($at)
    /* 1F9C 8000139C 0C001C74 */  jal        func_800071D0
    /* 1FA0 800013A0 8FA40024 */   lw        $a0, 0x24($sp)
    /* 1FA4 800013A4 10000001 */  b          .L800013AC
    /* 1FA8 800013A8 00000000 */   nop
  .L800013AC:
    /* 1FAC 800013AC 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1FB0 800013B0 D7B40010 */  ldc1       $fs0, 0x10($sp)
    /* 1FB4 800013B4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 1FB8 800013B8 03E00008 */  jr         $ra
    /* 1FBC 800013BC 00000000 */   nop