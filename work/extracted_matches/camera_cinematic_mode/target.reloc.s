.set noreorder
.set noat
.text
.globl camera_cinematic_mode
camera_cinematic_mode:
.L800BE4B4:
    addiu   $sp,$sp,-24
.L800BE4B8:
    sw      $ra,20($sp)
.L800BE4BC:
    sw      $a0,24($sp)
.L800BE4C0:
    sw      $a1,28($sp)
.L800BE4C4:
    sw      $a3,36($sp)
.L800BE4C8:
    jal     dispatch_handler
.L800BE4CC:
    move    $a0,$a2
.L800BE4D0:
    lh      $a0,26($sp)
.L800BE4D4:
    lh      $a1,30($sp)
.L800BE4D8:
    jal     state_utility
.L800BE4DC:
    lw      $a2,36($sp)
.L800BE4E0:
    lw      $ra,20($sp)
.L800BE4E4:
    addiu   $sp,$sp,24
.L800BE4E8:
    jr      $ra
.L800BE4EC:
    nop
