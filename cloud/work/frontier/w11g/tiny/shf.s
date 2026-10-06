	.verstamp	3 19
	.option	pic0
	.extern	D_80120ECC 16
	.extern	D_80120EDC 16
	.extern	D_8013FECB 1
	.extern	D_80120EBC 16
	.extern	D_80120EEC 112
	.extern	D_8014A110 4
	.extern	D_80034840 4
	.extern	D_801543CC 4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 103
 # 103	{
	.ent	func_800E7FA0 2
func_800E7FA0:
	.option	O3
	subu	$sp, 16
	.frame	$sp, 16, $31
	.loc	2 103
	sw	$4, 16($sp)
	sll	$14, $4, 16
	move	$4, $14
	sra	$15, $4, 16
	move	$4, $15
	li	$5, 8
	.loc	2 103
	.loc	2 118
 # 118	    car = &D_8014A250[index];
	.loc	2 119
 # 119	    blocked = car->sviscode[1] == 8 || car->sviscode[2] == 8;
	.noalias	$2,$sp
	mul	$24, $4, 2056
	la	$25, D_8014A250
	addu	$2, $24, $25
	lhu	$3, 1566($2)
	seq	$7, $5, $3
	bne	$7, 0, $32
	lhu	$14, 1568($2)
	seq	$7, $5, $14
$32:
	li	$6, 2
	move	$8, $7
	.loc	2 120
 # 120	    alternate = car->sviscode[1] == 2 || car->sviscode[2] == 2;
	seq	$7, $6, $3
	bne	$7, 0, $33
	lhu	$15, 1568($2)
	seq	$7, $6, $15
$33:
	.loc	2 121
 # 121	    for (i = 0; i < 4; i++) {
	move	$9, $0
	move	$3, $0
	la	$10, D_80120ECC
	move	$11, $2
	la	$9, D_80120ECC+16
	li	$5, 1
	la	$4, D_80120EDC
$34:
	.loc	2 121
	.loc	2 122
 # 122	        if (car->sviscode[i] == 1) {
	lhu	$24, 1564($11)
	bne	$5, $24, $35
	.loc	2 122
	.loc	2 123
 # 123	            car->appearance |= D_80120ECC[i];
	.noalias	$10,$2
	.noalias	$10,$sp
	lw	$25, 2004($2)
	lw	$14, 0($10)
	or	$15, $25, $14
	sw	$15, 2004($2)
	b	$36
$35:
	.loc	2 124
 # 124	        } else {
	.loc	2 125
 # 125	            car->appearance &= ~(D_80120EDC[i] | D_80120ECC[i]);
	.noalias	$4,$2
	.noalias	$4,$10
	.noalias	$4,$sp
	addu	$24, $4, $3
	.noalias	$24,$2
	.noalias	$24,$10
	.noalias	$24,$sp
	lw	$25, 0($24)
	.alias	$24,$2
	.alias	$24,$10
	.alias	$24,$sp
	lw	$14, 0($10)
	nor	$15, $25, $14
	lw	$24, 2004($2)
	and	$25, $24, $15
	sw	$25, 2004($2)
$36:
	.loc	2 121
 # 121	    for (i = 0; i < 4; i++) {
	addu	$3, $3, 4
	addu	$10, $10, 4
	addu	$11, $11, 2
	bltu	$10, $9, $34
	.alias	$10,$2
	.alias	$10,$4
	.alias	$10,$sp
	.loc	2 128
 # 128	    if (blocked || alternate || D_8013FECB != 0) {
	bne	$8, 0, $37
	bne	$7, 0, $37
	lb	$14, D_8013FECB
	beq	$14, 0, $38
	.alias	$4,$2
	.alias	$4,$sp
	.loc	2 128
	.loc	2 129
 # 129	        for (i = 0; i < 4; i++) {
$37:
	li.s	$f2, 0.0
	.loc	2 129
	.loc	2 130
 # 130	            car->level[i] = 0.0f;
	move	$3, $2
	s.s	$f2, 2040($3)
	.loc	2 131
 # 131	            car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
	lw	$24, D_80120EDC
	lw	$15, D_80120EBC
	nor	$25, $24, $15
	lw	$14, 2004($2)
	and	$24, $14, $25
	sw	$24, 2004($2)
	.loc	2 129
 # 129	        for (i = 0; i < 4; i++) {
	s.s	$f2, 2044($3)
	lw	$15, D_80120EDC+4
	lw	$14, D_80120EBC+4
	nor	$25, $15, $14
	lw	$24, 2004($2)
	and	$15, $24, $25
	sw	$15, 2004($2)
	s.s	$f2, 2048($3)
	lw	$14, D_80120EDC+8
	lw	$24, D_80120EBC+8
	nor	$25, $14, $24
	lw	$15, 2004($2)
	and	$14, $15, $25
	sw	$14, 2004($2)
	s.s	$f2, 2052($3)
	lw	$24, D_80120EDC+12
	lw	$15, D_80120EBC+12
	nor	$25, $24, $15
	lw	$14, 2004($2)
	and	$24, $14, $25
	sw	$24, 2004($2)
	.alias	$2,$sp
	b	$61
$38:
	.loc	2 133
 # 130	            car->level[i] = 0.0f;
 # 131	            car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
 # 132	        }
 # 133	    } else {
	.loc	2 134
 # 134	        for (i = 0; i < 4; i++) {
	move	$9, $0
	move	$3, $0
	la	$7, D_80120EEC
	move	$12, $2
	li.s	$f16, 0.125
	li.s	$f14, 0.25
	li.s	$f12, 3.0
	li.s	$f2, 0.0
	li	$19, 4
	la	$18, D_80120EBC
	li	$17, 6
	li	$16, 3
	lw	$8, 0($sp)
$39:
	.loc	2 134
	.loc	2 135
 # 135	            curve = &D_80120EEC[i];
	.loc	2 136
 # 136	            switch (i) {
	lw	$10, D_8014A110
	beq	$9, 0, $40
	beq	$9, $5, $43
	beq	$9, $6, $46
	beq	$9, $16, $49
	mtc1	$8, $f4
	cvt.s.w	$f20, $f4
	b	$50
$40:
	.loc	2 138
 # 137	            case 0:
 # 138	                if (fabsf(car->f4DC) > fabsf(car->f594)) {
	l.s	$f22, 1244($2)
	abs.s	$f22, $f22
	mov.s	$f0, $f22
	mov.s	$f18, $f0
	l.s	$f20, 1428($2)
	abs.s	$f20, $f20
	mov.s	$f0, $f20
	c.lt.s	$f0, $f18
	bc1f	$41
	.loc	2 138
	.loc	2 139
 # 139	                    sample = fabsf(car->f4DC);
	mov.s	$f0, $f22
	trunc.w.s	$f6, $f0, $15
	mfc1	$8, $f6
	b	$42
$41:
	.loc	2 140
 # 140	                } else {
	.loc	2 141
 # 141	                    sample = fabsf(car->f594);
	mov.s	$f0, $f20
	trunc.w.s	$f8, $f0, $14
	mfc1	$8, $f8
$42:
	.loc	2 143
 # 142	                }
 # 143	                break;
	mtc1	$8, $f10
	cvt.s.w	$f20, $f10
	b	$50
$43:
	.loc	2 145
 # 144	            case 1:
 # 145	                if (fabsf(car->f480) > fabsf(car->f538)) {
	l.s	$f22, 1152($2)
	abs.s	$f22, $f22
	mov.s	$f0, $f22
	mov.s	$f18, $f0
	l.s	$f20, 1336($2)
	abs.s	$f20, $f20
	mov.s	$f0, $f20
	c.lt.s	$f0, $f18
	bc1f	$44
	.loc	2 145
	.loc	2 146
 # 146	                    sample = fabsf(car->f480);
	mov.s	$f0, $f22
	trunc.w.s	$f4, $f0, $25
	mfc1	$8, $f4
	b	$45
$44:
	.loc	2 147
 # 147	                } else {
	.loc	2 148
 # 148	                    sample = fabsf(car->f538);
	mov.s	$f0, $f20
	trunc.w.s	$f6, $f0, $24
	mfc1	$8, $f6
$45:
	.loc	2 150
 # 149	                }
 # 150	                break;
	mtc1	$8, $f8
	cvt.s.w	$f20, $f8
	b	$50
$46:
	.loc	2 152
 # 151	            case 2:
 # 152	                if (fabsf(car->f484) > fabsf(car->f4E0)) {
	l.s	$f22, 1156($2)
	abs.s	$f22, $f22
	mov.s	$f0, $f22
	mov.s	$f18, $f0
	l.s	$f20, 1248($2)
	abs.s	$f20, $f20
	mov.s	$f0, $f20
	c.lt.s	$f0, $f18
	bc1f	$47
	.loc	2 152
	.loc	2 153
 # 153	                    sample = fabsf(car->f484);
	mov.s	$f0, $f22
	trunc.w.s	$f10, $f0, $15
	mfc1	$8, $f10
	b	$48
$47:
	.loc	2 154
 # 154	                } else {
	.loc	2 155
 # 155	                    sample = fabsf(car->f4E0);
	mov.s	$f0, $f20
	trunc.w.s	$f4, $f0, $14
	mfc1	$8, $f4
$48:
	.loc	2 157
 # 156	                }
 # 157	                break;
	mtc1	$8, $f6
	cvt.s.w	$f20, $f6
	b	$50
$49:
	.loc	2 159
 # 158	            case 3:
 # 159	                sample = fabsf(car->f53C) + fabsf(car->f598);
	l.s	$f0, 1340($2)
	abs.s	$f0, $f0
	mov.s	$f18, $f0
	l.s	$f0, 1432($2)
	abs.s	$f0, $f0
	add.s	$f8, $f0, $f18
	trunc.w.s	$f10, $f8, $25
	mfc1	$8, $f10
	.loc	2 160
 # 160	                break;
	mtc1	$8, $f4
	cvt.s.w	$f20, $f4
$50:
	.loc	2 162
 # 161	            }
 # 162	            magnitude = sample;
	.loc	2 163
 # 163	            if (D_8014A110 == 6) {
	bne	$17, $10, $51
	.loc	2 163
	.loc	2 164
 # 164	                scale = 3.0f;
	mov.s	$f0, $f12
	b	$52
$51:
	.loc	2 165
 # 165	            } else {
	.loc	2 166
 # 166	                scale = car->params->scale;
	lw	$24, 4($2)
	l.s	$f0, 32($24)
$52:
	.loc	2 168
 # 167	            }
 # 168	            if (curve->level[0] * scale <= magnitude) {
	lw	$15, 0($7)
	mtc1	$15, $f6
	cvt.s.w	$f8, $f6
	mul.s	$f10, $f8, $f0
	c.le.s	$f10, $f20
	bc1f	$57
	.loc	2 168
	.loc	2 169
 # 169	                segment = 0;
	move	$10, $0
	.loc	2 170
 # 170	                hi = curve->level[4];
	lw	$11, 16($7)
	.loc	2 171
 # 171	                if (hi < sample) {
	bge	$11, $8, $53
	.loc	2 171
	.loc	2 172
 # 172	                    sample = hi;
	move	$8, $11
$53:
	.loc	2 174
 # 173	                }
 # 174	                for (; segment < 4; segment++) {
	mul	$14, $10, 4
	addu	$11, $7, $14
$54:
	.loc	2 174
	.loc	2 175
 # 175	                    if (curve->level[segment + 1] >= sample) {
	lw	$25, 4($11)
	bge	$25, $8, $55
	.loc	2 175
	.loc	2 176
 # 176	                        break;
	.loc	2 174
 # 174	                for (; segment < 4; segment++) {
	addu	$10, $10, 1
	addu	$11, $11, 4
	blt	$10, 4, $54
$55:
	.loc	2 179
 # 175	                    if (curve->level[segment + 1] >= sample) {
 # 176	                        break;
 # 177	                    }
 # 178	                }
 # 179	                f = ((f32)(sample - curve->level[segment]) / (f32)(curve->level[segment + 1] - curve->level[segment]) + segment) * 0.25f;
	mul	$24, $10, 4
	addu	$11, $7, $24
	lw	$13, 0($11)
	subu	$15, $8, $13
	mtc1	$15, $f4
	cvt.s.w	$f6, $f4
	lw	$14, 4($11)
	subu	$25, $14, $13
	mtc1	$25, $f8
	cvt.s.w	$f10, $f8
	div.s	$f4, $f6, $f10
	mtc1	$10, $f8
	cvt.s.w	$f6, $f8
	add.s	$f10, $f4, $f6
	mul.s	$f0, $f10, $f14
	.loc	2 180
 # 180	                car->level[i] = f;
	s.s	$f0, 2040($12)
	.loc	2 181
 # 181	                if (curve->first < f) {
	l.s	$f8, 20($7)
	c.lt.s	$f8, $f0
	bc1f	$56
	.loc	2 181
	.loc	2 182
 # 182	                    car->appearance |= D_80120EDC[i];
	.noalias	$4,$sp
	lw	$24, 2004($2)
	addu	$15, $4, $3
	.noalias	$15,$sp
	lw	$14, 0($15)
	.alias	$15,$sp
	or	$25, $24, $14
	sw	$25, 2004($2)
$56:
	.loc	2 184
 # 183	                }
 # 184	                if (curve->second < car->level[i]) {
	l.s	$f4, 24($7)
	l.s	$f6, 2040($12)
	c.lt.s	$f4, $f6
	bc1f	$60
	.loc	2 184
	.loc	2 185
 # 185	                    car->appearance |= D_80120EBC[i];
	.noalias	$18,$4
	.noalias	$18,$sp
	lw	$15, 2004($2)
	addu	$24, $18, $3
	.noalias	$24,$4
	.noalias	$24,$sp
	lw	$14, 0($24)
	.alias	$24,$4
	.alias	$24,$sp
	or	$25, $15, $14
	sw	$25, 2004($2)
	b	$60
$57:
	.loc	2 187
 # 186	                }
 # 187	            } else if (car->level[i] != 0.0f) {
	l.s	$f0, 2040($12)
	c.eq.s	$f2, $f0
	bc1t	$60
	.loc	2 187
	.loc	2 188
 # 188	                if (curve->first < car->level[i]) {
	l.s	$f10, 20($7)
	c.lt.s	$f10, $f0
	bc1f	$58
	.loc	2 188
	.loc	2 189
 # 189	                    car->appearance |= D_80120EDC[i];
	lw	$24, 2004($2)
	addu	$15, $4, $3
	.noalias	$15,$18
	.noalias	$15,$sp
	lw	$14, 0($15)
	.alias	$15,$18
	.alias	$15,$sp
	or	$25, $24, $14
	sw	$25, 2004($2)
	l.s	$f0, 2040($12)
$58:
	.loc	2 191
 # 190	                }
 # 191	                if (curve->second < car->level[i]) {
	l.s	$f8, 24($7)
	c.lt.s	$f8, $f0
	bc1f	$59
	.loc	2 191
	.loc	2 192
 # 192	                    car->appearance |= D_80120EBC[i];
	lw	$15, 2004($2)
	addu	$24, $18, $3
	.noalias	$24,$4
	.noalias	$24,$sp
	lw	$14, 0($24)
	.alias	$24,$4
	.alias	$24,$sp
	or	$25, $15, $14
	sw	$25, 2004($2)
	l.s	$f0, 2040($12)
$59:
	.loc	2 194
 # 193	                }
 # 194	                car->level[i] -= 0.125f;
	sub.s	$f4, $f0, $f16
	s.s	$f4, 2040($12)
	.loc	2 195
 # 195	                if (car->level[i] <= 0.0f) {
	l.s	$f6, 2040($12)
	c.le.s	$f6, $f2
	bc1f	$60
	.loc	2 195
	.loc	2 196
 # 196	                    car->level[i] = 0.0f;
	s.s	$f2, 2040($12)
	.loc	2 197
 # 197	                    car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
	addu	$24, $4, $3
	.noalias	$24,$18
	.noalias	$24,$sp
	lw	$15, 0($24)
	.alias	$24,$18
	.alias	$24,$sp
	addu	$14, $18, $3
	.noalias	$14,$4
	.noalias	$14,$sp
	lw	$25, 0($14)
	.alias	$14,$4
	.alias	$14,$sp
	nor	$24, $15, $25
	lw	$14, 2004($2)
	and	$15, $14, $24
	sw	$15, 2004($2)
$60:
	.loc	2 134
 # 134	        for (i = 0; i < 4; i++) {
	addu	$9, $9, 1
	addu	$3, $3, 4
	addu	$7, $7, 28
	addu	$12, $12, 4
	bne	$9, $19, $39
	.alias	$4,$18
	.alias	$4,$sp
	.alias	$18,$sp
	sw	$8, 0($sp)
	.loc	2 202
 # 202	}
$61:
	.livereg	0x0000FF0E,0x00000FFF
	addu	$sp, 16
	j	$31
	.end	func_800E7FA0
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800E847C
	.loc	2 262
 # 262	void func_800E847C(void) {
	.ent	func_800E847C 2
func_800E847C:
	.option	O3
	subu	$sp, 184
	sw	$31, 100($sp)
	sw	$30, 96($sp)
	sw	$23, 92($sp)
	sw	$22, 88($sp)
	sw	$21, 84($sp)
	sw	$20, 80($sp)
	sw	$19, 76($sp)
	sw	$18, 72($sp)
	sw	$17, 68($sp)
	sw	$16, 64($sp)
	s.d	$f28, 56($sp)
	s.d	$f26, 48($sp)
	s.d	$f24, 40($sp)
	s.d	$f22, 32($sp)
	s.d	$f20, 24($sp)
	.mask	0xC0FF0000, -84
	.fmask	0x3FF00000, -128
	.frame	$sp, 184, $31
	.loc	2 262
	.loc	2 262
	.loc	2 277
 # 277	    osPfsChecker_full(&D_80034840);
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osPfsChecker_full
	.loc	2 278
 # 278	    battle_mode_setup(D_801543CC);
	l.s	$f12, D_801543CC
	.livereg	0x0000000E,0x00080000
	jal	battle_mode_setup
	.loc	2 279
 # 279	    for (i = 0; i < 6; i++) {
	sh	$0, 182($sp)
	li.s	$f28, 0.0000000000000000e+00
	li.s	$f26, 20.0
	li.s	$f24, 0.0
	addu	$30, $sp, 164
	li	$23, 12
	lhu	$22, 134($sp)
	li	$19, 8
	move	$2, $0
$62:
	.loc	2 279
	.loc	2 280
 # 280	        gc = &D_80152818[i];
	.loc	2 281
 # 281	        if (!model[i].in_game || gc->f359 >= 2) {
	.noalias	$20,$sp
	mul	$14, $2, 2056
	la	$15, D_8014A250
	addu	$20, $14, $15
	lh	$24, 1992($20)
	beq	$24, 0, $94
	mul	$25, $2, 952
	la	$14, D_80152818
	addu	$21, $25, $14
	lb	$15, 857($21)
	bge	$15, 2, $94
	.alias	$20,$sp
	.loc	2 281
	.loc	2 282
 # 282	            continue;
	sh	$2, 182($sp)
	.loc	2 284
 # 283	        }
 # 284	        m = &model[i];
	.loc	2 285
 # 285	        gc->u0 = m->u710;
	.noalias	$20,$sp
	lw	$24, 1808($20)
	sw	$24, 0($21)
	.loc	2 286
 # 286	        gc->f4 = m->f714;
	l.s	$f4, 1812($20)
	s.s	$f4, 4($21)
	.loc	2 287
 # 287	        gc->dr_pos[0] = m->pos[0];
	l.s	$f6, 1940($20)
	s.s	$f6, 8($21)
	.loc	2 288
 # 288	        gc->dr_pos[1] = m->pos[1];
	l.s	$f8, 1944($20)
	s.s	$f8, 12($21)
	.loc	2 289
 # 289	        gc->dr_pos[2] = m->pos[2];
	l.s	$f10, 1948($20)
	s.s	$f10, 16($21)
	.loc	2 290
 # 290	        gc->f368[0] = m->f2E0[0];
	l.s	$f4, 736($20)
	s.s	$f4, 872($21)
	.loc	2 291
 # 291	        gc->f368[1] = m->f2E0[1];
	l.s	$f6, 740($20)
	s.s	$f6, 876($21)
	.loc	2 292
 # 292	        gc->f368[2] = m->f2E0[2];
	l.s	$f8, 744($20)
	s.s	$f8, 880($21)
	.loc	2 293
 # 293	        m->f2E0[0] = 0;
	s.s	$f28, 736($20)
	.loc	2 294
 # 294	        m->f2E0[1] = 0;
	s.s	$f28, 740($20)
	.loc	2 295
 # 295	        m->f2E0[2] = 0;
	s.s	$f28, 744($20)
	.loc	2 296
 # 296	        math_utility(m->uvs, gc->dr_uvs);
	addu	$16, $20, 1952
	move	$4, $16
	addu	$18, $21, 44
	move	$5, $18
	.livereg	0x0C00A00E,0x00000000
	jal	math_utility
	.loc	2 297
 # 297	        math_utility(m->uvs, gc->uvs2);
	move	$4, $16
	addu	$17, $21, 80
	move	$5, $17
	.livereg	0x0C00400E,0x00000000
	jal	math_utility
	.loc	2 298
 # 298	        temp[0] = m->force[0][0] + m->force[1][0];
	l.s	$f10, 112($20)
	l.s	$f4, 100($20)
	add.s	$f6, $f10, $f4
	s.s	$f6, 144($sp)
	.loc	2 299
 # 299	        temp[1] = m->force[0][1] + m->force[1][1];
	l.s	$f8, 116($20)
	l.s	$f10, 104($20)
	add.s	$f4, $f8, $f10
	s.s	$f4, 148($sp)
	.loc	2 300
 # 300	        temp[2] = m->force[0][2] + m->force[1][2];
	l.s	$f6, 120($20)
	l.s	$f8, 108($20)
	add.s	$f10, $f6, $f8
	s.s	$f10, 152($sp)
	.loc	2 301
 # 301	        temp[0] += m->force[2][0];
	l.s	$f4, 144($sp)
	l.s	$f6, 124($20)
	add.s	$f8, $f4, $f6
	s.s	$f8, 144($sp)
	.loc	2 302
 # 302	        temp[1] += m->force[2][1];
	l.s	$f10, 148($sp)
	l.s	$f4, 128($20)
	add.s	$f6, $f10, $f4
	s.s	$f6, 148($sp)
	.loc	2 303
 # 303	        temp[2] += m->force[2][2];
	l.s	$f10, 152($sp)
	l.s	$f4, 132($20)
	add.s	$f10, $f10, $f4
	s.s	$f10, 152($sp)
	.loc	2 304
 # 304	        temp[0] += m->force[3][0];
	l.s	$f4, 136($20)
	add.s	$f8, $f8, $f4
	s.s	$f8, 144($sp)
	.loc	2 305
 # 305	        temp[1] += m->force[3][1];
	l.s	$f4, 140($20)
	add.s	$f8, $f6, $f4
	s.s	$f8, 148($sp)
	.loc	2 306
 # 306	        temp[2] += m->force[3][2];
	l.s	$f6, 144($20)
	add.s	$f4, $f10, $f6
	s.s	$f4, 152($sp)
	.loc	2 307
 # 307	        curve = D_80111560[D_8011156C[m->body]];
	lbu	$25, 8($20)
	lb	$14, D_8011156C($25)
	mul	$15, $14, 4
	lw	$16, D_80111560($15)
	.loc	2 308
 # 308	        value = temp[2] - curve[0];
	l.s	$f8, 0($16)
	sub.s	$f2, $f4, $f8
	.loc	2 309
 # 309	        if (value > 0.0f) {
	c.lt.s	$f24, $f2
	bc1f	$63
	.loc	2 309
	.loc	2 310
 # 310	            bound = curve[2];
	l.s	$f0, 8($16)
	.loc	2 311
 # 311	            value *= bound / curve[1];
	l.s	$f10, 4($16)
	div.s	$f6, $f0, $f10
	mul.s	$f2, $f2, $f6
	.loc	2 312
 # 312	            if (bound < value) {
	c.lt.s	$f0, $f2
	bc1f	$65
	.loc	2 312
	.loc	2 313
 # 313	                value = bound;
	mov.s	$f2, $f0
	b	$65
$63:
	.loc	2 315
 # 314	            }
 # 315	        } else {
	.loc	2 316
 # 316	            value = temp[2] - curve[3];
	l.s	$f4, 152($sp)
	l.s	$f8, 12($16)
	sub.s	$f2, $f4, $f8
	.loc	2 317
 # 317	            if (value < 0.0f) {
	c.lt.s	$f2, $f24
	bc1f	$64
	.loc	2 317
	.loc	2 318
 # 318	                bound = curve[5];
	l.s	$f0, 20($16)
	.loc	2 319
 # 319	                value *= -bound / curve[4];
	neg.s	$f10, $f0
	l.s	$f6, 16($16)
	div.s	$f4, $f10, $f6
	mul.s	$f2, $f2, $f4
	.loc	2 320
 # 320	                if (value < bound) {
	c.lt.s	$f2, $f0
	bc1f	$65
	.loc	2 320
	.loc	2 321
 # 321	                    value = bound;
	mov.s	$f2, $f0
	b	$65
$64:
	.loc	2 323
 # 322	                }
 # 323	            } else {
	.loc	2 324
 # 324	                value = 0.0f;
	mov.s	$f2, $f24
$65:
	.loc	2 327
 # 325	            }
 # 326	        }
 # 327	        gc->roll = gc->roll * 0.6f + 0.4f * value;
	l.s	$f8, 224($21)
	li.s	$f10, 0.6
	mul.s	$f6, $f8, $f10
	li.s	$f4, 0.4
	mul.s	$f8, $f4, $f2
	add.s	$f10, $f6, $f8
	s.s	$f10, 224($21)
	.loc	2 328
 # 328	        func_80090F44(gc->roll, gc->uvs2);
	l.s	$f12, 224($21)
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_80090F44
	.loc	2 329
 # 329	        bound = curve[6];
	l.s	$f0, 24($16)
	.loc	2 330
 # 330	        value = temp[0] - bound;
	l.s	$f4, 144($sp)
	sub.s	$f2, $f4, $f0
	.loc	2 331
 # 331	        if (value > 0.0f) {
	c.lt.s	$f24, $f2
	bc1f	$66
	.loc	2 331
	.loc	2 332
 # 332	            bound = curve[8];
	l.s	$f0, 32($16)
	.loc	2 333
 # 333	            value *= -bound / curve[7];
	neg.s	$f6, $f0
	l.s	$f8, 28($16)
	div.s	$f10, $f6, $f8
	mul.s	$f2, $f2, $f10
	.loc	2 334
 # 334	            if (bound < value) {
	c.lt.s	$f0, $f2
	bc1f	$68
	.loc	2 334
	.loc	2 335
 # 335	                value = bound;
	mov.s	$f2, $f0
	b	$68
$66:
	.loc	2 337
 # 336	            }
 # 337	        } else {
	.loc	2 338
 # 338	            value = temp[0] + bound;
	l.s	$f4, 144($sp)
	add.s	$f2, $f4, $f0
	.loc	2 339
 # 339	            if (value < 0.0f) {
	c.lt.s	$f2, $f24
	bc1f	$67
	.loc	2 339
	.loc	2 340
 # 340	                bound = -curve[8];
	l.s	$f0, 32($16)
	neg.s	$f0, $f0
	.loc	2 341
 # 341	                value *= bound / curve[7];
	l.s	$f6, 28($16)
	div.s	$f8, $f0, $f6
	mul.s	$f2, $f2, $f8
	.loc	2 342
 # 342	                if (value < bound) {
	c.lt.s	$f2, $f0
	bc1f	$68
	.loc	2 342
	.loc	2 343
 # 343	                    value = bound;
	mov.s	$f2, $f0
	b	$68
$67:
	.loc	2 345
 # 344	                }
 # 345	            } else {
	.loc	2 346
 # 346	                value = 0.0f;
	mov.s	$f2, $f24
$68:
	.loc	2 349
 # 347	            }
 # 348	        }
 # 349	        gc->pitch = gc->pitch * 0.7f + 0.3f * value;
	l.s	$f10, 228($21)
	li.s	$f4, 0.7
	mul.s	$f6, $f10, $f4
	li.s	$f8, 0.3
	mul.s	$f10, $f8, $f2
	add.s	$f4, $f6, $f10
	s.s	$f4, 228($21)
	.loc	2 350
 # 350	        func_8009EA68(gc->pitch, gc->uvs2);
	l.s	$f12, 228($21)
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_8009EA68
	.loc	2 351
 # 351	        for (j = 0; j < 4; j++) {
	move	$17, $0
$69:
	.loc	2 351
	.loc	2 352
 # 352	            gc->TIRER[j][0] = m->tires->tirer[j][0];
	mul	$2, $17, $23
	addu	$16, $21, $2
	lw	$24, 0($20)
	addu	$25, $24, $2
	l.s	$f8, 112($25)
	s.s	$f8, 176($16)
	.loc	2 353
 # 353	            gc->TIRER[j][1] = m->tires->tirer[j][1];
	lw	$14, 0($20)
	addu	$15, $14, $2
	l.s	$f6, 116($15)
	s.s	$f6, 180($16)
	.loc	2 354
 # 354	            gc->TIRER[j][2] = m->tires->tirer[j][2];
	lw	$24, 0($20)
	addu	$25, $24, $2
	l.s	$f10, 120($25)
	s.s	$f10, 184($16)
	.loc	2 355
 # 355	            func_8009E820(m->tires->tirer[j], gc->dr_tirepos[j], gc->dr_uvs);
	lw	$14, 0($20)
	addu	$4, $14, $2
	addu	$4, $4, 112
	addu	$5, $16, 116
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
	.loc	2 356
 # 356	            gc->dr_tirepos[j][0] = gc->dr_tirepos[j][0] + gc->dr_pos[0];
	l.s	$f4, 8($21)
	l.s	$f8, 116($16)
	add.s	$f6, $f4, $f8
	s.s	$f6, 116($16)
	.loc	2 357
 # 357	            gc->dr_tirepos[j][1] = gc->dr_tirepos[j][1] + gc->dr_pos[1];
	l.s	$f10, 12($21)
	l.s	$f4, 120($16)
	add.s	$f8, $f10, $f4
	s.s	$f8, 120($16)
	.loc	2 358
 # 358	            gc->dr_tirepos[j][2] = gc->dr_tirepos[j][2] + gc->dr_pos[2];
	l.s	$f6, 16($21)
	l.s	$f10, 124($16)
	add.s	$f4, $f6, $f10
	s.s	$f4, 124($16)
	.loc	2 351
 # 351	        for (j = 0; j < 4; j++) {
	addu	$17, $17, 1
	sll	$15, $17, 16
	move	$17, $15
	sra	$24, $17, 16
	move	$17, $24
	blt	$17, 4, $69
	.loc	2 360
 # 360	        gc->dr_acc[0] = m->acc[0];
	l.s	$f8, 1928($20)
	s.s	$f8, 20($21)
	.loc	2 361
 # 361	        gc->dr_acc[1] = m->acc[1];
	l.s	$f6, 1932($20)
	s.s	$f6, 24($21)
	.loc	2 362
 # 362	        gc->dr_acc[2] = m->acc[2];
	l.s	$f10, 1936($20)
	s.s	$f10, 28($21)
	.loc	2 363
 # 363	        gc->dr_vel[0] = m->vel[0];
	l.s	$f4, 1916($20)
	s.s	$f4, 32($21)
	.loc	2 364
 # 364	        gc->dr_vel[1] = m->vel[1];
	l.s	$f8, 1920($20)
	s.s	$f8, 36($21)
	.loc	2 365
 # 365	        gc->dr_vel[2] = m->vel[2];
	l.s	$f6, 1924($20)
	s.s	$f6, 40($21)
	.loc	2 366
 # 366	        if (D_80153E88[i].b7 == 0 || D_80153E88[i].b7 == 6) {
	lh	$25, 182($sp)
	mul	$14, $25, 8
	lbu	$2, D_80153E88+7($14)
	beq	$2, 0, $70
	bne	$2, 6, $91
$70:
	.loc	2 366
	.loc	2 367
 # 367	            high_index = 0;
	move	$7, $0
	.loc	2 368
 # 368	            for (k = 0; k < 4; k++) {
	move	$6, $0
$71:
	.loc	2 368
	.loc	2 369
 # 369	                gc->sound_flags[k] = m->sound_flags[k];
	mul	$2, $6, 2
	addu	$4, $20, $2
	lhu	$15, 1580($4)
	addu	$24, $21, $2
	sh	$15, 836($24)
	.loc	2 370
 # 370	                same_count[k] = 0;
	.noalias	$30,$20
	.noalias	$30,$gp
	.noalias	$3,$20
	.noalias	$3,$gp
	addu	$3, $30, $2
	sh	$0, 0($3)
	.loc	2 371
 # 371	                if (m->sviscode[k] != 8 || m->sound_flags[k] != 0) {
	mul	$25, $7, 2
	addu	$8, $30, $25
	lhu	$14, 1564($4)
	bne	$19, $14, $72
	lhu	$15, 1580($4)
	beq	$15, 0, $75
$72:
	.loc	2 371
	.loc	2 372
 # 372	                    for (l = k + 1; l < 4; l++) {
	addu	$5, $6, 1
	sll	$2, $5, 16
	sra	$24, $2, 16
	move	$2, $24
	sll	$25, $5, 16
	sra	$14, $25, 16
	bge	$14, 4, $75
$73:
	.loc	2 372
	.loc	2 373
 # 373	                        if (m->sound_flags[k] == m->sound_flags[l]) {
	lhu	$15, 1580($4)
	mul	$24, $2, 2
	addu	$25, $20, $24
	.noalias	$25,$3
	.noalias	$25,$sp
	.noalias	$25,$30
	lhu	$14, 1580($25)
	.alias	$25,$3
	.alias	$25,$sp
	.alias	$25,$30
	bne	$15, $14, $74
	.loc	2 373
	.loc	2 374
 # 374	                            same_count[k]++;
	lh	$24, 0($3)
	addu	$25, $24, 1
	sh	$25, 0($3)
$74:
	.loc	2 372
 # 372	                    for (l = k + 1; l < 4; l++) {
	addu	$2, $2, 1
	sll	$15, $2, 16
	move	$2, $15
	sra	$14, $2, 16
	move	$2, $14
	blt	$2, 4, $73
$75:
	.loc	2 378
 # 373	                        if (m->sound_flags[k] == m->sound_flags[l]) {
 # 374	                            same_count[k]++;
 # 375	                        }
 # 376	                    }
 # 377	                }
 # 378	                if (same_count[k] >= same_count[high_index]) {
	addu	$5, $6, 1
	.noalias	$8,$20
	.noalias	$8,$gp
	lh	$24, 0($3)
	lh	$25, 0($8)
	blt	$24, $25, $76
	.alias	$3,$20
	.alias	$3,$gp
	.alias	$8,$20
	.alias	$8,$gp
	.loc	2 378
	.loc	2 379
 # 379	                    high_index = k;
	sll	$7, $6, 16
	sra	$15, $7, 16
	move	$7, $15
	.loc	2 380
 # 380	                    snd_flags = m->sound_flags[k];
	lhu	$22, 1580($4)
$76:
	.loc	2 368
 # 368	            for (k = 0; k < 4; k++) {
	sll	$6, $5, 16
	sra	$14, $6, 16
	move	$6, $14
	blt	$6, 4, $71
	and	$2, $22, 256
	sra	$24, $2, 8
	move	$2, $24
	.loc	2 383
 # 383	            gc->in_tunnel = (snd_flags & 0x100) >> 8;
	sb	$2, 858($21)
	.loc	2 384
 # 384	            if (m->crashflag != 0.0f) {
	l.s	$f10, 980($20)
	c.eq.s	$f24, $f10
	bc1t	$77
	.loc	2 384
	.loc	2 385
 # 385	                m->appearance |= 0x1000;
	lw	$25, 2004($20)
	or	$15, $25, 4096
	sw	$15, 2004($20)
	b	$78
$77:
	.loc	2 386
 # 386	            } else {
	.loc	2 387
 # 387	                m->appearance &= ~0x1000;
	lw	$14, 2004($20)
	and	$24, $14, -4097
	sw	$24, 2004($20)
$78:
	.loc	2 389
 # 388	            }
 # 389	            if (m->magvel > 20.0f && m->bodyforce[0][1] != 0.0f) {
	l.s	$f4, 1008($20)
	c.lt.s	$f26, $f4
	bc1f	$79
	l.s	$f8, 196($20)
	c.eq.s	$f24, $f8
	bc1t	$79
	.loc	2 389
	.loc	2 390
 # 390	                m->appearance |= 0x80;
	lw	$25, 2004($20)
	or	$15, $25, 128
	sw	$15, 2004($20)
	b	$80
$79:
	.loc	2 391
 # 391	            } else {
	.loc	2 392
 # 392	                m->appearance &= ~0x80;
	lw	$14, 2004($20)
	and	$24, $14, -129
	sw	$24, 2004($20)
$80:
	.loc	2 394
 # 393	            }
 # 394	            if (m->magvel > 20.0f && m->bodyforce[2][1] != 0.0f) {
	l.s	$f6, 1008($20)
	c.lt.s	$f26, $f6
	bc1f	$81
	l.s	$f10, 220($20)
	c.eq.s	$f24, $f10
	bc1t	$81
	.loc	2 394
	.loc	2 395
 # 395	                m->appearance |= 0x4000;
	lw	$25, 2004($20)
	or	$15, $25, 16384
	sw	$15, 2004($20)
	b	$82
$81:
	.loc	2 396
 # 396	            } else {
	.loc	2 397
 # 397	                m->appearance &= ~0x4000;
	lw	$14, 2004($20)
	and	$24, $14, -16385
	sw	$24, 2004($20)
$82:
	.loc	2 399
 # 398	            }
 # 399	            if (m->magvel > 20.0f && m->bodyforce[1][1] != 0.0f) {
	l.s	$f4, 1008($20)
	c.lt.s	$f26, $f4
	bc1f	$83
	l.s	$f8, 208($20)
	c.eq.s	$f24, $f8
	bc1t	$83
	.loc	2 399
	.loc	2 400
 # 400	                m->appearance |= 0x100;
	lw	$25, 2004($20)
	or	$15, $25, 256
	sw	$15, 2004($20)
	b	$84
$83:
	.loc	2 401
 # 401	            } else {
	.loc	2 402
 # 402	                m->appearance &= ~0x100;
	lw	$14, 2004($20)
	and	$24, $14, -257
	sw	$24, 2004($20)
$84:
	.loc	2 404
 # 403	            }
 # 404	            if (m->magvel > 20.0f && m->bodyforce[3][1] != 0.0f) {
	l.s	$f6, 1008($20)
	c.lt.s	$f26, $f6
	bc1f	$85
	l.s	$f10, 232($20)
	c.eq.s	$f24, $f10
	bc1t	$85
	.loc	2 404
	.loc	2 405
 # 405	                m->appearance |= 0x2000;
	lw	$25, 2004($20)
	or	$15, $25, 8192
	sw	$15, 2004($20)
	b	$86
$85:
	.loc	2 406
 # 406	            } else {
	.loc	2 407
 # 407	                m->appearance &= ~0x2000;
	lw	$14, 2004($20)
	and	$24, $14, -8193
	sw	$24, 2004($20)
$86:
	.loc	2 409
 # 408	            }
 # 409	            if (m->thumpflag > 2) {
	lb	$2, 1602($20)
	blt	$2, 3, $87
	.loc	2 409
	.loc	2 410
 # 410	                m->appearance |= 0x800;
	lw	$25, 2004($20)
	or	$15, $25, 2048
	sw	$15, 2004($20)
	b	$88
$87:
	.loc	2 411
 # 411	            } else if (m->thumpflag == 0) {
	bne	$2, 0, $88
	.loc	2 411
	.loc	2 412
 # 412	                m->appearance &= ~0x800;
	lw	$14, 2004($20)
	and	$24, $14, -2049
	sw	$24, 2004($20)
$88:
	.loc	2 414
 # 413	            }
 # 414	            if (gc->collidable && !m->collidable) {
	lb	$2, 777($21)
	beq	$2, 0, $89
	lb	$25, 2027($20)
	bne	$25, 0, $89
	.loc	2 414
	.loc	2 415
 # 415	                m->appearance &= ~8;
	lw	$15, 2004($20)
	and	$14, $15, -9
	sw	$14, 2004($20)
	b	$90
$89:
	.loc	2 416
 # 416	            } else if (!gc->collidable && m->collidable) {
	bne	$2, 0, $90
	lb	$24, 2027($20)
	beq	$24, 0, $90
	.loc	2 416
	.loc	2 417
 # 417	                m->appearance |= 8;
	lw	$25, 2004($20)
	or	$15, $25, 8
	sw	$15, 2004($20)
$90:
	.loc	2 419
 # 418	            }
 # 419	            func_800E7FA0(i);
	lh	$4, 182($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_800E7FA0
	li	$19, 8
	.loc	2 420
 # 420	            gc->V[0] = m->V[0];
	l.s	$f4, 64($20)
	s.s	$f4, 164($21)
	.loc	2 421
 # 421	            gc->V[1] = m->V[1];
	l.s	$f8, 68($20)
	s.s	$f8, 168($21)
	.loc	2 422
 # 422	            gc->V[2] = m->V[2];
	l.s	$f6, 72($20)
	s.s	$f6, 172($21)
	.loc	2 423
 # 423	            gc->mph = m->mph;
	lh	$14, 1880($20)
	sh	$14, 248($21)
$91:
	.loc	2 425
 # 424	        }
 # 425	        gc->appearance = m->appearance;
	lw	$24, 2004($20)
	sw	$24, 232($21)
	.loc	2 426
 # 426	        m->appearance &= ~0x100000;
	lw	$25, 2004($20)
	and	$15, $25, -1048577
	sw	$15, 2004($20)
	.loc	2 427
 # 427	        gc->f308 = m->f7EA;
	lb	$14, 2026($20)
	sb	$14, 776($21)
	.loc	2 428
 # 428	        gc->collidable = m->collidable;
	lb	$24, 2027($20)
	sb	$24, 777($21)
	.loc	2 429
 # 429	        if (gc->collidable == 0 && gc->collide_time == 0.0f) {
	lb	$2, 777($21)
	bne	$2, 0, $92
	l.s	$f10, 780($21)
	c.eq.s	$f24, $f10
	bc1f	$92
	.loc	2 429
	.loc	2 430
 # 430	            gc->collide_count = -1;
	li	$25, -1
	sb	$25, 785($21)
	.loc	2 431
 # 431	            gc->collide_state = -1;
	li	$15, -1
	sb	$15, 784($21)
	.loc	2 432
 # 432	            gc->collide_time = m->f714;
	l.s	$f4, 1812($20)
	s.s	$f4, 780($21)
	lb	$2, 777($21)
$92:
	.loc	2 434
 # 433	        }
 # 434	        if (gc->collidable == 1) {
	bne	$2, 1, $93
	.loc	2 434
	.loc	2 435
 # 435	            gc->collide_time = 0.0f;
	s.s	$f24, 780($21)
$93:
	.loc	2 437
 # 436	        }
 # 437	        gc->data_valid = m->data_valid;
	lb	$14, 2013($20)
	sb	$14, 236($21)
	.loc	2 279
 # 279	    for (i = 0; i < 6; i++) {
	lh	$2, 182($sp)
	.alias	$20,$30
	.alias	$20,$sp
$94:
	addu	$2, $2, 1
	sll	$24, $2, 16
	move	$2, $24
	sra	$25, $2, 16
	move	$2, $25
	blt	$2, 6, $62
	.alias	$30,$gp
	sh	$22, 134($sp)
	sh	$2, 182($sp)
	.loc	2 439
 # 439	    osStartThread(&D_80034840);
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osStartThread
	.loc	2 440
 # 440	}
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 24($sp)
	l.d	$f22, 32($sp)
	l.d	$f24, 40($sp)
	l.d	$f26, 48($sp)
	l.d	$f28, 56($sp)
	lw	$16, 64($sp)
	lw	$17, 68($sp)
	lw	$18, 72($sp)
	lw	$19, 76($sp)
	lw	$20, 80($sp)
	lw	$21, 84($sp)
	lw	$22, 88($sp)
	lw	$23, 92($sp)
	lw	$31, 100($sp)
	lw	$30, 96($sp)
	addu	$sp, 184
	j	$31
	.end	func_800E847C
