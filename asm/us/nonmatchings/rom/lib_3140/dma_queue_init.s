nonmatching dma_queue_init, 0x58

glabel dma_queue_init
    /* 3140 80002540 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 3144 80002544 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 3148 80002548 240E0001 */  addiu      $t6, $zero, 0x1
    /* 314C 8000254C 3C018003 */  lui        $at, %hi(gDmaInitialized)
    /* 3150 80002550 A02EB030 */  sb         $t6, %lo(gDmaInitialized)($at)
    /* 3154 80002554 3C048003 */  lui        $a0, %hi(gDmaMessageQueue)
    /* 3158 80002558 3C058003 */  lui        $a1, %hi(gDmaMessageBuffer)
    /* 315C 8000255C 24A5F1A8 */  addiu      $a1, $a1, %lo(gDmaMessageBuffer)
    /* 3160 80002560 2484F190 */  addiu      $a0, $a0, %lo(gDmaMessageQueue)
    /* 3164 80002564 0C001A80 */  jal        osCreateMesgQueue
    /* 3168 80002568 24060001 */   addiu     $a2, $zero, 0x1
    /* 316C 8000256C 3C048003 */  lui        $a0, %hi(gDmaMessageQueue)
    /* 3170 80002570 2484F190 */  addiu      $a0, $a0, %lo(gDmaMessageQueue)
    /* 3174 80002574 00002825 */  or         $a1, $zero, $zero
    /* 3178 80002578 0C001D78 */  jal        osJamMesg
    /* 317C 8000257C 00003025 */   or        $a2, $zero, $zero
    /* 3180 80002580 10000001 */  b          .L80002588
    /* 3184 80002584 00000000 */   nop
  .L80002588:
    /* 3188 80002588 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 318C 8000258C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 3190 80002590 03E00008 */  jr         $ra
    /* 3194 80002594 00000000 */   nop
endlabel dma_queue_init
