nonmatching func_80021D5C, 0x9C

glabel func_80021D5C
    /* 2295C 80021D5C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 22960 80021D60 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22964 80021D64 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 22968 80021D68 908E004A */  lbu        $t6, 0x4A($a0)
    /* 2296C 80021D6C 240100FF */  addiu      $at, $zero, 0xFF
    /* 22970 80021D70 00803025 */  or         $a2, $a0, $zero
    /* 22974 80021D74 51C1001C */  beql       $t6, $at, .L80021DE8
    /* 22978 80021D78 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 2297C 80021D7C 0C008532 */  jal        func_800214C8
    /* 22980 80021D80 AFA60018 */   sw        $a2, 0x18($sp)
    /* 22984 80021D84 8FAF001C */  lw         $t7, 0x1C($sp)
    /* 22988 80021D88 0002C1C3 */  sra        $t8, $v0, 7
    /* 2298C 80021D8C 331900FF */  andi       $t9, $t8, 0xFF
    /* 22990 80021D90 8DE30000 */  lw         $v1, 0x0($t7)
    /* 22994 80021D94 8FA60018 */  lw         $a2, 0x18($sp)
    /* 22998 80021D98 00034202 */  srl        $t0, $v1, 8
    /* 2299C 80021D9C 310900FF */  andi       $t1, $t0, 0xFF
    /* 229A0 80021DA0 0329082A */  slt        $at, $t9, $t1
    /* 229A4 80021DA4 1420000F */  bnez       $at, .L80021DE4
    /* 229A8 80021DA8 00032402 */   srl       $a0, $v1, 16
    /* 229AC 80021DAC 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 229B0 80021DB0 0C005B08 */  jal        func_80016C20
    /* 229B4 80021DB4 AFA60018 */   sw        $a2, 0x18($sp)
    /* 229B8 80021DB8 1040000A */  beqz       $v0, .L80021DE4
    /* 229BC 80021DBC 8FA60018 */   lw        $a2, 0x18($sp)
    /* 229C0 80021DC0 A8C20000 */  swl        $v0, 0x0($a2)
    /* 229C4 80021DC4 B8C20003 */  swr        $v0, 0x3($a2)
    /* 229C8 80021DC8 8FAA001C */  lw         $t2, 0x1C($sp)
    /* 229CC 80021DCC 8D4B0004 */  lw         $t3, 0x4($t2)
    /* 229D0 80021DD0 316CFFFF */  andi       $t4, $t3, 0xFFFF
    /* 229D4 80021DD4 000C68C0 */  sll        $t5, $t4, 3
    /* 229D8 80021DD8 01A27021 */  addu       $t6, $t5, $v0
    /* 229DC 80021DDC A8CE0004 */  swl        $t6, 0x4($a2)
    /* 229E0 80021DE0 B8CE0007 */  swr        $t6, 0x7($a2)
  .L80021DE4:
    /* 229E4 80021DE4 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80021DE8:
    /* 229E8 80021DE8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 229EC 80021DEC 00001025 */  or         $v0, $zero, $zero
    /* 229F0 80021DF0 03E00008 */  jr         $ra
    /* 229F4 80021DF4 00000000 */   nop
endlabel func_80021D5C
