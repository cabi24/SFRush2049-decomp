== explosion_effect @ 0x800EF8F4 (365 words)
  800ef8f4: addiu	sp,sp,-288
  800ef8f8: lui	t6,0x8016
  800ef8fc: lb	t6,-2260(t6)
  800ef900: sw	ra,60(sp)
  800ef904: sw	s8,56(sp)
  800ef908: sw	s7,52(sp)
  800ef90c: sw	s6,48(sp)
  800ef910: sw	s5,44(sp)
  800ef914: sw	s4,40(sp)
  800ef918: sw	s3,36(sp)
  800ef91c: sw	s2,32(sp)
  800ef920: sw	s1,28(sp)
  800ef924: sw	s0,24(sp)
  800ef928: bnez	t6,0x800ef938
  800ef92c: sw	a0,288(sp)
  800ef930: b	0x800efe78
  800ef934: li	v0,1
  800ef938: mtc1	zero,$f12
  800ef93c: jal	0x800b65b4   <render_helper>
  800ef940: nop
  800ef944: lui	v1,0x8012
  800ef948: addiu	v1,v1,-29152
  800ef94c: li	v0,1
  800ef950: li	t7,3
  800ef954: sw	t7,4(v1)
  800ef958: sw	v0,0(v1)
  800ef95c: lui	t8,0x8015
  800ef960: lh	t8,6864(t8)
  800ef964: lui	a0,0x8014
  800ef968: addiu	a0,a0,25040
  800ef96c: bne	v0,t8,0x800ef9b0
  800ef970: move	a1,zero
  800ef974: lui	a0,0x8014
  800ef978: addiu	a0,a0,25040
  800ef97c: move	a1,zero
  800ef980: jal	0x80007270   <osRecvMesg>
  800ef984: move	a2,v0
  800ef988: jal	0x800b4200   <slot_state_setup>
  800ef98c: li	s2,4
  800ef990: lui	a0,0x8014
  800ef994: addiu	a0,a0,25040
  800ef998: move	s0,v0
  800ef99c: move	a1,zero
  800ef9a0: jal	0x800075e0   <osSendMesg>
  800ef9a4: move	a2,zero
  800ef9a8: b	0x800ef9d8
  800ef9ac: nop
  800ef9b0: jal	0x80007270   <osRecvMesg>
  800ef9b4: move	a2,v0
  800ef9b8: jal	0x800b4200   <slot_state_setup>
  800ef9bc: li	s2,1
  800ef9c0: lui	a0,0x8014
  800ef9c4: addiu	a0,a0,25040
  800ef9c8: move	s0,v0
  800ef9cc: move	a1,zero
  800ef9d0: jal	0x800075e0   <osSendMesg>
  800ef9d4: move	a2,zero
  800ef9d8: jal	0x800bddfc   <camera_shake_update>
  800ef9dc: li	a0,56
  800ef9e0: lui	v1,0x8015
  800ef9e4: lh	v1,6864(v1)
  800ef9e8: addiu	s6,v0,1
  800ef9ec: move	a1,zero
  800ef9f0: blez	v1,0x800efe54
  800ef9f4: lui	t9,0x8015
  800ef9f8: addiu	t9,t9,-23984
  800ef9fc: lui	v0,0x8015
  800efa00: addiu	v0,v0,10264
  800efa04: sw	t9,80(sp)
  800efa08: li	s8,46
  800efa0c: li	s7,58
  800efa10: addiu	s5,sp,276
  800efa14: addiu	s3,sp,283
  800efa18: li	a3,1
  800efa1c: li	a2,952
  800efa20: lw	t6,80(sp)
  800efa24: lh	t7,1990(t6)
  800efa28: multu	t7,a2
  800efa2c: mflo	t8
  800efa30: addu	t9,v0,t8
  800efa34: lb	t6,239(t9)
  800efa38: beql	a3,t6,0x800efe40
  800efa3c: lw	t8,80(sp)
  800efa40: multu	a1,a2
  800efa44: lui	at,0x8012
  800efa48: lwc1	$f6,17836(at)
  800efa4c: mtc1	zero,$f10
  800efa50: sll	a0,a1,0x3
  800efa54: lui	at,0x8012
  800efa58: mflo	t7
  800efa5c: addu	t8,v0,t7
  800efa60: lh	t9,248(t8)
  800efa64: lui	t7,0x8016
  800efa68: lb	t7,5034(t7)
  800efa6c: sra	t6,t9,0x2
  800efa70: mtc1	t6,$f4
  800efa74: nop
  800efa78: cvt.s.w	$f0,$f4
  800efa7c: mul.s	$f0,$f0,$f6
  800efa80: beqzl	t7,0x800efa98
  800efa84: c.lt.s	$f0,$f10
  800efa88: lwc1	$f8,17840(at)
  800efa8c: mul.s	$f0,$f0,$f8
  800efa90: nop
  800efa94: c.lt.s	$f0,$f10
  800efa98: lui	at,0x42c8
  800efa9c: mtc1	at,$f4
  800efaa0: bc1fl	0x800efab0
  800efaa4: div.s	$f6,$f0,$f4
  800efaa8: neg.s	$f0,$f0
  800efaac: div.s	$f6,$f0,$f4
  800efab0: sll	t8,v1,0x5
  800efab4: lui	t6,0x8011
  800efab8: addiu	t6,t6,23144
  800efabc: addu	t9,t8,a0
  800efac0: addu	v0,t9,t6
  800efac4: li	a2,10
  800efac8: lw	t7,-28(v0)
  800efacc: lw	s4,-32(v0)
  800efad0: sw	a0,76(sp)
  800efad4: sw	t7,264(sp)
  800efad8: sw	a1,256(sp)
  800efadc: lui	a0,0x8014
  800efae0: addiu	a0,a0,25040
  800efae4: move	a1,zero
  800efae8: subu	s4,s4,s6
  800efaec: trunc.w.s	$f8,$f6
  800efaf0: mfc1	t9,$f8
  800efaf4: trunc.w.s	$f8,$f0
  800efaf8: div	zero,t9,a2
  800efafc: bnez	a2,0x800efb08
  800efb00: nop
  800efb04: break	0x7
  800efb08: li	at,-1
  800efb0c: bne	a2,at,0x800efb20
  800efb10: lui	at,0x8000
  800efb14: bne	t9,at,0x800efb20
  800efb18: nop
  800efb1c: break	0x6
  800efb20: lui	at,0x4120
  800efb24: mtc1	at,$f10
  800efb28: mfhi	t6
  800efb2c: addiu	t7,t6,48
  800efb30: div.s	$f4,$f0,$f10
  800efb34: sb	t7,280(sp)
  800efb38: trunc.w.s	$f6,$f4
  800efb3c: mfc1	t9,$f6
  800efb40: nop
  800efb44: div	zero,t9,a2
  800efb48: bnez	a2,0x800efb54
  800efb4c: nop
  800efb50: break	0x7
  800efb54: li	at,-1
  800efb58: bne	a2,at,0x800efb6c
  800efb5c: lui	at,0x8000
  800efb60: bne	t9,at,0x800efb6c
  800efb64: nop
  800efb68: break	0x6
  800efb6c: mfc1	t9,$f8
  800efb70: mfhi	t6
  800efb74: addiu	t7,t6,48
  800efb78: sb	t7,281(sp)
  800efb7c: div	zero,t9,a2
  800efb80: mfhi	t6
  800efb84: addiu	t7,t6,48
  800efb88: bnez	a2,0x800efb94
  800efb8c: nop
  800efb90: break	0x7
  800efb94: li	at,-1
  800efb98: bne	a2,at,0x800efbac
  800efb9c: lui	at,0x8000
  800efba0: bne	t9,at,0x800efbac
  800efba4: nop
  800efba8: break	0x6
  800efbac: sb	t7,282(sp)
  800efbb0: bne	a3,v1,0x800efbf4
  800efbb4: move	a2,a3
  800efbb8: lui	a0,0x8014
  800efbbc: addiu	a0,a0,25040
  800efbc0: jal	0x80007270   <osRecvMesg>
  800efbc4: move	a1,zero
  800efbc8: jal	0x800b4200   <slot_state_setup>
  800efbcc: li	s2,4
  800efbd0: lui	a0,0x8014
  800efbd4: addiu	a0,a0,25040
  800efbd8: addiu	s3,sp,283
  800efbdc: move	s0,v0
  800efbe0: move	a1,zero
  800efbe4: jal	0x800075e0   <osSendMesg>
  800efbe8: move	a2,zero
  800efbec: b	0x800efc24
  800efbf0: addiu	s0,sp,280
  800efbf4: jal	0x80007270   <osRecvMesg>
  800efbf8: move	a2,a3
  800efbfc: jal	0x800b4200   <slot_state_setup>
  800efc00: li	s2,1
  800efc04: lui	a0,0x8014
  800efc08: addiu	a0,a0,25040
  800efc0c: addiu	s3,sp,283
  800efc10: move	s0,v0
  800efc14: move	a1,zero
  800efc18: jal	0x800075e0   <osSendMesg>
  800efc1c: move	a2,zero
  800efc20: addiu	s0,sp,280
  800efc24: lh	s2,266(sp)
  800efc28: sb	zero,277(sp)
  800efc2c: lbu	v0,0(s0)
  800efc30: beq	s7,v0,0x800efc70
  800efc34: move	v1,v0
  800efc38: beq	s8,v1,0x800efc70
  800efc3c: li	t8,56
  800efc40: sb	t8,276(sp)
  800efc44: jal	0x800b74a0   <dispatch_handler>
  800efc48: move	a0,zero
  800efc4c: sll	a0,s4,0x10
  800efc50: sll	a1,s2,0x10
  800efc54: sra	t6,a1,0x10
  800efc58: sra	t9,a0,0x10
  800efc5c: move	a0,t9
  800efc60: move	a1,t6
  800efc64: jal	0x800b71d4   <state_utility>
  800efc68: move	a2,s5
  800efc6c: lbu	v0,0(s0)
  800efc70: sll	s1,s4,0x10
  800efc74: sra	t7,s1,0x10
  800efc78: sb	v0,276(sp)
  800efc7c: move	s1,t7
  800efc80: jal	0x800b74a0   <dispatch_handler>
  800efc84: li	a0,14
  800efc88: sll	a0,s1,0x10
  800efc8c: sll	a1,s2,0x10
  800efc90: sra	t9,a1,0x10
  800efc94: sra	t8,a0,0x10
  800efc98: move	a0,t8
  800efc9c: move	a1,t9
  800efca0: jal	0x800b71d4   <state_utility>
  800efca4: move	a2,s5
  800efca8: addiu	s0,s0,1
  800efcac: bne	s0,s3,0x800efc28
  800efcb0: addu	s4,s4,s6
  800efcb4: lui	t6,0x8015
  800efcb8: lh	t6,6864(t6)
  800efcbc: lw	t8,76(sp)
  800efcc0: lui	s4,0x8011
  800efcc4: sll	t7,t6,0x5
  800efcc8: addu	t9,t7,t8
  800efccc: addu	s4,s4,t9
  800efcd0: lw	s4,23112(s4)
  800efcd4: jal	0x800b3d18   <sound_update_channel>
  800efcd8: move	t0,zero
  800efcdc: lui	t6,0x8015
  800efce0: lw	t6,-26640(t6)
  800efce4: move	t0,zero
  800efce8: jal	0x800b3d18   <sound_update_channel>
  800efcec: lbu	t1,2(t6)
  800efcf0: lui	t7,0x8015
  800efcf4: lw	t7,-26640(t7)
  800efcf8: lw	t6,264(sp)
  800efcfc: lui	a0,0x8014
  800efd00: lbu	v0,3(t7)
  800efd04: addiu	a0,a0,25040
  800efd08: move	a1,zero
  800efd0c: addu	v0,v0,t1
  800efd10: sll	t8,v0,0x10
  800efd14: sra	t9,t8,0x10
  800efd18: addu	t7,t6,t9
  800efd1c: addiu	t8,t7,-4
  800efd20: sw	t8,264(sp)
  800efd24: jal	0x80007270   <osRecvMesg>
  800efd28: li	a2,1
  800efd2c: jal	0x800b4200   <slot_state_setup>
  800efd30: li	s2,12
  800efd34: lui	a0,0x8014
  800efd38: addiu	a0,a0,25040
  800efd3c: addiu	s3,sp,283
  800efd40: move	s0,v0
  800efd44: move	a1,zero
  800efd48: jal	0x800075e0   <osSendMesg>
  800efd4c: move	a2,zero
  800efd50: lui	t9,0x8016
  800efd54: lb	t9,5034(t9)
  800efd58: beqz	t9,0x800efdc4
  800efd5c: nop
  800efd60: jal	0x800b74a0   <dispatch_handler>
  800efd64: move	a0,zero
  800efd68: addiu	a0,s4,1
  800efd6c: lw	a1,264(sp)
  800efd70: sll	t6,a0,0x10
  800efd74: sra	a0,t6,0x10
  800efd78: lui	t6,0x8018
  800efd7c: lw	t6,-23324(t6)
  800efd80: addiu	a1,a1,1
  800efd84: sll	t8,a1,0x10
  800efd88: sra	a1,t8,0x10
  800efd8c: jal	0x800b71d4   <state_utility>
  800efd90: lw	a2,812(t6)
  800efd94: jal	0x800b74a0   <dispatch_handler>
  800efd98: li	a0,1
  800efd9c: lui	t8,0x8018
  800efda0: lw	t8,-23324(t8)
  800efda4: sll	a0,s4,0x10
  800efda8: sra	t7,a0,0x10
  800efdac: move	a0,t7
  800efdb0: lh	a1,266(sp)
  800efdb4: jal	0x800b71d4   <state_utility>
  800efdb8: lw	a2,812(t8)
  800efdbc: b	0x800efe20
  800efdc0: nop
  800efdc4: jal	0x800b74a0   <dispatch_handler>
  800efdc8: move	a0,zero
  800efdcc: addiu	a0,s4,1
  800efdd0: lw	a1,264(sp)
  800efdd4: sll	t9,a0,0x10
  800efdd8: sra	a0,t9,0x10
  800efddc: lui	t9,0x8018
  800efde0: lw	t9,-23324(t9)
  800efde4: addiu	a1,a1,1
  800efde8: sll	t7,a1,0x10
  800efdec: sra	a1,t7,0x10
  800efdf0: jal	0x800b71d4   <state_utility>
  800efdf4: lw	a2,808(t9)
  800efdf8: jal	0x800b74a0   <dispatch_handler>
  800efdfc: li	a0,1
  800efe00: lui	t7,0x8018
  800efe04: lw	t7,-23324(t7)
  800efe08: sll	a0,s4,0x10
  800efe0c: sra	t6,a0,0x10
  800efe10: move	a0,t6
  800efe14: lh	a1,266(sp)
  800efe18: jal	0x800b71d4   <state_utility>
  800efe1c: lw	a2,808(t7)
  800efe20: lui	v1,0x8015
  800efe24: lui	v0,0x8015
  800efe28: addiu	v0,v0,10264
  800efe2c: lh	v1,6864(v1)
  800efe30: li	a3,1
  800efe34: li	a2,952
  800efe38: lw	a1,256(sp)
  800efe3c: lw	t8,80(sp)
  800efe40: addiu	a1,a1,1
  800efe44: slt	at,a1,v1
  800efe48: addiu	t9,t8,2056
  800efe4c: bnez	at,0x800efa20
  800efe50: sw	t9,80(sp)
  800efe54: lui	at,0x8012
  800efe58: li	t6,3
  800efe5c: sw	t6,-29148(at)
  800efe60: sw	zero,-29152(at)
  800efe64: lui	at,0xbf80
  800efe68: mtc1	at,$f12
  800efe6c: jal	0x800b65b4   <render_helper>
  800efe70: nop
  800efe74: li	v0,1
  800efe78: lw	ra,60(sp)
  800efe7c: lw	s0,24(sp)
  800efe80: lw	s1,28(sp)
  800efe84: lw	s2,32(sp)
  800efe88: lw	s3,36(sp)
  800efe8c: lw	s4,40(sp)
  800efe90: lw	s5,44(sp)
  800efe94: lw	s6,48(sp)
  800efe98: lw	s7,52(sp)
  800efe9c: lw	s8,56(sp)
  800efea0: jr	ra
  800efea4: addiu	sp,sp,288
