glabel func_80006A00
    lui      $t6, %hi(D_8002C3D0)
    lui      $t7, %hi(D_8002C3D0)
    addiu    $t6, $t6, %lo(D_8002C3D0)
    addiu    $t7, $t7, %lo(D_8002C3D0)
    sw       $t6, 0x0($a0)
    sw       $t7, 0x4($a0)
    sw       $zero, 0x8($a0)
    sw       $zero, 0xC($a0)
    sw       $a2, 0x10($a0)
    jr       $ra
    sw       $a1, 0x14($a0)