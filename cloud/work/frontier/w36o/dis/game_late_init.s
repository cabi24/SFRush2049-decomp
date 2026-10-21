== game_late_init @ 0x800EEA7C (205 words)
  800eea7c: addiu	sp,sp,-128
  800eea80: sw	ra,44(sp)
  800eea84: sw	s1,40(sp)
  800eea88: sw	s0,36(sp)
  800eea8c: li	a0,50
  800eea90: jal	0x800e7d0c   <func_800E7D0C>
  800eea94: li	a1,10
  800eea98: jal	0x800cdeec   <menu_save_options>
  800eea9c: nop
  800eeaa0: lui	at,0x8016
  800eeaa4: sw	zero,-2248(at)
  800eeaa8: lui	at,0x8016
  800eeaac: sw	zero,4992(at)
  800eeab0: lui	at,0x8016
  800eeab4: sw	zero,5016(at)
  800eeab8: lui	at,0x8016
  800eeabc: sw	zero,5028(at)
  800eeac0: lui	at,0x42b4
  800eeac4: lui	v0,0x8000
  800eeac8: lw	v0,768(v0)
  800eeacc: mtc1	at,$f4
  800eead0: lui	at,0x8015
  800eead4: bnez	v0,0x800eeae4
  800eead8: swc1	$f4,16776(at)
  800eeadc: beqz	v0,0x800eeadc
  800eeae0: nop
  800eeae4: jal	0x80001dfc   <get_tv_offset>
  800eeae8: nop
  800eeaec: lui	a0,0x8000
  800eeaf0: lui	a2,0x8003
  800eeaf4: lui	a3,0x8003
  800eeaf8: lw	a3,-20540(a3)
  800eeafc: lw	a2,-20544(a2)
  800eeb00: lw	a0,768(a0)
  800eeb04: jal	0x80001b44   <viewport_setup>
  800eeb08: move	a1,v0
  800eeb0c: lui	a1,0x2
  800eeb10: ori	a1,a1,0x2620
  800eeb14: jal	0x80097470   <audio_dma_sync>
  800eeb18: move	a0,zero
  800eeb1c: lui	v1,0x8003
  800eeb20: addiu	v1,v1,-5200
  800eeb24: sw	v0,0(v1)
  800eeb28: move	a0,v0
  800eeb2c: jal	0x800962d4   <audio_loop_control,dma_request>
  800eeb30: move	a1,zero
  800eeb34: lui	a1,0x2
  800eeb38: ori	a1,a1,0x2620
  800eeb3c: jal	0x800cc3c0   <sound_play_menu>
  800eeb40: move	a0,zero
  800eeb44: lui	v1,0x8015
  800eeb48: addiu	v1,v1,27884
  800eeb4c: sw	v0,0(v1)
  800eeb50: move	a0,v0
  800eeb54: jal	0x800962d4   <audio_loop_control,dma_request>
  800eeb58: move	a1,zero
  800eeb5c: lui	a1,0x2
  800eeb60: ori	a1,a1,0x2620
  800eeb64: jal	0x800cc3c0   <sound_play_menu>
  800eeb68: move	a0,zero
  800eeb6c: lui	a2,0x8015
  800eeb70: addiu	a2,a2,29248
  800eeb74: sw	v0,0(a2)
  800eeb78: move	a0,v0
  800eeb7c: jal	0x800962d4   <audio_loop_control,dma_request>
  800eeb80: move	a1,zero
  800eeb84: lui	t1,0x8003
  800eeb88: addiu	t1,t1,-5200
  800eeb8c: lw	t6,0(t1)
  800eeb90: lui	t0,0x8015
  800eeb94: addiu	t0,t0,27884
  800eeb98: li	a0,-64
  800eeb9c: addiu	t7,t6,32
  800eeba0: lw	t9,0(t0)
  800eeba4: and	t8,t7,a0
  800eeba8: lui	a2,0x8015
  800eebac: addiu	a2,a2,29248
  800eebb0: sw	t8,0(t1)
  800eebb4: lw	t8,0(a2)
  800eebb8: addiu	t6,t9,32
  800eebbc: and	t7,t6,a0
  800eebc0: lui	v0,0x8015
  800eebc4: addiu	t9,t8,32
  800eebc8: and	t6,t9,a0
  800eebcc: addiu	v0,v0,27616
  800eebd0: sw	t6,252(v0)
  800eebd4: sw	t6,0(a2)
  800eebd8: lui	t6,0x8007
  800eebdc: lui	t9,0x8006
  800eebe0: li	a1,2
  800eebe4: addiu	t9,t9,-31024
  800eebe8: addiu	t6,t6,-24432
  800eebec: sw	t6,216(v0)
  800eebf0: sw	t9,88(v0)
  800eebf4: sh	a1,92(v0)
  800eebf8: sh	a1,220(v0)
  800eebfc: sw	t7,124(v0)
  800eec00: sw	t7,0(t0)
  800eec04: lui	at,0x8016
  800eec08: sw	t9,-19888(at)
  800eec0c: lui	at,0x8016
  800eec10: addiu	t7,t9,1728
  800eec14: sw	t7,-19872(at)
  800eec18: li	at,0x9cc0
  800eec1c: addu	t8,t9,at
  800eec20: lui	a3,0x8015
  800eec24: addiu	a3,a3,-26680
  800eec28: sw	t8,0(a3)
  800eec2c: lui	at,0x8015
  800eec30: jal	0x800a5b3c   <func_800A5B3C>
  800eec34: sw	t8,-26636(at)
  800eec38: jal	0x800a5488   <func_800A5488>
  800eec3c: nop
  800eec40: jal	0x800b82c8   <particle_velocity_set>
  800eec44: nop
  800eec48: lui	at,0x3f80
  800eec4c: mtc1	at,$f6
  800eec50: lui	at,0x4348
  800eec54: mtc1	at,$f8
  800eec58: li	t6,64
  800eec5c: sw	t6,16(sp)
  800eec60: li	a0,32
  800eec64: li	a1,16
  800eec68: li	a2,16
  800eec6c: li	a3,5
  800eec70: sw	zero,28(sp)
  800eec74: swc1	$f6,20(sp)
  800eec78: jal	0x800c8b8c   <hud_setup>
  800eec7c: swc1	$f8,24(sp)
  800eec80: lui	t7,0x8015
  800eec84: lb	t7,27028(t7)
  800eec88: lui	v0,0x8011
  800eec8c: bnez	t7,0x800eed28
  800eec90: nop
  800eec94: lw	v0,604(v0)
  800eec98: lui	a0,0x8015
  800eec9c: addiu	a0,a0,10096
  800eeca0: beqz	v0,0x800eecdc
  800eeca4: move	a1,zero
  800eeca8: sw	v0,72(sp)
  800eecac: jal	0x80007270   <osRecvMesg>
  800eecb0: li	a2,1
  800eecb4: lw	a1,72(sp)
  800eecb8: jal	0x80095fd8   <audio_reverb_update>
  800eecbc: move	a2,zero
  800eecc0: lui	a0,0x8015
  800eecc4: addiu	a0,a0,10096
  800eecc8: move	a1,zero
  800eeccc: jal	0x800075e0   <osSendMesg>
  800eecd0: move	a2,zero
  800eecd4: lui	at,0x8011
  800eecd8: sw	zero,604(at)
  800eecdc: lui	v0,0x8011
  800eece0: lw	v0,608(v0)
  800eece4: lui	a0,0x8015
  800eece8: addiu	a0,a0,10096
  800eecec: beqz	v0,0x800eed28
  800eecf0: move	a1,zero
  800eecf4: sw	v0,48(sp)
  800eecf8: jal	0x80007270   <osRecvMesg>
  800eecfc: li	a2,1
  800eed00: lw	a1,48(sp)
  800eed04: jal	0x80095fd8   <audio_reverb_update>
  800eed08: move	a2,zero
  800eed0c: lui	a0,0x8015
  800eed10: addiu	a0,a0,10096
  800eed14: move	a1,zero
  800eed18: jal	0x800075e0   <osSendMesg>
  800eed1c: move	a2,zero
  800eed20: lui	at,0x8011
  800eed24: sw	zero,608(at)
  800eed28: jal	0x800ee820   <effect_spawn>
  800eed2c: nop
  800eed30: li	a0,-1
  800eed34: jal	0x800c9158   <player_state_set>
  800eed38: move	a1,zero
  800eed3c: jal	0x800de45c   <func_800DE45C>
  800eed40: nop
  800eed44: jal	0x80001350   <viTickStart>
  800eed48: nop
  800eed4c: lui	t8,0x8011
  800eed50: lb	t8,18368(t8)
  800eed54: li	t9,1
  800eed58: lui	at,0x8011
  800eed5c: bnez	t8,0x800eed90
  800eed60: lui	a0,0x8014
  800eed64: lui	a1,0x8014
  800eed68: addiu	a1,a1,25084
  800eed6c: sb	t9,18368(at)
  800eed70: addiu	a0,a0,25040
  800eed74: jal	0x80006a00   <osCreateMesgQueue>
  800eed78: li	a2,1
  800eed7c: lui	a0,0x8014
  800eed80: addiu	a0,a0,25040
  800eed84: move	a1,zero
  800eed88: jal	0x800075e0   <osSendMesg>
  800eed8c: move	a2,zero
  800eed90: lw	ra,44(sp)
  800eed94: li	t6,-1
  800eed98: lui	at,0x8015
  800eed9c: lw	s0,36(sp)
  800eeda0: lw	s1,40(sp)
  800eeda4: sb	t6,-25184(at)
  800eeda8: jr	ra
  800eedac: addiu	sp,sp,128
