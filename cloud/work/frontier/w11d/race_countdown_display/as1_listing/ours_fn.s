	.ent	race_countdown_display 2
race_countdown_display:
	.option	O3
	subu	$sp, 160
	sw	$31, 36($sp)
	.mask	0x80000000, -124
	.frame	$sp, 160, $31
	.loc	4029 96
	move	$8, $4
	sw	$5, 164($sp)
	la	$6, D_80151CE8
	.loc	4029 96
	.loc	4029 103
	move	$7, $0
	.loc	4029 104
	lh	$9, 1990($8)
	.loc	4029 105
	lh	$14, 2020($8)
	sh	$14, 2018($8)
	.loc	4029 106
	lh	$5, 2018($8)
	move	$3, $5
	.noalias	$6,$sp
	addu	$15, $3, 1
	lh	$24, 8($6)
	bne	$15, $24, $2427
	lh	$2, 2($6)
	b	$2428
$2427:
	addu	$2, $3, 1
$2428:
	sh	$2, 2020($8)
	.loc	4029 107
	lh	$25, 4($6)
	bne	$25, $5, $2429
	lb	$11, 2012($8)
	beq	$11, 0, $2429
	lw	$12, D_8014A110
	beq	$12, 1, $2429
	lw	$13, D_801174B4
	and	$14, $13, 8
	bne	$14, 0, $2429
	.loc	4029 108
	.loc	4029 109
	lb	$15, 2024($8)
	addu	$24, $15, 1
	sb	$24, 2024($8)
	.loc	4029 110
	li	$7, 1
$2429:
	.loc	4029 112
	lb	$25, 2012($8)
	bne	$25, 0, $2430
	lh	$11, 6($6)
	lh	$12, 2018($8)
	bne	$11, $12, $2430
	.loc	4029 112
	.loc	4029 113
	li	$13, 1
	sb	$13, 2012($8)
$2430:
	.loc	4029 115
	.loc	4029 116
	mul	$14, $9, 8
	la	$15, D_80153E88
	addu	$24, $14, $15
	sw	$24, 40($sp)
	lbu	$25, 7($24)
	bne	$25, 6, $2433
	.loc	4029 116
	.loc	4029 117
	lh	$3, 4($6)
	lh	$11, 2018($8)
	bne	$3, $11, $2431
	.alias	$6,$sp
	lw	$12, D_801174B4
	and	$13, $12, 8
	bne	$13, 0, $2431
	.loc	4029 117
	.loc	4029 118
	move	$4, $9
	lw	$5, 164($sp)
	sw	$7, 140($sp)
	sw	$8, 160($sp)
	sw	$9, 152($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800D2458
	lw	$7, 140($sp)
	lw	$8, 160($sp)
	lw	$9, 152($sp)
	lh	$3, D_80151CE8+4
$2431:
	.loc	4029 120
	lh	$14, 2020($8)
	seq	$2, $3, $14
	beq	$2, 0, $2432
	lh	$15, D_80152734
	lb	$24, 2024($8)
	addu	$25, $24, 1
	seq	$2, $15, $25
$2432:
	sb	$2, D_80152015
$2433:
	.loc	4029 123
	beq	$7, 0, $2450
	.loc	4029 123
	.loc	4029 124
	lh	$4, 1990($8)
	lw	$5, 164($sp)
	sw	$8, 160($sp)
	sw	$9, 152($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800D2054
	lw	$8, 160($sp)
	lw	$9, 152($sp)
	.loc	4029 125
	lh	$11, D_80152734
	lb	$12, 2024($8)
	bne	$11, $12, $2434
	mul	$13, $9, 952
	lb	$14, D_80152818+239($13)
	bne	$14, 0, $2434
	.loc	4029 125
	.loc	4029 126
	lh	$4, 1990($8)
	lw	$5, 164($sp)
	sw	$8, 160($sp)
	sw	$9, 152($sp)
	.livereg	0x0C00000E,0x00000000
	jal	car_setup_confirm
	lw	$8, 160($sp)
	lw	$9, 152($sp)
$2434:
	.loc	4029 128
	lw	$24, 40($sp)
	lbu	$15, 7($24)
	bne	$15, 6, $2450
	li	$7, 2
	lw	$25, D_8014A110
	bne	$7, $25, $2435
	bne	$9, 0, $2450
$2435:
	la	$5, D_80152014
	.loc	4029 128
	.loc	4029 129
	.noalias	$5,$sp
	lb	$2, 0($5)
	move	$6, $2
	.loc	4029 130
	lh	$3, D_80152734
	lb	$11, 2024($8)
	subu	$4, $3, $11
	bge	$2, $4, $2436
	sb	$2, 0($5)
	b	$2437
$2436:
	sb	$4, 0($5)
$2437:
	.loc	4029 132
	lb	$12, 2024($8)
	bne	$3, $12, $2447
	.noalias	$3,$5
	.noalias	$3,$sp
	mul	$13, $9, 952
	la	$14, D_80152818
	addu	$3, $13, $14
	lb	$24, 239($3)
	beq	$24, -1, $2447
	.alias	$5,$3
	.alias	$5,$sp
	li	$10, 1
	.loc	4029 132
	.loc	4029 133
	.loc	4029 134
	sb	$10, D_80110680($9)
	.loc	4029 135
	la	$15, D_8002EB90
	.set	 volatile
	l.s	$f4, 0($15)
	.set	 novolatile
	mul	$25, $9, 4
	s.s	$f4, D_80110668($25)
	.loc	4029 136
	lb	$2, 238($3)
	beq	$2, 0, $2438
	beq	$2, $10, $2440
	beq	$2, $7, $2442
	b	$2444
$2438:
	.loc	4029 138
	lb	$11, D_8010FFC0
	bne	$11, 0, $2439
	li	$2, -1
	b	$2446
$2439:
	li	$4, 64
	move	$5, $9
	move	$6, $10
	li	$7, 1
	sw	$3, 44($sp)
	sw	$8, 160($sp)
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	lw	$3, 44($sp)
	lw	$8, 160($sp)
	li	$10, 1
	.loc	4029 139
	b	$2446
$2440:
	.loc	4029 141
	lb	$12, D_8010FFC0
	bne	$12, 0, $2441
	li	$2, -1
	b	$2446
$2441:
	li	$4, 68
	move	$5, $9
	move	$6, $10
	li	$7, 1
	sw	$3, 44($sp)
	sw	$8, 160($sp)
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	lw	$3, 44($sp)
	lw	$8, 160($sp)
	li	$10, 1
	.loc	4029 142
	b	$2446
$2442:
	.loc	4029 144
	lb	$13, D_8010FFC0
	bne	$13, 0, $2443
	li	$2, -1
	b	$2446
$2443:
	li	$4, 67
	move	$5, $9
	move	$6, $10
	li	$7, 1
	sw	$3, 44($sp)
	sw	$8, 160($sp)
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	lw	$3, 44($sp)
	lw	$8, 160($sp)
	li	$10, 1
	.loc	4029 145
	b	$2446
$2444:
	.loc	4029 147
	lb	$14, D_8010FFC0
	bne	$14, 0, $2445
	li	$2, -1
	b	$2446
$2445:
	li	$4, 65
	move	$5, $9
	move	$6, $10
	li	$7, 1
	sw	$3, 44($sp)
	sw	$8, 160($sp)
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	lw	$3, 44($sp)
	lw	$8, 160($sp)
	li	$10, 1
	.loc	4029 148
$2446:
	.loc	4029 150
	sb	$10, 776($3)
	.loc	4029 151
	sb	$10, 777($3)
	.loc	4029 152
	lb	$24, 785($3)
	or	$15, $24, 1
	sb	$15, 785($3)
	.loc	4029 153
	li.s	$f6, 0.0
	s.s	$f6, 780($3)
	.loc	4029 154
	sb	$10, 2026($8)
	.loc	4029 155
	sb	$10, 2027($8)
	.loc	4029 156
	lw	$25, 2004($8)
	and	$11, $25, -9
	sw	$11, 2004($8)
	b	$2450
	.alias	$3,$sp
$2447:
	.loc	4029 157
	.noalias	$5,$sp
	lb	$2, 0($5)
	beq	$6, $2, $2450
	.alias	$5,$sp
	.loc	4029 157
	.loc	4029 158
	bne	$2, 1, $2449
	.loc	4029 158
	.loc	4029 159
	lb	$12, D_8010FFC0
	bne	$12, 0, $2448
	li	$2, -1
	b	$2450
$2448:
	li	$4, 74
	move	$5, $9
	li	$6, 1
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	b	$2450
$2449:
	.loc	4029 160
	.loc	4029 161
	move	$4, $0
	li.s	$5, 2.0
	li	$6, 1
	move	$7, $0
	li	$13, 2
	sw	$13, 16($sp)
	addu	$14, $2, 79
	sw	$14, 20($sp)
	li	$24, 80
	sw	$24, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	car_stats_display
	.loc	4029 166
$2450:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 36($sp)
	addu	$sp, 160
	j	$31
	.end	race_countdown_display
