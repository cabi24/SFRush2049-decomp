.set noreorder
.set noat
.text
.globl func_80095F8C
func_80095F8C:
.L80095F8C:
    lui     $v0,%hi(D_801527C8)
.L80095F90:
    lw      $v0,%lo(D_801527C8)($v0)
.L80095F94:
    move    $v1,$zero
.L80095F98:
    beqz    $v0,.L80095FD0
.L80095F9C:
    nop
.L80095FA0:
    lw      $t6,8($v0)
.L80095FA4:
    sltu    $at,$a0,$t6
.L80095FA8:
    bnezl   $at,.L80095FC8
.L80095FAC:
    lw      $v0,4($v0)
.L80095FB0:
    lw      $t7,16($v0)
.L80095FB4:
    sltu    $at,$a0,$t7
.L80095FB8:
    beqzl   $at,.L80095FC8
.L80095FBC:
    lw      $v0,4($v0)
.L80095FC0:
    move    $v1,$v0
.L80095FC4:
    lw      $v0,4($v0)
.L80095FC8:
    bnezl   $v0,.L80095FA4
.L80095FCC:
    lw      $t6,8($v0)
.L80095FD0:
    jr      $ra
.L80095FD4:
    move    $v0,$v1
