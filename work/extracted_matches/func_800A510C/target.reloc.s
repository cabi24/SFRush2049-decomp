.set noreorder
.set noat
.text
.globl func_800A510C
func_800A510C:
.L800A510C:
    lui     $a0,%hi(D_80140A10)
.L800A5110:
    lui     $a1,%hi(D_8013FEC8)
.L800A5114:
    lh      $a1,%lo(D_8013FEC8)($a1)
.L800A5118:
    lbu     $a0,%lo(D_80140A10)($a0)
.L800A511C:
    addiu   $sp,$sp,-24
.L800A5120:
    sw      $ra,20($sp)
.L800A5124:
    beql    $a0,$a1,.L800A514C
.L800A5128:
    lw      $ra,20($sp)
.L800A512C:
    jal     suspension_setup
.L800A5130:
    nop
.L800A5134:
    lui     $t6,%hi(D_8013FEC8)
.L800A5138:
    lh      $t6,%lo(D_8013FEC8)($t6)
.L800A513C:
    lui     $at,%hi(D_80140A10)
.L800A5140:
    jal     func_800A4E58
.L800A5144:
    sb      $t6,%lo(D_80140A10)($at)
.L800A5148:
    lw      $ra,20($sp)
.L800A514C:
    addiu   $sp,$sp,24
.L800A5150:
    jr      $ra
.L800A5154:
    nop
