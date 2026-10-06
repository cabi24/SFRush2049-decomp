	.text
	.globl	func_800D5E64
	.ent	func_800D5E64 2
func_800D5E64:
	.option	O3
	subu	$sp, 136
	sw	$31, 108($sp)
	sw	$30, 104($sp)
	sw	$23, 100($sp)
	sw	$22, 96($sp)
	sw	$21, 92($sp)
	sw	$20, 88($sp)
	sw	$19, 84($sp)
	sw	$18, 80($sp)
	sw	$17, 76($sp)
	sw	$16, 72($sp)
	s.d	$f24, 64($sp)
	s.d	$f22, 56($sp)
	s.d	$f20, 48($sp)
	.mask	0xC0FF0000, -28
	.fmask	0x03F00000, -72
	.frame	$sp, 136, $31
	.loc	2242 80
	move	$22, $4
	.loc	2242 80
	.loc	2242 81
	lh	$20, 1990($22)
	.loc	2242 87
	lb	$14, D_8010FFC0
	beq	$14, 0, $3909
	lb	$15, 1600($22)
	bne	$15, 0, $3909
	.loc	2242 87
	.loc	2242 78
	la	$24, D_8010FFC4
	addu	$3, $20, $24
	move	$2, $3
	.loc	2242 89
	.loc	2242 90
	.noalias	$21,$sp
	mul	$25, $20, 84
	la	$14, D_80140420
	addu	$21, $25, $14
	lb	$15, 12($22)
	mul	$24, $15, 64
	la	$25, D_8010FD80
	addu	$14, $24, $25
	sw	$14, 0($21)
	.loc	2242 91
	move	$2, $0
	move	$17, $0
	move	$16, $21
	addu	$18, $16, 4
	li.s	$f24, 1.0
	li.s	$f22, 400.0
	li.s	$f20, 0.0
	li	$30, 2
	li	$23, 64
	la	$19, D_801141B0
	sw	$3, 116($sp)
$3905:
	.loc	2242 91
	.loc	2242 92
	move	$4, $18
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	.loc	2242 93
	lw	$15, 0($21)
	addu	$24, $15, $17
	lw	$8, 0($24)
	.loc	2242 94
	beq	$8, -1, $3907
	.loc	2242 94
	.loc	2242 95
	.loc	2242 96
	lb	$25, 1996($22)
	bne	$30, $25, $3906
	.loc	2242 96
	move	$4, $8
	move	$5, $20
	move	$6, $0
	move	$7, $0
	s.s	$f20, 16($sp)
	s.s	$f20, 20($sp)
	s.s	$f20, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	high_scores_display
	sw	$2, 4($16)
	b	$3907
$3906:
	.loc	2242 97
	move	$4, $19
	move	$5, $19
	mfc1	$6, $f22
	mfc1	$7, $f20
	s.s	$f24, 16($sp)
	s.s	$f20, 20($sp)
	sw	$8, 24($sp)
	sw	$20, 28($sp)
	sw	$0, 32($sp)
	li	$14, 128
	sw	$14, 36($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_target_track
	sw	$2, 4($16)
	.loc	2242 91
$3907:
	addu	$17, $17, 32
	addu	$16, $16, 20
	addu	$18, $18, 20
	bne	$17, $23, $3905
	.loc	2242 100
	addu	$4, $21, 64
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	.alias	$21,$sp
	.loc	2242 101
	mul	$15, $20, 20
	la	$24, D_80140640
	addu	$4, $15, $24
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	.loc	2242 102
	bge	$20, 4, $3908
	.loc	2242 102
	.loc	2242 103
	mul	$25, $20, 60
	la	$14, D_801406C0
	addu	$16, $25, $14
	move	$4, $16
	.livereg	0x0800800E,0x00000000
	jal	player_conditional_call
	.loc	2242 104
	addu	$4, $16, 20
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	.loc	2242 105
	addu	$4, $16, 40
	.livereg	0x0800000E,0x00000000
	jal	player_conditional_call
	.loc	2242 106
	mul	$19, $20, 4
	li	$15, -1
	sw	$15, D_80140AE0($19)
	.loc	2242 107
	sll	$18, $20, 16
	sra	$24, $18, 16
	move	$18, $24
	.livereg	0x0000200E,0x00000000
	jal	best_times_display
	.loc	2242 108
	li	$25, -1
	sw	$25, D_801407E0($19)
	.loc	2242 109
	li	$14, -1
	sw	$14, D_801407C0($19)
$3908:
	.loc	2242 111
	li	$15, 1
	lw	$24, 116($sp)
	sb	$15, 0($24)
	.loc	2242 112
$3909:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 48($sp)
	l.d	$f22, 56($sp)
	l.d	$f24, 64($sp)
	lw	$16, 72($sp)
	lw	$17, 76($sp)
	lw	$18, 80($sp)
	lw	$19, 84($sp)
	lw	$20, 88($sp)
	lw	$21, 92($sp)
	lw	$22, 96($sp)
	lw	$23, 100($sp)
	lw	$31, 108($sp)
	lw	$30, 104($sp)
	addu	$sp, 136
	j	$31
	.end	func_800D5E64
