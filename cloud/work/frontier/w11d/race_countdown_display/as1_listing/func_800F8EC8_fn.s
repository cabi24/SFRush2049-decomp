	.ent	func_800F8EC8 2
func_800F8EC8:
	.option	O3
	subu	$sp, 168
	sw	$31, 60($sp)
	sw	$21, 56($sp)
	sw	$20, 52($sp)
	sw	$19, 48($sp)
	sw	$18, 44($sp)
	sw	$17, 40($sp)
	sw	$16, 36($sp)
	s.d	$f22, 24($sp)
	s.d	$f20, 16($sp)
	.mask	0x803F0000, -108
	.fmask	0x00F00000, -144
	.frame	$sp, 168, $31
	.loc	917 95
	.loc	917 95
	.loc	917 108
	move	$18, $0
	la	$14, D_801543CA
	.set	 volatile
	lh	$15, 0($14)
	.set	 novolatile
	ble	$15, 0, $4662
	li.s	$f22, 0.0
	la	$21, D_801527E8
	li	$20, 80
	la	$19, D_80151CF4
$4656:
	.loc	917 108
	.loc	917 109
	.loc	917 110
	.loc	917 111
	.noalias	$16,$sp
	mul	$24, $18, 952
	la	$25, D_80152818
	addu	$16, $24, $25
	.noalias	$17,$16
	.noalias	$17,$sp
	mul	$14, $18, 12
	la	$15, D_80152218
	addu	$17, $14, $15
	l.s	$f4, 8($16)
	l.s	$f6, 0($17)
	sub.s	$f8, $f4, $f6
	s.s	$f8, 112($sp)
	.loc	917 112
	l.s	$f10, 12($16)
	l.s	$f4, 4($17)
	sub.s	$f6, $f10, $f4
	s.s	$f6, 116($sp)
	.loc	917 113
	l.s	$f8, 16($16)
	l.s	$f10, 8($17)
	sub.s	$f4, $f8, $f10
	s.s	$f4, 120($sp)
	.loc	917 114
	addu	$4, $sp, 112
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3C8
	l.s	$f6, 264($16)
	add.s	$f8, $f6, $f0
	s.s	$f8, 264($16)
	.loc	917 115
	l.s	$f10, 8($16)
	s.s	$f10, 0($17)
	.loc	917 116
	l.s	$f4, 12($16)
	s.s	$f4, 4($17)
	.loc	917 117
	l.s	$f6, 16($16)
	s.s	$f6, 8($17)
	.loc	917 118
	mul	$24, $18, 8
	lbu	$2, D_80153E88+7($24)
	beq	$2, 0, $4657
	.alias	$17,$16
	.alias	$17,$sp
	bne	$2, 6, $4661
$4657:
	lb	$25, 856($16)
	bne	$25, 0, $4661
	.loc	917 118
	.loc	917 119
	.loc	917 120
	mul	$14, $18, 2056
	la	$15, D_8014A250
	addu	$4, $14, $15
	lh	$24, 1732($4)
	bne	$24, -1, $4661
	.loc	917 120
	.loc	917 121
	.noalias	$19,$16
	.noalias	$19,$sp
	l.s	$f8, 8($16)
	lh	$25, 2020($4)
	mul	$14, $25, $20
	addu	$15, $19, $14
	.noalias	$15,$16
	.noalias	$15,$sp
	l.s	$f10, 0($15)
	.alias	$15,$16
	.alias	$15,$sp
	sub.s	$f4, $f8, $f10
	s.s	$f4, 148($sp)
	.loc	917 122
	l.s	$f6, 12($16)
	lh	$24, 2020($4)
	mul	$25, $24, $20
	addu	$14, $19, $25
	.noalias	$14,$16
	.noalias	$14,$sp
	l.s	$f8, 4($14)
	.alias	$14,$16
	.alias	$14,$sp
	sub.s	$f10, $f6, $f8
	s.s	$f10, 152($sp)
	.loc	917 123
	l.s	$f4, 16($16)
	lh	$15, 2020($4)
	mul	$24, $15, $20
	addu	$25, $19, $24
	.noalias	$25,$16
	.noalias	$25,$sp
	l.s	$f6, 8($25)
	.alias	$25,$16
	.alias	$25,$sp
	sub.s	$f8, $f4, $f6
	s.s	$f8, 156($sp)
	.loc	917 124
	.noalias	$2,$16
	.noalias	$2,$sp
	lh	$14, 2020($4)
	mul	$15, $14, $20
	addu	$2, $19, $15
	l.s	$f10, 148($sp)
	l.s	$f4, 12($2)
	mul.s	$f6, $f10, $f4
	l.s	$f8, 152($sp)
	l.s	$f10, 16($2)
	mul.s	$f4, $f8, $f10
	add.s	$f8, $f6, $f4
	l.s	$f10, 20($2)
	l.s	$f6, 156($sp)
	mul.s	$f4, $f10, $f6
	add.s	$f20, $f4, $f8
	.loc	917 125
	c.lt.s	$f20, $f22
	bc1f	$4658
	.loc	917 125
	.loc	917 126
	li	$3, -1
	b	$4659
$4658:
	.loc	917 127
	.loc	917 128
	li	$3, 1
$4659:
	.loc	917 130
	lb	$24, 2025($4)
	beq	$3, $24, $4660
	.loc	917 130
	.loc	917 131
	sb	$3, 2025($4)
	.loc	917 132
	l.s	$f10, 148($sp)
	mul.s	$f6, $f10, $f10
	l.s	$f4, 156($sp)
	mul.s	$f8, $f4, $f4
	add.s	$f10, $f6, $f8
	lw	$25, 24($2)
	mtc1	$25, $f4
	cvt.s.w	$f6, $f4
	c.lt.s	$f10, $f6
	bc1f	$4660
	.alias	$2,$16
	.alias	$2,$sp
	.loc	917 132
	.loc	917 133
	.noalias	$21,$16
	.noalias	$21,$19
	.noalias	$21,$sp
	mul	$14, $18, 4
	addu	$15, $21, $14
	.noalias	$15,$16
	.noalias	$15,$19
	.noalias	$15,$sp
	l.s	$f8, 0($15)
	.alias	$15,$16
	.alias	$15,$19
	.alias	$15,$sp
	sub.s	$f4, $f20, $f8
	div.s	$f0, $f20, $f4
	.loc	917 134
	la	$24, D_8002EB94
	.set	 volatile
	l.s	$f10, 0($24)
	.set	 novolatile
	mul.s	$f0, $f10, $f0
	.loc	917 135
	l.s	$f6, 4($16)
	sub.s	$f8, $f6, $f0
	mfc1	$5, $f8
	.livereg	0x0C00000E,0x00000000
	jal	race_countdown_display
	.alias	$16,$19
	.alias	$16,$21
	.alias	$16,$sp
$4660:
	.loc	917 138
	mul	$25, $18, 4
	addu	$14, $21, $25
	.noalias	$14,$19
	.noalias	$14,$sp
	s.s	$f20, 0($14)
	.alias	$14,$19
	.alias	$14,$sp
$4661:
	.loc	917 108
	addu	$18, $18, 1
	sll	$15, $18, 16
	move	$18, $15
	sra	$24, $18, 16
	move	$18, $24
	la	$25, D_801543CA
	.set	 volatile
	lh	$14, 0($25)
	.set	 novolatile
	blt	$18, $14, $4656
	.alias	$19,$21
	.alias	$19,$sp
	.alias	$21,$sp
$4662:
	li	$18, 1
	li.s	$f22, 0.0
	.loc	917 142
	lw	$15, D_8014A110
	beq	$18, $15, $4667
	.loc	917 142
	.loc	917 143
	.loc	917 145
	.livereg	0x0000000E,0x00000000
	jal	world_gravity_apply
	.loc	917 146
	.livereg	0x0000000E,0x00000000
	jal	func_800D1AB0
	la	$3, D_80152018
	.loc	917 147
	.noalias	$3,$sp
	l.s	$f0, 0($3)
	c.eq.s	$f22, $f0
	bc1t	$4663
	li.s	$f4, 3.0
	la	$24, D_8002EB90
	.set	 volatile
	l.s	$f10, 0($24)
	.set	 novolatile
	sub.s	$f6, $f10, $f0
	c.lt.s	$f4, $f6
	bc1f	$4663
	.loc	917 147
	.loc	917 148
	s.s	$f22, 0($3)
$4663:
	.loc	917 150
	la	$25, D_80153FD2
	.set	 volatile
	lh	$14, 0($25)
	.set	 novolatile
	bne	$18, $14, $4666
	lbu	$2, D_8014A118
	mul	$15, $2, 952
	lb	$24, D_80152818+238($15)
	bne	$24, 0, $4666
	lw	$25, D_801174B4
	and	$14, $25, 8
	bne	$14, 0, $4666
	la	$16, D_80152031
	.loc	917 150
	.loc	917 151
	.noalias	$16,$3
	.noalias	$16,$sp
	lb	$15, 0($16)
	bne	$15, 0, $4665
	l.s	$f8, 0($3)
	c.eq.s	$f22, $f8
	bc1f	$4665
	.loc	917 151
	.loc	917 152
	la	$24, D_8002EB90
	.set	 volatile
	l.s	$f10, 0($24)
	.set	 novolatile
	s.s	$f10, 0($3)
	.loc	917 153
	move	$5, $2
	lb	$25, D_8010FFC0
	bne	$25, 0, $4664
	.alias	$3,$16
	.alias	$3,$sp
	li	$2, -1
	b	$4665
$4664:
	li	$4, 16
	move	$6, $18
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
$4665:
	.loc	917 155
	sb	$18, 0($16)
	b	$4667
$4666:
	la	$16, D_80152031
	.loc	917 156
	.loc	917 157
	sb	$0, 0($16)
	.alias	$16,$sp
	.loc	917 159
$4667:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 16($sp)
	l.d	$f22, 24($sp)
	lw	$16, 36($sp)
	lw	$17, 40($sp)
	lw	$18, 44($sp)
	lw	$19, 48($sp)
	lw	$20, 52($sp)
	lw	$21, 56($sp)
	lw	$31, 60($sp)
	addu	$sp, 168
	j	$31
	.end	func_800F8EC8
