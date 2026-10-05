	.verstamp	3 19
	.option	pic0
	.extern	D_801525EC 4
	.extern	D_8015267C 2
	.extern	D_801497F8 4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 38
 #  38	void func_800AD650(s16 *p, f32 *o) {
	.ent	func_800AD650 2
func_800AD650:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 38
	li.s	$f0, 0.00006103515625
	.loc	2 38
	.loc	2 39
 #  39	    o[0] = (f32) p[0] * 0.00006103515625f;
	lh	$14, 0($5)
	mtc1	$14, $f4
	cvt.s.w	$f6, $f4
	mul.s	$f8, $f6, $f0
	s.s	$f8, 0($4)
	.loc	2 40
 #  40	    o[1] = (f32) p[1] * 0.00006103515625f;
	lh	$15, 2($5)
	mtc1	$15, $f10
	cvt.s.w	$f4, $f10
	mul.s	$f6, $f4, $f0
	s.s	$f6, 4($4)
	.loc	2 41
 #  41	    o[2] = (f32) p[2] * 0.00006103515625f;
	lh	$24, 4($5)
	mtc1	$24, $f8
	cvt.s.w	$f10, $f8
	mul.s	$f4, $f10, $f0
	s.s	$f4, 8($4)
	.loc	2 42
 #  42	    o[3] = (f32) p[3] * 0.00006103515625f;
	lh	$25, 6($5)
	mtc1	$25, $f6
	cvt.s.w	$f8, $f6
	mul.s	$f10, $f8, $f0
	s.s	$f10, 12($4)
	.loc	2 43
 #  43	    o[4] = (f32) p[4] * 0.00006103515625f;
	lh	$14, 8($5)
	mtc1	$14, $f4
	cvt.s.w	$f6, $f4
	mul.s	$f8, $f6, $f0
	s.s	$f8, 16($4)
	.loc	2 44
 #  44	    o[5] = (f32) p[5] * 0.00006103515625f;
	lh	$15, 10($5)
	mtc1	$15, $f10
	cvt.s.w	$f4, $f10
	mul.s	$f6, $f4, $f0
	s.s	$f6, 20($4)
	.loc	2 45
 #  45	    o[6] = (f32) p[6] * 0.00006103515625f;
	lh	$24, 12($5)
	mtc1	$24, $f8
	cvt.s.w	$f10, $f8
	mul.s	$f4, $f10, $f0
	s.s	$f4, 24($4)
	.loc	2 46
 #  46	    o[7] = (f32) p[7] * 0.00006103515625f;
	lh	$25, 14($5)
	mtc1	$25, $f6
	cvt.s.w	$f8, $f6
	mul.s	$f10, $f8, $f0
	s.s	$f10, 28($4)
	.loc	2 47
 #  47	    o[8] = (f32) p[8] * 0.00006103515625f;
	lh	$14, 16($5)
	mtc1	$14, $f4
	cvt.s.w	$f6, $f4
	mul.s	$f8, $f6, $f0
	s.s	$f8, 32($4)
	.loc	2 48
 #  48	}
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	func_800AD650
	.text	
	.align	2
	.file	2 "g.c"
	.globl	standin_ad650
	.loc	2 105
 # 105	void standin_ad650(void) { func_800AD650(D_standin_a, D_standin_b); }
	.ent	standin_ad650 2
standin_ad650:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	2 105
	.loc	2 105
	.loc	2 105
	la	$5, D_standin_a
	la	$4, D_standin_b
	.livereg	0x0C00000E,0x00000000
	jal	func_800AD650
	.loc	2 105
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	standin_ad650
	.text	
	.align	2
	.file	2 "g.c"
	.globl	camera_first_person
	.loc	2 53
 #  53	void camera_first_person(s32 tag, f32 *origin, f32 m1[3][3], f32 m2[3][3]) {
	.ent	camera_first_person 2
camera_first_person:
	.option	O3
	subu	$sp, 232
	sw	$31, 84($sp)
	sw	$30, 80($sp)
	sw	$23, 76($sp)
	sw	$22, 72($sp)
	sw	$21, 68($sp)
	sw	$20, 64($sp)
	sw	$19, 60($sp)
	sw	$18, 56($sp)
	sw	$17, 52($sp)
	sw	$16, 48($sp)
	s.d	$f24, 40($sp)
	s.d	$f22, 32($sp)
	s.d	$f20, 24($sp)
	.mask	0xC0FF0000, -148
	.fmask	0x03F00000, -192
	.frame	$sp, 232, $31
	.loc	2 53
	sw	$4, 232($sp)
	move	$17, $5
	move	$30, $6
	sw	$7, 244($sp)
	.loc	2 53
	.loc	2 68
 #  68	    count = 0;
	move	$23, $0
	.loc	2 69
 #  69	    item = D_801525EC;
	lw	$16, D_801525EC
	.loc	2 70
 #  70	    for (i = 0; i < D_8015267C; i++, item++) {
	move	$19, $0
	lhu	$2, D_8015267C
	bltu	$2, 1, $34
	li.s	$f24, 32.0
	li.s	$f22, 0.03125
	li.s	$f20, 16384.0
	addu	$22, $sp, 196
	addu	$21, $sp, 184
	addu	$20, $sp, 112
	addu	$18, $sp, 148
$32:
	.loc	2 70
	.loc	2 71
 #  71	        if (tag == item->tag) {
	lw	$14, 232($sp)
	lhu	$15, 0($16)
	bne	$14, $15, $33
	.loc	2 71
	.loc	2 72
 #  72	            count++;
	addu	$23, $23, 1
	.loc	2 73
 #  73	            func_800AD650(item->rot, (f32 *) mat);
	addu	$5, $16, 4
	move	$4, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_800AD650
	.loc	2 74
 #  74	            func_800BF780(m1, mat, tmp);
	move	$4, $30
	move	$5, $18
	move	$6, $20
	.livereg	0x0E00000E,0x00000000
	jal	func_800BF780
	.loc	2 75
 #  75	            func_800BF780(m2, tmp, mat);
	lw	$4, 244($sp)
	move	$5, $20
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	func_800BF780
	.loc	2 76
 #  76	            out = D_801497F8[item->matrix].rot;
	lw	$24, D_801497F8
	lhu	$25, 2($16)
	mul	$14, $25, 24
	addu	$2, $24, $14
	addu	$2, $2, 4
	.loc	2 77
 #  77	            out[0] = mat[0][0] * 16384.0f;
	l.s	$f4, 148($sp)
	mul.s	$f6, $f4, $f20
	trunc.w.s	$f8, $f6, $15
	mfc1	$25, $f8
	sh	$25, 0($2)
	.loc	2 78
 #  78	            out[1] = mat[0][1] * 16384.0f;
	l.s	$f10, 152($sp)
	mul.s	$f4, $f10, $f20
	trunc.w.s	$f6, $f4, $24
	mfc1	$14, $f6
	sh	$14, 2($2)
	.loc	2 79
 #  79	            out[2] = mat[0][2] * 16384.0f;
	l.s	$f8, 156($sp)
	mul.s	$f10, $f8, $f20
	trunc.w.s	$f4, $f10, $15
	mfc1	$25, $f4
	sh	$25, 4($2)
	.loc	2 80
 #  80	            out[3] = mat[1][0] * 16384.0f;
	l.s	$f6, 160($sp)
	mul.s	$f8, $f6, $f20
	trunc.w.s	$f10, $f8, $24
	mfc1	$14, $f10
	sh	$14, 6($2)
	.loc	2 81
 #  81	            out[4] = mat[1][1] * 16384.0f;
	l.s	$f4, 164($sp)
	mul.s	$f6, $f4, $f20
	trunc.w.s	$f8, $f6, $15
	mfc1	$25, $f8
	sh	$25, 8($2)
	.loc	2 82
 #  82	            out[5] = mat[1][2] * 16384.0f;
	l.s	$f10, 168($sp)
	mul.s	$f4, $f10, $f20
	trunc.w.s	$f6, $f4, $24
	mfc1	$14, $f6
	sh	$14, 10($2)
	.loc	2 83
 #  83	            out[6] = mat[2][0] * 16384.0f;
	l.s	$f8, 172($sp)
	mul.s	$f10, $f8, $f20
	trunc.w.s	$f4, $f10, $15
	mfc1	$25, $f4
	sh	$25, 12($2)
	.loc	2 84
 #  84	            out[7] = mat[2][1] * 16384.0f;
	l.s	$f6, 176($sp)
	mul.s	$f8, $f6, $f20
	trunc.w.s	$f10, $f8, $24
	mfc1	$14, $f10
	sh	$14, 14($2)
	.loc	2 85
 #  85	            out[8] = mat[2][2] * 16384.0f;
	l.s	$f4, 180($sp)
	mul.s	$f6, $f4, $f20
	trunc.w.s	$f8, $f6, $15
	mfc1	$25, $f8
	sh	$25, 16($2)
	.loc	2 86
 #  86	            pos[0] = ((item->pos[0] << 5) + ((item->frac & 0x7C00) >> 10)) * 0.03125f;
	lh	$24, 24($16)
	sll	$14, $24, 5
	lhu	$15, 30($16)
	and	$25, $15, 31744
	sra	$24, $25, 10
	addu	$15, $14, $24
	mtc1	$15, $f10
	cvt.s.w	$f4, $f10
	mul.s	$f6, $f4, $f22
	s.s	$f6, 196($sp)
	.loc	2 87
 #  87	            pos[1] = ((item->pos[1] << 5) + ((item->frac & 0x3E0) >> 5)) * 0.03125f;
	lh	$25, 26($16)
	sll	$14, $25, 5
	lhu	$24, 30($16)
	and	$15, $24, 992
	sra	$25, $15, 5
	addu	$24, $14, $25
	mtc1	$24, $f8
	cvt.s.w	$f10, $f8
	mul.s	$f4, $f10, $f22
	s.s	$f4, 200($sp)
	.loc	2 88
 #  88	            pos[2] = ((item->pos[2] << 5) + (item->frac & 0x1F)) * 0.03125f;
	lh	$15, 28($16)
	sll	$14, $15, 5
	lhu	$25, 30($16)
	and	$24, $25, 31
	addu	$15, $14, $24
	mtc1	$15, $f8
	cvt.s.w	$f10, $f8
	mul.s	$f8, $f10, $f22
	s.s	$f8, 204($sp)
	.loc	2 89
 #  89	            rel[0] = pos[0] - origin[0];
	l.s	$f10, 0($17)
	sub.s	$f6, $f6, $f10
	s.s	$f6, 184($sp)
	.loc	2 90
 #  90	            rel[1] = pos[1] - origin[1];
	l.s	$f10, 4($17)
	sub.s	$f6, $f4, $f10
	s.s	$f6, 188($sp)
	.loc	2 91
 #  91	            rel[2] = pos[2] - origin[2];
	l.s	$f4, 8($17)
	sub.s	$f10, $f8, $f4
	s.s	$f10, 192($sp)
	.loc	2 92
 #  92	            func_8009E820(rel, pos, (f32 *) m1);
	move	$4, $21
	move	$5, $22
	move	$6, $30
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
	.loc	2 93
 #  93	            func_8009E820(pos, rel, (f32 *) m2);
	move	$4, $22
	move	$5, $21
	lw	$6, 244($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
	.loc	2 94
 #  94	            vecadd(rel, origin, pos);
	.loc	2 94
	l.s	$f6, 0($17)
	l.s	$f8, 184($sp)
	add.s	$f4, $f6, $f8
	s.s	$f4, 196($sp)
	.loc	2 94
	l.s	$f10, 4($17)
	l.s	$f6, 188($sp)
	add.s	$f8, $f10, $f6
	s.s	$f8, 200($sp)
	.loc	2 94
	l.s	$f4, 8($17)
	l.s	$f10, 192($sp)
	add.s	$f6, $f4, $f10
	s.s	$f6, 204($sp)
	.loc	2 94
	.loc	2 95
 #  95	            ipos[0] = pos[0] * 32.0f;
	l.s	$f8, 196($sp)
	mul.s	$f4, $f8, $f24
	trunc.w.s	$f10, $f4, $25
	mfc1	$14, $f10
	sw	$14, 216($sp)
	.loc	2 96
 #  96	            ipos[1] = pos[1] * 32.0f;
	l.s	$f6, 200($sp)
	mul.s	$f8, $f6, $f24
	trunc.w.s	$f4, $f8, $24
	mfc1	$15, $f4
	sw	$15, 220($sp)
	.loc	2 97
 #  97	            ipos[2] = pos[2] * 32.0f;
	l.s	$f10, 204($sp)
	mul.s	$f6, $f10, $f24
	trunc.w.s	$f8, $f6, $25
	mfc1	$14, $f8
	sw	$14, 224($sp)
	lhu	$2, D_8015267C
$33:
	.loc	2 70
 #  70	    for (i = 0; i < D_8015267C; i++, item++) {
	addu	$19, $19, 1
	addu	$16, $16, 32
	bltu	$19, $2, $32
$34:
	.loc	2 100
 # 100	    if (count == 0) {
	.loc	2 100
	.loc	2 102
 # 101	    }
 # 102	}
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 24($sp)
	l.d	$f22, 32($sp)
	l.d	$f24, 40($sp)
	lw	$16, 48($sp)
	lw	$17, 52($sp)
	lw	$18, 56($sp)
	lw	$19, 60($sp)
	lw	$20, 64($sp)
	lw	$21, 68($sp)
	lw	$22, 72($sp)
	lw	$23, 76($sp)
	lw	$31, 84($sp)
	lw	$30, 80($sp)
	addu	$sp, 232
	j	$31
	.end	camera_first_person
