== func_80106874 @ 0x80106874 (178 words)
  80106874: addiu	sp,sp,-64
  80106878: sw	s4,52(sp)
  8010687c: lui	s4,0x8014
  80106880: addiu	s4,s4,25040
  80106884: sw	ra,60(sp)
  80106888: sw	a0,64(sp)
  8010688c: sw	s5,56(sp)
  80106890: sw	s3,48(sp)
  80106894: sw	s2,44(sp)
  80106898: sw	s1,40(sp)
  8010689c: sw	s0,36(sp)
  801068a0: move	a0,s4
  801068a4: move	a1,zero
  801068a8: jal	0x80007270   <osRecvMesg>
  801068ac: li	a2,1
  801068b0: jal	0x800b4200   <slot_state_setup>
  801068b4: li	s2,11
  801068b8: move	s0,v0
  801068bc: move	a0,s4
  801068c0: move	a1,zero
  801068c4: jal	0x800075e0   <osSendMesg>
  801068c8: move	a2,zero
  801068cc: lui	t6,0x8018
  801068d0: lw	t6,-23324(t6)
  801068d4: li	a1,-1
  801068d8: jal	0x800b3fa4   <object_manager_update,object_utility>
  801068dc: lw	a0,904(t6)
  801068e0: lui	t4,0x8015
  801068e4: lh	t4,6864(t4)
  801068e8: move	s5,v0
  801068ec: move	a1,zero
  801068f0: slti	at,t4,3
  801068f4: bnez	at,0x80106904
  801068f8: li	a2,1
  801068fc: b	0x8010691c
  80106900: li	v1,160
  80106904: slti	at,t4,2
  80106908: bnez	at,0x80106918
  8010690c: li	a0,320
  80106910: b	0x80106918
  80106914: li	a0,240
  80106918: move	v1,a0
  8010691c: addiu	t7,v1,8
  80106920: slt	at,t7,v0
  80106924: beqz	at,0x80106968
  80106928: nop
  8010692c: jal	0x80007270   <osRecvMesg>
  80106930: move	a0,s4
  80106934: jal	0x800b4200   <slot_state_setup>
  80106938: li	s2,12
  8010693c: move	s0,v0
  80106940: move	a0,s4
  80106944: move	a1,zero
  80106948: jal	0x800075e0   <osSendMesg>
  8010694c: move	a2,zero
  80106950: lui	t8,0x8018
  80106954: lw	t8,-23324(t8)
  80106958: li	a1,-1
  8010695c: jal	0x800b3fa4   <object_manager_update,object_utility>
  80106960: lw	a0,904(t8)
  80106964: move	s5,v0
  80106968: jal	0x800b3f50   <object_bytes_sum_global,object_helper>
  8010696c: nop
  80106970: lui	t4,0x8015
  80106974: lh	t4,6864(t4)
  80106978: move	s4,v0
  8010697c: move	s1,zero
  80106980: blez	t4,0x80106ad8
  80106984: lui	s2,0x8015
  80106988: addiu	s2,s2,17232
  8010698c: lui	t9,0x8011
  80106990: lw	t9,29876(t9)
  80106994: lui	t7,0x8011
  80106998: sll	t6,t9,0xa
  8010699c: bgezl	t6,0x80106aac
  801069a0: lw	a0,0(s2)
  801069a4: lw	t7,28924(t7)
  801069a8: sll	t8,s1,0x2
  801069ac: addu	t8,t8,s1
  801069b0: bnez	t7,0x80106aa8
  801069b4: sll	t8,t8,0x2
  801069b8: subu	t8,t8,s1
  801069bc: sll	t8,t8,0x2
  801069c0: lui	v1,0x8015
  801069c4: addu	v1,v1,t8
  801069c8: lbu	v1,-24295(v1)
  801069cc: lui	v0,0x8014
  801069d0: sll	t9,v1,0x2
  801069d4: subu	t9,t9,v1
  801069d8: sll	t9,t9,0x6
  801069dc: addu	t9,t9,v1
  801069e0: sll	t9,t9,0x2
  801069e4: addu	v0,v0,t9
  801069e8: lb	v0,16438(v0)
  801069ec: bnezl	v0,0x80106aac
  801069f0: lw	a0,0(s2)
  801069f4: lw	t6,0(s2)
  801069f8: sll	v1,s1,0x3
  801069fc: sll	t7,t4,0x5
  80106a00: bnez	t6,0x80106ac8
  80106a04: addu	t8,t7,v1
  80106a08: lui	t9,0x8011
  80106a0c: addiu	t9,t9,24488
  80106a10: addu	t0,t8,t9
  80106a14: lw	v0,-32(t0)
  80106a18: lw	t1,-28(t0)
  80106a1c: bgez	s5,0x80106a2c
  80106a20: sra	t2,s5,0x1
  80106a24: addiu	at,s5,1
  80106a28: sra	t2,at,0x1
  80106a2c: lui	t8,0x8011
  80106a30: addiu	t8,t8,24984
  80106a34: addu	s0,v1,t8
  80106a38: li	t6,255
  80106a3c: li	t7,1
  80106a40: subu	a0,v0,t2
  80106a44: addu	a2,t2,v0
  80106a48: addiu	a2,a2,8
  80106a4c: addiu	a0,a0,-8
  80106a50: bgez	s4,0x80106a60
  80106a54: sra	t3,s4,0x1
  80106a58: addiu	at,s4,1
  80106a5c: sra	t3,at,0x1
  80106a60: subu	a1,t1,t3
  80106a64: addu	a3,t3,t1
  80106a68: addiu	a3,a3,4
  80106a6c: addiu	a1,a1,-4
  80106a70: sw	t7,28(sp)
  80106a74: sw	t6,16(sp)
  80106a78: addiu	s3,s0,4
  80106a7c: sw	zero,20(sp)
  80106a80: jal	0x800b3b4c   <ambient_sound_set>
  80106a84: sw	s1,24(sp)
  80106a88: sw	v0,0(s2)
  80106a8c: move	a0,v0
  80106a90: move	a1,s0
  80106a94: jal	0x800dc794   <tournament_unlock_check,unlock_notification>
  80106a98: move	a2,s3
  80106a9c: lui	t4,0x8015
  80106aa0: b	0x80106ac8
  80106aa4: lh	t4,6864(t4)
  80106aa8: lw	a0,0(s2)
  80106aac: beqzl	a0,0x80106acc
  80106ab0: addiu	s1,s1,1
  80106ab4: jal	0x800dc720   <sound_handles_array_clear>
  80106ab8: nop
  80106abc: sw	zero,0(s2)
  80106ac0: lui	t4,0x8015
  80106ac4: lh	t4,6864(t4)
  80106ac8: addiu	s1,s1,1
  80106acc: slt	at,s1,t4
  80106ad0: bnez	at,0x8010698c
  80106ad4: addiu	s2,s2,4
  80106ad8: lui	t9,0x8011
  80106adc: lw	t9,29876(t9)
  80106ae0: lw	a0,64(sp)
  80106ae4: li	v0,1
  80106ae8: sll	t6,t9,0x9
  80106aec: bgez	t6,0x80106b18
  80106af0: nop
  80106af4: lb	t7,26(a0)
  80106af8: li	v0,1
  80106afc: sw	zero,40(a0)
  80106b00: beq	v0,t7,0x80106b10
  80106b04: nop
  80106b08: jal	0x80094ec8   <Input_ApplyPadConfig,input_handler>
  80106b0c: sb	v0,26(a0)
  80106b10: b	0x80106b18
  80106b14: move	v0,zero
  80106b18: lw	ra,60(sp)
  80106b1c: lw	s0,36(sp)
  80106b20: lw	s1,40(sp)
  80106b24: lw	s2,44(sp)
  80106b28: lw	s3,48(sp)
  80106b2c: lw	s4,52(sp)
  80106b30: lw	s5,56(sp)
  80106b34: jr	ra
  80106b38: addiu	sp,sp,64
