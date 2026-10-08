.set noreorder
.set noat
.globl sound_position_update
sound_position_update:
    addiu sp,sp,-24
    sw ra,20(sp)
    lw a3,8(a0)
    li t0,2
    move a2,a0
    lw v1,16(a3)
    li t1,1
    lui t6,0x8014
    beq t0,v1,L80095208
    nop
L80095208:
    beql t1,v1,L80095390
    lw ra,20(sp)
    lw t6,25088(t6)
    mtc1 a1,$f4
    lui at,0x3f40
    mtc1 t6,$f8
    cvt.s.w $f6,$f4
    mtc1 at,$f4
    cvt.s.w $f10,$f8
    mul.s $f8,$f10,$f4
    c.le.s $f8,$f6
    nop
    bc1t L800952bc
    nop
    lb a0,25(a3)
    beqz a0,L80095310
    nop
    bne t0,v1,L80095310
    nop
    mtc1 zero,$f10
    lwc1 $f4,36(a3)
    lui at,0x8012
    c.le.s $f4,$f10
    nop
    bc1f L80095310
    nop
    lwc1 $f6,14948(at)
    lui at,0x8015
    lwc1 $f14,10056(at)
    lwc1 $f0,32(a3)
    lui at,0x4661
    c.lt.s $f14,$f0
    add.s $f12,$f0,$f6
    bc1f L800952a0
    mov.s $f2,$f12
    mtc1 at,$f8
    nop
    sub.s $f2,$f12,$f8
L800952a0:
    c.lt.s $f2,$f14
    move v0,zero
    bc1f L800952b4
    nop
    li v0,1
L800952b4:
    beqz v0,L80095310
    nop
L800952bc:
    bnel t0,v1,L80095390
    lw ra,20(sp)
    jal 0x80091b00
    nop
    lui a1,0x8011
    addiu a1,a1,592
    lw v1,0(a1)
    lui at,0x8014
    li t9,5
    sll t7,v1,0x2
    addu at,at,t7
    sw v0,15600(at)
    addiu t8,v1,1
    sw t8,0(a1)
    sb t9,2(v0)
    lw t6,8(a2)
    sw t6,4(v0)
    lbu t7,26(t6)
    addiu t8,t7,1
    b L8009538c
    sb t8,26(t6)
L80095310:
    beq t0,v1,L8009538c
    lui at,0x8012
    lwc1 $f10,14952(at)
    lwc1 $f4,36(a3)
    c.lt.s $f10,$f4
    nop
    bc1t L80095344
    nop
    bnezl a0,L80095390
    lw ra,20(sp)
    lw t9,20(a3)
    bnel t1,t9,L80095390
    lw ra,20(sp)
L80095344:
    jal 0x80091b00
    nop
    lui a1,0x8011
    addiu a1,a1,588
    lw v1,0(a1)
    lui at,0x8014
    li t8,3
    sll t6,v1,0x2
    addu at,at,t6
    sw v0,15080(at)
    addiu t7,v1,1
    sw t7,0(a1)
    sb t8,2(v0)
    lw t9,8(a2)
    sw t9,4(v0)
    lbu t6,26(t9)
    addiu t7,t6,1
    sb t7,26(t9)
L8009538c:
    lw ra,20(sp)
L80095390:
    addiu sp,sp,24
    jr ra
    nop
