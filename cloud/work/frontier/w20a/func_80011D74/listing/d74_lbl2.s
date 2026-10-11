	.verstamp	7 10
	.option	pic0
	.text	
	.align	2
	.file	2 "r.c"
	.globl	func_80011D74
	.loc	2 33
 #  33	{
	.ent	func_80011D74 2
func_80011D74:
	.option	O2
	subu	$sp, 32
	sw	$31, 20($sp)
	.mask	0x80000000, -12
	.frame	$sp, 32, $31
	sw	$5, 36($sp)
	and	$5, $5, 65535
	.loc	2 33
	.loc	2 36
 #  34	    double increment;
 #  35	    double loop_end;
 #  36	    increment = (audio->rate * samples) * 0.000244140625;
	lhu	$14, 4($4)
	mul	$15, $14, $5
	mtc1	$15, $f4
	cvt.d.w	$f6, $f4
	li.d	$f8, 0.000244140625
	mul.d	$f2, $f6, $f8
	.loc	2 37
 #  37	    if (audio->loop_length != 0 && (audio->held != 0 || audio->stop_loop == 0)
	lw	$24, 56($4)
$99:
	beq	$24, 0, $34
	lbu	$25, 1($4)
	bne	$25, 0, $32
	lw	$8, 60($4)
	bne	$8, 0, $34
$32:
	l.d	$f12, 8($4)
	sw	$4, 32($sp)
	s.d	$f2, 24($sp)
	.livereg	0x0000000E,0x000C0000
	jal	func_8001E688
	lw	$4, 32($sp)
	l.d	$f2, 24($sp)
	lw	$2, 56($4)
	lw	$9, 52($4)
	addu	$10, $9, $2
	mtc1	$10, $f10
	cvt.d.w	$f12, $f10
	bge	$10, $0, $37
	li.d	$f18, 4294967296.0
	add.d	$f12, $f12, $f18
$37:
	c.le.d	$f0, $f12
	bc1f	$34
	.loc	2 38
 #  38	        && func_8001E688(audio->position) <= audio->loop_start + audio->loop_length) {
	.loc	2 39
 #  39	        loop_end = audio->loop_start + audio->loop_length;
	mov.d	$f16, $f12
	.loc	2 40
 #  40	        if (audio->position + increment < loop_end) {
	l.d	$f14, 8($4)
	add.d	$f0, $f14, $f2
	c.lt.d	$f0, $f16
	bc1f	$33
	.loc	2 40
	.loc	2 41
 #  41	            audio->position += increment;
	s.d	$f0, 8($4)
	b	$36
$33:
	.loc	2 42
 #  42	        } else {
	.loc	2 43
 #  43	            increment -= loop_end - audio->position;
	sub.d	$f4, $f16, $f14
	sub.d	$f2, $f2, $f4
	.loc	2 44
 #  44	            increment -= func_8001E688(increment / audio->loop_length) * audio->loop_length;
	mtc1	$2, $f6
	cvt.d.w	$f8, $f6
	bge	$2, $0, $38
	li.d	$f10, 4294967296.0
	add.d	$f8, $f8, $f10
$38:
	div.d	$f12, $f2, $f8
	sw	$4, 32($sp)
	s.d	$f2, 24($sp)
	.livereg	0x0000000E,0x000C0000
	jal	func_8001E688
	lw	$4, 32($sp)
	l.d	$f2, 24($sp)
	lw	$11, 56($4)
	mtc1	$11, $f18
	cvt.d.w	$f4, $f18
	bge	$11, $0, $39
	li.d	$f6, 4294967296.0
	add.d	$f4, $f4, $f6
$39:
	mul.d	$f10, $f0, $f4
	sub.d	$f2, $f2, $f10
	.loc	2 45
 #  45	            audio->position = audio->loop_start + increment;
	lw	$12, 52($4)
	mtc1	$12, $f8
	cvt.d.w	$f18, $f8
	bge	$12, $0, $40
	li.d	$f6, 4294967296.0
	add.d	$f18, $f18, $f6
$40:
	add.d	$f4, $f18, $f2
	s.d	$f4, 8($4)
	b	$36
$34:
	.loc	2 47
 #  46	        }
 #  47	    } else {
	.loc	2 48
 #  48	        if (audio->position + increment >= audio->length) {
	l.d	$f10, 8($4)
	add.d	$f0, $f10, $f2
	lw	$2, 48($4)
	mtc1	$2, $f8
	cvt.d.w	$f6, $f8
	bge	$2, $0, $41
	li.d	$f18, 4294967296.0
	add.d	$f6, $f6, $f18
$41:
	c.le.d	$f6, $f0
	bc1f	$35
	.loc	2 48
	.loc	2 49
 #  49	            audio->position = audio->length - 1;
	addu	$13, $2, -1
	mtc1	$13, $f4
	cvt.d.w	$f10, $f4
	bge	$13, $0, $42
	li.d	$f8, 4294967296.0
	add.d	$f10, $f10, $f8
$42:
	s.d	$f10, 8($4)
	b	$36
$35:
	.loc	2 50
 #  50	        } else {
	.loc	2 51
 #  51	            audio->position += increment;
	s.d	$f0, 8($4)
	.loc	2 54
 #  52	        }
 #  53	    }
 #  54	}
$36:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 32
	j	$31
	.end	func_80011D74
