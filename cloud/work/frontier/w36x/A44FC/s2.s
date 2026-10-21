	.text
	.globl	func_803A44FC
	.ent	func_803A44FC 2
func_803A44FC:
	.option	O3
	.alias	$4,$sp
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
	.loc	2 84
	sw	$4, 312($sp)
	.loc	2 84
	.loc	2 86
 #  85	    s32 i;
 #  86	    for (i = 0; i < D_8014A108; i++) {
	move	$19, $0
	lh	$3, D_8014A108
	ble	$3, 0, $88
	sw	$0, 152($sp)
	la	$2, D_803B9B90
	li.s	$f30, 1.5707964
	li.s	$f28, 2.443461
	li.s	$f26, 0.6981317
	li.s	$f24, 5.0E-1
	la	$21, countdown_state
$32:
	.loc	2 86
	.loc	2 87
 #  87	        if (D_803B9B90[i] == 1) animButtonText(i);
	.noalias	$2,$sp
	lw	$14, 0($2)
	bne	$14, 1, $87
	.loc	2 87
	sw	$19, 304($sp)
	.loc	2 94
 #  94	{
	.loc	2 102
 # 102	    f32 gTextPosScale = D_803B25AC;
	l.s	$f22, D_803B25AC
	.loc	2 103
 # 103	    if (D_80151AD0 > 1) gTextPosScale *= 0.5f;
	mul	$15, $19, 152
	la	$24, D_80150B70
	addu	$25, $15, $24
	sw	$25, 128($sp)
	la	$9, D_803BA028
	addu	$10, $19, $9
	sw	$10, 132($sp)
	mul	$11, $19, 76
	la	$12, D_8014A118
	addu	$22, $11, $12
	mul	$13, $19, 2816
	addu	$14, $13, 2480
	la	$15, D_803AF9A8
	addu	$16, $14, $15
	lh	$24, D_80151AD0
	blt	$24, 2, $33
	.loc	2 103
	li.s	$f4, 0.5
	mul.s	$f22, $f22, $f4
$33:
	.loc	2 104
 # 104	    render_helper(0.0f);
	li.s	$f12, 0.0
	sw	$2, 148($sp)
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.alias	$2,$sp
	.loc	2 105
 # 105	    func_800B669C(1, 1);
	li	$4, 1
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 106
 # 106	    if (D_80151AD0 == 1) { w = 140; object_create(13); }
	lh	$25, D_80151AD0
	bne	$25, 1, $34
	.loc	2 106
	.loc	2 106
	li	$20, 140
	.loc	2 106
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$35
$34:
	.loc	2 107
 # 107	    else { w = 69; object_create(12); }
	.loc	2 107
	li	$20, 69
	.loc	2 107
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$35:
	.loc	2 108
 # 108	    brake_light_update(p, D_803AF9A8[p][38].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	move	$5, $16
	lw	$6, 128($sp)
	move	$7, $0
	addu	$9, $sp, 216
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 109
 # 109	    x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 110
 # 110	    y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 111
 # 111	    str = (*D_8014A118[p].owner)->name;
	.noalias	$22,$sp
	lw	$10, 72($22)
	lw	$16, 0($10)
	addu	$16, $16, 20
	.loc	2 112
 # 112	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.alias	$22,$sp
	.loc	2 113
 # 113	    music_fade(x, y, w, str);
	sll	$4, $17, 16
	sra	$11, $4, 16
	move	$4, $11
	sll	$5, $18, 16
	sra	$12, $5, 16
	move	$5, $12
	sll	$6, $20, 16
	sra	$13, $6, 16
	move	$6, $13
	move	$7, $16
	.livereg	0x0F00000E,0x00000000
	jal	music_fade
	.loc	2 114
 # 114	    func_800B669C(0, 1);
	move	$4, $0
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 115
 # 115	    if (D_8014A110 == 2) {
	lw	$14, D_8014A110
	bne	$14, 2, $41
	.loc	2 115
	.loc	2 117
 # 116	        s32 ghostIdx;
 # 117	        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$20, 1
	lb	$15, D_803BA00C
	addu	$24, $15, 1
	blt	$24, 2, $41
	la	$23, D_80150B70+152
	li	$25, 4
	sw	$25, 140($sp)
	la	$30, D_80152698+4
$36:
	.loc	2 117
	.loc	2 119
 # 118	            f32 pos[3];
 # 119	            brake_light_update(ghostIdx, D_803AF9A8[0][38].position, &D_80150B70[ghostIdx], 0, screenPos);
	move	$4, $20
	la	$5, D_803AF9A8+2480
	move	$6, $23
	move	$7, $0
	addu	$9, $sp, 216
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 120
 # 120	            x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 121
 # 121	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 122
 # 122	            fcvt_wrapper(str1, "GHOST %d", ghostIdx);
	addu	$4, $sp, 220
	la	$5, $$23
	move	$6, $20
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 123
 # 123	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 124
 # 124	            state_utility(sound_pitch_diff_halved(str1, x), y, str1);
	addu	$4, $sp, 220
	sll	$5, $17, 16
	sra	$10, $5, 16
	move	$5, $10
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$11, $4, 16
	move	$4, $11
	sll	$5, $18, 16
	sra	$12, $5, 16
	move	$5, $12
	addu	$6, $sp, 220
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	la	$2, D_803AF9A8
	.loc	2 125
 # 125	            pos[0] = D_803AF9A8[0][38].position[0];
	.noalias	$2,$sp
	l.s	$f6, 2480($2)
	s.s	$f6, 196($sp)
	.loc	2 126
 # 126	            pos[1] = D_803AF9A8[0][38].position[1];
	l.s	$f0, 2484($2)
	.loc	2 127
 # 127	            pos[2] = D_803AF9A8[0][38].position[2];
	l.s	$f8, 2488($2)
	s.s	$f8, 204($sp)
	.loc	2 128
 # 128	            pos[1] = -pos[1] / 2.0f;
	neg.s	$f10, $f0
	mul.s	$f0, $f10, $f24
	.loc	2 129
 # 129	            brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, screenPos);
	move	$4, $20
	addu	$5, $sp, 196
	move	$6, $23
	move	$7, $0
	addu	$13, $sp, 216
	sw	$13, 16($sp)
	s.s	$f0, 200($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.alias	$2,$sp
	.loc	2 130
 # 130	            x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 131
 # 131	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 132
 # 132	            if (!D_80152698[ghostIdx]) {
	.noalias	$30,$sp
	lw	$2, 0($30)
	bne	$2, 0, $39
	.loc	2 132
	.loc	2 133
 # 133	                str = !D_803AF980 ? countdown_state.text[225] : countdown_state.text[224];
	lw	$14, D_803AF980
	bne	$14, 0, $37
	.noalias	$21,$30
	.noalias	$21,$sp
	lw	$15, 4($21)
	lw	$16, 900($15)
	b	$38
$37:
	lw	$24, 4($21)
	lw	$16, 896($24)
$38:
	.loc	2 134
 # 134	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 135
 # 135	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$9, $4, 16
	move	$4, $9
	sll	$5, $18, 16
	sra	$10, $5, 16
	move	$5, $10
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$40
$39:
	.loc	2 136
 # 136	            } else {
	.loc	2 137
 # 137	                str = (*D_80152698[ghostIdx])->name;
	lw	$16, 0($2)
	addu	$16, $16, 10
	.loc	2 138
 # 138	                dispatch_handler(D_801146AC[ghostIdx]);
	lw	$11, 140($sp)
	lw	$4, D_801146AC($11)
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 139
 # 139	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$22, $17, 16
	sra	$12, $22, 16
	move	$22, $12
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 140
 # 140	                y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	li	$8, 60
	addu	$18, $18, $2
	.loc	2 141
 # 141	                fcvt_wrapper(str1, "TIME %01d:%02d.%03d", (s32)(*D_80152698[ghostIdx])->time / 60,
	addu	$4, $sp, 220
	la	$5, $$31
	lw	$24, 0($30)
	lw	$25, 0($24)
	l.s	$f0, 32($25)
	trunc.w.s	$f16, $f0, $9
	mfc1	$3, $f16
	div	$6, $3, $8
	rem	$7, $3, $8
	li.s	$f18, 1000.0
	mul.s	$f4, $f0, $f18
	trunc.w.s	$f6, $f4, $10
	mfc1	$11, $f6
	rem	$12, $11, 1000
	sw	$12, 16($sp)
	.livereg	0x1F00000E,0x80000000
	jal	fcvt_wrapper
	.loc	2 143
 # 142	                             (s32)(*D_80152698[ghostIdx])->time % 60, (s32)(1000.0f * (*D_80152698[ghostIdx])->time) % 1000);
 # 143	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 144
 # 144	                state_utility(sound_pitch_diff_halved(str1, x), y, str1);
	addu	$4, $sp, 220
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	addu	$6, $sp, 220
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$40:
	.loc	2 117
 # 117	        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	addu	$20, $20, 1
	addu	$23, $23, 152
	lw	$24, 140($sp)
	addu	$25, $24, 4
	sw	$25, 140($sp)
	addu	$30, $30, 4
	lb	$9, D_803BA00C
	ble	$20, $9, $36
	.alias	$30,$21
	.alias	$30,$sp
$41:
	.loc	2 148
 # 148	    if (D_803BA028[p]) {
	lw	$10, 132($sp)
	lb	$11, 0($10)
	beq	$11, 0, $48
	addu	$5, $sp, 172
	.loc	2 148
	.loc	2 152
 # 149	        s32 notReadyCount;
 # 150	        s32 notReadyIdx;
 # 151	        s32 pIdx;
 # 152	        f32 msgPos[3] = {0.0f, -120.0f, 300.0f};
	.noalias	$5,$21
	.noalias	$5,$gp
	la	$12, $$33
	.set	 noat
	.set	 noat
	lw	$1, 0($12)
	sw	$1, 0($5)
	lw	$14, 4($12)
	sw	$14, 4($5)
	lw	$1, 8($12)
	sw	$1, 8($5)
	.set	 at
	.loc	2 153
 # 153	        brake_light_update(p, msgPos, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	lw	$6, 128($sp)
	move	$7, $0
	addu	$15, $sp, 216
	sw	$15, 16($sp)
	move	$20, $0
	move	$16, $0
	.livereg	0x0F00880E,0x00000000
	jal	brake_light_update
	.alias	$5,$21
	.alias	$5,$gp
	.loc	2 154
 # 154	        x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 155
 # 155	        y = screenPos[1] - object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lh	$24, 218($sp)
	subu	$18, $24, $2
	.loc	2 156
 # 156	        notReadyCount = 0;
	.loc	2 157
 # 157	        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	lh	$3, D_8014A108
	ble	$3, 0, $44
$42:
	.loc	2 157
	.loc	2 158
 # 158	            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
	beq	$19, $16, $43
	lb	$25, D_803BA028($16)
	bne	$25, 0, $43
	.loc	2 158
	.loc	2 158
	sw	$16, 188($sp)
	.loc	2 158
	addu	$20, $20, 1
$43:
	.loc	2 157
 # 157	        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	addu	$16, $16, 1
	blt	$16, $3, $42
$44:
	.loc	2 160
 # 158	            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
 # 159	        }
 # 160	        if (!notReadyCount) {
	bne	$20, 0, $45
	.loc	2 160
	.loc	2 161
 # 161	            y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 162
 # 162	            str = (u8 *)"GET READY TO RACE";
	la	$16, $$34
	.loc	2 163
 # 163	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	sll	$22, $17, 16
	sra	$9, $22, 16
	move	$22, $9
	b	$47
$45:
	.loc	2 164
 # 164	        } else {
	.loc	2 165
 # 165	            str = countdown_state.text[151];
	lw	$10, 4($21)
	lw	$16, 604($10)
	.loc	2 166
 # 166	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 167
 # 167	            state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$22, $17, 16
	sra	$11, $22, 16
	move	$22, $11
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$12, $4, 16
	move	$4, $12
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 168
 # 168	            y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 169
 # 169	            if (notReadyCount == 1) {
	bne	$20, 1, $46
	.loc	2 169
	.loc	2 170
 # 170	                dispatch_handler(D_801146AC[notReadyIdx]);
	lw	$15, 188($sp)
	mul	$24, $15, 4
	lw	$4, D_801146AC($24)
	mul	$25, $15, 76
	la	$9, D_8014A118
	addu	$17, $25, $9
	.livereg	0x0800400E,0x00000000
	jal	dispatch_handler
	.loc	2 171
 # 171	                str = (*D_8014A118[notReadyIdx].owner)->name;
	.noalias	$17,$21
	.noalias	$17,$sp
	lw	$10, 72($17)
	lw	$16, 0($10)
	addu	$16, $16, 20
	b	$47
	.alias	$17,$21
	.alias	$17,$sp
$46:
	.loc	2 172
 # 172	            } else str = countdown_state.text[31];
	lw	$11, 4($21)
	lw	$16, 124($11)
$47:
	.loc	2 174
 # 173	        }
 # 174	        state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$12, $4, 16
	move	$4, $12
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 175
 # 175	        func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 176
 # 176	        func_800ED66C(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 177
 # 177	        return;
	b	$86
$48:
	.loc	2 179
 # 178	    }
 # 179	    if (D_80151AD0 == 1) object_create(11); else object_create(12);
	move	$2, $0
	lw	$24, 152($sp)
	la	$15, D_803BA050
	addu	$16, $24, $15
	lh	$25, D_80151AD0
	bne	$25, 1, $49
	.loc	2 179
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$50
$49:
	.loc	2 179
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$50:
	.loc	2 180
 # 180	    for (i = 0; i < D_803BA050[p]; i++) {
	.noalias	$16,$21
	.noalias	$16,$sp
	lw	$9, 0($16)
	ble	$9, 0, $85
	.alias	$16,$21
	.alias	$16,$sp
	lw	$10, 304($sp)
	mul	$11, $10, 4
	la	$13, D_803BA050
	addu	$12, $11, $13
	sw	$12, 156($sp)
	move	$23, $0
	mul	$14, $19, 2816
	la	$24, D_803AF9A8
	addu	$15, $14, $24
	sw	$15, 132($sp)
	mul	$25, $19, 76
	la	$9, D_803BA060
	addu	$30, $25, $9
$51:
	.loc	2 180
	.loc	2 181
 # 181	        s32 item = D_803BA060[p][i];
	.noalias	$30,$21
	.noalias	$30,$sp
	lw	$20, 0($30)
	.loc	2 182
 # 182	        if ((D_803AF9A8[p][item].angle > 0.6981317f && D_803AF9A8[p][item].angle < 2.443461f) || !D_803AF9A8[p][item].alpha) continue;
	.noalias	$22,$21
	.noalias	$22,$30
	.noalias	$22,$sp
	lw	$10, 132($sp)
	mul	$11, $20, 64
	addu	$22, $10, $11
	l.s	$f0, 8($22)
	c.lt.s	$f26, $f0
	bc1f	$52
	c.lt.s	$f0, $f28
	bc1t	$53
$52:
	lbu	$2, 60($22)
	bne	$2, 0, $54
	.alias	$22,$21
	.alias	$22,$30
	.alias	$22,$sp
$53:
	.loc	2 182
	lw	$13, 156($sp)
	lw	$2, 0($13)
	mul	$3, $2, 4
	b	$84
$54:
	.loc	2 183
 # 183	        func_800ED66C(D_803AF9A8[p][item].alpha);
	mtc1	$2, $f8
	cvt.s.w	$f12, $f8
	bge	$2, $0, $89
	li.s	$f10, 4294967296.0
	add.s	$f12, $f12, $f10
$89:
	li.s	$f16, 45.0
	mul.s	$f18, $f16, $f22
	li.s	$f4, 300.0
	mul.s	$f20, $f18, $f4
	.livereg	0x0000000E,0x00080800
	jal	func_800ED66C
	.loc	2 184
 # 184	        if (D_803AF9A8[p][item].angle < 1.5707964f) {
	.noalias	$22,$21
	.noalias	$22,$30
	.noalias	$22,$sp
	l.s	$f6, 8($22)
	c.lt.s	$f6, $f30
	bc1f	$55
	.loc	2 184
	.loc	2 185
 # 185	            func_800BEA3C(D_801146BC, D_801146C0);
	lw	$12, D_801146BC
	sw	$12, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 186
 # 186	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$56
$55:
	.loc	2 187
 # 187	        } else {
	.loc	2 188
 # 188	            func_800BEA3C(D_801146BC, D_801146C0);
	lw	$24, D_801146BC
	sw	$24, 0($sp)
	lw	$15, D_801146C0
	sw	$15, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 189
 # 189	            dispatch_handler(22);
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$56:
	.loc	2 191
 # 190	        }
 # 191	        func_800F6928(D_803AF9A8[p][item].position[2] - 10.0f);
	l.s	$f8, 56($22)
	li.s	$f10, 10.0
	sub.s	$f12, $f8, $f10
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 192
 # 192	        brake_light_update(p, D_803AF9A8[p][item].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	addu	$5, $22, 48
	lw	$6, 128($sp)
	move	$7, $0
	addu	$25, $sp, 216
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 193
 # 193	        x = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_803B25C0;
	lh	$9, 216($sp)
	mtc1	$9, $f16
	cvt.s.w	$f18, $f16
	l.s	$f4, D_803B25C0
	div.s	$f6, $f20, $f4
	sub.s	$f8, $f18, $f6
	trunc.w.s	$f10, $f8, $10
	mfc1	$17, $f10
	.loc	2 194
 # 194	        y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 195
 # 195	        str = D_80151AD0 == 1 ? countdown_state.indexed[countdown_state.indices[2] + item]
	lh	$11, D_80151AD0
	bne	$11, 1, $57
	lw	$13, 16($21)
	lw	$12, 12($21)
	lhu	$14, 4($12)
	mul	$24, $14, 4
	addu	$15, $13, $24
	mul	$25, $20, 4
	addu	$9, $15, $25
	lw	$16, 0($9)
	b	$58
$57:
	lw	$10, 16($21)
	lw	$11, 12($21)
	lhu	$12, 6($11)
	mul	$14, $12, 4
	addu	$13, $10, $14
	mul	$24, $20, 4
	addu	$15, $13, $24
	lw	$16, 0($15)
$58:
	.loc	2 197
 # 196	                              : countdown_state.indexed[countdown_state.indices[3] + item];
 # 197	        state_utility(x, y, str);
	sll	$4, $17, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$9, $5, 16
	move	$5, $9
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 198
 # 198	        switch (item) {
	bgeu	$20, 19, $81
	sll	$11, $20, 2
	lw	$11, $59($11)
	j	$11
	.rdata	
$59:
	.word	$61
	.word	$62
	.word	$63
	.word	$64
	.word	$65
	.word	$66
	.word	$67
	.word	$81
	.word	$81
	.word	$81
	.word	$68
	.word	$69
	.word	$70
	.word	$81
	.word	$81
	.word	$71
	.word	$72
	.word	$75
	.word	$78
	.text	
$61:
	.loc	2 199
 # 199	        case 0: str = D_80110D70[D_803B9FD0[p]]; break;
	lb	$12, D_803B9FD0($19)
	mul	$10, $12, 4
	lw	$16, D_80110D70($10)
	.loc	2 199
	b	$82
$62:
	.loc	2 200
 # 200	        case 1: str = countdown_state.indexed[countdown_state.indices[4] + D_80110E78[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$13, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$15, $13, $24
	lb	$25, D_80110E78+13($15)
	mul	$9, $25, 4
	addu	$11, $14, $9
	lw	$12, 12($21)
	lhu	$10, 8($12)
	mul	$13, $10, 4
	addu	$24, $11, $13
	lw	$16, 0($24)
	.loc	2 200
	b	$82
$63:
	.loc	2 201
 # 201	        case 2: str = countdown_state.indexed[countdown_state.indices[5] + D_8011103C[p+1][D_803B9FD0[p]]]; break;
	lw	$15, 16($21)
	mul	$25, $19, 13
	lb	$14, D_803B9FD0($19)
	addu	$9, $25, $14
	lb	$12, D_8011103C+13($9)
	mul	$10, $12, 4
	addu	$11, $15, $10
	lw	$13, 12($21)
	lhu	$24, 10($13)
	mul	$25, $24, 4
	addu	$14, $11, $25
	lw	$16, 0($14)
	.loc	2 201
	b	$82
$64:
	.loc	2 202
 # 202	        case 3: str = countdown_state.indexed[countdown_state.indices[6] + D_80111080[p+1][D_803B9FD0[p]]]; break;
	lw	$9, 16($21)
	mul	$12, $19, 13
	lb	$15, D_803B9FD0($19)
	addu	$10, $12, $15
	lb	$13, D_80111080+13($10)
	mul	$24, $13, 4
	addu	$11, $9, $24
	lw	$25, 12($21)
	lhu	$14, 12($25)
	mul	$12, $14, 4
	addu	$15, $11, $12
	lw	$16, 0($15)
	.loc	2 202
	b	$82
$65:
	.loc	2 203
 # 203	        case 4: str = countdown_state.indexed[countdown_state.indices[7] + D_8011119C[p+1][D_803B9FD0[p]]]; break;
	lw	$10, 16($21)
	mul	$13, $19, 13
	lb	$9, D_803B9FD0($19)
	addu	$24, $13, $9
	lb	$25, D_8011119C+13($24)
	mul	$14, $25, 4
	addu	$11, $10, $14
	lw	$12, 12($21)
	lhu	$15, 14($12)
	mul	$13, $15, 4
	addu	$9, $11, $13
	lw	$16, 0($9)
	.loc	2 203
	b	$82
$66:
	.loc	2 204
 # 204	        case 5: str = countdown_state.indexed[countdown_state.indices[8] + D_80111230[p+1][D_803B9FD0[p]]]; break;
	lw	$24, 16($21)
	mul	$25, $19, 13
	lb	$10, D_803B9FD0($19)
	addu	$14, $25, $10
	lb	$12, D_80111230+13($14)
	mul	$15, $12, 4
	addu	$11, $24, $15
	lw	$13, 12($21)
	lhu	$9, 16($13)
	mul	$25, $9, 4
	addu	$10, $11, $25
	lw	$16, 0($10)
	.loc	2 204
	b	$82
$67:
	.loc	2 205
 # 205	        case 6: str = countdown_state.indexed[countdown_state.indices[9] + D_8011128C[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$12, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$15, $12, $24
	lb	$13, D_8011128C+13($15)
	mul	$9, $13, 4
	addu	$11, $14, $9
	lw	$25, 12($21)
	lhu	$10, 18($25)
	mul	$12, $10, 4
	addu	$24, $11, $12
	lw	$16, 0($24)
	.loc	2 205
	b	$82
$68:
	.loc	2 206
 # 206	        case 10: str = countdown_state.indexed[countdown_state.indices[12] + D_8012E67C[p]]; break;
	lw	$15, 16($21)
	lb	$13, D_8012E67C($19)
	mul	$14, $13, 4
	addu	$9, $15, $14
	lw	$25, 12($21)
	lhu	$10, 24($25)
	mul	$11, $10, 4
	addu	$12, $9, $11
	lw	$16, 0($12)
	.loc	2 206
	b	$82
$69:
	.loc	2 207
 # 207	        case 11: str = countdown_state.indexed[countdown_state.indices[10] + D_8011157C[p+1][D_803B9FD0[p]]]; break;
	lw	$24, 16($21)
	mul	$13, $19, 13
	lb	$15, D_803B9FD0($19)
	addu	$14, $13, $15
	lb	$25, D_8011157C+13($14)
	mul	$10, $25, 4
	addu	$9, $24, $10
	lw	$11, 12($21)
	lhu	$12, 20($11)
	mul	$13, $12, 4
	addu	$15, $9, $13
	lw	$16, 0($15)
	.loc	2 207
	b	$82
$70:
	.loc	2 208
 # 208	        case 12: str = countdown_state.indexed[countdown_state.indices[11] + D_801115C0[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$25, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$10, $25, $24
	lb	$11, D_801115C0+13($10)
	mul	$12, $11, 4
	addu	$9, $14, $12
	lw	$13, 12($21)
	lhu	$15, 22($13)
	mul	$25, $15, 4
	addu	$24, $9, $25
	lw	$16, 0($24)
	.loc	2 208
	b	$82
$71:
	.loc	2 209
 # 209	        case 15: fcvt_wrapper(str1, "%d", D_803B28B0[p] + 1); str = str1; break;
	addu	$4, $sp, 220
	la	$5, $$54
	mul	$10, $19, 2
	lh	$6, D_803B28B0($10)
	addu	$6, $6, 1
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 209
	addu	$16, $sp, 220
	.loc	2 209
	b	$82
$72:
	.loc	2 211
 # 210	        case 16:
 # 211	            if (D_80152698[1]) func_800BE6A4(str1, (*D_80152698[1])->name);
	lw	$2, D_80152698+4
	beq	$2, 0, $73
	.loc	2 211
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$74
$73:
	.loc	2 212
 # 212	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$11, 16($21)
	lw	$14, 12($21)
	lhu	$12, 30($14)
	mul	$13, $12, 4
	addu	$15, $11, $13
	lw	$5, 0($15)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$74:
	.loc	2 213
 # 213	            str = str1;
	addu	$16, $sp, 220
	.loc	2 214
 # 214	            break;
	b	$82
$75:
	.loc	2 216
 # 215	        case 17:
 # 216	            if (D_80152698[2]) func_800BE6A4(str1, (*D_80152698[2])->name);
	lw	$2, D_80152698+8
	beq	$2, 0, $76
	.loc	2 216
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$77
$76:
	.loc	2 217
 # 217	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$9, 16($21)
	lw	$25, 12($21)
	lhu	$24, 30($25)
	mul	$10, $24, 4
	addu	$14, $9, $10
	lw	$5, 0($14)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$77:
	.loc	2 218
 # 218	            str = str1;
	addu	$16, $sp, 220
	.loc	2 219
 # 219	            break;
	b	$82
$78:
	.loc	2 221
 # 220	        case 18:
 # 221	            if (D_80152698[3]) func_800BE6A4(str1, (*D_80152698[3])->name);
	lw	$2, D_80152698+12
	beq	$2, 0, $79
	.loc	2 221
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$80
$79:
	.loc	2 222
 # 222	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$12, 16($21)
	lw	$11, 12($21)
	lhu	$13, 30($11)
	mul	$15, $13, 4
	addu	$25, $12, $15
	lw	$5, 0($25)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$80:
	.loc	2 223
 # 223	            str = str1;
	addu	$16, $sp, 220
	.loc	2 224
 # 224	            break;
	b	$82
$81:
	.loc	2 225
 # 225	        default: str = 0; break;
	move	$16, $0
	.loc	2 225
$82:
	.loc	2 227
 # 226	        }
 # 227	        if (str) {
	beq	$16, 0, $83
	.loc	2 227
	.loc	2 228
 # 228	            brake_light_update(p, D_803AF9A8[p][item+19].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	addu	$5, $22, 1264
	lw	$6, 128($sp)
	move	$7, $0
	addu	$24, $sp, 216
	sw	$24, 16($sp)
	li.s	$f16, 50.0
	mul.s	$f4, $f16, $f22
	li.s	$f18, 300.0
	mul.s	$f20, $f4, $f18
	.livereg	0x0F00000E,0x00000800
	jal	brake_light_update
	.alias	$22,$21
	.alias	$22,$30
	.alias	$22,$sp
	.loc	2 229
 # 229	            x = screenPos[0] - 50.0f * gTextPosScale * 300.0f / D_803B25C0;
	lh	$9, 216($sp)
	mtc1	$9, $f6
	cvt.s.w	$f8, $f6
	l.s	$f10, D_803B25C0
	div.s	$f16, $f20, $f10
	sub.s	$f4, $f8, $f16
	trunc.w.s	$f18, $f4, $10
	mfc1	$17, $f18
	.loc	2 230
 # 230	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 231
 # 231	            state_utility(x, y, str);
	sll	$4, $17, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$11, $5, 16
	move	$5, $11
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$83:
	.loc	2 180
 # 180	    for (i = 0; i < D_803BA050[p]; i++) {
	lw	$13, 156($sp)
	lw	$2, 0($13)
	mul	$3, $2, 4
$84:
	addu	$23, $23, 4
	addu	$30, $30, 4
	blt	$23, $3, $51
	.alias	$30,$21
	.alias	$30,$sp
$85:
	.loc	2 234
 # 234	    func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 235
 # 235	    func_800ED66C(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 236
 # 236	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 237
 # 237	}
$86:
	lh	$3, D_8014A108
	lw	$2, 148($sp)
	.loc	2 88
 #  88	        else drawOptions(i);
	.loc	2 240
 # 240	{
	.loc	2 241
 # 241	}
$87:
	.loc	2 86
 #  86	    for (i = 0; i < D_8014A108; i++) {
	addu	$19, $19, 1
	lw	$12, 152($sp)
	addu	$15, $12, 4
	sw	$15, 152($sp)
	addu	$2, $2, 4
	blt	$19, $3, $32
	.alias	$21,$sp
$88:
	.loc	2 90
 #  87	        if (D_803B9B90[i] == 1) animButtonText(i);
 #  88	        else drawOptions(i);
 #  89	    }
 #  90	    return 1;
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
	.end	func_803A44FC
