.set noreorder
.set noat
.text
.globl func_80092484
func_80092484:
.L80092484:
    sll     $t6,$a0,0x10
.L80092488:
    sll     $t8,$a1,0x10
.L8009248C:
    sra     $t9,$t8,0x10
.L80092490:
    sra     $t7,$t6,0x10
.L80092494:
    sw      $a0,0($sp)
.L80092498:
    move    $a0,$t7
.L8009249C:
    beqz    $t9,.L800924CC
.L800924A0:
    sw      $a1,4($sp)
.L800924A4:
    lui     $t1,%hi(D_801392D8)
.L800924A8:
    addiu   $t1,$t1,%lo(D_801392D8)
.L800924AC:
    sll     $t0,$t7,0x2
.L800924B0:
    addu    $v0,$t0,$t1
.L800924B4:
    lw      $t2,0($v0)
.L800924B8:
    lui     $at,0xffff
.L800924BC:
    ori     $at,$at,0x7fff
.L800924C0:
    and     $t3,$t2,$at
.L800924C4:
    jr      $ra
.L800924C8:
    sw      $t3,0($v0)
.L800924CC:
    lui     $t5,%hi(D_801392D8)
.L800924D0:
    addiu   $t5,$t5,%lo(D_801392D8)
.L800924D4:
    sll     $t4,$a0,0x2
.L800924D8:
    addu    $v0,$t4,$t5
.L800924DC:
    lw      $t6,0($v0)
.L800924E0:
    li      $at,-16385
.L800924E4:
    and     $t7,$t6,$at
.L800924E8:
    sw      $t7,0($v0)
.L800924EC:
    jr      $ra
.L800924F0:
    nop
