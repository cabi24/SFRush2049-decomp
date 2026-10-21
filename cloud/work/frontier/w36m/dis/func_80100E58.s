== func_80100E58 @ 0x80100E58 (683 words)
  80100e58: lui	t6,0x8015
  80100e5c: lh	t6,-24312(t6)
  80100e60: addiu	sp,sp,-528
  80100e64: lui	t8,0x8015
  80100e68: addiu	t7,t6,1
  80100e6c: sw	ra,28(sp)
  80100e70: sw	t7,468(sp)
  80100e74: addiu	t8,t8,17354
  80100e78: lh	t9,0(t8)
  80100e7c: li	at,1
  80100e80: lui	t6,0x8015
  80100e84: beq	t9,at,0x80100ec8
  80100e88: addiu	s0,sp,444
  80100e8c: lw	t6,-24304(t6)
  80100e90: li	at,3
  80100e94: lui	t7,0x8015
  80100e98: bnel	t6,at,0x80100ed8
  80100e9c: li	t7,4
  80100ea0: lbu	t7,17364(t7)
  80100ea4: lui	t9,0x8015
  80100ea8: sll	t8,t7,0x4
  80100eac: subu	t8,t8,t7
  80100eb0: sll	t8,t8,0x3
  80100eb4: subu	t8,t8,t7
  80100eb8: sll	t8,t8,0x3
  80100ebc: addu	t9,t9,t8
  80100ec0: lb	t9,10503(t9)
  80100ec4: bnez	t9,0x80100ed4
  80100ec8: li	t6,3
  80100ecc: b	0x80100edc
  80100ed0: sw	t6,464(sp)
  80100ed4: li	t7,4
  80100ed8: sw	t7,464(sp)
  80100edc: lw	t8,468(sp)
  80100ee0: move	v1,zero
  80100ee4: blezl	t8,0x80101024
  80100ee8: mtc1	zero,$f12
  80100eec: lw	t9,464(sp)
  80100ef0: move	s5,zero
  80100ef4: blezl	t9,0x80101010
  80100ef8: lw	t6,468(sp)
  80100efc: bnez	v1,0x80100f08
  80100f00: lw	t6,464(sp)
  80100f04: beqz	s5,0x80100ffc
  80100f08: sll	t7,t6,0x4
  80100f0c: subu	t7,t7,t6
  80100f10: sll	t7,t7,0x2
  80100f14: sll	t3,v1,0x3
  80100f18: subu	t3,t3,v1
  80100f1c: sll	s1,s5,0x4
  80100f20: subu	s1,s1,s5
  80100f24: sll	t3,t3,0x1
  80100f28: bgez	t7,0x80100f38
  80100f2c: sra	t8,t7,0x1
  80100f30: addiu	at,t7,1
  80100f34: sra	t8,at,0x1
  80100f38: li	t9,160
  80100f3c: subu	s8,t9,t8
  80100f40: addiu	t3,t3,36
  80100f44: blez	s5,0x80100fcc
  80100f48: sll	s1,s1,0x2
  80100f4c: blez	v1,0x80100fcc
  80100f50: li	t6,192
  80100f54: lui	v0,0x8014
  80100f58: addu	v0,v0,v1
  80100f5c: lb	v0,16211(v0)
  80100f60: sb	zero,444(sp)
  80100f64: sb	zero,445(sp)
  80100f68: sb	zero,446(sp)
  80100f6c: beqz	v0,0x80100fa0
  80100f70: sb	t6,447(sp)
  80100f74: li	at,1
  80100f78: beq	v0,at,0x80100fac
  80100f7c: li	t9,128
  80100f80: li	at,2
  80100f84: beq	v0,at,0x80100fb4
  80100f88: li	t8,128
  80100f8c: li	at,3
  80100f90: beq	v0,at,0x80100fc4
  80100f94: li	t7,128
  80100f98: b	0x80100fe4
  80100f9c: addu	t1,s8,s1
  80100fa0: li	t7,128
  80100fa4: b	0x80100fe0
  80100fa8: sb	t7,446(sp)
  80100fac: b	0x80100fe0
  80100fb0: sb	t9,444(sp)
  80100fb4: li	t6,128
  80100fb8: sb	t6,445(sp)
  80100fbc: b	0x80100fe0
  80100fc0: sb	t8,444(sp)
  80100fc4: b	0x80100fe0
  80100fc8: sb	t7,445(sp)
  80100fcc: li	t9,192
  80100fd0: sb	zero,444(sp)
  80100fd4: sb	zero,445(sp)
  80100fd8: sb	zero,446(sp)
  80100fdc: sb	t9,447(sp)
  80100fe0: addu	t1,s8,s1
  80100fe4: li	t2,60
  80100fe8: li	t4,14
  80100fec: move	a2,s0
  80100ff0: jal	0x80100d5c   <func_80100D5C>
  80100ff4: sw	v1,460(sp)
  80100ff8: lw	v1,460(sp)
  80100ffc: lw	t8,464(sp)
  80101000: addiu	s5,s5,1
  80101004: bne	s5,t8,0x80100efc
  80101008: nop
  8010100c: lw	t6,468(sp)
  80101010: addiu	v1,v1,1
  80101014: bnel	v1,t6,0x80100ef0
  80101018: lw	t9,464(sp)
  8010101c: move	v1,zero
  80101020: mtc1	zero,$f12
  80101024: jal	0x800b65b4   <render_helper>
  80101028: sw	v1,460(sp)
  8010102c: lui	v1,0x8012
  80101030: lui	s7,0x8014
  80101034: addiu	s7,s7,25040
  80101038: addiu	v1,v1,-29152
  8010103c: li	a2,1
  80101040: sw	a2,0(v1)
  80101044: sw	a2,4(v1)
  80101048: move	a0,s7
  8010104c: jal	0x80007270   <osRecvMesg>
  80101050: move	a1,zero
  80101054: jal	0x800b4200   <slot_state_setup>
  80101058: li	s2,11
  8010105c: move	s0,v0
  80101060: move	a0,s7
  80101064: move	a1,zero
  80101068: jal	0x800075e0   <osSendMesg>
  8010106c: move	a2,zero
  80101070: lw	t7,468(sp)
  80101074: lui	at,0x447a
  80101078: lw	v0,464(sp)
  8010107c: blezl	t7,0x801018d8
  80101080: lui	at,0xbf80
  80101084: mtc1	at,$f24
  80101088: lui	at,0x4270
  8010108c: mtc1	at,$f22
  80101090: lw	s6,512(sp)
  80101094: blez	v0,0x801018bc
  80101098: move	s5,zero
  8010109c: lw	t9,460(sp)
  801010a0: bnez	t9,0x801010b0
  801010a4: nop
  801010a8: beqzl	s5,0x801018b4
  801010ac: addiu	s5,s5,1
  801010b0: bnez	s5,0x801011f0
  801010b4: lw	t9,460(sp)
  801010b8: lw	t8,460(sp)
  801010bc: sll	t7,v0,0x4
  801010c0: subu	t7,t7,v0
  801010c4: sll	t7,t7,0x2
  801010c8: sll	s3,t8,0x3
  801010cc: lui	t6,0x8014
  801010d0: subu	s3,s3,t8
  801010d4: sll	s3,s3,0x1
  801010d8: addiu	t6,t6,16212
  801010dc: addiu	s3,s3,43
  801010e0: sll	s1,s5,0x4
  801010e4: addu	s4,t8,t6
  801010e8: li	t6,160
  801010ec: subu	s1,s1,s5
  801010f0: sll	s1,s1,0x2
  801010f4: move	a0,s7
  801010f8: move	a1,zero
  801010fc: li	a2,1
  80101100: bgez	t7,0x80101110
  80101104: sra	t9,t7,0x1
  80101108: addiu	at,t7,1
  8010110c: sra	t9,at,0x1
  80101110: sll	t7,s3,0x10
  80101114: sra	s3,t7,0x10
  80101118: jal	0x80007270   <osRecvMesg>
  8010111c: subu	s8,t6,t9
  80101120: li	s2,11
  80101124: sw	s1,40(sp)
  80101128: jal	0x800b4200   <slot_state_setup>
  8010112c: sw	s3,36(sp)
  80101130: lw	s1,40(sp)
  80101134: lw	s3,36(sp)
  80101138: move	s0,v0
  8010113c: move	a0,s7
  80101140: move	a1,zero
  80101144: jal	0x800075e0   <osSendMesg>
  80101148: move	a2,zero
  8010114c: lb	v0,-1(s4)
  80101150: lui	t9,0x8015
  80101154: lh	t9,-24312(t9)
  80101158: lui	a0,0x8011
  8010115c: sll	t8,v0,0x2
  80101160: slt	at,v0,t9
  80101164: beqz	at,0x801011a4
  80101168: addu	a0,a0,t8
  8010116c: jal	0x800b74a0   <dispatch_handler>
  80101170: lw	a0,18092(a0)
  80101174: lb	t7,-1(s4)
  80101178: lui	t9,0x8015
  8010117c: sll	t6,t7,0x2
  80101180: addu	t6,t6,t7
  80101184: sll	t6,t6,0x2
  80101188: subu	t6,t6,t7
  8010118c: sll	t6,t6,0x2
  80101190: addu	t9,t9,t6
  80101194: lw	t9,-24224(t9)
  80101198: lw	s6,0(t9)
  8010119c: b	0x801011b4
  801011a0: addiu	s6,s6,20
  801011a4: lui	s6,0x8012
  801011a8: addiu	s6,s6,796
  801011ac: jal	0x800b74a0   <dispatch_handler>
  801011b0: li	a0,4
  801011b4: addiu	a0,sp,208
  801011b8: move	a1,s6
  801011bc: jal	0x800be7bc   <race_position_update>
  801011c0: li	a2,60
  801011c4: addu	a0,s8,s1
  801011c8: addiu	a0,a0,30
  801011cc: sll	a1,s3,0x10
  801011d0: sra	t6,a1,0x10
  801011d4: sll	t8,a0,0x10
  801011d8: sra	a0,t8,0x10
  801011dc: move	a1,t6
  801011e0: jal	0x800b71d4   <state_utility>
  801011e4: addiu	a2,sp,208
  801011e8: b	0x80101840
  801011ec: lw	v0,464(sp)
  801011f0: bnez	t9,0x80101278
  801011f4: li	at,1
  801011f8: move	a0,s7
  801011fc: move	a1,zero
  80101200: jal	0x80007270   <osRecvMesg>
  80101204: li	a2,1
  80101208: jal	0x800b4200   <slot_state_setup>
  8010120c: li	s2,11
  80101210: move	s0,v0
  80101214: move	a0,s7
  80101218: move	a1,zero
  8010121c: jal	0x800075e0   <osSendMesg>
  80101220: move	a2,zero
  80101224: jal	0x800b74a0   <dispatch_handler>
  80101228: li	a0,1
  8010122c: li	at,1
  80101230: bne	s5,at,0x80101244
  80101234: lui	t8,0x8018
  80101238: lw	t8,-23324(t8)
  8010123c: b	0x80101270
  80101240: lw	s6,832(t8)
  80101244: li	at,2
  80101248: bne	s5,at,0x8010125c
  8010124c: lui	t7,0x8018
  80101250: lw	t7,-23324(t7)
  80101254: b	0x80101270
  80101258: lw	s6,840(t7)
  8010125c: li	at,3
  80101260: bne	s5,at,0x80101270
  80101264: lui	t6,0x8018
  80101268: lw	t6,-23324(t6)
  8010126c: lw	s6,88(t6)
  80101270: b	0x80101840
  80101274: lw	v0,464(sp)
  80101278: bne	s5,at,0x8010150c
  8010127c: lw	t9,460(sp)
  80101280: lui	v0,0x8014
  80101284: addu	v0,v0,t9
  80101288: lb	v0,16211(v0)
  8010128c: lui	t7,0x8014
  80101290: lui	t8,0x8015
  80101294: addu	t7,t7,v0
  80101298: lbu	t7,16408(t7)
  8010129c: lh	t8,10036(t8)
  801012a0: move	a0,s7
  801012a4: move	a1,zero
  801012a8: bne	t8,t7,0x801014cc
  801012ac: sll	t6,v0,0x4
  801012b0: subu	t6,t6,v0
  801012b4: sll	t6,t6,0x3
  801012b8: subu	t6,t6,v0
  801012bc: sll	t6,t6,0x3
  801012c0: lui	at,0x8015
  801012c4: addu	at,at,t6
  801012c8: lwc1	$f20,10504(at)
  801012cc: move	a0,s7
  801012d0: move	a1,zero
  801012d4: li	a2,1
  801012d8: jal	0x80007270   <osRecvMesg>
  801012dc: addiu	s6,sp,472
  801012e0: jal	0x800b4200   <slot_state_setup>
  801012e4: li	s2,3
  801012e8: move	s0,v0
  801012ec: move	a0,s7
  801012f0: move	a1,zero
  801012f4: jal	0x800075e0   <osSendMesg>
  801012f8: move	a2,zero
  801012fc: jal	0x800b74a0   <dispatch_handler>
  80101300: li	a0,1
  80101304: div.s	$f4,$f20,$f22
  80101308: li	a2,1
  8010130c: lui	a1,0x8012
  80101310: addiu	a1,a1,804
  80101314: addiu	a0,sp,472
  80101318: lui	at,0x4f00
  8010131c: cfc1	t9,c1_fcsr
  80101320: ctc1	a2,c1_fcsr
  80101324: nop
  80101328: cvt.w.s	$f6,$f4
  8010132c: cfc1	a2,c1_fcsr
  80101330: nop
  80101334: andi	a2,a2,0x78
  80101338: beqzl	a2,0x80101388
  8010133c: mfc1	a2,$f6
  80101340: mtc1	at,$f6
  80101344: li	a2,1
  80101348: sub.s	$f6,$f4,$f6
  8010134c: ctc1	a2,c1_fcsr
  80101350: nop
  80101354: cvt.w.s	$f6,$f6
  80101358: cfc1	a2,c1_fcsr
  8010135c: nop
  80101360: andi	a2,a2,0x78
  80101364: bnez	a2,0x8010137c
  80101368: nop
  8010136c: mfc1	a2,$f6
  80101370: lui	at,0x8000
  80101374: b	0x80101394
  80101378: or	a2,a2,at
  8010137c: b	0x80101394
  80101380: li	a2,-1
  80101384: mfc1	a2,$f6
  80101388: nop
  8010138c: bltz	a2,0x8010137c
  80101390: nop
  80101394: ctc1	t9,c1_fcsr
  80101398: li	a3,1
  8010139c: lui	at,0x4f00
  801013a0: cfc1	t8,c1_fcsr
  801013a4: ctc1	a3,c1_fcsr
  801013a8: nop
  801013ac: cvt.w.s	$f8,$f20
  801013b0: cfc1	a3,c1_fcsr
  801013b4: nop
  801013b8: andi	a3,a3,0x78
  801013bc: beqzl	a3,0x8010140c
  801013c0: mfc1	a3,$f8
  801013c4: mtc1	at,$f8
  801013c8: li	a3,1
  801013cc: sub.s	$f8,$f20,$f8
  801013d0: ctc1	a3,c1_fcsr
  801013d4: nop
  801013d8: cvt.w.s	$f8,$f8
  801013dc: cfc1	a3,c1_fcsr
  801013e0: nop
  801013e4: andi	a3,a3,0x78
  801013e8: bnez	a3,0x80101400
  801013ec: nop
  801013f0: mfc1	a3,$f8
  801013f4: lui	at,0x8000
  801013f8: b	0x80101418
  801013fc: or	a3,a3,at
  80101400: b	0x80101418
  80101404: li	a3,-1
  80101408: mfc1	a3,$f8
  8010140c: nop
  80101410: bltz	a3,0x80101400
  80101414: nop
  80101418: li	at,60
  8010141c: divu	zero,a3,at
  80101420: ctc1	t8,c1_fcsr
  80101424: li	t9,1
  80101428: mfhi	a3
  8010142c: mul.s	$f10,$f20,$f24
  80101430: lui	at,0x4f00
  80101434: cfc1	t6,c1_fcsr
  80101438: ctc1	t9,c1_fcsr
  8010143c: nop
  80101440: cvt.w.s	$f4,$f10
  80101444: cfc1	t9,c1_fcsr
  80101448: nop
  8010144c: andi	t9,t9,0x78
  80101450: beqzl	t9,0x801014a0
  80101454: mfc1	t9,$f4
  80101458: mtc1	at,$f4
  8010145c: li	t9,1
  80101460: sub.s	$f4,$f10,$f4
  80101464: ctc1	t9,c1_fcsr
  80101468: nop
  8010146c: cvt.w.s	$f4,$f4
  80101470: cfc1	t9,c1_fcsr
  80101474: nop
  80101478: andi	t9,t9,0x78
  8010147c: bnez	t9,0x80101494
  80101480: nop
  80101484: mfc1	t9,$f4
  80101488: lui	at,0x8000
  8010148c: b	0x801014ac
  80101490: or	t9,t9,at
  80101494: b	0x801014ac
  80101498: li	t9,-1
  8010149c: mfc1	t9,$f4
  801014a0: nop
  801014a4: bltz	t9,0x80101494
  801014a8: nop
  801014ac: li	at,1000
  801014b0: divu	zero,t9,at
  801014b4: mfhi	t8
  801014b8: ctc1	t6,c1_fcsr
  801014bc: jal	0x800b4360   <fcvt_wrapper>
  801014c0: sw	t8,16(sp)
  801014c4: b	0x80101504
  801014c8: nop
  801014cc: jal	0x80007270   <osRecvMesg>
  801014d0: li	a2,1
  801014d4: jal	0x800b4200   <slot_state_setup>
  801014d8: li	s2,11
  801014dc: move	s0,v0
  801014e0: move	a0,s7
  801014e4: move	a1,zero
  801014e8: jal	0x800075e0   <osSendMesg>
  801014ec: move	a2,zero
  801014f0: jal	0x800b74a0   <dispatch_handler>
  801014f4: li	a0,1
  801014f8: lui	t7,0x8018
  801014fc: lw	t7,-23324(t7)
  80101500: lw	s6,860(t7)
  80101504: b	0x80101840
  80101508: lw	v0,464(sp)
  8010150c: li	at,2
  80101510: bne	s5,at,0x8010178c
  80101514: lw	t6,460(sp)
  80101518: lui	v0,0x8014
  8010151c: addu	v0,v0,t6
  80101520: lb	v0,16211(v0)
  80101524: lui	t9,0x8014
  80101528: lui	at,0x8014
  8010152c: addu	t9,t9,v0
  80101530: lbu	t9,16408(t9)
  80101534: move	a0,s7
  80101538: move	a1,zero
  8010153c: blez	t9,0x8010174c
  80101540: sll	t8,v0,0x2
  80101544: addu	at,at,t8
  80101548: lwc1	$f20,19880(at)
  8010154c: move	a0,s7
  80101550: move	a1,zero
  80101554: li	a2,1
  80101558: jal	0x80007270   <osRecvMesg>
  8010155c: addiu	s6,sp,472
  80101560: jal	0x800b4200   <slot_state_setup>
  80101564: li	s2,3
  80101568: move	s0,v0
  8010156c: move	a0,s7
  80101570: move	a1,zero
  80101574: jal	0x800075e0   <osSendMesg>
  80101578: move	a2,zero
  8010157c: jal	0x800b74a0   <dispatch_handler>
  80101580: li	a0,1
  80101584: div.s	$f6,$f20,$f22
  80101588: li	a2,1
  8010158c: lui	a1,0x8012
  80101590: addiu	a1,a1,820
  80101594: addiu	a0,sp,472
  80101598: lui	at,0x4f00
  8010159c: cfc1	t7,c1_fcsr
  801015a0: ctc1	a2,c1_fcsr
  801015a4: nop
  801015a8: cvt.w.s	$f8,$f6
  801015ac: cfc1	a2,c1_fcsr
  801015b0: nop
  801015b4: andi	a2,a2,0x78
  801015b8: beqzl	a2,0x80101608
  801015bc: mfc1	a2,$f8
  801015c0: mtc1	at,$f8
  801015c4: li	a2,1
  801015c8: sub.s	$f8,$f6,$f8
  801015cc: ctc1	a2,c1_fcsr
  801015d0: nop
  801015d4: cvt.w.s	$f8,$f8
  801015d8: cfc1	a2,c1_fcsr
  801015dc: nop
  801015e0: andi	a2,a2,0x78
  801015e4: bnez	a2,0x801015fc
  801015e8: nop
  801015ec: mfc1	a2,$f8
  801015f0: lui	at,0x8000
  801015f4: b	0x80101614
  801015f8: or	a2,a2,at
  801015fc: b	0x80101614
  80101600: li	a2,-1
  80101604: mfc1	a2,$f8
  80101608: nop
  8010160c: bltz	a2,0x801015fc
  80101610: nop
  80101614: ctc1	t7,c1_fcsr
  80101618: li	a3,1
  8010161c: lui	at,0x4f00
  80101620: cfc1	t6,c1_fcsr
  80101624: ctc1	a3,c1_fcsr
  80101628: nop
  8010162c: cvt.w.s	$f10,$f20
  80101630: cfc1	a3,c1_fcsr
  80101634: nop
  80101638: andi	a3,a3,0x78
  8010163c: beqzl	a3,0x8010168c
  80101640: mfc1	a3,$f10
  80101644: mtc1	at,$f10
  80101648: li	a3,1
  8010164c: sub.s	$f10,$f20,$f10
  80101650: ctc1	a3,c1_fcsr
  80101654: nop
  80101658: cvt.w.s	$f10,$f10
  8010165c: cfc1	a3,c1_fcsr
  80101660: nop
  80101664: andi	a3,a3,0x78
  80101668: bnez	a3,0x80101680
  8010166c: nop
  80101670: mfc1	a3,$f10
  80101674: lui	at,0x8000
  80101678: b	0x80101698
  8010167c: or	a3,a3,at
  80101680: b	0x80101698
  80101684: li	a3,-1
  80101688: mfc1	a3,$f10
  8010168c: nop
  80101690: bltz	a3,0x80101680
  80101694: nop
  80101698: li	at,60
  8010169c: divu	zero,a3,at
  801016a0: ctc1	t6,c1_fcsr
  801016a4: li	t7,1
  801016a8: mfhi	a3
  801016ac: mul.s	$f4,$f20,$f24
  801016b0: lui	at,0x4f00
  801016b4: cfc1	t8,c1_fcsr
  801016b8: ctc1	t7,c1_fcsr
  801016bc: nop
  801016c0: cvt.w.s	$f6,$f4
  801016c4: cfc1	t7,c1_fcsr
  801016c8: nop
  801016cc: andi	t7,t7,0x78
  801016d0: beqzl	t7,0x80101720
  801016d4: mfc1	t7,$f6
  801016d8: mtc1	at,$f6
  801016dc: li	t7,1
  801016e0: sub.s	$f6,$f4,$f6
  801016e4: ctc1	t7,c1_fcsr
  801016e8: nop
  801016ec: cvt.w.s	$f6,$f6
  801016f0: cfc1	t7,c1_fcsr
  801016f4: nop
  801016f8: andi	t7,t7,0x78
  801016fc: bnez	t7,0x80101714
  80101700: nop
  80101704: mfc1	t7,$f6
  80101708: lui	at,0x8000
  8010170c: b	0x8010172c
  80101710: or	t7,t7,at
  80101714: b	0x8010172c
  80101718: li	t7,-1
  8010171c: mfc1	t7,$f6
  80101720: nop
  80101724: bltz	t7,0x80101714
  80101728: nop
  8010172c: li	at,1000
  80101730: divu	zero,t7,at
  80101734: mfhi	t6
  80101738: ctc1	t8,c1_fcsr
  8010173c: jal	0x800b4360   <fcvt_wrapper>
  80101740: sw	t6,16(sp)
  80101744: b	0x80101784
  80101748: nop
  8010174c: jal	0x80007270   <osRecvMesg>
  80101750: li	a2,1
  80101754: jal	0x800b4200   <slot_state_setup>
  80101758: li	s2,11
  8010175c: move	s0,v0
  80101760: move	a0,s7
  80101764: move	a1,zero
  80101768: jal	0x800075e0   <osSendMesg>
  8010176c: move	a2,zero
  80101770: jal	0x800b74a0   <dispatch_handler>
  80101774: li	a0,1
  80101778: lui	t9,0x8018
  8010177c: lw	t9,-23324(t9)
  80101780: lw	s6,860(t9)
  80101784: b	0x80101840
  80101788: lw	v0,464(sp)
  8010178c: li	at,3
  80101790: bne	s5,at,0x80101840
  80101794: move	a0,s7
  80101798: lw	t8,460(sp)
  8010179c: lui	t7,0x8014
  801017a0: addiu	t7,t7,16212
  801017a4: move	a1,zero
  801017a8: li	a2,1
  801017ac: jal	0x80007270   <osRecvMesg>
  801017b0: addu	s4,t8,t7
  801017b4: jal	0x800b4200   <slot_state_setup>
  801017b8: li	s2,11
  801017bc: move	s0,v0
  801017c0: move	a0,s7
  801017c4: move	a1,zero
  801017c8: jal	0x800075e0   <osSendMesg>
  801017cc: move	a2,zero
  801017d0: jal	0x800b74a0   <dispatch_handler>
  801017d4: li	a0,1
  801017d8: lb	t9,-1(s4)
  801017dc: lui	t7,0x8015
  801017e0: lui	t6,0x8018
  801017e4: sll	t8,t9,0x8
  801017e8: addu	t8,t8,t9
  801017ec: sll	t8,t8,0x3
  801017f0: addu	t7,t7,t8
  801017f4: lh	t7,-21994(t7)
  801017f8: lui	t8,0x8015
  801017fc: lw	t6,-23312(t6)
  80101800: sll	t9,t7,0x4
  80101804: subu	t9,t9,t7
  80101808: sll	t9,t9,0x3
  8010180c: subu	t9,t9,t7
  80101810: sll	t9,t9,0x3
  80101814: addu	t8,t8,t9
  80101818: lb	t8,10502(t8)
  8010181c: lw	v0,464(sp)
  80101820: sll	t7,t8,0x2
  80101824: lui	t8,0x8018
  80101828: lw	t8,-23316(t8)
  8010182c: addu	t9,t6,t7
  80101830: lhu	t6,108(t8)
  80101834: sll	t7,t6,0x2
  80101838: addu	t8,t9,t7
  8010183c: lw	s6,0(t8)
  80101840: beqzl	s5,0x801018b4
  80101844: addiu	s5,s5,1
  80101848: li	v0,60
  8010184c: multu	s5,v0
  80101850: lw	t9,464(sp)
  80101854: lw	a1,460(sp)
  80101858: move	a2,s6
  8010185c: mflo	t6
  80101868: multu	t9,v0
  8010186c: mflo	t7
  80101870: bgez	t7,0x80101880
  80101874: sra	t8,t7,0x1
  80101878: addiu	at,t7,1
  8010187c: sra	t8,at,0x1
  80101880: subu	a0,t6,t8
  80101884: sll	t6,a1,0x3
  80101888: subu	t6,t6,a1
  8010188c: sll	t6,t6,0x1
  80101890: addiu	a1,t6,43
  80101894: addiu	a0,a0,190
  80101898: sll	t9,a0,0x10
  8010189c: sll	t8,a1,0x10
  801018a0: sra	a1,t8,0x10
  801018a4: jal	0x800b71d4   <state_utility>
  801018a8: sra	a0,t9,0x10
  801018ac: lw	v0,464(sp)
  801018b0: addiu	s5,s5,1
  801018b4: bnel	s5,v0,0x801010a0
  801018b8: lw	t9,460(sp)
  801018bc: lw	t7,460(sp)
  801018c0: lw	t8,468(sp)
  801018c4: addiu	t6,t7,1
  801018c8: bne	t6,t8,0x80101094
  801018cc: sw	t6,460(sp)
  801018d0: sw	s6,512(sp)
  801018d4: lui	at,0xbf80
  801018d8: mtc1	at,$f12
  801018dc: jal	0x800b65b4   <render_helper>
  801018e0: nop
  801018e4: lw	ra,28(sp)
  801018e8: lui	v0,0x8012
  801018ec: addiu	v0,v0,-29152
  801018f0: li	t9,3
  801018f4: sw	t9,4(v0)
  801018f8: sw	zero,0(v0)
  801018fc: jr	ra
  80101900: addiu	sp,sp,528
