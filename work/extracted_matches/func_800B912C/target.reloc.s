.set noreorder
.set noat
.text
.globl func_800B912C
func_800B912C:
.L800B912C:
    lui     $at,%hi(D_80149818)
.L800B9130:
    addiu   $sp,$sp,-24
.L800B9134:
    sw      $zero,%lo(D_80149818)($at)
.L800B9138:
    sw      $ra,20($sp)
.L800B913C:
    lui     $at,%hi(D_80149B80)
.L800B9140:
    sw      $a0,24($sp)
.L800B9144:
    jal     func_800B90F8
.L800B9148:
    sw      $zero,%lo(D_80149B80)($at)
.L800B914C:
    lui     $a0,%hi(D_80143A18)
.L800B9150:
    jal     func_80096130
.L800B9154:
    lw      $a0,%lo(D_80143A18)($a0)
.L800B9158:
    lui     $a0,%hi(D_80143A20)
.L800B915C:
    jal     func_80096130
.L800B9160:
    lw      $a0,%lo(D_80143A20)($a0)
.L800B9164:
    lui     $a0,%hi(D_80143A28)
.L800B9168:
    jal     func_80096130
.L800B916C:
    lw      $a0,%lo(D_80143A28)($a0)
.L800B9170:
    lui     $a0,%hi(D_80143A10)
.L800B9174:
    addiu   $a0,$a0,%lo(D_80143A10)
.L800B9178:
    move    $a1,$zero
.L800B917C:
    jal     memset
.L800B9180:
    li      $a2,44
.L800B9184:
    lw      $ra,20($sp)
.L800B9188:
    addiu   $sp,$sp,24
.L800B918C:
    jr      $ra
.L800B9190:
    nop
