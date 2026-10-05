	.ent	func_800E95DC 2
func_800E95DC:
	.option	O3
	subu	$sp, 232
	sw	$31, 100($sp)
	sw	$23, 96($sp)
	sw	$22, 92($sp)
	sw	$21, 88($sp)
	sw	$20, 84($sp)
	sw	$19, 80($sp)
	sw	$18, 76($sp)
	sw	$17, 72($sp)
	sw	$16, 68($sp)
	s.d	$f30, 56($sp)
	s.d	$f28, 48($sp)
	s.d	$f26, 40($sp)
	s.d	$f24, 32($sp)
	s.d	$f22, 24($sp)
	s.d	$f20, 16($sp)
	.mask	0x80FF0000, -132
	.fmask	0xFFF00000, -176
	.frame	$sp, 232, $31
	.loc	2 3819
	sw	$4, 232($sp)
	sll	$14, $4, 16
	move	$4, $14
	sra	$15, $4, 16
	move	$4, $15
	move	$11, $5
	move	$10, $6
	sw	$7, 244($sp)
	.loc	2 3819
	.loc	2 3824
 #3820	    V3 delta[4];
 #3821	    f32 res[3], pos2[3];
 #3822	    s32 pl;
 #3823	
 #3824	    pl = car->player;
	lb	$23, 860($11)
	.loc	2 3825
 #3825	    if (mode == 0) {
	bne	$4, 0, $51
	li	$31, 12
	.loc	2 3825
	.loc	2 3826
 #3826	        D_8012E690[pl].x = pos[0];
	.noalias	$3,$sp
	mul	$24, $23, $31
	la	$25, D_8012E690
	addu	$3, $24, $25
	l.s	$f4, 0($10)
	s.s	$f4, 0($3)
	.loc	2 3827
 #3827	        D_8012E690[pl].y = pos[1];
	l.s	$f6, 4($10)
	s.s	$f6, 4($3)
	.loc	2 3828
 #3828	        D_8012E690[pl].z = pos[2];
	l.s	$f8, 8($10)
	s.s	$f8, 8($3)
	.loc	2 3829
 #3829	        D_80150B70[pl].v84[0] = pos[0];
	.noalias	$2,$3
	.noalias	$2,$sp
	mul	$14, $23, 152
	la	$15, D_80150B70
	addu	$2, $14, $15
	l.s	$f10, 0($10)
	s.s	$f10, 132($2)
	.loc	2 3830
 #3830	        D_80150B70[pl].v84[1] = pos[1];
	l.s	$f4, 4($10)
	s.s	$f4, 136($2)
	.loc	2 3831
 #3831	        D_80150B70[pl].v84[2] = pos[2];
	l.s	$f6, 8($10)
	s.s	$f6, 140($2)
	.loc	2 3832
 #3832	        res[0] = car->pos[0];
	l.s	$f8, 8($11)
	s.s	$f8, 172($sp)
	.loc	2 3833
 #3833	        res[1] = car->pos[1];
	l.s	$f10, 12($11)
	s.s	$f10, 176($sp)
	.loc	2 3834
 #3834	        res[2] = car->pos[2];
	l.s	$f4, 16($11)
	s.s	$f4, 180($sp)
	.loc	2 3835
 #3835	        pos[0] = pos[0] - res[0];
	l.s	$f6, 0($10)
	l.s	$f8, 172($sp)
	sub.s	$f10, $f6, $f8
	s.s	$f10, 0($10)
	.loc	2 3836
 #3836	        pos[1] = pos[1] - res[1];
	l.s	$f4, 4($10)
	l.s	$f6, 176($sp)
	sub.s	$f8, $f4, $f6
	s.s	$f8, 4($10)
	.loc	2 3837
 #3837	        pos[2] = pos[2] - res[2];
	l.s	$f10, 8($10)
	l.s	$f4, 180($sp)
	sub.s	$f6, $f10, $f4
	s.s	$f6, 8($10)
	.alias	$3,$2
	.alias	$3,$sp
	.loc	2 3838
 #3838	        vector_normalize_length(pos, &D_80150B70[pl].n60);
	move	$4, $10
	addu	$5, $2, 96
	.livereg	0x0C00000E,0x00000000
	jal	vector_normalize_length
	.alias	$2,$sp
	.loc	2 3839
 #3839	        D_8012E6C8[pl] = 0;
	mul	$24, $23, 2
	sh	$0, D_8012E6C8($24)
	.loc	2 3840
 #3840	        D_8012E6E8[pl] = 0.0f;
	li.s	$f8, 0.0
	mul	$25, $23, 4
	s.s	$f8, D_8012E6E8($25)
	.loc	2 3841
 #3841	        return;
	b	$60
$51:
	.loc	2 3843
 #3842	    }
 #3843	    switch (D_8012E6C8[pl]) {
	.noalias	$13,$sp
	mul	$14, $23, 2
	la	$15, D_8012E6C8
	addu	$13, $14, $15
	lh	$4, 0($13)
	move	$2, $4
	beq	$2, 0, $52
	beq	$2, 1, $53
	beq	$2, 2, $54
	li.s	$f0, 0.0000000000000000e+00
	li	$31, 12
	b	$58
$52:
	.loc	2 3845
 #3844	    case 0:
 #3845	        func_800E92C8(car, D_801526F8[pl], D_801526E0[pl], res);
	move	$16, $11
	mul	$8, $23, 4
	l.s	$f22, D_801526F8($8)
	l.s	$f24, D_801526E0($8)
	addu	$20, $sp, 172
	sw	$8, 140($sp)
	sw	$10, 240($sp)
	sw	$11, 236($sp)
	sw	$13, 136($sp)
	.livereg	0x0080880E,0x00000280
	jal	func_800E92C8
	lw	$8, 140($sp)
	lw	$10, 240($sp)
	lw	$11, 236($sp)
	lw	$13, 136($sp)
	li	$31, 12
	.loc	2 3846
 #3846	        res[0] = pos[0] + res[0];
	l.s	$f10, 172($sp)
	l.s	$f4, 0($10)
	add.s	$f6, $f10, $f4
	s.s	$f6, 172($sp)
	.loc	2 3847
 #3847	        res[1] = pos[1] + res[1];
	l.s	$f8, 176($sp)
	l.s	$f10, 4($10)
	add.s	$f4, $f8, $f10
	s.s	$f4, 176($sp)
	.loc	2 3848
 #3848	        res[2] = pos[2] + res[2];
	l.s	$f8, 180($sp)
	l.s	$f10, 8($10)
	add.s	$f4, $f8, $f10
	s.s	$f4, 180($sp)
	.loc	2 3849
 #3849	        delta[pl].x = (res[0] - D_8012E690[pl].x) * (.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	mul	$9, $23, $31
	.noalias	$2,$13
	.noalias	$2,$gp
	addu	$24, $sp, 184
	addu	$2, $9, $24
	l.s	$f2, D_8012E6E8($8)
	lh	$4, 0($13)
	mul	$25, $4, 4
	l.s	$f12, D_801108C8($25)
	li.s	$f8, .1
	mul.s	$f10, $f8, $f2
	div.s	$f0, $f10, $f12
	.noalias	$3,$2
	.noalias	$3,$13
	.noalias	$3,$sp
	la	$14, D_8012E690
	addu	$3, $9, $14
	l.s	$f14, 0($3)
	sub.s	$f4, $f6, $f14
	mul.s	$f8, $f0, $f4
	s.s	$f8, 0($2)
	.loc	2 3850
 #3850	        delta[pl].y = (res[1] - D_8012E690[pl].y) * (.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	l.s	$f16, 4($3)
	l.s	$f10, 176($sp)
	sub.s	$f6, $f10, $f16
	mul.s	$f4, $f0, $f6
	s.s	$f4, 4($2)
	.loc	2 3851
 #3851	        delta[pl].z = (res[2] - D_8012E690[pl].z) * (.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	l.s	$f18, 8($3)
	l.s	$f8, 180($sp)
	sub.s	$f10, $f8, $f18
	mul.s	$f6, $f0, $f10
	s.s	$f6, 8($2)
	.loc	2 3852
 #3852	        pos2[0] = car->pos[0] - car->m[6] * 40.0f;
	l.s	$f4, 68($11)
	li.s	$f8, 40.0
	mul.s	$f10, $f4, $f8
	l.s	$f6, 8($11)
	sub.s	$f4, $f6, $f10
	s.s	$f4, 160($sp)
	.loc	2 3853
 #3853	        pos2[1] = car->pos[1] + 5.0f;
	l.s	$f8, 12($11)
	li.s	$f6, 5.0
	add.s	$f10, $f8, $f6
	s.s	$f10, 164($sp)
	.loc	2 3854
 #3854	        pos2[2] = car->pos[2] - car->m[8] * 40.0f;
	l.s	$f4, 76($11)
	li.s	$f8, 40.0
	mul.s	$f6, $f4, $f8
	l.s	$f10, 16($11)
	sub.s	$f4, $f10, $f6
	s.s	$f4, 168($sp)
	.alias	$3,$2
	.alias	$3,$13
	.alias	$3,$sp
	.loc	2 3855
 #3855	        delta[pl].x = (pos2[0] - D_8012E690[pl].x) * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	div.s	$f0, $f2, $f12
	l.s	$f8, 160($sp)
	sub.s	$f10, $f8, $f14
	mul.s	$f6, $f0, $f10
	s.s	$f6, 0($2)
	.loc	2 3856
 #3856	        delta[pl].y = (pos2[1] - D_8012E690[pl].y) * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	l.s	$f4, 164($sp)
	sub.s	$f8, $f4, $f16
	mul.s	$f10, $f0, $f8
	s.s	$f10, 4($2)
	.loc	2 3857
 #3857	        delta[pl].z = (pos2[2] - D_8012E690[pl].z) * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	l.s	$f6, 168($sp)
	sub.s	$f4, $f6, $f18
	mul.s	$f8, $f0, $f4
	s.s	$f8, 8($2)
	.loc	2 3858
 #3858	        break;
	li.s	$f0, 0.0000000000000000e+00
	b	$58
	.alias	$2,$13
	.alias	$2,$gp
$53:
	.loc	2 3860
 #3859	    case 1:
 #3860	        func_800E92C8(car, D_801526F8[pl], D_801526E0[pl], res);
	move	$16, $11
	mul	$8, $23, 4
	l.s	$f22, D_801526F8($8)
	l.s	$f24, D_801526E0($8)
	addu	$20, $sp, 172
	sw	$8, 140($sp)
	sw	$10, 240($sp)
	sw	$11, 236($sp)
	sw	$13, 136($sp)
	.livereg	0x0080880E,0x00000280
	jal	func_800E92C8
	lw	$8, 140($sp)
	lw	$10, 240($sp)
	lw	$11, 236($sp)
	lw	$13, 136($sp)
	li	$31, 12
	.loc	2 3861
 #3861	        res[0] = pos[0] + res[0];
	l.s	$f10, 172($sp)
	l.s	$f6, 0($10)
	add.s	$f4, $f10, $f6
	s.s	$f4, 172($sp)
	.loc	2 3862
 #3862	        res[1] = pos[1] + res[1];
	l.s	$f8, 176($sp)
	l.s	$f10, 4($10)
	add.s	$f6, $f8, $f10
	s.s	$f6, 176($sp)
	.loc	2 3863
 #3863	        res[2] = pos[2] + res[2];
	l.s	$f8, 180($sp)
	l.s	$f10, 8($10)
	add.s	$f6, $f8, $f10
	s.s	$f6, 180($sp)
	.loc	2 3864
 #3864	        delta[pl].x = (res[0] - D_8012E690[pl].x) * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	mul	$9, $23, $31
	.noalias	$2,$13
	.noalias	$2,$gp
	addu	$15, $sp, 184
	addu	$2, $9, $15
	lh	$4, 0($13)
	l.s	$f8, D_8012E6E8($8)
	mul	$24, $4, 4
	l.s	$f10, D_801108C8($24)
	div.s	$f0, $f8, $f10
	.noalias	$3,$2
	.noalias	$3,$13
	.noalias	$3,$sp
	la	$25, D_8012E690
	addu	$3, $9, $25
	l.s	$f6, 0($3)
	sub.s	$f8, $f4, $f6
	mul.s	$f10, $f0, $f8
	s.s	$f10, 0($2)
	.loc	2 3865
 #3865	        delta[pl].y = (res[1] - D_8012E690[pl].y) * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	l.s	$f4, 176($sp)
	l.s	$f6, 4($3)
	sub.s	$f8, $f4, $f6
	mul.s	$f10, $f0, $f8
	s.s	$f10, 4($2)
	.loc	2 3866
 #3866	        delta[pl].z = (res[2] - D_8012E690[pl].z) * (D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]);
	l.s	$f4, 180($sp)
	l.s	$f6, 8($3)
	sub.s	$f8, $f4, $f6
	mul.s	$f10, $f0, $f8
	s.s	$f10, 8($2)
	.loc	2 3867
 #3867	        break;
	li.s	$f0, 0.0000000000000000e+00
	b	$58
	.alias	$2,$3
	.alias	$2,$13
	.alias	$2,$gp
	.alias	$3,$13
	.alias	$3,$sp
$54:
	.loc	2 3869
 #3868	    case 2:
 #3869	        func_800E9234(car);
	move	$4, $11
	sw	$10, 240($sp)
	sw	$11, 236($sp)
	sw	$13, 136($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_800E9234
	lw	$10, 240($sp)
	lw	$11, 236($sp)
	lw	$13, 136($sp)
	.loc	2 3870
 #3870	        if (!(state_word_a & 8)) {
	lw	$14, state_word_a
	and	$15, $14, 8
	bne	$15, 0, $57
	.loc	2 3870
	.loc	2 3872
 #3871	            s32 v;
 #3872	            if (car->q->h[0]->f2c != 0)
	lw	$24, 896($11)
	lw	$4, 72($24)
	lw	$25, 0($4)
	lw	$14, 44($25)
	beq	$14, 0, $55
	.loc	2 3873
 #3873	                v = func_800CDE38(car->q->h);
	sw	$10, 240($sp)
	sw	$11, 236($sp)
	sw	$13, 136($sp)
	.livereg	0x0800000E,0x00000000
	jal	func_800CDE38
	lw	$10, 240($sp)
	lw	$11, 236($sp)
	lw	$13, 136($sp)
	move	$3, $2
	b	$56
$55:
	.loc	2 3875
 #3874	            else
 #3875	                v = 2;
	li	$3, 2
$56:
	.loc	2 3876
 #3876	            car->mode = v;
	sb	$3, 861($11)
	.loc	2 3877
 #3877	            car->mode2 = v;
	sb	$3, 862($11)
	.loc	2 3878
 #3878	            if (car->mode == 1) {
	lb	$15, 861($11)
	bne	$15, 1, $57
	la	$2, D_801526A8
	li	$31, 12
	li.s	$f0, 0.0000000000000000e+00
	.loc	2 3878
	.loc	2 3879
 #3879	                D_801526A8[car->player].x = 0;
	.noalias	$2,$13
	.noalias	$2,$sp
	lb	$24, 860($11)
	mul	$25, $24, $31
	addu	$14, $2, $25
	.noalias	$14,$13
	.noalias	$14,$sp
	s.s	$f0, 0($14)
	.alias	$14,$13
	.alias	$14,$sp
	.loc	2 3880
 #3880	                D_801526A8[car->player].y = 0;
	lb	$15, 860($11)
	mul	$24, $15, $31
	addu	$25, $2, $24
	.noalias	$25,$13
	.noalias	$25,$sp
	s.s	$f0, 4($25)
	.alias	$25,$13
	.alias	$25,$sp
	.loc	2 3881
 #3881	                D_801526A8[car->player].z = 0;
	lb	$14, 860($11)
	mul	$15, $14, $31
	addu	$24, $2, $15
	.noalias	$24,$13
	.noalias	$24,$sp
	s.s	$f0, 8($24)
	.alias	$24,$13
	.alias	$24,$sp
	.alias	$2,$13
	.alias	$2,$sp
$57:
	li	$31, 12
	li.s	$f0, 0.0000000000000000e+00
	.loc	2 3884
 #3882	            }
 #3883	        }
 #3884	        break;
	lh	$4, 0($13)
$58:
	.loc	2 3886
 #3885	    }
 #3886	    if (D_8012E6C8[pl] == 0 || D_8012E6C8[pl] == 1) {
	beq	$4, 0, $59
	bne	$4, 1, $60
$59:
	.loc	2 3886
	.loc	2 3887
 #3887	        pos2[0] = D_8012E690[pl].x + delta[pl].x;
	mul	$9, $23, $31
	.noalias	$2,$13
	.noalias	$2,$gp
	addu	$25, $sp, 184
	addu	$2, $9, $25
	.noalias	$3,$2
	.noalias	$3,$13
	.noalias	$3,$sp
	la	$14, D_8012E690
	addu	$3, $9, $14
	l.s	$f4, 0($2)
	l.s	$f6, 0($3)
	add.s	$f8, $f4, $f6
	s.s	$f8, 160($sp)
	.loc	2 3888
 #3888	        pos2[1] = D_8012E690[pl].y + delta[pl].y;
	l.s	$f10, 4($2)
	l.s	$f4, 4($3)
	add.s	$f6, $f10, $f4
	s.s	$f6, 164($sp)
	.loc	2 3889
 #3889	        pos2[2] = D_8012E690[pl].z + delta[pl].z;
	l.s	$f8, 8($2)
	l.s	$f10, 8($3)
	add.s	$f4, $f8, $f10
	s.s	$f4, 168($sp)
	.loc	2 3890
 #3890	        res[0] = pos2[0] - pos[0];
	l.s	$f6, 160($sp)
	l.s	$f8, 0($10)
	sub.s	$f10, $f6, $f8
	s.s	$f10, 172($sp)
	.loc	2 3891
 #3891	        res[1] = pos2[1] - pos[1];
	l.s	$f4, 164($sp)
	l.s	$f6, 4($10)
	sub.s	$f8, $f4, $f6
	s.s	$f8, 176($sp)
	.loc	2 3892
 #3892	        res[2] = pos2[2] - pos[2];
	l.s	$f10, 168($sp)
	l.s	$f4, 8($10)
	sub.s	$f6, $f10, $f4
	s.s	$f6, 180($sp)
	.loc	2 3893
 #3893	        D_80152720[pl] = 0;
	mul	$8, $23, 4
	s.s	$f0, D_80152720($8)
	.loc	2 3894
 #3894	        func_800E8D50(car, pos, uvs, res);
	move	$4, $11
	move	$5, $10
	lw	$6, 244($sp)
	addu	$7, $sp, 172
	la	$15, D_8012E6E8
	addu	$12, $8, $15
	sw	$3, 144($sp)
	sw	$12, 132($sp)
	sw	$13, 136($sp)
	.livereg	0x0F08000E,0x00000000
	jal	func_800E8D50
	.alias	$2,$3
	.alias	$2,$13
	.alias	$2,$gp
	lw	$3, 144($sp)
	lw	$12, 132($sp)
	lw	$13, 136($sp)
	.loc	2 3895
 #3895	        D_80150B70[pl].v84[1] += 4.0f;
	.noalias	$2,$3
	.noalias	$2,$13
	.noalias	$2,$sp
	mul	$24, $23, 152
	la	$25, D_80150B70
	addu	$2, $24, $25
	l.s	$f8, 136($2)
	li.s	$f10, 4.0
	add.s	$f4, $f8, $f10
	s.s	$f4, 136($2)
	.loc	2 3896
 #3896	        D_8012E6E8[pl] += D_8002EB94;
	.noalias	$12,$2
	.noalias	$12,$3
	.noalias	$12,$13
	.noalias	$12,$sp
	l.s	$f6, 0($12)
	la	$14, D_8002EB94
	.set	 volatile
	l.s	$f8, 0($14)
	.set	 novolatile
	add.s	$f10, $f6, $f8
	s.s	$f10, 0($12)
	.loc	2 3897
 #3897	        if (D_801108C8[D_8012E6C8[pl]] <= D_8012E6E8[pl]) {
	lh	$4, 0($13)
	l.s	$f4, 0($12)
	mul	$15, $4, 4
	l.s	$f6, D_801108C8($15)
	c.le.s	$f6, $f4
	bc1f	$60
	.loc	2 3897
	.loc	2 3898
 #3898	            D_8012E6C8[pl]++;
	addu	$24, $4, 1
	sh	$24, 0($13)
	.loc	2 3899
 #3899	            D_8012E6E8[pl] = 0.0f;
	li.s	$f8, 0.0
	s.s	$f8, 0($12)
	.loc	2 3900
 #3900	            D_8012E690[pl].x = D_80150B70[pl].v84[0];
	l.s	$f10, 132($2)
	s.s	$f10, 0($3)
	.loc	2 3901
 #3901	            D_8012E690[pl].y = D_80150B70[pl].v84[1];
	l.s	$f4, 136($2)
	s.s	$f4, 4($3)
	.loc	2 3902
 #3902	            D_8012E690[pl].z = D_80150B70[pl].v84[2];
	l.s	$f6, 140($2)
	s.s	$f6, 8($3)
	.alias	$2,$3
	.alias	$2,$12
	.alias	$2,$13
	.alias	$2,$sp
	.alias	$3,$12
	.alias	$3,$13
	.alias	$3,$sp
	.alias	$12,$13
	.alias	$12,$sp
	.alias	$13,$sp
	.loc	2 3905
 #3903	        }
 #3904	    }
 #3905	}
$60:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 16($sp)
	l.d	$f22, 24($sp)
	l.d	$f24, 32($sp)
	l.d	$f26, 40($sp)
	l.d	$f28, 48($sp)
	l.d	$f30, 56($sp)
	lw	$16, 68($sp)
	lw	$17, 72($sp)
	lw	$18, 76($sp)
	lw	$19, 80($sp)
	lw	$20, 84($sp)
	lw	$21, 88($sp)
	lw	$22, 92($sp)
	lw	$23, 96($sp)
	lw	$31, 100($sp)
	addu	$sp, 232
	j	$31
	.end	func_800E95DC
