	.ent	music_tempo_set 2
music_tempo_set:
	.option	O3
	subu	$sp, 88
	sw	$31, 36($sp)
	sw	$18, 32($sp)
	sw	$17, 28($sp)
	sw	$16, 24($sp)
	.mask	0x80070000, -52
	.frame	$sp, 88, $31
	.loc	4045 50
	sw	$4, 88($sp)
	sw	$5, 92($sp)
	sw	$6, 96($sp)
	.loc	4045 50
	.loc	4045 56
	lw	$14, 96($sp)
	beq	$14, 0, $1675
	lbu	$15, 95($sp)
	beq	$15, 13, $1675
	.loc	4045 56
	.loc	4045 57
	.noalias	$2,$sp
	lh	$24, 90($sp)
	mul	$25, $24, 952
	la	$11, player_array
	addu	$2, $25, $11
	lw	$12, 232($2)
	and	$13, $12, 16
	beq	$13, 0, $1675
	.loc	4045 57
	.loc	4045 58
	la	$4, D_80034840
	sw	$2, 40($sp)
	.livereg	0x0800000E,0x00000000
	jal	osPfsChecker_full
	.alias	$2,$sp
	lh	$3, 90($sp)
	.loc	4045 59
	mul	$14, $3, 8
	lbu	$15, D_80153E88+7($14)
	bne	$15, 6, $1671
	lb	$2, D_80146180($3)
	bne	$2, 0, $1672
$1671:
	.loc	4045 59
	li	$2, -17
	b	$1674
$1672:
	.loc	4045 60
	bne	$2, 2, $1673
	.loc	4045 60
	li	$2, -97
	b	$1674
$1673:
	.loc	4045 61
	li	$2, -1
$1674:
	lw	$5, 40($sp)
	.loc	4045 62
	.noalias	$16,$sp
	mul	$24, $3, 2056
	la	$25, D_8014A250
	addu	$16, $24, $25
	lw	$11, 2004($16)
	and	$12, $11, $2
	sw	$12, 2004($16)
	.loc	4045 63
	.noalias	$5,$16
	.noalias	$5,$sp
	lw	$13, 232($5)
	and	$14, $13, $2
	sw	$14, 232($5)
	.loc	4045 64
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osStartThread
	.alias	$5,$16
	.alias	$5,$sp
	.alias	$16,$sp
$1675:
	.loc	4045 67
	.loc	4045 68
	lh	$15, 90($sp)
	mul	$24, $15, 952
	la	$25, player_array
	addu	$2, $24, $25
	.loc	4045 68
	.loc	4045 69
	lh	$4, 90($sp)
	lbu	$5, 95($sp)
	sw	$2, 40($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_80092B80
	la	$4, D_8012E700
	li	$5, 68
	lh	$10, 90($sp)
	.loc	4045 70
	.loc	4045 71
	.noalias	$18,$sp
	mul	$11, $10, 64
	la	$12, D_80139320
	addu	$18, $11, $12
	lw	$6, 0($18)
	sll	$7, $6, 16
	sra	$13, $7, 16
	move	$7, $13
	.noalias	$4,$18
	.noalias	$4,$sp
	mul	$14, $7, $5
	addu	$15, $4, $14
	.noalias	$15,$18
	.noalias	$15,$sp
	sh	$2, 20($15)
	.alias	$15,$18
	.alias	$15,$sp
	.loc	4045 72
	lw	$24, 40($sp)
	sw	$6, 244($24)
	.loc	4045 73
	lh	$8, 18($18)
	mul	$25, $10, 3
	sll	$11, $25, 16
	sra	$12, $11, 16
	mul	$13, $12, 2
	lhu	$9, D_801427C2($13)
	mul	$14, $8, $5
	addu	$15, $4, $14
	.noalias	$15,$18
	.noalias	$15,$sp
	sh	$9, 20($15)
	.alias	$15,$18
	.alias	$15,$sp
	.loc	4045 74
	move	$16, $0
	mul	$24, $10, 64
	la	$25, D_80139320
	addu	$17, $24, $25
	lhu	$3, D_801428F8
$1676:
	.loc	4045 74
	.noalias	$17,$4
	.noalias	$17,$sp
	mul	$11, $16, 4
	addu	$12, $17, $11
	.noalias	$12,$4
	.noalias	$12,$sp
	lh	$2, 30($12)
	.alias	$12,$4
	.alias	$12,$sp
	mul	$13, $2, $5
	addu	$14, $4, $13
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	sh	$3, 20($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	.loc	4045 74
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 4, $1676
	.alias	$4,$17
	.alias	$4,$18
	.alias	$4,$sp
	.loc	4045 75
	lb	$25, D_80156994
	bne	$25, 0, $1677
	lb	$11, D_8014978C
	blt	$11, 6, $1678
$1677:
	.loc	4045 75
	lh	$4, 30($18)
	mul	$12, $10, 2056
	lb	$13, D_8014A250+15($12)
	mul	$14, $13, 4
	lw	$5, D_80143F74($14)
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
	b	$1679
$1678:
	.loc	4045 76
	lh	$4, 30($18)
	lw	$5, D_80143F74+52
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
$1679:
	.loc	4045 77
	lw	$4, 0($18)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
	.loc	4045 78
	lw	$15, 40($sp)
	lb	$24, 861($15)
	beq	$24, 1, $1680
	.loc	4045 78
	lw	$4, 16($18)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
$1680:
	.loc	4045 79
	lw	$25, 96($sp)
	beq	$25, 0, $1683
	.loc	4045 79
	.loc	4045 80
	lw	$4, 44($18)
	li	$5, 1
	li	$6, 15
	lbu	$11, 95($sp)
	sw	$11, 44($sp)
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	4045 81
	lw	$4, 48($18)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	4045 82
	lw	$4, 52($18)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	4045 83
	lw	$4, 56($18)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	4045 84
	lw	$4, 60($18)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	4045 85
	lw	$12, 44($sp)
	beq	$12, 13, $1681
	lw	$13, 40($sp)
	lb	$14, 861($13)
	bge	$14, 2, $1683
$1681:
	.loc	4045 85
	.loc	4045 86
	move	$16, $0
$1682:
	.loc	4045 86
	mul	$15, $16, 4
	addu	$24, $17, $15
	.noalias	$24,$sp
	lw	$4, 28($24)
	.alias	$24,$sp
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	4045 86
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$11, $16, 16
	move	$16, $11
	blt	$16, 4, $1682
	.alias	$17,$sp
	.loc	4045 87
	lw	$12, 44($sp)
	bne	$12, 13, $1683
	.loc	4045 87
	.loc	4045 88
	lw	$4, 16($18)
	li	$5, 1
	li	$6, 15
	lh	$13, 90($sp)
	mul	$14, $13, 2056
	la	$15, D_8014A250
	addu	$16, $14, $15
	.livereg	0x0E00800E,0x00000000
	jal	model_data_load
	.alias	$18,$sp
	.loc	4045 89
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osPfsChecker_full
	.loc	4045 90
	.noalias	$16,$sp
	lw	$24, 2004($16)
	or	$25, $24, 16
	sw	$25, 2004($16)
	.loc	4045 91
	lw	$11, 40($sp)
	lw	$12, 232($11)
	or	$13, $12, 16
	sw	$13, 232($11)
	.loc	4045 92
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osStartThread
	.alias	$16,$sp
	.loc	4045 96
$1683:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 24($sp)
	lw	$17, 28($sp)
	lw	$18, 32($sp)
	lw	$31, 36($sp)
	addu	$sp, 88
	j	$31
	.end	music_tempo_set
