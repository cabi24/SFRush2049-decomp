	.ent	ghost_race_setup 2
ghost_race_setup:
	.option	O3
	subu	$sp, 64
	sw	$31, 60($sp)
	sw	$30, 56($sp)
	sw	$23, 52($sp)
	sw	$22, 48($sp)
	sw	$21, 44($sp)
	sw	$20, 40($sp)
	sw	$19, 36($sp)
	sw	$18, 32($sp)
	sw	$17, 28($sp)
	sw	$16, 24($sp)
	.mask	0xC0FF0000, -4
	.frame	$sp, 64, $31
	.loc	2196 32
	.loc	2196 32
	.loc	2196 38
	sw	$0, D_80140800
	.loc	2196 39
	lw	$14, D_8014A110
	bne	$14, 2, $3430
	.loc	2196 39
	.loc	2196 40
	li	$15, 1
	la	$24, D_80114738
	.set	 volatile
	sb	$15, 0($24)
	.set	 novolatile
	.loc	2196 41
	move	$19, $0
	lh	$5, D_8014A108
	ble	$5, 0, $3429
	la	$20, D_80152698
	la	$30, D_80152818
	li	$23, 2056
	la	$22, D_8014A250
	la	$21, D_80152770
$3425:
	.loc	2196 41
	.loc	2196 42
	.noalias	$20,$sp
	lw	$2, 0($20)
	beq	$2, 0, $3428
	.loc	2196 42
	.loc	2196 43
	move	$4, $2
	.loc	2196 44
	sw	$0, 0($20)
	.loc	2196 45
	.noalias	$22,$20
	.noalias	$22,$sp
	mul	$25, $19, $23
	addu	$14, $22, $25
	.noalias	$14,$20
	.noalias	$14,$sp
	sh	$0, 1820($14)
	.alias	$14,$20
	.alias	$14,$sp
	.loc	2196 46
	lw	$15, 0($4)
	lw	$24, 40($15)
	lw	$18, 0($24)
	.loc	2196 47
	.loc	2196 48
	lb	$3, 5($18)
	blt	$3, 0, $3426
	.loc	2196 48
	.loc	2196 49
	ble	$3, 0, $3428
	.loc	2196 49
	.loc	2196 50
	.livereg	0x0800000E,0x00000000
	jal	menu_transition
	lh	$5, D_8014A108
	b	$3428
$3426:
	.loc	2196 52
	.loc	2196 53
	sb	$0, 5($18)
	.loc	2196 54
	mul	$25, $19, 952
	addu	$2, $30, $25
	lb	$14, 239($2)
	beq	$14, 0, $3427
	.loc	2196 54
	.loc	2196 55
	l.s	$f4, 240($2)
	s.s	$f4, 56($18)
	.loc	2196 56
	lw	$15, 80($18)
	sw	$15, 84($18)
	.loc	2196 57
	sw	$18, D_80140800
	lh	$5, D_8014A108
	b	$3428
$3427:
	.loc	2196 58
	.loc	2196 59
	move	$4, $21
	move	$5, $0
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2196 60
	lw	$5, 76($18)
	move	$6, $0
	.livereg	0x0600000E,0x00000000
	jal	audio_reverb_update
	.loc	2196 61
	move	$4, $21
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	.loc	2196 62
	sw	$0, 76($18)
	lh	$5, D_8014A108
$3428:
	.loc	2196 41
	addu	$19, $19, 1
	addu	$20, $20, 4
	blt	$19, $5, $3425
	.alias	$20,$22
	.alias	$20,$sp
	.alias	$22,$sp
$3429:
	.loc	2196 67
	li	$24, 1
	sh	$24, D_8014A108
	.loc	2196 68
	la	$25, D_80114738
	.set	 volatile
	sb	$0, 0($25)
	.set	 novolatile
	.loc	2196 70
$3430:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 24($sp)
	lw	$17, 28($sp)
	lw	$18, 32($sp)
	lw	$19, 36($sp)
	lw	$20, 40($sp)
	lw	$21, 44($sp)
	lw	$22, 48($sp)
	lw	$23, 52($sp)
	lw	$31, 60($sp)
	lw	$30, 56($sp)
	addu	$sp, 64
	j	$31
	.end	ghost_race_setup
