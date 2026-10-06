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
	move	$9, $4
	lh	$3, 1990($9)
	lb	$14, D_8010FFC0
	beq	$14, 0, $795
	.noalias	$2,$sp
	la	$15, D_8010FFCC
	addu	$2, $3, $15
	lb	$24, 0($2)
	beq	$24, 0, $777
	sb	$0, 0($2)
	b	$795
$777:
	li	$25, 1
	sb	$25, 0($2)
	lb	$14, D_8010FFC4($3)
	beq	$14, 0, $795
	.alias	$2,$sp
	sw	$9, 0($sp)
	sw	$3, 220($sp)
	sw	$9, 224($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_800E0050
	lw	$9, 224($sp)
	lh	$10, 1990($9)
	move	$11, $10
	lb	$7, 1996($9)
	bne	$7, 2, $791
	li.s	$f28, 0.5
	li.s	$f20, 0.0
	move	$3, $0
	li	$5, 4
	move	$4, $9
	li.s	$f18, 0.6
	li.s	$f16, 1.0000000000000000e+00
	li.s	$f14, 5.0E-1
	la	$6, D_8011F060
$778:
	lhu	$15, 1564($4)
	bne	$15, 0, $780
	mul	$2, $3, 4
	.noalias	$6,$sp
	addu	$24, $6, $2
	.noalias	$24,$sp
	l.s	$f0, 0($24)
	.alias	$24,$sp
	addu	$25, $9, $2
	l.s	$f4, 2040($25)
	mul.s	$f12, $f0, $f4
	mul.s	$f6, $f0, $f14
	sub.s	$f8, $f12, $f6
	add.s	$f2, $f8, $f16
	c.lt.s	$f28, $f2
	bc1f	$779
	mov.s	$f28, $f2
$779:
	mul.s	$f0, $f12, $f18
	c.lt.s	$f20, $f0
	bc1f	$780
	mov.s	$f20, $f0
$780:
	addu	$3, $3, 1
	addu	$4, $4, 2
	bne	$3, $5, $778
	.alias	$6,$sp
	.noalias	$8,$sp
	mul	$14, $11, 20
	la	$15, D_80140640
	addu	$8, $14, $15
	lw	$4, 0($8)
	bne	$4, -1, $782
	li.s	$f10, 0.0
	c.lt.s	$f10, $f20
	bc1f	$782
	bne	$7, 2, $781
	li	$4, 36
	move	$5, $10
	li	$6, 2
	li	$7, 2
	sw	$8, 140($sp)
	sw	$9, 224($sp)
	.livereg	0x0F00000E,0x00000000
	jal	frame_sync
	lw	$8, 140($sp)
	lw	$9, 224($sp)
	sw	$2, 0($8)
	lb	$7, 1996($9)
	lw	$4, 0($8)
	b	$782
$781:
	addu	$4, $9, 556
	la	$5, D_801141B0
	li.s	$6, 400.0
	li.s	$7, 0.0
	li.s	$f4, 1.0
	s.s	$f4, 16($sp)
	li.s	$f6, 0.0
	s.s	$f6, 20($sp)
	li	$24, 36
	sw	$24, 24($sp)
	sw	$11, 28($sp)
	sw	$0, 32($sp)
	li	$25, 130
	sw	$25, 36($sp)
	sw	$8, 140($sp)
	sw	$9, 224($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_target_track
	lw	$8, 140($sp)
	lw	$9, 224($sp)
	sw	$2, 0($8)
	lb	$7, 1996($9)
	lw	$4, 0($8)
$782:
	beq	$4, -1, $791
	li.s	$f8, 0.0
	c.le.s	$f20, $f8
	bc1f	$785
	bne	$7, 2, $783
	sw	$8, 140($sp)
	sw	$9, 224($sp)
	.livereg	0x0800000E,0x00000000
	jal	scheduler_recv
	lw	$8, 140($sp)
	lw	$9, 224($sp)
	b	$784
$783:
	sw	$8, 140($sp)
	sw	$9, 224($sp)
	.livereg	0x0800000E,0x00000000
	jal	results_screen_update
	lw	$8, 140($sp)
	lw	$9, 224($sp)
$784:
	move	$4, $8
	sw	$8, 140($sp)
	sw	$9, 224($sp)
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	lw	$8, 140($sp)
	lw	$9, 224($sp)
	.alias	$8,$sp
	lb	$7, 1996($9)
	b	$791
$785:
	bne	$7, 2, $788
	l.s	$f10, 4($8)
	c.eq.s	$f20, $f10
	bc1t	$786
	s.s	$f20, 4($8)
	mfc1	$5, $f20
	sw	$8, 140($sp)
	sw	$9, 224($sp)
	.livereg	0x0C00000E,0x00000000
	jal	client_sync
	lw	$8, 140($sp)
	lw	$9, 224($sp)
$786:
	l.s	$f4, 8($8)
	c.eq.s	$f28, $f4
	bc1t	$787
	s.s	$f28, 8($8)
	lw	$14, 0($8)
	sw	$14, 168($sp)
	la	$4, D_80142728
	move	$5, $0
	li	$6, 1
	sw	$9, 224($sp)
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	lw	$9, 224($sp)
	li.s	$f12, -2.0000000000000000e+00
	lw	$4, 168($sp)
	mov.s	$f22, $f12
	mov.s	$f24, $f12
	mov.s	$f26, $f12
	.livereg	0x0800000E,0x000002A8
	jal	entity_transform_calc
	lw	$9, 224($sp)
	la	$4, D_80142728
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	lw	$9, 224($sp)
$787:
	lb	$7, 1996($9)
	b	$791
$788:
	addu	$5, $9, 556
	l.s	$f6, 8($8)
	c.eq.s	$f28, $f6
	bc1t	$789
	s.s	$f28, 8($8)
	lw	$4, 0($8)
	b	$790
$789:
	li.s	$f12, -2.0000000000000000e+00
	mov.s	$f28, $f12
$790:
	la	$6, D_801141B0
	mfc1	$7, $f20
	s.s	$f28, 16($sp)
	sw	$9, 224($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_clip_planes
	lw	$9, 224($sp)
	lb	$7, 1996($9)
$791:
	bne	$7, 2, $795
	move	$16, $9
	sw	$9, 224($sp)
	.livereg	0x0000800E,0x00000000
	jal	func_800DFBA0
	lw	$9, 224($sp)
	move	$17, $9
	.livereg	0x0000400E,0x00000000
	jal	mode_select_handler
	lw	$5, 220($sp)
	lw	$9, 224($sp)
	la	$10, D_8002EB90
	mul	$8, $5, 4
	.noalias	$3,$sp
	la	$15, D_80110020
	addu	$3, $8, $15
	.noalias	$10,$3
	.noalias	$10,$sp
	.set	 volatile
	l.s	$f8, 0($10)
	.set	 novolatile
	l.s	$f10, 0($3)
	c.lt.s	$f8, $f10
	bc1f	$792
	li.s	$f4, 0.0
	s.s	$f4, 0($3)
$792:
	lw	$24, 2004($9)
	and	$25, $24, 16
	bne	$25, 0, $793
	.noalias	$2,$3
	.noalias	$2,$10
	.noalias	$2,$sp
	mul	$14, $5, 76
	la	$15, input_rec0
	addu	$2, $14, $15
	lw	$24, 56($2)
	lw	$25, 4($2)
	and	$14, $24, $25
	beq	$14, 0, $793
	.alias	$2,$3
	.alias	$2,$10
	.alias	$2,$sp
	li.s	$f6, 0.1
	.set	 volatile
	l.s	$f8, 0($10)
	.set	 novolatile
	l.s	$f10, 0($3)
	sub.s	$f4, $f8, $f10
	c.lt.s	$f6, $f4
	bc1f	$793
	.set	 volatile
	l.s	$f8, 0($10)
	.set	 novolatile
	s.s	$f8, 0($3)
	mul	$15, $5, 13
	lbu	$24, 8($9)
	addu	$25, $15, $24
	lb	$4, D_801115CD($25)
	addu	$4, $4, 26
	li	$6, 1
	li	$7, 4
	sw	$8, 148($sp)
	sw	$9, 224($sp)
	.livereg	0x0F00000E,0x00000000
	jal	frame_sync
	.alias	$3,$10
	.alias	$3,$sp
	.alias	$10,$sp
	lw	$8, 148($sp)
	lw	$9, 224($sp)
	sw	$2, D_801407E0($8)
	lw	$5, 220($sp)
$793:
	lh	$2, 1628($9)
	bne	$2, 1, $794
	li	$4, 25
	li	$6, 2
	li	$7, 1
	sw	$8, 148($sp)
	sw	$9, 224($sp)
	.livereg	0x0F00000E,0x00000000
	jal	frame_sync
	lw	$8, 148($sp)
	lw	$9, 224($sp)
	sw	$2, D_801407C0($8)
	li	$14, 2
	sh	$14, 1628($9)
	b	$795
$794:
	bne	$2, 3, $795
	.noalias	$2,$sp
	la	$15, D_801407C0
	addu	$2, $8, $15
	lw	$4, 0($2)
	sw	$2, 144($sp)
	sw	$9, 224($sp)
	.livereg	0x2800000E,0x00000000
	jal	scheduler_recv
	lw	$2, 144($sp)
	lw	$9, 224($sp)
	li	$24, -1
	sw	$24, 0($2)
	sh	$0, 1628($9)
	.alias	$2,$sp
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
