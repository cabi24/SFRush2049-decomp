== control_settings @ 0x800DD4AC (638 words)
  800dd4ac: addiu	sp,sp,-1896
  800dd4b0: lw	t0,1912(sp)
  800dd4b4: sw	s3,52(sp)
  800dd4b8: move	s3,a0
  800dd4bc: sw	ra,76(sp)
  800dd4c0: sw	s8,72(sp)
  800dd4c4: sw	s7,68(sp)
  800dd4c8: sw	s6,64(sp)
  800dd4cc: sw	s5,60(sp)
  800dd4d0: sw	s4,56(sp)
  800dd4d4: sw	s2,48(sp)
  800dd4d8: sw	s1,44(sp)
  800dd4dc: sw	s0,40(sp)
  800dd4e0: sw	a1,1900(sp)
  800dd4e4: sw	a2,1904(sp)
  800dd4e8: beqz	t0,0x800dd508
  800dd4ec: sw	a3,1908(sp)
  800dd4f0: jalr	t0
  800dd4f4: nop
  800dd4f8: beqzl	v0,0x800dd50c
  800dd4fc: sll	t6,s3,0x2
  800dd500: jal	0x800dd45c   <func_800DD45C>
  800dd504: move	a0,s3
  800dd508: sll	t6,s3,0x2
  800dd50c: subu	t6,t6,s3
  800dd510: sll	t6,t6,0x2
  800dd514: subu	t6,t6,s3
  800dd518: lui	t7,0x8015
  800dd51c: addiu	t7,t7,16344
  800dd520: sll	t6,t6,0x3
  800dd524: addu	t8,t6,t7
  800dd528: sw	t8,92(sp)
  800dd52c: addiu	s1,t8,44
  800dd530: li	s4,1
  800dd534: lw	t9,20(s1)
  800dd538: bnez	t9,0x800dd57c
  800dd53c: nop
  800dd540: lb	t6,0(s1)
  800dd544: bnezl	t6,0x800dd574
  800dd548: addiu	s4,s4,-1
  800dd54c: lw	t7,1908(sp)
  800dd550: li	t0,1
  800dd554: sb	t0,0(s1)
  800dd558: sb	t0,0(t7)
  800dd55c: lw	t8,1904(sp)
  800dd560: sb	zero,0(t8)
  800dd564: lw	t9,1900(sp)
  800dd568: b	0x800dde24
  800dd56c: sb	zero,0(t9)
  800dd570: addiu	s4,s4,-1
  800dd574: bgez	s4,0x800dd534
  800dd578: addiu	s1,s1,-44
  800dd57c: bltz	s4,0x800dde74
  800dd580: lui	t6,0x8015
  800dd584: lw	t6,26956(t6)
  800dd588: lui	t7,0x8015
  800dd58c: lui	a0,0x8014
  800dd590: sw	t6,32(s1)
  800dd594: lw	t7,26948(t7)
  800dd598: addiu	a0,a0,25040
  800dd59c: move	a1,zero
  800dd5a0: li	a2,1
  800dd5a4: jal	0x80007270   <osRecvMesg>
  800dd5a8: sw	t7,36(s1)
  800dd5ac: li	s2,11
  800dd5b0: sw	s1,88(sp)
  800dd5b4: jal	0x800b4200   <slot_state_setup>
  800dd5b8: sw	s3,1896(sp)
  800dd5bc: lui	a0,0x8014
  800dd5c0: addiu	a0,a0,25040
  800dd5c4: lw	s1,88(sp)
  800dd5c8: lw	s3,1896(sp)
  800dd5cc: move	s0,v0
  800dd5d0: move	a1,zero
  800dd5d4: jal	0x800075e0   <osSendMesg>
  800dd5d8: move	a2,zero
  800dd5dc: jal	0x800b74a0   <dispatch_handler>
  800dd5e0: li	a0,1
  800dd5e4: lui	a0,0x8014
  800dd5e8: addiu	a0,a0,25040
  800dd5ec: move	a1,zero
  800dd5f0: jal	0x80007270   <osRecvMesg>
  800dd5f4: li	a2,1
  800dd5f8: li	s2,11
  800dd5fc: sw	s1,88(sp)
  800dd600: jal	0x800b4200   <slot_state_setup>
  800dd604: sw	s3,1896(sp)
  800dd608: lui	a0,0x8014
  800dd60c: addiu	a0,a0,25040
  800dd610: lw	s1,88(sp)
  800dd614: lw	s3,1896(sp)
  800dd618: move	s0,v0
  800dd61c: move	a1,zero
  800dd620: jal	0x800075e0   <osSendMesg>
  800dd624: move	a2,zero
  800dd628: jal	0x800b74a0   <dispatch_handler>
  800dd62c: li	a0,1
  800dd630: lui	a1,0x8012
  800dd634: addiu	a1,a1,4192
  800dd638: addiu	a0,sp,1816
  800dd63c: jal	0x80004990   <sprintf>
  800dd640: addiu	a2,s3,1
  800dd644: lh	t8,28(s1)
  800dd648: lh	t6,24(s1)
  800dd64c: addiu	a0,sp,1816
  800dd650: bgez	t8,0x800dd660
  800dd654: sra	t9,t8,0x1
  800dd658: addiu	at,t8,1
  800dd65c: sra	t9,at,0x1
  800dd660: addu	s0,t9,t6
  800dd664: sll	t7,s0,0x10
  800dd668: sra	s0,t7,0x10
  800dd66c: jal	0x800b3fa4   <object_manager_update,object_utility>
  800dd670: li	a1,-1
  800dd674: srl	t9,v0,0x1
  800dd678: subu	t7,s0,t9
  800dd67c: sll	a0,t7,0x10
  800dd680: sra	t8,a0,0x10
  800dd684: move	a0,t8
  800dd688: lh	a1,26(s1)
  800dd68c: jal	0x800b71d4   <state_utility>
  800dd690: addiu	a2,sp,1816
  800dd694: lw	t9,20(s1)
  800dd698: addiu	t6,t9,-1
  800dd69c: sltiu	at,t6,15
  800dd6a0: beqz	at,0x800dddf0
  800dd6a4: sll	t6,t6,0x2
  800dd6a8: lui	at,0x8012
  800dd6ac: addu	at,at,t6
  800dd6b0: lw	t6,17100(at)
  800dd6b4: jr	t6
  800dd6b8: nop
  800dd6bc: move	a0,s3
  800dd6c0: move	a1,s4
  800dd6c4: jal	0x800dcd58   <func_800DCD58>
  800dd6c8: sw	s1,88(sp)
  800dd6cc: lui	a0,0x8014
  800dd6d0: addiu	a0,a0,25040
  800dd6d4: move	a1,zero
  800dd6d8: li	a2,1
  800dd6dc: jal	0x80007270   <osRecvMesg>
  800dd6e0: lw	s1,88(sp)
  800dd6e4: li	s2,11
  800dd6e8: sw	s1,88(sp)
  800dd6ec: jal	0x800b4200   <slot_state_setup>
  800dd6f0: sw	s3,1896(sp)
  800dd6f4: lui	a0,0x8014
  800dd6f8: addiu	a0,a0,25040
  800dd6fc: lw	s1,88(sp)
  800dd700: move	s0,v0
  800dd704: move	a1,zero
  800dd708: jal	0x800075e0   <osSendMesg>
  800dd70c: move	a2,zero
  800dd710: jal	0x800b74a0   <dispatch_handler>
  800dd714: li	a0,1
  800dd718: lui	s0,0x8018
  800dd71c: addiu	s0,s0,-23328
  800dd720: lw	t7,4(s0)
  800dd724: addiu	a0,sp,1520
  800dd728: jal	0x800be6a4   <func_800BE6A4>
  800dd72c: lw	a1,404(t7)
  800dd730: lui	a1,0x8012
  800dd734: addiu	a1,a1,4208
  800dd738: jal	0x800be4f0   <func_800BE4F0>
  800dd73c: addiu	a0,sp,1520
  800dd740: lw	t8,4(s0)
  800dd744: addiu	a0,sp,1520
  800dd748: jal	0x800be4f0   <func_800BE4F0>
  800dd74c: lw	a1,408(t8)
  800dd750: lh	a2,28(s1)
  800dd754: lh	t6,24(s1)
  800dd758: lh	v1,30(s1)
  800dd75c: lui	s0,0x8012
  800dd760: addiu	s0,s0,-29152
  800dd764: addiu	v1,v1,-20
  800dd768: sll	a3,v1,0x10
  800dd76c: li	t0,1
  800dd770: sw	t0,0(s0)
  800dd774: sw	t0,4(s0)
  800dd778: bgez	a2,0x800dd788
  800dd77c: sra	t9,a2,0x1
  800dd780: addiu	at,a2,1
  800dd784: sra	t9,at,0x1
  800dd788: addu	a0,t9,t6
  800dd78c: lh	t6,26(s1)
  800dd790: sll	t7,a0,0x10
  800dd794: sra	t8,t7,0x10
  800dd798: move	a0,t8
  800dd79c: bgez	v1,0x800dd7ac
  800dd7a0: sra	t9,v1,0x1
  800dd7a4: addiu	at,v1,1
  800dd7a8: sra	t9,at,0x1
  800dd7ac: addu	a1,t9,t6
  800dd7b0: addiu	a1,a1,20
  800dd7b4: sll	t7,a1,0x10
  800dd7b8: sra	a1,t7,0x10
  800dd7bc: addiu	t7,sp,1520
  800dd7c0: li	t6,-1
  800dd7c4: sra	t9,a3,0x10
  800dd7c8: move	a3,t9
  800dd7cc: sw	t6,16(sp)
  800dd7d0: sw	t7,24(sp)
  800dd7d4: jal	0x800be078   <camera_auto_follow>
  800dd7d8: sw	zero,20(sp)
  800dd7dc: li	t8,3
  800dd7e0: lui	a0,0x8014
  800dd7e4: sw	t8,4(s0)
  800dd7e8: sw	zero,0(s0)
  800dd7ec: addiu	a0,a0,25040
  800dd7f0: move	a1,zero
  800dd7f4: jal	0x80007270   <osRecvMesg>
  800dd7f8: li	a2,1
  800dd7fc: li	s2,11
  800dd800: jal	0x800b4200   <slot_state_setup>
  800dd804: sw	s1,88(sp)
  800dd808: lui	a0,0x8014
  800dd80c: addiu	a0,a0,25040
  800dd810: lw	s1,88(sp)
  800dd814: move	s0,v0
  800dd818: move	a1,zero
  800dd81c: jal	0x800075e0   <osSendMesg>
  800dd820: move	a2,zero
  800dd824: b	0x800dddf4
  800dd828: lb	t9,0(s1)
  800dd82c: lui	t9,0x8011
  800dd830: lw	t9,29876(t9)
  800dd834: lui	at,0x7c03
  800dd838: ori	at,at,0xfffe
  800dd83c: and	t6,t9,at
  800dd840: bnez	t6,0x800dd860
  800dd844: move	a1,s3
  800dd848: move	a0,s3
  800dd84c: move	a1,s4
  800dd850: jal	0x800dcd58   <func_800DCD58>
  800dd854: sw	s1,88(sp)
  800dd858: b	0x800dd868
  800dd85c: lw	s1,88(sp)
  800dd860: jal	0x800dcce0   <func_800DCCE0>
  800dd864: move	a2,s4
  800dd868: sw	s3,0(sp)
  800dd86c: sw	s4,4(sp)
  800dd870: sw	s1,88(sp)
  800dd874: jal	0x800dd0c0   <func_800DD0C0>
  800dd878: sw	s4,1892(sp)
  800dd87c: lw	s1,88(sp)
  800dd880: b	0x800dddf0
  800dd884: lw	s4,1892(sp)
  800dd888: move	a0,s3
  800dd88c: move	a1,s4
  800dd890: jal	0x800dcd58   <func_800DCD58>
  800dd894: sw	s1,88(sp)
  800dd898: lw	s1,88(sp)
  800dd89c: lui	s0,0x8018
  800dd8a0: addiu	s0,s0,-23328
  800dd8a4: lb	t7,16(s1)
  800dd8a8: beqzl	t7,0x800dd8c8
  800dd8ac: lw	t9,4(s0)
  800dd8b0: lui	s0,0x8018
  800dd8b4: addiu	s0,s0,-23328
  800dd8b8: lw	t8,4(s0)
  800dd8bc: b	0x800dd8cc
  800dd8c0: lw	a1,424(t8)
  800dd8c4: lw	t9,4(s0)
  800dd8c8: lw	a1,428(t9)
  800dd8cc: jal	0x800be6a4   <func_800BE6A4>
  800dd8d0: addiu	a0,sp,1192
  800dd8d4: lui	a1,0x8012
  800dd8d8: addiu	a1,a1,4212
  800dd8dc: jal	0x800be4f0   <func_800BE4F0>
  800dd8e0: addiu	a0,sp,1192
  800dd8e4: lw	t6,4(s0)
  800dd8e8: addiu	a0,sp,1192
  800dd8ec: jal	0x800be4f0   <func_800BE4F0>
  800dd8f0: lw	a1,432(t6)
  800dd8f4: lw	t8,12(s0)
  800dd8f8: lw	t7,16(s0)
  800dd8fc: move	s6,s3
  800dd900: lhu	t9,50(t8)
  800dd904: move	s7,s4
  800dd908: addiu	s8,sp,1192
  800dd90c: sll	t6,t9,0x2
  800dd910: addu	t8,t7,t6
  800dd914: lw	t9,0(t8)
  800dd918: lw	t7,4(s0)
  800dd91c: lb	s5,1(s1)
  800dd920: sw	t9,12(sp)
  800dd924: lw	t6,944(t7)
  800dd928: sw	s4,1892(sp)
  800dd92c: sw	s1,88(sp)
  800dd930: jal	0x800dcdf4   <attract_video_handler>
  800dd934: sw	t6,16(sp)
  800dd938: lw	s1,88(sp)
  800dd93c: b	0x800dddf0
  800dd940: lw	s4,1892(sp)
  800dd944: move	a1,s3
  800dd948: jal	0x800dcce0   <func_800DCCE0>
  800dd94c: move	a2,s4
  800dd950: lui	s0,0x8018
  800dd954: addiu	s0,s0,-23328
  800dd958: lw	t8,4(s0)
  800dd95c: addiu	a0,sp,920
  800dd960: jal	0x800be6a4   <func_800BE6A4>
  800dd964: lw	a1,436(t8)
  800dd968: lui	a1,0x8012
  800dd96c: addiu	a1,a1,4216
  800dd970: jal	0x800be4f0   <func_800BE4F0>
  800dd974: addiu	a0,sp,920
  800dd978: lw	t9,4(s0)
  800dd97c: addiu	a0,sp,920
  800dd980: jal	0x800be4f0   <func_800BE4F0>
  800dd984: lw	a1,440(t9)
  800dd988: move	s5,s3
  800dd98c: move	s6,s4
  800dd990: addiu	s7,sp,920
  800dd994: sw	s1,88(sp)
  800dd998: jal	0x800dc88c   <attract_mode_handler>
  800dd99c: sw	s4,1892(sp)
  800dd9a0: lw	s1,88(sp)
  800dd9a4: b	0x800dddf0
  800dd9a8: lw	s4,1892(sp)
  800dd9ac: move	a1,s3
  800dd9b0: jal	0x800dcce0   <func_800DCCE0>
  800dd9b4: move	a2,s4
  800dd9b8: lui	s0,0x8018
  800dd9bc: addiu	s0,s0,-23328
  800dd9c0: lw	t7,4(s0)
  800dd9c4: addiu	a0,sp,656
  800dd9c8: jal	0x800be6a4   <func_800BE6A4>
  800dd9cc: lw	a1,444(t7)
  800dd9d0: lui	a1,0x8012
  800dd9d4: addiu	a1,a1,4232
  800dd9d8: jal	0x800be4f0   <func_800BE4F0>
  800dd9dc: addiu	a0,sp,656
  800dd9e0: lw	t6,4(s0)
  800dd9e4: addiu	a0,sp,656
  800dd9e8: jal	0x800be4f0   <func_800BE4F0>
  800dd9ec: lw	a1,408(t6)
  800dd9f0: move	s5,s3
  800dd9f4: move	s6,s4
  800dd9f8: addiu	s7,sp,656
  800dd9fc: sw	s1,88(sp)
  800dda00: jal	0x800dc88c   <attract_mode_handler>
  800dda04: sw	s4,1892(sp)
  800dda08: lw	s1,88(sp)
  800dda0c: b	0x800dddf0
  800dda10: lw	s4,1892(sp)
  800dda14: move	a0,s3
  800dda18: move	a1,s4
  800dda1c: jal	0x800dcd58   <func_800DCD58>
  800dda20: sw	s1,88(sp)
  800dda24: lui	s0,0x8018
  800dda28: addiu	s0,s0,-23328
  800dda2c: lw	t9,12(s0)
  800dda30: lw	t8,16(s0)
  800dda34: lw	v0,4(s0)
  800dda38: lhu	t7,52(t9)
  800dda3c: lw	s1,88(sp)
  800dda40: lw	s8,444(v0)
  800dda44: sll	t6,t7,0x2
  800dda48: addu	t9,t8,t6
  800dda4c: lw	t7,4(t9)
  800dda50: move	s6,s3
  800dda54: move	s7,s4
  800dda58: sw	t7,12(sp)
  800dda5c: lw	t8,944(v0)
  800dda60: sw	s4,1892(sp)
  800dda64: lb	s5,1(s1)
  800dda68: jal	0x800dcdf4   <attract_video_handler>
  800dda6c: sw	t8,16(sp)
  800dda70: lw	s1,88(sp)
  800dda74: b	0x800dddf0
  800dda78: lw	s4,1892(sp)
  800dda7c: move	a0,s3
  800dda80: move	a1,s4
  800dda84: jal	0x800dcd58   <func_800DCD58>
  800dda88: sw	s1,88(sp)
  800dda8c: lui	s0,0x8018
  800dda90: addiu	s0,s0,-23328
  800dda94: lw	t7,12(s0)
  800dda98: lw	t6,4(s0)
  800dda9c: lw	t9,16(s0)
  800ddaa0: lhu	t8,48(t7)
  800ddaa4: lw	s8,448(t6)
  800ddaa8: lw	s1,88(sp)
  800ddaac: sll	t6,t8,0x2
  800ddab0: addu	v0,t9,t6
  800ddab4: lw	t7,0(v0)
  800ddab8: move	s6,s3
  800ddabc: move	s7,s4
  800ddac0: sw	t7,12(sp)
  800ddac4: lw	t8,4(v0)
  800ddac8: sw	s4,1892(sp)
  800ddacc: lb	s5,1(s1)
  800ddad0: jal	0x800dcdf4   <attract_video_handler>
  800ddad4: sw	t8,16(sp)
  800ddad8: lw	s1,88(sp)
  800ddadc: b	0x800dddf0
  800ddae0: lw	s4,1892(sp)
  800ddae4: move	a0,s3
  800ddae8: move	a1,s4
  800ddaec: jal	0x800dcd58   <func_800DCD58>
  800ddaf0: sw	s1,88(sp)
  800ddaf4: lui	s0,0x8018
  800ddaf8: addiu	s0,s0,-23328
  800ddafc: lw	t9,4(s0)
  800ddb00: addiu	a0,sp,392
  800ddb04: lw	s1,88(sp)
  800ddb08: jal	0x800be6a4   <func_800BE6A4>
  800ddb0c: lw	a1,452(t9)
  800ddb10: lui	a1,0x8012
  800ddb14: addiu	a1,a1,4220
  800ddb18: jal	0x800be4f0   <func_800BE4F0>
  800ddb1c: addiu	a0,sp,392
  800ddb20: lw	t6,4(s0)
  800ddb24: addiu	a0,sp,392
  800ddb28: jal	0x800be4f0   <func_800BE4F0>
  800ddb2c: lw	a1,456(t6)
  800ddb30: move	s5,s3
  800ddb34: move	s6,s4
  800ddb38: addiu	s7,sp,392
  800ddb3c: sw	s1,88(sp)
  800ddb40: jal	0x800dc88c   <attract_mode_handler>
  800ddb44: sw	s4,1892(sp)
  800ddb48: lw	s1,88(sp)
  800ddb4c: b	0x800dddf0
  800ddb50: lw	s4,1892(sp)
  800ddb54: move	a1,s3
  800ddb58: jal	0x800dcce0   <func_800DCCE0>
  800ddb5c: move	a2,s4
  800ddb60: lui	s0,0x8018
  800ddb64: addiu	s0,s0,-23328
  800ddb68: lw	t7,4(s0)
  800ddb6c: addiu	a0,sp,128
  800ddb70: jal	0x800be6a4   <func_800BE6A4>
  800ddb74: lw	a1,460(t7)
  800ddb78: lui	a1,0x8012
  800ddb7c: addiu	a1,a1,4224
  800ddb80: jal	0x800be4f0   <func_800BE4F0>
  800ddb84: addiu	a0,sp,128
  800ddb88: lw	t8,4(s0)
  800ddb8c: addiu	a0,sp,128
  800ddb90: jal	0x800be4f0   <func_800BE4F0>
  800ddb94: lw	a1,440(t8)
  800ddb98: move	s5,s3
  800ddb9c: move	s6,s4
  800ddba0: addiu	s7,sp,128
  800ddba4: sw	s1,88(sp)
  800ddba8: jal	0x800dc88c   <attract_mode_handler>
  800ddbac: sw	s4,1892(sp)
  800ddbb0: lw	s1,88(sp)
  800ddbb4: b	0x800dddf0
  800ddbb8: lw	s4,1892(sp)
  800ddbbc: lw	a0,32(s1)
  800ddbc0: andi	t9,a0,0x800
  800ddbc4: beqzl	t9,0x800ddc1c
  800ddbc8: andi	t8,a0,0x400
  800ddbcc: jal	0x800b5f4c   <audio_distance_atten>
  800ddbd0: nop
  800ddbd4: lb	t6,1(s1)
  800ddbd8: li	t0,1
  800ddbdc: beqzl	t6,0x800ddbf8
  800ddbe0: lb	t7,2(s1)
  800ddbe4: sb	zero,1(s1)
  800ddbe8: sb	t0,2(s1)
  800ddbec: b	0x800ddc18
  800ddbf0: lw	a0,32(s1)
  800ddbf4: lb	t7,2(s1)
  800ddbf8: li	t0,1
  800ddbfc: beqzl	t7,0x800ddc14
  800ddc00: sb	t0,1(s1)
  800ddc04: sb	zero,2(s1)
  800ddc08: b	0x800ddc18
  800ddc0c: lw	a0,32(s1)
  800ddc10: sb	t0,1(s1)
  800ddc14: lw	a0,32(s1)
  800ddc18: andi	t8,a0,0x400
  800ddc1c: beqz	t8,0x800ddc6c
  800ddc20: li	t0,1
  800ddc24: jal	0x800b5f4c   <audio_distance_atten>
  800ddc28: nop
  800ddc2c: lb	t9,1(s1)
  800ddc30: li	t0,1
  800ddc34: beqzl	t9,0x800ddc4c
  800ddc38: lb	t6,2(s1)
  800ddc3c: sb	zero,1(s1)
  800ddc40: b	0x800ddc6c
  800ddc44: lw	a0,32(s1)
  800ddc48: lb	t6,2(s1)
  800ddc4c: beqzl	t6,0x800ddc68
  800ddc50: sb	t0,2(s1)
  800ddc54: sb	zero,2(s1)
  800ddc58: sb	t0,1(s1)
  800ddc5c: b	0x800ddc6c
  800ddc60: lw	a0,32(s1)
  800ddc64: sb	t0,2(s1)
  800ddc68: lw	a0,32(s1)
  800ddc6c: andi	t7,a0,0x2
  800ddc70: beqz	t7,0x800ddc90
  800ddc74: li	a0,37
  800ddc78: move	a1,zero
  800ddc7c: move	a2,t0
  800ddc80: jal	0x80092360   <entity_flags_apply,frame_sync>
  800ddc84: move	a3,zero
  800ddc88: li	t0,1
  800ddc8c: sb	t0,0(s1)
  800ddc90: lui	s0,0x8018
  800ddc94: addiu	s0,s0,-23328
  800ddc98: lw	v0,4(s0)
  800ddc9c: lw	t7,12(s0)
  800ddca0: lw	t6,16(s0)
  800ddca4: lw	t8,472(v0)
  800ddca8: move	s6,s3
  800ddcac: move	s7,s4
  800ddcb0: sw	t8,8(sp)
  800ddcb4: lw	t9,944(v0)
  800ddcb8: lb	s5,1(s1)
  800ddcbc: lb	s8,2(s1)
  800ddcc0: sw	t9,12(sp)
  800ddcc4: lhu	t8,52(t7)
  800ddcc8: sll	t9,t8,0x2
  800ddccc: addu	v1,t6,t9
  800ddcd0: lw	t7,0(v1)
  800ddcd4: sw	t7,16(sp)
  800ddcd8: lw	t8,4(v1)
  800ddcdc: sw	s4,1892(sp)
  800ddce0: sw	s1,88(sp)
  800ddce4: jal	0x800dc99c   <attract_demo_handler>
  800ddce8: sw	t8,20(sp)
  800ddcec: lw	s1,88(sp)
  800ddcf0: b	0x800dddf0
  800ddcf4: lw	s4,1892(sp)
  800ddcf8: lui	t9,0x8003
  800ddcfc: addiu	t9,t9,-5912
  800ddd00: lw	t7,636(t9)
  800ddd04: lw	t6,40(s1)
  800ddd08: lui	s0,0x8018
  800ddd0c: addiu	s0,s0,-23328
  800ddd10: sltu	at,t6,t7
  800ddd14: beqz	at,0x800ddd20
  800ddd18: li	t0,1
  800ddd1c: sb	t0,0(s1)
  800ddd20: lw	t8,4(s0)
  800ddd24: move	s5,s3
  800ddd28: move	s6,s4
  800ddd2c: lw	s7,476(t8)
  800ddd30: sw	s4,1892(sp)
  800ddd34: jal	0x800dc88c   <attract_mode_handler>
  800ddd38: sw	s1,88(sp)
  800ddd3c: lw	s1,88(sp)
  800ddd40: b	0x800dddf0
  800ddd44: lw	s4,1892(sp)
  800ddd48: lui	t6,0x8003
  800ddd4c: addiu	t6,t6,-5912
  800ddd50: lw	t7,636(t6)
  800ddd54: lw	t9,40(s1)
  800ddd58: lui	s0,0x8018
  800ddd5c: addiu	s0,s0,-23328
  800ddd60: sltu	at,t9,t7
  800ddd64: beqz	at,0x800ddd70
  800ddd68: li	t0,1
  800ddd6c: sb	t0,0(s1)
  800ddd70: lw	t8,4(s0)
  800ddd74: move	s5,s3
  800ddd78: move	s6,s4
  800ddd7c: lw	s7,480(t8)
  800ddd80: sw	s4,1892(sp)
  800ddd84: jal	0x800dc88c   <attract_mode_handler>
  800ddd88: sw	s1,88(sp)
  800ddd8c: lw	s1,88(sp)
  800ddd90: b	0x800dddf0
  800ddd94: lw	s4,1892(sp)
  800ddd98: lui	t9,0x8003
  800ddd9c: addiu	t9,t9,-5912
  800ddda0: lw	t7,636(t9)
  800ddda4: lw	t6,40(s1)
  800ddda8: lui	s0,0x8018
  800dddac: addiu	s0,s0,-23328
  800dddb0: sltu	at,t6,t7
  800dddb4: beqz	at,0x800dddc0
  800dddb8: li	t0,1
  800dddbc: sb	t0,0(s1)
  800dddc0: lw	t8,4(s0)
  800dddc4: move	s5,s3
  800dddc8: move	s6,s4
  800dddcc: lw	s7,484(t8)
  800dddd0: sw	s4,1892(sp)
  800dddd4: jal	0x800dc88c   <attract_mode_handler>
  800dddd8: sw	s1,88(sp)
  800ddddc: lw	s1,88(sp)
  800ddde0: b	0x800dddf0
  800ddde4: lw	s4,1892(sp)
  800ddde8: li	t0,1
  800dddec: sb	t0,0(s1)
  800dddf0: lb	t9,0(s1)
  800dddf4: lw	t6,1908(sp)
  800dddf8: sb	t9,0(t6)
  800dddfc: lb	t7,0(s1)
  800dde00: beqzl	t7,0x800dde78
  800dde04: lw	ra,76(sp)
  800dde08: lb	t8,1(s1)
  800dde0c: lw	t9,1900(sp)
  800dde10: sw	zero,20(s1)
  800dde14: sb	t8,0(t9)
  800dde18: lw	t7,1904(sp)
  800dde1c: lb	t6,2(s1)
  800dde20: sb	t6,0(t7)
  800dde24: sll	t9,s4,0x2
  800dde28: subu	t9,t9,s4
  800dde2c: lw	t8,92(sp)
  800dde30: sll	t9,t9,0x2
  800dde34: subu	t9,t9,s4
  800dde38: sll	t9,t9,0x2
  800dde3c: move	s0,zero
  800dde40: addu	s1,t8,t9
  800dde44: move	a0,s0
  800dde48: jal	0x800c9158   <player_state_set>
  800dde4c: lb	a1,4(s1)
  800dde50: move	a0,s0
  800dde54: jal	0x800c84c0   <player_mode_set>
  800dde58: lb	a1,8(s1)
  800dde5c: addiu	s0,s0,1
  800dde60: li	at,4
  800dde64: bne	s0,at,0x800dde44
  800dde68: addiu	s1,s1,1
  800dde6c: jal	0x800c997c   <process_inputs>
  800dde70: nop
  800dde74: lw	ra,76(sp)
  800dde78: lw	s0,40(sp)
  800dde7c: lw	s1,44(sp)
  800dde80: lw	s2,48(sp)
  800dde84: lw	s3,52(sp)
  800dde88: lw	s4,56(sp)
  800dde8c: lw	s5,60(sp)
  800dde90: lw	s6,64(sp)
  800dde94: lw	s7,68(sp)
  800dde98: lw	s8,72(sp)
  800dde9c: jr	ra
  800ddea0: addiu	sp,sp,1896
