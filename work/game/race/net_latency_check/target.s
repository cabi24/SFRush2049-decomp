# Source: game_code.bin (decompressed)
# Address: 0x800F1C98

glabel net_latency_check
    /* 800F1C98 A2600000 */  sb $zero, 0($s3)
    /* 800F1C9C 14A20002 */  bne $a1, $v0, 0x800F1CA8
    /* 800F1CA0 00000000 */  nop
    /* 800F1CA4 A1100000 */  sb $s0, 0($t0)
    /* 800F1CA8 1643000A */  bne $s2, $v1, 0x800F1CD4
    /* 800F1CAC 3C058015 */  lui $a1, 0x8015
    /* 800F1CB0 3C068014 */  lui $a2, 0x8014
    /* 800F1CB4 8CC661A8 */  lw $a2, 25000($a2)
    /* 800F1CB8 8CA5A160 */  lw $a1, -24224($a1)
    /* 800F1CBC 82240000 */  lb $a0, 0($s1)
    /* 800F1CC0 02003825 */  or $a3, $s0, $zero
    /* 800F1CC4 0C033010 */  jal 0x800CC040
    /* 800F1CC8 24140001 */  addiu $s4, $zero, 1
    /* 800F1CCC 3C018014 */  lui $at, 0x8014
    /* 800F1CD0 AC2061A8 */  sw $zero, 25000($at)
    /* 800F1CD4 16900005 */  bne $s4, $s0, 0x800F1CEC
    /* 800F1CD8 00000000 */  nop
    /* 800F1CDC 0C03C484 */  jal 0x800F1210
    /* 800F1CE0 24120005 */  addiu $s2, $zero, 5
    /* 800F1CE4 3C118014 */  lui $s1, 0x8014
    /* 800F1CE8 263160C0 */  addiu $s1, $s1, 24768
    /* 800F1CEC 0C03C3D1 */  jal 0x800F0F44
    /* 800F1CF0 823E0000 */  lb $fp, 0($s1)
    /* 800F1CF4 8FBF0014 */  lw $ra, 20($sp)
    /* 800F1CF8 27BD0018 */  addiu $sp, $sp, 24
    /* 800F1CFC 03E00008 */  jr $ra
    /* 800F1D00 00000000 */  nop
