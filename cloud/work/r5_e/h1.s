	.verstamp	3 19
	.option	pic0
	.text	
	.align	2
	.file	2 "h1.c"
	.globl	model_data_load
	.loc	2 3
 #   1	#include "mdl_hdr.h"
 #   2	#define T D_8012E700
 #   3	void model_data_load(int idx, int mode, int f) {
	.ent	model_data_load 2
model_data_load:
	.option	O2
	subu	$sp, 48
	sw	$31, 44($sp)
	sw	$21, 40($sp)
	sw	$20, 36($sp)
	sw	$19, 32($sp)
	sw	$18, 28($sp)
	sw	$17, 24($sp)
	sw	$16, 20($sp)
	.mask	0x803F0000, -4
	.frame	$sp, 48, $31
	move	$7, $4
	move	$19, $6
	li	$20, -1
	li	$18, 68
	la	$17, D_8012E700
	li	$6, 2
$32:
	.loc	2 3
	.loc	2 4
 #   4	    if (mode == 2) {
	bne	$5, $6, $33
	.loc	2 4
	.loc	2 5
 #   5	        { unsigned int t = T[(short)idx].flags; T[idx].flags = t | (f << 8); }
	.loc	2 5
	.noalias	$17,$sp
	sll	$14, $7, 16
	sra	$15, $14, 16
	mul	$24, $15, $18
	addu	$25, $17, $24
	.noalias	$25,$sp
	lw	$3, 0($25)
	.alias	$25,$sp
	.loc	2 5
	.noalias	$2,$sp
	mul	$8, $7, $18
	addu	$2, $17, $8
	sll	$9, $19, 8
	or	$10, $9, $3
	sw	$10, 0($2)
	.loc	2 6
 #   6	        if (T[idx].child != -1) {
	lh	$4, 22($2)
	beq	$20, $4, $38
	.alias	$2,$sp
	.loc	2 6
	.loc	2 7
 #   7	            model_data_load(T[idx].child, 3, f);
	move	$7, $4
	li	$5, 3
	b	$32
	.alias	$17,$sp
$33:
	.loc	2 9
 #   8	        }
 #   9	    } else if (mode == 0) {
	bne	$5, 0, $34
	.loc	2 9
	.loc	2 10
 #  10	        { unsigned int t = T[(short)idx].flags; T[idx].flags = t | 0x80000000; }
	.loc	2 10
	.noalias	$17,$sp
	sll	$11, $7, 16
	sra	$12, $11, 16
	mul	$13, $12, $18
	addu	$14, $17, $13
	.noalias	$14,$sp
	lw	$2, 0($14)
	.alias	$14,$sp
	.loc	2 10
	or	$15, $2, -2147483648
	mul	$24, $7, $18
	addu	$25, $17, $24
	.noalias	$25,$sp
	sw	$15, 0($25)
	.alias	$25,$sp
	b	$38
$34:
	.loc	2 11
 #  11	    } else if (mode == 1) {
	bne	$5, 1, $35
	.loc	2 11
	.loc	2 12
 #  12	        { unsigned int t = T[(short)idx].flags; T[idx].flags = t | (f << 8); }
	.loc	2 12
	sll	$8, $7, 16
	sra	$9, $8, 16
	mul	$10, $9, $18
	addu	$11, $17, $10
	.noalias	$11,$sp
	lw	$2, 0($11)
	.alias	$11,$sp
	.loc	2 12
	sll	$12, $19, 8
	or	$13, $12, $2
	mul	$14, $7, $18
	addu	$24, $17, $14
	.noalias	$24,$sp
	sw	$13, 0($24)
	.alias	$24,$sp
	b	$38
$35:
	.loc	2 13
 #  13	    } else if (mode == 3) {
	bne	$5, 3, $38
	.loc	2 13
	.loc	2 14
 #  14	        int n = idx;
	move	$2, $7
	.loc	2 15
 #  15	        do {
	sll	$21, $19, 8
$36:
	.loc	2 15
	.loc	2 16
 #  16	            { unsigned int t = T[(short)n].flags; T[n].flags = t | (f << 8); }
	.loc	2 16
	sll	$15, $2, 16
	sra	$25, $15, 16
	mul	$8, $25, $18
	addu	$9, $17, $8
	.noalias	$9,$sp
	lw	$3, 0($9)
	.alias	$9,$sp
	.loc	2 16
	.noalias	$16,$sp
	mul	$10, $2, $18
	addu	$16, $17, $10
	or	$11, $21, $3
	sw	$11, 0($16)
	.loc	2 17
 #  17	            if (T[n].child != -1) {
	lh	$4, 22($16)
	beq	$20, $4, $37
	.loc	2 17
	.loc	2 18
 #  18	                model_data_load(T[n].child, 3, f);
	li	$5, 3
	move	$6, $19
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
$37:
	.loc	2 20
 #  19	            }
 #  20	            n = T[n].sibling;
	lh	$2, 24($16)
	bne	$2, $20, $36
	.alias	$16,$sp
	.alias	$17,$sp
	.loc	2 23
 #  21	        } while (n != -1);
 #  22	    }
 #  23	}
$38:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 20($sp)
	lw	$17, 24($sp)
	lw	$18, 28($sp)
	lw	$19, 32($sp)
	lw	$20, 36($sp)
	lw	$21, 40($sp)
	lw	$31, 44($sp)
	addu	$sp, 48
	j	$31
	.end	model_data_load
