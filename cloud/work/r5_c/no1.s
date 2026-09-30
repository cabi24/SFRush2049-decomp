	.verstamp	3 19
	.option	pic0
	.extern	active_player_count 2
	.extern	D_80152030 1
	.extern	D_80150F14 1
	.extern	D_80124310 4
	.extern	D_80124314 4
	.extern	D_80124318 4
	.extern	D_8012431C 4
	.text	
	.align	2
	.file	2 "w/no1.c"
	.globl	func_800DE860
	.loc	2 66
 #  66	{
	.ent	func_800DE860 2
func_800DE860:
	.option	O2
	subu	$sp, 24
	s.d	$f22, 16($sp)
	s.d	$f20, 8($sp)
	.fmask	0x00F00000, -8
	.frame	$sp, 24, $31
	.loc	2 66
	.loc	2 76
 #  76	    if ((active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0) {
	lh	$14, active_player_count
	bne	$14, 1, $32
	la	$6, D_80152030
	.noalias	$6,$sp
	lb	$15, 0($6)
	blt	$15, 5, $33
$32:
	la	$6, D_80152030
	lb	$24, D_80150F14
	bne	$24, 0, $45
	.alias	$6,$sp
$33:
	.loc	2 76
	.loc	2 77
 #  77	        for (j = 0; j < 6; j++) {
	lbu	$2, D_80153E88+7
	move	$3, $2
	beq	$3, 0, $34
	li	$5, 6
	bne	$5, $3, $35
$34:
	li	$5, 6
	li.s	$f22, 1.0
	s.s	$f22, D_8014A250+1024
$35:
	lbu	$2, D_80153E88+15
	li.s	$f22, 1.0
	move	$3, $2
	beq	$3, 0, $36
	bne	$5, $3, $37
$36:
	s.s	$f22, D_8014A250+3080
$37:
	li	$2, 2
	.loc	2 77
	.loc	2 78
 #  78	            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {
	.noalias	$3,$sp
	mul	$25, $2, 8
	la	$14, D_80153E88
	addu	$3, $25, $14
	lbu	$4, 7($3)
	beq	$4, 0, $38
	bne	$5, $4, $39
$38:
	la	$7, D_8014A250
	li	$8, 2056
	.loc	2 78
	.loc	2 79
 #  79	                D_8014A250[j].scale = 1.0f;
	.noalias	$7,$3
	.noalias	$7,$sp
	mul	$15, $2, $8
	addu	$24, $7, $15
	.noalias	$24,$3
	.noalias	$24,$sp
	s.s	$f22, 1024($24)
	.alias	$24,$3
	.alias	$24,$sp
$39:
	la	$7, D_8014A250
	li	$8, 2056
	.loc	2 77
 #  77	        for (j = 0; j < 6; j++) {
	lbu	$4, 15($3)
	beq	$4, 0, $40
	bne	$5, $4, $41
$40:
	mul	$25, $2, $8
	addu	$14, $7, $25
	.noalias	$14,$3
	.noalias	$14,$sp
	s.s	$f22, 3080($14)
	.alias	$14,$3
	.alias	$14,$sp
$41:
	lbu	$4, 23($3)
	beq	$4, 0, $42
	bne	$5, $4, $43
$42:
	mul	$15, $2, $8
	addu	$24, $7, $15
	.noalias	$24,$3
	.noalias	$24,$sp
	s.s	$f22, 5136($24)
	.alias	$24,$3
	.alias	$24,$sp
$43:
	lbu	$4, 31($3)
	beq	$4, 0, $44
	.alias	$3,$7
	.alias	$3,$sp
	bne	$5, $4, $55
$44:
	mul	$25, $2, $8
	addu	$14, $7, $25
	.noalias	$14,$sp
	s.s	$f22, 7192($14)
	.alias	$14,$sp
	.alias	$7,$sp
	b	$55
$45:
	.loc	2 82
 #  78	            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {
 #  79	                D_8014A250[j].scale = 1.0f;
 #  80	            }
 #  81	        }
 #  82	    } else {
	.loc	2 83
 #  83	        best = -1;
	li	$2, -1
	.loc	2 84
 #  84	        for (i = 0; i < 6; i++) {
	move	$3, $0
	li	$13, -1
	li	$12, 5
	li	$11, 2
	li	$10, 952
	la	$9, player_array
	li	$8, 2056
	la	$7, D_8014A250
$46:
	.loc	2 84
	.loc	2 85
 #  85	            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
	.noalias	$7,$sp
	.noalias	$4,$sp
	mul	$15, $3, $8
	addu	$4, $7, $15
	lh	$24, 1992($4)
	beq	$24, 0, $49
	.noalias	$9,$4
	.noalias	$9,$7
	.noalias	$9,$sp
	.noalias	$5,$4
	.noalias	$5,$7
	.noalias	$5,$sp
	mul	$25, $3, $10
	addu	$5, $9, $25
	lb	$14, 857($5)
	bge	$14, 2, $49
	lb	$15, 1996($4)
	beq	$11, $15, $47
	.alias	$4,$5
	.alias	$4,$9
	.alias	$4,$sp
	.noalias	$6,$5
	.noalias	$6,$7
	.noalias	$6,$9
	.noalias	$6,$sp
	lb	$24, 0($6)
	bne	$12, $24, $49
$47:
	.loc	2 86
 #  86	                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
	.loc	2 87
 #  87	                if (best == -1) {
	bne	$2, $13, $48
	.alias	$5,$6
	.alias	$5,$7
	.alias	$5,$sp
	.loc	2 87
	.loc	2 88
 #  88	                    best = i;
	sll	$2, $3, 16
	sra	$25, $2, 16
	move	$2, $25
	b	$49
$48:
	.loc	2 89
 #  89	                } else if (player_array[best].dist < player_array[i].dist) {
	.noalias	$5,$6
	.noalias	$5,$7
	.noalias	$5,$sp
	mul	$14, $2, $10
	addu	$15, $9, $14
	.noalias	$15,$6
	.noalias	$15,$7
	.noalias	$15,$sp
	l.s	$f4, 256($15)
	.alias	$15,$6
	.alias	$15,$7
	.alias	$15,$sp
	l.s	$f6, 256($5)
	c.lt.s	$f4, $f6
	bc1f	$49
	.alias	$5,$6
	.alias	$5,$7
	.alias	$5,$sp
	.loc	2 89
	.loc	2 90
 #  90	                    best = i;
	sll	$2, $3, 16
	sra	$24, $2, 16
	move	$2, $24
$49:
	.loc	2 84
 #  84	        for (i = 0; i < 6; i++) {
	addu	$3, $3, 1
	sll	$25, $3, 16
	move	$3, $25
	sra	$14, $3, 16
	move	$3, $14
	blt	$3, 6, $46
	mul	$15, $2, 952
	addu	$24, $9, $15
	.noalias	$24,$6
	.noalias	$24,$7
	.noalias	$24,$sp
	l.s	$f0, 256($24)
	.alias	$24,$6
	.alias	$24,$7
	.alias	$24,$sp
	.loc	2 94
 #  94	        k1 = D_80124310;
	l.s	$f2, D_80124310
	.loc	2 95
 #  95	        k2 = D_80124314;
	l.s	$f12, D_80124314
	.loc	2 96
 #  96	        bd = player_array[best].dist;
	mov.s	$f14, $f0
	.loc	2 97
 #  97	        for (i = 0; i < 6; i++) {
	move	$3, $0
	li.s	$f22, 1.0
$50:
	.loc	2 97
	.loc	2 98
 #  98	            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
	.noalias	$4,$6
	.noalias	$4,$9
	.noalias	$4,$sp
	mul	$25, $3, $8
	addu	$4, $7, $25
	lh	$14, 1992($4)
	beq	$14, 0, $54
	.noalias	$5,$4
	.noalias	$5,$6
	.noalias	$5,$7
	.noalias	$5,$sp
	mul	$15, $3, $10
	addu	$5, $9, $15
	lb	$24, 857($5)
	bge	$24, 2, $54
	lb	$25, 1996($4)
	beq	$11, $25, $51
	lb	$14, 0($6)
	bne	$12, $14, $54
$51:
	.loc	2 99
 #  99	                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
	.loc	2 100
 # 100	                d = bd - player_array[i].dist;
	l.s	$f8, 256($5)
	sub.s	$f0, $f14, $f8
	.loc	2 101
 # 101	                if (d > k2) {
	l.s	$f18, D_80124318
	l.s	$f20, D_8012431C
	c.lt.s	$f12, $f0
	bc1f	$52
	.alias	$5,$4
	.alias	$5,$6
	.alias	$5,$7
	.alias	$5,$sp
	.loc	2 101
	.loc	2 102
 # 102	                    s = k1 + 1.0f;
	add.s	$f16, $f2, $f22
	b	$53
$52:
	.loc	2 103
 # 103	                } else {
	.loc	2 104
 # 104	                    s = d * k1 / k2 + 1.0f;
	mul.s	$f10, $f0, $f2
	div.s	$f4, $f10, $f12
	add.s	$f16, $f4, $f22
$53:
	.loc	2 106
 # 105	                }
 # 106	                D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;
	l.s	$f6, 1024($4)
	mul.s	$f8, $f6, $f18
	mul.s	$f10, $f20, $f16
	add.s	$f4, $f8, $f10
	s.s	$f4, 1024($4)
	.alias	$4,$6
	.alias	$4,$9
	.alias	$4,$sp
$54:
	.loc	2 97
 #  97	        for (i = 0; i < 6; i++) {
	addu	$3, $3, 1
	sll	$15, $3, 16
	move	$3, $15
	sra	$24, $3, 16
	move	$3, $24
	blt	$3, 6, $50
	.alias	$6,$7
	.alias	$6,$9
	.alias	$6,$sp
	.alias	$7,$9
	.alias	$7,$sp
	.alias	$9,$sp
	.loc	2 110
 # 110	}
$55:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 8($sp)
	l.d	$f22, 16($sp)
	addu	$sp, 24
	j	$31
	.end	func_800DE860
