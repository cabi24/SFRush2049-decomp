	.verstamp	3 19
	.option	pic0
	.data	
	.align	2
	.align	0
$$178:
	.word	-1 : 1
	.data	
	.align	2
	.align	0
zoom:
	.float	1.0:1
	.data	
	.align	2
	.align	0
$$195:
	.word	-1 : 1
	.data	
	.align	2
	.align	0
D_803B2858:
	.word	8388608 : 1
	.word	8388608 : 1
	.word	-1061109760 : 1
	.word	-1061109760 : 1
	.word	6553600 : 1
	.byte	0 : 4
	.byte	0 : 1
	.byte	0 : 2
	.byte	0 : 13
	.word	-16777216 : 1
	.word	-16777216 : 1
	.word	-1061109760 : 1
	.word	-1061109760 : 1
	.word	6553600 : 1
	.byte	0 : 4
	.byte	0 : 1
	.byte	0 : 2
	.byte	0 : 13
	.data	
	.align	2
	.align	0
$$208:
	.float	-5.6999999284744263e-01:1
	.float	0.57:1
	.float	-5.6999999284744263e-01:1
	.extern	D_80152698 16
	.extern	D_8014A110 4
	.extern	D_801426EC 1
	.extern	D_80152020 16
	.extern	D_8014978C 1
	.extern	D_80152570 1
	.extern	D_803BA010 4
	.extern	D_80156994 1
	.extern	D_803BA02C 1
	.extern	D_8014A108 2
	.extern	D_803B9AA0 192
	.extern	D_80146180 4
	.extern	D_80154188 4
	.extern	D_80151AD0 2
	.extern	D_803BA00C 1
	.extern	D_803B25A8 4
	.extern	D_803B25AC 4
	.extern	D_803B25B0 4
	.extern	D_803B25B4 4
	.extern	D_803B25B8 4
	.extern	D_803B25BC 4
	.extern	D_803B25C0 4
	.extern	D_803B25C4 4
	.extern	D_8011418C 48
	.extern	D_80140BDC 1
	.extern	D_801407D0 1
	.extern	D_803B28AC 4
	.extern	D_803B28A8 1
	.extern	D_8012E6F8 4
	.extern	D_801174BC 4
	.extern	D_801174B4 4
	.extern	D_803BA030 4
	.extern	D_80157244 1
	.extern	D_803B3030 12
	.extern	countdown_state 20
	.extern	D_801164BE 2
	.extern	D_803AF980 4
	.extern	D_803B25C8 4
	.lcomm	cursorSpeed 16
	.lcomm	scrollSpeed 16
	.extern	D_8002EB94 4
	.extern	D_803B3054 4
	.extern	D_803B3058 4
	.extern	D_803B2600 22
	.extern	D_803B25EC 20
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 970
 # 970	{
	.ent	func_8039D05C 2
func_8039D05C:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	2 970
	.loc	2 970
	.loc	2 975
 # 971	    s32 player,i,count;
 # 972	    FiveParts **box;
 # 973	    Slot *slot;
 # 974	    PanelPart *part;
 # 975	    wheel_setup_initial(62);
	li	$4, 62
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
	.loc	2 976
 # 976	    if(D_80156994) {
	lb	$14, D_80156994
	beq	$14, 0, $32
	.loc	2 976
	.loc	2 977
 # 977	        wheel_setup_initial(80);
	li	$4, 80
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
	.loc	2 978
 # 978	        wheel_setup_initial(61);
	li	$4, 61
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
	b	$34
$32:
	.loc	2 979
 # 979	    } else if(D_8014978C>=6)wheel_setup_initial(80);
	lb	$15, D_8014978C
	blt	$15, 6, $33
	.loc	2 979
	li	$4, 80
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
	b	$34
$33:
	.loc	2 980
 # 980	    else wheel_setup_initial(81);
	li	$4, 81
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
$34:
	.loc	2 981
 # 981	    for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {
	la	$16, D_803B9FC0
	la	$17, D_803B9FC0+16
$35:
	.loc	2 981
	.loc	2 982
 # 982	        if(*box) {sound_handles_array_clear(*box);*box=0;}
	lw	$4, 0($16)
	beq	$4, 0, $36
	.loc	2 982
	.loc	2 982
	.livereg	0x0800000E,0x00000000
	jal	sound_handles_array_clear
	.loc	2 982
	sw	$0, 0($16)
$36:
	.loc	2 981
 # 981	    for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {
	addu	$16, $16, 4
	bltu	$16, $17, $35
	.loc	2 984
 # 982	        if(*box) {sound_handles_array_clear(*box);*box=0;}
 # 983	    }
 # 984	    for(player=0;player<(count=D_8014A108);player++) {
	move	$20, $0
	lh	$2, D_8014A108
	ble	$2, 0, $47
	la	$30, D_803B25CC
	li	$22, 572
	li	$21, 2816
	li	$18, -1
$37:
	.loc	2 984
	.loc	2 985
 # 985	        for(i=0;i!=44;i++) {
	move	$2, $0
	sll	$23, $20, 24
	sra	$24, $23, 24
	move	$23, $24
	mul	$25, $23, 2816
	la	$14, D_803AF9A8
	addu	$19, $25, $14
	move	$16, $0
$38:
	.loc	2 985
	.loc	2 986
 # 986	            slot=&D_803AF9A8[(s8)player][i];
	.loc	2 987
 # 987	            if(slot->handle!=-1) {sound_call_minimal(slot->handle);slot->handle=-1;}
	.noalias	$19,$sp
	.noalias	$17,$sp
	addu	$17, $19, $16
	lw	$2, 4($17)
	beq	$18, $2, $39
	.loc	2 987
	.loc	2 987
	sll	$4, $2, 16
	sra	$15, $4, 16
	move	$4, $15
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 987
	sw	$18, 4($17)
	.alias	$17,$sp
$39:
	.loc	2 985
 # 985	        for(i=0;i!=44;i++) {
	addu	$16, $16, 64
	bne	$16, $21, $38
	.alias	$19,$sp
	.loc	2 989
 # 986	            slot=&D_803AF9A8[(s8)player][i];
 # 987	            if(slot->handle!=-1) {sound_call_minimal(slot->handle);slot->handle=-1;}
 # 988	        }
 # 989	        D_803B9B90[(s8)player]=0;
	mul	$24, $23, 4
	sw	$0, D_803B9B90($24)
	.loc	2 990
 # 990	        if(D_803B25CC[player][0]!=-1) {sound_call_minimal(D_803B25CC[player][0]);D_803B25CC[player][0]=-1;}
	.noalias	$30,$sp
	lw	$2, 0($30)
	beq	$18, $2, $40
	.loc	2 990
	.loc	2 990
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 990
	sw	$18, 0($30)
$40:
	.loc	2 991
 # 991	        if(D_803B25CC[player][1]!=-1) {sound_call_minimal(D_803B25CC[player][1]);D_803B25CC[player][1]=-1;}
	lw	$2, 4($30)
	beq	$18, $2, $41
	.loc	2 991
	.loc	2 991
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 991
	sw	$18, 4($30)
$41:
	.loc	2 992
 # 992	        if(player>0)continue;
	ble	$20, 0, $42
	.loc	2 992
	lh	$2, D_8014A108
	b	$46
$42:
	.loc	2 993
 # 993	        {
	.loc	2 994
 # 994	            for(i=0;i!=11;i++) {
	move	$2, $0
	move	$17, $0
	mul	$15, $20, 572
	la	$24, D_803B2618
	addu	$16, $15, $24
	.loc	2 994
	.loc	2 995
 # 995	                if (player) {} /* Code-free allocation-priority read; see header. */
	.loc	2 995
$43:
	.loc	2 996
 # 996	                part=&D_803B2618[player][i];
	.loc	2 997
 # 997	                if(part->handle!=-1) {sound_call_minimal(part->handle);part->handle=-1;part->auxiliary=-1;}
	.noalias	$16,$30
	.noalias	$16,$sp
	lh	$4, 0($16)
	beq	$18, $4, $44
	.loc	2 997
	.loc	2 997
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 997
	sh	$18, 0($16)
	.loc	2 997
	sh	$18, 2($16)
$44:
	.loc	2 994
 # 994	            for(i=0;i!=11;i++) {
	addu	$17, $17, 52
	addu	$16, $16, 52
	bne	$17, $22, $43
	.alias	$16,$30
	.alias	$16,$sp
	lw	$2, D_803B25C8
	.loc	2 999
 # 995	                if (player) {} /* Code-free allocation-priority read; see header. */
 # 996	                part=&D_803B2618[player][i];
 # 997	                if(part->handle!=-1) {sound_call_minimal(part->handle);part->handle=-1;part->auxiliary=-1;}
 # 998	            }
 # 999	            if(D_803B25C8!=-1) {sound_call_minimal(D_803B25C8);D_803B25C8=-1;}
	beq	$18, $2, $45
	.loc	2 999
	.loc	2 999
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 999
	sw	$18, D_803B25C8
$45:
	.loc	2 984
 # 984	    for(player=0;player<(count=D_8014A108);player++) {
	lh	$2, D_8014A108
$46:
	addu	$20, $20, 1
	addu	$30, $30, 8
	blt	$20, $2, $37
	.alias	$30,$sp
	move	$20, $0
$47:
	la	$16, D_803B28AC
	.loc	2 1002
 #1002	    if(D_803B28AC) {sound_stop(D_803B28AC);D_803B28AC=0;}
	.noalias	$16,$sp
	lw	$4, 0($16)
	beq	$4, 0, $48
	.loc	2 1002
	.loc	2 1002
	.livereg	0x0800000E,0x00000000
	jal	sound_stop
	.loc	2 1002
	sw	$0, 0($16)
	.alias	$16,$sp
$48:
	.loc	2 1003
 #1003	    ambient_sounds_clear();
	.livereg	0x0000000E,0x00000000
	jal	ambient_sounds_clear
	.loc	2 1004
 #1004	    D_80154188=90.0f;D_80151AD0=1;
	li.s	$f4, 90.0
	s.s	$f4, D_80154188
	.loc	2 1004
	li	$14, 1
	sh	$14, D_80151AD0
	.loc	2 1005
 #1005	    func_800A5A40();func_800A5B3C();
	.livereg	0x0000000E,0x00000000
	jal	func_800A5A40
	.loc	2 1005
	.livereg	0x0000000E,0x00000000
	jal	func_800A5B3C
	.loc	2 1006
 #1006	    for(player=0;player<D_8014A108;player++)scheduler_recv(D_803BA1D0[player]);
	lh	$15, D_8014A108
	ble	$15, 0, $50
	la	$16, D_803BA1D0
$49:
	.loc	2 1006
	.noalias	$16,$sp
	lw	$4, 0($16)
	.livereg	0x0800000E,0x00000000
	jal	scheduler_recv
	.loc	2 1006
	addu	$20, $20, 1
	addu	$16, $16, 4
	lh	$24, D_8014A108
	blt	$20, $24, $49
	.alias	$16,$sp
$50:
	.loc	2 1007
 #1007	    D_803B28A8=0;
	sb	$0, D_803B28A8
	.loc	2 1008
 #1008	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	func_8039D05C
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 163
 # 163	{
	.ent	func_8039D300 2
func_8039D300:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 163
	.loc	2 163
	.loc	2 166
 # 164	    s16 component;
 # 165	
 # 166	    for (component = 0; component < 4; component++) {
	move	$2, $0
	mul	$14, $4, 16
	la	$15, D_803BA190
	addu	$3, $14, $15
	la	$24, D_803B9FD0
	addu	$5, $4, $24
	.noalias	$5,$sp
	mul	$25, $4, 13
	lb	$14, 0($5)
	addu	$15, $25, $14
	lb	$24, D_80110E85($15)
	mul	$25, $24, 16
	la	$14, D_803B28C8
	addu	$6, $25, $14
	mul	$15, $4, 13
	lb	$24, 0($5)
	addu	$25, $15, $24
	lb	$14, D_80111049($25)
	mul	$15, $14, 16
	la	$24, D_803B2948
	addu	$7, $15, $24
	mul	$25, $4, 13
	lb	$14, 0($5)
	addu	$15, $25, $14
	lb	$24, D_8011108D($15)
	mul	$25, $24, 16
	la	$14, D_803B2978
	addu	$8, $25, $14
	mul	$15, $4, 13
	lb	$24, 0($5)
	addu	$25, $15, $24
	lb	$14, D_801111A9($25)
	mul	$15, $14, 16
	la	$24, D_803B2A08
	addu	$9, $15, $24
	mul	$25, $4, 13
	lb	$14, 0($5)
	addu	$15, $25, $14
	lb	$24, D_8011123D($15)
	mul	$25, $24, 16
	la	$14, D_803B2A58
	addu	$10, $25, $14
	.alias	$5,$sp
	.loc	2 166
	.loc	2 167
 # 167	        do {
$51:
	.loc	2 167
	.loc	2 168
 # 168	            D_803BA190[player][component] = D_803B2948[D_80111049[player][D_803B9FD0[player]]][component] * D_803B28C8[D_80110E85[player][D_803B9FD0[player]]][component];
	.loc	2 169
 # 169	            D_803BA190[player][component] *= D_803B2978[D_8011108D[player][D_803B9FD0[player]]][component];
	mul	$4, $2, 4
	.noalias	$3,$sp
	.noalias	$5,$sp
	addu	$5, $3, $4
	.noalias	$6,$3
	.noalias	$6,$5
	.noalias	$6,$sp
	.noalias	$7,$3
	.noalias	$7,$5
	.noalias	$7,$6
	.noalias	$7,$sp
	.noalias	$8,$3
	.noalias	$8,$5
	.noalias	$8,$6
	.noalias	$8,$7
	.noalias	$8,$sp
	addu	$15, $6, $4
	.noalias	$15,$3
	.noalias	$15,$5
	.noalias	$15,$7
	.noalias	$15,$8
	.noalias	$15,$sp
	l.s	$f4, 0($15)
	.alias	$15,$3
	.alias	$15,$5
	.alias	$15,$7
	.alias	$15,$8
	.alias	$15,$sp
	addu	$24, $7, $4
	.noalias	$24,$3
	.noalias	$24,$5
	.noalias	$24,$6
	.noalias	$24,$8
	.noalias	$24,$sp
	l.s	$f6, 0($24)
	.alias	$24,$3
	.alias	$24,$5
	.alias	$24,$6
	.alias	$24,$8
	.alias	$24,$sp
	mul.s	$f8, $f4, $f6
	addu	$25, $8, $4
	.noalias	$25,$3
	.noalias	$25,$5
	.noalias	$25,$6
	.noalias	$25,$7
	.noalias	$25,$sp
	l.s	$f10, 0($25)
	.alias	$25,$3
	.alias	$25,$5
	.alias	$25,$6
	.alias	$25,$7
	.alias	$25,$sp
	mul.s	$f4, $f8, $f10
	s.s	$f4, 0($5)
	.loc	2 171
 # 170	        } while (0);
 # 171	        do {
	.loc	2 171
	.loc	2 172
 # 172	            D_803BA190[player][component] *= D_803B2A08[D_801111A9[player][D_803B9FD0[player]]][component];
	.noalias	$9,$3
	.noalias	$9,$5
	.noalias	$9,$6
	.noalias	$9,$7
	.noalias	$9,$8
	.noalias	$9,$sp
	l.s	$f6, 0($5)
	addu	$14, $9, $4
	.noalias	$14,$3
	.noalias	$14,$5
	.noalias	$14,$6
	.noalias	$14,$7
	.noalias	$14,$8
	.noalias	$14,$sp
	l.s	$f8, 0($14)
	.alias	$14,$3
	.alias	$14,$5
	.alias	$14,$6
	.alias	$14,$7
	.alias	$14,$8
	.alias	$14,$sp
	mul.s	$f10, $f6, $f8
	s.s	$f10, 0($5)
	.loc	2 174
 # 173	        } while (0);
 # 174	        D_803BA190[player][component] *= D_803B2A58[D_8011123D[player][D_803B9FD0[player]]][component];
	.noalias	$10,$3
	.noalias	$10,$5
	.noalias	$10,$6
	.noalias	$10,$7
	.noalias	$10,$8
	.noalias	$10,$9
	.noalias	$10,$sp
	l.s	$f4, 0($5)
	addu	$15, $10, $4
	.noalias	$15,$3
	.noalias	$15,$5
	.noalias	$15,$6
	.noalias	$15,$7
	.noalias	$15,$8
	.noalias	$15,$9
	.noalias	$15,$sp
	l.s	$f6, 0($15)
	.alias	$15,$3
	.alias	$15,$5
	.alias	$15,$6
	.alias	$15,$7
	.alias	$15,$8
	.alias	$15,$9
	.alias	$15,$sp
	mul.s	$f8, $f4, $f6
	s.s	$f8, 0($5)
	.loc	2 166
 # 166	    for (component = 0; component < 4; component++) {
	addu	$2, $2, 1
	sll	$24, $2, 16
	move	$2, $24
	sra	$25, $2, 16
	move	$2, $25
	blt	$2, 4, $51
	.alias	$3,$6
	.alias	$3,$7
	.alias	$3,$8
	.alias	$3,$9
	.alias	$3,$10
	.alias	$3,$sp
	.alias	$5,$6
	.alias	$5,$7
	.alias	$5,$8
	.alias	$5,$9
	.alias	$5,$10
	.alias	$5,$sp
	.alias	$6,$7
	.alias	$6,$8
	.alias	$6,$9
	.alias	$6,$10
	.alias	$6,$sp
	.alias	$7,$8
	.alias	$7,$9
	.alias	$7,$10
	.alias	$7,$sp
	.alias	$8,$9
	.alias	$8,$10
	.alias	$8,$sp
	.alias	$9,$10
	.alias	$9,$sp
	.alias	$10,$sp
	.loc	2 176
 # 176	}
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	func_8039D300
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 899
 # 899	{
	.ent	func_8039D494 2
func_8039D494:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 899
	.loc	2 899
	.loc	2 900
 # 900	    s32 available = 1;
	li	$3, 1
	.loc	2 901
 # 901	    if ((item == 13 || item == 14) && !D_801407D0) available = 0;
	beq	$4, 13, $52
	bne	$4, 14, $53
$52:
	lb	$14, D_801407D0
	bne	$14, 0, $53
	.loc	2 901
	move	$3, $0
$53:
	.loc	2 902
 # 902	    if (!D_80156994) {
	lb	$15, D_80156994
	bne	$15, 0, $56
	.loc	2 902
	.loc	2 903
 # 903	        if (item == 11 && D_8014978C >= 0 && D_8014978C < 6) available = 0;
	bne	$4, 11, $54
	lb	$2, D_8014978C
	blt	$2, 0, $54
	bge	$2, 6, $54
	.loc	2 903
	move	$3, $0
$54:
	li	$6, 17
	.loc	2 904
 # 904	        if (item == 17 || item == 18) available = 0;
	beq	$4, $6, $55
	bne	$4, 18, $56
$55:
	.loc	2 904
	move	$3, $0
$56:
	li	$6, 17
	.loc	2 906
 # 905	    }
 # 906	    if (!D_803AF980 && (item == 16 || item == 17 || item == 18)) available = 0;
	lw	$24, D_803AF980
	bne	$24, 0, $58
	beq	$4, 16, $57
	beq	$4, $6, $57
	bne	$4, 18, $58
$57:
	.loc	2 906
	move	$3, $0
$58:
	li	$7, 6
	.loc	2 907
 # 907	    if (item == 6 && (D_8014A110 == 2 || D_8014A110 == 6)) available = 0;
	bne	$4, $7, $60
	la	$8, D_8014A110
	.noalias	$8,$sp
	lw	$2, 0($8)
	beq	$2, 2, $59
	bne	$7, $2, $60
$59:
	.loc	2 907
	move	$3, $0
$60:
	li	$2, 7
	la	$8, D_8014A110
	.loc	2 908
 # 908	    if ((item == 7 || item == 8 || item == 9) && D_8014A110 == 6 && !D_801164BE) available = 0;
	beq	$4, $2, $61
	beq	$4, 8, $61
	bne	$4, 9, $62
$61:
	lw	$25, 0($8)
	bne	$7, $25, $62
	lh	$14, D_801164BE
	bne	$14, 0, $62
	.loc	2 908
	move	$3, $0
$62:
	.loc	2 909
 # 909	    if (item == 7 && !(D_803B3020[D_803B9FD0[player]] & 1)) available = 0;
	bne	$4, $2, $63
	la	$2, D_803B3020
	la	$9, D_803B9FD0
	.noalias	$2,$8
	.noalias	$2,$sp
	.noalias	$9,$2
	.noalias	$9,$8
	.noalias	$9,$sp
	addu	$15, $9, $5
	.noalias	$15,$2
	.noalias	$15,$8
	.noalias	$15,$sp
	lb	$24, 0($15)
	.alias	$15,$2
	.alias	$15,$8
	.alias	$15,$sp
	addu	$25, $2, $24
	.noalias	$25,$8
	.noalias	$25,$9
	.noalias	$25,$sp
	lb	$14, 0($25)
	.alias	$25,$8
	.alias	$25,$9
	.alias	$25,$sp
	and	$15, $14, 1
	bne	$15, 0, $63
	.loc	2 909
	move	$3, $0
$63:
	la	$2, D_803B3020
	la	$9, D_803B9FD0
	.loc	2 910
 # 910	    if (item == 8 && !(D_803B3020[D_803B9FD0[player]] & 2)) available = 0;
	bne	$4, 8, $64
	addu	$24, $9, $5
	.noalias	$24,$2
	.noalias	$24,$8
	.noalias	$24,$sp
	lb	$25, 0($24)
	.alias	$24,$2
	.alias	$24,$8
	.alias	$24,$sp
	addu	$14, $2, $25
	.noalias	$14,$8
	.noalias	$14,$9
	.noalias	$14,$sp
	lb	$15, 0($14)
	.alias	$14,$8
	.alias	$14,$9
	.alias	$14,$sp
	and	$24, $15, 2
	bne	$24, 0, $64
	.loc	2 910
	move	$3, $0
$64:
	.loc	2 911
 # 911	    if (item == 9 && !(D_803B3020[D_803B9FD0[player]] & 4)) available = 0;
	bne	$4, 9, $65
	addu	$25, $9, $5
	.noalias	$25,$2
	.noalias	$25,$8
	.noalias	$25,$sp
	lb	$14, 0($25)
	.alias	$25,$2
	.alias	$25,$8
	.alias	$25,$sp
	addu	$15, $2, $14
	.noalias	$15,$8
	.noalias	$15,$9
	.noalias	$15,$sp
	lb	$24, 0($15)
	.alias	$15,$8
	.alias	$15,$9
	.alias	$15,$sp
	and	$25, $24, 4
	bne	$25, 0, $65
	.alias	$2,$8
	.alias	$2,$9
	.alias	$2,$sp
	.alias	$9,$8
	.alias	$9,$sp
	.loc	2 911
	move	$3, $0
$65:
	.loc	2 912
 # 912	    if (item == 10 && D_8014A110 != 6) available = 0;
	bne	$4, 10, $66
	lw	$14, 0($8)
	beq	$7, $14, $66
	.loc	2 912
	move	$3, $0
$66:
	.loc	2 913
 # 913	    if ((item == 16 || item == 17 || item == 18) && D_8014A110 != 2) available = 0;
	beq	$4, 16, $67
	beq	$4, $6, $67
	bne	$4, 18, $68
$67:
	lw	$15, 0($8)
	beq	$15, 2, $68
	.alias	$8,$sp
	.loc	2 913
	move	$3, $0
$68:
	.loc	2 914
 # 914	    if (item == 12 || item == 15) available = 0;
	beq	$4, 12, $69
	bne	$4, 15, $70
$69:
	.loc	2 914
	move	$3, $0
$70:
	.loc	2 915
 # 915	    return available;
	move	$2, $3
	.livereg	0x2000FF0E,0x00000FFF
	j	$31
	.end	func_8039D494
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 1135
 #1135	{
	.ent	func_8039D6A4 2
func_8039D6A4:
	.option	O3
	subu	$sp, 152
	sw	$31, 28($sp)
	.mask	0x80000000, -124
	.frame	$sp, 152, $31
	.loc	2 1135
	sw	$30, 152($sp)
	sll	$14, $30, 24
	move	$30, $14
	sra	$15, $30, 24
	move	$30, $15
	.loc	2 1135
	.loc	2 1142
 #1142	    s8 clearScrollSpeed=1;
	li	$24, 1
	sb	$24, 127($sp)
	.loc	2 1146
 #1143	    s32 mask;
 #1144	    void *tex;
 #1145	    u16 texIdx;
 #1146	    Color4 buttonColor={{255,255,255,255}};
	lw	$25, $$195
	sw	$25, 108($sp)
	.loc	2 1147
 #1147	    if(!D_803B9B90[p])return;
	mul	$3, $30, 4
	lw	$14, D_803B9B90($3)
	beq	$14, 0, $111
	.loc	2 1147
	la	$22, D_803B25AC
	.loc	2 1148
 #1148	    if((D_803BA050[p]-1)*D_803B25B0*D_803B25AC>D_803B25B8-D_803B25BC)
	l.s	$f14, D_803B25B8
	l.s	$f4, D_803B25BC
	sub.s	$f0, $f14, $f4
	lw	$15, D_803BA050($3)
	addu	$24, $15, -1
	mtc1	$24, $f6
	cvt.s.w	$f2, $f6
	l.s	$f12, D_803B25B0
	.noalias	$22,$sp
	l.s	$f18, 0($22)
	mul.s	$f8, $f2, $f12
	mul.s	$f10, $f8, $f18
	c.lt.s	$f0, $f10
	bc1f	$71
	.loc	2 1149
 #1149	        selectedPos=D_803B25B8-D_803B28B8[p]*(D_803B25B8-D_803B25BC)/(D_803BA050[p]-1);
	la	$25, D_803B28B8
	addu	$14, $3, $25
	sw	$14, 76($sp)
	lw	$15, 76($sp)
	lw	$24, 0($15)
	mtc1	$24, $f4
	cvt.s.w	$f6, $f4
	mul.s	$f8, $f6, $f0
	div.s	$f10, $f8, $f2
	sub.s	$f20, $f14, $f10
	b	$72
$71:
	.loc	2 1150
 #1150	    else selectedPos=D_803B25B8-D_803B28B8[p]*D_803B25B0*D_803B25AC;
	.noalias	$2,$22
	.noalias	$2,$sp
	la	$25, D_803B28B8
	addu	$2, $3, $25
	lw	$14, 0($2)
	mtc1	$14, $f4
	cvt.s.w	$f6, $f4
	mul.s	$f8, $f6, $f12
	mul.s	$f10, $f8, $f18
	sub.s	$f20, $f14, $f10
	sw	$2, 76($sp)
	.alias	$2,$22
	.alias	$2,$sp
$72:
	.loc	2 1151
 #1151	    memcpy(prevPos,D_803AF9A8[p][39].position,12);
	addu	$4, $sp, 136
	mul	$15, $30, 2816
	la	$24, D_803AF9A8
	addu	$2, $15, $24
	addu	$5, $2, 2544
	li	$6, 12
	sw	$2, 88($sp)
	sw	$3, 100($sp)
	.livereg	0x2E00000E,0x00000000
	jal	memcpy
	lw	$3, 88($sp)
	la	$19, D_8002EB94
	li.s	$f2, 6.2831855
	.loc	2 1152
 #1152	    D_803AF9A8[p][39].angle+=6.2831855f*D_8002EB94;
	.noalias	$3,$22
	.noalias	$3,$sp
	.noalias	$19,$3
	.noalias	$19,$22
	.noalias	$19,$sp
	l.s	$f4, 2504($3)
	.set	 volatile
	l.s	$f6, 0($19)
	.set	 novolatile
	mul.s	$f8, $f2, $f6
	add.s	$f10, $f4, $f8
	s.s	$f10, 2504($3)
	.loc	2 1153
 #1153	    if(D_803AF9A8[p][39].angle>6.2831855f)D_803AF9A8[p][39].angle-=6.2831855f;
	l.s	$f0, 2504($3)
	c.lt.s	$f2, $f0
	bc1f	$73
	.loc	2 1153
	sub.s	$f6, $f0, $f2
	s.s	$f6, 2504($3)
$73:
	lw	$8, 100($sp)
	.loc	2 1154
 #1154	    D_803AF9A8[p][39].position[0]=D_803B25B4*D_803B25C4/D_803B25C0+-170.0f*D_803B25AC;
	l.s	$f0, D_803B25C4
	l.s	$f2, D_803B25C0
	l.s	$f18, 0($22)
	l.s	$f4, D_803B25B4
	mul.s	$f8, $f4, $f0
	div.s	$f10, $f8, $f2
	li.s	$f6, -1.7000000000000000e+02
	mul.s	$f4, $f6, $f18
	add.s	$f8, $f10, $f4
	s.s	$f8, 2544($3)
	.loc	2 1155
 #1155	    D_803AF9A8[p][39].position[1]=selectedPos*D_803B25C4/D_803B25C0;
	mul.s	$f6, $f20, $f0
	div.s	$f10, $f6, $f2
	s.s	$f10, 2548($3)
	.loc	2 1156
 #1156	    D_803AF9A8[p][39].position[2]=D_803B25C4;
	s.s	$f0, 2552($3)
	.loc	2 1157
 #1157	    if(!D_803B9BA0[p]) {
	.noalias	$7,$3
	.noalias	$7,$19
	.noalias	$7,$22
	.noalias	$7,$sp
	la	$25, D_803B9BA0
	addu	$7, $8, $25
	lw	$14, 0($7)
	bne	$14, 0, $80
	l.s	$f22, 140($sp)
	.loc	2 1157
	.loc	2 1158
 #1158	        if(cursorSpeed[p]<fabsf(D_803AF9A8[p][39].position[1]-prevPos[1])*60.0f*D_803B25AC/10.0f) {
	l.s	$f16, 2548($3)
	sub.s	$f12, $f16, $f22
	abs.s	$f12, $f12
	mov.s	$f0, $f12
	.noalias	$2,$3
	.noalias	$2,$7
	.noalias	$2,$19
	.noalias	$2,$22
	.noalias	$2,$sp
	la	$15, cursorSpeed
	addu	$2, $8, $15
	l.s	$f2, 0($2)
	li.s	$f4, 60.0
	mul.s	$f8, $f0, $f4
	mul.s	$f6, $f8, $f18
	li.s	$f10, 10.0
	div.s	$f4, $f6, $f10
	c.lt.s	$f2, $f4
	bc1f	$77
	.loc	2 1158
	.loc	2 1159
 #1159	            cursorSpeed[p]=fabsf(D_803AF9A8[p][39].position[1]-prevPos[1])*60.0f*D_803B25AC/10.0f;
	mov.s	$f0, $f12
	li.s	$f8, 60.0
	mul.s	$f6, $f0, $f8
	mul.s	$f10, $f6, $f18
	li.s	$f4, 10.0
	div.s	$f2, $f10, $f4
	s.s	$f2, 0($2)
	.loc	2 1160
 #1160	            cursorSpeed[p]=cursorSpeed[p]<540.0f*D_803B25AC ? cursorSpeed[p] : 540.0f*D_803B25AC;
	li.s	$f8, 540.0
	mul.s	$f14, $f8, $f18
	c.lt.s	$f2, $f14
	bc1f	$74
	l.s	$f6, 0($2)
	s.s	$f6, 0($2)
	b	$75
$74:
	s.s	$f14, 0($2)
$75:
	.loc	2 1161
 #1161	            cursorSpeed[p]=cursorSpeed[p]>180.0f*D_803B25AC ? cursorSpeed[p] : 180.0f*D_803B25AC;
	li.s	$f10, 180.0
	mul.s	$f0, $f10, $f18
	l.s	$f2, 0($2)
	c.lt.s	$f0, $f2
	bc1f	$76
	s.s	$f2, 0($2)
	l.s	$f2, 0($2)
	b	$77
$76:
	s.s	$f0, 0($2)
	l.s	$f2, 0($2)
$77:
	.loc	2 1163
 #1162	        }
 #1163	        if(D_803AF9A8[p][39].position[1]>prevPos[1]+D_8002EB94*cursorSpeed[p])
	.set	 volatile
	l.s	$f4, 0($19)
	.set	 novolatile
	mul.s	$f8, $f2, $f4
	add.s	$f6, $f8, $f22
	c.lt.s	$f6, $f16
	bc1f	$78
	.alias	$2,$3
	.alias	$2,$7
	.alias	$2,$19
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 1164
 #1164	            D_803AF9A8[p][39].position[1]=prevPos[1]+D_8002EB94*cursorSpeed[p];
	.set	 volatile
	l.s	$f10, 0($19)
	.set	 novolatile
	mul.s	$f4, $f2, $f10
	add.s	$f8, $f4, $f22
	s.s	$f8, 2548($3)
	b	$80
$78:
	.loc	2 1165
 #1165	        else if(D_803AF9A8[p][39].position[1]<prevPos[1]-D_8002EB94*cursorSpeed[p])
	.set	 volatile
	l.s	$f6, 0($19)
	.set	 novolatile
	mul.s	$f10, $f2, $f6
	sub.s	$f4, $f22, $f10
	c.lt.s	$f16, $f4
	bc1f	$79
	.loc	2 1166
 #1166	            D_803AF9A8[p][39].position[1]=prevPos[1]-D_8002EB94*cursorSpeed[p];
	.set	 volatile
	l.s	$f8, 0($19)
	.set	 novolatile
	mul.s	$f6, $f2, $f8
	sub.s	$f10, $f22, $f6
	s.s	$f10, 2548($3)
	b	$80
$79:
	.loc	2 1167
 #1167	        else cursorSpeed[p]=180.0f*D_803B25AC;
	.noalias	$2,$3
	.noalias	$2,$7
	.noalias	$2,$19
	.noalias	$2,$22
	.noalias	$2,$sp
	li.s	$f4, 180.0
	mul.s	$f8, $f4, $f18
	s.s	$f8, 0($2)
	.alias	$2,$3
	.alias	$2,$7
	.alias	$2,$19
	.alias	$2,$22
	.alias	$2,$sp
$80:
	.loc	2 1169
 #1168	    }
 #1169	    func_8008B32C(D_8011418C,D_803AF9A8[p][39].matrix,D_803B25AC*0.4f);
	la	$4, D_8011418C
	addu	$16, $3, 2508
	move	$5, $16
	li.s	$f6, 0.4
	mul.s	$f10, $f18, $f6
	mfc1	$6, $f10
	sw	$7, 84($sp)
	.livereg	0x0E00800E,0x00000000
	jal	func_8008B32C
	.alias	$3,$7
	.alias	$3,$19
	.alias	$3,$22
	.alias	$3,$sp
	.alias	$7,$19
	.alias	$7,$22
	.alias	$7,$sp
	.loc	2 1170
 #1170	    func_800B5940(D_803AF9A8[p][39].angle,D_803AF9A8[p][39].matrix);
	lw	$24, 88($sp)
	l.s	$f12, 2504($24)
	move	$5, $16
	.livereg	0x0400000E,0x00080000
	jal	func_800B5940
	li.s	$f30, 1.5707964
	.loc	2 1171
 #1171	    func_800B5898(1.5707964f,D_803AF9A8[p][39].matrix);
	mov.s	$f12, $f30
	move	$5, $16
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	lw	$2, 76($sp)
	.loc	2 1172
 #1172	    buttonY=selectedPos+D_803B28B8[p]*D_803B25B0*D_803B25AC;
	.noalias	$2,$19
	.noalias	$2,$22
	.noalias	$2,$sp
	lw	$25, 0($2)
	mtc1	$25, $f4
	cvt.s.w	$f8, $f4
	l.s	$f6, D_803B25B0
	mul.s	$f10, $f8, $f6
	l.s	$f4, 0($22)
	mul.s	$f8, $f10, $f4
	add.s	$f24, $f8, $f20
	.loc	2 1173
 #1173	    for(buttonIdx=0;buttonIdx<19;buttonIdx++) {
	move	$18, $0
	mul	$14, $30, 76
	la	$15, D_803BA060
	addu	$24, $14, $15
	sw	$24, 72($sp)
	mul	$25, $30, 2816
	la	$14, D_803AF9A8
	addu	$16, $25, $14
	addu	$23, $16, 48
	addu	$20, $16, 12
	addu	$21, $16, 1228
	la	$15, D_803BA028
	addu	$24, $30, $15
	sw	$24, 40($sp)
	li.s	$f28, 0.0
	li.s	$f26, 3.1415927
$81:
	lw	$2, 76($sp)
	.loc	2 1173
	.loc	2 1174
 #1174	        if(buttonIdx==D_803BA060[p][D_803B28B8[p]]) {
	lw	$25, 72($sp)
	lw	$14, 0($2)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lw	$14, 0($24)
	bne	$18, $14, $84
	.loc	2 1174
	.loc	2 1175
 #1175	            if(D_803AF9A8[p][buttonIdx].angle<3.1415927f)D_803AF9A8[p][buttonIdx].angle+=12.566371f*D_8002EB94;
	.noalias	$16,$2
	.noalias	$16,$19
	.noalias	$16,$22
	.noalias	$16,$sp
	l.s	$f0, 8($16)
	c.lt.s	$f0, $f26
	bc1f	$82
	.loc	2 1175
	li.s	$f6, 12.566371
	.set	 volatile
	l.s	$f10, 0($19)
	.set	 novolatile
	mul.s	$f4, $f6, $f10
	add.s	$f8, $f0, $f4
	s.s	$f8, 8($16)
	l.s	$f0, 8($16)
$82:
	.loc	2 1176
 #1176	            if(D_803AF9A8[p][buttonIdx].angle>3.1415927f)D_803AF9A8[p][buttonIdx].angle=3.1415927f;
	c.lt.s	$f26, $f0
	bc1f	$83
	.loc	2 1176
	s.s	$f26, 8($16)
$83:
	.loc	2 1177
 #1177	            if(D_803B9BA0[p])D_803AF9A8[p][buttonIdx].angle=3.1415927f;
	lw	$25, 84($sp)
	lw	$15, 0($25)
	beq	$15, 0, $87
	.loc	2 1177
	s.s	$f26, 8($16)
	b	$87
$84:
	.loc	2 1178
 #1178	        } else {
	.loc	2 1179
 #1179	            if(D_803AF9A8[p][buttonIdx].angle>0.0f)D_803AF9A8[p][buttonIdx].angle-=12.566371f*D_8002EB94;
	l.s	$f0, 8($16)
	c.lt.s	$f28, $f0
	bc1f	$85
	.loc	2 1179
	li.s	$f6, 12.566371
	.set	 volatile
	l.s	$f10, 0($19)
	.set	 novolatile
	mul.s	$f4, $f6, $f10
	sub.s	$f8, $f0, $f4
	s.s	$f8, 8($16)
	l.s	$f0, 8($16)
$85:
	.loc	2 1180
 #1180	            if(D_803AF9A8[p][buttonIdx].angle<0.0f)D_803AF9A8[p][buttonIdx].angle=0.0f;
	c.lt.s	$f0, $f28
	bc1f	$86
	.loc	2 1180
	s.s	$f28, 8($16)
$86:
	.loc	2 1181
 #1181	            if(D_803B9BA0[p])D_803AF9A8[p][buttonIdx].angle=0.0f;
	lw	$24, 84($sp)
	lw	$14, 0($24)
	beq	$14, 0, $87
	.loc	2 1181
	s.s	$f28, 8($16)
$87:
	.loc	2 1183
 #1182	        }
 #1183	        memcpy(prevPos,D_803AF9A8[p][buttonIdx].position,12);
	addu	$4, $sp, 136
	move	$5, $23
	li	$6, 12
	.livereg	0x0E00000E,0x00000000
	jal	memcpy
	.alias	$2,$16
	.alias	$2,$19
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 1184
 #1184	        D_803AF9A8[p][buttonIdx].position[0]=-120.0f*D_803B25AC+D_803B25B4;
	l.s	$f18, 0($22)
	l.s	$f20, D_803B25B4
	li.s	$f6, -1.2000000000000000e+02
	mul.s	$f10, $f6, $f18
	add.s	$f4, $f10, $f20
	s.s	$f4, 48($16)
	.loc	2 1185
 #1185	        D_803AF9A8[p][buttonIdx].position[1]=buttonY;
	s.s	$f24, 52($16)
	.loc	2 1186
 #1186	        D_803AF9A8[p][buttonIdx].position[2]=D_803B25C0;
	l.s	$f8, D_803B25C0
	s.s	$f8, 56($16)
	.loc	2 1187
 #1187	        if(!D_803B9BA0[p]) {
	lw	$25, 84($sp)
	lw	$15, 0($25)
	bne	$15, 0, $93
	l.s	$f22, 140($sp)
	.loc	2 1187
	.loc	2 1188
 #1188	            if(scrollSpeed[p]<fabsf(D_803AF9A8[p][buttonIdx].position[1]-prevPos[1])*60.0f/10.0f) {
	l.s	$f12, 52($16)
	sub.s	$f16, $f12, $f22
	abs.s	$f16, $f16
	mov.s	$f0, $f16
	.noalias	$2,$16
	.noalias	$2,$19
	.noalias	$2,$22
	.noalias	$2,$sp
	lw	$24, 100($sp)
	la	$14, scrollSpeed
	addu	$2, $24, $14
	l.s	$f2, 0($2)
	li.s	$f6, 60.0
	mul.s	$f10, $f0, $f6
	li.s	$f4, 10.0
	div.s	$f8, $f10, $f4
	c.lt.s	$f2, $f8
	bc1f	$91
	.loc	2 1188
	.loc	2 1189
 #1189	                scrollSpeed[p]=fabsf(D_803AF9A8[p][buttonIdx].position[1]-prevPos[1])*60.0f/10.0f;
	mov.s	$f0, $f16
	li.s	$f6, 60.0
	mul.s	$f10, $f0, $f6
	li.s	$f4, 10.0
	div.s	$f2, $f10, $f4
	s.s	$f2, 0($2)
	.loc	2 1190
 #1190	                scrollSpeed[p]=scrollSpeed[p]<540.0f*D_803B25AC ? scrollSpeed[p] : 540.0f*D_803B25AC;
	li.s	$f8, 540.0
	mul.s	$f14, $f8, $f18
	c.lt.s	$f2, $f14
	bc1f	$88
	l.s	$f6, 0($2)
	s.s	$f6, 0($2)
	b	$89
$88:
	s.s	$f14, 0($2)
$89:
	.loc	2 1191
 #1191	                scrollSpeed[p]=scrollSpeed[p]>180.0f*D_803B25AC ? scrollSpeed[p] : 180.0f*D_803B25AC;
	li.s	$f10, 180.0
	mul.s	$f0, $f10, $f18
	l.s	$f2, 0($2)
	c.lt.s	$f0, $f2
	bc1f	$90
	s.s	$f2, 0($2)
	l.s	$f2, 0($2)
	b	$91
$90:
	s.s	$f0, 0($2)
	l.s	$f2, 0($2)
	.alias	$2,$16
	.alias	$2,$19
	.alias	$2,$22
	.alias	$2,$sp
$91:
	.loc	2 1193
 #1192	            }
 #1193	            if(D_803AF9A8[p][buttonIdx].position[1]>prevPos[1]+D_8002EB94*scrollSpeed[p]) {
	.set	 volatile
	l.s	$f4, 0($19)
	.set	 novolatile
	mul.s	$f8, $f2, $f4
	add.s	$f6, $f8, $f22
	c.lt.s	$f6, $f12
	bc1f	$92
	.loc	2 1193
	.loc	2 1194
 #1194	                D_803AF9A8[p][buttonIdx].position[1]=prevPos[1]+D_8002EB94*scrollSpeed[p];
	.set	 volatile
	l.s	$f10, 0($19)
	.set	 novolatile
	mul.s	$f4, $f2, $f10
	add.s	$f8, $f4, $f22
	s.s	$f8, 52($16)
	.loc	2 1195
 #1195	                clearScrollSpeed=0;
	sb	$0, 127($sp)
	b	$93
$92:
	.loc	2 1196
 #1196	            } else if(D_803AF9A8[p][buttonIdx].position[1]<prevPos[1]-D_8002EB94*scrollSpeed[p]) {
	.set	 volatile
	l.s	$f6, 0($19)
	.set	 novolatile
	mul.s	$f10, $f2, $f6
	sub.s	$f4, $f22, $f10
	c.lt.s	$f12, $f4
	bc1f	$93
	.loc	2 1196
	.loc	2 1197
 #1197	                D_803AF9A8[p][buttonIdx].position[1]=prevPos[1]-D_8002EB94*scrollSpeed[p];
	.set	 volatile
	l.s	$f8, 0($19)
	.set	 novolatile
	mul.s	$f6, $f2, $f8
	sub.s	$f10, $f22, $f6
	s.s	$f10, 52($16)
	.loc	2 1198
 #1198	                clearScrollSpeed=0;
	sb	$0, 127($sp)
$93:
	.loc	2 1201
 #1199	            }
 #1200	        }
 #1201	        D_803AF9A8[p][buttonIdx+19].position[0]=120.0f*D_803B25AC+D_803B25B4;
	li.s	$f4, 120.0
	mul.s	$f8, $f4, $f18
	add.s	$f6, $f8, $f20
	s.s	$f6, 1264($16)
	.loc	2 1202
 #1202	        D_803AF9A8[p][buttonIdx+19].position[1]=D_803AF9A8[p][buttonIdx].position[1];
	l.s	$f10, 52($16)
	s.s	$f10, 1268($16)
	.loc	2 1203
 #1203	        D_803AF9A8[p][buttonIdx+19].position[2]=D_803AF9A8[p][buttonIdx].position[2];
	l.s	$f4, 56($16)
	s.s	$f4, 1272($16)
	.loc	2 1204
 #1204	        D_803AF9A8[p][buttonIdx+19].angle=D_803AF9A8[p][buttonIdx].angle;
	l.s	$f8, 8($16)
	s.s	$f8, 1224($16)
	.loc	2 1205
 #1205	        func_8008B32C(D_8011418C,D_803AF9A8[p][buttonIdx].matrix,D_803B25AC);
	la	$4, D_8011418C
	move	$5, $20
	mfc1	$6, $f18
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1206
 #1206	        func_8008B32C(D_8011418C,D_803AF9A8[p][buttonIdx+19].matrix,D_803B25AC);
	la	$4, D_8011418C
	move	$5, $21
	lw	$6, 0($22)
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1207
 #1207	        func_800B5898(D_803AF9A8[p][buttonIdx].angle-1.5707964f,D_803AF9A8[p][buttonIdx].matrix);
	l.s	$f6, 8($16)
	sub.s	$f12, $f6, $f30
	move	$5, $20
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1208
 #1208	        func_800B5898(D_803AF9A8[p][buttonIdx+19].angle-1.5707964f,D_803AF9A8[p][buttonIdx+19].matrix);
	l.s	$f10, 1224($16)
	sub.s	$f12, $f10, $f30
	move	$5, $21
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1209
 #1209	        if(D_803AF9A8[p][buttonIdx].angle>1.5707964f)tex=func_800B24EC(D_803B8650,&texIdx,0,D_80140BDC-1,1);
	l.s	$f4, 8($16)
	c.lt.s	$f30, $f4
	bc1f	$94
	.loc	2 1209
	la	$4, D_803B8650
	addu	$5, $sp, 114
	move	$6, $0
	la	$25, D_80140BDC
	.set	 volatile
	lbu	$7, 0($25)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$15, $7, 24
	move	$7, $15
	sra	$24, $7, 24
	move	$7, $24
	li	$14, 1
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800B24EC
	move	$17, $2
	b	$95
$94:
	.loc	2 1210
 #1210	        else tex=func_800B24EC(D_803B8660,&texIdx,0,D_80140BDC-1,1);
	la	$4, D_803B8660
	addu	$5, $sp, 114
	move	$6, $0
	la	$25, D_80140BDC
	.set	 volatile
	lbu	$7, 0($25)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$15, $7, 24
	move	$7, $15
	sra	$24, $7, 24
	move	$7, $24
	li	$14, 1
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800B24EC
	move	$17, $2
$95:
	.loc	2 1211
 #1211	        func_8008D870((s16)D_803AF9A8[p][buttonIdx].handle,tex,-1);
	lh	$4, 6($16)
	move	$5, $17
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
	.loc	2 1212
 #1212	        func_8008D870((s16)D_803AF9A8[p][buttonIdx+19].handle,tex,-1);
	lh	$4, 1222($16)
	move	$5, $17
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
	.loc	2 1213
 #1213	        if(D_803AF9A8[p][buttonIdx].position[1]>D_803B25B8+D_803B25B0*D_803B25AC || D_803AF9A8[p][buttonIdx].position[1]<D_803B25BC-D_803B25B0*D_803B25AC)D_803AF9A8[p][buttonIdx].alpha=0;
	l.s	$f14, D_803B25B8
	l.s	$f8, D_803B25B0
	l.s	$f6, 0($22)
	mul.s	$f2, $f8, $f6
	l.s	$f12, 52($16)
	add.s	$f10, $f14, $f2
	c.lt.s	$f10, $f12
	bc1t	$96
	l.s	$f0, D_803B25BC
	sub.s	$f4, $f0, $f2
	c.lt.s	$f12, $f4
	bc1f	$97
$96:
	.loc	2 1213
	sb	$0, 60($16)
	b	$100
$97:
	.loc	2 1214
 #1214	        else if(D_803AF9A8[p][buttonIdx].position[1]>D_803B25B8)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803AF9A8[p][buttonIdx].position[1]-D_803B25B8)*255/(D_803B25B0*D_803B25AC);
	c.lt.s	$f14, $f12
	bc1f	$98
	.loc	2 1214
	sub.s	$f8, $f12, $f14
	li.s	$f6, 2.5500000000000000e+02
	mul.s	$f10, $f8, $f6
	div.s	$f4, $f10, $f2
	li.s	$f8, 255.0
	sub.s	$f6, $f8, $f4
	cfc1	$25, $31
	li	$15, 1
	ctc1	$15, $31
	cvt.w.s	$f10, $f6
	cfc1	$15, $31
	.set	 noat
	and	$1, $15, 4
	and	$15, $15, 120
	.set	 at
	beq	$15, 0, $113
	li.s	$f10, 2147483648.0
	sub.s	$f10, $f6, $f10
	li	$15, 1
	ctc1	$15, $31
	cvt.w.s	$f10, $f10
	cfc1	$15, $31
	.set	 noat
	and	$1, $15, 4
	and	$15, $15, 120
	.set	 at
	bne	$15, 0, $114
	.set	 noat
	mfc1	$15, $f10
	li	$1, -2147483648
	or	$15, $15, $1
	.set	 at
	b	$112
$114:
	li	$15, -1
	b	$112
$113:
	mfc1	$15, $f10
	blt	$15, 0, $114
$112:
	ctc1	$25, $31
	sb	$15, 60($16)
	b	$100
$98:
	.loc	2 1215
 #1215	        else if(D_803AF9A8[p][buttonIdx].position[1]<D_803B25BC)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803B25BC-D_803AF9A8[p][buttonIdx].position[1])*255/(D_803B25B0*D_803B25AC);
	c.lt.s	$f12, $f0
	bc1f	$99
	.loc	2 1215
	sub.s	$f8, $f0, $f12
	li.s	$f4, 2.5500000000000000e+02
	mul.s	$f6, $f8, $f4
	div.s	$f10, $f6, $f2
	li.s	$f8, 255.0
	sub.s	$f4, $f8, $f10
	cfc1	$24, $31
	li	$14, 1
	ctc1	$14, $31
	cvt.w.s	$f6, $f4
	cfc1	$14, $31
	.set	 noat
	and	$1, $14, 4
	and	$14, $14, 120
	.set	 at
	beq	$14, 0, $116
	li.s	$f6, 2147483648.0
	sub.s	$f6, $f4, $f6
	li	$14, 1
	ctc1	$14, $31
	cvt.w.s	$f6, $f6
	cfc1	$14, $31
	.set	 noat
	and	$1, $14, 4
	and	$14, $14, 120
	.set	 at
	bne	$14, 0, $117
	.set	 noat
	mfc1	$14, $f6
	li	$1, -2147483648
	or	$14, $14, $1
	.set	 at
	b	$115
$117:
	li	$14, -1
	b	$115
$116:
	mfc1	$14, $f6
	blt	$14, 0, $117
$115:
	ctc1	$24, $31
	sb	$14, 60($16)
	b	$100
$99:
	.loc	2 1216
 #1216	        else D_803AF9A8[p][buttonIdx].alpha=255;
	li	$25, 255
	sb	$25, 60($16)
$100:
	.loc	2 1217
 #1217	        if(!func_8039D494(p,buttonIdx))D_803AF9A8[p][buttonIdx].alpha=0;
	move	$5, $30
	move	$4, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8039D494
	bne	$2, 0, $101
	.loc	2 1217
	sb	$0, 60($16)
$101:
	.loc	2 1218
 #1218	        D_803AF9A8[p][buttonIdx+19].alpha=buttonColor.c[3]=D_803AF9A8[p][buttonIdx].alpha;
	lbu	$2, 60($16)
	sb	$2, 111($sp)
	sb	$2, 1276($16)
	.loc	2 1219
 #1219	        func_8008E06C((s16)D_803AF9A8[p][buttonIdx].handle,&buttonColor);
	lh	$4, 6($16)
	addu	$5, $sp, 108
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1220
 #1220	        func_8008E06C((s16)D_803AF9A8[p][buttonIdx+19].handle,&buttonColor);
	lh	$4, 1222($16)
	addu	$5, $sp, 108
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1221
 #1221	        if(!func_8039D494(p,buttonIdx)) {
	move	$5, $30
	move	$4, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8039D494
	bne	$2, 0, $102
	.loc	2 1221
	.loc	2 1222
 #1222	            model_data_load(D_803AF9A8[p][buttonIdx].handle,1,15);
	lw	$4, 4($16)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1223
 #1223	            model_data_load(D_803AF9A8[p][buttonIdx+19].handle,1,15);
	lw	$4, 1220($16)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	b	$105
$102:
	.loc	2 1224
 #1224	        } else if(!D_803AF9A8[p][buttonIdx].alpha || D_803BA028[p]==1) {
	lbu	$15, 60($16)
	beq	$15, 0, $103
	lw	$24, 40($sp)
	lb	$14, 0($24)
	bne	$14, 1, $104
$103:
	.loc	2 1224
	.loc	2 1225
 #1225	            model_data_load(D_803AF9A8[p][buttonIdx].handle,1,15);
	lw	$4, 4($16)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1226
 #1226	            model_data_load(D_803AF9A8[p][buttonIdx+19].handle,1,15);
	lw	$4, 1220($16)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1227
 #1227	            buttonY-=D_803B25B0*D_803B25AC;
	l.s	$f8, D_803B25B0
	l.s	$f10, 0($22)
	mul.s	$f4, $f8, $f10
	sub.s	$f24, $f24, $f4
	b	$105
$104:
	.loc	2 1228
 #1228	        } else {
	.loc	2 1229
 #1229	            mask=1<<p;
	.loc	2 1230
 #1230	            model_transform_setup(D_803AF9A8[p][buttonIdx].handle,0,mask);
	lw	$4, 4($16)
	move	$5, $0
	li	$25, 1
	sll	$17, $25, $30
	move	$6, $17
	.livereg	0x0E00400E,0x00000000
	jal	model_transform_setup
	.loc	2 1231
 #1231	            model_transform_setup(D_803AF9A8[p][buttonIdx+19].handle,0,mask);
	lw	$4, 1220($16)
	move	$5, $0
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
	.loc	2 1232
 #1232	            buttonY-=D_803B25B0*D_803B25AC;
	l.s	$f6, D_803B25B0
	l.s	$f8, 0($22)
	mul.s	$f10, $f6, $f8
	sub.s	$f24, $f24, $f10
$105:
	.loc	2 1173
 #1173	    for(buttonIdx=0;buttonIdx<19;buttonIdx++) {
	addu	$18, $18, 1
	addu	$16, $16, 64
	addu	$23, $23, 64
	addu	$20, $20, 64
	addu	$21, $21, 64
	bne	$18, 19, $81
	.alias	$16,$19
	.alias	$16,$22
	.alias	$16,$sp
	.alias	$19,$22
	.alias	$19,$sp
	.alias	$22,$sp
	.loc	2 1235
 #1235	    if(D_803BA028[p]==1) {
	lw	$15, 40($sp)
	lb	$24, 0($15)
	bne	$24, 1, $106
	.loc	2 1235
	.loc	2 1236
 #1236	        model_data_load(D_803AF9A8[p][40].handle,1,15);
	lw	$14, 88($sp)
	lw	$4, 2564($14)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1237
 #1237	        model_data_load(D_803AF9A8[p][41].handle,1,15);
	lw	$25, 88($sp)
	lw	$4, 2628($25)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	lw	$2, 88($sp)
	.loc	2 1238
 #1238	        D_803AF9A8[p][39].position[2]=2000.0f;
	.noalias	$2,$sp
	li.s	$f4, 2000.0
	s.s	$f4, 2552($2)
	b	$107
	.alias	$2,$sp
$106:
	.loc	2 1239
 #1239	    } else {
	.loc	2 1240
 #1240	        mask=1<<p;
	.loc	2 1241
 #1241	        model_transform_setup(D_803AF9A8[p][40].handle,0,mask);
	lw	$15, 88($sp)
	lw	$4, 2564($15)
	move	$5, $0
	li	$24, 1
	sll	$17, $24, $30
	move	$6, $17
	.livereg	0x0E00400E,0x00000000
	jal	model_transform_setup
	.loc	2 1242
 #1242	        model_transform_setup(D_803AF9A8[p][41].handle,0,mask);
	lw	$14, 88($sp)
	lw	$4, 2628($14)
	move	$5, $0
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
	lw	$2, 88($sp)
$107:
	.loc	2 1244
 #1243	    }
 #1244	    if(D_803BA02C==1) {
	lb	$25, D_803BA02C
	bne	$25, 1, $108
	.loc	2 1244
	.loc	2 1245
 #1245	        model_data_load(D_803AF9A8[p][42].handle,1,15);
	.noalias	$2,$sp
	lw	$4, 2692($2)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.alias	$2,$sp
	.loc	2 1246
 #1246	        model_data_load(D_803AF9A8[p][43].handle,1,15);
	lw	$15, 88($sp)
	lw	$4, 2756($15)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	b	$109
$108:
	.loc	2 1247
 #1247	    } else {
	.loc	2 1248
 #1248	        mask=1<<p;
	.loc	2 1249
 #1249	        model_transform_setup(D_803AF9A8[p][42].handle,0,mask);
	.noalias	$2,$sp
	lw	$4, 2692($2)
	move	$5, $0
	li	$24, 1
	sll	$17, $24, $30
	move	$6, $17
	.livereg	0x0E00400E,0x00000000
	jal	model_transform_setup
	.alias	$2,$sp
	.loc	2 1250
 #1250	        model_transform_setup(D_803AF9A8[p][43].handle,0,mask);
	lw	$14, 88($sp)
	lw	$4, 2756($14)
	move	$5, $0
	move	$6, $17
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
$109:
	.loc	2 1252
 #1251	    }
 #1252	    if(clearScrollSpeed==1)scrollSpeed[p]=0.0f;
	lb	$25, 127($sp)
	bne	$25, 1, $110
	.loc	2 1252
	lw	$15, 100($sp)
	s.s	$f28, scrollSpeed($15)
$110:
	.loc	2 1253
 #1253	    D_803B9BA0[p]=0;
	lw	$24, 84($sp)
	sw	$0, 0($24)
	.loc	2 1254
 #1254	}
$111:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 152
	j	$31
	.end	func_8039D6A4
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 927
 # 927	{
	.ent	func_8039E200 2
func_8039E200:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	2 927
	sw	$18, 28($sp)
	sll	$14, $18, 24
	move	$18, $14
	sra	$15, $18, 24
	move	$18, $15
	.loc	2 927
	.loc	2 930
 # 928	    Record **next;
 # 929	    Record *record;
 # 930	    if(!node) {
	bne	$17, 0, $119
	.loc	2 930
	.loc	2 931
 # 931	        if(direction>0)node=D_80152020.first;
	ble	$18, 0, $118
	la	$20, D_80152020
	.loc	2 931
	.noalias	$20,$sp
	lw	$17, 8($20)
	b	$119
	.alias	$20,$sp
$118:
	la	$22, D_803BA010
	.loc	2 932
 # 932	        else node=D_803BA010+2;
	.noalias	$22,$sp
	lw	$17, 0($22)
	addu	$17, $17, 8
$119:
	la	$20, D_80152020
	la	$22, D_803BA010
	.loc	2 934
 # 933	    }
 # 934	    while(node) {
	beq	$17, 0, $133
	la	$21, D_80152570
	la	$19, D_80152698
$120:
	.loc	2 934
	.loc	2 935
 # 935	        record=*node;
	lw	$4, 0($17)
	move	$3, $4
	.loc	2 936
 # 936	        if(direction>0)next=record->next;
	lb	$5, D_8014978C
	ble	$18, 0, $121
	.loc	2 936
	lw	$16, 0($3)
	b	$124
$121:
	.loc	2 937
 # 937	        else if(node==D_80152020.first)next=0;
	.noalias	$20,$22
	.noalias	$20,$sp
	lw	$2, 8($20)
	bne	$17, $2, $122
	.loc	2 937
	move	$16, $0
	b	$124
$122:
	.loc	2 938
 # 938	        else {
	.loc	2 939
 # 939	            for(next=D_80152020.first;next && (*next)->next!=node;next=(*next)->next) {}
	move	$16, $2
	beq	$16, 0, $124
	lw	$24, 0($16)
	lw	$2, 0($24)
	beq	$17, $2, $124
$123:
	.loc	2 939
	.loc	2 939
	move	$16, $2
	beq	$16, 0, $124
	lw	$25, 0($16)
	lw	$2, 0($25)
	bne	$17, $2, $123
$124:
	.loc	2 941
 # 940	        }
 # 941	        if(record->track==D_8014978C && record->mode==D_80152570 &&
	lb	$14, 8($3)
	bne	$5, $14, $126
	.noalias	$21,$20
	.noalias	$21,$22
	.noalias	$21,$sp
	lb	$15, 0($21)
	lbu	$24, 9($3)
	bne	$15, $24, $126
	.noalias	$19,$20
	.noalias	$19,$21
	.noalias	$19,$22
	.noalias	$19,$sp
	lw	$25, 4($19)
	beq	$17, $25, $126
	lw	$14, 8($19)
	beq	$17, $14, $126
	lw	$15, 12($19)
	beq	$17, $15, $126
	lw	$24, 4($4)
	beq	$24, 0, $125
	move	$4, $17
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $126
	lw	$4, 0($17)
$125:
	lw	$25, 36($4)
	bne	$25, 0, $133
	.alias	$19,$20
	.alias	$19,$21
	.alias	$19,$22
	.alias	$19,$sp
	.alias	$20,$21
	.alias	$20,$22
	.alias	$20,$sp
	.alias	$21,$22
	.alias	$21,$sp
	.alias	$22,$sp
	.loc	2 943
 # 942	           node!=D_80152698[1] && node!=D_80152698[2] && node!=D_80152698[3] &&
 # 943	           (!(*node)->file || menu_item_value_get(node)) && (*node)->unknown24)break;
$126:
	.loc	2 944
 # 944	        if(!next) {
	bne	$16, 0, $132
	.loc	2 944
	.loc	2 945
 # 945	            if(node==D_803BA010) {
	.noalias	$22,$sp
	lw	$2, 0($22)
	bne	$17, $2, $128
	.loc	2 945
	.loc	2 946
 # 946	                if(direction>0)next=D_803BA010+1;
	ble	$18, 0, $127
	.loc	2 946
	addu	$16, $2, 4
	b	$132
$127:
	.loc	2 947
 # 947	                else next=D_80152020.last;
	.noalias	$20,$22
	.noalias	$20,$sp
	lw	$16, 12($20)
	b	$132
$128:
	.loc	2 948
 # 948	            } else if(node==D_803BA010+1) {
	addu	$3, $2, 4
	bne	$17, $3, $130
	.loc	2 948
	.loc	2 949
 # 949	                if(direction>0)next=D_803BA010+2;
	ble	$18, 0, $129
	.loc	2 949
	addu	$16, $2, 8
	b	$132
$129:
	.loc	2 950
 # 950	                else next=D_803BA010;
	move	$16, $2
	b	$132
$130:
	.loc	2 951
 # 951	            } else if(node==D_803BA010+2) {
	addu	$14, $2, 8
	bne	$17, $14, $131
	.loc	2 951
	.loc	2 952
 # 952	                if(direction<0)next=D_803BA010+1;
	bge	$18, 0, $132
	.loc	2 952
	move	$16, $3
	b	$132
$131:
	.loc	2 953
 # 953	            } else if(direction>0)next=D_803BA010;
	ble	$18, 0, $132
	.loc	2 953
	move	$16, $2
$132:
	.loc	2 955
 # 954	        }
 # 955	        node=next;
	move	$17, $16
	bne	$16, 0, $120
	.alias	$20,$22
	.alias	$20,$sp
	.alias	$22,$sp
$133:
	.loc	2 957
 # 956	    }
 # 957	    return node;
	move	$2, $17
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	func_8039E200
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 919
 # 919	{
	.ent	func_8039E3BC 2
func_8039E3BC:
	.option	O3
	subu	$sp, 24
	sw	$31, 20($sp)
	.mask	0x80000000, -4
	.frame	$sp, 24, $31
	.loc	2 919
	.loc	2 919
	.loc	2 921
 # 920	    s32 item;
 # 921	    D_803BA050[player]=0; for(item=0;item<19;item++) {
	.noalias	$11,$sp
	mul	$14, $12, 4
	la	$15, D_803BA050
	addu	$11, $14, $15
	sw	$0, 0($11)
	.loc	2 921
	move	$10, $0
	li	$16, 19
	la	$13, D_803BA060
$134:
	.loc	2 921
	.loc	2 922
 # 922	        if(func_8039D494(player,item))D_803BA060[player][D_803BA050[player]++]=item;
	move	$5, $12
	move	$4, $10
	.livereg	0x0C00000E,0x00000000
	jal	func_8039D494
	beq	$2, 0, $135
	.loc	2 922
	lw	$2, 0($11)
	.noalias	$13,$11
	.noalias	$13,$sp
	mul	$24, $12, 76
	addu	$25, $13, $24
	mul	$14, $2, 4
	addu	$15, $25, $14
	sw	$10, 0($15)
	addu	$24, $2, 1
	sw	$24, 0($11)
$135:
	.loc	2 921
 # 921	    D_803BA050[player]=0; for(item=0;item<19;item++) {
	addu	$10, $10, 1
	bne	$10, $16, $134
	.alias	$11,$13
	.alias	$11,$sp
	.alias	$13,$sp
	.loc	2 924
 # 922	        if(func_8039D494(player,item))D_803BA060[player][D_803BA050[player]++]=item;
 # 923	    }
 # 924	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 24
	j	$31
	.end	func_8039E3BC
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 1335
 #1335	{
	.ent	func_8039E444 2
func_8039E444:
	.option	O3
	subu	$sp, 144
	sw	$31, 28($sp)
	.mask	0x80000000, -116
	.frame	$sp, 144, $31
	.loc	2 1335
	addu	$17, $sp, 128
	.loc	2 1335
	.loc	2 1337
 #1336	    s32 i;
 #1337	    f32 lightVec[3] = {-0.57f, 0.57f, -0.57f};
	.noalias	$17,$gp
	la	$14, $$208
	.set	 noat
	lw	$1, 0($14)
	sw	$1, 0($17)
	lw	$25, 4($14)
	sw	$25, 4($17)
	lw	$1, 8($14)
	sw	$1, 8($17)
	.set	 at
	.loc	2 1345
 #1345	    if (D_8014A108 > 1 || D_8014A110 == 2) return;
	lh	$24, D_8014A108
	bge	$24, 2, $174
	li	$8, 2
	lw	$15, D_8014A110
	beq	$8, $15, $174
	.alias	$17,$gp
	.loc	2 1345
	li	$3, 1
	.loc	2 1346
 #1346	    if (D_803BA028[p] == 1 || (D_803BA060[p][D_803B28B8[p]] != 1 && D_803BA060[p][D_803B28B8[p]] != 2 && D_803BA060[p][D_803B28B8[p]] != 3 && D_803BA060[p][D_803B28B8[p]] != 4 && D_803BA060[p][D_803B28B8[p]] != 5)) {
	lb	$14, D_803BA028($22)
	beq	$3, $14, $136
	.noalias	$23,$sp
	mul	$25, $22, 76
	la	$24, D_803BA060
	addu	$23, $25, $24
	.noalias	$30,$23
	.noalias	$30,$sp
	mul	$15, $22, 4
	la	$14, D_803B28B8
	addu	$30, $15, $14
	lw	$25, 0($30)
	mul	$24, $25, 4
	addu	$15, $23, $24
	.noalias	$15,$sp
	.noalias	$15,$30
	lw	$2, 0($15)
	.alias	$15,$sp
	.alias	$15,$30
	beq	$3, $2, $139
	beq	$8, $2, $139
	beq	$2, 3, $139
	beq	$2, 4, $139
	beq	$2, 5, $139
	.alias	$23,$30
	.alias	$23,$sp
	.alias	$30,$sp
$136:
	.loc	2 1346
	.loc	2 1347
 #1347	        for (i = 0; i < 11; i++) {
	move	$2, $0
	move	$17, $0
	mul	$14, $22, 572
	la	$25, D_803B2618
	addu	$16, $14, $25
	li	$18, 572
$137:
	.loc	2 1347
	.loc	2 1348
 #1348	            if (gCarComponents[p][i].idx != -1) {
	.noalias	$16,$sp
	lh	$4, 0($16)
	beq	$4, -1, $138
	.loc	2 1348
	.loc	2 1349
 #1349	                model_data_load(gCarComponents[p][i].idx, 1, 15);
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1350
 #1350	                car_mass_set(gCarComponents[p][i].idx, -1);
	lh	$4, 0($16)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	car_mass_set
$138:
	.loc	2 1347
 #1347	        for (i = 0; i < 11; i++) {
	addu	$17, $17, 52
	addu	$16, $16, 52
	bne	$17, $18, $137
	.alias	$16,$sp
	.loc	2 1353
 #1348	            if (gCarComponents[p][i].idx != -1) {
 #1349	                model_data_load(gCarComponents[p][i].idx, 1, 15);
 #1350	                car_mass_set(gCarComponents[p][i].idx, -1);
 #1351	            }
 #1352	        }
 #1353	        return;
	b	$174
$139:
	.loc	2 1355
 #1354	    }
 #1355	    car = &D_80139320[p * 13 + D_803B9FD0[p]];
	.noalias	$21,$sp
	la	$24, D_803B9FD0
	addu	$21, $22, $24
	lb	$15, 0($21)
	mul	$14, $15, 64
	mul	$25, $22, 13
	mul	$24, $25, 64
	addu	$15, $14, $24
	la	$25, D_80139320
	addu	$19, $15, $25
	.loc	2 1356
 #1356	    if (D_803B25C8 == -1) {
	lw	$14, D_803B25C8
	bne	$14, -1, $140
	.loc	2 1356
	.loc	2 1357
 #1357	        D_803B25C8 = engine_rpm_calc(string_copy_format(D_803B85CC, 0, D_80140BDC - 1, 1), func_8008B3A0((s16)car->main), 15, -1, 0);
	la	$4, D_803B85CC
	move	$5, $0
	la	$24, D_80140BDC
	.set	 volatile
	lbu	$6, 0($24)
	.set	 novolatile
	addu	$6, $6, -1
	sll	$15, $6, 24
	move	$6, $15
	sra	$25, $6, 24
	move	$6, $25
	move	$7, $3
	.livereg	0x0F00000E,0x00000000
	jal	string_copy_format
	move	$16, $2
	lh	$4, 2($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	move	$4, $16
	move	$5, $2
	li	$6, 15
	li	$7, -1
	sw	$0, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	engine_rpm_calc
	la	$3, D_803B25C8
	.noalias	$3,$21
	.noalias	$3,$sp
	sw	$2, 0($3)
	.loc	2 1358
 #1358	        model_data_load(D_803B25C8, 1, 15);
	lw	$4, 0($3)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.alias	$3,$21
	.alias	$3,$sp
	b	$141
$140:
	.loc	2 1359
 #1359	    } else {
	.loc	2 1360
 #1360	        D_8012E708[D_803B25C8].mat = func_8008B3A0((s16)car->main);
	lh	$4, 2($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	lw	$14, D_803B25C8
	mul	$24, $14, 68
	sw	$2, D_8012E708($24)
$141:
	.loc	2 1362
 #1361	    }
 #1362	    D_803B2858[1].col[0] = 255.0f * func_800BEA30();
	.livereg	0x0000000E,0x00000000
	jal	func_800BEA30
	li.s	$f4, 255.0
	mul.s	$f6, $f0, $f4
	cfc1	$15, $31
	li	$25, 1
	ctc1	$25, $31
	cvt.w.s	$f8, $f6
	cfc1	$25, $31
	.set	 noat
	and	$1, $25, 4
	and	$25, $25, 120
	.set	 at
	beq	$25, 0, $176
	li.s	$f8, 2147483648.0
	sub.s	$f8, $f6, $f8
	li	$25, 1
	ctc1	$25, $31
	cvt.w.s	$f8, $f8
	cfc1	$25, $31
	.set	 noat
	and	$1, $25, 4
	and	$25, $25, 120
	.set	 at
	bne	$25, 0, $177
	.set	 noat
	mfc1	$25, $f8
	li	$1, -2147483648
	or	$25, $25, $1
	.set	 at
	b	$175
$177:
	li	$25, -1
	b	$175
$176:
	mfc1	$25, $f8
	blt	$25, 0, $177
$175:
	ctc1	$15, $31
	sb	$25, D_803B2858+40
	.loc	2 1363
 #1363	    func_800A61B0(lightVec, lightDir, func_8008B3A0((s16)car->main)->uvs);
	lh	$4, 2($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	move	$4, $17
	addu	$5, $sp, 116
	move	$6, $2
	.livereg	0x0E00000E,0x00000000
	jal	func_800A61B0
	li.s	$f0, 10.0
	.loc	2 1364
 #1364	    D_803B2858[0].l[0].dir[0] = lightVec[0] * 10.0f;
	l.s	$f10, 128($sp)
	mul.s	$f4, $f10, $f0
	trunc.w.s	$f6, $f4, $14
	mfc1	$2, $f6
	sb	$2, D_803B2858+16
	.loc	2 1365
 #1365	    D_803B2858[0].l[0].dir[1] = lightVec[1] * 10.0f;
	l.s	$f8, 132($sp)
	mul.s	$f10, $f8, $f0
	trunc.w.s	$f4, $f10, $24
	mfc1	$3, $f4
	sb	$3, D_803B2858+17
	.loc	2 1366
 #1366	    D_803B2858[0].l[0].dir[2] = lightVec[2] * 10.0f;
	l.s	$f6, 136($sp)
	mul.s	$f8, $f6, $f0
	trunc.w.s	$f10, $f8, $15
	mfc1	$4, $f10
	sb	$4, D_803B2858+18
	.loc	2 1367
 #1367	    D_803B2858[1].l[0].dir[0] = lightVec[0] * 10.0f;
	sb	$2, D_803B2858+56
	.loc	2 1368
 #1368	    D_803B2858[1].l[0].dir[1] = lightVec[1] * 10.0f;
	sb	$3, D_803B2858+57
	.loc	2 1369
 #1369	    D_803B2858[1].l[0].dir[2] = lightVec[2] * 10.0f;
	sb	$4, D_803B2858+58
	.loc	2 1370
 #1370	    for (i = 0; i < 11; i++) {
	sw	$0, 140($sp)
	mul	$25, $22, 572
	la	$14, D_803B2618
	addu	$16, $25, $14
	la	$24, D_803B2600
	sw	$24, 48($sp)
	li	$15, 1
	sll	$25, $15, $22
	sw	$25, 40($sp)
	li.s	$f30, 0.
	li.s	$f28, 15.0
	li.s	$f26, 0.0
	li.s	$f24, 0.1
	li.s	$f22, 1.0
$142:
	.loc	2 1370
	.loc	2 1371
 #1371	        currentLevel = -1;
	li	$18, -1
	.loc	2 1372
 #1372	        whichLight = 0;
	move	$20, $0
	.loc	2 1373
 #1373	        compScale = 1.0f;
	mov.s	$f20, $f22
	.loc	2 1374
 #1374	        switch (D_803B2600[i]) {
	lw	$14, 48($sp)
	lhu	$24, 0($14)
	bgeu	$24, 5, $155
	sll	$24, $24, 2
	lw	$24, $143($24)
	j	$24
	.rdata	
$143:
	.word	$145
	.word	$147
	.word	$149
	.word	$151
	.word	$153
	.text	
$145:
	.loc	2 1376
 #1375	        case 0:
 #1376	            if (D_803BA060[p][D_803B28B8[p]] == 3) {
	mul	$15, $22, 13
	la	$25, D_80111080
	addu	$17, $15, $25
	.noalias	$23,$21
	.noalias	$23,$sp
	.noalias	$30,$21
	.noalias	$30,$23
	.noalias	$30,$sp
	lw	$14, 0($30)
	mul	$24, $14, 4
	addu	$15, $23, $24
	.noalias	$15,$21
	.noalias	$15,$sp
	.noalias	$15,$30
	lw	$25, 0($15)
	.alias	$15,$21
	.alias	$15,$sp
	.alias	$15,$30
	bne	$25, 3, $146
	.loc	2 1376
	.loc	2 1377
 #1377	                whichLight = 1;
	li	$20, 1
	.loc	2 1378
 #1378	                compScale = 0.1f * func_800BEA30() + 1.0f;
	.livereg	0x0000000E,0x00000000
	jal	func_800BEA30
	mul.s	$f4, $f0, $f24
	add.s	$f20, $f4, $f22
$146:
	.loc	2 1380
 #1379	            }
 #1380	            currentLevel = D_80111080[p+1][D_803B9FD0[p]] % 5;
	.noalias	$17,$21
	.noalias	$17,$23
	.noalias	$17,$30
	.noalias	$17,$sp
	lb	$14, 0($21)
	addu	$24, $17, $14
	.noalias	$24,$21
	.noalias	$24,$23
	.noalias	$24,$sp
	.noalias	$24,$30
	lb	$18, 13($24)
	.alias	$24,$21
	.alias	$24,$23
	.alias	$24,$sp
	.alias	$24,$30
	rem	$15, $18, 5
	move	$18, $15
	sll	$25, $18, 16
	move	$18, $25
	sra	$14, $18, 16
	move	$18, $14
	.loc	2 1381
 #1381	            break;
	b	$155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$147:
	.loc	2 1383
 #1382	        case 1:
 #1383	            if (D_803BA060[p][D_803B28B8[p]] == 5) {
	mul	$24, $22, 13
	la	$15, D_80111230
	addu	$17, $24, $15
	lw	$25, 0($30)
	mul	$14, $25, 4
	addu	$24, $23, $14
	.noalias	$24,$21
	.noalias	$24,$sp
	.noalias	$24,$30
	lw	$15, 0($24)
	.alias	$24,$21
	.alias	$24,$sp
	.alias	$24,$30
	bne	$15, 5, $148
	.loc	2 1383
	.loc	2 1384
 #1384	                whichLight = 1;
	li	$20, 1
	.loc	2 1385
 #1385	                compScale = 0.1f * func_800BEA30() + 1.0f;
	.livereg	0x0000000E,0x00000000
	jal	func_800BEA30
	mul.s	$f6, $f0, $f24
	add.s	$f20, $f6, $f22
$148:
	.loc	2 1387
 #1386	            }
 #1387	            currentLevel = D_80111230[p+1][D_803B9FD0[p]] % 3;
	.noalias	$17,$21
	.noalias	$17,$23
	.noalias	$17,$30
	.noalias	$17,$sp
	lb	$25, 0($21)
	addu	$14, $17, $25
	.noalias	$14,$21
	.noalias	$14,$23
	.noalias	$14,$sp
	.noalias	$14,$30
	lb	$18, 13($14)
	.alias	$14,$21
	.alias	$14,$23
	.alias	$14,$sp
	.alias	$14,$30
	rem	$24, $18, 3
	move	$18, $24
	sll	$15, $18, 16
	move	$18, $15
	sra	$25, $18, 16
	move	$18, $25
	.loc	2 1388
 #1388	            break;
	b	$155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$149:
	.loc	2 1390
 #1389	        case 2:
 #1390	            if (D_803BA060[p][D_803B28B8[p]] == 2) {
	mul	$14, $22, 13
	la	$24, D_8011103C
	addu	$17, $14, $24
	lw	$15, 0($30)
	mul	$25, $15, 4
	addu	$14, $23, $25
	.noalias	$14,$21
	.noalias	$14,$sp
	.noalias	$14,$30
	lw	$24, 0($14)
	.alias	$14,$21
	.alias	$14,$sp
	.alias	$14,$30
	bne	$24, 2, $150
	.loc	2 1390
	.loc	2 1391
 #1391	                whichLight = 1;
	li	$20, 1
	.loc	2 1392
 #1392	                compScale = 0.1f * func_800BEA30() + 1.0f;
	.livereg	0x0000000E,0x00000000
	jal	func_800BEA30
	mul.s	$f8, $f0, $f24
	add.s	$f20, $f8, $f22
$150:
	.loc	2 1394
 #1393	            }
 #1394	            currentLevel = D_8011103C[p+1][D_803B9FD0[p]] % 3;
	.noalias	$17,$21
	.noalias	$17,$23
	.noalias	$17,$30
	.noalias	$17,$sp
	lb	$15, 0($21)
	addu	$25, $17, $15
	.noalias	$25,$21
	.noalias	$25,$23
	.noalias	$25,$sp
	.noalias	$25,$30
	lb	$18, 13($25)
	.alias	$25,$21
	.alias	$25,$23
	.alias	$25,$sp
	.alias	$25,$30
	rem	$14, $18, 3
	move	$18, $14
	sll	$24, $18, 16
	move	$18, $24
	sra	$15, $18, 16
	move	$18, $15
	.loc	2 1395
 #1395	            break;
	b	$155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$151:
	.loc	2 1397
 #1396	        case 3:
 #1397	            if (D_803BA060[p][D_803B28B8[p]] == 4) {
	mul	$25, $22, 13
	la	$14, D_8011119C
	addu	$17, $25, $14
	lw	$24, 0($30)
	mul	$15, $24, 4
	addu	$25, $23, $15
	.noalias	$25,$21
	.noalias	$25,$sp
	.noalias	$25,$30
	lw	$14, 0($25)
	.alias	$25,$21
	.alias	$25,$sp
	.alias	$25,$30
	bne	$14, 4, $152
	.loc	2 1397
	.loc	2 1398
 #1398	                whichLight = 1;
	li	$20, 1
	.loc	2 1399
 #1399	                compScale = 0.1f * func_800BEA30() + 1.0f;
	.livereg	0x0000000E,0x00000000
	jal	func_800BEA30
	mul.s	$f10, $f0, $f24
	add.s	$f20, $f10, $f22
$152:
	.loc	2 1401
 #1400	            }
 #1401	            currentLevel = D_8011119C[p+1][D_803B9FD0[p]] % 4;
	.noalias	$17,$21
	.noalias	$17,$23
	.noalias	$17,$30
	.noalias	$17,$sp
	lb	$24, 0($21)
	addu	$15, $17, $24
	.noalias	$15,$21
	.noalias	$15,$23
	.noalias	$15,$sp
	.noalias	$15,$30
	lb	$18, 13($15)
	.alias	$15,$21
	.alias	$15,$23
	.alias	$15,$sp
	.alias	$15,$30
	rem	$25, $18, 4
	move	$18, $25
	sll	$14, $18, 16
	move	$18, $14
	sra	$24, $18, 16
	move	$18, $24
	.loc	2 1402
 #1402	            break;
	b	$155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$153:
	.loc	2 1404
 #1403	        case 4:
 #1404	            if (D_803BA060[p][D_803B28B8[p]] == 1) {
	mul	$15, $22, 13
	la	$25, D_80110E78
	addu	$17, $15, $25
	lw	$14, 0($30)
	mul	$24, $14, 4
	addu	$15, $23, $24
	.noalias	$15,$21
	.noalias	$15,$sp
	.noalias	$15,$30
	lw	$25, 0($15)
	.alias	$15,$21
	.alias	$15,$sp
	.alias	$15,$30
	bne	$25, 1, $154
	.loc	2 1404
	.loc	2 1405
 #1405	                whichLight = 1;
	li	$20, 1
	.loc	2 1406
 #1406	                compScale = 0.1f * func_800BEA30() + 1.0f;
	.livereg	0x0000000E,0x00000000
	jal	func_800BEA30
	mul.s	$f4, $f0, $f24
	add.s	$f20, $f4, $f22
$154:
	.loc	2 1408
 #1407	            }
 #1408	            currentLevel = D_80110E78[p+1][D_803B9FD0[p]] % 2;
	.noalias	$17,$21
	.noalias	$17,$23
	.noalias	$17,$30
	.noalias	$17,$sp
	lb	$14, 0($21)
	addu	$24, $17, $14
	.noalias	$24,$21
	.noalias	$24,$23
	.noalias	$24,$sp
	.noalias	$24,$30
	lb	$18, 13($24)
	.alias	$24,$21
	.alias	$24,$23
	.alias	$24,$sp
	.alias	$24,$30
	rem	$15, $18, 2
	move	$18, $15
	sll	$25, $18, 16
	move	$18, $25
	sra	$14, $18, 16
	move	$18, $14
	.loc	2 1409
 #1409	            break;
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$155:
	.loc	2 1411
 #1410	        }
 #1411	        if (currentLevel != gCarComponents[p][i].level) {
	.noalias	$16,$21
	.noalias	$16,$23
	.noalias	$16,$30
	.noalias	$16,$sp
	lh	$24, 2($16)
	beq	$18, $24, $159
	.loc	2 1411
	.loc	2 1412
 #1412	            sprintf(componentName, D_803B25EC[D_803B2600[i]], currentLevel + 1);
	addu	$4, $sp, 84
	lw	$15, 48($sp)
	lh	$25, 0($15)
	mul	$14, $25, 4
	lw	$5, D_803B25EC($14)
	addu	$6, $18, 1
	addu	$17, $16, 4
	.livereg	0x0E00400E,0x00000000
	jal	sprintf
	.loc	2 1413
 #1413	            if (gCarComponents[p][i].idx != -1) {
	lh	$4, 0($16)
	beq	$4, -1, $156
	.loc	2 1413
	.loc	2 1414
 #1414	                sound_call_minimal(gCarComponents[p][i].idx);
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 1415
 #1415	                gCarComponents[p][i].idx = -1;
	li	$24, -1
	sh	$24, 0($16)
	.loc	2 1416
 #1416	                gCarComponents[p][i].level = -1;
	li	$15, -1
	sh	$15, 2($16)
$156:
	.loc	2 1418
 #1417	            }
 #1418	            gCarComponents[p][i].idx = sign_extend_call(string_copy_format(componentName, 0, D_80140BDC - 1, 1), gCarComponents[p][i].uvs, -1, 0);
	addu	$4, $sp, 84
	move	$5, $0
	la	$25, D_80140BDC
	.set	 volatile
	lbu	$6, 0($25)
	.set	 novolatile
	addu	$6, $6, -1
	sll	$14, $6, 24
	move	$6, $14
	sra	$24, $6, 24
	move	$6, $24
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	string_copy_format
	move	$4, $2
	move	$5, $17
	li	$6, -1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	sign_extend_call
	sh	$2, 0($16)
	.loc	2 1419
 #1419	            model_data_load(gCarComponents[p][i].idx, 1, 15);
	lh	$4, 0($16)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1420
 #1420	            if (D_80151AD0 == 1) {
	lh	$15, D_80151AD0
	bne	$15, 1, $157
	.loc	2 1420
	.loc	2 1421
 #1421	                gCarComponents[p][i].pos[0] = 0.0f;
	s.s	$f26, 40($16)
	.loc	2 1422
 #1422	                gCarComponents[p][i].pos[1] = 1.0f;
	s.s	$f22, 44($16)
	.loc	2 1423
 #1423	                gCarComponents[p][i].pos[2] = 15.0f;
	s.s	$f28, 48($16)
	b	$158
$157:
	.loc	2 1424
 #1424	            } else {
	.loc	2 1425
 #1425	                gCarComponents[p][i].pos[0] = 0.0f;
	s.s	$f26, 40($16)
	.loc	2 1426
 #1426	                gCarComponents[p][i].pos[1] = 2.0f;
	li.s	$f6, 2.0
	s.s	$f6, 44($16)
	.loc	2 1427
 #1427	                gCarComponents[p][i].pos[2] = 15.0f;
	s.s	$f28, 48($16)
$158:
	.loc	2 1429
 #1428	            }
 #1429	            gCarComponents[p][i].level = currentLevel;
	sh	$18, 2($16)
$159:
	.loc	2 1431
 #1430	        }
 #1431	        switch (i) {
	lw	$25, 140($sp)
	bgeu	$25, 11, $173
	sll	$25, $25, 2
	lw	$25, $160($25)
	j	$25
	.rdata	
$160:
	.word	$162
	.word	$163
	.word	$165
	.word	$167
	.word	$171
	.word	$169
	.word	$166
	.word	$168
	.word	$172
	.word	$170
	.word	$164
	.text	
$162:
	.loc	2 1432
 #1432	        case 0: case 1: case 10:
$163:
	.loc	2 1432
$164:
	.loc	2 1433
 #1433	            func_8008B32C(D_8011418C, gCarComponents[p][i].uvs, compScale);
	la	$4, D_8011418C
	addu	$5, $16, 4
	mfc1	$6, $f20
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1434
 #1434	            gCarComponents[p][i].pos[0] = 0.f;
	s.s	$f30, 40($16)
	.loc	2 1435
 #1435	            gCarComponents[p][i].pos[1] = 0.f;
	.loc	2 1436
 #1436	            gCarComponents[p][i].pos[2] = 0.f;
	s.s	$f30, 48($16)
	.loc	2 1437
 #1437	            gCarComponents[p][i].pos[1] += 0.1f;
	s.s	$f24, 44($16)
	.loc	2 1438
 #1438	            break;
	b	$173
$165:
	.loc	2 1439
 #1439	        case 2: case 6:
$166:
	.loc	2 1440
 #1440	            func_8008B32C(func_8008B3A0((s16)car->parts[3])->uvs, gCarComponents[p][i].uvs, compScale);
	lh	$4, 30($19)
	addu	$17, $16, 4
	.livereg	0x0800400E,0x00000000
	jal	func_8008B3A0
	move	$4, $2
	move	$5, $17
	mfc1	$6, $f20
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1441
 #1441	            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[3])->pos[0];
	lh	$4, 30($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f8, 36($2)
	s.s	$f8, 40($16)
	.loc	2 1442
 #1442	            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[3])->pos[1];
	lh	$4, 30($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f10, 40($2)
	s.s	$f10, 44($16)
	.loc	2 1443
 #1443	            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[3])->pos[2];
	lh	$4, 30($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f4, 44($2)
	s.s	$f4, 48($16)
	.loc	2 1444
 #1444	            func_80090E9C(3.1415927f, gCarComponents[p][i].uvs);
	li.s	$f12, 3.1415927
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_80090E9C
	.loc	2 1445
 #1445	            break;
	b	$173
$167:
	.loc	2 1446
 #1446	        case 3: case 7:
$168:
	.loc	2 1447
 #1447	            func_8008B32C(func_8008B3A0((s16)car->parts[4])->uvs, gCarComponents[p][i].uvs, compScale);
	lh	$4, 34($19)
	addu	$17, $16, 4
	.livereg	0x0800400E,0x00000000
	jal	func_8008B3A0
	move	$4, $2
	move	$5, $17
	mfc1	$6, $f20
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1448
 #1448	            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[4])->pos[0];
	lh	$4, 34($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f6, 36($2)
	s.s	$f6, 40($16)
	.loc	2 1449
 #1449	            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[4])->pos[1];
	lh	$4, 34($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f8, 40($2)
	s.s	$f8, 44($16)
	.loc	2 1450
 #1450	            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[4])->pos[2];
	lh	$4, 34($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f10, 44($2)
	s.s	$f10, 48($16)
	.loc	2 1451
 #1451	            break;
	b	$173
$169:
	.loc	2 1452
 #1452	        case 5: case 9:
$170:
	.loc	2 1453
 #1453	            func_8008B32C(func_8008B3A0((s16)car->parts[5])->uvs, gCarComponents[p][i].uvs, compScale);
	lh	$4, 38($19)
	addu	$17, $16, 4
	.livereg	0x0800400E,0x00000000
	jal	func_8008B3A0
	move	$4, $2
	move	$5, $17
	mfc1	$6, $f20
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1454
 #1454	            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[5])->pos[0];
	lh	$4, 38($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f4, 36($2)
	s.s	$f4, 40($16)
	.loc	2 1455
 #1455	            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[5])->pos[1];
	lh	$4, 38($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f6, 40($2)
	s.s	$f6, 44($16)
	.loc	2 1456
 #1456	            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[5])->pos[2];
	lh	$4, 38($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f8, 44($2)
	s.s	$f8, 48($16)
	.loc	2 1457
 #1457	            func_80090E9C(3.1415927f, gCarComponents[p][i].uvs);
	li.s	$f12, 3.1415927
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_80090E9C
	.loc	2 1458
 #1458	            break;
	b	$173
$171:
	.loc	2 1459
 #1459	        case 4: case 8:
$172:
	.loc	2 1460
 #1460	            func_8008B32C(func_8008B3A0((s16)car->parts[6])->uvs, gCarComponents[p][i].uvs, compScale);
	lh	$4, 42($19)
	addu	$17, $16, 4
	.livereg	0x0800400E,0x00000000
	jal	func_8008B3A0
	move	$4, $2
	move	$5, $17
	mfc1	$6, $f20
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1461
 #1461	            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[6])->pos[0];
	lh	$4, 42($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f10, 36($2)
	s.s	$f10, 40($16)
	.loc	2 1462
 #1462	            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[6])->pos[1];
	lh	$4, 42($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f4, 40($2)
	s.s	$f4, 44($16)
	.loc	2 1463
 #1463	            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[6])->pos[2];
	lh	$4, 42($19)
	.livereg	0x0800000E,0x00000000
	jal	func_8008B3A0
	l.s	$f6, 44($2)
	s.s	$f6, 48($16)
	.loc	2 1464
 #1464	            break;
$173:
	.loc	2 1466
 #1465	        }
 #1466	        car_mass_set(gCarComponents[p][i].idx, (s16)D_803B25C8);
	lh	$4, 0($16)
	lh	$5, D_803B25C8+2
	.livereg	0x0C00000E,0x00000000
	jal	car_mass_set
	.loc	2 1467
 #1467	        mask = 1 << p;
	.loc	2 1468
 #1468	        model_transform_setup(gCarComponents[p][i].idx, 0, mask);
	lh	$4, 0($16)
	move	$5, $0
	lw	$6, 40($sp)
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
	.loc	2 1469
 #1469	        func_800A79B8(gCarComponents[p][i].idx, 1, &D_803B2858[whichLight]);
	lh	$4, 0($16)
	li	$5, 1
	mul	$14, $20, 40
	la	$24, D_803B2858
	addu	$6, $14, $24
	.livereg	0x0E00000E,0x00000000
	jal	func_800A79B8
	.loc	2 1370
 #1370	    for (i = 0; i < 11; i++) {
	lw	$15, 140($sp)
	addu	$25, $15, 1
	sw	$25, 140($sp)
	addu	$16, $16, 52
	lw	$14, 48($sp)
	addu	$24, $14, 2
	sw	$24, 48($sp)
	bne	$25, 11, $142
	.alias	$16,$21
	.alias	$16,$23
	.alias	$16,$30
	.alias	$16,$sp
	.alias	$21,$23
	.alias	$21,$30
	.alias	$21,$sp
	.alias	$23,$30
	.alias	$23,$sp
	.alias	$30,$sp
	.loc	2 1471
 #1471	}
$174:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 144
	j	$31
	.end	func_8039E444
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 1260
 #1260	{
	.ent	func_8039ED20 2
func_8039ED20:
	.option	O3
	subu	$sp, 40
	sw	$31, 20($sp)
	.mask	0x80000000, -20
	.frame	$sp, 40, $31
	.loc	2 1260
	.loc	2 1260
	.loc	2 1261
 #1261	    if(enabled==1) {
	bne	$4, 1, $178
	la	$16, D_80139320
	la	$17, D_803B9FD0
	addu	$18, $sp, 36
	.loc	2 1261
	.loc	2 1262
 #1262	        Color4 color=D_803B3054;
	.noalias	$18,$gp
	la	$14, D_803B3054
	.set	 noat
	.set	 noat
	lw	$1, 0($14)
	sw	$1, 0($18)
	.set	 at
	.loc	2 1263
 #1263	        if(D_80139320[D_803B9FD0[0]].main) {
	.noalias	$16,$18
	.noalias	$16,$sp
	.noalias	$17,$16
	.noalias	$17,$18
	.noalias	$17,$sp
	lb	$25, 0($17)
	mul	$15, $25, 64
	addu	$14, $16, $15
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	lw	$2, 0($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	beq	$2, 0, $179
	.loc	2 1263
	.loc	2 1264
 #1264	            func_8008E06C(D_80139320[D_803B9FD0[0]].main,&color);
	sll	$4, $2, 16
	sra	$24, $4, 16
	move	$4, $24
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1265
 #1265	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[0],&color);
	lb	$25, 0($17)
	mul	$15, $25, 64
	addu	$14, $16, $15
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	lh	$4, 18($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1266
 #1266	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[1],&color);
	lb	$24, 0($17)
	mul	$25, $24, 64
	addu	$15, $16, $25
	.noalias	$15,$17
	.noalias	$15,$18
	.noalias	$15,$sp
	lh	$4, 22($15)
	.alias	$15,$17
	.alias	$15,$18
	.alias	$15,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1267
 #1267	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[3],&color);
	lb	$14, 0($17)
	mul	$24, $14, 64
	addu	$25, $16, $24
	.noalias	$25,$17
	.noalias	$25,$18
	.noalias	$25,$sp
	lh	$4, 30($25)
	.alias	$25,$17
	.alias	$25,$18
	.alias	$25,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1268
 #1268	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[4],&color);
	lb	$15, 0($17)
	mul	$14, $15, 64
	addu	$24, $16, $14
	.noalias	$24,$17
	.noalias	$24,$18
	.noalias	$24,$sp
	lh	$4, 34($24)
	.alias	$24,$17
	.alias	$24,$18
	.alias	$24,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1269
 #1269	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[5],&color);
	lb	$25, 0($17)
	mul	$15, $25, 64
	addu	$14, $16, $15
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	lh	$4, 38($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1270
 #1270	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[6],&color);
	lb	$24, 0($17)
	mul	$25, $24, 64
	addu	$15, $16, $25
	.noalias	$15,$17
	.noalias	$15,$18
	.noalias	$15,$sp
	lh	$4, 42($15)
	.alias	$15,$17
	.alias	$15,$18
	.alias	$15,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.alias	$16,$17
	.alias	$16,$18
	.alias	$16,$sp
	.alias	$17,$18
	.alias	$17,$sp
	.alias	$18,$gp
	b	$179
$178:
	la	$16, D_80139320
	la	$17, D_803B9FD0
	addu	$18, $sp, 32
	.loc	2 1272
 #1271	        }
 #1272	    } else {
	.loc	2 1273
 #1273	        Color4 color=D_803B3058;
	.noalias	$18,$gp
	la	$14, D_803B3058
	.set	 noat
	.set	 noat
	lw	$1, 0($14)
	sw	$1, 0($18)
	.set	 at
	.loc	2 1274
 #1274	        if(D_80139320[D_803B9FD0[0]].main) {
	.noalias	$16,$18
	.noalias	$16,$sp
	.noalias	$17,$16
	.noalias	$17,$18
	.noalias	$17,$sp
	lb	$15, 0($17)
	mul	$24, $15, 64
	addu	$14, $16, $24
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	lw	$2, 0($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	beq	$2, 0, $179
	.loc	2 1274
	.loc	2 1275
 #1275	            func_8008E06C(D_80139320[D_803B9FD0[0]].main,&color);
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1276
 #1276	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[0],&color);
	lb	$15, 0($17)
	mul	$24, $15, 64
	addu	$14, $16, $24
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	lh	$4, 18($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1277
 #1277	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[1],&color);
	lb	$25, 0($17)
	mul	$15, $25, 64
	addu	$24, $16, $15
	.noalias	$24,$17
	.noalias	$24,$18
	.noalias	$24,$sp
	lh	$4, 22($24)
	.alias	$24,$17
	.alias	$24,$18
	.alias	$24,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1278
 #1278	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[3],&color);
	lb	$14, 0($17)
	mul	$25, $14, 64
	addu	$15, $16, $25
	.noalias	$15,$17
	.noalias	$15,$18
	.noalias	$15,$sp
	lh	$4, 30($15)
	.alias	$15,$17
	.alias	$15,$18
	.alias	$15,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1279
 #1279	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[4],&color);
	lb	$24, 0($17)
	mul	$14, $24, 64
	addu	$25, $16, $14
	.noalias	$25,$17
	.noalias	$25,$18
	.noalias	$25,$sp
	lh	$4, 34($25)
	.alias	$25,$17
	.alias	$25,$18
	.alias	$25,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1280
 #1280	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[5],&color);
	lb	$15, 0($17)
	mul	$24, $15, 64
	addu	$14, $16, $24
	.noalias	$14,$17
	.noalias	$14,$18
	.noalias	$14,$sp
	lh	$4, 38($14)
	.alias	$14,$17
	.alias	$14,$18
	.alias	$14,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1281
 #1281	            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[6],&color);
	lb	$25, 0($17)
	mul	$15, $25, 64
	addu	$24, $16, $15
	.noalias	$24,$17
	.noalias	$24,$18
	.noalias	$24,$sp
	lh	$4, 42($24)
	.alias	$24,$17
	.alias	$24,$18
	.alias	$24,$sp
	move	$5, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.alias	$16,$17
	.alias	$16,$18
	.alias	$16,$sp
	.alias	$17,$18
	.alias	$17,$sp
	.alias	$18,$gp
	.loc	2 1284
 #1282	        }
 #1283	    }
 #1284	}
$179:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 40
	j	$31
	.end	func_8039ED20
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 1511
 #1511	{
	.ent	func_8039EF00 2
func_8039EF00:
	.option	O3
	.frame	$sp, 0, $31
	.loc	2 1511
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	func_8039EF00
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 1514
 #1512	}
 #1513	void func_8039EF08(void)
 #1514	{
	.ent	func_8039EF08 2
func_8039EF08:
	.option	O3
	subu	$sp, 248
	sw	$31, 44($sp)
	.mask	0x80000000, -204
	.frame	$sp, 248, $31
	.loc	2 1514
	.loc	2 1514
	.loc	2 1526
 #1526	    for (p = 0; p < D_8014A108; p++) {
	move	$22, $0
	lh	$2, D_8014A108
	ble	$2, 0, $231
	sw	$0, 96($sp)
	la	$14, D_803BA020
	sw	$14, 92($sp)
	li.s	$f30, 0.0
	li.s	$f28, 0.75
	li.s	$f26, 100.0
	li.s	$f24, -3.1415927410125732e+00
	li.s	$f22, 3.1415927
	li.s	$f20, 6.2831855
	li	$30, 12
	la	$23, D_803B9BC8
	addu	$21, $sp, 192
	addu	$20, $sp, 212
$180:
	.loc	2 1526
	.loc	2 1527
 #1527	        for (i = 0; i < D_803BA020[p]; i++) {
	sw	$0, 208($sp)
	lw	$15, 92($sp)
	lh	$24, 0($15)
	ble	$24, 0, $228
	la	$25, D_803B9FD4
	addu	$14, $22, $25
	sw	$14, 100($sp)
	sw	$0, 88($sp)
	mul	$15, $22, 156
	addu	$19, $23, $15
	la	$24, D_803BA028
	addu	$25, $22, $24
	sw	$25, 80($sp)
	sll	$14, $22, 16
	sra	$15, $14, 16
	sw	$15, 68($sp)
	lw	$24, 96($sp)
	la	$25, D_803B28B0
	addu	$14, $24, $25
	sw	$14, 64($sp)
$181:
	.loc	2 1527
	.loc	2 1528
 #1528	            if (((volatile s8 *)D_803B9FD4)[p] == i) {
	lw	$15, 208($sp)
	lw	$24, 100($sp)
	.set	 volatile
	lb	$25, 0($24)
	.set	 novolatile
	bne	$15, $25, $182
	.loc	2 1528
	.loc	2 1529
 #1529	                D_803B9BC8[p][i].selection += 5.0f * D_8002EB94;
	.noalias	$19,$sp
	li.s	$f4, 5.0
	la	$14, D_8002EB94
	.set	 volatile
	l.s	$f6, 0($14)
	.set	 novolatile
	mul.s	$f8, $f4, $f6
	l.s	$f10, 0($19)
	add.s	$f4, $f10, $f8
	s.s	$f4, 0($19)
	.loc	2 1530
 #1530	                if (D_803B9BC8[p][i].selection > 1.0f) D_803B9BC8[p][i].selection = 1.0f;
	li.s	$f6, 1.0
	l.s	$f10, 0($19)
	c.lt.s	$f6, $f10
	bc1f	$183
	.loc	2 1530
	li.s	$f8, 1.0
	s.s	$f8, 0($19)
	b	$183
$182:
	.loc	2 1531
 #1531	            } else {
	.loc	2 1532
 #1532	                D_803B9BC8[p][i].selection -= 5.0f * D_8002EB94;
	li.s	$f4, 5.0
	la	$24, D_8002EB94
	.set	 volatile
	l.s	$f6, 0($24)
	.set	 novolatile
	mul.s	$f10, $f4, $f6
	l.s	$f8, 0($19)
	sub.s	$f4, $f8, $f10
	s.s	$f4, 0($19)
	.loc	2 1533
 #1533	                if (D_803B9BC8[p][i].selection < 0.0f) D_803B9BC8[p][i].selection = 0.0f;
	l.s	$f6, 0($19)
	c.lt.s	$f6, $f30
	bc1f	$183
	.loc	2 1533
	s.s	$f30, 0($19)
$183:
	.loc	2 1535
 #1534	            }
 #1535	            if (D_803BA028[p]) {
	lw	$15, 80($sp)
	lb	$25, 0($15)
	beq	$25, 0, $189
	.loc	2 1535
	.loc	2 1536
 #1536	                framesLeft = 10 - D_803BA030;
	lw	$14, D_803BA030
	li	$24, 10
	subu	$2, $24, $14
	.loc	2 1537
 #1537	                if (framesLeft <= 1) D_803B9BC8[p][i].angle = 3.6651914f;
	bge	$2, 2, $184
	.loc	2 1537
	li.s	$f8, 3.6651914
	s.s	$f8, 4($19)
	b	$207
$184:
	.loc	2 1538
 #1538	                else {
	.loc	2 1539
 #1539	                    D_803B9BC8[p][i].angle -= 3.6651914f;
	l.s	$f10, 4($19)
	li.s	$f4, 3.6651914
	sub.s	$f6, $f10, $f4
	s.s	$f6, 4($19)
	.loc	2 1540
 #1540	                    while (D_803B9BC8[p][i].angle > 3.1415927f) D_803B9BC8[p][i].angle -= 6.2831855f;
	l.s	$f0, 4($19)
	c.lt.s	$f22, $f0
	bc1f	$186
$185:
	.loc	2 1540
	sub.s	$f8, $f0, $f20
	s.s	$f8, 4($19)
	l.s	$f0, 4($19)
	c.lt.s	$f22, $f0
	bc1t	$185
$186:
	.loc	2 1541
 #1541	                    while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
	c.le.s	$f0, $f24
	bc1f	$188
$187:
	.loc	2 1541
	add.s	$f10, $f0, $f20
	s.s	$f10, 4($19)
	l.s	$f0, 4($19)
	c.le.s	$f0, $f24
	bc1t	$187
$188:
	.loc	2 1542
 #1542	                    D_803B9BC8[p][i].angle -= D_803B9BC8[p][i].angle / (framesLeft - 1);
	addu	$15, $2, -1
	mtc1	$15, $f4
	cvt.s.w	$f6, $f4
	div.s	$f8, $f0, $f6
	sub.s	$f10, $f0, $f8
	s.s	$f10, 4($19)
	.loc	2 1543
 #1543	                    D_803B9BC8[p][i].angle += 3.6651914f;
	l.s	$f4, 4($19)
	li.s	$f6, 3.6651914
	add.s	$f8, $f4, $f6
	s.s	$f8, 4($19)
	b	$207
$189:
	.loc	2 1545
 #1544	                }
 #1545	            } else if (D_803BA060[p][D_803B28B8[p]] == 11) {
	mul	$25, $22, 76
	mul	$24, $22, 4
	lw	$14, D_803B28B8($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lw	$14, D_803BA060($24)
	bne	$14, 11, $206
	.loc	2 1545
	.loc	2 1546
 #1546	                D_803B9BC8[p][i].angle -= 1.5707964f;
	l.s	$f10, 4($19)
	li.s	$f4, 1.5707964
	sub.s	$f6, $f10, $f4
	s.s	$f6, 4($19)
	.loc	2 1547
 #1547	                while (D_803B9BC8[p][i].angle > 3.1415927f) D_803B9BC8[p][i].angle -= 6.2831855f;
	l.s	$f0, 4($19)
	c.lt.s	$f22, $f0
	bc1f	$191
$190:
	.loc	2 1547
	sub.s	$f8, $f0, $f20
	s.s	$f8, 4($19)
	l.s	$f0, 4($19)
	c.lt.s	$f22, $f0
	bc1t	$190
$191:
	.loc	2 1548
 #1548	                while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
	c.le.s	$f0, $f24
	bc1f	$193
$192:
	.loc	2 1548
	add.s	$f10, $f0, $f20
	s.s	$f10, 4($19)
	l.s	$f0, 4($19)
	c.le.s	$f0, $f24
	bc1t	$192
$193:
	.loc	2 1549
 #1549	                if (D_803B9BC8[p][i].angle > 1.5707964f)
	li.s	$f4, 1.5707964
	c.lt.s	$f4, $f0
	bc1f	$196
	.loc	2 1550
 #1550	                    D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f - D_803B9BC8[p][i].angle);
	sub.s	$f12, $f22, $f0
	li.s	$f6, 1.0471976
	la	$25, D_8002EB94
	.set	 volatile
	l.s	$f8, 0($25)
	.set	 novolatile
	mul.s	$f10, $f6, $f8
	li.s	$f4, 4.0
	mul.s	$f6, $f10, $f4
	c.lt.s	$f6, $f12
	bc1f	$194
	li.s	$f8, 1.0471976
	.set	 volatile
	l.s	$f10, 0($25)
	.set	 novolatile
	mul.s	$f4, $f8, $f10
	li.s	$f6, 4.0
	mul.s	$f2, $f4, $f6
	b	$195
$194:
	mov.s	$f2, $f12
$195:
	add.s	$f8, $f0, $f2
	s.s	$f8, 4($19)
	b	$205
$196:
	.loc	2 1551
 #1551	                else if (D_803B9BC8[p][i].angle < -1.5707964f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f + D_803B9BC8[p][i].angle);
	li.s	$f10, -1.5707963705062866e+00
	c.lt.s	$f0, $f10
	bc1f	$199
	.loc	2 1551
	add.s	$f12, $f0, $f22
	li.s	$f4, 1.0471976
	la	$15, D_8002EB94
	.set	 volatile
	l.s	$f6, 0($15)
	.set	 novolatile
	mul.s	$f8, $f4, $f6
	li.s	$f10, 4.0
	mul.s	$f4, $f8, $f10
	c.lt.s	$f4, $f12
	bc1f	$197
	li.s	$f6, 1.0471976
	.set	 volatile
	l.s	$f8, 0($15)
	.set	 novolatile
	mul.s	$f10, $f6, $f8
	li.s	$f4, 4.0
	mul.s	$f2, $f10, $f4
	b	$198
$197:
	mov.s	$f2, $f12
$198:
	sub.s	$f6, $f0, $f2
	s.s	$f6, 4($19)
	b	$205
$199:
	.loc	2 1552
 #1552	                else if (D_803B9BC8[p][i].angle > 0.f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, D_803B9BC8[p][i].angle);
	li.s	$f8, 0.
	c.lt.s	$f8, $f0
	bc1f	$202
	.loc	2 1552
	li.s	$f10, 1.0471976
	la	$24, D_8002EB94
	.set	 volatile
	l.s	$f4, 0($24)
	.set	 novolatile
	mul.s	$f6, $f10, $f4
	li.s	$f8, 4.0
	mul.s	$f10, $f6, $f8
	c.lt.s	$f10, $f0
	bc1f	$200
	li.s	$f4, 1.0471976
	.set	 volatile
	l.s	$f6, 0($24)
	.set	 novolatile
	mul.s	$f8, $f4, $f6
	li.s	$f10, 4.0
	mul.s	$f2, $f8, $f10
	b	$201
$200:
	mov.s	$f2, $f0
$201:
	sub.s	$f4, $f0, $f2
	s.s	$f4, 4($19)
	b	$205
$202:
	.loc	2 1553
 #1553	                else D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, -D_803B9BC8[p][i].angle);
	neg.s	$f12, $f0
	li.s	$f6, 1.0471976
	la	$14, D_8002EB94
	.set	 volatile
	l.s	$f8, 0($14)
	.set	 novolatile
	mul.s	$f10, $f6, $f8
	li.s	$f4, 4.0
	mul.s	$f6, $f10, $f4
	c.lt.s	$f6, $f12
	bc1f	$203
	li.s	$f8, 1.0471976
	.set	 volatile
	l.s	$f10, 0($14)
	.set	 novolatile
	mul.s	$f4, $f8, $f10
	li.s	$f6, 4.0
	mul.s	$f2, $f4, $f6
	b	$204
$203:
	mov.s	$f2, $f12
$204:
	add.s	$f8, $f0, $f2
	s.s	$f8, 4($19)
$205:
	.loc	2 1554
 #1554	                D_803B9BC8[p][i].angle += 1.5707964f;
	l.s	$f10, 4($19)
	li.s	$f4, 1.5707964
	add.s	$f6, $f10, $f4
	s.s	$f6, 4($19)
	b	$207
$206:
	.loc	2 1555
 #1555	            } else {
	.loc	2 1556
 #1556	                D_803B9BC8[p][i].angle += 1.0471976f * D_8002EB94;
	li.s	$f8, 1.0471976
	la	$25, D_8002EB94
	.set	 volatile
	l.s	$f10, 0($25)
	.set	 novolatile
	mul.s	$f4, $f8, $f10
	l.s	$f6, 4($19)
	add.s	$f8, $f6, $f4
	s.s	$f8, 4($19)
	.loc	2 1557
 #1557	                if (D_803B9BC8[p][i].angle >= 6.2831855f) D_803B9BC8[p][i].angle -= 6.2831855f;
	l.s	$f0, 4($19)
	c.le.s	$f20, $f0
	bc1f	$207
	.loc	2 1557
	sub.s	$f10, $f0, $f20
	s.s	$f10, 4($19)
$207:
	.loc	2 1559
 #1558	            }
 #1559	            func_8008B32C(D_8011418C, uv, 2.5f);
	la	$4, D_8011418C
	move	$5, $20
	li.s	$6, 2.5
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1560
 #1560	            func_80090F44(0.34906584f, uv);
	li.s	$f12, 0.34906584
	move	$5, $20
	.livereg	0x0400000E,0x00080000
	jal	func_80090F44
	.loc	2 1561
 #1561	            func_80090E9C(D_803B9BC8[p][i].angle, uv);
	l.s	$f12, 4($19)
	move	$5, $20
	.livereg	0x0400000E,0x00080000
	jal	func_80090E9C
	.loc	2 1562
 #1562	            if (D_803B9BC8[p][i].car % 13 == 10) {
	lb	$4, 9($19)
	rem	$2, $4, 13
	bne	$2, 10, $209
	.loc	2 1562
	.loc	2 1563
 #1563	                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 3.2f; carPos[2] = 40.5f; }
	lh	$15, D_80151AD0
	bne	$15, 1, $208
	.loc	2 1563
	.loc	2 1563
	s.s	$f30, 192($sp)
	.loc	2 1563
	li.s	$f6, 3.2
	s.s	$f6, 196($sp)
	.loc	2 1563
	li.s	$f4, 40.5
	s.s	$f4, 200($sp)
	b	$211
$208:
	.loc	2 1564
 #1564	                else { carPos[0] = 0.0f; carPos[1] = 5.9f; carPos[2] = 40.5f; }
	.loc	2 1564
	s.s	$f30, 192($sp)
	.loc	2 1564
	li.s	$f8, 5.9
	s.s	$f8, 196($sp)
	.loc	2 1564
	li.s	$f10, 40.5
	s.s	$f10, 200($sp)
	b	$211
$209:
	.loc	2 1565
 #1565	            } else {
	.loc	2 1566
 #1566	                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 2.6f; carPos[2] = 40.5f; }
	lh	$24, D_80151AD0
	bne	$24, 1, $210
	.loc	2 1566
	.loc	2 1566
	s.s	$f30, 192($sp)
	.loc	2 1566
	li.s	$f6, 2.6
	s.s	$f6, 196($sp)
	.loc	2 1566
	li.s	$f4, 40.5
	s.s	$f4, 200($sp)
	b	$211
$210:
	.loc	2 1567
 #1567	                else { carPos[0] = 0.0f; carPos[1] = 5.3f; carPos[2] = 40.5f; }
	.loc	2 1567
	s.s	$f30, 192($sp)
	.loc	2 1567
	li.s	$f8, 5.3
	s.s	$f8, 196($sp)
	.loc	2 1567
	li.s	$f10, 40.5
	s.s	$f10, 200($sp)
$211:
	.loc	2 1569
 #1568	            }
 #1569	            alpha = D_803B9BC8[p][i].selection * 255.0f;
	l.s	$f6, 0($19)
	li.s	$f4, 255.0
	mul.s	$f8, $f6, $f4
	cfc1	$14, $31
	li	$17, 1
	ctc1	$17, $31
	cvt.w.s	$f10, $f8
	cfc1	$17, $31
	.set	 noat
	and	$1, $17, 4
	and	$17, $17, 120
	.set	 at
	beq	$17, 0, $233
	li.s	$f10, 2147483648.0
	sub.s	$f10, $f8, $f10
	li	$17, 1
	ctc1	$17, $31
	cvt.w.s	$f10, $f10
	cfc1	$17, $31
	.set	 noat
	and	$1, $17, 4
	and	$17, $17, 120
	.set	 at
	bne	$17, 0, $234
	.set	 noat
	mfc1	$17, $f10
	li	$1, -2147483648
	or	$17, $17, $1
	.set	 at
	b	$232
$234:
	li	$17, -1
	b	$232
$233:
	mfc1	$17, $f10
	blt	$17, 0, $234
$232:
	ctc1	$14, $31
	and	$25, $17, 255
	move	$17, $25
	.loc	2 1570
 #1570	            if (D_8014A110 == 6 && D_801164BE == 0)
	lw	$15, D_8014A110
	bne	$15, 6, $212
	lh	$24, D_801164BE
	bne	$24, 0, $212
	.loc	2 1571
 #1571	                func_80390DCC(D_803B9BC8[p][i].car, p, D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_803B28B0[p], carPos, uv, alpha);
	lh	$5, 70($sp)
	lb	$14, D_8012E67C($22)
	lbu	$6, D_80138664($14)
	and	$7, $6, 255
	sw	$6, 16($sp)
	lw	$25, 64($sp)
	lh	$15, 0($25)
	sw	$15, 20($sp)
	sw	$21, 24($sp)
	sw	$20, 28($sp)
	sw	$17, 32($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_80390DCC
	b	$213
$212:
	.loc	2 1572
 #1572	            else func_80390DCC(D_803B9BC8[p][i].car, p, D_80111604[p + 1][D_803B9BC8[p][i].car % 13], D_80111648[p + 1][D_803B9BC8[p][i].car % 13], D_8011168C[p + 1][D_803B9BC8[p][i].car % 13], D_803B28B0[p], carPos, uv, alpha);
	lh	$5, 70($sp)
	mul	$24, $22, 13
	addu	$14, $24, $2
	lbu	$6, D_80111604+13($14)
	mul	$25, $22, 13
	addu	$15, $25, $2
	lbu	$7, D_80111648+13($15)
	mul	$24, $22, 13
	addu	$14, $24, $2
	lb	$25, D_8011168C+13($14)
	sw	$25, 16($sp)
	lw	$15, 64($sp)
	lh	$24, 0($15)
	sw	$24, 20($sp)
	sw	$21, 24($sp)
	sw	$20, 28($sp)
	sw	$17, 32($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_80390DCC
$213:
	.loc	2 1573
 #1573	            if (D_8014A108 < 3 && D_8014A110 != 2) {
	lh	$14, D_8014A108
	bge	$14, 3, $219
	lw	$25, D_8014A110
	beq	$25, 2, $219
	.loc	2 1573
	.loc	2 1574
 #1574	                reflectIdx = p == 0 ? 2 : 3;
	bne	$22, 0, $214
	li	$16, 2
	b	$215
$214:
	li	$16, 3
$215:
	.loc	2 1575
 #1575	                func_8008B32C(uv, reflectUV, 1.2f);
	move	$4, $20
	addu	$5, $sp, 144
	li.s	$6, 1.2
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1576
 #1576	                func_8009EA68(3.14159265f, reflectUV);
	li.s	$f12, 3.14159265
	addu	$5, $sp, 144
	.livereg	0x0400000E,0x00080000
	jal	func_8009EA68
	li.s	$f2, 1.2
	.loc	2 1577
 #1577	                reflectPos[0] = 1.2f * carPos[0];
	l.s	$f6, 192($sp)
	mul.s	$f4, $f6, $f2
	s.s	$f4, 132($sp)
	.loc	2 1578
 #1578	                reflectPos[1] = 1.2f * carPos[1];
	l.s	$f8, 196($sp)
	mul.s	$f0, $f8, $f2
	.loc	2 1579
 #1579	                reflectPos[2] = 1.2f * carPos[2];
	l.s	$f10, 200($sp)
	mul.s	$f6, $f10, $f2
	s.s	$f6, 140($sp)
	.loc	2 1580
 #1580	                if (D_803B9BC8[p][i].car % 13 == 10) reflectPos[1] -= 4.0f;
	lb	$4, 9($19)
	rem	$15, $4, 13
	bne	$15, 10, $216
	.loc	2 1580
	li.s	$f4, 4.0
	sub.s	$f0, $f0, $f4
	s.s	$f0, 136($sp)
	b	$217
$216:
	.loc	2 1581
 #1581	                else reflectPos[1] -= 2.5f;
	li.s	$f8, 2.5
	sub.s	$f0, $f0, $f8
	s.s	$f0, 136($sp)
$217:
	.loc	2 1582
 #1582	                D_803B9BC8[reflectIdx][i].car = D_803B9BC8[p][i].car + 26;
	.noalias	$23,$sp
	.noalias	$3,$sp
	mul	$24, $16, 156
	addu	$14, $23, $24
	lw	$25, 88($sp)
	addu	$3, $14, $25
	addu	$15, $4, 26
	sb	$15, 9($3)
	.loc	2 1583
 #1583	                if (D_8014A110 == 6 && D_801164BE == 0)
	lw	$24, D_8014A110
	bne	$24, 6, $218
	lh	$14, D_801164BE
	bne	$14, 0, $218
	.loc	2 1584
 #1584	                    func_80390DCC(D_803B9BC8[reflectIdx][i].car, reflectIdx, D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_803B28B0[p], reflectPos, reflectUV, alpha / 1.5f);
	lb	$4, 9($3)
	sll	$5, $16, 16
	sra	$25, $5, 16
	move	$5, $25
	lb	$15, D_8012E67C($22)
	lbu	$6, D_80138664($15)
	and	$7, $6, 255
	sw	$6, 16($sp)
	lw	$24, 64($sp)
	lh	$14, 0($24)
	sw	$14, 20($sp)
	addu	$25, $sp, 132
	sw	$25, 24($sp)
	addu	$15, $sp, 144
	sw	$15, 28($sp)
	mtc1	$17, $f10
	cvt.s.w	$f6, $f10
	bge	$17, $0, $235
	li.s	$f4, 4294967296.0
	add.s	$f6, $f6, $f4
$235:
	li.s	$f8, 1.5
	div.s	$f10, $f6, $f8
	cfc1	$24, $31
	li	$14, 1
	ctc1	$14, $31
	cvt.w.s	$f4, $f10
	cfc1	$14, $31
	.set	 noat
	and	$1, $14, 4
	and	$14, $14, 120
	.set	 at
	beq	$14, 0, $237
	li.s	$f4, 2147483648.0
	sub.s	$f4, $f10, $f4
	li	$14, 1
	ctc1	$14, $31
	cvt.w.s	$f4, $f4
	cfc1	$14, $31
	.set	 noat
	and	$1, $14, 4
	and	$14, $14, 120
	.set	 at
	bne	$14, 0, $238
	.set	 noat
	mfc1	$14, $f4
	li	$1, -2147483648
	or	$14, $14, $1
	.set	 at
	b	$236
$238:
	li	$14, -1
	b	$236
$237:
	mfc1	$14, $f4
	blt	$14, 0, $238
$236:
	ctc1	$24, $31
	sw	$14, 32($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_80390DCC
	.alias	$3,$sp
	b	$219
$218:
	.loc	2 1585
 #1585	                else func_80390DCC(D_803B9BC8[reflectIdx][i].car, reflectIdx, D_80111604[p + 1][D_803B9BC8[p][i].car % 13], D_80111648[p + 1][D_803B9BC8[p][i].car % 13], D_8011168C[p + 1][D_803B9BC8[p][i].car % 13], D_803B28B0[p], reflectPos, reflectUV, alpha / 1.5f);
	.noalias	$3,$sp
	lb	$4, 9($3)
	sll	$5, $16, 16
	sra	$25, $5, 16
	move	$5, $25
	lb	$2, 9($19)
	rem	$15, $2, 13
	move	$2, $15
	mul	$24, $22, 13
	addu	$14, $24, $2
	lbu	$6, D_80111604+13($14)
	mul	$25, $22, 13
	addu	$15, $25, $2
	lbu	$7, D_80111648+13($15)
	mul	$24, $22, 13
	addu	$14, $24, $2
	lb	$25, D_8011168C+13($14)
	sw	$25, 16($sp)
	lw	$15, 64($sp)
	lh	$24, 0($15)
	sw	$24, 20($sp)
	addu	$14, $sp, 132
	sw	$14, 24($sp)
	addu	$25, $sp, 144
	sw	$25, 28($sp)
	mtc1	$17, $f6
	cvt.s.w	$f8, $f6
	bge	$17, $0, $239
	li.s	$f10, 4294967296.0
	add.s	$f8, $f8, $f10
$239:
	li.s	$f4, 1.5
	div.s	$f6, $f8, $f4
	cfc1	$15, $31
	li	$24, 1
	ctc1	$24, $31
	cvt.w.s	$f10, $f6
	cfc1	$24, $31
	.set	 noat
	and	$1, $24, 4
	and	$24, $24, 120
	.set	 at
	beq	$24, 0, $241
	li.s	$f10, 2147483648.0
	sub.s	$f10, $f6, $f10
	li	$24, 1
	ctc1	$24, $31
	cvt.w.s	$f10, $f10
	cfc1	$24, $31
	.set	 noat
	and	$1, $24, 4
	and	$24, $24, 120
	.set	 at
	bne	$24, 0, $242
	.set	 noat
	mfc1	$24, $f10
	li	$1, -2147483648
	or	$24, $24, $1
	.set	 at
	b	$240
$242:
	li	$24, -1
	b	$240
$241:
	mfc1	$24, $f10
	blt	$24, 0, $242
$240:
	ctc1	$15, $31
	sw	$24, 32($sp)
	.livereg	0x2F00000E,0x00000000
	jal	func_80390DCC
	.alias	$3,$sp
$219:
	lw	$2, 208($sp)
	.loc	2 1587
 #1586	            }
 #1587	            if (((volatile s8 *)D_803B9FD4)[p] == i) {
	lw	$14, 100($sp)
	.set	 volatile
	lb	$25, 0($14)
	.set	 novolatile
	bne	$2, $25, $227
	.loc	2 1587
	.loc	2 1588
 #1588	                func_8008B32C(D_8011418C, D_803B9E40[p][0], 1.3f);
	la	$4, D_8011418C
	mul	$15, $22, 96
	la	$24, D_803B9E40
	addu	$16, $15, $24
	move	$5, $16
	li.s	$6, 1.3
	addu	$17, $16, 48
	.livereg	0x0E00C00E,0x00000000
	jal	func_8008B32C
	.loc	2 1589
 #1589	                func_8008B32C(D_8011418C, D_803B9E40[p][1], 1.3f);
	la	$4, D_8011418C
	move	$5, $17
	li.s	$6, 1.3
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1590
 #1590	                func_80090F44(0.34906584f, D_803B9E40[p][0]);
	li.s	$f12, 0.34906584
	move	$5, $16
	.livereg	0x0400000E,0x00080000
	jal	func_80090F44
	.loc	2 1591
 #1591	                func_80090F44(0.34906584f, D_803B9E40[p][1]);
	li.s	$f12, 0.34906584
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_80090F44
	.loc	2 1592
 #1592	                func_80090E9C(D_803B9BC8[p][i].angle, D_803B9E40[p][0]);
	l.s	$f12, 4($19)
	move	$5, $16
	.livereg	0x0400000E,0x00080000
	jal	func_80090E9C
	.loc	2 1593
 #1593	                if (D_80151AD0 == 1) {
	lh	$14, D_80151AD0
	bne	$14, 1, $220
	li.s	$f0, 45.0
	li.s	$f2, 1.8
	.loc	2 1593
	.loc	2 1594
 #1594	                    D_803B9E40[p][0][9] = 0.0f;
	.noalias	$16,$19
	.noalias	$16,$23
	.noalias	$16,$sp
	s.s	$f30, 36($16)
	.loc	2 1595
 #1595	                    D_803B9E40[p][0][10] = 1.8f;
	s.s	$f2, 40($16)
	.loc	2 1596
 #1596	                    D_803B9E40[p][0][11] = 45.0f;
	s.s	$f0, 44($16)
	.loc	2 1597
 #1597	                    D_803B9E40[p][1][9] = 0.0f;
	s.s	$f30, 84($16)
	.loc	2 1598
 #1598	                    D_803B9E40[p][1][10] = 1.8f;
	s.s	$f2, 88($16)
	.loc	2 1599
 #1599	                    D_803B9E40[p][1][11] = 45.0f;
	s.s	$f0, 92($16)
	b	$221
$220:
	li.s	$f0, 4.8
	.loc	2 1600
 #1600	                } else {
	.loc	2 1601
 #1601	                    D_803B9E40[p][0][9] = 0.0f;
	s.s	$f30, 36($16)
	.loc	2 1602
 #1602	                    D_803B9E40[p][0][10] = 4.8f;
	s.s	$f0, 40($16)
	.loc	2 1603
 #1603	                    D_803B9E40[p][0][11] = 45.0f;
	li.s	$f8, 45.0
	s.s	$f8, 44($16)
	.loc	2 1604
 #1604	                    D_803B9E40[p][1][9] = 0.0f;
	s.s	$f30, 84($16)
	.loc	2 1605
 #1605	                    D_803B9E40[p][1][10] = 4.8f;
	s.s	$f0, 88($16)
	.loc	2 1606
 #1606	                    D_803B9E40[p][1][11] = 45.0f;
	li.s	$f4, 45.0
	s.s	$f4, 92($16)
	.alias	$16,$19
	.alias	$16,$23
	.alias	$16,$sp
$221:
	.loc	2 1608
 #1607	                }
 #1608	                if (D_8014A110 == 2) {
	lw	$25, D_8014A110
	bne	$25, 2, $227
	.loc	2 1608
	.loc	2 1609
 #1609	                    for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$16, 1
	lb	$15, D_803BA00C
	addu	$24, $15, 1
	blt	$24, 2, $227
	la	$18, D_80152698+4
	la	$17, D_803BA018+2
$222:
	.loc	2 1609
	.loc	2 1610
 #1610	                        if (!D_80152698[ghostIdx] || D_803BA018[ghostIdx] != (*(*D_80152698[ghostIdx])->active)->car) {
	.noalias	$18,$19
	.noalias	$18,$23
	.noalias	$18,$sp
	lw	$8, 0($18)
	beq	$8, 0, $223
	.noalias	$17,$18
	.noalias	$17,$19
	.noalias	$17,$23
	.noalias	$17,$sp
	lh	$14, 0($17)
	lw	$25, 0($8)
	lw	$15, 40($25)
	lw	$24, 0($15)
	lbu	$25, 8($24)
	beq	$14, $25, $224
$223:
	.loc	2 1610
	.loc	2 1611
 #1611	                            if (D_803BA018[ghostIdx] != -1) func_80390DCC(D_803B9BC8[ghostIdx][D_803BA018[ghostIdx]].car, ghostIdx, 0, 0, 0, 0, carPos, uv, 0);
	lh	$2, 0($17)
	beq	$2, -1, $224
	.loc	2 1611
	mul	$15, $16, 156
	addu	$24, $23, $15
	mul	$14, $2, $30
	addu	$25, $24, $14
	lb	$4, 9($25)
	sll	$5, $16, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $0
	move	$7, $0
	sw	$0, 16($sp)
	sw	$0, 20($sp)
	sw	$21, 24($sp)
	sw	$20, 28($sp)
	sw	$0, 32($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_80390DCC
	lw	$8, 0($18)
$224:
	.loc	2 1613
 #1612	                        }
 #1613	                        if (!D_80152698[ghostIdx]) continue;
	bne	$8, 0, $225
	.loc	2 1613
	lb	$2, D_803BA00C
	addu	$2, $2, 1
	b	$226
$225:
	.loc	2 1614
 #1614	                        ghostCar = (*(*D_80152698[ghostIdx])->active)->car;
	lw	$24, 0($8)
	lw	$14, 40($24)
	lw	$25, 0($14)
	lbu	$2, 8($25)
	.loc	2 1615
 #1615	                        D_803BA018[ghostIdx] = ghostCar;
	sh	$2, 0($17)
	.loc	2 1616
 #1616	                        D_803B9BC8[ghostIdx][ghostCar].car = ghostCar + ghostIdx * 13;
	.noalias	$9,$17
	.noalias	$9,$18
	.noalias	$9,$sp
	mul	$15, $16, 156
	addu	$24, $23, $15
	mul	$14, $2, $30
	addu	$9, $24, $14
	mul	$25, $16, 13
	addu	$15, $25, $2
	sb	$15, 9($9)
	.loc	2 1617
 #1617	                        D_8011119C[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->component;
	lw	$24, 0($8)
	lw	$14, 40($24)
	lw	$25, 0($14)
	lb	$15, 12($25)
	mul	$24, $16, 13
	addu	$14, $24, $2
	sb	$15, D_8011119C+13($14)
	.loc	2 1618
 #1618	                        D_8011157C[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->part;
	lw	$25, 0($8)
	lw	$24, 40($25)
	lw	$15, 0($24)
	lb	$14, 18($15)
	mul	$25, $16, 13
	addu	$24, $25, $2
	sb	$14, D_8011157C+13($24)
	.loc	2 1619
 #1619	                        D_801112DC[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->stat0 / 100.0f + 0.75f;
	mul	$10, $2, 4
	lw	$15, 0($8)
	lw	$25, 40($15)
	lw	$14, 0($25)
	lb	$24, 20($14)
	mtc1	$24, $f6
	cvt.s.w	$f10, $f6
	div.s	$f8, $f10, $f26
	add.s	$f4, $f8, $f28
	mul	$15, $16, 52
	addu	$25, $15, $10
	s.s	$f4, D_801112DC+52($25)
	.loc	2 1620
 #1620	                        D_801113E0[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->stat1 / 100.0f + 0.75f;
	lw	$14, 0($8)
	lw	$24, 40($14)
	lw	$15, 0($24)
	lb	$25, 21($15)
	mtc1	$25, $f6
	cvt.s.w	$f10, $f6
	div.s	$f8, $f10, $f26
	add.s	$f4, $f8, $f28
	mul	$14, $16, 52
	addu	$24, $14, $10
	s.s	$f4, D_801113E0+52($24)
	.loc	2 1621
 #1621	                        func_80390DCC(D_803B9BC8[ghostIdx][ghostCar].car, ghostIdx, (*(*D_80152698[ghostIdx])->active)->red, (*(*D_80152698[ghostIdx])->active)->green, (*(*D_80152698[ghostIdx])->active)->blue, 0, carPos, uv, 255);
	lb	$4, 9($9)
	sll	$5, $16, 16
	sra	$15, $5, 16
	move	$5, $15
	lw	$25, 0($8)
	lw	$14, 40($25)
	lw	$3, 0($14)
	lbu	$6, 15($3)
	lbu	$7, 16($3)
	lb	$24, 17($3)
	sw	$24, 16($sp)
	sw	$0, 20($sp)
	sw	$21, 24($sp)
	sw	$20, 28($sp)
	li	$15, 255
	sw	$15, 32($sp)
	.livereg	0x1F00000E,0x00000000
	jal	func_80390DCC
	.alias	$9,$17
	.alias	$9,$18
	.alias	$9,$sp
	.loc	2 1609
 #1609	                    for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	lb	$2, D_803BA00C
	addu	$2, $2, 1
$226:
	addu	$16, $16, 1
	addu	$18, $18, 4
	addu	$17, $17, 2
	blt	$16, $2, $222
	.alias	$17,$18
	.alias	$17,$19
	.alias	$17,$23
	.alias	$17,$sp
	.alias	$18,$19
	.alias	$18,$23
	.alias	$18,$sp
$227:
	lw	$2, 208($sp)
	.loc	2 1527
 #1527	        for (i = 0; i < D_803BA020[p]; i++) {
	addu	$2, $2, 1
	lw	$25, 88($sp)
	addu	$14, $25, 12
	sw	$14, 88($sp)
	addu	$19, $19, 12
	sw	$2, 208($sp)
	lw	$24, 92($sp)
	lh	$15, 0($24)
	blt	$2, $15, $181
	.alias	$19,$sp
	lh	$2, D_8014A108
$228:
	.loc	2 1626
 #1626	        if (D_8014A108 == 1 && D_8014A110 != 2) {
	bne	$2, 1, $230
	lw	$25, D_8014A110
	beq	$25, 2, $230
	.loc	2 1626
	.loc	2 1628
 #1627	            s32 show;
 #1628	            func_8039EF00();
	.loc	2 1511
 #1511	{
	.loc	2 1512
 #1512	}
	.loc	2 1629
 #1629	            show = !D_803BA028[0] && (D_803BA060[p][D_803B28B8[p]] == 1 || D_803BA060[p][D_803B28B8[p]] == 2 || D_803BA060[p][D_803B28B8[p]] == 3 || D_803BA060[p][D_803B28B8[p]] == 4 || D_803BA060[p][D_803B28B8[p]] == 5);
	lb	$4, D_803BA028
	seq	$14, $4, 0
	move	$4, $14
	beq	$4, 0, $229
	mul	$24, $22, 76
	mul	$15, $22, 4
	lw	$25, D_803B28B8($15)
	mul	$14, $25, 4
	addu	$15, $24, $14
	lw	$2, D_803BA060($15)
	seq	$4, $2, 1
	bne	$4, 0, $229
	seq	$4, $2, 2
	bne	$4, 0, $229
	seq	$4, $2, 3
	bne	$4, 0, $229
	seq	$4, $2, 4
	bne	$4, 0, $229
	seq	$4, $2, 5
$229:
	.loc	2 1630
 #1630	            func_8039ED20(show);
	.livereg	0x0800000E,0x00000000
	jal	func_8039ED20
	.loc	2 1631
 #1631	            func_8039E444(p);
	sw	$22, 204($sp)
	.livereg	0x0000020E,0x00000000
	jal	func_8039E444
	addu	$20, $sp, 212
	addu	$21, $sp, 192
	lw	$22, 204($sp)
	la	$23, D_803B9BC8
	li	$30, 12
	li.s	$f20, 6.2831855
	li.s	$f22, 3.1415927
	li.s	$f24, -3.1415927410125732e+00
	li.s	$f26, 100.0
	li.s	$f28, 0.75
	li.s	$f30, 0.0
	lh	$2, D_8014A108
$230:
	.loc	2 1526
 #1526	    for (p = 0; p < D_8014A108; p++) {
	addu	$22, $22, 1
	lw	$25, 96($sp)
	addu	$24, $25, 2
	sw	$24, 96($sp)
	lw	$14, 92($sp)
	addu	$15, $14, 2
	sw	$15, 92($sp)
	blt	$22, $2, $180
	.alias	$23,$sp
	.loc	2 1634
 #1634	}
$231:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 44($sp)
	addu	$sp, 248
	j	$31
	.end	func_8039EF08
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 1038
 #1038	{
	.ent	func_8039FF04 2
func_8039FF04:
	.option	O3
	subu	$sp, 80
	sw	$31, 28($sp)
	.mask	0x80000000, -52
	.frame	$sp, 80, $31
	.loc	2 1038
	addu	$30, $sp, 76
	.loc	2 1038
	.loc	2 1039
 #1039	    Color4 buttonColor={{255,255,255,255}};
	.noalias	$30,$gp
	la	$14, $$178
	.set	 noat
	lw	$1, 0($14)
	sw	$1, 0($30)
	.set	 at
	.loc	2 1044
 #1040	    void *tex;
 #1041	    u16 texIdx;
 #1042	    s32 buttonIdx;
 #1043	    static f32 zoom=1.0f;
 #1044	    if(D_80151AD0==1)zoom=1.0;
	lh	$24, D_80151AD0
	bne	$24, 1, $243
	.loc	2 1044
	li.s	$f4, 1.0000000000000000e+00
	s.s	$f4, zoom
$243:
	.loc	2 1045
 #1045	    for(buttonIdx=0;buttonIdx<44;buttonIdx++) {
	move	$2, $0
	move	$18, $0
	lb	$15, 83($sp)
	mul	$14, $15, 2816
	la	$25, D_803AF9A8
	addu	$16, $14, $25
	li	$23, 2816
	li	$22, 270336
	la	$21, D_803AF984
	li	$20, -1
$244:
	.loc	2 1045
	.loc	2 1046
 #1046	        if(D_803AF9A8[p][buttonIdx].handle==-1) {
	.noalias	$16,$30
	.noalias	$16,$sp
	lw	$24, 4($16)
	bne	$20, $24, $248
	.loc	2 1046
	.loc	2 1047
 #1047	            memcpy(D_803AF9A8[p][buttonIdx].matrix,D_8011418C,36);
	addu	$17, $16, 12
	move	$4, $17
	la	$5, D_8011418C
	li	$6, 36
	lb	$15, 83($sp)
	li	$14, 1
	sll	$19, $14, $15
	.livereg	0x0E00500E,0x00000000
	jal	memcpy
	.loc	2 1048
 #1048	            D_803AF9A8[p][buttonIdx].handle=sign_extend_call(
	.noalias	$21,$16
	.noalias	$21,$30
	.noalias	$21,$sp
	lw	$25, 0($16)
	mul	$24, $25, 4
	addu	$14, $21, $24
	.noalias	$14,$16
	.noalias	$14,$sp
	.noalias	$14,$30
	lw	$4, 0($14)
	.alias	$14,$16
	.alias	$14,$sp
	.alias	$14,$30
	move	$5, $0
	la	$15, D_80140BDC
	.set	 volatile
	lbu	$6, 0($15)
	.set	 novolatile
	addu	$6, $6, -1
	sll	$25, $6, 24
	move	$6, $25
	sra	$24, $6, 24
	move	$6, $24
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	string_copy_format
	move	$4, $2
	lw	$3, 0($16)
	beq	$3, 0, $245
	beq	$3, 1, $245
	bne	$3, 2, $246
$245:
	move	$7, $22
	b	$247
$246:
	move	$7, $0
$247:
	move	$5, $17
	move	$6, $20
	.livereg	0x0F00000E,0x00000000
	jal	sign_extend_call
	sw	$2, 4($16)
	.loc	2 1052
 #1049	                string_copy_format(D_803AF984[D_803AF9A8[p][buttonIdx].kind],0,D_80140BDC-1,1),
 #1050	                D_803AF9A8[p][buttonIdx].matrix,-1,
 #1051	                (D_803AF9A8[p][buttonIdx].kind==0 || D_803AF9A8[p][buttonIdx].kind==1 || D_803AF9A8[p][buttonIdx].kind==2) ? 0x42000 : 0);
 #1052	            func_8008E06C((s16)D_803AF9A8[p][buttonIdx].handle,&buttonColor);
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	move	$5, $30
	.livereg	0x0C00000E,0x00000000
	jal	func_8008E06C
	.loc	2 1053
 #1053	            model_data_load(D_803AF9A8[p][buttonIdx].handle,1,15);
	lw	$4, 4($16)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 1054
 #1054	            model_transform_setup(D_803AF9A8[p][buttonIdx].handle,0,1<<p);
	lw	$4, 4($16)
	move	$5, $0
	move	$6, $19
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
$248:
	.loc	2 1045
 #1045	    for(buttonIdx=0;buttonIdx<44;buttonIdx++) {
	addu	$18, $18, 64
	addu	$16, $16, 64
	bne	$18, $23, $244
	.alias	$16,$21
	.alias	$16,$30
	.alias	$16,$sp
	.alias	$21,$30
	.alias	$21,$sp
	.alias	$30,$gp
	addu	$18, $sp, 70
	.loc	2 1057
 #1057	    tex=func_800B24EC(D_803B8630,&texIdx,0,D_80140BDC-1,1);
	la	$4, D_803B8630
	move	$5, $18
	move	$6, $0
	la	$15, D_80140BDC
	.set	 volatile
	lbu	$7, 0($15)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$25, $7, 24
	move	$7, $25
	sra	$24, $7, 24
	move	$7, $24
	li	$14, 1
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800B24EC
	.loc	2 1058
 #1058	    func_8008D870((s16)D_803AF9A8[p][38].handle,tex,-1);
	.noalias	$16,$sp
	lb	$15, 83($sp)
	mul	$25, $15, 2816
	la	$24, D_803AF9A8
	addu	$16, $25, $24
	lh	$4, 2438($16)
	move	$5, $2
	li	$6, -1
	.livereg	0x0E00800E,0x00000000
	jal	func_8008D870
	.loc	2 1059
 #1059	    memcpy(D_803AF9A8[p][38].matrix,D_8011418C,36);
	addu	$17, $16, 2444
	move	$4, $17
	la	$5, D_8011418C
	li	$6, 36
	.livereg	0x0E00400E,0x00000000
	jal	memcpy
	li.s	$f22, -1.5707963705062866e+00
	.loc	2 1060
 #1060	    func_800B5898(-1.5707964f,D_803AF9A8[p][38].matrix);
	mov.s	$f12, $f22
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1061
 #1061	    D_803AF9A8[p][38].position[0]=0.0f;
	li.s	$f6, 0.0
	s.s	$f6, 2480($16)
	.loc	2 1062
 #1062	    D_803AF9A8[p][38].position[1]=D_80151AD0>1 ? 105.0f : 115.0f;
	lh	$14, D_80151AD0
	blt	$14, 2, $249
	li.s	$f8, 105.0
	s.s	$f8, 2484($16)
	b	$250
$249:
	li.s	$f10, 115.0
	s.s	$f10, 2484($16)
$250:
	li.s	$f28, 1.0
	.loc	2 1063
 #1063	    D_803AF9A8[p][38].position[2]=200.0f;
	.loc	2 1064
 #1064	    D_803AF9A8[p][38].position[0]=(1.0f/zoom)*D_803AF9A8[p][38].position[0];
	l.s	$f4, zoom
	div.s	$f0, $f28, $f4
	l.s	$f6, 2480($16)
	mul.s	$f8, $f6, $f0
	s.s	$f8, 2480($16)
	.loc	2 1065
 #1065	    D_803AF9A8[p][38].position[1]=(1.0f/zoom)*D_803AF9A8[p][38].position[1];
	l.s	$f10, 2484($16)
	mul.s	$f4, $f10, $f0
	s.s	$f4, 2484($16)
	.loc	2 1066
 #1066	    D_803AF9A8[p][38].position[2]=(1.0f/zoom)*D_803AF9A8[p][38].position[2];
	li.s	$f6, 200.0
	mul.s	$f8, $f6, $f0
	s.s	$f8, 2488($16)
	.loc	2 1067
 #1067	    if(D_8014A110==2)model_transform_setup(D_803AF9A8[p][38].handle,0,15);
	lw	$15, D_8014A110
	bne	$15, 2, $251
	.loc	2 1067
	lw	$4, 2436($16)
	move	$5, $0
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
$251:
	.loc	2 1068
 #1068	    tex=func_800B24EC(D_803B8640,&texIdx,0,D_80140BDC-1,1);
	la	$4, D_803B8640
	move	$5, $18
	move	$6, $0
	la	$25, D_80140BDC
	.set	 volatile
	lbu	$7, 0($25)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$24, $7, 24
	move	$7, $24
	sra	$14, $7, 24
	move	$7, $14
	li	$15, 1
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800B24EC
	.loc	2 1069
 #1069	    func_8008D870((s16)D_803AF9A8[p][39].handle,tex,-1);
	lh	$4, 2502($16)
	move	$5, $2
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
	.loc	2 1070
 #1070	    tex=func_800B24EC(D_803B8648,&texIdx,0,D_80140BDC-1,1);
	la	$4, D_803B8648
	move	$5, $18
	move	$6, $0
	la	$25, D_80140BDC
	.set	 volatile
	lbu	$7, 0($25)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$24, $7, 24
	move	$7, $24
	sra	$14, $7, 24
	move	$7, $14
	li	$15, 1
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800B24EC
	.loc	2 1071
 #1071	    func_8008D870((s16)D_803AF9A8[p][40].handle,tex,-1);
	lh	$4, 2566($16)
	move	$5, $2
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
	.loc	2 1072
 #1072	    memcpy(D_803AF9A8[p][40].matrix,D_8011418C,36);
	addu	$17, $16, 2572
	move	$4, $17
	la	$5, D_8011418C
	li	$6, 36
	.livereg	0x0E00400E,0x00000000
	jal	memcpy
	.loc	2 1073
 #1073	    func_800B5898(-1.5707964f,D_803AF9A8[p][40].matrix);
	mov.s	$f12, $f22
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1074
 #1074	    D_803AF9A8[p][40].position[0]=75.1f;
	li.s	$f10, 75.1
	s.s	$f10, 2608($16)
	.loc	2 1075
 #1075	    D_803AF9A8[p][40].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
	lh	$25, D_80151AD0
	blt	$25, 2, $252
	li.s	$f24, 50.2
	s.s	$f24, 2612($16)
	li.s	$f26, 55.2
	b	$253
$252:
	li.s	$f26, 55.2
	s.s	$f26, 2612($16)
	li.s	$f24, 50.2
$253:
	li.s	$f20, 100.0
	li.s	$f30, 0.5
	.loc	2 1076
 #1076	    D_803AF9A8[p][40].position[2]=100.0f;
	s.s	$f20, 2616($16)
	.loc	2 1077
 #1077	    func_8008B32C(D_803AF9A8[p][40].matrix,D_803AF9A8[p][40].matrix,0.5f);
	move	$4, $17
	move	$5, $17
	mfc1	$6, $f30
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1078
 #1078	    memcpy(D_803AF9A8[p][41].matrix,D_8011418C,36);
	addu	$17, $16, 2636
	move	$4, $17
	la	$5, D_8011418C
	li	$6, 36
	.livereg	0x0E00400E,0x00000000
	jal	memcpy
	.loc	2 1079
 #1079	    func_800B5898(-1.5707964f,D_803AF9A8[p][41].matrix);
	mov.s	$f12, $f22
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1080
 #1080	    func_800B5940(3.1415927f,D_803AF9A8[p][41].matrix);
	li.s	$f12, 3.1415927
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_800B5940
	.loc	2 1081
 #1081	    D_803AF9A8[p][41].position[0]=87.1f;
	li.s	$f4, 87.1
	s.s	$f4, 2672($16)
	.loc	2 1082
 #1082	    D_803AF9A8[p][41].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
	lh	$24, D_80151AD0
	blt	$24, 2, $254
	s.s	$f24, 2676($16)
	b	$255
$254:
	s.s	$f26, 2676($16)
$255:
	.loc	2 1083
 #1083	    D_803AF9A8[p][41].position[2]=100.0f;
	s.s	$f20, 2680($16)
	.loc	2 1084
 #1084	    tex=func_800B24EC(D_803B864C,&texIdx,0,D_80140BDC-1,1);
	la	$4, D_803B864C
	move	$5, $18
	move	$6, $0
	la	$14, D_80140BDC
	.set	 volatile
	lbu	$7, 0($14)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$15, $7, 24
	move	$7, $15
	sra	$25, $7, 24
	move	$7, $25
	li	$24, 1
	sw	$24, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_800B24EC
	.loc	2 1085
 #1085	    func_8008D870((s16)D_803AF9A8[p][42].handle,tex,-1);
	lh	$4, 2694($16)
	move	$5, $2
	li	$6, -1
	.livereg	0x0E00000E,0x00000000
	jal	func_8008D870
	.loc	2 1086
 #1086	    memcpy(D_803AF9A8[p][42].matrix,D_8011418C,36);
	addu	$17, $16, 2700
	move	$4, $17
	la	$5, D_8011418C
	li	$6, 36
	.livereg	0x0E00400E,0x00000000
	jal	memcpy
	.loc	2 1087
 #1087	    func_800B5898(-1.5707964f,D_803AF9A8[p][42].matrix);
	mov.s	$f12, $f22
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1088
 #1088	    D_803AF9A8[p][42].position[0]=-75.1f;
	li.s	$f6, -7.5099998474121094e+01
	s.s	$f6, 2736($16)
	.loc	2 1089
 #1089	    D_803AF9A8[p][42].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
	lh	$14, D_80151AD0
	blt	$14, 2, $256
	s.s	$f24, 2740($16)
	b	$257
$256:
	s.s	$f26, 2740($16)
$257:
	.loc	2 1090
 #1090	    D_803AF9A8[p][42].position[2]=100.0f;
	.loc	2 1091
 #1091	    D_803AF9A8[p][42].position[0]=(1.0f/zoom)*D_803AF9A8[p][42].position[0];
	l.s	$f8, zoom
	div.s	$f0, $f28, $f8
	l.s	$f10, 2736($16)
	mul.s	$f4, $f10, $f0
	s.s	$f4, 2736($16)
	.loc	2 1092
 #1092	    D_803AF9A8[p][42].position[1]=(1.0f/zoom)*D_803AF9A8[p][42].position[1];
	l.s	$f6, 2740($16)
	mul.s	$f8, $f6, $f0
	s.s	$f8, 2740($16)
	.loc	2 1093
 #1093	    D_803AF9A8[p][42].position[2]=(1.0f/zoom)*D_803AF9A8[p][42].position[2];
	mul.s	$f10, $f20, $f0
	s.s	$f10, 2744($16)
	.loc	2 1094
 #1094	    func_8008B32C(D_803AF9A8[p][42].matrix,D_803AF9A8[p][42].matrix,0.5f);
	move	$4, $17
	move	$5, $17
	mfc1	$6, $f30
	.livereg	0x0E00000E,0x00000000
	jal	func_8008B32C
	.loc	2 1095
 #1095	    memcpy(D_803AF9A8[p][43].matrix,D_8011418C,36);
	addu	$17, $16, 2764
	move	$4, $17
	la	$5, D_8011418C
	li	$6, 36
	.livereg	0x0E00400E,0x00000000
	jal	memcpy
	.loc	2 1096
 #1096	    func_800B5898(-1.5707964f,D_803AF9A8[p][43].matrix);
	mov.s	$f12, $f22
	move	$5, $17
	.livereg	0x0400000E,0x00080000
	jal	func_800B5898
	.loc	2 1097
 #1097	    D_803AF9A8[p][43].position[0]=-87.1f;
	li.s	$f4, -8.7099998474121094e+01
	s.s	$f4, 2800($16)
	.loc	2 1098
 #1098	    D_803AF9A8[p][43].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
	lh	$15, D_80151AD0
	blt	$15, 2, $258
	s.s	$f24, 2804($16)
	b	$259
$258:
	s.s	$f26, 2804($16)
$259:
	.loc	2 1099
 #1099	    D_803AF9A8[p][43].position[2]=100.0f;
	.loc	2 1100
 #1100	    D_803AF9A8[p][43].position[0]=(1.0f/zoom)*D_803AF9A8[p][43].position[0];
	l.s	$f6, zoom
	div.s	$f0, $f28, $f6
	l.s	$f8, 2800($16)
	mul.s	$f10, $f8, $f0
	s.s	$f10, 2800($16)
	.loc	2 1101
 #1101	    D_803AF9A8[p][43].position[1]=(1.0f/zoom)*D_803AF9A8[p][43].position[1];
	l.s	$f4, 2804($16)
	mul.s	$f6, $f4, $f0
	s.s	$f6, 2804($16)
	.loc	2 1102
 #1102	    D_803AF9A8[p][43].position[2]=(1.0f/zoom)*D_803AF9A8[p][43].position[2];
	mul.s	$f8, $f20, $f0
	s.s	$f8, 2808($16)
	.loc	2 1103
 #1103	    D_803B9B90[p]=1;
	lb	$2, 83($sp)
	mul	$25, $2, 4
	move	$2, $25
	li	$24, 1
	sw	$24, D_803B9B90($2)
	.loc	2 1104
 #1104	    D_803B9BA0[p]=1;
	li	$14, 1
	sw	$14, D_803B9BA0($2)
	.loc	2 1105
 #1105	    particle_velocity_set();
	.livereg	0x0000000E,0x00000000
	jal	particle_velocity_set
	.alias	$16,$sp
	.loc	2 1106
 #1106	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 80
	j	$31
	.end	func_8039FF04
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 191
 # 191	{
	.ent	func_803A0498 2
func_803A0498:
	.option	O3
	subu	$sp, 104
	sw	$31, 20($sp)
	.mask	0x80000000, -84
	.frame	$sp, 104, $31
	.loc	2 191
	.loc	2 191
	.loc	2 195
 # 192	    Record **nearGhosts[5];
 # 193	    Record **ghosth;
 # 194	    Record *ghost;
 # 195	    Record **best = 0;
	move	$22, $0
	.loc	2 201
 # 196	    Record **nextGhost;
 # 197	    Profile *player;
 # 198	    s32 ghostCount;
 # 199	    s32 i;
 # 200	    s32 j;
 # 201	    player = *D_8014A118[0].owner;
	lw	$14, D_8014A118+72
	lw	$20, 0($14)
	.loc	2 202
 # 202	    for (i = 0; i < 4; i++) D_80152698[i] = 0;
	move	$16, $0
	la	$17, D_80152698
	la	$2, D_80152698+16
$260:
	.loc	2 202
	.noalias	$17,$sp
	sw	$0, 0($17)
	.loc	2 202
	addu	$17, $17, 4
	bltu	$17, $2, $260
	.alias	$17,$sp
	.loc	2 203
 # 203	    for (i = 0; i < 5; i++) nearGhosts[i] = 0;
	move	$16, $0
	addu	$3, $sp, 84
	addu	$2, $sp, 104
$261:
	.loc	2 203
	.noalias	$3,$gp
	sw	$0, 0($3)
	.loc	2 203
	addu	$3, $3, 4
	bltu	$3, $2, $261
	.alias	$3,$gp
	.loc	2 204
 # 204	    for (i = 1; i < 4; i++) D_803BA018[i] = -1;
	li	$16, 1
	la	$2, D_803BA018+2
	la	$4, D_803BA018+8
	li	$3, -1
$262:
	.loc	2 204
	.noalias	$2,$sp
	sh	$3, 0($2)
	.loc	2 204
	addu	$2, $2, 2
	bne	$2, $4, $262
	.alias	$2,$sp
	.loc	2 205
 # 205	    if (D_8014A110 != 2) return;
	lw	$15, D_8014A110
	bne	$15, 2, $340
	.loc	2 205
	.loc	2 206
 # 206	    if (!D_801426EC) {
	lb	$24, D_801426EC
	bne	$24, 0, $318
	.loc	2 206
	.loc	2 207
 # 207	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	lw	$19, D_80152020+8
	beq	$19, 0, $266
	la	$30, D_8014978C
$263:
	.loc	2 207
	.loc	2 208
 # 208	            ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 209
 # 209	            nextGhost = ghost->next;
	lw	$21, 0($18)
	.loc	2 210
 # 210	            if (ghost->track == D_8014978C && ghost->mode == D_80152570 &&
	.noalias	$30,$sp
	lb	$25, 0($30)
	lb	$14, 8($18)
	bne	$25, $14, $265
	lb	$15, D_80152570
	lbu	$24, 9($18)
	bne	$15, $24, $265
	addu	$4, $18, 10
	addu	$5, $20, 20
	.livereg	0x0C00000E,0x00000000
	jal	func_8008AD04
	bne	$2, 0, $265
	lw	$25, 24($18)
	lw	$14, 12($20)
	bne	$25, $14, $265
	lw	$15, 28($18)
	lw	$24, 16($20)
	bne	$15, $24, $265
	.loc	2 212
 # 211	                !func_8008AD04(ghost->name, player->name) &&
 # 212	                player->id0 == ghost->id0 && player->id1 == ghost->id1) {
	.loc	2 213
 # 213	                if ((!best || ghost->time < (*best)->time) && menu_item_value_get(ghosth)) best = ghosth;
	beq	$22, 0, $264
	l.s	$f4, 32($18)
	lw	$25, 0($22)
	l.s	$f6, 32($25)
	c.lt.s	$f4, $f6
	bc1f	$265
$264:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $265
	.loc	2 213
	move	$22, $19
$265:
	.loc	2 207
 # 207	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $263
$266:
	la	$30, D_8014978C
	.loc	2 216
 # 216	        for (i = 0; i < 3; i++) {
	move	$16, $0
	li	$23, 3
$267:
	.loc	2 216
	.loc	2 217
 # 217	            ghosth = D_803BA010 + i;
	sll	$14, $16, 2
	lw	$15, D_803BA010
	addu	$19, $14, $15
	.loc	2 218
 # 218	            ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 219
 # 219	            if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570 &&
	lw	$24, 40($18)
	beq	$24, 0, $269
	lb	$25, 0($30)
	lb	$14, 8($18)
	bne	$25, $14, $269
	lb	$15, D_80152570
	lbu	$24, 9($18)
	bne	$15, $24, $269
	addu	$4, $18, 10
	addu	$5, $20, 20
	.livereg	0x0C00000E,0x00000000
	jal	func_8008AD04
	bne	$2, 0, $269
	lw	$25, 24($18)
	lw	$14, 12($20)
	bne	$25, $14, $269
	lw	$15, 28($18)
	lw	$24, 16($20)
	bne	$15, $24, $269
	.loc	2 221
 # 220	                !func_8008AD04(ghost->name, player->name) &&
 # 221	                player->id0 == ghost->id0 && player->id1 == ghost->id1) {
	.loc	2 222
 # 222	                if ((!best || ghost->time < (*best)->time) && menu_item_value_get(ghosth)) best = ghosth;
	beq	$22, 0, $268
	l.s	$f8, 32($18)
	lw	$25, 0($22)
	l.s	$f10, 32($25)
	c.lt.s	$f8, $f10
	bc1f	$269
$268:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $269
	.loc	2 222
	move	$22, $19
$269:
	.loc	2 216
 # 216	        for (i = 0; i < 3; i++) {
	addu	$16, $16, 1
	bne	$16, $23, $267
	lw	$2, D_80152020+8
	.loc	2 225
 # 225	        if (!best) {
	bne	$22, 0, $292
	.loc	2 225
	.loc	2 226
 # 226	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $2
	beq	$19, 0, $279
$270:
	.loc	2 226
	.loc	2 227
 # 227	                ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 228
 # 228	                nextGhost = ghost->next;
	lw	$21, 0($18)
	.loc	2 229
 # 229	                if (ghost->track == D_8014978C && ghost->mode == D_80152570) {
	lb	$14, 0($30)
	lb	$15, 8($18)
	bne	$14, $15, $278
	lb	$24, D_80152570
	lbu	$25, 9($18)
	bne	$24, $25, $278
	.loc	2 229
	.loc	2 230
 # 230	                    for (i = 3; i >= 1; i--) {
	li	$16, 3
	la	$17, D_80152698+12
$271:
	.loc	2 230
	.loc	2 231
 # 231	                        if ((!D_80152698[i] || ghost->time > (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $272
	lw	$14, 0($2)
	l.s	$f4, 32($14)
	l.s	$f6, 32($18)
	c.lt.s	$f4, $f6
	bc1f	$277
$272:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $277
	.loc	2 231
	.loc	2 232
 # 232	                            for (j = 1; j < i; j++)
	li	$3, 1
	blt	$16, 2, $276
	addu	$5, $16, -1
	and	$15, $5, 3
	move	$5, $15
	move	$4, $5
	beq	$4, 0, $274
	addu	$4, $5, 1
	mul	$24, $3, 4
	la	$25, D_80152698
	addu	$2, $24, $25
$273:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$14, 4($2)
	sw	$14, 0($2)
	addu	$3, $3, 1
	addu	$2, $2, 4
	bne	$4, $3, $273
	.alias	$2,$30
	.alias	$2,$sp
	beq	$3, $16, $276
$274:
	mul	$15, $3, 4
	la	$24, D_80152698
	addu	$2, $15, $24
	mul	$25, $16, 4
	addu	$4, $25, $24
$275:
	.loc	2 233
 # 233	                                D_80152698[j] = D_80152698[j+1];
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$14, 4($2)
	sw	$14, 0($2)
	.loc	2 232
 # 232	                            for (j = 1; j < i; j++)
	lw	$15, 8($2)
	sw	$15, 4($2)
	lw	$25, 12($2)
	sw	$25, 8($2)
	lw	$24, 16($2)
	sw	$24, 12($2)
	addu	$2, $2, 16
	bne	$2, $4, $275
	.alias	$2,$30
	.alias	$2,$sp
$276:
	.loc	2 234
 # 233	                                D_80152698[j] = D_80152698[j+1];
 # 234	                            D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 235
 # 235	                            break;
	b	$278
$277:
	.loc	2 230
 # 230	                    for (i = 3; i >= 1; i--) {
	addu	$16, $16, -1
	addu	$17, $17, -4
	bne	$16, 0, $271
	.alias	$17,$30
	.alias	$17,$sp
$278:
	.loc	2 226
 # 226	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $270
$279:
	.loc	2 240
 # 240	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$3, D_803BA010
	move	$19, $3
	addu	$2, $3, 12
	bgeu	$3, $2, $289
$280:
	.loc	2 240
	.loc	2 241
 # 241	                ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 242
 # 242	                if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570) {
	lw	$14, 40($18)
	beq	$14, 0, $288
	lb	$15, 0($30)
	lb	$25, 8($18)
	bne	$15, $25, $288
	lb	$24, D_80152570
	lbu	$14, 9($18)
	bne	$24, $14, $288
	.loc	2 242
	.loc	2 243
 # 243	                    for (i = 3; i >= 1; i--) {
	li	$16, 3
	la	$17, D_80152698+12
$281:
	.loc	2 243
	.loc	2 244
 # 244	                        if ((!D_80152698[i] || ghost->time > (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $282
	lw	$15, 0($2)
	l.s	$f8, 32($15)
	l.s	$f10, 32($18)
	c.lt.s	$f8, $f10
	bc1f	$287
$282:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $287
	.loc	2 244
	.loc	2 245
 # 245	                            for (j = 1; j < i; j++)
	li	$3, 1
	blt	$16, 2, $286
	addu	$5, $16, -1
	and	$25, $5, 3
	move	$5, $25
	move	$4, $5
	beq	$4, 0, $284
	addu	$4, $5, 1
	mul	$24, $3, 4
	la	$14, D_80152698
	addu	$2, $24, $14
$283:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$15, 4($2)
	sw	$15, 0($2)
	addu	$3, $3, 1
	addu	$2, $2, 4
	bne	$4, $3, $283
	.alias	$2,$30
	.alias	$2,$sp
	beq	$3, $16, $286
$284:
	mul	$25, $3, 4
	la	$24, D_80152698
	addu	$2, $25, $24
	mul	$14, $16, 4
	addu	$4, $14, $24
$285:
	.loc	2 246
 # 246	                                D_80152698[j] = D_80152698[j+1];
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$15, 4($2)
	sw	$15, 0($2)
	.loc	2 245
 # 245	                            for (j = 1; j < i; j++)
	lw	$25, 8($2)
	sw	$25, 4($2)
	lw	$14, 12($2)
	sw	$14, 8($2)
	lw	$24, 16($2)
	sw	$24, 12($2)
	addu	$2, $2, 16
	bne	$2, $4, $285
	.alias	$2,$30
	.alias	$2,$sp
$286:
	.loc	2 247
 # 246	                                D_80152698[j] = D_80152698[j+1];
 # 247	                            D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 248
 # 248	                            break;
	lw	$2, D_803BA010
	addu	$2, $2, 12
	b	$288
$287:
	.loc	2 243
 # 243	                    for (i = 3; i >= 1; i--) {
	addu	$16, $16, -1
	addu	$17, $17, -4
	bne	$16, 0, $281
	.alias	$17,$30
	.alias	$17,$sp
	lw	$2, D_803BA010
	addu	$2, $2, 12
$288:
	.loc	2 240
 # 240	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	addu	$19, $19, 4
	bltu	$19, $2, $280
	.alias	$30,$sp
$289:
	.loc	2 253
 # 253	            for (i = 1; i < 3; i++) {
	li	$16, 1
	la	$18, D_80152698
$290:
	.loc	2 253
	.loc	2 254
 # 254	                if (!D_80152698[i] && D_80152698[i+1]) {
	.noalias	$18,$sp
	.noalias	$17,$sp
	mul	$15, $16, 4
	addu	$17, $18, $15
	lw	$25, 0($17)
	bne	$25, 0, $291
	lw	$2, 4($17)
	beq	$2, 0, $291
	.loc	2 254
	.loc	2 255
 # 255	                    D_80152698[i] = D_80152698[i+1];
	sw	$2, 0($17)
	.loc	2 256
 # 256	                    D_80152698[i+1] = 0;
	sw	$0, 4($17)
	.loc	2 257
 # 257	                    i = 0;
	move	$16, $0
	.alias	$17,$sp
$291:
	.loc	2 253
 # 253	            for (i = 1; i < 3; i++) {
	addu	$16, $16, 1
	blt	$16, 3, $290
	b	$338
	.alias	$18,$sp
$292:
	.loc	2 260
 # 260	        } else {
	.loc	2 261
 # 261	            nearGhosts[2] = best;
	sw	$22, 92($sp)
	.loc	2 262
 # 262	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $2
	beq	$19, 0, $302
$293:
	.loc	2 262
	.loc	2 263
 # 263	                ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 264
 # 264	                nextGhost = ghost->next;
	lw	$21, 0($18)
	.loc	2 265
 # 265	                if (ghost->track == D_8014978C && ghost->mode == D_80152570) {
	.noalias	$30,$sp
	lb	$14, 0($30)
	lb	$24, 8($18)
	bne	$14, $24, $301
	lb	$15, D_80152570
	lbu	$25, 9($18)
	bne	$15, $25, $301
	.loc	2 265
	.loc	2 266
 # 266	                    if (ghosth != best) {
	beq	$19, $22, $301
	.loc	2 266
	.loc	2 267
 # 267	                        if (ghost->time < (*best)->time) {
	l.s	$f0, 32($18)
	lw	$14, 0($22)
	l.s	$f4, 32($14)
	c.lt.s	$f0, $f4
	bc1f	$297
	.loc	2 267
	.loc	2 268
 # 268	                            if (!nearGhosts[1] || ghost->time > (*nearGhosts[1])->time) {
	lw	$24, 88($sp)
	beq	$24, $0, $294
	lw	$15, 0($24)
	l.s	$f6, 32($15)
	c.lt.s	$f6, $f0
	bc1f	$295
$294:
	.loc	2 268
	.loc	2 269
 # 269	                                if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $301
	.loc	2 269
	.loc	2 270
 # 270	                                    nearGhosts[0] = nearGhosts[1];
	lw	$25, 88($sp)
	sw	$25, 84($sp)
	.loc	2 271
 # 271	                                    nearGhosts[1] = ghosth;
	sw	$19, 88($sp)
	b	$301
$295:
	.loc	2 273
 # 272	                                }
 # 273	                            } else if (!nearGhosts[0] || ghost->time > (*nearGhosts[0])->time) {
	lw	$14, 84($sp)
	beq	$14, $0, $296
	lw	$24, 0($14)
	l.s	$f8, 32($24)
	c.lt.s	$f8, $f0
	bc1f	$301
$296:
	.loc	2 273
	.loc	2 274
 # 274	                                if (menu_item_value_get(ghosth)) nearGhosts[0] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $301
	.loc	2 274
	sw	$19, 84($sp)
	b	$301
$297:
	.loc	2 276
 # 275	                            }
 # 276	                        } else {
	.loc	2 277
 # 277	                            if (!nearGhosts[3] || ghost->time < (*nearGhosts[3])->time) {
	lw	$15, 96($sp)
	beq	$15, $0, $298
	lw	$25, 0($15)
	l.s	$f10, 32($25)
	c.lt.s	$f0, $f10
	bc1f	$299
$298:
	.loc	2 277
	.loc	2 278
 # 278	                                if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $301
	.loc	2 278
	.loc	2 279
 # 279	                                    nearGhosts[4] = nearGhosts[3];
	lw	$14, 96($sp)
	sw	$14, 100($sp)
	.loc	2 280
 # 280	                                    nearGhosts[3] = ghosth;
	sw	$19, 96($sp)
	b	$301
$299:
	.loc	2 282
 # 281	                                }
 # 282	                            } else if (!nearGhosts[4] || ghost->time < (*nearGhosts[4])->time) {
	lw	$24, 100($sp)
	beq	$24, $0, $300
	lw	$15, 0($24)
	l.s	$f4, 32($15)
	c.lt.s	$f0, $f4
	bc1f	$301
$300:
	.loc	2 282
	.loc	2 283
 # 283	                                if (menu_item_value_get(ghosth)) nearGhosts[4] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $301
	.loc	2 283
	sw	$19, 100($sp)
$301:
	.loc	2 262
 # 262	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $293
$302:
	.loc	2 289
 # 289	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$3, D_803BA010
	move	$19, $3
	addu	$2, $3, 12
	bgeu	$3, $2, $313
$303:
	.loc	2 289
	.loc	2 290
 # 290	                ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 291
 # 291	                if (!ghost->active || ghost->track != D_8014978C || ghost->mode != D_80152570 || ghosth == best) continue;
	lw	$25, 40($18)
	beq	$25, 0, $312
	lb	$14, 0($30)
	lb	$24, 8($18)
	bne	$14, $24, $312
	lb	$15, D_80152570
	lbu	$25, 9($18)
	bne	$15, $25, $312
	beq	$19, $22, $312
	.loc	2 291
	.loc	2 292
 # 292	                if (ghost->time < (*best)->time) {
	l.s	$f0, 32($18)
	lw	$14, 0($22)
	l.s	$f6, 32($14)
	c.lt.s	$f0, $f6
	bc1f	$307
	.loc	2 292
	.loc	2 293
 # 293	                    if (!nearGhosts[1] || ghost->time > (*nearGhosts[1])->time) {
	lw	$24, 88($sp)
	beq	$24, $0, $304
	lw	$15, 0($24)
	l.s	$f8, 32($15)
	c.lt.s	$f8, $f0
	bc1f	$305
$304:
	.loc	2 293
	.loc	2 294
 # 294	                        if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $311
	.loc	2 294
	.loc	2 295
 # 295	                            nearGhosts[0] = nearGhosts[1];
	lw	$25, 88($sp)
	sw	$25, 84($sp)
	.loc	2 296
 # 296	                            nearGhosts[1] = ghosth;
	sw	$19, 88($sp)
	b	$311
$305:
	.loc	2 298
 # 297	                        }
 # 298	                    } else if (!nearGhosts[0] || ghost->time > (*nearGhosts[0])->time) {
	lw	$14, 84($sp)
	beq	$14, $0, $306
	lw	$24, 0($14)
	l.s	$f10, 32($24)
	c.lt.s	$f10, $f0
	bc1f	$311
$306:
	.loc	2 298
	.loc	2 299
 # 299	                        if (menu_item_value_get(ghosth)) nearGhosts[0] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $311
	.loc	2 299
	sw	$19, 84($sp)
	b	$311
$307:
	.loc	2 301
 # 300	                    }
 # 301	                } else {
	.loc	2 302
 # 302	                    if (!nearGhosts[3] || ghost->time < (*nearGhosts[3])->time) {
	lw	$15, 96($sp)
	beq	$15, $0, $308
	lw	$25, 0($15)
	l.s	$f4, 32($25)
	c.lt.s	$f0, $f4
	bc1f	$309
$308:
	.loc	2 302
	.loc	2 303
 # 303	                        if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $311
	.loc	2 303
	.loc	2 304
 # 304	                            nearGhosts[4] = nearGhosts[3];
	lw	$14, 96($sp)
	sw	$14, 100($sp)
	.loc	2 305
 # 305	                            nearGhosts[3] = ghosth;
	sw	$19, 96($sp)
	b	$311
$309:
	.loc	2 307
 # 306	                        }
 # 307	                    } else if (!nearGhosts[4] || ghost->time < (*nearGhosts[4])->time) {
	lw	$24, 100($sp)
	beq	$24, $0, $310
	lw	$15, 0($24)
	l.s	$f6, 32($15)
	c.lt.s	$f0, $f6
	bc1f	$311
$310:
	.loc	2 307
	.loc	2 308
 # 308	                        if (menu_item_value_get(ghosth)) nearGhosts[4] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $311
	.loc	2 308
	sw	$19, 100($sp)
$311:
	.loc	2 289
 # 289	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$2, D_803BA010
	addu	$2, $2, 12
$312:
	addu	$19, $19, 4
	bltu	$19, $2, $303
	.alias	$30,$sp
$313:
	.loc	2 312
 # 312	            ghostCount = 1;
	li	$2, 1
	.loc	2 313
 # 313	            for (i = 0; i < 5; i++) {
	lw	$25, 84($sp)
	beq	$25, $0, $314
	sw	$25, D_80152698+4
	li	$2, 2
$314:
	li	$16, 1
	.loc	2 313
	.loc	2 314
 # 314	                if (nearGhosts[i] && ghostCount < 4) { D_80152698[ghostCount] = nearGhosts[i]; ghostCount++; }
	.noalias	$3,$gp
	mul	$14, $16, 4
	addu	$24, $sp, 84
	addu	$3, $14, $24
	lw	$4, 0($3)
	beq	$4, 0, $315
	bge	$2, 4, $315
	la	$18, D_80152698
	.loc	2 314
	.loc	2 314
	.noalias	$18,$3
	.noalias	$18,$sp
	mul	$15, $2, 4
	addu	$25, $18, $15
	.noalias	$25,$3
	.noalias	$25,$sp
	sw	$4, 0($25)
	.alias	$25,$3
	.alias	$25,$sp
	.loc	2 314
	addu	$2, $2, 1
$315:
	la	$18, D_80152698
	.loc	2 313
 # 313	            for (i = 0; i < 5; i++) {
	lw	$4, 4($3)
	beq	$4, 0, $316
	bge	$2, 4, $316
	mul	$14, $2, 4
	addu	$24, $18, $14
	.noalias	$24,$3
	.noalias	$24,$sp
	sw	$4, 0($24)
	.alias	$24,$3
	.alias	$24,$sp
	addu	$2, $2, 1
$316:
	lw	$4, 8($3)
	beq	$4, 0, $317
	bge	$2, 4, $317
	mul	$15, $2, 4
	addu	$25, $18, $15
	.noalias	$25,$3
	.noalias	$25,$sp
	sw	$4, 0($25)
	.alias	$25,$3
	.alias	$25,$sp
	addu	$2, $2, 1
$317:
	lw	$4, 12($3)
	beq	$4, 0, $338
	.alias	$3,$18
	.alias	$3,$gp
	bge	$2, 4, $338
	mul	$14, $2, 4
	addu	$24, $18, $14
	.noalias	$24,$sp
	sw	$4, 0($24)
	.alias	$24,$sp
	b	$338
	.alias	$18,$sp
$318:
	.loc	2 317
 # 314	                if (nearGhosts[i] && ghostCount < 4) { D_80152698[ghostCount] = nearGhosts[i]; ghostCount++; }
 # 315	            }
 # 316	        }
 # 317	    } else {
	.loc	2 318
 # 318	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	lw	$19, D_80152020+8
	beq	$19, 0, $328
	la	$30, D_8014978C
	li	$23, 3
	li	$20, 4
$319:
	.loc	2 318
	.loc	2 319
 # 319	            ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 320
 # 320	            nextGhost = ghost->next;
	lw	$21, 0($18)
	.loc	2 321
 # 321	            if (ghost->track == D_8014978C && ghost->mode == D_80152570) {
	.noalias	$30,$sp
	lb	$15, 0($30)
	lb	$25, 8($18)
	bne	$15, $25, $327
	lb	$14, D_80152570
	lbu	$24, 9($18)
	bne	$14, $24, $327
	.loc	2 321
	.loc	2 322
 # 322	                for (i = 1; i < 4; i++) {
	li	$16, 1
	la	$17, D_80152698+4
$320:
	.loc	2 322
	.loc	2 323
 # 323	                    if ((!D_80152698[i] || ghost->time < (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $321
	l.s	$f8, 32($18)
	lw	$15, 0($2)
	l.s	$f10, 32($15)
	c.lt.s	$f8, $f10
	bc1f	$326
$321:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $326
	.loc	2 323
	.loc	2 324
 # 324	                        for (j = 3; j > i; j--)
	move	$3, $23
	bge	$16, 3, $325
	subu	$5, $23, $16
	and	$25, $5, 3
	move	$5, $25
	negu	$5, $5
	move	$4, $5
	beq	$4, 0, $323
	addu	$4, $5, 3
	mul	$14, $3, 4
	la	$24, D_80152698
	addu	$2, $14, $24
$322:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$15, -4($2)
	sw	$15, 0($2)
	addu	$3, $3, -1
	addu	$2, $2, -4
	bne	$4, $3, $322
	.alias	$2,$30
	.alias	$2,$sp
	beq	$16, $3, $325
$323:
	mul	$25, $3, 4
	la	$14, D_80152698
	addu	$2, $25, $14
	mul	$24, $16, 4
	addu	$4, $24, $14
$324:
	.loc	2 325
 # 325	                            D_80152698[j] = D_80152698[j-1];
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$15, -4($2)
	sw	$15, 0($2)
	.loc	2 324
 # 324	                        for (j = 3; j > i; j--)
	lw	$25, -8($2)
	sw	$25, -4($2)
	lw	$24, -12($2)
	sw	$24, -8($2)
	lw	$14, -16($2)
	sw	$14, -12($2)
	addu	$2, $2, -16
	bne	$4, $2, $324
	.alias	$2,$30
	.alias	$2,$sp
$325:
	.loc	2 326
 # 325	                            D_80152698[j] = D_80152698[j-1];
 # 326	                        D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 327
 # 327	                        break;
	b	$327
$326:
	.loc	2 322
 # 322	                for (i = 1; i < 4; i++) {
	addu	$16, $16, 1
	addu	$17, $17, 4
	bne	$16, $20, $320
	.alias	$17,$30
	.alias	$17,$sp
$327:
	.loc	2 318
 # 318	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $319
$328:
	li	$20, 4
	li	$23, 3
	la	$30, D_8014978C
	.loc	2 332
 # 332	        for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$3, D_803BA010
	move	$19, $3
	addu	$2, $3, 12
	bgeu	$3, $2, $338
$329:
	.loc	2 332
	.loc	2 333
 # 333	            ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 334
 # 334	            if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570) {
	lw	$15, 40($18)
	beq	$15, 0, $337
	lb	$25, 0($30)
	lb	$24, 8($18)
	bne	$25, $24, $337
	lb	$14, D_80152570
	lbu	$15, 9($18)
	bne	$14, $15, $337
	.loc	2 334
	.loc	2 335
 # 335	                for (i = 1; i < 4; i++) {
	li	$16, 1
	la	$17, D_80152698+4
$330:
	.loc	2 335
	.loc	2 336
 # 336	                    if ((!D_80152698[i] || ghost->time < (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $331
	l.s	$f4, 32($18)
	lw	$25, 0($2)
	l.s	$f6, 32($25)
	c.lt.s	$f4, $f6
	bc1f	$336
$331:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $336
	.loc	2 336
	.loc	2 337
 # 337	                        for (j = 3; j > i; j--)
	move	$3, $23
	bge	$16, 3, $335
	subu	$5, $23, $16
	and	$24, $5, 3
	move	$5, $24
	negu	$5, $5
	move	$4, $5
	beq	$4, 0, $333
	addu	$4, $5, 3
	mul	$14, $3, 4
	la	$15, D_80152698
	addu	$2, $14, $15
$332:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$25, -4($2)
	sw	$25, 0($2)
	addu	$3, $3, -1
	addu	$2, $2, -4
	bne	$4, $3, $332
	.alias	$2,$30
	.alias	$2,$sp
	beq	$16, $3, $335
$333:
	mul	$24, $3, 4
	la	$14, D_80152698
	addu	$2, $24, $14
	mul	$15, $16, 4
	addu	$4, $15, $14
$334:
	.loc	2 338
 # 338	                            D_80152698[j] = D_80152698[j-1];
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$25, -4($2)
	sw	$25, 0($2)
	.loc	2 337
 # 337	                        for (j = 3; j > i; j--)
	lw	$24, -8($2)
	sw	$24, -4($2)
	lw	$15, -12($2)
	sw	$15, -8($2)
	lw	$14, -16($2)
	sw	$14, -12($2)
	addu	$2, $2, -16
	bne	$4, $2, $334
	.alias	$2,$30
	.alias	$2,$sp
$335:
	.loc	2 339
 # 338	                            D_80152698[j] = D_80152698[j-1];
 # 339	                        D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 340
 # 340	                        break;
	lw	$2, D_803BA010
	addu	$2, $2, 12
	b	$337
$336:
	.loc	2 335
 # 335	                for (i = 1; i < 4; i++) {
	addu	$16, $16, 1
	addu	$17, $17, 4
	bne	$16, $20, $330
	.alias	$17,$30
	.alias	$17,$sp
	lw	$2, D_803BA010
	addu	$2, $2, 12
$337:
	.loc	2 332
 # 332	        for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	addu	$19, $19, 4
	bltu	$19, $2, $329
	.alias	$30,$sp
$338:
	la	$18, D_80152698
	.loc	2 346
 # 346	    if (!D_80156994) {
	lb	$25, D_80156994
	bne	$25, 0, $340
	.loc	2 346
	.loc	2 347
 # 347	        if (D_80152698[2]) { menu_transition((void **)D_80152698[2]); D_80152698[2] = 0; }
	.noalias	$18,$sp
	lw	$4, 8($18)
	beq	$4, 0, $339
	.loc	2 347
	.loc	2 347
	.livereg	0x0800000E,0x00000000
	jal	menu_transition
	.loc	2 347
	sw	$0, 8($18)
$339:
	.loc	2 348
 # 348	        if (D_80152698[3]) { menu_transition((void **)D_80152698[3]); D_80152698[3] = 0; }
	lw	$4, 12($18)
	beq	$4, 0, $340
	.loc	2 348
	.loc	2 348
	.livereg	0x0800000E,0x00000000
	jal	menu_transition
	.loc	2 348
	sw	$0, 12($18)
	.alias	$18,$sp
	.loc	2 350
 # 349	    }
 # 350	}
$340:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 104
	j	$31
	.end	func_803A0498
	.text	
	.align	2
	.file	2 "g.c"
	.loc	2 376
 # 376	{
	.ent	func_803A0FD8 2
func_803A0FD8:
	.option	O3
	subu	$sp, 208
	sw	$31, 36($sp)
	.mask	0x80000000, -172
	.frame	$sp, 208, $31
	.loc	2 376
	.loc	2 376
	.loc	2 381
 # 377	    Player *pl;
 # 378	    s32 p;
 # 379	    s32 i;
 # 380	
 # 381	    func_803A0498();
	.livereg	0x0000000E,0x00000000
	jal	func_803A0498
	.loc	2 382
 # 382	    audio_frame_sync(62, 0, 0, 0, 0);
	li	$4, 62
	move	$5, $0
	move	$6, $0
	move	$7, $0
	sw	$0, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	audio_frame_sync
	la	$18, D_80156994
	.loc	2 383
 # 383	    if (D_80156994) {
	.noalias	$18,$sp
	lb	$14, 0($18)
	beq	$14, 0, $341
	.loc	2 383
	.loc	2 384
 # 384	        audio_frame_sync(80, 0, 0, 0, 0);
	li	$4, 80
	move	$5, $0
	move	$6, $0
	move	$7, $0
	sw	$0, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	audio_frame_sync
	.loc	2 385
 # 385	        audio_frame_sync(61, 0, 0, 0, 0);
	li	$4, 61
	move	$5, $0
	move	$6, $0
	move	$7, $0
	sw	$0, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	audio_frame_sync
	b	$343
$341:
	.loc	2 386
 # 386	    } else if (D_8014978C >= 6) {
	lb	$15, D_8014978C
	blt	$15, 6, $342
	.loc	2 386
	.loc	2 387
 # 387	        audio_frame_sync(80, 0, 0, 0, 0);
	li	$4, 80
	move	$5, $0
	move	$6, $0
	move	$7, $0
	sw	$0, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	audio_frame_sync
	b	$343
$342:
	.loc	2 388
 # 388	    } else {
	.loc	2 389
 # 389	        audio_frame_sync(81, 0, 0, 0, 0);
	li	$4, 81
	move	$5, $0
	move	$6, $0
	move	$7, $0
	sw	$0, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	audio_frame_sync
$343:
	.loc	2 391
 # 390	    }
 # 391	    D_803BA02C = 0;
	sb	$0, D_803BA02C
	.loc	2 392
 # 392	    for (p = 0; p < D_8014A108; p++) {
	move	$20, $0
	lh	$6, D_8014A108
	ble	$6, 0, $345
	la	$11, D_803B28B8
	la	$12, D_803BA048
	la	$13, D_803BA038
	la	$16, D_803BA028
	la	$17, D_803BA1D0
$344:
	li	$2, 1
	.loc	2 392
	.loc	2 393
 # 393	        pl = &D_8014A118[p];
	.loc	2 394
 # 394	        D_803B28B8[p] = 0;
	.noalias	$11,$18
	.noalias	$11,$sp
	sw	$0, 0($11)
	.loc	2 395
 # 395	        D_803BA048[p] = 1;
	.noalias	$12,$11
	.noalias	$12,$18
	.noalias	$12,$sp
	sb	$2, 0($12)
	.loc	2 396
 # 396	        D_803BA038[p] = 1;
	.noalias	$13,$11
	.noalias	$13,$12
	.noalias	$13,$18
	.noalias	$13,$sp
	sw	$2, 0($13)
	.loc	2 397
 # 397	        D_803BA028[p] = 0;
	.noalias	$16,$11
	.noalias	$16,$12
	.noalias	$16,$13
	.noalias	$16,$18
	.noalias	$16,$sp
	sb	$0, 0($16)
	.loc	2 398
 # 398	        D_803BA1D0[p] = -1;
	.noalias	$17,$11
	.noalias	$17,$12
	.noalias	$17,$13
	.noalias	$17,$16
	.noalias	$17,$18
	.noalias	$17,$sp
	li	$24, -1
	sw	$24, 0($17)
	.loc	2 399
 # 399	        func_8039D300(p);
	move	$4, $20
	.livereg	0x0800000E,0x00000000
	jal	func_8039D300
	.loc	2 392
 # 392	    for (p = 0; p < D_8014A108; p++) {
	addu	$20, $20, 1
	addu	$11, $11, 4
	addu	$12, $12, 1
	addu	$13, $13, 4
	addu	$16, $16, 1
	addu	$17, $17, 4
	lh	$6, D_8014A108
	blt	$20, $6, $344
	.alias	$11,$12
	.alias	$11,$13
	.alias	$11,$16
	.alias	$11,$17
	.alias	$11,$18
	.alias	$11,$sp
	.alias	$12,$13
	.alias	$12,$16
	.alias	$12,$17
	.alias	$12,$18
	.alias	$12,$sp
	.alias	$13,$16
	.alias	$13,$17
	.alias	$13,$18
	.alias	$13,$sp
	.alias	$16,$17
	.alias	$16,$18
	.alias	$16,$sp
	.alias	$17,$18
	.alias	$17,$sp
	move	$20, $0
$345:
	.loc	2 401
 # 401	    for (p = 0; p < D_8014A108; p++) {
	ble	$6, 0, $352
	la	$4, D_803BA020
	li	$7, 13
	la	$5, D_803B9FD8
	la	$2, D_803B3010
$346:
	.loc	2 401
	.loc	2 402
 # 402	        for (D_803BA020[p] = 0, i = 0; i < 13; i++) {
	.noalias	$4,$18
	.noalias	$4,$sp
	sh	$0, 0($4)
	lh	$3, 0($4)
	.noalias	$5,$4
	.noalias	$5,$18
	.noalias	$5,$sp
	lb	$25, D_803B3010
	mul	$14, $20, 13
	addu	$15, $5, $14
	addu	$24, $15, $3
	sb	$25, 0($24)
	addu	$14, $3, 1
	sh	$14, 0($4)
	li	$21, 1
$347:
	.loc	2 402
	.loc	2 403
 # 403	            if (i < 13) D_803B9FD8[p][D_803BA020[p]++] = D_803B3010[i];
	bge	$21, 13, $348
	.loc	2 403
	lh	$3, 0($4)
	.noalias	$2,$4
	.noalias	$2,$5
	.noalias	$2,$18
	.noalias	$2,$sp
	addu	$15, $2, $21
	.noalias	$15,$4
	.noalias	$15,$5
	.noalias	$15,$18
	.noalias	$15,$sp
	lb	$25, 0($15)
	.alias	$15,$4
	.alias	$15,$5
	.alias	$15,$18
	.alias	$15,$sp
	mul	$24, $20, 13
	addu	$14, $5, $24
	addu	$15, $14, $3
	sb	$25, 0($15)
	addu	$24, $3, 1
	sh	$24, 0($4)
$348:
	.loc	2 402
 # 402	        for (D_803BA020[p] = 0, i = 0; i < 13; i++) {
	bge	$21, 12, $349
	lh	$3, 0($4)
	addu	$14, $2, $21
	.noalias	$14,$4
	.noalias	$14,$5
	.noalias	$14,$18
	.noalias	$14,$sp
	lb	$25, 1($14)
	.alias	$14,$4
	.alias	$14,$5
	.alias	$14,$18
	.alias	$14,$sp
	mul	$15, $20, 13
	addu	$24, $5, $15
	addu	$14, $24, $3
	sb	$25, 0($14)
	addu	$15, $3, 1
	sh	$15, 0($4)
$349:
	bge	$21, 11, $350
	lh	$3, 0($4)
	addu	$24, $2, $21
	.noalias	$24,$4
	.noalias	$24,$5
	.noalias	$24,$18
	.noalias	$24,$sp
	lb	$25, 2($24)
	.alias	$24,$4
	.alias	$24,$5
	.alias	$24,$18
	.alias	$24,$sp
	mul	$14, $20, 13
	addu	$15, $5, $14
	addu	$24, $15, $3
	sb	$25, 0($24)
	addu	$14, $3, 1
	sh	$14, 0($4)
$350:
	bge	$21, 10, $351
	lh	$3, 0($4)
	addu	$15, $2, $21
	.noalias	$15,$4
	.noalias	$15,$5
	.noalias	$15,$18
	.noalias	$15,$sp
	lb	$25, 3($15)
	.alias	$15,$4
	.alias	$15,$5
	.alias	$15,$18
	.alias	$15,$sp
	mul	$24, $20, 13
	addu	$14, $5, $24
	addu	$15, $14, $3
	sb	$25, 0($15)
	addu	$24, $3, 1
	sh	$24, 0($4)
$351:
	addu	$21, $21, 4
	bne	$21, $7, $347
	.loc	2 401
 # 401	    for (p = 0; p < D_8014A108; p++) {
	addu	$20, $20, 1
	addu	$4, $4, 2
	blt	$20, $6, $346
	.alias	$2,$4
	.alias	$2,$5
	.alias	$2,$18
	.alias	$2,$sp
	.alias	$4,$5
	.alias	$4,$18
	.alias	$4,$sp
	move	$20, $0
$352:
	la	$5, D_803B9FD8
	li	$7, 13
	.loc	2 406
 # 402	        for (D_803BA020[p] = 0, i = 0; i < 13; i++) {
 # 403	            if (i < 13) D_803B9FD8[p][D_803BA020[p]++] = D_803B3010[i];
 # 404	        }
 # 405	    }
 # 406	    for (i = 0; i < 48; i++) D_803B9AA0[i] = func_800A7D6C();
	move	$21, $0
	la	$16, D_803B9AA0
	la	$17, D_803B9AA0+192
$353:
	la	$5, D_803B9FD8
	li	$7, 13
	.loc	2 406
	.livereg	0x0000000E,0x00000000
	jal	func_800A7D6C
	.alias	$5,$18
	.alias	$5,$sp
	.noalias	$16,$18
	.noalias	$16,$sp
	sw	$2, 0($16)
	.loc	2 406
	addu	$16, $16, 4
	bne	$16, $17, $353
	.alias	$16,$18
	.alias	$16,$sp
	.loc	2 407
 # 407	    init_state_begin();
	.livereg	0x0000000E,0x00000000
	jal	init_state_begin
	la	$2, D_80146180
	.loc	2 408
 # 408	    D_80146180[0] = 0;
	.noalias	$2,$18
	.noalias	$2,$sp
	sb	$0, 0($2)
	.loc	2 409
 # 409	    D_80146180[1] = 0;
	sb	$0, 1($2)
	.loc	2 410
 # 410	    D_80146180[2] = 0;
	sb	$0, 2($2)
	.loc	2 411
 # 411	    D_80146180[3] = 0;
	sb	$0, 3($2)
	.loc	2 412
 # 412	    D_80154188 = 90.0f;
	li.s	$f4, 90.0
	s.s	$f4, D_80154188
	.loc	2 413
 # 413	    func_800A5A40();
	.livereg	0x0000000E,0x00000000
	jal	func_800A5A40
	.alias	$2,$18
	.alias	$2,$sp
	li	$2, 3
	la	$16, D_80151AD0
	.loc	2 414
 # 414	    D_80151AD0 = D_8014A108;
	.noalias	$16,$18
	.noalias	$16,$sp
	lh	$14, D_8014A108
	sh	$14, 0($16)
	.loc	2 415
 # 415	    if (D_80151AD0 == 3) D_80151AD0 = 4;
	lh	$25, 0($16)
	bne	$2, $25, $354
	.loc	2 415
	li	$15, 4
	sh	$15, 0($16)
$354:
	li	$5, 2
	.loc	2 416
 # 416	    if (D_8014A110 == 2) {
	lw	$24, D_8014A110
	bne	$5, $24, $355
	la	$3, D_803BA00C
	.loc	2 416
	.loc	2 417
 # 417	        D_803BA00C = 3;
	.noalias	$3,$16
	.noalias	$3,$18
	.noalias	$3,$sp
	sb	$2, 0($3)
	.loc	2 418
 # 418	        D_80151AD0 = 4;
	li	$14, 4
	sh	$14, 0($16)
	.loc	2 419
 # 419	        if (!D_80156994) {
	lb	$25, 0($18)
	bne	$25, 0, $355
	.alias	$18,$3
	.alias	$18,$16
	.alias	$18,$sp
	.loc	2 419
	.loc	2 420
 # 420	            D_803BA00C = 1;
	li	$15, 1
	sb	$15, 0($3)
	.loc	2 421
 # 421	            D_80151AD0 = 2;
	sh	$5, 0($16)
	.alias	$3,$16
	.alias	$3,$sp
$355:
	.loc	2 424
 # 422	        }
 # 423	    }
 # 424	    car_render_full(D_80151AD0);
	lh	$4, 0($16)
	.livereg	0x0800000E,0x00000000
	jal	car_render_full
	.loc	2 425
 # 425	    if (D_80151AD0 == 1) {
	lh	$24, 0($16)
	bne	$24, 1, $356
	.alias	$16,$sp
	.loc	2 425
	.loc	2 426
 # 426	        D_803B25A8 = 1.0f;
	li.s	$f6, 1.0
	s.s	$f6, D_803B25A8
	.loc	2 427
 # 427	        D_803B25AC = 0.045f;
	li.s	$f8, 0.045
	s.s	$f8, D_803B25AC
	.loc	2 428
 # 428	        D_803B25B0 = 40.0f;
	li.s	$f10, 40.0
	s.s	$f10, D_803B25B0
	.loc	2 429
 # 429	        D_803B25B4 = 0.0f;
	li.s	$f4, 0.0
	s.s	$f4, D_803B25B4
	.loc	2 430
 # 430	        D_803B25B8 = -5.4f;
	li.s	$f6, -5.4000000953674316e+00
	s.s	$f6, D_803B25B8
	.loc	2 431
 # 431	        D_803B25BC = -8.4f;
	li.s	$f8, -8.3999996185302734e+00
	s.s	$f8, D_803B25BC
	.loc	2 432
 # 432	        D_803B25C0 = 15.0f;
	li.s	$f10, 15.0
	s.s	$f10, D_803B25C0
	.loc	2 433
 # 433	        D_803B25C4 = 11.25f;
	li.s	$f4, 11.25
	s.s	$f4, D_803B25C4
	b	$357
$356:
	.loc	2 434
 # 434	    } else {
	.loc	2 435
 # 435	        D_803B25A8 = 1.0f;
	li.s	$f6, 1.0
	s.s	$f6, D_803B25A8
	.loc	2 436
 # 436	        D_803B25AC = 0.055f;
	li.s	$f8, 0.055
	s.s	$f8, D_803B25AC
	.loc	2 437
 # 437	        D_803B25B0 = 40.0f;
	li.s	$f10, 40.0
	s.s	$f10, D_803B25B0
	.loc	2 438
 # 438	        D_803B25B4 = 0.0f;
	li.s	$f4, 0.0
	s.s	$f4, D_803B25B4
	.loc	2 439
 # 439	        D_803B25B8 = -3.45f;
	li.s	$f6, -3.4500000476837158e+00
	s.s	$f6, D_803B25B8
	.loc	2 440
 # 440	        D_803B25BC = -7.7f;
	li.s	$f8, -7.6999998092651367e+00
	s.s	$f8, D_803B25BC
	.loc	2 441
 # 441	        D_803B25C0 = 15.0f;
	li.s	$f10, 15.0
	s.s	$f10, D_803B25C0
	.loc	2 442
 # 442	        D_803B25C4 = 11.25f;
	li.s	$f4, 11.25
	s.s	$f4, D_803B25C4
$357:
	.loc	2 444
 # 443	    }
 # 444	    audio_interrupt_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	audio_interrupt_handler
	.loc	2 445
 # 445	    sfx_position_3d(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	sfx_position_3d
	.loc	2 446
 # 446	    particle_lifetime_set();
	.livereg	0x0000000E,0x00000000
	jal	particle_lifetime_set
	.loc	2 447
 # 447	    for (p = 0; p < 4; p++) D_803B9FC0[p] = ambient_sound_set(-20, 0, -10, 10, 192, 0, 0, 0);
	la	$16, D_803B9FC0
	la	$17, D_803B9FC0+16
$358:
	.loc	2 447
	li	$4, -20
	move	$5, $0
	li	$6, -10
	li	$7, 10
	li	$14, 192
	sw	$14, 16($sp)
	sw	$0, 20($sp)
	sw	$0, 24($sp)
	sw	$0, 28($sp)
	.livereg	0x0F00000E,0x00000000
	jal	ambient_sound_set
	.noalias	$16,$sp
	sw	$2, 0($16)
	.loc	2 447
	addu	$16, $16, 4
	bltu	$16, $17, $358
	.alias	$16,$sp
	.loc	2 448
 # 448	    for (p = 0; p < D_8014A108; p++) {
	move	$20, $0
	lh	$25, D_8014A108
	ble	$25, 0, $389
	la	$22, D_8014A118
	la	$15, D_803BA020
	sw	$15, 180($sp)
	la	$5, D_803B9E40
	la	$24, D_803B9E40+48
	sw	$24, 168($sp)
	la	$14, D_803B25CC
	sw	$14, 164($sp)
	la	$25, D_8011103C
	sw	$25, 160($sp)
	la	$15, D_80111080
	sw	$15, 152($sp)
	la	$24, D_8011119C
	sw	$24, 140($sp)
	la	$14, D_80111230
	sw	$14, 132($sp)
	la	$25, D_8011128C
	sw	$25, 124($sp)
	la	$15, D_80111604
	sw	$15, 116($sp)
	la	$24, D_80111648
	sw	$24, 108($sp)
	la	$14, D_8011168C
	sw	$14, 100($sp)
	la	$25, D_8011157C
	sw	$25, 92($sp)
	la	$15, D_801115C0
	sw	$15, 84($sp)
	la	$24, D_801112DC
	sw	$24, 76($sp)
	la	$14, D_801113E0
	sw	$14, 68($sp)
	la	$25, D_803B9FD0
	sw	$25, 60($sp)
	la	$15, D_803B9FD4
	sw	$15, 56($sp)
	la	$24, D_803BA018
	sw	$24, 52($sp)
	la	$14, D_80110E78
	sw	$14, 48($sp)
	la	$25, D_80110E78
	sw	$25, 44($sp)
	sw	$5, 172($sp)
	li.s	$f24, 3.1415927
	li.s	$f22, 0.75
	li.s	$f20, 100.0
$359:
	lw	$5, 172($sp)
	.loc	2 448
	.loc	2 449
 # 449	        pl = &D_8014A118[p];
	.loc	2 450
 # 450	        math_utility(D_8011418C, D_803B9E40[p][0]);
	la	$4, D_8011418C
	.livereg	0x0C00000E,0x00000000
	jal	math_utility
	.loc	2 451
 # 451	        math_utility(D_8011418C, D_803B9E40[p][1]);
	la	$4, D_8011418C
	lw	$5, 168($sp)
	.livereg	0x0C00000E,0x00000000
	jal	math_utility
	.loc	2 452
 # 452	        D_803B25CC[p][0] = sign_extend_call(string_copy_format(D_803B85B0, 0, D_80140BDC - 1, 1), D_803B9E40[p][0], -1, 0x40000);
	la	$4, D_803B85B0
	move	$5, $0
	la	$15, D_80140BDC
	.set	 volatile
	lbu	$6, 0($15)
	.set	 novolatile
	addu	$6, $6, -1
	sll	$24, $6, 24
	move	$6, $24
	sra	$14, $6, 24
	move	$6, $14
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	string_copy_format
	move	$4, $2
	lw	$5, 172($sp)
	li	$6, -1
	li	$7, 262144
	.livereg	0x0F00000E,0x00000000
	jal	sign_extend_call
	lw	$25, 164($sp)
	sw	$2, 0($25)
	.loc	2 453
 # 453	        D_803B25CC[p][1] = sign_extend_call(string_copy_format(D_803B85BC, 0, D_80140BDC - 1, 1), D_803B9E40[p][1], -1, 0);
	la	$4, D_803B85BC
	move	$5, $0
	la	$15, D_80140BDC
	.set	 volatile
	lbu	$6, 0($15)
	.set	 novolatile
	addu	$6, $6, -1
	sll	$24, $6, 24
	move	$6, $24
	sra	$14, $6, 24
	move	$6, $14
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	string_copy_format
	move	$4, $2
	lw	$5, 168($sp)
	li	$6, -1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	sign_extend_call
	lw	$3, 164($sp)
	.noalias	$3,$sp
	sw	$2, 4($3)
	.loc	2 454
 # 454	        model_data_load(D_803B25CC[p][0], 1, 15);
	lw	$4, 0($3)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.alias	$3,$sp
	.loc	2 455
 # 455	        model_data_load(D_803B25CC[p][1], 1, 15);
	lw	$25, 164($sp)
	lw	$4, 4($25)
	li	$5, 1
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_data_load
	.loc	2 456
 # 456	        if (D_8014A110 == 2) {
	lw	$15, D_8014A110
	bne	$15, 2, $360
	.loc	2 456
	.loc	2 457
 # 457	            model_transform_setup(D_803B25CC[p][0], 0, 15);
	lw	$24, 164($sp)
	lw	$4, 0($24)
	move	$5, $0
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
	.loc	2 458
 # 458	            model_transform_setup(D_803B25CC[p][1], 0, 15);
	lw	$14, 164($sp)
	lw	$4, 4($14)
	move	$5, $0
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
	b	$361
$360:
	.loc	2 459
 # 459	        } else {
	.loc	2 460
 # 460	            model_transform_setup(D_803B25CC[p][0], 0, 1 << p);
	lw	$25, 164($sp)
	lw	$4, 0($25)
	move	$5, $0
	li	$15, 1
	sll	$16, $15, $20
	move	$6, $16
	.livereg	0x0E00800E,0x00000000
	jal	model_transform_setup
	.loc	2 461
 # 461	            model_transform_setup(D_803B25CC[p][1], 0, 1 << p);
	lw	$24, 164($sp)
	lw	$4, 4($24)
	move	$5, $0
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
$361:
	lw	$2, 172($sp)
	li.s	$f0, 0.0000000000000000e+00
	li.s	$f2, 2.0
	li.s	$f12, 15.0
	.loc	2 463
 # 462	        }
 # 463	        D_803B9E40[p][0][9] = 0.0;
	.noalias	$2,$sp
	s.s	$f0, 36($2)
	.loc	2 464
 # 464	        D_803B9E40[p][0][10] = 2.0f;
	s.s	$f2, 40($2)
	.loc	2 465
 # 465	        D_803B9E40[p][0][11] = 15.0f;
	s.s	$f12, 44($2)
	.loc	2 466
 # 466	        D_803B9E40[p][1][9] = 0.0;
	s.s	$f0, 84($2)
	.loc	2 467
 # 467	        D_803B9E40[p][1][10] = 2.0f;
	s.s	$f2, 88($2)
	.loc	2 468
 # 468	        D_803B9E40[p][1][11] = 15.0f;
	s.s	$f12, 92($2)
	.loc	2 469
 # 469	        func_8039FF04(p);
	sw	$20, 0($sp)
	sw	$20, 200($sp)
	sw	$22, 188($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_8039FF04
	.alias	$2,$sp
	lw	$20, 200($sp)
	lw	$22, 188($sp)
	li.s	$f20, 100.0
	li.s	$f22, 0.75
	li.s	$f24, 3.1415927
	.loc	2 470
 # 470	        for (i = 0; i < 13; i++) {
	move	$21, $0
	move	$30, $0
	lw	$14, 160($sp)
	sw	$14, 156($sp)
	lw	$16, 152($sp)
	lw	$17, 140($sp)
	lw	$18, 132($sp)
	lw	$23, 124($sp)
	lw	$25, 116($sp)
	sw	$25, 112($sp)
	lw	$15, 108($sp)
	sw	$15, 104($sp)
	lw	$24, 100($sp)
	sw	$24, 96($sp)
	lw	$14, 92($sp)
	sw	$14, 88($sp)
	lw	$25, 84($sp)
	sw	$25, 80($sp)
	lw	$15, 76($sp)
	sw	$15, 72($sp)
	lw	$24, 68($sp)
	sw	$24, 64($sp)
$362:
	.loc	2 470
	.loc	2 471
 # 471	            D_8011103C[p + 1][i] = func_800CDA60(pl->owner, i, 0);
	lw	$4, 72($22)
	and	$19, $21, 255
	and	$5, $19, 255
	move	$6, $0
	.livereg	0x0E00100E,0x00000000
	jal	func_800CDA60
	lw	$14, 156($sp)
	sb	$2, 13($14)
	.loc	2 472
 # 472	            D_80111080[p + 1][i] = func_800CDA60(pl->owner, i, 1);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	.noalias	$16,$sp
	sb	$2, 13($16)
	.loc	2 473
 # 473	            while (!func_800F7604(p, D_80111080[p + 1][i])) {
	move	$4, $20
	lb	$5, 13($16)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7604
	bne	$2, 0, $365
$363:
	.loc	2 473
	.loc	2 474
 # 474	                D_80111080[p + 1][i]--;
	lb	$25, 13($16)
	addu	$15, $25, -1
	sb	$15, 13($16)
	.loc	2 475
 # 475	                if (D_80111080[p + 1][i] < 0) { D_80111080[p + 1][i] = 0; break; }
	lb	$5, 13($16)
	bge	$5, 0, $364
	.loc	2 475
	.loc	2 475
	sb	$0, 13($16)
	.loc	2 475
	b	$365
$364:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7604
	beq	$2, 0, $363
$365:
	.loc	2 477
 # 476	            }
 # 477	            func_800C7578(pl->owner, i, 1, D_80111080[p + 1][i]);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 1
	lb	$7, 13($16)
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 478
 # 478	            D_8011119C[p + 1][i] = func_800CDA60(pl->owner, i, 2);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 2
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	.noalias	$17,$16
	.noalias	$17,$sp
	sb	$2, 13($17)
	.loc	2 479
 # 479	            while (!func_800F75D0(p, D_8011119C[p + 1][i])) {
	move	$4, $20
	lb	$5, 13($17)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75D0
	bne	$2, 0, $368
$366:
	.loc	2 479
	.loc	2 480
 # 480	                D_8011119C[p + 1][i]--;
	lb	$24, 13($17)
	addu	$14, $24, -1
	sb	$14, 13($17)
	.loc	2 481
 # 481	                if (D_8011119C[p + 1][i] < 0) { D_8011119C[p + 1][i] = 0; break; }
	lb	$5, 13($17)
	bge	$5, 0, $367
	.loc	2 481
	.loc	2 481
	sb	$0, 13($17)
	.loc	2 481
	b	$368
$367:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75D0
	beq	$2, 0, $366
$368:
	.loc	2 483
 # 482	            }
 # 483	            func_800C7578(pl->owner, i, 2, D_8011119C[p + 1][i]);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 2
	lb	$7, 13($17)
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 484
 # 484	            D_80111230[p + 1][i] = func_800CDA60(pl->owner, i, 3);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 3
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	.noalias	$18,$16
	.noalias	$18,$17
	.noalias	$18,$sp
	sb	$2, 13($18)
	.loc	2 485
 # 485	            while (!func_800F75B0(p, D_80111230[p + 1][i])) {
	move	$4, $20
	lb	$5, 13($18)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75B0
	bne	$2, 0, $371
$369:
	.loc	2 485
	.loc	2 486
 # 486	                D_80111230[p + 1][i]--;
	lb	$25, 13($18)
	addu	$15, $25, -1
	sb	$15, 13($18)
	.loc	2 487
 # 487	                if (D_80111230[p + 1][i] < 0) { D_80111230[p + 1][i] = 0; break; }
	lb	$5, 13($18)
	bge	$5, 0, $370
	.loc	2 487
	.loc	2 487
	sb	$0, 13($18)
	.loc	2 487
	b	$371
$370:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75B0
	beq	$2, 0, $369
$371:
	.loc	2 489
 # 488	            }
 # 489	            func_800C7578(pl->owner, i, 3, D_80111230[p + 1][i]);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 3
	lb	$7, 13($18)
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 490
 # 490	            D_8011128C[p + 1][i] = func_800CDA60(pl->owner, i, 4);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 4
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	.noalias	$23,$16
	.noalias	$23,$17
	.noalias	$23,$18
	.noalias	$23,$sp
	sb	$2, 13($23)
	.loc	2 491
 # 491	            if (D_8014A110 != 4 && D_8011128C[p + 1][i] > 1) D_8011128C[p + 1][i] = 1;
	lw	$24, D_8014A110
	beq	$24, 4, $372
	lb	$14, 13($23)
	blt	$14, 2, $372
	.loc	2 491
	li	$25, 1
	sb	$25, 13($23)
$372:
	.loc	2 492
 # 492	            D_80111604[p + 1][i] = func_800CDA60(pl->owner, i, 5);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 5
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	lw	$15, 112($sp)
	sb	$2, 13($15)
	.loc	2 493
 # 493	            D_80111648[p + 1][i] = func_800CDA60(pl->owner, i, 6);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 6
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	lw	$24, 104($sp)
	sb	$2, 13($24)
	.loc	2 494
 # 494	            D_8011168C[p + 1][i] = func_800CDA60(pl->owner, i, 7);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 7
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	lw	$14, 96($sp)
	sb	$2, 13($14)
	.loc	2 495
 # 495	            D_8011157C[p + 1][i] = func_800CDA60(pl->owner, i, 8);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 8
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	lw	$25, 88($sp)
	sb	$2, 13($25)
	.loc	2 496
 # 496	            D_801115C0[p + 1][i] = func_800CDA60(pl->owner, i, 9);
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 9
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	lw	$15, 80($sp)
	sb	$2, 13($15)
	.loc	2 497
 # 497	            D_801112DC[p + 1][i] = func_800CDA60(pl->owner, i, 10) / 100.0f + 0.75f;
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 10
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	mtc1	$2, $f6
	cvt.s.w	$f8, $f6
	div.s	$f10, $f8, $f20
	add.s	$f4, $f10, $f22
	lw	$24, 72($sp)
	s.s	$f4, 52($24)
	.loc	2 498
 # 498	            D_801113E0[p + 1][i] = func_800CDA60(pl->owner, i, 11) / 100.0f + 0.75f;
	lw	$4, 72($22)
	and	$5, $19, 255
	li	$6, 11
	.livereg	0x0E00000E,0x00000000
	jal	func_800CDA60
	mtc1	$2, $f6
	cvt.s.w	$f8, $f6
	div.s	$f10, $f8, $f20
	add.s	$f4, $f10, $f22
	lw	$14, 64($sp)
	s.s	$f4, 52($14)
	.loc	2 499
 # 499	            if (!D_801407D0) {
	lb	$25, D_801407D0
	bne	$25, 0, $373
	.loc	2 499
	.loc	2 500
 # 500	                D_801112DC[p + 1][i] = D_801112DC[0][i];
	l.s	$f6, D_801112DC($30)
	lw	$15, 72($sp)
	s.s	$f6, 52($15)
	.loc	2 501
 # 501	                D_801113E0[p + 1][i] = D_801113E0[0][i];
	l.s	$f8, D_801113E0($30)
	lw	$24, 64($sp)
	s.s	$f8, 52($24)
$373:
	.loc	2 470
 # 470	        for (i = 0; i < 13; i++) {
	addu	$21, $21, 1
	addu	$30, $30, 4
	lw	$14, 156($sp)
	addu	$25, $14, 1
	sw	$25, 156($sp)
	addu	$16, $16, 1
	addu	$17, $17, 1
	addu	$18, $18, 1
	addu	$23, $23, 1
	lw	$15, 112($sp)
	addu	$24, $15, 1
	sw	$24, 112($sp)
	lw	$14, 104($sp)
	addu	$25, $14, 1
	sw	$25, 104($sp)
	lw	$15, 96($sp)
	addu	$24, $15, 1
	sw	$24, 96($sp)
	lw	$14, 88($sp)
	addu	$25, $14, 1
	sw	$25, 88($sp)
	lw	$15, 80($sp)
	addu	$24, $15, 1
	sw	$24, 80($sp)
	lw	$14, 72($sp)
	addu	$25, $14, 4
	sw	$25, 72($sp)
	lw	$15, 64($sp)
	addu	$24, $15, 4
	sw	$24, 64($sp)
	blt	$21, 13, $362
	.alias	$16,$17
	.alias	$16,$18
	.alias	$16,$23
	.alias	$16,$sp
	.alias	$17,$18
	.alias	$17,$23
	.alias	$17,$sp
	.alias	$18,$23
	.alias	$18,$sp
	.alias	$23,$sp
	.loc	2 504
 # 504	        D_803B9FD0[p] = func_800CDE88(pl->owner);
	lw	$4, 72($22)
	.livereg	0x0800000E,0x00000000
	jal	func_800CDE88
	lw	$3, 60($sp)
	.noalias	$3,$sp
	sb	$2, 0($3)
	.loc	2 505
 # 505	        while (!func_800F7620(p, D_803B9FD0[p])) {
	move	$4, $20
	lb	$5, 0($3)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7620
	.alias	$3,$sp
	bne	$2, 0, $377
$374:
	.loc	2 505
	.loc	2 506
 # 506	            if (!D_803B9FD0[p]) D_803B9FD0[p] = 12;
	lw	$14, 60($sp)
	lb	$2, 0($14)
	bne	$2, 0, $375
	.loc	2 506
	li	$25, 12
	sb	$25, 0($14)
	b	$376
$375:
	.loc	2 507
 # 507	            else D_803B9FD0[p]--;
	addu	$15, $2, -1
	lw	$24, 60($sp)
	sb	$15, 0($24)
$376:
	move	$4, $20
	lw	$25, 60($sp)
	lb	$5, 0($25)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7620
	beq	$2, 0, $374
$377:
	lw	$2, 56($sp)
	.loc	2 509
 # 508	        }
 # 509	        for (D_803B9FD4[p] = 0; D_803B9FD4[p] < D_803BA020[p]; D_803B9FD4[p]++) {
	.noalias	$2,$sp
	sb	$0, 0($2)
	lb	$4, 0($2)
	lw	$14, 180($sp)
	lh	$3, 0($14)
	bge	$4, $3, $379
	.alias	$2,$sp
	mul	$15, $20, 13
	la	$24, D_803B9FD8
	addu	$5, $15, $24
	lw	$25, 60($sp)
	lb	$2, 0($25)
$378:
	.loc	2 509
	.loc	2 510
 # 510	            if (D_803B9FD0[p] == D_803B9FD8[p][D_803B9FD4[p]]) break;
	.noalias	$5,$sp
	addu	$14, $5, $4
	.noalias	$14,$sp
	lb	$15, 0($14)
	.alias	$14,$sp
	beq	$2, $15, $379
	.alias	$5,$sp
	.loc	2 510
	.loc	2 509
 # 509	        for (D_803B9FD4[p] = 0; D_803B9FD4[p] < D_803BA020[p]; D_803B9FD4[p]++) {
	addu	$24, $4, 1
	lw	$25, 56($sp)
	sb	$24, 0($25)
	lw	$14, 56($sp)
	lb	$4, 0($14)
	blt	$4, $3, $378
$379:
	.loc	2 512
 # 510	            if (D_803B9FD0[p] == D_803B9FD8[p][D_803B9FD4[p]]) break;
 # 511	        }
 # 512	        if (D_803B9FD4[p] >= D_803BA020[p]) {
	blt	$4, $3, $380
	lw	$2, 56($sp)
	.loc	2 512
	.loc	2 513
 # 513	            D_803B9FD4[p] = p + 1;
	.noalias	$2,$sp
	addu	$15, $20, 1
	sb	$15, 0($2)
	.loc	2 514
 # 514	            D_803B9FD0[p] = D_803B9FD8[p][D_803B9FD4[p]];
	mul	$24, $20, 13
	lb	$25, 0($2)
	addu	$14, $24, $25
	lb	$15, D_803B9FD8($14)
	lw	$24, 60($sp)
	sb	$15, 0($24)
	.loc	2 515
 # 515	            func_800CCA04(pl->owner, D_803B9FD0[p]);
	lw	$4, 72($22)
	lw	$25, 60($sp)
	lbu	$5, 0($25)
	.livereg	0x0C00000E,0x00000000
	jal	func_800CCA04
	.alias	$2,$sp
$380:
	.loc	2 517
 # 516	        }
 # 517	        D_803BA018[p] = -1;
	li	$14, -1
	lw	$15, 52($sp)
	sh	$14, 0($15)
	.loc	2 518
 # 518	        func_8039E3BC(p);
	move	$12, $20
	addu	$17, $20, 1
	.livereg	0x0008400E,0x00000000
	jal	func_8039E3BC
	.loc	2 519
 # 519	        for (i = 0; i < D_803BA020[p]; i++) {
	move	$21, $0
	lw	$24, 180($sp)
	lh	$3, 0($24)
	ble	$3, 0, $383
	mul	$25, $20, 156
	la	$14, D_803B9BC8
	addu	$2, $25, $14
	lw	$15, 56($sp)
	lb	$4, 0($15)
	negu	$4, $4
	mul	$24, $20, 13
	la	$25, D_803B9FD8
	addu	$5, $24, $25
	mul	$7, $20, 13
$381:
	.loc	2 519
	.loc	2 520
 # 520	            D_803B9BC8[p][i].position = i - D_803B9FD4[p];
	.noalias	$2,$sp
	sb	$4, 8($2)
	.loc	2 521
 # 521	            if (D_803B9BC8[p][i].position < 0) D_803B9BC8[p][i].position += D_803BA020[p];
	lb	$6, 8($2)
	bge	$6, 0, $382
	.loc	2 521
	addu	$14, $6, $3
	sb	$14, 8($2)
$382:
	.loc	2 522
 # 522	            D_803B9BC8[p][i].car = D_803B9FD8[p][i] + p * 13;
	.noalias	$5,$2
	.noalias	$5,$sp
	lb	$15, 0($5)
	addu	$24, $15, $7
	sb	$24, 9($2)
	.loc	2 523
 # 523	            D_803B9BC8[p][i].angle = 3.1415927f;
	s.s	$f24, 4($2)
	.loc	2 519
 # 519	        for (i = 0; i < D_803BA020[p]; i++) {
	addu	$21, $21, 1
	addu	$2, $2, 12
	addu	$4, $4, 1
	addu	$5, $5, 1
	blt	$21, $3, $381
	.alias	$2,$5
	.alias	$2,$sp
	.alias	$5,$sp
$383:
	.loc	2 525
 # 520	            D_803B9BC8[p][i].position = i - D_803B9FD4[p];
 # 521	            if (D_803B9BC8[p][i].position < 0) D_803B9BC8[p][i].position += D_803BA020[p];
 # 522	            D_803B9BC8[p][i].car = D_803B9FD8[p][i] + p * 13;
 # 523	            D_803B9BC8[p][i].angle = 3.1415927f;
 # 524	        }
 # 525	        if (D_8014A110 == 6) D_8014A118[p].paint = 6;
	lw	$25, D_8014A110
	bne	$25, 6, $384
	.loc	2 525
	li	$14, 6
	sb	$14, 66($22)
	b	$387
$384:
	.loc	2 526
 # 526	        else {
	.loc	2 527
 # 527	            D_8014A118[p].paint = func_800CDDE8((void **)pl->owner);
	lw	$4, 72($22)
	.livereg	0x0800000E,0x00000000
	jal	func_800CDDE8
	sb	$2, 66($22)
	.loc	2 528
 # 528	            while (!func_800F75EC(p, D_8014A118[p].paint)) {
	move	$4, $20
	lbu	$5, 66($22)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75EC
	bne	$2, 0, $387
$385:
	.loc	2 528
	.loc	2 529
 # 529	                D_8014A118[p].paint -= 2;
	lbu	$15, 66($22)
	addu	$24, $15, -2
	sb	$24, 66($22)
	.loc	2 530
 # 530	                if ((s32)D_8014A118[p].paint < 0) { D_8014A118[p].paint += 2; break; }
	lbu	$5, 66($22)
	bge	$5, 0, $386
	.loc	2 530
	.loc	2 530
	addu	$25, $5, 2
	sb	$25, 66($22)
	.loc	2 530
	b	$387
$386:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75EC
	beq	$2, 0, $385
$387:
	.loc	2 533
 # 531	            }
 # 532	        }
 # 533	        for (i = 0; i < 13; i++)
	lw	$4, 48($sp)
	lw	$6, 44($sp)
	lbu	$5, 66($22)
	.noalias	$6,$22
	.noalias	$6,$sp
	sb	$5, 13($6)
	li	$21, 1
	addu	$2, $4, 1
	li	$3, 13
$388:
	.loc	2 534
 # 534	            D_80110E78[p + 1][i] = D_8014A118[p].paint;
	.noalias	$2,$22
	.noalias	$2,$sp
	sb	$5, 13($2)
	.loc	2 533
 # 533	        for (i = 0; i < 13; i++)
	sb	$5, 14($2)
	sb	$5, 15($2)
	sb	$5, 16($2)
	addu	$21, $21, 4
	addu	$2, $2, 4
	bne	$21, $3, $388
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 448
 # 448	    for (p = 0; p < D_8014A108; p++) {
	move	$20, $17
	addu	$22, $22, 76
	lw	$14, 180($sp)
	addu	$15, $14, 2
	sw	$15, 180($sp)
	lw	$24, 172($sp)
	addu	$25, $24, 96
	sw	$25, 172($sp)
	lw	$14, 168($sp)
	addu	$15, $14, 96
	sw	$15, 168($sp)
	lw	$24, 164($sp)
	addu	$25, $24, 8
	sw	$25, 164($sp)
	lw	$14, 160($sp)
	addu	$15, $14, 13
	sw	$15, 160($sp)
	lw	$24, 152($sp)
	addu	$25, $24, 13
	sw	$25, 152($sp)
	lw	$14, 140($sp)
	addu	$15, $14, 13
	sw	$15, 140($sp)
	lw	$24, 132($sp)
	addu	$25, $24, 13
	sw	$25, 132($sp)
	lw	$14, 124($sp)
	addu	$15, $14, 13
	sw	$15, 124($sp)
	lw	$24, 116($sp)
	addu	$25, $24, 13
	sw	$25, 116($sp)
	lw	$14, 108($sp)
	addu	$15, $14, 13
	sw	$15, 108($sp)
	lw	$24, 100($sp)
	addu	$25, $24, 13
	sw	$25, 100($sp)
	lw	$14, 92($sp)
	addu	$15, $14, 13
	sw	$15, 92($sp)
	lw	$24, 84($sp)
	addu	$25, $24, 13
	sw	$25, 84($sp)
	lw	$14, 76($sp)
	addu	$15, $14, 52
	sw	$15, 76($sp)
	lw	$24, 68($sp)
	addu	$25, $24, 52
	sw	$25, 68($sp)
	lw	$14, 60($sp)
	addu	$15, $14, 1
	sw	$15, 60($sp)
	lw	$24, 56($sp)
	addu	$25, $24, 1
	sw	$25, 56($sp)
	lw	$14, 52($sp)
	addu	$15, $14, 2
	sw	$15, 52($sp)
	addu	$4, $4, 13
	addu	$6, $6, 13
	sw	$4, 48($sp)
	sw	$6, 44($sp)
	lh	$24, D_8014A108
	blt	$20, $24, $359
	.alias	$6,$22
	.alias	$6,$sp
$389:
	.loc	2 536
 # 536	    D_803B28AC = sound_control(0, 0, D_803B2AB8, 38);
	move	$4, $0
	move	$5, $0
	la	$6, D_803B2AB8
	li	$7, 38
	.livereg	0x0F00000E,0x00000000
	jal	sound_control
	sw	$2, D_803B28AC
	.loc	2 537
 # 537	    particle_velocity_set();
	.livereg	0x0000000E,0x00000000
	jal	particle_velocity_set
	.loc	2 538
 # 538	    effects_update_emitters();
	.livereg	0x0000000E,0x00000000
	jal	effects_update_emitters
	.loc	2 539
 # 539	    set_race_state();
	.livereg	0x0000000E,0x00000000
	jal	set_race_state
	.loc	2 540
 # 540	    D_803B28A8 = 1;
	li	$25, 1
	sb	$25, D_803B28A8
	.loc	2 541
 # 541	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 36($sp)
	addu	$sp, 208
	j	$31
	.end	func_803A0FD8
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A1EAC
	.loc	2 565
 # 565	{
	.ent	func_803A1EAC 2
func_803A1EAC:
	.option	O3
	subu	$sp, 264
	sw	$31, 116($sp)
	sw	$30, 112($sp)
	sw	$23, 108($sp)
	sw	$22, 104($sp)
	sw	$21, 100($sp)
	sw	$20, 96($sp)
	sw	$19, 92($sp)
	sw	$18, 88($sp)
	sw	$17, 84($sp)
	sw	$16, 80($sp)
	s.d	$f30, 72($sp)
	s.d	$f28, 64($sp)
	s.d	$f26, 56($sp)
	s.d	$f24, 48($sp)
	s.d	$f22, 40($sp)
	s.d	$f20, 32($sp)
	.mask	0xC0FF0000, -148
	.fmask	0xFFF00000, -192
	.frame	$sp, 264, $31
	.loc	2 565
	.loc	2 565
	.loc	2 568
 # 566	    s16 i;
 # 567	    s16 p;
 # 568	    s32 sound = 1;
	li	$14, 1
	sw	$14, 256($sp)
	.loc	2 573
 # 569	    Record **ghosth;
 # 570	    Record *ghost;
 # 571	    Record **nextGhost;
 # 572	
 # 573	    if (!D_80156994) D_803BA010 = D_8012E6F8;
	lb	$15, D_80156994
	bne	$15, 0, $390
	.loc	2 573
	lw	$24, D_8012E6F8
	sw	$24, D_803BA010
	b	$393
$390:
	.loc	2 574
 # 574	    else D_803BA010 = D_8012E6F8 + ((D_80152570 ? 6 : 0) + D_8014978C) * 3;
	lb	$25, D_80152570
	beq	$25, 0, $391
	li	$3, 6
	b	$392
$391:
	move	$3, $0
$392:
	lb	$14, D_8014978C
	addu	$15, $3, $14
	mul	$24, $15, 3
	sll	$25, $24, 2
	lw	$14, D_8012E6F8
	addu	$15, $25, $14
	sw	$15, D_803BA010
$393:
	li	$3, 1
	la	$4, D_801174BC
	.loc	2 575
 # 575	    if (D_801174BC != 1) {
	.noalias	$4,$sp
	lw	$2, 0($4)
	beq	$3, $2, $395
	.loc	2 575
	.loc	2 576
 # 576	        if (D_801174BC != D_801174B4) { func_803A0498(); func_800B5570(0x40000); return; }
	lw	$24, D_801174B4
	beq	$24, $2, $394
	.alias	$4,$sp
	.loc	2 576
	.loc	2 576
	.livereg	0x0000000E,0x00000000
	jal	func_803A0498
	.loc	2 576
	li	$4, 262144
	.livereg	0x0800000E,0x00000000
	jal	func_800B5570
	.loc	2 576
	b	$556
$394:
	.loc	2 577
 # 577	        D_801174BC = 1;
	.noalias	$4,$sp
	sw	$3, 0($4)
	.alias	$4,$sp
$395:
	.loc	2 579
 # 578	    }
 # 579	    for (i = 1; i < 4; i++) {
	li	$16, 1
$396:
	.loc	2 579
	.loc	2 580
 # 580	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	lw	$17, D_80152020+8
	beq	$17, 0, $400
$397:
	.loc	2 580
	.loc	2 581
 # 581	            ghost = *ghosth;
	lw	$2, 0($17)
	.loc	2 582
 # 582	            nextGhost = ghost->next;
	lw	$18, 0($2)
	.loc	2 583
 # 583	            if (ghost->file && !func_800A1A3C((*ghost->file)->controller)) continue;
	lw	$3, 4($2)
	beq	$3, 0, $398
	lw	$25, 0($3)
	lbu	$4, 16($25)
	.livereg	0x0800000E,0x00000000
	jal	func_800A1A3C
	beq	$2, 0, $399
	.loc	2 583
$398:
	.loc	2 584
 # 584	            if (ghosth == D_80152698[i]) break;
	mul	$14, $16, 4
	lw	$15, D_80152698($14)
	beq	$17, $15, $400
	.loc	2 584
	.loc	2 580
 # 580	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
$399:
	move	$17, $18
	bne	$18, 0, $397
$400:
	.loc	2 586
 # 581	            ghost = *ghosth;
 # 582	            nextGhost = ghost->next;
 # 583	            if (ghost->file && !func_800A1A3C((*ghost->file)->controller)) continue;
 # 584	            if (ghosth == D_80152698[i]) break;
 # 585	        }
 # 586	        if (!ghosth) {
	mul	$24, $16, 4
	la	$25, D_80152698
	addu	$4, $24, $25
	bne	$17, 0, $402
	.loc	2 586
	.loc	2 587
 # 587	            for (p = 0; p < 3; p++) {
	move	$23, $0
	.noalias	$4,$sp
	lw	$2, 0($4)
	lw	$3, D_803BA010
$401:
	.loc	2 587
	.loc	2 588
 # 588	                ghosth = D_803BA010 + p;
	sll	$14, $23, 2
	addu	$17, $14, $3
	.loc	2 589
 # 589	                if (ghosth == D_80152698[i]) break;
	beq	$17, $2, $402
	.loc	2 589
	.loc	2 590
 # 590	                ghosth = 0;
	move	$17, $0
	.loc	2 587
 # 587	            for (p = 0; p < 3; p++) {
	addu	$23, $23, 1
	sll	$15, $23, 16
	move	$23, $15
	sra	$24, $23, 16
	move	$23, $24
	blt	$23, 3, $401
$402:
	.loc	2 593
 # 588	                ghosth = D_803BA010 + p;
 # 589	                if (ghosth == D_80152698[i]) break;
 # 590	                ghosth = 0;
 # 591	            }
 # 592	        }
 # 593	        D_80152698[i] = ghosth;
	sw	$17, 0($4)
	.loc	2 579
 # 579	    for (i = 1; i < 4; i++) {
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, 4, $396
	.alias	$4,$sp
	.loc	2 595
 # 595	    if (!D_803B28A8) func_803A0FD8();
	lb	$15, D_803B28A8
	bne	$15, 0, $403
	.loc	2 595
	.livereg	0x0000000E,0x00000000
	jal	func_803A0FD8
$403:
	.loc	2 596
 # 596	    func_8039EF08();
	.livereg	0x0000000E,0x00000000
	jal	func_8039EF08
	.loc	2 597
 # 597	    for (i = 0; i < D_8014A108; i++) {
	move	$16, $0
	lh	$5, D_8014A108
	ble	$5, 0, $406
$404:
	.loc	2 597
	.loc	2 598
 # 598	        if ((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
	mul	$24, $16, 76
	lw	$25, D_8014A118+72($24)
	lw	$14, 0($25)
	lw	$2, 8($14)
	beq	$2, 0, $405
	lw	$15, 0($2)
	lbu	$4, 16($15)
	.livereg	0x0800000E,0x00000000
	jal	func_800A361C
	beq	$2, 0, $405
	.loc	2 598
	.loc	2 599
 # 599	            func_8039D05C(); func_800B5570(32); D_803BA030 = 0; return;
	.livereg	0x0000000E,0x00000000
	jal	func_8039D05C
	.loc	2 599
	li	$4, 32
	.livereg	0x0800000E,0x00000000
	jal	func_800B5570
	.loc	2 599
	sw	$0, D_803BA030
	.loc	2 599
	b	$556
$405:
	.loc	2 597
 # 597	    for (i = 0; i < D_8014A108; i++) {
	addu	$16, $16, 1
	sll	$24, $16, 16
	move	$16, $24
	sra	$25, $16, 16
	move	$16, $25
	lh	$5, D_8014A108
	blt	$16, $5, $404
$406:
	.loc	2 602
 # 598	        if ((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
 # 599	            func_8039D05C(); func_800B5570(32); D_803BA030 = 0; return;
 # 600	        }
 # 601	    }
 # 602	    if (D_80157244) { func_8039D05C(); func_800B5570(4); D_803BA030 = 0; return; }
	lb	$14, D_80157244
	beq	$14, 0, $407
	.loc	2 602
	.loc	2 602
	.livereg	0x0000000E,0x00000000
	jal	func_8039D05C
	.loc	2 602
	li	$4, 4
	.livereg	0x0800000E,0x00000000
	jal	func_800B5570
	.loc	2 602
	sw	$0, D_803BA030
	.loc	2 602
	b	$556
$407:
	la	$2, D_803BA030
	.loc	2 603
 # 603	    D_803BA030++;
	.noalias	$2,$sp
	lw	$15, 0($2)
	addu	$24, $15, 1
	sw	$24, 0($2)
	.loc	2 604
 # 604	    for (p = 0; p < D_8014A108; p++) {
	move	$23, $0
	ble	$5, 0, $543
	li.s	$f20, 5.0E-1
	.alias	$2,$sp
$408:
	.loc	2 604
	.loc	2 615
 # 615	        if (!D_803BA02C && (D_8014A118[p].pressed & 4)) {
	lb	$25, D_803BA02C
	bne	$25, 0, $410
	mul	$14, $23, 76
	lw	$15, D_8014A118+4($14)
	and	$24, $15, 4
	beq	$24, 0, $410
	.loc	2 615
	.loc	2 616
 # 616	            if (D_803BA028[p] == 1) D_803BA028[p] = 0;
	.noalias	$17,$sp
	la	$25, D_803BA028
	addu	$17, $23, $25
	lb	$14, 0($17)
	bne	$14, 1, $409
	.loc	2 616
	sb	$0, 0($17)
	b	$410
	.alias	$17,$sp
$409:
	.loc	2 617
 # 617	            else { entity_flags_apply(38, 0, 1, 0); func_8039D05C(); func_800B5570(128); D_803BA030 = 0; return; }
	.loc	2 617
	li	$4, 38
	move	$5, $0
	li	$6, 1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	.loc	2 617
	.livereg	0x0000000E,0x00000000
	jal	func_8039D05C
	.loc	2 617
	li	$4, 128
	.livereg	0x0800000E,0x00000000
	jal	func_800B5570
	.loc	2 617
	sw	$0, D_803BA030
	.loc	2 617
	b	$556
$410:
	.loc	2 619
 # 618	        }
 # 619	        if (D_803BA028[p]) {
	.noalias	$17,$sp
	la	$15, D_803BA028
	addu	$17, $23, $15
	lb	$24, 0($17)
	beq	$24, 0, $421
	.alias	$17,$sp
	.loc	2 619
	.loc	2 620
 # 620	            msgPos = D_803B3030;
	addu	$25, $sp, 208
	la	$14, D_803B3030
	.set	 noat
	.noalias	$14,$25
	lw	$1, 0($14)
	sw	$1, 0($25)
	lw	$24, 4($14)
	sw	$24, 4($25)
	lw	$1, 8($14)
	sw	$1, 8($25)
	.alias	$14,$25
	.set	 at
	.loc	2 621
 # 621	            if (D_80151AD0 == 1) object_create(13); else object_create(12);
	move	$17, $0
	move	$16, $0
	lh	$15, D_80151AD0
	bne	$15, 1, $411
	.loc	2 621
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$412
$411:
	.loc	2 621
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$412:
	.loc	2 622
 # 622	            brake_light_update(p, msgPos.v, &D_80150B70[p], 0, screenPos);
	move	$4, $23
	addu	$5, $sp, 208
	mul	$25, $23, 152
	la	$14, D_80150B70
	addu	$6, $25, $14
	move	$7, $0
	addu	$24, $sp, 220
	sw	$24, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 623
 # 623	            x = screenPos[0];
	lh	$20, 220($sp)
	.loc	2 624
 # 624	            y = screenPos[1] - object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lh	$15, 222($sp)
	subu	$18, $15, $2
	sll	$25, $18, 16
	move	$18, $25
	sra	$14, $18, 16
	move	$18, $14
	.loc	2 625
 # 625	            notReadyCount = 0;
	.loc	2 626
 # 626	            for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	lh	$5, D_8014A108
	ble	$5, 0, $415
$413:
	.loc	2 626
	.loc	2 627
 # 627	                if (pIdx != p && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
	beq	$23, $16, $414
	lb	$24, D_803BA028($16)
	bne	$24, 0, $414
	.loc	2 627
	.loc	2 627
	sw	$16, 236($sp)
	.loc	2 627
	addu	$17, $17, 1
$414:
	.loc	2 626
 # 626	            for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	addu	$16, $16, 1
	blt	$16, $5, $413
$415:
	.loc	2 629
 # 627	                if (pIdx != p && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
 # 628	            }
 # 629	            if (!notReadyCount) {
	bne	$17, 0, $416
	.loc	2 629
	.loc	2 630
 # 630	                y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	sll	$15, $18, 16
	move	$18, $15
	sra	$25, $18, 16
	move	$18, $25
	.loc	2 631
 # 631	                h = object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	sll	$19, $2, 16
	sra	$14, $19, 16
	move	$19, $14
	.loc	2 632
 # 632	                w = object_manager_update(D_803B857C, -1);
	la	$4, D_803B857C
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$24, $16, 16
	move	$16, $24
	b	$420
$416:
	.loc	2 633
 # 633	            } else {
	.loc	2 634
 # 634	                w = object_manager_update(countdown_state.text[151], -1);
	lw	$15, countdown_state+4
	lw	$4, 604($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$25, $16, 16
	move	$16, $25
	.loc	2 635
 # 635	                h = object_bytes_sum_global() * 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$19, $2, 2
	sll	$14, $19, 16
	move	$19, $14
	sra	$24, $19, 16
	move	$19, $24
	.loc	2 636
 # 636	                if (notReadyCount == 1) w = (w > object_manager_update((*D_8014A118[notReadyIdx].owner)->name, -1) ? w : object_manager_update((*D_8014A118[notReadyIdx].owner)->name, -1));
	bne	$17, 1, $418
	.loc	2 636
	.noalias	$17,$sp
	lw	$15, 236($sp)
	mul	$25, $15, 76
	la	$14, D_8014A118
	addu	$17, $25, $14
	lw	$24, 72($17)
	lw	$4, 0($24)
	addu	$4, $4, 20
	li	$5, -1
	.livereg	0x0C00400E,0x00000000
	jal	object_manager_update
	move	$3, $16
	bgeu	$2, $3, $417
	.alias	$17,$sp
	sll	$16, $3, 16
	sra	$15, $16, 16
	move	$16, $15
	b	$420
$417:
	.noalias	$17,$sp
	lw	$25, 72($17)
	lw	$4, 0($25)
	addu	$4, $4, 20
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	.alias	$17,$sp
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
	b	$420
$418:
	.loc	2 637
 # 637	                else w = (w > object_manager_update(countdown_state.text[31], -1) ? w : object_manager_update(countdown_state.text[31], -1));
	lw	$24, countdown_state+4
	lw	$4, 124($24)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $16
	bgeu	$2, $3, $419
	sll	$16, $3, 16
	sra	$15, $16, 16
	move	$16, $15
	b	$420
$419:
	lw	$25, countdown_state+4
	lw	$4, 124($25)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
$420:
	.loc	2 639
 # 638	            }
 # 639	            y -= object_bytes_sum_global() / 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	div	$24, $2, 2
	subu	$18, $18, $24
	sll	$15, $18, 16
	move	$18, $15
	sra	$25, $18, 16
	move	$18, $25
	.loc	2 640
 # 640	            crowd_cheer_play(D_803B9FC0[p], x - w / 2 - 4, y - 4, x + w / 2 + 4, y + h + 4);
	mul	$14, $23, 4
	lw	$4, D_803B9FC0($14)
	div	$3, $16, 2
	subu	$5, $20, $3
	addu	$5, $5, -4
	addu	$6, $18, -4
	addu	$7, $20, $3
	addu	$7, $7, 4
	addu	$24, $18, $19
	addu	$15, $24, 4
	sw	$15, 16($sp)
	.livereg	0x1F00000E,0x00000000
	jal	crowd_cheer_play
	.loc	2 641
 # 641	            func_8039D6A4(p);
	sll	$30, $23, 24
	sra	$25, $30, 24
	move	$30, $25
	sh	$23, 260($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_8039D6A4
	lh	$23, 260($sp)
	li.s	$f20, 5.0E-1
	.loc	2 642
 # 642	            continue;
	b	$542
$421:
	.loc	2 644
 # 643	        }
 # 644	        crowd_cheer_play(D_803B9FC0[p], -20, 0, -10, 10);
	mul	$30, $23, 4
	lw	$4, D_803B9FC0($30)
	li	$5, -20
	move	$6, $0
	li	$7, -10
	li	$14, 10
	sw	$14, 16($sp)
	mul	$24, $23, 76
	la	$15, D_8014A118
	addu	$22, $24, $15
	.livereg	0x0F00020E,0x00000000
	jal	crowd_cheer_play
	.loc	2 645
 # 645	        if (D_8014A118[p].pressed & 3) {
	.noalias	$22,$sp
	lw	$4, 4($22)
	and	$25, $4, 3
	beq	$25, 0, $424
	.loc	2 645
	.loc	2 646
 # 646	            resource_type_select(D_8014A118[p].pressed);
	.livereg	0x0800000E,0x00000000
	jal	resource_type_select
	.loc	2 647
 # 647	            D_803BA028[p] = 1;
	.noalias	$17,$22
	.noalias	$17,$sp
	li	$14, 1
	sb	$14, 0($17)
	.loc	2 648
 # 648	            D_80153E88[p].flags = 160;
	.noalias	$2,$17
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$24, $23, 8
	la	$15, D_80153E88
	addu	$2, $24, $15
	li	$25, 160
	sb	$25, 6($2)
	.loc	2 649
 # 649	            D_80153E88[p].car = D_803B9FD0[p];
	lb	$16, D_803B9FD0($23)
	sb	$16, 1($2)
	.loc	2 650
 # 650	            if (D_8014A110 == 6 && D_801164BE == 0) {
	lw	$14, D_8014A110
	bne	$14, 6, $422
	.alias	$17,$2
	.alias	$17,$22
	.alias	$17,$sp
	lh	$24, D_801164BE
	bne	$24, 0, $422
	.loc	2 650
	.loc	2 651
 # 651	                D_80153E88[p].color[0] = D_80138664[D_8012E67C[p]];
	lb	$15, D_8012E67C($23)
	lb	$3, D_80138664($15)
	sb	$3, 2($2)
	.loc	2 652
 # 652	                D_80153E88[p].color[1] = D_80138664[D_8012E67C[p]];
	sb	$3, 3($2)
	.loc	2 653
 # 653	                D_80153E88[p].color[2] = D_80138664[D_8012E67C[p]];
	sb	$3, 4($2)
	b	$423
$422:
	.loc	2 654
 # 654	            } else {
	.loc	2 655
 # 655	                D_80153E88[p].color[0] = D_80111604[p + 1][D_803B9FD0[p]];
	mul	$25, $23, 13
	addu	$14, $25, $16
	lb	$24, D_80111604+13($14)
	sb	$24, 2($2)
	.loc	2 656
 # 656	                D_80153E88[p].color[1] = D_80111648[p + 1][D_803B9FD0[p]];
	mul	$15, $23, 13
	addu	$25, $15, $16
	lb	$14, D_80111648+13($25)
	sb	$14, 3($2)
	.loc	2 657
 # 657	                D_80153E88[p].color[2] = D_8011168C[p + 1][D_803B9FD0[p]];
	mul	$24, $23, 13
	addu	$15, $24, $16
	lb	$25, D_8011168C+13($15)
	sb	$25, 4($2)
	.alias	$2,$22
	.alias	$2,$sp
$423:
	.loc	2 659
 # 658	            }
 # 659	            D_803BA030 = 0;
	sw	$0, D_803BA030
$424:
	.loc	2 661
 # 660	        }
 # 661	        if (D_8014A118[p].repeated & 0x400) {
	lw	$2, 12($22)
	la	$14, D_803B9FD0
	addu	$21, $23, $14
	and	$24, $2, 1024
	beq	$24, 0, $425
	.loc	2 661
	.loc	2 662
 # 662	            entity_flags_apply(40, 0, 1, 0);
	li	$4, 40
	move	$5, $0
	li	$6, 1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	.loc	2 663
 # 663	            D_803B28B8[p]--;
	.noalias	$3,$22
	.noalias	$3,$sp
	la	$15, D_803B28B8
	addu	$3, $30, $15
	lw	$25, 0($3)
	addu	$14, $25, -1
	sw	$14, 0($3)
	.loc	2 664
 # 664	            if (D_803B28B8[p] < 0) D_803B28B8[p] = D_803BA050[p] - 1;
	lw	$24, 0($3)
	bge	$24, 0, $525
	.loc	2 664
	lw	$15, D_803BA050($30)
	addu	$25, $15, -1
	sw	$25, 0($3)
	.alias	$3,$22
	.alias	$3,$sp
	b	$525
$425:
	.loc	2 665
 # 665	        } else if (D_8014A118[p].repeated & 0x800) {
	and	$14, $2, 2048
	beq	$14, 0, $426
	.loc	2 665
	.loc	2 666
 # 666	            entity_flags_apply(39, 0, 1, 0);
	li	$4, 39
	move	$5, $0
	li	$6, 1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	.loc	2 667
 # 667	            D_803B28B8[p]++;
	.noalias	$3,$22
	.noalias	$3,$sp
	la	$24, D_803B28B8
	addu	$3, $30, $24
	lw	$15, 0($3)
	addu	$25, $15, 1
	sw	$25, 0($3)
	.loc	2 668
 # 668	            if (D_803B28B8[p] > D_803BA050[p] - 1) D_803B28B8[p] = 0;
	lw	$14, 0($3)
	lw	$24, D_803BA050($30)
	blt	$14, $24, $525
	.loc	2 668
	sw	$0, 0($3)
	.alias	$3,$22
	.alias	$3,$sp
	b	$525
$426:
	.loc	2 669
 # 669	        } else if (D_8014A118[p].repeated & 0x3080) {
	and	$15, $2, 12416
	beq	$15, 0, $525
	.loc	2 669
	.loc	2 670
 # 670	            s32 d = (D_8014A118[p].repeated & 0x1000) ? -1 : ((D_8014A118[p].repeated & 0x2000) ? 1 : 0);
	and	$25, $2, 4096
	beq	$25, 0, $427
	li	$19, -1
	b	$430
$427:
	and	$14, $2, 8192
	beq	$14, 0, $428
	li	$3, 1
	b	$429
$428:
	move	$3, $0
$429:
	move	$19, $3
$430:
	.loc	2 671
 # 671	            if (!D_803BA048[p] || (!d && !D_803BA04C[p])) {
	lb	$24, D_803BA048($23)
	beq	$24, 0, $431
	bne	$19, 0, $432
	lb	$15, D_803BA04C($23)
	bne	$15, 0, $432
$431:
	.loc	2 671
	.loc	2 672
 # 672	                entity_flags_apply(42, 0, 1, 0);
	li	$4, 42
	move	$5, $0
	li	$6, 1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	.loc	2 673
 # 673	                goto after_options;
	.noalias	$21,$22
	.noalias	$21,$sp
	lb	$16, 0($21)
	mul	$6, $23, 2
	b	$530
$432:
	.loc	2 675
 # 674	            }
 # 675	            switch (D_803BA060[p][D_803B28B8[p]]) {
	mul	$25, $23, 76
	lw	$14, D_803B28B8($30)
	mul	$24, $14, 4
	addu	$15, $25, $24
	lw	$14, D_803BA060($15)
	bgeu	$14, 19, $524
	sll	$14, $14, 2
	lw	$14, $433($14)
	j	$14
	.rdata	
$433:
	.word	$435
	.word	$443
	.word	$451
	.word	$455
	.word	$461
	.word	$467
	.word	$473
	.word	$481
	.word	$485
	.word	$489
	.word	$493
	.word	$501
	.word	$505
	.word	$509
	.word	$513
	.word	$517
	.word	$521
	.word	$522
	.word	$523
	.text	
$435:
	.loc	2 677
 # 676	            case 0:
 # 677	                if (d) {
	move	$16, $0
	beq	$19, 0, $439
	.loc	2 677
	.loc	2 678
 # 678	                    do {
	la	$25, D_803B9FD4
	addu	$17, $23, $25
	mul	$24, $23, 2
	la	$15, D_803BA020
	addu	$18, $24, $15
	mul	$14, $23, 13
	la	$25, D_803B9FD8
	addu	$20, $14, $25
$436:
	.loc	2 678
	.loc	2 679
 # 679	                        D_803B9FD4[p] += d;
	.noalias	$17,$21
	.noalias	$17,$22
	.noalias	$17,$sp
	lb	$24, 0($17)
	addu	$15, $24, $19
	sb	$15, 0($17)
	.loc	2 680
 # 680	                        if (D_803B9FD4[p] < 0) D_803B9FD4[p] = D_803BA020[p] - 1;
	lb	$2, 0($17)
	bge	$2, 0, $437
	.loc	2 680
	.noalias	$18,$17
	.noalias	$18,$21
	.noalias	$18,$22
	.noalias	$18,$sp
	lh	$14, 0($18)
	addu	$25, $14, -1
	sb	$25, 0($17)
	lb	$2, 0($17)
	b	$438
$437:
	.loc	2 681
 # 681	                        else if (D_803B9FD4[p] >= D_803BA020[p]) D_803B9FD4[p] = 0;
	lh	$24, 0($18)
	blt	$2, $24, $438
	.loc	2 681
	sb	$0, 0($17)
	lb	$2, 0($17)
$438:
	move	$4, $23
	.noalias	$20,$17
	.noalias	$20,$18
	.noalias	$20,$21
	.noalias	$20,$22
	.noalias	$20,$sp
	addu	$15, $20, $2
	.noalias	$15,$17
	.noalias	$15,$18
	.noalias	$15,$21
	.noalias	$15,$22
	.noalias	$15,$sp
	lb	$5, 0($15)
	.alias	$15,$17
	.alias	$15,$18
	.alias	$15,$21
	.alias	$15,$22
	.alias	$15,$sp
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7620
	beq	$2, 0, $436
	.alias	$17,$18
	.alias	$17,$20
	.alias	$17,$21
	.alias	$17,$22
	.alias	$17,$sp
	.alias	$18,$20
	.alias	$18,$21
	.alias	$18,$22
	.alias	$18,$sp
	.alias	$20,$21
	.alias	$20,$22
	.alias	$20,$sp
$439:
	.loc	2 684
 # 682	                    } while (!func_800F7620(p, D_803B9FD8[p][D_803B9FD4[p]]));
 # 683	                }
 # 684	                D_803B9FD0[p] = D_803B9FD8[p][D_803B9FD4[p]];
	mul	$14, $23, 13
	lb	$25, D_803B9FD4($23)
	addu	$24, $14, $25
	lb	$15, D_803B9FD8($24)
	sb	$15, 0($21)
	.loc	2 685
 # 685	                if (D_8014A110 == 6) D_8014A118[p].paint = 6;
	lw	$14, D_8014A110
	bne	$14, 6, $440
	.loc	2 685
	li	$25, 6
	sb	$25, 66($22)
	b	$441
$440:
	.loc	2 686
 # 686	                else D_8014A118[p].paint = func_800CDDE8((void **)D_8014A118[p].owner);
	lw	$4, 72($22)
	.livereg	0x0800000E,0x00000000
	jal	func_800CDDE8
	sb	$2, 66($22)
$441:
	.loc	2 687
 # 687	                for (i = 0; i < 13; i++) D_80110E78[p + 1][i] = D_8014A118[p].paint;
	mul	$24, $23, 13
	la	$15, D_80110E78
	addu	$17, $24, $15
	lbu	$2, 66($22)
$442:
	.loc	2 687
	.noalias	$17,$21
	.noalias	$17,$22
	.noalias	$17,$sp
	addu	$14, $17, $16
	.noalias	$14,$21
	.noalias	$14,$22
	.noalias	$14,$sp
	sb	$2, 13($14)
	.alias	$14,$21
	.alias	$14,$22
	.alias	$14,$sp
	.loc	2 687
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, 13, $442
	.alias	$17,$21
	.alias	$17,$22
	.alias	$17,$sp
	.loc	2 688
 # 688	                func_8039E3BC(p);
	move	$12, $23
	.livereg	0x0008000E,0x00000000
	jal	func_8039E3BC
	.loc	2 689
 # 689	                D_803B9BA0[p] = 1;
	li	$15, 1
	sw	$15, D_803B9BA0($30)
	.loc	2 690
 # 690	                break;
	b	$524
$443:
	.loc	2 692
 # 691	            case 1:
 # 692	                if (D_8014A110 != 6) {
	lw	$14, D_8014A110
	beq	$14, 6, $450
	.loc	2 692
	.loc	2 693
 # 693	                    if (d) {
	beq	$19, 0, $448
	.loc	2 693
	.loc	2 694
 # 694	                        do {
	mul	$25, $23, 13
	la	$24, D_80110E78
	addu	$17, $25, $24
$444:
	.loc	2 694
	.loc	2 695
 # 695	                            D_80110E78[p + 1][D_803B9FD0[p]] += d;
	.noalias	$17,$21
	.noalias	$17,$22
	.noalias	$17,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$15, 0($21)
	addu	$2, $17, $15
	lb	$14, 13($2)
	addu	$25, $14, $19
	sb	$25, 13($2)
	.loc	2 696
 # 696	                            if (D_80110E78[p + 1][D_803B9FD0[p]] < 0) D_80110E78[p + 1][D_803B9FD0[p]] = 7;
	lb	$6, 13($2)
	bge	$6, 0, $445
	.loc	2 696
	li	$24, 7
	sb	$24, 13($2)
	lb	$6, 13($2)
	b	$446
$445:
	.loc	2 697
 # 697	                            else if (D_80110E78[p + 1][D_803B9FD0[p]] >= 8) D_80110E78[p + 1][D_803B9FD0[p]] = 0;
	blt	$6, 8, $446
	.loc	2 697
	sb	$0, 13($2)
	lb	$6, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$446:
	move	$4, $23
	move	$5, $6
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75EC
	beq	$2, 0, $444
	lw	$15, D_8014A110
	bne	$15, 6, $447
	lb	$14, 0($21)
	addu	$25, $17, $14
	.noalias	$25,$21
	.noalias	$25,$22
	.noalias	$25,$sp
	lb	$24, 13($25)
	.alias	$25,$21
	.alias	$25,$22
	.alias	$25,$sp
	and	$15, $24, 1
	bne	$15, 0, $444
$447:
	lb	$14, 0($21)
	addu	$25, $17, $14
	.noalias	$25,$21
	.noalias	$25,$22
	.noalias	$25,$sp
	lb	$6, 13($25)
	.alias	$25,$21
	.alias	$25,$22
	.alias	$25,$sp
	b	$449
	.alias	$17,$21
	.alias	$17,$22
	.alias	$17,$sp
$448:
	la	$3, D_80110E78
	.loc	2 699
 # 698	                        } while (!func_800F75EC(p, D_80110E78[p + 1][D_803B9FD0[p]]) || (D_8014A110 == 6 && (D_80110E78[p + 1][D_803B9FD0[p]] & 1)));
 # 699	                    } else D_80110E78[p + 1][D_803B9FD0[p]] = D_80110E78[0][D_803B9FD0[p]];
	lb	$16, 0($21)
	.noalias	$3,$21
	.noalias	$3,$22
	.noalias	$3,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$24, $23, 13
	addu	$15, $3, $24
	addu	$2, $15, $16
	addu	$14, $3, $16
	.noalias	$14,$21
	.noalias	$14,$22
	.noalias	$14,$sp
	lb	$25, 0($14)
	.alias	$14,$21
	.alias	$14,$22
	.alias	$14,$sp
	sb	$25, 13($2)
	lb	$6, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	.alias	$3,$21
	.alias	$3,$22
	.alias	$3,$sp
$449:
	.loc	2 700
 # 700	                    D_8014A118[p].paint = D_80110E78[p + 1][D_803B9FD0[p]];
	sb	$6, 66($22)
	.loc	2 701
 # 701	                    func_800CDCAC(D_8014A118[p].owner, D_8014A118[p].paint);
	lw	$4, 72($22)
	lbu	$5, 66($22)
	.livereg	0x0C00000E,0x00000000
	jal	func_800CDCAC
	b	$524
$450:
	.loc	2 702
 # 702	                } else sound = 0;
	sw	$0, 256($sp)
	.loc	2 703
 # 703	                break;
	b	$524
$451:
	.loc	2 705
 # 704	            case 2:
 # 705	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $453
	.loc	2 705
	.loc	2 706
 # 706	                    D_8011103C[p + 1][D_803B9FD0[p]] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$24, $23, 13
	addu	$15, $24, $16
	la	$14, D_8011103C
	addu	$2, $15, $14
	lb	$25, 13($2)
	addu	$24, $25, $19
	sb	$24, 13($2)
	.loc	2 707
 # 707	                    if (D_8011103C[p + 1][D_803B9FD0[p]] < 0) D_8011103C[p + 1][D_803B9FD0[p]] = 2;
	lb	$7, 13($2)
	bge	$7, 0, $452
	.loc	2 707
	li	$15, 2
	sb	$15, 13($2)
	lb	$7, 13($2)
	b	$454
$452:
	.loc	2 708
 # 708	                    else if (D_8011103C[p + 1][D_803B9FD0[p]] > 2) D_8011103C[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 3, $454
	.loc	2 708
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$454
$453:
	.loc	2 709
 # 709	                } else D_8011103C[p + 1][D_803B9FD0[p]] = D_8011103C[0][D_803B9FD0[p]];
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$14, $23, 13
	addu	$25, $14, $16
	la	$24, D_8011103C
	addu	$2, $25, $24
	addu	$15, $16, $24
	lb	$14, 0($15)
	sb	$14, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$454:
	.loc	2 710
 # 710	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 0, D_8011103C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	move	$6, $0
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 711
 # 711	                break;
	b	$524
$455:
	.loc	2 713
 # 712	            case 3:
 # 713	                if (d) {
	beq	$19, 0, $459
	.loc	2 713
	.loc	2 714
 # 714	                    do {
	mul	$25, $23, 13
	la	$24, D_80111080
	addu	$16, $25, $24
$456:
	.loc	2 714
	.loc	2 715
 # 715	                        D_80111080[p + 1][D_803B9FD0[p]] += d;
	.noalias	$16,$21
	.noalias	$16,$22
	.noalias	$16,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$15, 0($21)
	addu	$2, $16, $15
	lb	$14, 13($2)
	addu	$25, $14, $19
	sb	$25, 13($2)
	.loc	2 716
 # 716	                        if (D_80111080[p + 1][D_803B9FD0[p]] < 0) D_80111080[p + 1][D_803B9FD0[p]] = 8;
	lb	$5, 13($2)
	bge	$5, 0, $457
	.loc	2 716
	li	$24, 8
	sb	$24, 13($2)
	lb	$5, 13($2)
	b	$458
$457:
	.loc	2 717
 # 717	                        else if (D_80111080[p + 1][D_803B9FD0[p]] > 8) D_80111080[p + 1][D_803B9FD0[p]] = 0;
	blt	$5, 9, $458
	.loc	2 717
	sb	$0, 13($2)
	lb	$5, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$458:
	move	$4, $23
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7604
	beq	$2, 0, $456
	lb	$8, 0($21)
	lb	$15, 0($21)
	addu	$2, $16, $15
	b	$460
	.alias	$16,$21
	.alias	$16,$22
	.alias	$16,$sp
$459:
	la	$3, D_80111080
	.loc	2 719
 # 718	                    } while (!func_800F7604(p, D_80111080[p + 1][D_803B9FD0[p]]));
 # 719	                } else D_80111080[p + 1][D_803B9FD0[p]] = D_80111080[0][D_803B9FD0[p]];
	lb	$16, 0($21)
	.noalias	$3,$21
	.noalias	$3,$22
	.noalias	$3,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$14, $23, 13
	addu	$25, $3, $14
	addu	$2, $25, $16
	addu	$24, $3, $16
	.noalias	$24,$21
	.noalias	$24,$22
	.noalias	$24,$sp
	lb	$15, 0($24)
	.alias	$24,$21
	.alias	$24,$22
	.alias	$24,$sp
	sb	$15, 13($2)
	move	$8, $16
	.alias	$3,$21
	.alias	$3,$22
	.alias	$3,$sp
$460:
	.loc	2 720
 # 720	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 1, D_80111080[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 1
	lb	$7, 13($2)
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 721
 # 721	                break;
	b	$524
$461:
	.loc	2 723
 # 722	            case 4:
 # 723	                if (d) {
	beq	$19, 0, $465
	.loc	2 723
	.loc	2 724
 # 724	                    do {
	mul	$14, $23, 13
	la	$25, D_8011119C
	addu	$16, $14, $25
$462:
	.loc	2 724
	.loc	2 725
 # 725	                        D_8011119C[p + 1][D_803B9FD0[p]] += d;
	.noalias	$16,$21
	.noalias	$16,$22
	.noalias	$16,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$24, 0($21)
	addu	$2, $16, $24
	lb	$15, 13($2)
	addu	$14, $15, $19
	sb	$14, 13($2)
	.loc	2 726
 # 726	                        if (D_8011119C[p + 1][D_803B9FD0[p]] < 0) D_8011119C[p + 1][D_803B9FD0[p]] = 4;
	lb	$5, 13($2)
	bge	$5, 0, $463
	.loc	2 726
	li	$25, 4
	sb	$25, 13($2)
	lb	$5, 13($2)
	b	$464
$463:
	.loc	2 727
 # 727	                        else if (D_8011119C[p + 1][D_803B9FD0[p]] > 4) D_8011119C[p + 1][D_803B9FD0[p]] = 0;
	blt	$5, 5, $464
	.loc	2 727
	sb	$0, 13($2)
	lb	$5, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$464:
	move	$4, $23
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75D0
	beq	$2, 0, $462
	lb	$8, 0($21)
	lb	$24, 0($21)
	addu	$2, $16, $24
	b	$466
	.alias	$16,$21
	.alias	$16,$22
	.alias	$16,$sp
$465:
	la	$3, D_8011119C
	.loc	2 729
 # 728	                    } while (!func_800F75D0(p, D_8011119C[p + 1][D_803B9FD0[p]]));
 # 729	                } else D_8011119C[p + 1][D_803B9FD0[p]] = D_8011119C[0][D_803B9FD0[p]];
	lb	$16, 0($21)
	.noalias	$3,$21
	.noalias	$3,$22
	.noalias	$3,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$15, $23, 13
	addu	$14, $3, $15
	addu	$2, $14, $16
	addu	$25, $3, $16
	.noalias	$25,$21
	.noalias	$25,$22
	.noalias	$25,$sp
	lb	$24, 0($25)
	.alias	$25,$21
	.alias	$25,$22
	.alias	$25,$sp
	sb	$24, 13($2)
	move	$8, $16
	.alias	$3,$21
	.alias	$3,$22
	.alias	$3,$sp
$466:
	.loc	2 730
 # 730	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 2, D_8011119C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 2
	lb	$7, 13($2)
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 731
 # 731	                break;
	b	$524
$467:
	.loc	2 733
 # 732	            case 5:
 # 733	                if (d) {
	beq	$19, 0, $471
	.loc	2 733
	.loc	2 734
 # 734	                    do {
	mul	$15, $23, 13
	la	$14, D_80111230
	addu	$16, $15, $14
$468:
	.loc	2 734
	.loc	2 735
 # 735	                        D_80111230[p + 1][D_803B9FD0[p]] += d;
	.noalias	$16,$21
	.noalias	$16,$22
	.noalias	$16,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$25, 0($21)
	addu	$2, $16, $25
	lb	$24, 13($2)
	addu	$15, $24, $19
	sb	$15, 13($2)
	.loc	2 736
 # 736	                        if (D_80111230[p + 1][D_803B9FD0[p]] < 0) D_80111230[p + 1][D_803B9FD0[p]] = 5;
	lb	$5, 13($2)
	bge	$5, 0, $469
	.loc	2 736
	li	$14, 5
	sb	$14, 13($2)
	lb	$5, 13($2)
	b	$470
$469:
	.loc	2 737
 # 737	                        else if (D_80111230[p + 1][D_803B9FD0[p]] > 5) D_80111230[p + 1][D_803B9FD0[p]] = 0;
	blt	$5, 6, $470
	.loc	2 737
	sb	$0, 13($2)
	lb	$5, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$470:
	move	$4, $23
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75B0
	beq	$2, 0, $468
	lb	$8, 0($21)
	lb	$25, 0($21)
	addu	$2, $16, $25
	b	$472
	.alias	$16,$21
	.alias	$16,$22
	.alias	$16,$sp
$471:
	la	$3, D_80111230
	.loc	2 739
 # 738	                    } while (!func_800F75B0(p, D_80111230[p + 1][D_803B9FD0[p]]));
 # 739	                } else D_80111230[p + 1][D_803B9FD0[p]] = D_80111230[0][D_803B9FD0[p]];
	lb	$16, 0($21)
	.noalias	$3,$21
	.noalias	$3,$22
	.noalias	$3,$sp
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$24, $23, 13
	addu	$15, $3, $24
	addu	$2, $15, $16
	addu	$14, $3, $16
	.noalias	$14,$21
	.noalias	$14,$22
	.noalias	$14,$sp
	lb	$25, 0($14)
	.alias	$14,$21
	.alias	$14,$22
	.alias	$14,$sp
	sb	$25, 13($2)
	move	$8, $16
	.alias	$3,$21
	.alias	$3,$22
	.alias	$3,$sp
$472:
	.loc	2 740
 # 740	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 3, D_80111230[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 3
	lb	$7, 13($2)
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 741
 # 741	                break;
	b	$524
$473:
	.loc	2 743
 # 742	            case 6:
 # 743	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $479
	.loc	2 743
	.loc	2 744
 # 744	                    D_8011128C[p + 1][D_803B9FD0[p]] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$24, $23, 13
	addu	$15, $24, $16
	la	$14, D_8011128C
	addu	$2, $15, $14
	lb	$25, 13($2)
	addu	$24, $25, $19
	sb	$24, 13($2)
	.loc	2 745
 # 745	                    if (D_8011128C[p + 1][D_803B9FD0[p]] < 0) D_8011128C[p + 1][D_803B9FD0[p]] = (D_8014A110 != 4 ? 1 : 2);
	lb	$7, 13($2)
	lw	$3, D_8014A110
	bge	$7, 0, $476
	.loc	2 745
	beq	$3, 4, $474
	li	$15, 1
	sb	$15, 13($2)
	b	$475
$474:
	li	$14, 2
	sb	$14, 13($2)
$475:
	lb	$7, 13($2)
	b	$480
$476:
	.loc	2 746
 # 746	                    else if (D_8011128C[p + 1][D_803B9FD0[p]] > (D_8014A110 != 4 ? 1 : 2)) D_8011128C[p + 1][D_803B9FD0[p]] = 0;
	beq	$3, 4, $477
	li	$3, 1
	b	$478
$477:
	li	$3, 2
$478:
	bge	$3, $7, $480
	.loc	2 746
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$480
$479:
	.loc	2 747
 # 747	                } else D_8011128C[p + 1][D_803B9FD0[p]] = D_8011128C[0][D_803B9FD0[p]];
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$25, $23, 13
	addu	$24, $25, $16
	la	$15, D_8011128C
	addu	$2, $24, $15
	addu	$14, $16, $15
	lb	$25, 0($14)
	sb	$25, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$480:
	.loc	2 748
 # 748	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 4, D_8011128C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 4
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 749
 # 749	                break;
	b	$524
$481:
	.loc	2 751
 # 750	            case 7:
 # 751	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	mul	$24, $23, 13
	addu	$15, $24, $16
	la	$14, D_80111604
	addu	$2, $15, $14
	beq	$19, 0, $483
	.loc	2 751
	.loc	2 752
 # 752	                    D_80111604[p + 1][D_803B9FD0[p]] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$25, 13($2)
	addu	$24, $25, $19
	sb	$24, 13($2)
	.loc	2 753
 # 753	                    if (D_80111604[p + 1][D_803B9FD0[p]] < 0) D_80111604[p + 1][D_803B9FD0[p]] = 31;
	lb	$7, 13($2)
	bge	$7, 0, $482
	.loc	2 753
	li	$15, 31
	sb	$15, 13($2)
	lb	$7, 13($2)
	b	$484
$482:
	.loc	2 754
 # 754	                    else if (D_80111604[p + 1][D_803B9FD0[p]] >= 32) D_80111604[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 32, $484
	.loc	2 754
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$484
$483:
	.loc	2 755
 # 755	                } else D_80111604[p + 1][D_803B9FD0[p]] = D_80111604[0][D_803B9FD0[p]];
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$14, D_80111604($16)
	sb	$14, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$484:
	.loc	2 756
 # 756	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 5, D_80111604[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 5
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 757
 # 757	                break;
	b	$524
$485:
	.loc	2 759
 # 758	            case 8:
 # 759	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	mul	$25, $23, 13
	addu	$24, $25, $16
	la	$15, D_80111648
	addu	$2, $24, $15
	beq	$19, 0, $487
	.loc	2 759
	.loc	2 760
 # 760	                    D_80111648[p + 1][D_803B9FD0[p]] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$14, 13($2)
	addu	$25, $14, $19
	sb	$25, 13($2)
	.loc	2 761
 # 761	                    if (D_80111648[p + 1][D_803B9FD0[p]] < 0) D_80111648[p + 1][D_803B9FD0[p]] = 31;
	lb	$7, 13($2)
	bge	$7, 0, $486
	.loc	2 761
	li	$24, 31
	sb	$24, 13($2)
	lb	$7, 13($2)
	b	$488
$486:
	.loc	2 762
 # 762	                    else if (D_80111648[p + 1][D_803B9FD0[p]] >= 32) D_80111648[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 32, $488
	.loc	2 762
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$488
$487:
	.loc	2 763
 # 763	                } else D_80111648[p + 1][D_803B9FD0[p]] = D_80111648[0][D_803B9FD0[p]];
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$15, D_80111648($16)
	sb	$15, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$488:
	.loc	2 764
 # 764	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 6, D_80111648[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 6
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 765
 # 765	                break;
	b	$524
$489:
	.loc	2 767
 # 766	            case 9:
 # 767	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	mul	$14, $23, 13
	addu	$25, $14, $16
	la	$24, D_8011168C
	addu	$2, $25, $24
	beq	$19, 0, $491
	.loc	2 767
	.loc	2 768
 # 768	                    D_8011168C[p + 1][D_803B9FD0[p]] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$15, 13($2)
	addu	$14, $15, $19
	sb	$14, 13($2)
	.loc	2 769
 # 769	                    if (D_8011168C[p + 1][D_803B9FD0[p]] < 0) D_8011168C[p + 1][D_803B9FD0[p]] = 31;
	lb	$7, 13($2)
	bge	$7, 0, $490
	.loc	2 769
	li	$25, 31
	sb	$25, 13($2)
	lb	$7, 13($2)
	b	$492
$490:
	.loc	2 770
 # 770	                    else if (D_8011168C[p + 1][D_803B9FD0[p]] >= 32) D_8011168C[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 32, $492
	.loc	2 770
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$492
$491:
	.loc	2 771
 # 771	                } else D_8011168C[p + 1][D_803B9FD0[p]] = D_8011168C[0][D_803B9FD0[p]];
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$24, D_8011168C($16)
	sb	$24, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$492:
	.loc	2 772
 # 772	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 7, D_8011168C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 7
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 773
 # 773	                break;
	b	$524
$493:
	.loc	2 775
 # 774	            case 10:
 # 775	                if (d) {
	la	$15, D_8012E67C
	addu	$2, $23, $15
	beq	$19, 0, $500
	.loc	2 775
	.loc	2 776
 # 776	                    s32 teamFull = 1;
	li	$3, 1
	.loc	2 777
 # 777	                    do {
	lh	$5, D_8014A108
$494:
	.loc	2 777
	.loc	2 778
 # 778	                        D_8012E67C[p] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	lb	$14, 0($2)
	addu	$25, $14, $19
	sb	$25, 0($2)
	.loc	2 779
 # 779	                        if (D_8012E67C[p] < 0) D_8012E67C[p] = 3;
	lb	$4, 0($2)
	move	$16, $0
	bge	$4, 0, $495
	.loc	2 779
	li	$24, 3
	sb	$24, 0($2)
	b	$496
$495:
	.loc	2 780
 # 780	                        else if (D_8012E67C[p] > 3) D_8012E67C[p] = 0;
	blt	$4, 4, $496
	.loc	2 780
	sb	$0, 0($2)
$496:
	.loc	2 781
 # 781	                        for (i = 0; i < D_8014A108; i++) {
	ble	$5, 0, $499
$497:
	.loc	2 781
	.loc	2 782
 # 782	                            if (i != p && D_8012E67C[i] != D_8012E67C[p]) teamFull = 0;
	beq	$16, $23, $498
	lb	$15, D_8012E67C($16)
	lb	$14, 0($2)
	beq	$15, $14, $498
	.loc	2 782
	move	$3, $0
$498:
	.loc	2 781
 # 781	                        for (i = 0; i < D_8014A108; i++) {
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, $5, $497
$499:
	beq	$3, 1, $494
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$524
$500:
	.loc	2 785
 # 782	                            if (i != p && D_8012E67C[i] != D_8012E67C[p]) teamFull = 0;
 # 783	                        }
 # 784	                    } while (teamFull == 1);
 # 785	                } else D_8012E67C[p] = p;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	sb	$23, 0($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	.loc	2 786
 # 786	                break;
	b	$524
$501:
	.loc	2 788
 # 787	            case 11:
 # 788	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $503
	.loc	2 788
	.loc	2 789
 # 789	                    D_8011157C[p + 1][D_803B9FD0[p]] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$15, $23, 13
	addu	$14, $15, $16
	la	$25, D_8011157C
	addu	$2, $14, $25
	lb	$24, 13($2)
	addu	$15, $24, $19
	sb	$15, 13($2)
	.loc	2 790
 # 790	                    if (D_8011157C[p + 1][D_803B9FD0[p]] < 0) D_8011157C[p + 1][D_803B9FD0[p]] = 20;
	lb	$7, 13($2)
	bge	$7, 0, $502
	.loc	2 790
	li	$14, 20
	sb	$14, 13($2)
	lb	$7, 13($2)
	b	$504
$502:
	.loc	2 791
 # 791	                    else if (D_8011157C[p + 1][D_803B9FD0[p]] > 20) D_8011157C[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 21, $504
	.loc	2 791
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$504
$503:
	.loc	2 792
 # 792	                } else D_8011157C[p + 1][D_803B9FD0[p]] = D_8011157C[0][D_803B9FD0[p]];
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$25, $23, 13
	addu	$24, $25, $16
	la	$15, D_8011157C
	addu	$2, $24, $15
	addu	$14, $16, $15
	lb	$25, 0($14)
	sb	$25, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$504:
	.loc	2 793
 # 793	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 8, D_8011157C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 8
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 794
 # 794	                break;
	b	$524
$505:
	.loc	2 796
 # 795	            case 12:
 # 796	                if (d) {
	lb	$16, 0($21)
	sw	$0, 256($sp)
	beq	$19, 0, $507
	.loc	2 796
	.loc	2 797
 # 797	                    D_801115C0[p + 1][D_803B9FD0[p]] += d;
	.noalias	$17,$21
	.noalias	$17,$22
	.noalias	$17,$sp
	mul	$24, $23, 13
	la	$15, D_801115C0
	addu	$17, $24, $15
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	addu	$2, $17, $16
	lb	$14, 13($2)
	addu	$25, $14, $19
	sb	$25, 13($2)
	.loc	2 798
 # 798	                    if (D_801115C0[p + 1][D_803B9FD0[p]] < 0) D_801115C0[p + 1][D_803B9FD0[p]] = 9;
	lb	$3, 13($2)
	bge	$3, 0, $506
	.loc	2 798
	li	$24, 9
	sb	$24, 13($2)
	b	$508
$506:
	.loc	2 799
 # 799	                    else if (D_801115C0[p + 1][D_803B9FD0[p]] > 9) D_801115C0[p + 1][D_803B9FD0[p]] = 0;
	blt	$3, 10, $508
	.loc	2 799
	sb	$0, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$508
$507:
	.loc	2 800
 # 800	                } else D_801115C0[p + 1][D_803B9FD0[p]] = D_801115C0[0][D_803B9FD0[p]];
	mul	$15, $23, 13
	la	$14, D_801115C0
	addu	$17, $15, $14
	addu	$25, $16, $14
	lb	$24, 0($25)
	addu	$15, $17, $16
	.noalias	$15,$21
	.noalias	$15,$22
	.noalias	$15,$sp
	sb	$24, 13($15)
	.alias	$15,$21
	.alias	$15,$22
	.alias	$15,$sp
$508:
	.loc	2 801
 # 801	                sound = 0;
	.loc	2 802
 # 802	                scheduler_recv(D_803BA1D0[p]);
	.noalias	$18,$17
	.noalias	$18,$21
	.noalias	$18,$22
	.noalias	$18,$sp
	la	$14, D_803BA1D0
	addu	$18, $30, $14
	lw	$4, 0($18)
	.livereg	0x0800200E,0x00000000
	jal	scheduler_recv
	.loc	2 803
 # 803	                D_803BA1D0[p] = entity_flags_apply(D_801115C0[p + 1][D_803B9FD0[p]] + 26, 0, 1, 0);
	lb	$25, 0($21)
	addu	$24, $17, $25
	.noalias	$24,$18
	.noalias	$24,$21
	.noalias	$24,$22
	.noalias	$24,$sp
	lb	$4, 13($24)
	.alias	$24,$18
	.alias	$24,$21
	.alias	$24,$22
	.alias	$24,$sp
	addu	$4, $4, 26
	move	$5, $0
	li	$6, 1
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	entity_flags_apply
	sw	$2, 0($18)
	.loc	2 804
 # 804	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 9, D_801115C0[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	lb	$16, 0($21)
	and	$5, $16, 255
	li	$6, 9
	addu	$15, $17, $16
	.noalias	$15,$18
	.noalias	$15,$21
	.noalias	$15,$22
	.noalias	$15,$sp
	lb	$7, 13($15)
	.alias	$15,$18
	.alias	$15,$21
	.alias	$15,$22
	.alias	$15,$sp
	.livereg	0x0F00800E,0x00000000
	jal	func_800C7578
	.alias	$17,$18
	.alias	$17,$21
	.alias	$17,$22
	.alias	$17,$sp
	.alias	$18,$21
	.alias	$18,$22
	.alias	$18,$sp
	.loc	2 805
 # 805	                break;
	b	$524
$509:
	.loc	2 807
 # 806	            case 13:
 # 807	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $511
	.loc	2 807
	.loc	2 808
 # 808	                    D_801112DC[p + 1][D_803B9FD0[p]] += d * 0.05f;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$14, $23, 52
	mul	$25, $16, 4
	addu	$24, $14, $25
	la	$15, D_801112DC
	addu	$2, $24, $15
	mtc1	$19, $f4
	cvt.s.w	$f6, $f4
	li.s	$f8, 0.05
	mul.s	$f10, $f6, $f8
	l.s	$f4, 52($2)
	add.s	$f6, $f4, $f10
	s.s	$f6, 52($2)
	.loc	2 809
 # 809	                    if (D_801112DC[p + 1][D_803B9FD0[p]] < 0.74f) D_801112DC[p + 1][D_803B9FD0[p]] = 1.5f;
	l.s	$f0, 52($2)
	li.s	$f8, 0.74
	c.lt.s	$f0, $f8
	bc1f	$510
	.loc	2 809
	li.s	$f4, 1.5
	s.s	$f4, 52($2)
	l.s	$f0, 52($2)
	b	$512
$510:
	.loc	2 810
 # 810	                    else if (D_801112DC[p + 1][D_803B9FD0[p]] > 1.6f) D_801112DC[p + 1][D_803B9FD0[p]] = 0.75f;
	li.s	$f10, 1.6
	c.lt.s	$f10, $f0
	bc1f	$512
	.loc	2 810
	li.s	$f6, 0.75
	s.s	$f6, 52($2)
	l.s	$f0, 52($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$512
$511:
	.loc	2 811
 # 811	                } else D_801112DC[p + 1][D_803B9FD0[p]] = D_801112DC[0][D_803B9FD0[p]];
	mul	$3, $16, 4
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$14, $23, 52
	addu	$25, $14, $3
	la	$24, D_801112DC
	addu	$2, $25, $24
	addu	$15, $3, $24
	l.s	$f8, 0($15)
	s.s	$f8, 52($2)
	l.s	$f0, 52($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$512:
	.loc	2 812
 # 812	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 10, (s8)(100.0f * D_801112DC[p + 1][D_803B9FD0[p]] - 75.0f));
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 10
	li.s	$f4, 100.0
	mul.s	$f10, $f0, $f4
	li.s	$f6, 75.0
	sub.s	$f8, $f10, $f6
	trunc.w.s	$f4, $f8, $14
	mfc1	$7, $f4
	sll	$25, $7, 24
	move	$7, $25
	sra	$24, $7, 24
	move	$7, $24
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 813
 # 813	                break;
	b	$524
$513:
	.loc	2 815
 # 814	            case 14:
 # 815	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $515
	.loc	2 815
	.loc	2 816
 # 816	                    D_801113E0[p + 1][D_803B9FD0[p]] += d * 0.05f;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$15, $23, 52
	mul	$14, $16, 4
	addu	$25, $15, $14
	la	$24, D_801113E0
	addu	$2, $25, $24
	mtc1	$19, $f10
	cvt.s.w	$f6, $f10
	li.s	$f8, 0.05
	mul.s	$f4, $f6, $f8
	l.s	$f10, 52($2)
	add.s	$f6, $f10, $f4
	s.s	$f6, 52($2)
	.loc	2 817
 # 817	                    if (D_801113E0[p + 1][D_803B9FD0[p]] < 0.74f) D_801113E0[p + 1][D_803B9FD0[p]] = 1.5f;
	l.s	$f0, 52($2)
	li.s	$f8, 0.74
	c.lt.s	$f0, $f8
	bc1f	$514
	.loc	2 817
	li.s	$f10, 1.5
	s.s	$f10, 52($2)
	l.s	$f0, 52($2)
	b	$516
$514:
	.loc	2 818
 # 818	                    else if (D_801113E0[p + 1][D_803B9FD0[p]] > 1.5f) D_801113E0[p + 1][D_803B9FD0[p]] = 0.75f;
	li.s	$f4, 1.5
	c.lt.s	$f4, $f0
	bc1f	$516
	.loc	2 818
	li.s	$f6, 0.75
	s.s	$f6, 52($2)
	l.s	$f0, 52($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$516
$515:
	.loc	2 819
 # 819	                } else D_801113E0[p + 1][D_803B9FD0[p]] = D_801113E0[0][D_803B9FD0[p]];
	mul	$3, $16, 4
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$15, $23, 52
	addu	$14, $15, $3
	la	$25, D_801113E0
	addu	$2, $14, $25
	addu	$24, $3, $25
	l.s	$f8, 0($24)
	s.s	$f8, 52($2)
	l.s	$f0, 52($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$516:
	.loc	2 820
 # 820	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 11, (s8)(100.0f * D_801113E0[p + 1][D_803B9FD0[p]] - 75.0f));
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 11
	li.s	$f10, 100.0
	mul.s	$f4, $f0, $f10
	li.s	$f6, 75.0
	sub.s	$f8, $f4, $f6
	trunc.w.s	$f10, $f8, $15
	mfc1	$7, $f10
	sll	$14, $7, 24
	move	$7, $14
	sra	$25, $7, 24
	move	$7, $25
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 821
 # 821	                break;
	b	$524
$517:
	.loc	2 823
 # 822	            case 15:
 # 823	                if (d) {
	lb	$16, 0($21)
	beq	$19, 0, $519
	.loc	2 823
	.loc	2 824
 # 824	                    D_803B28B0[p] += d;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$24, $23, 2
	la	$15, D_803B28B0
	addu	$2, $24, $15
	lh	$14, 0($2)
	addu	$25, $14, $19
	sh	$25, 0($2)
	.loc	2 825
 # 825	                    if (D_803B28B0[p] < 0) D_803B28B0[p] = 10;
	lh	$6, 0($2)
	bge	$6, 0, $518
	.loc	2 825
	li	$24, 10
	sh	$24, 0($2)
	lh	$6, 0($2)
	b	$520
$518:
	.loc	2 826
 # 826	                    else if (D_803B28B0[p] > 10) D_803B28B0[p] = 0;
	blt	$6, 11, $520
	.loc	2 826
	sh	$0, 0($2)
	lh	$6, 0($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$520
$519:
	.loc	2 827
 # 827	                } else D_803B28B0[p] = 0;
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	mul	$15, $23, 2
	la	$14, D_803B28B0
	addu	$2, $15, $14
	sh	$0, 0($2)
	lh	$6, 0($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$520:
	.loc	2 828
 # 828	                sfx_stop(D_803B9FD0[p], p, D_803B28B0[p]);
	sll	$4, $16, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $23, 16
	sra	$24, $5, 16
	move	$5, $24
	.livereg	0x0E00000E,0x00000000
	jal	sfx_stop
	.loc	2 829
 # 829	                break;
	b	$524
$521:
	.loc	2 830
 # 830	            case 16: D_80152698[1] = func_8039E200(D_80152698[1], d); break;
	lw	$17, D_80152698+4
	sll	$18, $19, 24
	sra	$15, $18, 24
	move	$18, $15
	sw	$21, 148($sp)
	sw	$22, 152($sp)
	.livereg	0x0000600E,0x00000000
	jal	func_8039E200
	lw	$21, 148($sp)
	lw	$22, 152($sp)
	sw	$2, D_80152698+4
	.loc	2 830
	b	$524
$522:
	.loc	2 831
 # 831	            case 17: D_80152698[2] = func_8039E200(D_80152698[2], d); break;
	lw	$17, D_80152698+8
	sll	$18, $19, 24
	sra	$14, $18, 24
	move	$18, $14
	sw	$21, 148($sp)
	sw	$22, 152($sp)
	.livereg	0x0000600E,0x00000000
	jal	func_8039E200
	lw	$21, 148($sp)
	lw	$22, 152($sp)
	sw	$2, D_80152698+8
	.loc	2 831
	b	$524
$523:
	.loc	2 832
 # 832	            case 18: D_80152698[3] = func_8039E200(D_80152698[3], d); break;
	lw	$17, D_80152698+12
	sll	$18, $19, 24
	sra	$25, $18, 24
	move	$18, $25
	sw	$21, 148($sp)
	sw	$22, 152($sp)
	.livereg	0x0000600E,0x00000000
	jal	func_8039E200
	lw	$21, 148($sp)
	lw	$22, 152($sp)
	sw	$2, D_80152698+12
	.loc	2 832
$524:
	.loc	2 834
 # 833	            }
 # 834	            if (sound) audio_doppler(D_8014A118[p].repeated);
	lw	$24, 256($sp)
	beq	$24, 0, $525
	.loc	2 834
	lw	$4, 12($22)
	.livereg	0x0800000E,0x00000000
	jal	audio_doppler
$525:
	.loc	2 836
 # 835	        }
 # 836	        if (D_803BA038[p] > D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
	lw	$4, D_803B28B8($30)
	addu	$4, $4, -1
	.noalias	$2,$21
	.noalias	$2,$22
	.noalias	$2,$sp
	la	$15, D_803BA038
	addu	$2, $30, $15
	lw	$3, 0($2)
	lb	$16, 0($21)
	lw	$5, D_803BA050($30)
	la	$14, D_803BA048
	addu	$7, $23, $14
	mul	$6, $23, 2
	bge	$4, $3, $526
	.loc	2 836
	sw	$4, 0($2)
	lw	$3, 0($2)
$526:
	.loc	2 837
 # 837	        if (D_803BA038[p] < D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
	bge	$3, $4, $527
	.loc	2 837
	sw	$4, 0($2)
	lw	$3, 0($2)
$527:
	.loc	2 838
 # 838	        if (D_803BA038[p] > D_803BA050[p] - 3) D_803BA038[p] = D_803BA050[p] - 3;
	addu	$4, $5, -3
	bge	$4, $3, $528
	.loc	2 838
	sw	$4, 0($2)
	lw	$3, 0($2)
$528:
	.loc	2 839
 # 839	        if (D_803BA038[p] < 1) D_803BA038[p] = 1;
	bgt	$3, 0, $529
	.loc	2 839
	li	$25, 1
	sw	$25, 0($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$529:
	.loc	2 840
 # 840	        D_803BA048[p] = 1;
	.noalias	$7,$21
	.noalias	$7,$22
	.noalias	$7,$sp
	li	$24, 1
	sb	$24, 0($7)
	.loc	2 841
 # 841	after_options:
	.alias	$7,$21
	.alias	$7,$22
	.alias	$7,$sp
$530:
	.loc	2 842
 # 842	        if (D_803BA018[p] != D_803B9FD0[p]) {
	.noalias	$17,$21
	.noalias	$17,$22
	.noalias	$17,$sp
	la	$15, D_803BA018
	addu	$17, $6, $15
	lh	$2, 0($17)
	beq	$2, $16, $532
	.loc	2 842
	.loc	2 843
 # 843	            if (D_803BA018[p] >= 0) func_800CCA04(D_8014A118[p].owner, D_803B9FD0[p]);
	blt	$2, 0, $531
	.loc	2 843
	lw	$4, 72($22)
	and	$5, $16, 255
	.livereg	0x0C00000E,0x00000000
	jal	func_800CCA04
	.alias	$22,$17
	.alias	$22,$21
	.alias	$22,$sp
	lb	$16, 0($21)
	.alias	$21,$17
	.alias	$21,$sp
$531:
	.loc	2 844
 # 844	            D_803BA018[p] = D_803B9FD0[p];
	sh	$16, 0($17)
	.alias	$17,$sp
$532:
	.loc	2 846
 # 845	        }
 # 846	        if (D_8014A110 == 2) {
	lw	$14, D_8014A110
	bne	$14, 2, $541
	.loc	2 846
	.loc	2 847
 # 847	            for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$19, 1
	lb	$2, D_803BA00C
	addu	$2, $2, 1
	blt	$2, 2, $541
	li	$22, 4
	la	$20, D_803B9FC0+4
$533:
	.loc	2 847
	.loc	2 848
 # 848	                if (D_803B9FC0[ghostIdx]) {
	.noalias	$20,$sp
	lw	$25, 0($20)
	beq	$25, 0, $540
	la	$2, D_803AF9A8
	.loc	2 848
	.loc	2 849
 # 849	                    pos[0] = D_803AF9A8[0][38].position[0];
	.noalias	$2,$20
	.noalias	$2,$sp
	l.s	$f4, 2480($2)
	s.s	$f4, 184($sp)
	.loc	2 850
 # 850	                    pos[1] = D_803AF9A8[0][38].position[1];
	l.s	$f0, 2484($2)
	.loc	2 851
 # 851	                    pos[2] = D_803AF9A8[0][38].position[2];
	l.s	$f6, 2488($2)
	s.s	$f6, 192($sp)
	.loc	2 852
 # 852	                    pos[1] = -pos[1] / 2.0f;
	neg.s	$f8, $f0
	mul.s	$f0, $f8, $f20
	.loc	2 853
 # 853	                    brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, gscreenPos);
	move	$4, $19
	addu	$5, $sp, 184
	mul	$24, $19, 152
	la	$15, D_80150B70
	addu	$6, $24, $15
	move	$7, $0
	addu	$14, $sp, 180
	sw	$14, 16($sp)
	la	$25, D_80152698
	addu	$16, $22, $25
	s.s	$f0, 188($sp)
	.livereg	0x0F00800E,0x00000000
	jal	brake_light_update
	.alias	$2,$20
	.alias	$2,$sp
	.loc	2 854
 # 854	                    gx = gscreenPos[0];
	lh	$21, 180($sp)
	.loc	2 855
 # 855	                    gy = gscreenPos[1];
	lh	$18, 182($sp)
	.loc	2 856
 # 856	                    object_create(12);
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 857
 # 857	                    if (!D_80152698[ghostIdx]) {
	.noalias	$16,$20
	.noalias	$16,$sp
	lw	$3, 0($16)
	bne	$3, 0, $536
	.alias	$16,$20
	.alias	$16,$sp
	.loc	2 857
	.loc	2 858
 # 858	                        if (!D_803AF980) gw = object_manager_update(countdown_state.text[225], -1);
	lw	$24, D_803AF980
	bne	$24, 0, $534
	.loc	2 858
	lw	$15, countdown_state+4
	lw	$4, 900($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
	b	$535
$534:
	.loc	2 859
 # 859	                        else gw = object_manager_update(countdown_state.text[224], -1);
	lw	$25, countdown_state+4
	lw	$4, 896($25)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$24, $16, 16
	move	$16, $24
$535:
	.loc	2 860
 # 860	                        gh = object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	sll	$17, $2, 16
	sra	$15, $17, 16
	move	$17, $15
	b	$539
$536:
	.loc	2 861
 # 861	                    } else {
	.loc	2 862
 # 862	                        gw = object_manager_update((*D_80152698[ghostIdx])->name, -1);
	lw	$4, 0($3)
	addu	$4, $4, 10
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
	.loc	2 863
 # 863	                        gw = (gw > object_manager_update(D_803B8590, -1) ? gw : object_manager_update(D_803B85A0, -1));
	la	$4, D_803B8590
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $16
	bgeu	$2, $3, $537
	sll	$16, $3, 16
	sra	$25, $16, 16
	move	$16, $25
	b	$538
$537:
	la	$4, D_803B85A0
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$24, $16, 16
	move	$16, $24
$538:
	.loc	2 864
 # 864	                        gh = object_bytes_sum_global() * 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$17, $2, 2
	sll	$15, $17, 16
	move	$17, $15
	sra	$14, $17, 16
	move	$17, $14
$539:
	.loc	2 866
 # 865	                    }
 # 866	                    gy -= object_bytes_sum_global() / 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	div	$25, $2, 2
	subu	$18, $18, $25
	sll	$24, $18, 16
	move	$18, $24
	sra	$15, $18, 16
	move	$18, $15
	.loc	2 867
 # 867	                    crowd_cheer_play(D_803B9FC0[ghostIdx], gx - gw / 2 - 4, gy - 4, gx + gw / 2 + 4, gy + gh + 4);
	lw	$4, 0($20)
	div	$3, $16, 2
	subu	$5, $21, $3
	addu	$5, $5, -4
	addu	$6, $18, -4
	addu	$7, $21, $3
	addu	$7, $7, 4
	addu	$14, $18, $17
	addu	$25, $14, 4
	sw	$25, 16($sp)
	.livereg	0x1F00000E,0x00000000
	jal	crowd_cheer_play
	lb	$2, D_803BA00C
	addu	$2, $2, 1
$540:
	.loc	2 847
 # 847	            for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	addu	$19, $19, 1
	addu	$22, $22, 4
	addu	$20, $20, 4
	blt	$19, $2, $533
	.alias	$20,$sp
$541:
	.loc	2 871
 # 871	        func_8039D6A4(p);
	sll	$30, $23, 24
	sra	$24, $30, 24
	move	$30, $24
	sh	$23, 260($sp)
	.livereg	0x0000000E,0x00000000
	jal	func_8039D6A4
	lh	$23, 260($sp)
	li.s	$f20, 5.0E-1
	.loc	2 872
 # 872	        func_8039D300(p);
	move	$4, $23
	.livereg	0x0800000E,0x00000000
	jal	func_8039D300
	.loc	2 604
 # 604	    for (p = 0; p < D_8014A108; p++) {
$542:
	addu	$23, $23, 1
	sll	$15, $23, 16
	move	$23, $15
	sra	$14, $23, 16
	move	$23, $14
	lh	$5, D_8014A108
	blt	$23, $5, $408
	la	$2, D_803BA030
$543:
	.loc	2 874
 # 874	    D_803BA02C = 1;
	li	$25, 1
	sb	$25, D_803BA02C
	.loc	2 875
 # 875	    for (i = 0; i < D_8014A108; i++) if (!D_803BA028[i]) D_803BA02C = 0;
	move	$16, $0
	ble	$5, 0, $546
$544:
	.loc	2 875
	lb	$24, D_803BA028($16)
	bne	$24, 0, $545
	.loc	2 875
	sb	$0, D_803BA02C
$545:
	.loc	2 875
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, $5, $544
$546:
	.loc	2 876
 # 876	    if (D_803BA02C && D_803BA030 >= 10) {
	lb	$25, D_803BA02C
	beq	$25, 0, $556
	.noalias	$2,$sp
	lw	$24, 0($2)
	blt	$24, 10, $556
	.alias	$2,$sp
	.loc	2 876
	.loc	2 877
 # 877	        speed_mode0_wrapper(0.0f, 3.0f);
	li.s	$f12, 0.0
	li.s	$f14, 3.0
	.livereg	0x0000000E,0x000A0000
	jal	speed_mode0_wrapper
	.loc	2 878
 # 878	        func_800B5570(0x40000);
	li	$4, 262144
	.livereg	0x0800000E,0x00000000
	jal	func_800B5570
	.loc	2 879
 # 879	        func_8039D05C();
	.livereg	0x0000000E,0x00000000
	jal	func_8039D05C
	.loc	2 880
 # 880	        if (D_8014A110 == 3) net_session_update();
	lw	$3, D_8014A110
	bne	$3, 3, $547
	.loc	2 880
	.livereg	0x0000000E,0x00000000
	jal	net_session_update
	lw	$3, D_8014A110
$547:
	.loc	2 881
 # 881	        if (D_8014A110 == 2) {
	bne	$3, 2, $555
	.loc	2 881
	.loc	2 882
 # 882	            {
	.loc	2 884
 # 883	                Record **gh;
 # 884	                for (gh = D_80152020.first; gh; gh = (*gh)->next) menu_transition((void **)gh);
	lw	$16, D_80152020+8
	beq	$16, 0, $549
$548:
	.loc	2 884
	move	$4, $16
	.livereg	0x0800000E,0x00000000
	jal	menu_transition
	.loc	2 884
	lw	$15, 0($16)
	lw	$16, 0($15)
	bne	$16, 0, $548
$549:
	.loc	2 886
 # 885	            }
 # 886	            for (i = 1; i < D_803BA00C; i++) {
	li	$16, 1
	lb	$2, D_803BA00C
	ble	$2, 1, $552
	la	$3, D_80152698
$550:
	.loc	2 886
	.loc	2 887
 # 887	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
	.noalias	$3,$sp
	.noalias	$4,$sp
	mul	$14, $16, 4
	addu	$4, $3, $14
	lw	$25, 0($4)
	bne	$25, 0, $551
	.loc	2 887
	.loc	2 887
	lw	$24, 4($4)
	sw	$24, 0($4)
	.loc	2 887
	sw	$0, 4($4)
	.alias	$4,$sp
$551:
	.loc	2 886
 # 886	            for (i = 1; i < D_803BA00C; i++) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, $2, $550
	li	$16, 1
$552:
	la	$3, D_80152698
	.loc	2 889
 # 887	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
 # 888	            }
 # 889	            for (i = 1; i < D_803BA00C; i++) {
	ble	$2, 1, $555
$553:
	.loc	2 889
	.loc	2 890
 # 890	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
	.noalias	$4,$sp
	mul	$25, $16, 4
	addu	$4, $3, $25
	lw	$24, 0($4)
	bne	$24, 0, $554
	.loc	2 890
	.loc	2 890
	lw	$15, 4($4)
	sw	$15, 0($4)
	.loc	2 890
	sw	$0, 4($4)
	.alias	$4,$sp
$554:
	.loc	2 889
 # 889	            for (i = 1; i < D_803BA00C; i++) {
	addu	$16, $16, 1
	sll	$14, $16, 16
	move	$16, $14
	sra	$25, $16, 16
	move	$16, $25
	blt	$16, $2, $553
	.alias	$3,$sp
$555:
	.loc	2 893
 # 890	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
 # 891	            }
 # 892	        }
 # 893	        pause_resume(0);
	move	$4, $0
	.livereg	0x0800000E,0x00000000
	jal	pause_resume
	.loc	2 895
 # 894	    }
 # 895	}
$556:
	.livereg	0x0000FF0E,0x00000FFF
	l.d	$f20, 32($sp)
	l.d	$f22, 40($sp)
	l.d	$f24, 48($sp)
	l.d	$f26, 56($sp)
	l.d	$f28, 64($sp)
	l.d	$f30, 72($sp)
	lw	$16, 80($sp)
	lw	$17, 84($sp)
	lw	$18, 88($sp)
	lw	$19, 92($sp)
	lw	$20, 96($sp)
	lw	$21, 100($sp)
	lw	$22, 104($sp)
	lw	$23, 108($sp)
	lw	$31, 116($sp)
	lw	$30, 112($sp)
	addu	$sp, 264
	j	$31
	.end	func_803A1EAC
