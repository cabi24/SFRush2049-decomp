== func_8038B940 @ 0x8038B940 (157 words, 628 B)
  8038b940: 27bdffe8 addiu	sp,sp,-24
  8038b944: 3c10803b lui	s0,0x803b
  8038b948: 3c14803b lui	s4,0x803b
  8038b94c: 3c137c03 lui	s3,0x7c03
  8038b950: 3c118011 lui	s1,0x8011
  8038b954: afbf0014 sw	ra,20(sp)
  8038b958: 263174b4 addiu	s1,s1,29876   ; 801174B4 state_word_a
  8038b95c: 3673fffe ori	s3,s3,0xfffe   ; 7C03FFFE 
  8038b960: 26947714 addiu	s4,s4,30484   ; 803B7714 [00000000 0]
  8038b964: 26106b14 addiu	s0,s0,27412   ; 803B6B14 [00000000 0]
  8038b968: 2412ffff li	s2,-1
  8038b96c: 8e020004 lw	v0,4(s0)   ; 803B6B18 [FFFFFFFF nan]
  8038b970: 5242000b beql	s2,v0,0x8038b9a0
  8038b974: 26100040 addiu	s0,s0,64   ; 803B6B54 [00000000 0]
  8038b978: 8e2e0000 lw	t6,0(s1)   ; 801174B4 state_word_a
  8038b97c: 00022400 sll	a0,v0,0x10
  8038b980: 0004c403 sra	t8,a0,0x10
  8038b984: 01d37824 and	t7,t6,s3
  8038b988: 51e00004 beqzl	t7,0x8038b99c
  8038b98c: ae120004 sw	s2,4(s0)   ; 803B6B58 [FFFFFFFF nan]
  8038b990: 0c024095 jal	0x80090254   <sound_call_minimal>
  8038b994: 03002025 move	a0,t8
  8038b998: ae120004 sw	s2,4(s0)   ; 803B6B58 [FFFFFFFF nan]
  8038b99c: 26100040 addiu	s0,s0,64   ; 803B6B94 [00000000 0]
  8038b9a0: 5614fff3 bnel	s0,s4,0x8038b970
  8038b9a4: 8e020004 lw	v0,4(s0)   ; 803B6B98 [FFFFFFFF nan]
  8038b9a8: 3c10803b lui	s0,0x803b
  8038b9ac: 3c01803b lui	at,0x803b
  8038b9b0: 26107cd0 addiu	s0,s0,31952   ; 803B7CD0 [00000000 0]
  8038b9b4: ac207714 sw	zero,30484(at)   ; 803B7714 [00000000 0]
  8038b9b8: 8e040000 lw	a0,0(s0)   ; 803B7CD0 [00000000 0]
  8038b9bc: 10800004 beqz	a0,0x8038b9d0
  8038b9c0: 00000000 nop
  8038b9c4: 0c02cd63 jal	0x800b358c   <sound_stop>
  8038b9c8: 00000000 nop
  8038b9cc: ae000000 sw	zero,0(s0)   ; 803B7CD0 [00000000 0]
  8038b9d0: 0c02d55f jal	0x800b557c   <ambient_sounds_clear>
  8038b9d4: 00000000 nop
  8038b9d8: 0c0e2b94 jal	0x8038ae50   <func_8038AE50>
  8038b9dc: 00000000 nop
  8038b9e0: 3c10803c lui	s0,0x803c
  8038b9e4: 26108284 addiu	s0,s0,-32124   ; 803B8284 [FFFFFFFF nan]
  8038b9e8: 8e020000 lw	v0,0(s0)   ; 803B8284 [FFFFFFFF nan]
  8038b9ec: 12420005 beq	s2,v0,0x8038ba04
  8038b9f0: 00022400 sll	a0,v0,0x10
  8038b9f4: 0004cc03 sra	t9,a0,0x10
  8038b9f8: 0c024095 jal	0x80090254   <sound_call_minimal>
  8038b9fc: 03202025 move	a0,t9
  8038ba00: ae120000 sw	s2,0(s0)   ; 803B8284 [FFFFFFFF nan]
  8038ba04: 3c138003 lui	s3,0x8003
  8038ba08: 2673eb70 addiu	s3,s3,-5264   ; 8002EB70 D_8002EB70
  8038ba0c: 866e0000 lh	t6,0(s3)   ; 8002EB70 D_8002EB70
  8038ba10: 3c18803c lui	t8,0x803c
  8038ba14: 11c00004 beqz	t6,0x8038ba28
  8038ba18: 00000000 nop
  8038ba1c: 866f0000 lh	t7,0(s3)   ; 8002EB70 D_8002EB70
  8038ba20: 55e0ffff bnezl	t7,0x8038ba20
  8038ba24: 866f0000 lh	t7,0(s3)   ; 8002EB70 D_8002EB70
  8038ba28: 8318aae0 lb	t8,-21792(t8)   ; 803BAAE0 
  8038ba2c: 1300005a beqz	t8,0x8038bb98
  8038ba30: 00000000 nop
  8038ba34: 3c10803b lui	s0,0x803b
  8038ba38: 3c11803c lui	s1,0x803c
  8038ba3c: 26318284 addiu	s1,s1,-32124   ; 803B8284 [FFFFFFFF nan]
  8038ba40: 26107ce4 addiu	s0,s0,31972   ; 803B7CE4 [FFFFFFFF nan]
  8038ba44: 8e020000 lw	v0,0(s0)   ; 803B7CE4 [FFFFFFFF nan]
  8038ba48: 12420005 beq	s2,v0,0x8038ba60
  8038ba4c: 00022400 sll	a0,v0,0x10
  8038ba50: 0004cc03 sra	t9,a0,0x10
  8038ba54: 0c024095 jal	0x80090254   <sound_call_minimal>
  8038ba58: 03202025 move	a0,t9
  8038ba5c: ae120000 sw	s2,0(s0)   ; 803B7CE4 [FFFFFFFF nan]
  8038ba60: 26100048 addiu	s0,s0,72   ; 803B7D2C [FFFFFFFF nan]
  8038ba64: 5611fff8 bnel	s0,s1,0x8038ba48
  8038ba68: 8e020000 lw	v0,0(s0)   ; 803B7D2C [FFFFFFFF nan]
  8038ba6c: 866e0000 lh	t6,0(s3)   ; 8002EB70 D_8002EB70
  8038ba70: 3c14803c lui	s4,0x803c
  8038ba74: 2694ab58 addiu	s4,s4,-21672   ; 803BAB58 
  8038ba78: 11c00004 beqz	t6,0x8038ba8c
  8038ba7c: 00008825 move	s1,zero
  8038ba80: 866f0000 lh	t7,0(s3)   ; 8002EB70 D_8002EB70
  8038ba84: 55e0ffff bnezl	t7,0x8038ba84
  8038ba88: 866f0000 lh	t7,0(s3)   ; 8002EB70 D_8002EB70
  8038ba8c: 3c10803c lui	s0,0x803c
  8038ba90: 3c12803c lui	s2,0x803c
  8038ba94: 3c13803c lui	s3,0x803c
  8038ba98: 3c178011 lui	s7,0x8011
  8038ba9c: 3c168014 lui	s6,0x8014
  8038baa0: 3c15803b lui	s5,0x803b
  8038baa4: 26b57738 addiu	s5,s5,30520   ; 803B7738 [803B8DDC -5.469184e-39]
  8038baa8: 26d60bdc addiu	s6,s6,3036   ; 80140BDC gNumModels
  8038baac: 26f761f4 addiu	s7,s7,25076   ; 801161F4 gRomObjDefPtrs
  8038bab0: 2673aae8 addiu	s3,s3,-21784   ; 803BAAE8 
  8038bab4: 2652abe8 addiu	s2,s2,-21528   ; 803BABE8 
  8038bab8: 2610abc8 addiu	s0,s0,-21560   ; 803BABC8 
  8038babc: 241e0058 li	s8,88
  8038bac0: 2a210006 slti	at,s1,6
  8038bac4: 10200003 beqz	at,0x8038bad4
  8038bac8: 00002825 move	a1,zero
  8038bacc: 10000002 b	0x8038bad8
  8038bad0: 02201025 move	v0,s1
  8038bad4: 24020013 li	v0,19
  8038bad8: 92c60000 lbu	a2,0(s6)   ; 80140BDC gNumModels
  8038badc: 0002c080 sll	t8,v0,0x2
  8038bae0: 02b8c821 addu	t9,s5,t8
  8038bae4: 24c6ffff addiu	a2,a2,-1
  8038bae8: 00067600 sll	t6,a2,0x18
  8038baec: 000e3603 sra	a2,t6,0x18
  8038baf0: 8f240000 lw	a0,0(t9)
  8038baf4: 0c024b8b jal	0x80092e2c   <string_copy_format>
  8038baf8: 24070001 li	a3,1
  8038bafc: 0002c283 sra	t8,v0,0xa
  8038bb00: 0018c8c0 sll	t9,t8,0x3
  8038bb04: 305803ff andi	t8,v0,0x3ff
  8038bb08: 031e0019 multu	t8,s8
  8038bb0c: 02f97021 addu	t6,s7,t9
  8038bb10: 8dcf0000 lw	t7,0(t6)
  8038bb14: 8e810000 lw	at,0(s4)   ; 803BAB58 
  8038bb18: 0000c812 mflo	t9
  8038bb1c: 01f91821 addu	v1,t7,t9
  8038bb20: ac610018 sw	at,24(v1)
  8038bb24: 8e980004 lw	t8,4(s4)   ; 803BAB5C 
  8038bb28: ac78001c sw	t8,28(v1)
  8038bb2c: 8e810008 lw	at,8(s4)   ; 803BAB60 
  8038bb30: ac610020 sw	at,32(v1)
  8038bb34: 8e98000c lw	t8,12(s4)   ; 803BAB64 
  8038bb38: ac780024 sw	t8,36(v1)
  8038bb3c: 8e040000 lw	a0,0(s0)   ; 803BABC8 
  8038bb40: 50800004 beqzl	a0,0x8038bb54
  8038bb44: 8e440000 lw	a0,0(s2)   ; 803BABE8 
  8038bb48: 0c025835 jal	0x800960d4   <audio_effect_process>
  8038bb4c: 00000000 nop
  8038bb50: 8e440000 lw	a0,0(s2)   ; 803BABE8 
  8038bb54: ae000000 sw	zero,0(s0)   ; 803BABC8 
  8038bb58: 50800004 beqzl	a0,0x8038bb6c
  8038bb5c: 26310001 addiu	s1,s1,1   ; 803B8285 [FFFFFF00 nan]
  8038bb60: 0c025835 jal	0x800960d4   <audio_effect_process>
  8038bb64: 00000000 nop
  8038bb68: 26310001 addiu	s1,s1,1   ; 803B8286 [FFFF0000 nan]
  8038bb6c: 24010007 li	at,7
  8038bb70: 26940010 addiu	s4,s4,16   ; 803BAB68 
  8038bb74: 26100004 addiu	s0,s0,4   ; 803BABCC 
  8038bb78: 26520004 addiu	s2,s2,4   ; 803BABEC 
  8038bb7c: 26730010 addiu	s3,s3,16   ; 803BAAF8 
  8038bb80: ae40fffc sw	zero,-4(s2)   ; 803BABE8 
  8038bb84: ae60fff8 sw	zero,-8(s3)   ; 803BAAF0 
  8038bb88: 1621ffcd bne	s1,at,0x8038bac0
  8038bb8c: ae60fffc sw	zero,-4(s3)   ; 803BAAF4 
  8038bb90: 3c01803c lui	at,0x803c
  8038bb94: a020aae0 sb	zero,-21792(at)   ; 803BAAE0 
  8038bb98: 0c0296cf jal	0x800a5b3c   <func_800A5B3C>
  8038bb9c: 00000000 nop
  8038bba0: 8fbf0014 lw	ra,20(sp)
  8038bba4: 3c01803b lui	at,0x803b
  8038bba8: a0207cd4 sb	zero,31956(at)   ; 803B7CD4 [00000000 0]
  8038bbac: 03e00008 jr	ra
  8038bbb0: 27bd0018 addiu	sp,sp,24
