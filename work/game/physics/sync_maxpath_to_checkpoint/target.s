# Source: game_code.bin (decompressed)
# Address: 0x800A133C

glabel sync_maxpath_to_checkpoint
    /* 800A133C 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800A1340 AFB40038 */  sw $s4, 56($sp)
    /* 800A1344 AFB30034 */  sw $s3, 52($sp)
    /* 800A1348 AFB20030 */  sw $s2, 48($sp)
    /* 800A134C AFB1002C */  sw $s1, 44($sp)
    /* 800A1350 AFB00028 */  sw $s0, 40($sp)
    /* 800A1354 3C0E8012 */  lui $t6, 0x8012
    /* 800A1358 3C0F00BE */  lui $t7, 0x00BE
    /* 800A135C AFBF003C */  sw $ra, 60($sp)
    /* 800A1360 25EFA100 */  addiu $t7, $t7, -24320
    /* 800A1364 25CE3850 */  addiu $t6, $t6, 14416
    /* 800A1368 3C108038 */  lui $s0, 0x8038
    /* 800A136C 3C11803C */  lui $s1, 0x803C
    /* 800A1370 3C12803C */  lui $s2, 0x803C
    /* 800A1374 3C138012 */  lui $s3, 0x8012
    /* 800A1378 3C148003 */  lui $s4, 0x8003
    /* 800A137C 3C06803B */  lui $a2, 0x803B
    /* 800A1380 34C6B380 */  ori $a2, $a2, 0xB380
    /* 800A1384 8E94B020 */  lw $s4, -20448($s4)
    /* 800A1388 2673ED00 */  addiu $s3, $s3, -4864
    /* 800A138C 26529A50 */  addiu $s2, $s2, -26032
    /* 800A1390 2631AE20 */  addiu $s1, $s1, -20960
    /* 800A1394 3610A400 */  ori $s0, $s0, 0xA400
    /* 800A1398 AFAE0004 */  sw $t6, 4($sp)
    /* 800A139C 0C028479 */  jal 0x800A11E4
    /* 800A13A0 AFAF0010 */  sw $t7, 16($sp)
    /* 800A13A4 8FBF003C */  lw $ra, 60($sp)
    /* 800A13A8 8FB00028 */  lw $s0, 40($sp)
    /* 800A13AC 8FB1002C */  lw $s1, 44($sp)
    /* 800A13B0 8FB20030 */  lw $s2, 48($sp)
    /* 800A13B4 8FB30034 */  lw $s3, 52($sp)
    /* 800A13B8 8FB40038 */  lw $s4, 56($sp)
    /* 800A13BC 03E00008 */  jr $ra
    /* 800A13C0 27BD0040 */  addiu $sp, $sp, 64
