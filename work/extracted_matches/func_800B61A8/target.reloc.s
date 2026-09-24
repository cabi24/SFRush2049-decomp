.set noreorder
.set noat
.text
.globl func_800B61A8
func_800B61A8:
.L800B61A8:
    lui     $t7,%hi(D_8010FFC0)
.L800B61AC:
    lb      $t7,%lo(D_8010FFC0)($t7)
.L800B61B0:
    addiu   $sp,$sp,-24
.L800B61B4:
    sw      $a3,36($sp)
.L800B61B8:
    andi    $t6,$a3,0xff
.L800B61BC:
    move    $a3,$t6
.L800B61C0:
    bnez    $t7,.L800B61D0
.L800B61C4:
    sw      $ra,20($sp)
.L800B61C8:
    b       .L800B61EC
.L800B61CC:
    li      $v0,-1
.L800B61D0:
    li      $at,-1
.L800B61D4:
    bne     $a0,$at,.L800B61E4
.L800B61D8:
    nop
.L800B61DC:
    b       .L800B61EC
.L800B61E0:
    li      $v0,-1
.L800B61E4:
    jal     entity_flags_apply
.L800B61E8:
    nop
.L800B61EC:
    lw      $ra,20($sp)
.L800B61F0:
    addiu   $sp,$sp,24
.L800B61F4:
    jr      $ra
.L800B61F8:
    nop
