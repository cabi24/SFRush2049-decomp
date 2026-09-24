.set noreorder
.set noat
.text
.globl func_80095EC0
func_80095EC0:
.L80095EC0:
    srl     $a2,$a1,0x2
.L80095EC4:
    move    $v0,$zero
.L80095EC8:
    beqz    $a2,.L80095EEC
.L80095ECC:
    move    $v1,$a0
.L80095ED0:
    lui     $a0,0x7fff
.L80095ED4:
    ori     $a0,$a0,0xbad
.L80095ED8:
    addiu   $v0,$v0,1
.L80095EDC:
    sltu    $at,$v0,$a2
.L80095EE0:
    sw      $a0,0($v1)
.L80095EE4:
    bnez    $at,.L80095ED8
.L80095EE8:
    addiu   $v1,$v1,4
.L80095EEC:
    jr      $ra
.L80095EF0:
    nop
