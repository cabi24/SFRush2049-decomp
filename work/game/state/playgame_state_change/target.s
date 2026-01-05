# Source: game_code.bin (decompressed)
# Address: 0x800CA3B4

glabel playgame_state_change
    /* 800CA3B4 27BDFF18 */  addiu $sp, $sp, -232
    /* 800CA3B8 AFB00038 */  sw $s0, 56($sp)
    /* 800CA3BC 3C108011 */  lui $s0, 0x8011
    /* 800CA3C0 261074B4 */  addiu $s0, $s0, 29876
    /* 800CA3C4 3C0F8011 */  lui $t7, 0x8011
    /* 800CA3C8 8DEF74B8 */  lw $t7, 29880($t7)
    /* 800CA3CC 8E0E0000 */  lw $t6, 0($s0)
    /* 800CA3D0 AFBF004C */  sw $ra, 76($sp)
    /* 800CA3D4 AFB40048 */  sw $s4, 72($sp)
    /* 800CA3D8 AFB30044 */  sw $s3, 68($sp)
    /* 800CA3DC AFB20040 */  sw $s2, 64($sp)
    /* 800CA3E0 AFB1003C */  sw $s1, 60($sp)
    /* 800CA3E4 F7B60030 */  .word 0xF7B60030
    /* 800CA3E8 11CF0264 */  beq $t6, $t7, 0x800CAD7C
    /* 800CA3EC F7B40028 */  .word 0xF7B40028
    /* 800CA3F0 0C0328C0 */  jal 0x800CA300
    /* 800CA3F4 00000000 */  nop
    /* 800CA3F8 3C188011 */  lui $t8, 0x8011
    /* 800CA3FC 8F0374B8 */  lw $v1, 29880($t8)
    /* 800CA400 3C018014 */  lui $at, 0x8014
    /* 800CA404 30790001 */  andi $t9, $v1, 0x0001
    /* 800CA408 1320003A */  beq $t9, $zero, 0x800CA4F4
    /* 800CA40C AE030000 */  sw $v1, 0($s0)
    /* 800CA410 A02061F8 */  sb $zero, 25080($at)
    /* 800CA414 3C018014 */  lui $at, 0x8014
    /* 800CA418 A0206205 */  sb $zero, 25093($at)
    /* 800CA41C 3C018015 */  lui $at, 0x8015
    /* 800CA420 A0209414 */  sb $zero, -27628($at)
    /* 800CA424 3C018015 */  lui $at, 0x8015
    /* 800CA428 A0209774 */  sb $zero, -26764($at)
    /* 800CA42C 3C018015 */  lui $at, 0x8015
    /* 800CA430 A0209794 */  sb $zero, -26732($at)
    /* 800CA434 3C018015 */  lui $at, 0x8015
    /* 800CA438 0C0296EE */  jal 0x800A5BB8
    /* 800CA43C A02097C4 */  sb $zero, -26684($at)
    /* 800CA440 3C0E8015 */  lui $t6, 0x8015
    /* 800CA444 81CE6994 */  lb $t6, 27028($t6)
    /* 800CA448 24040020 */  addiu $a0, $zero, 32
    /* 800CA44C 24050010 */  addiu $a1, $zero, 16
    /* 800CA450 15C0000E */  bne $t6, $zero, 0x800CA48C
    /* 800CA454 24060010 */  addiu $a2, $zero, 16
    /* 800CA458 3C013F80 */  lui $at, 0x3F80
    /* 800CA45C 44812000 */  .word 0x44812000
    /* 800CA460 3C014348 */  lui $at, 0x4348
    /* 800CA464 44813000 */  .word 0x44813000
    /* 800CA468 240F0040 */  addiu $t7, $zero, 64
    /* 800CA46C AFAF0010 */  sw $t7, 16($sp)
    /* 800CA470 24070005 */  addiu $a3, $zero, 5
    /* 800CA474 AFA0001C */  sw $zero, 28($sp)
    /* 800CA478 E7A40014 */  swc1 $f4, 20($sp)
    /* 800CA47C 0C0322E3 */  jal 0x800C8B8C
    /* 800CA480 E7A60018 */  swc1 $f6, 24($sp)
    /* 800CA484 0C032217 */  jal 0x800C885C
    /* 800CA488 00000000 */  nop
    /* 800CA48C 0C0323E9 */  jal 0x800C8FA4
    /* 800CA490 24040001 */  addiu $a0, $zero, 1
    /* 800CA494 24020001 */  addiu $v0, $zero, 1
    /* 800CA498 3C018015 */  lui $at, 0x8015
    /* 800CA49C A422A108 */  sh $v0, -24312($at)
    /* 800CA4A0 3C018015 */  lui $at, 0x8015
    /* 800CA4A4 0C0326F8 */  jal 0x800C9BE0
    /* 800CA4A8 A4221AD0 */  sh $v0, 6864($at)
    /* 800CA4AC 3C0141F0 */  lui $at, 0x41F0
    /* 800CA4B0 44816000 */  .word 0x44816000
    /* 800CA4B4 0C00053C */  jal 0x800014F0
    /* 800CA4B8 00000000 */  nop
    /* 800CA4BC 0C0326B8 */  jal 0x800C9AE0
    /* 800CA4C0 00000000 */  nop
    /* 800CA4C4 0C0326B8 */  jal 0x800C9AE0
    /* 800CA4C8 00000000 */  nop
    /* 800CA4CC 0C0326B8 */  jal 0x800C9AE0
    /* 800CA4D0 00000000 */  nop
    /* 800CA4D4 0C032520 */  jal 0x800C9480
    /* 800CA4D8 00000000 */  nop
    /* 800CA4DC 24180002 */  addiu $t8, $zero, 2
    /* 800CA4E0 3C018011 */  lui $at, 0x8011
    /* 800CA4E4 3C038011 */  lui $v1, 0x8011
    /* 800CA4E8 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA4EC 10000208 */  beq $zero, $zero, 0x800CAD10
    /* 800CA4F0 AC3874B8 */  sw $t8, 29880($at)
    /* 800CA4F4 30790002 */  andi $t9, $v1, 0x0002
    /* 800CA4F8 53200035 */  .word 0x53200035
    /* 800CA4FC 0003C840 */  sll $t9, $v1, 1
    /* 800CA500 0C032438 */  jal 0x800C90E0
    /* 800CA504 00000000 */  nop
    /* 800CA508 0C0296EE */  jal 0x800A5BB8
    /* 800CA50C 00000000 */  nop
    /* 800CA510 3C0E8015 */  lui $t6, 0x8015
    /* 800CA514 81CE6994 */  lb $t6, 27028($t6)
    /* 800CA518 24040020 */  addiu $a0, $zero, 32
    /* 800CA51C 24050010 */  addiu $a1, $zero, 16
    /* 800CA520 15C0000E */  bne $t6, $zero, 0x800CA55C
    /* 800CA524 24060010 */  addiu $a2, $zero, 16
    /* 800CA528 3C013F80 */  lui $at, 0x3F80
    /* 800CA52C 44814000 */  .word 0x44814000
    /* 800CA530 3C014348 */  lui $at, 0x4348
    /* 800CA534 44815000 */  .word 0x44815000
    /* 800CA538 240F0040 */  addiu $t7, $zero, 64
    /* 800CA53C AFAF0010 */  sw $t7, 16($sp)
    /* 800CA540 24070005 */  addiu $a3, $zero, 5
    /* 800CA544 AFA0001C */  sw $zero, 28($sp)
    /* 800CA548 E7A80014 */  swc1 $f8, 20($sp)
    /* 800CA54C 0C0322E3 */  jal 0x800C8B8C
    /* 800CA550 E7AA0018 */  swc1 $f10, 24($sp)
    /* 800CA554 0C032217 */  jal 0x800C885C
    /* 800CA558 00000000 */  nop
    /* 800CA55C 3C188014 */  lui $t8, 0x8014
    /* 800CA560 83186115 */  lb $t8, 24853($t8)
    /* 800CA564 3C014120 */  lui $at, 0x4120
    /* 800CA568 44814000 */  .word 0x44814000
    /* 800CA56C 44982000 */  .word 0x44982000
    /* 800CA570 3C018012 */  lui $at, 0x8012
    /* 800CA574 C4363FB4 */  lwc1 $f22, 16308($at)
    /* 800CA578 468021A0 */  .word 0x468021A0
    /* 800CA57C 00008825 */  or $s1, $zero, $zero
    /* 800CA580 24120001 */  addiu $s2, $zero, 1
    /* 800CA584 0C032484 */  jal 0x800C9210
    /* 800CA588 46083503 */  .word 0x46083503
    /* 800CA58C 0C0323E9 */  jal 0x800C8FA4
    /* 800CA590 24040001 */  addiu $a0, $zero, 1
    /* 800CA594 0C0324DF */  jal 0x800C937C
    /* 800CA598 00000000 */  nop
    /* 800CA59C 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA5A0 0C032456 */  jal 0x800C9158
    /* 800CA5A4 00002825 */  or $a1, $zero, $zero
    /* 800CA5A8 00002025 */  or $a0, $zero, $zero
    /* 800CA5AC 0C032456 */  jal 0x800C9158
    /* 800CA5B0 24050001 */  addiu $a1, $zero, 1
    /* 800CA5B4 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA5B8 0C032130 */  jal 0x800C84C0
    /* 800CA5BC 24050001 */  addiu $a1, $zero, 1
    /* 800CA5C0 3C038011 */  lui $v1, 0x8011
    /* 800CA5C4 100001D2 */  beq $zero, $zero, 0x800CAD10
    /* 800CA5C8 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA5CC 0003C840 */  sll $t9, $v1, 1
    /* 800CA5D0 072001CF */  bltz $t9, 0x800CAD10
    /* 800CA5D4 306E0004 */  andi $t6, $v1, 0x0004
    /* 800CA5D8 51C0006F */  .word 0x51C0006F
    /* 800CA5DC 306E0008 */  andi $t6, $v1, 0x0008
    /* 800CA5E0 0C032438 */  jal 0x800C90E0
    /* 800CA5E4 00000000 */  nop
    /* 800CA5E8 0C0296EE */  jal 0x800A5BB8
    /* 800CA5EC 00000000 */  nop
    /* 800CA5F0 3C0F8015 */  lui $t7, 0x8015
    /* 800CA5F4 81EF6994 */  lb $t7, 27028($t7)
    /* 800CA5F8 24040020 */  addiu $a0, $zero, 32
    /* 800CA5FC 24050010 */  addiu $a1, $zero, 16
    /* 800CA600 15E0000E */  bne $t7, $zero, 0x800CA63C
    /* 800CA604 24060010 */  addiu $a2, $zero, 16
    /* 800CA608 3C013F80 */  lui $at, 0x3F80
    /* 800CA60C 44815000 */  .word 0x44815000
    /* 800CA610 3C014348 */  lui $at, 0x4348
    /* 800CA614 44812000 */  .word 0x44812000
    /* 800CA618 24180040 */  addiu $t8, $zero, 64
    /* 800CA61C AFB80010 */  sw $t8, 16($sp)
    /* 800CA620 24070005 */  addiu $a3, $zero, 5
    /* 800CA624 AFA0001C */  sw $zero, 28($sp)
    /* 800CA628 E7AA0014 */  swc1 $f10, 20($sp)
    /* 800CA62C 0C0322E3 */  jal 0x800C8B8C
    /* 800CA630 E7A40018 */  swc1 $f4, 24($sp)
    /* 800CA634 0C032217 */  jal 0x800C885C
    /* 800CA638 00000000 */  nop
    /* 800CA63C 3C198014 */  lui $t9, 0x8014
    /* 800CA640 83396115 */  lb $t9, 24853($t9)
    /* 800CA644 3C014120 */  lui $at, 0x4120
    /* 800CA648 44815000 */  .word 0x44815000
    /* 800CA64C 44993000 */  .word 0x44993000
    /* 800CA650 3C018012 */  lui $at, 0x8012
    /* 800CA654 C4363FB8 */  lwc1 $f22, 16312($at)
    /* 800CA658 46803220 */  .word 0x46803220
    /* 800CA65C 00008825 */  or $s1, $zero, $zero
    /* 800CA660 24120001 */  addiu $s2, $zero, 1
    /* 800CA664 0C032484 */  jal 0x800C9210
    /* 800CA668 460A4503 */  .word 0x460A4503
    /* 800CA66C 0C0323E9 */  jal 0x800C8FA4
    /* 800CA670 24040001 */  addiu $a0, $zero, 1
    /* 800CA674 0C0324DF */  jal 0x800C937C
    /* 800CA678 00000000 */  nop
    /* 800CA67C 3C0E8011 */  lui $t6, 0x8011
    /* 800CA680 8DCE74BC */  lw $t6, 29884($t6)
    /* 800CA684 3C01007C */  lui $at, 0x007C
    /* 800CA688 3421DFC0 */  ori $at, $at, 0xDFC0
    /* 800CA68C 01C17824 */  and $t7, $t6, $at
    /* 800CA690 15E00015 */  bne $t7, $zero, 0x800CA6E8
    /* 800CA694 3C038015 */  lui $v1, 0x8015
    /* 800CA698 8463A108 */  lh $v1, -24312($v1)
    /* 800CA69C 3C108015 */  lui $s0, 0x8015
    /* 800CA6A0 2610A118 */  addiu $s0, $s0, -24296
    /* 800CA6A4 18600010 */  blez $v1, 0x800CA6E8
    /* 800CA6A8 00003025 */  or $a2, $zero, $zero
    /* 800CA6AC 8E020048 */  lw $v0, 72($s0)
    /* 800CA6B0 5040000A */  .word 0x5040000A
    /* 800CA6B4 24C60001 */  addiu $a2, $a2, 1
    /* 800CA6B8 8C580000 */  lw $t8, 0($v0)
    /* 800CA6BC 00002825 */  or $a1, $zero, $zero
    /* 800CA6C0 8F040008 */  lw $a0, 8($t8)
    /* 800CA6C4 0C028D09 */  jal 0x800A3424
    /* 800CA6C8 AFA600DC */  sw $a2, 220($sp)
    /* 800CA6CC 3C038015 */  lui $v1, 0x8015
    /* 800CA6D0 8463A108 */  lh $v1, -24312($v1)
    /* 800CA6D4 8FA600DC */  lw $a2, 220($sp)
    /* 800CA6D8 24C60001 */  addiu $a2, $a2, 1
    /* 800CA6DC 00C3082A */  slt $at, $a2, $v1
    /* 800CA6E0 1420FFF2 */  bne $at, $zero, 0x800CA6AC
    /* 800CA6E4 2610004C */  addiu $s0, $s0, 76
    /* 800CA6E8 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA6EC 0C032456 */  jal 0x800C9158
    /* 800CA6F0 00002825 */  or $a1, $zero, $zero
    /* 800CA6F4 00002025 */  or $a0, $zero, $zero
    /* 800CA6F8 0C032456 */  jal 0x800C9158
    /* 800CA6FC 24050001 */  addiu $a1, $zero, 1
    /* 800CA700 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA704 0C032130 */  jal 0x800C84C0
    /* 800CA708 24050001 */  addiu $a1, $zero, 1
    /* 800CA70C 3C198015 */  lui $t9, 0x8015
    /* 800CA710 83396994 */  lb $t9, 27028($t9)
    /* 800CA714 3C0E8011 */  lui $t6, 0x8011
    /* 800CA718 13200019 */  beq $t9, $zero, 0x800CA780
    /* 800CA71C 00000000 */  nop
    /* 800CA720 8DCE74BC */  lw $t6, 29884($t6)
    /* 800CA724 3C01007C */  lui $at, 0x007C
    /* 800CA728 3C028014 */  lui $v0, 0x8014
    /* 800CA72C 01C17824 */  and $t7, $t6, $at
    /* 800CA730 15E00013 */  bne $t7, $zero, 0x800CA780
    /* 800CA734 24426108 */  addiu $v0, $v0, 24840
    /* 800CA738 8058000E */  lb $t8, 14($v0)
    /* 800CA73C 2B01000D */  slti $at, $t8, 13
    /* 800CA740 1020000F */  beq $at, $zero, 0x800CA780
    /* 800CA744 00000000 */  nop
    /* 800CA748 8059000C */  lb $t9, 12($v0)
    /* 800CA74C 3C014120 */  lui $at, 0x4120
    /* 800CA750 44814000 */  .word 0x44814000
    /* 800CA754 44992000 */  .word 0x44992000
    /* 800CA758 3C018012 */  lui $at, 0x8012
    /* 800CA75C C4363FBC */  lwc1 $f22, 16316($at)
    /* 800CA760 468021A0 */  .word 0x468021A0
    /* 800CA764 24110001 */  addiu $s1, $zero, 1
    /* 800CA768 00009025 */  or $s2, $zero, $zero
    /* 800CA76C 0C032484 */  jal 0x800C9210
    /* 800CA770 46083503 */  .word 0x46083503
    /* 800CA774 24040006 */  addiu $a0, $zero, 6
    /* 800CA778 0C032465 */  jal 0x800C9194
    /* 800CA77C 24050001 */  addiu $a1, $zero, 1
    /* 800CA780 3C018015 */  lui $at, 0x8015
    /* 800CA784 3C038011 */  lui $v1, 0x8011
    /* 800CA788 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA78C 10000160 */  beq $zero, $zero, 0x800CAD10
    /* 800CA790 A0201AD8 */  sb $zero, 6872($at)
    /* 800CA794 306E0008 */  andi $t6, $v1, 0x0008
    /* 800CA798 11C0002B */  beq $t6, $zero, 0x800CA848
    /* 800CA79C 3C108013 */  lui $s0, 0x8013
    /* 800CA7A0 8E10E6E0 */  lw $s0, -6432($s0)
    /* 800CA7A4 12000007 */  beq $s0, $zero, 0x800CA7C4
    /* 800CA7A8 00000000 */  nop
    /* 800CA7AC 0C031CC2 */  jal 0x800C7308
    /* 800CA7B0 02002025 */  or $a0, $s0, $zero
    /* 800CA7B4 8E0F0000 */  lw $t7, 0($s0)
    /* 800CA7B8 8DF00000 */  lw $s0, 0($t7)
    /* 800CA7BC 1600FFFB */  bne $s0, $zero, 0x800CA7AC
    /* 800CA7C0 00000000 */  nop
    /* 800CA7C4 0C031C2F */  jal 0x800C70BC
    /* 800CA7C8 00000000 */  nop
    /* 800CA7CC 0C032438 */  jal 0x800C90E0
    /* 800CA7D0 00000000 */  nop
    /* 800CA7D4 0C0296EE */  jal 0x800A5BB8
    /* 800CA7D8 00000000 */  nop
    /* 800CA7DC 3C188015 */  lui $t8, 0x8015
    /* 800CA7E0 83186994 */  lb $t8, 27028($t8)
    /* 800CA7E4 24040008 */  addiu $a0, $zero, 8
    /* 800CA7E8 24050008 */  addiu $a1, $zero, 8
    /* 800CA7EC 1700000B */  bne $t8, $zero, 0x800CA81C
    /* 800CA7F0 00003025 */  or $a2, $zero, $zero
    /* 800CA7F4 3C013F80 */  lui $at, 0x3F80
    /* 800CA7F8 44815000 */  .word 0x44815000
    /* 800CA7FC 3C014348 */  lui $at, 0x4348
    /* 800CA800 44812000 */  .word 0x44812000
    /* 800CA804 00003825 */  or $a3, $zero, $zero
    /* 800CA808 AFA00010 */  sw $zero, 16($sp)
    /* 800CA80C AFA0001C */  sw $zero, 28($sp)
    /* 800CA810 E7AA0014 */  swc1 $f10, 20($sp)
    /* 800CA814 0C0322E3 */  jal 0x800C8B8C
    /* 800CA818 E7A40018 */  swc1 $f4, 24($sp)
    /* 800CA81C 0C0323E9 */  jal 0x800C8FA4
    /* 800CA820 00002025 */  or $a0, $zero, $zero
    /* 800CA824 3C198015 */  lui $t9, 0x8015
    /* 800CA828 83396994 */  lb $t9, 27028($t9)
    /* 800CA82C 13200003 */  beq $t9, $zero, 0x800CA83C
    /* 800CA830 00000000 */  nop
    /* 800CA834 0C028491 */  jal 0x800A1244
    /* 800CA838 00000000 */  nop
    /* 800CA83C 3C038011 */  lui $v1, 0x8011
    /* 800CA840 10000133 */  beq $zero, $zero, 0x800CAD10
    /* 800CA844 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA848 306E0010 */  andi $t6, $v1, 0x0010
    /* 800CA84C 11C0000D */  beq $t6, $zero, 0x800CA884
    /* 800CA850 306F0020 */  andi $t7, $v1, 0x0020
    /* 800CA854 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA858 0C032456 */  jal 0x800C9158
    /* 800CA85C 00002825 */  or $a1, $zero, $zero
    /* 800CA860 00002025 */  or $a0, $zero, $zero
    /* 800CA864 0C032456 */  jal 0x800C9158
    /* 800CA868 24050001 */  addiu $a1, $zero, 1
    /* 800CA86C 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA870 0C032130 */  jal 0x800C84C0
    /* 800CA874 24050001 */  addiu $a1, $zero, 1
    /* 800CA878 3C038011 */  lui $v1, 0x8011
    /* 800CA87C 10000124 */  beq $zero, $zero, 0x800CAD10
    /* 800CA880 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA884 11E0001D */  beq $t7, $zero, 0x800CA8FC
    /* 800CA888 306E0040 */  andi $t6, $v1, 0x0040
    /* 800CA88C 3C188015 */  lui $t8, 0x8015
    /* 800CA890 8718A108 */  lh $t8, -24312($t8)
    /* 800CA894 00008025 */  or $s0, $zero, $zero
    /* 800CA898 1B00000C */  blez $t8, 0x800CA8CC
    /* 800CA89C 02002025 */  or $a0, $s0, $zero
    /* 800CA8A0 0C032456 */  jal 0x800C9158
    /* 800CA8A4 24050001 */  addiu $a1, $zero, 1
    /* 800CA8A8 02002025 */  or $a0, $s0, $zero
    /* 800CA8AC 0C032130 */  jal 0x800C84C0
    /* 800CA8B0 24050001 */  addiu $a1, $zero, 1
    /* 800CA8B4 3C198015 */  lui $t9, 0x8015
    /* 800CA8B8 8739A108 */  lh $t9, -24312($t9)
    /* 800CA8BC 26100001 */  addiu $s0, $s0, 1
    /* 800CA8C0 0219082A */  slt $at, $s0, $t9
    /* 800CA8C4 5420FFF6 */  .word 0x5420FFF6
    /* 800CA8C8 02002025 */  or $a0, $s0, $zero
    /* 800CA8CC 2A010004 */  slti $at, $s0, 4
    /* 800CA8D0 10200007 */  beq $at, $zero, 0x800CA8F0
    /* 800CA8D4 02002025 */  or $a0, $s0, $zero
    /* 800CA8D8 0C032456 */  jal 0x800C9158
    /* 800CA8DC 00002825 */  or $a1, $zero, $zero
    /* 800CA8E0 26100001 */  addiu $s0, $s0, 1
    /* 800CA8E4 2A010004 */  slti $at, $s0, 4
    /* 800CA8E8 5420FFFB */  .word 0x5420FFFB
    /* 800CA8EC 02002025 */  or $a0, $s0, $zero
    /* 800CA8F0 3C038011 */  lui $v1, 0x8011
    /* 800CA8F4 10000106 */  beq $zero, $zero, 0x800CAD10
    /* 800CA8F8 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA8FC 11C0000A */  beq $t6, $zero, 0x800CA928
    /* 800CA900 306F0200 */  andi $t7, $v1, 0x0200
    /* 800CA904 2404FFFF */  addiu $a0, $zero, -1
    /* 800CA908 0C032456 */  jal 0x800C9158
    /* 800CA90C 00002825 */  or $a1, $zero, $zero
    /* 800CA910 00002025 */  or $a0, $zero, $zero
    /* 800CA914 0C032456 */  jal 0x800C9158
    /* 800CA918 24050001 */  addiu $a1, $zero, 1
    /* 800CA91C 3C038011 */  lui $v1, 0x8011
    /* 800CA920 100000FB */  beq $zero, $zero, 0x800CAD10
    /* 800CA924 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA928 15E000F9 */  bne $t7, $zero, 0x800CAD10
    /* 800CA92C 30780400 */  andi $t8, $v1, 0x0400
    /* 800CA930 170000F7 */  bne $t8, $zero, 0x800CAD10
    /* 800CA934 30790800 */  andi $t9, $v1, 0x0800
    /* 800CA938 172000F5 */  bne $t9, $zero, 0x800CAD10
    /* 800CA93C 306E1000 */  andi $t6, $v1, 0x1000
    /* 800CA940 15C000F3 */  bne $t6, $zero, 0x800CAD10
    /* 800CA944 306F2000 */  andi $t7, $v1, 0x2000
    /* 800CA948 15E000F1 */  bne $t7, $zero, 0x800CAD10
    /* 800CA94C 0003C100 */  sll $t8, $v1, 4
    /* 800CA950 070000EF */  bltz $t8, 0x800CAD10
    /* 800CA954 30794000 */  andi $t9, $v1, 0x4000
    /* 800CA958 1320000A */  beq $t9, $zero, 0x800CA984
    /* 800CA95C 306E0100 */  andi $t6, $v1, 0x0100
    /* 800CA960 24040039 */  addiu $a0, $zero, 57
    /* 800CA964 00002825 */  or $a1, $zero, $zero
    /* 800CA968 00003025 */  or $a2, $zero, $zero
    /* 800CA96C 00003825 */  or $a3, $zero, $zero
    /* 800CA970 0C025DE6 */  jal 0x80097798
    /* 800CA974 AFA00010 */  sw $zero, 16($sp)
    /* 800CA978 3C038011 */  lui $v1, 0x8011
    /* 800CA97C 100000E4 */  beq $zero, $zero, 0x800CAD10
    /* 800CA980 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CA984 11C00023 */  beq $t6, $zero, 0x800CAA14
    /* 800CA988 30790080 */  andi $t9, $v1, 0x0080
    /* 800CA98C 24040038 */  addiu $a0, $zero, 56
    /* 800CA990 00002825 */  or $a1, $zero, $zero
    /* 800CA994 00003025 */  or $a2, $zero, $zero
    /* 800CA998 00003825 */  or $a3, $zero, $zero
    /* 800CA99C 0C025DE6 */  jal 0x80097798
    /* 800CA9A0 AFA00010 */  sw $zero, 16($sp)
    /* 800CA9A4 3C0F8015 */  lui $t7, 0x8015
    /* 800CA9A8 85EFA108 */  lh $t7, -24312($t7)
    /* 800CA9AC 00008025 */  or $s0, $zero, $zero
    /* 800CA9B0 19E0000C */  blez $t7, 0x800CA9E4
    /* 800CA9B4 02002025 */  or $a0, $s0, $zero
    /* 800CA9B8 0C032456 */  jal 0x800C9158
    /* 800CA9BC 24050001 */  addiu $a1, $zero, 1
    /* 800CA9C0 02002025 */  or $a0, $s0, $zero
    /* 800CA9C4 0C032130 */  jal 0x800C84C0
    /* 800CA9C8 24050001 */  addiu $a1, $zero, 1
    /* 800CA9CC 3C188015 */  lui $t8, 0x8015
    /* 800CA9D0 8718A108 */  lh $t8, -24312($t8)
    /* 800CA9D4 26100001 */  addiu $s0, $s0, 1
    /* 800CA9D8 0218082A */  slt $at, $s0, $t8
    /* 800CA9DC 5420FFF6 */  .word 0x5420FFF6
    /* 800CA9E0 02002025 */  or $a0, $s0, $zero
    /* 800CA9E4 2A010004 */  slti $at, $s0, 4
    /* 800CA9E8 10200007 */  beq $at, $zero, 0x800CAA08
    /* 800CA9EC 02002025 */  or $a0, $s0, $zero
    /* 800CA9F0 0C032456 */  jal 0x800C9158
    /* 800CA9F4 00002825 */  or $a1, $zero, $zero
    /* 800CA9F8 26100001 */  addiu $s0, $s0, 1
    /* 800CA9FC 2A010004 */  slti $at, $s0, 4
    /* 800CAA00 5420FFFB */  .word 0x5420FFFB
    /* 800CAA04 02002025 */  or $a0, $s0, $zero
    /* 800CAA08 3C038011 */  lui $v1, 0x8011
    /* 800CAA0C 100000C0 */  beq $zero, $zero, 0x800CAD10
    /* 800CAA10 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CAA14 13200010 */  beq $t9, $zero, 0x800CAA58
    /* 800CAA18 00037340 */  sll $t6, $v1, 13
    /* 800CAA1C 2404003C */  addiu $a0, $zero, 60
    /* 800CAA20 00002825 */  or $a1, $zero, $zero
    /* 800CAA24 00003025 */  or $a2, $zero, $zero
    /* 800CAA28 00003825 */  or $a3, $zero, $zero
    /* 800CAA2C 0C025DE6 */  jal 0x80097798
    /* 800CAA30 AFA00010 */  sw $zero, 16($sp)
    /* 800CAA34 2404FFFF */  addiu $a0, $zero, -1
    /* 800CAA38 0C032456 */  jal 0x800C9158
    /* 800CAA3C 00002825 */  or $a1, $zero, $zero
    /* 800CAA40 00002025 */  or $a0, $zero, $zero
    /* 800CAA44 0C032456 */  jal 0x800C9158
    /* 800CAA48 24050001 */  addiu $a1, $zero, 1
    /* 800CAA4C 3C038011 */  lui $v1, 0x8011
    /* 800CAA50 100000AF */  beq $zero, $zero, 0x800CAD10
    /* 800CAA54 8C6374B4 */  lw $v1, 29876($v1)
    /* 800CAA58 05C1008D */  bgez $t6, 0x800CAC90
    /* 800CAA5C 0003C1C0 */  sll $t8, $v1, 7
    /* 800CAA60 3C108011 */  lui $s0, 0x8011
    /* 800CAA64 26104650 */  addiu $s0, $s0, 18000
    /* 800CAA68 820F0000 */  lb $t7, 0($s0)
    /* 800CAA6C 15E00007 */  bne $t7, $zero, 0x800CAA8C
    /* 800CAA70 00000000 */  nop
    /* 800CAA74 0C031C2F */  jal 0x800C70BC
    /* 800CAA78 00000000 */  nop
    /* 800CAA7C 0C032438 */  jal 0x800C90E0
    /* 800CAA80 00000000 */  nop
    /* 800CAA84 0C0296EE */  jal 0x800A5BB8
    /* 800CAA88 00000000 */  nop
    /* 800CAA8C 0C0323E9 */  jal 0x800C8FA4
    /* 800CAA90 00002025 */  or $a0, $zero, $zero
    /* 800CAA94 3C108011 */  lui $s0, 0x8011
    /* 800CAA98 26104650 */  addiu $s0, $s0, 18000
    /* 800CAA9C 82180000 */  lb $t8, 0($s0)
    /* 800CAAA0 3C038015 */  lui $v1, 0x8015
    /* 800CAAA4 17000028 */  bne $t8, $zero, 0x800CAB48
    /* 800CAAA8 00000000 */  nop
    /* 800CAAAC 80636994 */  lb $v1, 27028($v1)
    /* 800CAAB0 3C028015 */  lui $v0, 0x8015
    /* 800CAAB4 14600017 */  bne $v1, $zero, 0x800CAB14
    /* 800CAAB8 00000000 */  nop
    /* 800CAABC 8C42A110 */  lw $v0, -24304($v0)
    /* 800CAAC0 24010006 */  addiu $at, $zero, 6
    /* 800CAAC4 10410013 */  beq $v0, $at, 0x800CAB14
    /* 800CAAC8 24010004 */  addiu $at, $zero, 4
    /* 800CAACC 10410011 */  beq $v0, $at, 0x800CAB14
    /* 800CAAD0 24010005 */  addiu $at, $zero, 5
    /* 800CAAD4 1041000F */  beq $v0, $at, 0x800CAB14
    /* 800CAAD8 24040008 */  addiu $a0, $zero, 8
    /* 800CAADC 3C013F80 */  lui $at, 0x3F80
    /* 800CAAE0 44813000 */  .word 0x44813000
    /* 800CAAE4 3C014348 */  lui $at, 0x4348
    /* 800CAAE8 44814000 */  .word 0x44814000
    /* 800CAAEC 24050008 */  addiu $a1, $zero, 8
    /* 800CAAF0 00003025 */  or $a2, $zero, $zero
    /* 800CAAF4 00003825 */  or $a3, $zero, $zero
    /* 800CAAF8 AFA00010 */  sw $zero, 16($sp)
    /* 800CAAFC AFA0001C */  sw $zero, 28($sp)
    /* 800CAB00 E7A60014 */  swc1 $f6, 20($sp)
    /* 800CAB04 0C0322E3 */  jal 0x800C8B8C
    /* 800CAB08 E7A80018 */  swc1 $f8, 24($sp)
    /* 800CAB0C 3C038015 */  lui $v1, 0x8015
    /* 800CAB10 80636994 */  lb $v1, 27028($v1)
    /* 800CAB14 1460000A */  bne $v1, $zero, 0x800CAB40
    /* 800CAB18 3C198015 */  lui $t9, 0x8015
    /* 800CAB1C 8F39A110 */  lw $t9, -24304($t9)
    /* 800CAB20 24010006 */  addiu $at, $zero, 6
    /* 800CAB24 24040005 */  addiu $a0, $zero, 5
    /* 800CAB28 17210005 */  bne $t9, $at, 0x800CAB40
    /* 800CAB2C 24050040 */  addiu $a1, $zero, 64
    /* 800CAB30 3C063F80 */  lui $a2, 0x3F80
    /* 800CAB34 3C074348 */  lui $a3, 0x4348
    /* 800CAB38 0C03213F */  jal 0x800C84FC
    /* 800CAB3C AFA00010 */  sw $zero, 16($sp)
    /* 800CAB40 0C02D57F */  jal 0x800B55FC
    /* 800CAB44 24040001 */  addiu $a0, $zero, 1
    /* 800CAB48 3C028015 */  lui $v0, 0x8015
    /* 800CAB4C 8C42A110 */  lw $v0, -24304($v0)
    /* 800CAB50 24010006 */  addiu $at, $zero, 6
    /* 800CAB54 10410008 */  beq $v0, $at, 0x800CAB78
    /* 800CAB58 24010004 */  addiu $at, $zero, 4
    /* 800CAB5C 10410006 */  beq $v0, $at, 0x800CAB78
    /* 800CAB60 24010005 */  addiu $at, $zero, 5
    /* 800CAB64 10410004 */  beq $v0, $at, 0x800CAB78
    /* 800CAB68 3C0E8015 */  lui $t6, 0x8015
    /* 800CAB6C 81CE6994 */  lb $t6, 27028($t6)
    /* 800CAB70 51C00004 */  .word 0x51C00004
    /* 800CAB74 820F0000 */  lb $t7, 0($s0)
    /* 800CAB78 0C028491 */  jal 0x800A1244
    /* 800CAB7C 00000000 */  nop
    /* 800CAB80 820F0000 */  lb $t7, 0($s0)
