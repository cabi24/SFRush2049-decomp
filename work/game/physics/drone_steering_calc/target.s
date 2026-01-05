# Source: game_code.bin (decompressed)
# Address: 0x800A0800

glabel drone_steering_calc
    /* 800A0800 37181D00 */  ori $t8, $t8, 0x1D00
    /* 800A0804 244F0008 */  addiu $t7, $v0, 8
    /* 800A0808 AC6F0000 */  sw $t7, 0($v1)
    /* 800A080C AC400004 */  sw $zero, 4($v0)
    /* 800A0810 AC580000 */  sw $t8, 0($v0)
    /* 800A0814 8C680000 */  lw $t0, 0($v1)
    /* 800A0818 3C0EE300 */  lui $t6, 0xE300
    /* 800A081C 35CE0A01 */  ori $t6, $t6, 0x0A01
    /* 800A0820 25190008 */  addiu $t9, $t0, 8
    /* 800A0824 AC790000 */  sw $t9, 0($v1)
    /* 800A0828 AD000004 */  sw $zero, 4($t0)
    /* 800A082C AD0E0000 */  sw $t6, 0($t0)
    /* 800A0830 8C690000 */  lw $t1, 0($v1)
    /* 800A0834 3C18E300 */  lui $t8, 0xE300
    /* 800A0838 37181201 */  ori $t8, $t8, 0x1201
    /* 800A083C 252F0008 */  addiu $t7, $t1, 8
    /* 800A0840 AC6F0000 */  sw $t7, 0($v1)
    /* 800A0844 AD200004 */  sw $zero, 4($t1)
    /* 800A0848 AD380000 */  sw $t8, 0($t1)
    /* 800A084C 8FB90100 */  lw $t9, 256($sp)
    /* 800A0850 3C0E8003 */  lui $t6, 0x8003
    /* 800A0854 572001B5 */  .word 0x572001B5
    /* 800A0858 8FBF0074 */  lw $ra, 116($sp)
    /* 800A085C 8DCEAFC4 */  lw $t6, -20540($t6)
    /* 800A0860 29C100F1 */  slti $at, $t6, 241
    /* 800A0864 14200003 */  bne $at, $zero, 0x800A0874
    /* 800A0868 00000000 */  nop
    /* 800A086C 0C021E38 */  jal 0x800878E0
    /* 800A0870 34048000 */  ori $a0, $zero, 0x8000
    /* 800A0874 3C028016 */  lui $v0, 0x8016
    /* 800A0878 8C4213AC */  lw $v0, 5036($v0)
    /* 800A087C 3C1E8014 */  lui $fp, 0x8014
    /* 800A0880 27DE0BF0 */  addiu $fp, $fp, 3056
    /* 800A0884 584001A9 */  .word 0x584001A9
    /* 800A0888 8FBF0074 */  lw $ra, 116($sp)
    /* 800A088C 83CF0016 */  lb $t7, 22($fp)
    /* 800A0890 8FC30004 */  lw $v1, 4($fp)
    /* 800A0894 AFBE00B4 */  sw $fp, 180($sp)
    /* 800A0898 AFA00090 */  sw $zero, 144($sp)
    /* 800A089C 11E00006 */  beq $t7, $zero, 0x800A08B8
    /* 800A08A0 0060A825 */  or $s5, $v1, $zero
    /* 800A08A4 3C198014 */  lui $t9, 0x8014
    /* 800A08A8 27390BF0 */  addiu $t9, $t9, 3056
    /* 800A08AC 0002C140 */  sll $t8, $v0, 5
    /* 800A08B0 10000199 */  beq $zero, $zero, 0x800A0F18
    /* 800A08B4 03191821 */  addu $v1, $t8, $t9
    /* 800A08B8 93CE0015 */  lbu $t6, 21($fp)
    /* 800A08BC 31CF0080 */  andi $t7, $t6, 0x0080
    /* 800A08C0 51E0000D */  .word 0x51E0000D
    /* 800A08C4 97C4000E */  lhu $a0, 14($fp)
    /* 800A08C8 10600005 */  beq $v1, $zero, 0x800A08E0
    /* 800A08CC 00000000 */  nop
    /* 800A08D0 0060F809 */  jalr $ra, $v1
    /* 800A08D4 8FC40000 */  lw $a0, 0($fp)
    /* 800A08D8 3C028016 */  lui $v0, 0x8016
    /* 800A08DC 8C4213AC */  lw $v0, 5036($v0)
    /* 800A08E0 3C198014 */  lui $t9, 0x8014
    /* 800A08E4 27390BF0 */  addiu $t9, $t9, 3056
    /* 800A08E8 0002C140 */  sll $t8, $v0, 5
    /* 800A08EC 1000018A */  beq $zero, $zero, 0x800A0F18
    /* 800A08F0 03191821 */  addu $v1, $t8, $t9
    /* 800A08F4 97C4000E */  lhu $a0, 14($fp)
    /* 800A08F8 10800005 */  beq $a0, $zero, 0x800A0910
    /* 800A08FC 00000000 */  nop
    /* 800A0900 0C022991 */  jal 0x8008A644
    /* 800A0904 00000000 */  nop
    /* 800A0908 10000004 */  beq $zero, $zero, 0x800A091C
    /* 800A090C 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0910 0C021C17 */  jal 0x8008705C
    /* 800A0914 24040010 */  addiu $a0, $zero, 16
    /* 800A0918 93CE0015 */  lbu $t6, 21($fp)
    /* 800A091C 31CF0004 */  andi $t7, $t6, 0x0004
    /* 800A0920 11E00005 */  beq $t7, $zero, 0x800A0938
    /* 800A0924 00000000 */  nop
    /* 800A0928 0C021E38 */  jal 0x800878E0
    /* 800A092C 24040004 */  addiu $a0, $zero, 4
    /* 800A0930 10000004 */  beq $zero, $zero, 0x800A0944
    /* 800A0934 87C30010 */  lh $v1, 16($fp)
    /* 800A0938 0C021C17 */  jal 0x8008705C
    /* 800A093C 24040004 */  addiu $a0, $zero, 4
    /* 800A0940 87C30010 */  lh $v1, 16($fp)
    /* 800A0944 10600004 */  beq $v1, $zero, 0x800A0958
    /* 800A0948 00000000 */  nop
    /* 800A094C 87C20012 */  lh $v0, 18($fp)
    /* 800A0950 14400008 */  bne $v0, $zero, 0x800A0974
    /* 800A0954 00000000 */  nop
    /* 800A0958 3C028016 */  lui $v0, 0x8016
    /* 800A095C 8C4213AC */  lw $v0, 5036($v0)
    /* 800A0960 3C198014 */  lui $t9, 0x8014
    /* 800A0964 27390BF0 */  addiu $t9, $t9, 3056
    /* 800A0968 0002C140 */  sll $t8, $v0, 5
    /* 800A096C 1000016A */  beq $zero, $zero, 0x800A0F18
    /* 800A0970 03191821 */  addu $v1, $t8, $t9
    /* 800A0974 16A00024 */  bne $s5, $zero, 0x800A0A08
    /* 800A0978 3C0F8003 */  lui $t7, 0x8003
    /* 800A097C 93C40014 */  lbu $a0, 20($fp)
    /* 800A0980 240100FF */  addiu $at, $zero, 255
    /* 800A0984 14810005 */  bne $a0, $at, 0x800A099C
    /* 800A0988 00000000 */  nop
    /* 800A098C 0C021C17 */  jal 0x8008705C
    /* 800A0990 24040020 */  addiu $a0, $zero, 32
    /* 800A0994 10000004 */  beq $zero, $zero, 0x800A09A8
    /* 800A0998 87C4000A */  lh $a0, 10($fp)
    /* 800A099C 0C0228E3 */  jal 0x8008A38C
    /* 800A09A0 00000000 */  nop
    /* 800A09A4 87C4000A */  lh $a0, 10($fp)
    /* 800A09A8 87C5000C */  lh $a1, 12($fp)
    /* 800A09AC 87CE0010 */  lh $t6, 16($fp)
    /* 800A09B0 87CF0012 */  lh $t7, 18($fp)
    /* 800A09B4 AFA40098 */  sw $a0, 152($sp)
    /* 800A09B8 008E8021 */  addu $s0, $a0, $t6
    /* 800A09BC 00AF9821 */  addu $s3, $a1, $t7
    /* 800A09C0 2667FFFF */  addiu $a3, $s3, -1
    /* 800A09C4 2606FFFF */  addiu $a2, $s0, -1
    /* 800A09C8 0C0228F9 */  jal 0x8008A3E4
    /* 800A09CC 00A0A025 */  or $s4, $a1, $zero
    /* 800A09D0 8FD80000 */  lw $t8, 0($fp)
    /* 800A09D4 8FA40098 */  lw $a0, 152($sp)
    /* 800A09D8 02802825 */  or $a1, $s4, $zero
    /* 800A09DC 02003025 */  or $a2, $s0, $zero
    /* 800A09E0 02603825 */  or $a3, $s3, $zero
    /* 800A09E4 0C02291B */  jal 0x8008A46C
    /* 800A09E8 AFB80010 */  sw $t8, 16($sp)
    /* 800A09EC 3C028016 */  lui $v0, 0x8016
    /* 800A09F0 8C4213AC */  lw $v0, 5036($v0)
    /* 800A09F4 3C0E8014 */  lui $t6, 0x8014
    /* 800A09F8 25CE0BF0 */  addiu $t6, $t6, 3056
    /* 800A09FC 0002C940 */  sll $t9, $v0, 5
    /* 800A0A00 10000145 */  beq $zero, $zero, 0x800A0F18
    /* 800A0A04 032E1821 */  addu $v1, $t9, $t6
    /* 800A0A08 8DEFAFC4 */  lw $t7, -20540($t7)
    /* 800A0A0C 87C4000A */  lh $a0, 10($fp)
    /* 800A0A10 87D4000C */  lh $s4, 12($fp)
    /* 800A0A14 29E100F1 */  slti $at, $t7, 241
    /* 800A0A18 14200002 */  bne $at, $zero, 0x800A0A24
    /* 800A0A1C 00409825 */  or $s3, $v0, $zero
    /* 800A0A20 00429821 */  addu $s3, $v0, $v0
    /* 800A0A24 00643021 */  addu $a2, $v1, $a0
    /* 800A0A28 02933821 */  addu $a3, $s4, $s3
    /* 800A0A2C 24E7FFFF */  addiu $a3, $a3, -1
    /* 800A0A30 24C6FFFF */  addiu $a2, $a2, -1
    /* 800A0A34 02802825 */  or $a1, $s4, $zero
    /* 800A0A38 0C0228F9 */  jal 0x8008A3E4
    /* 800A0A3C AFA40098 */  sw $a0, 152($sp)
    /* 800A0A40 87D8001E */  lh $t8, 30($fp)
    /* 800A0A44 87D9001A */  lh $t9, 26($fp)
    /* 800A0A48 87CE001C */  lh $t6, 28($fp)
    /* 800A0A4C 87CF0018 */  lh $t7, 24($fp)
    /* 800A0A50 92A20014 */  lbu $v0, 20($s5)
    /* 800A0A54 96A30010 */  lhu $v1, 16($s5)
    /* 800A0A58 03199023 */  subu $s2, $t8, $t9
    /* 800A0A5C 24180004 */  addiu $t8, $zero, 4
    /* 800A0A60 01CFB023 */  subu $s6, $t6, $t7
    /* 800A0A64 0302C823 */  subu $t9, $t8, $v0
    /* 800A0A68 92A50015 */  lbu $a1, 21($s5)
    /* 800A0A6C 03237007 */  .word 0x03237007
    /* 800A0A70 000E7840 */  sll $t7, $t6, 1
    /* 800A0A74 3C138012 */  lui $s3, 0x8012
    /* 800A0A78 24060002 */  addiu $a2, $zero, 2
    /* 800A0A7C 026F9821 */  addu $s3, $s3, $t7
    /* 800A0A80 26520001 */  addiu $s2, $s2, 1
    /* 800A0A84 26D60001 */  addiu $s6, $s6, 1
    /* 800A0A88 9673ED0C */  lhu $s3, -4852($s3)
    /* 800A0A8C 10C50003 */  beq $a2, $a1, 0x800A0A9C
    /* 800A0A90 00608025 */  or $s0, $v1, $zero
    /* 800A0A94 24010005 */  addiu $at, $zero, 5
    /* 800A0A98 14A10002 */  bne $a1, $at, 0x800A0AA4
    /* 800A0A9C 0013C043 */  sra $t8, $s3, 1
    /* 800A0AA0 03009825 */  or $s3, $t8, $zero
    /* 800A0AA4 2A610002 */  slti $at, $s3, 2
    /* 800A0AA8 14200006 */  bne $at, $zero, 0x800A0AC4
    /* 800A0AAC 24180001 */  addiu $t8, $zero, 1
    /* 800A0AB0 2419000F */  addiu $t9, $zero, 15
    /* 800A0AB4 00597007 */  .word 0x00597007
    /* 800A0AB8 01D07824 */  and $t7, $t6, $s0
    /* 800A0ABC 51E00031 */  .word 0x51E00031
    /* 800A0AC0 93D80015 */  lbu $t8, 21($fp)
    /* 800A0AC4 1440000F */  bne $v0, $zero, 0x800A0B04
    /* 800A0AC8 AFB80090 */  sw $t8, 144($sp)
    /* 800A0ACC 24191000 */  addiu $t9, $zero, 4096
    /* 800A0AD0 0323001A */  div $t9, $v1
    /* 800A0AD4 00009812 */  mflo $s3
    /* 800A0AD8 14600002 */  bne $v1, $zero, 0x800A0AE4
    /* 800A0ADC 00000000 */  nop
    /* 800A0AE0 0007000D */  .word 0x0007000D
    /* 800A0AE4 2401FFFF */  addiu $at, $zero, -1
    /* 800A0AE8 14610004 */  bne $v1, $at, 0x800A0AFC
    /* 800A0AEC 3C018000 */  lui $at, 0x8000
    /* 800A0AF0 17210002 */  bne $t9, $at, 0x800A0AFC
    /* 800A0AF4 00000000 */  nop
    /* 800A0AF8 0006000D */  .word 0x0006000D
    /* 800A0AFC 10000021 */  beq $zero, $zero, 0x800A0B84
    /* 800A0B00 93D80015 */  lbu $t8, 21($fp)
    /* 800A0B04 24010001 */  addiu $at, $zero, 1
    /* 800A0B08 50410004 */  .word 0x50410004
    /* 800A0B0C 240E0800 */  addiu $t6, $zero, 2048
    /* 800A0B10 54C20010 */  .word 0x54C20010
    /* 800A0B14 240F0400 */  addiu $t7, $zero, 1024
    /* 800A0B18 240E0800 */  addiu $t6, $zero, 2048
    /* 800A0B1C 01C3001A */  div $t6, $v1
    /* 800A0B20 00009812 */  mflo $s3
    /* 800A0B24 14600002 */  bne $v1, $zero, 0x800A0B30
    /* 800A0B28 00000000 */  nop
    /* 800A0B2C 0007000D */  .word 0x0007000D
    /* 800A0B30 2401FFFF */  addiu $at, $zero, -1
    /* 800A0B34 14610004 */  bne $v1, $at, 0x800A0B48
    /* 800A0B38 3C018000 */  lui $at, 0x8000
    /* 800A0B3C 15C10002 */  bne $t6, $at, 0x800A0B48
    /* 800A0B40 00000000 */  nop
    /* 800A0B44 0006000D */  .word 0x0006000D
    /* 800A0B48 1000000E */  beq $zero, $zero, 0x800A0B84
    /* 800A0B4C 93D80015 */  lbu $t8, 21($fp)
    /* 800A0B50 240F0400 */  addiu $t7, $zero, 1024
    /* 800A0B54 01E3001A */  div $t7, $v1
    /* 800A0B58 00009812 */  mflo $s3
    /* 800A0B5C 14600002 */  bne $v1, $zero, 0x800A0B68
    /* 800A0B60 00000000 */  nop
    /* 800A0B64 0007000D */  .word 0x0007000D
    /* 800A0B68 2401FFFF */  addiu $at, $zero, -1
    /* 800A0B6C 14610004 */  bne $v1, $at, 0x800A0B80
    /* 800A0B70 3C018000 */  lui $at, 0x8000
    /* 800A0B74 15E10002 */  bne $t7, $at, 0x800A0B80
    /* 800A0B78 00000000 */  nop
    /* 800A0B7C 0006000D */  .word 0x0006000D
    /* 800A0B80 93D80015 */  lbu $t8, 21($fp)
    /* 800A0B84 00008025 */  or $s0, $zero, $zero
    /* 800A0B88 24040008 */  addiu $a0, $zero, 8
    /* 800A0B8C 33190008 */  andi $t9, $t8, 0x0008
    /* 800A0B90 13200005 */  beq $t9, $zero, 0x800A0BA8
    /* 800A0B94 00000000 */  nop
    /* 800A0B98 0C021C17 */  jal 0x8008705C
    /* 800A0B9C 24040008 */  addiu $a0, $zero, 8
    /* 800A0BA0 10000004 */  beq $zero, $zero, 0x800A0BB4
    /* 800A0BA4 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0BA8 0C021E38 */  jal 0x800878E0
    /* 800A0BAC 24100001 */  addiu $s0, $zero, 1
    /* 800A0BB0 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0BB4 31CF0001 */  andi $t7, $t6, 0x0001
    /* 800A0BB8 51E00004 */  .word 0x51E00004
    /* 800A0BBC 93C40014 */  lbu $a0, 20($fp)
    /* 800A0BC0 0C021E38 */  jal 0x800878E0
    /* 800A0BC4 24040001 */  addiu $a0, $zero, 1
    /* 800A0BC8 93C40014 */  lbu $a0, 20($fp)
    /* 800A0BCC 240100FF */  addiu $at, $zero, 255
    /* 800A0BD0 14810005 */  bne $a0, $at, 0x800A0BE8
    /* 800A0BD4 00000000 */  nop
    /* 800A0BD8 0C021C17 */  jal 0x8008705C
    /* 800A0BDC 24040020 */  addiu $a0, $zero, 32
    /* 800A0BE0 10000004 */  beq $zero, $zero, 0x800A0BF4
    /* 800A0BE4 8FC30000 */  lw $v1, 0($fp)
    /* 800A0BE8 0C0228E3 */  jal 0x8008A38C
    /* 800A0BEC 00000000 */  nop
    /* 800A0BF0 8FC30000 */  lw $v1, 0($fp)
    /* 800A0BF4 00003825 */  or $a3, $zero, $zero
    /* 800A0BF8 50600008 */  .word 0x50600008
    /* 800A0BFC 86A30016 */  lh $v1, 22($s5)
    /* 800A0C00 8C640014 */  lw $a0, 20($v1)
    /* 800A0C04 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C08 0C022852 */  jal 0x8008A148
    /* 800A0C0C 92A60014 */  lbu $a2, 20($s5)
    /* 800A0C10 10000034 */  beq $zero, $zero, 0x800A0CE4
    /* 800A0C14 87C4001C */  lh $a0, 28($fp)
    /* 800A0C18 86A30016 */  lh $v1, 22($s5)
    /* 800A0C1C 3C0F8014 */  lui $t7, 0x8014
    /* 800A0C20 00003825 */  or $a3, $zero, $zero
    /* 800A0C24 04620011 */  .word 0x04620011
    /* 800A0C28 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C2C 97D80008 */  lhu $t8, 8($fp)
    /* 800A0C30 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C34 92A60014 */  lbu $a2, 20($s5)
    /* 800A0C38 0018CA83 */  sra $t9, $t8, 10
    /* 800A0C3C 001970C0 */  sll $t6, $t9, 3
    /* 800A0C40 01EE7821 */  addu $t7, $t7, $t6
    /* 800A0C44 8DEF8670 */  lw $t7, -31120($t7)
    /* 800A0C48 0003C080 */  sll $t8, $v1, 2
    /* 800A0C4C 0303C023 */  subu $t8, $t8, $v1
    /* 800A0C50 0018C0C0 */  sll $t8, $t8, 3
    /* 800A0C54 01F81021 */  addu $v0, $t7, $t8
    /* 800A0C58 0C022852 */  jal 0x8008A148
    /* 800A0C5C 8C440014 */  lw $a0, 20($v0)
    /* 800A0C60 10000020 */  beq $zero, $zero, 0x800A0CE4
    /* 800A0C64 87C4001C */  lh $a0, 28($fp)
    /* 800A0C68 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C6C 24010003 */  addiu $at, $zero, 3
    /* 800A0C70 3C048012 */  lui $a0, 0x8012
    /* 800A0C74 14A1000A */  bne $a1, $at, 0x800A0CA0
    /* 800A0C78 2484ED08 */  addiu $a0, $a0, -4856
    /* 800A0C7C 93D90014 */  lbu $t9, 20($fp)
    /* 800A0C80 3C018012 */  lui $at, 0x8012
    /* 800A0C84 00003825 */  or $a3, $zero, $zero
    /* 800A0C88 A039ED0B */  sb $t9, -4853($at)
    /* 800A0C8C 92A60014 */  lbu $a2, 20($s5)
    /* 800A0C90 0C022852 */  jal 0x8008A148
    /* 800A0C94 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C98 10000012 */  beq $zero, $zero, 0x800A0CE4
    /* 800A0C9C 87C4001C */  lh $a0, 28($fp)
    /* 800A0CA0 50A00010 */  .word 0x50A00010
    /* 800A0CA4 87C4001C */  lh $a0, 28($fp)
    /* 800A0CA8 97CE0008 */  lhu $t6, 8($fp)
    /* 800A0CAC 3C198014 */  lui $t9, 0x8014
    /* 800A0CB0 27398670 */  addiu $t9, $t9, -31120
    /* 800A0CB4 000E7A83 */  sra $t7, $t6, 10
    /* 800A0CB8 000FC0C0 */  sll $t8, $t7, 3
    /* 800A0CBC 03191821 */  addu $v1, $t8, $t9
    /* 800A0CC0 8C6E0004 */  lw $t6, 4($v1)
    /* 800A0CC4 00003825 */  or $a3, $zero, $zero
    /* 800A0CC8 51C00006 */  .word 0x51C00006
    /* 800A0CCC 87C4001C */  lh $a0, 28($fp)
    /* 800A0CD0 8C620000 */  lw $v0, 0($v1)
    /* 800A0CD4 92A60014 */  lbu $a2, 20($s5)
    /* 800A0CD8 0C022852 */  jal 0x8008A148
    /* 800A0CDC 8C440014 */  lw $a0, 20($v0)
    /* 800A0CE0 87C4001C */  lh $a0, 28($fp)
    /* 800A0CE4 96A30012 */  lhu $v1, 18($s5)
    /* 800A0CE8 0083082A */  slt $at, $a0, $v1
    /* 800A0CEC 14200003 */  bne $at, $zero, 0x800A0CFC
    /* 800A0CF0 246FFFFF */  addiu $t7, $v1, -1
    /* 800A0CF4 A7CF001C */  sh $t7, 28($fp)
    /* 800A0CF8 87C4001C */  lh $a0, 28($fp)
    /* 800A0CFC 87C20018 */  lh $v0, 24($fp)
    /* 800A0D00 02D3082A */  slt $at, $s6, $s3
    /* 800A0D04 04410003 */  bgez $v0, 0x800A0D14
    /* 800A0D08 00408825 */  or $s1, $v0, $zero
    /* 800A0D0C 10000001 */  beq $zero, $zero, 0x800A0D14
    /* 800A0D10 00008825 */  or $s1, $zero, $zero
    /* 800A0D14 10200002 */  beq $at, $zero, 0x800A0D20
    /* 800A0D18 00000000 */  nop
    /* 800A0D1C 02C09825 */  or $s3, $s6, $zero
    /* 800A0D20 1200003E */  beq $s0, $zero, 0x800A0E1C
    /* 800A0D24 0224082A */  slt $at, $s1, $a0
    /* 800A0D28 0224082A */  slt $at, $s1, $a0
    /* 800A0D2C 1020006E */  beq $at, $zero, 0x800A0EE8
    /* 800A0D30 2656FFFF */  addiu $s6, $s2, -1
    /* 800A0D34 8FB90098 */  lw $t9, 152($sp)
    /* 800A0D38 32D8FFFF */  andi $t8, $s6, 0xFFFF
    /* 800A0D3C 0300B025 */  or $s6, $t8, $zero
    /* 800A0D40 0332B821 */  addu $s7, $t9, $s2
    /* 800A0D44 26F7FFFF */  addiu $s7, $s7, -1
    /* 800A0D48 87C4001C */  lh $a0, 28($fp)
    /* 800A0D4C 96A20012 */  lhu $v0, 18($s5)
    /* 800A0D50 02339021 */  addu $s2, $s1, $s3
    /* 800A0D54 0092082A */  slt $at, $a0, $s2
    /* 800A0D58 92A50015 */  lbu $a1, 21($s5)
    /* 800A0D5C 10200004 */  beq $at, $zero, 0x800A0D70
    /* 800A0D60 00401825 */  or $v1, $v0, $zero
    /* 800A0D64 00919823 */  subu $s3, $a0, $s1
    /* 800A0D68 26730001 */  addiu $s3, $s3, 1
    /* 800A0D6C 02339021 */  addu $s2, $s1, $s3
    /* 800A0D70 8EA40018 */  lw $a0, 24($s5)
    /* 800A0D74 92A60014 */  lbu $a2, 20($s5)
    /* 800A0D78 96A70010 */  lhu $a3, 16($s5)
    /* 800A0D7C 00717023 */  subu $t6, $v1, $s1
    /* 800A0D80 01D37823 */  subu $t7, $t6, $s3
    /* 800A0D84 8FAE0090 */  lw $t6, 144($sp)
    /* 800A0D88 0071C023 */  subu $t8, $v1, $s1
    /* 800A0D8C 2719FFFF */  addiu $t9, $t8, -1
    /* 800A0D90 AFB90020 */  sw $t9, 32($sp)
    /* 800A0D94 AFAF0018 */  sw $t7, 24($sp)
    /* 800A0D98 AFA00024 */  sw $zero, 36($sp)
    /* 800A0D9C AFB6001C */  sw $s6, 28($sp)
    /* 800A0DA0 AFA00014 */  sw $zero, 20($sp)
    /* 800A0DA4 AFA20010 */  sw $v0, 16($sp)
    /* 800A0DA8 0C021E82 */  jal 0x80087A08
    /* 800A0DAC AFAE0028 */  sw $t6, 40($sp)
    /* 800A0DB0 87CF001A */  lh $t7, 26($fp)
    /* 800A0DB4 02938021 */  addu $s0, $s4, $s3
    /* 800A0DB8 2607FFFF */  addiu $a3, $s0, -1
    /* 800A0DBC AFAF0010 */  sw $t7, 16($sp)
    /* 800A0DC0 96B80012 */  lhu $t8, 18($s5)
    /* 800A0DC4 8FA40098 */  lw $a0, 152($sp)
    /* 800A0DC8 02802825 */  or $a1, $s4, $zero
    /* 800A0DCC 0311C823 */  subu $t9, $t8, $s1
    /* 800A0DD0 03337023 */  subu $t6, $t9, $s3
    /* 800A0DD4 AFAE0014 */  sw $t6, 20($sp)
    /* 800A0DD8 0C021C44 */  jal 0x80087110
    /* 800A0DDC 02E03025 */  or $a2, $s7, $zero
    /* 800A0DE0 3C0F8003 */  lui $t7, 0x8003
    /* 800A0DE4 8DEFAFC4 */  lw $t7, -20540($t7)
    /* 800A0DE8 8FB800B4 */  lw $t8, 180($sp)
    /* 800A0DEC 29E100F1 */  slti $at, $t7, 241
    /* 800A0DF0 54200003 */  .word 0x54200003
    /* 800A0DF4 8719001C */  lh $t9, 28($t8)
    /* 800A0DF8 02138021 */  addu $s0, $s0, $s3
    /* 800A0DFC 8719001C */  lh $t9, 28($t8)
    /* 800A0E00 02408825 */  or $s1, $s2, $zero
    /* 800A0E04 0200A025 */  or $s4, $s0, $zero
    /* 800A0E08 0259082A */  slt $at, $s2, $t9
    /* 800A0E0C 5420FFCF */  .word 0x5420FFCF
    /* 800A0E10 87C4001C */  lh $a0, 28($fp)
    /* 800A0E14 10000035 */  beq $zero, $zero, 0x800A0EEC
    /* 800A0E18 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0E1C 10200032 */  beq $at, $zero, 0x800A0EE8
    /* 800A0E20 2656FFFF */  addiu $s6, $s2, -1
    /* 800A0E24 8FAF0098 */  lw $t7, 152($sp)
    /* 800A0E28 32CEFFFF */  andi $t6, $s6, 0xFFFF
    /* 800A0E2C 01C0B025 */  or $s6, $t6, $zero
    /* 800A0E30 01F2B821 */  addu $s7, $t7, $s2
    /* 800A0E34 26F7FFFF */  addiu $s7, $s7, -1
    /* 800A0E38 87C4001C */  lh $a0, 28($fp)
    /* 800A0E3C 02339021 */  addu $s2, $s1, $s3
    /* 800A0E40 92A50015 */  lbu $a1, 21($s5)
    /* 800A0E44 0092082A */  slt $at, $a0, $s2
    /* 800A0E48 10200004 */  beq $at, $zero, 0x800A0E5C
    /* 800A0E4C 96A20012 */  lhu $v0, 18($s5)
    /* 800A0E50 00919823 */  subu $s3, $a0, $s1
    /* 800A0E54 26730001 */  addiu $s3, $s3, 1
    /* 800A0E58 02339021 */  addu $s2, $s1, $s3
    /* 800A0E5C 8EA40018 */  lw $a0, 24($s5)
    /* 800A0E60 92A60014 */  lbu $a2, 20($s5)
    /* 800A0E64 96A70010 */  lhu $a3, 16($s5)
    /* 800A0E68 8FB90090 */  lw $t9, 144($sp)
    /* 800A0E6C 2658FFFF */  addiu $t8, $s2, -1
    /* 800A0E70 AFB80020 */  sw $t8, 32($sp)
    /* 800A0E74 AFA00024 */  sw $zero, 36($sp)
    /* 800A0E78 AFB6001C */  sw $s6, 28($sp)
    /* 800A0E7C AFB10018 */  sw $s1, 24($sp)
    /* 800A0E80 AFA00014 */  sw $zero, 20($sp)
    /* 800A0E84 AFA20010 */  sw $v0, 16($sp)
    /* 800A0E88 0C021E82 */  jal 0x80087A08
    /* 800A0E8C AFB90028 */  sw $t9, 40($sp)
    /* 800A0E90 87CE001A */  lh $t6, 26($fp)
    /* 800A0E94 02938021 */  addu $s0, $s4, $s3
    /* 800A0E98 2607FFFF */  addiu $a3, $s0, -1
    /* 800A0E9C 8FA40098 */  lw $a0, 152($sp)
    /* 800A0EA0 02802825 */  or $a1, $s4, $zero
    /* 800A0EA4 02E03025 */  or $a2, $s7, $zero
    /* 800A0EA8 AFB10014 */  sw $s1, 20($sp)
    /* 800A0EAC 0C021C44 */  jal 0x80087110
    /* 800A0EB0 AFAE0010 */  sw $t6, 16($sp)
    /* 800A0EB4 3C0F8003 */  lui $t7, 0x8003
    /* 800A0EB8 8DEFAFC4 */  lw $t7, -20540($t7)
    /* 800A0EBC 8FB800B4 */  lw $t8, 180($sp)
    /* 800A0EC0 29E100F1 */  slti $at, $t7, 241
    /* 800A0EC4 54200003 */  .word 0x54200003
    /* 800A0EC8 8719001C */  lh $t9, 28($t8)
    /* 800A0ECC 02138021 */  addu $s0, $s0, $s3
    /* 800A0ED0 8719001C */  lh $t9, 28($t8)
    /* 800A0ED4 02408825 */  or $s1, $s2, $zero
    /* 800A0ED8 0200A025 */  or $s4, $s0, $zero
    /* 800A0EDC 0259082A */  slt $at, $s2, $t9
    /* 800A0EE0 5420FFD6 */  .word 0x5420FFD6
    /* 800A0EE4 87C4001C */  lh $a0, 28($fp)
    /* 800A0EE8 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0EEC 31CF0001 */  andi $t7, $t6, 0x0001
    /* 800A0EF0 11E00003 */  beq $t7, $zero, 0x800A0F00
    /* 800A0EF4 00000000 */  nop
    /* 800A0EF8 0C021C17 */  jal 0x8008705C
    /* 800A0EFC 24040001 */  addiu $a0, $zero, 1
    /* 800A0F00 3C028016 */  lui $v0, 0x8016
    /* 800A0F04 8C4213AC */  lw $v0, 5036($v0)
    /* 800A0F08 3C198014 */  lui $t9, 0x8014
    /* 800A0F0C 27390BF0 */  addiu $t9, $t9, 3056
    /* 800A0F10 0002C140 */  sll $t8, $v0, 5
    /* 800A0F14 03191821 */  addu $v1, $t8, $t9
    /* 800A0F18 27DE0020 */  addiu $fp, $fp, 32
    /* 800A0F1C 03C3082B */  sltu $at, $fp, $v1
    /* 800A0F20 5420FE5B */  .word 0x5420FE5B
    /* 800A0F24 83CF0016 */  lb $t7, 22($fp)
    /* 800A0F28 8FBF0074 */  lw $ra, 116($sp)
    /* 800A0F2C D7B40038 */  .word 0xD7B40038
    /* 800A0F30 D7B60040 */  .word 0xD7B60040
    /* 800A0F34 D7B80048 */  .word 0xD7B80048
    /* 800A0F38 8FB00050 */  lw $s0, 80($sp)
    /* 800A0F3C 8FB10054 */  lw $s1, 84($sp)
    /* 800A0F40 8FB20058 */  lw $s2, 88($sp)
    /* 800A0F44 8FB3005C */  lw $s3, 92($sp)
    /* 800A0F48 8FB40060 */  lw $s4, 96($sp)
    /* 800A0F4C 8FB50064 */  lw $s5, 100($sp)
    /* 800A0F50 8FB60068 */  lw $s6, 104($sp)
    /* 800A0F54 8FB7006C */  lw $s7, 108($sp)
    /* 800A0F58 8FBE0070 */  lw $fp, 112($sp)
    /* 800A0F5C 03E00008 */  jr $ra
    /* 800A0F60 27BD0100 */  addiu $sp, $sp, 256
