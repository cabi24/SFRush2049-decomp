	.text
	.globl	countdown_handler
	.ent	countdown_handler 2
countdown_handler:
	.option	O3
	subu	$sp, 344
	sw	$31, 44($sp)
	.mask	0x80000000, -300
	.frame	$sp, 344, $31
	.loc	1800 43
	.loc	1800 43
	.loc	1800 50
	lw	$14, gameplay_mode
	bne	$14, 2, $7056
	li	$16, 1
	b	$7057
$7056:
	lh	$16, D_8014A108
$7057:
	.loc	1800 51
	move	$3, $0
	ble	$16, 0, $7059
	la	$2, D_80156CF0
$7058:
	.loc	1800 51
	.loc	1800 52
	.noalias	$2,$sp
	lb	$15, 0($2)
	beq	$15, 0, $7059
	.alias	$2,$sp
	.loc	1800 52
	.loc	1800 51
	addu	$3, $3, 1
	addu	$2, $2, 16
	bne	$3, $16, $7058
$7059:
	.loc	1800 55
	blt	$3, $16, $7060
	.loc	1800 55
	.loc	1800 56
	sw	$3, D_801613B0
	.loc	1800 57
	li	$16, 320
	.loc	1800 58
	sw	$16, D_801146F8
	b	$7064
$7060:
	addu	$17, $sp, 80
	.loc	1800 60
	sw	$3, D_801613B0
	.loc	1800 61
	move	$4, $17
	la	$5, $$1809
	addu	$6, $3, 1
	.livereg	0x0E00000E,0x00000000
	jal	sprintf
	la	$18, countdown_state
	addu	$20, $sp, 88
	.loc	1800 62
	move	$4, $20
	la	$5, $$1810
	.noalias	$18,$sp
	lw	$3, 4($18)
	lw	$6, 496($3)
	move	$7, $17
	lw	$24, 500($3)
	sw	$24, 16($sp)
	sw	$17, 20($sp)
	lw	$25, 504($3)
	sw	$25, 24($sp)
	sw	$16, 28($sp)
	lw	$14, 508($3)
	sw	$14, 32($sp)
	.livereg	0x1F00000E,0x00000000
	jal	sprintf
	.loc	1800 64
	move	$4, $20
	lw	$15, 4($18)
	lw	$5, 512($15)
	.livereg	0x0C00000E,0x00000000
	jal	func_800A4770
	.alias	$18,$sp
	la	$21, D_801461D0
	.loc	1800 65
	.loc	1800 33
	.loc	1800 36
	.loc	1800 28
	.loc	1800 29
	move	$4, $21
	move	$5, $0
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	1800 30
	.loc	1800 37
	li	$18, 11
	.livereg	0x0000200E,0x00000000
	jal	slot_state_setup
	move	$16, $2
	.loc	1800 38
	move	$4, $21
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osSendMesg
	.loc	1800 39
	move	$2, $16
	.loc	1800 66
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	1800 67
	lw	$2, state_word_a
	and	$24, $2, 1
	bne	$24, 0, $7063
	and	$25, $2, 2
	bne	$25, 0, $7063
	.loc	1800 67
	.loc	1800 68
	and	$14, $2, 1073741824
	bne	$14, 0, $7061
	and	$15, $2, 4
	bne	$15, 0, $7061
	and	$24, $2, 8
	beq	$24, 0, $7062
$7061:
	lw	$16, D_801146F8
	.loc	1800 68
	.loc	1800 69
	sll	$4, $16, 16
	sra	$25, $4, 16
	move	$4, $25
	li	$5, 8
	move	$6, $20
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$7063
$7062:
	lw	$16, D_801146F8
	.loc	1800 70
	.loc	1800 71
	sll	$4, $16, 16
	sra	$14, $4, 16
	move	$4, $14
	li	$5, 205
	move	$6, $20
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$7063:
	lw	$16, D_801146F8
	.loc	1800 74
	addu	$16, $16, -1
	move	$4, $20
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sw	$16, D_801146F8
	negu	$15, $2
	bge	$16, $15, $7064
	.loc	1800 74
	.loc	1800 75
	li	$16, 320
	sw	$16, D_801146F8
	.loc	1800 77
$7064:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 44($sp)
	addu	$sp, 344
	j	$31
	.end	countdown_handler
