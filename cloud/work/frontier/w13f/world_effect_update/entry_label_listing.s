	.text
	.globl	world_effect_update
	.ent	world_effect_update 2
world_effect_update:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
$9999:
	.loc	1384 19
	.loc	1384 19
	.loc	1384 20
	sw	$0, D_8015F738
	.loc	1384 21
	sw	$0, D_80161380
	.loc	1384 22
	sw	$0, D_80161398
	.loc	1384 23
	sw	$0, D_801613A4
	.loc	1384 24
	li.s	$f4, 90.0
	s.s	$f4, D_80154188
	.loc	1384 25
	lw	$2, D_80000300
	bne	$2, 0, $4430
$4429:
	.loc	1384 25
	beq	$2, 0, $4429
$4430:
	.loc	1384 26
	.livereg	0x0000000E,0x00000000
	jal	get_tv_offset
	lw	$4, D_80000300
	move	$5, $2
	lw	$6, D_8002AFC0
	lw	$7, D_8002AFC4
	.livereg	0x0F00000E,0x00000000
	jal	viewport_setup
	.loc	1384 27
	move	$4, $0
	li	$5, 140832
	.livereg	0x0C00000E,0x00000000
	jal	audio_dma_sync
	la	$3, D_8002EBB0
	.noalias	$3,$sp
	sw	$2, 0($3)
	.loc	1384 28
	lw	$4, 0($3)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	audio_loop_control
	.alias	$3,$sp
	.loc	1384 29
	move	$4, $0
	li	$5, 140832
	.livereg	0x0C00000E,0x00000000
	jal	sound_play_menu
	la	$3, D_80156CEC
	.noalias	$3,$sp
	sw	$2, 0($3)
	.loc	1384 30
	lw	$4, 0($3)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	audio_loop_control
	.alias	$3,$sp
	.loc	1384 31
	move	$4, $0
	li	$5, 140832
	.livereg	0x0C00000E,0x00000000
	jal	sound_play_menu
	la	$6, D_80157240
	.noalias	$6,$sp
	sw	$2, 0($6)
	.loc	1384 32
	lw	$4, 0($6)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	audio_loop_control
	la	$2, D_80156BE0
	li	$4, -64
	li	$5, 2
	la	$6, D_80157240
	la	$7, D_800586D0
	la	$8, D_8015B250
	la	$9, D_801497C8
	la	$10, D_80156CEC
	la	$11, D_8002EBB0
	.loc	1384 33
	.noalias	$10,$6
	.noalias	$10,$sp
	lw	$14, 0($10)
	addu	$15, $14, 32
	and	$24, $15, $4
	sw	$24, 0($10)
	.noalias	$2,$6
	.noalias	$2,$10
	.noalias	$2,$sp
	lw	$25, 0($10)
	sw	$25, 124($2)
	.loc	1384 34
	.noalias	$11,$2
	.noalias	$11,$6
	.noalias	$11,$10
	.noalias	$11,$sp
	lw	$12, 0($11)
	addu	$13, $12, 32
	and	$14, $13, $4
	sw	$14, 0($11)
	.loc	1384 35
	lw	$15, 0($6)
	addu	$24, $15, 32
	and	$25, $24, $4
	sw	$25, 0($6)
	lw	$12, 0($6)
	sw	$12, 252($2)
	.loc	1384 36
	sh	$5, 92($2)
	.loc	1384 37
	sh	$5, 220($2)
	.loc	1384 38
	sw	$7, 88($2)
	.loc	1384 39
	la	$13, D_8006A090
	sw	$13, 216($2)
	.loc	1384 40
	.noalias	$8,$2
	.noalias	$8,$6
	.noalias	$8,$10
	.noalias	$8,$11
	.noalias	$8,$sp
	sw	$7, 0($8)
	.loc	1384 41
	lw	$3, 0($8)
	addu	$14, $3, 1728
	sw	$14, D_8015B260
	.loc	1384 42
	.noalias	$9,$2
	.noalias	$9,$6
	.noalias	$9,$8
	.noalias	$9,$10
	.noalias	$9,$11
	.noalias	$9,$sp
	addu	$15, $3, 40128
	.set	 volatile
	sw	$15, 0($9)
	.set	 novolatile
	.loc	1384 43
	.set	 volatile
	lw	$24, 0($9)
	.set	 novolatile
	sw	$24, D_801497F4
	.loc	1384 44
	.livereg	0x0000000E,0x00000000
	jal	func_800A5B3C
	.alias	$2,$6
	.alias	$2,$8
	.alias	$2,$9
	.alias	$2,$10
	.alias	$2,$11
	.alias	$2,$sp
	.alias	$6,$8
	.alias	$6,$9
	.alias	$6,$10
	.alias	$6,$11
	.alias	$6,$sp
	.alias	$8,$9
	.alias	$8,$10
	.alias	$8,$11
	.alias	$8,$sp
	.alias	$9,$10
	.alias	$9,$11
	.alias	$9,$sp
	.alias	$10,$11
	.alias	$10,$sp
	.alias	$11,$sp
	.loc	1384 45
	.livereg	0x0000000E,0x00000000
	jal	func_800A5488
	.loc	1384 46
	.livereg	0x0000000E,0x00000000
	jal	particle_velocity_set
	.loc	1384 47
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	world_effect_update
