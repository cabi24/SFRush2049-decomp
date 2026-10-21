== highscore_animText @ 0x80105480 (445 words)
  80105480: addiu	sp,sp,-584
  80105484: sw	ra,76(sp)
  80105488: mtc1	zero,$f12
  8010548c: sw	s8,72(sp)
  80105490: sw	s7,68(sp)
  80105494: sw	s6,64(sp)
  80105498: sw	s5,60(sp)
  8010549c: sw	s4,56(sp)
  801054a0: sw	s3,52(sp)
  801054a4: sw	s2,48(sp)
  801054a8: sw	s1,44(sp)
  801054ac: sw	s0,40(sp)
  801054b0: sdc1	$f22,32(sp)
  801054b4: sdc1	$f20,24(sp)
  801054b8: jal	0x800b65b4   <render_helper>
  801054bc: sw	a0,584(sp)
  801054c0: lui	t6,0x8014
  801054c4: lw	t6,2848(t6)
  801054c8: addiu	t7,t6,-1
  801054cc: sltiu	at,t7,5
  801054d0: beqz	at,0x80105b28
  801054d4: sll	t7,t7,0x2
  801054d8: lui	at,0x8012
  801054dc: addu	at,at,t7
  801054e0: lw	t7,18592(at)
  801054e4: jr	t7
  801054e8: nop
  801054ec: jal	0x80103d28   <DrawUnlocked>
  801054f0: nop
  801054f4: b	0x80105b2c
  801054f8: lui	at,0xbf80
  801054fc: lui	s7,0x8014
  80105500: addiu	s7,s7,25040
  80105504: move	a0,s7
  80105508: move	a1,zero
  8010550c: jal	0x80007270   <osRecvMesg>
  80105510: li	a2,1
  80105514: jal	0x800b4200   <slot_state_setup>
  80105518: li	s2,11
  8010551c: move	s0,v0
  80105520: move	a0,s7
  80105524: move	a1,zero
  80105528: jal	0x800075e0   <osSendMesg>
  8010552c: move	a2,zero
  80105530: jal	0x800b74a0   <dispatch_handler>
  80105534: li	a0,1
  80105538: lui	t8,0x8018
  8010553c: lw	t8,-23324(t8)
  80105540: li	a1,-1
  80105544: jal	0x800b3fa4   <object_manager_update,object_utility>
  80105548: lw	a0,256(t8)
  8010554c: li	s0,160
  80105550: srl	t9,v0,0x1
  80105554: subu	t7,s0,t9
  80105558: lui	t9,0x8018
  8010555c: lw	t9,-23324(t9)
  80105560: sll	a0,t7,0x10
  80105564: sra	t8,a0,0x10
  80105568: move	a0,t8
  8010556c: li	a1,80
  80105570: jal	0x800b71d4   <state_utility>
  80105574: lw	a2,256(t9)
  80105578: lui	t6,0x8018
  8010557c: lw	t6,-23324(t6)
  80105580: li	a1,-1
  80105584: jal	0x800b3fa4   <object_manager_update,object_utility>
  80105588: lw	a0,260(t6)
  8010558c: srl	t7,v0,0x1
  80105590: subu	t9,s0,t7
  80105594: lui	t7,0x8018
  80105598: lw	t7,-23324(t7)
  8010559c: sll	a0,t9,0x10
  801055a0: sra	t6,a0,0x10
  801055a4: move	a0,t6
  801055a8: li	a1,120
  801055ac: jal	0x800b71d4   <state_utility>
  801055b0: lw	a2,260(t7)
  801055b4: lui	s0,0x8014
  801055b8: addiu	s0,s0,16112
  801055bc: jal	0x800be744   <func_800BE744>
  801055c0: move	a0,s0
  801055c4: sltiu	at,v0,23
  801055c8: bnez	at,0x801055d8
  801055cc: nop
  801055d0: b	0x801055e4
  801055d4: li	s8,22
  801055d8: jal	0x800be744   <func_800BE744>
  801055dc: move	a0,s0
  801055e0: move	s8,v0
  801055e4: addiu	s6,s8,1
  801055e8: li	t7,160
  801055ec: sb	zero,497(sp)
  801055f0: bgez	s6,0x80105600
  801055f4: sra	t8,s6,0x1
  801055f8: addiu	at,s6,1
  801055fc: sra	t8,at,0x1
  80105600: sll	t9,t8,0x2
  80105604: addu	t9,t9,t8
  80105608: sll	t9,t9,0x1
  8010560c: move	s6,t8
  80105610: bgez	t9,0x80105620
  80105614: sra	t6,t9,0x1
  80105618: addiu	at,t9,1
  8010561c: sra	t6,at,0x1
  80105620: subu	s7,t7,t6
  80105624: move	v1,s7
  80105628: li	s4,95
  8010562c: blez	s8,0x80105b28
  80105630: move	a3,zero
  80105634: lui	s0,0x8014
  80105638: addiu	s0,s0,16112
  8010563c: addiu	s5,sp,496
  80105640: lbu	t8,0(s0)
  80105644: sll	t9,s4,0x10
  80105648: sra	t7,t9,0x10
  8010564c: sw	t7,88(sp)
  80105650: move	a0,s5
  80105654: li	a1,-1
  80105658: sw	v1,572(sp)
  8010565c: sw	a3,564(sp)
  80105660: jal	0x800b3fa4   <object_manager_update,object_utility>
  80105664: sb	t8,496(sp)
  80105668: lw	v1,572(sp)
  8010566c: srl	t7,v0,0x1
  80105670: lw	a3,564(sp)
  80105674: addiu	t9,v1,5
  80105678: subu	t8,t9,t7
  8010567c: sll	a0,t8,0x10
  80105680: sra	t9,a0,0x10
  80105684: move	a0,t9
  80105688: lh	a1,90(sp)
  8010568c: move	a2,s5
  80105690: sw	v1,572(sp)
  80105694: jal	0x800b71d4   <state_utility>
  80105698: sw	a3,564(sp)
  8010569c: lw	a3,564(sp)
  801056a0: lw	v1,572(sp)
  801056a4: addiu	v0,a3,1
  801056a8: div	zero,v0,s6
  801056ac: mfhi	t7
  801056b0: move	a3,v0
  801056b4: bnez	s6,0x801056c0
  801056b8: nop
  801056bc: break	0x7
  801056c0: li	at,-1
  801056c4: bne	s6,at,0x801056d8
  801056c8: lui	at,0x8000
  801056cc: bne	v0,at,0x801056d8
  801056d0: nop
  801056d4: break	0x6
  801056d8: addiu	v1,v1,10
  801056dc: bnez	t7,0x801056f0
  801056e0: nop
  801056e4: move	v1,s7
  801056e8: b	0x801056f0
  801056ec: addiu	s4,s4,10
  801056f0: bne	v0,s8,0x80105640
  801056f4: addiu	s0,s0,1
  801056f8: b	0x80105b2c
  801056fc: lui	at,0xbf80
  80105700: lui	v1,0x8015
  80105704: lh	v1,-24312(v1)
  80105708: lui	v0,0x8014
  8010570c: addiu	v0,v0,16152
  80105710: blez	v1,0x80105b28
  80105714: move	a0,zero
  80105718: lb	t6,0(v0)
  8010571c: li	at,1
  80105720: move	s5,a0
  80105724: bnel	t6,at,0x8010574c
  80105728: addiu	a0,a0,1
  8010572c: sw	v0,96(sp)
  80105730: jal	0x80104704   <DrawName>
  80105734: sw	a0,580(sp)
  80105738: lui	v1,0x8015
  8010573c: lh	v1,-24312(v1)
  80105740: lw	v0,96(sp)
  80105744: lw	a0,580(sp)
  80105748: addiu	a0,a0,1
  8010574c: slt	at,a0,v1
  80105750: bnez	at,0x80105718
  80105754: addiu	v0,v0,1
  80105758: b	0x80105b2c
  8010575c: lui	at,0xbf80
  80105760: jal	0x80104b14   <DrawGhost>
  80105764: nop
  80105768: b	0x80105b2c
  8010576c: lui	at,0xbf80
  80105770: lui	s7,0x8014
  80105774: addiu	s7,s7,25040
  80105778: move	a0,s7
  8010577c: move	a1,zero
  80105780: jal	0x80007270   <osRecvMesg>
  80105784: li	a2,1
  80105788: jal	0x800b4200   <slot_state_setup>
  8010578c: li	s2,13
  80105790: move	s0,v0
  80105794: move	a0,s7
  80105798: move	a1,zero
  8010579c: jal	0x800075e0   <osSendMesg>
  801057a0: move	a2,zero
  801057a4: jal	0x800b74a0   <dispatch_handler>
  801057a8: li	a0,1
  801057ac: lui	v0,0x8018
  801057b0: addiu	v0,v0,-23328
  801057b4: lw	t9,12(v0)
  801057b8: lw	t8,16(v0)
  801057bc: li	a1,-1
  801057c0: lhu	t7,54(t9)
  801057c4: sll	t6,t7,0x2
  801057c8: lui	t7,0x8011
  801057cc: lb	t7,18640(t7)
  801057d0: addu	t9,t8,t6
  801057d4: sll	t8,t7,0x2
  801057d8: addu	t6,t9,t8
  801057dc: jal	0x800b3fa4   <object_manager_update,object_utility>
  801057e0: lw	a0,0(t6)
  801057e4: lui	v1,0x8018
  801057e8: addiu	v1,v1,-23328
  801057ec: lw	t9,12(v1)
  801057f0: li	s0,160
  801057f4: srl	t7,v0,0x1
  801057f8: subu	t8,s0,t7
  801057fc: sll	a0,t8,0x10
  80105800: lhu	t8,54(t9)
  80105804: sra	t6,a0,0x10
  80105808: move	a0,t6
  8010580c: sll	t6,t8,0x2
  80105810: lw	t7,16(v1)
  80105814: lui	t8,0x8011
  80105818: lb	t8,18640(t8)
  8010581c: addu	t9,t7,t6
  80105820: li	a1,10
  80105824: sll	t7,t8,0x2
  80105828: addu	t6,t9,t7
  8010582c: jal	0x800b71d4   <state_utility>
  80105830: lw	a2,0(t6)
  80105834: li	t8,78
  80105838: sw	t8,88(sp)
  8010583c: move	s4,zero
  80105840: li	s8,76
  80105844: sw	zero,84(sp)
  80105848: addiu	s6,sp,392
  8010584c: lui	a1,0x8012
  80105850: addiu	s5,s4,1
  80105854: move	a2,s5
  80105858: addiu	a1,a1,1652
  8010585c: jal	0x80004990   <sprintf>
  80105860: move	a0,s6
  80105864: move	a0,s7
  80105868: move	a1,zero
  8010586c: jal	0x80007270   <osRecvMesg>
  80105870: li	a2,1
  80105874: jal	0x800b4200   <slot_state_setup>
  80105878: li	s2,5
  8010587c: move	s0,v0
  80105880: move	a0,s7
  80105884: move	a1,zero
  80105888: jal	0x800075e0   <osSendMesg>
  8010588c: move	a2,zero
  80105890: lui	t9,0x8011
  80105894: lb	t9,18640(t9)
  80105898: lui	v0,0x8015
  8010589c: li	at,-1
  801058a0: sll	t7,t9,0x2
  801058a4: addu	t7,t7,t9
  801058a8: addu	t6,t7,s4
  801058ac: addu	v0,v0,t6
  801058b0: lb	v0,6848(v0)
  801058b4: lui	a0,0x8011
  801058b8: bne	v0,at,0x801058c8
  801058bc: sll	t8,v0,0x2
  801058c0: b	0x801058d0
  801058c4: li	a0,1
  801058c8: addu	a0,a0,t8
  801058cc: lw	a0,18092(a0)
  801058d0: jal	0x800b74a0   <dispatch_handler>
  801058d4: nop
  801058d8: move	a0,s6
  801058dc: jal	0x800b3fa4   <object_manager_update,object_utility>
  801058e0: li	a1,-1
  801058e4: sll	a1,s8,0x10
  801058e8: sra	t9,a1,0x10
  801058ec: move	s4,v0
  801058f0: move	a1,t9
  801058f4: li	a0,16
  801058f8: jal	0x800b71d4   <state_utility>
  801058fc: move	a2,s6
  80105900: move	a0,s7
  80105904: move	a1,zero
  80105908: jal	0x80007270   <osRecvMesg>
  8010590c: li	a2,1
  80105910: jal	0x800b4200   <slot_state_setup>
  80105914: li	s2,11
  80105918: move	s0,v0
  8010591c: move	a0,s7
  80105920: move	a1,zero
  80105924: jal	0x800075e0   <osSendMesg>
  80105928: move	a2,zero
  8010592c: lui	v0,0x8018
  80105930: addiu	v0,v0,-23328
  80105934: lw	t9,12(v0)
  80105938: addiu	a0,s4,16
  8010593c: sll	t7,a0,0x10
  80105940: sra	a0,t7,0x10
  80105944: lhu	t7,110(t9)
  80105948: lw	t8,16(v0)
  8010594c: lh	a1,90(sp)
  80105950: sll	t6,t7,0x2
  80105954: lw	t7,84(sp)
  80105958: addu	t9,t8,t6
  8010595c: addu	t8,t9,t7
  80105960: jal	0x800b71d4   <state_utility>
  80105964: lw	a2,0(t8)
  80105968: lw	t6,88(sp)
  8010596c: lw	t7,84(sp)
  80105970: slti	at,s5,5
  80105974: addiu	t9,t6,24
  80105978: addiu	t8,t7,4
  8010597c: sw	t8,84(sp)
  80105980: sw	t9,88(sp)
  80105984: move	s4,s5
  80105988: bnez	at,0x8010584c
  8010598c: addiu	s8,s8,24
  80105990: lui	s6,0x8014
  80105994: addiu	s6,s6,16352
  80105998: move	s4,zero
  8010599c: li	s5,80
  801059a0: addiu	s8,sp,136
  801059a4: move	a0,s7
  801059a8: move	a1,zero
  801059ac: jal	0x80007270   <osRecvMesg>
  801059b0: li	a2,1
  801059b4: jal	0x800b4200   <slot_state_setup>
  801059b8: li	s2,10
  801059bc: move	s0,v0
  801059c0: move	a0,s7
  801059c4: move	a1,zero
  801059c8: jal	0x800075e0   <osSendMesg>
  801059cc: move	a2,zero
  801059d0: lui	t6,0x8011
  801059d4: lb	t6,18640(t6)
  801059d8: lui	v0,0x8015
  801059dc: li	at,-1
  801059e0: sll	t9,t6,0x2
  801059e4: addu	t9,t9,t6
  801059e8: addu	t7,t9,s4
  801059ec: addu	v0,v0,t7
  801059f0: lb	v0,6848(v0)
  801059f4: lui	a0,0x8011
  801059f8: bne	v0,at,0x80105a08
  801059fc: sll	t8,v0,0x2
  80105a00: b	0x80105a10
  80105a04: li	a0,1
  80105a08: addu	a0,a0,t8
  80105a0c: lw	a0,18092(a0)
  80105a10: jal	0x800b74a0   <dispatch_handler>
  80105a14: nop
  80105a18: lw	a1,0(s6)
  80105a1c: move	a0,s8
  80105a20: jal	0x800be7bc   <race_position_update>
  80105a24: li	a2,160
  80105a28: sll	a1,s5,0x10
  80105a2c: sra	t6,a1,0x10
  80105a30: move	a1,t6
  80105a34: li	a0,69
  80105a38: jal	0x800b71d4   <state_utility>
  80105a3c: move	a2,s8
  80105a40: addiu	s4,s4,1
  80105a44: slti	at,s4,5
  80105a48: addiu	s5,s5,24
  80105a4c: bnez	at,0x801059a4
  80105a50: addiu	s6,s6,4
  80105a54: lui	s0,0x8014
  80105a58: addiu	s0,s0,16160
  80105a5c: move	s4,zero
  80105a60: li	s5,80
  80105a64: li	s6,304
  80105a68: li	s8,5
  80105a6c: move	a0,s7
  80105a70: move	a1,zero
  80105a74: jal	0x80007270   <osRecvMesg>
  80105a78: li	a2,1
  80105a7c: li	s2,2
  80105a80: jal	0x800b4200   <slot_state_setup>
  80105a84: sw	s0,92(sp)
  80105a88: lw	s0,92(sp)
  80105a8c: sw	v0,124(sp)
  80105a90: move	a0,s7
  80105a94: move	a1,zero
  80105a98: jal	0x800075e0   <osSendMesg>
  80105a9c: move	a2,zero
  80105aa0: lui	t9,0x8011
  80105aa4: lb	t9,18640(t9)
  80105aa8: lui	v0,0x8015
  80105aac: li	at,-1
  80105ab0: sll	t7,t9,0x2
  80105ab4: addu	t7,t7,t9
  80105ab8: addu	t8,t7,s4
  80105abc: addu	v0,v0,t8
  80105ac0: lb	v0,6848(v0)
  80105ac4: lui	a0,0x8011
  80105ac8: bne	v0,at,0x80105ad8
  80105acc: sll	t6,v0,0x2
  80105ad0: b	0x80105ae0
  80105ad4: li	a0,1
  80105ad8: addu	a0,a0,t6
  80105adc: lw	a0,18092(a0)
  80105ae0: jal	0x800b74a0   <dispatch_handler>
  80105ae4: nop
  80105ae8: move	a0,s0
  80105aec: jal	0x800b3fa4   <object_manager_update,object_utility>
  80105af0: li	a1,-1
  80105af4: subu	t7,s6,v0
  80105af8: sll	a0,t7,0x10
  80105afc: sll	a1,s5,0x10
  80105b00: sra	t6,a1,0x10
  80105b04: sra	t8,a0,0x10
  80105b08: move	a0,t8
  80105b0c: move	a1,t6
  80105b10: jal	0x800b71d4   <state_utility>
  80105b14: move	a2,s0
  80105b18: addiu	s4,s4,1
  80105b1c: addiu	s5,s5,24
  80105b20: bne	s4,s8,0x80105a6c
  80105b24: addiu	s0,s0,10
  80105b28: lui	at,0xbf80
  80105b2c: mtc1	at,$f12
  80105b30: jal	0x800b65b4   <render_helper>
  80105b34: nop
  80105b38: lw	ra,76(sp)
  80105b3c: ldc1	$f20,24(sp)
  80105b40: ldc1	$f22,32(sp)
  80105b44: lw	s0,40(sp)
  80105b48: lw	s1,44(sp)
  80105b4c: lw	s2,48(sp)
  80105b50: lw	s3,52(sp)
  80105b54: lw	s4,56(sp)
  80105b58: lw	s5,60(sp)
  80105b5c: lw	s6,64(sp)
  80105b60: lw	s7,68(sp)
  80105b64: lw	s8,72(sp)
  80105b68: addiu	sp,sp,584
  80105b6c: jr	ra
  80105b70: li	v0,1
