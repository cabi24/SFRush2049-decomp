	.verstamp	3 19
	.option	pic0
	.extern	D_8011194C 1
	.extern	D_801497D0 24
	.extern	D_801527E4 4
	.extern	D_8011EAE4 1
	.extern	D_80035458 24
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 42
 #  42	{
	.ent	pak_unlock 2
pak_unlock:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 42
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	pak_unlock
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 26
 #  26	{
	.ent	pak_queue_init 2
pak_queue_init:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 26
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	pak_queue_init
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 34
 #  34	{
	.ent	pak_lock 2
pak_lock:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 34
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	pak_lock
	.text	
	.align	2
	.file	2 "g.c"
	.globl	check_mpath_save
	.loc	2 52
 #  52	{
	.ent	check_mpath_save 2
check_mpath_save:
	.option	O3
	subu	$sp, 88
	sw	$31, 44($sp)
	sw	$20, 40($sp)
	sw	$19, 36($sp)
	sw	$18, 32($sp)
	sw	$17, 28($sp)
	sw	$16, 24($sp)
	.mask	0x801F0000, -44
	.frame	$sp, 88, $31
	.loc	2 52
	la	$2, D_8011194C
	.loc	2 52
	.loc	2 56
 #  53	    s32 i;
 #  54	    Pak772 *p;
 #  55	
 #  56	    D_8011EAE4 = 0;
	sb	$0, D_8011EAE4
	.loc	2 57
 #  57	    pak_lock();
	.loc	2 34
 #  34	{
	.loc	2 37
 #  35	    OSMesg message;
 #  36	
 #  37	    pak_queue_init();
	.loc	2 26
 #  26	{
	.loc	2 27
 #  27	    if (!D_8011194C) {
	.noalias	$2,$sp
	lb	$14, 0($2)
	bne	$14, 0, $32
	.loc	2 27
	.loc	2 28
 #  28	        D_8011194C = 1; osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
	li	$15, 1
	sb	$15, 0($2)
	.loc	2 28
	la	$4, D_801497D0
	la	$5, D_801527E4
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osCreateMesgQueue
	.alias	$2,$sp
	.loc	2 29
 #  29	        osJamMesg(&D_801497D0,0,0);
	la	$4, D_801497D0
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
$32:
	.loc	2 31
 #  30	    }
 #  31	}
	.loc	2 38
 #  38	    osRecvMesg(&D_801497D0,&message,1);
	la	$4, D_801497D0
	addu	$5, $sp, 68
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 39
 #  39	}
	.loc	2 58
 #  58	    for (i = 0; i < 4; i++) {
	move	$18, $0
	la	$16, D_80144030
	la	$20, D_80035458
	li	$19, 4
$33:
	.loc	2 58
	.loc	2 59
 #  59	        p = &D_80144030[i];
	.loc	2 60
 #  60	        if (!p->present) {
	.noalias	$16,$sp
	lb	$24, 6($16)
	bne	$24, 0, $99
	b	$34
$99:
	.loc	2 60
	.loc	2 61
 #  61	            continue;
	.loc	2 63
 #  62	        }
 #  63	        p->opaque116[9] = 0;
	sb	$0, 125($16)
	.loc	2 64
 #  64	        osMotorStart(&D_80035458, &p->pfs, i);
	move	$4, $20
	addu	$17, $16, 12
	move	$5, $17
	move	$6, $18
	.livereg	0x0E00400E,0x00000000
	jal	osMotorStart
	.loc	2 65
 #  65	        if (osMotorInit(&p->pfs, 0) == 0) {
	move	$4, $17
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	jal	osMotorInit
	bne	$2, 0, $34
	.loc	2 65
	.loc	2 66
 #  66	            p->opaque116[8] = 0;
	sb	$0, 124($16)
	.loc	2 58
 #  58	    for (i = 0; i < 4; i++) {
$34:
	addu	$18, $18, 1
	addu	$16, $16, 772
	bne	$18, $19, $33
	.alias	$16,$sp
	.loc	2 69
 #  69	    pak_unlock();
	.loc	2 42
 #  42	{
	.loc	2 43
 #  43	    osJamMesg(&D_801497D0,0,0);
	la	$4, D_801497D0
	move	$5, $0
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osJamMesg
	.loc	2 44
 #  44	}
	.loc	2 70
 #  70	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 24($sp)
	lw	$17, 28($sp)
	lw	$18, 32($sp)
	lw	$19, 36($sp)
	lw	$20, 40($sp)
	lw	$31, 44($sp)
	addu	$sp, 88
	j	$31
	.end	check_mpath_save
