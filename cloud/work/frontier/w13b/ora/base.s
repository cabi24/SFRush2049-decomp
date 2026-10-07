	.text
	.globl	func_800E05F0
	.ent	func_800E05F0 2
func_800E05F0:
	.option	O3
	subu	$sp, 224
	sw	$31, 132($sp)
	sw	$30, 128($sp)
	sw	$23, 124($sp)
	sw	$22, 120($sp)
	sw	$21, 116($sp)
	sw	$20, 112($sp)
	sw	$19, 108($sp)
	sw	$18, 104($sp)
	sw	$17, 100($sp)
	sw	$16, 96($sp)
	s.d	$f30, 88($sp)
	s.d	$f28, 80($sp)
	s.d	$f26, 72($sp)
	s.d	$f24, 64($sp)
	s.d	$f22, 56($sp)
	s.d	$f20, 48($sp)
	.mask	0xC0FF0000, -92
	.fmask	0xFFF00000, -136
	.frame	$sp, 224, $31
	.loc	6524 301
	move	$19, $4
	.loc	6524 301
	.loc	6524 302
	lh	$3, 1990($19)
	.loc	6524 311
	lb	$14, D_8010FFC0
	beq	$14, 0, $795
	.loc	6524 311
	.loc	6524 312
	.noalias	$2,$sp
	la	$15, D_8010FFCC
	addu	$2, $3, $15
	lb	$24, 0($2)
	beq	$24, 0, $777
	.loc	6524 312
	.loc	6524 313
	sb	$0, 0($2)
	.loc	6524 314
	b	$795
$777:
	.loc	6524 316
	li	$25, 1
	sb	$25, 0($2)
	.loc	6524 317
	lb	$14, D_8010FFC4($3)
	beq	$14, 0, $795
	.alias	$2,$sp
	.loc	6524 317
	.loc	6524 318
	sw	$19, 0($sp)
	sw	$3, 220($sp)
	sw	$19, 224($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_800E0050
	lw	$19, 224($sp)
	.loc	6524 319
	lh	$9, 1990($19)
	move	$10, $9
	.loc	6524 320
	lb	$7, 1996($19)
	bne	$7, 2, $791
	.loc	6524 320
	.loc	6524 321
	li.s	$f28, 0.5
	.loc	6524 321
	li.s	$f14, 0.0
	.loc	6524 322
	move	$3, $0
	li	$5, 4
	move	$4, $19
	li.s	$f18, 1.0000000000000000e+00
	li.s	$f16, 5.0E-1
	la	$6, D_8011F060
$778:
	.loc	6524 322
	.loc	6524 323
	lhu	$15, 1564($4)
	bne	$15, 0, $780
	.loc	6524 323
	.loc	6524 324
	mul	$2, $3, 4
	.noalias	$6,$sp
	addu	$24, $6, $2
	.noalias	$24,$sp
	l.s	$f0, 0($24)
	.alias	$24,$sp
	addu	$25, $19, $2
	l.s	$f4, 2040($25)
	mul.s	$f12, $f0, $f4
	.loc	6524 325
	mul.s	$f6, $f0, $f16
	sub.s	$f8, $f12, $f6
	add.s	$f2, $f8, $f18
	.loc	6524 326
	c.lt.s	$f28, $f2
	bc1f	$779
	mov.s	$f28, $f2
$779:
	.loc	6524 327
	.loc	6524 328
	li.s	$f10, 0.6
	mul.s	$f0, $f12, $f10
	c.lt.s	$f14, $f0
	bc1f	$780
	mov.s	$f14, $f0
$780:
	.loc	6524 322
	addu	$3, $3, 1
	addu	$4, $4, 2
	bne	$3, $5, $778
	.alias	$6,$sp
	.loc	6524 331
	.loc	6524 332
	.noalias	$8,$sp
	mul	$14, $10, 20
	la	$15, D_80140640
	addu	$8, $14, $15
	lw	$4, 0($8)
	bne	$4, -1, $782
	li.s	$f4, 0.0
	c.lt.s	$f4, $f14
	bc1f	$782
	.loc	6524 332
	.loc	6524 333
	bne	$7, 2, $781
	.loc	6524 333
	.loc	6524 334
	li	$4, 36
	move	$5, $9
	li	$6, 2
	li	$7, 2
	sw	$8, 140($sp)
	s.s	$f14, 180($sp)
	.livereg	0x0F00000E,0x00000000
	jal	frame_sync
	lw	$8, 140($sp)
	l.s	$f14, 180($sp)
	sw	$2, 0($8)
	lb	$7, 1996($19)
	lw	$4, 0($8)
	b	$782
$781:
	.loc	6524 335
	.loc	6524 336
	addu	$4, $19, 556
	la	$5, D_801141B0
	li.s	$6, 400.0
	li.s	$7, 0.0
	li.s	$f6, 1.0
	s.s	$f6, 16($sp)
	li.s	$f8, 0.0
	s.s	$f8, 20($sp)
	li	$24, 36
	sw	$24, 24($sp)
	sw	$10, 28($sp)
	sw	$0, 32($sp)
	li	$25, 130
	sw	$25, 36($sp)
	sw	$8, 140($sp)
	s.s	$f14, 180($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_target_track
	lw	$8, 140($sp)
	l.s	$f14, 180($sp)
	sw	$2, 0($8)
	lb	$7, 1996($19)
	lw	$4, 0($8)
$782:
	.loc	6524 339
	beq	$4, -1, $791
	.loc	6524 339
	.loc	6524 340
	li.s	$f10, 0.0
	c.le.s	$f14, $f10
	bc1f	$785
	.loc	6524 340
	.loc	6524 341
	bne	$7, 2, $783
	.loc	6524 341
	sw	$8, 140($sp)
	.livereg	0x0800000E,0x00000000
	jal	scheduler_recv
	lw	$8, 140($sp)
	b	$784
$783:
	.loc	6524 342
	sw	$8, 140($sp)
	.livereg	0x0800000E,0x00000000
	jal	results_screen_update
	lw	$8, 140($sp)
$784:
	.loc	6524 343
	move	$4, $8
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	.alias	$8,$sp
	lb	$7, 1996($19)
	b	$791
$785:
	.loc	6524 344
	bne	$7, 2, $788
	.loc	6524 344
	.loc	6524 345
	l.s	$f4, 4($8)
	c.eq.s	$f14, $f4
	bc1t	$786
	.loc	6524 345
	.loc	6524 346
	s.s	$f14, 4($8)
	.loc	6524 347
	mfc1	$5, $f14
	sw	$8, 140($sp)
	.livereg	0x0C00000E,0x00000000
	jal	client_sync
	lw	$8, 140($sp)
$786:
	.loc	6524 349
	l.s	$f6, 8($8)
	c.eq.s	$f28, $f6
	bc1t	$787
	.loc	6524 349
	.loc	6524 350
	s.s	$f28, 8($8)
	.loc	6524 351
	lw	$14, 0($8)
	sw	$14, 168($sp)
	la	$4, D_80142728
	move	$5, $0
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	li.s	$f12, -2.0000000000000000e+00
	lw	$4, 168($sp)
	mov.s	$f22, $f12
	mov.s	$f24, $f12
	mov.s	$f26, $f12
	.livereg	0x0800000E,0x000002A8
	jal	entity_transform_calc
	la	$4, D_80142728
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
$787:
	lb	$7, 1996($19)
	b	$791
$788:
	.loc	6524 353
	.loc	6524 354
	addu	$5, $19, 556
	l.s	$f8, 8($8)
	c.eq.s	$f28, $f8
	bc1t	$789
	.loc	6524 354
	s.s	$f28, 8($8)
	lw	$4, 0($8)
	b	$790
$789:
	li.s	$f12, -2.0000000000000000e+00
	.loc	6524 355
	mov.s	$f28, $f12
$790:
	.loc	6524 356
	.loc	6524 184
	.loc	6524 185
	la	$6, D_801141B0
	mfc1	$7, $f14
	s.s	$f28, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_clip_planes
	.loc	6524 186
	lb	$7, 1996($19)
$791:
	.loc	6524 360
	bne	$7, 2, $795
	.loc	6524 360
	.loc	6524 361
	move	$16, $19
	.livereg	0x0000800E,0x00000000
	jal	func_800DFBA0
	.loc	6524 362
	move	$17, $19
	sw	$19, 224($sp)
	.livereg	0x0000400E,0x00000000
	jal	mode_select_handler
	lw	$5, 220($sp)
	la	$9, D_8002EB90
	lw	$19, 224($sp)
	.loc	6524 363
	mul	$8, $5, 4
	.noalias	$3,$sp
	la	$15, D_80110020
	addu	$3, $8, $15
	.noalias	$9,$3
	.noalias	$9,$sp
	.set	 volatile
	l.s	$f10, 0($9)
	.set	 novolatile
	l.s	$f4, 0($3)
	c.lt.s	$f10, $f4
	bc1f	$792
	.loc	6524 363
	li.s	$f6, 0.0
	s.s	$f6, 0($3)
$792:
	.loc	6524 364
	lw	$24, 2004($19)
	and	$25, $24, 16
	bne	$25, 0, $793
	.noalias	$2,$3
	.noalias	$2,$9
	.noalias	$2,$sp
	mul	$14, $5, 76
	la	$15, input_rec0
	addu	$2, $14, $15
	lw	$24, 56($2)
	lw	$25, 4($2)
	and	$14, $24, $25
	beq	$14, 0, $793
	.alias	$2,$3
	.alias	$2,$9
	.alias	$2,$sp
	li.s	$f8, 0.1
	.set	 volatile
	l.s	$f10, 0($9)
	.set	 novolatile
	l.s	$f4, 0($3)
	sub.s	$f6, $f10, $f4
	c.lt.s	$f8, $f6
	bc1f	$793
	.loc	6524 366
	.loc	6524 367
	.set	 volatile
	l.s	$f10, 0($9)
	.set	 novolatile
	s.s	$f10, 0($3)
	.loc	6524 368
	mul	$15, $5, 13
	lbu	$24, 8($19)
	addu	$25, $15, $24
	lb	$4, D_801115CD($25)
	addu	$4, $4, 26
	li	$6, 1
	li	$7, 4
	sw	$8, 148($sp)
	.livereg	0x0F00000E,0x00000000
	jal	frame_sync
	.alias	$3,$9
	.alias	$3,$sp
	.alias	$9,$sp
	lw	$8, 148($sp)
	sw	$2, D_801407E0($8)
	lw	$5, 220($sp)
$793:
	.loc	6524 370
	lh	$2, 1628($19)
	bne	$2, 1, $794
	.loc	6524 370
	.loc	6524 371
	li	$4, 25
	li	$6, 2
	li	$7, 1
	sw	$8, 148($sp)
	.livereg	0x0F00000E,0x00000000
	jal	frame_sync
	lw	$8, 148($sp)
	sw	$2, D_801407C0($8)
	.loc	6524 372
	li	$14, 2
	sh	$14, 1628($19)
	b	$795
$794:
	.loc	6524 373
	bne	$2, 3, $795
	.loc	6524 373
	.loc	6524 374
	.noalias	$2,$sp
	la	$15, D_801407C0
	addu	$2, $8, $15
	lw	$4, 0($2)
	sw	$2, 144($sp)
	.livereg	0x2800000E,0x00000000
	jal	scheduler_recv
	lw	$2, 144($sp)
	.loc	6524 375
	li	$24, -1
	sw	$24, 0($2)
	.loc	6524 376
	sh	$0, 1628($19)
	.alias	$2,$sp
	.loc	6524 378
$795:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 48($sp)
	l.d	$f22, 56($sp)
	l.d	$f24, 64($sp)
	l.d	$f26, 72($sp)
	l.d	$f28, 80($sp)
	l.d	$f30, 88($sp)
	lw	$16, 96($sp)
	lw	$17, 100($sp)
	lw	$18, 104($sp)
	lw	$19, 108($sp)
	lw	$20, 112($sp)
	lw	$21, 116($sp)
	lw	$22, 120($sp)
	lw	$23, 124($sp)
	lw	$31, 132($sp)
	lw	$30, 128($sp)
	addu	$sp, 224
	j	$31
	.end	func_800E05F0
