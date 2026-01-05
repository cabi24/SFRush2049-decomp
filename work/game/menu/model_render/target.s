# Source: game_code.bin (decompressed)
# Address: 0x800EB400

glabel model_render
    /* 800EB400 1000009E */  beq $zero, $zero, 0x800EB67C
    /* 800EB404 E612002C */  swc1 $f18, 44($s0)
    /* 800EB408 C6080024 */  lwc1 $f8, 36($s0)
    /* 800EB40C C6060028 */  lwc1 $f6, 40($s0)
    /* 800EB410 C610002C */  lwc1 $f16, 44($s0)
    /* 800EB414 26050024 */  addiu $a1, $s0, 36
    /* 800EB418 AFA50030 */  sw $a1, 48($sp)
    /* 800EB41C 01002025 */  or $a0, $t0, $zero
    /* 800EB420 E7A8004C */  swc1 $f8, 76($sp)
    /* 800EB424 E7A60050 */  swc1 $f6, 80($sp)
    /* 800EB428 0C03A8FD */  jal 0x800EA3F4
    /* 800EB42C E7B00054 */  swc1 $f16, 84($sp)
    /* 800EB430 26040060 */  addiu $a0, $s0, 96
    /* 800EB434 0C0235AC */  jal 0x8008D6B0
    /* 800EB438 02002825 */  or $a1, $s0, $zero
    /* 800EB43C C60A0084 */  lwc1 $f10, 132($s0)
    /* 800EB440 C6040088 */  lwc1 $f4, 136($s0)
    /* 800EB444 C612008C */  lwc1 $f18, 140($s0)
    /* 800EB448 8FB8003C */  lw $t8, 60($sp)
    /* 800EB44C E60A0024 */  swc1 $f10, 36($s0)
    /* 800EB450 E6040028 */  swc1 $f4, 40($s0)
    /* 800EB454 E612002C */  swc1 $f18, 44($s0)
    /* 800EB458 871906C4 */  lh $t9, 1732($t8)
    /* 800EB45C 8FA40030 */  lw $a0, 48($sp)
    /* 800EB460 07230087 */  .word 0x07230087
    /* 800EB464 8FBF001C */  lw $ra, 28($sp)
    /* 800EB468 0C031A70 */  jal 0x800C69C0
    /* 800EB46C 27A5004C */  addiu $a1, $sp, 76
    /* 800EB470 10000083 */  beq $zero, $zero, 0x800EB680
    /* 800EB474 8FBF001C */  lw $ra, 28($sp)
    /* 800EB478 C6080024 */  lwc1 $f8, 36($s0)
    /* 800EB47C C6060028 */  lwc1 $f6, 40($s0)
    /* 800EB480 C610002C */  lwc1 $f16, 44($s0)
    /* 800EB484 26050024 */  addiu $a1, $s0, 36
    /* 800EB488 AFA50030 */  sw $a1, 48($sp)
    /* 800EB48C 01002025 */  or $a0, $t0, $zero
    /* 800EB490 25060050 */  addiu $a2, $t0, 80
    /* 800EB494 27A7004C */  addiu $a3, $sp, 76
    /* 800EB498 E7A8004C */  swc1 $f8, 76($sp)
    /* 800EB49C E7A60050 */  swc1 $f6, 80($sp)
    /* 800EB4A0 0C03A8B7 */  jal 0x800EA2DC
    /* 800EB4A4 E7B00054 */  swc1 $f16, 84($sp)
    /* 800EB4A8 26040060 */  addiu $a0, $s0, 96
    /* 800EB4AC 0C0235AC */  jal 0x8008D6B0
    /* 800EB4B0 02002825 */  or $a1, $s0, $zero
    /* 800EB4B4 C60A0084 */  lwc1 $f10, 132($s0)
    /* 800EB4B8 C6040088 */  lwc1 $f4, 136($s0)
    /* 800EB4BC C612008C */  lwc1 $f18, 140($s0)
    /* 800EB4C0 8FA40030 */  lw $a0, 48($sp)
    /* 800EB4C4 27A5004C */  addiu $a1, $sp, 76
    /* 800EB4C8 E60A0024 */  swc1 $f10, 36($s0)
    /* 800EB4CC E6040028 */  swc1 $f4, 40($s0)
    /* 800EB4D0 0C031A70 */  jal 0x800C69C0
    /* 800EB4D4 E612002C */  swc1 $f18, 44($s0)
    /* 800EB4D8 10000069 */  beq $zero, $zero, 0x800EB680
    /* 800EB4DC 8FBF001C */  lw $ra, 28($sp)
    /* 800EB4E0 C6080024 */  lwc1 $f8, 36($s0)
    /* 800EB4E4 C6060028 */  lwc1 $f6, 40($s0)
    /* 800EB4E8 C610002C */  lwc1 $f16, 44($s0)
    /* 800EB4EC 01002025 */  or $a0, $t0, $zero
    /* 800EB4F0 26050024 */  addiu $a1, $s0, 36
    /* 800EB4F4 25060050 */  addiu $a2, $t0, 80
    /* 800EB4F8 27A7004C */  addiu $a3, $sp, 76
    /* 800EB4FC E7A8004C */  swc1 $f8, 76($sp)
    /* 800EB500 E7A60050 */  swc1 $f6, 80($sp)
    /* 800EB504 0C03A8B7 */  jal 0x800EA2DC
    /* 800EB508 E7B00054 */  swc1 $f16, 84($sp)
    /* 800EB50C 26040060 */  addiu $a0, $s0, 96
    /* 800EB510 0C0235AC */  jal 0x8008D6B0
    /* 800EB514 02002825 */  or $a1, $s0, $zero
    /* 800EB518 C60A0084 */  lwc1 $f10, 132($s0)
    /* 800EB51C C6040088 */  lwc1 $f4, 136($s0)
    /* 800EB520 C612008C */  lwc1 $f18, 140($s0)
    /* 800EB524 E60A0024 */  swc1 $f10, 36($s0)
    /* 800EB528 E6040028 */  swc1 $f4, 40($s0)
    /* 800EB52C 10000053 */  beq $zero, $zero, 0x800EB67C
    /* 800EB530 E612002C */  swc1 $f18, 44($s0)
    /* 800EB534 26050024 */  addiu $a1, $s0, 36
    /* 800EB538 AFA50030 */  sw $a1, 48($sp)
    /* 800EB53C 01002025 */  or $a0, $t0, $zero
    /* 800EB540 25060050 */  addiu $a2, $t0, 80
    /* 800EB544 0C03A842 */  jal 0x800EA108
    /* 800EB548 AFA80060 */  sw $t0, 96($sp)
    /* 800EB54C C6080084 */  lwc1 $f8, 132($s0)
    /* 800EB550 C6060088 */  lwc1 $f6, 136($s0)
    /* 800EB554 C610008C */  lwc1 $f16, 140($s0)
    /* 800EB558 26040060 */  addiu $a0, $s0, 96
    /* 800EB55C 02002825 */  or $a1, $s0, $zero
    /* 800EB560 E6080024 */  swc1 $f8, 36($s0)
    /* 800EB564 E6060028 */  swc1 $f6, 40($s0)
    /* 800EB568 0C0235AC */  jal 0x8008D6B0
    /* 800EB56C E610002C */  swc1 $f16, 44($s0)
    /* 800EB570 8FA80060 */  lw $t0, 96($sp)
    /* 800EB574 8FA40030 */  lw $a0, 48($sp)
    /* 800EB578 0C031A70 */  jal 0x800C69C0
    /* 800EB57C 25050008 */  addiu $a1, $t0, 8
    /* 800EB580 1000003F */  beq $zero, $zero, 0x800EB680
    /* 800EB584 8FBF001C */  lw $ra, 28($sp)
    /* 800EB588 01002025 */  or $a0, $t0, $zero
    /* 800EB58C 26050024 */  addiu $a1, $s0, 36
    /* 800EB590 0C03A78B */  jal 0x800E9E2C
    /* 800EB594 25060050 */  addiu $a2, $t0, 80
    /* 800EB598 C60A0084 */  lwc1 $f10, 132($s0)
    /* 800EB59C C6040088 */  lwc1 $f4, 136($s0)
    /* 800EB5A0 C612008C */  lwc1 $f18, 140($s0)
    /* 800EB5A4 26040060 */  addiu $a0, $s0, 96
    /* 800EB5A8 02002825 */  or $a1, $s0, $zero
    /* 800EB5AC E60A0024 */  swc1 $f10, 36($s0)
    /* 800EB5B0 E6040028 */  swc1 $f4, 40($s0)
    /* 800EB5B4 0C0235AC */  jal 0x8008D6B0
    /* 800EB5B8 E612002C */  swc1 $f18, 44($s0)
    /* 800EB5BC 10000030 */  beq $zero, $zero, 0x800EB680
    /* 800EB5C0 8FBF001C */  lw $ra, 28($sp)
    /* 800EB5C4 24040002 */  addiu $a0, $zero, 2
    /* 800EB5C8 01002825 */  or $a1, $t0, $zero
    /* 800EB5CC 26060024 */  addiu $a2, $s0, 36
    /* 800EB5D0 0C03A71C */  jal 0x800E9C70
    /* 800EB5D4 25070050 */  addiu $a3, $t0, 80
    /* 800EB5D8 C6080084 */  lwc1 $f8, 132($s0)
    /* 800EB5DC C6060088 */  lwc1 $f6, 136($s0)
    /* 800EB5E0 C610008C */  lwc1 $f16, 140($s0)
    /* 800EB5E4 26040060 */  addiu $a0, $s0, 96
    /* 800EB5E8 02002825 */  or $a1, $s0, $zero
    /* 800EB5EC E6080024 */  swc1 $f8, 36($s0)
    /* 800EB5F0 E6060028 */  swc1 $f6, 40($s0)
    /* 800EB5F4 0C0235AC */  jal 0x8008D6B0
    /* 800EB5F8 E610002C */  swc1 $f16, 44($s0)
    /* 800EB5FC 10000020 */  beq $zero, $zero, 0x800EB680
    /* 800EB600 8FBF001C */  lw $ra, 28($sp)
    /* 800EB604 24040002 */  addiu $a0, $zero, 2
    /* 800EB608 01002825 */  or $a1, $t0, $zero
    /* 800EB60C 26060024 */  addiu $a2, $s0, 36
    /* 800EB610 0C03A577 */  jal 0x800E95DC
    /* 800EB614 25070050 */  addiu $a3, $t0, 80
    /* 800EB618 C60A0084 */  lwc1 $f10, 132($s0)
    /* 800EB61C C6040088 */  lwc1 $f4, 136($s0)
    /* 800EB620 C612008C */  lwc1 $f18, 140($s0)
    /* 800EB624 26040060 */  addiu $a0, $s0, 96
    /* 800EB628 02002825 */  or $a1, $s0, $zero
    /* 800EB62C E60A0024 */  swc1 $f10, 36($s0)
    /* 800EB630 E6040028 */  swc1 $f4, 40($s0)
    /* 800EB634 0C0235AC */  jal 0x8008D6B0
    /* 800EB638 E612002C */  swc1 $f18, 44($s0)
    /* 800EB63C 10000010 */  beq $zero, $zero, 0x800EB680
    /* 800EB640 8FBF001C */  lw $ra, 28($sp)
    /* 800EB644 24040002 */  addiu $a0, $zero, 2
    /* 800EB648 01002825 */  or $a1, $t0, $zero
    /* 800EB64C 26060024 */  addiu $a2, $s0, 36
    /* 800EB650 0C03A3C4 */  jal 0x800E8F10
    /* 800EB654 25070050 */  addiu $a3, $t0, 80
    /* 800EB658 C6080084 */  lwc1 $f8, 132($s0)
    /* 800EB65C C6060088 */  lwc1 $f6, 136($s0)
    /* 800EB660 C610008C */  lwc1 $f16, 140($s0)
    /* 800EB664 26040060 */  addiu $a0, $s0, 96
    /* 800EB668 02002825 */  or $a1, $s0, $zero
    /* 800EB66C E6080024 */  swc1 $f8, 36($s0)
    /* 800EB670 E6060028 */  swc1 $f6, 40($s0)
    /* 800EB674 0C0235AC */  jal 0x8008D6B0
    /* 800EB678 E610002C */  swc1 $f16, 44($s0)
    /* 800EB67C 8FBF001C */  lw $ra, 28($sp)
    /* 800EB680 8FB00018 */  lw $s0, 24($sp)
    /* 800EB684 27BD0060 */  addiu $sp, $sp, 96
    /* 800EB688 03E00008 */  jr $ra
    /* 800EB68C 00000000 */  nop
