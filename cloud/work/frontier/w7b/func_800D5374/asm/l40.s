	.verstamp	3 19
	.option	pic0
	.extern	D_8014A108 2
	.extern	D_80146188 16
	.extern	D_80146170 16
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800D5374
	.loc	2 30
 #  30	{
	.ent	func_800D5374 2
func_800D5374:
	.option	O3
	subu	$sp, 56
	sw	$31, 52($sp)
	sw	$23, 48($sp)
	sw	$22, 44($sp)
	sw	$21, 40($sp)
	sw	$20, 36($sp)
	sw	$19, 32($sp)
	sw	$18, 28($sp)
	sw	$17, 24($sp)
	sw	$16, 20($sp)
	.mask	0x80FF0000, -4
	.frame	$sp, 56, $31
	.loc	2 30
	.loc	2 30
	.loc	2 34
 #  31	    s32 i;
 #  32	    s32 *obj;
 #  33	
 #  34	    for (i = 0; i < D_8014A108; i++) {
	move	$2, $0
	lh	$3, D_8014A108
	ble	$3, 0, $36
	la	$17, D_8014A118
	li	$23, 1
	la	$22, D_80146188
	li	$21, -1
	li	$20, -1
	la	$19, D_80146170
	la	$18, D_80142728
$32:
	.loc	2 34
	.loc	2 35
 #  35	        obj = (s32 *)D_8014A118[i].object;
	.noalias	$17,$sp
	lw	$16, 68($17)
	.loc	2 36
 #  36	        if (obj == (s32 *)-1) {
	bne	$16, $20, $33
	.loc	2 36
	.loc	2 40
	mul	$14, $3, 76
	la	$15, D_8014A118
	addu	$2, $14, $15
	b	$35
$33:
	.loc	2 39
 #  38	        }
 #  39	        osRecvMesg(&D_80142728, 0, 1);
	move	$4, $18
	move	$5, $0
	move	$6, $23
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 40
 #  40	        func_800D52CC(obj);
	move	$4, $16
	.livereg	0x0800000E,0x00000000
	jal	func_800D52CC
	.loc	2 41
 #  41	        if (((s8 *)obj)[9] != 0) {
	lb	$24, 9($16)
	beq	$24, 0, $34
	.loc	2 41
	.loc	2 42
 #  42	            func_8009211C(&D_80146188, obj);
	move	$4, $22
	move	$5, $16
	.livereg	0x0C00000E,0x00000000
	jal	func_8009211C
	.loc	2 43
 #  43	            ((s8 *)obj)[9] = 0;
	sb	$0, 9($16)
$34:
	.loc	2 45
 #  44	        }
 #  45	        func_80091FBC(&D_80146170, obj, D_80146170.head);
	move	$4, $19
	move	$5, $16
	.noalias	$19,$17
	.noalias	$19,$sp
	lw	$6, 8($19)
	.livereg	0x0E00000E,0x00000000
	jal	func_80091FBC
	.loc	2 46
 #  46	        ((s8 *)obj)[8] = 1;
	sb	$23, 8($16)
	.loc	2 47
 #  47	        osJamMesg(&D_80142728, 0, 0);
	move	$4, $18
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	.loc	2 48
 #  48	    next:
	lh	$3, D_8014A108
	mul	$25, $3, 76
	la	$8, D_8014A118
	addu	$2, $25, $8
$35:
	.loc	2 49
 #  49	        D_8014A118[i].object = -1;
	sw	$21, 68($17)
	.loc	2 34
 #  34	    for (i = 0; i < D_8014A108; i++) {
	addu	$17, $17, 76
	bltu	$17, $2, $32
	.alias	$17,$19
	.alias	$17,$sp
	.alias	$19,$sp
	.loc	2 51
 #  51	}
$36:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 20($sp)
	lw	$17, 24($sp)
	lw	$18, 28($sp)
	lw	$19, 32($sp)
	lw	$20, 36($sp)
	lw	$21, 40($sp)
	lw	$22, 44($sp)
	lw	$23, 48($sp)
	lw	$31, 52($sp)
	addu	$sp, 56
	j	$31
	.end	func_800D5374
