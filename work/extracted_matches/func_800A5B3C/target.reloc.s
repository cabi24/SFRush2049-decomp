.set noreorder
.set noat
.text
.globl func_800A5B3C
func_800A5B3C:
.L800A5B3C:
    lui     $at,%hi(D_80156990)
.L800A5B40:
    sw      $zero,%lo(D_80156990)($at)
.L800A5B44:
    lui     $at,%hi(D_80156CE0)
.L800A5B48:
    sw      $zero,%lo(D_80156CE0)($at)
.L800A5B4C:
    lui     $at,%hi(D_801613AC)
.L800A5B50:
    sw      $zero,%lo(D_801613AC)($at)
.L800A5B54:
    lui     $at,%hi(D_80156BAC)
.L800A5B58:
    sw      $zero,%lo(D_80156BAC)($at)
.L800A5B5C:
    lui     $at,%hi(D_801613B4)
.L800A5B60:
    addiu   $sp,$sp,-24
.L800A5B64:
    sw      $zero,%lo(D_801613B4)($at)
.L800A5B68:
    sw      $ra,20($sp)
.L800A5B6C:
    lui     $at,%hi(D_8015B254)
.L800A5B70:
    li      $t6,-1
.L800A5B74:
    jal     func_800A5A40
.L800A5B78:
    sh      $t6,%lo(D_8015B254)($at)
.L800A5B7C:
    lui     $at,0x3f80
.L800A5B80:
    mtc1    $at,$f4
.L800A5B84:
    lui     $at,%hi(D_801613B8)
.L800A5B88:
    lw      $ra,20($sp)
.L800A5B8C:
    swc1    $f4,%lo(D_801613B8)($at)
.L800A5B90:
    lui     $at,%hi(D_80140618)
.L800A5B94:
    sh      $zero,%lo(D_80140618)($at)
.L800A5B98:
    lui     $t7,%hi(D_8011EA18)
.L800A5B9C:
    addiu   $t7,$t7,%lo(D_8011EA18)
.L800A5BA0:
    lui     $at,%hi(D_801406B8)
.L800A5BA4:
    sw      $t7,%lo(D_801406B8)($at)
.L800A5BA8:
    jr      $ra
.L800A5BAC:
    addiu   $sp,$sp,24
