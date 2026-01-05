# Source: game_code.bin (decompressed)
# Address: 0x800BA644

glabel audio_heap_alloc
    /* 800BA644 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800BA648 C4440018 */  lwc1 $f4, 24($v0)
    /* 800BA64C C4460020 */  lwc1 $f6, 32($v0)
    /* 800BA650 3C018012 */  lui $at, 0x8012
    /* 800BA654 F7B40008 */  .word 0xF7B40008
    /* 800BA658 AFA40038 */  sw $a0, 56($sp)
    /* 800BA65C C4203E00 */  lwc1 $f0, 15872($at)
    /* 800BA660 2403FFFF */  addiu $v1, $zero, -1
    /* 800BA664 00002825 */  or $a1, $zero, $zero
    /* 800BA668 E7A40020 */  swc1 $f4, 32($sp)
    /* 800BA66C 04C0004E */  bltz $a2, 0x800BA7A8
    /* 800BA670 E7A60024 */  swc1 $f6, 36($sp)
    /* 800BA674 3C048014 */  lui $a0, 0x8014
    /* 800BA678 4480A000 */  .word 0x4480A000
    /* 800BA67C 8C8407F4 */  lw $a0, 2036($a0)
    /* 800BA680 C442000C */  lwc1 $f2, 12($v0)
    /* 800BA684 C44C0014 */  lwc1 $f12, 20($v0)
    /* 800BA688 C7B20030 */  lwc1 $f18, 48($sp)
    /* 800BA68C 24090006 */  addiu $t1, $zero, 6
    /* 800BA690 87A80014 */  lh $t0, 20($sp)
    /* 800BA694 14A60002 */  bne $a1, $a2, 0x800BA6A0
    /* 800BA698 00000000 */  nop
    /* 800BA69C 00002825 */  or $a1, $zero, $zero
    /* 800BA6A0 00A90019 */  multu $a1, $t1
    /* 800BA6A4 00005012 */  mflo $t2
    /* 800BA6A8 008A3821 */  addu $a3, $a0, $t2
    /* 800BA6AC 84EB0000 */  lh $t3, 0($a3)
    /* 800BA6B0 448B4000 */  .word 0x448B4000
    /* 800BA6B4 00000000 */  nop
    /* 800BA6B8 468042A0 */  .word 0x468042A0
    /* 800BA6BC 46025101 */  .word 0x46025101
    /* 800BA6C0 E7A40028 */  swc1 $f4, 40($sp)
    /* 800BA6C4 84EC0004 */  lh $t4, 4($a3)
    /* 800BA6C8 24070001 */  addiu $a3, $zero, 1
    /* 800BA6CC 448C3000 */  .word 0x448C3000
    /* 800BA6D0 00000000 */  nop
    /* 800BA6D4 46803220 */  .word 0x46803220
    /* 800BA6D8 C7A60024 */  lwc1 $f6, 36($sp)
    /* 800BA6DC 460C4281 */  .word 0x460C4281
    /* 800BA6E0 460A3202 */  .word 0x460A3202
    /* 800BA6E4 C7A60020 */  lwc1 $f6, 32($sp)
    /* 800BA6E8 E7AA002C */  swc1 $f10, 44($sp)
    /* 800BA6EC 46062282 */  .word 0x46062282
    /* 800BA6F0 C7A6002C */  lwc1 $f6, 44($sp)
    /* 800BA6F4 460A4100 */  .word 0x460A4100
    /* 800BA6F8 46063202 */  .word 0x46063202
    /* 800BA6FC C7AA0028 */  lwc1 $f10, 40($sp)
    /* 800BA700 4614203C */  .word 0x4614203C
    /* 800BA704 460A5102 */  .word 0x460A5102
    /* 800BA708 45000003 */  .word 0x45000003
    /* 800BA70C 46044380 */  .word 0x46044380
    /* 800BA710 10000001 */  beq $zero, $zero, 0x800BA718
    /* 800BA714 2407FFFF */  addiu $a3, $zero, -1
    /* 800BA718 4600703C */  .word 0x4600703C
    /* 800BA71C 00000000 */  nop
    /* 800BA720 45000003 */  .word 0x45000003
    /* 800BA724 00000000 */  nop
    /* 800BA728 46007006 */  .word 0x46007006
    /* 800BA72C A7A50012 */  sh $a1, 18($sp)
    /* 800BA730 04620010 */  .word 0x04620010
    /* 800BA734 24630001 */  addiu $v1, $v1, 1
    /* 800BA738 8C4D0024 */  lw $t5, 36($v0)
    /* 800BA73C 448D3000 */  .word 0x448D3000
    /* 800BA740 00000000 */  nop
    /* 800BA744 46803420 */  .word 0x46803420
    /* 800BA748 4610703E */  .word 0x4610703E
    /* 800BA74C 00000000 */  nop
    /* 800BA750 45020008 */  .word 0x45020008
    /* 800BA754 24630001 */  addiu $v1, $v1, 1
    /* 800BA758 4610903E */  .word 0x4610903E
    /* 800BA75C 00000000 */  nop
    /* 800BA760 45020004 */  .word 0x45020004
    /* 800BA764 24630001 */  addiu $v1, $v1, 1
    /* 800BA768 14E8000F */  bne $a3, $t0, 0x800BA7A8
    /* 800BA76C 00000000 */  nop
    /* 800BA770 24630001 */  addiu $v1, $v1, 1
    /* 800BA774 0003CC00 */  sll $t9, $v1, 16
    /* 800BA778 00191C03 */  sra $v1, $t9, 16
    /* 800BA77C 24A50001 */  addiu $a1, $a1, 1
    /* 800BA780 00074400 */  sll $t0, $a3, 16
    /* 800BA784 00057C00 */  sll $t7, $a1, 16
    /* 800BA788 0066082A */  slt $at, $v1, $a2
    /* 800BA78C 00087403 */  sra $t6, $t0, 16
    /* 800BA790 000F2C03 */  sra $a1, $t7, 16
    /* 800BA794 46007486 */  .word 0x46007486
    /* 800BA798 1420FFBE */  bne $at, $zero, 0x800BA694
    /* 800BA79C 01C04025 */  or $t0, $t6, $zero
    /* 800BA7A0 E7B20030 */  swc1 $f18, 48($sp)
    /* 800BA7A4 A7AE0014 */  sh $t6, 20($sp)
    /* 800BA7A8 14660003 */  bne $v1, $a2, 0x800BA7B8
    /* 800BA7AC D7B40008 */  .word 0xD7B40008
    /* 800BA7B0 10000002 */  beq $zero, $zero, 0x800BA7BC
    /* 800BA7B4 87A20012 */  lh $v0, 18($sp)
    /* 800BA7B8 00A01025 */  or $v0, $a1, $zero
    /* 800BA7BC 03E00008 */  jr $ra
    /* 800BA7C0 27BD0038 */  addiu $sp, $sp, 56
