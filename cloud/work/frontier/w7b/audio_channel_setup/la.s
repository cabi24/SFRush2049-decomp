	.verstamp	3 19
	.option	pic0
	.extern	D_801170FC 4
	.extern	D_8002EB94 4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 33
 #  33	{
	.ent	resource_set_object 2
resource_set_object:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 33
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	resource_set_object
	.text	
	.align	2
	.file	2 "g.c"
	.globl	audio_channel_setup
	.loc	2 37
 #  34	    D_8012E700[index].object = object;
 #  35	}
 #  36	
 #  37	void audio_channel_setup(Obj *obj, s16 mode) {
	.ent	audio_channel_setup 2
audio_channel_setup:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	2 37
	move	$7, $4
	sw	$5, 28($sp)
	sll	$14, $5, 16
	move	$5, $14
	sra	$15, $5, 16
	move	$5, $15
	.loc	2 37
	.loc	2 42
 #  38	    Model *m;
 #  39	    Kind *k;
 #  40	    s16 f;
 #  41	
 #  42	    if (mode == 0) {
	bne	$5, 0, $33
	.loc	2 42
	.loc	2 43
 #  43	remove:
$32:
	.loc	2 44
 #  44	        entity_transform_apply(obj, 1);
	move	$4, $7
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	entity_transform_apply
	.loc	2 45
 #  45	        return;
	b	$36
$33:
	.loc	2 47
 #  46	    }
 #  47	    if (D_801170FC != 0) {
	lw	$24, D_801170FC
	bne	$24, 0, $36
	.loc	2 47
	.loc	2 48
 #  48	        return;
	.loc	2 50
 #  49	    }
 #  50	    m = obj->model;
	lw	$2, 12($7)
	.loc	2 51
 #  51	    obj->timer -= D_8002EB94;
	l.s	$f4, 16($7)
	la	$25, D_8002EB94
	.set	 volatile
	l.s	$f6, 0($25)
	.set	 novolatile
	sub.s	$f8, $f4, $f6
	s.s	$f8, 16($7)
	.loc	2 52
 #  52	    if (obj->timer > 0.0f) {
	li.s	$f10, 0.0
	l.s	$f16, 16($7)
	c.lt.s	$f10, $f16
	bc1t	$36
	.loc	2 52
	.loc	2 53
 #  53	        return;
	.loc	2 55
 #  54	    }
 #  55	    obj->step++;
	lh	$8, 4($7)
	addu	$9, $8, 1
	sh	$9, 4($7)
	.loc	2 56
 #  56	    obj->timer = 0.0625f;
	li.s	$f18, 0.0625
	s.s	$f18, 16($7)
	.loc	2 57
 #  57	    if (obj->step >= m->count) {
	lh	$3, 4($7)
	lh	$10, 90($2)
	blt	$3, $10, $35
	.loc	2 57
	.loc	2 58
 #  58	        k = &D_80117530[m->type];
	lh	$11, 16($2)
	mul	$12, $11, 48
	la	$13, D_80117530
	addu	$3, $12, $13
	.loc	2 59
 #  59	        if (k->flags & 0x1000) {
	lhu	$4, 18($3)
	and	$14, $4, 4096
	bne	$14, 0, $32
	.loc	2 59
	.loc	2 60
 #  60	            goto remove;
	.loc	2 62
 #  61	        }
 #  62	        if (k->flags & 0x2000) {
	and	$15, $4, 8192
	beq	$15, 0, $34
	.loc	2 62
	.loc	2 63
 #  63	            entity_spawn_callback(m->id, 0, 0);
	lh	$4, 14($2)
	move	$5, $0
	move	$6, $0
	sw	$7, 24($sp)
	.livereg	0x0E00000E,0x00000000
	jal	entity_spawn_callback
	lw	$7, 24($sp)
	.loc	2 64
 #  64	            goto remove;
	b	$32
$34:
	.loc	2 66
 #  65	        }
 #  66	        obj->step = 0;
	sh	$0, 4($7)
	lh	$3, 4($7)
$35:
	.loc	2 68
 #  67	    }
 #  68	    f = m->base + obj->step;
	lh	$24, 88($2)
	addu	$4, $24, $3
	sll	$25, $4, 16
	move	$4, $25
	sra	$8, $4, 16
	move	$4, $8
	.loc	2 69
 #  69	    if (f != m->frame) {
	lh	$9, 80($2)
	beq	$4, $9, $36
	.loc	2 69
	.loc	2 70
 #  70	        resource_set_object(m->id, D_801427C0[f]);
	lh	$3, 14($2)
	mul	$10, $4, 2
	la	$6, D_801427C0($10)
	lhu	$5, 0($6)
	.loc	2 33
 #  33	{
	.loc	2 34
 #  34	    D_8012E700[index].object = object;
	mul	$11, $3, 68
	sh	$5, D_8012E700+20($11)
	.loc	2 35
 #  35	}
	.loc	2 71
 #  71	        m->frame = f;
	sh	$4, 80($2)
	.loc	2 73
 #  72	    }
 #  73	}
$36:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	audio_channel_setup
