	.verstamp	3 19
	.option	pic0
	.extern	D_8012E60C 4
	.extern	D_8012E608 4
	.extern	D_8012E668 4
	.extern	D_8012E610 4
	.extern	D_8012E674 4
	.extern	D_8014A248 4
	.extern	D_80149438 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_80087110
	.loc	2 17
 #  17	{
	.ent	func_80087110 2
func_80087110:
	.option	O3
	subu	$sp, 104
	.frame	$sp, 104, $31
	.loc	2 17
	move	$8, $4
	move	$10, $5
	move	$9, $6
	.loc	2 17
	.loc	2 19
 #  18	    int height,step,offset,texture_edge;
 #  19	    if(x<D_8012E60C) {
	lw	$2, D_8012E60C
	bge	$8, $2, $33
	la	$3, D_8012E608
	.loc	2 19
	.loc	2 20
 #  20	        if(!(D_8012E608&4))s+=D_8012E60C-x;
	.noalias	$3,$sp
	lw	$14, 0($3)
	and	$15, $14, 4
	bne	$15, 0, $32
	lw	$5, 120($sp)
	.loc	2 20
	addu	$24, $5, $2
	subu	$5, $24, $8
	sw	$5, 120($sp)
$32:
	lw	$5, 120($sp)
	.loc	2 21
 #  21	        x=D_8012E60C;
	move	$8, $2
$33:
	la	$3, D_8012E608
	lw	$5, 120($sp)
	.loc	2 23
 #  22	    }
 #  23	    if(y<D_8012E668) {
	lw	$2, D_8012E668
	bge	$10, $2, $35
	.loc	2 23
	.loc	2 24
 #  24	        if(!(D_8012E608&8))t+=D_8012E668-y;
	lw	$25, 0($3)
	and	$14, $25, 8
	bne	$14, 0, $34
	lw	$4, 124($sp)
	.loc	2 24
	addu	$15, $4, $2
	subu	$4, $15, $10
	sw	$4, 124($sp)
$34:
	lw	$4, 124($sp)
	.loc	2 25
 #  25	        y=D_8012E668;
	move	$10, $2
$35:
	lw	$4, 124($sp)
	.loc	2 27
 #  26	    }
 #  27	    if(right>D_8012E610) {
	lw	$2, D_8012E610
	bge	$2, $9, $37
	.loc	2 27
	.loc	2 28
 #  28	        if(D_8012E608&4)s=s-D_8012E610+right;
	lw	$24, 0($3)
	and	$25, $24, 4
	beq	$25, 0, $36
	.loc	2 28
	subu	$14, $5, $2
	addu	$5, $14, $9
$36:
	.loc	2 29
 #  29	        right=D_8012E610;
	move	$9, $2
$37:
	.loc	2 31
 #  30	    }
 #  31	    if(bottom>D_8012E674) {
	lw	$2, D_8012E674
	bge	$2, $7, $39
	.loc	2 31
	.loc	2 32
 #  32	        if(D_8012E608&8)t=t-D_8012E674+bottom;
	lw	$15, 0($3)
	and	$24, $15, 8
	beq	$24, 0, $38
	.alias	$3,$sp
	.loc	2 32
	subu	$25, $4, $2
	addu	$4, $25, $7
$38:
	.loc	2 33
 #  33	        bottom=D_8012E674;
	move	$7, $2
$39:
	.loc	2 35
 #  34	    }
 #  35	    if(right<x || bottom<y)return;
	blt	$9, $8, $49
	blt	$7, $10, $49
	.loc	2 35
	.loc	2 36
 #  36	    if(!D_8014A248) {
	lw	$2, D_8012E608
	and	$3, $2, 4
	lw	$14, D_8014A248
	bne	$14, 0, $43
	.loc	2 36
	.loc	2 37
 #  37	        if((D_8012E608&4)&&(D_8012E608&8)) {
	beq	$3, 0, $40
	and	$15, $2, 8
	beq	$15, 0, $40
	la	$3, D_80149438
	.loc	2 37
	.loc	2 38
 #  38	            texture_edge=s+right;s=texture_edge-x;t+=bottom-y;
	.loc	2 38
	addu	$24, $5, $9
	subu	$5, $24, $8
	.loc	2 38
	addu	$25, $4, $7
	subu	$4, $25, $10
	.loc	2 39
 #  39	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,right<<2,bottom<<2,0,s<<5,t<<5,-4096,-1024);
	.loc	2 39
	.noalias	$3,$sp
	lw	$2, 0($3)
	move	$6, $2
	addu	$14, $2, 8
	sw	$14, 0($3)
	.loc	2 39
	sll	$15, $9, 2
	and	$24, $15, 4095
	sll	$25, $24, 12
	or	$14, $25, -469762048
	sll	$15, $7, 2
	and	$24, $15, 4095
	or	$25, $14, $24
	sw	$25, 0($6)
	.loc	2 39
	sll	$15, $8, 2
	and	$14, $15, 4095
	sll	$24, $14, 12
	sll	$25, $10, 2
	and	$15, $25, 4095
	or	$14, $24, $15
	sw	$14, 4($6)
	.loc	2 39
	.loc	2 39
	lw	$2, 0($3)
	move	$11, $2
	addu	$25, $2, 8
	sw	$25, 0($3)
	.loc	2 39
	li	$24, -520093696
	sw	$24, 0($11)
	.loc	2 39
	sll	$15, $5, 5
	and	$14, $15, 65535
	sll	$25, $14, 16
	sll	$24, $4, 5
	and	$15, $24, 65535
	or	$14, $25, $15
	sw	$14, 4($11)
	.loc	2 39
	.loc	2 39
	.loc	2 39
	lw	$2, 0($3)
	move	$12, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 39
	li	$25, -251658240
	sw	$25, 0($12)
	.loc	2 39
	li	$15, -268370944
	sw	$15, 4($12)
	.loc	2 39
	.loc	2 39
	b	$49
	.alias	$3,$sp
$40:
	.loc	2 40
 #  40	        } else if(D_8012E608&4) {
	beq	$3, 0, $41
	la	$3, D_80149438
	.loc	2 40
	.loc	2 41
 #  41	            texture_edge=s+right;s=texture_edge-x;
	.loc	2 41
	addu	$14, $5, $9
	subu	$5, $14, $8
	.loc	2 42
 #  42	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,right<<2,bottom<<2,0,s<<5,t<<5,-4096,1024);
	.loc	2 42
	.noalias	$3,$sp
	lw	$2, 0($3)
	move	$6, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 42
	sll	$25, $9, 2
	and	$15, $25, 4095
	sll	$14, $15, 12
	or	$24, $14, -469762048
	sll	$25, $7, 2
	and	$15, $25, 4095
	or	$14, $24, $15
	sw	$14, 0($6)
	.loc	2 42
	sll	$25, $8, 2
	and	$24, $25, 4095
	sll	$15, $24, 12
	sll	$14, $10, 2
	and	$25, $14, 4095
	or	$24, $15, $25
	sw	$24, 4($6)
	.loc	2 42
	.loc	2 42
	lw	$2, 0($3)
	move	$11, $2
	addu	$14, $2, 8
	sw	$14, 0($3)
	.loc	2 42
	li	$15, -520093696
	sw	$15, 0($11)
	.loc	2 42
	sll	$25, $5, 5
	and	$24, $25, 65535
	sll	$14, $24, 16
	sll	$15, $4, 5
	and	$25, $15, 65535
	or	$24, $14, $25
	sw	$24, 4($11)
	.loc	2 42
	.loc	2 42
	.loc	2 42
	lw	$2, 0($3)
	move	$12, $2
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 42
	li	$14, -251658240
	sw	$14, 0($12)
	.loc	2 42
	li	$25, -268434432
	sw	$25, 4($12)
	.loc	2 42
	.loc	2 42
	b	$49
	.alias	$3,$sp
$41:
	.loc	2 43
 #  43	        } else if(D_8012E608&8) {
	and	$24, $2, 8
	beq	$24, 0, $42
	la	$3, D_80149438
	.loc	2 43
	.loc	2 44
 #  44	            t+=bottom-y;
	addu	$15, $4, $7
	subu	$4, $15, $10
	.loc	2 45
 #  45	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,right<<2,bottom<<2,0,s<<5,t<<5,4096,-1024);
	.loc	2 45
	.noalias	$3,$sp
	lw	$2, 0($3)
	move	$6, $2
	addu	$14, $2, 8
	sw	$14, 0($3)
	.loc	2 45
	sll	$25, $9, 2
	and	$24, $25, 4095
	sll	$15, $24, 12
	or	$14, $15, -469762048
	sll	$25, $7, 2
	and	$24, $25, 4095
	or	$15, $14, $24
	sw	$15, 0($6)
	.loc	2 45
	sll	$25, $8, 2
	and	$14, $25, 4095
	sll	$24, $14, 12
	sll	$15, $10, 2
	and	$25, $15, 4095
	or	$14, $24, $25
	sw	$14, 4($6)
	.loc	2 45
	.loc	2 45
	lw	$2, 0($3)
	move	$11, $2
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 45
	li	$24, -520093696
	sw	$24, 0($11)
	.loc	2 45
	sll	$25, $5, 5
	and	$14, $25, 65535
	sll	$15, $14, 16
	sll	$24, $4, 5
	and	$25, $24, 65535
	or	$14, $15, $25
	sw	$14, 4($11)
	.loc	2 45
	.loc	2 45
	.loc	2 45
	lw	$2, 0($3)
	move	$12, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 45
	li	$15, -251658240
	sw	$15, 0($12)
	.loc	2 45
	li	$25, 268499968
	sw	$25, 4($12)
	.loc	2 45
	.loc	2 45
	b	$49
$42:
	la	$3, D_80149438
	.loc	2 46
 #  46	        } else {
	.loc	2 47
 #  47	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,right<<2,bottom<<2,0,s<<5,t<<5,4096,1024);
	.loc	2 47
	lw	$2, 0($3)
	move	$6, $2
	addu	$14, $2, 8
	sw	$14, 0($3)
	.loc	2 47
	sll	$24, $9, 2
	and	$15, $24, 4095
	sll	$25, $15, 12
	or	$14, $25, -469762048
	sll	$24, $7, 2
	and	$15, $24, 4095
	or	$25, $14, $15
	sw	$25, 0($6)
	.loc	2 47
	sll	$24, $8, 2
	and	$14, $24, 4095
	sll	$15, $14, 12
	sll	$25, $10, 2
	and	$24, $25, 4095
	or	$14, $15, $24
	sw	$14, 4($6)
	.loc	2 47
	.loc	2 47
	lw	$2, 0($3)
	move	$11, $2
	addu	$25, $2, 8
	sw	$25, 0($3)
	.loc	2 47
	li	$15, -520093696
	sw	$15, 0($11)
	.loc	2 47
	sll	$24, $5, 5
	and	$14, $24, 65535
	sll	$25, $14, 16
	sll	$15, $4, 5
	and	$24, $15, 65535
	or	$14, $25, $24
	sw	$14, 4($11)
	.loc	2 47
	.loc	2 47
	.loc	2 47
	lw	$2, 0($3)
	move	$12, $2
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 47
	li	$25, -251658240
	sw	$25, 0($12)
	.loc	2 47
	li	$24, 268436480
	sw	$24, 4($12)
	.loc	2 47
	.loc	2 47
	.alias	$3,$sp
	b	$49
$43:
	.loc	2 49
 #  48	        }
 #  49	    } else {
	.loc	2 50
 #  50	        height=bottom-y;
	subu	$11, $7, $10
	.loc	2 51
 #  51	        if(D_8012E608&0x8000) {bottom+=height+1;step=512;offset=16;}
	and	$14, $2, 32768
	beq	$14, 0, $44
	.loc	2 51
	.loc	2 51
	addu	$7, $7, $11
	addu	$7, $7, 1
	.loc	2 51
	li	$6, 512
	.loc	2 51
	li	$12, 16
	b	$45
$44:
	.loc	2 52
 #  52	        else {step=1024;offset=0;}
	.loc	2 52
	li	$6, 1024
	.loc	2 52
	move	$12, $0
$45:
	.loc	2 53
 #  53	        if((D_8012E608&4)&&(D_8012E608&8)) {
	beq	$3, 0, $46
	and	$15, $2, 8
	beq	$15, 0, $46
	la	$3, D_80149438
	.loc	2 53
	.loc	2 54
 #  54	            texture_edge=s+right;s=texture_edge-x;t+=height;
	.loc	2 54
	addu	$25, $5, $9
	subu	$5, $25, $8
	.loc	2 54
	addu	$4, $4, $11
	.loc	2 55
 #  55	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,(right+1)<<2,(bottom+1)<<2,0,s<<5,(t<<5)+offset,-1024,-step);
	.loc	2 55
	.noalias	$3,$sp
	lw	$2, 0($3)
	move	$13, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 55
	addu	$14, $9, 1
	sll	$15, $14, 2
	and	$25, $15, 4095
	sll	$24, $25, 12
	or	$14, $24, -469762048
	addu	$15, $7, 1
	sll	$25, $15, 2
	and	$24, $25, 4095
	or	$15, $14, $24
	sw	$15, 0($13)
	.loc	2 55
	sll	$25, $8, 2
	and	$14, $25, 4095
	sll	$24, $14, 12
	sll	$15, $10, 2
	and	$25, $15, 4095
	or	$14, $24, $25
	sw	$14, 4($13)
	.loc	2 55
	.loc	2 55
	lw	$2, 0($3)
	sw	$2, 32($sp)
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 55
	li	$24, -520093696
	lw	$25, 32($sp)
	sw	$24, 0($25)
	.loc	2 55
	sll	$14, $5, 5
	and	$15, $14, 65535
	sll	$24, $15, 16
	sll	$14, $4, 5
	addu	$15, $14, $12
	and	$14, $15, 65535
	or	$15, $24, $14
	sw	$15, 4($25)
	.loc	2 55
	.loc	2 55
	.loc	2 55
	lw	$2, 0($3)
	sw	$2, 28($sp)
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 55
	li	$14, -251658240
	lw	$15, 28($sp)
	sw	$14, 0($15)
	.loc	2 55
	negu	$25, $6
	and	$24, $25, 65535
	or	$14, $24, -67108864
	sw	$14, 4($15)
	.loc	2 55
	.loc	2 55
	b	$49
	.alias	$3,$sp
$46:
	.loc	2 56
 #  56	        } else if(D_8012E608&4) {
	beq	$3, 0, $47
	la	$3, D_80149438
	.loc	2 56
	.loc	2 56
 #  57	            texture_edge=s+right;s=texture_edge-x;
	.loc	2 56
	addu	$25, $5, $9
	subu	$5, $25, $8
	.loc	2 58
 #  58	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,(right+1)<<2,(bottom+1)<<2,0,s<<5,t<<5,-1024,step);
	.loc	2 58
	.noalias	$3,$sp
	lw	$2, 0($3)
	move	$11, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 58
	addu	$14, $9, 1
	sll	$15, $14, 2
	and	$25, $15, 4095
	sll	$24, $25, 12
	or	$14, $24, -469762048
	addu	$15, $7, 1
	sll	$25, $15, 2
	and	$24, $25, 4095
	or	$15, $14, $24
	sw	$15, 0($11)
	.loc	2 58
	sll	$25, $8, 2
	and	$14, $25, 4095
	sll	$24, $14, 12
	sll	$15, $10, 2
	and	$25, $15, 4095
	or	$14, $24, $25
	sw	$14, 4($11)
	.loc	2 58
	.loc	2 58
	lw	$2, 0($3)
	move	$12, $2
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 58
	li	$24, -520093696
	sw	$24, 0($12)
	.loc	2 58
	sll	$25, $5, 5
	and	$14, $25, 65535
	sll	$15, $14, 16
	sll	$24, $4, 5
	and	$25, $24, 65535
	or	$14, $15, $25
	sw	$14, 4($12)
	.loc	2 58
	.loc	2 58
	.loc	2 58
	lw	$2, 0($3)
	move	$13, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 58
	li	$15, -251658240
	sw	$15, 0($13)
	.loc	2 58
	and	$25, $6, 65535
	or	$14, $25, -67108864
	sw	$14, 4($13)
	.loc	2 58
	.loc	2 58
	b	$49
	.alias	$3,$sp
$47:
	.loc	2 59
 #  59	        } else if(D_8012E608&8) {
	and	$24, $2, 8
	beq	$24, 0, $48
	la	$3, D_80149438
	.loc	2 59
	.loc	2 60
 #  60	            t+=height;
	addu	$4, $4, $11
	.loc	2 61
 #  61	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,(right+1)<<2,(bottom+1)<<2,0,s<<5,(t<<5)+offset,1024,-step);
	.loc	2 61
	.noalias	$3,$sp
	lw	$2, 0($3)
	move	$13, $2
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 61
	addu	$25, $9, 1
	sll	$14, $25, 2
	and	$24, $14, 4095
	sll	$15, $24, 12
	or	$25, $15, -469762048
	addu	$14, $7, 1
	sll	$24, $14, 2
	and	$15, $24, 4095
	or	$14, $25, $15
	sw	$14, 0($13)
	.loc	2 61
	sll	$24, $8, 2
	and	$25, $24, 4095
	sll	$15, $25, 12
	sll	$14, $10, 2
	and	$24, $14, 4095
	or	$25, $15, $24
	sw	$25, 4($13)
	.loc	2 61
	.loc	2 61
	lw	$2, 0($3)
	sw	$2, 8($sp)
	addu	$14, $2, 8
	sw	$14, 0($3)
	.loc	2 61
	li	$15, -520093696
	lw	$24, 8($sp)
	sw	$15, 0($24)
	.loc	2 61
	sll	$25, $5, 5
	and	$14, $25, 65535
	sll	$15, $14, 16
	sll	$25, $4, 5
	addu	$14, $25, $12
	and	$25, $14, 65535
	or	$14, $15, $25
	sw	$14, 4($24)
	.loc	2 61
	.loc	2 61
	.loc	2 61
	lw	$2, 0($3)
	sw	$2, 4($sp)
	addu	$15, $2, 8
	sw	$15, 0($3)
	.loc	2 61
	li	$25, -251658240
	lw	$14, 4($sp)
	sw	$25, 0($14)
	.loc	2 61
	negu	$24, $6
	and	$15, $24, 65535
	or	$25, $15, 67108864
	sw	$25, 4($14)
	.loc	2 61
	.loc	2 61
	b	$49
$48:
	la	$3, D_80149438
	.loc	2 62
 #  62	        } else {
	.loc	2 63
 #  63	            gSPTextureRectangle(D_80149438++,x<<2,y<<2,(right+1)<<2,(bottom+1)<<2,0,s<<5,t<<5,1024,step);
	.loc	2 63
	lw	$2, 0($3)
	move	$11, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 63
	addu	$15, $9, 1
	sll	$25, $15, 2
	and	$14, $25, 4095
	sll	$24, $14, 12
	or	$15, $24, -469762048
	addu	$25, $7, 1
	sll	$14, $25, 2
	and	$24, $14, 4095
	or	$25, $15, $24
	sw	$25, 0($11)
	.loc	2 63
	sll	$14, $8, 2
	and	$15, $14, 4095
	sll	$24, $15, 12
	sll	$25, $10, 2
	and	$14, $25, 4095
	or	$15, $24, $14
	sw	$15, 4($11)
	.loc	2 63
	.loc	2 63
	lw	$2, 0($3)
	move	$12, $2
	addu	$25, $2, 8
	sw	$25, 0($3)
	.loc	2 63
	li	$24, -520093696
	sw	$24, 0($12)
	.loc	2 63
	sll	$14, $5, 5
	and	$15, $14, 65535
	sll	$25, $15, 16
	sll	$24, $4, 5
	and	$14, $24, 65535
	or	$15, $25, $14
	sw	$15, 4($12)
	.loc	2 63
	.loc	2 63
	.loc	2 63
	lw	$2, 0($3)
	move	$13, $2
	addu	$24, $2, 8
	sw	$24, 0($3)
	.loc	2 63
	li	$25, -251658240
	sw	$25, 0($13)
	.loc	2 63
	and	$14, $6, 65535
	or	$15, $14, 67108864
	sw	$15, 4($13)
	.loc	2 63
	.loc	2 63
	.alias	$3,$sp
	.loc	2 66
 #  64	        }
 #  65	    }
 #  66	}
$49:
	.livereg	0x0000FF0E,0x00000FFF
	addu	$sp, 104
	j	$31
	.end	func_80087110
