	.verstamp	3 19
	.option	pic0
	.extern	D_80152744 1
	.extern	D_801543CA 2
	.extern	D_8015274C 2
	.extern	D_80152768 2
	.extern	D_80153FD2 2
	.extern	D_80143FF4 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800EC914
	.loc	2 75
 #  75	void func_800EC914(void) {
	.ent	func_800EC914 2
func_800EC914:
	.option	O3
	subu	$sp, 16
	sw	$17, 12($sp)
	sw	$16, 8($sp)
	.mask	0x00030000, -4
	.frame	$sp, 16, $31
	.loc	2 75
	la	$10, D_80152744
	la	$11, D_801543CA
	.loc	2 75
	.loc	2 82
 #  82	    D_80152744 = 0;
	.noalias	$10,$sp
	sb	$0, 0($10)
	.loc	2 83
 #  83	    for (i = 0; i < D_801543CA; i++) {
	move	$6, $0
	.noalias	$11,$10
	.noalias	$11,$sp
	.set	 volatile
	lh	$14, 0($11)
	.set	 novolatile
	ble	$14, 0, $35
	la	$2, D_8014A250
	la	$8, D_80153E88
	li	$16, 1
	li	$13, 952
	la	$12, D_80152818
	li	$5, 2
	li	$4, 2056
	la	$3, D_8014A250
$32:
	.loc	2 83
	.loc	2 84
 #  84	        m = &D_8014A250[i];
	.loc	2 85
 #  85	        gc = &D_80152818[i];
	.loc	2 86
 #  86	        m->we_control = (D_80153E88[i].owner == 0);
	.noalias	$2,$10
	.noalias	$2,$11
	.noalias	$2,$sp
	.noalias	$8,$2
	.noalias	$8,$10
	.noalias	$8,$11
	.noalias	$8,$sp
	lbu	$15, 7($8)
	seq	$24, $15, 0
	sh	$24, 1994($2)
	.loc	2 87
 #  87	        m->drone_type = 0;
	sb	$0, 1996($2)
	.loc	2 88
 #  88	        m->in_game = (D_80153E88[i].flags & LINK_ACTIVE) != 0;
	lbu	$25, 6($8)
	and	$14, $25, 128
	sne	$15, $14, 0
	sh	$15, 1992($2)
	.loc	2 89
 #  89	        if (m->in_game) {
	lh	$24, 1992($2)
	beq	$24, 0, $34
	.loc	2 89
	.loc	2 90
 #  90	            D_8014A250[D_80152744++].slot = i;
	lb	$9, 0($10)
	.noalias	$3,$8
	.noalias	$3,$10
	.noalias	$3,$11
	.noalias	$3,$sp
	mul	$25, $9, $4
	addu	$14, $3, $25
	.noalias	$14,$8
	.noalias	$14,$10
	.noalias	$14,$11
	.noalias	$14,$sp
	sh	$6, 1990($14)
	.alias	$14,$8
	.alias	$14,$10
	.alias	$14,$11
	.alias	$14,$sp
	addu	$15, $9, 1
	sb	$15, 0($10)
	.loc	2 91
 #  91	            gc->place = i;
	mul	$24, $6, $13
	addu	$7, $12, $24
	sb	$6, 859($7)
	.loc	2 92
 #  92	            gc->unkEE = D_80153E88[i].unk5;
	lbu	$25, 5($8)
	sb	$25, 238($7)
	.loc	2 93
 #  93	            m->drone_type = (D_80153E88[i].owner < MAX_LINKS) ? DRONE : HUMAN;
	lbu	$14, 7($8)
	bge	$14, 6, $33
	sb	$16, 1996($2)
	b	$34
$33:
	sb	$5, 1996($2)
$34:
	.loc	2 83
 #  83	    for (i = 0; i < D_801543CA; i++) {
	addu	$6, $6, 1
	addu	$2, $2, 2056
	addu	$8, $8, 8
	.set	 volatile
	lh	$15, 0($11)
	.set	 novolatile
	blt	$6, $15, $32
	.alias	$2,$8
	.alias	$2,$10
	.alias	$2,$11
	.alias	$2,$sp
	.alias	$8,$3
	.alias	$8,$10
	.alias	$8,$11
	.alias	$8,$sp
$35:
	la	$3, D_8014A250
	li	$4, 2056
	li	$5, 2
	.loc	2 96
 #  96	    for (i = D_801543CA; i < MAX_LINKS; i++) {
	.set	 volatile
	lh	$6, 0($11)
	.set	 novolatile
	bge	$6, 6, $37
	.alias	$11,$3
	.alias	$11,$10
	.alias	$11,$sp
	mul	$24, $6, 2056
	addu	$2, $3, $24
	mul	$25, $6, 8
	la	$14, D_80153E88
	addu	$8, $25, $14
	la	$6, D_80153E88+48
$36:
	.loc	2 96
	.loc	2 97
 #  97	        m = &D_8014A250[i];
	.loc	2 98
 #  98	        m->we_control = (D_80153E88[i].owner == 0);
	.noalias	$2,$10
	.noalias	$2,$sp
	.noalias	$8,$2
	.noalias	$8,$3
	.noalias	$8,$10
	.noalias	$8,$sp
	lbu	$15, 7($8)
	seq	$24, $15, 0
	sh	$24, 1994($2)
	.loc	2 99
 #  99	        m->drone_type = 0;
	sb	$0, 1996($2)
	.loc	2 100
 # 100	        m->in_game = 0;
	sh	$0, 1992($2)
	.loc	2 96
 #  96	    for (i = D_801543CA; i < MAX_LINKS; i++) {
	addu	$2, $2, 2056
	addu	$8, $8, 8
	bltu	$8, $6, $36
	.alias	$2,$8
	.alias	$2,$10
	.alias	$2,$sp
	.alias	$8,$3
	.alias	$8,$10
	.alias	$8,$sp
$37:
	la	$11, D_8015274C
	la	$12, D_80152768
	la	$13, D_80153FD2
	.loc	2 102
 #  97	        m = &D_8014A250[i];
 #  98	        m->we_control = (D_80153E88[i].owner == 0);
 #  99	        m->drone_type = 0;
 # 100	        m->in_game = 0;
 # 101	    }
 # 102	    D_8015274C = 0;
	.noalias	$11,$3
	.noalias	$11,$10
	.noalias	$11,$sp
	sh	$0, 0($11)
	.loc	2 103
 # 103	    j = D_8015274C;
	lh	$7, 0($11)
	.loc	2 104
 # 104	    D_80152768 = j;
	.noalias	$12,$3
	.noalias	$12,$10
	.noalias	$12,$11
	.noalias	$12,$sp
	sh	$7, 0($12)
	.loc	2 105
 # 105	    D_80153FD2 = j;
	.noalias	$13,$3
	.noalias	$13,$10
	.noalias	$13,$11
	.noalias	$13,$12
	.noalias	$13,$sp
	sh	$7, 0($13)
	.loc	2 106
 # 106	    for (i = 0; i < D_80152744; i++) {
	move	$6, $0
	lb	$9, 0($10)
	ble	$9, 0, $41
	.alias	$10,$3
	.alias	$10,$11
	.alias	$10,$12
	.alias	$10,$13
	.alias	$10,$sp
	la	$25, D_8014A250
	move	$2, $25
	mul	$14, $9, 2056
	addu	$10, $14, $25
	la	$17, D_80152808
	la	$16, D_801527D8
	la	$9, D_80143A40
$38:
	.loc	2 106
	.loc	2 107
 # 107	        index = D_8014A250[i].slot;
	.noalias	$2,$11
	.noalias	$2,$12
	.noalias	$2,$13
	.noalias	$2,$sp
	lh	$8, 1990($2)
	.loc	2 108
 # 108	        if (D_8014A250[index].drone_type == HUMAN) {
	.noalias	$6,$11
	.noalias	$6,$12
	.noalias	$6,$13
	.noalias	$6,$sp
	mul	$15, $8, $4
	addu	$6, $3, $15
	lb	$24, 1996($6)
	bne	$5, $24, $39
	.alias	$6,$11
	.alias	$6,$12
	.alias	$6,$13
	.alias	$6,$sp
	.loc	2 108
	.loc	2 109
 # 109	            j = D_80153FD2;
	lh	$7, 0($13)
	.loc	2 110
 # 110	            D_80143A40[j] = index;
	.noalias	$9,$2
	.noalias	$9,$3
	.noalias	$9,$11
	.noalias	$9,$12
	.noalias	$9,$13
	.noalias	$9,$sp
	mul	$14, $7, 2
	addu	$25, $9, $14
	.noalias	$25,$2
	.noalias	$25,$3
	.noalias	$25,$11
	.noalias	$25,$12
	.noalias	$25,$13
	.noalias	$25,$sp
	sh	$8, 0($25)
	.alias	$25,$2
	.alias	$25,$3
	.alias	$25,$11
	.alias	$25,$12
	.alias	$25,$13
	.alias	$25,$sp
	.loc	2 111
 # 111	            D_80153FD2 = j + 1;
	addu	$15, $7, 1
	sh	$15, 0($13)
	b	$40
$39:
	.loc	2 112
 # 112	        } else {
	.loc	2 113
 # 113	            j = D_80152768;
	lh	$7, 0($12)
	.loc	2 114
 # 114	            D_801527D8[j] = index;
	.noalias	$16,$2
	.noalias	$16,$3
	.noalias	$16,$9
	.noalias	$16,$11
	.noalias	$16,$12
	.noalias	$16,$13
	.noalias	$16,$sp
	mul	$24, $7, 2
	addu	$14, $16, $24
	.noalias	$14,$2
	.noalias	$14,$3
	.noalias	$14,$9
	.noalias	$14,$11
	.noalias	$14,$12
	.noalias	$14,$13
	.noalias	$14,$sp
	sh	$8, 0($14)
	.alias	$14,$2
	.alias	$14,$3
	.alias	$14,$9
	.alias	$14,$11
	.alias	$14,$12
	.alias	$14,$13
	.alias	$14,$sp
	.loc	2 115
 # 115	            D_80152768 = j + 1;
	addu	$25, $7, 1
	sh	$25, 0($12)
	.loc	2 116
 # 116	            if (D_8014A250[index].we_control) {
	.noalias	$6,$9
	.noalias	$6,$11
	.noalias	$6,$12
	.noalias	$6,$13
	.noalias	$6,$16
	.noalias	$6,$sp
	lh	$15, 1994($6)
	beq	$15, 0, $40
	.alias	$6,$9
	.alias	$6,$11
	.alias	$6,$12
	.alias	$6,$13
	.alias	$6,$16
	.alias	$6,$sp
	.loc	2 116
	.loc	2 117
 # 117	                D_80152808[D_8015274C++] = index;
	lh	$6, 0($11)
	.noalias	$17,$2
	.noalias	$17,$3
	.noalias	$17,$9
	.noalias	$17,$11
	.noalias	$17,$12
	.noalias	$17,$13
	.noalias	$17,$16
	.noalias	$17,$sp
	mul	$24, $6, 2
	addu	$14, $17, $24
	.noalias	$14,$2
	.noalias	$14,$3
	.noalias	$14,$9
	.noalias	$14,$11
	.noalias	$14,$12
	.noalias	$14,$13
	.noalias	$14,$16
	.noalias	$14,$sp
	sh	$8, 0($14)
	.alias	$14,$2
	.alias	$14,$3
	.alias	$14,$9
	.alias	$14,$11
	.alias	$14,$12
	.alias	$14,$13
	.alias	$14,$16
	.alias	$14,$sp
	addu	$25, $6, 1
	sh	$25, 0($11)
$40:
	.loc	2 106
 # 106	    for (i = 0; i < D_80152744; i++) {
	addu	$2, $2, 2056
	bltu	$2, $10, $38
	.alias	$2,$9
	.alias	$2,$11
	.alias	$2,$12
	.alias	$2,$13
	.alias	$2,$16
	.alias	$2,$17
	.alias	$2,$sp
	.alias	$3,$9
	.alias	$3,$11
	.alias	$3,$12
	.alias	$3,$13
	.alias	$3,$16
	.alias	$3,$17
	.alias	$3,$sp
	.alias	$9,$11
	.alias	$9,$12
	.alias	$9,$13
	.alias	$9,$16
	.alias	$9,$17
	.alias	$9,$sp
	.alias	$11,$12
	.alias	$11,$13
	.alias	$11,$16
	.alias	$11,$17
	.alias	$11,$sp
	.alias	$12,$13
	.alias	$12,$16
	.alias	$12,$17
	.alias	$12,$sp
	.alias	$13,$16
	.alias	$13,$17
	.alias	$13,$sp
	.alias	$16,$17
	.alias	$16,$sp
	.alias	$17,$sp
$41:
	.loc	2 121
 # 121	    D_80143FF4 = 0;
	sw	$0, D_80143FF4
	.loc	2 122
 # 122	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 8($sp)
	lw	$17, 12($sp)
	addu	$sp, 16
	j	$31
	.end	func_800EC914
