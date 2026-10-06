	.text
	.globl	func_80096130
	.ent	func_80096130 2
func_80096130:
	.option	O3
	subu	$sp, 72
	sw	$31, 28($sp)
	sw	$17, 24($sp)
	sw	$16, 20($sp)
	.mask	0x80030000, -44
	.frame	$sp, 72, $31
	.loc	6075 20
	sw	$4, 72($sp)
	.loc	6075 20
	.loc	6075 21
	.loc	6075 23
	.noalias	$3,$sp
	lw	$14, 72($sp)
	mul	$15, $14, 20
	la	$24, D_80156D38
	addu	$3, $15, $24
	lw	$7, 12($3)
	beq	$7, 0, $734
	la	$2, D_8002EB70
	.loc	6075 23
	.loc	6075 24
	.loc	6075 14
	.loc	6075 15
	.noalias	$2,$3
	.noalias	$2,$sp
	.set	 volatile
	lh	$25, 0($2)
	.set	 novolatile
	beq	$25, 0, $733
$732:
	.loc	6075 15
	.set	 volatile
	lh	$14, 0($2)
	.set	 novolatile
	bne	$14, 0, $732
	.alias	$2,$3
	.alias	$2,$sp
$733:
	.loc	6075 17
	.loc	6075 25
	sw	$7, 56($sp)
	la	$4, D_80152770
	move	$5, $0
	li	$6, 1
	sw	$3, 36($sp)
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	lw	$3, 36($sp)
	lw	$5, 56($sp)
	move	$6, $0
	.livereg	0x0600000E,0x00000000
	jal	audio_reverb_update
	lw	$3, 36($sp)
	la	$4, D_80152770
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	.alias	$3,$sp
	lw	$3, 36($sp)
	.loc	6075 26
	.loc	6075 26
	.loc	6075 27
	sw	$0, 12($3)
	.loc	6075 28
	lw	$2, 72($sp)
	mul	$15, $2, 8
	move	$2, $15
	la	$24, D_801161F4
	addu	$4, $2, $24
	move	$5, $0
	li	$6, 8
	sw	$2, 32($sp)
	.livereg	0x2E00000E,0x00000000
	jal	memset
	.alias	$3,$sp
	.loc	6075 29
	lw	$25, 32($sp)
	la	$14, D_80151AE8
	addu	$4, $25, $14
	move	$5, $0
	li	$6, 8
	.livereg	0x0E00000E,0x00000000
	jal	memset
	.loc	6075 30
	lw	$15, 32($sp)
	la	$24, D_80138670
	addu	$4, $15, $24
	move	$5, $0
	li	$6, 8
	.livereg	0x0E00000E,0x00000000
	jal	memset
	.loc	6075 32
$734:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 20($sp)
	lw	$17, 24($sp)
	lw	$31, 28($sp)
	addu	$sp, 72
	j	$31
	.end	func_80096130
