	.text
	.globl	world_effect_update
	.ent	world_effect_update 2
world_effect_update:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
$9990:
	.loc	1519 34
	.loc	1519 34
	.loc	1519 37
	sw	$0, D_8015F738
	.loc	1519 38
	sw	$0, D_80161380
	.loc	1519 39
	sw	$0, D_80161398
	.loc	1519 40
	sw	$0, D_801613A4
	.loc	1519 41
	li.s	$f4, 90.0
	s.s	$f4, D_80154188
	.loc	1519 42
	lw	$2, D_80000300
	.loc	1519 43
	bne	$2, 0, $4855
$4854:
	.loc	1519 43
	beq	$2, 0, $4854
$4855:
	.loc	1519 45
	.livereg	0x0000000E,0x00000000
	jal	get_tv_offset
	lw	$4, D_80000300
	move	$5, $2
	lw	$6, D_8002AFC0
	lw	$7, D_8002AFC4
	.livereg	0x0F00000E,0x00000000
	jal	viewport_setup
	.loc	1519 46
	move	$4, $0
	li	$5, 140832
	.livereg	0x0C00000E,0x00000000
	jal	audio_dma_sync
	la	$3, D_8002EBB0
	.noalias	$3,$sp
	sw	$2, 0($3)
	.loc	1519 47
	lw	$4, 0($3)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	audio_loop_control
	.alias	$3,$sp
	.loc	1519 48
	move	$4, $0
	li	$5, 140832
	.livereg	0x0C00000E,0x00000000
	jal	sound_play_menu
	la	$3, D_80156CEC
	.noalias	$3,$sp
	sw	$2, 0($3)
	.loc	1519 49
	lw	$4, 0($3)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	audio_loop_control
	.alias	$3,$sp
	.loc	1519 50
	move	$4, $0
	li	$5, 140832
	.livereg	0x0C00000E,0x00000000
	jal	sound_play_menu
	la	$6, D_80157240
	.noalias	$6,$sp
	sw	$2, 0($6)
	.loc	1519 51
	lw	$4, 0($6)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	audio_loop_control
	la	$2, D_80156BE0
	li	$4, -64
	li	$5, 2
	la	$6, D_80157240
	la	$7, msgq_ptr
	la	$8, D_80156CEC
	la	$9, D_8002EBB0
	.loc	1519 52
	.noalias	$9,$6
	.noalias	$9,$sp
	lw	$14, 0($9)
	addu	$15, $14, 32
	and	$24, $15, $4
	sw	$24, 0($9)
	.loc	1519 53
	.noalias	$8,$6
	.noalias	$8,$9
	.noalias	$8,$sp
	lw	$25, 0($8)
	addu	$10, $25, 32
	and	$11, $10, $4
	sw	$11, 0($8)
	.loc	1519 54
	lw	$12, 0($6)
	addu	$13, $12, 32
	and	$14, $13, $4
	sw	$14, 0($6)
	.loc	1519 55
	.noalias	$2,$6
	.noalias	$2,$8
	.noalias	$2,$9
	.noalias	$2,$sp
	lw	$15, 0($8)
	sw	$15, 124($2)
	.loc	1519 56
	lw	$24, 0($6)
	sw	$24, 252($2)
	.loc	1519 57
	sh	$5, 92($2)
	.loc	1519 58
	sh	$5, 220($2)
	.loc	1519 59
	la	$25, D_800586D0
	sw	$25, 88($2)
	.loc	1519 60
	la	$10, D_8006A090
	sw	$10, 216($2)
	.loc	1519 61
	lw	$3, 88($2)
	sw	$3, D_8015B250
	.loc	1519 62
	addu	$11, $3, 1728
	sw	$11, D_8015B260
	.loc	1519 63
	.noalias	$7,$2
	.noalias	$7,$6
	.noalias	$7,$8
	.noalias	$7,$9
	.noalias	$7,$sp
	addu	$12, $3, 40128
	sw	$12, 0($7)
$9991:
	.loc	1519 64
	lw	$13, 0($7)
	sw	$13, D_801497F4
	.loc	1519 65
	.livereg	0x0000000E,0x00000000
	jal	func_800A5B3C
	.alias	$2,$6
	.alias	$2,$7
	.alias	$2,$8
	.alias	$2,$9
	.alias	$2,$sp
	.alias	$6,$7
	.alias	$6,$8
	.alias	$6,$9
	.alias	$6,$sp
	.alias	$7,$8
	.alias	$7,$9
	.alias	$7,$sp
	.alias	$8,$9
	.alias	$8,$sp
	.alias	$9,$sp
	.loc	1519 66
	.livereg	0x0000000E,0x00000000
	jal	func_800A5488
	.loc	1519 67
	.livereg	0x0000000E,0x00000000
	jal	particle_velocity_set
	.loc	1519 68
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	world_effect_update
