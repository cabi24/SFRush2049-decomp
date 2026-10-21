== func_800F87A0 @ 0x800F87A0 (242 words)
  800f87a0: lui	s0,0x8015
  800f87a4: addiu	s0,s0,26956
  800f87a8: lw	a0,0(s0)
  800f87ac: addiu	sp,sp,-24
  800f87b0: sw	ra,20(sp)
  800f87b4: andi	t6,a0,0x3
  800f87b8: beqz	t6,0x800f8abc
  800f87bc: nop
  800f87c0: jal	0x800b5fc4   <resource_type_select>
  800f87c4: nop
  800f87c8: lui	s1,0x8014
  800f87cc: addiu	s1,s1,-28192
  800f87d0: lw	t7,0(s1)
  800f87d4: sltiu	at,t7,7
  800f87d8: beqz	at,0x800f8b58
  800f87dc: sll	t7,t7,0x2
  800f87e0: lui	at,0x8012
  800f87e4: addu	at,at,t7
  800f87e8: lw	t7,17948(at)
  800f87ec: jr	t7
  800f87f0: nop
  800f87f4: jal	0x800f857c   <func_800F857C>
  800f87f8: li	s0,2
  800f87fc: b	0x800f8b5c
  800f8800: lw	ra,20(sp)
  800f8804: lui	t8,0x8015
  800f8808: lw	t8,-24304(t8)
  800f880c: li	at,2
  800f8810: lui	s2,0x8014
  800f8814: bne	t8,at,0x800f88f8
  800f8818: addiu	s2,s2,10024
  800f881c: move	a0,s2
  800f8820: move	a1,zero
  800f8824: jal	0x80007270   <osRecvMesg>
  800f8828: li	a2,1
  800f882c: jal	0x80091b00   <func_80091B00>
  800f8830: nop
  800f8834: li	t9,1
  800f8838: sb	t9,2(v0)
  800f883c: move	s0,v0
  800f8840: move	a0,s2
  800f8844: move	a1,zero
  800f8848: jal	0x800075e0   <osSendMesg>
  800f884c: move	a2,zero
  800f8850: lui	a0,0x8014
  800f8854: addiu	a0,a0,10152
  800f8858: move	a1,s0
  800f885c: jal	0x800075e0   <osSendMesg>
  800f8860: move	a2,zero
  800f8864: lui	a0,0x8015
  800f8868: jal	0x800b912c   <func_800B912C>
  800f886c: lb	a0,-26740(a0)
  800f8870: lui	s2,0x8014
  800f8874: addiu	s2,s2,-28176
  800f8878: lw	s0,0(s2)
  800f887c: beqz	s0,0x800f8898
  800f8880: move	a0,s0
  800f8884: jal	0x8009079c   <entity_transform_apply>
  800f8888: li	a1,1
  800f888c: lw	s0,0(s2)
  800f8890: bnezl	s0,0x800f8884
  800f8894: move	a0,s0
  800f8898: jal	0x800b0580   <func_800B0580>
  800f889c: nop
  800f88a0: li	t6,1
  800f88a4: lui	at,0x8015
  800f88a8: sh	t6,6864(at)
  800f88ac: jal	0x800a6be4   <wheel_render_full>
  800f88b0: li	a0,1
  800f88b4: lui	s0,0x8015
  800f88b8: addiu	s0,s0,16804
  800f88bc: lw	a0,0(s0)
  800f88c0: beqz	a0,0x800f88d4
  800f88c4: nop
  800f88c8: jal	0x800b358c   <sound_stop>
  800f88cc: nop
  800f88d0: sw	zero,0(s0)
  800f88d4: jal	0x800f8754   <audio_update_d>
  800f88d8: nop
  800f88dc: lui	t7,0x4
  800f88e0: lui	at,0x8011
  800f88e4: sw	t7,29884(at)
  800f88e8: lui	at,0x8011
  800f88ec: li	t8,4
  800f88f0: b	0x800f8b58
  800f88f4: sw	t8,29880(at)
  800f88f8: jal	0x800f8754   <audio_update_d>
  800f88fc: nop
  800f8900: lui	s2,0x8014
  800f8904: addiu	s2,s2,10024
  800f8908: li	t9,1
  800f890c: lui	at,0x8011
  800f8910: sb	t9,18000(at)
  800f8914: move	a0,s2
  800f8918: move	a1,zero
  800f891c: jal	0x80007270   <osRecvMesg>
  800f8920: li	a2,1
  800f8924: jal	0x80091b00   <func_80091B00>
  800f8928: nop
  800f892c: li	t6,7
  800f8930: sb	t6,2(v0)
  800f8934: move	s0,v0
  800f8938: move	a0,s2
  800f893c: move	a1,zero
  800f8940: jal	0x800075e0   <osSendMesg>
  800f8944: move	a2,zero
  800f8948: lui	a0,0x8014
  800f894c: addiu	a0,a0,10152
  800f8950: move	a1,s0
  800f8954: jal	0x800075e0   <osSendMesg>
  800f8958: move	a2,zero
  800f895c: jal	0x800d5a04   <func_800D5A04>
  800f8960: nop
  800f8964: b	0x800f8b5c
  800f8968: lw	ra,20(sp)
  800f896c: lui	s2,0x8014
  800f8970: addiu	s2,s2,10024
  800f8974: move	a0,s2
  800f8978: move	a1,zero
  800f897c: jal	0x80007270   <osRecvMesg>
  800f8980: li	a2,1
  800f8984: jal	0x80091b00   <func_80091B00>
  800f8988: nop
  800f898c: li	t7,1
  800f8990: sb	t7,2(v0)
  800f8994: move	s0,v0
  800f8998: move	a0,s2
  800f899c: move	a1,zero
  800f89a0: jal	0x800075e0   <osSendMesg>
  800f89a4: move	a2,zero
  800f89a8: lui	a0,0x8014
  800f89ac: addiu	a0,a0,10152
  800f89b0: move	a1,s0
  800f89b4: jal	0x800075e0   <osSendMesg>
  800f89b8: move	a2,zero
  800f89bc: lui	a0,0x8015
  800f89c0: jal	0x800b912c   <func_800B912C>
  800f89c4: lb	a0,-26740(a0)
  800f89c8: lui	s2,0x8014
  800f89cc: addiu	s2,s2,-28176
  800f89d0: lw	s0,0(s2)
  800f89d4: beqz	s0,0x800f89f0
  800f89d8: move	a0,s0
  800f89dc: jal	0x8009079c   <entity_transform_apply>
  800f89e0: li	a1,1
  800f89e4: lw	s0,0(s2)
  800f89e8: bnezl	s0,0x800f89dc
  800f89ec: move	a0,s0
  800f89f0: jal	0x800b0580   <func_800B0580>
  800f89f4: nop
  800f89f8: li	t8,1
  800f89fc: lui	at,0x8015
  800f8a00: sh	t8,6864(at)
  800f8a04: jal	0x800a6be4   <wheel_render_full>
  800f8a08: li	a0,1
  800f8a0c: lui	s0,0x8015
  800f8a10: addiu	s0,s0,16804
  800f8a14: lw	a0,0(s0)
  800f8a18: beqzl	a0,0x800f8a30
  800f8a1c: lw	a0,0(s1)
  800f8a20: jal	0x800b358c   <sound_stop>
  800f8a24: nop
  800f8a28: sw	zero,0(s0)
  800f8a2c: lw	a0,0(s1)
  800f8a30: li	at,2
  800f8a34: li	t9,64
  800f8a38: bne	a0,at,0x800f8a48
  800f8a3c: lui	at,0x8011
  800f8a40: b	0x800f8aa4
  800f8a44: sw	t9,29884(at)
  800f8a48: li	at,3
  800f8a4c: bne	a0,at,0x800f8a60
  800f8a50: li	t6,128
  800f8a54: lui	at,0x8011
  800f8a58: b	0x800f8aa4
  800f8a5c: sw	t6,29884(at)
  800f8a60: li	at,4
  800f8a64: bne	a0,at,0x800f8a78
  800f8a68: li	t7,256
  800f8a6c: lui	at,0x8011
  800f8a70: b	0x800f8aa4
  800f8a74: sw	t7,29884(at)
  800f8a78: li	at,5
  800f8a7c: bne	a0,at,0x800f8a94
  800f8a80: li	a3,6
  800f8a84: li	t8,16384
  800f8a88: lui	at,0x8011
  800f8a8c: b	0x800f8aa4
  800f8a90: sw	t8,29884(at)
  800f8a94: bne	a3,a0,0x800f8aa4
  800f8a98: li	t9,16
  800f8a9c: lui	at,0x8011
  800f8aa0: sw	t9,29884(at)
  800f8aa4: jal	0x800f8754   <audio_update_d>
  800f8aa8: nop
  800f8aac: li	t6,4
  800f8ab0: lui	at,0x8011
  800f8ab4: b	0x800f8b58
  800f8ab8: sw	t6,29880(at)
  800f8abc: lui	a0,0x8015
  800f8ac0: lw	a0,-26748(a0)
  800f8ac4: andi	t7,a0,0x400
  800f8ac8: beqz	t7,0x800f8b14
  800f8acc: andi	t6,a0,0x800
  800f8ad0: jal	0x800b5f4c   <audio_distance_atten>
  800f8ad4: nop
  800f8ad8: lui	s1,0x8014
  800f8adc: addiu	s1,s1,-28192
  800f8ae0: li	a3,6
  800f8ae4: lw	t8,0(s1)
  800f8ae8: addiu	a0,t8,-1
  800f8aec: bgez	a0,0x800f8afc
  800f8af0: sw	a0,0(s1)
  800f8af4: sw	a3,0(s1)
  800f8af8: move	a0,a3
  800f8afc: jal	0x800f84b0   <func_800F84B0>
  800f8b00: nop
  800f8b04: beqzl	v0,0x800f8ae8
  800f8b08: lw	t8,0(s1)
  800f8b0c: b	0x800f8b5c
  800f8b10: lw	ra,20(sp)
  800f8b14: beqzl	t6,0x800f8b5c
  800f8b18: lw	ra,20(sp)
  800f8b1c: jal	0x800b5f4c   <audio_distance_atten>
  800f8b20: nop
  800f8b24: lui	s1,0x8014
  800f8b28: addiu	s1,s1,-28192
  800f8b2c: lw	t7,0(s1)
  800f8b30: addiu	a0,t7,1
  800f8b34: slti	at,a0,7
  800f8b38: bnez	at,0x800f8b48
  800f8b3c: sw	a0,0(s1)
  800f8b40: sw	zero,0(s1)
  800f8b44: move	a0,zero
  800f8b48: jal	0x800f84b0   <func_800F84B0>
  800f8b4c: nop
  800f8b50: beqzl	v0,0x800f8b30
  800f8b54: lw	t7,0(s1)
  800f8b58: lw	ra,20(sp)
  800f8b5c: addiu	sp,sp,24
  800f8b60: jr	ra
  800f8b64: nop
