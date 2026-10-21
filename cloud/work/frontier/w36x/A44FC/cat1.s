	.text
	.globl	func_803AD57C
	.ent	func_803AD57C 2
func_803AD57C:
	.option	O3
	subu	$sp, 312
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
	.mask	0xC0FF0000, -196
	.fmask	0xFFF00000, -240
	.frame	$sp, 312, $31
	.loc	2 775
	sw	$4, 312($sp)
	.loc	2 775
	.loc	2 776
 # 776	    render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 777
 # 777	    switch(D_803BA804) {
	lw	$2, D_803BA804
	beq	$2, 0, $100277
	beq	$2, 1, $100278
	b	$100316
$100277:
	.loc	2 778
 # 778	    case 0:func_803AA3D8();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AA3D8
	.loc	2 778
	b	$100316
$100278:
	.loc	2 779
 # 779	    case 1:recordscr_drawView();break;
	.loc	2 698
 # 698	{
	.loc	2 703
 # 699	    s32 i, x, y, cx;
 # 700	    u8 *str;
 # 701	    u8 str1[64];
 # 702	    s16 screenPos[2];
 # 703	        func_800B669C(1,1);
	li	$4, 1
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 704
 # 704	        brake_light_update(0,D_803B5F28[17].position,&D_80150B70[0],0,screenPos);
	move	$4, $0
	la	$5, D_803B5F28+1136
	la	$6, D_80150B70
	move	$7, $0
	addu	$14, $sp, 216
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 705
 # 705	        cx=screenPos[0];y=screenPos[1];
	lh	$22, 216($sp)
	.loc	2 705
	lh	$19, 218($sp)
	.loc	2 706
 # 706	        func_800F6928(D_803B5F28[17].position[2]-10.0f);
	l.s	$f4, D_803B5F28+1144
	li.s	$f6, 10.0
	sub.s	$f12, $f4, $f6
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 707
 # 707	        dispatch_handler(1);object_create(13);dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 707
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 707
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 708
 # 708	        str=!D_803BA800 ? countdown_state.text[176] : (*D_803BA800)->name;music_fade(cx,y,140,str);
	lw	$2, D_803BA800
	bne	$2, 0, $100279
	la	$21, countdown_state
	.noalias	$21,$sp
	lw	$15, 4($21)
	lw	$7, 704($15)
	b	$100280
	.alias	$21,$sp
$100279:
	lw	$7, 0($2)
	addu	$7, $7, 20
	la	$21, countdown_state
$100280:
	.loc	2 708
	sll	$4, $22, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $19, 16
	sra	$25, $5, 16
	move	$5, $25
	li	$6, 140
	.livereg	0x0F00000E,0x00000000
	jal	music_fade
	.loc	2 709
 # 709	        func_800B669C(0,1);object_bytes_sum_global();object_create(11);
	move	$4, $0
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 709
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	.loc	2 709
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 710
 # 710	        for(i=0;i<2;i++) {
	move	$20, $0
	la	$18, D_803B5F28
	addu	$23, $sp, 220
$100281:
	.loc	2 710
	.loc	2 711
 # 711	            if(D_803B5F28[i+11].angle>0.6981316804885864f && D_803B5F28[i+11].angle<2.4434609413146973f)continue;
	.noalias	$18,$sp
	l.s	$f0, 712($18)
	li.s	$f8, 0.6981316804885864
	c.lt.s	$f8, $f0
	bc1f	$100282
	li.s	$f10, 2.4434609413146973
	c.lt.s	$f0, $f10
	bc1t	$100294
	.loc	2 711
$100282:
	.loc	2 712
 # 712	            if(!D_803B5F28[i+11].alpha)continue;
	lbu	$2, 764($18)
	beq	$2, 0, $100294
	.loc	2 712
	.loc	2 713
 # 713	            func_800ED66C((f32)(u32)D_803B5F28[i+11].alpha);
	mtc1	$2, $f4
	cvt.s.w	$f12, $f4
	bge	$2, $0, $100317
	li.s	$f6, 4294967296.0
	add.s	$f12, $f12, $f6
$100317:
	addu	$16, $18, 752
	mul	$14, $20, 4
	sw	$14, 124($sp)
	.livereg	0x0000800E,0x00080000
	jal	func_800ED66C
	.loc	2 714
 # 714	            if(D_803B5F28[i+11].angle<1.5707963705062866f) {
	l.s	$f8, 712($18)
	li.s	$f10, 1.5707963705062866
	c.lt.s	$f8, $f10
	bc1f	$100283
	.loc	2 714
	.loc	2 716
 # 715	                Color4 c1,c2;
 # 716	                func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(1);
	lw	$15, D_801146BC
	sw	$15, 0($sp)
	lw	$24, D_801146C0
	sw	$24, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 716
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$100284
$100283:
	.loc	2 717
 # 717	            } else {
	.loc	2 718
 # 718	                func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(22);
	lw	$25, D_801146BC
	sw	$25, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 718
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$100284:
	.loc	2 720
 # 719	            }
 # 720	            func_800F6928(D_803B5F28[i+11].position[2]-10.0f);
	l.s	$f4, 760($18)
	li.s	$f6, 10.0
	sub.s	$f12, $f4, $f6
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 721
 # 721	            brake_light_update(0,D_803B5F28[i+11].position,&D_80150B70[0],0,screenPos);
	move	$4, $0
	move	$5, $16
	la	$6, D_80150B70
	move	$7, $0
	addu	$15, $sp, 216
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 722
 # 722	            x=screenPos[0]-35.0f*D_803B64E8*300.0f/D_803B64FC;y=screenPos[1];
	li.s	$f8, 35.0
	l.s	$f10, D_803B64E8
	mul.s	$f4, $f8, $f10
	li.s	$f6, 300.0
	mul.s	$f8, $f4, $f6
	l.s	$f10, D_803B64FC
	div.s	$f4, $f8, $f10
	lh	$24, 216($sp)
	mtc1	$24, $f6
	cvt.s.w	$f8, $f6
	sub.s	$f10, $f8, $f4
	trunc.w.s	$f6, $f10, $25
	mfc1	$16, $f6
	.loc	2 722
	lh	$19, 218($sp)
	.loc	2 723
 # 723	            brake_light_update(0,D_803B5F28[15].position,&D_80150B70[0],0,screenPos);
	move	$4, $0
	la	$5, D_803B5F28+1008
	la	$6, D_80150B70
	move	$7, $0
	addu	$14, $sp, 216
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 724
 # 724	            cx=screenPos[0]-55.0f*D_803B64E8*300.0f/D_803B64FC;
	li.s	$f8, 55.0
	l.s	$f4, D_803B64E8
	mul.s	$f10, $f8, $f4
	li.s	$f6, 300.0
	mul.s	$f8, $f10, $f6
	l.s	$f4, D_803B64FC
	div.s	$f10, $f8, $f4
	lh	$15, 216($sp)
	mtc1	$15, $f6
	cvt.s.w	$f8, $f6
	sub.s	$f4, $f8, $f10
	trunc.w.s	$f6, $f4, $24
	mfc1	$22, $f6
	.loc	2 725
 # 725	            str=NAME(51,i);state_utility(x,y,str);
	.noalias	$21,$18
	.noalias	$21,$sp
	lw	$25, 16($21)
	lw	$14, 12($21)
	lhu	$15, 102($14)
	mul	$24, $15, 4
	addu	$14, $25, $24
	lw	$15, 124($sp)
	addu	$25, $14, $15
	lw	$7, 0($25)
	.loc	2 725
	sll	$4, $16, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$17, $19, 16
	sra	$14, $17, 16
	move	$17, $14
	sll	$5, $17, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $7
	.livereg	0x0E00400E,0x00000000
	jal	state_utility
	.loc	2 726
 # 726	            x=cx;
	.loc	2 727
 # 727	            if(i==0) {
	bne	$20, 0, $100293
	.loc	2 727
	.loc	2 728
 # 728	                if(D_803BA880==4) {
	lw	$2, D_803BA880
	bne	$2, 4, $100285
	.loc	2 728
	.loc	2 729
 # 729	                    fcvt_wrapper(str1,D_803B8AD8,NAME(44,D_803BA860),countdown_state.text[42]);str=str1;
	move	$4, $23
	la	$5, D_803B8AD8
	lw	$25, 16($21)
	lw	$24, 12($21)
	lhu	$14, 88($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lb	$14, D_803BA860
	mul	$25, $14, 4
	addu	$15, $24, $25
	lw	$6, 0($15)
	lw	$14, 4($21)
	lw	$7, 168($14)
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 729
	move	$7, $23
	b	$100292
$100285:
	.loc	2 730
 # 730	                } else if(D_803BA880==5)str=countdown_state.text[176];
	bne	$2, 5, $100286
	.loc	2 730
	lw	$24, 4($21)
	lw	$7, 704($24)
	b	$100292
$100286:
	.loc	2 731
 # 731	                else if(D_803BA858<12 && (D_803BA858&1)) {
	lb	$2, D_803BA858
	bge	$2, 12, $100289
	and	$25, $2, 1
	beq	$25, 0, $100289
	.loc	2 731
	.loc	2 732
 # 732	                    fcvt_wrapper(str1,D_803B8AE0,D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2],countdown_state.text[6]);str=str1;
	blt	$2, 13, $100287
	addu	$3, $2, -6
	b	$100288
$100287:
	div	$3, $2, 2
$100288:
	move	$4, $23
	la	$5, D_803B8AE0
	mul	$15, $3, 4
	lw	$6, D_8011A8D8($15)
	lw	$14, 4($21)
	lw	$7, 24($14)
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 732
	move	$7, $23
	b	$100292
$100289:
	.loc	2 733
 # 733	                } else str=D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2];
	blt	$2, 13, $100290
	addu	$3, $2, -6
	b	$100291
$100290:
	div	$3, $2, 2
$100291:
	mul	$24, $3, 4
	lw	$7, D_8011A8D8($24)
$100292:
	.loc	2 734
 # 734	                state_utility(x,y,str);
	sll	$4, $22, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $17, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $7
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$100294
$100293:
	.loc	2 735
 # 735	            } else if(i==1){str=NAME(52,D_803BA818[D_803BA880]);state_utility(x,y,str);}
	bne	$20, 1, $100294
	.loc	2 735
	.loc	2 735
	lw	$14, 16($21)
	lw	$24, D_803BA880
	mul	$25, $24, 4
	lw	$15, D_803BA818($25)
	mul	$24, $15, 4
	addu	$25, $14, $24
	lw	$15, 12($21)
	lhu	$14, 104($15)
	mul	$24, $14, 4
	addu	$15, $25, $24
	lw	$7, 0($15)
	.loc	2 735
	sll	$4, $22, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $7
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 710
 # 710	        for(i=0;i<2;i++) {
$100294:
	addu	$20, $20, 1
	addu	$18, $18, 64
	bne	$20, 2, $100281
	.alias	$18,$21
	.alias	$18,$sp
	.loc	2 737
 # 737	        func_800B669C(0,3);func_800ED66C(-1.0f);render_helper(0.0f);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 737
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 737
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 738
 # 738	        switch(D_803BA818[D_803BA880]) {
	lw	$24, D_803BA880
	mul	$15, $24, 4
	lw	$14, D_803BA818($15)
	bgeu	$14, 13, $100315
	sll	$14, $14, 2
	lw	$14, $100295($14)
	j	$14
	.rdata	
$100295:
	.word	$100297
	.word	$100298
	.word	$100299
	.word	$100300
	.word	$100301
	.word	$100302
	.word	$100303
	.word	$100309
	.word	$100310
	.word	$100311
	.word	$100312
	.word	$100313
	.word	$100314
	.text	
	.alias	$21,$sp
$100297:
	.loc	2 739
 # 739	        case 0:func_803AD0D4();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AD0D4
	.loc	2 739
	b	$100315
$100298:
	.loc	2 740
 # 740	        case 1:func_803ACF5C(0);break;
	move	$20, $0
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 740
	b	$100315
$100299:
	.loc	2 741
 # 741	        case 2:func_803ACF5C(1);break;
	li	$20, 1
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 741
	b	$100315
$100300:
	.loc	2 742
 # 742	        case 3:func_803ACF5C(2);break;
	li	$20, 2
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 742
	b	$100315
$100301:
	.loc	2 743
 # 743	        case 4:func_803ACD5C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ACD5C
	.loc	2 743
	b	$100315
$100302:
	.loc	2 744
 # 744	        case 5:func_803ACA94();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ACA94
	.loc	2 744
	b	$100315
$100303:
	.loc	2 745
 # 745	        case 6:recordscr_drawStuntTotals();break;
	.loc	2 757
 # 757	{
	.loc	2 761
 # 758	            s32 statIdx, x, y;
 # 759	            u8 *stats;
 # 760	            u8 str[40];
 # 761	            if(D_803BA800)stats=PROFILE_DATA+12+D_803BA858*64;
	lw	$2, D_803BA800
	beq	$2, 0, $100304
	.loc	2 761
	lw	$25, 0($2)
	lw	$24, 44($25)
	lw	$15, 0($24)
	lb	$14, D_803BA858
	mul	$25, $14, 64
	addu	$17, $15, $25
	addu	$17, $17, 12
	b	$100305
$100304:
	.loc	2 762
 # 762	            else stats=D_80151410+D_803BA858*64-1280;
	lb	$24, D_803BA858
	mul	$14, $24, 64
	addu	$15, $14, -1280
	la	$25, D_80151410
	addu	$17, $15, $25
$100305:
	.loc	2 763
 # 763	            object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 763
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 764
 # 764	            for(statIdx=0;statIdx<10;statIdx++) {
	move	$2, $0
	move	$16, $0
	move	$19, $17
	la	$23, D_803B8B64
	la	$22, D_803B8B60
	lw	$20, 192($sp)
	lw	$18, 188($sp)
	addu	$17, $sp, 144
$100306:
	.loc	2 764
	.loc	2 765
 # 765	                if(statIdx==0) {x=110;y=90;}
	bne	$16, 0, $100307
	.loc	2 765
	.loc	2 765
	li	$20, 110
	.loc	2 765
	li	$18, 90
	b	$100308
$100307:
	.loc	2 766
 # 766	                else if(statIdx==5) {x=250;y=90;}
	bne	$16, 20, $100308
	.loc	2 766
	.loc	2 766
	li	$20, 250
	.loc	2 766
	li	$18, 90
$100308:
	.loc	2 767
 # 767	                fcvt_wrapper(str,D_803B8B60,NAME(37,statIdx));
	move	$4, $17
	move	$5, $22
	.noalias	$21,$sp
	lw	$24, 16($21)
	lw	$14, 12($21)
	lhu	$15, 74($14)
	mul	$25, $15, 4
	addu	$14, $24, $25
	addu	$15, $14, $16
	lw	$6, 0($15)
	addu	$24, $20, -4
	sll	$25, $24, 16
	sra	$14, $25, 16
	sw	$14, 132($sp)
	addu	$15, $20, 4
	sll	$24, $15, 16
	sra	$25, $24, 16
	sw	$25, 124($sp)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 768
 # 768	                state_utility(sound_pitch_diff_calc(str,x-4),y,str);
	move	$4, $17
	lh	$5, 134($sp)
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	sw	$5, 132($sp)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 769
 # 769	                fcvt_wrapper(str,D_803B8B64,U32_AT(stats,20+statIdx*4));
	move	$4, $17
	move	$5, $23
	lw	$6, 20($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 770
 # 770	                state_utility(x+4,y,str);y+=object_bytes_sum_global();
	lh	$4, 126($sp)
	lh	$5, 134($sp)
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 770
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 764
 # 764	            for(statIdx=0;statIdx<10;statIdx++) {
	addu	$16, $16, 4
	addu	$19, $19, 4
	bne	$16, 40, $100306
	.alias	$21,$sp
	sw	$20, 192($sp)
	sw	$18, 188($sp)
	.loc	2 772
 # 772	            crowd_cheer_play(D_803B6530,20,86,300,94+object_bytes_sum_global()*5);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 20
	li	$6, 86
	li	$7, 300
	mul	$24, $2, 5
	addu	$25, $24, 94
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 773
 # 773	            }
	.loc	2 745
 # 745	        case 6:recordscr_drawStuntTotals();break;
	b	$100315
$100309:
	.loc	2 746
 # 746	        case 7:func_803AC7F4();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AC7F4
	.loc	2 746
	b	$100315
$100310:
	.loc	2 747
 # 747	        case 8:func_803AC54C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AC54C
	.loc	2 747
	b	$100315
$100311:
	.loc	2 748
 # 748	        case 9:func_803ABEC8();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ABEC8
	.loc	2 748
	b	$100315
$100312:
	.loc	2 749
 # 749	        case 10:func_803AB848();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AB848
	.loc	2 749
	b	$100315
$100313:
	.loc	2 750
 # 750	        case 11:func_803AB5FC();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AB5FC
	.loc	2 750
	b	$100315
$100314:
	.loc	2 751
 # 751	        case 12:func_803AAF0C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AAF0C
	.loc	2 751
$100315:
	.loc	2 753
 # 752	        }
 # 753	        func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 754
 # 754	}
	.loc	2 779
 # 779	    case 1:recordscr_drawView();break;
$100316:
	.loc	2 781
 # 780	    }
 # 781	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 782
 # 782	    return 1;
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
	addu	$sp, 312
	j	$31
	.end	func_803AD57C

	.text
	.globl	func_803A44FC
	.ent	func_803A44FC 2
func_803A44FC:
	.option	O3
	subu	$sp, 312
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
	.mask	0xC0FF0000, -196
	.fmask	0xFFF00000, -240
	.frame	$sp, 312, $31
	.loc	2 84
	sw	$4, 312($sp)
	.loc	2 84
	.loc	2 86
 #  85	    s32 i;
 #  86	    for (i = 0; i < D_8014A108; i++) {
	move	$19, $0
	lh	$3, D_8014A108
	ble	$3, 0, $200088
	sw	$0, 152($sp)
	la	$2, D_803B9B90
	li.s	$f30, 1.5707964
	li.s	$f28, 2.443461
	li.s	$f26, 0.6981317
	li.s	$f24, 5.0E-1
	la	$21, countdown_state
$200032:
	.loc	2 86
	.loc	2 87
 #  87	        if (D_803B9B90[i] == 1) animButtonText(i);
	.noalias	$2,$sp
	lw	$14, 0($2)
	bne	$14, 1, $200087
	.loc	2 87
	sw	$19, 304($sp)
	.loc	2 94
 #  94	{
	.loc	2 102
 # 102	    f32 gTextPosScale = D_803B25AC;
	l.s	$f22, D_803B25AC
	.loc	2 103
 # 103	    if (D_80151AD0 > 1) gTextPosScale *= 0.5f;
	mul	$15, $19, 152
	la	$24, D_80150B70
	addu	$25, $15, $24
	sw	$25, 128($sp)
	la	$9, D_803BA028
	addu	$10, $19, $9
	sw	$10, 132($sp)
	mul	$11, $19, 76
	la	$12, D_8014A118
	addu	$22, $11, $12
	mul	$13, $19, 2816
	addu	$14, $13, 2480
	la	$15, D_803AF9A8
	addu	$16, $14, $15
	lh	$24, D_80151AD0
	blt	$24, 2, $200033
	.loc	2 103
	li.s	$f4, 0.5
	mul.s	$f22, $f22, $f4
$200033:
	.loc	2 104
 # 104	    render_helper(0.0f);
	li.s	$f12, 0.0
	sw	$2, 148($sp)
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.alias	$2,$sp
	.loc	2 105
 # 105	    func_800B669C(1, 1);
	li	$4, 1
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 106
 # 106	    if (D_80151AD0 == 1) { w = 140; object_create(13); }
	lh	$25, D_80151AD0
	bne	$25, 1, $200034
	.loc	2 106
	.loc	2 106
	li	$20, 140
	.loc	2 106
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$200035
$200034:
	.loc	2 107
 # 107	    else { w = 69; object_create(12); }
	.loc	2 107
	li	$20, 69
	.loc	2 107
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$200035:
	.loc	2 108
 # 108	    brake_light_update(p, D_803AF9A8[p][38].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	move	$5, $16
	lw	$6, 128($sp)
	move	$7, $0
	addu	$9, $sp, 216
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 109
 # 109	    x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 110
 # 110	    y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 111
 # 111	    str = (*D_8014A118[p].owner)->name;
	.noalias	$22,$sp
	lw	$10, 72($22)
	lw	$16, 0($10)
	addu	$16, $16, 20
	.loc	2 112
 # 112	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.alias	$22,$sp
	.loc	2 113
 # 113	    music_fade(x, y, w, str);
	sll	$4, $17, 16
	sra	$11, $4, 16
	move	$4, $11
	sll	$5, $18, 16
	sra	$12, $5, 16
	move	$5, $12
	sll	$6, $20, 16
	sra	$13, $6, 16
	move	$6, $13
	move	$7, $16
	.livereg	0x0F00000E,0x00000000
	jal	music_fade
	.loc	2 114
 # 114	    func_800B669C(0, 1);
	move	$4, $0
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 115
 # 115	    if (D_8014A110 == 2) {
	lw	$14, D_8014A110
	bne	$14, 2, $200041
	.loc	2 115
	.loc	2 117
 # 116	        s32 ghostIdx;
 # 117	        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$20, 1
	lb	$15, D_803BA00C
	addu	$24, $15, 1
	blt	$24, 2, $200041
	la	$23, D_80150B70+152
	li	$25, 4
	sw	$25, 140($sp)
	la	$30, D_80152698+4
$200036:
	.loc	2 117
	.loc	2 119
 # 118	            f32 pos[3];
 # 119	            brake_light_update(ghostIdx, D_803AF9A8[0][38].position, &D_80150B70[ghostIdx], 0, screenPos);
	move	$4, $20
	la	$5, D_803AF9A8+2480
	move	$6, $23
	move	$7, $0
	addu	$9, $sp, 216
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 120
 # 120	            x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 121
 # 121	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 122
 # 122	            fcvt_wrapper(str1, "GHOST %d", ghostIdx);
	addu	$4, $sp, 220
	la	$5, $$200023
	move	$6, $20
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 123
 # 123	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 124
 # 124	            state_utility(sound_pitch_diff_halved(str1, x), y, str1);
	addu	$4, $sp, 220
	sll	$5, $17, 16
	sra	$10, $5, 16
	move	$5, $10
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$11, $4, 16
	move	$4, $11
	sll	$5, $18, 16
	sra	$12, $5, 16
	move	$5, $12
	addu	$6, $sp, 220
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	la	$2, D_803AF9A8
	.loc	2 125
 # 125	            pos[0] = D_803AF9A8[0][38].position[0];
	.noalias	$2,$sp
	l.s	$f6, 2480($2)
	s.s	$f6, 196($sp)
	.loc	2 126
 # 126	            pos[1] = D_803AF9A8[0][38].position[1];
	l.s	$f0, 2484($2)
	.loc	2 127
 # 127	            pos[2] = D_803AF9A8[0][38].position[2];
	l.s	$f8, 2488($2)
	s.s	$f8, 204($sp)
	.loc	2 128
 # 128	            pos[1] = -pos[1] / 2.0f;
	neg.s	$f10, $f0
	mul.s	$f0, $f10, $f24
	.loc	2 129
 # 129	            brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, screenPos);
	move	$4, $20
	addu	$5, $sp, 196
	move	$6, $23
	move	$7, $0
	addu	$13, $sp, 216
	sw	$13, 16($sp)
	s.s	$f0, 200($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.alias	$2,$sp
	.loc	2 130
 # 130	            x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 131
 # 131	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 132
 # 132	            if (!D_80152698[ghostIdx]) {
	.noalias	$30,$sp
	lw	$2, 0($30)
	bne	$2, 0, $200039
	.loc	2 132
	.loc	2 133
 # 133	                str = !D_803AF980 ? countdown_state.text[225] : countdown_state.text[224];
	lw	$14, D_803AF980
	bne	$14, 0, $200037
	.noalias	$21,$30
	.noalias	$21,$sp
	lw	$15, 4($21)
	lw	$16, 900($15)
	b	$200038
$200037:
	lw	$24, 4($21)
	lw	$16, 896($24)
$200038:
	.loc	2 134
 # 134	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 135
 # 135	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$9, $4, 16
	move	$4, $9
	sll	$5, $18, 16
	sra	$10, $5, 16
	move	$5, $10
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$200040
$200039:
	.loc	2 136
 # 136	            } else {
	.loc	2 137
 # 137	                str = (*D_80152698[ghostIdx])->name;
	lw	$16, 0($2)
	addu	$16, $16, 10
	.loc	2 138
 # 138	                dispatch_handler(D_801146AC[ghostIdx]);
	lw	$11, 140($sp)
	lw	$4, D_801146AC($11)
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 139
 # 139	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$22, $17, 16
	sra	$12, $22, 16
	move	$22, $12
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 140
 # 140	                y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	li	$8, 60
	addu	$18, $18, $2
	.loc	2 141
 # 141	                fcvt_wrapper(str1, "TIME %01d:%02d.%03d", (s32)(*D_80152698[ghostIdx])->time / 60,
	addu	$4, $sp, 220
	la	$5, $$200031
	lw	$24, 0($30)
	lw	$25, 0($24)
	l.s	$f0, 32($25)
	trunc.w.s	$f16, $f0, $9
	mfc1	$3, $f16
	div	$6, $3, $8
	rem	$7, $3, $8
	li.s	$f18, 1000.0
	mul.s	$f4, $f0, $f18
	trunc.w.s	$f6, $f4, $10
	mfc1	$11, $f6
	rem	$12, $11, 1000
	sw	$12, 16($sp)
	.livereg	0x1F00000E,0x80000000
	jal	fcvt_wrapper
	.loc	2 143
 # 142	                             (s32)(*D_80152698[ghostIdx])->time % 60, (s32)(1000.0f * (*D_80152698[ghostIdx])->time) % 1000);
 # 143	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 144
 # 144	                state_utility(sound_pitch_diff_halved(str1, x), y, str1);
	addu	$4, $sp, 220
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	addu	$6, $sp, 220
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$200040:
	.loc	2 117
 # 117	        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	addu	$20, $20, 1
	addu	$23, $23, 152
	lw	$24, 140($sp)
	addu	$25, $24, 4
	sw	$25, 140($sp)
	addu	$30, $30, 4
	lb	$9, D_803BA00C
	ble	$20, $9, $200036
	.alias	$30,$21
	.alias	$30,$sp
$200041:
	.loc	2 148
 # 148	    if (D_803BA028[p]) {
	lw	$10, 132($sp)
	lb	$11, 0($10)
	beq	$11, 0, $200048
	addu	$5, $sp, 172
	.loc	2 148
	.loc	2 152
 # 149	        s32 notReadyCount;
 # 150	        s32 notReadyIdx;
 # 151	        s32 pIdx;
 # 152	        f32 msgPos[3] = {0.0f, -120.0f, 300.0f};
	.noalias	$5,$21
	.noalias	$5,$gp
	la	$12, $$200033
	.set	 noat
	.set	 noat
	lw	$1, 0($12)
	sw	$1, 0($5)
	lw	$14, 4($12)
	sw	$14, 4($5)
	lw	$1, 8($12)
	sw	$1, 8($5)
	.set	 at
	.loc	2 153
 # 153	        brake_light_update(p, msgPos, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	lw	$6, 128($sp)
	move	$7, $0
	addu	$15, $sp, 216
	sw	$15, 16($sp)
	move	$20, $0
	move	$16, $0
	.livereg	0x0F00880E,0x00000000
	jal	brake_light_update
	.alias	$5,$21
	.alias	$5,$gp
	.loc	2 154
 # 154	        x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 155
 # 155	        y = screenPos[1] - object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lh	$24, 218($sp)
	subu	$18, $24, $2
	.loc	2 156
 # 156	        notReadyCount = 0;
	.loc	2 157
 # 157	        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	lh	$3, D_8014A108
	ble	$3, 0, $200044
$200042:
	.loc	2 157
	.loc	2 158
 # 158	            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
	beq	$19, $16, $200043
	lb	$25, D_803BA028($16)
	bne	$25, 0, $200043
	.loc	2 158
	.loc	2 158
	sw	$16, 188($sp)
	.loc	2 158
	addu	$20, $20, 1
$200043:
	.loc	2 157
 # 157	        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	addu	$16, $16, 1
	blt	$16, $3, $200042
$200044:
	.loc	2 160
 # 158	            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
 # 159	        }
 # 160	        if (!notReadyCount) {
	bne	$20, 0, $200045
	.loc	2 160
	.loc	2 161
 # 161	            y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 162
 # 162	            str = (u8 *)"GET READY TO RACE";
	la	$16, $$200034
	.loc	2 163
 # 163	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	sll	$22, $17, 16
	sra	$9, $22, 16
	move	$22, $9
	b	$200047
$200045:
	.loc	2 164
 # 164	        } else {
	.loc	2 165
 # 165	            str = countdown_state.text[151];
	lw	$10, 4($21)
	lw	$16, 604($10)
	.loc	2 166
 # 166	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 167
 # 167	            state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$22, $17, 16
	sra	$11, $22, 16
	move	$22, $11
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$12, $4, 16
	move	$4, $12
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 168
 # 168	            y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 169
 # 169	            if (notReadyCount == 1) {
	bne	$20, 1, $200046
	.loc	2 169
	.loc	2 170
 # 170	                dispatch_handler(D_801146AC[notReadyIdx]);
	lw	$15, 188($sp)
	mul	$24, $15, 4
	lw	$4, D_801146AC($24)
	mul	$25, $15, 76
	la	$9, D_8014A118
	addu	$17, $25, $9
	.livereg	0x0800400E,0x00000000
	jal	dispatch_handler
	.loc	2 171
 # 171	                str = (*D_8014A118[notReadyIdx].owner)->name;
	.noalias	$17,$21
	.noalias	$17,$sp
	lw	$10, 72($17)
	lw	$16, 0($10)
	addu	$16, $16, 20
	b	$200047
	.alias	$17,$21
	.alias	$17,$sp
$200046:
	.loc	2 172
 # 172	            } else str = countdown_state.text[31];
	lw	$11, 4($21)
	lw	$16, 124($11)
$200047:
	.loc	2 174
 # 173	        }
 # 174	        state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$12, $4, 16
	move	$4, $12
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 175
 # 175	        func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 176
 # 176	        func_800ED66C(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 177
 # 177	        return;
	b	$200086
$200048:
	.loc	2 179
 # 178	    }
 # 179	    if (D_80151AD0 == 1) object_create(11); else object_create(12);
	move	$2, $0
	lw	$24, 152($sp)
	la	$15, D_803BA050
	addu	$16, $24, $15
	lh	$25, D_80151AD0
	bne	$25, 1, $200049
	.loc	2 179
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$200050
$200049:
	.loc	2 179
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$200050:
	.loc	2 180
 # 180	    for (i = 0; i < D_803BA050[p]; i++) {
	.noalias	$16,$21
	.noalias	$16,$sp
	lw	$9, 0($16)
	ble	$9, 0, $200085
	.alias	$16,$21
	.alias	$16,$sp
	lw	$10, 304($sp)
	mul	$11, $10, 4
	la	$13, D_803BA050
	addu	$12, $11, $13
	sw	$12, 156($sp)
	move	$23, $0
	mul	$14, $19, 2816
	la	$24, D_803AF9A8
	addu	$15, $14, $24
	sw	$15, 132($sp)
	mul	$25, $19, 76
	la	$9, D_803BA060
	addu	$30, $25, $9
$200051:
	.loc	2 180
	.loc	2 181
 # 181	        s32 item = D_803BA060[p][i];
	.noalias	$30,$21
	.noalias	$30,$sp
	lw	$20, 0($30)
	.loc	2 182
 # 182	        if ((D_803AF9A8[p][item].angle > 0.6981317f && D_803AF9A8[p][item].angle < 2.443461f) || !D_803AF9A8[p][item].alpha) continue;
	.noalias	$22,$21
	.noalias	$22,$30
	.noalias	$22,$sp
	lw	$10, 132($sp)
	mul	$11, $20, 64
	addu	$22, $10, $11
	l.s	$f0, 8($22)
	c.lt.s	$f26, $f0
	bc1f	$200052
	c.lt.s	$f0, $f28
	bc1t	$200053
$200052:
	lbu	$2, 60($22)
	bne	$2, 0, $200054
	.alias	$22,$21
	.alias	$22,$30
	.alias	$22,$sp
$200053:
	.loc	2 182
	lw	$13, 156($sp)
	lw	$2, 0($13)
	mul	$3, $2, 4
	b	$200084
$200054:
	.loc	2 183
 # 183	        func_800ED66C(D_803AF9A8[p][item].alpha);
	mtc1	$2, $f8
	cvt.s.w	$f12, $f8
	bge	$2, $0, $200089
	li.s	$f10, 4294967296.0
	add.s	$f12, $f12, $f10
$200089:
	li.s	$f16, 45.0
	mul.s	$f18, $f16, $f22
	li.s	$f4, 300.0
	mul.s	$f20, $f18, $f4
	.livereg	0x0000000E,0x00080800
	jal	func_800ED66C
	.loc	2 184
 # 184	        if (D_803AF9A8[p][item].angle < 1.5707964f) {
	.noalias	$22,$21
	.noalias	$22,$30
	.noalias	$22,$sp
	l.s	$f6, 8($22)
	c.lt.s	$f6, $f30
	bc1f	$200055
	.loc	2 184
	.loc	2 185
 # 185	            func_800BEA3C(D_801146BC, D_801146C0);
	lw	$12, D_801146BC
	sw	$12, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 186
 # 186	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$200056
$200055:
	.loc	2 187
 # 187	        } else {
	.loc	2 188
 # 188	            func_800BEA3C(D_801146BC, D_801146C0);
	lw	$24, D_801146BC
	sw	$24, 0($sp)
	lw	$15, D_801146C0
	sw	$15, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 189
 # 189	            dispatch_handler(22);
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$200056:
	.loc	2 191
 # 190	        }
 # 191	        func_800F6928(D_803AF9A8[p][item].position[2] - 10.0f);
	l.s	$f8, 56($22)
	li.s	$f10, 10.0
	sub.s	$f12, $f8, $f10
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 192
 # 192	        brake_light_update(p, D_803AF9A8[p][item].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	addu	$5, $22, 48
	lw	$6, 128($sp)
	move	$7, $0
	addu	$25, $sp, 216
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 193
 # 193	        x = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_803B25C0;
	lh	$9, 216($sp)
	mtc1	$9, $f16
	cvt.s.w	$f18, $f16
	l.s	$f4, D_803B25C0
	div.s	$f6, $f20, $f4
	sub.s	$f8, $f18, $f6
	trunc.w.s	$f10, $f8, $10
	mfc1	$17, $f10
	.loc	2 194
 # 194	        y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 195
 # 195	        str = D_80151AD0 == 1 ? countdown_state.indexed[countdown_state.indices[2] + item]
	lh	$11, D_80151AD0
	bne	$11, 1, $200057
	lw	$13, 16($21)
	lw	$12, 12($21)
	lhu	$14, 4($12)
	mul	$24, $14, 4
	addu	$15, $13, $24
	mul	$25, $20, 4
	addu	$9, $15, $25
	lw	$16, 0($9)
	b	$200058
$200057:
	lw	$10, 16($21)
	lw	$11, 12($21)
	lhu	$12, 6($11)
	mul	$14, $12, 4
	addu	$13, $10, $14
	mul	$24, $20, 4
	addu	$15, $13, $24
	lw	$16, 0($15)
$200058:
	.loc	2 197
 # 196	                              : countdown_state.indexed[countdown_state.indices[3] + item];
 # 197	        state_utility(x, y, str);
	sll	$4, $17, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$9, $5, 16
	move	$5, $9
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 198
 # 198	        switch (item) {
	bgeu	$20, 19, $200081
	sll	$11, $20, 2
	lw	$11, $200059($11)
	j	$11
	.rdata	
$200059:
	.word	$200061
	.word	$200062
	.word	$200063
	.word	$200064
	.word	$200065
	.word	$200066
	.word	$200067
	.word	$200081
	.word	$200081
	.word	$200081
	.word	$200068
	.word	$200069
	.word	$200070
	.word	$200081
	.word	$200081
	.word	$200071
	.word	$200072
	.word	$200075
	.word	$200078
	.text	
$200061:
	.loc	2 199
 # 199	        case 0: str = D_80110D70[D_803B9FD0[p]]; break;
	lb	$12, D_803B9FD0($19)
	mul	$10, $12, 4
	lw	$16, D_80110D70($10)
	.loc	2 199
	b	$200082
$200062:
	.loc	2 200
 # 200	        case 1: str = countdown_state.indexed[countdown_state.indices[4] + D_80110E78[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$13, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$15, $13, $24
	lb	$25, D_80110E78+13($15)
	mul	$9, $25, 4
	addu	$11, $14, $9
	lw	$12, 12($21)
	lhu	$10, 8($12)
	mul	$13, $10, 4
	addu	$24, $11, $13
	lw	$16, 0($24)
	.loc	2 200
	b	$200082
$200063:
	.loc	2 201
 # 201	        case 2: str = countdown_state.indexed[countdown_state.indices[5] + D_8011103C[p+1][D_803B9FD0[p]]]; break;
	lw	$15, 16($21)
	mul	$25, $19, 13
	lb	$14, D_803B9FD0($19)
	addu	$9, $25, $14
	lb	$12, D_8011103C+13($9)
	mul	$10, $12, 4
	addu	$11, $15, $10
	lw	$13, 12($21)
	lhu	$24, 10($13)
	mul	$25, $24, 4
	addu	$14, $11, $25
	lw	$16, 0($14)
	.loc	2 201
	b	$200082
$200064:
	.loc	2 202
 # 202	        case 3: str = countdown_state.indexed[countdown_state.indices[6] + D_80111080[p+1][D_803B9FD0[p]]]; break;
	lw	$9, 16($21)
	mul	$12, $19, 13
	lb	$15, D_803B9FD0($19)
	addu	$10, $12, $15
	lb	$13, D_80111080+13($10)
	mul	$24, $13, 4
	addu	$11, $9, $24
	lw	$25, 12($21)
	lhu	$14, 12($25)
	mul	$12, $14, 4
	addu	$15, $11, $12
	lw	$16, 0($15)
	.loc	2 202
	b	$200082
$200065:
	.loc	2 203
 # 203	        case 4: str = countdown_state.indexed[countdown_state.indices[7] + D_8011119C[p+1][D_803B9FD0[p]]]; break;
	lw	$10, 16($21)
	mul	$13, $19, 13
	lb	$9, D_803B9FD0($19)
	addu	$24, $13, $9
	lb	$25, D_8011119C+13($24)
	mul	$14, $25, 4
	addu	$11, $10, $14
	lw	$12, 12($21)
	lhu	$15, 14($12)
	mul	$13, $15, 4
	addu	$9, $11, $13
	lw	$16, 0($9)
	.loc	2 203
	b	$200082
$200066:
	.loc	2 204
 # 204	        case 5: str = countdown_state.indexed[countdown_state.indices[8] + D_80111230[p+1][D_803B9FD0[p]]]; break;
	lw	$24, 16($21)
	mul	$25, $19, 13
	lb	$10, D_803B9FD0($19)
	addu	$14, $25, $10
	lb	$12, D_80111230+13($14)
	mul	$15, $12, 4
	addu	$11, $24, $15
	lw	$13, 12($21)
	lhu	$9, 16($13)
	mul	$25, $9, 4
	addu	$10, $11, $25
	lw	$16, 0($10)
	.loc	2 204
	b	$200082
$200067:
	.loc	2 205
 # 205	        case 6: str = countdown_state.indexed[countdown_state.indices[9] + D_8011128C[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$12, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$15, $12, $24
	lb	$13, D_8011128C+13($15)
	mul	$9, $13, 4
	addu	$11, $14, $9
	lw	$25, 12($21)
	lhu	$10, 18($25)
	mul	$12, $10, 4
	addu	$24, $11, $12
	lw	$16, 0($24)
	.loc	2 205
	b	$200082
$200068:
	.loc	2 206
 # 206	        case 10: str = countdown_state.indexed[countdown_state.indices[12] + D_8012E67C[p]]; break;
	lw	$15, 16($21)
	lb	$13, D_8012E67C($19)
	mul	$14, $13, 4
	addu	$9, $15, $14
	lw	$25, 12($21)
	lhu	$10, 24($25)
	mul	$11, $10, 4
	addu	$12, $9, $11
	lw	$16, 0($12)
	.loc	2 206
	b	$200082
$200069:
	.loc	2 207
 # 207	        case 11: str = countdown_state.indexed[countdown_state.indices[10] + D_8011157C[p+1][D_803B9FD0[p]]]; break;
	lw	$24, 16($21)
	mul	$13, $19, 13
	lb	$15, D_803B9FD0($19)
	addu	$14, $13, $15
	lb	$25, D_8011157C+13($14)
	mul	$10, $25, 4
	addu	$9, $24, $10
	lw	$11, 12($21)
	lhu	$12, 20($11)
	mul	$13, $12, 4
	addu	$15, $9, $13
	lw	$16, 0($15)
	.loc	2 207
	b	$200082
$200070:
	.loc	2 208
 # 208	        case 12: str = countdown_state.indexed[countdown_state.indices[11] + D_801115C0[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$25, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$10, $25, $24
	lb	$11, D_801115C0+13($10)
	mul	$12, $11, 4
	addu	$9, $14, $12
	lw	$13, 12($21)
	lhu	$15, 22($13)
	mul	$25, $15, 4
	addu	$24, $9, $25
	lw	$16, 0($24)
	.loc	2 208
	b	$200082
$200071:
	.loc	2 209
 # 209	        case 15: fcvt_wrapper(str1, "%d", D_803B28B0[p] + 1); str = str1; break;
	addu	$4, $sp, 220
	la	$5, $$200054
	mul	$10, $19, 2
	lh	$6, D_803B28B0($10)
	addu	$6, $6, 1
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 209
	addu	$16, $sp, 220
	.loc	2 209
	b	$200082
$200072:
	.loc	2 211
 # 210	        case 16:
 # 211	            if (D_80152698[1]) func_800BE6A4(str1, (*D_80152698[1])->name);
	lw	$2, D_80152698+4
	beq	$2, 0, $200073
	.loc	2 211
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$200074
$200073:
	.loc	2 212
 # 212	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$11, 16($21)
	lw	$14, 12($21)
	lhu	$12, 30($14)
	mul	$13, $12, 4
	addu	$15, $11, $13
	lw	$5, 0($15)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$200074:
	.loc	2 213
 # 213	            str = str1;
	addu	$16, $sp, 220
	.loc	2 214
 # 214	            break;
	b	$200082
$200075:
	.loc	2 216
 # 215	        case 17:
 # 216	            if (D_80152698[2]) func_800BE6A4(str1, (*D_80152698[2])->name);
	lw	$2, D_80152698+8
	beq	$2, 0, $200076
	.loc	2 216
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$200077
$200076:
	.loc	2 217
 # 217	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$9, 16($21)
	lw	$25, 12($21)
	lhu	$24, 30($25)
	mul	$10, $24, 4
	addu	$14, $9, $10
	lw	$5, 0($14)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$200077:
	.loc	2 218
 # 218	            str = str1;
	addu	$16, $sp, 220
	.loc	2 219
 # 219	            break;
	b	$200082
$200078:
	.loc	2 221
 # 220	        case 18:
 # 221	            if (D_80152698[3]) func_800BE6A4(str1, (*D_80152698[3])->name);
	lw	$2, D_80152698+12
	beq	$2, 0, $200079
	.loc	2 221
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$200080
$200079:
	.loc	2 222
 # 222	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$12, 16($21)
	lw	$11, 12($21)
	lhu	$13, 30($11)
	mul	$15, $13, 4
	addu	$25, $12, $15
	lw	$5, 0($25)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$200080:
	.loc	2 223
 # 223	            str = str1;
	addu	$16, $sp, 220
	.loc	2 224
 # 224	            break;
	b	$200082
$200081:
	.loc	2 225
 # 225	        default: str = 0; break;
	move	$16, $0
	.loc	2 225
$200082:
	.loc	2 227
 # 226	        }
 # 227	        if (str) {
	beq	$16, 0, $200083
	.loc	2 227
	.loc	2 228
 # 228	            brake_light_update(p, D_803AF9A8[p][item+19].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	addu	$5, $22, 1264
	lw	$6, 128($sp)
	move	$7, $0
	addu	$24, $sp, 216
	sw	$24, 16($sp)
	li.s	$f16, 50.0
	mul.s	$f4, $f16, $f22
	li.s	$f18, 300.0
	mul.s	$f20, $f4, $f18
	.livereg	0x0F00000E,0x00000800
	jal	brake_light_update
	.alias	$22,$21
	.alias	$22,$30
	.alias	$22,$sp
	.loc	2 229
 # 229	            x = screenPos[0] - 50.0f * gTextPosScale * 300.0f / D_803B25C0;
	lh	$9, 216($sp)
	mtc1	$9, $f6
	cvt.s.w	$f8, $f6
	l.s	$f10, D_803B25C0
	div.s	$f16, $f20, $f10
	sub.s	$f4, $f8, $f16
	trunc.w.s	$f18, $f4, $10
	mfc1	$17, $f18
	.loc	2 230
 # 230	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 231
 # 231	            state_utility(x, y, str);
	sll	$4, $17, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$11, $5, 16
	move	$5, $11
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$200083:
	.loc	2 180
 # 180	    for (i = 0; i < D_803BA050[p]; i++) {
	lw	$13, 156($sp)
	lw	$2, 0($13)
	mul	$3, $2, 4
$200084:
	addu	$23, $23, 4
	addu	$30, $30, 4
	blt	$23, $3, $200051
	.alias	$30,$21
	.alias	$30,$sp
$200085:
	.loc	2 234
 # 234	    func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 235
 # 235	    func_800ED66C(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 236
 # 236	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 237
 # 237	}
$200086:
	lh	$3, D_8014A108
	lw	$2, 148($sp)
	.loc	2 88
 #  88	        else drawOptions(i);
	.loc	2 240
 # 240	{
	.loc	2 241
 # 241	}
$200087:
	.loc	2 86
 #  86	    for (i = 0; i < D_8014A108; i++) {
	addu	$19, $19, 1
	lw	$12, 152($sp)
	addu	$15, $12, 4
	sw	$15, 152($sp)
	addu	$2, $2, 4
	blt	$19, $3, $200032
	.alias	$21,$sp
$200088:
	.loc	2 90
 #  87	        if (D_803B9B90[i] == 1) animButtonText(i);
 #  88	        else drawOptions(i);
 #  89	    }
 #  90	    return 1;
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
	addu	$sp, 312
	j	$31
	.end	func_803A44FC
