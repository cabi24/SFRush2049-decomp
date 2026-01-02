# Source: F160.s
# Address: 0x8000E620

glabel func_8000E620
    /* F220 8000E620 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* F224 8000E624 AFA40030 */  sw         $a0, 0x30($sp)
    /* F228 8000E628 AFBF0014 */  sw         $ra, 0x14($sp)
    /* F22C 8000E62C AFA50034 */  sw         $a1, 0x34($sp)
    /* F230 8000E630 240E00FA */  addiu      $t6, $zero, 0xFA
    /* F234 8000E634 3C018003 */  lui        $at, %hi(__osSiChannelMask)
    /* F238 8000E638 00A02025 */  or         $a0, $a1, $zero
    /* F23C 8000E63C A02EC4D4 */  sb         $t6, %lo(__osSiChannelMask)($at)
    /* F240 8000E640 0C0039BC */  jal        func_8000E6F0
    /* F244 8000E644 00002825 */   or        $a1, $zero, $zero
    /* F248 8000E648 3C058003 */  lui        $a1, %hi(__osPfsBuffer)
    /* F24C 8000E64C 24A57F60 */  addiu      $a1, $a1, %lo(__osPfsBuffer)
    /* F250 8000E650 0C00392C */  jal        func_8000E4B0
    /* F254 8000E654 24040001 */   addiu     $a0, $zero, 0x1
    /* F258 8000E658 8FA40030 */  lw         $a0, 0x30($sp)
    /* F25C 8000E65C 27A50028 */  addiu      $a1, $sp, 0x28
    /* F260 8000E660 0C001C9C */  jal        func_80007270
    /* F264 8000E664 24060001 */   addiu     $a2, $zero, 0x1
    /* F268 8000E668 3C058003 */  lui        $a1, %hi(__osPfsBuffer)
    /* F26C 8000E66C 24A57F60 */  addiu      $a1, $a1, %lo(__osPfsBuffer)
    /* F270 8000E670 0C00392C */  jal        func_8000E4B0
    /* F274 8000E674 00002025 */   or        $a0, $zero, $zero
    /* F278 8000E678 AFA2002C */  sw         $v0, 0x2C($sp)
    /* F27C 8000E67C 8FA40030 */  lw         $a0, 0x30($sp)
    /* F280 8000E680 27A50028 */  addiu      $a1, $sp, 0x28
    /* F284 8000E684 0C001C9C */  jal        func_80007270
    /* F288 8000E688 24060001 */   addiu     $a2, $zero, 0x1
    /* F28C 8000E68C 8FA40034 */  lw         $a0, 0x34($sp)
    /* F290 8000E690 0C0039ED */  jal        func_8000E7B4
    /* F294 8000E694 27A50024 */   addiu     $a1, $sp, 0x24
    /* F298 8000E698 93A20026 */  lbu        $v0, 0x26($sp)
    /* F29C 8000E69C 93B80027 */  lbu        $t8, 0x27($sp)
    /* F2A0 8000E6A0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F2A4 8000E6A4 30430001 */  andi       $v1, $v0, 0x1
    /* F2A8 8000E6A8 10600005 */  beqz       $v1, .L8000E6C0
    /* F2AC 8000E6AC 304F0002 */   andi      $t7, $v0, 0x2
    /* F2B0 8000E6B0 11E00003 */  beqz       $t7, .L8000E6C0
    /* F2B4 8000E6B4 00000000 */   nop
    /* F2B8 8000E6B8 1000000B */  b          .L8000E6E8
    /* F2BC 8000E6BC 24020002 */   addiu     $v0, $zero, 0x2
  .L8000E6C0:
    /* F2C0 8000E6C0 17000003 */  bnez       $t8, .L8000E6D0
    /* F2C4 8000E6C4 00000000 */   nop
    /* F2C8 8000E6C8 14600003 */  bnez       $v1, .L8000E6D8
    /* F2CC 8000E6CC 30590004 */   andi      $t9, $v0, 0x4
  .L8000E6D0:
    /* F2D0 8000E6D0 10000005 */  b          .L8000E6E8
    /* F2D4 8000E6D4 24020001 */   addiu     $v0, $zero, 0x1
  .L8000E6D8:
    /* F2D8 8000E6D8 13200003 */  beqz       $t9, .L8000E6E8
    /* F2DC 8000E6DC 8FA2002C */   lw        $v0, 0x2C($sp)
    /* F2E0 8000E6E0 10000001 */  b          .L8000E6E8
    /* F2E4 8000E6E4 24020004 */   addiu     $v0, $zero, 0x4
  .L8000E6E8:
    /* F2E8 8000E6E8 03E00008 */  jr         $ra
    /* F2EC 8000E6EC 27BD0030 */   addiu     $sp, $sp, 0x30