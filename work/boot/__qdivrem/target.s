# Source: E4F0.s
# Address: 0x8000DA88

glabel func_8000DA88
    /* E688 8000DA88 87AF0012 */  lh         $t7, 0x12($sp)
    /* E68C 8000DA8C AFA60008 */  sw         $a2, 0x8($sp)
    /* E690 8000DA90 AFA7000C */  sw         $a3, 0xC($sp)
    /* E694 8000DA94 DFAE0008 */  ld         $t6, 0x8($sp)
    /* E698 8000DA98 01E0C025 */  or         $t8, $t7, $zero
    /* E69C 8000DA9C 0300C825 */  or         $t9, $t8, $zero
    /* E6A0 8000DAA0 01D9001F */  ddivu      $zero, $t6, $t9
    /* E6A4 8000DAA4 17200002 */  bnez       $t9, .L8000DAB0
    /* E6A8 8000DAA8 00000000 */   nop
    /* E6AC 8000DAAC 0007000D */  break      7
  .L8000DAB0:
    /* E6B0 8000DAB0 00004012 */  mflo       $t0
    /* E6B4 8000DAB4 FC880000 */  sd         $t0, 0x0($a0)
    /* E6B8 8000DAB8 87AA0012 */  lh         $t2, 0x12($sp)
    /* E6BC 8000DABC DFA90008 */  ld         $t1, 0x8($sp)
    /* E6C0 8000DAC0 01405825 */  or         $t3, $t2, $zero
    /* E6C4 8000DAC4 01606025 */  or         $t4, $t3, $zero
    /* E6C8 8000DAC8 012C001F */  ddivu      $zero, $t1, $t4
    /* E6CC 8000DACC 15800002 */  bnez       $t4, .L8000DAD8
    /* E6D0 8000DAD0 00000000 */   nop
    /* E6D4 8000DAD4 0007000D */  break      7
  .L8000DAD8:
    /* E6D8 8000DAD8 00006810 */  mfhi       $t5
    /* E6DC 8000DADC FCAD0000 */  sd         $t5, 0x0($a1)
    /* E6E0 8000DAE0 03E00008 */  jr         $ra
    /* E6E4 8000DAE4 00000000 */   nop