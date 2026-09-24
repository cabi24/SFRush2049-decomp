.set noreorder
.set noat
.text
.globl object_process_thunk
object_process_thunk:
.L800A7DF0:
    addiu   $sp,$sp,-24
.L800A7DF4:
    sw      $ra,20($sp)
.L800A7DF8:
    jal     func_800A5B3C
.L800A7DFC:
    nop
.L800A7E00:
    lw      $ra,20($sp)
.L800A7E04:
    addiu   $sp,$sp,24
.L800A7E08:
    jr      $ra
.L800A7E0C:
    nop
