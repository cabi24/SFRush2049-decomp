	.verstamp	3 19
	.option	pic0
	.extern	state_word_a 4
	.extern	D_8002EB94 4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 3732
 #3732	void func_800E92C8(Cr *car, f32 ab, f32 AB, f32 *camoff) {
	.ent	func_800E92C8 2
func_800E92C8:
	.option	O3
	subu	$sp, 160
	sw	$31, 36($sp)
	.mask	0x80000000, -124
	.frame	$sp, 160, $31
	.loc	2 3732
	li	$21, 8
	.loc	2 3732
	.loc	2 3739
 #3739	    pl = car->player;
	lb	$19, 860($16)
	.loc	2 3740
 #3740	    if (car->mode == 8)
	lb	$14, 861($16)
	bne	$21, $14, $32
	.loc	2 3741
 #3741	        magvel = 0;
	li.s	$f20, 0.0000000000000000e+00
	b	$33
$32:
	.loc	2 3743
 #3742	    else
 #3743	        magvel = sqrtf(car->vx*car->vx + car->vz*car->vz);
	l.s	$f2, 20($16)
	l.s	$f12, 28($16)
	mul.s	$f4, $f2, $f2
	mul.s	$f6, $f12, $f12
	add.s	$f0, $f4, $f6
	sqrt.s	$f0, $f0
	mov.s	$f20, $f0
$33:
	li.s	$f26, 100.0
	.loc	2 3745
 #3744	
 #3745	    if (magvel <= MAX_VEL) {
	c.le.s	$f20, $f26
	bc1f	$36
	li.s	$f28, 0.0
	.loc	2 3745
	.loc	2 3746
 #3746	        vec[0] = 0.0f;
	s.s	$f28, 148($sp)
	.loc	2 3747
 #3747	        vec[1] = AB;
	s.s	$f24, 152($sp)
	.loc	2 3748
 #3748	        vec[2] = -ab;
	neg.s	$f8, $f22
	s.s	$f8, 156($sp)
	.loc	2 3749
 #3749	        if (car->mode == 8) {
	lb	$15, 861($16)
	bne	$21, $15, $34
	.loc	2 3749
	.loc	2 3750
 #3750	            temp[0] = vec[0];
	s.s	$f28, 124($sp)
	.loc	2 3751
 #3751	            temp[1] = vec[1];
	s.s	$f24, 128($sp)
	.loc	2 3752
 #3752	            temp[2] = vec[2];
	l.s	$f10, 156($sp)
	s.s	$f10, 132($sp)
	addu	$22, $sp, 148
	b	$35
$34:
	addu	$22, $sp, 148
	.loc	2 3754
 #3753	        } else
 #3754	            func_8009E820(vec, temp, car->m);
	move	$4, $22
	addu	$5, $sp, 124
	addu	$6, $16, 44
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
$35:
	.loc	2 3755
 #3755	        temp[1] = fabsf(temp[1]);
	l.s	$f0, 128($sp)
	abs.s	$f0, $f0
	s.s	$f0, 128($sp)
$36:
	addu	$22, $sp, 148
	li.s	$f28, 0.0
	li.s	$f30, 1.0000000000000000e+00
	.loc	2 3758
 #3756	    }
 #3757	
 #3758	    if (magvel > 1) {
	c.lt.s	$f30, $f20
	bc1f	$41
	.loc	2 3758
	.loc	2 3759
 #3759	        if (car->mode == 4) {
	addu	$17, $16, 44
	lb	$24, 861($16)
	bne	$24, 4, $37
	addu	$18, $sp, 136
	.loc	2 3759
	.loc	2 3760
 #3760	            vec[0] = car->f_ac;
	l.s	$f4, 172($16)
	s.s	$f4, 148($sp)
	.loc	2 3761
 #3761	            vec[1] = car->f_a8;
	l.s	$f6, 168($16)
	s.s	$f6, 152($sp)
	.loc	2 3762
 #3762	            vec[2] = 0.0f;
	s.s	$f28, 156($sp)
	.loc	2 3763
 #3763	            func_8009E820(vec, res, car->m);
	move	$4, $22
	move	$5, $18
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
	l.s	$f12, 136($sp)
	l.s	$f14, 144($sp)
	.loc	2 3764
 #3764	            res[0] = -res[0];
	neg.s	$f12, $f12
	.loc	2 3765
 #3765	            res[2] = -res[2];
	neg.s	$f14, $f14
	b	$39
$37:
	.loc	2 3766
 #3766	        } else {
	.loc	2 3767
 #3767	            res[0] = car->vx;
	l.s	$f8, 20($16)
	s.s	$f8, 136($sp)
	.loc	2 3768
 #3768	            res[1] = car->vy;
	l.s	$f10, 24($16)
	s.s	$f10, 140($sp)
	.loc	2 3769
 #3769	            res[2] = car->vz;
	l.s	$f4, 28($16)
	s.s	$f4, 144($sp)
	.loc	2 3770
 #3770	            if (ab < 0)
	li.s	$f6, 0.0000000000000000e+00
	c.lt.s	$f22, $f6
	bc1f	$38
	l.s	$f16, 140($sp)
	.loc	2 3771
 #3771	                res[1] = -res[1];
	neg.s	$f16, $f16
	s.s	$f16, 140($sp)
$38:
	addu	$18, $sp, 136
	.loc	2 3772
 #3772	            func_800A61B0(res, vec, car->m);
	move	$4, $18
	move	$5, $22
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	func_800A61B0
	.loc	2 3773
 #3773	            vec[2] = fabsf(vec[2]);
	l.s	$f0, 156($sp)
	abs.s	$f0, $f0
	s.s	$f0, 156($sp)
	.loc	2 3774
 #3774	            func_8009E820(vec, res, car->m);
	move	$4, $22
	move	$5, $18
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	func_8009E820
	l.s	$f12, 136($sp)
	l.s	$f14, 144($sp)
	.loc	2 3775
 #3775	            res[0] = -res[0];
	neg.s	$f12, $f12
	.loc	2 3776
 #3776	            res[2] = -res[2];
	neg.s	$f14, $f14
$39:
	l.s	$f16, 140($sp)
	.loc	2 3779
 #3777	        }
 #3778	
 #3779	        fact = ab / magvel;
	.loc	2 3780
 #3780	        res[0] *= fact;
	div.s	$f2, $f22, $f20
	mul.s	$f12, $f12, $f2
	.loc	2 3781
 #3781	        res[1] *= fact;
	mul.s	$f16, $f16, $f2
	.loc	2 3782
 #3782	        res[2] *= fact;
	mul.s	$f14, $f14, $f2
	.loc	2 3784
 #3783	
 #3784	        a = res[1];
	.loc	2 3785
 #3785	        b = sqrtf(res[0]*res[0] + res[2]*res[2]);
	mul.s	$f8, $f14, $f14
	mul.s	$f10, $f12, $f12
	add.s	$f0, $f8, $f10
	sqrt.s	$f0, $f0
	mov.s	$f18, $f0
	.loc	2 3787
 #3786	
 #3787	        if (b > .001f) {
	s.s	$f12, 136($sp)
	s.s	$f14, 144($sp)
	li.s	$f4, .001
	c.lt.s	$f4, $f0
	bc1f	$40
	.loc	2 3787
	.loc	2 3788
 #3788	            A = -a * AB / ab;
	.loc	2 3789
 #3789	            fact = (b - A) / b;
	.loc	2 3790
 #3790	            res[0] *= fact;
	neg.s	$f6, $f16
	mul.s	$f8, $f6, $f24
	div.s	$f10, $f8, $f22
	sub.s	$f4, $f0, $f10
	div.s	$f2, $f4, $f0
	mul.s	$f12, $f12, $f2
	.loc	2 3791
 #3791	            res[2] *= fact;
	mul.s	$f14, $f14, $f2
	s.s	$f12, 136($sp)
	s.s	$f14, 144($sp)
$40:
	.loc	2 3794
 #3792	        }
 #3793	
 #3794	        B = b * AB / ab;
	.loc	2 3795
 #3795	        res[1] = fabsf(B - res[1]);
	mul.s	$f6, $f18, $f24
	div.s	$f8, $f6, $f22
	sub.s	$f0, $f8, $f16
	abs.s	$f0, $f0
	mov.s	$f16, $f0
	.loc	2 3797
 #3796	
 #3797	        if (magvel < MAX_VEL)
	s.s	$f16, 140($sp)
	c.lt.s	$f20, $f26
	bc1f	$42
	.loc	2 3798
 #3798	            func_800CFDEC(temp, res, 3, 1, MAX_VEL, magvel, res);
	addu	$4, $sp, 124
	move	$5, $18
	li	$6, 3
	mfc1	$7, $f30
	s.s	$f26, 16($sp)
	s.s	$f20, 20($sp)
	sw	$18, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800CFDEC
	b	$42
$41:
	.loc	2 3799
 #3799	    } else {
	.loc	2 3800
 #3800	        res[0] = temp[0];
	l.s	$f12, 124($sp)
	.loc	2 3801
 #3801	        res[1] = temp[1];
	l.s	$f16, 128($sp)
	.loc	2 3802
 #3802	        res[2] = temp[2];
	l.s	$f14, 132($sp)
	s.s	$f12, 136($sp)
	s.s	$f14, 144($sp)
	s.s	$f16, 140($sp)
$42:
	.loc	2 3805
 #3803	    }
 #3804	
 #3805	    if (car->mode == 8)
	lb	$2, 861($16)
	bne	$21, $2, $43
	.loc	2 3806
 #3806	        D_80152720[pl] = 0.0f;
	mul	$25, $19, 4
	s.s	$f28, D_80152720($25)
	b	$46
$43:
	.loc	2 3807
 #3807	    else if (car->mode == 4)
	bne	$2, 4, $44
	.loc	2 3808
 #3808	        D_80152720[pl] = 1.0f - magvel / MAX_VEL;
	li.s	$f10, 1.0
	div.s	$f4, $f20, $f26
	sub.s	$f6, $f10, $f4
	mul	$14, $19, 4
	s.s	$f6, D_80152720($14)
	b	$46
$44:
	.loc	2 3809
 #3809	    else if (magvel < MAX_VEL)
	c.lt.s	$f20, $f26
	bc1f	$45
	.loc	2 3810
 #3810	        D_80152720[pl] = 1.0f - magvel * .01f;
	li.s	$f8, 1.0
	li.s	$f10, .01
	mul.s	$f4, $f20, $f10
	sub.s	$f6, $f8, $f4
	mul	$15, $19, 4
	s.s	$f6, D_80152720($15)
	b	$46
$45:
	.loc	2 3812
 #3811	    else
 #3812	        D_80152720[pl] = 0.0f;
	mul	$24, $19, 4
	s.s	$f28, D_80152720($24)
$46:
	.loc	2 3814
 #3813	
 #3814	    camoff[0] = res[0];
	l.s	$f10, 136($sp)
	s.s	$f10, 0($20)
	.loc	2 3815
 #3815	    camoff[1] = res[1];
	l.s	$f8, 140($sp)
	s.s	$f8, 4($20)
	.loc	2 3816
 #3816	    camoff[2] = res[2];
	l.s	$f4, 144($sp)
	s.s	$f4, 8($20)
	.loc	2 3817
 #3817	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 36($sp)
	addu	$sp, 160
	j	$31
	.end	func_800E92C8
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800EA2DC
	.loc	2 3924
 #3924	void func_800EA2DC(Cr *car, void *arg1, void *arg2, s32 arg3) {
	.ent	func_800EA2DC 2
func_800EA2DC:
	.option	O3
	subu	$sp, 144
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
	.mask	0x80FF0000, -44
	.fmask	0xFFF00000, -88
	.frame	$sp, 144, $31
	.loc	2 3924
	move	$23, $4
	sw	$7, 156($sp)
	.loc	2 3924
	.loc	2 3929
 #3925	    volatile s32 padv[5];
 #3926	    f32 v[3];
 #3927	    s32 pl;
 #3928	
 #3929	    pl = car->player;
	lb	$3, 860($23)
	.loc	2 3930
 #3930	    func_800E8CB8(car, arg1, arg2);
	move	$4, $23
	sw	$3, 108($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_800E8CB8
	lw	$3, 108($sp)
	.loc	2 3931
 #3931	    if (car->mode == 0xA) {
	lb	$14, 861($23)
	bne	$14, 10, $47
	.loc	2 3931
	.loc	2 3932
 #3932	        func_800E92C8(car, -D_801526F8[pl], D_801526E0[pl], v);
	move	$16, $23
	mul	$2, $3, 4
	l.s	$f22, D_801526F8($2)
	neg.s	$f22, $f22
	l.s	$f24, D_801526E0($2)
	addu	$20, $sp, 112
	.livereg	0x2000880E,0x00000280
	jal	func_800E92C8
	b	$48
$47:
	.loc	2 3933
 #3933	    } else {
	.loc	2 3934
 #3934	        func_800E92C8(car, D_801526F8[pl], D_801526E0[pl], v);
	move	$16, $23
	mul	$2, $3, 4
	l.s	$f22, D_801526F8($2)
	l.s	$f24, D_801526E0($2)
	addu	$20, $sp, 112
	.livereg	0x2000880E,0x00000280
	jal	func_800E92C8
$48:
	.loc	2 3936
 #3935	    }
 #3936	    func_800E8D50(car, arg3, 0, v);
	move	$4, $23
	lw	$5, 156($sp)
	move	$6, $0
	addu	$7, $sp, 112
	.livereg	0x0F00000E,0x00000000
	jal	func_800E8D50
	.loc	2 3937
 #3937	}
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
	addu	$sp, 144
	j	$31
	.end	func_800EA2DC
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800EA108
	.loc	2 3908
 #3908	void func_800EA108(Cr *car, f32 *pos, f32 *uvs) {
	.ent	func_800EA108 2
func_800EA108:
	.option	O3
	subu	$sp, 168
	sw	$31, 100($sp)
	sw	$22, 96($sp)
	sw	$21, 92($sp)
	sw	$20, 88($sp)
	sw	$19, 84($sp)
	sw	$18, 80($sp)
	sw	$17, 76($sp)
	sw	$16, 72($sp)
	s.d	$f30, 64($sp)
	s.d	$f28, 56($sp)
	s.d	$f26, 48($sp)
	s.d	$f24, 40($sp)
	s.d	$f22, 32($sp)
	s.d	$f20, 24($sp)
	.mask	0x807F0000, -68
	.fmask	0xFFF00000, -104
	.frame	$sp, 168, $31
	.loc	2 3908
	move	$7, $4
	sw	$5, 172($sp)
	.loc	2 3908
	.loc	2 3913
 #3909	    s32 i, j;
 #3910	    f32 rpos[3], res[3];
 #3911	    s32 slot;
 #3912	
 #3913	    slot = car->player;
	lb	$14, 860($7)
	sw	$14, 132($sp)
	.loc	2 3914
 #3914	    func_8008D6FC(car->s_f6, pos, uvs);
	lh	$4, 246($7)
	lw	$5, 172($sp)
	sw	$7, 168($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D6FC
	lw	$7, 168($sp)
	.loc	2 3915
 #3915	    D_80152708[slot] = .13f;
	.noalias	$5,$sp
	lw	$15, 132($sp)
	mul	$24, $15, 4
	la	$25, D_80152708
	addu	$5, $24, $25
	li.s	$f4, .13
	s.s	$f4, 0($5)
	.loc	2 3916
 #3916	    func_800E92C8(car, 30, 16, res);
	move	$16, $7
	li.s	$f22, 3.0000000000000000e+01
	li.s	$f24, 1.6000000000000000e+01
	addu	$20, $sp, 136
	sw	$5, 128($sp)
	.livereg	0x0000880E,0x00000280
	jal	func_800E92C8
	lw	$5, 128($sp)
	lw	$8, 172($sp)
	lw	$9, 132($sp)
	.loc	2 3917
 #3917	    for (i = 0; i < 3; i++)
	move	$7, $0
	mul	$14, $9, 152
	la	$15, D_80150B70
	addu	$6, $14, $15
	move	$2, $6
	l.s	$f0, 0($5)
	move	$3, $8
	addu	$4, $sp, 136
	li.s	$f6, 1.0
	sub.s	$f2, $f6, $f0
	addu	$5, $sp, 148
	.alias	$5,$sp
$49:
	.loc	2 3918
 #3918	        D_80150B70[slot].v84[i] = (D_80150B70[slot].v84[i]*(1.0f-D_80152708[slot]) + (pos[i]+res[i])*D_80152708[slot]);
	.noalias	$2,$sp
	.noalias	$4,$2
	.noalias	$4,$gp
	l.s	$f8, 0($3)
	l.s	$f10, 0($4)
	add.s	$f4, $f8, $f10
	mul.s	$f6, $f0, $f4
	l.s	$f8, 132($2)
	mul.s	$f10, $f8, $f2
	add.s	$f4, $f6, $f10
	s.s	$f4, 132($2)
	.loc	2 3917
 #3917	    for (i = 0; i < 3; i++)
	addu	$2, $2, 4
	addu	$3, $3, 4
	addu	$4, $4, 4
	bltu	$4, $5, $49
	.alias	$2,$4
	.alias	$2,$sp
	.alias	$4,$gp
	.loc	2 3919
 #3918	        D_80150B70[slot].v84[i] = (D_80150B70[slot].v84[i]*(1.0f-D_80152708[slot]) + (pos[i]+res[i])*D_80152708[slot]);
 #3919	    for (i = 0; i < 3; i++)
	move	$7, $0
	move	$2, $6
	move	$3, $8
	addu	$4, $sp, 148
	addu	$5, $sp, 160
$50:
	.loc	2 3920
 #3920	        rpos[i] = pos[i] - D_80150B70[slot].v84[i];
	.noalias	$4,$gp
	.noalias	$2,$4
	.noalias	$2,$sp
	l.s	$f8, 0($3)
	l.s	$f6, 132($2)
	sub.s	$f10, $f8, $f6
	s.s	$f10, 0($4)
	.loc	2 3919
 #3919	    for (i = 0; i < 3; i++)
	addu	$2, $2, 4
	addu	$3, $3, 4
	addu	$4, $4, 4
	bne	$4, $5, $50
	.alias	$2,$4
	.alias	$2,$sp
	.alias	$4,$gp
	.loc	2 3921
 #3920	        rpos[i] = pos[i] - D_80150B70[slot].v84[i];
 #3921	    vector_normalize_length(rpos, &D_80150B70[slot].n60);
	addu	$4, $sp, 148
	mul	$24, $9, 152
	addu	$25, $24, 96
	la	$14, D_80150B70
	addu	$5, $25, $14
	.livereg	0x0C00000E,0x00000000
	jal	vector_normalize_length
	.loc	2 3922
 #3922	}
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 24($sp)
	l.d	$f22, 32($sp)
	l.d	$f24, 40($sp)
	l.d	$f26, 48($sp)
	l.d	$f28, 56($sp)
	l.d	$f30, 64($sp)
	lw	$16, 72($sp)
	lw	$17, 76($sp)
	lw	$18, 80($sp)
	lw	$19, 84($sp)
	lw	$20, 88($sp)
	lw	$21, 92($sp)
	lw	$22, 96($sp)
	lw	$31, 100($sp)
	addu	$sp, 168
	j	$31
	.end	func_800EA108
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_800E95DC
	.loc	2 3819
 #3819	void func_800E95DC(s16 mode, Cr *car, f32 *pos, s32 uvs) {
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
