.set noreorder
.set noat
.globl assign_drones
assign_drones:
addiu $sp,$sp,-136
lui $v0,0x8015
lui $t6,0x8015
lb $v0,-26740($v0)
lb $t6,9584($t6)
sw $ra,60($sp)
sw $s8,56($sp)
sw $s7,52($sp)
sw $s6,48($sp)
sw $s5,44($sp)
sw $s4,40($sp)
sw $s3,36($sp)
sw $s2,32($sp)
sw $s1,28($sp)
sw $s0,24($sp)
move $ra,$v0
beqz $t6,.L800f5040
sw $v0,128($sp)
addiu $t8,$v0,19
addiu $ra,$v0,6
sw $t8,128($sp)
.L800f5040:
lui $t9,0x8015
lh $t9,-24312($t9)
lui $s4,0x8015
addiu $s4,$s4,-24296
blez $t9,.L800f5490
move $s7,$zero
lui $at,0x4404
lui $s8,0x8015
lui $t5,0x8015
lui $t4,0x8015
mtc1 $at,$f14
mtc1 $zero,$f12
addiu $t4,$t4,6848
addiu $t5,$t5,5776
addiu $s8,$s8,10264
li $s3,5
li $t3,1
.L800f5084:
lw $v0,72($s4)
sll $s6,$ra,0x2
subu $s6,$s6,$ra
bnez $v0,.L800f50b0
move $t1,$zero
lbu $t6,1($s4)
lui $t8,0x8014
addiu $t8,$t8,24912
sll $t7,$t6,0x2
addu $v0,$t7,$t8
sw $v0,72($s4)
.L800f50b0:
lw $t6,0($v0)
sll $s6,$s6,0x5
lui $t8,0x8014
lw $t7,44($t6)
lbu $t9,131($sp)
beqz $t7,.L800f5490
addiu $t8,$t8,16408
addu $s2,$s7,$t8
sw $t9,64($sp)
.L800f50d4:
bnez $t1,.L800f50f8
lui $t6,0x8015
lw $t6,72($s4)
lw $t7,0($t6)
lw $t8,44($t7)
lw $t9,0($t8)
addu $s0,$t9,$s6
b .L800f5100
addiu $s0,$s0,140
.L800f50f8:
addiu $t6,$t6,3976
addu $s0,$s6,$t6
.L800f5100:
lbu $t7,0($s4)
move $t0,$zero
move $s1,$zero
sll $t8,$t7,0x4
subu $t8,$t8,$t7
sll $t8,$t8,0x3
subu $t8,$t8,$t7
sll $t8,$t8,0x3
addu $v0,$s8,$t8
lb $t9,239($v0)
move $s5,$v0
move $t2,$s0
beqzl $t9,.L800f530c
lhu $t6,70($s0)
lwc1 $f2,240($v0)
.L800f513c:
lwc1 $f0,44($t2)
c.eq.s $f12,$f0
    nop
bc1tl .L800f5160
slti $at,$t0,4
c.lt.s $f2,$f0
    nop
bc1f .L800f52f8
slti $at,$t0,4
.L800f5160:
beqz $at,.L800f52b8
li $a1,4
li $t6,4
subu $v0,$t6,$t0
andi $t7,$v0,0x3
negu $v0,$t7
beqz $v0,.L800f51d0
addiu $a3,$v0,4
sll $a2,$a1,0x2
addu $a0,$s0,$a2
.L800f5188:
lwc1 $f4,40($a0)
addiu $a0,$a0,-4
bne $t1,$t3,.L800f51c0
swc1 $f4,48($a0)
sll $t9,$ra,0x4
subu $t9,$t9,$ra
sll $t9,$t9,0x2
addu $t6,$t5,$t9
addu $v1,$t6,$a2
addu $v0,$t4,$a1
lb $t8,9($v0)
lw $t7,36($v1)
sb $t8,10($v0)
sw $t7,40($v1)
.L800f51c0:
addiu $a1,$a1,-1
bne $a3,$a1,.L800f5188
addiu $a2,$a2,-4
beq $t0,$a1,.L800f52b8
.L800f51d0:
sll $a2,$a1,0x2
addu $a0,$s0,$a2
.L800f51d8:
lwc1 $f6,40($a0)
addu $v0,$t4,$a1
bne $t1,$t3,.L800f520c
swc1 $f6,44($a0)
sll $t9,$ra,0x4
subu $t9,$t9,$ra
sll $t9,$t9,0x2
addu $t6,$t5,$t9
addu $v1,$t6,$a2
lb $t8,9($v0)
lw $t7,36($v1)
sb $t8,10($v0)
sw $t7,40($v1)
.L800f520c:
lwc1 $f8,36($a0)
addu $v0,$t4,$a1
bne $t1,$t3,.L800f5240
swc1 $f8,40($a0)
sll $t9,$ra,0x4
subu $t9,$t9,$ra
sll $t9,$t9,0x2
addu $t6,$t5,$t9
addu $v1,$t6,$a2
lw $t7,32($v1)
lb $t8,8($v0)
sw $t7,36($v1)
sb $t8,9($v0)
.L800f5240:
lwc1 $f10,32($a0)
addu $v0,$t4,$a1
bne $t1,$t3,.L800f5274
swc1 $f10,36($a0)
sll $t9,$ra,0x4
subu $t9,$t9,$ra
sll $t9,$t9,0x2
addu $t6,$t5,$t9
addu $v1,$t6,$a2
lw $t7,28($v1)
lb $t8,7($v0)
sw $t7,32($v1)
sb $t8,8($v0)
.L800f5274:
lwc1 $f16,28($a0)
addiu $a0,$a0,-16
bne $t1,$t3,.L800f52ac
swc1 $f16,48($a0)
sll $t9,$ra,0x4
subu $t9,$t9,$ra
sll $t9,$t9,0x2
addu $t6,$t5,$t9
addu $v1,$t6,$a2
addu $v0,$t4,$a1
lb $t8,6($v0)
lw $t7,24($v1)
sb $t8,7($v0)
sw $t7,28($v1)
.L800f52ac:
addiu $a1,$a1,-4
bne $t0,$a1,.L800f51d8
addiu $a2,$a2,-16
.L800f52b8:
bne $t1,$t3,.L800f5308
swc1 $f2,44($t2)
addu $t8,$t4,$a1
sb $s7,10($t8)
lw $v0,72($s4)
sll $t7,$ra,0x4
subu $t7,$t7,$ra
lw $t9,0($v0)
sll $t7,$t7,0x2
addu $t8,$t5,$t7
lw $t6,8($t9)
addu $t9,$t8,$s1
beqzl $t6,.L800f530c
lhu $t6,70($s0)
b .L800f5308
sw $v0,40($t9)
.L800f52f8:
addiu $t0,$t0,1
addiu $s1,$s1,4
bne $t0,$s3,.L800f513c
addiu $t2,$t2,4
.L800f5308:
lhu $t6,70($s0)
.L800f530c:
lhu $t8,84($s0)
addiu $t1,$t1,1
addiu $t7,$t6,1
addiu $t9,$t8,1
sh $t7,70($s0)
sh $t9,84($s0)
lhu $t7,64($s4)
lhu $t6,78($s0)
lhu $t9,68($s0)
move $a1,$zero
addu $t8,$t6,$t7
sh $t8,78($s0)
lbu $t6,0($s2)
addu $t7,$t9,$t6
sh $t7,68($s0)
lbu $t8,0($s2)
lui $t6,0x8015
addiu $t6,$t6,-25992
blez $t8,.L800f5388
sll $t9,$s7,0x5
addu $v0,$t9,$t6
lwc1 $f18,64($s0)
.L800f5364:
lwc1 $f4,0($v0)
addiu $a1,$a1,1
addiu $v0,$v0,4
add.s $f6,$f18,$f4
swc1 $f6,64($s0)
lbu $t7,0($s2)
slt $at,$a1,$t7
bnezl $at,.L800f5364
lwc1 $f18,64($s0)
.L800f5388:
lw $t8,88($s0)
lui $at,0x4f80
mtc1 $t8,$f8
bgez $t8,.L800f53a8
cvt.s.w $f10,$f8
mtc1 $at,$f16
    nop
add.s $f10,$f10,$f16
.L800f53a8:
lwc1 $f18,264($s5)
li $t6,1
lui $at,0x4f00
div.s $f4,$f18,$f14
add.s $f6,$f10,$f4
cfc1 $t9,c1_fcsr
ctc1 $t6,c1_fcsr
    nop
cvt.w.s $f8,$f6
cfc1 $t6,c1_fcsr
    nop
andi $t6,$t6,0x78
beqzl $t6,.L800f5428
mfc1 $t6,$f8
mtc1 $at,$f8
li $t6,1
sub.s $f8,$f6,$f8
ctc1 $t6,c1_fcsr
    nop
cvt.w.s $f8,$f8
cfc1 $t6,c1_fcsr
    nop
andi $t6,$t6,0x78
bnez $t6,.L800f541c
    nop
mfc1 $t6,$f8
lui $at,0x8000
b .L800f5434
or $t6,$t6,$at
.L800f541c:
b .L800f5434
li $t6,-1
mfc1 $t6,$f8
.L800f5428:
    nop
bltz $t6,.L800f541c
    nop
.L800f5434:
li $at,2
ctc1 $t9,c1_fcsr
bne $t1,$at,.L800f50d4
sw $t6,88($s0)
sw $ra,132($sp)
lw $a0,72($s4)
jal 0x800cd8ec
lbu $a1,67($sp)
lui $t7,0x8015
lh $t7,-24312($t7)
lui $at,0x4404
addiu $s7,$s7,1
mtc1 $at,$f14
lui $t4,0x8015
lui $t5,0x8015
mtc1 $zero,$f12
slt $at,$s7,$t7
addiu $s4,$s4,76
addiu $t5,$t5,5776
addiu $t4,$t4,6848
li $t3,1
bnez $at,.L800f5084
lw $ra,132($sp)
.L800f5490:
lw $ra,60($sp)
lw $s0,24($sp)
lw $s1,28($sp)
lw $s2,32($sp)
lw $s3,36($sp)
lw $s4,40($sp)
lw $s5,44($sp)
lw $s6,48($sp)
lw $s7,52($sp)
lw $s8,56($sp)
jr $ra
addiu $sp,$sp,136
