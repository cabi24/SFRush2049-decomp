.set noreorder
.set noat
.text
.globl collision_check_thunk
collision_check_thunk:
.L800EE88C:
    addiu   $sp,$sp,-24
.L800EE890:
    sw      $ra,20($sp)
.L800EE894:
    jal     particle_velocity_set
.L800EE898:
    nop
.L800EE89C:
    lw      $ra,20($sp)
.L800EE8A0:
    addiu   $sp,$sp,24
.L800EE8A4:
    jr      $ra
.L800EE8A8:
    nop
