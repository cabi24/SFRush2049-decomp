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
	.loc	2 93
 #  93	void standin_ad650(void) { func_800AD650(D_standin_a, D_standin_b); }
	.ent	standin_ad650 2
standin_ad650:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	2 93
	.loc	2 93
	.loc	2 93
	la	$5, D_standin_a
	la	$4, D_standin_b
	.livereg	0x0C00000E,0x00000000
	jal	func_800AD650
	.loc	2 93
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
	subu	$sp, 216
	sw	$31, 68($sp)
	sw	$30, 64($sp)
	sw	$23, 60($sp)
	sw	$22, 56($sp)
	sw	$21, 52($sp)
	sw	$20, 48($sp)
	sw	$19, 44($sp)
	sw	$18, 40($sp)
	sw	$17, 36($sp)
	sw	$16, 32($sp)
	s.d	$f20, 24($sp)
	.mask	0xC0FF0000, -148
	.fmask	0x00300000, -192
	.frame	$sp, 216, $31
	.loc	2 53
	move	$21, $4
	sw	$5, 220($sp)
	move	$22, $6
	move	$23, $7
	.loc	2 53
	.loc	2 68
 #  68	    count = 0;
	move	$20, $0
	.loc	2 69
 #  69	    item = D_801525EC;
	lw	$16, D_801525EC
	.loc	2 70
 #  70	    for (i = 0; i < D_8015267C; i++, item++) {
	move	$17, $0
	lhu	$3, D_8015267C
	bltu	$3, 1, $34
	li.s	$f20, 16384.0
	la	$30, D_801497F8
	addu	$19, $sp, 96
	addu	$18, $sp, 132
$32:
	.loc	2 70
	.loc	2 71
 #  71	        if (tag == item->tag) {
	lhu	$14, 0($16)
	bne	$21, $14, $33
	.loc	2 71
	.loc	2 72
 #  72	            count++;
	addu	$20, $20, 1
	.loc	2 73
 #  73	            func_800AD650(item->rot, (f32 *) mat);
	addu	$5, $16, 4
	move	$4, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_800AD650
	.loc	2 74
 #  74	            func_800BF780(m1, mat, tmp);
	move	$4, $22
	move	$5, $18
	move	$6, $19
	.livereg	0x0E00000E,0x00000000
	jal	func_800BF780
	.loc	2 75
 #  75	            func_800BF780(m2, tmp, mat);
	move	$4, $23
	move	$5, $19
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	func_800BF780
	.loc	2 76
 #  76	            out = D_801497F8[item->matrix].rot;
	.noalias	$30,$sp
	lw	$15, 0($30)
	lhu	$24, 2($16)
	mul	$25, $24, 24
	addu	$2, $15, $25
	addu	$2, $2, 4
	.loc	2 77
 #  77	            out[0] = mat[0][0] * 16384.0f;
	l.s	$f4, 132($sp)
	mul.s	$f6, $f4, $f20
	trunc.w.s	$f8, $f6, $14
	mfc1	$24, $f8
	sh	$24, 0($2)
	.loc	2 78
 #  78	            out[1] = mat[0][1] * 16384.0f;
	l.s	$f10, 136($sp)
	mul.s	$f4, $f10, $f20
	trunc.w.s	$f6, $f4, $15
	mfc1	$25, $f6
	sh	$25, 2($2)
	.loc	2 79
 #  79	            out[2] = mat[0][2] * 16384.0f;
	l.s	$f8, 140($sp)
	mul.s	$f10, $f8, $f20
	trunc.w.s	$f4, $f10, $14
	mfc1	$24, $f4
	sh	$24, 4($2)
	.loc	2 80
 #  80	            out[3] = mat[1][0] * 16384.0f;
	l.s	$f6, 144($sp)
	mul.s	$f8, $f6, $f20
	trunc.w.s	$f10, $f8, $15
	mfc1	$25, $f10
	sh	$25, 6($2)
	.loc	2 81
 #  81	            out[4] = mat[1][1] * 16384.0f;
	l.s	$f4, 148($sp)
	mul.s	$f6, $f4, $f20
	trunc.w.s	$f8, $f6, $14
	mfc1	$24, $f8
	sh	$24, 8($2)
	.loc	2 82
 #  82	            out[5] = mat[1][2] * 16384.0f;
	l.s	$f10, 152($sp)
	mul.s	$f4, $f10, $f20
	trunc.w.s	$f6, $f4, $15
	mfc1	$25, $f6
	sh	$25, 10($2)
	.loc	2 83
 #  83	            out[6] = mat[2][0] * 16384.0f;
	l.s	$f8, 156($sp)
	mul.s	$f10, $f8, $f20
	trunc.w.s	$f4, $f10, $14
	mfc1	$24, $f4
	sh	$24, 12($2)
	.loc	2 84
 #  84	            out[7] = mat[2][1] * 16384.0f;
	l.s	$f6, 160($sp)
	mul.s	$f8, $f6, $f20
	trunc.w.s	$f10, $f8, $15
	mfc1	$25, $f10
	sh	$25, 14($2)
	.loc	2 85
 #  85	            out[8] = mat[2][2] * 16384.0f;
	l.s	$f4, 164($sp)
	mul.s	$f6, $f4, $f20
	trunc.w.s	$f8, $f6, $14
	mfc1	$24, $f8
	sh	$24, 16($2)
	lhu	$3, D_8015267C
$33:
	.loc	2 70
 #  70	    for (i = 0; i < D_8015267C; i++, item++) {
	addu	$17, $17, 1
	addu	$16, $16, 32
	bltu	$17, $3, $32
	.alias	$30,$sp
$34:
	.loc	2 88
 #  88	    if (count == 0) {
	.loc	2 88
	.loc	2 90
 #  89	    }
 #  90	}
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 24($sp)
	lw	$16, 32($sp)
	lw	$17, 36($sp)
	lw	$18, 40($sp)
	lw	$19, 44($sp)
	lw	$20, 48($sp)
	lw	$21, 52($sp)
	lw	$22, 56($sp)
	lw	$23, 60($sp)
	lw	$31, 68($sp)
	lw	$30, 64($sp)
	addu	$sp, 216
	j	$31
	.end	camera_first_person
