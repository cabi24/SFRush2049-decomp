	.verstamp	3 19
	.option	pic0
	.extern	D_8014A108 2
	.extern	D_803B97A0 4
	.extern	D_803B97A4 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A3A6C
	.loc	2 41
 #  41	{
	.ent	func_803A3A6C 2
func_803A3A6C:
	.option	O3
	subu	$sp, 72
	sw	$31, 28($sp)
	.mask	0x80000000, -44
	.frame	$sp, 72, $31
	.loc	2 41
	move	$9, $4
	.loc	2 41
	.loc	2 42
 #  42	    s32 player=blt->AnimID&15;
	lw	$2, 44($9)
	and	$8, $2, 15
	.loc	2 43
 #  43	    s32 marker=(blt->AnimID>>16)&255;
	srl	$11, $2, 16
	and	$14, $11, 255
	move	$11, $14
	.loc	2 49
 #  44	    MenuSlot *slot;
 #  45	    s32 palette;
 #  46	    s16 position[2];
 #  47	    s32 channel;
 #  48	
 #  49	    if (player>=D_8014A108) {
	lh	$15, D_8014A108
	blt	$8, $15, $32
	.loc	2 49
	.loc	2 50
 #  50	        input_new_data_wrapper(blt,1);
	move	$4, $9
	li	$5, 1
	sw	$9, 72($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$9, 72($sp)
	.loc	2 51
 #  51	        blt->AnimFunc=0;
	sw	$0, 40($9)
	.loc	2 52
 #  52	        return 1;
	li	$2, 1
	b	$40
$32:
	.loc	2 54
 #  53	    }
 #  54	    if (D_803BA028[player]) {
	lb	$24, D_803BA028($8)
	beq	$24, 0, $33
	.loc	2 54
	.loc	2 55
 #  55	        input_new_data_wrapper(blt,1);
	move	$4, $9
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	.loc	2 56
 #  56	        return 1;
	li	$2, 1
	b	$40
$33:
	.loc	2 58
 #  57	    }
 #  58	    slot=&D_803AF9A8[player][marker];
	.loc	2 59
 #  59	    if (input_new_data_wrapper(blt,
	mul	$10, $11, 64
	.noalias	$3,$sp
	mul	$25, $8, 2816
	addu	$12, $25, $10
	la	$13, D_803AF9A8
	addu	$3, $12, $13
	l.s	$f0, 8($3)
	l.s	$f4, D_803B97A0
	c.lt.s	$f4, $f0
	move	$5, $0
	bc1f	$41
	li	$5, 1
$41:
	beq	$5, 0, $34
	l.s	$f6, D_803B97A4
	c.lt.s	$f0, $f6
	move	$5, $0
	bc1f	$42
	li	$5, 1
$42:
	bne	$5, 0, $35
$34:
	lbu	$5, 60($3)
	seq	$14, $5, 0
	move	$5, $14
$35:
	move	$4, $9
	sw	$3, 36($sp)
	sw	$8, 68($sp)
	sw	$9, 72($sp)
	sw	$10, 40($sp)
	sw	$11, 64($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$3, 36($sp)
	lw	$8, 68($sp)
	lw	$9, 72($sp)
	lw	$10, 40($sp)
	lw	$11, 64($sp)
	beq	$2, 0, $36
	.alias	$3,$sp
	.loc	2 61
 #  60	        (D_803B97A0<slot->state.depth && slot->state.depth<D_803B97A4) || !slot->state.alpha))
 #  61	        return 1;
	li	$2, 1
	b	$40
$36:
	.loc	2 62
 #  62	    brake_light_update(player,&slot[19].matrix[12],&D_80150B70[player],0,position);
	move	$4, $8
	addu	$5, $3, 1264
	mul	$15, $8, 152
	la	$24, D_80150B70
	addu	$6, $15, $24
	move	$7, $0
	addu	$25, $sp, 52
	sw	$25, 16($sp)
	sw	$3, 36($sp)
	sw	$8, 68($sp)
	sw	$9, 72($sp)
	sw	$10, 40($sp)
	sw	$11, 64($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	lw	$3, 36($sp)
	lw	$8, 68($sp)
	lw	$9, 72($sp)
	lw	$10, 40($sp)
	lw	$11, 64($sp)
	.loc	2 63
 #  63	    blt->X=position[0]-blt->Width/2;
	lh	$12, 52($sp)
	lh	$13, 20($9)
	div	$14, $13, 2
	subu	$15, $12, $14
	sh	$15, 14($9)
	.loc	2 64
 #  64	    blt->Y=position[1]-blt->Height/2;
	lh	$24, 54($sp)
	lh	$25, 22($9)
	div	$13, $25, 2
	subu	$12, $24, $13
	sh	$12, 16($9)
	.loc	2 65
 #  65	    blt->Alpha=slot->state.alpha;
	lbu	$14, 60($3)
	sb	$14, 24($9)
	.loc	2 66
 #  66	    if (marker==7) {
	bne	$11, 7, $37
	.loc	2 66
	.loc	2 67
 #  67	        palette=D_80111611[player][D_803B9FD0[player]];
	mul	$15, $8, 13
	lb	$25, D_803B9FD0($8)
	addu	$24, $15, $25
	lb	$5, D_80111611($24)
	.loc	2 68
 #  68	        channel=0;
	sw	$0, 48($sp)
	sw	$5, 56($sp)
	b	$39
$37:
	.loc	2 69
 #  69	    } else if (marker==8) {
	bne	$10, 512, $38
	.loc	2 69
	.loc	2 70
 #  70	        palette=D_80111655[player][D_803B9FD0[player]];
	mul	$13, $8, 13
	lb	$12, D_803B9FD0($8)
	addu	$14, $13, $12
	lb	$5, D_80111655($14)
	.loc	2 71
 #  71	        channel=1;
	li	$15, 1
	sw	$15, 48($sp)
	sw	$5, 56($sp)
	b	$39
$38:
	.loc	2 72
 #  72	    } else if (marker==9) {
	bne	$10, 576, $39
	.loc	2 72
	.loc	2 73
 #  73	        palette=D_80111699[player][D_803B9FD0[player]];
	mul	$25, $8, 13
	lb	$24, D_803B9FD0($8)
	addu	$13, $25, $24
	lb	$5, D_80111699($13)
	.loc	2 74
 #  74	        channel=2;
	li	$12, 2
	sw	$12, 48($sp)
	sw	$5, 56($sp)
$39:
	lw	$5, 56($sp)
	.loc	2 76
 #  75	    }
 #  76	    D_803B9B60[player][channel].r=D_801226C0[palette].r;
	.noalias	$2,$sp
	mul	$14, $8, 12
	lw	$15, 48($sp)
	mul	$25, $15, 4
	addu	$24, $14, $25
	la	$13, D_803B9B60
	addu	$2, $24, $13
	.noalias	$3,$2
	.noalias	$3,$sp
	mul	$12, $5, 4
	la	$15, D_801226C0
	addu	$3, $12, $15
	lbu	$14, 0($3)
	sb	$14, 0($2)
	.loc	2 77
 #  77	    D_803B9B60[player][channel].g=D_801226C0[palette].g;
	lbu	$25, 1($3)
	sb	$25, 1($2)
	.loc	2 78
 #  78	    D_803B9B60[player][channel].b=D_801226C0[palette].b;
	lbu	$24, 2($3)
	sb	$24, 2($2)
	.loc	2 79
 #  79	    D_803B9B60[player][channel].a=blt->Alpha;
	lbu	$13, 24($9)
	sb	$13, 3($2)
	.loc	2 80
 #  80	    blt->Image=&D_803B9B60[player][channel];
	sw	$2, 4($9)
	.loc	2 81
 #  81	    blt->AnimID=(blt->AnimID&0x0fffffff)|((u32)palette<<28);
	lw	$12, 44($9)
	and	$15, $12, 268435455
	sll	$14, $5, 28
	or	$25, $15, $14
	sw	$25, 44($9)
	.loc	2 82
 #  82	    Input_ApplyPadConfig(blt);
	move	$4, $9
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.alias	$2,$3
	.alias	$2,$sp
	.alias	$3,$sp
	.loc	2 83
 #  83	    return 1;
	li	$2, 1
$40:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 72
	j	$31
	.end	func_803A3A6C
