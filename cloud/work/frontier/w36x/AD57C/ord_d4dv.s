	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 403
 # 403	{
	.ent	func_803AD0D4 2
func_803AD0D4:
	.option	O3
	subu	$sp, 144
	sw	$31, 28($sp)
	.mask	0x80000000, -116
	.frame	$sp, 144, $31
	.loc	2 403
	.loc	2 403
	.loc	2 408
 # 404	    s32 track,i,x,y;
 # 405	    u8 *label,text[48],*data;
 # 406	    u32 distance;
 # 407	    f32 average;
 # 408	    func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 409
 # 409	    track=D_803BA858/2;
	lb	$2, D_803BA858
	div	$3, $2, 2
	.loc	2 410
 # 410	    if(D_803BA858&1)track+=6;
	and	$14, $2, 1
	beq	$14, 0, $189
	.loc	2 410
	addu	$3, $3, 6
$189:
	.loc	2 411
 # 411	    if(D_803BA800)data=PROFILE_DATA+140+track*96;
	lw	$2, D_803BA800
	beq	$2, 0, $190
	.loc	2 411
	lw	$15, 0($2)
	lw	$24, 44($15)
	lw	$25, 0($24)
	mul	$14, $3, 96
	addu	$19, $25, $14
	addu	$19, $19, 140
	b	$191
$190:
	.loc	2 412
 # 412	    else data=D_80150F88+track*96;
	mul	$15, $3, 96
	la	$24, D_80150F88
	addu	$19, $15, $24
$191:
	.loc	2 413
 # 413	    distance=U32_AT(data,88);
	lw	$30, 88($19)
	.loc	2 414
 # 414	    if(D_80146111)distance=distance*10/6;
	lb	$25, D_80146111
	beq	$25, 0, $192
	.loc	2 414
	mul	$14, $30, 10
	move	$30, $14
	divu	$15, $30, 6
	move	$30, $15
$192:
	.loc	2 415
 # 415	    object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 416
 # 416	    crowd_cheer_play(D_803B6530,12,86,308,94+object_bytes_sum_global()*8);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 12
	li	$6, 86
	li	$7, 308
	mul	$24, $2, 8
	addu	$25, $24, 94
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 417
 # 417	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 418
 # 418	    for(i=0;i<14;i++) {
	move	$17, $0
	li.s	$f20, 0.0
	lw	$21, 132($sp)
	lw	$20, 128($sp)
	addu	$18, $sp, 76
$193:
	.loc	2 418
	.loc	2 419
 # 419	        if(i==0) {x=100;y=90;}
	bne	$17, 0, $194
	.loc	2 419
	.loc	2 419
	li	$21, 100
	.loc	2 419
	li	$20, 90
	b	$199
$194:
	.loc	2 420
 # 420	        else if(i==3) {x=110;y+=object_bytes_sum_global();}
	bne	$17, 3, $195
	.loc	2 420
	.loc	2 420
	li	$21, 110
	.loc	2 420
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	b	$199
$195:
	.loc	2 421
 # 421	        else if(i==7) {x=270;y=90+object_bytes_sum_global()*4;}
	bne	$17, 7, $196
	.loc	2 421
	.loc	2 421
	li	$21, 270
	.loc	2 421
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$20, $2, 4
	addu	$20, $20, 90
	b	$199
$196:
	.loc	2 422
 # 422	        else if(i==10) {x=260;y=90;continue;}
	bne	$17, 10, $197
	.loc	2 422
	.loc	2 422
	li	$21, 260
	.loc	2 422
	li	$20, 90
	.loc	2 422
	b	$223
$197:
	.loc	2 423
 # 423	        else if((i==12 || i==13) && !D_803BA800)continue;
	beq	$17, 12, $198
	bne	$17, 13, $199
$198:
	lw	$14, D_803BA800
	beq	$14, 0, $223
	.loc	2 423
$199:
	la	$4, countdown_state
	.loc	2 424
 # 424	        label=NAME(28,i);
	.noalias	$4,$sp
	lw	$2, 16($4)
	lw	$15, 12($4)
	lhu	$3, 56($15)
	mul	$24, $3, 4
	addu	$25, $2, $24
	mul	$14, $17, 4
	addu	$15, $25, $14
	lw	$16, 0($15)
	.loc	2 425
 # 425	        if(i==11 && D_80146111)label=NAME(28,14);
	addu	$5, $21, -4
	sll	$24, $5, 16
	move	$5, $24
	sra	$25, $5, 16
	move	$5, $25
	addu	$23, $21, 4
	sll	$14, $23, 16
	move	$23, $14
	sra	$15, $23, 16
	move	$23, $15
	bne	$17, 11, $200
	.alias	$4,$sp
	lb	$24, D_80146111
	beq	$24, 0, $200
	.loc	2 425
	mul	$25, $3, 4
	addu	$14, $2, $25
	lw	$16, 56($14)
$200:
	.loc	2 426
 # 426	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$22, $20, 16
	sra	$24, $22, 16
	move	$22, $24
	sll	$5, $22, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $16
	.livereg	0x0E00020E,0x00000000
	jal	state_utility
	.loc	2 427
 # 427	        switch(i) {
	bgeu	$17, 14, $218
	sll	$14, $17, 2
	lw	$14, $201($14)
	j	$14
	.rdata	
$201:
	.word	$203
	.word	$204
	.word	$205
	.word	$207
	.word	$208
	.word	$209
	.word	$210
	.word	$211
	.word	$212
	.word	$213
	.word	$214
	.word	$215
	.word	$216
	.word	$217
	.text	
$203:
	.loc	2 428
 # 428	        case 0:func_803AC330(text,F32_AT(data,64),0);break;
	move	$4, $18
	l.s	$f12, 64($19)
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 428
	b	$218
$204:
	.loc	2 429
 # 429	        case 1:fcvt_wrapper(text,D_803B8AE8,U16_AT(data,68));break;
	move	$4, $18
	la	$5, D_803B8AE8
	lhu	$6, 68($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 429
	b	$218
$205:
	.loc	2 431
 # 430	        case 2:
 # 431	            average=0.0f;
	mov.s	$f12, $f20
	.loc	2 432
 # 432	            if(U16_AT(data,68)>0)
	lhu	$2, 68($19)
	ble	$2, 0, $206
	.loc	2 433
 # 433	                average=F32_AT(data,64)/(f32)(u32)U16_AT(data,68);
	l.s	$f4, 64($19)
	mtc1	$2, $f6
	cvt.s.w	$f8, $f6
	bge	$2, $0, $224
	li.s	$f10, 4294967296.0
	add.s	$f8, $f8, $f10
$224:
	div.s	$f12, $f4, $f8
$206:
	.loc	2 434
 # 434	            func_803AC330(text,average,0);
	move	$4, $18
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 435
 # 435	            break;
	b	$218
$207:
	.loc	2 436
 # 436	        case 3:fcvt_wrapper(text,D_803B8AEC,U16_AT(data,70));break;
	move	$4, $18
	la	$5, D_803B8AEC
	lhu	$6, 70($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 436
	b	$218
$208:
	.loc	2 437
 # 437	        case 4:fcvt_wrapper(text,D_803B8AF0,U16_AT(data,72));break;
	move	$4, $18
	la	$5, D_803B8AF0
	lhu	$6, 72($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 437
	b	$218
$209:
	.loc	2 438
 # 438	        case 5:fcvt_wrapper(text,D_803B8AF4,U16_AT(data,74));break;
	move	$4, $18
	la	$5, D_803B8AF4
	lhu	$6, 74($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 438
	b	$218
$210:
	.loc	2 439
 # 439	        case 6:fcvt_wrapper(text,D_803B8AF8,U16_AT(data,76));break;
	move	$4, $18
	la	$5, D_803B8AF8
	lhu	$6, 76($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 439
	b	$218
$211:
	.loc	2 440
 # 440	        case 7:fcvt_wrapper(text,D_803B8AFC,U16_AT(data,78));break;
	move	$4, $18
	la	$5, D_803B8AFC
	lhu	$6, 78($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 440
	b	$218
$212:
	.loc	2 441
 # 441	        case 8:fcvt_wrapper(text,D_803B8B00,U16_AT(data,80));break;
	move	$4, $18
	la	$5, D_803B8B00
	lhu	$6, 80($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 441
	b	$218
$213:
	.loc	2 442
 # 442	        case 9:fcvt_wrapper(text,D_803B8B04,U16_AT(data,82));break;
	move	$4, $18
	la	$5, D_803B8B04
	lhu	$6, 82($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 442
	b	$218
$214:
	.loc	2 443
 # 443	        case 10:fcvt_wrapper(text,D_803B8B08,U16_AT(data,84));break;
	move	$4, $18
	la	$5, D_803B8B08
	lhu	$6, 84($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 443
	b	$218
$215:
	li	$2, 10
	.loc	2 444
 # 444	        case 11: {u32 divisor=10;fcvt_wrapper(text,D_803B8B0C,distance/divisor,distance%divisor);break;}
	.loc	2 444
	.loc	2 444
	move	$4, $18
	la	$5, D_803B8B0C
	divu	$6, $30, $2
	remu	$7, $30, $2
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 444
	b	$218
$216:
	.loc	2 445
 # 445	        case 12:fcvt_wrapper(text,D_803B8B18,func_800B78A4((u16)(U16_AT(data,92)&255),16));break;
	lhu	$4, 92($19)
	and	$15, $4, 255
	move	$4, $15
	and	$24, $4, 65535
	move	$4, $24
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	move	$4, $18
	la	$5, D_803B8B18
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 445
	b	$218
$217:
	.loc	2 446
 # 446	        case 13:fcvt_wrapper(text,D_803B8B20,func_800B78A4((u16)(U16_AT(data,92)&65280),16));break;
	lhu	$4, 92($19)
	and	$25, $4, 65280
	move	$4, $25
	and	$14, $4, 65535
	move	$4, $14
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	move	$4, $18
	la	$5, D_803B8B20
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 446
$218:
	.loc	2 448
 # 447	        }
 # 448	        if(i==0 || i==2)object_create(2);
	beq	$17, 0, $219
	bne	$17, 2, $220
$219:
	.loc	2 448
	li	$4, 2
	.livereg	0x0800000E,0x00000000
	jal	object_create
$220:
	.loc	2 449
 # 449	        state_utility(x+4,y,text);
	sll	$4, $23, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $22, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 450
 # 450	        if(i==0 || i==2)object_create(11);
	beq	$17, 0, $221
	bne	$17, 2, $222
$221:
	.loc	2 450
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
$222:
	.loc	2 451
 # 451	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	.loc	2 418
 # 418	    for(i=0;i<14;i++) {
$223:
	addu	$17, $17, 1
	bne	$17, 14, $193
	sw	$21, 132($sp)
	sw	$20, 128($sp)
	.loc	2 453
 # 453	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 144
	j	$31
	.end	func_803AD0D4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 698
 # 698	{
	.ent	recordscr_drawView 2
recordscr_drawView:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 698
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	recordscr_drawView
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803AD57C
	.loc	2 775
 # 775	{
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
	beq	$2, 0, $277
	beq	$2, 1, $278
	b	$316
$277:
	.loc	2 778
 # 778	    case 0:func_803AA3D8();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AA3D8
	.loc	2 778
	b	$316
$278:
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
	bne	$2, 0, $279
	la	$21, countdown_state
	.noalias	$21,$sp
	lw	$15, 4($21)
	lw	$7, 704($15)
	b	$280
	.alias	$21,$sp
$279:
	lw	$7, 0($2)
	addu	$7, $7, 20
	la	$21, countdown_state
$280:
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
$281:
	.loc	2 710
	.loc	2 711
 # 711	            if(D_803B5F28[i+11].angle>0.6981316804885864f && D_803B5F28[i+11].angle<2.4434609413146973f)continue;
	.noalias	$18,$sp
	l.s	$f0, 712($18)
	li.s	$f8, 0.6981316804885864
	c.lt.s	$f8, $f0
	bc1f	$282
	li.s	$f10, 2.4434609413146973
	c.lt.s	$f0, $f10
	bc1t	$294
	.loc	2 711
$282:
	.loc	2 712
 # 712	            if(!D_803B5F28[i+11].alpha)continue;
	lbu	$2, 764($18)
	beq	$2, 0, $294
	.loc	2 712
	.loc	2 713
 # 713	            func_800ED66C((f32)(u32)D_803B5F28[i+11].alpha);
	mtc1	$2, $f4
	cvt.s.w	$f12, $f4
	bge	$2, $0, $317
	li.s	$f6, 4294967296.0
	add.s	$f12, $f12, $f6
$317:
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
	bc1f	$283
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
	b	$284
$283:
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
$284:
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
	bne	$20, 0, $293
	.loc	2 727
	.loc	2 728
 # 728	                if(D_803BA880==4) {
	lw	$2, D_803BA880
	bne	$2, 4, $285
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
	b	$292
$285:
	.loc	2 730
 # 730	                } else if(D_803BA880==5)str=countdown_state.text[176];
	bne	$2, 5, $286
	.loc	2 730
	lw	$24, 4($21)
	lw	$7, 704($24)
	b	$292
$286:
	.loc	2 731
 # 731	                else if(D_803BA858<12 && (D_803BA858&1)) {
	lb	$2, D_803BA858
	bge	$2, 12, $289
	and	$25, $2, 1
	beq	$25, 0, $289
	.loc	2 731
	.loc	2 732
 # 732	                    fcvt_wrapper(str1,D_803B8AE0,D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2],countdown_state.text[6]);str=str1;
	blt	$2, 13, $287
	addu	$3, $2, -6
	b	$288
$287:
	div	$3, $2, 2
$288:
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
	b	$292
$289:
	.loc	2 733
 # 733	                } else str=D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2];
	blt	$2, 13, $290
	addu	$3, $2, -6
	b	$291
$290:
	div	$3, $2, 2
$291:
	mul	$24, $3, 4
	lw	$7, D_8011A8D8($24)
$292:
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
	b	$294
$293:
	.loc	2 735
 # 735	            } else if(i==1){str=NAME(52,D_803BA818[D_803BA880]);state_utility(x,y,str);}
	bne	$20, 1, $294
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
$294:
	addu	$20, $20, 1
	addu	$18, $18, 64
	bne	$20, 2, $281
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
	bgeu	$14, 13, $315
	sll	$14, $14, 2
	lw	$14, $295($14)
	j	$14
	.rdata	
$295:
	.word	$297
	.word	$298
	.word	$299
	.word	$300
	.word	$301
	.word	$302
	.word	$303
	.word	$309
	.word	$310
	.word	$311
	.word	$312
	.word	$313
	.word	$314
	.text	
	.alias	$21,$sp
$297:
	.loc	2 739
 # 739	        case 0:func_803AD0D4();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AD0D4
	.loc	2 739
	b	$315
$298:
	.loc	2 740
 # 740	        case 1:func_803ACF5C(0);break;
	move	$20, $0
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 740
	b	$315
$299:
	.loc	2 741
 # 741	        case 2:func_803ACF5C(1);break;
	li	$20, 1
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 741
	b	$315
$300:
	.loc	2 742
 # 742	        case 3:func_803ACF5C(2);break;
	li	$20, 2
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 742
	b	$315
$301:
	.loc	2 743
 # 743	        case 4:func_803ACD5C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ACD5C
	.loc	2 743
	b	$315
$302:
	.loc	2 744
 # 744	        case 5:func_803ACA94();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ACA94
	.loc	2 744
	b	$315
$303:
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
	beq	$2, 0, $304
	.loc	2 761
	lw	$25, 0($2)
	lw	$24, 44($25)
	lw	$15, 0($24)
	lb	$14, D_803BA858
	mul	$25, $14, 64
	addu	$17, $15, $25
	addu	$17, $17, 12
	b	$305
$304:
	.loc	2 762
 # 762	            else stats=D_80151410+D_803BA858*64-1280;
	lb	$24, D_803BA858
	mul	$14, $24, 64
	addu	$15, $14, -1280
	la	$25, D_80151410
	addu	$17, $15, $25
$305:
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
$306:
	.loc	2 764
	.loc	2 765
 # 765	                if(statIdx==0) {x=110;y=90;}
	bne	$16, 0, $307
	.loc	2 765
	.loc	2 765
	li	$20, 110
	.loc	2 765
	li	$18, 90
	b	$308
$307:
	.loc	2 766
 # 766	                else if(statIdx==5) {x=250;y=90;}
	bne	$16, 20, $308
	.loc	2 766
	.loc	2 766
	li	$20, 250
	.loc	2 766
	li	$18, 90
$308:
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
	bne	$16, 40, $306
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
	b	$315
$309:
	.loc	2 746
 # 746	        case 7:func_803AC7F4();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AC7F4
	.loc	2 746
	b	$315
$310:
	.loc	2 747
 # 747	        case 8:func_803AC54C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AC54C
	.loc	2 747
	b	$315
$311:
	.loc	2 748
 # 748	        case 9:func_803ABEC8();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ABEC8
	.loc	2 748
	b	$315
$312:
	.loc	2 749
 # 749	        case 10:func_803AB848();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AB848
	.loc	2 749
	b	$315
$313:
	.loc	2 750
 # 750	        case 11:func_803AB5FC();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AB5FC
	.loc	2 750
	b	$315
$314:
	.loc	2 751
 # 751	        case 12:func_803AAF0C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AAF0C
	.loc	2 751
$315:
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
$316:
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
