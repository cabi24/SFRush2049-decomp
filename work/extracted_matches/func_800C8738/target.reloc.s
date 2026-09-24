.set noreorder
.set noat
.text
.globl func_800C8738
func_800C8738:
.L800C8738:
    li      $v1,31
.L800C873C:
    li      $t6,1
.L800C8740:
    sllv    $t7,$t6,$v1
.L800C8744:
    and     $t8,$t7,$a0
.L800C8748:
    beqzl   $t8,.L800C875C
.L800C874C:
    addiu   $v1,$v1,-1
.L800C8750:
    jr      $ra
.L800C8754:
    move    $v0,$v1
.L800C8758:
    addiu   $v1,$v1,-1
.L800C875C:
    andi    $t9,$v1,0xff
.L800C8760:
    bnez    $t9,.L800C873C
.L800C8764:
    move    $v1,$t9
.L800C8768:
    move    $v0,$zero
.L800C876C:
    jr      $ra
.L800C8770:
    nop
