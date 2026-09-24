.set noreorder
.set noat
.text
.globl func_8008C720
func_8008C720:
.L8008C720:
    mtc1    $zero,$f4
.L8008C724:
    addiu   $sp,$sp,-24
.L8008C728:
    sw      $ra,20($sp)
.L8008C72C:
    c.lt.s  $f4,$f12
.L8008C730:
    mov.s   $f14,$f12
.L8008C734:
    bc1f    .L8008C74C
.L8008C738:
    nop
.L8008C73C:
    jal     func_8008C680
.L8008C740:
    nop
.L8008C744:
    b       .L8008C75C
.L8008C748:
    lw      $ra,20($sp)
.L8008C74C:
    jal     func_8008C680
.L8008C750:
    neg.s   $f12,$f14
.L8008C754:
    neg.s   $f0,$f0
.L8008C758:
    lw      $ra,20($sp)
.L8008C75C:
    addiu   $sp,$sp,24
.L8008C760:
    jr      $ra
.L8008C764:
    nop
