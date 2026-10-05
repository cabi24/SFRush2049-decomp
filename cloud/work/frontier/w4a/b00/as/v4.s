	.set	noat
	.text
	.align 2
	.globl func_80091B00
	.ent	func_80091B00 2
func_80091B00:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 9
	.loc	2 9
	.loc	2 11
	move	$2, $0
	la	$3, D_80142DD8
	la	$2, D_80142DD8+3072
$487:
	.loc	2 11
	.loc	2 12
	.noalias	$3,$sp
	lb	$14, 3($3)
	bne	$14, 0, $488
	.loc	2 12
	.loc	2 13
	li	$15, 1
	.set	volatile
	sb	$15, 3($3)
	.set	novolatile
	.loc	2 14
	li	$24, -1
	sh	$24, 0($3)
	.loc	2 15
	move	$2, $3
	.livereg	0x2000FF0E,0x00000FFF
	j	$31
$488:
	.loc	2 11
	lb	$25, 27($3)
	bne	$25, 0, $489
	li	$14, 1
	.set	volatile
	sb	$14, 27($3)
	.set	novolatile
	li	$15, -1
	sh	$15, 24($3)
	addu	$2, $3, 24
	.livereg	0x2000FF0E,0x00000FFF
	j	$31
$489:
	lb	$24, 51($3)
	bne	$24, 0, $490
	li	$25, 1
	.set	volatile
	sb	$25, 51($3)
	.set	novolatile
	li	$14, -1
	sh	$14, 48($3)
	addu	$2, $3, 48
	.livereg	0x2000FF0E,0x00000FFF
	j	$31
$490:
	lb	$15, 75($3)
	bne	$15, 0, $491
	li	$24, 1
	.set	volatile
	sb	$24, 75($3)
	.set	novolatile
	li	$25, -1
	sh	$25, 72($3)
	addu	$2, $3, 72
	.livereg	0x2000FF0E,0x00000FFF
	j	$31
$491:
	addu	$3, $3, 96
	bne	$3, $2, $487
	.alias	$3,$sp
	.loc	2 18
	move	$2, $0
$492:
	.livereg	0x2000FF0E,0x00000FFF
	j	$31
	.end	func_80091B00
