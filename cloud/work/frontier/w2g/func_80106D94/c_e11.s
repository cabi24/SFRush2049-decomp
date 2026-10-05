	.verstamp	3 19
	.option	pic0
	.extern	D_80146111 1
	.extern	D_80151AD0 2
	.extern	D_8014A110 4
	.extern	D_8015F734 1
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_80106D94
	.loc	2 57
 #  57	s32 func_80106D94(Blit *blt) {
	.ent	func_80106D94 2
func_80106D94:
	.option	O3
	subu	$sp, 64
	sw	$31, 20($sp)
	.mask	0x80000000, -44
	.frame	$sp, 64, $31
	.loc	2 57
	la	$3, D_80152818
	li	$5, 952
	.loc	2 57
	.loc	2 69
 #  69	    slot = (blt->AnimID & 0xF0) >> 4;
	lw	$2, 44($4)
	and	$9, $2, 240
	srl	$14, $9, 4
	move	$9, $14
	.loc	2 70
 #  70	    digit = blt->AnimID & 0xF;
	and	$6, $2, 15
	.loc	2 71
 #  71	    frac = 0.0f;
	li.s	$f12, 0.0
	.loc	2 72
 #  72	    dist = D_80152818[slot].distance;
	.noalias	$3,$sp
	mul	$15, $9, $5
	addu	$24, $3, $15
	.noalias	$24,$sp
	l.s	$f0, 264($24)
	.alias	$24,$sp
	.loc	2 73
 #  73	    if (D_80146111) {
	lb	$25, D_80146111
	beq	$25, 0, $32
	.loc	2 73
	.loc	2 74
 #  74	        dist = dist / 0.6f;
	li.s	$f4, 0.6
	div.s	$f0, $f0, $f4
$32:
	la	$11, D_80151AD0
	.loc	2 76
 #  75	    }
 #  76	    if (!(slot < D_80151AD0) || D_8014A110 == 5) {
	.noalias	$11,$3
	.noalias	$11,$sp
	lh	$12, 0($11)
	bge	$9, $12, $33
	lw	$13, D_8014A110
	bne	$13, 5, $35
	.alias	$3,$11
	.alias	$3,$sp
	.alias	$11,$sp
$33:
	li	$2, 1
	.loc	2 76
	.loc	2 77
 #  77	        blt->AnimDTA = 0;
	sw	$0, 40($4)
	.loc	2 78
 #  78	        if (blt->Hide != 1) {
	lb	$14, 26($4)
	beq	$2, $14, $34
	.loc	2 78
	.loc	2 79
 #  79	            blt->Hide = 1;
	sb	$2, 26($4)
	.loc	2 80
 #  80	            Input_ApplyPadConfig(blt);
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
$34:
	.loc	2 82
 #  81	        }
 #  82	        return 1;
	li	$2, 1
	b	$45
$35:
	.loc	2 84
 #  83	    }
 #  84	    hide = D_8015F734 == 0 || D_80152818[D_8014A250[slot].unk7C6].unkEF == 1;
	lb	$2, D_8015F734
	seq	$15, $2, 0
	move	$2, $15
	bne	$2, 0, $36
	.noalias	$3,$sp
	mul	$24, $9, 2056
	lh	$25, D_8014A250+1990($24)
	mul	$12, $25, $5
	addu	$13, $3, $12
	.noalias	$13,$sp
	lb	$2, 239($13)
	.alias	$13,$sp
	seq	$14, $2, 1
	move	$2, $14
	.alias	$3,$sp
$36:
	.loc	2 85
 #  85	    if (hide != blt->Hide) {
	lb	$3, 26($4)
	beq	$2, $3, $37
	.loc	2 85
	.loc	2 86
 #  86	        blt->Hide = hide;
	sb	$2, 26($4)
	.loc	2 87
 #  87	        Input_ApplyPadConfig(blt);
	sw	$4, 64($sp)
	sw	$6, 56($sp)
	sw	$9, 60($sp)
	s.s	$f0, 32($sp)
	s.s	$f12, 28($sp)
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	lw	$4, 64($sp)
	lw	$6, 56($sp)
	lw	$9, 60($sp)
	la	$11, D_80151AD0
	l.s	$f0, 32($sp)
	l.s	$f12, 28($sp)
	lb	$3, 26($4)
$37:
	.loc	2 89
 #  88	    }
 #  89	    hide = blt->Hide;
	move	$2, $3
	.loc	2 90
 #  90	    if (hide) {
	beq	$2, 0, $38
	.loc	2 90
	.loc	2 91
 #  91	        return 1;
	li	$2, 1
	b	$45
$38:
	li	$5, 10
	li	$8, 9
	.loc	2 93
 #  92	    }
 #  93	    tenths = dist / 528.0f;
	.loc	2 94
 #  94	    itenths = tenths;
	.loc	2 95
 #  95	    if (itenths % 10 == 9) {
	li.s	$f6, 528.0
	div.s	$f2, $f0, $f6
	mul	$2, $9, 8
	trunc.w.s	$f8, $f2, $15
	mfc1	$10, $f8
	rem	$7, $10, $5
	bne	$8, $7, $39
	.loc	2 95
	.loc	2 96
 #  96	        frac = tenths - itenths;
	mtc1	$10, $f10
	cvt.s.w	$f16, $f10
	sub.s	$f12, $f2, $f16
$39:
	.loc	2 98
 #  97	    }
 #  98	    switch (digit) {
	beq	$6, 0, $40
	beq	$6, 1, $41
	beq	$6, 2, $42
	beq	$6, 3, $43
	lh	$3, 20($4)
	div	$24, $3, 2
	move	$3, $24
	b	$44
$40:
	la	$6, D_80115AE8
	.loc	2 100
 #  99	    case 0:
 # 100	        w = blt->Width / 2;
	lh	$3, 20($4)
	div	$25, $3, 2
	move	$3, $25
	.loc	2 101
 # 101	        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x - w * 2;
	.noalias	$6,$sp
	.noalias	$11,$6
	.noalias	$11,$sp
	lh	$12, 0($11)
	mul	$13, $12, 32
	addu	$14, $6, $13
	addu	$15, $14, $2
	lw	$24, -32($15)
	mul	$25, $3, 2
	subu	$12, $24, $25
	sh	$12, 14($4)
	.loc	2 102
 # 102	        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
	lh	$13, 0($11)
	mul	$14, $13, 32
	addu	$15, $6, $14
	addu	$24, $15, $2
	lw	$25, -28($24)
	sh	$25, 16($4)
	.loc	2 103
 # 103	        blt->Left = 0;
	sh	$0, 32($4)
	.loc	2 104
 # 104	        blt->Top = ((s32) (dist / 528000.0f) % 10) * w;
	li.s	$f18, 528000.0
	div.s	$f4, $f0, $f18
	trunc.w.s	$f6, $f4, $12
	mfc1	$13, $f6
	rem	$14, $13, $5
	mul	$15, $14, $3
	sh	$15, 28($4)
	.loc	2 105
 # 105	        if ((s32) (dist / 52800.0f) % 10 == 9 && (s32) (dist / 5280.0f) % 10 == 9 && itenths % 10 == 9) {
	li.s	$f8, 52800.0
	div.s	$f10, $f0, $f8
	trunc.w.s	$f16, $f10, $24
	mfc1	$25, $f16
	rem	$12, $25, $5
	bne	$8, $12, $44
	.alias	$6,$11
	.alias	$6,$sp
	.alias	$11,$sp
	li.s	$f18, 5280.0
	div.s	$f4, $f0, $f18
	trunc.w.s	$f6, $f4, $13
	mfc1	$14, $f6
	rem	$15, $14, $5
	bne	$8, $15, $44
	bne	$8, $7, $44
	.loc	2 105
	.loc	2 106
 # 106	            blt->Top = blt->Top + frac * w;
	lh	$24, 28($4)
	mtc1	$24, $f8
	cvt.s.w	$f10, $f8
	mtc1	$3, $f16
	cvt.s.w	$f18, $f16
	mul.s	$f4, $f12, $f18
	add.s	$f6, $f10, $f4
	trunc.w.s	$f8, $f6, $25
	mfc1	$12, $f8
	sh	$12, 28($4)
	.loc	2 108
 # 107	        }
 # 108	        break;
	b	$44
$41:
	la	$6, D_80115AE8
	.loc	2 110
 # 109	    case 1:
 # 110	        w = blt->Width / 2;
	lh	$3, 20($4)
	div	$13, $3, 2
	move	$3, $13
	.loc	2 111
 # 111	        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x - w;
	.noalias	$6,$sp
	.noalias	$11,$6
	.noalias	$11,$sp
	lh	$14, 0($11)
	mul	$15, $14, 32
	addu	$24, $6, $15
	addu	$25, $24, $2
	lw	$12, -32($25)
	subu	$13, $12, $3
	sh	$13, 14($4)
	.loc	2 112
 # 112	        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
	lh	$14, 0($11)
	mul	$15, $14, 32
	addu	$24, $6, $15
	addu	$25, $24, $2
	lw	$12, -28($25)
	sh	$12, 16($4)
	.loc	2 113
 # 113	        blt->Left = 0;
	sh	$0, 32($4)
	.loc	2 114
 # 114	        blt->Top = ((s32) (dist / 52800.0f) % 10) * w;
	li.s	$f16, 52800.0
	div.s	$f18, $f0, $f16
	trunc.w.s	$f10, $f18, $13
	mfc1	$14, $f10
	rem	$15, $14, $5
	mul	$24, $15, $3
	sh	$24, 28($4)
	.loc	2 115
 # 115	        if ((s32) (dist / 5280.0f) % 10 == 9 && itenths % 10 == 9) {
	li.s	$f4, 5280.0
	div.s	$f6, $f0, $f4
	trunc.w.s	$f8, $f6, $25
	mfc1	$12, $f8
	rem	$13, $12, $5
	bne	$8, $13, $44
	.alias	$6,$11
	.alias	$6,$sp
	.alias	$11,$sp
	bne	$8, $7, $44
	.loc	2 115
	.loc	2 116
 # 116	            blt->Top = blt->Top + frac * w;
	lh	$14, 28($4)
	mtc1	$14, $f16
	cvt.s.w	$f18, $f16
	mtc1	$3, $f10
	cvt.s.w	$f4, $f10
	mul.s	$f6, $f12, $f4
	add.s	$f8, $f18, $f6
	trunc.w.s	$f16, $f8, $15
	mfc1	$24, $f16
	sh	$24, 28($4)
	.loc	2 118
 # 117	        }
 # 118	        break;
	b	$44
$42:
	la	$6, D_80115AE8
	.loc	2 120
 # 119	    case 2:
 # 120	        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x;
	.noalias	$6,$sp
	.noalias	$11,$6
	.noalias	$11,$sp
	lh	$25, 0($11)
	mul	$12, $25, 32
	addu	$13, $6, $12
	addu	$14, $13, $2
	lw	$15, -32($14)
	sh	$15, 14($4)
	.loc	2 121
 # 121	        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
	lh	$24, 0($11)
	mul	$25, $24, 32
	addu	$12, $6, $25
	addu	$13, $12, $2
	lw	$14, -28($13)
	sh	$14, 16($4)
	.loc	2 122
 # 122	        blt->Left = 0;
	sh	$0, 32($4)
	.loc	2 123
 # 123	        w = blt->Width / 2;
	lh	$3, 20($4)
	div	$15, $3, 2
	move	$3, $15
	.loc	2 124
 # 124	        blt->Top = ((s32) (dist / 5280.0f) % 10) * w;
	li.s	$f10, 5280.0
	div.s	$f4, $f0, $f10
	trunc.w.s	$f18, $f4, $24
	mfc1	$25, $f18
	rem	$12, $25, $5
	mul	$13, $12, $3
	sh	$13, 28($4)
	.loc	2 125
 # 125	        if (itenths % 10 == 9) {
	bne	$8, $7, $44
	.alias	$6,$11
	.alias	$6,$sp
	.alias	$11,$sp
	.loc	2 125
	.loc	2 126
 # 126	            blt->Top = blt->Top + frac * w;
	lh	$14, 28($4)
	mtc1	$14, $f6
	cvt.s.w	$f8, $f6
	mtc1	$3, $f16
	cvt.s.w	$f10, $f16
	mul.s	$f4, $f12, $f10
	add.s	$f18, $f8, $f4
	trunc.w.s	$f6, $f18, $15
	mfc1	$24, $f6
	sh	$24, 28($4)
	.loc	2 128
 # 127	        }
 # 128	        break;
	b	$44
$43:
	la	$6, D_80115AE8
	.loc	2 130
 # 129	    case 3:
 # 130	        w = blt->Width / 2;
	lh	$3, 20($4)
	div	$25, $3, 2
	move	$3, $25
	.loc	2 131
 # 131	        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x + w;
	.noalias	$6,$sp
	.noalias	$11,$6
	.noalias	$11,$sp
	lh	$12, 0($11)
	mul	$13, $12, 32
	addu	$14, $6, $13
	addu	$15, $14, $2
	lw	$24, -32($15)
	addu	$25, $24, $3
	sh	$25, 14($4)
	.loc	2 132
 # 132	        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
	lh	$12, 0($11)
	mul	$13, $12, 32
	addu	$14, $6, $13
	addu	$15, $14, $2
	lw	$24, -28($15)
	sh	$24, 16($4)
	.loc	2 133
 # 133	        blt->Left = w;
	sh	$3, 32($4)
	.loc	2 134
 # 134	        blt->Top = w * (itenths % 10 + tenths - itenths);
	mtc1	$7, $f16
	cvt.s.w	$f10, $f16
	add.s	$f8, $f10, $f2
	mtc1	$10, $f4
	cvt.s.w	$f18, $f4
	sub.s	$f6, $f8, $f18
	mtc1	$3, $f16
	cvt.s.w	$f10, $f16
	mul.s	$f4, $f10, $f6
	trunc.w.s	$f8, $f4, $25
	mfc1	$12, $f8
	sh	$12, 28($4)
	.loc	2 135
 # 135	        break;
	.alias	$6,$11
	.alias	$6,$sp
	.alias	$11,$sp
$44:
	.loc	2 140
 # 136	    default:
 # 137	        w = blt->Width / 2;
 # 138	        break;
 # 139	    }
 # 140	    blt->Right = blt->Left + w - 1;
	lh	$13, 32($4)
	addu	$14, $13, $3
	addu	$15, $14, -1
	sh	$15, 34($4)
	.loc	2 141
 # 141	    blt->Bot = blt->Top + w - 1;
	lh	$24, 28($4)
	addu	$25, $24, $3
	addu	$12, $25, -1
	sh	$12, 30($4)
	.loc	2 142
 # 142	    Input_ApplyPadConfig(blt);
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.loc	2 143
 # 143	    return 1;
	li	$2, 1
$45:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 64
	j	$31
	.end	func_80106D94
