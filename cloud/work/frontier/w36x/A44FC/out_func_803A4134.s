	.verstamp	3 19
	.option	pic0
	.extern	D_8014A108 2
	.extern	D_8014A110 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A4134
	.loc	2 47
 #  47	{
	.ent	func_803A4134 2
func_803A4134:
	.option	O3
	subu	$sp, 40
	sw	$31, 20($sp)
	.mask	0x80000000, -20
	.frame	$sp, 40, $31
	.loc	2 47
	.loc	2 47
	.loc	2 48
 #  48	    s32 segment = blt->AnimID & 15;
	lw	$2, 44($4)
	and	$8, $2, 15
	.loc	2 49
 #  49	    s32 bar = (blt->AnimID >> 4) & 15;
	srl	$9, $2, 4
	and	$14, $9, 15
	move	$9, $14
	.loc	2 50
 #  50	    s32 player = (blt->AnimID >> 8) & 15;
	srl	$7, $2, 8
	and	$15, $7, 15
	move	$7, $15
	.loc	2 52
 #  51	    s32 height;
 #  52	    s32 initialized = blt->AnimID >> 31;
	srl	$3, $2, 31
	.loc	2 53
 #  53	    if (!initialized) {
	bne	$3, 0, $35
	.loc	2 53
	.loc	2 54
 #  54	        if (D_8014A108 >= 3 && player < 2)
	lh	$24, D_8014A108
	blt	$24, 3, $32
	bge	$7, 2, $32
	.loc	2 55
 #  55	            func_800EF5B0(blt, D_803B85D8, 0);
	la	$5, D_803B85D8
	move	$6, $0
	sw	$4, 40($sp)
	sw	$7, 28($sp)
	sw	$8, 36($sp)
	sw	$9, 32($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_800EF5B0
	lw	$4, 40($sp)
	lw	$7, 28($sp)
	lw	$8, 36($sp)
	lw	$9, 32($sp)
	lw	$2, 44($4)
$32:
	.loc	2 56
 #  56	        blt->AnimID |= 0x80000000;
	or	$25, $2, -2147483648
	sw	$25, 44($4)
	.loc	2 57
 #  57	        if (player < D_8014A108 && D_8014A108 == 1 && D_8014A110 != 2) {
	lh	$3, D_8014A108
	bge	$7, $3, $33
	bne	$3, 1, $33
	lw	$10, D_8014A110
	beq	$10, 2, $33
	.loc	2 57
	.loc	2 58
 #  58	            blt->X = 10;
	li	$11, 10
	sh	$11, 14($4)
	.loc	2 59
 #  59	            height = blt->Height / 8;
	lh	$2, 22($4)
	div	$12, $2, 8
	move	$2, $12
	.loc	2 60
 #  60	            blt->Y = 48 + bar * 2 * height;
	mul	$3, $9, 2
	mul	$13, $3, $2
	addu	$14, $13, 48
	sh	$14, 16($4)
	b	$34
$33:
	.loc	2 61
 #  61	        } else {
	.loc	2 62
 #  62	            input_new_data_wrapper(blt, 1);
	li	$5, 1
	sw	$4, 40($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$4, 40($sp)
	.loc	2 63
 #  63	            blt->AnimFunc = 0;
	sw	$0, 40($4)
	.loc	2 64
 #  64	            return 1;
	li	$2, 1
	b	$40
$34:
	.loc	2 66
 #  65	        }
 #  66	        blt->Left = 0;
	sh	$0, 32($4)
	.loc	2 67
 #  67	        blt->Top = (bar * 2 + (segment == 0)) * height;
	seq	$15, $8, 0
	addu	$24, $3, $15
	mul	$25, $24, $2
	sh	$25, 28($4)
	.loc	2 68
 #  68	        blt->Bot = blt->Top + height - 1;
	lh	$10, 28($4)
	addu	$11, $10, $2
	addu	$12, $11, -1
	sh	$12, 30($4)
$35:
	.loc	2 70
 #  69	    }
 #  70	    if (input_new_data_wrapper(blt, D_803BA028[player] == 1))
	lb	$5, D_803BA028($7)
	seq	$13, $5, 1
	move	$5, $13
	sw	$4, 40($sp)
	sw	$7, 28($sp)
	sw	$8, 36($sp)
	sw	$9, 32($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$4, 40($sp)
	lw	$7, 28($sp)
	lw	$8, 36($sp)
	lw	$9, 32($sp)
	beq	$2, 0, $36
	.loc	2 71
 #  71	        return 1;
	li	$2, 1
	b	$40
$36:
	.loc	2 72
 #  72	    if (segment == 1) {
	bne	$8, 1, $38
	.loc	2 72
	.loc	2 73
 #  73	        blt->Right = D_803BA190[player][bar] * blt->Width - 1;
	mul	$14, $7, 16
	mul	$15, $9, 4
	addu	$24, $14, $15
	l.s	$f4, D_803BA190($24)
	lh	$25, 20($4)
	mtc1	$25, $f6
	cvt.s.w	$f8, $f6
	mul.s	$f10, $f4, $f8
	li.s	$f16, 1.0000000000000000e+00
	sub.s	$f18, $f10, $f16
	trunc.w.s	$f6, $f18, $10
	mfc1	$11, $f6
	sh	$11, 34($4)
	.loc	2 74
 #  74	        blt->Right = blt->Right < 2 ? 2 : blt->Right;
	lh	$2, 34($4)
	bge	$2, 2, $37
	li	$12, 2
	sh	$12, 34($4)
	b	$39
$37:
	sh	$2, 34($4)
	b	$39
$38:
	.loc	2 76
 #  75	    } else
 #  76	        blt->Right = blt->Width - 1;
	lh	$13, 20($4)
	addu	$14, $13, -1
	sh	$14, 34($4)
$39:
	.loc	2 77
 #  77	    Input_ApplyPadConfig(blt);
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.loc	2 78
 #  78	    return 1;
	li	$2, 1
$40:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 40
	j	$31
	.end	func_803A4134
