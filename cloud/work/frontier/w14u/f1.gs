.set noreorder
.set noat
.globl func_800F45F8
func_800F45F8:
    lui v0,0x8015
    lbu v0,17364(v0)
    lui t7,0x8015
    addiu sp,sp,-80
    sll t6,v0,0x2
    addu t6,t6,v0
    sll t6,t6,0x2
    subu t6,t6,v0
    sll t6,t6,0x2
    addu t7,t7,t6
    lw t7,-24224(t7)
    sw ra,20(sp)
    lui t6,0x8015
    lw t8,0(t7)
    lh t6,-24312(t6)
    mtc1 a1,$f12
    lw t9,44(t8)
    li t4,1
    bne t4,t6,.L800f4650
    lw t5,0(t9)
    b .L800f4654
    move v1,zero
.L800f4650:
    li v1,2
.L800f4654:
    li t7,6
    subu a1,t7,v1
    blez v0,.L800f4684
    move t2,a1
    lbu v1,1(a0)
    lbu t8,0(a0)
    xor v0,t8,v1
    xor t7,v1,v0
    xor t8,v0,t7
    sb v0,0(a0)
    sb t8,0(a0)
    sb t7,1(a0)
.L800f4684:
    blez a1,.L800f4714
    move t1,zero
    lui t3,0x8015
    addiu t3,t3,17960
    move a1,a0
    addiu a3,t5,1780
    li t0,3
.L800f46a0:
    lb t9,0(t3)
    addiu t1,t1,1
    move a2,zero
    multu t9,t0
    mflo a0
.L800f46bc:
    sra t6,a0,0x3
    addu v1,a3,t6
    andi v0,a0,0x7
    li t8,1
    sllv t9,t8,v0
    lbu t7,20(v1)
    nor t6,t9,zero
    lbu t9,0(a1)
    and t8,t7,t6
    addiu a2,a2,1
    andi t7,t9,0x1
    sllv t6,t7,v0
    or t9,t6,t8
    sb t9,20(v1)
    lbu t7,0(a1)
    addiu a0,a0,1
    srl t6,t7,0x1
    bne a2,t0,.L800f46bc
    sb t6,0(a1)
    addiu a1,a1,1
    bne t1,t2,.L800f46a0
    addiu a3,a3,9
.L800f4714:
    addiu t2,t5,1780
    lwc1 $f4,16(t2)
    lb t8,9(t2)
    add.s $f6,$f4,$f12
    addiu t9,t8,1
    sb t9,9(t2)
    swc1 $f6,16(t2)
    jal 0x800f34d8
    sw t2,28(sp)
    lui t3,0x8015
    addiu t3,t3,17960
    lb t7,0(t3)
    lw t2,28(sp)
    lui t9,0x8015
    addiu t6,t7,-1
    sb t6,0(t3)
    lb t9,17984(t9)
    lb t8,9(t2)
    li t0,3
    li t4,1
    slt at,t8,t9
    bnezl at,.L800f4788
    lb t7,8(t2)
    jal 0x800f43b8
    sw t2,28(sp)
    li t0,3
    lw t2,28(sp)
    li t4,1
    lb t7,8(t2)
.L800f4788:
    lui t6,0x8015
    li t1,2
    bne t0,t7,.L800f4940
    nop
    lh t6,-24312(t6)
    lui a2,0x8015
    addiu a2,a2,17488
    bne t4,t6,.L800f47bc
    li t0,76
    lui a2,0x8015
    addiu a2,a2,17488
    b .L800f47d8
    lbu a3,1(a2)
.L800f47bc:
    lbu v0,1(a2)
    lbu v1,77(a2)
    slt at,v0,v1
    beqz at,.L800f47d8
    move a3,v1
    b .L800f47d8
    move a3,v0
.L800f47d8:
    lui v1,0x8015
    lbu v0,17489(v1)
    lui v1,0x8015
    bnez v0,.L800f47f4
    nop
    b .L800f4800
    sw zero,48(sp)
.L800f47f4:
    bne t4,v0,.L800f4800
    nop
    sw zero,44(sp)
.L800f4800:
    lbu v0,17565(v1)
    lw a0,48(sp)
    lw a1,44(sp)
    bnez v0,.L800f481c
    nop
    b .L800f4828
    move a0,t4
.L800f481c:
    bne t4,v0,.L800f4828
    nop
    move a1,t4
.L800f4828:
    multu t1,t0
    mflo t8
    addu v1,a2,t8
    lbu v0,1(v1)
    bnez v0,.L800f4848
    nop
    b .L800f4854
    move a0,t1
.L800f4848:
    bnel t4,v0,.L800f4858
    lbu v0,77(v1)
    move a1,t1
.L800f4854:
    lbu v0,77(v1)
.L800f4858:
    bnez v0,.L800f4868
    nop
    b .L800f4874
    addiu a0,t1,1
.L800f4868:
    bnel t4,v0,.L800f4878
    lbu v0,153(v1)
    addiu a1,t1,1
.L800f4874:
    lbu v0,153(v1)
.L800f4878:
    bnez v0,.L800f4888
    nop
    b .L800f4894
    addiu a0,t1,2
.L800f4888:
    bnel t4,v0,.L800f4898
    lbu v0,229(v1)
    addiu a1,t1,2
.L800f4894:
    lbu v0,229(v1)
.L800f4898:
    bnez v0,.L800f48a8
    nop
    b .L800f48b4
    addiu a0,t1,3
.L800f48a8:
    bne t4,v0,.L800f48b4
    nop
    addiu a1,t1,3
.L800f48b4:
    multu a0,t0
    lbu v0,7(t2)
    slti at,v0,5
    mflo t9
    addu t7,a2,t9
    lhu t6,2(t7)
    multu a1,t0
    mflo t8
    addu t9,a2,t8
    lhu t7,2(t9)
    bnez a3,.L800f48f4
    subu v1,t6,t7
    beqz at,.L800f48f4
    slti at,v1,20
    beqzl at,.L800f493c
    addiu t8,v0,1
.L800f48f4:
    bnez a3,.L800f4908
    slti at,v0,4
    beqz at,.L800f4908
    slti at,v1,6
    beqz at,.L800f4938
.L800f4908:
    slti at,a3,2
    beqz at,.L800f4918
    slti at,v0,3
    bnez at,.L800f4938
.L800f4918:
    slti at,a3,3
    beqz at,.L800f4928
    slti at,v0,2
    bnez at,.L800f4938
.L800f4928:
    slti at,a3,4
    beqz at,.L800f4940
    nop
    bgtz v0,.L800f4940
.L800f4938:
    addiu t8,v0,1
.L800f493c:
    sb t8,7(t2)
.L800f4940:
    lui t9,0x8015
    lbu t9,17364(t9)
    lui a0,0x8015
    sll t6,t9,0x2
    addu t6,t6,t9
    sll t6,t6,0x2
    subu t6,t6,t9
    sll t6,t6,0x2
    addu a0,a0,t6
    jal 0x800cd748
    lw a0,-24224(a0)
    lw ra,20(sp)
    addiu sp,sp,80
    jr ra
    nop
