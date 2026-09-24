.set noreorder
.set noat
.text
.globl audio_update_b
audio_update_b:
.L800F73FC:
    addiu   $sp,$sp,-24
.L800F7400:
    sw      $ra,20($sp)
.L800F7404:
    beqz    $a0,.L800F7414
.L800F7408:
    sw      $a1,28($sp)
.L800F740C:
    jal     UpdateActiveObjects
.L800F7410:
    nop
.L800F7414:
    lw      $t6,28($sp)
.L800F7418:
    beqz    $t6,.L800F7428
.L800F741C:
    nop
.L800F7420:
    jal     PhysicsObjectList_Update
.L800F7424:
    nop
.L800F7428:
    jal     Effects_UpdateEmitters
.L800F742C:
    nop
.L800F7430:
    jal     Input_ProcessGameplayPad
.L800F7434:
    move    $a0,$zero
.L800F7438:
    lw      $ra,20($sp)
.L800F743C:
    addiu   $sp,$sp,24
.L800F7440:
    jr      $ra
.L800F7444:
    nop
