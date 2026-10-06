	.text
	.globl	entity_process_main
	.ent	entity_process_main 2
entity_process_main:
	.option	O3
	subu	$sp, 280
	sw	$31, 68($sp)
	s.d	$f26, 56($sp)
	s.d	$f24, 48($sp)
	s.d	$f22, 40($sp)
	s.d	$f20, 32($sp)
	.mask	0x80000000, -212
	.fmask	0x0FF00000, -224
	.frame	$sp, 280, $31
	.loc	6620 90
	move	$12, $4
	sw	$5, 284($sp)
	sll	$14, $5, 16
	move	$5, $14
	sra	$15, $5, 16
	move	$5, $15
	.loc	6620 90
	.loc	6620 95
	lh	$2, 8($12)
	mul	$24, $2, 952
	la	$25, D_80152818
	addu	$11, $24, $25
	.loc	6620 96
	mul	$14, $2, 2056
	la	$15, D_8014A250
	addu	$10, $14, $15
	.loc	6620 101
	bne	$5, 0, $472
	.loc	6620 101
	.loc	6620 102
	lh	$2, 6($12)
	blt	$2, 0, $471
	la	$10, D_8015B268
	li	$13, 88
	.loc	6620 102
	.loc	6620 103
	mul	$24, $2, $13
	addu	$4, $10, $24
	sw	$12, 280($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_8008D0C0
	lw	$12, 280($sp)
$471:
	.loc	6620 105
	sw	$0, 20($12)
	.loc	6620 106
	li	$25, -1
	sh	$25, 6($12)
	.loc	6620 107
	b	$494
$472:
	li.s	$f20, 2.0000000000000000e+01
	.loc	6620 110
	l.s	$f12, 1912($10)
	l.s	$f14, 1904($10)
	sub.s	$f26, $f12, $f14
	.loc	6620 111
	l.s	$f16, 1908($10)
	l.s	$f18, 1900($10)
	sub.s	$f2, $f16, $f18
	.loc	6620 112
	sub.s	$f22, $f14, $f18
	.loc	6620 113
	sub.s	$f24, $f12, $f16
	.loc	6620 114
	abs.s	$f0, $f26
	c.lt.s	$f20, $f0
	bc1t	$473
	abs.s	$f0, $f2
	c.lt.s	$f20, $f0
	bc1t	$473
	abs.s	$f0, $f22
	c.lt.s	$f20, $f0
	bc1t	$473
	abs.s	$f0, $f24
	c.lt.s	$f20, $f0
	bc1t	$473
	lb	$14, 863($11)
	bne	$14, 0, $473
	lh	$15, 1732($10)
	bge	$15, 0, $473
	lw	$24, 232($11)
	and	$25, $24, 8
	bne	$25, 0, $473
	lb	$14, D_80140418
	bne	$14, 0, $473
	lb	$15, 2015($10)
	beq	$15, 0, $474
$473:
	la	$10, D_8015B268
	li	$13, 88
	.loc	6620 115
	.loc	6620 116
	lh	$24, 6($12)
	mul	$25, $24, $13
	addu	$2, $10, $25
	.loc	6620 117
	lhu	$14, 2($2)
	or	$15, $14, 32768
	sh	$15, 2($2)
	.loc	6620 118
	b	$494
$474:
	.loc	6620 121
	move	$4, $0
	mtc1	$0, $f4
	cvt.s.w	$f26, $f4
	li.s	$f2, 0.0
	addu	$7, $sp, 168
$475:
	.loc	6620 121
	.loc	6620 122
	mul	$2, $4, 4
	addu	$24, $10, $2
	l.s	$f0, 1900($24)
	.noalias	$7,$gp
	.noalias	$3,$gp
	addu	$3, $7, $2
	s.s	$f0, 0($3)
	add.s	$f26, $f26, $f0
	.loc	6620 123
	c.lt.s	$f0, $f2
	bc1f	$476
	.loc	6620 123
	.loc	6620 124
	s.s	$f2, 0($3)
	.alias	$3,$gp
$476:
	.loc	6620 121
	addu	$4, $4, 1
	sll	$25, $4, 16
	move	$4, $25
	sra	$14, $4, 16
	move	$4, $14
	blt	$4, 4, $475
	.loc	6620 127
	li.s	$f6, 0.25
	mul.s	$f26, $f26, $f6
	.loc	6620 129
	move	$4, $0
	li.s	$f18, 1.0
	li.s	$f16, 0.1
	li.s	$f14, 2.0
	li.s	$f12, 10.0
	addu	$9, $sp, 184
	li	$8, 5
	li	$6, 12
$477:
	.loc	6620 129
	.loc	6620 130
	bge	$4, 2, $478
	sll	$5, $4, 16
	sra	$15, $5, 16
	move	$5, $15
	b	$479
$478:
	subu	$5, $8, $4
	sll	$24, $5, 16
	move	$5, $24
	sra	$25, $5, 16
	move	$5, $25
$479:
	.loc	6620 131
	mul	$14, $5, 4
	addu	$15, $7, $14
	.noalias	$15,$gp
	l.s	$f0, 0($15)
	.alias	$15,$gp
	c.lt.s	$f12, $f0
	bc1f	$480
	mov.s	$f2, $f14
	b	$481
$480:
	mul.s	$f8, $f0, $f16
	add.s	$f2, $f8, $f18
$481:
	.loc	6620 132
	.noalias	$9,$7
	.noalias	$9,$gp
	.noalias	$2,$7
	.noalias	$2,$gp
	mul	$24, $4, $6
	addu	$2, $9, $24
	mul	$25, $5, $6
	addu	$3, $11, $25
	l.s	$f10, 116($3)
	s.s	$f10, 0($2)
	.loc	6620 133
	l.s	$f4, 120($3)
	sub.s	$f6, $f4, $f0
	add.s	$f8, $f6, $f2
	s.s	$f8, 4($2)
	.loc	6620 134
	l.s	$f10, 124($3)
	s.s	$f10, 8($2)
	.loc	6620 129
	addu	$4, $4, 1
	sll	$14, $4, 16
	move	$4, $14
	sra	$15, $4, 16
	move	$4, $15
	blt	$4, 4, $477
	.alias	$2,$7
	.alias	$2,$gp
	.alias	$7,$9
	.alias	$7,$gp
	.loc	6620 137
	lw	$24, 232($11)
	and	$25, $24, 16
	beq	$25, 0, $482
	.loc	6620 137
	.loc	6620 138
	li	$2, 13
	b	$483
$482:
	.loc	6620 139
	.loc	6620 140
	lbu	$2, 8($10)
$483:
	.loc	6620 142
	.loc	6620 144
	.noalias	$3,$9
	.noalias	$3,$sp
	mul	$14, $2, 16
	la	$15, D_8011F914
	addu	$3, $14, $15
	l.s	$f22, 0($3)
	.loc	6620 145
	l.s	$f24, 4($3)
	.loc	6620 146
	move	$4, $0
$484:
	.loc	6620 146
	.loc	6620 147
	.noalias	$2,$3
	.noalias	$2,$gp
	mul	$24, $4, 4
	addu	$2, $9, $24
	l.s	$f2, 0($2)
	l.s	$f12, 12($2)
	sub.s	$f0, $f2, $f12
	.loc	6620 148
	mul.s	$f14, $f0, $f22
	add.s	$f4, $f2, $f14
	s.s	$f4, 0($2)
	.loc	6620 149
	sub.s	$f6, $f12, $f14
	s.s	$f6, 12($2)
	.loc	6620 151
	l.s	$f16, 36($2)
	l.s	$f18, 24($2)
	sub.s	$f0, $f16, $f18
	.loc	6620 152
	mul.s	$f20, $f0, $f24
	add.s	$f8, $f16, $f20
	s.s	$f8, 36($2)
	.loc	6620 153
	sub.s	$f10, $f18, $f20
	s.s	$f10, 24($2)
	.alias	$2,$3
	.alias	$2,$gp
	.loc	6620 146
	addu	$4, $4, 1
	sll	$25, $4, 16
	move	$4, $25
	sra	$14, $4, 16
	move	$4, $14
	blt	$4, 3, $484
	.loc	6620 156
	l.s	$f22, 8($3)
	.loc	6620 157
	l.s	$f24, 12($3)
	.loc	6620 158
	move	$4, $0
$485:
	.loc	6620 158
	.loc	6620 159
	.noalias	$2,$3
	.noalias	$2,$gp
	mul	$15, $4, 4
	addu	$2, $9, $15
	l.s	$f2, 0($2)
	l.s	$f16, 36($2)
	sub.s	$f0, $f2, $f16
	.loc	6620 160
	mul.s	$f4, $f0, $f22
	add.s	$f6, $f2, $f4
	s.s	$f6, 0($2)
	.loc	6620 161
	mul.s	$f8, $f0, $f24
	sub.s	$f10, $f16, $f8
	s.s	$f10, 36($2)
	.loc	6620 163
	l.s	$f12, 12($2)
	l.s	$f18, 24($2)
	sub.s	$f0, $f12, $f18
	.loc	6620 164
	mul.s	$f4, $f0, $f22
	add.s	$f6, $f12, $f4
	s.s	$f6, 12($2)
	.loc	6620 165
	mul.s	$f8, $f0, $f24
	sub.s	$f10, $f18, $f8
	s.s	$f10, 24($2)
	.alias	$2,$3
	.alias	$2,$gp
	.loc	6620 158
	addu	$4, $4, 1
	sll	$24, $4, 16
	move	$4, $24
	sra	$25, $4, 16
	move	$4, $25
	blt	$4, 3, $485
	la	$10, D_8015B268
	li	$13, 88
	li.s	$f2, 20.0
	.loc	6620 168
	.noalias	$10,$3
	.noalias	$10,$9
	.noalias	$10,$sp
	.noalias	$2,$3
	.noalias	$2,$9
	.noalias	$2,$sp
	lh	$14, 6($12)
	mul	$15, $14, $13
	addu	$2, $10, $15
	lhu	$24, 2($2)
	and	$25, $24, 32767
	sh	$25, 2($2)
	.loc	6620 169
	li.s	$f4, 8.0
	mul.s	$f0, $f26, $f4
	c.lt.s	$f0, $f2
	bc1f	$486
	.alias	$2,$3
	.alias	$2,$9
	.alias	$2,$sp
	mov.s	$f0, $f2
	li.s	$f12, 255.0
	b	$489
$486:
	li.s	$f12, 255.0
	c.lt.s	$f12, $f0
	bc1f	$487
	mov.s	$f2, $f12
	b	$488
$487:
	mov.s	$f2, $f0
$488:
	mov.s	$f0, $f2
$489:
	la	$8, D_8011AD8C
	.noalias	$8,$3
	.noalias	$8,$9
	.noalias	$8,$10
	.noalias	$8,$sp
	sub.s	$f6, $f12, $f0
	cfc1	$14, $31
	li	$15, 1
	ctc1	$15, $31
	cvt.w.s	$f8, $f6
	cfc1	$15, $31
	.set	 noat
	and	$1, $15, 4
	and	$15, $15, 120
	.set	 at
	beq	$15, 0, $496
	li.s	$f8, 2147483648.0
	sub.s	$f8, $f6, $f8
	li	$15, 1
	ctc1	$15, $31
	cvt.w.s	$f8, $f8
	cfc1	$15, $31
	.set	 noat
	and	$1, $15, 4
	and	$15, $15, 120
	.set	 at
	bne	$15, 0, $497
	.set	 noat
	mfc1	$15, $f8
	li	$1, -2147483648
	or	$15, $15, $1
	.set	 at
	b	$495
$497:
	li	$15, -1
	b	$495
$496:
	mfc1	$15, $f8
	blt	$15, 0, $497
$495:
	ctc1	$14, $31
	sb	$15, 3($8)
	.loc	6620 170
	lbu	$24, 3($8)
	blt	$24, 193, $490
	.loc	6620 170
	.loc	6620 171
	li	$25, 192
	sb	$25, 3($8)
$490:
	.loc	6620 174
	lb	$4, 860($11)
	.loc	6620 175
	blt	$4, 0, $492
	lb	$2, 861($11)
	beq	$2, 0, $491
	bne	$2, 1, $492
$491:
	.loc	6620 175
	.loc	6620 176
	lh	$14, 6($12)
	mul	$15, $14, $13
	addu	$2, $10, $15
	.loc	6620 177
	li	$24, 1
	sll	$25, $24, $4
	lhu	$14, 2($2)
	not	$15, $25
	and	$24, $14, $15
	sh	$24, 2($2)
	b	$493
$492:
	.loc	6620 178
	.loc	6620 179
	.noalias	$2,$3
	.noalias	$2,$8
	.noalias	$2,$9
	.noalias	$2,$sp
	lh	$25, 6($12)
	mul	$14, $25, $13
	addu	$2, $10, $14
	lhu	$15, 2($2)
	or	$24, $15, 15
	sh	$24, 2($2)
	.alias	$2,$3
	.alias	$2,$8
	.alias	$2,$9
	.alias	$2,$sp
$493:
	.loc	6620 181
	.alias	$3,$8
	.alias	$3,$9
	.alias	$3,$10
	.alias	$3,$sp
	.loc	6620 181
	.loc	6620 182
	lh	$25, 6($12)
	mul	$14, $25, $13
	addu	$4, $10, $14
	li	$5, 4
	move	$6, $9
	move	$7, $0
	sw	$8, 16($sp)
	sw	$0, 20($sp)
	sw	$0, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_8008C074
	.alias	$8,$9
	.alias	$8,$10
	.alias	$8,$sp
	.alias	$9,$10
	.alias	$9,$gp
	.alias	$10,$sp
	.loc	6620 183
$494:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 32($sp)
	l.d	$f22, 40($sp)
	l.d	$f24, 48($sp)
	l.d	$f26, 56($sp)
	lw	$31, 68($sp)
	addu	$sp, 280
	j	$31
	.end	entity_process_main
