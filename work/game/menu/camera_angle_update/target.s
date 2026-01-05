# Source: game_code.bin (decompressed)
# Address: 0x800E3400

glabel camera_angle_update
    /* 800E3400 E60403C0 */  swc1 $f4, 960($s0)
    /* 800E3404 3C014000 */  lui $at, 0x4000
    /* 800E3408 44815000 */  .word 0x44815000
    /* 800E340C C6080418 */  lwc1 $f8, 1048($s0)
    /* 800E3410 460A4002 */  .word 0x460A4002
    /* 800E3414 E60004FC */  swc1 $f0, 1276($s0)
    /* 800E3418 E6000558 */  swc1 $f0, 1368($s0)
    /* 800E341C 8FBF001C */  lw $ra, 28($sp)
    /* 800E3420 8FB00018 */  lw $s0, 24($sp)
    /* 800E3424 27BD0020 */  addiu $sp, $sp, 32
    /* 800E3428 03E00008 */  jr $ra
    /* 800E342C 00000000 */  nop
