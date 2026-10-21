	.verstamp	3 19
	.option	pic0
	.extern	D_8014A108 2
	.extern	D_8014A110 4
	.extern	D_80140BDC 1
	.extern	D_8017A4E0 20
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A4340
	.loc	2 40
 #  40	{
	.ent	func_803A4340 2
func_803A4340:
	.option	O3
	subu	$sp, 112
	sw	$31, 68($sp)
	sw	$30, 64($sp)
	sw	$23, 60($sp)
	sw	$22, 56($sp)
	sw	$21, 52($sp)
	sw	$20, 48($sp)
	sw	$19, 44($sp)
	sw	$18, 40($sp)
	sw	$17, 36($sp)
	sw	$16, 32($sp)
	.mask	0xC0FF0000, -44
	.frame	$sp, 112, $31
	.loc	2 40
	sw	$4, 112($sp)
	.loc	2 40
	.loc	2 47
 #  47	    render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 48
 #  48	    object_create(12);
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 49
 #  49	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 50
 #  50	    for (i = 0; i < D_8014A108; i++) {
	move	$21, $0
	lh	$2, D_8014A108
	ble	$2, 0, $36
	la	$14, D_803BA028
	sw	$14, 76($sp)
	la	$30, D_8014A110
	li	$23, 2
	li	$22, 1
	la	$20, D_8017A4E0
$32:
	.loc	2 50
	.loc	2 51
 #  51	        if (D_803BA028[i] == 1) {
	lw	$15, 76($sp)
	lb	$24, 0($15)
	beq	$22, $24, $35
	.loc	2 51
	.loc	2 52
 #  52	            continue;
	.loc	2 54
 #  53	        }
 #  54	        for (j = 0; j < 4; j++) {
	move	$16, $0
$33:
	.loc	2 54
	.loc	2 55
 #  55	            if (i < D_8014A108 && D_8014A108 == 1 && D_8014A110 != 2) {
	bge	$21, $2, $34
	bne	$22, $2, $34
	.noalias	$30,$sp
	lw	$25, 0($30)
	beq	$23, $25, $34
	.loc	2 55
	.loc	2 56
 #  56	                texture = func_800B24EC(D_803B85E8, &index, 0, D_80140BDC - 1, 1);
	la	$4, D_803B85E8
	addu	$5, $sp, 102
	move	$6, $0
	la	$8, D_80140BDC
	.set	 volatile
	lbu	$7, 0($8)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$9, $7, 24
	move	$7, $9
	sra	$10, $7, 24
	move	$7, $10
	sw	$22, 16($sp)
	mul	$18, $16, 2
	mul	$19, $16, 4
	.livereg	0x0F00300E,0x00000000
	jal	func_800B24EC
	.loc	2 57
 #  57	                y = 48 + 2 * j * (texture->height / 8);
	lhu	$11, 18($2)
	div	$12, $11, 8
	mul	$17, $18, $12
	addu	$17, $17, 48
	.loc	2 58
 #  58	                state_utility(10, y - object_bytes_sum_global(), D_8017A4E0.text[D_8017A4E0.indices->first + j]);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	li	$4, 10
	subu	$5, $17, $2
	sll	$13, $5, 16
	move	$5, $13
	sra	$14, $5, 16
	move	$5, $14
	.noalias	$20,$30
	.noalias	$20,$sp
	lw	$15, 16($20)
	lw	$24, 12($20)
	lhu	$25, 26($24)
	mul	$8, $25, 4
	addu	$9, $15, $8
	addu	$10, $9, $19
	lw	$6, 0($10)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	lh	$2, D_8014A108
$34:
	.loc	2 54
 #  54	        for (j = 0; j < 4; j++) {
	addu	$16, $16, 1
	bne	$16, 4, $33
	.loc	2 50
 #  50	    for (i = 0; i < D_8014A108; i++) {
$35:
	addu	$21, $21, 1
	lw	$11, 76($sp)
	addu	$12, $11, 1
	sw	$12, 76($sp)
	blt	$21, $2, $32
	.alias	$20,$30
	.alias	$20,$sp
	.alias	$30,$sp
$36:
	.loc	2 62
 #  62	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 63
 #  63	    return 1;
	li	$2, 1
	.livereg	0x2000FF0E,0x00000FFF
	lw	$16, 32($sp)
	lw	$17, 36($sp)
	lw	$18, 40($sp)
	lw	$19, 44($sp)
	lw	$20, 48($sp)
	lw	$21, 52($sp)
	lw	$22, 56($sp)
	lw	$23, 60($sp)
	lw	$31, 68($sp)
	lw	$30, 64($sp)
	addu	$sp, 112
	j	$31
	.end	func_803A4340
