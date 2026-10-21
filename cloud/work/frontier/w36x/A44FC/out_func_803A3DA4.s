	.verstamp	3 19
	.option	pic0
	.extern	D_8014A108 2
	.extern	D_803B97A8 4
	.extern	D_803B97AC 4
	.extern	D_803B97B0 4
	.extern	D_803B97B4 4
	.extern	D_803B97B8 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A3DA4
	.loc	2 40
 #  40	{
	.ent	func_803A3DA4 2
func_803A3DA4:
	.option	O3
	subu	$sp, 64
	sw	$31, 28($sp)
	.mask	0x80000000, -36
	.frame	$sp, 64, $31
	.loc	2 40
	move	$8, $4
	.loc	2 40
	.loc	2 41
 #  41	    s32 type=blt->AnimID&15;
	lw	$2, 44($8)
	and	$14, $2, 15
	sw	$14, 60($sp)
	.loc	2 42
 #  42	    s32 p=(blt->AnimID>>4)&15;
	srl	$9, $2, 4
	and	$15, $9, 15
	move	$9, $15
	.loc	2 43
 #  43	    s32 opt=blt->AnimID>>16;
	srl	$11, $2, 16
	.loc	2 45
 #  44	    s16 screenPos[2];
 #  45	    if (p>=D_8014A108) {
	lh	$24, D_8014A108
	blt	$9, $24, $32
	.loc	2 45
	.loc	2 46
 #  46	        input_new_data_wrapper(blt,1);
	move	$4, $8
	li	$5, 1
	sw	$8, 64($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$8, 64($sp)
	.loc	2 47
 #  47	        blt->AnimFunc=0;
	sw	$0, 40($8)
	.loc	2 48
 #  48	        return 1;
	li	$2, 1
	b	$41
$32:
	.loc	2 50
 #  49	    }
 #  50	    if (D_803BA028[p]) {
	lb	$25, D_803BA028($9)
	beq	$25, 0, $33
	.loc	2 50
	.loc	2 51
 #  51	        input_new_data_wrapper(blt,1);
	move	$4, $8
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	.loc	2 52
 #  52	        return 1;
	li	$2, 1
	b	$41
$33:
	.loc	2 54
 #  53	    }
 #  54	    if (input_new_data_wrapper(blt,
	.noalias	$10,$sp
	mul	$12, $9, 2816
	mul	$13, $11, 64
	addu	$14, $12, $13
	la	$15, D_803AF9A8
	addu	$10, $14, $15
	l.s	$f0, 8($10)
	l.s	$f4, D_803B97A8
	c.lt.s	$f4, $f0
	move	$5, $0
	bc1f	$42
	li	$5, 1
$42:
	beq	$5, 0, $34
	l.s	$f6, D_803B97AC
	c.lt.s	$f0, $f6
	move	$5, $0
	bc1f	$43
	li	$5, 1
$43:
	bne	$5, 0, $35
$34:
	lbu	$5, 60($10)
	seq	$24, $5, 0
	move	$5, $24
$35:
	move	$4, $8
	sw	$8, 64($sp)
	sw	$9, 56($sp)
	sw	$10, 40($sp)
	sw	$11, 52($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$8, 64($sp)
	lw	$9, 56($sp)
	lw	$10, 40($sp)
	lw	$11, 52($sp)
	beq	$2, 0, $36
	.alias	$10,$sp
	.loc	2 56
 #  55	        (D_803B97A8<D_803AF9A8[p][opt].state.depth && D_803AF9A8[p][opt].state.depth<D_803B97AC) || !D_803AF9A8[p][opt].state.alpha))
 #  56	        return 1;
	li	$2, 1
	b	$41
$36:
	.loc	2 57
 #  57	    brake_light_update(p,&D_803AF9A8[p][opt+19].matrix[12],&D_80150B70[p],0,screenPos);
	move	$4, $9
	addu	$5, $10, 1264
	mul	$25, $9, 152
	la	$12, D_80150B70
	addu	$6, $25, $12
	move	$7, $0
	addu	$13, $sp, 48
	sw	$13, 16($sp)
	sw	$8, 64($sp)
	sw	$9, 56($sp)
	sw	$10, 40($sp)
	sw	$11, 52($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	lw	$8, 64($sp)
	lw	$9, 56($sp)
	lw	$10, 40($sp)
	lw	$11, 52($sp)
	.loc	2 58
 #  58	    blt->X=screenPos[0]-blt->Width/2;
	lh	$2, 20($8)
	lh	$14, 48($sp)
	div	$15, $2, 2
	subu	$24, $14, $15
	sh	$24, 14($8)
	.loc	2 59
 #  59	    blt->Y=screenPos[1]-(blt->Height/4)/2;
	lh	$4, 22($8)
	div	$3, $4, 4
	lh	$25, 50($sp)
	div	$12, $3, 2
	subu	$13, $25, $12
	sh	$13, 16($8)
	.loc	2 60
 #  60	    blt->Left=0;
	sh	$0, 32($8)
	.loc	2 61
 #  61	    blt->Right=blt->Width-1;
	addu	$14, $2, -1
	sh	$14, 34($8)
	.loc	2 62
 #  62	    blt->Top=0;
	sh	$0, 28($8)
	.loc	2 63
 #  63	    blt->Bot=(blt->Height/4)-1;
	addu	$15, $3, -1
	sh	$15, 30($8)
	.loc	2 64
 #  64	    blt->Alpha=D_803AF9A8[p][opt].state.alpha;
	.noalias	$10,$sp
	lbu	$24, 60($10)
	sb	$24, 24($8)
	.loc	2 65
 #  65	    if (D_803B97B0 < D_803AF9A8[p][opt].state.depth) {
	l.s	$f8, D_803B97B0
	l.s	$f10, 8($10)
	c.lt.s	$f8, $f10
	bc1f	$37
	.alias	$10,$sp
	.loc	2 65
	.loc	2 66
 #  66	        blt->Top += blt->Height/2;
	div	$2, $4, 2
	lh	$25, 28($8)
	addu	$12, $25, $2
	sh	$12, 28($8)
	.loc	2 67
 #  67	        blt->Bot += blt->Height/2;
	lh	$13, 30($8)
	addu	$14, $13, $2
	sh	$14, 30($8)
$37:
	.loc	2 69
 #  68	    }
 #  69	    if (type==1) {
	lw	$15, 60($sp)
	bne	$15, 1, $40
	.loc	2 69
	.loc	2 70
 #  70	        blt->Top+=(blt->Height/4);
	lh	$24, 28($8)
	addu	$25, $24, $3
	sh	$25, 28($8)
	.loc	2 71
 #  71	        blt->Bot+=(blt->Height/4);
	lh	$12, 30($8)
	addu	$13, $12, $3
	sh	$13, 30($8)
	.loc	2 72
 #  72	        switch (opt) {
	beq	$11, 13, $38
	beq	$11, 14, $39
	b	$40
$38:
	.loc	2 74
 #  73	        case 13:
 #  74	            blt->Right=(D_80111310[p][D_803B9FD0[p]]-.75f)/D_803B97B4*(blt->Width-1)/15.0f;
	mul	$14, $9, 52
	lb	$15, D_803B9FD0($9)
	mul	$24, $15, 4
	addu	$25, $14, $24
	l.s	$f16, D_80111310($25)
	li.s	$f18, .75
	sub.s	$f4, $f16, $f18
	l.s	$f6, D_803B97B4
	div.s	$f8, $f4, $f6
	lh	$12, 20($8)
	addu	$13, $12, -1
	mtc1	$13, $f10
	cvt.s.w	$f16, $f10
	mul.s	$f18, $f8, $f16
	li.s	$f4, 15.0
	div.s	$f6, $f18, $f4
	trunc.w.s	$f10, $f6, $15
	mfc1	$14, $f10
	sh	$14, 34($8)
	.loc	2 75
 #  75	            break;
	b	$40
$39:
	.loc	2 77
 #  76	        case 14:
 #  77	            blt->Right=(D_80111414[p][D_803B9FD0[p]]-.75f)/D_803B97B8*(blt->Width-1)/15.0f;
	mul	$24, $9, 52
	lb	$25, D_803B9FD0($9)
	mul	$12, $25, 4
	addu	$13, $24, $12
	l.s	$f8, D_80111414($13)
	li.s	$f16, .75
	sub.s	$f18, $f8, $f16
	l.s	$f4, D_803B97B8
	div.s	$f6, $f18, $f4
	lh	$15, 20($8)
	addu	$14, $15, -1
	mtc1	$14, $f10
	cvt.s.w	$f8, $f10
	mul.s	$f16, $f6, $f8
	li.s	$f18, 15.0
	div.s	$f4, $f16, $f18
	trunc.w.s	$f10, $f4, $25
	mfc1	$24, $f10
	sh	$24, 34($8)
	.loc	2 78
 #  78	            break;
$40:
	.loc	2 81
 #  79	        }
 #  80	    }
 #  81	    Input_ApplyPadConfig(blt);
	move	$4, $8
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.loc	2 82
 #  82	    return 1;
	li	$2, 1
$41:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 64
	j	$31
	.end	func_803A3DA4
