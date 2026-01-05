# Source: game_code.bin (decompressed)
# Address: 0x800A29A8

glabel physics_force_limit
    /* 800A29A8 AFB20030 */  sw $s2, 48($sp)
    /* 800A29AC AFB1002C */  sw $s1, 44($sp)
    /* 800A29B0 AFB00028 */  sw $s0, 40($sp)
    /* 800A29B4 AFA400D8 */  sw $a0, 216($sp)
    /* 800A29B8 AFA600E0 */  sw $a2, 224($sp)
    /* 800A29BC 8C960000 */  lw $s6, 0($a0)
    /* 800A29C0 00A04025 */  or $t0, $a1, $zero
    /* 800A29C4 00002025 */  or $a0, $zero, $zero
    /* 800A29C8 8ECF0048 */  lw $t7, 72($s6)
    /* 800A29CC 92D50010 */  lbu $s5, 16($s6)
    /* 800A29D0 92C70011 */  lbu $a3, 17($s6)
    /* 800A29D4 15E000B9 */  bne $t7, $zero, 0x800A2CBC
    /* 800A29D8 0015C880 */  sll $t9, $s5, 2
    /* 800A29DC 0335C823 */  subu $t9, $t9, $s5
    /* 800A29E0 0019C980 */  sll $t9, $t9, 6
    /* 800A29E4 00077080 */  sll $t6, $a3, 2
    /* 800A29E8 01C77021 */  addu $t6, $t6, $a3
    /* 800A29EC 0335C821 */  addu $t9, $t9, $s5
    /* 800A29F0 0019C880 */  sll $t9, $t9, 2
    /* 800A29F4 000E70C0 */  sll $t6, $t6, 3
    /* 800A29F8 3C188014 */  lui $t8, 0x8014
    /* 800A29FC 27184030 */  addiu $t8, $t8, 16432
    /* 800A2A00 032E7821 */  addu $t7, $t9, $t6
    /* 800A2A04 8EC30040 */  lw $v1, 64($s6)
    /* 800A2A08 01F81021 */  addu $v0, $t7, $t8
    /* 800A2A0C 94590088 */  lhu $t9, 136($v0)
    /* 800A2A10 AFA20058 */  sw $v0, 88($sp)
    /* 800A2A14 AFA800DC */  sw $t0, 220($sp)
    /* 800A2A18 AFA700C4 */  sw $a3, 196($sp)
    /* 800A2A1C 0C025D3B */  jal 0x800974EC
    /* 800A2A20 03232821 */  addu $a1, $t9, $v1
    /* 800A2A24 AEC20048 */  sw $v0, 72($s6)
    /* 800A2A28 8FB80058 */  lw $t8, 88($sp)
    /* 800A2A2C 8C4E0000 */  lw $t6, 0($v0)
    /* 800A2A30 8ECF0040 */  lw $t7, 64($s6)
    /* 800A2A34 00002825 */  or $a1, $zero, $zero
    /* 800A2A38 97060088 */  lhu $a2, 136($t8)
    /* 800A2A3C 0C0009E4 */  jal 0x80002790
    /* 800A2A40 01CF2021 */  addu $a0, $t6, $t7
    /* 800A2A44 8ED90048 */  lw $t9, 72($s6)
    /* 800A2A48 8EC50040 */  lw $a1, 64($s6)
    /* 800A2A4C 0C002138 */  jal 0x800084E0
    /* 800A2A50 8F240000 */  lw $a0, 0($t9)
    /* 800A2A54 3C148011 */  lui $s4, 0x8011
    /* 800A2A58 2694194C */  addiu $s4, $s4, 6476
    /* 800A2A5C 828E0000 */  lb $t6, 0($s4)
    /* 800A2A60 3C138015 */  lui $s3, 0x8015
    /* 800A2A64 267397D0 */  addiu $s3, $s3, -26672
    /* 800A2A68 15C0000B */  bne $t6, $zero, 0x800A2A98
    /* 800A2A6C 240F0001 */  addiu $t7, $zero, 1
    /* 800A2A70 3C058015 */  lui $a1, 0x8015
    /* 800A2A74 24A527E4 */  addiu $a1, $a1, 10212
    /* 800A2A78 A28F0000 */  sb $t7, 0($s4)
    /* 800A2A7C 02602025 */  or $a0, $s3, $zero
    /* 800A2A80 0C001A80 */  jal 0x80006A00
    /* 800A2A84 24060001 */  addiu $a2, $zero, 1
    /* 800A2A88 02602025 */  or $a0, $s3, $zero
    /* 800A2A8C 00002825 */  or $a1, $zero, $zero
    /* 800A2A90 0C001D78 */  jal 0x800075E0
    /* 800A2A94 00003025 */  or $a2, $zero, $zero
    /* 800A2A98 3C138015 */  lui $s3, 0x8015
    /* 800A2A9C 267397D0 */  addiu $s3, $s3, -26672
    /* 800A2AA0 02602025 */  or $a0, $s3, $zero
    /* 800A2AA4 27A500AC */  addiu $a1, $sp, 172
    /* 800A2AA8 0C001C9C */  jal 0x80007270
    /* 800A2AAC 24060001 */  addiu $a2, $zero, 1
    /* 800A2AB0 0015C080 */  sll $t8, $s5, 2
    /* 800A2AB4 8FA300E0 */  lw $v1, 224($sp)
    /* 800A2AB8 0315C023 */  subu $t8, $t8, $s5
    /* 800A2ABC 0018C180 */  sll $t8, $t8, 6
    /* 800A2AC0 0315C021 */  addu $t8, $t8, $s5
    /* 800A2AC4 3C198014 */  lui $t9, 0x8014
    /* 800A2AC8 27394030 */  addiu $t9, $t9, 16432
    /* 800A2ACC 0018C080 */  sll $t8, $t8, 2
    /* 800A2AD0 2463001F */  addiu $v1, $v1, 31
    /* 800A2AD4 00037142 */  srl $t6, $v1, 5
    /* 800A2AD8 03198821 */  addu $s1, $t8, $t9
    /* 800A2ADC 2624000C */  addiu $a0, $s1, 12
    /* 800A2AE0 000E7940 */  sll $t7, $t6, 5
    /* 800A2AE4 3C128012 */  lui $s2, 0x8012
    /* 800A2AE8 2652EAE8 */  addiu $s2, $s2, -5400
    /* 800A2AEC AFAF004C */  sw $t7, 76($sp)
    /* 800A2AF0 AFA40054 */  sw $a0, 84($sp)
    /* 800A2AF4 8FA3004C */  lw $v1, 76($sp)
    /* 800A2AF8 8FA40054 */  lw $a0, 84($sp)
    /* 800A2AFC 8FA500C4 */  lw $a1, 196($sp)
    /* 800A2B00 AFA30010 */  sw $v1, 16($sp)
    /* 800A2B04 8ED80048 */  lw $t8, 72($s6)
    /* 800A2B08 00003025 */  or $a2, $zero, $zero
    /* 800A2B0C 8FA700DC */  lw $a3, 220($sp)
    /* 800A2B10 8F190000 */  lw $t9, 0($t8)
    /* 800A2B14 0C002BB1 */  jal 0x8000AEC4
    /* 800A2B18 AFB90014 */  sw $t9, 20($sp)
    /* 800A2B1C 1040005D */  beq $v0, $zero, 0x800A2C94
    /* 800A2B20 00402025 */  or $a0, $v0, $zero
    /* 800A2B24 0C0287A5 */  jal 0x800A1E94
    /* 800A2B28 00000000 */  nop
    /* 800A2B2C AE220074 */  sw $v0, 116($s1)
    /* 800A2B30 8E2E0074 */  lw $t6, 116($s1)
    /* 800A2B34 24010009 */  addiu $at, $zero, 9
    /* 800A2B38 02602025 */  or $a0, $s3, $zero
    /* 800A2B3C 15C10009 */  bne $t6, $at, 0x800A2B64
    /* 800A2B40 00002825 */  or $a1, $zero, $zero
    /* 800A2B44 02602025 */  or $a0, $s3, $zero
    /* 800A2B48 00002825 */  or $a1, $zero, $zero
    /* 800A2B4C 0C001D78 */  jal 0x800075E0
    /* 800A2B50 00003025 */  or $a2, $zero, $zero
    /* 800A2B54 0C0289A0 */  jal 0x800A2680
    /* 800A2B58 8FA400D8 */  lw $a0, 216($sp)
    /* 800A2B5C 10000058 */  beq $zero, $zero, 0x800A2CC0
    /* 800A2B60 8FBF0044 */  lw $ra, 68($sp)
    /* 800A2B64 3C108009 */  lui $s0, 0x8009
    /* 800A2B68 2610A6A4 */  addiu $s0, $s0, -22876
    /* 800A2B6C 0C001D78 */  jal 0x800075E0
    /* 800A2B70 00003025 */  or $a2, $zero, $zero
    /* 800A2B74 A2550000 */  sb $s5, 0($s2)
    /* 800A2B78 3C198014 */  lui $t9, 0x8014
    /* 800A2B7C 8F394008 */  lw $t9, 16392($t9)
    /* 800A2B80 27AF00C3 */  addiu $t7, $sp, 195
    /* 800A2B84 27B800C2 */  addiu $t8, $sp, 194
    /* 800A2B88 AFB80014 */  sw $t8, 20($sp)
    /* 800A2B8C AFAF0010 */  sw $t7, 16($sp)
    /* 800A2B90 02A02025 */  or $a0, $s5, $zero
    /* 800A2B94 8E250074 */  lw $a1, 116($s1)
    /* 800A2B98 00003025 */  or $a2, $zero, $zero
    /* 800A2B9C 00003825 */  or $a3, $zero, $zero
    /* 800A2BA0 0320F809 */  jalr $ra, $t9
    /* 800A2BA4 AFB00018 */  sw $s0, 24($sp)
    /* 800A2BA8 828F0000 */  lb $t7, 0($s4)
    /* 800A2BAC 240EFFFF */  addiu $t6, $zero, -1
    /* 800A2BB0 A24E0000 */  sb $t6, 0($s2)
    /* 800A2BB4 15E0000B */  bne $t7, $zero, 0x800A2BE4
    /* 800A2BB8 24180001 */  addiu $t8, $zero, 1
    /* 800A2BBC 3C058015 */  lui $a1, 0x8015
    /* 800A2BC0 24A527E4 */  addiu $a1, $a1, 10212
    /* 800A2BC4 A2980000 */  sb $t8, 0($s4)
    /* 800A2BC8 02602025 */  or $a0, $s3, $zero
    /* 800A2BCC 0C001A80 */  jal 0x80006A00
    /* 800A2BD0 24060001 */  addiu $a2, $zero, 1
    /* 800A2BD4 02602025 */  or $a0, $s3, $zero
    /* 800A2BD8 00002825 */  or $a1, $zero, $zero
    /* 800A2BDC 0C001D78 */  jal 0x800075E0
    /* 800A2BE0 00003025 */  or $a2, $zero, $zero
    /* 800A2BE4 02602025 */  or $a0, $s3, $zero
    /* 800A2BE8 27A5008C */  addiu $a1, $sp, 140
    /* 800A2BEC 0C001C9C */  jal 0x80007270
    /* 800A2BF0 24060001 */  addiu $a2, $zero, 1
    /* 800A2BF4 83B900C3 */  lb $t9, 195($sp)
    /* 800A2BF8 5720FFBF */  .word 0x5720FFBF
    /* 800A2BFC 8FA3004C */  lw $v1, 76($sp)
    /* 800A2C00 00157100 */  sll $t6, $s5, 4
    /* 800A2C04 3C028014 */  lui $v0, 0x8014
    /* 800A2C08 004E1021 */  addu $v0, $v0, $t6
    /* 800A2C0C 8C424D68 */  lw $v0, 19816($v0)
    /* 800A2C10 8FA300D8 */  lw $v1, 216($sp)
    /* 800A2C14 5040001A */  .word 0x5040001A
    /* 800A2C18 02602025 */  or $a0, $s3, $zero
    /* 800A2C1C 54620014 */  .word 0x54620014
    /* 800A2C20 8C4F0000 */  lw $t7, 0($v0)
    /* 800A2C24 8EC20048 */  lw $v0, 72($s6)
    /* 800A2C28 3C128015 */  lui $s2, 0x8015
    /* 800A2C2C 26522770 */  addiu $s2, $s2, 10096
    /* 800A2C30 10400012 */  beq $v0, $zero, 0x800A2C7C
    /* 800A2C34 00408025 */  or $s0, $v0, $zero
    /* 800A2C38 02402025 */  or $a0, $s2, $zero
    /* 800A2C3C 00002825 */  or $a1, $zero, $zero
    /* 800A2C40 0C001C9C */  jal 0x80007270
    /* 800A2C44 24060001 */  addiu $a2, $zero, 1
    /* 800A2C48 02002825 */  or $a1, $s0, $zero
    /* 800A2C4C 0C0257F6 */  jal 0x80095FD8
    /* 800A2C50 00003025 */  or $a2, $zero, $zero
    /* 800A2C54 02402025 */  or $a0, $s2, $zero
    /* 800A2C58 00002825 */  or $a1, $zero, $zero
    /* 800A2C5C 0C001D78 */  jal 0x800075E0
    /* 800A2C60 00003025 */  or $a2, $zero, $zero
    /* 800A2C64 10000005 */  beq $zero, $zero, 0x800A2C7C
    /* 800A2C68 AEC00048 */  sw $zero, 72($s6)
    /* 800A2C6C 8C4F0000 */  lw $t7, 0($v0)
    /* 800A2C70 8DE20000 */  lw $v0, 0($t7)
    /* 800A2C74 1440FFE9 */  bne $v0, $zero, 0x800A2C1C
    /* 800A2C78 00000000 */  nop
    /* 800A2C7C 02602025 */  or $a0, $s3, $zero
    /* 800A2C80 00002825 */  or $a1, $zero, $zero
    /* 800A2C84 0C001D78 */  jal 0x800075E0
    /* 800A2C88 00003025 */  or $a2, $zero, $zero
    /* 800A2C8C 1000000C */  beq $zero, $zero, 0x800A2CC0
    /* 800A2C90 8FBF0044 */  lw $ra, 68($sp)
    /* 800A2C94 02602025 */  or $a0, $s3, $zero
    /* 800A2C98 00002825 */  or $a1, $zero, $zero
    /* 800A2C9C 0C001D78 */  jal 0x800075E0
    /* 800A2CA0 00003025 */  or $a2, $zero, $zero
    /* 800A2CA4 8FB800E0 */  lw $t8, 224($sp)
    /* 800A2CA8 8ED90040 */  lw $t9, 64($s6)
    /* 800A2CAC 8FAF0058 */  lw $t7, 88($sp)
    /* 800A2CB0 03197026 */  xor $t6, $t8, $t9
    /* 800A2CB4 000E702B */  sltu $t6, $zero, $t6
    /* 800A2CB8 A1EE0086 */  sb $t6, 134($t7)
    /* 800A2CBC 8FBF0044 */  lw $ra, 68($sp)
    /* 800A2CC0 8FB00028 */  lw $s0, 40($sp)
    /* 800A2CC4 8FB1002C */  lw $s1, 44($sp)
    /* 800A2CC8 8FB20030 */  lw $s2, 48($sp)
    /* 800A2CCC 8FB30034 */  lw $s3, 52($sp)
    /* 800A2CD0 8FB40038 */  lw $s4, 56($sp)
    /* 800A2CD4 8FB5003C */  lw $s5, 60($sp)
    /* 800A2CD8 8FB60040 */  lw $s6, 64($sp)
    /* 800A2CDC 03E00008 */  jr $ra
    /* 800A2CE0 27BD00D8 */  addiu $sp, $sp, 216
