	.verstamp	3 19
	.option	pic0
	.extern	active_player_count 2
	.extern	D_80152030 1
	.extern	D_80150F14 1
	.extern	D_80124314 4
	.extern	D_80124310 4
	.extern	D_80124318 4
	.extern	D_8012431C 4
	.text	
	.align	2
	.file	2 "w/nok.c"
	.globl	func_800DE860
	.loc	2 66
 #  66	{
	.ent	func_800DE860 2
func_800DE860:
	.option	O2
	subu	$sp, 24
	sw	$16, 20($sp)
	s.d	$f22, 8($sp)
	s.d	$f20, 0($sp)
	.mask	0x00010000, -4
	.fmask	0x00F00000, -16
	.frame	$sp, 24, $31
	li	$7, 1
	.loc	2 66
	.loc	2 74
 #  74	    if ((active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0) {
	lh	$14, active_player_count
	bne	$7, $14, $32
	la	$8, D_80152030
	.noalias	$8,$sp
	lb	$15, 0($8)
	blt	$15, 5, $33
$32:
	la	$8, D_80152030
	lb	$2, D_80150F14
	bne	$2, 0, $45
	.alias	$8,$sp
$33:
	.loc	2 74
	.loc	2 75
 #  75	        for (j = 0; j < 6; j++) {
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
	.loc	2 75
	.loc	2 76
 #  76	            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {
	.noalias	$3,$sp
	mul	$24, $2, 8
	la	$25, D_80153E88
	addu	$3, $24, $25
	lbu	$4, 7($3)
	beq	$4, 0, $38
	bne	$5, $4, $39
$38:
	la	$9, D_8014A250
	li	$10, 2056
	.loc	2 76
	.loc	2 77
 #  77	                D_8014A250[j].scale = 1.0f;
	.noalias	$9,$3
	.noalias	$9,$sp
	mul	$14, $2, $10
	addu	$15, $9, $14
	.noalias	$15,$3
	.noalias	$15,$sp
	s.s	$f22, 1024($15)
	.alias	$15,$3
	.alias	$15,$sp
$39:
	la	$9, D_8014A250
	li	$10, 2056
	.loc	2 75
 #  75	        for (j = 0; j < 6; j++) {
	lbu	$4, 15($3)
	beq	$4, 0, $40
	bne	$5, $4, $41
$40:
	mul	$24, $2, $10
	addu	$25, $9, $24
	.noalias	$25,$3
	.noalias	$25,$sp
	s.s	$f22, 3080($25)
	.alias	$25,$3
	.alias	$25,$sp
$41:
	lbu	$4, 23($3)
	beq	$4, 0, $42
	bne	$5, $4, $43
$42:
	mul	$14, $2, $10
	addu	$15, $9, $14
	.noalias	$15,$3
	.noalias	$15,$sp
	s.s	$f22, 5136($15)
	.alias	$15,$3
	.alias	$15,$sp
$43:
	lbu	$4, 31($3)
	beq	$4, 0, $44
	.alias	$3,$9
	.alias	$3,$sp
	bne	$5, $4, $56
$44:
	mul	$24, $2, $10
	addu	$25, $9, $24
	.noalias	$25,$sp
	s.s	$f22, 7192($25)
	.alias	$25,$sp
	.alias	$9,$sp
	b	$56
$45:
	.loc	2 80
 #  76	            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {
 #  77	                D_8014A250[j].scale = 1.0f;
 #  78	            }
 #  79	        }
 #  80	    } else {
	.loc	2 81
 #  81	        best = -1;
	li	$3, -1
	.loc	2 82
 #  82	        for (i = 0; i < 6; i++) {
	move	$4, $0
	li	$16, 5
	li	$13, 2
	li	$12, 952
	la	$11, player_array
	li	$10, 2056
	la	$9, D_8014A250
$46:
	.loc	2 82
	.loc	2 83
 #  83	            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
	.noalias	$9,$sp
	.noalias	$5,$sp
	mul	$14, $4, $10
	addu	$5, $9, $14
	lh	$15, 1992($5)
	beq	$15, 0, $49
	.noalias	$11,$5
	.noalias	$11,$9
	.noalias	$11,$sp
	.noalias	$6,$5
	.noalias	$6,$9
	.noalias	$6,$sp
	mul	$24, $4, $12
	addu	$6, $11, $24
	lb	$25, 857($6)
	bge	$25, 2, $49
	lb	$14, 1996($5)
	beq	$13, $14, $47
	.alias	$5,$6
	.alias	$5,$11
	.alias	$5,$sp
	.noalias	$8,$6
	.noalias	$8,$9
	.noalias	$8,$11
	.noalias	$8,$sp
	lb	$15, 0($8)
	bne	$16, $15, $49
$47:
	.loc	2 84
 #  84	                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
	.loc	2 85
 #  85	                if (best == -1) {
	bne	$3, -1, $48
	.alias	$6,$8
	.alias	$6,$9
	.alias	$6,$sp
	.loc	2 85
	.loc	2 86
 #  86	                    best = i;
	sll	$3, $4, 16
	sra	$24, $3, 16
	move	$3, $24
	b	$49
$48:
	.loc	2 87
 #  87	                } else if (player_array[best].dist < player_array[i].dist) {
	.noalias	$6,$8
	.noalias	$6,$9
	.noalias	$6,$sp
	mul	$25, $3, $12
	addu	$14, $11, $25
	.noalias	$14,$8
	.noalias	$14,$9
	.noalias	$14,$sp
	l.s	$f4, 256($14)
	.alias	$14,$8
	.alias	$14,$9
	.alias	$14,$sp
	l.s	$f6, 256($6)
	c.lt.s	$f4, $f6
	bc1f	$49
	.alias	$6,$8
	.alias	$6,$9
	.alias	$6,$sp
	.loc	2 87
	.loc	2 88
 #  88	                    best = i;
	sll	$3, $4, 16
	sra	$15, $3, 16
	move	$3, $15
$49:
	.loc	2 82
 #  82	        for (i = 0; i < 6; i++) {
	addu	$4, $4, 1
	sll	$24, $4, 16
	move	$4, $24
	sra	$25, $4, 16
	move	$4, $25
	blt	$4, 6, $46
	mul	$14, $3, 952
	addu	$15, $11, $14
	.noalias	$15,$8
	.noalias	$15,$9
	.noalias	$15,$sp
	l.s	$f0, 256($15)
	.alias	$15,$8
	.alias	$15,$9
	.alias	$15,$sp
	.loc	2 92
 #  92	        bd = player_array[best].dist;
	mov.s	$f2, $f0
	.loc	2 93
 #  93	        for (i = 0; i < 6; i++) {
	move	$4, $0
	li.s	$f22, 1.0
	la	$3, D_80124314
$50:
	.loc	2 93
	.loc	2 94
 #  94	            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
	.noalias	$5,$8
	.noalias	$5,$11
	.noalias	$5,$sp
	mul	$24, $4, $10
	addu	$5, $9, $24
	lh	$25, 1992($5)
	beq	$25, 0, $55
	.noalias	$6,$5
	.noalias	$6,$8
	.noalias	$6,$9
	.noalias	$6,$sp
	mul	$14, $4, $12
	addu	$6, $11, $14
	lb	$15, 857($6)
	bge	$15, 2, $55
	lb	$24, 1996($5)
	beq	$13, $24, $51
	lb	$25, 0($8)
	bne	$16, $25, $55
$51:
	.loc	2 95
 #  95	                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
	.loc	2 96
 #  96	                d = bd - player_array[i].dist;
	l.s	$f8, 256($6)
	sub.s	$f0, $f2, $f8
	.loc	2 97
 #  97	                if (d > D_80124314) {
	.noalias	$3,$5
	.noalias	$3,$6
	.noalias	$3,$8
	.noalias	$3,$9
	.noalias	$3,$11
	.noalias	$3,$sp
	l.s	$f12, 0($3)
	l.s	$f16, D_80124310
	l.s	$f18, D_80124318
	l.s	$f20, D_8012431C
	c.lt.s	$f12, $f0
	bc1f	$52
	.alias	$6,$3
	.alias	$6,$5
	.alias	$6,$8
	.alias	$6,$9
	.alias	$6,$sp
	.loc	2 97
	.loc	2 98
 #  98	                    s = D_80124310 + 1.0f;
	add.s	$f14, $f16, $f22
	b	$53
$52:
	.loc	2 99
 #  99	                } else {
	.loc	2 100
 # 100	                    s = d * D_80124310 / D_80124314 + 1.0f;
	mul.s	$f10, $f0, $f16
	div.s	$f4, $f10, $f12
	add.s	$f14, $f4, $f22
$53:
	.loc	2 102
 # 101	                }
 # 102	                if (D_80150F14 == 1) {
	bne	$7, $2, $54
	.loc	2 102
	.loc	2 103
 # 103	                    s = (1.0f - s) * 0.5f + 1.0f;
	sub.s	$f6, $f22, $f14
	li.s	$f8, 0.5
	mul.s	$f10, $f6, $f8
	add.s	$f14, $f10, $f22
$54:
	.loc	2 105
 # 104	                }
 # 105	                D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;
	l.s	$f4, 1024($5)
	mul.s	$f6, $f4, $f18
	mul.s	$f8, $f20, $f14
	add.s	$f10, $f6, $f8
	s.s	$f10, 1024($5)
	.alias	$5,$3
	.alias	$5,$8
	.alias	$5,$11
	.alias	$5,$sp
$55:
	.loc	2 93
 #  93	        for (i = 0; i < 6; i++) {
	addu	$4, $4, 1
	sll	$14, $4, 16
	move	$4, $14
	sra	$15, $4, 16
	move	$4, $15
	blt	$4, 6, $50
	.alias	$3,$8
	.alias	$3,$9
	.alias	$3,$11
	.alias	$3,$sp
	.alias	$8,$9
	.alias	$8,$11
	.alias	$8,$sp
	.alias	$9,$11
	.alias	$9,$sp
	.alias	$11,$sp
	.loc	2 109
 # 109	}
$56:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 0($sp)
	l.d	$f22, 8($sp)
	lw	$16, 20($sp)
	addu	$sp, 24
	j	$31
	.end	func_800DE860
