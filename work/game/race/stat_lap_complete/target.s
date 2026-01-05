# Source: game_code.bin (decompressed)
# Address: 0x800FE924

glabel stat_lap_complete
    /* 800FE924 27BDFFB8 */  addiu $sp, $sp, -72
    /* 800FE928 44856000 */  .word 0x44856000
    /* 800FE92C AFA40048 */  sw $a0, 72($sp)
    /* 800FE930 AFBF0044 */  sw $ra, 68($sp)
    /* 800FE934 3C048014 */  lui $a0, 0x8014
    /* 800FE938 F7BC0038 */  .word 0xF7BC0038
    /* 800FE93C F7BA0030 */  .word 0xF7BA0030
    /* 800FE940 F7B80028 */  .word 0xF7B80028
    /* 800FE944 F7B60020 */  .word 0xF7B60020
    /* 800FE948 F7B40018 */  .word 0xF7B40018
    /* 800FE94C 24842728 */  addiu $a0, $a0, 10024
    /* 800FE950 00002825 */  or $a1, $zero, $zero
    /* 800FE954 24060001 */  addiu $a2, $zero, 1
    /* 800FE958 0C001C9C */  jal 0x80007270
    /* 800FE95C E7AC004C */  swc1 $f12, 76($sp)
    /* 800FE960 3C01BF80 */  lui $at, 0xBF80
    /* 800FE964 44810000 */  .word 0x44810000
    /* 800FE968 C7AC004C */  lwc1 $f12, 76($sp)
    /* 800FE96C 3C013F80 */  lui $at, 0x3F80
    /* 800FE970 4600603C */  .word 0x4600603C
    /* 800FE974 00000000 */  nop
    /* 800FE978 45020004 */  .word 0x45020004
    /* 800FE97C 44811000 */  .word 0x44811000
    /* 800FE980 1000000B */  beq $zero, $zero, 0x800FE9B0
    /* 800FE984 46000386 */  .word 0x46000386
    /* 800FE988 44811000 */  .word 0x44811000
    /* 800FE98C 00000000 */  nop
    /* 800FE990 460C103C */  .word 0x460C103C
    /* 800FE994 00000000 */  nop
    /* 800FE998 45020004 */  .word 0x45020004
    /* 800FE99C 46006006 */  .word 0x46006006
    /* 800FE9A0 10000002 */  beq $zero, $zero, 0x800FE9AC
    /* 800FE9A4 46001006 */  .word 0x46001006
    /* 800FE9A8 46006006 */  .word 0x46006006
    /* 800FE9AC 46000386 */  .word 0x46000386
    /* 800FE9B0 3C01C000 */  lui $at, 0xC000
    /* 800FE9B4 4481E000 */  .word 0x4481E000
    /* 800FE9B8 8FA40048 */  lw $a0, 72($sp)
    /* 800FE9BC 46007606 */  .word 0x46007606
    /* 800FE9C0 4600E586 */  .word 0x4600E586
    /* 800FE9C4 0C024729 */  jal 0x80091CA4
    /* 800FE9C8 4600E686 */  .word 0x4600E686
    /* 800FE9CC 3C048014 */  lui $a0, 0x8014
    /* 800FE9D0 24842728 */  addiu $a0, $a0, 10024
    /* 800FE9D4 00002825 */  or $a1, $zero, $zero
    /* 800FE9D8 0C001D78 */  jal 0x800075E0
    /* 800FE9DC 00003025 */  or $a2, $zero, $zero
    /* 800FE9E0 8FBF0044 */  lw $ra, 68($sp)
    /* 800FE9E4 D7B40018 */  .word 0xD7B40018
    /* 800FE9E8 D7B60020 */  .word 0xD7B60020
    /* 800FE9EC D7B80028 */  .word 0xD7B80028
    /* 800FE9F0 D7BA0030 */  .word 0xD7BA0030
    /* 800FE9F4 D7BC0038 */  .word 0xD7BC0038
    /* 800FE9F8 03E00008 */  jr $ra
    /* 800FE9FC 27BD0048 */  addiu $sp, $sp, 72
