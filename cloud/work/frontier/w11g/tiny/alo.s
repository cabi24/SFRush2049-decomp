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
	seq	$8, $5, $3
	bne	$8, 0, $32
	lhu	$14, 1568($2)
	seq	$8, $5, $14
$32:
	li	$7, 2
	move	$9, $8
	.loc	2 120
 # 120	    alternate = car->sviscode[1] == 2 || car->sviscode[2] == 2;
	seq	$8, $7, $3
	bne	$8, 0, $33
	lhu	$15, 1568($2)
	seq	$8, $7, $15
$33:
	.loc	2 121
 # 121	    for (i = 0; i < 4; i++) {
	move	$4, $0
	move	$3, $0
	la	$10, D_80120ECC
	move	$11, $2
	li	$6, 1
	la	$5, D_80120EDC
	la	$4, D_80120ECC+16
$34:
	.loc	2 121
	.loc	2 122
 # 122	        if (car->sviscode[i] == 1) {
	lhu	$24, 1564($11)
	bne	$6, $24, $35
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
	.noalias	$5,$2
	.noalias	$5,$10
	.noalias	$5,$sp
	addu	$24, $5, $3
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
	bltu	$10, $4, $34
	.alias	$10,$2
	.alias	$10,$5
	.alias	$10,$sp
	.loc	2 128
 # 128	    if (blocked || alternate || D_8013FECB != 0) {
	bne	$9, 0, $37
	bne	$8, 0, $37
	lb	$14, D_8013FECB
	beq	$14, 0, $38
	.alias	$5,$2
	.alias	$5,$sp
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
	b	$60
$38:
	.loc	2 133
 # 130	            car->level[i] = 0.0f;
 # 131	            car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
 # 132	        }
 # 133	    } else {
	.loc	2 134
 # 134	        for (i = 0; i < 4; i++) {
	move	$4, $0
	move	$3, $0
	la	$8, D_80120EEC
	move	$16, $2
	li.s	$f16, 0.125
	li.s	$f14, 0.25
	li.s	$f12, 3.0
	li.s	$f2, 0.0
	li	$20, 4
	la	$19, D_80120EBC
	li	$18, 6
	li	$17, 3
	lw	$9, 0($sp)
$39:
	.loc	2 134
	.loc	2 135
 # 135	            curve = &D_80120EEC[i];
	.loc	2 136
 # 136	            switch (i) {
	lw	$10, D_8014A110
	beq	$4, 0, $40
	beq	$4, $6, $43
	beq	$4, $7, $46
	beq	$4, $17, $49
	mtc1	$9, $f4
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
	mfc1	$9, $f6
	b	$42
$41:
	.loc	2 140
 # 140	                } else {
	.loc	2 141
 # 141	                    sample = fabsf(car->f594);
	mov.s	$f0, $f20
	trunc.w.s	$f8, $f0, $14
	mfc1	$9, $f8
$42:
	.loc	2 143
 # 142	                }
 # 143	                break;
	mtc1	$9, $f10
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
	mfc1	$9, $f4
	b	$45
$44:
	.loc	2 147
 # 147	                } else {
	.loc	2 148
 # 148	                    sample = fabsf(car->f538);
	mov.s	$f0, $f20
	trunc.w.s	$f6, $f0, $24
	mfc1	$9, $f6
$45:
	.loc	2 150
 # 149	                }
 # 150	                break;
	mtc1	$9, $f8
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
	mfc1	$9, $f10
	b	$48
$47:
	.loc	2 154
 # 154	                } else {
	.loc	2 155
 # 155	                    sample = fabsf(car->f4E0);
	mov.s	$f0, $f20
	trunc.w.s	$f4, $f0, $14
	mfc1	$9, $f4
$48:
	.loc	2 157
 # 156	                }
 # 157	                break;
	mtc1	$9, $f6
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
	mfc1	$9, $f10
	.loc	2 160
 # 160	                break;
	mtc1	$9, $f4
	cvt.s.w	$f20, $f4
$50:
	.loc	2 162
 # 161	            }
 # 162	            magnitude = sample;
	.loc	2 163
 # 163	            if (D_8014A110 == 6) {
	bne	$18, $10, $51
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
	lw	$15, 0($8)
	mtc1	$15, $f6
	cvt.s.w	$f8, $f6
	mul.s	$f10, $f8, $f0
	c.le.s	$f10, $f20
	bc1f	$56
	.loc	2 168
	.loc	2 169
 # 169	                segment = 0;
	.loc	2 170
 # 170	                lo = curve->level;
	move	$11, $8
	.loc	2 171
 # 171	                hi = lo[4];
	lw	$12, 16($8)
	.loc	2 172
 # 172	                if (hi < sample) {
	move	$10, $0
	bge	$12, $9, $53
	.loc	2 172
	.loc	2 173
 # 173	                    sample = hi;
	move	$9, $12
	.loc	2 175
 # 174	                }
 # 175	                for (segment = 0; segment < 4; segment++, lo++) {
$53:
	.loc	2 175
	.loc	2 176
 # 176	                    if (lo[1] >= sample) {
	lw	$12, 4($11)
	bge	$12, $9, $54
	.loc	2 176
	.loc	2 177
 # 177	                        break;
	.loc	2 175
 # 175	                for (segment = 0; segment < 4; segment++, lo++) {
	addu	$10, $10, 1
	addu	$11, $11, 4
	bne	$10, $20, $53
	lw	$12, 4($11)
$54:
	.loc	2 180
 # 176	                    if (lo[1] >= sample) {
 # 177	                        break;
 # 178	                    }
 # 179	                }
 # 180	                f = ((f32)(sample - lo[0]) / (f32)(lo[1] - lo[0]) + segment) * 0.25f;
	lw	$13, 0($11)
	subu	$14, $9, $13
	mtc1	$14, $f4
	cvt.s.w	$f6, $f4
	subu	$25, $12, $13
	mtc1	$25, $f8
	cvt.s.w	$f10, $f8
	div.s	$f4, $f6, $f10
	mtc1	$10, $f8
	cvt.s.w	$f6, $f8
	add.s	$f10, $f4, $f6
	mul.s	$f0, $f10, $f14
	.loc	2 181
 # 181	                car->level[i] = f;
	s.s	$f0, 2040($16)
	.loc	2 182
 # 182	                if (curve->first < f) {
	l.s	$f8, 20($8)
	c.lt.s	$f8, $f0
	bc1f	$55
	.loc	2 182
	.loc	2 183
 # 183	                    car->appearance |= D_80120EDC[i];
	.noalias	$5,$sp
	lw	$24, 2004($2)
	addu	$15, $5, $3
	.noalias	$15,$sp
	lw	$14, 0($15)
	.alias	$15,$sp
	or	$25, $24, $14
	sw	$25, 2004($2)
$55:
	.loc	2 185
 # 184	                }
 # 185	                if (curve->second < car->level[i]) {
	l.s	$f4, 24($8)
	l.s	$f6, 2040($16)
	c.lt.s	$f4, $f6
	bc1f	$59
	.loc	2 185
	.loc	2 186
 # 186	                    car->appearance |= D_80120EBC[i];
	.noalias	$19,$5
	.noalias	$19,$sp
	lw	$15, 2004($2)
	addu	$24, $19, $3
	.noalias	$24,$5
	.noalias	$24,$sp
	lw	$14, 0($24)
	.alias	$24,$5
	.alias	$24,$sp
	or	$25, $15, $14
	sw	$25, 2004($2)
	b	$59
$56:
	.loc	2 188
 # 187	                }
 # 188	            } else if (car->level[i] != 0.0f) {
	l.s	$f0, 2040($16)
	c.eq.s	$f2, $f0
	bc1t	$59
	.loc	2 188
	.loc	2 189
 # 189	                if (curve->first < car->level[i]) {
	l.s	$f10, 20($8)
	c.lt.s	$f10, $f0
	bc1f	$57
	.loc	2 189
	.loc	2 190
 # 190	                    car->appearance |= D_80120EDC[i];
	lw	$24, 2004($2)
	addu	$15, $5, $3
	.noalias	$15,$19
	.noalias	$15,$sp
	lw	$14, 0($15)
	.alias	$15,$19
	.alias	$15,$sp
	or	$25, $24, $14
	sw	$25, 2004($2)
	l.s	$f0, 2040($16)
$57:
	.loc	2 192
 # 191	                }
 # 192	                if (curve->second < car->level[i]) {
	l.s	$f8, 24($8)
	c.lt.s	$f8, $f0
	bc1f	$58
	.loc	2 192
	.loc	2 193
 # 193	                    car->appearance |= D_80120EBC[i];
	lw	$15, 2004($2)
	addu	$24, $19, $3
	.noalias	$24,$5
	.noalias	$24,$sp
	lw	$14, 0($24)
	.alias	$24,$5
	.alias	$24,$sp
	or	$25, $15, $14
	sw	$25, 2004($2)
	l.s	$f0, 2040($16)
$58:
	.loc	2 195
 # 194	                }
 # 195	                car->level[i] -= 0.125f;
	sub.s	$f4, $f0, $f16
	s.s	$f4, 2040($16)
	.loc	2 196
 # 196	                if (car->level[i] <= 0.0f) {
	l.s	$f6, 2040($16)
	c.le.s	$f6, $f2
	bc1f	$59
	.loc	2 196
	.loc	2 197
 # 197	                    car->level[i] = 0.0f;
	s.s	$f2, 2040($16)
	.loc	2 198
 # 198	                    car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
	addu	$24, $5, $3
	.noalias	$24,$19
	.noalias	$24,$sp
	lw	$15, 0($24)
	.alias	$24,$19
	.alias	$24,$sp
	addu	$14, $19, $3
	.noalias	$14,$5
	.noalias	$14,$sp
	lw	$25, 0($14)
	.alias	$14,$5
	.alias	$14,$sp
	nor	$24, $15, $25
	lw	$14, 2004($2)
	and	$15, $14, $24
	sw	$15, 2004($2)
$59:
	.loc	2 134
 # 134	        for (i = 0; i < 4; i++) {
	addu	$4, $4, 1
	addu	$3, $3, 4
	addu	$8, $8, 28
	addu	$16, $16, 4
	bne	$4, $20, $39
	.alias	$5,$19
	.alias	$5,$sp
	.alias	$19,$sp
	sw	$9, 0($sp)
	.loc	2 203
 # 203	}
$60:
	.livereg	0x0000FF0E,0x00000FFF
	addu	$sp, 16
	j	$31
	.end	func_800E7FA0
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800E847C
	.loc	2 263
 # 263	void func_800E847C(void) {
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
	.loc	2 263
	.loc	2 263
	.loc	2 278
 # 278	    osPfsChecker_full(&D_80034840);
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osPfsChecker_full
	.loc	2 279
 # 279	    battle_mode_setup(D_801543CC);
	l.s	$f12, D_801543CC
	.livereg	0x0000000E,0x00080000
	jal	battle_mode_setup
	.loc	2 280
 # 280	    for (i = 0; i < 6; i++) {
	sh	$0, 182($sp)
	li.s	$f28, 0.0000000000000000e+00
	li.s	$f26, 20.0
	li.s	$f24, 0.0
	li	$30, 12
	lhu	$23, 134($sp)
	li	$20, 8
	addu	$19, $sp, 164
	move	$2, $0
$61:
	.loc	2 280
	.loc	2 281
 # 281	        gc = &D_80152818[i];
	.loc	2 282
 # 282	        if (!model[i].in_game || gc->f359 >= 2) {
	.noalias	$21,$sp
	mul	$14, $2, 2056
	la	$15, D_8014A250
	addu	$21, $14, $15
	lh	$24, 1992($21)
	beq	$24, 0, $93
	mul	$25, $2, 952
	la	$14, D_80152818
	addu	$22, $25, $14
	lb	$15, 857($22)
	bge	$15, 2, $93
	.alias	$21,$sp
	.loc	2 282
	.loc	2 283
 # 283	            continue;
	sh	$2, 182($sp)
	.loc	2 285
 # 284	        }
 # 285	        m = &model[i];
	.loc	2 286
 # 286	        gc->u0 = m->u710;
	.noalias	$21,$sp
	lw	$24, 1808($21)
	sw	$24, 0($22)
	.loc	2 287
 # 287	        gc->f4 = m->f714;
	l.s	$f4, 1812($21)
	s.s	$f4, 4($22)
	.loc	2 288
 # 288	        gc->dr_pos[0] = m->pos[0];
	l.s	$f6, 1940($21)
	s.s	$f6, 8($22)
	.loc	2 289
 # 289	        gc->dr_pos[1] = m->pos[1];
	l.s	$f8, 1944($21)
	s.s	$f8, 12($22)
	.loc	2 290
 # 290	        gc->dr_pos[2] = m->pos[2];
	l.s	$f10, 1948($21)
	s.s	$f10, 16($22)
	.loc	2 291
 # 291	        gc->f368[0] = m->f2E0[0];
	l.s	$f4, 736($21)
	s.s	$f4, 872($22)
	.loc	2 292
 # 292	        gc->f368[1] = m->f2E0[1];
	l.s	$f6, 740($21)
	s.s	$f6, 876($22)
	.loc	2 293
 # 293	        gc->f368[2] = m->f2E0[2];
	l.s	$f8, 744($21)
	s.s	$f8, 880($22)
	.loc	2 294
 # 294	        m->f2E0[0] = 0;
	s.s	$f28, 736($21)
	.loc	2 295
 # 295	        m->f2E0[1] = 0;
	s.s	$f28, 740($21)
	.loc	2 296
 # 296	        m->f2E0[2] = 0;
	s.s	$f28, 744($21)
	.loc	2 297
 # 297	        math_utility(m->uvs, gc->dr_uvs);
	addu	$16, $21, 1952
	move	$4, $16
	addu	$18, $22, 44
	move	$5, $18
	.livereg	0x0C00A00E,0x00000000
	jal	math_utility
	.loc	2 298
 # 298	        math_utility(m->uvs, gc->uvs2);
	move	$4, $16
	addu	$17, $22, 80
	move	$5, $17
	.livereg	0x0C00400E,0x00000000
	jal	math_utility
	.loc	2 299
 # 299	        temp[0] = m->force[0][0] + m->force[1][0];
	l.s	$f10, 112($21)
	l.s	$f4, 100($21)
	add.s	$f6, $f10, $f4
	s.s	$f6, 144($sp)
	.loc	2 300
 # 300	        temp[1] = m->force[0][1] + m->force[1][1];
	l.s	$f8, 116($21)
	l.s	$f10, 104($21)
	add.s	$f4, $f8, $f10
	s.s	$f4, 148($sp)
	.loc	2 301
 # 301	        temp[2] = m->force[0][2] + m->force[1][2];
	l.s	$f6, 120($21)
	l.s	$f8, 108($21)
	add.s	$f10, $f6, $f8
	s.s	$f10, 152($sp)
	.loc	2 302
 # 302	        temp[0] += m->force[2][0];
	l.s	$f4, 144($sp)
	l.s	$f6, 124($21)
	add.s	$f8, $f4, $f6
	s.s	$f8, 144($sp)
	.loc	2 303
 # 303	        temp[1] += m->force[2][1];
	l.s	$f10, 148($sp)
	l.s	$f4, 128($21)
	add.s	$f6, $f10, $f4
	s.s	$f6, 148($sp)
	.loc	2 304
 # 304	        temp[2] += m->force[2][2];
	l.s	$f10, 152($sp)
	l.s	$f4, 132($21)
	add.s	$f10, $f10, $f4
	s.s	$f10, 152($sp)
	.loc	2 305
 # 305	        temp[0] += m->force[3][0];
	l.s	$f4, 136($21)
	add.s	$f8, $f8, $f4
	s.s	$f8, 144($sp)
	.loc	2 306
 # 306	        temp[1] += m->force[3][1];
	l.s	$f4, 140($21)
	add.s	$f8, $f6, $f4
	s.s	$f8, 148($sp)
	.loc	2 307
 # 307	        temp[2] += m->force[3][2];
	l.s	$f6, 144($21)
	add.s	$f4, $f10, $f6
	s.s	$f4, 152($sp)
	.loc	2 308
 # 308	        curve = D_80111560[D_8011156C[m->body]];
	lbu	$25, 8($21)
	lb	$14, D_8011156C($25)
	mul	$15, $14, 4
	lw	$16, D_80111560($15)
	.loc	2 309
 # 309	        value = temp[2] - curve[0];
	l.s	$f8, 0($16)
	sub.s	$f2, $f4, $f8
	.loc	2 310
 # 310	        if (value > 0.0f) {
	c.lt.s	$f24, $f2
	bc1f	$62
	.loc	2 310
	.loc	2 311
 # 311	            bound = curve[2];
	l.s	$f0, 8($16)
	.loc	2 312
 # 312	            value *= bound / curve[1];
	l.s	$f10, 4($16)
	div.s	$f6, $f0, $f10
	mul.s	$f2, $f2, $f6
	.loc	2 313
 # 313	            if (bound < value) {
	c.lt.s	$f0, $f2
	bc1f	$64
	.loc	2 313
	.loc	2 314
 # 314	                value = bound;
	mov.s	$f2, $f0
	b	$64
$62:
	.loc	2 316
 # 315	            }
 # 316	        } else {
	.loc	2 317
 # 317	            value = temp[2] - curve[3];
	l.s	$f4, 152($sp)
	l.s	$f8, 12($16)
	sub.s	$f2, $f4, $f8
	.loc	2 318
 # 318	            if (value < 0.0f) {
	c.lt.s	$f2, $f24
	bc1f	$63
	.loc	2 318
	.loc	2 319
 # 319	                bound = curve[5];
	l.s	$f0, 20($16)
	.loc	2 320
 # 320	                value *= -bound / curve[4];
	neg.s	$f10, $f0
	l.s	$f6, 16($16)
	div.s	$f4, $f10, $f6
	mul.s	$f2, $f2, $f4
	.loc	2 321
 # 321	                if (value < bound) {
	c.lt.s	$f2, $f0
	bc1f	$64
	.loc	2 321
	.loc	2 322
 # 322	                    value = bound;
	mov.s	$f2, $f0
	b	$64
$63:
	.loc	2 324
 # 323	                }
 # 324	            } else {
	.loc	2 325
 # 325	                value = 0.0f;
	mov.s	$f2, $f24
$64:
	.loc	2 328
 # 326	            }
 # 327	        }
 # 328	        gc->roll = gc->roll * 0.6f + 0.4f * value;
	l.s	$f8, 224($22)
	li.s	$f10, 0.6
	mul.s	$f6, $f8, $f10
	li.s	$f4, 0.4
	mul.s	$f8, $f4, $f2
	add.s	$f10, $f6, $f8
	s.s	$f10, 224($22)
	.loc	2 329
 # 329	        func_80090F44(gc->roll, gc->uvs2);
	l.s	$f12, 224($22)
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_80090F44
	.loc	2 330
 # 330	        bound = curve[6];
	l.s	$f0, 24($16)
	.loc	2 331
 # 331	        value = temp[0] - bound;
	l.s	$f4, 144($sp)
	sub.s	$f2, $f4, $f0
	.loc	2 332
 # 332	        if (value > 0.0f) {
	c.lt.s	$f24, $f2
	bc1f	$65
	.loc	2 332
	.loc	2 333
 # 333	            bound = curve[8];
	l.s	$f0, 32($16)
	.loc	2 334
 # 334	            value *= -bound / curve[7];
	neg.s	$f6, $f0
	l.s	$f8, 28($16)
	div.s	$f10, $f6, $f8
	mul.s	$f2, $f2, $f10
	.loc	2 335
 # 335	            if (bound < value) {
	c.lt.s	$f0, $f2
	bc1f	$67
	.loc	2 335
	.loc	2 336
 # 336	                value = bound;
	mov.s	$f2, $f0
	b	$67
$65:
	.loc	2 338
 # 337	            }
 # 338	        } else {
	.loc	2 339
 # 339	            value = temp[0] + bound;
	l.s	$f4, 144($sp)
	add.s	$f2, $f4, $f0
	.loc	2 340
 # 340	            if (value < 0.0f) {
	c.lt.s	$f2, $f24
	bc1f	$66
	.loc	2 340
	.loc	2 341
 # 341	                bound = -curve[8];
	l.s	$f0, 32($16)
	neg.s	$f0, $f0
	.loc	2 342
 # 342	                value *= bound / curve[7];
	l.s	$f6, 28($16)
	div.s	$f8, $f0, $f6
	mul.s	$f2, $f2, $f8
	.loc	2 343
 # 343	                if (value < bound) {
	c.lt.s	$f2, $f0
	bc1f	$67
	.loc	2 343
	.loc	2 344
 # 344	                    value = bound;
	mov.s	$f2, $f0
	b	$67
$66:
	.loc	2 346
 # 345	                }
 # 346	            } else {
	.loc	2 347
 # 347	                value = 0.0f;
	mov.s	$f2, $f24
$67:
	.loc	2 350
 # 348	            }
 # 349	        }
 # 350	        gc->pitch = gc->pitch * 0.7f + 0.3f * value;
	l.s	$f10, 228($22)
	li.s	$f4, 0.7
	mul.s	$f6, $f10, $f4
	li.s	$f8, 0.3
	mul.s	$f10, $f8, $f2
	add.s	$f4, $f6, $f10
	s.s	$f4, 228($22)
	.loc	2 351
 # 351	        func_8009EA68(gc->pitch, gc->uvs2);
	l.s	$f12, 228($22)
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_8009EA68
	.loc	2 352
 # 352	        for (j = 0; j < 4; j++) {
	move	$17, $0
$68:
	.loc	2 352
	.loc	2 353
 # 353	            gc->TIRER[j][0] = m->tires->tirer[j][0];
	mul	$2, $17, $30
	addu	$16, $22, $2
	lw	$24, 0($21)
	addu	$25, $24, $2
	l.s	$f8, 112($25)
	s.s	$f8, 176($16)
	.loc	2 354
 # 354	            gc->TIRER[j][1] = m->tires->tirer[j][1];
	lw	$14, 0($21)
	addu	$15, $14, $2
	l.s	$f6, 116($15)
	s.s	$f6, 180($16)
	.loc	2 355
 # 355	            gc->TIRER[j][2] = m->tires->tirer[j][2];
	lw	$24, 0($21)
	addu	$25, $24, $2
	l.s	$f10, 120($25)
	s.s	$f10, 184($16)
	.loc	2 356
 # 356	            func_8009E820(m->tires->tirer[j], gc->dr_tirepos[j], gc->dr_uvs);
	lw	$14, 0($21)
	addu	$4, $14, $2
	addu	$4, $4, 112
	addu	$5, $16, 116
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
	.loc	2 357
 # 357	            gc->dr_tirepos[j][0] = gc->dr_tirepos[j][0] + gc->dr_pos[0];
	l.s	$f4, 8($22)
	l.s	$f8, 116($16)
	add.s	$f6, $f4, $f8
	s.s	$f6, 116($16)
	.loc	2 358
 # 358	            gc->dr_tirepos[j][1] = gc->dr_tirepos[j][1] + gc->dr_pos[1];
	l.s	$f10, 12($22)
	l.s	$f4, 120($16)
	add.s	$f8, $f10, $f4
	s.s	$f8, 120($16)
	.loc	2 359
 # 359	            gc->dr_tirepos[j][2] = gc->dr_tirepos[j][2] + gc->dr_pos[2];
	l.s	$f6, 16($22)
	l.s	$f10, 124($16)
	add.s	$f4, $f6, $f10
	s.s	$f4, 124($16)
	.loc	2 352
 # 352	        for (j = 0; j < 4; j++) {
	addu	$17, $17, 1
	sll	$15, $17, 16
	move	$17, $15
	sra	$24, $17, 16
	move	$17, $24
	blt	$17, 4, $68
	.loc	2 361
 # 361	        gc->dr_acc[0] = m->acc[0];
	l.s	$f8, 1928($21)
	s.s	$f8, 20($22)
	.loc	2 362
 # 362	        gc->dr_acc[1] = m->acc[1];
	l.s	$f6, 1932($21)
	s.s	$f6, 24($22)
	.loc	2 363
 # 363	        gc->dr_acc[2] = m->acc[2];
	l.s	$f10, 1936($21)
	s.s	$f10, 28($22)
	.loc	2 364
 # 364	        gc->dr_vel[0] = m->vel[0];
	l.s	$f4, 1916($21)
	s.s	$f4, 32($22)
	.loc	2 365
 # 365	        gc->dr_vel[1] = m->vel[1];
	l.s	$f8, 1920($21)
	s.s	$f8, 36($22)
	.loc	2 366
 # 366	        gc->dr_vel[2] = m->vel[2];
	l.s	$f6, 1924($21)
	s.s	$f6, 40($22)
	.loc	2 367
 # 367	        if (D_80153E88[i].b7 == 0 || D_80153E88[i].b7 == 6) {
	lh	$25, 182($sp)
	mul	$14, $25, 8
	lbu	$2, D_80153E88+7($14)
	beq	$2, 0, $69
	bne	$2, 6, $90
$69:
	.loc	2 367
	.loc	2 368
 # 368	            high_index = 0;
	move	$7, $0
	.loc	2 369
 # 369	            for (k = 0; k < 4; k++) {
	move	$6, $0
$70:
	.loc	2 369
	.loc	2 370
 # 370	                gc->sound_flags[k] = m->sound_flags[k];
	mul	$2, $6, 2
	addu	$4, $21, $2
	lhu	$15, 1580($4)
	addu	$24, $22, $2
	sh	$15, 836($24)
	.loc	2 371
 # 371	                same_count[k] = 0;
	.noalias	$19,$21
	.noalias	$19,$gp
	.noalias	$3,$21
	.noalias	$3,$gp
	addu	$3, $19, $2
	sh	$0, 0($3)
	.loc	2 372
 # 372	                if (m->sviscode[k] != 8 || m->sound_flags[k] != 0) {
	mul	$25, $7, 2
	addu	$8, $19, $25
	lhu	$14, 1564($4)
	bne	$20, $14, $71
	lhu	$15, 1580($4)
	beq	$15, 0, $74
$71:
	.loc	2 372
	.loc	2 373
 # 373	                    for (l = k + 1; l < 4; l++) {
	addu	$5, $6, 1
	sll	$2, $5, 16
	sra	$24, $2, 16
	move	$2, $24
	sll	$25, $5, 16
	sra	$14, $25, 16
	bge	$14, 4, $74
$72:
	.loc	2 373
	.loc	2 374
 # 374	                        if (m->sound_flags[k] == m->sound_flags[l]) {
	lhu	$15, 1580($4)
	mul	$24, $2, 2
	addu	$25, $21, $24
	.noalias	$25,$3
	.noalias	$25,$19
	.noalias	$25,$sp
	lhu	$14, 1580($25)
	.alias	$25,$3
	.alias	$25,$19
	.alias	$25,$sp
	bne	$15, $14, $73
	.loc	2 374
	.loc	2 375
 # 375	                            same_count[k]++;
	lh	$24, 0($3)
	addu	$25, $24, 1
	sh	$25, 0($3)
$73:
	.loc	2 373
 # 373	                    for (l = k + 1; l < 4; l++) {
	addu	$2, $2, 1
	sll	$15, $2, 16
	move	$2, $15
	sra	$14, $2, 16
	move	$2, $14
	blt	$2, 4, $72
$74:
	.loc	2 379
 # 374	                        if (m->sound_flags[k] == m->sound_flags[l]) {
 # 375	                            same_count[k]++;
 # 376	                        }
 # 377	                    }
 # 378	                }
 # 379	                if (same_count[k] >= same_count[high_index]) {
	addu	$5, $6, 1
	.noalias	$8,$21
	.noalias	$8,$gp
	lh	$24, 0($3)
	lh	$25, 0($8)
	blt	$24, $25, $75
	.alias	$3,$21
	.alias	$3,$gp
	.alias	$8,$21
	.alias	$8,$gp
	.loc	2 379
	.loc	2 380
 # 380	                    high_index = k;
	sll	$7, $6, 16
	sra	$15, $7, 16
	move	$7, $15
	.loc	2 381
 # 381	                    snd_flags = m->sound_flags[k];
	lhu	$23, 1580($4)
$75:
	.loc	2 369
 # 369	            for (k = 0; k < 4; k++) {
	sll	$6, $5, 16
	sra	$14, $6, 16
	move	$6, $14
	blt	$6, 4, $70
	and	$2, $23, 256
	sra	$24, $2, 8
	move	$2, $24
	.loc	2 384
 # 384	            gc->in_tunnel = (snd_flags & 0x100) >> 8;
	sb	$2, 858($22)
	.loc	2 385
 # 385	            if (m->crashflag != 0.0f) {
	l.s	$f10, 980($21)
	c.eq.s	$f24, $f10
	bc1t	$76
	.loc	2 385
	.loc	2 386
 # 386	                m->appearance |= 0x1000;
	lw	$25, 2004($21)
	or	$15, $25, 4096
	sw	$15, 2004($21)
	b	$77
$76:
	.loc	2 387
 # 387	            } else {
	.loc	2 388
 # 388	                m->appearance &= ~0x1000;
	lw	$14, 2004($21)
	and	$24, $14, -4097
	sw	$24, 2004($21)
$77:
	.loc	2 390
 # 389	            }
 # 390	            if (m->magvel > 20.0f && m->bodyforce[0][1] != 0.0f) {
	l.s	$f4, 1008($21)
	c.lt.s	$f26, $f4
	bc1f	$78
	l.s	$f8, 196($21)
	c.eq.s	$f24, $f8
	bc1t	$78
	.loc	2 390
	.loc	2 391
 # 391	                m->appearance |= 0x80;
	lw	$25, 2004($21)
	or	$15, $25, 128
	sw	$15, 2004($21)
	b	$79
$78:
	.loc	2 392
 # 392	            } else {
	.loc	2 393
 # 393	                m->appearance &= ~0x80;
	lw	$14, 2004($21)
	and	$24, $14, -129
	sw	$24, 2004($21)
$79:
	.loc	2 395
 # 394	            }
 # 395	            if (m->magvel > 20.0f && m->bodyforce[2][1] != 0.0f) {
	l.s	$f6, 1008($21)
	c.lt.s	$f26, $f6
	bc1f	$80
	l.s	$f10, 220($21)
	c.eq.s	$f24, $f10
	bc1t	$80
	.loc	2 395
	.loc	2 396
 # 396	                m->appearance |= 0x4000;
	lw	$25, 2004($21)
	or	$15, $25, 16384
	sw	$15, 2004($21)
	b	$81
$80:
	.loc	2 397
 # 397	            } else {
	.loc	2 398
 # 398	                m->appearance &= ~0x4000;
	lw	$14, 2004($21)
	and	$24, $14, -16385
	sw	$24, 2004($21)
$81:
	.loc	2 400
 # 399	            }
 # 400	            if (m->magvel > 20.0f && m->bodyforce[1][1] != 0.0f) {
	l.s	$f4, 1008($21)
	c.lt.s	$f26, $f4
	bc1f	$82
	l.s	$f8, 208($21)
	c.eq.s	$f24, $f8
	bc1t	$82
	.loc	2 400
	.loc	2 401
 # 401	                m->appearance |= 0x100;
	lw	$25, 2004($21)
	or	$15, $25, 256
	sw	$15, 2004($21)
	b	$83
$82:
	.loc	2 402
 # 402	            } else {
	.loc	2 403
 # 403	                m->appearance &= ~0x100;
	lw	$14, 2004($21)
	and	$24, $14, -257
	sw	$24, 2004($21)
$83:
	.loc	2 405
 # 404	            }
 # 405	            if (m->magvel > 20.0f && m->bodyforce[3][1] != 0.0f) {
	l.s	$f6, 1008($21)
	c.lt.s	$f26, $f6
	bc1f	$84
	l.s	$f10, 232($21)
	c.eq.s	$f24, $f10
	bc1t	$84
	.loc	2 405
	.loc	2 406
 # 406	                m->appearance |= 0x2000;
	lw	$25, 2004($21)
	or	$15, $25, 8192
	sw	$15, 2004($21)
	b	$85
$84:
	.loc	2 407
 # 407	            } else {
	.loc	2 408
 # 408	                m->appearance &= ~0x2000;
	lw	$14, 2004($21)
	and	$24, $14, -8193
	sw	$24, 2004($21)
$85:
	.loc	2 410
 # 409	            }
 # 410	            if (m->thumpflag > 2) {
	lb	$2, 1602($21)
	blt	$2, 3, $86
	.loc	2 410
	.loc	2 411
 # 411	                m->appearance |= 0x800;
	lw	$25, 2004($21)
	or	$15, $25, 2048
	sw	$15, 2004($21)
	b	$87
$86:
	.loc	2 412
 # 412	            } else if (m->thumpflag == 0) {
	bne	$2, 0, $87
	.loc	2 412
	.loc	2 413
 # 413	                m->appearance &= ~0x800;
	lw	$14, 2004($21)
	and	$24, $14, -2049
	sw	$24, 2004($21)
$87:
	.loc	2 415
 # 414	            }
 # 415	            if (gc->collidable && !m->collidable) {
	lb	$2, 777($22)
	beq	$2, 0, $88
	lb	$25, 2027($21)
	bne	$25, 0, $88
	.loc	2 415
	.loc	2 416
 # 416	                m->appearance &= ~8;
	lw	$15, 2004($21)
	and	$14, $15, -9
	sw	$14, 2004($21)
	b	$89
$88:
	.loc	2 417
 # 417	            } else if (!gc->collidable && m->collidable) {
	bne	$2, 0, $89
	lb	$24, 2027($21)
	beq	$24, 0, $89
	.loc	2 417
	.loc	2 418
 # 418	                m->appearance |= 8;
	lw	$25, 2004($21)
	or	$15, $25, 8
	sw	$15, 2004($21)
$89:
	.loc	2 420
 # 419	            }
 # 420	            func_800E7FA0(i);
	lh	$4, 182($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_800E7FA0
	addu	$19, $sp, 164
	li	$20, 8
	.loc	2 421
 # 421	            gc->V[0] = m->V[0];
	l.s	$f4, 64($21)
	s.s	$f4, 164($22)
	.loc	2 422
 # 422	            gc->V[1] = m->V[1];
	l.s	$f8, 68($21)
	s.s	$f8, 168($22)
	.loc	2 423
 # 423	            gc->V[2] = m->V[2];
	l.s	$f6, 72($21)
	s.s	$f6, 172($22)
	.loc	2 424
 # 424	            gc->mph = m->mph;
	lh	$14, 1880($21)
	sh	$14, 248($22)
$90:
	.loc	2 426
 # 425	        }
 # 426	        gc->appearance = m->appearance;
	lw	$24, 2004($21)
	sw	$24, 232($22)
	.loc	2 427
 # 427	        m->appearance &= ~0x100000;
	lw	$25, 2004($21)
	and	$15, $25, -1048577
	sw	$15, 2004($21)
	.loc	2 428
 # 428	        gc->f308 = m->f7EA;
	lb	$14, 2026($21)
	sb	$14, 776($22)
	.loc	2 429
 # 429	        gc->collidable = m->collidable;
	lb	$24, 2027($21)
	sb	$24, 777($22)
	.loc	2 430
 # 430	        if (gc->collidable == 0 && gc->collide_time == 0.0f) {
	lb	$2, 777($22)
	bne	$2, 0, $91
	l.s	$f10, 780($22)
	c.eq.s	$f24, $f10
	bc1f	$91
	.loc	2 430
	.loc	2 431
 # 431	            gc->collide_count = -1;
	li	$25, -1
	sb	$25, 785($22)
	.loc	2 432
 # 432	            gc->collide_state = -1;
	li	$15, -1
	sb	$15, 784($22)
	.loc	2 433
 # 433	            gc->collide_time = m->f714;
	l.s	$f4, 1812($21)
	s.s	$f4, 780($22)
	lb	$2, 777($22)
$91:
	.loc	2 435
 # 434	        }
 # 435	        if (gc->collidable == 1) {
	bne	$2, 1, $92
	.loc	2 435
	.loc	2 436
 # 436	            gc->collide_time = 0.0f;
	s.s	$f24, 780($22)
$92:
	.loc	2 438
 # 437	        }
 # 438	        gc->data_valid = m->data_valid;
	lb	$14, 2013($21)
	sb	$14, 236($22)
	.loc	2 280
 # 280	    for (i = 0; i < 6; i++) {
	lh	$2, 182($sp)
	.alias	$21,$19
	.alias	$21,$sp
$93:
	addu	$2, $2, 1
	sll	$24, $2, 16
	move	$2, $24
	sra	$25, $2, 16
	move	$2, $25
	blt	$2, 6, $61
	.alias	$19,$gp
	sh	$23, 134($sp)
	sh	$2, 182($sp)
	.loc	2 440
 # 440	    osStartThread(&D_80034840);
	la	$4, D_80034840
	.livereg	0x0800000E,0x00000000
	jal	osStartThread
	.loc	2 441
 # 441	}
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
