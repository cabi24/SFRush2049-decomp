nonmatching osContStartQuery, 0x7C

glabel osContStartQuery
    /* A330 80009730 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* A334 80009734 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A338 80009738 0C00396C */  jal        __osSiGetAccess
    /* A33C 8000973C AFA40020 */   sw        $a0, 0x20($sp)
    /* A340 80009740 3C0E8003 */  lui        $t6, %hi(__osPfsRequestType)
    /* A344 80009744 91CE7AE0 */  lbu        $t6, %lo(__osPfsRequestType)($t6)
    /* A348 80009748 11C0000B */  beqz       $t6, .L80009778
    /* A34C 8000974C 00000000 */   nop
    /* A350 80009750 0C002596 */  jal        __osContRamReset
    /* A354 80009754 00002025 */   or        $a0, $zero, $zero
    /* A358 80009758 3C058003 */  lui        $a1, %hi(__osSiDmaBuffer)
    /* A35C 8000975C 24A57AA0 */  addiu      $a1, $a1, %lo(__osSiDmaBuffer)
    /* A360 80009760 0C00392C */  jal        __osSiRawStartDma
    /* A364 80009764 24040001 */   addiu     $a0, $zero, 0x1
    /* A368 80009768 8FA40020 */  lw         $a0, 0x20($sp)
    /* A36C 8000976C 00002825 */  or         $a1, $zero, $zero
    /* A370 80009770 0C001C9C */  jal        osRecvMesg
    /* A374 80009774 24060001 */   addiu     $a2, $zero, 0x1
  .L80009778:
    /* A378 80009778 3C058003 */  lui        $a1, %hi(__osSiDmaBuffer)
    /* A37C 8000977C 24A57AA0 */  addiu      $a1, $a1, %lo(__osSiDmaBuffer)
    /* A380 80009780 0C00392C */  jal        __osSiRawStartDma
    /* A384 80009784 00002025 */   or        $a0, $zero, $zero
    /* A388 80009788 3C018003 */  lui        $at, %hi(__osPfsRequestType)
    /* A38C 8000978C AFA2001C */  sw         $v0, 0x1C($sp)
    /* A390 80009790 0C00397D */  jal        __osSiRelAccess
    /* A394 80009794 A0207AE0 */   sb        $zero, %lo(__osPfsRequestType)($at)
    /* A398 80009798 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A39C 8000979C 8FA2001C */  lw         $v0, 0x1C($sp)
    /* A3A0 800097A0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* A3A4 800097A4 03E00008 */  jr         $ra
    /* A3A8 800097A8 00000000 */   nop
endlabel osContStartQuery
