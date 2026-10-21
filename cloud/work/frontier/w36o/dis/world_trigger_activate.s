== world_trigger_activate @ 0x800EE5DC (116 words)
  800ee5dc: addiu	sp,sp,-56
  800ee5e0: sw	ra,20(sp)
  800ee5e4: lui	a0,0x8003
  800ee5e8: sw	zero,48(sp)
  800ee5ec: addiu	a0,a0,-4872
  800ee5f0: addiu	a1,sp,48
  800ee5f4: jal	0x80007270   <osRecvMesg>
  800ee5f8: li	a2,1
  800ee5fc: lw	t6,48(sp)
  800ee600: li	at,1
  800ee604: lh	v1,0(t6)
  800ee608: beq	v1,at,0x800ee668
  800ee60c: li	at,2
  800ee610: beq	v1,at,0x800ee774
  800ee614: li	at,4
  800ee618: beq	v1,at,0x800ee630
  800ee61c: li	at,2750
  800ee620: beq	v1,at,0x800ee73c
  800ee624: lui	t2,0x8003
  800ee628: b	0x800ee7a0
  800ee62c: lw	ra,20(sp)
  800ee630: jal	0x800205e4   <func_800205E4>
  800ee634: nop
  800ee638: jal	0x80001e58   <apply_display_mode>
  800ee63c: nop
  800ee640: jal	0x80006da0   <osViBlack>
  800ee644: li	a0,1
  800ee648: lui	t7,0x8015
  800ee64c: addiu	t7,t7,9712
  800ee650: jal	0x800a13e8   <check_mpath_save>
  800ee654: sh	zero,0(t7)
  800ee658: lui	at,0x8012
  800ee65c: sb	zero,-5408(at)
  800ee660: b	0x800ee660
  800ee664: nop
  800ee668: jal	0x80007790   <osViGetCurrentFramebuffer>
  800ee66c: nop
  800ee670: jal	0x800077d0   <osViGetNextFramebuffer>
  800ee674: sw	v0,44(sp)
  800ee678: lw	v1,44(sp)
  800ee67c: lui	t8,0x8003
  800ee680: addiu	t8,t8,21617
  800ee684: bne	v0,v1,0x800ee724
  800ee688: nop
  800ee68c: lb	t9,0(t8)
  800ee690: lui	t0,0x8003
  800ee694: addiu	t0,t0,21616
  800ee698: bnez	t9,0x800ee724
  800ee69c: nop
  800ee6a0: lb	t1,0(t0)
  800ee6a4: lui	t2,0x8003
  800ee6a8: addiu	t2,t2,21618
  800ee6ac: bnez	t1,0x800ee724
  800ee6b0: nop
  800ee6b4: lb	t3,0(t2)
  800ee6b8: lui	t4,0x8003
  800ee6bc: addiu	t4,t4,-5912
  800ee6c0: bnez	t3,0x800ee724
  800ee6c4: lui	t6,0x8003
  800ee6c8: lw	t5,636(t4)
  800ee6cc: addiu	t6,t6,-5224
  800ee6d0: lw	t7,0(t6)
  800ee6d4: lui	v1,0x8011
  800ee6d8: addiu	v1,v1,18224
  800ee6dc: beq	t5,t7,0x800ee724
  800ee6e0: nop
  800ee6e4: lw	t8,0(v1)
  800ee6e8: sltiu	at,t8,2
  800ee6ec: beqz	at,0x800ee724
  800ee6f0: lui	at,0x8015
  800ee6f4: jal	0x800eddc0   <gfx_frame_begin>
  800ee6f8: sw	zero,6880(at)
  800ee6fc: lui	t0,0x8003
  800ee700: addiu	t0,t0,21616
  800ee704: li	t9,1
  800ee708: lui	a0,0x8003
  800ee70c: lui	a1,0x8014
  800ee710: sb	t9,0(t0)
  800ee714: addiu	a1,a1,11664
  800ee718: addiu	a0,a0,-4928
  800ee71c: jal	0x800075e0   <osSendMesg>
  800ee720: li	a2,1
  800ee724: lui	v1,0x8015
  800ee728: lw	v1,6880(v1)
  800ee72c: lui	at,0x8015
  800ee730: addiu	t1,v1,1
  800ee734: b	0x800ee79c
  800ee738: sw	t1,6880(at)
  800ee73c: addiu	t2,t2,21618
  800ee740: jal	0x800edacc   <world_collision_response>
  800ee744: sb	zero,0(t2)
  800ee748: lui	v0,0x8016
  800ee74c: lui	v1,0x8011
  800ee750: addiu	v1,v1,18224
  800ee754: addiu	v0,v0,-2259
  800ee758: lb	t3,0(v0)
  800ee75c: lw	t6,0(v1)
  800ee760: xori	t4,t3,0x1
  800ee764: addiu	t5,t6,1
  800ee768: sb	t4,0(v0)
  800ee76c: b	0x800ee79c
  800ee770: sw	t5,0(v1)
  800ee774: lui	t7,0x8011
  800ee778: lw	t7,25036(t7)
  800ee77c: lui	v1,0x8011
  800ee780: lui	at,0x8011
  800ee784: xori	t8,t7,0x1
  800ee788: addiu	v1,v1,18224
  800ee78c: sw	t8,25036(at)
  800ee790: lw	t9,0(v1)
  800ee794: addiu	t0,t9,-1
  800ee798: sw	t0,0(v1)
  800ee79c: lw	ra,20(sp)
  800ee7a0: addiu	sp,sp,56
  800ee7a4: jr	ra
  800ee7a8: nop
