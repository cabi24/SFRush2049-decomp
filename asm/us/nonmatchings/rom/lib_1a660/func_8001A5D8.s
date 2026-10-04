nonmatching func_8001A5D8, 0x80

glabel func_8001A5D8
    /* 1B1D8 8001A5D8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 1B1DC 8001A5DC 00803025 */  or         $a2, $a0, $zero
    /* 1B1E0 8001A5E0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1B1E4 8001A5E4 AFA5002C */  sw         $a1, 0x2C($sp)
    /* 1B1E8 8001A5E8 00052402 */  srl        $a0, $a1, 16
    /* 1B1EC 8001A5EC 88C5005C */  lwl        $a1, 0x5C($a2)
    /* 1B1F0 8001A5F0 308400FF */  andi       $a0, $a0, 0xFF
    /* 1B1F4 8001A5F4 0C007943 */  jal        func_8001E50C
    /* 1B1F8 8001A5F8 98C5005F */   lwr       $a1, 0x5F($a2)
    /* 1B1FC 8001A5FC 8FA3002C */  lw         $v1, 0x2C($sp)
    /* 1B200 8001A600 00022C00 */  sll        $a1, $v0, 16
    /* 1B204 8001A604 3066FFFF */  andi       $a2, $v1, 0xFFFF
    /* 1B208 8001A608 10C0000E */  beqz       $a2, .L8001A644
    /* 1B20C 8001A60C 00051C02 */   srl       $v1, $a1, 16
    /* 1B210 8001A610 3064FFFF */  andi       $a0, $v1, 0xFFFF
    /* 1B214 8001A614 AFA30018 */  sw         $v1, 0x18($sp)
    /* 1B218 8001A618 AFA50020 */  sw         $a1, 0x20($sp)
    /* 1B21C 8001A61C 0C007910 */  jal        func_8001E440
    /* 1B220 8001A620 AFA60024 */   sw        $a2, 0x24($sp)
    /* 1B224 8001A624 8FA30018 */  lw         $v1, 0x18($sp)
    /* 1B228 8001A628 8FA60024 */  lw         $a2, 0x24($sp)
    /* 1B22C 8001A62C 8FA50020 */  lw         $a1, 0x20($sp)
    /* 1B230 8001A630 00437023 */  subu       $t6, $v0, $v1
    /* 1B234 8001A634 01C60019 */  multu      $t6, $a2
    /* 1B238 8001A638 00007812 */  mflo       $t7
    /* 1B23C 8001A63C 00AF2821 */  addu       $a1, $a1, $t7
    /* 1B240 8001A640 00000000 */  nop
  .L8001A644:
    /* 1B244 8001A644 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1B248 8001A648 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 1B24C 8001A64C 00A01025 */  or         $v0, $a1, $zero
    /* 1B250 8001A650 03E00008 */  jr         $ra
    /* 1B254 8001A654 00000000 */   nop
endlabel func_8001A5D8
