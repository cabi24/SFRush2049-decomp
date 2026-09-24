.set noreorder
.set noat
.text
.globl func_800D2458
func_800D2458:
.L800D2458:
    lui     $at,0x40c0
.L800D245C:
    mtc1    $at,$f4
.L800D2460:
    lui     $at,%hi(D_80142740)
.L800D2464:
    sll     $v0,$a0,0x2
.L800D2468:
    lui     $t6,%hi(D_80142AC8)
.L800D246C:
    addu    $at,$at,$v0
.L800D2470:
    addiu   $t6,$t6,%lo(D_80142AC8)
.L800D2474:
    addu    $v1,$v0,$t6
.L800D2478:
    swc1    $f4,%lo(D_80142740)($at)
.L800D247C:
    mtc1    $a1,$f12
.L800D2480:
    lwc1    $f6,0($v1)
.L800D2484:
    lui     $at,%hi(D_80142770)
.L800D2488:
    addiu   $sp,$sp,-24
.L800D248C:
    sub.s   $f8,$f12,$f6
.L800D2490:
    addu    $at,$at,$v0
.L800D2494:
    lui     $t8,%hi(D_80143AC8)
.L800D2498:
    sw      $ra,20($sp)
.L800D249C:
    swc1    $f8,%lo(D_80142770)($at)
.L800D24A0:
    addiu   $t8,$t8,%lo(D_80143AC8)
.L800D24A4:
    sll     $t7,$a0,0x3
.L800D24A8:
    addu    $a1,$t7,$t8
.L800D24AC:
    li      $a2,102
.L800D24B0:
    jal     func_800D2128
.L800D24B4:
    swc1    $f12,0($v1)
.L800D24B8:
    lw      $ra,20($sp)
.L800D24BC:
    addiu   $sp,$sp,24
.L800D24C0:
    jr      $ra
.L800D24C4:
    nop
