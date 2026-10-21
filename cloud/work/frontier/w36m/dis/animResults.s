== animResults @ 0x80105EA8 (627 words)
  80105ea8: addiu	sp,sp,-400
  80105eac: sw	s8,80(sp)
  80105eb0: lui	s8,0x8015
  80105eb4: addiu	s8,s8,6864
  80105eb8: sw	a0,400(sp)
  80105ebc: lh	a0,0(s8)
  80105ec0: sw	s6,72(sp)
  80105ec4: sw	s5,68(sp)
  80105ec8: sw	s4,64(sp)
  80105ecc: sw	ra,84(sp)
  80105ed0: sw	s7,76(sp)
  80105ed4: sw	s3,60(sp)
  80105ed8: sw	s2,56(sp)
  80105edc: sw	s1,52(sp)
  80105ee0: sw	s0,48(sp)
  80105ee4: sdc1	$f20,40(sp)
  80105ee8: move	s4,zero
  80105eec: move	s5,zero
  80105ef0: blez	a0,0x80105f6c
  80105ef4: move	s6,zero
  80105ef8: sll	t7,a0,0x8
  80105efc: addu	t7,t7,a0
  80105f00: lui	t6,0x8015
  80105f04: addiu	s3,t6,-23984
  80105f08: sll	t7,t7,0x3
  80105f0c: lui	a3,0x8015
  80105f10: lui	v1,0x8015
  80105f14: addiu	v1,v1,10264
  80105f18: lh	a3,10036(a3)
  80105f1c: addu	v0,t7,s3
  80105f20: li	a0,1
  80105f24: li	a1,952
  80105f28: lb	t8,2024(s3)
  80105f2c: bnel	a3,t8,0x80105f3c
  80105f30: lh	t9,1990(s3)
  80105f34: move	s5,a0
  80105f38: lh	t9,1990(s3)
  80105f3c: addiu	s3,s3,2056
  80105f40: sltu	at,s3,v0
  80105f44: multu	t9,a1
  80105f48: mflo	t7
  80105f4c: addu	t6,v1,t7
  80105f50: lb	t8,239(t6)
  80105f54: bne	a0,t8,0x80105f60
  80105f58: nop
  80105f5c: addiu	s4,s4,1
  80105f60: bnezl	at,0x80105f2c
  80105f64: lb	t8,2024(s3)
  80105f68: move	s6,zero
  80105f6c: mtc1	zero,$f12
  80105f70: jal	0x800b65b4   <render_helper>
  80105f74: nop
  80105f78: lui	v1,0x8012
  80105f7c: lui	s7,0x8014
  80105f80: addiu	s7,s7,25040
  80105f84: addiu	v1,v1,-29152
  80105f88: li	a2,1
  80105f8c: sw	a2,0(v1)
  80105f90: sw	a2,4(v1)
  80105f94: move	a0,s7
  80105f98: jal	0x80007270   <osRecvMesg>
  80105f9c: move	a1,zero
  80105fa0: jal	0x800b4200   <slot_state_setup>
  80105fa4: li	s2,10
  80105fa8: move	s0,v0
  80105fac: move	a0,s7
  80105fb0: move	a1,zero
  80105fb4: jal	0x800075e0   <osSendMesg>
  80105fb8: move	a2,zero
  80105fbc: lh	a0,0(s8)
  80105fc0: slti	at,a0,2
  80105fc4: bnez	at,0x80106158
  80105fc8: nop
  80105fcc: bne	s4,a0,0x80106158
  80105fd0: lui	v0,0x8014
  80105fd4: lb	v0,10080(v0)
  80105fd8: beqz	v0,0x80105ff0
  80105fdc: nop
  80105fe0: beqz	v0,0x80106158
  80105fe4: nop
  80105fe8: beqz	s5,0x80106158
  80105fec: nop
  80105ff0: lui	s0,0x8015
  80105ff4: addiu	s0,s0,17232
  80105ff8: lw	t9,16(s0)
  80105ffc: bnez	t9,0x80106050
  80106000: nop
  80106004: jal	0x800b3f50   <object_bytes_sum_global,object_helper>
  80106008: nop
  8010600c: li	t7,106
  80106010: li	t6,192
  80106014: li	t8,1
  80106018: sw	t8,28(sp)
  8010601c: sw	t6,16(sp)
  80106020: li	a0,91
  80106024: bgez	v0,0x80106034
  80106028: sra	v1,v0,0x1
  8010602c: addiu	at,v0,1
  80106030: sra	v1,at,0x1
  80106034: subu	a1,t7,v1
  80106038: addiu	a3,v1,114
  8010603c: li	a2,229
  80106040: sw	zero,20(sp)
  80106044: jal	0x800b3b4c   <ambient_sound_set>
  80106048: sw	zero,24(sp)
  8010604c: sw	v0,16(s0)
  80106050: jal	0x800b74a0   <dispatch_handler>
  80106054: li	a0,22
  80106058: lui	v0,0x8014
  8010605c: lb	v0,16212(v0)
  80106060: li	v1,2056
  80106064: lui	a3,0x8015
  80106068: multu	v0,v1
  8010606c: addiu	a3,a3,-23984
  80106070: li	at,1
  80106074: lui	s4,0x8018
  80106078: addiu	s4,s4,-23328
  8010607c: addiu	s7,sp,260
  80106080: mflo	t9
  80106084: addu	t7,a3,t9
  80106088: lh	t6,1990(t7)
  8010608c: multu	t6,v1
  80106090: mflo	t8
  80106094: addu	t9,a3,t8
  80106098: lb	t7,1996(t9)
  8010609c: bne	t7,at,0x801060bc
  801060a0: nop
  801060a4: lw	t6,4(s4)
  801060a8: move	a0,s7
  801060ac: jal	0x800b4360   <fcvt_wrapper>
  801060b0: lw	a1,800(t6)
  801060b4: b	0x80106138
  801060b8: li	t8,-1
  801060bc: lui	v1,0x8015
  801060c0: lw	v1,2912(v1)
  801060c4: beqz	v1,0x801060f0
  801060c8: lui	s4,0x8018
  801060cc: addiu	s4,s4,-23328
  801060d0: lw	t8,4(s4)
  801060d4: addiu	s7,sp,260
  801060d8: move	a0,s7
  801060dc: addiu	a2,v1,1
  801060e0: jal	0x800b4360   <fcvt_wrapper>
  801060e4: lw	a1,804(t8)
  801060e8: b	0x80106138
  801060ec: li	t8,-1
  801060f0: sll	t7,v0,0x2
  801060f4: addu	t7,t7,v0
  801060f8: sll	t7,t7,0x2
  801060fc: subu	t7,t7,v0
  80106100: sll	t7,t7,0x2
  80106104: lui	t6,0x8015
  80106108: addu	t6,t6,t7
  8010610c: lw	t6,-24224(t6)
  80106110: lui	s4,0x8018
  80106114: addiu	s4,s4,-23328
  80106118: lw	t9,4(s4)
  8010611c: lw	a2,0(t6)
  80106120: addiu	s7,sp,260
  80106124: move	a0,s7
  80106128: lw	a1,796(t9)
  8010612c: jal	0x800b4360   <fcvt_wrapper>
  80106130: addiu	a2,a2,20
  80106134: li	t8,-1
  80106138: sw	t8,16(sp)
  8010613c: li	a0,160
  80106140: li	a1,110
  80106144: li	a2,160
  80106148: li	a3,110
  8010614c: sw	zero,20(sp)
  80106150: jal	0x800be078   <camera_auto_follow>
  80106154: sw	s7,24(sp)
  80106158: lui	s4,0x8018
  8010615c: addiu	s4,s4,-23328
  80106160: addiu	s7,sp,260
  80106164: jal	0x800b74a0   <dispatch_handler>
  80106168: li	a0,1
  8010616c: lh	a0,0(s8)
  80106170: blez	a0,0x80106818
  80106174: lui	at,0x447a
  80106178: lui	s3,0x8015
  8010617c: mtc1	at,$f20
  80106180: addiu	s3,s3,-23984
  80106184: lui	t9,0x8014
  80106188: lb	t9,10080(t9)
  8010618c: lui	t7,0x8015
  80106190: beqzl	t9,0x801061ac
  80106194: lh	t8,1990(s3)
  80106198: lh	t7,10036(t7)
  8010619c: lb	t6,2024(s3)
  801061a0: bnel	t7,t6,0x8010680c
  801061a4: addiu	s6,s6,1
  801061a8: lh	t8,1990(s3)
  801061ac: lui	t7,0x8015
  801061b0: li	at,1
  801061b4: sll	t9,t8,0x4
  801061b8: subu	t9,t9,t8
  801061bc: sll	t9,t9,0x3
  801061c0: subu	t9,t9,t8
  801061c4: sll	t9,t9,0x3
  801061c8: addu	t7,t7,t9
  801061cc: lb	t7,10503(t7)
  801061d0: sll	v1,s6,0x3
  801061d4: sll	t6,a0,0x5
  801061d8: bne	t7,at,0x80106808
  801061dc: addu	t8,t6,v1
  801061e0: lui	t9,0x8011
  801061e4: addiu	t9,t9,24360
  801061e8: addu	v0,t8,t9
  801061ec: sll	t8,s6,0x2
  801061f0: addu	t8,t8,s6
  801061f4: sll	t8,t8,0x2
  801061f8: subu	t8,t8,s6
  801061fc: lui	t9,0x8015
  80106200: lui	t6,0x8015
  80106204: addiu	t6,t6,17232
  80106208: addiu	t9,t9,-24296
  8010620c: sll	t8,t8,0x2
  80106210: sll	t7,s6,0x2
  80106214: li	at,4
  80106218: lw	s8,-32(v0)
  8010621c: lw	s5,-28(v0)
  80106220: addu	s2,t7,t6
  80106224: bne	a0,at,0x8010623c
  80106228: addu	s1,t8,t9
  8010622c: slti	at,s6,2
  80106230: bnezl	at,0x80106240
  80106234: lw	t7,0(s2)
  80106238: addiu	s5,s5,12
  8010623c: lw	t7,0(s2)
  80106240: bnez	t7,0x801062c8
  80106244: nop
  80106248: lui	t6,0x8011
  8010624c: addiu	t6,t6,24904
  80106250: addu	s0,v1,t6
  80106254: addiu	t8,s0,4
  80106258: jal	0x800b3f50   <object_bytes_sum_global,object_helper>
  8010625c: sw	t8,92(sp)
  80106260: sll	v1,v0,0x2
  80106264: subu	v1,v1,v0
  80106268: li	t7,192
  8010626c: sll	t6,s6,0x4
  80106270: li	t8,1
  80106274: sw	t8,28(sp)
  80106278: sw	t6,24(sp)
  8010627c: sw	t7,16(sp)
  80106280: addiu	a0,s8,-69
  80106284: bgez	v1,0x80106294
  80106288: sra	t9,v1,0x1
  8010628c: addiu	at,v1,1
  80106290: sra	t9,at,0x1
  80106294: subu	a1,s5,t9
  80106298: addu	a3,t9,s5
  8010629c: addiu	a3,a3,4
  801062a0: addiu	a1,a1,-4
  801062a4: move	v1,t9
  801062a8: addiu	a2,s8,69
  801062ac: jal	0x800b3b4c   <ambient_sound_set>
  801062b0: sw	zero,20(sp)
  801062b4: sw	v0,0(s2)
  801062b8: move	a0,v0
  801062bc: move	a1,s0
  801062c0: jal	0x800dc794   <tournament_unlock_check,unlock_notification>
  801062c4: lw	a2,92(sp)
  801062c8: lui	v0,0x8015
  801062cc: lw	v0,-24304(v0)
  801062d0: li	at,4
  801062d4: sll	t7,s6,0x4
  801062d8: bne	v0,at,0x80106320
  801062dc: subu	t7,t7,s6
  801062e0: lw	t9,72(s1)
  801062e4: lw	t6,4(s4)
  801062e8: sll	t7,t7,0x3
  801062ec: lw	a2,0(t9)
  801062f0: lw	t8,520(t6)
  801062f4: lui	a3,0x8015
  801062f8: addu	a3,a3,t7
  801062fc: lui	a1,0x8012
  80106300: addiu	a1,a1,3516
  80106304: lw	a3,8268(a3)
  80106308: move	a0,s7
  8010630c: addiu	a2,a2,20
  80106310: jal	0x800b4360   <fcvt_wrapper>
  80106314: sw	t8,16(sp)
  80106318: b	0x801067d0
  8010631c: sll	a0,s8,0x10
  80106320: li	at,5
  80106324: bne	v0,at,0x8010651c
  80106328: lui	t9,0x8014
  8010632c: addu	t9,t9,s6
  80106330: lbu	t9,16408(t9)
  80106334: li	at,1
  80106338: li	t0,1
  8010633c: beq	t9,at,0x80106370
  80106340: li	v1,1
  80106344: lw	t7,72(s1)
  80106348: lw	t6,4(s4)
  8010634c: lui	a1,0x8012
  80106350: lw	a2,0(t7)
  80106354: addiu	a1,a1,3528
  80106358: move	a0,s7
  8010635c: lw	a3,856(t6)
  80106360: jal	0x800b4360   <fcvt_wrapper>
  80106364: addiu	a2,a2,20
  80106368: b	0x801067d0
  8010636c: sll	a0,s8,0x10
  80106370: lbu	t8,0(s1)
  80106374: lui	at,0x8015
  80106378: cfc1	t7,c1_fcsr
  8010637c: sll	t9,t8,0x4
  80106380: subu	t9,t9,t8
  80106384: sll	t9,t9,0x3
  80106388: subu	t9,t9,t8
  8010638c: sll	t9,t9,0x3
  80106390: addu	at,at,t9
  80106394: ctc1	t0,c1_fcsr
  80106398: lwc1	$f0,10504(at)
  8010639c: lui	a1,0x8012
  801063a0: li	t2,60
  801063a4: cvt.w.s	$f4,$f0
  801063a8: addiu	a1,a1,3536
  801063ac: move	a0,s7
  801063b0: cfc1	t0,c1_fcsr
  801063b4: nop
  801063b8: andi	at,t0,0x4
  801063bc: andi	t0,t0,0x78
  801063c0: beqzl	t0,0x80106418
  801063c4: mfc1	t0,$f4
  801063c8: lui	at,0x4f00
  801063cc: mtc1	at,$f4
  801063d0: li	t0,1
  801063d4: sub.s	$f4,$f0,$f4
  801063d8: ctc1	t0,c1_fcsr
  801063dc: nop
  801063e0: cvt.w.s	$f4,$f4
  801063e4: cfc1	t0,c1_fcsr
  801063e8: nop
  801063ec: andi	at,t0,0x4
  801063f0: andi	t0,t0,0x78
  801063f4: bnez	t0,0x8010640c
  801063f8: nop
  801063fc: mfc1	t0,$f4
  80106400: lui	at,0x8000
  80106404: b	0x80106424
  80106408: or	t0,t0,at
  8010640c: b	0x80106424
  80106410: li	t0,-1
  80106414: mfc1	t0,$f4
  80106418: nop
  8010641c: bltz	t0,0x8010640c
  80106420: nop
  80106424: divu	zero,t0,t2
  80106428: move	v0,t0
  8010642c: mflo	v0
  80106430: li	at,100
  80106434: ctc1	t7,c1_fcsr
  80106438: divu	zero,v0,at
  8010643c: mfhi	t6
  80106440: mul.s	$f6,$f0,$f20
  80106444: bnez	t2,0x80106450
  80106448: nop
  8010644c: break	0x7
  80106450: divu	zero,t0,t2
  80106454: cfc1	t8,c1_fcsr
  80106458: ctc1	v1,c1_fcsr
  8010645c: mfhi	t1
  80106460: move	v0,t6
  80106464: cvt.w.s	$f8,$f6
  80106468: bnez	t2,0x80106474
  8010646c: nop
  80106470: break	0x7
  80106474: cfc1	v1,c1_fcsr
  80106478: nop
  8010647c: andi	at,v1,0x4
  80106480: andi	v1,v1,0x78
  80106484: beqz	v1,0x801064d0
  80106488: lui	at,0x4f00
  8010648c: mtc1	at,$f8
  80106490: li	v1,1
  80106494: sub.s	$f8,$f6,$f8
  80106498: ctc1	v1,c1_fcsr
  8010649c: nop
  801064a0: cvt.w.s	$f8,$f8
  801064a4: cfc1	v1,c1_fcsr
  801064a8: nop
  801064ac: andi	v1,v1,0x78
  801064b0: bnez	v1,0x801064c8
  801064b4: nop
  801064b8: mfc1	v1,$f8
  801064bc: lui	at,0x8000
  801064c0: b	0x801064e0
  801064c4: or	v1,v1,at
  801064c8: b	0x801064e0
  801064cc: li	v1,-1
  801064d0: mfc1	v1,$f8
  801064d4: nop
  801064d8: bltz	v1,0x801064c8
  801064dc: nop
  801064e0: li	at,1000
  801064e4: divu	zero,v1,at
  801064e8: lw	t7,72(s1)
  801064ec: lw	t6,4(s4)
  801064f0: ctc1	t8,c1_fcsr
  801064f4: lw	a2,0(t7)
  801064f8: lw	a3,832(t6)
  801064fc: mfhi	t9
  80106500: sw	t9,24(sp)
  80106504: sw	t1,20(sp)
  80106508: sw	v0,16(sp)
  8010650c: jal	0x800b4360   <fcvt_wrapper>
  80106510: addiu	a2,a2,20
  80106514: b	0x801067d0
  80106518: sll	a0,s8,0x10
  8010651c: li	at,6
  80106520: bne	v0,at,0x80106578
  80106524: addiu	a0,sp,200
  80106528: sll	t9,s6,0x4
  8010652c: lw	t8,72(s1)
  80106530: lw	t7,4(s4)
  80106534: subu	t9,t9,s6
  80106538: sll	t9,t9,0x3
  8010653c: subu	t9,t9,s6
  80106540: lw	a2,0(t8)
  80106544: lw	t6,520(t7)
  80106548: sll	t9,t9,0x3
  8010654c: lui	a3,0x8015
  80106550: addu	a3,a3,t9
  80106554: lui	a1,0x8012
  80106558: addiu	a1,a1,3556
  8010655c: lb	a3,11195(a3)
  80106560: move	a0,s7
  80106564: addiu	a2,a2,20
  80106568: jal	0x800b4360   <fcvt_wrapper>
  8010656c: sw	t6,16(sp)
  80106570: b	0x801067d0
  80106574: sll	a0,s8,0x10
  80106578: lbu	t8,0(s1)
  8010657c: lui	v0,0x8015
  80106580: lw	t7,4(s4)
  80106584: sll	t9,t8,0x4
  80106588: subu	t9,t9,t8
  8010658c: sll	t9,t9,0x3
  80106590: subu	t9,t9,t8
  80106594: lw	t8,12(s4)
  80106598: sll	t9,t9,0x3
  8010659c: addu	v0,v0,t9
  801065a0: lhu	t9,110(t8)
  801065a4: lb	v0,10502(v0)
  801065a8: lw	t6,16(s4)
  801065ac: lw	a1,792(t7)
  801065b0: sll	t7,t9,0x2
  801065b4: sll	t9,v0,0x2
  801065b8: addu	t8,t6,t7
  801065bc: addu	t6,t8,t9
  801065c0: lw	a3,0(t6)
  801065c4: jal	0x800b4360   <fcvt_wrapper>
  801065c8: addiu	a2,v0,1
  801065cc: lbu	t1,0(s1)
  801065d0: lui	a2,0x8015
  801065d4: lui	a3,0x8015
  801065d8: sll	t7,t1,0x8
  801065dc: addu	t7,t7,t1
  801065e0: sll	t7,t7,0x3
  801065e4: addu	a2,a2,t7
  801065e8: lb	a2,-21960(a2)
  801065ec: lh	a3,10036(a3)
  801065f0: sll	t9,t1,0x4
  801065f4: subu	t9,t9,t1
  801065f8: beq	a3,a2,0x80106618
  801065fc: sll	t9,t9,0x3
  80106600: lw	t8,4(s4)
  80106604: addiu	a0,sp,168
  80106608: jal	0x800b4360   <fcvt_wrapper>
  8010660c: lw	a1,828(t8)
  80106610: b	0x801067a8
  80106614: lw	t7,72(s1)
  80106618: subu	t9,t9,t1
  8010661c: sll	t9,t9,0x3
  80106620: lui	at,0x8015
  80106624: addu	at,at,t9
  80106628: cfc1	t6,c1_fcsr
  8010662c: li	t0,1
  80106630: ctc1	t0,c1_fcsr
  80106634: lwc1	$f0,10504(at)
  80106638: lui	a1,0x8012
  8010663c: li	t2,60
  80106640: cvt.w.s	$f10,$f0
  80106644: li	v0,1
  80106648: addiu	a1,a1,3568
  8010664c: addiu	a0,sp,168
  80106650: cfc1	t0,c1_fcsr
  80106654: nop
  80106658: andi	at,t0,0x4
  8010665c: andi	t0,t0,0x78
  80106660: beqzl	t0,0x801066b8
  80106664: mfc1	t0,$f10
  80106668: lui	at,0x4f00
  8010666c: mtc1	at,$f10
  80106670: li	t0,1
  80106674: sub.s	$f10,$f0,$f10
  80106678: ctc1	t0,c1_fcsr
  8010667c: nop
  80106680: cvt.w.s	$f10,$f10
  80106684: cfc1	t0,c1_fcsr
  80106688: nop
  8010668c: andi	at,t0,0x4
  80106690: andi	t0,t0,0x78
  80106694: bnez	t0,0x801066ac
  80106698: nop
  8010669c: mfc1	t0,$f10
  801066a0: lui	at,0x8000
  801066a4: b	0x801066c4
  801066a8: or	t0,t0,at
  801066ac: b	0x801066c4
  801066b0: li	t0,-1
  801066b4: mfc1	t0,$f10
  801066b8: nop
  801066bc: bltz	t0,0x801066ac
  801066c0: nop
  801066c4: divu	zero,t0,t2
  801066c8: move	a3,t0
  801066cc: mflo	a3
  801066d0: li	at,100
  801066d4: ctc1	t6,c1_fcsr
  801066d8: divu	zero,a3,at
  801066dc: mfhi	t7
  801066e0: mul.s	$f4,$f0,$f20
  801066e4: bnez	t2,0x801066f0
  801066e8: nop
  801066ec: break	0x7
  801066f0: divu	zero,t0,t2
  801066f4: cfc1	t8,c1_fcsr
  801066f8: ctc1	v0,c1_fcsr
  801066fc: mfhi	v1
  80106700: move	a3,t7
  80106704: cvt.w.s	$f6,$f4
  80106708: bnez	t2,0x80106714
  8010670c: nop
  80106710: break	0x7
  80106714: cfc1	v0,c1_fcsr
  80106718: nop
  8010671c: andi	at,v0,0x4
  80106720: andi	v0,v0,0x78
  80106724: beqz	v0,0x80106770
  80106728: lui	at,0x4f00
  8010672c: mtc1	at,$f6
  80106730: li	v0,1
  80106734: sub.s	$f6,$f4,$f6
  80106738: ctc1	v0,c1_fcsr
  8010673c: nop
  80106740: cvt.w.s	$f6,$f6
  80106744: cfc1	v0,c1_fcsr
  80106748: nop
  8010674c: andi	v0,v0,0x78
  80106750: bnez	v0,0x80106768
  80106754: nop
  80106758: mfc1	v0,$f6
  8010675c: lui	at,0x8000
  80106760: b	0x80106780
  80106764: or	v0,v0,at
  80106768: b	0x80106780
  8010676c: li	v0,-1
  80106770: mfc1	v0,$f6
  80106774: nop
  80106778: bltz	v0,0x80106768
  8010677c: nop
  80106780: li	at,1000
  80106784: divu	zero,v0,at
  80106788: lw	t6,4(s4)
  8010678c: ctc1	t8,c1_fcsr
  80106790: mfhi	t9
  80106794: lw	a2,832(t6)
  80106798: sw	t9,20(sp)
  8010679c: jal	0x800b4360   <fcvt_wrapper>
  801067a0: sw	v1,16(sp)
  801067a4: lw	t7,72(s1)
  801067a8: lui	a1,0x8012
  801067ac: addiu	t8,sp,168
  801067b0: lw	a2,0(t7)
  801067b4: sw	t8,16(sp)
  801067b8: addiu	a1,a1,3584
  801067bc: move	a0,s7
  801067c0: addiu	a3,sp,200
  801067c4: jal	0x800b4360   <fcvt_wrapper>
  801067c8: addiu	a2,a2,20
  801067cc: sll	a0,s8,0x10
  801067d0: sll	a1,s5,0x10
  801067d4: sra	t6,a1,0x10
  801067d8: sra	t9,a0,0x10
  801067dc: li	t7,-1
  801067e0: sw	t7,16(sp)
  801067e4: move	a0,t9
  801067e8: move	a1,t6
  801067ec: li	a2,160
  801067f0: li	a3,110
  801067f4: sw	zero,20(sp)
  801067f8: jal	0x800be078   <camera_auto_follow>
  801067fc: sw	s7,24(sp)
  80106800: lui	a0,0x8015
  80106804: lh	a0,6864(a0)
  80106808: addiu	s6,s6,1
  8010680c: slt	at,s6,a0
  80106810: bnez	at,0x80106184
  80106814: addiu	s3,s3,2056
  80106818: lui	at,0xbf80
  8010681c: mtc1	at,$f12
  80106820: jal	0x800b65b4   <render_helper>
  80106824: nop
  80106828: lw	ra,84(sp)
  8010682c: lui	v1,0x8012
  80106830: addiu	v1,v1,-29152
  80106834: li	t8,3
  80106838: ldc1	$f20,40(sp)
  8010683c: lw	s0,48(sp)
  80106840: lw	s1,52(sp)
  80106844: lw	s2,56(sp)
  80106848: lw	s3,60(sp)
  8010684c: lw	s4,64(sp)
  80106850: lw	s5,68(sp)
  80106854: lw	s6,72(sp)
  80106858: lw	s7,76(sp)
  8010685c: lw	s8,80(sp)
  80106860: sw	t8,4(v1)
  80106864: sw	zero,0(v1)
  80106868: addiu	sp,sp,400
  8010686c: jr	ra
  80106870: li	v0,1
