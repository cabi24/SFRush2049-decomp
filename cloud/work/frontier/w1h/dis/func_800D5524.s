== func_800D5524 @ 0x800D5524 (157 words)
  800d5524: addiu	sp,sp,-56
  800d5528: sw	ra,52(sp)
  800d552c: sw	s6,48(sp)
  800d5530: sw	s5,44(sp)
  800d5534: sw	s4,40(sp)
  800d5538: sw	s3,36(sp)
  800d553c: sw	s2,32(sp)
  800d5540: sw	s1,28(sp)
  800d5544: sw	s0,24(sp)
  800d5548: lh	s6,1990(a0)
  800d554c: lui	t6,0x8011
  800d5550: addiu	t6,t6,-60
  800d5554: addu	v0,s6,t6
  800d5558: lb	t7,0(v0)
  800d555c: andi	t8,s6,0x1
  800d5560: move	s3,a0
  800d5564: beqz	t7,0x800d5770
  800d5568: sltiu	t9,t8,1
  800d556c: sll	t0,s6,0x2
  800d5570: addu	t0,t0,s6
  800d5574: sll	t0,t0,0x2
  800d5578: lui	at,0x8011
  800d557c: addu	t0,t0,s6
  800d5580: lui	t1,0x8014
  800d5584: sb	zero,0(v0)
  800d5588: addu	at,at,s6
  800d558c: addiu	t1,t1,1056
  800d5590: sll	t0,t0,0x2
  800d5594: sb	t9,-52(at)
  800d5598: addu	s2,t0,t1
  800d559c: move	s1,zero
  800d55a0: li	s5,-1
  800d55a4: li	s4,2
  800d55a8: lw	t2,4(s2)
  800d55ac: beql	s5,t2,0x800d55e8
  800d55b0: addiu	s1,s1,20
  800d55b4: lb	t3,1996(s3)
  800d55b8: addiu	s0,s2,4
  800d55bc: beq	s4,t3,0x800d55d4
  800d55c0: nop
  800d55c4: jal	0x800bf024   <results_screen_update>
  800d55c8: lw	a0,0(s0)
  800d55cc: b	0x800d55dc
  800d55d0: nop
  800d55d4: jal	0x80091c04   <scheduler_recv>
  800d55d8: lw	a0,0(s0)
  800d55dc: jal	0x800d54bc   <player_conditional_call>
  800d55e0: move	a0,s0
  800d55e4: addiu	s1,s1,20
  800d55e8: li	at,40
  800d55ec: bne	s1,at,0x800d55a8
  800d55f0: addiu	s2,s2,20
  800d55f4: sll	t4,s6,0x2
  800d55f8: addu	t4,t4,s6
  800d55fc: sll	t4,t4,0x2
  800d5600: addu	t4,t4,s6
  800d5604: lui	t5,0x8014
  800d5608: addiu	t5,t5,1056
  800d560c: sll	t4,t4,0x2
  800d5610: addu	v0,t4,t5
  800d5614: lw	t6,64(v0)
  800d5618: beql	s5,t6,0x800d5658
  800d561c: lb	t8,1996(s3)
  800d5620: lb	t7,1996(s3)
  800d5624: addiu	s0,v0,64
  800d5628: beq	s4,t7,0x800d5644
  800d562c: nop
  800d5630: addiu	s0,v0,64
  800d5634: jal	0x800bf024   <results_screen_update>
  800d5638: lw	a0,0(s0)
  800d563c: b	0x800d564c
  800d5640: nop
  800d5644: jal	0x80091c04   <scheduler_recv>
  800d5648: lw	a0,0(s0)
  800d564c: jal	0x800d54bc   <player_conditional_call>
  800d5650: move	a0,s0
  800d5654: lb	t8,1996(s3)
  800d5658: sll	t1,s6,0x2
  800d565c: addu	t1,t1,s6
  800d5660: beq	s4,t8,0x800d5690
  800d5664: sll	t1,t1,0x2
  800d5668: sll	t9,s6,0x2
  800d566c: addu	t9,t9,s6
  800d5670: lui	t0,0x8014
  800d5674: addiu	t0,t0,1600
  800d5678: sll	t9,t9,0x2
  800d567c: addu	s0,t9,t0
  800d5680: jal	0x800bf024   <results_screen_update>
  800d5684: lw	a0,0(s0)
  800d5688: b	0x800d56a4
  800d568c: nop
  800d5690: lui	t2,0x8014
  800d5694: addiu	t2,t2,1600
  800d5698: addu	s0,t1,t2
  800d569c: jal	0x80091c04   <scheduler_recv>
  800d56a0: lw	a0,0(s0)
  800d56a4: jal	0x800d54bc   <player_conditional_call>
  800d56a8: move	a0,s0
  800d56ac: lb	t3,1996(s3)
  800d56b0: sll	t4,s6,0x4
  800d56b4: subu	t4,t4,s6
  800d56b8: bne	s4,t3,0x800d5770
  800d56bc: sll	t4,t4,0x2
  800d56c0: lui	t5,0x8014
  800d56c4: addiu	t5,t5,1728
  800d56c8: addu	s0,t4,t5
  800d56cc: jal	0x80091c04   <scheduler_recv>
  800d56d0: lw	a0,0(s0)
  800d56d4: jal	0x800d54bc   <player_conditional_call>
  800d56d8: move	a0,s0
  800d56dc: addiu	s1,s0,20
  800d56e0: jal	0x80091c04   <scheduler_recv>
  800d56e4: lw	a0,0(s1)
  800d56e8: jal	0x800d54bc   <player_conditional_call>
  800d56ec: move	a0,s1
  800d56f0: addiu	s1,s0,40
  800d56f4: jal	0x80091c04   <scheduler_recv>
  800d56f8: lw	a0,0(s1)
  800d56fc: jal	0x800d54bc   <player_conditional_call>
  800d5700: move	a0,s1
  800d5704: lui	t6,0x8014
  800d5708: addiu	t6,t6,2784
  800d570c: sll	s0,s6,0x2
  800d5710: addu	s1,s0,t6
  800d5714: jal	0x80091c04   <scheduler_recv>
  800d5718: lw	a0,0(s1)
  800d571c: sll	t7,s6,0x1
  800d5720: lui	at,0x8014
  800d5724: sw	s5,0(s1)
  800d5728: addu	at,at,t7
  800d572c: mtc1	zero,$f4
  800d5730: sh	zero,2568(at)
  800d5734: lui	at,0x8014
  800d5738: lui	t8,0x8014
  800d573c: addu	at,at,s0
  800d5740: addiu	t8,t8,2016
  800d5744: addu	s2,s0,t8
  800d5748: swc1	$f4,2832(at)
  800d574c: jal	0x80091c04   <scheduler_recv>
  800d5750: lw	a0,0(s2)
  800d5754: lui	t9,0x8014
  800d5758: addiu	t9,t9,1984
  800d575c: addu	s1,s0,t9
  800d5760: sw	s5,0(s2)
  800d5764: jal	0x80091c04   <scheduler_recv>
  800d5768: lw	a0,0(s1)
  800d576c: sw	s5,0(s1)
  800d5770: lw	ra,52(sp)
  800d5774: lw	s0,24(sp)
  800d5778: lw	s1,28(sp)
  800d577c: lw	s2,32(sp)
  800d5780: lw	s3,36(sp)
  800d5784: lw	s4,40(sp)
  800d5788: lw	s5,44(sp)
  800d578c: lw	s6,48(sp)
  800d5790: jr	ra
  800d5794: addiu	sp,sp,56
