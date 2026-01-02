.set noat
.set noreorder
.text
.globl __isnan

__isnan:
    addiu   $sp, $sp, -0x8
    sdc1    $f12, 0x0($sp)
    lw      $t6, 0x0($sp)
    addiu   $at, $zero, 0x7FF
    sll     $t7, $t6, 1
    srl     $t8, $t7, 21
    bne     $t8, $at, .L80002CB8
    nop
    lhu     $t9, 0x0($sp)
    andi    $t0, $t9, 0x800F
    sh      $t0, 0x0($sp)
    ldc1    $f4, 0x0($sp)
    mtc1    $zero, $f7
    mtc1    $zero, $f6
    or      $v0, $zero, $zero
    c.eq.d  $f4, $f6
    nop
    bc1f    .L80002CB0
    nop
    addiu   $v0, $zero, 0x1
.L80002CB0:
    b       .L80002CC8
    nop
.L80002CB8:
    b       .L80002CC8
    or      $v0, $zero, $zero
    b       .L80002CC8
    nop
.L80002CC8:
    jr      $ra
    addiu   $sp, $sp, 0x8
