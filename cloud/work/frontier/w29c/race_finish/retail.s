== race_finish @ 0x80107AF4 (250 words)
  80107af4: addiu	sp,sp,-280
  80107af8: lui	t6,0x8011
  80107afc: lw	t6,29876(t6)
  80107b00: sw	ra,60(sp)
  80107b04: sw	s8,56(sp)
  80107b08: andi	t7,t6,0x8
  80107b0c: sw	s7,52(sp)
  80107b10: sw	s6,48(sp)
  80107b14: sw	s5,44(sp)
  80107b18: sw	s4,40(sp)
  80107b1c: sw	s3,36(sp)
  80107b20: sw	s2,32(sp)
  80107b24: sw	s1,28(sp)
  80107b28: sw	s0,24(sp)
  80107b2c: bnez	t7,0x80107b70
  80107b30: sw	a0,280(sp)
  80107b34: lui	s4,0x8015
  80107b38: addiu	s4,s4,17354
  80107b3c: lh	t8,0(s4)
  80107b40: lui	t9,0x8015
  80107b44: slti	at,t8,2
  80107b48: bnez	at,0x80107b70
  80107b4c: nop
  80107b50: lw	t9,-24304(t9)
  80107b54: li	at,1
  80107b58: lui	t6,0x8016
  80107b5c: beq	t9,at,0x80107b70
  80107b60: nop
  80107b64: lb	t6,4988(t6)
  80107b68: bnezl	t6,0x80107b7c
  80107b6c: mtc1	zero,$f12
  80107b70: b	0x80107eac
  80107b74: li	v0,1
  80107b78: mtc1	zero,$f12
  80107b7c: jal	0x800b65b4   <render_helper>
  80107b80: nop
  80107b84: lui	v1,0x8012
  80107b88: addiu	v1,v1,-29152
  80107b8c: li	v0,1
  80107b90: li	t7,3
  80107b94: sw	t7,4(v1)
  80107b98: sw	v0,0(v1)
  80107b9c: lui	t8,0x8015
  80107ba0: lh	t8,6864(t8)
  80107ba4: lui	s5,0x8014
  80107ba8: addiu	s5,s5,25040
  80107bac: bne	v0,t8,0x80107bf0
  80107bb0: move	a0,s5
  80107bb4: lui	s5,0x8014
  80107bb8: addiu	s5,s5,25040
  80107bbc: move	a0,s5
  80107bc0: move	a1,zero
  80107bc4: jal	0x80007270   <osRecvMesg>
  80107bc8: move	a2,v0
  80107bcc: jal	0x800b4200   <slot_state_setup>
  80107bd0: li	s2,8
  80107bd4: move	s0,v0
  80107bd8: move	a0,s5
  80107bdc: move	a1,zero
  80107be0: jal	0x800075e0   <osSendMesg>
  80107be4: move	a2,zero
  80107be8: b	0x80107c18
  80107bec: nop
  80107bf0: move	a1,zero
  80107bf4: jal	0x80007270   <osRecvMesg>
  80107bf8: move	a2,v0
  80107bfc: jal	0x800b4200   <slot_state_setup>
  80107c00: li	s2,5
  80107c04: move	s0,v0
  80107c08: move	a0,s5
  80107c0c: move	a1,zero
  80107c10: jal	0x800075e0   <osSendMesg>
  80107c14: move	a2,zero
  80107c18: jal	0x800bddfc   <camera_shake_update>
  80107c1c: li	a0,56
  80107c20: lh	t6,0(s4)
  80107c24: lui	t8,0x8015
  80107c28: lh	t8,6864(t8)
  80107c2c: li	t9,47
  80107c30: addiu	t7,t6,48
  80107c34: sw	v0,268(sp)
  80107c38: sb	t9,277(sp)
  80107c3c: sb	t7,278(sp)
  80107c40: blez	t8,0x80107e88
  80107c44: move	s8,zero
  80107c48: addiu	s7,sp,272
  80107c4c: sll	a0,s8,0x10
  80107c50: sra	t9,a0,0x10
  80107c54: jal	0x800cf604   <func_800CF604>
  80107c58: move	a0,t9
  80107c5c: beqz	v0,0x80107c94
  80107c60: sll	t6,s8,0x4
  80107c64: subu	t6,t6,s8
  80107c68: sll	t6,t6,0x3
  80107c6c: subu	t6,t6,s8
  80107c70: lui	t7,0x8015
  80107c74: addiu	t7,t7,10264
  80107c78: sll	t6,t6,0x3
  80107c7c: addu	a0,t6,t7
  80107c80: lb	t8,239(a0)
  80107c84: li	a3,1
  80107c88: lui	t9,0x8015
  80107c8c: bne	a3,t8,0x80107c9c
  80107c90: nop
  80107c94: b	0x80107e70
  80107c98: nop
  80107c9c: lh	t9,6864(t9)
  80107ca0: sll	t7,s8,0x3
  80107ca4: addiu	s4,sp,276
  80107ca8: sll	t6,t9,0x5
  80107cac: lui	t9,0x8011
  80107cb0: addiu	t9,t9,23720
  80107cb4: addu	t8,t6,t7
  80107cb8: addu	v0,t8,t9
  80107cbc: lw	v1,-28(v0)
  80107cc0: lb	t6,238(a0)
  80107cc4: lw	s6,-32(v0)
  80107cc8: lw	t8,268(sp)
  80107ccc: addiu	t9,v1,1
  80107cd0: addiu	t7,t6,49
  80107cd4: sll	t6,t9,0x10
  80107cd8: subu	s6,s6,t8
  80107cdc: sb	t7,276(sp)
  80107ce0: sll	t8,v1,0x10
  80107ce4: sra	t9,t8,0x10
  80107ce8: sra	t7,t6,0x10
  80107cec: sw	t7,76(sp)
  80107cf0: sw	t9,72(sp)
  80107cf4: lbu	t6,0(s4)
  80107cf8: addiu	t7,sp,278
  80107cfc: sltu	at,s4,t7
  80107d00: lui	v0,0x8015
  80107d04: li	a3,1
  80107d08: sb	zero,273(sp)
  80107d0c: lh	v0,6864(v0)
  80107d10: beqz	at,0x80107d84
  80107d14: sb	t6,272(sp)
  80107d18: bne	a3,v0,0x80107d54
  80107d1c: move	a0,s5
  80107d20: move	a0,s5
  80107d24: move	a1,zero
  80107d28: jal	0x80007270   <osRecvMesg>
  80107d2c: move	a2,a3
  80107d30: jal	0x800b4200   <slot_state_setup>
  80107d34: li	s2,8
  80107d38: move	s0,v0
  80107d3c: move	a0,s5
  80107d40: move	a1,zero
  80107d44: jal	0x800075e0   <osSendMesg>
  80107d48: move	a2,zero
  80107d4c: b	0x80107de8
  80107d50: nop
  80107d54: move	a1,zero
  80107d58: jal	0x80007270   <osRecvMesg>
  80107d5c: move	a2,a3
  80107d60: jal	0x800b4200   <slot_state_setup>
  80107d64: li	s2,5
  80107d68: move	s0,v0
  80107d6c: move	a0,s5
  80107d70: move	a1,zero
  80107d74: jal	0x800075e0   <osSendMesg>
  80107d78: move	a2,zero
  80107d7c: b	0x80107de8
  80107d80: nop
  80107d84: bne	a3,v0,0x80107dc0
  80107d88: move	a0,s5
  80107d8c: move	a0,s5
  80107d90: move	a1,zero
  80107d94: jal	0x80007270   <osRecvMesg>
  80107d98: move	a2,a3
  80107d9c: jal	0x800b4200   <slot_state_setup>
  80107da0: li	s2,5
  80107da4: move	s0,v0
  80107da8: move	a0,s5
  80107dac: move	a1,zero
  80107db0: jal	0x800075e0   <osSendMesg>
  80107db4: move	a2,zero
  80107db8: b	0x80107de8
  80107dbc: nop
  80107dc0: move	a1,zero
  80107dc4: jal	0x80007270   <osRecvMesg>
  80107dc8: move	a2,a3
  80107dcc: jal	0x800b4200   <slot_state_setup>
  80107dd0: li	s2,6
  80107dd4: move	s0,v0
  80107dd8: move	a0,s5
  80107ddc: move	a1,zero
  80107de0: jal	0x800075e0   <osSendMesg>
  80107de4: move	a2,zero
  80107de8: jal	0x800b74a0   <dispatch_handler>
  80107dec: move	a0,zero
  80107df0: addiu	a0,s6,1
  80107df4: sll	t8,a0,0x10
  80107df8: sra	a0,t8,0x10
  80107dfc: lh	a1,78(sp)
  80107e00: jal	0x800b71d4   <state_utility>
  80107e04: move	a2,s7
  80107e08: addiu	t6,sp,276
  80107e0c: bne	s4,t6,0x80107e3c
  80107e10: sll	t7,s8,0x2
  80107e14: lui	t8,0x8011
  80107e18: addiu	t8,t8,24936
  80107e1c: addu	v0,t7,t8
  80107e20: lbu	a0,0(v0)
  80107e24: lbu	a1,1(v0)
  80107e28: lbu	a2,2(v0)
  80107e2c: jal	0x800b7360   <func_800B7360>
  80107e30: li	a3,255
  80107e34: b	0x80107e48
  80107e38: sll	a0,s6,0x10
  80107e3c: jal	0x800b74a0   <dispatch_handler>
  80107e40: li	a0,1
  80107e44: sll	a0,s6,0x10
  80107e48: sra	t9,a0,0x10
  80107e4c: move	a0,t9
  80107e50: lh	a1,74(sp)
  80107e54: jal	0x800b71d4   <state_utility>
  80107e58: move	a2,s7
  80107e5c: lw	t6,268(sp)
  80107e60: addiu	s4,s4,1
  80107e64: addiu	t7,sp,279
  80107e68: bne	s4,t7,0x80107cf4
  80107e6c: addu	s6,s6,t6
  80107e70: lui	t8,0x8015
  80107e74: lh	t8,6864(t8)
  80107e78: addiu	s8,s8,1
  80107e7c: slt	at,s8,t8
  80107e80: bnezl	at,0x80107c50
  80107e84: sll	a0,s8,0x10
  80107e88: lui	at,0x8012
  80107e8c: li	t9,3
  80107e90: sw	t9,-29148(at)
  80107e94: sw	zero,-29152(at)
  80107e98: lui	at,0xbf80
  80107e9c: mtc1	at,$f12
  80107ea0: jal	0x800b65b4   <render_helper>
  80107ea4: nop
  80107ea8: li	v0,1
  80107eac: lw	ra,60(sp)
  80107eb0: lw	s0,24(sp)
  80107eb4: lw	s1,28(sp)
  80107eb8: lw	s2,32(sp)
  80107ebc: lw	s3,36(sp)
  80107ec0: lw	s4,40(sp)
  80107ec4: lw	s5,44(sp)
  80107ec8: lw	s6,48(sp)
  80107ecc: lw	s7,52(sp)
  80107ed0: lw	s8,56(sp)
  80107ed4: jr	ra
  80107ed8: addiu	sp,sp,280
