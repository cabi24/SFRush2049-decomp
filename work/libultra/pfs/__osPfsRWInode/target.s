# Source: F700.s
# Address: 0x8000F3A4

glabel func_8000F3A4
    /* FFA4 8000F3A4 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* FFA8 8000F3A8 AFB40030 */  sw         $s4, 0x30($sp)
    /* FFAC 8000F3AC 30D400FF */  andi       $s4, $a2, 0xFF
    /* FFB0 8000F3B0 AFB20028 */  sw         $s2, 0x28($sp)
    /* FFB4 8000F3B4 00809025 */  or         $s2, $a0, $zero
    /* FFB8 8000F3B8 AFBF0034 */  sw         $ra, 0x34($sp)
    /* FFBC 8000F3BC AFB3002C */  sw         $s3, 0x2C($sp)
    /* FFC0 8000F3C0 AFB10024 */  sw         $s1, 0x24($sp)
    /* FFC4 8000F3C4 AFB00020 */  sw         $s0, 0x20($sp)
    /* FFC8 8000F3C8 AFA5005C */  sw         $a1, 0x5C($sp)
    /* FFCC 8000F3CC AFA60060 */  sw         $a2, 0x60($sp)
    /* FFD0 8000F3D0 16800011 */  bnez       $s4, . + 4 + (0x11 << 2)
    /* FFD4 8000F3D4 AFA70064 */   sw        $a3, 0x64($sp)
    /* FFD8 8000F3D8 3C0F8003 */  lui        $t7, %hi(__osSiChannelMask)
    /* FFDC 8000F3DC 91EFC4D4 */  lbu        $t7, %lo(__osSiChannelMask)($t7)
    /* FFE0 8000F3E0 93B80067 */  lbu        $t8, 0x67($sp)
    /* FFE4 8000F3E4 3C198003 */  lui        $t9, %hi(__osSiLastChannel)
    /* FFE8 8000F3E8 55F8000C */  bnel       $t7, $t8, . + 4 + (0xC << 2)
    /* FFEC 8000F3EC 92490065 */   lbu       $t1, 0x65($s2)
    /* FFF0 8000F3F0 8F39C4D0 */  lw         $t9, %lo(__osSiLastChannel)($t9)
    /* FFF4 8000F3F4 8C880008 */  lw         $t0, 0x8($a0)
    /* FFF8 8000F3F8 3C048003 */  lui        $a0, %hi(__osContPifInode)
    /* FFFC 8000F3FC 24847CB0 */  addiu      $a0, $a0, %lo(__osContPifInode)