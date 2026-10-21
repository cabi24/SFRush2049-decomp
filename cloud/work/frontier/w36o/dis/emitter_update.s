== emitter_update @ 0x800EEDB0 (310 words)
  800eedb0: addiu	sp,sp,-216
  800eedb4: lui	t6,0x8011
  800eedb8: lw	t6,29876(t6)
  800eedbc: sw	ra,44(sp)
  800eedc0: sw	s5,40(sp)
  800eedc4: sll	t7,t6,0x9
  800eedc8: sw	s4,36(sp)
  800eedcc: sw	s3,32(sp)
  800eedd0: sw	s2,28(sp)
  800eedd4: sw	s1,24(sp)
  800eedd8: sw	s0,20(sp)
  800eeddc: bgez	t7,0x800eee48
  800eede0: sw	a0,216(sp)
  800eede4: lui	t8,0x8016
  800eede8: lb	t8,-19892(t8)
  800eedec: lui	v0,0x8015
  800eedf0: beqz	t8,0x800eee48
  800eedf4: nop
  800eedf8: lw	v0,-24304(v0)
  800eedfc: li	at,6
  800eee00: lui	t9,0x8014
  800eee04: bne	v0,at,0x800eee18
  800eee08: li	s3,4
  800eee0c: lw	t9,2824(t9)
  800eee10: beqz	t9,0x800eee48
  800eee14: nop
  800eee18: bne	s3,v0,0x800eee30
  800eee1c: lui	t6,0x8014
  800eee20: lw	t6,1980(t6)
  800eee24: li	s2,1
  800eee28: beq	s2,t6,0x800eee48
  800eee2c: nop
  800eee30: beqz	v0,0x800eee48
  800eee34: li	s2,1
  800eee38: beq	s2,v0,0x800eee48
  800eee3c: li	at,3
  800eee40: bne	v0,at,0x800eee50
  800eee44: nop
  800eee48: b	0x800ef264
  800eee4c: li	v0,1
  800eee50: jal	0x80001578   <viGetTimeToDeadline>
  800eee54: nop
  800eee58: trunc.w.s	$f4,$f0
  800eee5c: lui	at,0x8012
  800eee60: li	v0,30
  800eee64: mfc1	s0,$f4
  800eee68: nop
  800eee6c: bgez	s0,0x800eee78
  800eee70: nop
  800eee74: move	s0,zero
  800eee78: sw	s0,20340(at)
  800eee7c: slti	at,s0,31
  800eee80: beqz	at,0x800eef1c
  800eee84: nop
  800eee88: lui	v0,0x8011
  800eee8c: lw	v0,25016(v0)
  800eee90: lui	t8,0x8011
  800eee94: slt	at,v0,s0
  800eee98: bnez	at,0x800eeee8
  800eee9c: slti	at,v0,6
  800eeea0: bnez	at,0x800eeeb8
  800eeea4: addiu	v0,v0,-1
  800eeea8: li	v0,5
  800eeeac: lui	at,0x8011
  800eeeb0: b	0x800eeec0
  800eeeb4: sw	v0,25016(at)
  800eeeb8: lui	at,0x8011
  800eeebc: sw	v0,25016(at)
  800eeec0: lb	t8,-64(t8)
  800eeec4: move	a3,zero
  800eeec8: bnezl	t8,0x800eeedc
  800eeecc: li	a0,2
  800eeed0: b	0x800eeee8
  800eeed4: nop
  800eeed8: li	a0,2
  800eeedc: move	a1,zero
  800eeee0: jal	0x80092360   <entity_flags_apply,frame_sync>
  800eeee4: li	a2,2
  800eeee8: lui	t9,0x8012
  800eeeec: lw	t9,20340(t9)
  800eeef0: lui	t6,0x8003
  800eeef4: addiu	t6,t6,-5912
  800eeef8: slti	at,t9,6
  800eeefc: beqz	at,0x800eef24
  800eef00: nop
  800eef04: lw	t7,636(t6)
  800eef08: andi	t8,t7,0x8
  800eef0c: beqz	t8,0x800eef24
  800eef10: nop
  800eef14: b	0x800ef264
  800eef18: li	v0,1
  800eef1c: lui	at,0x8011
  800eef20: sw	v0,25016(at)
  800eef24: lui	s1,0x8015
  800eef28: addiu	s1,s1,6864
  800eef2c: lh	t9,0(s1)
  800eef30: lui	t7,0x8011
  800eef34: addiu	t7,t7,23656
  800eef38: sll	t6,t9,0x3
  800eef3c: addu	v0,t6,t7
  800eef40: mtc1	zero,$f12
  800eef44: lw	s4,-8(v0)
  800eef48: jal	0x800b65b4   <render_helper>
  800eef4c: lw	s5,-4(v0)
  800eef50: lui	a3,0x8012
  800eef54: addiu	a3,a3,-29152
  800eef58: li	t8,3
  800eef5c: sw	t8,4(a3)
  800eef60: sw	s2,0(a3)
  800eef64: lui	t9,0x8015
  800eef68: lw	t9,-24304(t9)
  800eef6c: lui	a0,0x8014
  800eef70: addiu	a0,a0,25040
  800eef74: bne	s3,t9,0x800eefa4
  800eef78: move	a1,zero
  800eef7c: lh	v0,0(s1)
  800eef80: slti	at,v0,2
  800eef84: bnez	at,0x800eefa4
  800eef88: sll	t6,v0,0x3
  800eef8c: lui	t7,0x8011
  800eef90: addiu	t7,t7,24744
  800eef94: addu	v1,t6,t7
  800eef98: lw	s4,-8(v1)
  800eef9c: lw	s5,-4(v1)
  800eefa0: sw	s2,4(a3)
  800eefa4: li	at,600
  800eefa8: div	zero,s0,at
  800eefac: mflo	t8
  800eefb0: li	at,60
  800eefb4: li	v1,10
  800eefb8: div	zero,s0,at
  800eefbc: mflo	t6
  800eefc0: addiu	t9,t8,48
  800eefc4: sb	t9,204(sp)
  800eefc8: div	zero,t6,v1
  800eefcc: mfhi	t7
  800eefd0: addiu	t8,t7,48
  800eefd4: bnez	v1,0x800eefe0
  800eefd8: nop
  800eefdc: break	0x7
  800eefe0: li	at,-1
  800eefe4: bne	v1,at,0x800eeff8
  800eefe8: lui	at,0x8000
  800eefec: bne	t6,at,0x800eeff8
  800eeff0: nop
  800eeff4: break	0x6
  800eeff8: div	zero,s0,v1
  800eeffc: mflo	t6
  800ef000: li	t9,58
  800ef004: lh	v0,0(s1)
  800ef008: sb	t9,206(sp)
  800ef00c: sb	t8,205(sp)
  800ef010: bnez	v1,0x800ef01c
  800ef014: nop
  800ef018: break	0x7
  800ef01c: li	at,-1
  800ef020: bne	v1,at,0x800ef034
  800ef024: lui	at,0x8000
  800ef028: bne	s0,at,0x800ef034
  800ef02c: nop
  800ef030: break	0x6
  800ef034: li	at,6
  800ef038: div	zero,t6,at
  800ef03c: mfhi	t7
  800ef040: addiu	t8,t7,48
  800ef044: sb	t8,207(sp)
  800ef048: div	zero,s0,v1
  800ef04c: bnez	v1,0x800ef058
  800ef050: nop
  800ef054: break	0x7
  800ef058: li	at,-1
  800ef05c: bne	v1,at,0x800ef070
  800ef060: lui	at,0x8000
  800ef064: bne	s0,at,0x800ef070
  800ef068: nop
  800ef06c: break	0x6
  800ef070: mfhi	t9
  800ef074: addiu	t6,t9,48
  800ef078: sb	t6,208(sp)
  800ef07c: bne	s2,v0,0x800ef0b4
  800ef080: slti	at,v0,4
  800ef084: jal	0x80007270   <osRecvMesg>
  800ef088: li	a2,1
  800ef08c: jal	0x800b4200   <slot_state_setup>
  800ef090: li	s2,9
  800ef094: lui	a0,0x8014
  800ef098: addiu	a0,a0,25040
  800ef09c: move	s0,v0
  800ef0a0: move	a1,zero
  800ef0a4: jal	0x800075e0   <osSendMesg>
  800ef0a8: move	a2,zero
  800ef0ac: b	0x800ef128
  800ef0b0: nop
  800ef0b4: beqz	at,0x800ef0f8
  800ef0b8: lui	a0,0x8014
  800ef0bc: lui	a0,0x8014
  800ef0c0: addiu	a0,a0,25040
  800ef0c4: move	a1,zero
  800ef0c8: jal	0x80007270   <osRecvMesg>
  800ef0cc: li	a2,1
  800ef0d0: jal	0x800b4200   <slot_state_setup>
  800ef0d4: li	s2,8
  800ef0d8: lui	a0,0x8014
  800ef0dc: addiu	a0,a0,25040
  800ef0e0: move	s0,v0
  800ef0e4: move	a1,zero
  800ef0e8: jal	0x800075e0   <osSendMesg>
  800ef0ec: move	a2,zero
  800ef0f0: b	0x800ef128
  800ef0f4: nop
  800ef0f8: addiu	a0,a0,25040
  800ef0fc: move	a1,zero
  800ef100: jal	0x80007270   <osRecvMesg>
  800ef104: li	a2,1
  800ef108: jal	0x800b4200   <slot_state_setup>
  800ef10c: li	s2,5
  800ef110: lui	a0,0x8014
  800ef114: addiu	a0,a0,25040
  800ef118: move	s0,v0
  800ef11c: move	a1,zero
  800ef120: jal	0x800075e0   <osSendMesg>
  800ef124: move	a2,zero
  800ef128: jal	0x800bddfc   <camera_shake_update>
  800ef12c: li	a0,56
  800ef130: addiu	s3,v0,1
  800ef134: jal	0x800bddfc   <camera_shake_update>
  800ef138: li	a0,58
  800ef13c: sll	t8,s3,0x2
  800ef140: subu	t8,t8,s3
  800ef144: addiu	t7,v0,1
  800ef148: addu	t9,t8,v0
  800ef14c: sw	t7,192(sp)
  800ef150: addiu	t6,t9,1
  800ef154: bgez	t6,0x800ef164
  800ef158: sra	t7,t6,0x1
  800ef15c: addiu	at,t6,1
  800ef160: sra	t7,at,0x1
  800ef164: subu	s4,s4,t7
  800ef168: addiu	s0,sp,204
  800ef16c: lbu	t8,0(s0)
  800ef170: addiu	t9,sp,205
  800ef174: sltu	at,s0,t9
  800ef178: sb	zero,201(sp)
  800ef17c: beqz	at,0x800ef190
  800ef180: sb	t8,200(sp)
  800ef184: andi	t6,t8,0xff
  800ef188: li	at,48
  800ef18c: beq	t6,at,0x800ef1f8
  800ef190: addiu	s1,s5,1
  800ef194: sll	s2,s5,0x10
  800ef198: sra	t9,s2,0x10
  800ef19c: sll	t7,s1,0x10
  800ef1a0: sra	s1,t7,0x10
  800ef1a4: move	s2,t9
  800ef1a8: jal	0x800b74a0   <dispatch_handler>
  800ef1ac: move	a0,zero
  800ef1b0: addiu	a0,s4,1
  800ef1b4: sll	a1,s1,0x10
  800ef1b8: sra	t8,a1,0x10
  800ef1bc: sll	t6,a0,0x10
  800ef1c0: sra	a0,t6,0x10
  800ef1c4: move	a1,t8
  800ef1c8: jal	0x800b71d4   <state_utility>
  800ef1cc: addiu	a2,sp,200
  800ef1d0: jal	0x800b74a0   <dispatch_handler>
  800ef1d4: li	a0,14
  800ef1d8: sll	a0,s4,0x10
  800ef1dc: sll	a1,s2,0x10
  800ef1e0: sra	t6,a1,0x10
  800ef1e4: sra	t9,a0,0x10
  800ef1e8: move	a0,t9
  800ef1ec: move	a1,t6
  800ef1f0: jal	0x800b71d4   <state_utility>
  800ef1f4: addiu	a2,sp,200
  800ef1f8: addiu	t7,sp,205
  800ef1fc: beq	s0,t7,0x800ef20c
  800ef200: addiu	t8,sp,206
  800ef204: bnel	s0,t8,0x800ef230
  800ef208: addu	s4,s4,s3
  800ef20c: lw	t9,192(sp)
  800ef210: addu	t6,s3,t9
  800ef214: bgez	t6,0x800ef224
  800ef218: sra	t7,t6,0x1
  800ef21c: addiu	at,t6,1
  800ef220: sra	t7,at,0x1
  800ef224: b	0x800ef230
  800ef228: addu	s4,s4,t7
  800ef22c: addu	s4,s4,s3
  800ef230: addiu	s0,s0,1
  800ef234: addiu	t8,sp,209
  800ef238: bnel	s0,t8,0x800ef170
  800ef23c: lbu	t8,0(s0)
  800ef240: lui	at,0x8012
  800ef244: li	t9,3
  800ef248: sw	t9,-29148(at)
  800ef24c: sw	zero,-29152(at)
  800ef250: lui	at,0xbf80
  800ef254: mtc1	at,$f12
  800ef258: jal	0x800b65b4   <render_helper>
  800ef25c: nop
  800ef260: li	v0,1
  800ef264: lw	ra,44(sp)
  800ef268: lw	s0,20(sp)
  800ef26c: lw	s1,24(sp)
  800ef270: lw	s2,28(sp)
  800ef274: lw	s3,32(sp)
  800ef278: lw	s4,36(sp)
  800ef27c: lw	s5,40(sp)
  800ef280: jr	ra
  800ef284: addiu	sp,sp,216
