.set noreorder
.set noat
.text
.globl func_800FBF2C
func_800FBF2C:
.L800FBF2C:
    addiu   $sp,$sp,-24
.L800FBF30:
    sw      $ra,20($sp)
.L800FBF34:
    jal     viGetTimeToDeadline
.L800FBF38:
    nop
.L800FBF3C:
    lui     $at,%hi(D_801247F8)
.L800FBF40:
    lwc1    $f4,%lo(D_801247F8)($at)
.L800FBF44:
    lui     $v0,%hi(D_80152032)
.L800FBF48:
    addiu   $v0,$v0,%lo(D_80152032)
.L800FBF4C:
    add.s   $f6,$f0,$f4
.L800FBF50:
    lui     $at,%hi(state_word_b)
.L800FBF54:
    lui     $t9,0x10
.L800FBF58:
    trunc.w.s $f8,$f6
.L800FBF5C:
    mfc1    $t7,$f8
.L800FBF60:
    nop
.L800FBF64:
    sh      $t7,0($v0)
.L800FBF68:
    lh      $t8,0($v0)
.L800FBF6C:
    bgtzl   $t8,.L800FBF7C
.L800FBF70:
    lw      $ra,20($sp)
.L800FBF74:
    sw      $t9,%lo(state_word_b)($at)
.L800FBF78:
    lw      $ra,20($sp)
.L800FBF7C:
    addiu   $sp,$sp,24
.L800FBF80:
    jr      $ra
.L800FBF84:
    nop
