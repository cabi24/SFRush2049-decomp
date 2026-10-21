== func_803A9EEC @ 0x803A9EEC (315 words, 1260 B)
  803a9eec: 27bdfee8 addiu	sp,sp,-280
  803a9ef0: afbf0074 sw	ra,116(sp)
  803a9ef4: 44806000 mtc1	zero,$f12
  803a9ef8: afbe0070 sw	s8,112(sp)
  803a9efc: afb7006c sw	s7,108(sp)
  803a9f00: afb60068 sw	s6,104(sp)
  803a9f04: afb50064 sw	s5,100(sp)
  803a9f08: afb40060 sw	s4,96(sp)
  803a9f0c: afb3005c sw	s3,92(sp)
  803a9f10: afb20058 sw	s2,88(sp)
  803a9f14: afb10054 sw	s1,84(sp)
  803a9f18: afb00050 sw	s0,80(sp)
  803a9f1c: f7be0048 sdc1	$f30,72(sp)
  803a9f20: f7bc0040 sdc1	$f28,64(sp)
  803a9f24: f7ba0038 sdc1	$f26,56(sp)
  803a9f28: f7b80030 sdc1	$f24,48(sp)
  803a9f2c: f7b60028 sdc1	$f22,40(sp)
  803a9f30: f7b40020 sdc1	$f20,32(sp)
  803a9f34: 0c02d96d jal	0x800b65b4   <render_helper>
  803a9f38: afa40118 sw	a0,280(sp)
  803a9f3c: 3c038015 lui	v1,0x8015
  803a9f40: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803a9f44: 3c0e803c lui	t6,0x803c
  803a9f48: 25ceace0 addiu	t6,t6,-21280   ; 803BACE0 
  803a9f4c: 1860010b blez	v1,0x803aa37c
  803a9f50: 00009025 move	s2,zero
  803a9f54: afa00084 sw	zero,132(sp)
  803a9f58: afae0080 sw	t6,128(sp)
  803a9f5c: 8faf0080 lw	t7,128(sp)
  803a9f60: 8df80000 lw	t8,0(t7)
  803a9f64: 2f010007 sltiu	at,t8,7
  803a9f68: 102000fa beqz	at,0x803aa354
  803a9f6c: 0018c080 sll	t8,t8,0x2
  803a9f70: 3c01803c lui	at,0x803c
  803a9f74: 00380821 addu	at,at,t8
  803a9f78: 8c389890 lw	t8,-26480(at)   ; 803B9890 
  803a9f7c: 03000008 jr	t8
  803a9f80: 00000000 nop
  803a9f84: 0c0e9ed6 jal	0x803a7b58   <func_803A7B58>
  803a9f88: afb20114 sw	s2,276(sp)
  803a9f8c: 8fb20114 lw	s2,276(sp)
  803a9f90: 3c038015 lui	v1,0x8015
  803a9f94: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803a9f98: 100000ef b	0x803aa358
  803a9f9c: 26500001 addiu	s0,s2,1
  803a9fa0: 02409825 move	s3,s2
  803a9fa4: 0c0ea1f5 jal	0x803a87d4   <func_803A87D4>
  803a9fa8: afb20114 sw	s2,276(sp)
  803a9fac: 8fb20114 lw	s2,276(sp)
  803a9fb0: 3c038015 lui	v1,0x8015
  803a9fb4: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803a9fb8: 100000e7 b	0x803aa358
  803a9fbc: 26500001 addiu	s0,s2,1
  803a9fc0: 8fb90084 lw	t9,132(sp)
  803a9fc4: 3c04803c lui	a0,0x803c
  803a9fc8: 240e000a li	t6,10
  803a9fcc: 00992021 addu	a0,a0,t9
  803a9fd0: 8c84ac08 lw	a0,-21496(a0)   ; 803BAC08 
  803a9fd4: afae0010 sw	t6,16(sp)
  803a9fd8: 2405ffec li	a1,-20
  803a9fdc: 00003025 move	a2,zero
  803a9fe0: 0c02ce6f jal	0x800b39bc   <crowd_cheer_play>
  803a9fe4: 2407fff6 li	a3,-10
  803a9fe8: 3c038015 lui	v1,0x8015
  803a9fec: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803a9ff0: 100000d9 b	0x803aa358
  803a9ff4: 26500001 addiu	s0,s2,1
  803a9ff8: afb20000 sw	s2,0(sp)
  803a9ffc: 0c0ea2dd jal	0x803a8b74   <func_803A8B74>
  803aa000: afb20114 sw	s2,276(sp)
  803aa004: 8fb20114 lw	s2,276(sp)
  803aa008: 3c038015 lui	v1,0x8015
  803aa00c: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803aa010: 100000d1 b	0x803aa358
  803aa014: 26500001 addiu	s0,s2,1
  803aa018: 0c0ea422 jal	0x803a9088   <func_803A9088>
  803aa01c: afb20114 sw	s2,276(sp)
  803aa020: 8fb20114 lw	s2,276(sp)
  803aa024: 3c038015 lui	v1,0x8015
  803aa028: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803aa02c: 100000ca b	0x803aa358
  803aa030: 26500001 addiu	s0,s2,1
  803aa034: 00037880 sll	t7,v1,0x2
  803aa038: 0012c080 sll	t8,s2,0x2
  803aa03c: 0312c023 subu	t8,t8,s2
  803aa040: 01e37823 subu	t7,t7,v1
  803aa044: 000f7940 sll	t7,t7,0x5
  803aa048: 0018c0c0 sll	t8,t8,0x3
  803aa04c: 01f8c821 addu	t9,t7,t8
  803aa050: 3c0e803b lui	t6,0x803b
  803aa054: 8faf0084 lw	t7,132(sp)
  803aa058: 25ce5d50 addiu	t6,t6,23888   ; 803B5D50 
  803aa05c: 3c188011 lui	t8,0x8011
  803aa060: 032e1021 addu	v0,t9,t6
  803aa064: 271846ac addiu	t8,t8,18092   ; 801146AC D_801146AC
  803aa068: 28610003 slti	at,v1,3
  803aa06c: 8c51ffa4 lw	s1,-92(v0)
  803aa070: 8c54ffb0 lw	s4,-80(v0)
  803aa074: 14200005 bnez	at,0x803aa08c
  803aa078: 01f89821 addu	s3,t7,t8
  803aa07c: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa080: 2404000b li	a0,11
  803aa084: 1000000a b	0x803aa0b0
  803aa088: 00000000 nop
  803aa08c: 24010002 li	at,2
  803aa090: 14610005 bne	v1,at,0x803aa0a8
  803aa094: 00000000 nop
  803aa098: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa09c: 2404000a li	a0,10
  803aa0a0: 10000003 b	0x803aa0b0
  803aa0a4: 00000000 nop
  803aa0a8: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa0ac: 2404000d li	a0,13
  803aa0b0: 3c198018 lui	t9,0x8018
  803aa0b4: 8f39a4e4 lw	t9,-23324(t9)   ; 8017A4E4 countdown_object
  803aa0b8: 3c05803c lui	a1,0x803c
  803aa0bc: 26500001 addiu	s0,s2,1
  803aa0c0: 02003825 move	a3,s0
  803aa0c4: 24a589ac addiu	a1,a1,-30292   ; 803B89AC 
  803aa0c8: 27a400d0 addiu	a0,sp,208
  803aa0cc: 0c02d0d8 jal	0x800b4360   <fcvt_wrapper>
  803aa0d0: 8f260074 lw	a2,116(t9)   ; 80180074 
  803aa0d4: 0c02dd28 jal	0x800b74a0   <dispatch_handler>
  803aa0d8: 8e640000 lw	a0,0(s3)
  803aa0dc: 00149400 sll	s2,s4,0x10
  803aa0e0: 00127403 sra	t6,s2,0x10
  803aa0e4: 000e2c00 sll	a1,t6,0x10
  803aa0e8: 00057c03 sra	t7,a1,0x10
  803aa0ec: 01e02825 move	a1,t7
  803aa0f0: 01c09025 move	s2,t6
  803aa0f4: 0c02dc68 jal	0x800b71a0   <sound_pitch_diff_halved>
  803aa0f8: 27a400d0 addiu	a0,sp,208
  803aa0fc: 00022400 sll	a0,v0,0x10
  803aa100: 00112c00 sll	a1,s1,0x10
  803aa104: 0005cc03 sra	t9,a1,0x10
  803aa108: 0004c403 sra	t8,a0,0x10
  803aa10c: 03002025 move	a0,t8
  803aa110: 03202825 move	a1,t9
  803aa114: 0c02dc75 jal	0x800b71d4   <state_utility>
  803aa118: 27a600d0 addiu	a2,sp,208
  803aa11c: 0c02cfd4 jal	0x800b3f50   <object_bytes_sum_global>
  803aa120: 00000000 nop
  803aa124: 3c038015 lui	v1,0x8015
  803aa128: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803aa12c: 00027040 sll	t6,v0,0x1
  803aa130: 022e8821 addu	s1,s1,t6
  803aa134: 28610003 slti	at,v1,3
  803aa138: 54200006 bnezl	at,0x803aa154
  803aa13c: 24010002 li	at,2
  803aa140: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa144: 2404000c li	a0,12
  803aa148: 1000000a b	0x803aa174
  803aa14c: 00000000 nop
  803aa150: 24010002 li	at,2
  803aa154: 14610005 bne	v1,at,0x803aa16c
  803aa158: 00000000 nop
  803aa15c: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa160: 2404000b li	a0,11
  803aa164: 10000003 b	0x803aa174
  803aa168: 00000000 nop
  803aa16c: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa170: 2404000a li	a0,10
  803aa174: 0c02dd28 jal	0x800b74a0   <dispatch_handler>
  803aa178: 24040011 li	a0,17
  803aa17c: 00122c00 sll	a1,s2,0x10
  803aa180: 00057c03 sra	t7,a1,0x10
  803aa184: 3c04803c lui	a0,0x803c
  803aa188: 248489b4 addiu	a0,a0,-30284   ; 803B89B4 
  803aa18c: 0c02dc68 jal	0x800b71a0   <sound_pitch_diff_halved>
  803aa190: 01e02825 move	a1,t7
  803aa194: 00022400 sll	a0,v0,0x10
  803aa198: 00112c00 sll	a1,s1,0x10
  803aa19c: 0005cc03 sra	t9,a1,0x10
  803aa1a0: 0004c403 sra	t8,a0,0x10
  803aa1a4: 3c06803c lui	a2,0x803c
  803aa1a8: 24c689b4 addiu	a2,a2,-30284   ; 803B89B4 
  803aa1ac: 03002025 move	a0,t8
  803aa1b0: 0c02dc75 jal	0x800b71d4   <state_utility>
  803aa1b4: 03202825 move	a1,t9
  803aa1b8: 3c038015 lui	v1,0x8015
  803aa1bc: 10000066 b	0x803aa358
  803aa1c0: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803aa1c4: 00037080 sll	t6,v1,0x2
  803aa1c8: 00127880 sll	t7,s2,0x2
  803aa1cc: 01f27823 subu	t7,t7,s2
  803aa1d0: 01c37023 subu	t6,t6,v1
  803aa1d4: 000e7140 sll	t6,t6,0x5
  803aa1d8: 000f78c0 sll	t7,t7,0x3
  803aa1dc: 01cfc021 addu	t8,t6,t7
  803aa1e0: 3c19803b lui	t9,0x803b
  803aa1e4: 8fae0084 lw	t6,132(sp)
  803aa1e8: 27395d50 addiu	t9,t9,23888   ; 803B5D50 
  803aa1ec: 3c0f8011 lui	t7,0x8011
  803aa1f0: 03191021 addu	v0,t8,t9
  803aa1f4: 25ef46ac addiu	t7,t7,18092   ; 801146AC D_801146AC
  803aa1f8: 28610003 slti	at,v1,3
  803aa1fc: 8c51ffa4 lw	s1,-92(v0)
  803aa200: 8c54ffb0 lw	s4,-80(v0)
  803aa204: 14200005 bnez	at,0x803aa21c
  803aa208: 01cf9821 addu	s3,t6,t7
  803aa20c: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa210: 2404000b li	a0,11
  803aa214: 1000000a b	0x803aa240
  803aa218: 00000000 nop
  803aa21c: 24010002 li	at,2
  803aa220: 14610005 bne	v1,at,0x803aa238
  803aa224: 00000000 nop
  803aa228: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa22c: 2404000a li	a0,10
  803aa230: 10000003 b	0x803aa240
  803aa234: 00000000 nop
  803aa238: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa23c: 2404000d li	a0,13
  803aa240: 3c188018 lui	t8,0x8018
  803aa244: 8f18a4e4 lw	t8,-23324(t8)   ; 8017A4E4 countdown_object
  803aa248: 3c05803c lui	a1,0x803c
  803aa24c: 26500001 addiu	s0,s2,1
  803aa250: 02003825 move	a3,s0
  803aa254: 24a589c0 addiu	a1,a1,-30272   ; 803B89C0 
  803aa258: 27a40090 addiu	a0,sp,144
  803aa25c: 0c02d0d8 jal	0x800b4360   <fcvt_wrapper>
  803aa260: 8f060074 lw	a2,116(t8)   ; 80180074 
  803aa264: 0c02dd28 jal	0x800b74a0   <dispatch_handler>
  803aa268: 8e640000 lw	a0,0(s3)
  803aa26c: 00149400 sll	s2,s4,0x10
  803aa270: 0012cc03 sra	t9,s2,0x10
  803aa274: 00192c00 sll	a1,t9,0x10
  803aa278: 00057403 sra	t6,a1,0x10
  803aa27c: 01c02825 move	a1,t6
  803aa280: 03209025 move	s2,t9
  803aa284: 0c02dc68 jal	0x800b71a0   <sound_pitch_diff_halved>
  803aa288: 27a40090 addiu	a0,sp,144
  803aa28c: 00022400 sll	a0,v0,0x10
  803aa290: 00112c00 sll	a1,s1,0x10
  803aa294: 0005c403 sra	t8,a1,0x10
  803aa298: 00047c03 sra	t7,a0,0x10
  803aa29c: 01e02025 move	a0,t7
  803aa2a0: 03002825 move	a1,t8
  803aa2a4: 0c02dc75 jal	0x800b71d4   <state_utility>
  803aa2a8: 27a60090 addiu	a2,sp,144
  803aa2ac: 0c02cfd4 jal	0x800b3f50   <object_bytes_sum_global>
  803aa2b0: 00000000 nop
  803aa2b4: 3c038015 lui	v1,0x8015
  803aa2b8: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803aa2bc: 0002c840 sll	t9,v0,0x1
  803aa2c0: 02398821 addu	s1,s1,t9
  803aa2c4: 28610003 slti	at,v1,3
  803aa2c8: 54200006 bnezl	at,0x803aa2e4
  803aa2cc: 24010002 li	at,2
  803aa2d0: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa2d4: 2404000c li	a0,12
  803aa2d8: 1000000a b	0x803aa304
  803aa2dc: 00000000 nop
  803aa2e0: 24010002 li	at,2
  803aa2e4: 14610005 bne	v1,at,0x803aa2fc
  803aa2e8: 00000000 nop
  803aa2ec: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa2f0: 2404000b li	a0,11
  803aa2f4: 10000003 b	0x803aa304
  803aa2f8: 00000000 nop
  803aa2fc: 0c02d0bc jal	0x800b42f0   <object_create>
  803aa300: 2404000a li	a0,10
  803aa304: 0c02dd28 jal	0x800b74a0   <dispatch_handler>
  803aa308: 24040011 li	a0,17
  803aa30c: 00122c00 sll	a1,s2,0x10
  803aa310: 00057403 sra	t6,a1,0x10
  803aa314: 3c04803c lui	a0,0x803c
  803aa318: 248489c8 addiu	a0,a0,-30264   ; 803B89C8 
  803aa31c: 0c02dc68 jal	0x800b71a0   <sound_pitch_diff_halved>
  803aa320: 01c02825 move	a1,t6
  803aa324: 00022400 sll	a0,v0,0x10
  803aa328: 00112c00 sll	a1,s1,0x10
  803aa32c: 0005c403 sra	t8,a1,0x10
  803aa330: 00047c03 sra	t7,a0,0x10
  803aa334: 3c06803c lui	a2,0x803c
  803aa338: 24c689c8 addiu	a2,a2,-30264   ; 803B89C8 
  803aa33c: 01e02025 move	a0,t7
  803aa340: 0c02dc75 jal	0x800b71d4   <state_utility>
  803aa344: 03002825 move	a1,t8
  803aa348: 3c038015 lui	v1,0x8015
  803aa34c: 10000002 b	0x803aa358
  803aa350: 8463a108 lh	v1,-24312(v1)   ; 8014A108 active_player_count
  803aa354: 26500001 addiu	s0,s2,1
  803aa358: 8fb90084 lw	t9,132(sp)
  803aa35c: 8faf0080 lw	t7,128(sp)
  803aa360: 0203082a slt	at,s0,v1
  803aa364: 272e0004 addiu	t6,t9,4   ; 803B5D54 
  803aa368: 25f80004 addiu	t8,t7,4   ; 801146B0 
  803aa36c: afb80080 sw	t8,128(sp)
  803aa370: afae0084 sw	t6,132(sp)
  803aa374: 1420fef9 bnez	at,0x803a9f5c
  803aa378: 02009025 move	s2,s0
  803aa37c: 3c01bf80 lui	at,0xbf80
  803aa380: 44816000 mtc1	at,$f12
  803aa384: 0c02d96d jal	0x800b65b4   <render_helper>
  803aa388: 00000000 nop
  803aa38c: 8fbf0074 lw	ra,116(sp)
  803aa390: d7b40020 ldc1	$f20,32(sp)
  803aa394: d7b60028 ldc1	$f22,40(sp)
  803aa398: d7b80030 ldc1	$f24,48(sp)
  803aa39c: d7ba0038 ldc1	$f26,56(sp)
  803aa3a0: d7bc0040 ldc1	$f28,64(sp)
  803aa3a4: d7be0048 ldc1	$f30,72(sp)
  803aa3a8: 8fb00050 lw	s0,80(sp)
  803aa3ac: 8fb10054 lw	s1,84(sp)
  803aa3b0: 8fb20058 lw	s2,88(sp)
  803aa3b4: 8fb3005c lw	s3,92(sp)
  803aa3b8: 8fb40060 lw	s4,96(sp)
  803aa3bc: 8fb50064 lw	s5,100(sp)
  803aa3c0: 8fb60068 lw	s6,104(sp)
  803aa3c4: 8fb7006c lw	s7,108(sp)
  803aa3c8: 8fbe0070 lw	s8,112(sp)
  803aa3cc: 27bd0118 addiu	sp,sp,280
  803aa3d0: 03e00008 jr	ra
  803aa3d4: 24020001 li	v0,1
