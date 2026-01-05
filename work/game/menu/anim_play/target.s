# Source: game_code.bin (decompressed)
# Address: 0x800EA000

glabel anim_play
    /* 800EA000 46008424 */  .word 0x46008424
    /* 800EA004 4459F800 */  .word 0x4459F800
    /* 800EA008 00000000 */  nop
    /* 800EA00C 33390078 */  andi $t9, $t9, 0x0078
    /* 800EA010 17200005 */  bne $t9, $zero, 0x800EA028
    /* 800EA014 00000000 */  nop
    /* 800EA018 44198000 */  .word 0x44198000
    /* 800EA01C 3C018000 */  lui $at, 0x8000
    /* 800EA020 10000007 */  beq $zero, $zero, 0x800EA040
    /* 800EA024 0321C825 */  or $t9, $t9, $at
    /* 800EA028 10000005 */  beq $zero, $zero, 0x800EA040
    /* 800EA02C 2419FFFF */  addiu $t9, $zero, -1
    /* 800EA030 44198000 */  .word 0x44198000
    /* 800EA034 00000000 */  nop
    /* 800EA038 0720FFFB */  bltz $t9, 0x800EA028
    /* 800EA03C 00000000 */  nop
    /* 800EA040 24011770 */  addiu $at, $zero, 6000
    /* 800EA044 0321001B */  divu $t9, $at
    /* 800EA048 00004810 */  mfhi $t1
    /* 800EA04C 44899000 */  .word 0x44899000
    /* 800EA050 44D8F800 */  .word 0x44D8F800
    /* 800EA054 3C014F80 */  lui $at, 0x4F80
    /* 800EA058 05210004 */  bgez $t1, 0x800EA06C
    /* 800EA05C 46809120 */  .word 0x46809120
    /* 800EA060 44813000 */  .word 0x44813000
    /* 800EA064 00000000 */  nop
    /* 800EA068 46062100 */  .word 0x46062100
    /* 800EA06C 3C018012 */  lui $at, 0x8012
    /* 800EA070 C42A44E0 */  lwc1 $f10, 17632($at)
    /* 800EA074 3C018012 */  lui $at, 0x8012
    /* 800EA078 C43044E4 */  lwc1 $f16, 17636($at)
    /* 800EA07C 460A2202 */  .word 0x460A2202
    /* 800EA080 00000000 */  nop
    /* 800EA084 46104302 */  .word 0x46104302
    /* 800EA088 00000000 */  nop
    /* 800EA08C AFA40040 */  sw $a0, 64($sp)
    /* 800EA090 0C00223C */  jal 0x800088F0
    /* 800EA094 E7AC0030 */  swc1 $f12, 48($sp)
    /* 800EA098 3C014170 */  lui $at, 0x4170
    /* 800EA09C 44819000 */  .word 0x44819000
    /* 800EA0A0 3C014120 */  lui $at, 0x4120
    /* 800EA0A4 44812000 */  .word 0x44812000
    /* 800EA0A8 46120182 */  .word 0x46120182
    /* 800EA0AC C7AC0030 */  lwc1 $f12, 48($sp)
    /* 800EA0B0 E7A40038 */  swc1 $f4, 56($sp)
    /* 800EA0B4 0C0021CC */  jal 0x80008730
    /* 800EA0B8 E7A60034 */  swc1 $f6, 52($sp)
    /* 800EA0BC 3C014170 */  lui $at, 0x4170
    /* 800EA0C0 44815000 */  .word 0x44815000
    /* 800EA0C4 8FA8002C */  lw $t0, 44($sp)
    /* 800EA0C8 44808000 */  .word 0x44808000
    /* 800EA0CC 460A0202 */  .word 0x460A0202
    /* 800EA0D0 3C018015 */  lui $at, 0x8015
    /* 800EA0D4 00085080 */  sll $t2, $t0, 2
    /* 800EA0D8 002A0821 */  addu $at, $at, $t2
    /* 800EA0DC 8FA40040 */  lw $a0, 64($sp)
    /* 800EA0E0 8FA50044 */  lw $a1, 68($sp)
    /* 800EA0E4 8FA60048 */  lw $a2, 72($sp)
    /* 800EA0E8 E7A8003C */  swc1 $f8, 60($sp)
    /* 800EA0EC 27A70034 */  addiu $a3, $sp, 52
    /* 800EA0F0 0C03A354 */  jal 0x800E8D50
    /* 800EA0F4 E4302720 */  swc1 $f16, 10016($at)
    /* 800EA0F8 8FBF0014 */  lw $ra, 20($sp)
    /* 800EA0FC 27BD0040 */  addiu $sp, $sp, 64
    /* 800EA100 03E00008 */  jr $ra
    /* 800EA104 00000000 */  nop
