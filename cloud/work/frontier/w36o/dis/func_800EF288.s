== func_800EF288 @ 0x800EF288 (202 words)
  800ef288: addiu	sp,sp,-184
  800ef28c: sw	ra,60(sp)
  800ef290: mtc1	zero,$f12
  800ef294: sw	s8,56(sp)
  800ef298: sw	s7,52(sp)
  800ef29c: sw	s6,48(sp)
  800ef2a0: sw	s5,44(sp)
  800ef2a4: sw	s4,40(sp)
  800ef2a8: sw	s3,36(sp)
  800ef2ac: sw	s2,32(sp)
  800ef2b0: sw	s1,28(sp)
  800ef2b4: sw	s0,24(sp)
  800ef2b8: jal	0x800b65b4   <render_helper>
  800ef2bc: sw	a0,184(sp)
  800ef2c0: lui	v1,0x8015
  800ef2c4: lh	v1,6864(v1)
  800ef2c8: li	at,1
  800ef2cc: lui	s4,0x8014
  800ef2d0: bne	v1,at,0x800ef30c
  800ef2d4: addiu	s4,s4,25040
  800ef2d8: move	a0,s4
  800ef2dc: move	a1,zero
  800ef2e0: jal	0x80007270   <osRecvMesg>
  800ef2e4: li	a2,1
  800ef2e8: jal	0x800b4200   <slot_state_setup>
  800ef2ec: li	s2,10
  800ef2f0: move	s0,v0
  800ef2f4: move	a0,s4
  800ef2f8: move	a1,zero
  800ef2fc: jal	0x800075e0   <osSendMesg>
  800ef300: move	a2,zero
  800ef304: b	0x800ef384
  800ef308: nop
  800ef30c: li	at,2
  800ef310: bne	v1,at,0x800ef354
  800ef314: lui	s4,0x8014
  800ef318: lui	s4,0x8014
  800ef31c: addiu	s4,s4,25040
  800ef320: move	a0,s4
  800ef324: move	a1,zero
  800ef328: jal	0x80007270   <osRecvMesg>
  800ef32c: li	a2,1
  800ef330: jal	0x800b4200   <slot_state_setup>
  800ef334: li	s2,11
  800ef338: move	s0,v0
  800ef33c: move	a0,s4
  800ef340: move	a1,zero
  800ef344: jal	0x800075e0   <osSendMesg>
  800ef348: move	a2,zero
  800ef34c: b	0x800ef384
  800ef350: nop
  800ef354: addiu	s4,s4,25040
  800ef358: move	a0,s4
  800ef35c: move	a1,zero
  800ef360: jal	0x80007270   <osRecvMesg>
  800ef364: li	a2,1
  800ef368: jal	0x800b4200   <slot_state_setup>
  800ef36c: li	s2,12
  800ef370: move	s0,v0
  800ef374: move	a0,s4
  800ef378: move	a1,zero
  800ef37c: jal	0x800075e0   <osSendMesg>
  800ef380: move	a2,zero
  800ef384: lui	v1,0x8015
  800ef388: lh	v1,6864(v1)
  800ef38c: lui	s5,0x8015
  800ef390: addiu	s5,s5,-23984
  800ef394: blez	v1,0x800ef56c
  800ef398: move	s4,zero
  800ef39c: lui	s7,0x8011
  800ef3a0: lui	s6,0x8018
  800ef3a4: addiu	s6,s6,-23328
  800ef3a8: addiu	s7,s7,23528
  800ef3ac: addiu	s8,sp,180
  800ef3b0: slt	at,s4,v1
  800ef3b4: beqz	at,0x800ef55c
  800ef3b8: lb	s3,1840(s5)
  800ef3bc: lui	t6,0x8016
  800ef3c0: lb	t6,5012(t6)
  800ef3c4: beqzl	t6,0x800ef560
  800ef3c8: addiu	s4,s4,1
  800ef3cc: lb	t7,10(s5)
  800ef3d0: bnezl	t7,0x800ef560
  800ef3d4: addiu	s4,s4,1
  800ef3d8: lh	t8,1990(s5)
  800ef3dc: lui	t6,0x8015
  800ef3e0: li	at,1
  800ef3e4: sll	t9,t8,0x4
  800ef3e8: subu	t9,t9,t8
  800ef3ec: sll	t9,t9,0x3
  800ef3f0: subu	t9,t9,t8
  800ef3f4: sll	t9,t9,0x3
  800ef3f8: addu	t6,t6,t9
  800ef3fc: lb	t6,10503(t6)
  800ef400: sll	t7,v1,0x5
  800ef404: addu	t8,s7,t7
  800ef408: beq	t6,at,0x800ef55c
  800ef40c: sll	s2,s4,0x3
  800ef410: addu	v0,t8,s2
  800ef414: lw	s0,-32(v0)
  800ef418: lh	s1,-26(v0)
  800ef41c: move	a0,zero
  800ef420: addiu	s0,s0,-4
  800ef424: sll	t9,s0,0x10
  800ef428: jal	0x800b74a0   <dispatch_handler>
  800ef42c: sra	s0,t9,0x10
  800ef430: lw	t7,4(s6)
  800ef434: li	a1,-1
  800ef438: jal	0x800b3fa4   <object_manager_update,object_utility>
  800ef43c: lw	a0,816(t7)
  800ef440: addiu	t6,s0,1
  800ef444: subu	t8,t6,v0
  800ef448: sll	a0,t8,0x10
  800ef44c: lw	t8,4(s6)
  800ef450: addiu	a1,s1,1
  800ef454: sll	t6,a1,0x10
  800ef458: sra	t9,a0,0x10
  800ef45c: move	a0,t9
  800ef460: sra	a1,t6,0x10
  800ef464: jal	0x800b71d4   <state_utility>
  800ef468: lw	a2,816(t8)
  800ef46c: jal	0x800b74a0   <dispatch_handler>
  800ef470: li	a0,1
  800ef474: lw	t9,4(s6)
  800ef478: li	a1,-1
  800ef47c: jal	0x800b3fa4   <object_manager_update,object_utility>
  800ef480: lw	a0,816(t9)
  800ef484: subu	t7,s0,v0
  800ef488: lw	t6,4(s6)
  800ef48c: sll	a0,t7,0x10
  800ef490: sll	a1,s1,0x10
  800ef494: sra	t9,a1,0x10
  800ef498: sra	t8,a0,0x10
  800ef49c: move	a0,t8
  800ef4a0: move	a1,t9
  800ef4a4: jal	0x800b71d4   <state_utility>
  800ef4a8: lw	a2,816(t6)
  800ef4ac: lui	t7,0x8015
  800ef4b0: lh	t7,6864(t7)
  800ef4b4: li	at,-1
  800ef4b8: move	a0,zero
  800ef4bc: sll	t8,t7,0x5
  800ef4c0: addu	t9,s7,t8
  800ef4c4: addu	v0,t9,s2
  800ef4c8: lw	s0,-32(v0)
  800ef4cc: lh	s1,-26(v0)
  800ef4d0: li	t8,82
  800ef4d4: addiu	s0,s0,4
  800ef4d8: sll	t6,s0,0x10
  800ef4dc: bne	s3,at,0x800ef4ec
  800ef4e0: sra	s0,t6,0x10
  800ef4e4: b	0x800ef504
  800ef4e8: sb	t8,180(sp)
  800ef4ec: bnez	s3,0x800ef500
  800ef4f0: addiu	t6,s3,48
  800ef4f4: li	t9,78
  800ef4f8: b	0x800ef504
  800ef4fc: sb	t9,180(sp)
  800ef500: sb	t6,180(sp)
  800ef504: jal	0x800b74a0   <dispatch_handler>
  800ef508: sb	zero,181(sp)
  800ef50c: addiu	a0,s0,1
  800ef510: addiu	a1,s1,1
  800ef514: sll	t9,a1,0x10
  800ef518: sll	t7,a0,0x10
  800ef51c: sra	a0,t7,0x10
  800ef520: sra	a1,t9,0x10
  800ef524: jal	0x800b71d4   <state_utility>
  800ef528: move	a2,s8
  800ef52c: jal	0x800b74a0   <dispatch_handler>
  800ef530: li	a0,14
  800ef534: sll	a0,s0,0x10
  800ef538: sll	a1,s1,0x10
  800ef53c: sra	t8,a1,0x10
  800ef540: sra	t7,a0,0x10
  800ef544: move	a0,t7
  800ef548: move	a1,t8
  800ef54c: jal	0x800b71d4   <state_utility>
  800ef550: move	a2,s8
  800ef554: lui	v1,0x8015
  800ef558: lh	v1,6864(v1)
  800ef55c: addiu	s4,s4,1
  800ef560: slt	at,s4,v1
  800ef564: bnez	at,0x800ef3b0
  800ef568: addiu	s5,s5,2056
  800ef56c: lui	at,0xbf80
  800ef570: mtc1	at,$f12
  800ef574: jal	0x800b65b4   <render_helper>
  800ef578: nop
  800ef57c: lw	ra,60(sp)
  800ef580: lw	s0,24(sp)
  800ef584: lw	s1,28(sp)
  800ef588: lw	s2,32(sp)
  800ef58c: lw	s3,36(sp)
  800ef590: lw	s4,40(sp)
  800ef594: lw	s5,44(sp)
  800ef598: lw	s6,48(sp)
  800ef59c: lw	s7,52(sp)
  800ef5a0: lw	s8,56(sp)
  800ef5a4: addiu	sp,sp,184
  800ef5a8: jr	ra
  800ef5ac: li	v0,1
