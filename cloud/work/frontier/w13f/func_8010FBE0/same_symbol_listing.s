	.text
	.globl	func_8010FBE0
	.ent	func_8010FBE0 2
func_8010FBE0:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	354 54
	move	$5, $4
	.loc	354 54
	.loc	354 55
	sw	$0, D_80155238
	.loc	354 56
	la	$14, D_80152750
	sw	$14, D_80155238+80
	.loc	354 57
	sw	$0, D_80155238+84
	.loc	354 58
	li	$15, 2
	sw	$15, D_80155240
	.loc	354 59
	la	$4, D_80155248
	li	$6, 64
	.livereg	0x0E00000E,0x00000000
	jal	memcpy
	.loc	354 60
	la	$4, D_8002E960
	la	$5, D_80155238
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	.loc	354 61
	la	$4, D_8002E928
	li	$5, 670
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	.loc	354 62
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	func_8010FBE0
