	.data
	.space 0
	.globl D_8011AD90
D_8011AD90:
	.word 0x3f800000,0x3f800000,0,0xbf800000,0x3f800000,0,0xbf800000,0xbf800000,0,0x3f800000,0xbf800000,0
	.text
	.globl	entity_render_setup
	.ent	entity_render_setup 2
entity_render_setup:
	.option	O3
	subu	$sp, 184
	sw	$31, 44($sp)
	sw	$17, 40($sp)
	sw	$16, 36($sp)
	.mask	0x80030000, -140
	.frame	$sp, 184, $31
	.loc	7040 82
	move	$10, $4
	sw	$5, 188($sp)
	.loc	7040 82
	.loc	7040 95
	.loc	7040 96
	lh	$3, 8($10)
	mul	$14, $3, 952
	la	$15, D_80152818
	addu	$2, $14, $15
	.loc	7040 97
	mul	$24, $3, 2056
	la	$25, D_8014A250
	addu	$5, $24, $25
	.loc	7040 98
	lh	$11, 4($10)
	sw	$11, 120($sp)
	.loc	7040 99
	.loc	7040 100
	lw	$12, D_8011B578
	sw	$12, 112($sp)
	.loc	7040 101
	lb	$13, D_801613A8
	beq	$13, 0, $496
	lw	$14, state_word_a
	and	$15, $14, 4194304
	beq	$15, 0, $496
	lb	$24, 856($2)
	bne	$24, 0, $495
	lh	$25, 1732($5)
	blt	$25, 0, $497
$495:
	li	$9, 6
	lw	$11, D_8014A110
	bne	$9, $11, $497
$496:
	.loc	7040 101
	.loc	7040 102
	.noalias	$2,$sp
	lh	$12, 6($10)
	mul	$13, $12, 88
	la	$14, D_8015B268
	addu	$2, $13, $14
	lhu	$15, 2($2)
	or	$24, $15, 32768
	sh	$24, 2($2)
	.loc	7040 103
	b	$518
	.alias	$2,$sp
$497:
	li	$9, 6
	.loc	7040 105
	lw	$6, D_8014A110
	bne	$9, $6, $501
	lw	$25, 908($2)
	and	$11, $25, 1
	beq	$11, 0, $501
	.loc	7040 105
	.loc	7040 106
	lb	$3, 930($2)
	bne	$3, 1, $498
	.loc	7040 106
	.loc	7040 107
	lbu	$12, 929($2)
	sb	$12, 115($sp)
	b	$500
$498:
	.loc	7040 108
	bne	$3, 2, $499
	.loc	7040 108
	.loc	7040 109
	lbu	$13, 929($2)
	sb	$13, 115($sp)
	b	$500
$499:
	.loc	7040 110
	.loc	7040 111
	lb	$14, 928($2)
	lbu	$15, D_8011B56C($14)
	sb	$15, 115($sp)
$500:
	.loc	7040 113
	lbu	$24, 115($sp)
	bge	$24, 32, $501
	.loc	7040 113
	.loc	7040 114
	li	$25, 32
	sb	$25, 115($sp)
$501:
	.loc	7040 117
	lb	$11, 1996($5)
	bne	$11, 2, $505
	.loc	7040 117
	.loc	7040 118
	bne	$9, $6, $502
	la	$3, D_8011B558
	la	$4, D_8012E67C
	.loc	7040 118
	.loc	7040 119
	.noalias	$3,$sp
	.noalias	$4,$3
	.noalias	$4,$sp
	lh	$12, 1990($5)
	addu	$13, $4, $12
	.noalias	$13,$3
	.noalias	$13,$sp
	lb	$14, 0($13)
	.alias	$13,$3
	.alias	$13,$sp
	mul	$15, $14, 4
	addu	$24, $3, $15
	.noalias	$24,$4
	.noalias	$24,$sp
	lbu	$25, 0($24)
	.alias	$24,$4
	.alias	$24,$sp
	sb	$25, 112($sp)
	.loc	7040 120
	lh	$11, 1990($5)
	addu	$12, $4, $11
	.noalias	$12,$3
	.noalias	$12,$sp
	lb	$13, 0($12)
	.alias	$12,$3
	.alias	$12,$sp
	mul	$14, $13, 4
	addu	$15, $3, $14
	.noalias	$15,$4
	.noalias	$15,$sp
	lbu	$24, 1($15)
	.alias	$15,$4
	.alias	$15,$sp
	sb	$24, 113($sp)
	.loc	7040 121
	lh	$25, 1990($5)
	addu	$11, $4, $25
	.noalias	$11,$3
	.noalias	$11,$sp
	lb	$12, 0($11)
	.alias	$11,$3
	.alias	$11,$sp
	mul	$13, $12, 4
	addu	$14, $3, $13
	.noalias	$14,$4
	.noalias	$14,$sp
	lbu	$15, 2($14)
	.alias	$14,$4
	.alias	$14,$sp
	sb	$15, 114($sp)
	b	$503
	.alias	$4,$3
	.alias	$4,$sp
$502:
	la	$3, D_8011B558
	.loc	7040 122
	.loc	7040 123
	lh	$24, 1990($5)
	mul	$25, $24, 4
	addu	$11, $3, $25
	.noalias	$11,$sp
	lbu	$12, 0($11)
	.alias	$11,$sp
	sb	$12, 112($sp)
	.loc	7040 124
	lh	$13, 1990($5)
	mul	$14, $13, 4
	addu	$15, $3, $14
	.noalias	$15,$sp
	lbu	$24, 1($15)
	.alias	$15,$sp
	sb	$24, 113($sp)
	.loc	7040 125
	lh	$25, 1990($5)
	mul	$11, $25, 4
	addu	$12, $3, $11
	.noalias	$12,$sp
	lbu	$13, 2($12)
	.alias	$12,$sp
	sb	$13, 114($sp)
	.alias	$3,$sp
$503:
	.loc	7040 127
	lh	$14, 1990($5)
	mul	$15, $14, 4
	lw	$24, 120($sp)
	addu	$25, $15, $24
	lb	$11, D_801613C0($25)
	beq	$11, 0, $504
	lbu	$12, 115($sp)
	blt	$12, 113, $504
	.loc	7040 127
	.loc	7040 128
	li	$13, 112
	sb	$13, 115($sp)
$504:
	.loc	7040 130
	bne	$6, 2, $506
	lh	$14, 8($10)
	ble	$14, 0, $506
	.loc	7040 130
	.loc	7040 131
	li	$15, 128
	sb	$15, 115($sp)
	b	$506
$505:
	la	$3, D_8011B558
	.loc	7040 133
	.loc	7040 134
	.noalias	$3,$sp
	lbu	$24, 16($3)
	sb	$24, 112($sp)
	.loc	7040 135
	lbu	$25, 17($3)
	sb	$25, 113($sp)
	.loc	7040 136
	lbu	$11, 18($3)
	sb	$11, 114($sp)
	.alias	$3,$sp
$506:
	.loc	7040 138
	.noalias	$7,$sp
	lw	$12, 120($sp)
	mul	$13, $12, 152
	la	$14, D_80150B70
	addu	$7, $13, $14
	l.s	$f4, 8($2)
	l.s	$f6, 36($7)
	sub.s	$f8, $f4, $f6
	s.s	$f8, 100($sp)
	.loc	7040 139
	l.s	$f10, 12($2)
	l.s	$f4, 40($7)
	sub.s	$f6, $f10, $f4
	s.s	$f6, 104($sp)
	.loc	7040 140
	l.s	$f8, 16($2)
	l.s	$f10, 44($7)
	sub.s	$f4, $f8, $f10
	s.s	$f4, 108($sp)
	.loc	7040 141
	sw	$10, 184($sp)
	bne	$9, $6, $515
	addu	$8, $sp, 128
	.loc	7040 141
	.loc	7040 142
	addu	$4, $sp, 100
	move	$5, $8
	move	$6, $7
	sw	$2, 180($sp)
	sw	$7, 56($sp)
	sw	$10, 184($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_8008C544
	lw	$2, 180($sp)
	lw	$7, 56($sp)
	addu	$8, $sp, 128
	.loc	7040 143
	l.s	$f12, 128($sp)
	neg.s	$f12, $f12
	l.s	$f14, 136($sp)
	.livereg	0x0000000E,0x000A0000
	jal	func_8008C768
	lw	$2, 180($sp)
	lw	$7, 56($sp)
	addu	$8, $sp, 128
	lw	$9, 120($sp)
	mov.s	$f14, $f0
	.loc	7040 144
	.loc	7040 145
	mul	$15, $9, 72
	la	$24, D_8017A510
	addu	$3, $15, $24
	l.s	$f18, 12($3)
	li.s	$f6, 31.415927
	div.s	$f12, $f6, $f18
	.loc	7040 146
	lb	$25, D_8017A63C
	blt	$25, 2, $507
	.loc	7040 146
	.loc	7040 147
	li.s	$f8, 16.5
	s.s	$f8, 76($sp)
	.loc	7040 148
	l.s	$f10, 16($3)
	s.s	$f10, 52($sp)
	l.s	$f4, 52($sp)
	li.s	$f6, 0.75
	mul.s	$f8, $f4, $f6
	s.s	$f8, 72($sp)
	b	$508
$507:
	.loc	7040 149
	.loc	7040 150
	li.s	$f10, 14.5
	s.s	$f10, 76($sp)
	.loc	7040 151
	l.s	$f2, 16($3)
	li.s	$f4, 1.1
	mul.s	$f6, $f2, $f4
	s.s	$f6, 72($sp)
	s.s	$f2, 52($sp)
$508:
	li.s	$f2, 0.48
	.loc	7040 153
	neg.s	$f8, $f18
	mul.s	$f10, $f8, $f2
	c.lt.s	$f0, $f10
	bc1f	$509
	li.s	$f4, -1.5707963705062866e+00
	c.lt.s	$f4, $f0
	bc1f	$509
	.alias	$7,$sp
	li.s	$f16, 0.63661975
	.loc	7040 153
	.loc	7040 154
	.loc	7040 155
	mul.s	$f6, $f18, $f16
	li.s	$f8, 19.0
	mul.s	$f2, $f6, $f8
	.loc	7040 156
	neg.s	$f14, $f0
	.loc	7040 157
	li	$3, 3
	.loc	7040 158
	b	$513
$509:
	.loc	7040 160
	li.s	$f10, -1.5707963705062866e+00
	c.lt.s	$f0, $f10
	bc1f	$510
	.loc	7040 160
	.loc	7040 161
	li.s	$f4, 3.1415927
	add.s	$f14, $f0, $f4
	.loc	7040 162
	b	$511
$510:
	.loc	7040 164
	li.s	$f6, 1.5707964
	c.lt.s	$f6, $f0
	bc1f	$512
	.loc	7040 164
	.loc	7040 165
	li.s	$f8, 3.1415927
	sub.s	$f14, $f0, $f8
	.loc	7040 166
$511:
	li.s	$f16, 0.63661975
	.loc	7040 167
	.loc	7040 168
	mul.s	$f10, $f18, $f16
	li.s	$f4, 19.0
	mul.s	$f6, $f10, $f4
	mul.s	$f8, $f6, $f14
	mul.s	$f2, $f8, $f16
	.loc	7040 169
	l.s	$f10, 52($sp)
	mul.s	$f4, $f10, $f16
	l.s	$f6, 76($sp)
	mul.s	$f0, $f4, $f6
	.loc	7040 170
	move	$3, $0
	.loc	7040 171
	b	$514
$512:
	.loc	7040 173
	mul.s	$f8, $f18, $f2
	c.lt.s	$f8, $f0
	bc1f	$515
	li.s	$f10, 1.5707964
	c.lt.s	$f0, $f10
	bc1f	$515
	li.s	$f16, 0.63661975
	.loc	7040 173
	.loc	7040 174
	.loc	7040 175
	mul.s	$f4, $f18, $f16
	li.s	$f6, 19.0
	neg.s	$f8, $f6
	mul.s	$f2, $f4, $f8
	.loc	7040 176
	li	$3, 1
	.loc	7040 177
$513:
	.loc	7040 178
	l.s	$f10, 52($sp)
	mul.s	$f6, $f10, $f16
	l.s	$f4, 76($sp)
	mul.s	$f8, $f6, $f4
	l.s	$f10, 72($sp)
	sub.s	$f6, $f14, $f10
	mul.s	$f4, $f8, $f6
	li.s	$f8, 1.5707964
	sub.s	$f6, $f8, $f10
	li.s	$f8, 1.0
	div.s	$f10, $f8, $f6
	mul.s	$f0, $f4, $f10
	.loc	7040 179
	.loc	7040 180
$514:
	li	$4, 12
	.loc	7040 180
	.loc	7040 181
	.noalias	$8,$gp
	.noalias	$2,$gp
	mul	$11, $3, $4
	addu	$2, $8, $11
	l.s	$f8, D_8011AD90
	add.s	$f6, $f8, $f2
	s.s	$f6, 0($2)
	.loc	7040 182
	l.s	$f4, D_8011AD90+4
	sub.s	$f10, $f4, $f0
	s.s	$f10, 4($2)
	.loc	7040 183
	s.s	$f12, 8($2)
	.loc	7040 184
	addu	$3, $3, 1
	and	$12, $3, 3
	move	$3, $12
	.loc	7040 180
	mul	$13, $3, $4
	addu	$2, $8, $13
	l.s	$f8, D_8011AD90+12
	add.s	$f6, $f8, $f2
	s.s	$f6, 0($2)
	l.s	$f4, D_8011AD90+16
	sub.s	$f10, $f4, $f0
	s.s	$f10, 4($2)
	s.s	$f12, 8($2)
	addu	$3, $3, 1
	and	$14, $3, 3
	move	$3, $14
	mul	$15, $3, $4
	addu	$2, $8, $15
	l.s	$f8, D_8011AD90+24
	add.s	$f6, $f8, $f2
	s.s	$f6, 0($2)
	l.s	$f4, D_8011AD90+28
	sub.s	$f10, $f4, $f0
	s.s	$f10, 4($2)
	s.s	$f12, 8($2)
	addu	$3, $3, 1
	and	$24, $3, 3
	move	$3, $24
	mul	$25, $3, $4
	addu	$2, $8, $25
	l.s	$f8, D_8011AD90+36
	add.s	$f6, $f8, $f2
	s.s	$f6, 0($2)
	l.s	$f4, D_8011AD90+40
	sub.s	$f10, $f4, $f0
	s.s	$f10, 4($2)
	s.s	$f12, 8($2)
	.alias	$2,$gp
	.loc	7040 186
	lw	$11, 184($sp)
	lh	$12, 6($11)
	mul	$13, $12, 88
	la	$14, D_8015B268
	addu	$4, $13, $14
	li	$5, 4
	move	$6, $8
	move	$7, $0
	addu	$15, $sp, 112
	sw	$15, 16($sp)
	li	$24, 1
	sll	$25, $24, $9
	or	$11, $25, 13840
	sw	$11, 20($sp)
	sw	$0, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_8008C074
	.alias	$8,$gp
	.loc	7040 187
	b	$518
$515:
	addu	$8, $sp, 128
	li	$9, 6
	lw	$10, 184($sp)
	.loc	7040 191
	addu	$4, $sp, 100
	sw	$2, 180($sp)
	sw	$7, 56($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3C8
	lw	$2, 180($sp)
	lw	$7, 56($sp)
	addu	$8, $sp, 128
	li.s	$f8, 25.0
	div.s	$f14, $f0, $f8
	.loc	7040 192
	move	$3, $0
	la	$17, D_8011AD90
	addu	$16, $sp, 128
	addu	$6, $7, 48
	sw	$6, 48($sp)
$516:
	lw	$6, 48($sp)
	.loc	7040 192
	.loc	7040 193
	move	$4, $17
	move	$5, $16
	sw	$2, 180($sp)
	s.s	$f14, 124($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_8008C544
	lw	$2, 180($sp)
	addu	$8, $sp, 128
	l.s	$f14, 124($sp)
	.loc	7040 194
	.noalias	$16,$gp
	l.s	$f6, 0($16)
	mul.s	$f4, $f6, $f14
	l.s	$f10, 8($2)
	add.s	$f8, $f4, $f10
	s.s	$f8, 0($16)
	.loc	7040 195
	l.s	$f6, 4($16)
	mul.s	$f4, $f6, $f14
	l.s	$f10, 12($2)
	add.s	$f8, $f4, $f10
	add.s	$f6, $f8, $f14
	li.s	$f4, 5.0
	add.s	$f10, $f6, $f4
	s.s	$f10, 4($16)
	.loc	7040 196
	l.s	$f8, 8($16)
	mul.s	$f6, $f8, $f14
	l.s	$f4, 16($2)
	add.s	$f10, $f6, $f4
	s.s	$f10, 8($16)
	.loc	7040 192
	addu	$17, $17, 12
	addu	$16, $16, 12
	addu	$12, $sp, 176
	bne	$16, $12, $516
	.alias	$16,$gp
	.loc	7040 198
	lw	$13, D_8014A110
	bne	$13, 6, $517
	.loc	7040 198
	.loc	7040 199
	lw	$14, 184($sp)
	lh	$15, 6($14)
	mul	$24, $15, 88
	la	$25, D_8015B268
	addu	$4, $24, $25
	li	$5, 4
	move	$6, $8
	move	$7, $0
	addu	$11, $sp, 112
	sw	$11, 16($sp)
	lw	$12, 120($sp)
	li	$13, 1
	sll	$14, $13, $12
	or	$15, $14, 13824
	sw	$15, 20($sp)
	sw	$0, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_8008C074
	b	$518
$517:
	.loc	7040 200
	.loc	7040 201
	lw	$24, 184($sp)
	lh	$25, 6($24)
	mul	$11, $25, 88
	la	$13, D_8015B268
	addu	$4, $11, $13
	li	$5, 4
	move	$6, $8
	move	$7, $0
	addu	$12, $sp, 112
	sw	$12, 16($sp)
	lw	$14, 120($sp)
	li	$15, 1
	sll	$24, $15, $14
	or	$25, $24, 5632
	sw	$25, 20($sp)
	sw	$0, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_8008C074
	.loc	7040 203
$518:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 36($sp)
	lw	$17, 40($sp)
	lw	$31, 44($sp)
	addu	$sp, 184
	j	$31
	.end	entity_render_setup
