	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 140
 # 140	{
	.ent	func_803AA3D8 2
func_803AA3D8:
	.option	O3
	subu	$sp, 176
	sw	$31, 36($sp)
	.mask	0x80000000, -140
	.frame	$sp, 176, $31
	.loc	2 140
	.loc	2 140
	.loc	2 146
 # 141	    s32 pad;
 # 142	    s32 i, x, y, cx;
 # 143	    u8 *str;
 # 144	    u8 str1[64];
 # 145	    s16 screenPos[2];
 # 146	    func_800B669C(0, 1);
	move	$4, $0
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 147
 # 147	    brake_light_update(0, D_803B5F28[17].position, &D_80150B70[0], 0, screenPos);
	move	$4, $0
	la	$5, D_803B5F28+1136
	la	$6, D_80150B70
	move	$7, $0
	addu	$14, $sp, 84
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 148
 # 148	    cx = screenPos[0];
	lh	$30, 84($sp)
	.loc	2 149
 # 149	    y = screenPos[1];
	lh	$20, 86($sp)
	.loc	2 150
 # 150	    func_800F6928(D_803B5F28[17].position[2] - 10.0f);
	l.s	$f4, D_803B5F28+1144
	li.s	$f6, 10.0
	sub.s	$f12, $f4, $f6
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 151
 # 151	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 152
 # 152	    object_create(13);
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 153
 # 153	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	la	$19, countdown_state
	.loc	2 154
 # 154	    str = countdown_state.text[47];
	.noalias	$19,$sp
	lw	$15, 4($19)
	lw	$17, 188($15)
	.loc	2 155
 # 155	    state_utility(sound_pitch_diff_halved(str, cx), y, str);
	move	$4, $17
	sll	$23, $30, 16
	sra	$24, $23, 16
	move	$23, $24
	sll	$5, $23, 16
	sra	$25, $5, 16
	move	$5, $25
	.livereg	0x0C00010E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $20, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 156
 # 156	    y += object_bytes_sum_global() * 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$24, $2, 2
	addu	$20, $20, $24
	.loc	2 157
 # 157	    object_create(10);
	li	$4, 10
	.livereg	0x0800000E,0x00000000
	jal	object_create
	li	$3, 1
	.loc	2 158
 # 158	    if (D_803BA84D == 1) {
	lb	$25, D_803BA84D
	bne	$3, $25, $231
	.loc	2 158
	.loc	2 159
 # 159	        render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 160
 # 160	        y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	.loc	2 161
 # 161	        if (D_803BA800) {
	lw	$14, D_803BA800
	beq	$14, 0, $225
	addu	$16, $sp, 88
	.loc	2 161
	.loc	2 162
 # 162	            fcvt_wrapper(str1, countdown_state.text[143], D_803BA840);
	move	$4, $16
	lw	$15, 4($19)
	lw	$5, 572($15)
	la	$6, D_803BA840
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 163
 # 163	            str = str1;
	move	$17, $16
	b	$226
$225:
	.loc	2 164
 # 164	        } else str = countdown_state.text[144];
	lw	$24, 4($19)
	lw	$17, 576($24)
$226:
	la	$18, D_803B8ACC
	.loc	2 165
 # 165	        crowd_cheer_play(D_803B6530, cx - D_803B8ACC.width / 2 - 4, y - 4,
	.noalias	$18,$19
	.noalias	$18,$sp
	lh	$4, 4($18)
	move	$5, $17
	.livereg	0x0C00000E,0x00000000
	jal	camera_zoom_fov
	sll	$16, $2, 16
	sra	$25, $16, 16
	move	$16, $25
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	lh	$3, 4($18)
	div	$14, $3, 2
	move	$3, $14
	subu	$5, $30, $3
	addu	$5, $5, -4
	addu	$6, $20, -4
	addu	$7, $3, $30
	addu	$7, $7, 4
	addu	$15, $16, 2
	mul	$24, $2, $15
	addu	$25, $24, $20
	addu	$14, $25, 4
	sw	$14, 16($sp)
	.livereg	0x1F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 167
 # 166	                         cx + D_803B8ACC.width / 2 + 4, 4 + y + ((camera_zoom_fov(D_803B8ACC.width, str) + 2) * object_bytes_sum_global()));
 # 167	        dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 168
 # 168	        func_800B669C(1, 3);
	li	$4, 1
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 169
 # 169	        camera_auto_follow(cx, y, D_803B8ACC.width, D_803B8ACC.height, -1, 0, str);
	sll	$4, $23, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	lh	$6, 4($18)
	lh	$7, 6($18)
	li	$25, -1
	sw	$25, 16($sp)
	sw	$0, 20($sp)
	sw	$17, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_auto_follow
	.loc	2 170
 # 170	        func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 171
 # 171	        y += object_bytes_sum_global() * (camera_zoom_fov(D_803B8ACC.width, str) + 1);
	lh	$4, 4($18)
	move	$5, $17
	.livereg	0x0C00000E,0x00000000
	jal	camera_zoom_fov
	.alias	$18,$19
	.alias	$18,$sp
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	la	$21, D_803BA850
	addu	$15, $16, 1
	mul	$24, $2, $15
	addu	$20, $20, $24
	.loc	2 172
 # 172	        str = NAME(24, 0);
	lw	$25, 16($19)
	lw	$14, 12($19)
	lhu	$15, 48($14)
	mul	$24, $15, 4
	addu	$14, $25, $24
	lw	$17, 0($14)
	.loc	2 173
 # 173	        dispatch_handler(D_803BA850 == 1 ? 22 : 1);
	.noalias	$21,$19
	.noalias	$21,$sp
	lb	$15, 0($21)
	bne	$15, 1, $227
	li	$4, 22
	b	$228
$227:
	li	$4, 1
$228:
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 174
 # 174	        state_utility(sound_pitch_diff_calc(str, cx - 5), y, str);
	move	$4, $17
	addu	$5, $30, -5
	sll	$25, $5, 16
	move	$5, $25
	sra	$24, $5, 16
	move	$5, $24
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$18, $20, 16
	sra	$15, $18, 16
	move	$18, $15
	sll	$5, $18, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $17
	.livereg	0x0E00200E,0x00000000
	jal	state_utility
	.loc	2 175
 # 175	        str = NAME(24, 1);
	lw	$24, 16($19)
	lw	$14, 12($19)
	lhu	$15, 48($14)
	mul	$25, $15, 4
	addu	$14, $24, $25
	lw	$17, 4($14)
	.loc	2 176
 # 176	        dispatch_handler(D_803BA850 == 0 ? 22 : 1);
	lb	$15, 0($21)
	bne	$15, 0, $229
	.alias	$19,$21
	.alias	$19,$sp
	.alias	$21,$sp
	li	$4, 22
	b	$230
$229:
	li	$4, 1
$230:
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 177
 # 177	        state_utility(cx + 5, y, str);
	addu	$4, $30, 5
	sll	$24, $4, 16
	move	$4, $24
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$274
$231:
	.loc	2 178
 # 178	    } else if (D_803BA854 == 1) {
	lb	$15, D_803BA854
	bne	$3, $15, $234
	.loc	2 178
	.loc	2 179
 # 179	        render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 180
 # 180	        y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	.loc	2 181
 # 181	        if (D_803BA800) {
	lw	$24, D_803BA800
	beq	$24, 0, $232
	addu	$16, $sp, 88
	.loc	2 181
	.loc	2 182
 # 182	            fcvt_wrapper(str1, countdown_state.text[146], D_803BA840);
	move	$4, $16
	.noalias	$19,$sp
	lw	$25, 4($19)
	lw	$5, 584($25)
	la	$6, D_803BA840
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.alias	$19,$sp
	.loc	2 183
 # 183	            str = str1;
	move	$17, $16
	b	$233
$232:
	.loc	2 184
 # 184	        } else str = countdown_state.text[147];
	.noalias	$19,$sp
	lw	$14, 4($19)
	lw	$17, 588($14)
	.alias	$19,$sp
$233:
	la	$18, D_803B8ACC
	.loc	2 185
 # 185	        crowd_cheer_play(D_803B6530, cx - D_803B8ACC.width / 2 - 4, y - 4,
	.noalias	$18,$sp
	lh	$4, 4($18)
	move	$5, $17
	.livereg	0x0C00000E,0x00000000
	jal	camera_zoom_fov
	sll	$16, $2, 16
	sra	$15, $16, 16
	move	$16, $15
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	lh	$3, 4($18)
	div	$24, $3, 2
	move	$3, $24
	subu	$5, $30, $3
	addu	$5, $5, -4
	addu	$6, $20, -4
	addu	$7, $3, $30
	addu	$7, $7, 4
	mul	$25, $2, $16
	addu	$14, $25, $20
	addu	$15, $14, 4
	sw	$15, 16($sp)
	.livereg	0x1F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 187
 # 186	                         cx + D_803B8ACC.width / 2 + 4, 4 + y + (camera_zoom_fov(D_803B8ACC.width, str) * object_bytes_sum_global()));
 # 187	        dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 188
 # 188	        func_800B669C(1, 3);
	li	$4, 1
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 189
 # 189	        camera_auto_follow(cx, y, D_803B8ACC.width, D_803B8ACC.height, -1, 0, str);
	sll	$4, $23, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $20, 16
	sra	$25, $5, 16
	move	$5, $25
	lh	$6, 4($18)
	lh	$7, 6($18)
	li	$14, -1
	sw	$14, 16($sp)
	sw	$0, 20($sp)
	sw	$17, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	camera_auto_follow
	.alias	$18,$sp
	.loc	2 190
 # 190	        func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	b	$274
$234:
	.loc	2 191
 # 191	    } else {
	.loc	2 192
 # 192	        for (i = 0; i < 11; i++) {
	move	$16, $0
	la	$22, D_803B5F28
	la	$15, D_803B5F28+48
	sw	$15, 52($sp)
	li.s	$f30, 25.0
	li.s	$f28, 45.0
	li.s	$f26, 1.5707964
	li.s	$f24, 2.443461
	li.s	$f22, 0.6981317
	li.s	$f20, 300.0
$235:
	li	$3, 1
	.loc	2 192
	.loc	2 193
 # 193	            crowd_cheer_play(D_803B6530, -20, 0, -10, 10);
	lw	$4, D_803B6530
	li	$5, -20
	move	$6, $0
	li	$7, -10
	li	$24, 10
	sw	$24, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 194
 # 194	            func_800F6928(D_803B5F28[i].position[2] - 10.0f);
	.noalias	$22,$sp
	l.s	$f8, 56($22)
	li.s	$f10, 10.0
	sub.s	$f12, $f8, $f10
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 195
 # 195	            brake_light_update(0, D_803B5F28[i].position, &D_80150B70[0], 0, screenPos);
	move	$4, $0
	lw	$5, 52($sp)
	la	$6, D_80150B70
	move	$7, $0
	addu	$25, $sp, 84
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 196
 # 196	            if (D_803B5F28[i].kind == 11) x = screenPos[0];
	lw	$2, 0($22)
	bne	$2, 11, $236
	.loc	2 196
	lh	$21, 84($sp)
	b	$238
$236:
	.loc	2 197
 # 197	            else if (D_803B5F28[i].kind == 9) x = screenPos[0] - 65.0f * D_803B64E8 * 300.0f / D_803B64FC;
	bne	$2, 9, $237
	.loc	2 197
	li.s	$f4, 65.0
	l.s	$f6, D_803B64E8
	mul.s	$f8, $f4, $f6
	mul.s	$f10, $f8, $f20
	l.s	$f4, D_803B64FC
	div.s	$f6, $f10, $f4
	lh	$14, 84($sp)
	mtc1	$14, $f8
	cvt.s.w	$f10, $f8
	sub.s	$f4, $f10, $f6
	trunc.w.s	$f8, $f4, $15
	mfc1	$21, $f8
	b	$238
$237:
	.loc	2 198
 # 198	            else x = screenPos[0] - 45.0f * D_803B64E8 * 300.0f / D_803B64FC;
	l.s	$f10, D_803B64E8
	mul.s	$f6, $f28, $f10
	mul.s	$f4, $f6, $f20
	l.s	$f8, D_803B64FC
	div.s	$f10, $f4, $f8
	lh	$24, 84($sp)
	mtc1	$24, $f6
	cvt.s.w	$f4, $f6
	sub.s	$f8, $f4, $f10
	trunc.w.s	$f6, $f8, $25
	mfc1	$21, $f6
$238:
	.loc	2 199
 # 199	            y = screenPos[1];
	lh	$20, 86($sp)
	.loc	2 200
 # 200	            if (i == 1) {
	bne	$16, 1, $239
	.loc	2 200
	.loc	2 201
 # 201	                brake_light_update(0, D_803B5F28[14].position, &D_80150B70[0], 0, screenPos);
	move	$4, $0
	la	$5, D_803B5F28+944
	la	$6, D_80150B70
	move	$7, $0
	addu	$14, $sp, 84
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 202
 # 202	                cx = screenPos[0] - 25.0f * D_803B64E8 * 300.0f / D_803B64FC;
	l.s	$f4, D_803B64E8
	mul.s	$f10, $f30, $f4
	mul.s	$f8, $f10, $f20
	l.s	$f6, D_803B64FC
	div.s	$f4, $f8, $f6
	lh	$15, 84($sp)
	mtc1	$15, $f10
	cvt.s.w	$f8, $f10
	sub.s	$f6, $f8, $f4
	trunc.w.s	$f10, $f6, $24
	mfc1	$30, $f10
	b	$240
$239:
	.loc	2 203
 # 203	            } else {
	.loc	2 204
 # 204	                brake_light_update(0, D_803B5F28[13].position, &D_80150B70[0], 0, screenPos);
	move	$4, $0
	la	$5, D_803B5F28+880
	la	$6, D_80150B70
	move	$7, $0
	addu	$25, $sp, 84
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 205
 # 205	                cx = screenPos[0] - 45.0f * D_803B64E8 * 300.0f / D_803B64FC;
	l.s	$f8, D_803B64E8
	mul.s	$f4, $f28, $f8
	mul.s	$f6, $f4, $f20
	l.s	$f10, D_803B64FC
	div.s	$f8, $f6, $f10
	lh	$14, 84($sp)
	mtc1	$14, $f4
	cvt.s.w	$f6, $f4
	sub.s	$f10, $f6, $f8
	trunc.w.s	$f4, $f10, $15
	mfc1	$30, $f4
$240:
	.loc	2 207
 # 206	            }
 # 207	            if (i >= 3 && i < 11 && i == D_803BA808 &&
	blt	$16, 3, $244
	bge	$16, 11, $244
	lw	$24, D_803BA808
	bne	$16, $24, $244
	l.s	$f0, D_803B5F28+840
	c.lt.s	$f0, $f22
	bc1t	$241
	c.lt.s	$f24, $f0
	bc1f	$244
$241:
	lbu	$2, D_803B5F28+892
	beq	$2, 0, $244
	.loc	2 208
 # 208	                (D_803B5F28[13].angle < 0.6981317f || D_803B5F28[13].angle > 2.443461f) && D_803B5F28[13].alpha) {
	.loc	2 209
 # 209	                func_800ED66C(D_803B5F28[13].alpha);
	mtc1	$2, $f6
	cvt.s.w	$f12, $f6
	bge	$2, $0, $275
	li.s	$f8, 4294967296.0
	add.s	$f12, $f12, $f8
$275:
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 210
 # 210	                if (D_803B5F28[13].angle < 1.5707964f) {
	l.s	$f10, D_803B5F28+840
	c.lt.s	$f10, $f26
	bc1f	$242
	.loc	2 210
	.loc	2 211
 # 211	                    func_800BEA3C(D_801146BC, D_801146C0);
	lw	$25, D_801146BC
	sw	$25, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 212
 # 212	                    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$243
$242:
	.loc	2 213
 # 213	                } else {
	.loc	2 214
 # 214	                    func_800BEA3C(D_801146BC, D_801146C0);
	lw	$15, D_801146BC
	sw	$15, 0($sp)
	lw	$24, D_801146C0
	sw	$24, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 215
 # 215	                    dispatch_handler(22);
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$243:
	.loc	2 217
 # 216	                }
 # 217	                str = NAME(50, 2);
	.noalias	$19,$22
	.noalias	$19,$sp
	lw	$25, 16($19)
	lw	$14, 12($19)
	lhu	$15, 100($14)
	mul	$24, $15, 4
	addu	$14, $25, $24
	lw	$17, 8($14)
	.loc	2 218
 # 218	                object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 219
 # 219	                state_utility(cx, y, str);
	sll	$4, $30, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $20, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 220
 # 220	                object_create(10);
	li	$4, 10
	.livereg	0x0800000E,0x00000000
	jal	object_create
$244:
	.loc	2 222
 # 221	            }
 # 222	            if (D_803B5F28[i].angle > 0.6981317f && D_803B5F28[i].angle < 2.443461f) continue;
	l.s	$f0, 8($22)
	c.lt.s	$f22, $f0
	bc1f	$245
	c.lt.s	$f0, $f24
	bc1t	$273
	.loc	2 222
$245:
	.loc	2 223
 # 223	            if (!D_803B5F28[i].alpha) continue;
	lbu	$2, 60($22)
	beq	$2, 0, $273
	.loc	2 223
	.loc	2 224
 # 224	            func_800ED66C(D_803B5F28[i].alpha);
	mtc1	$2, $f4
	cvt.s.w	$f12, $f4
	bge	$2, $0, $276
	li.s	$f6, 4294967296.0
	add.s	$f12, $f12, $f6
$276:
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 225
 # 225	            if (D_803B5F28[i].angle < 1.5707964f) {
	l.s	$f8, 8($22)
	c.lt.s	$f8, $f26
	bc1f	$246
	.loc	2 225
	.loc	2 226
 # 226	                func_800BEA3C(D_801146BC, D_801146C0);
	lw	$24, D_801146BC
	sw	$24, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 227
 # 227	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$247
$246:
	.loc	2 228
 # 228	            } else {
	.loc	2 229
 # 229	                func_800BEA3C(D_801146BC, D_801146C0);
	lw	$15, D_801146BC
	sw	$15, 0($sp)
	lw	$25, D_801146C0
	sw	$25, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 230
 # 230	                dispatch_handler(22);
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$247:
	.loc	2 232
 # 231	            }
 # 232	            if (i == 0) {
	bne	$16, 0, $248
	.loc	2 232
	.loc	2 233
 # 233	                str = NAME(50, 0);
	lw	$24, 16($19)
	lw	$14, 12($19)
	lhu	$15, 100($14)
	mul	$25, $15, 4
	addu	$14, $24, $25
	lw	$17, 0($14)
	.loc	2 234
 # 234	                state_utility(x, y, str);
	sll	$4, $21, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$272
$248:
	.loc	2 235
 # 235	            } else if (i == 1) {
	bne	$16, 1, $249
	.loc	2 235
	.loc	2 236
 # 236	                str = NAME(50, 1);
	lw	$25, 16($19)
	lw	$14, 12($19)
	lhu	$15, 100($14)
	mul	$24, $15, 4
	addu	$14, $25, $24
	lw	$17, 4($14)
	.loc	2 237
 # 237	                state_utility(x, y, str);
	sll	$4, $21, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$18, $20, 16
	sra	$25, $18, 16
	move	$18, $25
	sll	$5, $18, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $17
	sll	$23, $30, 16
	sra	$14, $23, 16
	move	$23, $14
	.livereg	0x0E00210E,0x00000000
	jal	state_utility
	.loc	2 238
 # 238	                str = D_8011EAD0[D_803B65A4];
	lb	$15, D_803B65A4
	mul	$25, $15, 4
	lw	$17, D_8011EAD0($25)
	.loc	2 239
 # 239	                state_utility(cx, y, str);
	sll	$4, $23, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$272
$249:
	.loc	2 240
 # 240	            } else if (i == 2) {
	bne	$16, 2, $258
	.loc	2 240
	.loc	2 241
 # 241	                switch (D_803BA830[D_803B65A4]) {
	sll	$18, $20, 16
	sra	$15, $18, 16
	move	$18, $15
	lb	$25, D_803B65A4
	mul	$24, $25, 4
	lw	$14, D_803BA830($24)
	addu	$15, $14, 4
	bgeu	$15, 5, $257
	sll	$15, $15, 2
	lw	$15, $250($15)
	j	$15
	.rdata	
$250:
	.word	$252
	.word	$253
	.word	$255
	.word	$256
	.word	$254
	.text	
$252:
	.loc	2 242
 # 242	                case -4: str = countdown_state.text[165]; break;
	lw	$25, 4($19)
	lw	$17, 660($25)
	.loc	2 242
	b	$257
$253:
	.loc	2 243
 # 243	                case -3: str = countdown_state.text[169]; break;
	lw	$24, 4($19)
	lw	$17, 676($24)
	.loc	2 243
	b	$257
$254:
	.loc	2 244
 # 244	                case 0: str = countdown_state.text[148]; break;
	lw	$14, 4($19)
	lw	$17, 592($14)
	.loc	2 244
	b	$257
$255:
	.loc	2 245
 # 245	                case -2: str = countdown_state.text[166]; break;
	lw	$15, 4($19)
	lw	$17, 664($15)
	.loc	2 245
	b	$257
$256:
	.loc	2 246
 # 246	                case -1: str = countdown_state.text[168]; break;
	lw	$25, 4($19)
	lw	$17, 672($25)
	.loc	2 246
$257:
	.loc	2 248
 # 247	                }
 # 248	                dispatch_handler(0);
	move	$4, $0
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 249
 # 249	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $17
	sll	$5, $21, 16
	sra	$24, $5, 16
	move	$5, $24
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$272
$258:
	.loc	2 250
 # 250	            } else if (i >= 3 && i < 11) {
	blt	$16, 3, $272
	bge	$16, 11, $272
	.loc	2 250
	.loc	2 253
 # 251	                Profile **player;
 # 252	                s32 playerIdx;
 # 253	                player = D_803BA800;
	lw	$2, D_803BA800
	.loc	2 254
 # 254	                if (!player) {
	sll	$18, $20, 16
	sra	$25, $18, 16
	move	$18, $25
	move	$5, $0
	bne	$2, 0, $262
	.loc	2 254
	.loc	2 255
 # 255	                    for (playerIdx = 0, player = D_8012E6E0; player; player = (*player)->next) {
	lw	$2, D_8012E6E0
	beq	$2, 0, $270
	lb	$7, D_803B65A4
$259:
	.loc	2 255
	.loc	2 256
 # 256	                        if ((*(*player)->file)->controller == D_803B65A4) {
	lw	$3, 0($2)
	lw	$24, 8($3)
	lw	$14, 0($24)
	lbu	$15, 16($14)
	bne	$7, $15, $261
	.loc	2 256
	.loc	2 257
 # 257	                            if (playerIdx == i - 3) str = (*player)->name;
	addu	$25, $5, 3
	bne	$16, $25, $260
	.loc	2 257
	addu	$17, $3, 20
$260:
	.loc	2 258
 # 258	                            playerIdx++;
	addu	$5, $5, 1
$261:
	.loc	2 255
 # 255	                    for (playerIdx = 0, player = D_8012E6E0; player; player = (*player)->next) {
	lw	$2, 0($3)
	bne	$2, 0, $259
	b	$270
$262:
	.loc	2 261
 # 256	                        if ((*(*player)->file)->controller == D_803B65A4) {
 # 257	                            if (playerIdx == i - 3) str = (*player)->name;
 # 258	                            playerIdx++;
 # 259	                        }
 # 260	                    }
 # 261	                } else {
	.loc	2 262
 # 262	                    for (playerIdx = 0; player; ) {
	beq	$2, 0, $270
	lw	$6, D_803BA808
	lb	$7, D_803B65A4
$263:
	.loc	2 262
	.loc	2 263
 # 263	                        if ((*(*player)->file)->controller == D_803B65A4) {
	lw	$3, 0($2)
	lw	$24, 8($3)
	lw	$14, 0($24)
	lbu	$15, 16($14)
	bne	$7, $15, $267
	.loc	2 263
	.loc	2 264
 # 264	                            if (playerIdx == (i - D_803BA808 >= 0 ? i - D_803BA808 : -(i - D_803BA808))) str = (*player)->name;
	subu	$2, $16, $6
	blt	$2, 0, $264
	move	$4, $2
	b	$265
$264:
	negu	$4, $2
$265:
	bne	$4, $5, $266
	.loc	2 264
	addu	$17, $3, 20
$266:
	.loc	2 265
 # 265	                            playerIdx++;
	addu	$5, $5, 1
$267:
	.loc	2 267
 # 266	                        }
 # 267	                        player = i < D_803BA808 ? (*player)->previous : (*player)->next;
	bge	$16, $6, $268
	lw	$2, 4($3)
	b	$269
$268:
	lw	$2, 0($3)
$269:
	.loc	2 262
 # 262	                    for (playerIdx = 0; player; ) {
	bne	$2, 0, $263
$270:
	.loc	2 270
 # 270	                if ((u32)object_manager_update(str, -1) > 140) mode_byte_set(0);
	move	$4, $17
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	bltu	$2, 141, $271
	.loc	2 270
	move	$4, $0
	.livereg	0x0800000E,0x00000000
	jal	mode_byte_set
$271:
	.loc	2 271
 # 271	                music_fade(x, y, 98, str);
	sll	$4, $21, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$24, $5, 16
	move	$5, $24
	li	$6, 98
	move	$7, $17
	.livereg	0x0F00000E,0x00000000
	jal	music_fade
	.loc	2 272
 # 272	                mode_byte_set(-1);
	li	$4, -1
	.livereg	0x0800000E,0x00000000
	jal	mode_byte_set
$272:
	.loc	2 274
 # 273	            }
 # 274	            object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	.loc	2 192
 # 192	        for (i = 0; i < 11; i++) {
$273:
	addu	$16, $16, 1
	addu	$22, $22, 64
	lw	$14, 52($sp)
	addu	$15, $14, 64
	sw	$15, 52($sp)
	bne	$16, 11, $235
	.alias	$19,$22
	.alias	$19,$sp
	.alias	$22,$sp
	.loc	2 277
 # 277	}
$274:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 36($sp)
	addu	$sp, 176
	j	$31
	.end	func_803AA3D8
	.verstamp	3 19
	.option	pic0
	.extern	D_803BA90C 4
	.extern	countdown_state 20
	.extern	D_803BA84D 1
	.extern	D_803BA800 4
	.extern	D_803B6530 4
	.extern	D_803B8ACC 8
	.extern	D_803BA850 1
	.extern	D_803BA854 1
	.extern	D_803B64E8 4
	.extern	D_803B64FC 4
	.extern	D_803BA808 4
	.extern	D_801146BC 4
	.extern	D_801146C0 4
	.extern	D_803B65A4 1
	.extern	D_8012E6E0 4
	.extern	D_8014A108 2
	.extern	D_803BA858 1
	.extern	D_80146111 1
	.extern	D_803BA860 1
	.extern	D_803BA880 4
	.extern	D_803BA804 4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 279
 # 279	{
	.ent	func_803AAF0C 2
func_803AAF0C:
	.option	O3
	subu	$sp, 32
	sw	$31, 28($sp)
	.mask	0x80000000, -4
	.frame	$sp, 32, $31
	.loc	2 279
	la	$20, D_803BA800
	.loc	2 279
	.loc	2 282
 # 280	    Profile **saved_owner;
 # 281	    s16 saved_players,i,y,bottom;
 # 282	    s32 all_empty=1,empty;
	li	$23, 1
	.loc	2 284
 # 283	    u8 *label;
 # 284	    if(!D_803BA800)return;
	.noalias	$20,$sp
	lw	$14, 0($20)
	beq	$14, 0, $50
	.alias	$20,$sp
	.loc	2 284
	.loc	2 285
 # 285	    func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	la	$18, D_8014A118
	la	$19, D_8014A108
	li	$30, 1
	.loc	2 286
 # 286	    saved_owner=D_8014A118[0].owner;saved_players=D_8014A108;
	.noalias	$18,$sp
	lw	$16, 72($18)
	.loc	2 286
	.noalias	$19,$18
	.noalias	$19,$sp
	lh	$17, 0($19)
	.loc	2 287
 # 287	    D_8014A108=1;D_8014A118[0].owner=D_803BA800;
	sh	$30, 0($19)
	.loc	2 287
	.noalias	$20,$18
	.noalias	$20,$19
	.noalias	$20,$sp
	lw	$15, 0($20)
	sw	$15, 72($18)
	.loc	2 288
 # 288	    net_state_validate();
	.livereg	0x0000000E,0x00000000
	jal	net_state_validate
	.alias	$20,$18
	.alias	$20,$19
	.alias	$20,$sp
	.loc	2 289
 # 289	    D_8014A118[0].owner=saved_owner;D_8014A108=saved_players;
	sw	$16, 72($18)
	.loc	2 289
	sh	$17, 0($19)
	.loc	2 290
 # 290	    dispatch_handler(1);
	move	$4, $30
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.alias	$18,$19
	.alias	$18,$sp
	.alias	$19,$sp
	.loc	2 291
 # 291	    object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	la	$19, countdown_state
	.loc	2 292
 # 292	    y=90;
	.loc	2 293
 # 293	    label=NAME(34,0);
	.noalias	$19,$sp
	lw	$24, 16($19)
	lw	$25, 12($19)
	lhu	$14, 68($25)
	mul	$15, $14, 4
	addu	$25, $24, $15
	lw	$17, 0($25)
	.loc	2 294
 # 294	    state_utility(sound_pitch_diff_halved(label,80),y,label);
	move	$4, $17
	li	$5, 80
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	li	$5, 90
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 295
 # 295	    y+=object_bytes_sum_global();object_create(12);empty=1;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $2, 90
	sll	$24, $18, 16
	move	$18, $24
	sra	$15, $18, 16
	move	$18, $15
	.loc	2 295
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 295
	move	$21, $30
	.loc	2 296
 # 296	    for(i=3;i<9;i++)if(D_80150ED8[i]) {
	li	$16, 3
	la	$20, D_80150ED8
$32:
	.loc	2 296
	.noalias	$20,$19
	.noalias	$20,$sp
	addu	$25, $20, $16
	.noalias	$25,$19
	.noalias	$25,$sp
	lb	$14, 0($25)
	.alias	$25,$19
	.alias	$25,$sp
	beq	$14, 0, $33
	.loc	2 296
	.loc	2 297
 # 297	        empty=0;label=NAME(6,i);
	move	$21, $0
	.loc	2 297
	lw	$24, 16($19)
	lw	$15, 12($19)
	lhu	$25, 12($15)
	mul	$14, $25, 4
	addu	$15, $24, $14
	mul	$25, $16, 4
	addu	$24, $15, $25
	lw	$17, 0($24)
	.loc	2 298
 # 298	        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 80
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 298
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$24, $18, 16
	move	$18, $24
$33:
	.loc	2 296
 # 296	    for(i=3;i<9;i++)if(D_80150ED8[i]) {
	addu	$16, $16, 1
	sll	$14, $16, 16
	move	$16, $14
	sra	$15, $16, 16
	move	$16, $15
	blt	$16, 9, $32
	.alias	$20,$19
	.alias	$20,$sp
	.loc	2 300
 # 297	        empty=0;label=NAME(6,i);
 # 298	        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
 # 299	    }
 # 300	    if(empty==1) {
	seq	$2, $21, $30
	li	$21, 1
	beq	$2, 0, $34
	.loc	2 300
	.loc	2 301
 # 301	        label=NAME(15,0);
	lw	$25, 16($19)
	lw	$24, 12($19)
	lhu	$14, 30($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lw	$17, 0($24)
	.loc	2 302
 # 302	        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 80
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 302
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$24, $18, 16
	move	$18, $24
	b	$35
$34:
	.loc	2 303
 # 303	    } else all_empty=0;
	move	$23, $0
$35:
	.loc	2 304
 # 304	    y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$25, $18, 16
	move	$18, $25
	.loc	2 305
 # 305	    object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 306
 # 306	    label=NAME(34,1);
	lw	$15, 16($19)
	lw	$24, 12($19)
	lhu	$14, 68($24)
	mul	$25, $14, 4
	addu	$24, $15, $25
	lw	$17, 4($24)
	.loc	2 307
 # 307	    state_utility(sound_pitch_diff_halved(label,80),y,label);
	move	$4, $17
	li	$5, 80
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 308
 # 308	    y+=object_bytes_sum_global();object_create(12);empty=1;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$24, $18, 16
	move	$18, $24
	.loc	2 308
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 308
	.loc	2 309
 # 309	    for(i=3;i<6;i++)if(D_80150F40[i]) {
	li	$16, 3
	la	$20, D_80150F40
$36:
	.loc	2 309
	.noalias	$20,$19
	.noalias	$20,$sp
	addu	$14, $20, $16
	.noalias	$14,$19
	.noalias	$14,$sp
	lb	$15, 0($14)
	.alias	$14,$19
	.alias	$14,$sp
	beq	$15, 0, $37
	.loc	2 309
	.loc	2 310
 # 310	        empty=0;label=NAME(8,i);
	move	$21, $0
	.loc	2 310
	lw	$25, 16($19)
	lw	$24, 12($19)
	lhu	$14, 16($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	mul	$14, $16, 4
	addu	$25, $24, $14
	lw	$17, 0($25)
	.loc	2 311
 # 311	        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 80
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 311
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$25, $18, 16
	move	$18, $25
$37:
	.loc	2 309
 # 309	    for(i=3;i<6;i++)if(D_80150F40[i]) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 6, $36
	.alias	$20,$19
	.alias	$20,$sp
	.loc	2 313
 # 310	        empty=0;label=NAME(8,i);
 # 311	        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
 # 312	    }
 # 313	    if(empty==1) {
	seq	$2, $21, $30
	li	$21, 1
	beq	$2, 0, $38
	.loc	2 313
	.loc	2 314
 # 314	        label=NAME(15,0);
	lw	$14, 16($19)
	lw	$25, 12($19)
	lhu	$15, 30($25)
	mul	$24, $15, 4
	addu	$25, $14, $24
	lw	$17, 0($25)
	.loc	2 315
 # 315	        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 80
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 315
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$24, $18, 16
	move	$18, $24
	sra	$25, $18, 16
	move	$18, $25
	b	$39
$38:
	.loc	2 316
 # 316	    } else all_empty=0;
	move	$23, $0
$39:
	.loc	2 317
 # 317	    bottom=y+4;
	addu	$22, $18, 4
	sll	$15, $22, 16
	move	$22, $15
	sra	$14, $22, 16
	move	$22, $14
	.loc	2 318
 # 318	    object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 319
 # 319	    y=90;
	.loc	2 320
 # 320	    label=NAME(34,2);
	lw	$24, 16($19)
	lw	$25, 12($19)
	lhu	$15, 68($25)
	mul	$14, $15, 4
	addu	$25, $24, $14
	lw	$17, 8($25)
	.loc	2 321
 # 321	    state_utility(sound_pitch_diff_halved(label,240),y,label);
	move	$4, $17
	li	$5, 240
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	li	$5, 90
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 322
 # 322	    y+=object_bytes_sum_global();object_create(12);empty=1;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $2, 90
	sll	$24, $18, 16
	move	$18, $24
	sra	$14, $18, 16
	move	$18, $14
	.loc	2 322
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 322
	.loc	2 323
 # 323	    for(i=2;i<8;i++)if(D_80150EB8[i]) {
	li	$16, 2
	la	$20, D_80150EB8
$40:
	.loc	2 323
	.noalias	$20,$19
	.noalias	$20,$sp
	addu	$25, $20, $16
	.noalias	$25,$19
	.noalias	$25,$sp
	lb	$15, 0($25)
	.alias	$25,$19
	.alias	$25,$sp
	beq	$15, 0, $41
	.loc	2 323
	.loc	2 324
 # 324	        empty=0;label=NAME(4,i);
	move	$21, $0
	.loc	2 324
	lw	$24, 16($19)
	lw	$14, 12($19)
	lhu	$25, 8($14)
	mul	$15, $25, 4
	addu	$14, $24, $15
	mul	$25, $16, 4
	addu	$24, $14, $25
	lw	$17, 0($24)
	.loc	2 325
 # 325	        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 240
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 325
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$24, $18, 16
	move	$18, $24
$41:
	.loc	2 323
 # 323	    for(i=2;i<8;i++)if(D_80150EB8[i]) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, 8, $40
	.alias	$20,$19
	.alias	$20,$sp
	.loc	2 327
 # 324	        empty=0;label=NAME(4,i);
 # 325	        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
 # 326	    }
 # 327	    if(empty==1) {
	seq	$2, $21, $30
	li	$21, 1
	beq	$2, 0, $42
	.loc	2 327
	.loc	2 328
 # 328	        label=NAME(15,0);
	lw	$25, 16($19)
	lw	$24, 12($19)
	lhu	$15, 30($24)
	mul	$14, $15, 4
	addu	$24, $25, $14
	lw	$17, 0($24)
	.loc	2 329
 # 329	        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 240
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 329
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$24, $18, 16
	move	$18, $24
	b	$43
$42:
	.loc	2 330
 # 330	    } else all_empty=0;
	move	$23, $0
$43:
	.loc	2 331
 # 331	    y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$25, $18, 16
	move	$18, $25
	.loc	2 332
 # 332	    object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 333
 # 333	    label=NAME(34,3);
	lw	$14, 16($19)
	lw	$24, 12($19)
	lhu	$15, 68($24)
	mul	$25, $15, 4
	addu	$24, $14, $25
	lw	$17, 12($24)
	.loc	2 334
 # 334	    state_utility(sound_pitch_diff_halved(label,240),y,label);
	move	$4, $17
	li	$5, 240
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 335
 # 335	    y+=object_bytes_sum_global();object_create(12);empty=1;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$24, $18, 16
	move	$18, $24
	.loc	2 335
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 335
	.loc	2 336
 # 336	    for(i=1;i<5;i++)if(D_80150F00[i]) {
	sll	$16, $30, 16
	sra	$15, $16, 16
	move	$16, $15
	la	$20, D_80150F00
$44:
	.loc	2 336
	.noalias	$20,$19
	.noalias	$20,$sp
	addu	$14, $20, $16
	.noalias	$14,$19
	.noalias	$14,$sp
	lb	$25, 0($14)
	.alias	$14,$19
	.alias	$14,$sp
	beq	$25, 0, $45
	.loc	2 336
	.loc	2 337
 # 337	        empty=0;label=NAME(7,i);
	move	$21, $0
	.loc	2 337
	lw	$24, 16($19)
	lw	$15, 12($19)
	lhu	$14, 14($15)
	mul	$25, $14, 4
	addu	$15, $24, $25
	mul	$14, $16, 4
	addu	$24, $15, $14
	lw	$17, 0($24)
	.loc	2 338
 # 338	        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 240
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 338
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$24, $18, 16
	move	$18, $24
$45:
	.loc	2 336
 # 336	    for(i=1;i<5;i++)if(D_80150F00[i]) {
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$15, $16, 16
	move	$16, $15
	blt	$16, 5, $44
	.alias	$20,$19
	.alias	$20,$sp
	.loc	2 340
 # 337	        empty=0;label=NAME(7,i);
 # 338	        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
 # 339	    }
 # 340	    if(empty==1) {
	bne	$21, $30, $46
	.loc	2 340
	.loc	2 341
 # 341	        label=NAME(15,0);
	lw	$14, 16($19)
	lw	$24, 12($19)
	lhu	$25, 30($24)
	mul	$15, $25, 4
	addu	$24, $14, $15
	lw	$17, 0($24)
	.loc	2 342
 # 342	        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
	move	$4, $17
	li	$5, 240
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 342
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$24, $18, 16
	move	$18, $24
	b	$47
$46:
	.loc	2 343
 # 343	    } else all_empty=0;
	move	$23, $0
$47:
	.loc	2 344
 # 344	    if(all_empty) {
	beq	$23, 0, $48
	.loc	2 344
	.loc	2 345
 # 345	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$14, $18, 16
	move	$18, $14
	.loc	2 346
 # 346	        state_utility(sound_pitch_diff_halved(countdown_state.text[62],160),y,countdown_state.text[62]);
	lw	$15, 4($19)
	lw	$4, 248($15)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $18, 16
	sra	$25, $5, 16
	move	$5, $25
	lw	$14, 4($19)
	lw	$6, 248($14)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.alias	$19,$sp
	.loc	2 347
 # 347	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$24, $18, 16
	move	$18, $24
$48:
	.loc	2 349
 # 348	    }
 # 349	    if(y+4>=bottom)bottom=y+4;
	addu	$2, $18, 4
	blt	$2, $22, $49
	.loc	2 349
	sll	$22, $2, 16
	sra	$25, $22, 16
	move	$22, $25
$49:
	.loc	2 350
 # 350	    crowd_cheer_play(D_803B6530,12,86,308,bottom);
	lw	$4, D_803B6530
	li	$5, 12
	li	$6, 86
	li	$7, 308
	sw	$22, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 351
 # 351	}
$50:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 32
	j	$31
	.end	func_803AAF0C
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 508
 # 508	{
	.ent	func_803AB5FC 2
func_803AB5FC:
	.option	O3
	subu	$sp, 32
	sw	$31, 28($sp)
	.mask	0x80000000, -4
	.frame	$sp, 32, $31
	.loc	2 508
	la	$21, D_803BA800
	.loc	2 508
	.loc	2 510
 # 509	    Profile **saved_owner;
 # 510	    s16 saved_players,i,y,width=20;
	li	$19, 20
	.loc	2 511
 # 511	    if(!D_803BA800)return;
	.noalias	$21,$sp
	lw	$14, 0($21)
	beq	$14, 0, $58
	.alias	$21,$sp
	.loc	2 511
	.loc	2 512
 # 512	    func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	la	$18, D_8014A118
	la	$20, D_8014A108
	.loc	2 513
 # 513	    saved_owner=D_8014A118[0].owner;saved_players=D_8014A108;
	.noalias	$18,$sp
	lw	$16, 72($18)
	.loc	2 513
	.noalias	$20,$18
	.noalias	$20,$sp
	lh	$17, 0($20)
	.loc	2 514
 # 514	    D_8014A118[0].owner=D_803BA800;D_8014A108=1;
	.noalias	$21,$18
	.noalias	$21,$20
	.noalias	$21,$sp
	lw	$15, 0($21)
	sw	$15, 72($18)
	.loc	2 514
	li	$24, 1
	sh	$24, 0($20)
	.loc	2 515
 # 515	    net_state_validate();
	.livereg	0x0000000E,0x00000000
	jal	net_state_validate
	.alias	$21,$18
	.alias	$21,$20
	.alias	$21,$sp
	.loc	2 516
 # 516	    D_8014A118[0].owner=saved_owner;D_8014A108=saved_players;
	sw	$16, 72($18)
	.loc	2 516
	sh	$17, 0($20)
	.loc	2 517
 # 517	    object_create(12);dispatch_handler(1);y=90;
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.alias	$18,$20
	.alias	$18,$sp
	.alias	$20,$sp
	.loc	2 517
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 517
	li	$18, 90
	.loc	2 518
 # 518	    for(i=6;i<13;i++) {
	li	$16, 6
	la	$30, D_80150E30
	li	$23, 5
	li	$22, 4
	li	$21, 3
	li	$20, 2
$51:
	.loc	2 518
	.loc	2 519
 # 519	        if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)continue;
	beq	$16, 0, $54
	beq	$16, 1, $54
	beq	$16, $20, $54
	beq	$16, $21, $54
	beq	$16, $22, $54
	beq	$16, $23, $54
	.loc	2 519
	.loc	2 520
 # 520	        if(D_80150E30[i]) {
	.noalias	$30,$sp
	addu	$25, $30, $16
	.noalias	$25,$sp
	lb	$14, 0($25)
	.alias	$25,$sp
	beq	$14, 0, $54
	.loc	2 520
	.loc	2 521
 # 521	            width=(u32)object_manager_update(D_80110D70[i],-1)<(u32)width ? (u32)width : (u32)object_manager_update(D_80110D70[i],-1);
	.noalias	$17,$30
	.noalias	$17,$sp
	mul	$15, $16, 4
	la	$24, D_80110D70
	addu	$17, $15, $24
	lw	$4, 0($17)
	li	$5, -1
	.livereg	0x0C00400E,0x00000000
	jal	object_manager_update
	move	$3, $19
	bgeu	$2, $3, $52
	sll	$19, $3, 16
	sra	$25, $19, 16
	move	$19, $25
	b	$53
$52:
	lw	$4, 0($17)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$19, $2, 16
	sra	$14, $19, 16
	move	$19, $14
$53:
	.loc	2 522
 # 522	            state_utility(sound_pitch_diff_halved(D_80110D70[i],160),y,D_80110D70[i]);
	lw	$4, 0($17)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$24, $5, 16
	move	$5, $24
	lw	$6, 0($17)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.alias	$17,$30
	.alias	$17,$sp
	.loc	2 523
 # 523	            y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$14, $18, 16
	move	$18, $14
	.loc	2 518
 # 518	    for(i=6;i<13;i++) {
$54:
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 13, $51
	.alias	$30,$sp
	.loc	2 526
 # 526	    if(y==90) {
	bne	$18, 90, $57
	la	$16, countdown_state
	.loc	2 526
	.loc	2 527
 # 527	        width=(u32)object_manager_update(countdown_state.text[61],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[61],-1);
	.noalias	$16,$sp
	lw	$25, 4($16)
	lw	$4, 244($25)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $19
	bgeu	$2, $3, $55
	sll	$19, $3, 16
	sra	$14, $19, 16
	move	$19, $14
	b	$56
$55:
	lw	$15, 4($16)
	lw	$4, 244($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$19, $2, 16
	sra	$24, $19, 16
	move	$19, $24
$56:
	.loc	2 528
 # 528	        state_utility(sound_pitch_diff_halved(countdown_state.text[61],160),y,countdown_state.text[61]);
	lw	$25, 4($16)
	lw	$4, 244($25)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	lw	$24, 4($16)
	lw	$6, 244($24)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.alias	$16,$sp
	.loc	2 529
 # 529	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$14, $18, 16
	move	$18, $14
$57:
	.loc	2 531
 # 530	    }
 # 531	    crowd_cheer_play(D_803B6530,152-width/2,86,168+width/2,y+4);
	lw	$4, D_803B6530
	div	$2, $19, 2
	li	$15, 152
	subu	$5, $15, $2
	li	$6, 86
	addu	$7, $2, 168
	addu	$24, $18, 4
	sw	$24, 16($sp)
	.livereg	0x2F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 532
 # 532	}
$58:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 32
	j	$31
	.end	func_803AB5FC
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 353
 # 353	{
	.ent	func_803AB848 2
func_803AB848:
	.option	O3
	subu	$sp, 112
	sw	$31, 28($sp)
	.mask	0x80000000, -84
	.frame	$sp, 112, $31
	.loc	2 353
	la	$16, D_803BA800
	.loc	2 353
	.loc	2 356
 # 354	    s32 first;
 # 355	    Profile **saved_owner;
 # 356	    s16 saved_players,i,x,y,bottom,width=20;
	li	$22, 20
	.loc	2 359
 # 357	    s32 second;
 # 358	    u8 text[44];
 # 359	    if(!D_803BA800)return;
	.noalias	$16,$sp
	lw	$14, 0($16)
	beq	$14, 0, $89
	.alias	$16,$sp
	.loc	2 359
	.loc	2 360
 # 360	    func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	la	$19, D_8014A118
	la	$20, D_8014A108
	.loc	2 361
 # 361	    saved_owner=D_8014A118[0].owner;saved_players=D_8014A108;
	.noalias	$19,$sp
	lw	$17, 72($19)
	.loc	2 361
	.noalias	$20,$19
	.noalias	$20,$sp
	lh	$18, 0($20)
	.loc	2 362
 # 362	    D_8014A118[0].owner=D_803BA800;D_8014A108=1;
	.noalias	$16,$19
	.noalias	$16,$20
	.noalias	$16,$sp
	lw	$15, 0($16)
	sw	$15, 72($19)
	.loc	2 362
	li	$24, 1
	sh	$24, 0($20)
	.loc	2 363
 # 363	    net_state_validate();
	.livereg	0x0000000E,0x00000000
	jal	net_state_validate
	.alias	$16,$19
	.alias	$16,$20
	.alias	$16,$sp
	.loc	2 364
 # 364	    D_8014A118[0].owner=saved_owner;D_8014A108=saved_players;
	sw	$17, 72($19)
	.loc	2 364
	sh	$18, 0($20)
	.loc	2 365
 # 365	    first=0;second=0;
	move	$30, $0
	.loc	2 365
	move	$23, $0
	.loc	2 366
 # 366	    for(i=0;i<19;i++) {
	move	$16, $0
	li	$21, 2
	li	$19, 14
	li	$9, 1
	li	$8, 9
	li	$7, 8
	li	$6, 7
	li	$5, 6
	li	$3, 3
	.alias	$19,$20
	.alias	$19,$sp
	.alias	$20,$sp
$59:
	.loc	2 366
	.loc	2 367
 # 367	        if(i==0 || i==1 || i==2 || i==3 || i==14 || i==6 || i==7 || i==8 || i==9)continue;
	beq	$16, 0, $61
	beq	$16, $9, $61
	beq	$16, $21, $61
	beq	$16, $3, $61
	beq	$16, $19, $61
	beq	$16, $5, $61
	beq	$16, $6, $61
	beq	$16, $7, $61
	beq	$16, $8, $61
	.loc	2 367
	.loc	2 368
 # 368	        if(!D_80150DD8[i])continue; if(i<14)first=1;else second=1;
	lb	$25, D_80150DD8($16)
	beq	$25, 0, $61
	.loc	2 368
	.loc	2 368
	bge	$16, 14, $60
	.loc	2 368
	move	$30, $9
	b	$61
$60:
	.loc	2 368
	move	$23, $9
	.loc	2 366
 # 366	    for(i=0;i<19;i++) {
$61:
	addu	$16, $16, 1
	sll	$14, $16, 16
	move	$16, $14
	sra	$15, $16, 16
	move	$16, $15
	blt	$16, 19, $59
	.loc	2 370
 # 367	        if(i==0 || i==1 || i==2 || i==3 || i==14 || i==6 || i==7 || i==8 || i==9)continue;
 # 368	        if(!D_80150DD8[i])continue; if(i<14)first=1;else second=1;
 # 369	    }
 # 370	    for(i=1;i<4;i++)if(D_80150E88[i])second=1;
	sll	$16, $9, 16
	sra	$24, $16, 16
	move	$16, $24
	la	$2, D_80150E88
$62:
	.loc	2 370
	.noalias	$2,$sp
	addu	$25, $2, $16
	.noalias	$25,$sp
	lb	$14, 0($25)
	.alias	$25,$sp
	beq	$14, 0, $63
	.loc	2 370
	move	$23, $9
$63:
	.loc	2 370
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 4, $62
	.loc	2 371
 # 371	    object_create(12);dispatch_handler(1);
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.alias	$2,$sp
	.loc	2 371
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 372
 # 372	    y=90;x=first && second ? 80 : 160;bottom=90;
	li	$18, 90
	.loc	2 372
	beq	$30, 0, $64
	beq	$23, 0, $64
	li	$20, 80
	b	$65
$64:
	li	$20, 160
$65:
	.loc	2 372
	li	$25, 90
	sh	$25, 94($sp)
	.loc	2 373
 # 373	    for(i=0;i<19;i++) {
	move	$16, $0
$66:
	.loc	2 373
	.loc	2 374
 # 374	        if(i==14) {x=first && second ? 240 : 160;if(first && second)y=90;}
	bne	$16, $19, $69
	.loc	2 374
	.loc	2 374
	beq	$30, 0, $67
	beq	$23, 0, $67
	li	$20, 240
	b	$68
$67:
	li	$20, 160
$68:
	.loc	2 374
	beq	$30, 0, $69
	beq	$23, 0, $69
	.loc	2 374
	li	$18, 90
$69:
	.loc	2 375
 # 375	        if(i==0 || i==1 || i==2 || i==3 || i==14 || i==6 || i==7 || i==8 || i==9)continue;
	beq	$16, 0, $73
	beq	$16, 1, $73
	beq	$16, $21, $73
	beq	$16, 3, $73
	beq	$16, $19, $73
	beq	$16, 6, $73
	beq	$16, 7, $73
	beq	$16, 8, $73
	beq	$16, 9, $73
	.loc	2 375
	.loc	2 376
 # 376	        if(D_80150DD8[i]) {
	lb	$14, D_80150DD8($16)
	beq	$14, 0, $72
	.loc	2 376
	.loc	2 377
 # 377	            width=(u32)object_manager_update(D_8011A8D8[i],-1)<(u32)width ? (u32)width : (u32)object_manager_update(D_8011A8D8[i],-1);
	.noalias	$17,$sp
	mul	$15, $16, 4
	la	$24, D_8011A8D8
	addu	$17, $15, $24
	lw	$4, 0($17)
	li	$5, -1
	.livereg	0x0C00400E,0x00000000
	jal	object_manager_update
	move	$3, $22
	bgeu	$2, $3, $70
	sll	$22, $3, 16
	sra	$25, $22, 16
	move	$22, $25
	b	$71
$70:
	lw	$4, 0($17)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$22, $2, 16
	sra	$14, $22, 16
	move	$22, $14
$71:
	.loc	2 378
 # 378	            state_utility(sound_pitch_diff_halved(D_8011A8D8[i],x),y,D_8011A8D8[i]);
	lw	$4, 0($17)
	sll	$5, $20, 16
	sra	$15, $5, 16
	move	$5, $15
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $18, 16
	sra	$25, $5, 16
	move	$5, $25
	lw	$6, 0($17)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.alias	$17,$sp
	.loc	2 379
 # 379	            y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$15, $18, 16
	move	$18, $15
$72:
	.loc	2 381
 # 380	        }
 # 381	        if(y>=bottom)bottom=y;
	lh	$24, 94($sp)
	blt	$18, $24, $73
	.loc	2 381
	sh	$18, 94($sp)
	.loc	2 373
 # 373	    for(i=0;i<19;i++) {
$73:
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, 19, $66
	.loc	2 383
 # 383	    for(i=1;i<4;i++)if(D_80150E88[i]) {
	li	$16, 1
	la	$21, D_803B8BDC
	la	$19, countdown_state
	addu	$17, $sp, 44
$74:
	.loc	2 383
	lb	$15, D_80150E88($16)
	beq	$15, 0, $77
	.loc	2 383
	.loc	2 384
 # 384	        fcvt_wrapper(text,D_803B8BDC,NAME(44,i),countdown_state.text[42]);
	move	$4, $17
	move	$5, $21
	.noalias	$19,$sp
	lw	$24, 16($19)
	lw	$25, 12($19)
	lhu	$14, 88($25)
	mul	$15, $14, 4
	addu	$25, $24, $15
	mul	$14, $16, 4
	addu	$24, $25, $14
	lw	$6, 0($24)
	lw	$15, 4($19)
	lw	$7, 168($15)
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 385
 # 385	        width=(u32)object_manager_update(text,-1)<(u32)width ? (u32)width : (u32)object_manager_update(text,-1);
	move	$4, $17
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $22
	bgeu	$2, $3, $75
	sll	$22, $3, 16
	sra	$25, $22, 16
	move	$22, $25
	b	$76
$75:
	move	$4, $17
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$22, $2, 16
	sra	$14, $22, 16
	move	$22, $14
$76:
	.loc	2 386
 # 386	        state_utility(sound_pitch_diff_halved(text,x),y,text);y+=object_bytes_sum_global();
	move	$4, $17
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $18, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 386
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$24, $18, 16
	move	$18, $24
$77:
	.loc	2 383
 # 383	    for(i=1;i<4;i++)if(D_80150E88[i]) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$25, $16, 16
	move	$16, $25
	blt	$16, 4, $74
	.loc	2 388
 # 384	        fcvt_wrapper(text,D_803B8BDC,NAME(44,i),countdown_state.text[42]);
 # 385	        width=(u32)object_manager_update(text,-1)<(u32)width ? (u32)width : (u32)object_manager_update(text,-1);
 # 386	        state_utility(sound_pitch_diff_halved(text,x),y,text);y+=object_bytes_sum_global();
 # 387	    }
 # 388	    if(!first && !second) {
	bne	$30, 0, $86
	bne	$23, 0, $86
	.loc	2 388
	.loc	2 389
 # 389	        width=(u32)object_manager_update(countdown_state.text[57],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[57],-1);
	lw	$14, 4($19)
	lw	$4, 228($14)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $22
	bgeu	$2, $3, $78
	sll	$22, $3, 16
	sra	$24, $22, 16
	move	$22, $24
	b	$79
$78:
	lw	$15, 4($19)
	lw	$4, 228($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$22, $2, 16
	sra	$25, $22, 16
	move	$22, $25
$79:
	.loc	2 390
 # 390	        state_utility(sound_pitch_diff_halved(countdown_state.text[57],160),y,countdown_state.text[57]);y+=object_bytes_sum_global();
	lw	$14, 4($19)
	lw	$4, 228($14)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	lw	$25, 4($19)
	lw	$6, 228($25)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 390
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$24, $18, 16
	move	$18, $24
	.loc	2 391
 # 391	        width=(u32)object_manager_update(countdown_state.text[58],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[58],-1);
	lw	$15, 4($19)
	lw	$4, 232($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $22
	bgeu	$2, $3, $80
	sll	$22, $3, 16
	sra	$25, $22, 16
	move	$22, $25
	b	$81
$80:
	lw	$14, 4($19)
	lw	$4, 232($14)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$22, $2, 16
	sra	$24, $22, 16
	move	$22, $24
$81:
	.loc	2 392
 # 392	        state_utility(sound_pitch_diff_halved(countdown_state.text[58],160),y,countdown_state.text[58]);y+=object_bytes_sum_global();
	lw	$15, 4($19)
	lw	$4, 232($15)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	lw	$24, 4($19)
	lw	$6, 232($24)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 392
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$25, $18, 16
	move	$18, $25
	.loc	2 393
 # 393	        width=(u32)object_manager_update(countdown_state.text[59],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[59],-1);
	lw	$14, 4($19)
	lw	$4, 236($14)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $22
	bgeu	$2, $3, $82
	sll	$22, $3, 16
	sra	$24, $22, 16
	move	$22, $24
	b	$83
$82:
	lw	$15, 4($19)
	lw	$4, 236($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$22, $2, 16
	sra	$25, $22, 16
	move	$22, $25
$83:
	.loc	2 394
 # 394	        state_utility(sound_pitch_diff_halved(countdown_state.text[59],160),y,countdown_state.text[59]);y+=object_bytes_sum_global();
	lw	$14, 4($19)
	lw	$4, 236($14)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	lw	$25, 4($19)
	lw	$6, 236($25)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 394
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$14, $18, 16
	move	$18, $14
	sra	$24, $18, 16
	move	$18, $24
	.loc	2 395
 # 395	        width=(u32)object_manager_update(countdown_state.text[60],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[60],-1);
	lw	$15, 4($19)
	lw	$4, 240($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $22
	bgeu	$2, $3, $84
	sll	$22, $3, 16
	sra	$25, $22, 16
	move	$22, $25
	b	$85
$84:
	lw	$14, 4($19)
	lw	$4, 240($14)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$22, $2, 16
	sra	$24, $22, 16
	move	$22, $24
$85:
	.loc	2 396
 # 396	        state_utility(sound_pitch_diff_halved(countdown_state.text[60],160),y,countdown_state.text[60]);y+=object_bytes_sum_global();
	lw	$15, 4($19)
	lw	$4, 240($15)
	li	$5, 160
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	lw	$24, 4($19)
	lw	$6, 240($24)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.alias	$19,$sp
	.loc	2 396
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$25, $18, 16
	move	$18, $25
	b	$87
$86:
	.loc	2 397
 # 397	    } else if(first && second)width+=160;
	beq	$30, 0, $87
	beq	$23, 0, $87
	.loc	2 397
	addu	$22, $22, 160
	sll	$14, $22, 16
	move	$22, $14
	sra	$24, $22, 16
	move	$22, $24
$87:
	.loc	2 398
 # 398	    if(y>=bottom)bottom=y;
	lh	$15, 94($sp)
	blt	$18, $15, $88
	.loc	2 398
	sh	$18, 94($sp)
$88:
	.loc	2 399
 # 399	    crowd_cheer_play(D_803B6530,152-width/2,86,168+width/2,bottom+4);
	lw	$4, D_803B6530
	div	$2, $22, 2
	li	$25, 152
	subu	$5, $25, $2
	li	$6, 86
	addu	$7, $2, 168
	lh	$14, 94($sp)
	addu	$24, $14, 4
	sw	$24, 16($sp)
	.livereg	0x2F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 400
 # 400	}
$89:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 112
	j	$31
	.end	func_803AB848
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 455
 # 455	{
	.ent	func_803ABEC8 2
func_803ABEC8:
	.option	O3
	subu	$sp, 152
	sw	$31, 28($sp)
	.mask	0x80000000, -124
	.frame	$sp, 152, $31
	.loc	2 455
	la	$18, D_803BA800
	.loc	2 455
	.loc	2 458
 # 456	    u32 divisor;
 # 457	    s16 x,y;
 # 458	    u32 distance=0;
	move	$21, $0
	.loc	2 459
 # 459	    u32 total=0;
	sw	$0, 136($sp)
	.loc	2 460
 # 460	    u16 found=0,race_a=0,race_b=0,track_a=0,track_b=0;
	sh	$0, 134($sp)
	.loc	2 460
	sh	$0, 132($sp)
	.loc	2 460
	sh	$0, 130($sp)
	.loc	2 460
	sh	$0, 128($sp)
	.loc	2 460
	sh	$0, 126($sp)
	.loc	2 463
 # 461	    s16 i,row;
 # 462	    u8 text[48],*label,*data,*record,*track;
 # 463	    if(!D_803BA800)return;
	.noalias	$18,$sp
	lw	$14, 0($18)
	beq	$14, 0, $108
	.alias	$18,$sp
	.loc	2 463
	.loc	2 464
 # 464	    func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 465
 # 465	    for(i=0;i<12;i++) {
	move	$16, $0
	li	$19, 96
$90:
	.loc	2 465
	.loc	2 466
 # 466	        data=PROFILE_DATA+140+i*96;
	.noalias	$18,$sp
	lw	$15, 0($18)
	lw	$24, 0($15)
	lw	$25, 44($24)
	lw	$14, 0($25)
	mul	$15, $16, $19
	addu	$17, $14, $15
	addu	$17, $17, 140
	.loc	2 467
 # 467	        distance+=U32_AT(data,88);
	lw	$24, 88($17)
	addu	$21, $21, $24
	.loc	2 468
 # 468	        if(i<6) {
	bge	$16, 6, $91
	.loc	2 468
	.loc	2 469
 # 469	            race_a+=func_800B78A4(U16_AT(data,92)&255,16);
	lhu	$4, 92($17)
	and	$25, $4, 255
	move	$4, $25
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	lhu	$14, 132($sp)
	addu	$15, $14, $2
	sh	$15, 132($sp)
	.loc	2 470
 # 470	            race_b+=func_800B78A4(U16_AT(data,92)&65280,16);
	lhu	$4, 92($17)
	and	$24, $4, 65280
	move	$4, $24
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	lhu	$25, 130($sp)
	addu	$14, $25, $2
	sh	$14, 130($sp)
$91:
	.loc	2 465
 # 465	    for(i=0;i<12;i++) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 12, $90
	.loc	2 473
 # 473	    for(i=0;i<4;i++) {
	move	$16, $0
$92:
	.loc	2 473
	.loc	2 474
 # 474	        track=PROFILE_DATA+1292+i*64;
	lw	$25, 0($18)
	lw	$14, 0($25)
	lw	$15, 44($14)
	lw	$24, 0($15)
	mul	$25, $16, 64
	addu	$17, $24, $25
	addu	$17, $17, 1292
	.loc	2 475
 # 475	        total+=U32_AT(track,12);
	lw	$14, 136($sp)
	lw	$15, 12($17)
	addu	$24, $14, $15
	sw	$24, 136($sp)
	.loc	2 476
 # 476	        track_a+=func_800B78A4(U16_AT(track,60)&255,16);
	lhu	$4, 60($17)
	and	$25, $4, 255
	move	$4, $25
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	lhu	$14, 128($sp)
	addu	$15, $14, $2
	sh	$15, 128($sp)
	.loc	2 477
 # 477	        track_b+=func_800B78A4(U16_AT(track,60)&65280,16);
	lhu	$4, 60($17)
	and	$24, $4, 65280
	move	$4, $24
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	lhu	$25, 126($sp)
	addu	$14, $25, $2
	sh	$14, 126($sp)
	.loc	2 473
 # 473	    for(i=0;i<4;i++) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 4, $92
	.alias	$18,$sp
	.loc	2 479
 # 474	        track=PROFILE_DATA+1292+i*64;
 # 475	        total+=U32_AT(track,12);
 # 476	        track_a+=func_800B78A4(U16_AT(track,60)&255,16);
 # 477	        track_b+=func_800B78A4(U16_AT(track,60)&65280,16);
 # 478	    }
 # 479	    for(i=0;i<8;i++) {
	move	$16, $0
	lw	$25, D_803BA800
	lw	$14, 0($25)
	lw	$15, 44($14)
	lw	$4, 0($15)
	lhu	$5, 134($sp)
	li	$3, 12
$93:
	.loc	2 479
	.loc	2 480
 # 480	        record=PROFILE_DATA+1548+i*12;
	mul	$24, $16, $3
	addu	$2, $4, $24
	addu	$2, $2, 1548
	.loc	2 481
 # 481	        found+=U16_AT(record,8);
	lhu	$25, 8($2)
	addu	$5, $5, $25
	and	$14, $5, 65535
	move	$5, $14
	.loc	2 479
 # 479	    for(i=0;i<8;i++) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 8, $93
	.loc	2 483
 # 480	        record=PROFILE_DATA+1548+i*12;
 # 481	        found+=U16_AT(record,8);
 # 482	    }
 # 483	    object_create(11);dispatch_handler(1);
	li	$4, 11
	sh	$5, 134($sp)
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 483
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 484
 # 484	    divisor=10;
	.loc	2 485
 # 485	    for(row=0;row<7;row++) {
	move	$18, $0
	la	$30, countdown_state
	lh	$23, 146($sp)
	lh	$20, 144($sp)
	li	$19, 10
	addu	$17, $sp, 72
$94:
	.loc	2 485
	.loc	2 486
 # 486	        if(row==0) {x=180;y=90;}
	bne	$18, 0, $95
	.loc	2 486
	.loc	2 486
	li	$23, 180
	.loc	2 486
	li	$20, 90
$95:
	.loc	2 487
 # 487	        label=NAME(33,row);
	.noalias	$30,$sp
	lw	$2, 16($30)
	lw	$3, 12($30)
	lhu	$25, 66($3)
	mul	$14, $25, 4
	addu	$15, $2, $14
	mul	$24, $18, 4
	addu	$25, $15, $24
	lw	$16, 0($25)
	.loc	2 488
 # 488	        if(row==0 && D_80146111)label=NAME(28,7);
	addu	$5, $23, -4
	sll	$14, $5, 16
	move	$5, $14
	sra	$15, $5, 16
	move	$5, $15
	addu	$22, $23, 4
	sll	$24, $22, 16
	move	$22, $24
	sra	$25, $22, 16
	move	$22, $25
	bne	$18, 0, $96
	lb	$14, D_80146111
	beq	$14, 0, $96
	.loc	2 488
	lhu	$15, 56($3)
	mul	$24, $15, 4
	addu	$25, $2, $24
	lw	$16, 28($25)
$96:
	.loc	2 489
 # 489	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $20, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 490
 # 490	        switch(row) {
	bgeu	$18, 7, $107
	sll	$24, $18, 2
	lw	$24, $97($24)
	j	$24
	.rdata	
$97:
	.word	$99
	.word	$101
	.word	$102
	.word	$103
	.word	$104
	.word	$105
	.word	$106
	.text	
$99:
	.loc	2 492
 # 491	        case 0:
 # 492	            if(D_80146111) {
	lb	$25, D_80146111
	beq	$25, 0, $100
	.loc	2 492
	.loc	2 493
 # 493	                fcvt_wrapper(text,D_803B8B9C,(distance*divisor/6)/divisor,(distance*divisor/6)%divisor);
	move	$4, $17
	la	$5, D_803B8B9C
	mul	$2, $21, $19
	divu	$14, $2, 6
	move	$2, $14
	divu	$6, $2, $19
	remu	$7, $2, $19
	.livereg	0x2F00000E,0x00000000
	jal	fcvt_wrapper
	b	$107
$100:
	.loc	2 494
 # 494	            } else fcvt_wrapper(text,D_803B8BA8,distance/divisor,distance%divisor);
	move	$4, $17
	la	$5, D_803B8BA8
	divu	$6, $21, $19
	remu	$7, $21, $19
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 495
 # 495	            break;
	b	$107
$101:
	.loc	2 496
 # 496	        case 1:fcvt_wrapper(text,D_803B8BB4,total);break;
	move	$4, $17
	la	$5, D_803B8BB4
	lw	$6, 136($sp)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 496
	b	$107
$102:
	.loc	2 497
 # 497	        case 2:fcvt_wrapper(text,D_803B8BB8,found);break;
	move	$4, $17
	la	$5, D_803B8BB8
	lhu	$6, 134($sp)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 497
	b	$107
$103:
	.loc	2 498
 # 498	        case 3:fcvt_wrapper(text,D_803B8BBC,race_a,48);break;
	move	$4, $17
	la	$5, D_803B8BBC
	lhu	$6, 132($sp)
	li	$7, 48
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 498
	b	$107
$104:
	.loc	2 499
 # 499	        case 4:fcvt_wrapper(text,D_803B8BC4,race_b,48);break;
	move	$4, $17
	la	$5, D_803B8BC4
	lhu	$6, 130($sp)
	li	$7, 48
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 499
	b	$107
$105:
	.loc	2 500
 # 500	        case 5:fcvt_wrapper(text,D_803B8BCC,track_a,32);break;
	move	$4, $17
	la	$5, D_803B8BCC
	lhu	$6, 128($sp)
	li	$7, 32
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 500
	b	$107
$106:
	.loc	2 501
 # 501	        case 6:fcvt_wrapper(text,D_803B8BD4,track_b,32);break;
	move	$4, $17
	la	$5, D_803B8BD4
	lhu	$6, 126($sp)
	li	$7, 32
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 501
$107:
	.loc	2 503
 # 502	        }
 # 503	        state_utility(x+4,y,text);y+=object_bytes_sum_global();
	sll	$4, $22, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 503
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	sll	$25, $20, 16
	move	$20, $25
	sra	$14, $20, 16
	move	$20, $14
	.loc	2 485
 # 485	    for(row=0;row<7;row++) {
	addu	$18, $18, 1
	sll	$15, $18, 16
	move	$18, $15
	sra	$24, $18, 16
	move	$18, $24
	blt	$18, 7, $94
	.alias	$30,$sp
	sh	$23, 146($sp)
	sh	$20, 144($sp)
	.loc	2 505
 # 505	    crowd_cheer_play(D_803B6530,12,86,308,94+object_bytes_sum_global()*7);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 12
	li	$6, 86
	li	$7, 308
	mul	$25, $2, 7
	addu	$14, $25, 94
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 506
 # 506	}
$108:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 152
	j	$31
	.end	func_803ABEC8
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 106
 # 106	{
	.ent	func_803AC330 2
func_803AC330:
	.option	O3
	subu	$sp, 32
	sw	$31, 28($sp)
	.mask	0x80000000, -4
	.frame	$sp, 32, $31
	.loc	2 106
	.loc	2 106
	.loc	2 107
 # 107	    if(time==0.0f)func_800BE6A4(destination,D_803B8BF4);
	li.s	$f4, 0.0
	c.eq.s	$f12, $f4
	bc1f	$109
	.loc	2 107
	la	$5, D_803B8BF4
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$110
$109:
	.loc	2 108
 # 108	    else {
	.loc	2 109
 # 109	        D_803BA90C=time;
	s.s	$f12, D_803BA90C
	.loc	2 110
 # 110	        fcvt_wrapper(destination,D_803B8C00,(u32)(time/60.0f)%100,(u32)time%60,(u32)(time*1000.0f)%1000);
	la	$5, D_803B8C00
	li.s	$f6, 60.0
	div.s	$f8, $f12, $f6
	cfc1	$14, $31
	li	$6, 1
	ctc1	$6, $31
	cvt.w.s	$f10, $f8
	cfc1	$6, $31
	.set	 noat
	and	$1, $6, 4
	and	$6, $6, 120
	.set	 at
	beq	$6, 0, $112
	li.s	$f10, 2147483648.0
	sub.s	$f10, $f8, $f10
	li	$6, 1
	ctc1	$6, $31
	cvt.w.s	$f10, $f10
	cfc1	$6, $31
	.set	 noat
	and	$1, $6, 4
	and	$6, $6, 120
	.set	 at
	bne	$6, 0, $113
	.set	 noat
	mfc1	$6, $f10
	li	$1, -2147483648
	or	$6, $6, $1
	.set	 at
	b	$111
$113:
	li	$6, -1
	b	$111
$112:
	mfc1	$6, $f10
	blt	$6, 0, $113
$111:
	ctc1	$14, $31
	remu	$15, $6, 100
	move	$6, $15
	cfc1	$24, $31
	li	$7, 1
	ctc1	$7, $31
	cvt.w.s	$f4, $f12
	cfc1	$7, $31
	.set	 noat
	and	$1, $7, 4
	and	$7, $7, 120
	.set	 at
	beq	$7, 0, $115
	li.s	$f4, 2147483648.0
	sub.s	$f4, $f12, $f4
	li	$7, 1
	ctc1	$7, $31
	cvt.w.s	$f4, $f4
	cfc1	$7, $31
	.set	 noat
	and	$1, $7, 4
	and	$7, $7, 120
	.set	 at
	bne	$7, 0, $116
	.set	 noat
	mfc1	$7, $f4
	li	$1, -2147483648
	or	$7, $7, $1
	.set	 at
	b	$114
$116:
	li	$7, -1
	b	$114
$115:
	mfc1	$7, $f4
	blt	$7, 0, $116
$114:
	ctc1	$24, $31
	remu	$25, $7, 60
	move	$7, $25
	li.s	$f6, 1000.0
	mul.s	$f8, $f12, $f6
	cfc1	$14, $31
	li	$15, 1
	ctc1	$15, $31
	cvt.w.s	$f10, $f8
	cfc1	$15, $31
	.set	 noat
	and	$1, $15, 4
	and	$15, $15, 120
	.set	 at
	beq	$15, 0, $118
	li.s	$f10, 2147483648.0
	sub.s	$f10, $f8, $f10
	li	$15, 1
	ctc1	$15, $31
	cvt.w.s	$f10, $f10
	cfc1	$15, $31
	.set	 noat
	and	$1, $15, 4
	and	$15, $15, 120
	.set	 at
	bne	$15, 0, $119
	.set	 noat
	mfc1	$15, $f10
	li	$1, -2147483648
	or	$15, $15, $1
	.set	 at
	b	$117
$119:
	li	$15, -1
	b	$117
$118:
	mfc1	$15, $f10
	blt	$15, 0, $119
$117:
	ctc1	$14, $31
	remu	$24, $15, 1000
	sw	$24, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 112
 # 111	    }
 # 112	}
$110:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 32
	j	$31
	.end	func_803AC330
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 611
 # 611	{
	.ent	func_803AC54C 2
func_803AC54C:
	.option	O3
	subu	$sp, 112
	sw	$31, 28($sp)
	.mask	0x80000000, -84
	.frame	$sp, 112, $31
	.loc	2 611
	.loc	2 611
	.loc	2 617
 # 612	    u8 *data;
 # 613	    s32 x,y;
 # 614	    u8 *label;
 # 615	    s32 i;
 # 616	    u8 text[40];
 # 617	    if(D_803BA800)data=PROFILE_DATA+1668+D_803BA860*28;
	lw	$2, D_803BA800
	beq	$2, 0, $120
	.loc	2 617
	lw	$14, 0($2)
	lw	$15, 44($14)
	lw	$24, 0($15)
	lb	$25, D_803BA860
	mul	$14, $25, 28
	addu	$18, $24, $14
	addu	$18, $18, 1668
	b	$121
$120:
	.loc	2 618
 # 618	    else data=D_80151618+D_803BA860*28;
	lb	$15, D_803BA860
	mul	$25, $15, 28
	la	$24, D_80151618
	addu	$18, $25, $24
$121:
	.loc	2 619
 # 619	    object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 619
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 620
 # 620	    for(i=0;i<8;i++) {
	move	$19, $0
	move	$21, $0
	lw	$30, 104($sp)
	lw	$22, 100($sp)
	addu	$17, $sp, 52
$122:
	.loc	2 620
	.loc	2 621
 # 621	        if(i==0) {x=180;y=90;}
	bne	$19, 0, $123
	.loc	2 621
	.loc	2 621
	li	$30, 180
	.loc	2 621
	li	$22, 90
$123:
	la	$2, countdown_state
	.loc	2 622
 # 622	        label=NAME(32,i);
	.noalias	$2,$sp
	lw	$14, 16($2)
	lw	$15, 12($2)
	lhu	$25, 64($15)
	mul	$24, $25, 4
	addu	$15, $14, $24
	addu	$25, $15, $21
	lw	$16, 0($25)
	.loc	2 623
 # 623	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	addu	$5, $30, -4
	sll	$14, $5, 16
	move	$5, $14
	sra	$24, $5, 16
	move	$5, $24
	addu	$23, $30, 4
	sll	$15, $23, 16
	move	$23, $15
	sra	$25, $23, 16
	move	$23, $25
	.livereg	0x0C00010E,0x00000000
	jal	sound_pitch_diff_calc
	.alias	$2,$sp
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$20, $22, 16
	sra	$24, $20, 16
	move	$20, $24
	sll	$5, $20, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $16
	.livereg	0x0E00080E,0x00000000
	jal	state_utility
	.loc	2 624
 # 624	        switch(i) {
	bgeu	$19, 9, $135
	sll	$25, $19, 2
	lw	$25, $124($25)
	j	$25
	.rdata	
$124:
	.word	$126
	.word	$127
	.word	$128
	.word	$129
	.word	$130
	.word	$131
	.word	$132
	.word	$133
	.word	$134
	.text	
$126:
	.loc	2 625
 # 625	        case 0:fcvt_wrapper(text,D_803B8B7C,U16_AT(data,4));break;
	move	$4, $17
	la	$5, D_803B8B7C
	lhu	$6, 4($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 625
	b	$135
$127:
	.loc	2 626
 # 626	        case 1:fcvt_wrapper(text,D_803B8B80,U16_AT(data,6));break;
	move	$4, $17
	la	$5, D_803B8B80
	lhu	$6, 6($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 626
	b	$135
$128:
	.loc	2 627
 # 627	        case 2:fcvt_wrapper(text,D_803B8B84,U16_AT(data,8));break;
	move	$4, $17
	la	$5, D_803B8B84
	lhu	$6, 8($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 627
	b	$135
$129:
	.loc	2 628
 # 628	        case 3:fcvt_wrapper(text,D_803B8B88,U16_AT(data,10));break;
	move	$4, $17
	la	$5, D_803B8B88
	lhu	$6, 10($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 628
	b	$135
$130:
	.loc	2 629
 # 629	        case 4:fcvt_wrapper(text,D_803B8B8C,U16_AT(data,12));break;
	move	$4, $17
	la	$5, D_803B8B8C
	lhu	$6, 12($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 629
	b	$135
$131:
	.loc	2 630
 # 630	        case 5:fcvt_wrapper(text,D_803B8B90,U16_AT(data,14));break;
	move	$4, $17
	la	$5, D_803B8B90
	lhu	$6, 14($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 630
	b	$135
$132:
	.loc	2 631
 # 631	        case 6:fcvt_wrapper(text,D_803B8B94,U16_AT(data,16));break;
	move	$4, $17
	la	$5, D_803B8B94
	lhu	$6, 16($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 631
	b	$135
$133:
	.loc	2 632
 # 632	        case 7:func_803AC330(text,F32_AT(data,20),0);break;
	move	$4, $17
	l.s	$f12, 20($18)
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 632
	b	$135
$134:
	.loc	2 633
 # 633	        case 8:fcvt_wrapper(text,D_803B8B98,U32_AT(data,24));break;
	move	$4, $17
	la	$5, D_803B8B98
	lw	$6, 24($18)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 633
$135:
	.loc	2 635
 # 634	        }
 # 635	        if(i==7)object_create(2);
	bne	$19, 7, $136
	.loc	2 635
	li	$4, 2
	.livereg	0x0800000E,0x00000000
	jal	object_create
$136:
	.loc	2 636
 # 636	        state_utility(x+4,y,text);
	sll	$4, $23, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 637
 # 637	        if(i==7)object_create(11);
	bne	$19, 7, $137
	.loc	2 637
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
$137:
	.loc	2 638
 # 638	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$22, $22, $2
	.loc	2 620
 # 620	    for(i=0;i<8;i++) {
	addu	$19, $19, 1
	addu	$21, $21, 4
	bne	$19, 8, $122
	sw	$30, 104($sp)
	.loc	2 640
 # 640	    crowd_cheer_play(D_803B6530,50,86,270,y+4);
	lw	$4, D_803B6530
	li	$5, 50
	li	$6, 86
	li	$7, 270
	addu	$15, $22, 4
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 641
 # 641	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 112
	j	$31
	.end	func_803AC54C
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 643
 # 642	void func_803AC7F4(void)
 # 643	{
	.ent	func_803AC7F4 2
func_803AC7F4:
	.option	O3
	subu	$sp, 104
	sw	$31, 28($sp)
	.mask	0x80000000, -76
	.frame	$sp, 104, $31
	.loc	2 643
	.loc	2 643
	.loc	2 649
 # 644	    u8 *data;
 # 645	    s32 x,y;
 # 646	    u8 *label;
 # 647	    s32 i;
 # 648	    u8 text[40];
 # 649	    if(D_803BA800)data=PROFILE_DATA+1068+D_803BA858*24;
	lw	$2, D_803BA800
	beq	$2, 0, $138
	.loc	2 649
	lw	$14, 0($2)
	lw	$15, 44($14)
	lw	$24, 0($15)
	lb	$25, D_803BA858
	mul	$14, $25, 24
	addu	$19, $24, $14
	addu	$19, $19, 1068
	b	$139
$138:
	.loc	2 650
 # 650	    else data=D_801515F8+D_803BA858*24-576;
	lb	$15, D_803BA858
	mul	$25, $15, 24
	addu	$24, $25, -576
	la	$14, D_801515F8
	addu	$19, $24, $14
$139:
	.loc	2 651
 # 651	    object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 651
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 652
 # 652	    for(i=0;i<4;i++) {
	move	$17, $0
	la	$30, countdown_state
	lw	$23, 96($sp)
	lw	$21, 92($sp)
	addu	$18, $sp, 44
	li	$2, 4
$140:
	.loc	2 652
	.loc	2 653
 # 653	        if(i==0) {x=190;y=90;}
	bne	$17, 0, $141
	.loc	2 653
	.loc	2 653
	li	$23, 190
	.loc	2 653
	li	$21, 90
	b	$142
$141:
	.loc	2 654
 # 654	        else if(i==4)continue;
	beq	$17, $2, $157
	.loc	2 654
$142:
	.loc	2 655
 # 655	        label=NAME(31,i);
	.noalias	$30,$sp
	lw	$15, 16($30)
	lw	$25, 12($30)
	lhu	$24, 62($25)
	mul	$14, $24, 4
	addu	$25, $15, $14
	mul	$24, $17, 4
	addu	$15, $25, $24
	lw	$16, 0($15)
	.loc	2 656
 # 656	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	addu	$5, $23, -4
	sll	$14, $5, 16
	move	$5, $14
	sra	$25, $5, 16
	move	$5, $25
	addu	$22, $23, 4
	sll	$24, $22, 16
	move	$22, $24
	sra	$15, $22, 16
	move	$22, $15
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$20, $21, 16
	sra	$25, $20, 16
	move	$20, $25
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $16
	.livereg	0x0E00080E,0x00000000
	jal	state_utility
	.loc	2 657
 # 657	        switch(i) {
	bgeu	$17, 7, $152
	sll	$15, $17, 2
	lw	$15, $143($15)
	j	$15
	.rdata	
$143:
	.word	$145
	.word	$146
	.word	$147
	.word	$148
	.word	$149
	.word	$150
	.word	$151
	.text	
$145:
	.loc	2 658
 # 658	        case 0:func_803AC330(text,F32_AT(data,4),0);break;
	move	$4, $18
	l.s	$f12, 4($19)
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 658
	b	$152
$146:
	.loc	2 659
 # 659	        case 1:func_803AC330(text,F32_AT(data,8),0);break;
	move	$4, $18
	l.s	$f12, 8($19)
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 659
	b	$152
$147:
	.loc	2 660
 # 660	        case 2:fcvt_wrapper(text,D_803B8B68,U16_AT(data,12));break;
	move	$4, $18
	la	$5, D_803B8B68
	lhu	$6, 12($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 660
	b	$152
$148:
	.loc	2 661
 # 661	        case 3:fcvt_wrapper(text,D_803B8B6C,U16_AT(data,14));break;
	move	$4, $18
	la	$5, D_803B8B6C
	lhu	$6, 14($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 661
	b	$152
$149:
	.loc	2 662
 # 662	        case 4:fcvt_wrapper(text,D_803B8B70,U16_AT(data,16));break;
	move	$4, $18
	la	$5, D_803B8B70
	lhu	$6, 16($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 662
	b	$152
$150:
	.loc	2 663
 # 663	        case 5:fcvt_wrapper(text,D_803B8B74,U16_AT(data,18));break;
	move	$4, $18
	la	$5, D_803B8B74
	lhu	$6, 18($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 663
	b	$152
$151:
	.loc	2 664
 # 664	        case 6:fcvt_wrapper(text,D_803B8B78,U16_AT(data,20));break;
	move	$4, $18
	la	$5, D_803B8B78
	lhu	$6, 20($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 664
$152:
	.loc	2 666
 # 665	        }
 # 666	        if(i==0 || i==1)object_create(2);
	beq	$17, 0, $153
	bne	$17, 1, $154
$153:
	.loc	2 666
	li	$4, 2
	.livereg	0x0800000E,0x00000000
	jal	object_create
$154:
	.loc	2 667
 # 667	        state_utility(x+4,y,text);
	sll	$4, $22, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $20, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 668
 # 668	        if(i==0 || i==1)object_create(11);
	beq	$17, 0, $155
	bne	$17, 1, $156
$155:
	.loc	2 668
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
$156:
	.loc	2 669
 # 669	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$21, $21, $2
	.loc	2 652
 # 652	    for(i=0;i<4;i++) {
	li	$2, 4
$157:
	addu	$17, $17, 1
	bne	$17, $2, $140
	.alias	$30,$sp
	sw	$23, 96($sp)
	sw	$21, 92($sp)
	.loc	2 671
 # 671	    crowd_cheer_play(D_803B6530,40,86,280,94+object_bytes_sum_global()*4);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 40
	li	$6, 86
	li	$7, 280
	mul	$24, $2, 4
	addu	$15, $24, 94
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 672
 # 672	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 104
	j	$31
	.end	func_803AC7F4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 757
 # 757	{
	.ent	recordscr_drawStuntTotals 2
recordscr_drawStuntTotals:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 757
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	recordscr_drawStuntTotals
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 581
 # 581	{
	.ent	func_803ACA94 2
func_803ACA94:
	.option	O3
	subu	$sp, 128
	sw	$31, 28($sp)
	.mask	0x80000000, -100
	.frame	$sp, 128, $31
	.loc	2 581
	.loc	2 581
	.loc	2 586
 # 582	    s32 unresolved_local;
 # 583	    s32 x,y;
 # 584	    u8 *label,text[48],*data;
 # 585	    s32 i,rows;
 # 586	    if(D_803BA800)data=PROFILE_DATA+12+D_803BA858*64;
	lw	$2, D_803BA800
	beq	$2, 0, $158
	.loc	2 586
	lw	$14, 0($2)
	lw	$15, 44($14)
	lw	$24, 0($15)
	lb	$25, D_803BA858
	mul	$14, $25, 64
	addu	$19, $24, $14
	addu	$19, $19, 12
	b	$159
$158:
	.loc	2 587
 # 587	    else data=D_80151410+D_803BA858*64-1280;
	lb	$15, D_803BA858
	mul	$25, $15, 64
	addu	$24, $25, -1280
	la	$14, D_80151410
	addu	$19, $24, $14
$159:
	.loc	2 588
 # 588	    object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 588
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 589
 # 589	    rows=0;
	move	$23, $0
	.loc	2 590
 # 590	    for(i=0;i<8;i++) {
	move	$17, $0
	lw	$30, 120($sp)
	lw	$21, 116($sp)
	addu	$18, $sp, 64
$160:
	.loc	2 590
	.loc	2 591
 # 591	        if(i==0) {x=190;y=90;}
	bne	$17, 0, $161
	.loc	2 591
	.loc	2 591
	li	$30, 190
	.loc	2 591
	li	$21, 90
	b	$163
$161:
	.loc	2 592
 # 592	        else if(i==4 || i==5)continue;
	beq	$17, 4, $175
	beq	$17, 5, $175
	.loc	2 592
	.loc	2 593
 # 593	        else if((i==6 || i==7) && !D_803BA800)continue;
	beq	$17, 6, $162
	bne	$17, 7, $163
$162:
	lw	$15, D_803BA800
	beq	$15, 0, $175
	.loc	2 593
$163:
	la	$2, countdown_state
	.loc	2 594
 # 594	        label=NAME(30,i);
	.noalias	$2,$sp
	lw	$25, 16($2)
	lw	$24, 12($2)
	lhu	$14, 60($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	mul	$14, $17, 4
	addu	$25, $24, $14
	lw	$16, 0($25)
	.loc	2 595
 # 595	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	addu	$5, $30, -4
	sll	$15, $5, 16
	move	$5, $15
	sra	$24, $5, 16
	move	$5, $24
	addu	$22, $30, 4
	sll	$14, $22, 16
	move	$22, $14
	sra	$25, $22, 16
	move	$22, $25
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_calc
	.alias	$2,$sp
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$20, $21, 16
	sra	$24, $20, 16
	move	$20, $24
	sll	$5, $20, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00080E,0x00000000
	jal	state_utility
	.loc	2 596
 # 596	        switch(i) {
	bgeu	$17, 8, $174
	sll	$25, $17, 2
	lw	$25, $164($25)
	j	$25
	.rdata	
$164:
	.word	$166
	.word	$167
	.word	$168
	.word	$169
	.word	$170
	.word	$171
	.word	$172
	.word	$173
	.text	
$166:
	.loc	2 597
 # 597	        case 0:fcvt_wrapper(text,D_803B8B38,U32_AT(data,4));break;
	move	$4, $18
	la	$5, D_803B8B38
	lw	$6, 4($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 597
	b	$174
$167:
	.loc	2 598
 # 598	        case 1:fcvt_wrapper(text,D_803B8B3C,U32_AT(data,8));break;
	move	$4, $18
	la	$5, D_803B8B3C
	lw	$6, 8($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 598
	b	$174
$168:
	.loc	2 599
 # 599	        case 2:fcvt_wrapper(text,D_803B8B40,U32_AT(data,12));break;
	move	$4, $18
	la	$5, D_803B8B40
	lw	$6, 12($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 599
	b	$174
$169:
	.loc	2 600
 # 600	        case 3:fcvt_wrapper(text,D_803B8B44,U16_AT(data,16));break;
	move	$4, $18
	la	$5, D_803B8B44
	lhu	$6, 16($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 600
	b	$174
$170:
	.loc	2 601
 # 601	        case 4:fcvt_wrapper(text,D_803B8B48,U16_AT(data,18));break;
	move	$4, $18
	la	$5, D_803B8B48
	lhu	$6, 18($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 601
	b	$174
$171:
	.loc	2 602
 # 602	        case 5:fcvt_wrapper(text,D_803B8B4C,U32_AT(data,20));break;
	move	$4, $18
	la	$5, D_803B8B4C
	lw	$6, 20($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 602
	b	$174
$172:
	.loc	2 603
 # 603	        case 6:fcvt_wrapper(text,D_803B8B50,func_800B78A4((u16)(U16_AT(data,60)&255),16));break;
	lhu	$4, 60($19)
	and	$15, $4, 255
	move	$4, $15
	and	$24, $4, 65535
	move	$4, $24
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	move	$4, $18
	la	$5, D_803B8B50
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 603
	b	$174
$173:
	.loc	2 604
 # 604	        case 7:fcvt_wrapper(text,D_803B8B58,func_800B78A4((u16)(U16_AT(data,60)&65280),16));break;
	lhu	$4, 60($19)
	and	$14, $4, 65280
	move	$4, $14
	and	$25, $4, 65535
	move	$4, $25
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	move	$4, $18
	la	$5, D_803B8B58
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 604
$174:
	.loc	2 606
 # 605	        }
 # 606	        state_utility(x+4,y,text);y+=object_bytes_sum_global();rows++;
	sll	$4, $22, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $20, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 606
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$21, $21, $2
	.loc	2 606
	addu	$23, $23, 1
	.loc	2 590
 # 590	    for(i=0;i<8;i++) {
$175:
	addu	$17, $17, 1
	bne	$17, 8, $160
	sw	$30, 120($sp)
	sw	$21, 116($sp)
	.loc	2 608
 # 608	    crowd_cheer_play(D_803B6530,80,86,240,94+object_bytes_sum_global()*rows);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 80
	li	$6, 86
	li	$7, 240
	mul	$14, $2, $23
	addu	$25, $14, 94
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 609
 # 609	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 128
	j	$31
	.end	func_803ACA94
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 556
 # 556	{
	.ent	func_803ACD5C 2
func_803ACD5C:
	.option	O3
	subu	$sp, 112
	sw	$31, 28($sp)
	.mask	0x80000000, -84
	.frame	$sp, 112, $31
	.loc	2 556
	.loc	2 556
	.loc	2 562
 # 557	    u8 *data;
 # 558	    s32 x,y;
 # 559	    u8 *label;
 # 560	    s32 i;
 # 561	    u8 text[40];
 # 562	    if(D_803BA800)data=PROFILE_DATA+1404+D_803BA858*12;
	lw	$2, D_803BA800
	beq	$2, 0, $176
	.loc	2 562
	lw	$14, 0($2)
	lw	$15, 44($14)
	lw	$24, 0($15)
	lb	$25, D_803BA858
	mul	$14, $25, 12
	addu	$19, $24, $14
	addu	$19, $19, 1404
	b	$177
$176:
	.loc	2 563
 # 563	    else data=D_80151578+D_803BA858*12-144;
	lb	$15, D_803BA858
	mul	$25, $15, 12
	addu	$24, $25, -144
	la	$14, D_80151578
	addu	$19, $24, $14
$177:
	.loc	2 564
 # 564	    object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 564
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 565
 # 565	    for(i=0;i<3;i++) {
	move	$17, $0
	move	$21, $0
	lw	$30, 104($sp)
	lw	$22, 100($sp)
	addu	$18, $sp, 52
$178:
	.loc	2 565
	.loc	2 566
 # 566	        if(i==0) {x=190;y=90;}
	bne	$17, 0, $179
	.loc	2 566
	.loc	2 566
	li	$30, 190
	.loc	2 566
	li	$22, 90
$179:
	la	$2, countdown_state
	.loc	2 567
 # 567	        label=NAME(29,i);
	.noalias	$2,$sp
	lw	$15, 16($2)
	lw	$25, 12($2)
	lhu	$24, 58($25)
	mul	$14, $24, 4
	addu	$25, $15, $14
	addu	$24, $25, $21
	lw	$16, 0($24)
	.loc	2 568
 # 568	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	addu	$5, $30, -4
	sll	$15, $5, 16
	move	$5, $15
	sra	$14, $5, 16
	move	$5, $14
	addu	$23, $30, 4
	sll	$25, $23, 16
	move	$23, $25
	sra	$24, $23, 16
	move	$23, $24
	.livereg	0x0C00010E,0x00000000
	jal	sound_pitch_diff_calc
	.alias	$2,$sp
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$20, $22, 16
	sra	$14, $20, 16
	move	$20, $14
	sll	$5, $20, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $16
	.livereg	0x0E00080E,0x00000000
	jal	state_utility
	.loc	2 569
 # 569	        switch(i) {
	beq	$17, 0, $180
	beq	$17, 1, $181
	beq	$17, 2, $182
	beq	$17, 3, $183
	b	$184
$180:
	.loc	2 570
 # 570	        case 0:fcvt_wrapper(text,D_803B8B28,U16_AT(data,4));break;
	move	$4, $18
	la	$5, D_803B8B28
	lhu	$6, 4($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 570
	b	$184
$181:
	.loc	2 571
 # 571	        case 1:fcvt_wrapper(text,D_803B8B2C,U16_AT(data,6));break;
	move	$4, $18
	la	$5, D_803B8B2C
	lhu	$6, 6($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 571
	b	$184
$182:
	.loc	2 572
 # 572	        case 2:fcvt_wrapper(text,D_803B8B30,U16_AT(data,8));break;
	move	$4, $18
	la	$5, D_803B8B30
	lhu	$6, 8($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 572
	b	$184
$183:
	.loc	2 573
 # 573	        case 3:fcvt_wrapper(text,D_803B8B34,U16_AT(data,10));break;
	move	$4, $18
	la	$5, D_803B8B34
	lhu	$6, 10($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 573
$184:
	.loc	2 575
 # 574	        }
 # 575	        state_utility(x+4,y,text);
	sll	$4, $23, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $20, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 576
 # 576	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$22, $22, $2
	.loc	2 565
 # 565	    for(i=0;i<3;i++) {
	addu	$17, $17, 1
	addu	$21, $21, 4
	bne	$17, 3, $178
	sw	$30, 104($sp)
	.loc	2 578
 # 578	    crowd_cheer_play(D_803B6530,80,86,240,y+4);
	lw	$4, D_803B6530
	li	$5, 80
	li	$6, 86
	li	$7, 240
	addu	$14, $22, 4
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 579
 # 579	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 112
	j	$31
	.end	func_803ACD5C
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 535
 # 535	{
	.ent	func_803ACF5C 2
func_803ACF5C:
	.option	O3
	subu	$sp, 112
	sw	$31, 28($sp)
	.mask	0x80000000, -84
	.frame	$sp, 112, $31
	.loc	2 535
	.loc	2 535
	.loc	2 539
 # 536	    s32 track,y,i;
 # 537	    u8 *data;
 # 538	    u8 text[48];
 # 539	    func_800B669C(1,3);
	li	$4, 1
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 540
 # 540	    track=D_803BA858/2;
	lb	$2, D_803BA858
	div	$3, $2, 2
	.loc	2 541
 # 541	    if(D_803BA858&1)track+=6;
	and	$14, $2, 1
	beq	$14, 0, $185
	.loc	2 541
	addu	$3, $3, 6
$185:
	.loc	2 542
 # 542	    if(D_803BA800)data=PROFILE_DATA+140+track*96;
	lw	$2, D_803BA800
	beq	$2, 0, $186
	.loc	2 542
	lw	$15, 0($2)
	lw	$24, 44($15)
	lw	$25, 0($24)
	mul	$14, $3, 96
	addu	$19, $25, $14
	addu	$19, $19, 140
	b	$187
$186:
	.loc	2 543
 # 543	    else data=D_80150F88+track*96;
	mul	$15, $3, 96
	la	$24, D_80150F88
	addu	$19, $15, $24
$187:
	.loc	2 544
 # 544	    object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 544
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	la	$2, countdown_state
	.loc	2 545
 # 545	    state_utility(160,90,NAME(27,category));
	li	$4, 160
	li	$5, 90
	.noalias	$2,$sp
	lw	$25, 16($2)
	lw	$14, 12($2)
	lhu	$15, 54($14)
	mul	$24, $15, 4
	addu	$14, $25, $24
	mul	$15, $20, 4
	addu	$25, $14, $15
	lw	$6, 0($25)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.alias	$2,$sp
	.loc	2 546
 # 546	    y=90+object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $2, 90
	.loc	2 547
 # 547	    object_create(2);
	li	$4, 2
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 548
 # 548	    for(i=0;i<5;i++) {
	move	$3, $0
	move	$16, $0
	mul	$24, $20, 20
	addu	$17, $19, $24
	li	$20, 20
	addu	$19, $sp, 48
$188:
	.loc	2 548
	.loc	2 549
 # 549	        func_803AC330(text,F32_AT(data,4+category*20+i*4),0);
	move	$4, $19
	l.s	$f12, 4($17)
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 550
 # 550	        state_utility(160,y,text);
	li	$4, 160
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $19
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 551
 # 551	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 548
 # 548	    for(i=0;i<5;i++) {
	addu	$16, $16, 4
	addu	$17, $17, 4
	bne	$16, $20, $188
	.loc	2 553
 # 549	        func_803AC330(text,F32_AT(data,4+category*20+i*4),0);
 # 550	        state_utility(160,y,text);
 # 551	        y+=object_bytes_sum_global();
 # 552	    }
 # 553	    crowd_cheer_play(D_803B6530,100,86,220,94+object_bytes_sum_global()*6);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 100
	li	$6, 86
	li	$7, 220
	mul	$15, $2, 6
	addu	$25, $15, 94
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 554
 # 554	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 112
	j	$31
	.end	func_803ACF5C
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 403
 # 403	{
	.ent	func_803AD0D4 2
func_803AD0D4:
	.option	O3
	subu	$sp, 144
	sw	$31, 28($sp)
	.mask	0x80000000, -116
	.frame	$sp, 144, $31
	.loc	2 403
	.loc	2 403
	.loc	2 408
 # 404	    s32 track,i,x,y;
 # 405	    u8 *label,text[48],*data;
 # 406	    u32 distance;
 # 407	    f32 average;
 # 408	    func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 409
 # 409	    track=D_803BA858/2;
	lb	$2, D_803BA858
	div	$3, $2, 2
	.loc	2 410
 # 410	    if(D_803BA858&1)track+=6;
	and	$14, $2, 1
	beq	$14, 0, $189
	.loc	2 410
	addu	$3, $3, 6
$189:
	.loc	2 411
 # 411	    if(D_803BA800)data=PROFILE_DATA+140+track*96;
	lw	$2, D_803BA800
	beq	$2, 0, $190
	.loc	2 411
	lw	$15, 0($2)
	lw	$24, 44($15)
	lw	$25, 0($24)
	mul	$14, $3, 96
	addu	$19, $25, $14
	addu	$19, $19, 140
	b	$191
$190:
	.loc	2 412
 # 412	    else data=D_80150F88+track*96;
	mul	$15, $3, 96
	la	$24, D_80150F88
	addu	$19, $15, $24
$191:
	.loc	2 413
 # 413	    distance=U32_AT(data,88);
	lw	$30, 88($19)
	.loc	2 414
 # 414	    if(D_80146111)distance=distance*10/6;
	lb	$25, D_80146111
	beq	$25, 0, $192
	.loc	2 414
	mul	$14, $30, 10
	move	$30, $14
	divu	$15, $30, 6
	move	$30, $15
$192:
	.loc	2 415
 # 415	    object_create(11);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 416
 # 416	    crowd_cheer_play(D_803B6530,12,86,308,94+object_bytes_sum_global()*8);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 12
	li	$6, 86
	li	$7, 308
	mul	$24, $2, 8
	addu	$25, $24, 94
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 417
 # 417	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 418
 # 418	    for(i=0;i<14;i++) {
	move	$17, $0
	li.s	$f20, 0.0
	lw	$21, 132($sp)
	lw	$20, 128($sp)
	addu	$18, $sp, 76
$193:
	.loc	2 418
	.loc	2 419
 # 419	        if(i==0) {x=100;y=90;}
	bne	$17, 0, $194
	.loc	2 419
	.loc	2 419
	li	$21, 100
	.loc	2 419
	li	$20, 90
	b	$199
$194:
	.loc	2 420
 # 420	        else if(i==3) {x=110;y+=object_bytes_sum_global();}
	bne	$17, 3, $195
	.loc	2 420
	.loc	2 420
	li	$21, 110
	.loc	2 420
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	b	$199
$195:
	.loc	2 421
 # 421	        else if(i==7) {x=270;y=90+object_bytes_sum_global()*4;}
	bne	$17, 7, $196
	.loc	2 421
	.loc	2 421
	li	$21, 270
	.loc	2 421
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$20, $2, 4
	addu	$20, $20, 90
	b	$199
$196:
	.loc	2 422
 # 422	        else if(i==10) {x=260;y=90;continue;}
	bne	$17, 10, $197
	.loc	2 422
	.loc	2 422
	li	$21, 260
	.loc	2 422
	li	$20, 90
	.loc	2 422
	b	$223
$197:
	.loc	2 423
 # 423	        else if((i==12 || i==13) && !D_803BA800)continue;
	beq	$17, 12, $198
	bne	$17, 13, $199
$198:
	lw	$14, D_803BA800
	beq	$14, 0, $223
	.loc	2 423
$199:
	la	$4, countdown_state
	.loc	2 424
 # 424	        label=NAME(28,i);
	.noalias	$4,$sp
	lw	$2, 16($4)
	lw	$15, 12($4)
	lhu	$3, 56($15)
	mul	$24, $3, 4
	addu	$25, $2, $24
	mul	$14, $17, 4
	addu	$15, $25, $14
	lw	$16, 0($15)
	.loc	2 425
 # 425	        if(i==11 && D_80146111)label=NAME(28,14);
	addu	$5, $21, -4
	sll	$24, $5, 16
	move	$5, $24
	sra	$25, $5, 16
	move	$5, $25
	addu	$23, $21, 4
	sll	$14, $23, 16
	move	$23, $14
	sra	$15, $23, 16
	move	$23, $15
	bne	$17, 11, $200
	.alias	$4,$sp
	lb	$24, D_80146111
	beq	$24, 0, $200
	.loc	2 425
	mul	$25, $3, 4
	addu	$14, $2, $25
	lw	$16, 56($14)
$200:
	.loc	2 426
 # 426	        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
	move	$4, $16
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$22, $20, 16
	sra	$24, $22, 16
	move	$22, $24
	sll	$5, $22, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $16
	.livereg	0x0E00020E,0x00000000
	jal	state_utility
	.loc	2 427
 # 427	        switch(i) {
	bgeu	$17, 14, $218
	sll	$14, $17, 2
	lw	$14, $201($14)
	j	$14
	.rdata	
$201:
	.word	$203
	.word	$204
	.word	$205
	.word	$207
	.word	$208
	.word	$209
	.word	$210
	.word	$211
	.word	$212
	.word	$213
	.word	$214
	.word	$215
	.word	$216
	.word	$217
	.text	
$203:
	.loc	2 428
 # 428	        case 0:func_803AC330(text,F32_AT(data,64),0);break;
	move	$4, $18
	l.s	$f12, 64($19)
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 428
	b	$218
$204:
	.loc	2 429
 # 429	        case 1:fcvt_wrapper(text,D_803B8AE8,U16_AT(data,68));break;
	move	$4, $18
	la	$5, D_803B8AE8
	lhu	$6, 68($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 429
	b	$218
$205:
	.loc	2 431
 # 430	        case 2:
 # 431	            average=0.0f;
	mov.s	$f12, $f20
	.loc	2 432
 # 432	            if(U16_AT(data,68)>0)
	lhu	$2, 68($19)
	ble	$2, 0, $206
	.loc	2 433
 # 433	                average=F32_AT(data,64)/(f32)(u32)U16_AT(data,68);
	l.s	$f4, 64($19)
	mtc1	$2, $f6
	cvt.s.w	$f8, $f6
	bge	$2, $0, $224
	li.s	$f10, 4294967296.0
	add.s	$f8, $f8, $f10
$224:
	div.s	$f12, $f4, $f8
$206:
	.loc	2 434
 # 434	            func_803AC330(text,average,0);
	move	$4, $18
	sw	$0, 8($sp)
	.livereg	0x0800000E,0x00080000
	jal	func_803AC330
	.loc	2 435
 # 435	            break;
	b	$218
$207:
	.loc	2 436
 # 436	        case 3:fcvt_wrapper(text,D_803B8AEC,U16_AT(data,70));break;
	move	$4, $18
	la	$5, D_803B8AEC
	lhu	$6, 70($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 436
	b	$218
$208:
	.loc	2 437
 # 437	        case 4:fcvt_wrapper(text,D_803B8AF0,U16_AT(data,72));break;
	move	$4, $18
	la	$5, D_803B8AF0
	lhu	$6, 72($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 437
	b	$218
$209:
	.loc	2 438
 # 438	        case 5:fcvt_wrapper(text,D_803B8AF4,U16_AT(data,74));break;
	move	$4, $18
	la	$5, D_803B8AF4
	lhu	$6, 74($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 438
	b	$218
$210:
	.loc	2 439
 # 439	        case 6:fcvt_wrapper(text,D_803B8AF8,U16_AT(data,76));break;
	move	$4, $18
	la	$5, D_803B8AF8
	lhu	$6, 76($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 439
	b	$218
$211:
	.loc	2 440
 # 440	        case 7:fcvt_wrapper(text,D_803B8AFC,U16_AT(data,78));break;
	move	$4, $18
	la	$5, D_803B8AFC
	lhu	$6, 78($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 440
	b	$218
$212:
	.loc	2 441
 # 441	        case 8:fcvt_wrapper(text,D_803B8B00,U16_AT(data,80));break;
	move	$4, $18
	la	$5, D_803B8B00
	lhu	$6, 80($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 441
	b	$218
$213:
	.loc	2 442
 # 442	        case 9:fcvt_wrapper(text,D_803B8B04,U16_AT(data,82));break;
	move	$4, $18
	la	$5, D_803B8B04
	lhu	$6, 82($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 442
	b	$218
$214:
	.loc	2 443
 # 443	        case 10:fcvt_wrapper(text,D_803B8B08,U16_AT(data,84));break;
	move	$4, $18
	la	$5, D_803B8B08
	lhu	$6, 84($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 443
	b	$218
$215:
	li	$2, 10
	.loc	2 444
 # 444	        case 11: {u32 divisor=10;fcvt_wrapper(text,D_803B8B0C,distance/divisor,distance%divisor);break;}
	.loc	2 444
	.loc	2 444
	move	$4, $18
	la	$5, D_803B8B0C
	divu	$6, $30, $2
	remu	$7, $30, $2
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 444
	b	$218
$216:
	.loc	2 445
 # 445	        case 12:fcvt_wrapper(text,D_803B8B18,func_800B78A4((u16)(U16_AT(data,92)&255),16));break;
	lhu	$4, 92($19)
	and	$15, $4, 255
	move	$4, $15
	and	$24, $4, 65535
	move	$4, $24
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	move	$4, $18
	la	$5, D_803B8B18
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 445
	b	$218
$217:
	.loc	2 446
 # 446	        case 13:fcvt_wrapper(text,D_803B8B20,func_800B78A4((u16)(U16_AT(data,92)&65280),16));break;
	lhu	$4, 92($19)
	and	$25, $4, 65280
	move	$4, $25
	and	$14, $4, 65535
	move	$4, $14
	li	$5, 16
	.livereg	0x0C00000E,0x00000000
	jal	func_800B78A4
	move	$4, $18
	la	$5, D_803B8B20
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 446
$218:
	.loc	2 448
 # 447	        }
 # 448	        if(i==0 || i==2)object_create(2);
	beq	$17, 0, $219
	bne	$17, 2, $220
$219:
	.loc	2 448
	li	$4, 2
	.livereg	0x0800000E,0x00000000
	jal	object_create
$220:
	.loc	2 449
 # 449	        state_utility(x+4,y,text);
	sll	$4, $23, 16
	sra	$15, $4, 16
	move	$4, $15
	sll	$5, $22, 16
	sra	$24, $5, 16
	move	$5, $24
	move	$6, $18
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 450
 # 450	        if(i==0 || i==2)object_create(11);
	beq	$17, 0, $221
	bne	$17, 2, $222
$221:
	.loc	2 450
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
$222:
	.loc	2 451
 # 451	        y+=object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$20, $20, $2
	.loc	2 418
 # 418	    for(i=0;i<14;i++) {
$223:
	addu	$17, $17, 1
	bne	$17, 14, $193
	sw	$21, 132($sp)
	sw	$20, 128($sp)
	.loc	2 453
 # 453	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 144
	j	$31
	.end	func_803AD0D4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 698
 # 698	{
	.ent	recordscr_drawView 2
recordscr_drawView:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 698
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	recordscr_drawView
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803AD57C
	.loc	2 775
 # 775	{
	.ent	func_803AD57C 2
func_803AD57C:
	.option	O3
	subu	$sp, 312
	sw	$31, 116($sp)
	sw	$30, 112($sp)
	sw	$23, 108($sp)
	sw	$22, 104($sp)
	sw	$21, 100($sp)
	sw	$20, 96($sp)
	sw	$19, 92($sp)
	sw	$18, 88($sp)
	sw	$17, 84($sp)
	sw	$16, 80($sp)
	s.d	$f30, 72($sp)
	s.d	$f28, 64($sp)
	s.d	$f26, 56($sp)
	s.d	$f24, 48($sp)
	s.d	$f22, 40($sp)
	s.d	$f20, 32($sp)
	.mask	0xC0FF0000, -196
	.fmask	0xFFF00000, -240
	.frame	$sp, 312, $31
	.loc	2 775
	sw	$4, 312($sp)
	.loc	2 775
	.loc	2 776
 # 776	    render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 777
 # 777	    switch(D_803BA804) {
	lw	$2, D_803BA804
	beq	$2, 0, $277
	beq	$2, 1, $278
	b	$316
$277:
	.loc	2 778
 # 778	    case 0:func_803AA3D8();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AA3D8
	.loc	2 778
	b	$316
$278:
	.loc	2 779
 # 779	    case 1:recordscr_drawView();break;
	.loc	2 698
 # 698	{
	.loc	2 703
 # 699	    s32 i, x, y, cx;
 # 700	    u8 *str;
 # 701	    u8 str1[64];
 # 702	    s16 screenPos[2];
 # 703	        func_800B669C(1,1);
	li	$4, 1
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 704
 # 704	        brake_light_update(0,D_803B5F28[17].position,&D_80150B70[0],0,screenPos);
	move	$4, $0
	la	$5, D_803B5F28+1136
	la	$6, D_80150B70
	move	$7, $0
	addu	$14, $sp, 216
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 705
 # 705	        cx=screenPos[0];y=screenPos[1];
	lh	$22, 216($sp)
	.loc	2 705
	lh	$19, 218($sp)
	.loc	2 706
 # 706	        func_800F6928(D_803B5F28[17].position[2]-10.0f);
	l.s	$f4, D_803B5F28+1144
	li.s	$f6, 10.0
	sub.s	$f12, $f4, $f6
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 707
 # 707	        dispatch_handler(1);object_create(13);dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 707
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 707
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 708
 # 708	        str=!D_803BA800 ? countdown_state.text[176] : (*D_803BA800)->name;music_fade(cx,y,140,str);
	lw	$2, D_803BA800
	bne	$2, 0, $279
	la	$21, countdown_state
	.noalias	$21,$sp
	lw	$15, 4($21)
	lw	$7, 704($15)
	b	$280
	.alias	$21,$sp
$279:
	lw	$7, 0($2)
	addu	$7, $7, 20
	la	$21, countdown_state
$280:
	.loc	2 708
	sll	$4, $22, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$5, $19, 16
	sra	$25, $5, 16
	move	$5, $25
	li	$6, 140
	.livereg	0x0F00000E,0x00000000
	jal	music_fade
	.loc	2 709
 # 709	        func_800B669C(0,1);object_bytes_sum_global();object_create(11);
	move	$4, $0
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 709
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	.loc	2 709
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 710
 # 710	        for(i=0;i<2;i++) {
	move	$20, $0
	la	$18, D_803B5F28
	addu	$23, $sp, 220
$281:
	.loc	2 710
	.loc	2 711
 # 711	            if(D_803B5F28[i+11].angle>0.6981316804885864f && D_803B5F28[i+11].angle<2.4434609413146973f)continue;
	.noalias	$18,$sp
	l.s	$f0, 712($18)
	li.s	$f8, 0.6981316804885864
	c.lt.s	$f8, $f0
	bc1f	$282
	li.s	$f10, 2.4434609413146973
	c.lt.s	$f0, $f10
	bc1t	$294
	.loc	2 711
$282:
	.loc	2 712
 # 712	            if(!D_803B5F28[i+11].alpha)continue;
	lbu	$2, 764($18)
	beq	$2, 0, $294
	.loc	2 712
	.loc	2 713
 # 713	            func_800ED66C((f32)(u32)D_803B5F28[i+11].alpha);
	mtc1	$2, $f4
	cvt.s.w	$f12, $f4
	bge	$2, $0, $317
	li.s	$f6, 4294967296.0
	add.s	$f12, $f12, $f6
$317:
	addu	$16, $18, 752
	mul	$14, $20, 4
	sw	$14, 124($sp)
	.livereg	0x0000800E,0x00080000
	jal	func_800ED66C
	.loc	2 714
 # 714	            if(D_803B5F28[i+11].angle<1.5707963705062866f) {
	l.s	$f8, 712($18)
	li.s	$f10, 1.5707963705062866
	c.lt.s	$f8, $f10
	bc1f	$283
	.loc	2 714
	.loc	2 716
 # 715	                Color4 c1,c2;
 # 716	                func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(1);
	lw	$15, D_801146BC
	sw	$15, 0($sp)
	lw	$24, D_801146C0
	sw	$24, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 716
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$284
$283:
	.loc	2 717
 # 717	            } else {
	.loc	2 718
 # 718	                func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(22);
	lw	$25, D_801146BC
	sw	$25, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 718
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$284:
	.loc	2 720
 # 719	            }
 # 720	            func_800F6928(D_803B5F28[i+11].position[2]-10.0f);
	l.s	$f4, 760($18)
	li.s	$f6, 10.0
	sub.s	$f12, $f4, $f6
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 721
 # 721	            brake_light_update(0,D_803B5F28[i+11].position,&D_80150B70[0],0,screenPos);
	move	$4, $0
	move	$5, $16
	la	$6, D_80150B70
	move	$7, $0
	addu	$15, $sp, 216
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 722
 # 722	            x=screenPos[0]-35.0f*D_803B64E8*300.0f/D_803B64FC;y=screenPos[1];
	li.s	$f8, 35.0
	l.s	$f10, D_803B64E8
	mul.s	$f4, $f8, $f10
	li.s	$f6, 300.0
	mul.s	$f8, $f4, $f6
	l.s	$f10, D_803B64FC
	div.s	$f4, $f8, $f10
	lh	$24, 216($sp)
	mtc1	$24, $f6
	cvt.s.w	$f8, $f6
	sub.s	$f10, $f8, $f4
	trunc.w.s	$f6, $f10, $25
	mfc1	$16, $f6
	.loc	2 722
	lh	$19, 218($sp)
	.loc	2 723
 # 723	            brake_light_update(0,D_803B5F28[15].position,&D_80150B70[0],0,screenPos);
	move	$4, $0
	la	$5, D_803B5F28+1008
	la	$6, D_80150B70
	move	$7, $0
	addu	$14, $sp, 216
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 724
 # 724	            cx=screenPos[0]-55.0f*D_803B64E8*300.0f/D_803B64FC;
	li.s	$f8, 55.0
	l.s	$f4, D_803B64E8
	mul.s	$f10, $f8, $f4
	li.s	$f6, 300.0
	mul.s	$f8, $f10, $f6
	l.s	$f4, D_803B64FC
	div.s	$f10, $f8, $f4
	lh	$15, 216($sp)
	mtc1	$15, $f6
	cvt.s.w	$f8, $f6
	sub.s	$f4, $f8, $f10
	trunc.w.s	$f6, $f4, $24
	mfc1	$22, $f6
	.loc	2 725
 # 725	            str=NAME(51,i);state_utility(x,y,str);
	.noalias	$21,$18
	.noalias	$21,$sp
	lw	$25, 16($21)
	lw	$14, 12($21)
	lhu	$15, 102($14)
	mul	$24, $15, 4
	addu	$14, $25, $24
	lw	$15, 124($sp)
	addu	$25, $14, $15
	lw	$7, 0($25)
	.loc	2 725
	sll	$4, $16, 16
	sra	$24, $4, 16
	move	$4, $24
	sll	$17, $19, 16
	sra	$14, $17, 16
	move	$17, $14
	sll	$5, $17, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $7
	.livereg	0x0E00400E,0x00000000
	jal	state_utility
	.loc	2 726
 # 726	            x=cx;
	.loc	2 727
 # 727	            if(i==0) {
	bne	$20, 0, $293
	.loc	2 727
	.loc	2 728
 # 728	                if(D_803BA880==4) {
	lw	$2, D_803BA880
	bne	$2, 4, $285
	.loc	2 728
	.loc	2 729
 # 729	                    fcvt_wrapper(str1,D_803B8AD8,NAME(44,D_803BA860),countdown_state.text[42]);str=str1;
	move	$4, $23
	la	$5, D_803B8AD8
	lw	$25, 16($21)
	lw	$24, 12($21)
	lhu	$14, 88($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lb	$14, D_803BA860
	mul	$25, $14, 4
	addu	$15, $24, $25
	lw	$6, 0($15)
	lw	$14, 4($21)
	lw	$7, 168($14)
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 729
	move	$7, $23
	b	$292
$285:
	.loc	2 730
 # 730	                } else if(D_803BA880==5)str=countdown_state.text[176];
	bne	$2, 5, $286
	.loc	2 730
	lw	$24, 4($21)
	lw	$7, 704($24)
	b	$292
$286:
	.loc	2 731
 # 731	                else if(D_803BA858<12 && (D_803BA858&1)) {
	lb	$2, D_803BA858
	bge	$2, 12, $289
	and	$25, $2, 1
	beq	$25, 0, $289
	.loc	2 731
	.loc	2 732
 # 732	                    fcvt_wrapper(str1,D_803B8AE0,D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2],countdown_state.text[6]);str=str1;
	blt	$2, 13, $287
	addu	$3, $2, -6
	b	$288
$287:
	div	$3, $2, 2
$288:
	move	$4, $23
	la	$5, D_803B8AE0
	mul	$15, $3, 4
	lw	$6, D_8011A8D8($15)
	lw	$14, 4($21)
	lw	$7, 24($14)
	.livereg	0x0F00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 732
	move	$7, $23
	b	$292
$289:
	.loc	2 733
 # 733	                } else str=D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2];
	blt	$2, 13, $290
	addu	$3, $2, -6
	b	$291
$290:
	div	$3, $2, 2
$291:
	mul	$24, $3, 4
	lw	$7, D_8011A8D8($24)
$292:
	.loc	2 734
 # 734	                state_utility(x,y,str);
	sll	$4, $22, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $17, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $7
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$294
$293:
	.loc	2 735
 # 735	            } else if(i==1){str=NAME(52,D_803BA818[D_803BA880]);state_utility(x,y,str);}
	bne	$20, 1, $294
	.loc	2 735
	.loc	2 735
	lw	$14, 16($21)
	lw	$24, D_803BA880
	mul	$25, $24, 4
	lw	$15, D_803BA818($25)
	mul	$24, $15, 4
	addu	$25, $14, $24
	lw	$15, 12($21)
	lhu	$14, 104($15)
	mul	$24, $14, 4
	addu	$15, $25, $24
	lw	$7, 0($15)
	.loc	2 735
	sll	$4, $22, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	move	$6, $7
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 710
 # 710	        for(i=0;i<2;i++) {
$294:
	addu	$20, $20, 1
	addu	$18, $18, 64
	bne	$20, 2, $281
	.alias	$18,$21
	.alias	$18,$sp
	.loc	2 737
 # 737	        func_800B669C(0,3);func_800ED66C(-1.0f);render_helper(0.0f);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 737
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 737
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 738
 # 738	        switch(D_803BA818[D_803BA880]) {
	lw	$24, D_803BA880
	mul	$15, $24, 4
	lw	$14, D_803BA818($15)
	bgeu	$14, 13, $315
	sll	$14, $14, 2
	lw	$14, $295($14)
	j	$14
	.rdata	
$295:
	.word	$297
	.word	$298
	.word	$299
	.word	$300
	.word	$301
	.word	$302
	.word	$303
	.word	$309
	.word	$310
	.word	$311
	.word	$312
	.word	$313
	.word	$314
	.text	
	.alias	$21,$sp
$297:
	.loc	2 739
 # 739	        case 0:func_803AD0D4();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AD0D4
	.loc	2 739
	b	$315
$298:
	.loc	2 740
 # 740	        case 1:func_803ACF5C(0);break;
	move	$20, $0
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 740
	b	$315
$299:
	.loc	2 741
 # 741	        case 2:func_803ACF5C(1);break;
	li	$20, 1
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 741
	b	$315
$300:
	.loc	2 742
 # 742	        case 3:func_803ACF5C(2);break;
	li	$20, 2
	.livereg	0x0000080E,0x00000000
	jal	func_803ACF5C
	.loc	2 742
	b	$315
$301:
	.loc	2 743
 # 743	        case 4:func_803ACD5C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ACD5C
	.loc	2 743
	b	$315
$302:
	.loc	2 744
 # 744	        case 5:func_803ACA94();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ACA94
	.loc	2 744
	b	$315
$303:
	.loc	2 745
 # 745	        case 6:recordscr_drawStuntTotals();break;
	.loc	2 757
 # 757	{
	.loc	2 761
 # 758	            s32 statIdx, x, y;
 # 759	            u8 *stats;
 # 760	            u8 str[40];
 # 761	            if(D_803BA800)stats=PROFILE_DATA+12+D_803BA858*64;
	lw	$2, D_803BA800
	beq	$2, 0, $304
	.loc	2 761
	lw	$25, 0($2)
	lw	$24, 44($25)
	lw	$15, 0($24)
	lb	$14, D_803BA858
	mul	$25, $14, 64
	addu	$17, $15, $25
	addu	$17, $17, 12
	b	$305
$304:
	.loc	2 762
 # 762	            else stats=D_80151410+D_803BA858*64-1280;
	lb	$24, D_803BA858
	mul	$14, $24, 64
	addu	$15, $14, -1280
	la	$25, D_80151410
	addu	$17, $15, $25
$305:
	.loc	2 763
 # 763	            object_create(11);dispatch_handler(1);
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 763
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 764
 # 764	            for(statIdx=0;statIdx<10;statIdx++) {
	move	$2, $0
	move	$16, $0
	move	$19, $17
	la	$23, D_803B8B64
	la	$22, D_803B8B60
	lw	$20, 192($sp)
	lw	$18, 188($sp)
	addu	$17, $sp, 144
$306:
	.loc	2 764
	.loc	2 765
 # 765	                if(statIdx==0) {x=110;y=90;}
	bne	$16, 0, $307
	.loc	2 765
	.loc	2 765
	li	$20, 110
	.loc	2 765
	li	$18, 90
	b	$308
$307:
	.loc	2 766
 # 766	                else if(statIdx==5) {x=250;y=90;}
	bne	$16, 20, $308
	.loc	2 766
	.loc	2 766
	li	$20, 250
	.loc	2 766
	li	$18, 90
$308:
	.loc	2 767
 # 767	                fcvt_wrapper(str,D_803B8B60,NAME(37,statIdx));
	move	$4, $17
	move	$5, $22
	.noalias	$21,$sp
	lw	$24, 16($21)
	lw	$14, 12($21)
	lhu	$15, 74($14)
	mul	$25, $15, 4
	addu	$14, $24, $25
	addu	$15, $14, $16
	lw	$6, 0($15)
	addu	$24, $20, -4
	sll	$25, $24, 16
	sra	$14, $25, 16
	sw	$14, 132($sp)
	addu	$15, $20, 4
	sll	$24, $15, 16
	sra	$25, $24, 16
	sw	$25, 124($sp)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 768
 # 768	                state_utility(sound_pitch_diff_calc(str,x-4),y,str);
	move	$4, $17
	lh	$5, 134($sp)
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_calc
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $17
	sw	$5, 132($sp)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 769
 # 769	                fcvt_wrapper(str,D_803B8B64,U32_AT(stats,20+statIdx*4));
	move	$4, $17
	move	$5, $23
	lw	$6, 20($19)
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 770
 # 770	                state_utility(x+4,y,str);y+=object_bytes_sum_global();
	lh	$4, 126($sp)
	lh	$5, 134($sp)
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 770
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 764
 # 764	            for(statIdx=0;statIdx<10;statIdx++) {
	addu	$16, $16, 4
	addu	$19, $19, 4
	bne	$16, 40, $306
	.alias	$21,$sp
	sw	$20, 192($sp)
	sw	$18, 188($sp)
	.loc	2 772
 # 772	            crowd_cheer_play(D_803B6530,20,86,300,94+object_bytes_sum_global()*5);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lw	$4, D_803B6530
	li	$5, 20
	li	$6, 86
	li	$7, 300
	mul	$24, $2, 5
	addu	$25, $24, 94
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 773
 # 773	            }
	.loc	2 745
 # 745	        case 6:recordscr_drawStuntTotals();break;
	b	$315
$309:
	.loc	2 746
 # 746	        case 7:func_803AC7F4();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AC7F4
	.loc	2 746
	b	$315
$310:
	.loc	2 747
 # 747	        case 8:func_803AC54C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AC54C
	.loc	2 747
	b	$315
$311:
	.loc	2 748
 # 748	        case 9:func_803ABEC8();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803ABEC8
	.loc	2 748
	b	$315
$312:
	.loc	2 749
 # 749	        case 10:func_803AB848();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AB848
	.loc	2 749
	b	$315
$313:
	.loc	2 750
 # 750	        case 11:func_803AB5FC();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AB5FC
	.loc	2 750
	b	$315
$314:
	.loc	2 751
 # 751	        case 12:func_803AAF0C();break;
	.livereg	0x0000000E,0x00000000
	jal	func_803AAF0C
	.loc	2 751
$315:
	.loc	2 753
 # 752	        }
 # 753	        func_800B669C(0,3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 754
 # 754	}
	.loc	2 779
 # 779	    case 1:recordscr_drawView();break;
$316:
	.loc	2 781
 # 780	    }
 # 781	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 782
 # 782	    return 1;
	li	$2, 1
	.livereg	0x2000FF0E,0x00000FFF
	l.d	$f20, 32($sp)
	l.d	$f22, 40($sp)
	l.d	$f24, 48($sp)
	l.d	$f26, 56($sp)
	l.d	$f28, 64($sp)
	l.d	$f30, 72($sp)
	lw	$16, 80($sp)
	lw	$17, 84($sp)
	lw	$18, 88($sp)
	lw	$19, 92($sp)
	lw	$20, 96($sp)
	lw	$21, 100($sp)
	lw	$22, 104($sp)
	lw	$23, 108($sp)
	lw	$31, 116($sp)
	lw	$30, 112($sp)
	addu	$sp, 312
	j	$31
	.end	func_803AD57C
