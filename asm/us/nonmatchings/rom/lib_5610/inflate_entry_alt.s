nonmatching inflate_entry_alt, 0x80

glabel inflate_entry_alt
    /* 757C 8000697C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 7580 80006980 AFA60028 */  sw         $a2, 0x28($sp)
    /* 7584 80006984 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 7588 80006988 3C028003 */  lui        $v0, %hi(gInflateInPtr)
    /* 758C 8000698C 3C018003 */  lui        $at, %hi(gInflateOutPtr)
    /* 7590 80006990 244254B0 */  addiu      $v0, $v0, %lo(gInflateInPtr)
    /* 7594 80006994 00803825 */  or         $a3, $a0, $zero
    /* 7598 80006998 AC2E54B8 */  sw         $t6, %lo(gInflateOutPtr)($at)
    /* 759C 8000699C AC470000 */  sw         $a3, 0x0($v0)
    /* 75A0 800069A0 8C4F0000 */  lw         $t7, 0x0($v0)
    /* 75A4 800069A4 3C018003 */  lui        $at, %hi(gInflateInEnd)
    /* 75A8 800069A8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 75AC 800069AC 01E5C021 */  addu       $t8, $t7, $a1
    /* 75B0 800069B0 AC3854B4 */  sw         $t8, %lo(gInflateInEnd)($at)
    /* 75B4 800069B4 24052EE0 */  addiu      $a1, $zero, 0x2EE0
    /* 75B8 800069B8 0C0330F0 */  jal        sound_play_menu
    /* 75BC 800069BC 24040000 */   addiu     $a0, $zero, 0x0
    /* 75C0 800069C0 AFA2001C */  sw         $v0, 0x1C($sp)
    /* 75C4 800069C4 00402025 */  or         $a0, $v0, $zero
    /* 75C8 800069C8 0C001354 */  jal        inflate_flush_window
    /* 75CC 800069CC 24052EE0 */   addiu     $a1, $zero, 0x2EE0
    /* 75D0 800069D0 0C00199E */  jal        inflate_loop
    /* 75D4 800069D4 00000000 */   nop
    /* 75D8 800069D8 0C025835 */  jal        dma_queue_sync
    /* 75DC 800069DC 8FA4001C */   lw        $a0, 0x1C($sp)
    /* 75E0 800069E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 75E4 800069E4 3C198003 */  lui        $t9, %hi(gInflateOutPtr)
    /* 75E8 800069E8 8F3954B8 */  lw         $t9, %lo(gInflateOutPtr)($t9)
    /* 75EC 800069EC 8FA80028 */  lw         $t0, 0x28($sp)
    /* 75F0 800069F0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 75F4 800069F4 03E00008 */  jr         $ra
    /* 75F8 800069F8 03281023 */   subu      $v0, $t9, $t0
endlabel inflate_entry_alt
    /* 75FC 800069FC 00000000 */  nop
