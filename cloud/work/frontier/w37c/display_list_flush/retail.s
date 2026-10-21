== display_list_flush @ 0x800FB2C8 (589 words)
  800fb2c8: addiu	sp,sp,-256
  800fb2cc: sw	ra,76(sp)
  800fb2d0: sw	s8,72(sp)
  800fb2d4: sw	s7,68(sp)
  800fb2d8: sw	s6,64(sp)
  800fb2dc: sw	s5,60(sp)
  800fb2e0: sw	s4,56(sp)
  800fb2e4: sw	s3,52(sp)
  800fb2e8: sw	s2,48(sp)
  800fb2ec: sw	s1,44(sp)
  800fb2f0: sw	s0,40(sp)
  800fb2f4: beqz	a1,0x800fb30c
  800fb2f8: sw	a0,256(sp)
  800fb2fc: lui	at,0x8011
  800fb300: sb	zero,18164(at)
  800fb304: b	0x800fbbcc
  800fb308: move	v0,zero
  800fb30c: lui	t6,0x8011
  800fb310: lb	t6,18164(t6)
  800fb314: li	t7,1
  800fb318: lui	at,0x8011
  800fb31c: bnez	t6,0x800fb764
  800fb320: nop
  800fb324: jal	0x800c9be0   <init_state_begin>
  800fb328: sb	t7,18164(at)
  800fb32c: lui	v0,0x8011
  800fb330: lw	v0,29876(v0)
  800fb334: lui	at,0x200
  800fb338: andi	a0,v0,0x8
  800fb33c: sltiu	t8,a0,1
  800fb340: beqz	t8,0x800fb354
  800fb344: move	a0,t8
  800fb348: and	a0,v0,at
  800fb34c: sltiu	t9,a0,1
  800fb350: move	a0,t9
  800fb354: jal	0x800fb234   <vsync_wait>
  800fb358: nop
  800fb35c: lui	v0,0x8011
  800fb360: lw	v0,29876(v0)
  800fb364: lui	t1,0x8015
  800fb368: lui	at,0x42b4
  800fb36c: sll	t6,v0,0x6
  800fb370: bgez	t6,0x800fb654
  800fb374: andi	t8,v0,0x8
  800fb378: lui	t2,0x8015
  800fb37c: addiu	t2,t2,10060
  800fb380: sh	zero,0(t2)
  800fb384: lh	v1,0(t2)
  800fb388: addiu	t1,t1,10088
  800fb38c: lui	t7,0x8015
  800fb390: addiu	t7,t7,16338
  800fb394: sh	v1,0(t1)
  800fb398: lui	t8,0x8015
  800fb39c: sh	v1,0(t7)
  800fb3a0: lh	t8,-24312(t8)
  800fb3a4: lui	s0,0x8015
  800fb3a8: addiu	s0,s0,-24296
  800fb3ac: blez	t8,0x800fb4a8
  800fb3b0: move	s1,zero
  800fb3b4: lbu	v1,0(s0)
  800fb3b8: lui	t6,0x8015
  800fb3bc: addiu	t6,t6,16008
  800fb3c0: sll	t9,v1,0x3
  800fb3c4: addu	t0,t9,t6
  800fb3c8: lbu	t7,7(t0)
  800fb3cc: sll	t9,v1,0x4
  800fb3d0: subu	t9,t9,v1
  800fb3d4: addiu	t8,t7,-6
  800fb3d8: sb	t8,7(t0)
  800fb3dc: sll	t9,t9,0x3
  800fb3e0: sll	t7,v1,0x8
  800fb3e4: lui	t6,0x8015
  800fb3e8: lui	t8,0x8015
  800fb3ec: subu	t9,t9,v1
  800fb3f0: addu	t7,t7,v1
  800fb3f4: sll	t9,t9,0x3
  800fb3f8: addiu	t6,t6,10264
  800fb3fc: sll	t7,t7,0x3
  800fb400: addiu	t8,t8,-23984
  800fb404: addu	a3,t9,t6
  800fb408: addu	v0,t7,t8
  800fb40c: li	a2,1
  800fb410: sll	a0,v1,0x10
  800fb414: sra	t9,a0,0x10
  800fb418: sb	zero,857(a3)
  800fb41c: sb	zero,239(a3)
  800fb420: sw	zero,896(a3)
  800fb424: sb	a2,1996(v0)
  800fb428: sh	a2,1994(v0)
  800fb42c: sb	a2,10(v0)
  800fb430: sw	zero,2004(v0)
  800fb434: sw	zero,232(a3)
  800fb438: move	a0,t9
  800fb43c: jal	0x800b200c   <music_tempo_set>
  800fb440: lbu	a1,8(v0)
  800fb444: lui	t1,0x8015
  800fb448: addiu	t1,t1,10088
  800fb44c: lh	v1,0(t1)
  800fb450: lui	at,0x8015
  800fb454: lui	t2,0x8015
  800fb458: sll	t6,v1,0x1
  800fb45c: addu	at,at,t6
  800fb460: addiu	t2,t2,10060
  800fb464: sh	s1,10200(at)
  800fb468: lh	a0,0(t2)
  800fb46c: lui	at,0x8015
  800fb470: addiu	t7,v1,1
  800fb474: sll	t8,a0,0x1
  800fb478: sh	t7,0(t1)
  800fb47c: addu	at,at,t8
  800fb480: sh	s1,10248(at)
  800fb484: addiu	t9,a0,1
  800fb488: lui	t6,0x8015
  800fb48c: sh	t9,0(t2)
  800fb490: lh	t6,-24312(t6)
  800fb494: addiu	s1,s1,1
  800fb498: addiu	s0,s0,76
  800fb49c: slt	at,s1,t6
  800fb4a0: bnezl	at,0x800fb3b8
  800fb4a4: lbu	v1,0(s0)
  800fb4a8: lui	t7,0x8015
  800fb4ac: lb	t7,10052(t7)
  800fb4b0: sll	t8,s1,0x4
  800fb4b4: subu	t8,t8,s1
  800fb4b8: slt	at,s1,t7
  800fb4bc: beqz	at,0x800fb584
  800fb4c0: sll	t8,t8,0x3
  800fb4c4: sll	t6,s1,0x8
  800fb4c8: addu	t6,t6,s1
  800fb4cc: subu	t8,t8,s1
  800fb4d0: lui	t9,0x8015
  800fb4d4: lui	t7,0x8015
  800fb4d8: addiu	t7,t7,-23984
  800fb4dc: addiu	t9,t9,10264
  800fb4e0: sll	t8,t8,0x3
  800fb4e4: sll	t6,t6,0x3
  800fb4e8: addu	s0,t6,t7
  800fb4ec: addu	v0,t8,t9
  800fb4f0: sll	a0,s1,0x10
  800fb4f4: sra	t8,a0,0x10
  800fb4f8: sb	zero,857(v0)
  800fb4fc: sb	zero,239(v0)
  800fb500: sw	zero,232(v0)
  800fb504: sw	zero,2004(s0)
  800fb508: move	a0,t8
  800fb50c: lbu	a1,8(s0)
  800fb510: li	a2,1
  800fb514: jal	0x800b200c   <music_tempo_set>
  800fb518: sw	v0,84(sp)
  800fb51c: lui	t1,0x8015
  800fb520: addiu	t1,t1,10088
  800fb524: lh	v1,0(t1)
  800fb528: lui	at,0x8015
  800fb52c: lui	t2,0x8015
  800fb530: sll	t9,v1,0x1
  800fb534: addu	at,at,t9
  800fb538: addiu	t2,t2,10060
  800fb53c: sh	s1,10200(at)
  800fb540: lh	a0,0(t2)
  800fb544: lui	at,0x8015
  800fb548: addiu	t6,v1,1
  800fb54c: sll	t7,a0,0x1
  800fb550: sh	t6,0(t1)
  800fb554: addu	at,at,t7
  800fb558: sh	s1,10248(at)
  800fb55c: addiu	t8,a0,1
  800fb560: lui	t9,0x8015
  800fb564: sh	t8,0(t2)
  800fb568: lb	t9,10052(t9)
  800fb56c: lw	v0,84(sp)
  800fb570: addiu	s1,s1,1
  800fb574: slt	at,s1,t9
  800fb578: addiu	s0,s0,2056
  800fb57c: bnez	at,0x800fb4f0
  800fb580: addiu	v0,v0,952
  800fb584: lh	t6,0(t2)
  800fb588: lui	v0,0x8015
  800fb58c: addiu	v0,v0,16004
  800fb590: sh	t6,0(v0)
  800fb594: lh	t7,0(v0)
  800fb598: lui	at,0x8015
  800fb59c: li	t8,-1
  800fb5a0: sh	t7,16136(at)
  800fb5a4: lui	at,0x8015
  800fb5a8: sh	t8,16164(at)
  800fb5ac: lui	at,0x8015
  800fb5b0: sh	zero,16192(at)
  800fb5b4: move	s1,zero
  800fb5b8: jal	0x800faf6c   <init_state_continue>
  800fb5bc: sw	zero,252(sp)
  800fb5c0: lui	at,0x8015
  800fb5c4: lui	t9,0x8015
  800fb5c8: lh	t9,10036(t9)
  800fb5cc: sb	zero,10008(at)
  800fb5d0: lui	at,0x8015
  800fb5d4: lw	s1,252(sp)
  800fb5d8: jal	0x800faee4   <func_800FAEE4>
  800fb5dc: sb	t9,8212(at)
  800fb5e0: lui	at,0x8014
  800fb5e4: sb	zero,-309(at)
  800fb5e8: lui	at,0x8014
  800fb5ec: sb	zero,9881(at)
  800fb5f0: lui	at,0x8012
  800fb5f4: lui	t6,0x8015
  800fb5f8: lwc1	$f4,18416(at)
  800fb5fc: lb	t6,10052(t6)
  800fb600: lui	at,0x8014
  800fb604: lui	s0,0x8015
  800fb608: blez	t6,0x800fb63c
  800fb60c: swc1	$f4,10084(at)
  800fb610: addiu	s0,s0,-23984
  800fb614: move	a0,zero
  800fb618: jal	0x800e543c   <func_800E543C>
  800fb61c: lh	a1,1990(s0)
  800fb620: lui	t7,0x8015
  800fb624: lb	t7,10052(t7)
  800fb628: addiu	s1,s1,1
  800fb62c: addiu	s0,s0,2056
  800fb630: slt	at,s1,t7
  800fb634: bnezl	at,0x800fb618
  800fb638: move	a0,zero
  800fb63c: jal	0x800f0100   <skid_mark_render>
  800fb640: nop
  800fb644: lui	at,0x8011
  800fb648: sb	zero,18164(at)
  800fb64c: b	0x800fbbcc
  800fb650: li	v0,1
  800fb654: mtc1	at,$f6
  800fb658: lui	at,0x8015
  800fb65c: bnez	t8,0x800fb6b4
  800fb660: swc1	$f6,16776(at)
  800fb664: lui	a0,0x8014
  800fb668: addiu	a0,a0,10024
  800fb66c: move	a1,zero
  800fb670: jal	0x80007270   <osRecvMesg>
  800fb674: li	a2,1
  800fb678: jal	0x80091b00   <func_80091B00>
  800fb67c: nop
  800fb680: li	t9,1
  800fb684: lui	a0,0x8014
  800fb688: addiu	a0,a0,10024
  800fb68c: sb	t9,2(v0)
  800fb690: move	s0,v0
  800fb694: move	a1,zero
  800fb698: jal	0x800075e0   <osSendMesg>
  800fb69c: move	a2,zero
  800fb6a0: lui	a0,0x8014
  800fb6a4: addiu	a0,a0,10152
  800fb6a8: move	a1,s0
  800fb6ac: jal	0x800075e0   <osSendMesg>
  800fb6b0: move	a2,zero
  800fb6b4: lui	s0,0x8011
  800fb6b8: addiu	s0,s0,18000
  800fb6bc: lb	t6,0(s0)
  800fb6c0: lui	at,0x8015
  800fb6c4: li	t7,-1
  800fb6c8: beqz	t6,0x800fb734
  800fb6cc: lui	t9,0x8011
  800fb6d0: sw	zero,27024(at)
  800fb6d4: lui	at,0x8015
  800fb6d8: sw	zero,27872(at)
  800fb6dc: lui	at,0x8016
  800fb6e0: sw	zero,5036(at)
  800fb6e4: lui	at,0x8015
  800fb6e8: sw	zero,27564(at)
  800fb6ec: lui	at,0x8016
  800fb6f0: sw	zero,5044(at)
  800fb6f4: lui	at,0x8016
  800fb6f8: jal	0x800a5a40   <func_800A5A40>
  800fb6fc: sh	t7,-19884(at)
  800fb700: lui	at,0x3f80
  800fb704: mtc1	at,$f8
  800fb708: lui	at,0x8016
  800fb70c: lui	t8,0x8012
  800fb710: swc1	$f8,5048(at)
  800fb714: lui	at,0x8014
  800fb718: sh	zero,1560(at)
  800fb71c: lui	at,0x8014
  800fb720: addiu	t8,t8,-5608
  800fb724: jal	0x800a5488   <func_800A5488>
  800fb728: sw	t8,1720(at)
  800fb72c: b	0x800fb74c
  800fb730: nop
  800fb734: lw	t9,29876(t9)
  800fb738: andi	t6,t9,0x8
  800fb73c: bnez	t6,0x800fb74c
  800fb740: nop
  800fb744: jal	0x800a5bb8   <tire_compound_set>
  800fb748: nop
  800fb74c: jal	0x800ec914   <func_800EC914>
  800fb750: nop
  800fb754: jal	0x800f6ab8   <GhostStart>
  800fb758: nop
  800fb75c: jal	0x800f7eb0   <func_800F7EB0>
  800fb760: nop
  800fb764: lui	t7,0x8014
  800fb768: lb	t7,10144(t7)
  800fb76c: lui	s0,0x8011
  800fb770: addiu	s0,s0,18000
  800fb774: slti	at,t7,19
  800fb778: beqz	at,0x800fb790
  800fb77c: lui	t9,0x8011
  800fb780: lui	t8,0x8015
  800fb784: lb	t8,-26740(t8)
  800fb788: lui	at,0x8014
  800fb78c: sb	t8,10144(at)
  800fb790: lw	t9,29876(t9)
  800fb794: andi	t6,t9,0x8
  800fb798: bnez	t6,0x800fb7ec
  800fb79c: lui	at,0x42b4
  800fb7a0: mtc1	at,$f10
  800fb7a4: lui	at,0x8015
  800fb7a8: jal	0x800a5a40   <func_800A5A40>
  800fb7ac: swc1	$f10,16776(at)
  800fb7b0: lui	t7,0x8015
  800fb7b4: lw	t7,-24304(t7)
  800fb7b8: li	at,2
  800fb7bc: lui	t9,0x8015
  800fb7c0: bne	t7,at,0x800fb7d8
  800fb7c4: lui	a0,0x8015
  800fb7c8: li	t8,1
  800fb7cc: lui	at,0x8015
  800fb7d0: b	0x800fb7e4
  800fb7d4: sh	t8,6864(at)
  800fb7d8: lh	t9,-24312(t9)
  800fb7dc: lui	at,0x8015
  800fb7e0: sh	t9,6864(at)
  800fb7e4: jal	0x800a6be4   <wheel_render_full>
  800fb7e8: lh	a0,6864(a0)
  800fb7ec: lb	t6,0(s0)
  800fb7f0: lui	a0,0x8015
  800fb7f4: lb	a0,-26740(a0)
  800fb7f8: beqz	t6,0x800fb818
  800fb7fc: lui	at,0x8014
  800fb800: sb	zero,14864(at)
  800fb804: move	a1,zero
  800fb808: jal	0x800bb9b0   <LoadTrack>
  800fb80c: li	a2,1
  800fb810: b	0x800fb834
  800fb814: nop
  800fb818: li	a1,1
  800fb81c: jal	0x800bb9b0   <LoadTrack>
  800fb820: move	a2,zero
  800fb824: bnez	v0,0x800fb834
  800fb828: nop
  800fb82c: b	0x800fbbcc
  800fb830: move	v0,zero
  800fb834: jal	0x800bdaa8   <camera_shake_start>
  800fb838: nop
  800fb83c: sb	zero,0(s0)
  800fb840: lui	t7,0x8015
  800fb844: lh	t7,6864(t7)
  800fb848: move	s1,zero
  800fb84c: blez	t7,0x800fb8c4
  800fb850: lui	s0,0x8015
  800fb854: addiu	s0,s0,2964
  800fb858: lui	t8,0x8014
  800fb85c: lb	t8,10144(t8)
  800fb860: lui	t6,0x8011
  800fb864: addiu	t6,t6,18008
  800fb868: sll	t9,t8,0x2
  800fb86c: subu	t9,t9,t8
  800fb870: addu	v0,t9,t6
  800fb874: lbu	t7,2(v0)
  800fb878: li	t8,255
  800fb87c: sw	t8,20(sp)
  800fb880: lbu	a2,0(v0)
  800fb884: lbu	a3,1(v0)
  800fb888: li	a0,1000
  800fb88c: li	a1,999
  800fb890: sw	s1,24(sp)
  800fb894: jal	0x800a7480   <func_800A7480>
  800fb898: sw	t7,16(sp)
  800fb89c: move	a0,s1
  800fb8a0: jal	0x800ab18c   <func_800AB18C>
  800fb8a4: move	a1,s0
  800fb8a8: lui	t9,0x8015
  800fb8ac: lh	t9,6864(t9)
  800fb8b0: addiu	s1,s1,1
  800fb8b4: addiu	s0,s0,152
  800fb8b8: slt	at,s1,t9
  800fb8bc: bnez	at,0x800fb858
  800fb8c0: nop
  800fb8c4: lui	t6,0x8011
  800fb8c8: lw	t6,29876(t6)
  800fb8cc: andi	t7,t6,0x8
  800fb8d0: bnez	t7,0x800fb924
  800fb8d4: lui	a0,0x8014
  800fb8d8: addiu	a0,a0,10024
  800fb8dc: move	a1,zero
  800fb8e0: jal	0x80007270   <osRecvMesg>
  800fb8e4: li	a2,1
  800fb8e8: jal	0x80091b00   <func_80091B00>
  800fb8ec: nop
  800fb8f0: li	t8,7
  800fb8f4: lui	a0,0x8014
  800fb8f8: addiu	a0,a0,10024
  800fb8fc: sb	t8,2(v0)
  800fb900: move	s0,v0
  800fb904: move	a1,zero
  800fb908: jal	0x800075e0   <osSendMesg>
  800fb90c: move	a2,zero
  800fb910: lui	a0,0x8014
  800fb914: addiu	a0,a0,10152
  800fb918: move	a1,s0
  800fb91c: jal	0x800075e0   <osSendMesg>
  800fb920: move	a2,zero
  800fb924: lui	at,0x8015
  800fb928: lui	t9,0x8015
  800fb92c: lh	t9,10036(t9)
  800fb930: sb	zero,10008(at)
  800fb934: lui	at,0x8015
  800fb938: jal	0x800ec2f8   <world_physics_tick>
  800fb93c: sb	t9,8212(at)
  800fb940: lui	v0,0x8014
  800fb944: lui	v1,0x8014
  800fb948: addiu	v1,v1,14846
  800fb94c: addiu	v0,v0,14840
  800fb950: addiu	v0,v0,1
  800fb954: sltu	at,v0,v1
  800fb958: bnez	at,0x800fb950
  800fb95c: sb	zero,-1(v0)
  800fb960: jal	0x800faee4   <func_800FAEE4>
  800fb964: nop
  800fb968: lui	at,0x8014
  800fb96c: sb	zero,-309(at)
  800fb970: lui	at,0x8014
  800fb974: lui	t6,0x8015
  800fb978: lb	t6,17352(t6)
  800fb97c: sb	zero,9881(at)
  800fb980: lui	at,0x8014
  800fb984: sb	zero,9872(at)
  800fb988: lui	at,0x8012
  800fb98c: sll	t7,t6,0x2
  800fb990: addu	at,at,t7
  800fb994: lwc1	$f4,760(at)
  800fb998: lui	at,0x8012
  800fb99c: lwc1	$f6,18420(at)
  800fb9a0: lui	a0,0x8015
  800fb9a4: lb	a0,10052(a0)
  800fb9a8: mul.s	$f8,$f4,$f6
  800fb9ac: lui	at,0x8014
  800fb9b0: lui	s0,0x8015
  800fb9b4: addiu	s0,s0,-23984
  800fb9b8: move	s1,zero
  800fb9bc: blez	a0,0x800fba18
  800fb9c0: swc1	$f8,10084(at)
  800fb9c4: lh	v0,1990(s0)
  800fb9c8: lui	v1,0x8015
  800fb9cc: li	at,6
  800fb9d0: sll	t8,v0,0x3
  800fb9d4: addu	v1,v1,t8
  800fb9d8: lbu	v1,16015(v1)
  800fb9dc: sll	a1,v0,0x10
  800fb9e0: sra	t9,a1,0x10
  800fb9e4: beqz	v1,0x800fb9f4
  800fb9e8: move	a1,t9
  800fb9ec: bnel	v1,at,0x800fba08
  800fb9f0: addiu	s1,s1,1
  800fb9f4: jal	0x800e543c   <func_800E543C>
  800fb9f8: move	a0,zero
  800fb9fc: lui	a0,0x8015
  800fba00: lb	a0,10052(a0)
  800fba04: addiu	s1,s1,1
  800fba08: slt	at,s1,a0
  800fba0c: bnez	at,0x800fb9c4
  800fba10: addiu	s0,s0,2056
  800fba14: move	s1,zero
  800fba18: jal	0x800b1b48   <music_control>
  800fba1c: nop
  800fba20: jal	0x800fade0   <render_post_process>
  800fba24: nop
  800fba28: lw	t6,256(sp)
  800fba2c: beqz	t6,0x800fba3c
  800fba30: nop
  800fba34: jal	0x800f0100   <skid_mark_render>
  800fba38: nop
  800fba3c: lui	v1,0x8015
  800fba40: lh	v1,-24312(v1)
  800fba44: lui	s0,0x8015
  800fba48: addiu	s0,s0,-24296
  800fba4c: blez	v1,0x800fbabc
  800fba50: lui	v0,0x8015
  800fba54: addiu	v0,v0,10264
  800fba58: mtc1	zero,$f10
  800fba5c: li	t7,800
  800fba60: sh	zero,64(s0)
  800fba64: sh	t7,902(v0)
  800fba68: sb	zero,931(v0)
  800fba6c: lui	t8,0x8015
  800fba70: swc1	$f10,940(v0)
  800fba74: lw	t8,-24304(t8)
  800fba78: li	at,6
  800fba7c: sll	a0,s1,0x10
  800fba80: bne	t8,at,0x800fbaa0
  800fba84: sra	t9,a0,0x10
  800fba88: move	a0,t9
  800fba8c: jal	0x8038ca24   <?>
  800fba90: sw	v0,84(sp)
  800fba94: lui	v1,0x8015
  800fba98: lh	v1,-24312(v1)
  800fba9c: lw	v0,84(sp)
  800fbaa0: lui	at,0x8011
  800fbaa4: addiu	s1,s1,1
  800fbaa8: sb	zero,18004(at)
  800fbaac: slt	at,s1,v1
  800fbab0: addiu	s0,s0,76
  800fbab4: bnez	at,0x800fba58
  800fbab8: addiu	v0,v0,952
  800fbabc: lui	t6,0x8011
  800fbac0: lw	t6,29876(t6)
  800fbac4: andi	t7,t6,0x8
  800fbac8: bnez	t7,0x800fbad8
  800fbacc: nop
  800fbad0: jal	0x800b6138   <entity_audio_update,set_race_state>
  800fbad4: nop
  800fbad8: jal	0x800d6530   <func_800D6530>
  800fbadc: nop
  800fbae0: lui	v0,0x8015
  800fbae4: lw	v0,-24304(v0)
  800fbae8: li	at,6
  800fbaec: lui	a0,0x8015
  800fbaf0: bne	v0,at,0x800fbb28
  800fbaf4: addiu	a0,a0,-27608
  800fbaf8: move	a1,zero
  800fbafc: jal	0x80002790   <memset>
  800fbb00: li	a2,16
  800fbb04: lui	a0,0x8015
  800fbb08: addiu	a0,a0,9580
  800fbb0c: move	a1,zero
  800fbb10: jal	0x80002790   <memset>
  800fbb14: li	a2,4
  800fbb18: jal	0x803908d0   <?>
  800fbb1c: nop
  800fbb20: lui	v0,0x8015
  800fbb24: lw	v0,-24304(v0)
  800fbb28: li	at,4
  800fbb2c: bne	v0,at,0x800fbb94
  800fbb30: lui	a0,0x8015
  800fbb34: addiu	a0,a0,8248
  800fbb38: move	a1,zero
  800fbb3c: jal	0x80002790   <memset>
  800fbb40: li	a2,480
  800fbb44: lui	a0,0x8015
  800fbb48: lui	s1,0x8015
  800fbb4c: addiu	s1,s1,27560
  800fbb50: addiu	a0,a0,27064
  800fbb54: lui	s0,0x1500
  800fbb58: move	a1,zero
  800fbb5c: li	a2,124
  800fbb60: jal	0x80002790   <memset>
  800fbb64: sw	a0,92(sp)
  800fbb68: lw	a0,92(sp)
  800fbb6c: lw	t8,0(a0)
  800fbb70: addiu	a0,a0,124
  800fbb74: or	t9,t8,s0
  800fbb78: bne	a0,s1,0x800fbb58
  800fbb7c: sw	t9,-124(a0)
  800fbb80: mtc1	zero,$f4
  800fbb84: lui	at,0x8016
  800fbb88: swc1	$f4,5020(at)
  800fbb8c: lui	at,0x8015
  800fbb90: sw	zero,10040(at)
  800fbb94: lui	t6,0x8011
  800fbb98: lw	t6,29876(t6)
  800fbb9c: li	t8,1
  800fbba0: lui	at,0x8012
  800fbba4: andi	t7,t6,0x8
  800fbba8: bnez	t7,0x800fbbb4
  800fbbac: lui	t9,0x8011
  800fbbb0: sb	t8,-5404(at)
  800fbbb4: lh	t9,25770(t9)
  800fbbb8: lui	at,0x8015
  800fbbbc: li	v0,1
  800fbbc0: sb	t9,6872(at)
  800fbbc4: lui	at,0x8011
  800fbbc8: sb	zero,18164(at)
  800fbbcc: lw	ra,76(sp)
  800fbbd0: lw	s0,40(sp)
  800fbbd4: lw	s1,44(sp)
  800fbbd8: lw	s2,48(sp)
  800fbbdc: lw	s3,52(sp)
  800fbbe0: lw	s4,56(sp)
  800fbbe4: lw	s5,60(sp)
  800fbbe8: lw	s6,64(sp)
  800fbbec: lw	s7,68(sp)
  800fbbf0: lw	s8,72(sp)
  800fbbf4: jr	ra
  800fbbf8: addiu	sp,sp,256
