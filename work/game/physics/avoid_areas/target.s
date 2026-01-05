# Source: game_code.bin (decompressed)
# Address: 0x800A1800

glabel avoid_areas
    /* 800A1800 000C6840 */  sll $t5, $t4, 1
    /* 800A1804 010D7021 */  addu $t6, $t0, $t5
    /* 800A1808 95CF0000 */  lhu $t7, 0($t6)
    /* 800A180C 000FC203 */  sra $t8, $t7, 8
    /* 800A1810 A058FFFE */  sb $t8, -2($v0)
    /* 800A1814 90F9FFFF */  lbu $t9, -1($a3)
    /* 800A1818 00195840 */  sll $t3, $t9, 1
    /* 800A181C 010B6021 */  addu $t4, $t0, $t3
    /* 800A1820 958E0000 */  lhu $t6, 0($t4)
    /* 800A1824 14E4FFD2 */  bne $a3, $a0, 0x800A1770
    /* 800A1828 A04EFFFF */  sb $t6, -1($v0)
    /* 800A182C A0400000 */  sb $zero, 0($v0)
    /* 800A1830 24420001 */  addiu $v0, $v0, 1
    /* 800A1834 10000034 */  beq $zero, $zero, 0x800A1908
    /* 800A1838 A0400000 */  sb $zero, 0($v0)
    /* 800A183C 18C00030 */  blez $a2, 0x800A1900
    /* 800A1840 00C02825 */  or $a1, $a2, $zero
    /* 800A1844 30C80003 */  andi $t0, $a2, 0x0003
    /* 800A1848 11000010 */  beq $t0, $zero, 0x800A188C
    /* 800A184C 01001025 */  or $v0, $t0, $zero
    /* 800A1850 27AF0020 */  addiu $t7, $sp, 32
    /* 800A1854 3C088012 */  lui $t0, 0x8012
    /* 800A1858 2508EAEC */  addiu $t0, $t0, -5396
    /* 800A185C 000F3821 */  addu $a3, $zero, $t7
    /* 800A1860 00803021 */  addu $a2, $a0, $zero
    /* 800A1864 90F80000 */  lbu $t8, 0($a3)
    /* 800A1868 24630001 */  addiu $v1, $v1, 1
    /* 800A186C 24E70001 */  addiu $a3, $a3, 1
    /* 800A1870 0018C840 */  sll $t9, $t8, 1
    /* 800A1874 01195821 */  addu $t3, $t0, $t9
    /* 800A1878 956C0000 */  lhu $t4, 0($t3)
    /* 800A187C 24C60001 */  addiu $a2, $a2, 1
    /* 800A1880 1443FFF8 */  bne $v0, $v1, 0x800A1864
    /* 800A1884 A0CCFFFF */  sb $t4, -1($a2)
    /* 800A1888 1065001D */  beq $v1, $a1, 0x800A1900
    /* 800A188C 3C088012 */  lui $t0, 0x8012
    /* 800A1890 27AD0020 */  addiu $t5, $sp, 32
    /* 800A1894 006D3821 */  addu $a3, $v1, $t5
    /* 800A1898 2508EAEC */  addiu $t0, $t0, -5396
    /* 800A189C 00833021 */  addu $a2, $a0, $v1
    /* 800A18A0 90EE0000 */  lbu $t6, 0($a3)
    /* 800A18A4 24630004 */  addiu $v1, $v1, 4
    /* 800A18A8 24E70004 */  addiu $a3, $a3, 4
    /* 800A18AC 000E7840 */  sll $t7, $t6, 1
    /* 800A18B0 010FC021 */  addu $t8, $t0, $t7
    /* 800A18B4 97190000 */  lhu $t9, 0($t8)
    /* 800A18B8 24C60004 */  addiu $a2, $a2, 4
    /* 800A18BC A0D9FFFC */  sb $t9, -4($a2)
    /* 800A18C0 90EBFFFD */  lbu $t3, -3($a3)
    /* 800A18C4 000B6040 */  sll $t4, $t3, 1
    /* 800A18C8 010C6821 */  addu $t5, $t0, $t4
    /* 800A18CC 95AE0000 */  lhu $t6, 0($t5)
    /* 800A18D0 A0CEFFFD */  sb $t6, -3($a2)
    /* 800A18D4 90EFFFFE */  lbu $t7, -2($a3)
    /* 800A18D8 000FC040 */  sll $t8, $t7, 1
    /* 800A18DC 0118C821 */  addu $t9, $t0, $t8
    /* 800A18E0 972B0000 */  lhu $t3, 0($t9)
    /* 800A18E4 A0CBFFFE */  sb $t3, -2($a2)
    /* 800A18E8 90ECFFFF */  lbu $t4, -1($a3)
    /* 800A18EC 000C6840 */  sll $t5, $t4, 1
    /* 800A18F0 010D7021 */  addu $t6, $t0, $t5
    /* 800A18F4 95CF0000 */  lhu $t7, 0($t6)
    /* 800A18F8 1465FFE9 */  bne $v1, $a1, 0x800A18A0
    /* 800A18FC A0CFFFFF */  sb $t7, -1($a2)
    /* 800A1900 0083C021 */  addu $t8, $a0, $v1
    /* 800A1904 A3000000 */  sb $zero, 0($t8)
    /* 800A1908 03E00008 */  jr $ra
    /* 800A190C 27BD0068 */  addiu $sp, $sp, 104
