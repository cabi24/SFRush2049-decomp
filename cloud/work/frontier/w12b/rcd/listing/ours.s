	.text
	.globl	race_countdown_display
	.ent	race_countdown_display 2
race_countdown_display:
	.option	O3
	subu	$sp, 160
	sw	$31, 36($sp)
	.mask	0x80000000, -124
	.frame	$sp, 160, $31
	.loc	4022 96
	move	$8, $4
	sw	$5, 164($sp)
	la	$6, D_80151CE8
	.loc	4022 96
	.loc	4022 103
	move	$7, $0
	.loc	4022 104
	lh	$9, 1990($8)
	.loc	4022 105
	lh	$14, 2020($8)
	sh	$14, 2018($8)
	.loc	4022 106
	lh	$5, 2018($8)
	move	$3, $5
	.noalias	$6,$sp
	addu	$15, $3, 1
	lh	$24, 8($6)
	bne	$15, $24, $2526
	lh	$2, 2($6)
	b	$2527
$2526:
	addu	$2, $3, 1
$2527:
	sh	$2, 2020($8)
	.loc	4022 107
	lh	$25, 4($6)
	bne	$25, $5, $2528
	lb	$11, 2012($8)
	beq	$11, 0, $2528
	lw	$12, D_8014A110
	beq	$12, 1, $2528
	lw	$13, D_801174B4
	and	$14, $13, 8
	bne	$14, 0, $2528
	.loc	4022 108
	.loc	4022 109
	lb	$15, 2024($8)
	addu	$24, $15, 1
	sb	$24, 2024($8)
	.loc	4022 110
	li	$7, 1
$2528:
	.loc	4022 112
	lb	$25, 2012($8)
	bne	$25, 0, $2529
	lh	$11, 6($6)
	lh	$12, 2018($8)
	bne	$11, $12, $2529
	.loc	4022 112
	.loc	4022 113
	li	$13, 1
	sb	$13, 2012($8)
$2529:
	.loc	4022 115
	.loc	4022 116
	mul	$14, $9, 8
	la	$15, D_80153E88
	addu	$24, $14, $15
	sw	$24, 40($sp)
	lbu	$25, 7($24)
	bne	$25, 6, $2532
	.loc	4022 116
	.loc	4022 117
	lh	$3, 4($6)
	lh	$11, 2018($8)
	bne	$3, $11, $2530
	.alias	$6,$sp
	lw	$12, D_801174B4
	and	$13, $12, 8
	bne	$13, 0, $2530
	.loc	4022 117
	.loc	4022 118
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
$2530:
	.loc	4022 120
	lh	$14, 2020($8)
	seq	$2, $3, $14
	beq	$2, 0, $2531
	lh	$15, D_80152734
	lb	$24, 2024($8)
	addu	$25, $24, 1
	seq	$2, $15, $25
$2531:
	sb	$2, D_80152015
$2532:
	.loc	4022 123
	beq	$7, 0, $2549
	.loc	4022 123
	.loc	4022 124
	lh	$4, 1990($8)
	lw	$5, 164($sp)
	sw	$8, 160($sp)
	sw	$9, 152($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800D2054
	lw	$8, 160($sp)
	lw	$9, 152($sp)
	.loc	4022 125
	lh	$11, D_80152734
	lb	$12, 2024($8)
	bne	$11, $12, $2533
	mul	$13, $9, 952
	lb	$14, D_80152818+239($13)
	bne	$14, 0, $2533
	.loc	4022 125
	.loc	4022 126
	lh	$4, 1990($8)
	lw	$5, 164($sp)
	sw	$8, 160($sp)
	sw	$9, 152($sp)
	.livereg	0x0C00000E,0x00000000
	jal	car_setup_confirm
	lw	$8, 160($sp)
	lw	$9, 152($sp)
$2533:
	.loc	4022 128
	lw	$24, 40($sp)
	lbu	$15, 7($24)
	bne	$15, 6, $2549
	li	$7, 2
	lw	$25, D_8014A110
	bne	$7, $25, $2534
	bne	$9, 0, $2549
$2534:
	la	$5, D_80152014
	.loc	4022 128
	.loc	4022 129
	.noalias	$5,$sp
	lb	$2, 0($5)
	move	$6, $2
	.loc	4022 130
	lh	$3, D_80152734
	lb	$11, 2024($8)
	subu	$4, $3, $11
	bge	$2, $4, $2535
	sb	$2, 0($5)
	b	$2536
$2535:
	sb	$4, 0($5)
$2536:
	.loc	4022 132
	lb	$12, 2024($8)
	bne	$3, $12, $2546
	.noalias	$3,$5
	.noalias	$3,$sp
	mul	$13, $9, 952
	la	$14, D_80152818
	addu	$3, $13, $14
	lb	$24, 239($3)
	beq	$24, -1, $2546
	.alias	$5,$3
	.alias	$5,$sp
	li	$10, 1
	.loc	4022 132
	.loc	4022 133
	.loc	4022 134
	sb	$10, D_80110680($9)
	.loc	4022 135
	la	$15, D_8002EB90
	.set	 volatile
	l.s	$f4, 0($15)
	.set	 novolatile
	mul	$25, $9, 4
	s.s	$f4, D_80110668($25)
	.loc	4022 136
	lb	$2, 238($3)
	beq	$2, 0, $2537
	beq	$2, $10, $2539
	beq	$2, $7, $2541
	b	$2543
$2537:
	.loc	4022 138
	lb	$11, D_8010FFC0
	bne	$11, 0, $2538
	li	$2, -1
	b	$2545
$2538:
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
	.loc	4022 139
	b	$2545
$2539:
	.loc	4022 141
	lb	$12, D_8010FFC0
	bne	$12, 0, $2540
	li	$2, -1
	b	$2545
$2540:
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
	.loc	4022 142
	b	$2545
$2541:
	.loc	4022 144
	lb	$13, D_8010FFC0
	bne	$13, 0, $2542
	li	$2, -1
	b	$2545
$2542:
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
	.loc	4022 145
	b	$2545
$2543:
	.loc	4022 147
	lb	$14, D_8010FFC0
	bne	$14, 0, $2544
	li	$2, -1
	b	$2545
$2544:
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
	.loc	4022 148
$2545:
	.loc	4022 150
	sb	$10, 776($3)
	.loc	4022 151
	sb	$10, 777($3)
	.loc	4022 152
	lb	$24, 785($3)
	or	$15, $24, 1
	sb	$15, 785($3)
	.loc	4022 153
	li.s	$f6, 0.0
	s.s	$f6, 780($3)
	.loc	4022 154
	sb	$10, 2026($8)
	.loc	4022 155
	sb	$10, 2027($8)
	.loc	4022 156
	lw	$25, 2004($8)
	and	$11, $25, -9
	sw	$11, 2004($8)
	b	$2549
	.alias	$3,$sp
$2546:
	.loc	4022 157
	.noalias	$5,$sp
	lb	$2, 0($5)
	beq	$6, $2, $2549
	.alias	$5,$sp
	.loc	4022 157
	.loc	4022 158
	bne	$2, 1, $2548
	.loc	4022 158
	.loc	4022 159
	lb	$12, D_8010FFC0
	bne	$12, 0, $2547
	li	$2, -1
	b	$2549
$2547:
	li	$4, 74
	move	$5, $9
	li	$6, 1
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	b	$2549
$2548:
	.loc	4022 160
	.loc	4022 161
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
	.loc	4022 166
$2549:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 36($sp)
	addu	$sp, 160
	j	$31
	.end	race_countdown_display
