glabel control_settings
.L800DD4AC:
    addiu   $sp,$sp,-1896
.L800DD4B0:
    lw      $t0,1912($sp)
.L800DD4B4:
    sw      $s3,52($sp)
.L800DD4B8:
    move    $s3,$a0
.L800DD4BC:
    sw      $ra,76($sp)
.L800DD4C0:
    sw      $s8,72($sp)
.L800DD4C4:
    sw      $s7,68($sp)
.L800DD4C8:
    sw      $s6,64($sp)
.L800DD4CC:
    sw      $s5,60($sp)
.L800DD4D0:
    sw      $s4,56($sp)
.L800DD4D4:
    sw      $s2,48($sp)
.L800DD4D8:
    sw      $s1,44($sp)
.L800DD4DC:
    sw      $s0,40($sp)
.L800DD4E0:
    sw      $a1,1900($sp)
.L800DD4E4:
    sw      $a2,1904($sp)
.L800DD4E8:
    beqz    $t0,.L800DD508
.L800DD4EC:
    sw      $a3,1908($sp)
.L800DD4F0:
    jalr    $t0
.L800DD4F4:
    nop
.L800DD4F8:
    beqzl   $v0,.L800DD50C
.L800DD4FC:
    sll     $t6,$s3,0x2
.L800DD500:
    jal     func_800DD45C
.L800DD504:
    move    $a0,$s3
.L800DD508:
    sll     $t6,$s3,0x2
.L800DD50C:
    subu    $t6,$t6,$s3
.L800DD510:
    sll     $t6,$t6,0x2
.L800DD514:
    subu    $t6,$t6,$s3
.L800DD518:
    lui     $t7,0x8015
.L800DD51C:
    addiu   $t7,$t7,16344
.L800DD520:
    sll     $t6,$t6,0x3
.L800DD524:
    addu    $t8,$t6,$t7
.L800DD528:
    sw      $t8,92($sp)
.L800DD52C:
    addiu   $s1,$t8,44
.L800DD530:
    li      $s4,1
.L800DD534:
    lw      $t9,20($s1)
.L800DD538:
    bnez    $t9,.L800DD57C
.L800DD53C:
    nop
.L800DD540:
    lb      $t6,0($s1)
.L800DD544:
    bnezl   $t6,.L800DD574
.L800DD548:
    addiu   $s4,$s4,-1
.L800DD54C:
    lw      $t7,1908($sp)
.L800DD550:
    li      $t0,1
.L800DD554:
    sb      $t0,0($s1)
.L800DD558:
    sb      $t0,0($t7)
.L800DD55C:
    lw      $t8,1904($sp)
.L800DD560:
    sb      $zero,0($t8)
.L800DD564:
    lw      $t9,1900($sp)
.L800DD568:
    b       .L800DDE24
.L800DD56C:
    sb      $zero,0($t9)
.L800DD570:
    addiu   $s4,$s4,-1
.L800DD574:
    bgez    $s4,.L800DD534
.L800DD578:
    addiu   $s1,$s1,-44
.L800DD57C:
    bltz    $s4,.L800DDE74
.L800DD580:
    lui     $t6,0x8015
.L800DD584:
    lw      $t6,26956($t6)
.L800DD588:
    lui     $t7,0x8015
.L800DD58C:
    lui     $a0,0x8014
.L800DD590:
    sw      $t6,32($s1)
.L800DD594:
    lw      $t7,26948($t7)
.L800DD598:
    addiu   $a0,$a0,25040
.L800DD59C:
    move    $a1,$zero
.L800DD5A0:
    li      $a2,1
.L800DD5A4:
    jal     func_80007270
.L800DD5A8:
    sw      $t7,36($s1)
.L800DD5AC:
    li      $s2,11
.L800DD5B0:
    sw      $s1,88($sp)
.L800DD5B4:
    jal     slot_state_setup
.L800DD5B8:
    sw      $s3,1896($sp)
.L800DD5BC:
    lui     $a0,0x8014
.L800DD5C0:
    addiu   $a0,$a0,25040
.L800DD5C4:
    lw      $s1,88($sp)
.L800DD5C8:
    lw      $s3,1896($sp)
.L800DD5CC:
    move    $s0,$v0
.L800DD5D0:
    move    $a1,$zero
.L800DD5D4:
    jal     func_800075e0
.L800DD5D8:
    move    $a2,$zero
.L800DD5DC:
    jal     dispatch_handler
.L800DD5E0:
    li      $a0,1
.L800DD5E4:
    lui     $a0,0x8014
.L800DD5E8:
    addiu   $a0,$a0,25040
.L800DD5EC:
    move    $a1,$zero
.L800DD5F0:
    jal     func_80007270
.L800DD5F4:
    li      $a2,1
.L800DD5F8:
    li      $s2,11
.L800DD5FC:
    sw      $s1,88($sp)
.L800DD600:
    jal     slot_state_setup
.L800DD604:
    sw      $s3,1896($sp)
.L800DD608:
    lui     $a0,0x8014
.L800DD60C:
    addiu   $a0,$a0,25040
.L800DD610:
    lw      $s1,88($sp)
.L800DD614:
    lw      $s3,1896($sp)
.L800DD618:
    move    $s0,$v0
.L800DD61C:
    move    $a1,$zero
.L800DD620:
    jal     func_800075e0
.L800DD624:
    move    $a2,$zero
.L800DD628:
    jal     dispatch_handler
.L800DD62C:
    li      $a0,1
.L800DD630:
    lui     $a1,0x8012
.L800DD634:
    addiu   $a1,$a1,4192
.L800DD638:
    addiu   $a0,$sp,1816
.L800DD63C:
    jal     func_80004990
.L800DD640:
    addiu   $a2,$s3,1
.L800DD644:
    lh      $t8,28($s1)
.L800DD648:
    lh      $t6,24($s1)
.L800DD64C:
    addiu   $a0,$sp,1816
.L800DD650:
    bgez    $t8,.L800DD660
.L800DD654:
    sra     $t9,$t8,0x1
.L800DD658:
    addiu   $at,$t8,1
.L800DD65C:
    sra     $t9,$at,0x1
.L800DD660:
    addu    $s0,$t9,$t6
.L800DD664:
    sll     $t7,$s0,0x10
.L800DD668:
    sra     $s0,$t7,0x10
.L800DD66C:
    jal     object_utility
.L800DD670:
    li      $a1,-1
.L800DD674:
    srl     $t9,$v0,0x1
.L800DD678:
    subu    $t7,$s0,$t9
.L800DD67C:
    sll     $a0,$t7,0x10
.L800DD680:
    sra     $t8,$a0,0x10
.L800DD684:
    move    $a0,$t8
.L800DD688:
    lh      $a1,26($s1)
.L800DD68C:
    jal     state_utility
.L800DD690:
    addiu   $a2,$sp,1816
.L800DD694:
    lw      $t9,20($s1)
.L800DD698:
    addiu   $t6,$t9,-1
.L800DD69C:
    sltiu   $at,$t6,15
.L800DD6A0:
    beqz    $at,.L800DDDF0
.L800DD6A4:
    sll     $t6,$t6,0x2
.L800DD6A8:
    lui     $at,%hi(jtbl_801242CC)
.L800DD6AC:
    addu    $at,$at,$t6
.L800DD6B0:
    lw      $t6,%lo(jtbl_801242CC)($at)
.L800DD6B4:
    jr      $t6
.L800DD6B8:
    nop
.L800DD6BC:
    move    $a0,$s3
.L800DD6C0:
    move    $a1,$s4
.L800DD6C4:
    jal     func_800DCD58
.L800DD6C8:
    sw      $s1,88($sp)
.L800DD6CC:
    lui     $a0,0x8014
.L800DD6D0:
    addiu   $a0,$a0,25040
.L800DD6D4:
    move    $a1,$zero
.L800DD6D8:
    li      $a2,1
.L800DD6DC:
    jal     func_80007270
.L800DD6E0:
    lw      $s1,88($sp)
.L800DD6E4:
    li      $s2,11
.L800DD6E8:
    sw      $s1,88($sp)
.L800DD6EC:
    jal     slot_state_setup
.L800DD6F0:
    sw      $s3,1896($sp)
.L800DD6F4:
    lui     $a0,0x8014
.L800DD6F8:
    addiu   $a0,$a0,25040
.L800DD6FC:
    lw      $s1,88($sp)
.L800DD700:
    move    $s0,$v0
.L800DD704:
    move    $a1,$zero
.L800DD708:
    jal     func_800075e0
.L800DD70C:
    move    $a2,$zero
.L800DD710:
    jal     dispatch_handler
.L800DD714:
    li      $a0,1
.L800DD718:
    lui     $s0,%hi(countdown_state)
.L800DD71C:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DD720:
    lui     $s0,%hi(countdown_object)
    lw      $t7,%lo(countdown_object)($s0)
.L800DD724:
    addiu   $a0,$sp,1520
.L800DD728:
    jal     func_800BE6A4
.L800DD72C:
    lw      $a1,404($t7)
.L800DD730:
    lui     $a1,0x8012
.L800DD734:
    addiu   $a1,$a1,4208
.L800DD738:
    jal     func_800BE4F0
.L800DD73C:
    addiu   $a0,$sp,1520
.L800DD740:
    lw      $t8,%lo(countdown_object)($s0)
.L800DD744:
    addiu   $a0,$sp,1520
.L800DD748:
    jal     func_800BE4F0
.L800DD74C:
    lw      $a1,408($t8)
.L800DD750:
    lh      $a2,28($s1)
.L800DD754:
    lh      $t6,24($s1)
.L800DD758:
    lh      $v1,30($s1)
.L800DD75C:
    lui     $s0,0x8012
.L800DD760:
    addiu   $s0,$s0,-29152
.L800DD764:
    addiu   $v1,$v1,-20
.L800DD768:
    sll     $a3,$v1,0x10
.L800DD76C:
    li      $t0,1
.L800DD770:
    sw      $t0,0($s0)
.L800DD774:
    sw      $t0,4($s0)
.L800DD778:
    bgez    $a2,.L800DD788
.L800DD77C:
    sra     $t9,$a2,0x1
.L800DD780:
    addiu   $at,$a2,1
.L800DD784:
    sra     $t9,$at,0x1
.L800DD788:
    addu    $a0,$t9,$t6
.L800DD78C:
    lh      $t6,26($s1)
.L800DD790:
    sll     $t7,$a0,0x10
.L800DD794:
    sra     $t8,$t7,0x10
.L800DD798:
    move    $a0,$t8
.L800DD79C:
    bgez    $v1,.L800DD7AC
.L800DD7A0:
    sra     $t9,$v1,0x1
.L800DD7A4:
    addiu   $at,$v1,1
.L800DD7A8:
    sra     $t9,$at,0x1
.L800DD7AC:
    addu    $a1,$t9,$t6
.L800DD7B0:
    addiu   $a1,$a1,20
.L800DD7B4:
    sll     $t7,$a1,0x10
.L800DD7B8:
    sra     $a1,$t7,0x10
.L800DD7BC:
    addiu   $t7,$sp,1520
.L800DD7C0:
    li      $t6,-1
.L800DD7C4:
    sra     $t9,$a3,0x10
.L800DD7C8:
    move    $a3,$t9
.L800DD7CC:
    sw      $t6,16($sp)
.L800DD7D0:
    sw      $t7,24($sp)
.L800DD7D4:
    jal     camera_auto_follow
.L800DD7D8:
    sw      $zero,20($sp)
.L800DD7DC:
    li      $t8,3
.L800DD7E0:
    lui     $a0,0x8014
.L800DD7E4:
    sw      $t8,4($s0)
.L800DD7E8:
    sw      $zero,0($s0)
.L800DD7EC:
    addiu   $a0,$a0,25040
.L800DD7F0:
    move    $a1,$zero
.L800DD7F4:
    jal     func_80007270
.L800DD7F8:
    li      $a2,1
.L800DD7FC:
    li      $s2,11
.L800DD800:
    jal     slot_state_setup
.L800DD804:
    sw      $s1,88($sp)
.L800DD808:
    lui     $a0,0x8014
.L800DD80C:
    addiu   $a0,$a0,25040
.L800DD810:
    lw      $s1,88($sp)
.L800DD814:
    move    $s0,$v0
.L800DD818:
    move    $a1,$zero
.L800DD81C:
    jal     func_800075e0
.L800DD820:
    move    $a2,$zero
.L800DD824:
    b       .L800DDDF4
.L800DD828:
    lb      $t9,0($s1)
.L800DD82C:
    lui     $t9,%hi(state_word_a)
.L800DD830:
    lw      $t9,%lo(state_word_a)($t9)
.L800DD834:
    lui     $at,0x7c03
.L800DD838:
    ori     $at,$at,0xfffe
.L800DD83C:
    and     $t6,$t9,$at
.L800DD840:
    bnez    $t6,.L800DD860
.L800DD844:
    move    $a1,$s3
.L800DD848:
    move    $a0,$s3
.L800DD84C:
    move    $a1,$s4
.L800DD850:
    jal     func_800DCD58
.L800DD854:
    sw      $s1,88($sp)
.L800DD858:
    b       .L800DD868
.L800DD85C:
    lw      $s1,88($sp)
.L800DD860:
    jal     func_800DCCE0
.L800DD864:
    move    $a2,$s4
.L800DD868:
    sw      $s3,0($sp)
.L800DD86C:
    sw      $s4,4($sp)
.L800DD870:
    sw      $s1,88($sp)
.L800DD874:
    jal     func_800DD0C0
.L800DD878:
    sw      $s4,1892($sp)
.L800DD87C:
    lw      $s1,88($sp)
.L800DD880:
    b       .L800DDDF0
.L800DD884:
    lw      $s4,1892($sp)
.L800DD888:
    move    $a0,$s3
.L800DD88C:
    move    $a1,$s4
.L800DD890:
    jal     func_800DCD58
.L800DD894:
    sw      $s1,88($sp)
.L800DD898:
    lw      $s1,88($sp)
.L800DD89C:
    lui     $s0,%hi(countdown_state)
.L800DD8A0:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DD8A4:
    lb      $t7,16($s1)
.L800DD8A8:
    beqzl   $t7,.L800DD8C8
.L800DD8AC:
    lui     $s0,%hi(countdown_object)
    lw      $t9,%lo(countdown_object)($s0)
.L800DD8B0:
    lui     $s0,%hi(countdown_state)
.L800DD8B4:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DD8B8:
    lui     $s0,%hi(countdown_object)
    lw      $t8,%lo(countdown_object)($s0)
.L800DD8BC:
    b       .L800DD8CC
.L800DD8C0:
    lw      $a1,424($t8)
.L800DD8C4:
    lw      $t9,%lo(countdown_object)($s0)
.L800DD8C8:
    lw      $a1,428($t9)
.L800DD8CC:
    jal     func_800BE6A4
.L800DD8D0:
    addiu   $a0,$sp,1192
.L800DD8D4:
    lui     $a1,0x8012
.L800DD8D8:
    addiu   $a1,$a1,4212
.L800DD8DC:
    jal     func_800BE4F0
.L800DD8E0:
    addiu   $a0,$sp,1192
.L800DD8E4:
    lw      $t6,%lo(countdown_object)($s0)
.L800DD8E8:
    addiu   $a0,$sp,1192
.L800DD8EC:
    jal     func_800BE4F0
.L800DD8F0:
    lw      $a1,432($t6)
.L800DD8F4:
    lw      $t8,12($s0)
.L800DD8F8:
    lw      $t7,16($s0)
.L800DD8FC:
    move    $s6,$s3
.L800DD900:
    lhu     $t9,50($t8)
.L800DD904:
    move    $s7,$s4
.L800DD908:
    addiu   $s8,$sp,1192
.L800DD90C:
    sll     $t6,$t9,0x2
.L800DD910:
    addu    $t8,$t7,$t6
.L800DD914:
    lw      $t9,0($t8)
.L800DD918:
    lw      $t7,%lo(countdown_object)($s0)
.L800DD91C:
    lb      $s5,1($s1)
.L800DD920:
    sw      $t9,12($sp)
.L800DD924:
    lw      $t6,944($t7)
.L800DD928:
    sw      $s4,1892($sp)
.L800DD92C:
    sw      $s1,88($sp)
.L800DD930:
    jal     attract_video_handler
.L800DD934:
    sw      $t6,16($sp)
.L800DD938:
    lw      $s1,88($sp)
.L800DD93C:
    b       .L800DDDF0
.L800DD940:
    lw      $s4,1892($sp)
.L800DD944:
    move    $a1,$s3
.L800DD948:
    jal     func_800DCCE0
.L800DD94C:
    move    $a2,$s4
.L800DD950:
    lui     $s0,%hi(countdown_state)
.L800DD954:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DD958:
    lui     $s0,%hi(countdown_object)
    lw      $t8,%lo(countdown_object)($s0)
.L800DD95C:
    addiu   $a0,$sp,920
.L800DD960:
    jal     func_800BE6A4
.L800DD964:
    lw      $a1,436($t8)
.L800DD968:
    lui     $a1,0x8012
.L800DD96C:
    addiu   $a1,$a1,4216
.L800DD970:
    jal     func_800BE4F0
.L800DD974:
    addiu   $a0,$sp,920
.L800DD978:
    lw      $t9,%lo(countdown_object)($s0)
.L800DD97C:
    addiu   $a0,$sp,920
.L800DD980:
    jal     func_800BE4F0
.L800DD984:
    lw      $a1,440($t9)
.L800DD988:
    move    $s5,$s3
.L800DD98C:
    move    $s6,$s4
.L800DD990:
    addiu   $s7,$sp,920
.L800DD994:
    sw      $s1,88($sp)
.L800DD998:
    jal     attract_mode_handler
.L800DD99C:
    sw      $s4,1892($sp)
.L800DD9A0:
    lw      $s1,88($sp)
.L800DD9A4:
    b       .L800DDDF0
.L800DD9A8:
    lw      $s4,1892($sp)
.L800DD9AC:
    move    $a1,$s3
.L800DD9B0:
    jal     func_800DCCE0
.L800DD9B4:
    move    $a2,$s4
.L800DD9B8:
    lui     $s0,%hi(countdown_state)
.L800DD9BC:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DD9C0:
    lui     $s0,%hi(countdown_object)
    lw      $t7,%lo(countdown_object)($s0)
.L800DD9C4:
    addiu   $a0,$sp,656
.L800DD9C8:
    jal     func_800BE6A4
.L800DD9CC:
    lw      $a1,444($t7)
.L800DD9D0:
    lui     $a1,0x8012
.L800DD9D4:
    addiu   $a1,$a1,4232
.L800DD9D8:
    jal     func_800BE4F0
.L800DD9DC:
    addiu   $a0,$sp,656
.L800DD9E0:
    lw      $t6,%lo(countdown_object)($s0)
.L800DD9E4:
    addiu   $a0,$sp,656
.L800DD9E8:
    jal     func_800BE4F0
.L800DD9EC:
    lw      $a1,408($t6)
.L800DD9F0:
    move    $s5,$s3
.L800DD9F4:
    move    $s6,$s4
.L800DD9F8:
    addiu   $s7,$sp,656
.L800DD9FC:
    sw      $s1,88($sp)
.L800DDA00:
    jal     attract_mode_handler
.L800DDA04:
    sw      $s4,1892($sp)
.L800DDA08:
    lw      $s1,88($sp)
.L800DDA0C:
    b       .L800DDDF0
.L800DDA10:
    lw      $s4,1892($sp)
.L800DDA14:
    move    $a0,$s3
.L800DDA18:
    move    $a1,$s4
.L800DDA1C:
    jal     func_800DCD58
.L800DDA20:
    sw      $s1,88($sp)
.L800DDA24:
    lui     $s0,%hi(countdown_state)
.L800DDA28:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDA2C:
    lw      $t9,12($s0)
.L800DDA30:
    lw      $t8,16($s0)
.L800DDA34:
    lui     $s0,%hi(countdown_object)
    lw      $v0,%lo(countdown_object)($s0)
.L800DDA38:
    lhu     $t7,52($t9)
.L800DDA3C:
    lw      $s1,88($sp)
.L800DDA40:
    lw      $s8,444($v0)
.L800DDA44:
    sll     $t6,$t7,0x2
.L800DDA48:
    addu    $t9,$t8,$t6
.L800DDA4C:
    lw      $t7,4($t9)
.L800DDA50:
    move    $s6,$s3
.L800DDA54:
    move    $s7,$s4
.L800DDA58:
    sw      $t7,12($sp)
.L800DDA5C:
    lw      $t8,944($v0)
.L800DDA60:
    sw      $s4,1892($sp)
.L800DDA64:
    lb      $s5,1($s1)
.L800DDA68:
    jal     attract_video_handler
.L800DDA6C:
    sw      $t8,16($sp)
.L800DDA70:
    lw      $s1,88($sp)
.L800DDA74:
    b       .L800DDDF0
.L800DDA78:
    lw      $s4,1892($sp)
.L800DDA7C:
    move    $a0,$s3
.L800DDA80:
    move    $a1,$s4
.L800DDA84:
    jal     func_800DCD58
.L800DDA88:
    sw      $s1,88($sp)
.L800DDA8C:
    lui     $s0,%hi(countdown_state)
.L800DDA90:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDA94:
    lw      $t7,12($s0)
.L800DDA98:
    lui     $s0,%hi(countdown_object)
    lw      $t6,%lo(countdown_object)($s0)
.L800DDA9C:
    lw      $t9,16($s0)
.L800DDAA0:
    lhu     $t8,48($t7)
.L800DDAA4:
    lw      $s8,448($t6)
.L800DDAA8:
    lw      $s1,88($sp)
.L800DDAAC:
    sll     $t6,$t8,0x2
.L800DDAB0:
    addu    $v0,$t9,$t6
.L800DDAB4:
    lw      $t7,0($v0)
.L800DDAB8:
    move    $s6,$s3
.L800DDABC:
    move    $s7,$s4
.L800DDAC0:
    sw      $t7,12($sp)
.L800DDAC4:
    lw      $t8,4($v0)
.L800DDAC8:
    sw      $s4,1892($sp)
.L800DDACC:
    lb      $s5,1($s1)
.L800DDAD0:
    jal     attract_video_handler
.L800DDAD4:
    sw      $t8,16($sp)
.L800DDAD8:
    lw      $s1,88($sp)
.L800DDADC:
    b       .L800DDDF0
.L800DDAE0:
    lw      $s4,1892($sp)
.L800DDAE4:
    move    $a0,$s3
.L800DDAE8:
    move    $a1,$s4
.L800DDAEC:
    jal     func_800DCD58
.L800DDAF0:
    sw      $s1,88($sp)
.L800DDAF4:
    lui     $s0,%hi(countdown_state)
.L800DDAF8:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDAFC:
    lui     $s0,%hi(countdown_object)
    lw      $t9,%lo(countdown_object)($s0)
.L800DDB00:
    addiu   $a0,$sp,392
.L800DDB04:
    lw      $s1,88($sp)
.L800DDB08:
    jal     func_800BE6A4
.L800DDB0C:
    lw      $a1,452($t9)
.L800DDB10:
    lui     $a1,0x8012
.L800DDB14:
    addiu   $a1,$a1,4220
.L800DDB18:
    jal     func_800BE4F0
.L800DDB1C:
    addiu   $a0,$sp,392
.L800DDB20:
    lw      $t6,%lo(countdown_object)($s0)
.L800DDB24:
    addiu   $a0,$sp,392
.L800DDB28:
    jal     func_800BE4F0
.L800DDB2C:
    lw      $a1,456($t6)
.L800DDB30:
    move    $s5,$s3
.L800DDB34:
    move    $s6,$s4
.L800DDB38:
    addiu   $s7,$sp,392
.L800DDB3C:
    sw      $s1,88($sp)
.L800DDB40:
    jal     attract_mode_handler
.L800DDB44:
    sw      $s4,1892($sp)
.L800DDB48:
    lw      $s1,88($sp)
.L800DDB4C:
    b       .L800DDDF0
.L800DDB50:
    lw      $s4,1892($sp)
.L800DDB54:
    move    $a1,$s3
.L800DDB58:
    jal     func_800DCCE0
.L800DDB5C:
    move    $a2,$s4
.L800DDB60:
    lui     $s0,%hi(countdown_state)
.L800DDB64:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDB68:
    lui     $s0,%hi(countdown_object)
    lw      $t7,%lo(countdown_object)($s0)
.L800DDB6C:
    addiu   $a0,$sp,128
.L800DDB70:
    jal     func_800BE6A4
.L800DDB74:
    lw      $a1,460($t7)
.L800DDB78:
    lui     $a1,0x8012
.L800DDB7C:
    addiu   $a1,$a1,4224
.L800DDB80:
    jal     func_800BE4F0
.L800DDB84:
    addiu   $a0,$sp,128
.L800DDB88:
    lw      $t8,%lo(countdown_object)($s0)
.L800DDB8C:
    addiu   $a0,$sp,128
.L800DDB90:
    jal     func_800BE4F0
.L800DDB94:
    lw      $a1,440($t8)
.L800DDB98:
    move    $s5,$s3
.L800DDB9C:
    move    $s6,$s4
.L800DDBA0:
    addiu   $s7,$sp,128
.L800DDBA4:
    sw      $s1,88($sp)
.L800DDBA8:
    jal     attract_mode_handler
.L800DDBAC:
    sw      $s4,1892($sp)
.L800DDBB0:
    lw      $s1,88($sp)
.L800DDBB4:
    b       .L800DDDF0
.L800DDBB8:
    lw      $s4,1892($sp)
.L800DDBBC:
    lw      $a0,32($s1)
.L800DDBC0:
    andi    $t9,$a0,0x800
.L800DDBC4:
    beqzl   $t9,.L800DDC1C
.L800DDBC8:
    andi    $t8,$a0,0x400
.L800DDBCC:
    jal     audio_distance_atten
.L800DDBD0:
    nop
.L800DDBD4:
    lb      $t6,1($s1)
.L800DDBD8:
    li      $t0,1
.L800DDBDC:
    beqzl   $t6,.L800DDBF8
.L800DDBE0:
    lb      $t7,2($s1)
.L800DDBE4:
    sb      $zero,1($s1)
.L800DDBE8:
    sb      $t0,2($s1)
.L800DDBEC:
    b       .L800DDC18
.L800DDBF0:
    lw      $a0,32($s1)
.L800DDBF4:
    lb      $t7,2($s1)
.L800DDBF8:
    li      $t0,1
.L800DDBFC:
    beqzl   $t7,.L800DDC14
.L800DDC00:
    sb      $t0,1($s1)
.L800DDC04:
    sb      $zero,2($s1)
.L800DDC08:
    b       .L800DDC18
.L800DDC0C:
    lw      $a0,32($s1)
.L800DDC10:
    sb      $t0,1($s1)
.L800DDC14:
    lw      $a0,32($s1)
.L800DDC18:
    andi    $t8,$a0,0x400
.L800DDC1C:
    beqz    $t8,.L800DDC6C
.L800DDC20:
    li      $t0,1
.L800DDC24:
    jal     audio_distance_atten
.L800DDC28:
    nop
.L800DDC2C:
    lb      $t9,1($s1)
.L800DDC30:
    li      $t0,1
.L800DDC34:
    beqzl   $t9,.L800DDC4C
.L800DDC38:
    lb      $t6,2($s1)
.L800DDC3C:
    sb      $zero,1($s1)
.L800DDC40:
    b       .L800DDC6C
.L800DDC44:
    lw      $a0,32($s1)
.L800DDC48:
    lb      $t6,2($s1)
.L800DDC4C:
    beqzl   $t6,.L800DDC68
.L800DDC50:
    sb      $t0,2($s1)
.L800DDC54:
    sb      $zero,2($s1)
.L800DDC58:
    sb      $t0,1($s1)
.L800DDC5C:
    b       .L800DDC6C
.L800DDC60:
    lw      $a0,32($s1)
.L800DDC64:
    sb      $t0,2($s1)
.L800DDC68:
    lw      $a0,32($s1)
.L800DDC6C:
    andi    $t7,$a0,0x2
.L800DDC70:
    beqz    $t7,.L800DDC90
.L800DDC74:
    li      $a0,37
.L800DDC78:
    move    $a1,$zero
.L800DDC7C:
    move    $a2,$t0
.L800DDC80:
    jal     frame_sync
.L800DDC84:
    move    $a3,$zero
.L800DDC88:
    li      $t0,1
.L800DDC8C:
    sb      $t0,0($s1)
.L800DDC90:
    lui     $s0,%hi(countdown_state)
.L800DDC94:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDC98:
    lui     $s0,%hi(countdown_object)
    lw      $v0,%lo(countdown_object)($s0)
.L800DDC9C:
    lw      $t7,12($s0)
.L800DDCA0:
    lw      $t6,16($s0)
.L800DDCA4:
    lw      $t8,472($v0)
.L800DDCA8:
    move    $s6,$s3
.L800DDCAC:
    move    $s7,$s4
.L800DDCB0:
    sw      $t8,8($sp)
.L800DDCB4:
    lw      $t9,944($v0)
.L800DDCB8:
    lb      $s5,1($s1)
.L800DDCBC:
    lb      $s8,2($s1)
.L800DDCC0:
    sw      $t9,12($sp)
.L800DDCC4:
    lhu     $t8,52($t7)
.L800DDCC8:
    sll     $t9,$t8,0x2
.L800DDCCC:
    addu    $v1,$t6,$t9
.L800DDCD0:
    lw      $t7,0($v1)
.L800DDCD4:
    sw      $t7,16($sp)
.L800DDCD8:
    lw      $t8,4($v1)
.L800DDCDC:
    sw      $s4,1892($sp)
.L800DDCE0:
    sw      $s1,88($sp)
.L800DDCE4:
    jal     attract_demo_handler
.L800DDCE8:
    sw      $t8,20($sp)
.L800DDCEC:
    lw      $s1,88($sp)
.L800DDCF0:
    b       .L800DDDF0
.L800DDCF4:
    lw      $s4,1892($sp)
.L800DDCF8:
    lui     $t9,%hi(game_loop_tick)
.L800DDCFC:
    lui     $t9,0x8003
    addiu   $t9,$t9,-5912
.L800DDD00:
    lui     $t9,%hi(game_loop_tick)
    lw      $t7,%lo(game_loop_tick)($t9)
.L800DDD04:
    lw      $t6,40($s1)
.L800DDD08:
    lui     $s0,%hi(countdown_state)
.L800DDD0C:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDD10:
    sltu    $at,$t6,$t7
.L800DDD14:
    beqz    $at,.L800DDD20
.L800DDD18:
    li      $t0,1
.L800DDD1C:
    sb      $t0,0($s1)
.L800DDD20:
    lui     $s0,%hi(countdown_object)
    lw      $t8,%lo(countdown_object)($s0)
.L800DDD24:
    move    $s5,$s3
.L800DDD28:
    move    $s6,$s4
.L800DDD2C:
    lw      $s7,476($t8)
.L800DDD30:
    sw      $s4,1892($sp)
.L800DDD34:
    jal     attract_mode_handler
.L800DDD38:
    sw      $s1,88($sp)
.L800DDD3C:
    lw      $s1,88($sp)
.L800DDD40:
    b       .L800DDDF0
.L800DDD44:
    lw      $s4,1892($sp)
.L800DDD48:
    lui     $t6,%hi(game_loop_tick)
.L800DDD4C:
    lui     $t6,0x8003
    addiu   $t6,$t6,-5912
.L800DDD50:
    lui     $t6,%hi(game_loop_tick)
    lw      $t7,%lo(game_loop_tick)($t6)
.L800DDD54:
    lw      $t9,40($s1)
.L800DDD58:
    lui     $s0,%hi(countdown_state)
.L800DDD5C:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDD60:
    sltu    $at,$t9,$t7
.L800DDD64:
    beqz    $at,.L800DDD70
.L800DDD68:
    li      $t0,1
.L800DDD6C:
    sb      $t0,0($s1)
.L800DDD70:
    lui     $s0,%hi(countdown_object)
    lw      $t8,%lo(countdown_object)($s0)
.L800DDD74:
    move    $s5,$s3
.L800DDD78:
    move    $s6,$s4
.L800DDD7C:
    lw      $s7,480($t8)
.L800DDD80:
    sw      $s4,1892($sp)
.L800DDD84:
    jal     attract_mode_handler
.L800DDD88:
    sw      $s1,88($sp)
.L800DDD8C:
    lw      $s1,88($sp)
.L800DDD90:
    b       .L800DDDF0
.L800DDD94:
    lw      $s4,1892($sp)
.L800DDD98:
    lui     $t9,%hi(game_loop_tick)
.L800DDD9C:
    lui     $t9,0x8003
    addiu   $t9,$t9,-5912
.L800DDDA0:
    lui     $t9,%hi(game_loop_tick)
    lw      $t7,%lo(game_loop_tick)($t9)
.L800DDDA4:
    lw      $t6,40($s1)
.L800DDDA8:
    lui     $s0,%hi(countdown_state)
.L800DDDAC:
    addiu   $s0,$s0,%lo(countdown_state)
.L800DDDB0:
    sltu    $at,$t6,$t7
.L800DDDB4:
    beqz    $at,.L800DDDC0
.L800DDDB8:
    li      $t0,1
.L800DDDBC:
    sb      $t0,0($s1)
.L800DDDC0:
    lui     $s0,%hi(countdown_object)
    lw      $t8,%lo(countdown_object)($s0)
.L800DDDC4:
    move    $s5,$s3
.L800DDDC8:
    move    $s6,$s4
.L800DDDCC:
    lw      $s7,484($t8)
.L800DDDD0:
    sw      $s4,1892($sp)
.L800DDDD4:
    jal     attract_mode_handler
.L800DDDD8:
    sw      $s1,88($sp)
.L800DDDDC:
    lw      $s1,88($sp)
.L800DDDE0:
    b       .L800DDDF0
.L800DDDE4:
    lw      $s4,1892($sp)
.L800DDDE8:
    li      $t0,1
.L800DDDEC:
    sb      $t0,0($s1)
.L800DDDF0:
    lb      $t9,0($s1)
.L800DDDF4:
    lw      $t6,1908($sp)
.L800DDDF8:
    sb      $t9,0($t6)
.L800DDDFC:
    lb      $t7,0($s1)
.L800DDE00:
    beqzl   $t7,.L800DDE78
.L800DDE04:
    lw      $ra,76($sp)
.L800DDE08:
    lb      $t8,1($s1)
.L800DDE0C:
    lw      $t9,1900($sp)
.L800DDE10:
    sw      $zero,20($s1)
.L800DDE14:
    sb      $t8,0($t9)
.L800DDE18:
    lw      $t7,1904($sp)
.L800DDE1C:
    lb      $t6,2($s1)
.L800DDE20:
    sb      $t6,0($t7)
.L800DDE24:
    sll     $t9,$s4,0x2
.L800DDE28:
    subu    $t9,$t9,$s4
.L800DDE2C:
    lw      $t8,92($sp)
.L800DDE30:
    sll     $t9,$t9,0x2
.L800DDE34:
    subu    $t9,$t9,$s4
.L800DDE38:
    sll     $t9,$t9,0x2
.L800DDE3C:
    move    $s0,$zero
.L800DDE40:
    addu    $s1,$t8,$t9
.L800DDE44:
    move    $a0,$s0
.L800DDE48:
    jal     player_state_set
.L800DDE4C:
    lb      $a1,4($s1)
.L800DDE50:
    move    $a0,$s0
.L800DDE54:
    jal     player_mode_set
.L800DDE58:
    lb      $a1,8($s1)
.L800DDE5C:
    addiu   $s0,$s0,1
.L800DDE60:
    li      $at,4
.L800DDE64:
    bne     $s0,$at,.L800DDE44
.L800DDE68:
    addiu   $s1,$s1,1
.L800DDE6C:
    jal     process_inputs
.L800DDE70:
    nop
.L800DDE74:
    lw      $ra,76($sp)
.L800DDE78:
    lw      $s0,40($sp)
.L800DDE7C:
    lw      $s1,44($sp)
.L800DDE80:
    lw      $s2,48($sp)
.L800DDE84:
    lw      $s3,52($sp)
.L800DDE88:
    lw      $s4,56($sp)
.L800DDE8C:
    lw      $s5,60($sp)
.L800DDE90:
    lw      $s6,64($sp)
.L800DDE94:
    lw      $s7,68($sp)
.L800DDE98:
    lw      $s8,72($sp)
.L800DDE9C:
    jr      $ra
.L800DDEA0:
    addiu   $sp,$sp,1896

.section .rodata
glabel jtbl_801242CC
    .word .L800DD6BC
    .word .L800DD82C
    .word .L800DD888
    .word .L800DD944
    .word .L800DDA14
    .word .L800DDAE4
    .word .L800DDA14
    .word .L800DDA7C
    .word .L800DDB54
    .word .L800DDBBC
    .word .L800DDCF8
    .word .L800DDD48
    .word .L800DDD98
    .word .L800DDDE8
    .word .L800DD9AC
