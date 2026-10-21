== RaceStateMachine_Update @ 0x800DB81C (537 words)
  800db81c: addiu	sp,sp,-272
  800db820: lui	v1,0x8011
  800db824: lw	v1,28924(v1)
  800db828: sw	ra,60(sp)
  800db82c: sw	s5,56(sp)
  800db830: sw	s4,52(sp)
  800db834: sw	s3,48(sp)
  800db838: sw	s2,44(sp)
  800db83c: sw	s1,40(sp)
  800db840: sw	s0,36(sp)
  800db844: sdc1	$f22,24(sp)
  800db848: beqz	v1,0x800db864
  800db84c: sdc1	$f20,16(sp)
  800db850: move	a0,zero
  800db854: jal	0x800c813c   <SaveServer>
  800db858: move	a1,zero
  800db85c: lui	v1,0x8011
  800db860: lw	v1,28924(v1)
  800db864: sltiu	at,v1,9
  800db868: beqz	at,0x800dbf68
  800db86c: li	t0,2
  800db870: sll	t6,v1,0x2
  800db874: lui	at,0x8012
  800db878: addu	at,at,t6
  800db87c: lw	t6,17028(at)
  800db880: jr	t6
  800db884: nop
  800db888: lui	a1,0x8015
  800db88c: lh	a1,6864(a1)
  800db890: lui	a0,0x8015
  800db894: addiu	a0,a0,-24296
  800db898: blez	a1,0x800dc054
  800db89c: move	v1,zero
  800db8a0: lui	a2,0x8015
  800db8a4: addiu	a2,a2,27888
  800db8a8: lw	v0,4(a0)
  800db8ac: sll	t9,v1,0x4
  800db8b0: addu	t6,a2,t9
  800db8b4: andi	t7,v0,0x1
  800db8b8: sltu	v0,zero,t7
  800db8bc: bnez	v0,0x800db8d8
  800db8c0: sll	t9,v1,0x4
  800db8c4: lb	v0,0(t6)
  800db8c8: sltiu	t7,v0,1
  800db8cc: bnez	t7,0x800db8d8
  800db8d0: move	v0,t7
  800db8d4: move	v0,zero
  800db8d8: beqz	v0,0x800db900
  800db8dc: addu	t6,a2,t9
  800db8e0: lb	v0,0(t6)
  800db8e4: lui	a1,0x8015
  800db8e8: addiu	a1,a1,27020
  800db8ec: sw	v1,0(a1)
  800db8f0: lui	at,0x8011
  800db8f4: sltiu	t7,v0,1
  800db8f8: b	0x800db918
  800db8fc: sb	t7,29520(at)
  800db900: addiu	v1,v1,1
  800db904: slt	at,v1,a1
  800db908: bnez	at,0x800db8a8
  800db90c: addiu	a0,a0,76
  800db910: b	0x800dc058
  800db914: lw	ra,60(sp)
  800db918: jal	0x800b4fb0   <func_800B4FB0>
  800db91c: li	a0,1
  800db920: lui	v1,0x8015
  800db924: addiu	v1,v1,-25192
  800db928: jal	0x800013c0   <viEnableAccum>
  800db92c: sw	zero,0(v1)
  800db930: lui	a0,0x8014
  800db934: addiu	a0,a0,10024
  800db938: move	a1,zero
  800db93c: jal	0x80007270   <osRecvMesg>
  800db940: li	a2,1
  800db944: jal	0x80091b00   <func_80091B00>
  800db948: nop
  800db94c: sw	v0,244(sp)
  800db950: li	t8,7
  800db954: lui	a0,0x8014
  800db958: addiu	a0,a0,10024
  800db95c: sb	t8,2(v0)
  800db960: move	a1,zero
  800db964: jal	0x800075e0   <osSendMesg>
  800db968: move	a2,zero
  800db96c: lui	a0,0x8014
  800db970: addiu	a0,a0,10152
  800db974: lw	a1,244(sp)
  800db978: jal	0x800075e0   <osSendMesg>
  800db97c: move	a2,zero
  800db980: mtc1	zero,$f20
  800db984: lui	at,0x8012
  800db988: lwc1	$f22,17064(at)
  800db98c: li	s1,1
  800db990: jal	0x800c9210   <speed_set>
  800db994: move	s2,zero
  800db998: jal	0x800d5374   <func_800D5374>
  800db99c: nop
  800db9a0: jal	0x800d5798   <players_frame_update>
  800db9a4: nop
  800db9a8: lui	v0,0x8014
  800db9ac: lw	v0,16344(v0)
  800db9b0: li	t0,2
  800db9b4: li	v1,-1
  800db9b8: beqz	v0,0x800db9d4
  800db9bc: li	a0,-1
  800db9c0: sb	t0,100(v0)
  800db9c4: sw	v1,96(v0)
  800db9c8: lw	v0,0(v0)
  800db9cc: bnezl	v0,0x800db9c4
  800db9d0: sb	t0,100(v0)
  800db9d4: jal	0x800c84c0   <player_mode_set>
  800db9d8: li	a1,1
  800db9dc: jal	0x800b55fc   <visual_objects_update>
  800db9e0: li	a0,1
  800db9e4: li	a0,46
  800db9e8: move	a1,zero
  800db9ec: li	a2,1
  800db9f0: jal	0x80092360   <entity_flags_apply,frame_sync>
  800db9f4: move	a3,zero
  800db9f8: lui	v1,0x8011
  800db9fc: li	t0,2
  800dba00: b	0x800dbf68
  800dba04: lw	v1,28924(v1)
  800dba08: jal	0x800db1e0   <func_800DB1E0>
  800dba0c: nop
  800dba10: lui	v1,0x8011
  800dba14: lw	v1,28924(v1)
  800dba18: b	0x800dbf68
  800dba1c: li	t0,2
  800dba20: jal	0x800d91a0   <func_800D91A0>
  800dba24: nop
  800dba28: lui	v1,0x8011
  800dba2c: lw	v1,28924(v1)
  800dba30: b	0x800dbf68
  800dba34: li	t0,2
  800dba38: jal	0x800d7634   <func_800D7634>
  800dba3c: nop
  800dba40: lui	v1,0x8011
  800dba44: lw	v1,28924(v1)
  800dba48: b	0x800dbf68
  800dba4c: li	t0,2
  800dba50: jal	0x800b8c14   <physics_sym>
  800dba54: nop
  800dba58: lui	v1,0x8011
  800dba5c: lw	v1,28924(v1)
  800dba60: b	0x800dbf68
  800dba64: li	t0,2
  800dba68: lui	t9,0x8015
  800dba6c: lw	t9,27020(t9)
  800dba70: lui	a0,0x8015
  800dba74: move	a1,zero
  800dba78: sll	t6,t9,0x2
  800dba7c: addu	t6,t6,t9
  800dba80: sll	t6,t6,0x2
  800dba84: subu	t6,t6,t9
  800dba88: sll	t6,t6,0x2
  800dba8c: addu	a0,a0,t6
  800dba90: lw	a0,-24292(a0)
  800dba94: li	a2,1
  800dba98: move	a3,zero
  800dba9c: andi	t7,a0,0x2
  800dbaa0: beqzl	t7,0x800dbc8c
  800dbaa4: andi	t9,a0,0x400
  800dbaa8: jal	0x80092360   <entity_flags_apply,frame_sync>
  800dbaac: li	a0,37
  800dbab0: lui	v1,0x8015
  800dbab4: addiu	v1,v1,-25192
  800dbab8: lw	t8,0(v1)
  800dbabc: sltiu	at,t8,8
  800dbac0: beqz	at,0x800dbc78
  800dbac4: sll	t8,t8,0x2
  800dbac8: lui	at,0x8012
  800dbacc: addu	at,at,t8
  800dbad0: lw	t8,17068(at)
  800dbad4: jr	t8
  800dbad8: nop
  800dbadc: jal	0x800b4fb0   <func_800B4FB0>
  800dbae0: move	a0,zero
  800dbae4: jal	0x800013dc   <viDisableAccum>
  800dbae8: nop
  800dbaec: jal	0x800d6530   <func_800D6530>
  800dbaf0: nop
  800dbaf4: lui	a0,0x8014
  800dbaf8: addiu	a0,a0,10024
  800dbafc: move	a1,zero
  800dbb00: jal	0x80007270   <osRecvMesg>
  800dbb04: li	a2,1
  800dbb08: jal	0x80091b00   <func_80091B00>
  800dbb0c: nop
  800dbb10: sw	v0,180(sp)
  800dbb14: li	t9,9
  800dbb18: lui	a0,0x8014
  800dbb1c: addiu	a0,a0,10024
  800dbb20: sb	t9,2(v0)
  800dbb24: move	a1,zero
  800dbb28: jal	0x800075e0   <osSendMesg>
  800dbb2c: move	a2,zero
  800dbb30: lui	a0,0x8014
  800dbb34: addiu	a0,a0,10152
  800dbb38: lw	a1,180(sp)
  800dbb3c: jal	0x800075e0   <osSendMesg>
  800dbb40: move	a2,zero
  800dbb44: jal	0x800d6160   <func_800D6160>
  800dbb48: li	t0,1
  800dbb4c: lui	t6,0x8011
  800dbb50: lw	t6,29876(t6)
  800dbb54: sll	t7,t6,0xa
  800dbb58: bltz	t7,0x800dbb68
  800dbb5c: nop
  800dbb60: jal	0x800d6160   <func_800D6160>
  800dbb64: move	t0,zero
  800dbb68: jal	0x800d60ac   <func_800D60AC>
  800dbb6c: nop
  800dbb70: li	a0,-1
  800dbb74: jal	0x800c84c0   <player_mode_set>
  800dbb78: move	a1,zero
  800dbb7c: b	0x800dc058
  800dbb80: lw	ra,60(sp)
  800dbb84: lui	at,0x8015
  800dbb88: lwc1	$f4,17356(at)
  800dbb8c: lui	at,0x4170
  800dbb90: mtc1	at,$f6
  800dbb94: lui	at,0x8011
  800dbb98: li	t8,1
  800dbb9c: c.lt.s	$f4,$f6
  800dbba0: nop
  800dbba4: bc1f	0x800dbbb4
  800dbba8: nop
  800dbbac: b	0x800dbe68
  800dbbb0: sb	t8,18000(at)
  800dbbb4: jal	0x800b4fb0   <func_800B4FB0>
  800dbbb8: li	a0,7
  800dbbbc: b	0x800dbc78
  800dbbc0: nop
  800dbbc4: lui	at,0x8015
  800dbbc8: lwc1	$f8,17356(at)
  800dbbcc: lui	at,0x4170
  800dbbd0: mtc1	at,$f10
  800dbbd4: lui	t9,0x8011
  800dbbd8: c.lt.s	$f8,$f10
  800dbbdc: nop
  800dbbe0: bc1f	0x800dbc08
  800dbbe4: nop
  800dbbe8: lw	t9,29876(t9)
  800dbbec: lui	at,0x8011
  800dbbf0: sb	zero,18000(at)
  800dbbf4: sll	t6,t9,0x9
  800dbbf8: bgez	t6,0x800dbe68
  800dbbfc: nop
  800dbc00: b	0x800dbde0
  800dbc04: li	t9,1
  800dbc08: jal	0x800b4fb0   <func_800B4FB0>
  800dbc0c: li	a0,8
  800dbc10: b	0x800dbc78
  800dbc14: nop
  800dbc18: jal	0x800b4fb0   <func_800B4FB0>
  800dbc1c: li	a0,2
  800dbc20: b	0x800dbc78
  800dbc24: nop
  800dbc28: jal	0x800b4fb0   <func_800B4FB0>
  800dbc2c: li	a0,3
  800dbc30: b	0x800dbc78
  800dbc34: nop
  800dbc38: jal	0x800b4fb0   <func_800B4FB0>
  800dbc3c: li	a0,5
  800dbc40: b	0x800dbc78
  800dbc44: nop
  800dbc48: lui	t8,0x8015
  800dbc4c: lw	t8,27020(t8)
  800dbc50: lui	at,0x8015
  800dbc54: li	t7,1
  800dbc58: addu	at,at,t8
  800dbc5c: sb	t7,16780(at)
  800dbc60: jal	0x800b4fb0   <func_800B4FB0>
  800dbc64: li	a0,4
  800dbc68: b	0x800dbc78
  800dbc6c: nop
  800dbc70: jal	0x800b4fb0   <func_800B4FB0>
  800dbc74: li	a0,6
  800dbc78: lui	v1,0x8011
  800dbc7c: lw	v1,28924(v1)
  800dbc80: b	0x800dbf68
  800dbc84: li	t0,2
  800dbc88: andi	t9,a0,0x400
  800dbc8c: beqz	t9,0x800dbccc
  800dbc90: andi	t6,a0,0x800
  800dbc94: jal	0x800b5f4c   <audio_distance_atten>
  800dbc98: nop
  800dbc9c: lui	v1,0x8015
  800dbca0: addiu	v1,v1,-25192
  800dbca4: lw	t6,0(v1)
  800dbca8: li	t9,7
  800dbcac: li	t0,2
  800dbcb0: addiu	t7,t6,-1
  800dbcb4: bgez	t7,0x800dbcc0
  800dbcb8: sw	t7,0(v1)
  800dbcbc: sw	t9,0(v1)
  800dbcc0: lui	v1,0x8011
  800dbcc4: b	0x800dbf68
  800dbcc8: lw	v1,28924(v1)
  800dbccc: beqzl	t6,0x800dbd10
  800dbcd0: andi	t6,a0,0x5
  800dbcd4: jal	0x800b5f4c   <audio_distance_atten>
  800dbcd8: nop
  800dbcdc: lui	v1,0x8015
  800dbce0: addiu	v1,v1,-25192
  800dbce4: lw	t7,0(v1)
  800dbce8: li	t0,2
  800dbcec: addiu	t8,t7,1
  800dbcf0: slti	at,t8,8
  800dbcf4: bnez	at,0x800dbd00
  800dbcf8: sw	t8,0(v1)
  800dbcfc: sw	zero,0(v1)
  800dbd00: lui	v1,0x8011
  800dbd04: b	0x800dbf68
  800dbd08: lw	v1,28924(v1)
  800dbd0c: andi	t6,a0,0x5
  800dbd10: beqz	t6,0x800dbd28
  800dbd14: nop
  800dbd18: jal	0x800b5fc4   <resource_type_select>
  800dbd1c: nop
  800dbd20: b	0x800dbadc
  800dbd24: nop
  800dbd28: b	0x800dbf68
  800dbd2c: li	t0,2
  800dbd30: lui	t7,0x8015
  800dbd34: lw	t7,27020(t7)
  800dbd38: lui	a0,0x8015
  800dbd3c: lui	v1,0x8011
  800dbd40: sll	t8,t7,0x2
  800dbd44: addu	t8,t8,t7
  800dbd48: sll	t8,t8,0x2
  800dbd4c: subu	t8,t8,t7
  800dbd50: sll	t8,t8,0x2
  800dbd54: addu	a0,a0,t8
  800dbd58: lw	a0,-24292(a0)
  800dbd5c: addiu	v1,v1,29524
  800dbd60: andi	t9,a0,0x3000
  800dbd64: beqzl	t9,0x800dbda8
  800dbd68: andi	t9,a0,0x2
  800dbd6c: lb	v0,0(v1)
  800dbd70: sltiu	t6,v0,1
  800dbd74: jal	0x800b5f88   <audio_doppler>
  800dbd78: sb	t6,0(v1)
  800dbd7c: lui	t7,0x8015
  800dbd80: lw	t7,27020(t7)
  800dbd84: lui	a0,0x8015
  800dbd88: sll	t8,t7,0x2
  800dbd8c: addu	t8,t8,t7
  800dbd90: sll	t8,t8,0x2
  800dbd94: subu	t8,t8,t7
  800dbd98: sll	t8,t8,0x2
  800dbd9c: addu	a0,a0,t8
  800dbda0: lw	a0,-24292(a0)
  800dbda4: andi	t9,a0,0x2
  800dbda8: beqz	t9,0x800dbee8
  800dbdac: lui	t6,0x8011
  800dbdb0: lb	t6,29524(t6)
  800dbdb4: lui	t7,0x8011
  800dbdb8: beqz	t6,0x800dbed8
  800dbdbc: nop
  800dbdc0: lw	t7,29876(t7)
  800dbdc4: lui	v1,0x8011
  800dbdc8: lw	v1,28924(v1)
  800dbdcc: sll	t8,t7,0x9
  800dbdd0: bgez	t8,0x800dbe58
  800dbdd4: li	at,8
  800dbdd8: bne	v1,at,0x800dbe58
  800dbddc: li	t9,1
  800dbde0: lui	at,0x8014
  800dbde4: sb	t9,9881(at)
  800dbde8: jal	0x800b4fb0   <func_800B4FB0>
  800dbdec: move	a0,zero
  800dbdf0: jal	0x800013dc   <viDisableAccum>
  800dbdf4: nop
  800dbdf8: lui	a0,0x8014
  800dbdfc: addiu	a0,a0,10024
  800dbe00: move	a1,zero
  800dbe04: jal	0x80007270   <osRecvMesg>
  800dbe08: li	a2,1
  800dbe0c: jal	0x80091b00   <func_80091B00>
  800dbe10: nop
  800dbe14: sw	v0,132(sp)
  800dbe18: li	t6,7
  800dbe1c: lui	a0,0x8014
  800dbe20: addiu	a0,a0,10024
  800dbe24: sb	t6,2(v0)
  800dbe28: move	a1,zero
  800dbe2c: jal	0x800075e0   <osSendMesg>
  800dbe30: move	a2,zero
  800dbe34: lui	a0,0x8014
  800dbe38: addiu	a0,a0,10152
  800dbe3c: lw	a1,132(sp)
  800dbe40: jal	0x800075e0   <osSendMesg>
  800dbe44: move	a2,zero
  800dbe48: beqz	v0,0x800dbf04
  800dbe4c: nop
  800dbe50: b	0x800dbf04
  800dbe54: nop
  800dbe58: xori	t7,v1,0x7
  800dbe5c: sltiu	t7,t7,1
  800dbe60: lui	at,0x8011
  800dbe64: sb	t7,18000(at)
  800dbe68: jal	0x800b4fb0   <func_800B4FB0>
  800dbe6c: move	a0,zero
  800dbe70: jal	0x800013dc   <viDisableAccum>
  800dbe74: nop
  800dbe78: lui	a0,0x8014
  800dbe7c: addiu	a0,a0,10024
  800dbe80: move	a1,zero
  800dbe84: jal	0x80007270   <osRecvMesg>
  800dbe88: li	a2,1
  800dbe8c: jal	0x80091b00   <func_80091B00>
  800dbe90: nop
  800dbe94: sw	v0,100(sp)
  800dbe98: li	t8,7
  800dbe9c: lui	a0,0x8014
  800dbea0: addiu	a0,a0,10024
  800dbea4: sb	t8,2(v0)
  800dbea8: move	a1,zero
  800dbeac: jal	0x800075e0   <osSendMesg>
  800dbeb0: move	a2,zero
  800dbeb4: lui	a0,0x8014
  800dbeb8: addiu	a0,a0,10152
  800dbebc: lw	a1,100(sp)
  800dbec0: jal	0x800075e0   <osSendMesg>
  800dbec4: move	a2,zero
  800dbec8: jal	0x800d5a04   <func_800D5A04>
  800dbecc: nop
  800dbed0: b	0x800dbf04
  800dbed4: nop
  800dbed8: jal	0x800b4fb0   <func_800B4FB0>
  800dbedc: li	a0,1
  800dbee0: b	0x800dbf04
  800dbee4: nop
  800dbee8: andi	t9,a0,0x5
  800dbeec: beqz	t9,0x800dbf04
  800dbef0: nop
  800dbef4: jal	0x800b5fc4   <resource_type_select>
  800dbef8: nop
  800dbefc: jal	0x800b4fb0   <func_800B4FB0>
  800dbf00: li	a0,1
  800dbf04: lui	v1,0x8011
  800dbf08: lw	v1,28924(v1)
  800dbf0c: b	0x800dbf68
  800dbf10: li	t0,2
  800dbf14: lui	t6,0x8015
  800dbf18: lw	t6,27020(t6)
  800dbf1c: lui	a0,0x8015
  800dbf20: sll	t7,t6,0x2
  800dbf24: addu	t7,t7,t6
  800dbf28: sll	t7,t7,0x2
  800dbf2c: subu	t7,t7,t6
  800dbf30: sll	t7,t7,0x2
  800dbf34: addu	a0,a0,t7
  800dbf38: lw	a0,-24292(a0)
  800dbf3c: andi	t8,a0,0x7
  800dbf40: beqz	t8,0x800dbf60
  800dbf44: nop
  800dbf48: jal	0x800b5fc4   <resource_type_select>
  800dbf4c: nop
  800dbf50: jal	0x800b4fb0   <func_800B4FB0>
  800dbf54: li	a0,1
  800dbf58: lui	v1,0x8011
  800dbf5c: lw	v1,28924(v1)
  800dbf60: b	0x800dbf68
  800dbf64: li	t0,2
  800dbf68: beqz	v1,0x800dbfd8
  800dbf6c: lui	t9,0x8015
  800dbf70: lw	t9,27020(t9)
  800dbf74: lui	t7,0x8015
  800dbf78: addiu	t7,t7,27888
  800dbf7c: sll	t6,t9,0x4
  800dbf80: addu	a0,t6,t7
  800dbf84: lb	v0,0(a0)
  800dbf88: lui	t9,0x8011
  800dbf8c: lb	t9,29520(t9)
  800dbf90: sltiu	t8,v0,1
  800dbf94: beq	t8,t9,0x800dbfd8
  800dbf98: nop
  800dbf9c: lb	v0,0(a0)
  800dbfa0: lui	at,0x8011
  800dbfa4: sltiu	t6,v0,1
  800dbfa8: sb	t6,29520(at)
  800dbfac: li	at,5
  800dbfb0: beq	v1,at,0x800dbfd8
  800dbfb4: li	at,4
  800dbfb8: beq	v1,at,0x800dbfd8
  800dbfbc: nop
  800dbfc0: beq	t0,v1,0x800dbfd8
  800dbfc4: li	at,3
  800dbfc8: beq	v1,at,0x800dbfd8
  800dbfcc: nop
  800dbfd0: jal	0x800b438c   <voice_stop>
  800dbfd4: nop
  800dbfd8: lui	t7,0x8015
  800dbfdc: lb	t7,29252(t7)
  800dbfe0: beqz	t7,0x800dc054
  800dbfe4: lui	at,0x8011
  800dbfe8: sb	zero,18000(at)
  800dbfec: jal	0x800b4fb0   <func_800B4FB0>
  800dbff0: move	a0,zero
  800dbff4: jal	0x800013dc   <viDisableAccum>
  800dbff8: nop
  800dbffc: lui	a0,0x8014
  800dc000: addiu	a0,a0,10024
  800dc004: move	a1,zero
  800dc008: jal	0x80007270   <osRecvMesg>
  800dc00c: li	a2,1
  800dc010: jal	0x80091b00   <func_80091B00>
  800dc014: nop
  800dc018: sw	v0,68(sp)
  800dc01c: li	t8,7
  800dc020: lui	a0,0x8014
  800dc024: addiu	a0,a0,10024
  800dc028: sb	t8,2(v0)
  800dc02c: move	a1,zero
  800dc030: jal	0x800075e0   <osSendMesg>
  800dc034: move	a2,zero
  800dc038: lui	a0,0x8014
  800dc03c: addiu	a0,a0,10152
  800dc040: lw	a1,68(sp)
  800dc044: jal	0x800075e0   <osSendMesg>
  800dc048: move	a2,zero
  800dc04c: jal	0x800d5a04   <func_800D5A04>
  800dc050: nop
  800dc054: lw	ra,60(sp)
  800dc058: ldc1	$f20,16(sp)
  800dc05c: ldc1	$f22,24(sp)
  800dc060: lw	s0,36(sp)
  800dc064: lw	s1,40(sp)
  800dc068: lw	s2,44(sp)
  800dc06c: lw	s3,48(sp)
  800dc070: lw	s4,52(sp)
  800dc074: lw	s5,56(sp)
  800dc078: jr	ra
  800dc07c: addiu	sp,sp,272
