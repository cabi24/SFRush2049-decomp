	.text
	.globl	func_803A9EEC
	.ent	func_803A9EEC 2
func_803A9EEC:
	.option	O3
	subu	$sp, 280
	sw	$31, 116($sp)
	sw	$30, 112($sp)
	sw	$23, 108($sp)
	sw	$22, 104($sp)
	sw	$21, 100($sp)
	sw	$20, 96($sp)
	sw	$19, 92($sp)
	sw	$18, 88($sp)
	sw	$17, 84($sp)
	sw	$16, 80($sp)
	s.d	$f30, 72($sp)
	s.d	$f28, 64($sp)
	s.d	$f26, 56($sp)
	s.d	$f24, 48($sp)
	s.d	$f22, 40($sp)
	s.d	$f20, 32($sp)
	.mask	0xC0FF0000, -164
	.fmask	0xFFF00000, -208
	.frame	$sp, 280, $31
	.loc	2 558
	sw	$4, 280($sp)
	.loc	2 558
	.loc	2 560
 # 559	    s32 playerIdx;
 # 560	    render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 561
 # 561	    for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++) {
	move	$18, $0
	lh	$3, D_8014A108
	ble	$3, 0, $313
	sw	$0, 132($sp)
	la	$14, D_803BACE0
	sw	$14, 128($sp)
$289:
	.loc	2 561
	.loc	2 562
 # 562	        switch (D_803BACE0[playerIdx]) {
	lw	$15, 128($sp)
	lw	$24, 0($15)
	bgeu	$24, 7, $311
	sll	$24, $24, 2
	lw	$24, $290($24)
	j	$24
$292:
	.loc	2 563
 # 563	        case 0: func_803A7B58(playerIdx); break;
	sw	$18, 276($sp)
	.livereg	0x0000200E,0x00000000
	jal	func_803A7B58
	lw	$18, 276($sp)
	.loc	2 563
	lh	$3, D_8014A108
	addu	$16, $18, 1
	b	$312
$293:
	.loc	2 564
 # 564	        case 1: func_803A87D4(playerIdx); break;
	move	$19, $18
	sw	$18, 276($sp)
	.livereg	0x0000100E,0x00000000
	jal	func_803A87D4
	lw	$18, 276($sp)
	.loc	2 564
	lh	$3, D_8014A108
	addu	$16, $18, 1
	b	$312
$294:
	.loc	2 565
 # 565	        case 2: func_803A8B6C(playerIdx); break;
	.loc	2 301
 # 301	void func_803A8B6C(s32 p) { crowd_cheer_play(D_803BAC08[p], -20, 0, -10, 10); }
	.loc	2 301
	lw	$25, 132($sp)
	lw	$4, D_803BAC08($25)
	li	$5, -20
	move	$6, $0
	li	$7, -10
	li	$14, 10
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 301
	.loc	2 565
 # 565	        case 2: func_803A8B6C(playerIdx); break;
	lh	$3, D_8014A108
	addu	$16, $18, 1
	b	$312
$295:
	.loc	2 566
 # 566	        case 3: func_803A8B74(playerIdx); break;
	sw	$18, 0($sp)
	sw	$18, 276($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_803A8B74
	lw	$18, 276($sp)
	.loc	2 566
	lh	$3, D_8014A108
	addu	$16, $18, 1
	b	$312
$296:
	.loc	2 567
 # 567	        case 4: func_803A9088(playerIdx); break;
	sw	$18, 276($sp)
	.livereg	0x0000200E,0x00000000
	jal	func_803A9088
	lw	$18, 276($sp)
	.loc	2 567
	lh	$3, D_8014A108
	addu	$16, $18, 1
	b	$312
$297:
	.loc	2 568
 # 568	        case 5: func_803A9EDC(playerIdx); break;
	.loc	2 507
 # 507	{
	.loc	2 508
 # 508	    s32 y = D_803B5D50[D_8014A108 - 1][p].y;
	.noalias	$2,$sp
	mul	$15, $3, 96
	mul	$24, $18, 24
	addu	$25, $15, $24
	la	$14, D_803B5D50
	addu	$2, $25, $14
	lw	$17, -92($2)
	.loc	2 509
 # 509	    s32 cx = D_803B5D50[D_8014A108 - 1][p].x;
	lw	$20, -80($2)
	.loc	2 512
 # 510	    u8 *str;
 # 511	    u8 str1[44];
 # 512	    if (D_8014A108 > 2) object_create(11);
	lw	$15, 132($sp)
	la	$24, D_801146AC
	addu	$19, $15, $24
	blt	$3, 3, $298
	.alias	$2,$sp
	.loc	2 512
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$300
$298:
	.loc	2 513
 # 513	    else if (D_8014A108 == 2) object_create(10);
	bne	$3, 2, $299
	.loc	2 513
	li	$4, 10
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$300
$299:
	.loc	2 514
 # 514	    else object_create(13);
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
$300:
	.loc	2 515
 # 515	    fcvt_wrapper(str1, "%s %d", countdown_state.text[29], p + 1);
	addu	$4, $sp, 208
	la	$5, $$76
	lw	$25, countdown_state+4
	lw	$6, 116($25)
	addu	$16, $18, 1
	move	$7, $16
	.livereg	0x0F00800E,0x00000000
	jal	fcvt_wrapper
	.loc	2 516
 # 516	    dispatch_handler(D_801146AC[p]);
	.noalias	$19,$sp
	lw	$4, 0($19)
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.alias	$19,$sp
	.loc	2 517
 # 517	    state_utility(sound_pitch_diff_halved(str1, cx), y, str1);
	addu	$4, $sp, 208
	sll	$18, $20, 16
	sra	$14, $18, 16
	move	$18, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	.livereg	0x0C00200E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	addu	$6, $sp, 208
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 518
 # 518	    y += object_bytes_sum_global() * 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$14, $2, 2
	addu	$17, $17, $14
	.loc	2 519
 # 519	    if (D_8014A108 > 2) object_create(12);
	lh	$3, D_8014A108
	blt	$3, 3, $301
	.loc	2 519
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$303
$301:
	.loc	2 520
 # 520	    else if (D_8014A108 == 2) object_create(11);
	bne	$3, 2, $302
	.loc	2 520
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$303
$302:
	.loc	2 521
 # 521	    else object_create(10);
	li	$4, 10
	.livereg	0x0800000E,0x00000000
	jal	object_create
$303:
	.loc	2 522
 # 522	    str = (u8 *)"PAK FULL";
	.loc	2 523
 # 523	    dispatch_handler(17);
	li	$4, 17
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 524
 # 524	    state_utility(sound_pitch_diff_halved(str, cx), y, str);
	la	$4, $$77
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	la	$6, $$77
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 525
 # 525	}
	.loc	2 568
 # 568	        case 5: func_803A9EDC(playerIdx); break;
	lh	$3, D_8014A108
	b	$312
$304:
	.loc	2 569
 # 569	        case 6: func_803A9EE4(playerIdx); break;
	.loc	2 528
 # 528	{
	.loc	2 529
 # 529	    s32 y = D_803B5D50[D_8014A108 - 1][p].y;
	.noalias	$2,$sp
	mul	$14, $3, 96
	mul	$15, $18, 24
	addu	$24, $14, $15
	la	$25, D_803B5D50
	addu	$2, $24, $25
	lw	$17, -92($2)
	.loc	2 530
 # 530	    s32 cx = D_803B5D50[D_8014A108 - 1][p].x;
	lw	$20, -80($2)
	.loc	2 533
 # 531	    u8 *str;
 # 532	    u8 str1[44];
 # 533	    if (D_8014A108 > 2) object_create(11);
	lw	$14, 132($sp)
	la	$15, D_801146AC
	addu	$19, $14, $15
	blt	$3, 3, $305
	.alias	$2,$sp
	.loc	2 533
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$307
$305:
	.loc	2 534
 # 534	    else if (D_8014A108 == 2) object_create(10);
	bne	$3, 2, $306
	.loc	2 534
	li	$4, 10
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$307
$306:
	.loc	2 535
 # 535	    else object_create(13);
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
$307:
	.loc	2 536
 # 536	    fcvt_wrapper(str1, "%s %d", countdown_state.text[29], p + 1);
	addu	$4, $sp, 144
	la	$5, $$80
	lw	$24, countdown_state+4
	lw	$6, 116($24)
	addu	$16, $18, 1
	move	$7, $16
	.livereg	0x0F00800E,0x00000000
	jal	fcvt_wrapper
	.loc	2 537
 # 537	    dispatch_handler(D_801146AC[p]);
	.noalias	$19,$sp
	lw	$4, 0($19)
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.alias	$19,$sp
	.loc	2 538
 # 538	    state_utility(sound_pitch_diff_halved(str1, cx), y, str1);
	addu	$4, $sp, 144
	sll	$18, $20, 16
	sra	$25, $18, 16
	move	$18, $25
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	.livereg	0x0C00200E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $17, 16
	sra	$24, $5, 16
	move	$5, $24
	addu	$6, $sp, 144
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 539
 # 539	    y += object_bytes_sum_global() * 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$25, $2, 2
	addu	$17, $17, $25
	.loc	2 540
 # 540	    if (D_8014A108 > 2) object_create(12);
	lh	$3, D_8014A108
	blt	$3, 3, $308
	.loc	2 540
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$310
$308:
	.loc	2 541
 # 541	    else if (D_8014A108 == 2) object_create(11);
	bne	$3, 2, $309
	.loc	2 541
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$310
$309:
	.loc	2 542
 # 542	    else object_create(10);
	li	$4, 10
	.livereg	0x0800000E,0x00000000
	jal	object_create
$310:
	.loc	2 543
 # 543	    str = (u8 *)"PAK DAMAGED";
	.loc	2 544
 # 544	    dispatch_handler(17);
	li	$4, 17
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 545
 # 545	    state_utility(sound_pitch_diff_halved(str, cx), y, str);
	la	$4, $$81
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $17, 16
	sra	$24, $5, 16
	move	$5, $24
	la	$6, $$81
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 546
 # 546	}
	.loc	2 569
 # 569	        case 6: func_803A9EE4(playerIdx); break;
	lh	$3, D_8014A108
	b	$312
	.rdata	
$290:
	.word	$292
	.word	$293
	.word	$294
	.word	$295
	.word	$296
	.word	$297
	.word	$304
	.text	
$311:
	.loc	2 570
 # 570	        default: break;
	addu	$16, $18, 1
$312:
	.loc	2 561
 # 561	    for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++) {
	move	$18, $16
	lw	$25, 132($sp)
	addu	$14, $25, 4
	sw	$14, 132($sp)
	lw	$15, 128($sp)
	addu	$24, $15, 4
	sw	$24, 128($sp)
	blt	$18, $3, $289
$313:
	.loc	2 574
 # 574	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 575
 # 575	    return 1;
	li	$2, 1
	.livereg	0x2000FF0E,0x00000FFF
	l.d	$f20, 32($sp)
	l.d	$f22, 40($sp)
	l.d	$f24, 48($sp)
	l.d	$f26, 56($sp)
	l.d	$f28, 64($sp)
	l.d	$f30, 72($sp)
	lw	$16, 80($sp)
	lw	$17, 84($sp)
	lw	$18, 88($sp)
	lw	$19, 92($sp)
	lw	$20, 96($sp)
	lw	$21, 100($sp)
	lw	$22, 104($sp)
	lw	$23, 108($sp)
	lw	$31, 116($sp)
	lw	$30, 112($sp)
	addu	$sp, 280
	j	$31
	.end	func_803A9EEC
