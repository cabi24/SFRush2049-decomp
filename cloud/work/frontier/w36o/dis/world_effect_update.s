== world_effect_update @ 0x800EE8B4 (114 words)
  800ee8b4: addiu	sp,sp,-24
  800ee8b8: sw	ra,20(sp)
  800ee8bc: lui	at,0x8016
  800ee8c0: sw	zero,-2248(at)
  800ee8c4: lui	at,0x8016
  800ee8c8: sw	zero,4992(at)
  800ee8cc: lui	at,0x8016
  800ee8d0: sw	zero,5016(at)
  800ee8d4: lui	at,0x8016
  800ee8d8: sw	zero,5028(at)
  800ee8dc: lui	at,0x42b4
  800ee8e0: lui	v0,0x8000
  800ee8e4: mtc1	at,$f4
  800ee8e8: lw	v0,768(v0)
  800ee8ec: lui	at,0x8015
  800ee8f0: swc1	$f4,16776(at)
  800ee8f4: bnez	v0,0x800ee904
  800ee8f8: nop
  800ee8fc: beqz	v0,0x800ee8fc
  800ee900: nop
  800ee904: jal	0x80001dfc   <get_tv_offset>
  800ee908: nop
  800ee90c: lui	a0,0x8000
  800ee910: lui	a2,0x8003
  800ee914: lui	a3,0x8003
  800ee918: lw	a3,-20540(a3)
  800ee91c: lw	a2,-20544(a2)
  800ee920: lw	a0,768(a0)
  800ee924: jal	0x80001b44   <viewport_setup>
  800ee928: move	a1,v0
  800ee92c: lui	a1,0x2
  800ee930: ori	a1,a1,0x2620
  800ee934: jal	0x80097470   <audio_dma_sync>
  800ee938: move	a0,zero
  800ee93c: lui	v1,0x8003
  800ee940: addiu	v1,v1,-5200
  800ee944: sw	v0,0(v1)
  800ee948: move	a0,v0
  800ee94c: jal	0x800962d4   <audio_loop_control,dma_request>
  800ee950: move	a1,zero
  800ee954: lui	a1,0x2
  800ee958: ori	a1,a1,0x2620
  800ee95c: jal	0x800cc3c0   <sound_play_menu>
  800ee960: move	a0,zero
  800ee964: lui	v1,0x8015
  800ee968: addiu	v1,v1,27884
  800ee96c: sw	v0,0(v1)
  800ee970: move	a0,v0
  800ee974: jal	0x800962d4   <audio_loop_control,dma_request>
  800ee978: move	a1,zero
  800ee97c: lui	a1,0x2
  800ee980: ori	a1,a1,0x2620
  800ee984: jal	0x800cc3c0   <sound_play_menu>
  800ee988: move	a0,zero
  800ee98c: lui	a2,0x8015
  800ee990: addiu	a2,a2,29248
  800ee994: sw	v0,0(a2)
  800ee998: move	a0,v0
  800ee99c: jal	0x800962d4   <audio_loop_control,dma_request>
  800ee9a0: move	a1,zero
  800ee9a4: lui	t0,0x8015
  800ee9a8: addiu	t0,t0,27884
  800ee9ac: lw	t9,0(t0)
  800ee9b0: lui	a2,0x8015
  800ee9b4: lui	t1,0x8003
  800ee9b8: addiu	t1,t1,-5200
  800ee9bc: addiu	a2,a2,29248
  800ee9c0: lui	v0,0x8015
  800ee9c4: lw	t6,0(t1)
  800ee9c8: lw	t4,0(a2)
  800ee9cc: addiu	v0,v0,27616
  800ee9d0: addiu	t2,t9,32
  800ee9d4: li	a0,-64
  800ee9d8: and	t3,t2,a0
  800ee9dc: addiu	t7,t6,32
  800ee9e0: addiu	t5,t4,32
  800ee9e4: li	a1,2
  800ee9e8: and	t6,t5,a0
  800ee9ec: lui	t9,0x8006
  800ee9f0: lui	t2,0x8007
  800ee9f4: and	t8,t7,a0
  800ee9f8: addiu	t9,t9,-31024
  800ee9fc: addiu	t2,t2,-24432
  800eea00: sw	t8,0(t1)
  800eea04: sw	t3,0(t0)
  800eea08: sw	t6,0(a2)
  800eea0c: sw	t3,124(v0)
  800eea10: sw	t6,252(v0)
  800eea14: sh	a1,92(v0)
  800eea18: sh	a1,220(v0)
  800eea1c: sw	t9,88(v0)
  800eea20: sw	t2,216(v0)
  800eea24: lui	at,0x8016
  800eea28: sw	t9,-19888(at)
  800eea2c: lui	at,0x8016
  800eea30: addiu	t3,t9,1728
  800eea34: sw	t3,-19872(at)
  800eea38: lui	a3,0x8015
  800eea3c: li	at,0x9cc0
  800eea40: addiu	a3,a3,-26680
  800eea44: addu	t4,t9,at
  800eea48: sw	t4,0(a3)
  800eea4c: lw	t5,0(a3)
  800eea50: lui	at,0x8015
  800eea54: jal	0x800a5b3c   <func_800A5B3C>
  800eea58: sw	t5,-26636(at)
  800eea5c: jal	0x800a5488   <func_800A5488>
  800eea60: nop
  800eea64: jal	0x800b82c8   <particle_velocity_set>
  800eea68: nop
  800eea6c: lw	ra,20(sp)
  800eea70: addiu	sp,sp,24
  800eea74: jr	ra
  800eea78: nop
