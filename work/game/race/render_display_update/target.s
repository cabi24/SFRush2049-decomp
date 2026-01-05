# Source: game_code.bin (decompressed)
# Address: 0x800FAD58

glabel render_display_update
    /* 800FAD58 27BDFFB8 */  addiu $sp, $sp, -72
    /* 800FAD5C AFBF0014 */  sw $ra, 20($sp)
    /* 800FAD60 3C0142A0 */  lui $at, 0x42A0
    /* 800FAD64 44811000 */  .word 0x44811000
    /* 800FAD68 C4860044 */  lwc1 $f6, 68($a0)
    /* 800FAD6C C490002C */  lwc1 $f16, 44($a0)
    /* 800FAD70 C4840008 */  lwc1 $f4, 8($a0)
    /* 800FAD74 46023202 */  .word 0x46023202
    /* 800FAD78 3C0140A0 */  lui $at, 0x40A0
    /* 800FAD7C 00802825 */  or $a1, $a0, $zero
    /* 800FAD80 46008482 */  .word 0x46008482
    /* 800FAD84 27A6003C */  addiu $a2, $sp, 60
    /* 800FAD88 27A70018 */  addiu $a3, $sp, 24
    /* 800FAD8C 46082280 */  .word 0x46082280
    /* 800FAD90 44814000 */  .word 0x44814000
    /* 800FAD94 460A9180 */  .word 0x460A9180
    /* 800FAD98 E7A6003C */  swc1 $f6, 60($sp)
    /* 800FAD9C C484000C */  lwc1 $f4, 12($a0)
    /* 800FADA0 46082400 */  .word 0x46082400
    /* 800FADA4 E7B00040 */  swc1 $f16, 64($sp)
    /* 800FADA8 C48A004C */  lwc1 $f10, 76($a0)
    /* 800FADAC C4880034 */  lwc1 $f8, 52($a0)
    /* 800FADB0 C4920010 */  lwc1 $f18, 16($a0)
    /* 800FADB4 46025182 */  .word 0x46025182
    /* 800FADB8 00002025 */  or $a0, $zero, $zero
    /* 800FADBC 46004402 */  .word 0x46004402
    /* 800FADC0 46069100 */  .word 0x46069100
    /* 800FADC4 46048280 */  .word 0x46048280
    /* 800FADC8 0C03A577 */  jal 0x800E95DC
    /* 800FADCC E7AA0044 */  swc1 $f10, 68($sp)
    /* 800FADD0 8FBF0014 */  lw $ra, 20($sp)
    /* 800FADD4 27BD0048 */  addiu $sp, $sp, 72
    /* 800FADD8 03E00008 */  jr $ra
    /* 800FADDC 00000000 */  nop
