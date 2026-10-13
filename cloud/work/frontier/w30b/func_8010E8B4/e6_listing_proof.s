	.text
	.globl	func_8010E8B4
	.ent	func_8010E8B4 2
func_8010E8B4:
	.option	O3
	subu	$sp, 96
	sw	$31, 60($sp)
	sw	$20, 56($sp)
	sw	$19, 52($sp)
	sw	$18, 48($sp)
	sw	$17, 44($sp)
	sw	$16, 40($sp)
	.mask	0x801F0000, -36
	.frame	$sp, 96, $31
	.loc	4873 114
	sw	$4, 96($sp)
	.loc	4873 114
	.loc	4873 120
	lb	$14, D_8011AD38
	bne	$14, 0, $2828
	.loc	4873 120
	.loc	4873 121
	lw	$15, D_801174B4
	and	$24, $15, 2080636926
	bne	$24, 0, $2827
	.loc	4873 121
	.loc	4873 122
	la	$4, D_801461D0
	move	$5, $0
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	li	$18, 13
	.livereg	0x0000200E,0x00000000
	jal	slot_state_setup
	sw	$2, 68($sp)
	la	$4, D_801461D0
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osSendMesg
	lw	$2, 68($sp)
	.loc	4873 123
	lw	$25, D_8015698C
	mul	$14, $25, 16
	lb	$2, D_80156CF0($14)
	.loc	4873 124
	seq	$15, $2, 0
	sb	$15, D_8011AD6C
	.loc	4873 125
	.livereg	0x0000000E,0x00000000
	jal	TitlePane
	.loc	4873 126
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	la	$9, D_8011AD70
	.loc	4873 126
	.loc	4873 126
	.noalias	$9,$sp
	lw	$3, 0($9)
	addu	$4, $3, -8
	mul	$8, $2, 2
	addu	$8, $8, 10
	sll	$24, $8, 16
	move	$10, $24
	sra	$25, $10, 16
	move	$10, $25
	addu	$5, $10, -4
	lw	$14, 4($9)
	addu	$6, $3, $14
	addu	$6, $6, 8
	lw	$15, 8($9)
	addu	$7, $15, $10
	addu	$7, $7, 4
	li	$24, 176
	sw	$24, 16($sp)
	sw	$0, 20($sp)
	sw	$0, 24($sp)
	sw	$0, 28($sp)
	.livereg	0x1F80000E,0x00000000
	jal	ambient_sound_set
	.alias	$9,$sp
$2827:
	.loc	4873 128
	li	$4, 20
	li	$5, 136
	li	$6, 300
	li	$7, 204
	li	$25, 176
	sw	$25, 16($sp)
	sw	$0, 20($sp)
	sw	$0, 24($sp)
	sw	$0, 28($sp)
	.livereg	0x0F00000E,0x00000000
	jal	ambient_sound_set
	.loc	4873 129
	li	$14, 1
	sb	$14, D_8011AD38
$2828:
	.loc	4873 131
	li	$2, 1
	.livereg	0x2000FF0E,0x00000FFF
	lw	$16, 40($sp)
	lw	$17, 44($sp)
	lw	$18, 48($sp)
	lw	$19, 52($sp)
	lw	$20, 56($sp)
	lw	$31, 60($sp)
	addu	$sp, 96
	j	$31
	.end	func_8010E8B4
