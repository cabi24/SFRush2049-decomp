	.verstamp	3 19
	.option	pic0
	.extern	D_80121D60 4
	.extern	D_80121D64 4
	.extern	D_80121D68 4
	.extern	D_80121D6C 4
	.extern	D_80121D70 4
	.extern	D_80121D74 4
	.extern	D_80121D78 4
	.extern	D_80121D7C 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_8010D3C0
	.loc	2 50
 #  50	{
	.ent	func_8010D3C0 2
func_8010D3C0:
	.option	O3
	subu	$sp, 32
	sw	$31, 20($sp)
	.mask	0x80000000, -12
	.frame	$sp, 32, $31
	.loc	2 50
	move	$8, $4
	.loc	2 50
	.loc	2 56
 #  51	    Car952 *car;
 #  52	    Vehicle2056 *vehicle;
 #  53	    s32 kind,amount;
 #  54	    Limits16 *limits;
 #  55	    f32 maximum;
 #  56	    if(effect->flags&4) {
	lbu	$3, 4($8)
	and	$14, $3, 4
	beq	$14, 0, $52
	.loc	2 56
	.loc	2 57
 #  57	        car=&D_80152818[effect->owner];
	lb	$5, 92($8)
	mul	$15, $5, 952
	la	$24, D_80152818
	addu	$2, $15, $24
	.loc	2 58
 #  58	        vehicle=&D_8014A250[effect->owner];
	mul	$25, $5, 2056
	la	$10, D_8014A250
	addu	$9, $25, $10
	.loc	2 59
 #  59	        effect->flags&=~6;
	and	$11, $3, -7
	sb	$11, 4($8)
	.loc	2 60
 #  60	        if(car->flags&16) {
	lw	$12, 232($2)
	and	$13, $12, 16
	beq	$13, 0, $32
	.loc	2 60
	.loc	2 61
 #  61	            effect->flags|=2;
	lbu	$14, 4($8)
	or	$15, $14, 2
	sb	$15, 4($8)
	.loc	2 62
 #  62	            return;
	b	$53
$32:
	.loc	2 64
 #  63	        }
 #  64	        stat_lap_split(D_80117530[effect->metadata].resource,effect->owner,&effect->position,2);
	lh	$24, 16($8)
	mul	$25, $24, 48
	lw	$4, D_80117530+28($25)
	addu	$6, $8, 56
	li	$7, 2
	sw	$2, 28($sp)
	sw	$8, 32($sp)
	sw	$9, 24($sp)
	.livereg	0x0F00000E,0x00000000
	jal	stat_lap_split
	lw	$2, 28($sp)
	lw	$8, 32($sp)
	lw	$9, 24($sp)
	.loc	2 65
 #  65	        switch(effect->effect) {
	lh	$10, 80($8)
	addu	$11, $10, -350
	bgeu	$11, 11, $46
	sll	$11, $11, 2
	lw	$11, $33($11)
	j	$11
	.rdata	
$33:
	.word	$35
	.word	$36
	.word	$37
	.word	$38
	.word	$39
	.word	$40
	.word	$41
	.word	$42
	.word	$43
	.word	$44
	.word	$45
	.text	
$35:
	.loc	2 67
 #  66	        case 350:
 #  67	            kind=0;
	move	$4, $0
	.loc	2 68
 #  68	            amount=D_80121D60;
	lw	$3, D_80121D60
	.loc	2 69
 #  69	            break;
	b	$47
$36:
	.loc	2 71
 #  70	        case 351:
 #  71	            kind=1;
	li	$4, 1
	.loc	2 72
 #  72	            amount=D_80121D64;
	lw	$3, D_80121D64
	.loc	2 73
 #  73	            break;
	b	$47
$37:
	.loc	2 75
 #  74	        case 352:
 #  75	            kind=2;
	li	$4, 2
	.loc	2 76
 #  76	            amount=D_80121D68;
	lw	$3, D_80121D68
	.loc	2 77
 #  77	            break;
	b	$47
$38:
	.loc	2 78
 #  78	        case 353: car->timer=800; return;
	li	$12, 800
	sh	$12, 902($2)
	.loc	2 78
	b	$53
$39:
	.loc	2 80
 #  79	        case 354:
 #  80	            car->extra_flags|=1;
	lw	$13, 908($2)
	or	$14, $13, 1
	sw	$14, 908($2)
	.loc	2 81
 #  81	            car->state=1;
	li	$15, 1
	sb	$15, 930($2)
	.loc	2 82
 #  82	            car->boost=30.0f;
	li.s	$f4, 30.0
	s.s	$f4, 912($2)
	.loc	2 83
 #  83	            car->field936=0.666667f;
	li.s	$f6, 0.666667
	s.s	$f6, 936($2)
	.loc	2 84
 #  84	            car->color=255;
	li	$24, 255
	sb	$24, 929($2)
	.loc	2 85
 #  85	            return;
	b	$53
$40:
	.loc	2 87
 #  86	        case 355:
 #  87	            kind=3;
	li	$4, 3
	.loc	2 88
 #  88	            amount=D_80121D6C;
	lw	$3, D_80121D6C
	.loc	2 89
 #  89	            break;
	b	$47
$41:
	.loc	2 91
 #  90	        case 356:
 #  91	            kind=4;
	li	$4, 4
	.loc	2 92
 #  92	            amount=D_80121D70;
	lw	$3, D_80121D70
	.loc	2 93
 #  93	            break;
	b	$47
$42:
	.loc	2 95
 #  94	        case 357:
 #  95	            kind=5;
	li	$4, 5
	.loc	2 96
 #  96	            amount=D_80121D74;
	lw	$3, D_80121D74
	.loc	2 97
 #  97	            break;
	b	$47
$43:
	.loc	2 99
 #  98	        case 358:
 #  99	            kind=6;
	li	$4, 6
	.loc	2 100
 # 100	            amount=D_80121D78;
	lw	$3, D_80121D78
	.loc	2 101
 # 101	            break;
	b	$47
$44:
	.loc	2 103
 # 102	        case 359:
 # 103	            car->extra_flags|=10;
	lw	$25, 908($2)
	or	$10, $25, 10
	sw	$10, 908($2)
	.loc	2 104
 # 104	            car->field916=0.0333333f;
	li.s	$f8, 0.0333333
	s.s	$f8, 916($2)
	.loc	2 105
 # 105	            car->field920=0.05f;
	li.s	$f10, 0.05
	s.s	$f10, 920($2)
	.loc	2 106
 # 106	            car->field924=0.0f;
	li.s	$f16, 0.0
	s.s	$f16, 924($2)
	.loc	2 107
 # 107	            return;
	b	$53
$45:
	.loc	2 109
 # 108	        case 360:
 # 109	            kind=7;
	li	$4, 7
	.loc	2 110
 # 110	            amount=D_80121D7C;
	lw	$3, D_80121D7C
	.loc	2 111
 # 111	            break;
	b	$47
$46:
 # 112	        default:
 # 113	            kind=1;
	li	$4, 1
 # 114	            amount=D_80121D64;
	lw	$3, D_80121D64
 # 115	            break;
$47:
	.loc	2 117
 # 116	        }
 # 117	        if(amount) {}
	.loc	2 117
	.loc	2 118
 # 118	        if(amount) {}
	.loc	2 118
	.loc	2 119
 # 119	        if(car->kind==kind) car->amount+=amount;
	lb	$11, 900($2)
	bne	$4, $11, $48
	.loc	2 119
	lb	$12, 901($2)
	addu	$13, $12, $3
	sb	$13, 901($2)
	b	$49
$48:
	.loc	2 120
 # 120	        else {car->kind=kind;car->amount=amount;}
	.loc	2 120
	sb	$4, 900($2)
	.loc	2 120
	sb	$3, 901($2)
$49:
	.loc	2 121
 # 121	        if(kind==5) {
	bne	$4, 5, $53
	.loc	2 121
	.loc	2 122
 # 122	            limits=&D_8011F844[vehicle->model];
	lbu	$14, 8($9)
	mul	$15, $14, 16
	la	$24, D_8011F844
	addu	$2, $15, $24
	.loc	2 123
 # 123	            vehicle->speed=vehicle->boost=limits->bias+3.0f;
	l.s	$f18, 0($2)
	li.s	$f4, 3.0
	add.s	$f0, $f18, $f4
	s.s	$f0, 264($9)
	s.s	$f0, 252($9)
	.loc	2 124
 # 124	            maximum=vehicle->speed>limits->maximum?vehicle->speed:limits->maximum;
	l.s	$f2, 4($2)
	c.lt.s	$f2, $f0
	bc1f	$50
	l.s	$f14, 252($9)
	b	$51
$50:
	mov.s	$f14, $f2
$51:
	.loc	2 125
 # 125	            vehicle->length=sqrtf(maximum*maximum+limits->y*limits->y+limits->z*limits->z);
	l.s	$f2, 8($2)
	l.s	$f12, 12($2)
	mul.s	$f6, $f14, $f14
	mul.s	$f8, $f2, $f2
	add.s	$f10, $f6, $f8
	mul.s	$f16, $f12, $f12
	add.s	$f0, $f10, $f16
	sqrt.s	$f0, $f0
	s.s	$f0, 1620($9)
	b	$53
$52:
	.loc	2 127
 # 126	        }
 # 127	    } else {
	.loc	2 128
 # 128	        func_80391490();
	sw	$8, 32($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_80391490
	lw	$8, 32($sp)
	.loc	2 129
 # 129	        effect->flags&=~2;
	lbu	$25, 4($8)
	and	$10, $25, -3
	sb	$10, 4($8)
	.loc	2 130
 # 130	        effect->flags|=32;
	lbu	$11, 4($8)
	or	$12, $11, 32
	sb	$12, 4($8)
	.loc	2 131
 # 131	        model_data_load(*(void **)((u8 *)effect+12),0,15);
	lw	$4, 12($8)
	move	$5, $0
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 133
 # 132	    }
 # 133	}
$53:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 32
	j	$31
	.end	func_8010D3C0
