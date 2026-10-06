	.ent	input_deadzone_apply 2
input_deadzone_apply:
	.option	O3
	subu	$sp, 400
	sw	$31, 132($sp)
	sw	$30, 128($sp)
	sw	$23, 124($sp)
	sw	$22, 120($sp)
	sw	$21, 116($sp)
	sw	$20, 112($sp)
	sw	$19, 108($sp)
	sw	$18, 104($sp)
	sw	$17, 100($sp)
	sw	$16, 96($sp)
	s.d	$f30, 88($sp)
	s.d	$f28, 80($sp)
	s.d	$f26, 72($sp)
	s.d	$f24, 64($sp)
	s.d	$f22, 56($sp)
	s.d	$f20, 48($sp)
	.mask	0xC0FF0000, -268
	.fmask	0xFFF00000, -312
	.frame	$sp, 400, $31
	.loc	4599 98
	move	$18, $5
	sw	$6, 408($sp)
	mtc1	$7, $f26
	li.s	$f28, 0.0
	.loc	4599 128
	sw	$0, 184($sp)
	.loc	4599 129
	l.s	$f8, 0($4)
	s.s	$f8, 332($sp)
	.loc	4599 130
	l.s	$f4, 4($4)
	s.s	$f4, 336($sp)
	.loc	4599 131
	l.s	$f10, 8($4)
	s.s	$f10, 340($sp)
	.loc	4599 132
	l.s	$f6, 332($sp)
	c.lt.s	$f6, $f28
	bc1f	$2186
	trunc.w.s	$f8, $f6, $24
	mfc1	$14, $f8
	mtc1	$14, $f4
	cvt.s.w	$f10, $f4
	c.eq.s	$f10, $f6
	bc1t	$2186
	li.s	$f30, 1.0
	sub.s	$f12, $f6, $f30
	b	$2187
$2186:
	l.s	$f12, 332($sp)
	li.s	$f30, 1.0
$2187:
	.loc	4599 133
	l.s	$f8, 340($sp)
	c.lt.s	$f8, $f28
	bc1f	$2188
	trunc.w.s	$f4, $f8, $25
	mfc1	$15, $f4
	mtc1	$15, $f10
	cvt.s.w	$f6, $f10
	c.eq.s	$f6, $f8
	bc1t	$2188
	sub.s	$f2, $f8, $f30
	b	$2189
$2188:
	l.s	$f2, 340($sp)
$2189:
	.loc	4599 134
	lw	$4, D_80124EEC
	trunc.w.s	$f4, $f12, $24
	mfc1	$5, $f4
	sll	$14, $5, 16
	move	$5, $14
	sra	$25, $5, 16
	move	$5, $25
	trunc.w.s	$f10, $f2, $15
	mfc1	$6, $f10
	sll	$24, $6, 16
	move	$6, $24
	sra	$14, $6, 16
	move	$6, $14
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 135
	bne	$2, 0, $2190
	.loc	4599 135
	.loc	4599 136
	move	$2, $0
	b	$2236
$2190:
	.loc	4599 138
	l.s	$f0, 0($18)
	c.lt.s	$f0, $f28
	bc1f	$2191
	trunc.w.s	$f6, $f0, $25
	mfc1	$15, $f6
	mtc1	$15, $f8
	cvt.s.w	$f4, $f8
	c.eq.s	$f4, $f0
	bc1t	$2191
	.loc	4599 138
	.loc	4599 139
	sub.s	$f12, $f0, $f30
	b	$2192
$2191:
	.loc	4599 140
	.loc	4599 141
	mov.s	$f12, $f0
$2192:
	.loc	4599 143
	l.s	$f0, 8($18)
	c.lt.s	$f0, $f28
	bc1f	$2193
	trunc.w.s	$f10, $f0, $24
	mfc1	$14, $f10
	mtc1	$14, $f6
	cvt.s.w	$f8, $f6
	c.eq.s	$f8, $f0
	bc1t	$2193
	.loc	4599 143
	.loc	4599 144
	sub.s	$f2, $f0, $f30
	b	$2194
$2193:
	.loc	4599 145
	.loc	4599 146
	mov.s	$f2, $f0
$2194:
	.loc	4599 148
	lw	$4, D_80124EEC
	trunc.w.s	$f4, $f12, $25
	mfc1	$5, $f4
	sll	$15, $5, 16
	move	$5, $15
	sra	$24, $5, 16
	move	$5, $24
	trunc.w.s	$f10, $f2, $14
	mfc1	$6, $f10
	sll	$25, $6, 16
	move	$6, $25
	sra	$15, $6, 16
	move	$6, $15
	addu	$7, $sp, 356
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 192($sp)
	.loc	4599 149
	lh	$24, 358($sp)
	and	$6, $24, 1
	and	$9, $24, 2
	lw	$14, 196($sp)
	mul	$25, $24, 2
	addu	$11, $14, $25
	li	$15, 16
	sll	$12, $15, $24
	move	$13, $14
$2195:
	.loc	4599 149
	.loc	4599 150
	lh	$4, 4($13)
	lh	$5, 6($13)
	addu	$2, $4, $5
	sll	$25, $2, 16
	move	$2, $25
	sra	$15, $2, 16
	move	$2, $15
	.loc	4599 151
	bge	$2, 0, $2196
	.loc	4599 151
	.loc	4599 152
	addu	$2, $2, 1
	sll	$24, $2, 16
	move	$2, $24
	sra	$14, $2, 16
	move	$2, $14
$2196:
	.loc	4599 154
	lh	$7, 8($13)
	lh	$8, 10($13)
	addu	$3, $7, $8
	sll	$25, $3, 16
	move	$3, $25
	sra	$15, $3, 16
	move	$3, $15
	.loc	4599 155
	bge	$3, 0, $2197
	.loc	4599 155
	.loc	4599 156
	addu	$3, $3, 1
	sll	$24, $3, 16
	move	$3, $24
	sra	$14, $3, 16
	move	$3, $14
$2197:
	.loc	4599 158
	sra	$25, $2, 1
	move	$2, $25
	sll	$15, $2, 16
	move	$2, $15
	sra	$24, $2, 16
	move	$2, $24
	.loc	4599 159
	sra	$14, $3, 1
	move	$3, $14
	sll	$25, $3, 16
	move	$3, $25
	sra	$15, $3, 16
	move	$3, $15
	.loc	4599 160
	beq	$6, 0, $2198
	sh	$2, 214($sp)
	b	$2199
$2198:
	sh	$4, 214($sp)
$2199:
	.loc	4599 161
	beq	$6, 0, $2200
	sll	$10, $5, 16
	sra	$24, $10, 16
	move	$10, $24
	b	$2201
$2200:
	sh	$2, 212($sp)
	lh	$10, 212($sp)
$2201:
	.loc	4599 162
	beq	$9, 0, $2202
	sh	$7, 210($sp)
	b	$2203
$2202:
	sh	$3, 210($sp)
$2203:
	.loc	4599 163
	beq	$9, 0, $2204
	sll	$8, $3, 16
	sra	$14, $8, 16
	move	$8, $14
	b	$2205
$2204:
	sh	$8, 208($sp)
	lh	$8, 208($sp)
$2205:
	.loc	4599 164
	lhu	$2, 12($11)
	.loc	4599 165
	lbu	$25, 3($13)
	and	$15, $25, $12
	beq	$15, 0, $2206
	.loc	4599 165
	.loc	4599 166
	or	$24, $2, 65536
	move	$2, $24
$2206:
	.loc	4599 168
	sh	$8, 208($sp)
	sh	$10, 212($sp)
	sw	$13, 196($sp)
	beq	$2, 0, $2209
	.loc	4599 168
	.loc	4599 169
	lw	$14, D_80152460
	addu	$4, $14, $2
	.loc	4599 170
	lbu	$8, 0($4)
	addu	$4, $4, 1
	.loc	4599 171
	move	$16, $8
	move	$5, $16
	addu	$6, $sp, 220
	li	$7, -1
	move	$3, $0
	sw	$3, 360($sp)
	.livereg	0x1F00800E,0x00000000
	jal	func_800ADCE0
	lw	$3, 360($sp)
	.loc	4599 172
	sw	$16, 168($sp)
	lw	$25, 168($sp)
	bltu	$25, 1, $2209
	addu	$20, $sp, 220
$2207:
	.loc	4599 172
	.loc	4599 173
	.noalias	$20,$gp
	lhu	$15, 0($20)
	mul	$24, $15, 24
	lw	$14, D_801497F8
	addu	$16, $24, $14
	.loc	4599 174
	lhu	$2, 0($16)
	and	$25, $2, 15
	move	$2, $25
	beq	$2, 15, $2208
	lw	$15, 420($sp)
	beq	$15, $2, $2208
	.loc	4599 174
	.loc	4599 175
	.loc	4599 177
	move	$7, $16
	addu	$17, $sp, 332
	addu	$19, $sp, 308
	addu	$23, $sp, 296
	addu	$30, $sp, 364
	move	$22, $0
	addu	$21, $sp, 292
	mul.s	$f24, $f26, $f26
	sw	$3, 360($sp)
	sw	$16, 188($sp)
	sw	$18, 404($sp)
	sw	$20, 172($sp)
	.livereg	0x0100770E,0x00000080
	jal	input_process_controller
	lw	$3, 360($sp)
	lw	$16, 188($sp)
	lw	$18, 404($sp)
	lw	$20, 172($sp)
	beq	$2, 0, $2208
	.loc	4599 177
	.loc	4599 178
	l.s	$f6, 308($sp)
	lh	$24, 214($sp)
	mtc1	$24, $f8
	cvt.s.w	$f4, $f8
	c.lt.s	$f6, $f4
	bc1t	$2208
	lh	$14, 212($sp)
	mtc1	$14, $f10
	cvt.s.w	$f8, $f10
	c.le.s	$f8, $f6
	bc1t	$2208
	l.s	$f4, 316($sp)
	lh	$25, 210($sp)
	mtc1	$25, $f10
	cvt.s.w	$f6, $f10
	c.lt.s	$f4, $f6
	bc1t	$2208
	lh	$15, 208($sp)
	mtc1	$15, $f8
	cvt.s.w	$f10, $f8
	c.le.s	$f10, $f4
	bc1t	$2208
	.loc	4599 178
	.loc	4599 179
	.loc	4599 181
	l.s	$f6, 308($sp)
	s.s	$f6, 0($18)
	.loc	4599 182
	l.s	$f8, 312($sp)
	s.s	$f8, 4($18)
	.loc	4599 183
	l.s	$f4, 316($sp)
	s.s	$f4, 8($18)
	.loc	4599 184
	addu	$4, $sp, 364
	lw	$5, 408($sp)
	sw	$3, 360($sp)
	.livereg	0x0C00000E,0x00000000
	jal	math_utility
	lw	$3, 360($sp)
	.loc	4599 185
	sw	$16, 184($sp)
	.loc	4599 186
	lhu	$24, 292($sp)
	sh	$24, 290($sp)
	.loc	4599 172
$2208:
	addu	$3, $3, 1
	addu	$20, $20, 2
	lw	$14, 168($sp)
	bne	$3, $14, $2207
	.alias	$20,$gp
$2209:
	lh	$8, 208($sp)
	lh	$10, 212($sp)
	lw	$13, 196($sp)
	.loc	4599 190
	lw	$25, 184($sp)
	beq	$25, 0, $2212
	.loc	4599 190
	.loc	4599 191
	lw	$15, 416($sp)
	beq	$15, 0, $2211
	.loc	4599 191
	.loc	4599 192
	lhu	$2, 0($25)
	and	$24, $2, 8192
	beq	$24, 0, $2210
	.loc	4599 192
	.loc	4599 193
	move	$4, $25
	lhu	$5, 290($sp)
	move	$6, $18
	addu	$7, $sp, 296
	lw	$14, 408($sp)
	sw	$14, 16($sp)
	li.s	$f10, 250.0
	s.s	$f10, 20($sp)
	.livereg	0x0F00000E,0x00000000
	jal	steering_sensitivity
	b	$2211
$2210:
	.loc	4599 194
	and	$15, $2, 4096
	beq	$15, 0, $2211
	.loc	4599 194
	.loc	4599 195
	lw	$4, 184($sp)
	lhu	$5, 290($sp)
	move	$6, $18
	addu	$7, $sp, 296
	lw	$24, 408($sp)
	sw	$24, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	traction_control
	.loc	4599 196
	lw	$4, 408($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_8008E0B8
	.loc	4599 197
	lw	$4, 408($sp)
	addu	$4, $4, 12
	.livereg	0x0800000E,0x00000000
	jal	func_8008E0B8
	.loc	4599 198
	lw	$4, 408($sp)
	addu	$4, $4, 24
	.livereg	0x0800000E,0x00000000
	jal	func_8008E0B8
$2211:
	.loc	4599 201
	lw	$2, 184($sp)
	b	$2236
$2212:
	.loc	4599 203
	lw	$25, 192($sp)
	bne	$13, $25, $2213
	lh	$14, 358($sp)
	lh	$15, 356($sp)
	bne	$14, $15, $2213
	.loc	4599 203
	.loc	4599 204
	move	$2, $0
	b	$2236
$2213:
	.loc	4599 206
	l.s	$f6, 0($18)
	l.s	$f8, 332($sp)
	sub.s	$f4, $f6, $f8
	s.s	$f4, 320($sp)
	.loc	4599 207
	l.s	$f10, 4($18)
	l.s	$f6, 336($sp)
	sub.s	$f4, $f10, $f6
	s.s	$f4, 324($sp)
	.loc	4599 208
	l.s	$f10, 8($18)
	l.s	$f4, 340($sp)
	sub.s	$f10, $f10, $f4
	s.s	$f10, 328($sp)
	.loc	4599 209
	l.s	$f10, 320($sp)
	c.lt.s	$f10, $f28
	bc1f	$2218
	.loc	4599 209
	.loc	4599 210
	lh	$24, 214($sp)
	s.s	$f8, 136($sp)
	mtc1	$24, $f8
	cvt.s.w	$f8, $f8
	s.s	$f8, 308($sp)
	.loc	4599 211
	s.s	$f6, 140($sp)
	l.s	$f6, 136($sp)
	sub.s	$f8, $f8, $f6
	div.s	$f0, $f8, $f10
	.loc	4599 212
	l.s	$f6, 140($sp)
	l.s	$f8, 324($sp)
	mul.s	$f10, $f8, $f0
	add.s	$f8, $f6, $f10
	s.s	$f8, 312($sp)
	.loc	4599 213
	l.s	$f6, 328($sp)
	mul.s	$f10, $f6, $f0
	add.s	$f8, $f4, $f10
	s.s	$f8, 316($sp)
	.loc	4599 214
	lh	$25, 210($sp)
	mtc1	$25, $f4
	cvt.s.w	$f2, $f4
	c.eq.s	$f2, $f8
	bc1f	$2214
	c.lt.s	$f6, $f28
	bc1f	$2214
	.loc	4599 214
	.loc	4599 215
	lw	$4, D_80124EEC
	addu	$5, $24, -1
	sll	$14, $5, 16
	move	$5, $14
	sra	$15, $5, 16
	move	$5, $15
	lh	$6, 210($sp)
	addu	$6, $6, -1
	sll	$25, $6, 16
	move	$6, $25
	sra	$24, $6, 16
	move	$6, $24
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	move	$13, $2
	.loc	4599 216
	l.s	$f10, 308($sp)
	s.s	$f10, 332($sp)
	l.s	$f4, 312($sp)
	s.s	$f4, 336($sp)
	l.s	$f8, 316($sp)
	s.s	$f8, 340($sp)
	b	$2234
$2214:
	.loc	4599 218
	mtc1	$8, $f6
	cvt.s.w	$f0, $f6
	l.s	$f10, 316($sp)
	c.eq.s	$f0, $f10
	bc1f	$2215
	l.s	$f4, 328($sp)
	c.lt.s	$f4, $f28
	bc1f	$2215
	.loc	4599 218
	.loc	4599 219
	lw	$4, D_80124EEC
	lh	$5, 214($sp)
	addu	$5, $5, -1
	sll	$14, $5, 16
	move	$5, $14
	sra	$15, $5, 16
	move	$5, $15
	addu	$6, $8, -1
	sll	$25, $6, 16
	move	$6, $25
	sra	$24, $6, 16
	move	$6, $24
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 220
	l.s	$f8, 308($sp)
	s.s	$f8, 332($sp)
	l.s	$f6, 312($sp)
	s.s	$f6, 336($sp)
	l.s	$f10, 316($sp)
	s.s	$f10, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2215:
	.loc	4599 222
	l.s	$f4, 316($sp)
	c.le.s	$f2, $f4
	bc1f	$2223
	c.le.s	$f4, $f0
	bc1f	$2223
	.loc	4599 222
	.loc	4599 223
	c.lt.s	$f4, $f28
	bc1f	$2216
	trunc.w.s	$f8, $f4, $14
	mfc1	$15, $f8
	mtc1	$15, $f6
	cvt.s.w	$f10, $f6
	c.eq.s	$f10, $f4
	bc1t	$2216
	sub.s	$f0, $f4, $f30
	b	$2217
$2216:
	l.s	$f0, 316($sp)
$2217:
	lw	$4, D_80124EEC
	lh	$5, 214($sp)
	addu	$5, $5, -1
	sll	$25, $5, 16
	move	$5, $25
	sra	$24, $5, 16
	move	$5, $24
	trunc.w.s	$f8, $f0, $14
	mfc1	$6, $f8
	sll	$15, $6, 16
	move	$6, $15
	sra	$25, $6, 16
	move	$6, $25
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 224
	l.s	$f6, 308($sp)
	s.s	$f6, 332($sp)
	l.s	$f10, 312($sp)
	s.s	$f10, 336($sp)
	l.s	$f4, 316($sp)
	s.s	$f4, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2218:
	.loc	4599 226
	l.s	$f8, 320($sp)
	c.lt.s	$f28, $f8
	bc1f	$2223
	.loc	4599 226
	.loc	4599 227
	mtc1	$10, $f6
	cvt.s.w	$f10, $f6
	s.s	$f10, 308($sp)
	.loc	4599 228
	l.s	$f4, 308($sp)
	l.s	$f6, 332($sp)
	sub.s	$f10, $f4, $f6
	div.s	$f0, $f10, $f8
	.loc	4599 229
	l.s	$f4, 336($sp)
	l.s	$f6, 324($sp)
	mul.s	$f10, $f6, $f0
	add.s	$f8, $f4, $f10
	s.s	$f8, 312($sp)
	.loc	4599 230
	l.s	$f6, 340($sp)
	l.s	$f4, 328($sp)
	mul.s	$f10, $f4, $f0
	add.s	$f8, $f6, $f10
	s.s	$f8, 316($sp)
	.loc	4599 231
	lh	$24, 210($sp)
	mtc1	$24, $f6
	cvt.s.w	$f2, $f6
	c.eq.s	$f2, $f8
	bc1f	$2219
	c.lt.s	$f4, $f28
	bc1f	$2219
	.loc	4599 231
	.loc	4599 232
	lw	$4, D_80124EEC
	sll	$5, $10, 16
	sra	$14, $5, 16
	move	$5, $14
	addu	$6, $24, -1
	sll	$15, $6, 16
	move	$6, $15
	sra	$25, $6, 16
	move	$6, $25
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 233
	l.s	$f10, 308($sp)
	s.s	$f10, 332($sp)
	l.s	$f6, 312($sp)
	s.s	$f6, 336($sp)
	l.s	$f8, 316($sp)
	s.s	$f8, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2219:
	.loc	4599 235
	mtc1	$8, $f4
	cvt.s.w	$f0, $f4
	l.s	$f10, 316($sp)
	c.eq.s	$f0, $f10
	bc1f	$2220
	l.s	$f6, 328($sp)
	c.lt.s	$f6, $f28
	bc1f	$2220
	.loc	4599 235
	.loc	4599 236
	lw	$4, D_80124EEC
	sll	$5, $10, 16
	sra	$14, $5, 16
	move	$5, $14
	addu	$6, $8, -1
	sll	$24, $6, 16
	move	$6, $24
	sra	$15, $6, 16
	move	$6, $15
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 237
	l.s	$f8, 308($sp)
	s.s	$f8, 332($sp)
	l.s	$f4, 312($sp)
	s.s	$f4, 336($sp)
	l.s	$f10, 316($sp)
	s.s	$f10, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2220:
	.loc	4599 239
	l.s	$f6, 316($sp)
	c.le.s	$f2, $f6
	bc1f	$2223
	c.le.s	$f6, $f0
	bc1f	$2223
	.loc	4599 239
	.loc	4599 240
	c.lt.s	$f6, $f28
	bc1f	$2221
	trunc.w.s	$f8, $f6, $25
	mfc1	$14, $f8
	mtc1	$14, $f4
	cvt.s.w	$f10, $f4
	c.eq.s	$f10, $f6
	bc1t	$2221
	sub.s	$f0, $f6, $f30
	b	$2222
$2221:
	l.s	$f0, 316($sp)
$2222:
	lw	$4, D_80124EEC
	sll	$5, $10, 16
	sra	$24, $5, 16
	move	$5, $24
	trunc.w.s	$f8, $f0, $15
	mfc1	$6, $f8
	sll	$25, $6, 16
	move	$6, $25
	sra	$14, $6, 16
	move	$6, $14
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 241
	l.s	$f4, 308($sp)
	s.s	$f4, 332($sp)
	l.s	$f10, 312($sp)
	s.s	$f10, 336($sp)
	l.s	$f6, 316($sp)
	s.s	$f6, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2223:
	.loc	4599 244
	l.s	$f8, 328($sp)
	c.lt.s	$f8, $f28
	bc1f	$2228
	.loc	4599 244
	.loc	4599 245
	lh	$24, 210($sp)
	mtc1	$24, $f4
	cvt.s.w	$f10, $f4
	s.s	$f10, 316($sp)
	.loc	4599 246
	l.s	$f6, 340($sp)
	sub.s	$f4, $f10, $f6
	div.s	$f0, $f4, $f8
	.loc	4599 247
	l.s	$f10, 336($sp)
	l.s	$f6, 324($sp)
	mul.s	$f4, $f6, $f0
	add.s	$f8, $f10, $f4
	s.s	$f8, 312($sp)
	.loc	4599 248
	l.s	$f6, 332($sp)
	l.s	$f10, 320($sp)
	mul.s	$f4, $f10, $f0
	add.s	$f8, $f6, $f4
	s.s	$f8, 308($sp)
	.loc	4599 249
	lh	$15, 214($sp)
	mtc1	$15, $f6
	cvt.s.w	$f2, $f6
	c.eq.s	$f2, $f8
	bc1f	$2224
	c.lt.s	$f10, $f28
	bc1f	$2224
	.loc	4599 249
	.loc	4599 250
	lw	$4, D_80124EEC
	lh	$5, 214($sp)
	addu	$5, $5, -1
	sll	$25, $5, 16
	move	$5, $25
	sra	$14, $5, 16
	move	$5, $14
	addu	$6, $24, -1
	sll	$15, $6, 16
	move	$6, $15
	sra	$25, $6, 16
	move	$6, $25
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 251
	l.s	$f4, 308($sp)
	s.s	$f4, 332($sp)
	l.s	$f6, 312($sp)
	s.s	$f6, 336($sp)
	l.s	$f8, 316($sp)
	s.s	$f8, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2224:
	.loc	4599 253
	mtc1	$10, $f10
	cvt.s.w	$f0, $f10
	l.s	$f4, 308($sp)
	c.eq.s	$f0, $f4
	bc1f	$2225
	l.s	$f6, 320($sp)
	c.lt.s	$f6, $f28
	bc1f	$2225
	.loc	4599 253
	.loc	4599 254
	lw	$4, D_80124EEC
	addu	$5, $10, -1
	sll	$14, $5, 16
	move	$5, $14
	sra	$24, $5, 16
	move	$5, $24
	lh	$6, 210($sp)
	addu	$6, $6, -1
	sll	$15, $6, 16
	move	$6, $15
	sra	$25, $6, 16
	move	$6, $25
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 255
	l.s	$f8, 308($sp)
	s.s	$f8, 332($sp)
	l.s	$f10, 312($sp)
	s.s	$f10, 336($sp)
	l.s	$f4, 316($sp)
	s.s	$f4, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2225:
	.loc	4599 257
	l.s	$f6, 308($sp)
	c.le.s	$f2, $f6
	bc1f	$2233
	c.le.s	$f6, $f0
	bc1f	$2233
	.loc	4599 257
	.loc	4599 258
	c.lt.s	$f6, $f28
	bc1f	$2226
	trunc.w.s	$f8, $f6, $14
	mfc1	$24, $f8
	mtc1	$24, $f10
	cvt.s.w	$f4, $f10
	c.eq.s	$f4, $f6
	bc1t	$2226
	sub.s	$f0, $f6, $f30
	b	$2227
$2226:
	l.s	$f0, 308($sp)
$2227:
	lw	$4, D_80124EEC
	trunc.w.s	$f8, $f0, $15
	mfc1	$5, $f8
	sll	$25, $5, 16
	move	$5, $25
	sra	$14, $5, 16
	move	$5, $14
	lh	$6, 210($sp)
	addu	$6, $6, -1
	sll	$24, $6, 16
	move	$6, $24
	sra	$15, $6, 16
	move	$6, $15
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 259
	l.s	$f10, 308($sp)
	s.s	$f10, 332($sp)
	l.s	$f4, 312($sp)
	s.s	$f4, 336($sp)
	l.s	$f6, 316($sp)
	s.s	$f6, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2228:
	.loc	4599 261
	l.s	$f8, 328($sp)
	c.lt.s	$f28, $f8
	bc1f	$2233
	.loc	4599 261
	.loc	4599 262
	mtc1	$8, $f10
	cvt.s.w	$f4, $f10
	s.s	$f4, 316($sp)
	.loc	4599 263
	l.s	$f6, 316($sp)
	l.s	$f10, 340($sp)
	sub.s	$f4, $f6, $f10
	div.s	$f0, $f4, $f8
	.loc	4599 264
	l.s	$f6, 336($sp)
	l.s	$f10, 324($sp)
	mul.s	$f4, $f10, $f0
	add.s	$f8, $f6, $f4
	s.s	$f8, 312($sp)
	.loc	4599 265
	l.s	$f10, 332($sp)
	l.s	$f6, 320($sp)
	mul.s	$f4, $f6, $f0
	add.s	$f8, $f10, $f4
	s.s	$f8, 308($sp)
	.loc	4599 266
	lh	$25, 214($sp)
	mtc1	$25, $f10
	cvt.s.w	$f2, $f10
	c.eq.s	$f2, $f8
	bc1f	$2229
	c.lt.s	$f6, $f28
	bc1f	$2229
	.loc	4599 266
	.loc	4599 267
	lw	$4, D_80124EEC
	addu	$5, $25, -1
	sll	$14, $5, 16
	move	$5, $14
	sra	$24, $5, 16
	move	$5, $24
	sll	$6, $8, 16
	sra	$15, $6, 16
	move	$6, $15
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 268
	l.s	$f4, 308($sp)
	s.s	$f4, 332($sp)
	l.s	$f10, 312($sp)
	s.s	$f10, 336($sp)
	l.s	$f8, 316($sp)
	s.s	$f8, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2229:
	.loc	4599 270
	mtc1	$10, $f6
	cvt.s.w	$f0, $f6
	l.s	$f4, 308($sp)
	c.eq.s	$f0, $f4
	bc1f	$2230
	l.s	$f10, 320($sp)
	c.lt.s	$f10, $f28
	bc1f	$2230
	.loc	4599 270
	.loc	4599 271
	lw	$4, D_80124EEC
	addu	$5, $10, -1
	sll	$25, $5, 16
	move	$5, $25
	sra	$14, $5, 16
	move	$5, $14
	sll	$6, $8, 16
	sra	$24, $6, 16
	move	$6, $24
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 272
	l.s	$f8, 308($sp)
	s.s	$f8, 332($sp)
	l.s	$f6, 312($sp)
	s.s	$f6, 336($sp)
	l.s	$f4, 316($sp)
	s.s	$f4, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2230:
	.loc	4599 274
	l.s	$f10, 308($sp)
	c.le.s	$f2, $f10
	bc1f	$2233
	c.le.s	$f10, $f0
	bc1f	$2233
	.loc	4599 274
	.loc	4599 275
	c.lt.s	$f10, $f28
	bc1f	$2231
	trunc.w.s	$f8, $f10, $15
	mfc1	$25, $f8
	mtc1	$25, $f6
	cvt.s.w	$f4, $f6
	c.eq.s	$f4, $f10
	bc1t	$2231
	sub.s	$f0, $f10, $f30
	b	$2232
$2231:
	l.s	$f0, 308($sp)
$2232:
	lw	$4, D_80124EEC
	trunc.w.s	$f8, $f0, $14
	mfc1	$5, $f8
	sll	$24, $5, 16
	move	$5, $24
	sra	$15, $5, 16
	move	$5, $15
	sll	$6, $8, 16
	sra	$25, $6, 16
	move	$6, $25
	addu	$7, $sp, 358
	.livereg	0x0F00000E,0x00000000
	jal	handbrake_apply
	sw	$2, 196($sp)
	.loc	4599 276
	l.s	$f6, 308($sp)
	s.s	$f6, 332($sp)
	l.s	$f4, 312($sp)
	s.s	$f4, 336($sp)
	l.s	$f10, 316($sp)
	s.s	$f10, 340($sp)
	lw	$13, 196($sp)
	b	$2234
$2233:
	.loc	4599 279
	l.s	$f8, 308($sp)
	s.s	$f8, 332($sp)
	l.s	$f6, 312($sp)
	s.s	$f6, 336($sp)
	l.s	$f4, 316($sp)
	s.s	$f4, 340($sp)
$2234:
	.loc	4599 280
	.loc	4599 281
	.loc	4599 282
	.loc	4599 283
	bne	$13, 0, $2235
	.loc	4599 283
	.loc	4599 284
	move	$2, $0
	b	$2236
$2235:
	.loc	4599 149
	lh	$14, 358($sp)
	and	$6, $14, 1
	and	$9, $14, 2
	mul	$24, $14, 2
	addu	$11, $13, $24
	li	$15, 16
	sll	$12, $15, $14
	b	$2195
$2236:
	.livereg	0x2000FF0E,0x00000FFF
	l.d	$f20, 48($sp)
	l.d	$f22, 56($sp)
	l.d	$f24, 64($sp)
	l.d	$f26, 72($sp)
	l.d	$f28, 80($sp)
	l.d	$f30, 88($sp)
	lw	$16, 96($sp)
	lw	$17, 100($sp)
	lw	$18, 104($sp)
	lw	$19, 108($sp)
	lw	$20, 112($sp)
	lw	$21, 116($sp)
	lw	$22, 120($sp)
	lw	$23, 124($sp)
	lw	$31, 132($sp)
	lw	$30, 128($sp)
	addu	$sp, 400
	j	$31
	.end	input_deadzone_apply
