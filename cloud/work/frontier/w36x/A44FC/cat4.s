	.verstamp	3 19
	.option	pic0
	.data	
	.align	2
	.align	0
$$100178:
	.word	-1 : 1
	.data	
	.align	2
	.align	0
zoom:
	.float	1.0:1
	.data	
	.align	2
	.align	0
$$100195:
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
$$100208:
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
	beq	$14, 0, $100032
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
	b	$100034
$100032:
	.loc	2 979
 # 979	    } else if(D_8014978C>=6)wheel_setup_initial(80);
	lb	$15, D_8014978C
	blt	$15, 6, $100033
	.loc	2 979
	li	$4, 80
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
	b	$100034
$100033:
	.loc	2 980
 # 980	    else wheel_setup_initial(81);
	li	$4, 81
	.livereg	0x0800000E,0x00000000
	jal	wheel_setup_initial
$100034:
	.loc	2 981
 # 981	    for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {
	la	$16, D_803B9FC0
	la	$17, D_803B9FC0+16
$100035:
	.loc	2 981
	.loc	2 982
 # 982	        if(*box) {sound_handles_array_clear(*box);*box=0;}
	lw	$4, 0($16)
	beq	$4, 0, $100036
	.loc	2 982
	.loc	2 982
	.livereg	0x0800000E,0x00000000
	jal	sound_handles_array_clear
	.loc	2 982
	sw	$0, 0($16)
$100036:
	.loc	2 981
 # 981	    for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {
	addu	$16, $16, 4
	bltu	$16, $17, $100035
	.loc	2 984
 # 982	        if(*box) {sound_handles_array_clear(*box);*box=0;}
 # 983	    }
 # 984	    for(player=0;player<(count=D_8014A108);player++) {
	move	$20, $0
	lh	$2, D_8014A108
	ble	$2, 0, $100047
	la	$30, D_803B25CC
	li	$22, 572
	li	$21, 2816
	li	$18, -1
$100037:
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
$100038:
	.loc	2 985
	.loc	2 986
 # 986	            slot=&D_803AF9A8[(s8)player][i];
	.loc	2 987
 # 987	            if(slot->handle!=-1) {sound_call_minimal(slot->handle);slot->handle=-1;}
	.noalias	$19,$sp
	.noalias	$17,$sp
	addu	$17, $19, $16
	lw	$2, 4($17)
	beq	$18, $2, $100039
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
$100039:
	.loc	2 985
 # 985	        for(i=0;i!=44;i++) {
	addu	$16, $16, 64
	bne	$16, $21, $100038
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
	beq	$18, $2, $100040
	.loc	2 990
	.loc	2 990
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 990
	sw	$18, 0($30)
$100040:
	.loc	2 991
 # 991	        if(D_803B25CC[player][1]!=-1) {sound_call_minimal(D_803B25CC[player][1]);D_803B25CC[player][1]=-1;}
	lw	$2, 4($30)
	beq	$18, $2, $100041
	.loc	2 991
	.loc	2 991
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 991
	sw	$18, 4($30)
$100041:
	.loc	2 992
 # 992	        if(player>0)continue;
	ble	$20, 0, $100042
	.loc	2 992
	lh	$2, D_8014A108
	b	$100046
$100042:
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
$100043:
	.loc	2 996
 # 996	                part=&D_803B2618[player][i];
	.loc	2 997
 # 997	                if(part->handle!=-1) {sound_call_minimal(part->handle);part->handle=-1;part->auxiliary=-1;}
	.noalias	$16,$30
	.noalias	$16,$sp
	lh	$4, 0($16)
	beq	$18, $4, $100044
	.loc	2 997
	.loc	2 997
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 997
	sh	$18, 0($16)
	.loc	2 997
	sh	$18, 2($16)
$100044:
	.loc	2 994
 # 994	            for(i=0;i!=11;i++) {
	addu	$17, $17, 52
	addu	$16, $16, 52
	bne	$17, $22, $100043
	.alias	$16,$30
	.alias	$16,$sp
	lw	$2, D_803B25C8
	.loc	2 999
 # 995	                if (player) {} /* Code-free allocation-priority read; see header. */
 # 996	                part=&D_803B2618[player][i];
 # 997	                if(part->handle!=-1) {sound_call_minimal(part->handle);part->handle=-1;part->auxiliary=-1;}
 # 998	            }
 # 999	            if(D_803B25C8!=-1) {sound_call_minimal(D_803B25C8);D_803B25C8=-1;}
	beq	$18, $2, $100045
	.loc	2 999
	.loc	2 999
	sll	$4, $2, 16
	sra	$25, $4, 16
	move	$4, $25
	.livereg	0x0800000E,0x00000000
	jal	sound_call_minimal
	.loc	2 999
	sw	$18, D_803B25C8
$100045:
	.loc	2 984
 # 984	    for(player=0;player<(count=D_8014A108);player++) {
	lh	$2, D_8014A108
$100046:
	addu	$20, $20, 1
	addu	$30, $30, 8
	blt	$20, $2, $100037
	.alias	$30,$sp
	move	$20, $0
$100047:
	la	$16, D_803B28AC
	.loc	2 1002
 #1002	    if(D_803B28AC) {sound_stop(D_803B28AC);D_803B28AC=0;}
	.noalias	$16,$sp
	lw	$4, 0($16)
	beq	$4, 0, $100048
	.loc	2 1002
	.loc	2 1002
	.livereg	0x0800000E,0x00000000
	jal	sound_stop
	.loc	2 1002
	sw	$0, 0($16)
	.alias	$16,$sp
$100048:
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
	ble	$15, 0, $100050
	la	$16, D_803BA1D0
$100049:
	.loc	2 1006
	.noalias	$16,$sp
	lw	$4, 0($16)
	.livereg	0x0800000E,0x00000000
	jal	scheduler_recv
	.loc	2 1006
	addu	$20, $20, 1
	addu	$16, $16, 4
	lh	$24, D_8014A108
	blt	$20, $24, $100049
	.alias	$16,$sp
$100050:
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
$100051:
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
	blt	$2, 4, $100051
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
	beq	$4, 13, $100052
	bne	$4, 14, $100053
$100052:
	lb	$14, D_801407D0
	bne	$14, 0, $100053
	.loc	2 901
	move	$3, $0
$100053:
	.loc	2 902
 # 902	    if (!D_80156994) {
	lb	$15, D_80156994
	bne	$15, 0, $100056
	.loc	2 902
	.loc	2 903
 # 903	        if (item == 11 && D_8014978C >= 0 && D_8014978C < 6) available = 0;
	bne	$4, 11, $100054
	lb	$2, D_8014978C
	blt	$2, 0, $100054
	bge	$2, 6, $100054
	.loc	2 903
	move	$3, $0
$100054:
	li	$6, 17
	.loc	2 904
 # 904	        if (item == 17 || item == 18) available = 0;
	beq	$4, $6, $100055
	bne	$4, 18, $100056
$100055:
	.loc	2 904
	move	$3, $0
$100056:
	li	$6, 17
	.loc	2 906
 # 905	    }
 # 906	    if (!D_803AF980 && (item == 16 || item == 17 || item == 18)) available = 0;
	lw	$24, D_803AF980
	bne	$24, 0, $100058
	beq	$4, 16, $100057
	beq	$4, $6, $100057
	bne	$4, 18, $100058
$100057:
	.loc	2 906
	move	$3, $0
$100058:
	li	$7, 6
	.loc	2 907
 # 907	    if (item == 6 && (D_8014A110 == 2 || D_8014A110 == 6)) available = 0;
	bne	$4, $7, $100060
	la	$8, D_8014A110
	.noalias	$8,$sp
	lw	$2, 0($8)
	beq	$2, 2, $100059
	bne	$7, $2, $100060
$100059:
	.loc	2 907
	move	$3, $0
$100060:
	li	$2, 7
	la	$8, D_8014A110
	.loc	2 908
 # 908	    if ((item == 7 || item == 8 || item == 9) && D_8014A110 == 6 && !D_801164BE) available = 0;
	beq	$4, $2, $100061
	beq	$4, 8, $100061
	bne	$4, 9, $100062
$100061:
	lw	$25, 0($8)
	bne	$7, $25, $100062
	lh	$14, D_801164BE
	bne	$14, 0, $100062
	.loc	2 908
	move	$3, $0
$100062:
	.loc	2 909
 # 909	    if (item == 7 && !(D_803B3020[D_803B9FD0[player]] & 1)) available = 0;
	bne	$4, $2, $100063
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
	bne	$15, 0, $100063
	.loc	2 909
	move	$3, $0
$100063:
	la	$2, D_803B3020
	la	$9, D_803B9FD0
	.loc	2 910
 # 910	    if (item == 8 && !(D_803B3020[D_803B9FD0[player]] & 2)) available = 0;
	bne	$4, 8, $100064
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
	bne	$24, 0, $100064
	.loc	2 910
	move	$3, $0
$100064:
	.loc	2 911
 # 911	    if (item == 9 && !(D_803B3020[D_803B9FD0[player]] & 4)) available = 0;
	bne	$4, 9, $100065
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
	bne	$25, 0, $100065
	.alias	$2,$8
	.alias	$2,$9
	.alias	$2,$sp
	.alias	$9,$8
	.alias	$9,$sp
	.loc	2 911
	move	$3, $0
$100065:
	.loc	2 912
 # 912	    if (item == 10 && D_8014A110 != 6) available = 0;
	bne	$4, 10, $100066
	lw	$14, 0($8)
	beq	$7, $14, $100066
	.loc	2 912
	move	$3, $0
$100066:
	.loc	2 913
 # 913	    if ((item == 16 || item == 17 || item == 18) && D_8014A110 != 2) available = 0;
	beq	$4, 16, $100067
	beq	$4, $6, $100067
	bne	$4, 18, $100068
$100067:
	lw	$15, 0($8)
	beq	$15, 2, $100068
	.alias	$8,$sp
	.loc	2 913
	move	$3, $0
$100068:
	.loc	2 914
 # 914	    if (item == 12 || item == 15) available = 0;
	beq	$4, 12, $100069
	bne	$4, 15, $100070
$100069:
	.loc	2 914
	move	$3, $0
$100070:
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
	lw	$25, $$100195
	sw	$25, 108($sp)
	.loc	2 1147
 #1147	    if(!D_803B9B90[p])return;
	mul	$3, $30, 4
	lw	$14, D_803B9B90($3)
	beq	$14, 0, $100111
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
	bc1f	$100071
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
	b	$100072
$100071:
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
$100072:
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
	bc1f	$100073
	.loc	2 1153
	sub.s	$f6, $f0, $f2
	s.s	$f6, 2504($3)
$100073:
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
	bne	$14, 0, $100080
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
	bc1f	$100077
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
	bc1f	$100074
	l.s	$f6, 0($2)
	s.s	$f6, 0($2)
	b	$100075
$100074:
	s.s	$f14, 0($2)
$100075:
	.loc	2 1161
 #1161	            cursorSpeed[p]=cursorSpeed[p]>180.0f*D_803B25AC ? cursorSpeed[p] : 180.0f*D_803B25AC;
	li.s	$f10, 180.0
	mul.s	$f0, $f10, $f18
	l.s	$f2, 0($2)
	c.lt.s	$f0, $f2
	bc1f	$100076
	s.s	$f2, 0($2)
	l.s	$f2, 0($2)
	b	$100077
$100076:
	s.s	$f0, 0($2)
	l.s	$f2, 0($2)
$100077:
	.loc	2 1163
 #1162	        }
 #1163	        if(D_803AF9A8[p][39].position[1]>prevPos[1]+D_8002EB94*cursorSpeed[p])
	.set	 volatile
	l.s	$f4, 0($19)
	.set	 novolatile
	mul.s	$f8, $f2, $f4
	add.s	$f6, $f8, $f22
	c.lt.s	$f6, $f16
	bc1f	$100078
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
	b	$100080
$100078:
	.loc	2 1165
 #1165	        else if(D_803AF9A8[p][39].position[1]<prevPos[1]-D_8002EB94*cursorSpeed[p])
	.set	 volatile
	l.s	$f6, 0($19)
	.set	 novolatile
	mul.s	$f10, $f2, $f6
	sub.s	$f4, $f22, $f10
	c.lt.s	$f16, $f4
	bc1f	$100079
	.loc	2 1166
 #1166	            D_803AF9A8[p][39].position[1]=prevPos[1]-D_8002EB94*cursorSpeed[p];
	.set	 volatile
	l.s	$f8, 0($19)
	.set	 novolatile
	mul.s	$f6, $f2, $f8
	sub.s	$f10, $f22, $f6
	s.s	$f10, 2548($3)
	b	$100080
$100079:
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
$100080:
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
$100081:
	lw	$2, 76($sp)
	.loc	2 1173
	.loc	2 1174
 #1174	        if(buttonIdx==D_803BA060[p][D_803B28B8[p]]) {
	lw	$25, 72($sp)
	lw	$14, 0($2)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lw	$14, 0($24)
	bne	$18, $14, $100084
	.loc	2 1174
	.loc	2 1175
 #1175	            if(D_803AF9A8[p][buttonIdx].angle<3.1415927f)D_803AF9A8[p][buttonIdx].angle+=12.566371f*D_8002EB94;
	.noalias	$16,$2
	.noalias	$16,$19
	.noalias	$16,$22
	.noalias	$16,$sp
	l.s	$f0, 8($16)
	c.lt.s	$f0, $f26
	bc1f	$100082
	.loc	2 1175
	li.s	$f6, 12.566371
	.set	 volatile
	l.s	$f10, 0($19)
	.set	 novolatile
	mul.s	$f4, $f6, $f10
	add.s	$f8, $f0, $f4
	s.s	$f8, 8($16)
	l.s	$f0, 8($16)
$100082:
	.loc	2 1176
 #1176	            if(D_803AF9A8[p][buttonIdx].angle>3.1415927f)D_803AF9A8[p][buttonIdx].angle=3.1415927f;
	c.lt.s	$f26, $f0
	bc1f	$100083
	.loc	2 1176
	s.s	$f26, 8($16)
$100083:
	.loc	2 1177
 #1177	            if(D_803B9BA0[p])D_803AF9A8[p][buttonIdx].angle=3.1415927f;
	lw	$25, 84($sp)
	lw	$15, 0($25)
	beq	$15, 0, $100087
	.loc	2 1177
	s.s	$f26, 8($16)
	b	$100087
$100084:
	.loc	2 1178
 #1178	        } else {
	.loc	2 1179
 #1179	            if(D_803AF9A8[p][buttonIdx].angle>0.0f)D_803AF9A8[p][buttonIdx].angle-=12.566371f*D_8002EB94;
	l.s	$f0, 8($16)
	c.lt.s	$f28, $f0
	bc1f	$100085
	.loc	2 1179
	li.s	$f6, 12.566371
	.set	 volatile
	l.s	$f10, 0($19)
	.set	 novolatile
	mul.s	$f4, $f6, $f10
	sub.s	$f8, $f0, $f4
	s.s	$f8, 8($16)
	l.s	$f0, 8($16)
$100085:
	.loc	2 1180
 #1180	            if(D_803AF9A8[p][buttonIdx].angle<0.0f)D_803AF9A8[p][buttonIdx].angle=0.0f;
	c.lt.s	$f0, $f28
	bc1f	$100086
	.loc	2 1180
	s.s	$f28, 8($16)
$100086:
	.loc	2 1181
 #1181	            if(D_803B9BA0[p])D_803AF9A8[p][buttonIdx].angle=0.0f;
	lw	$24, 84($sp)
	lw	$14, 0($24)
	beq	$14, 0, $100087
	.loc	2 1181
	s.s	$f28, 8($16)
$100087:
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
	bne	$15, 0, $100093
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
	bc1f	$100091
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
	bc1f	$100088
	l.s	$f6, 0($2)
	s.s	$f6, 0($2)
	b	$100089
$100088:
	s.s	$f14, 0($2)
$100089:
	.loc	2 1191
 #1191	                scrollSpeed[p]=scrollSpeed[p]>180.0f*D_803B25AC ? scrollSpeed[p] : 180.0f*D_803B25AC;
	li.s	$f10, 180.0
	mul.s	$f0, $f10, $f18
	l.s	$f2, 0($2)
	c.lt.s	$f0, $f2
	bc1f	$100090
	s.s	$f2, 0($2)
	l.s	$f2, 0($2)
	b	$100091
$100090:
	s.s	$f0, 0($2)
	l.s	$f2, 0($2)
	.alias	$2,$16
	.alias	$2,$19
	.alias	$2,$22
	.alias	$2,$sp
$100091:
	.loc	2 1193
 #1192	            }
 #1193	            if(D_803AF9A8[p][buttonIdx].position[1]>prevPos[1]+D_8002EB94*scrollSpeed[p]) {
	.set	 volatile
	l.s	$f4, 0($19)
	.set	 novolatile
	mul.s	$f8, $f2, $f4
	add.s	$f6, $f8, $f22
	c.lt.s	$f6, $f12
	bc1f	$100092
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
	b	$100093
$100092:
	.loc	2 1196
 #1196	            } else if(D_803AF9A8[p][buttonIdx].position[1]<prevPos[1]-D_8002EB94*scrollSpeed[p]) {
	.set	 volatile
	l.s	$f6, 0($19)
	.set	 novolatile
	mul.s	$f10, $f2, $f6
	sub.s	$f4, $f22, $f10
	c.lt.s	$f12, $f4
	bc1f	$100093
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
$100093:
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
	bc1f	$100094
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
	b	$100095
$100094:
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
$100095:
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
	bc1t	$100096
	l.s	$f0, D_803B25BC
	sub.s	$f4, $f0, $f2
	c.lt.s	$f12, $f4
	bc1f	$100097
$100096:
	.loc	2 1213
	sb	$0, 60($16)
	b	$100100
$100097:
	.loc	2 1214
 #1214	        else if(D_803AF9A8[p][buttonIdx].position[1]>D_803B25B8)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803AF9A8[p][buttonIdx].position[1]-D_803B25B8)*255/(D_803B25B0*D_803B25AC);
	c.lt.s	$f14, $f12
	bc1f	$100098
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
	beq	$15, 0, $100113
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
	bne	$15, 0, $100114
	.set	 noat
	mfc1	$15, $f10
	li	$1, -2147483648
	or	$15, $15, $1
	.set	 at
	b	$100112
$100114:
	li	$15, -1
	b	$100112
$100113:
	mfc1	$15, $f10
	blt	$15, 0, $100114
$100112:
	ctc1	$25, $31
	sb	$15, 60($16)
	b	$100100
$100098:
	.loc	2 1215
 #1215	        else if(D_803AF9A8[p][buttonIdx].position[1]<D_803B25BC)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803B25BC-D_803AF9A8[p][buttonIdx].position[1])*255/(D_803B25B0*D_803B25AC);
	c.lt.s	$f12, $f0
	bc1f	$100099
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
	beq	$14, 0, $100116
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
	bne	$14, 0, $100117
	.set	 noat
	mfc1	$14, $f6
	li	$1, -2147483648
	or	$14, $14, $1
	.set	 at
	b	$100115
$100117:
	li	$14, -1
	b	$100115
$100116:
	mfc1	$14, $f6
	blt	$14, 0, $100117
$100115:
	ctc1	$24, $31
	sb	$14, 60($16)
	b	$100100
$100099:
	.loc	2 1216
 #1216	        else D_803AF9A8[p][buttonIdx].alpha=255;
	li	$25, 255
	sb	$25, 60($16)
$100100:
	.loc	2 1217
 #1217	        if(!func_8039D494(p,buttonIdx))D_803AF9A8[p][buttonIdx].alpha=0;
	move	$5, $30
	move	$4, $18
	.livereg	0x0C00000E,0x00000000
	jal	func_8039D494
	bne	$2, 0, $100101
	.loc	2 1217
	sb	$0, 60($16)
$100101:
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
	bne	$2, 0, $100102
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
	b	$100105
$100102:
	.loc	2 1224
 #1224	        } else if(!D_803AF9A8[p][buttonIdx].alpha || D_803BA028[p]==1) {
	lbu	$15, 60($16)
	beq	$15, 0, $100103
	lw	$24, 40($sp)
	lb	$14, 0($24)
	bne	$14, 1, $100104
$100103:
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
	b	$100105
$100104:
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
$100105:
	.loc	2 1173
 #1173	    for(buttonIdx=0;buttonIdx<19;buttonIdx++) {
	addu	$18, $18, 1
	addu	$16, $16, 64
	addu	$23, $23, 64
	addu	$20, $20, 64
	addu	$21, $21, 64
	bne	$18, 19, $100081
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
	bne	$24, 1, $100106
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
	b	$100107
	.alias	$2,$sp
$100106:
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
$100107:
	.loc	2 1244
 #1243	    }
 #1244	    if(D_803BA02C==1) {
	lb	$25, D_803BA02C
	bne	$25, 1, $100108
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
	b	$100109
$100108:
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
$100109:
	.loc	2 1252
 #1251	    }
 #1252	    if(clearScrollSpeed==1)scrollSpeed[p]=0.0f;
	lb	$25, 127($sp)
	bne	$25, 1, $100110
	.loc	2 1252
	lw	$15, 100($sp)
	s.s	$f28, scrollSpeed($15)
$100110:
	.loc	2 1253
 #1253	    D_803B9BA0[p]=0;
	lw	$24, 84($sp)
	sw	$0, 0($24)
	.loc	2 1254
 #1254	}
$100111:
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
	bne	$17, 0, $100119
	.loc	2 930
	.loc	2 931
 # 931	        if(direction>0)node=D_80152020.first;
	ble	$18, 0, $100118
	la	$20, D_80152020
	.loc	2 931
	.noalias	$20,$sp
	lw	$17, 8($20)
	b	$100119
	.alias	$20,$sp
$100118:
	la	$22, D_803BA010
	.loc	2 932
 # 932	        else node=D_803BA010+2;
	.noalias	$22,$sp
	lw	$17, 0($22)
	addu	$17, $17, 8
$100119:
	la	$20, D_80152020
	la	$22, D_803BA010
	.loc	2 934
 # 933	    }
 # 934	    while(node) {
	beq	$17, 0, $100133
	la	$21, D_80152570
	la	$19, D_80152698
$100120:
	.loc	2 934
	.loc	2 935
 # 935	        record=*node;
	lw	$4, 0($17)
	move	$3, $4
	.loc	2 936
 # 936	        if(direction>0)next=record->next;
	lb	$5, D_8014978C
	ble	$18, 0, $100121
	.loc	2 936
	lw	$16, 0($3)
	b	$100124
$100121:
	.loc	2 937
 # 937	        else if(node==D_80152020.first)next=0;
	.noalias	$20,$22
	.noalias	$20,$sp
	lw	$2, 8($20)
	bne	$17, $2, $100122
	.loc	2 937
	move	$16, $0
	b	$100124
$100122:
	.loc	2 938
 # 938	        else {
	.loc	2 939
 # 939	            for(next=D_80152020.first;next && (*next)->next!=node;next=(*next)->next) {}
	move	$16, $2
	beq	$16, 0, $100124
	lw	$24, 0($16)
	lw	$2, 0($24)
	beq	$17, $2, $100124
$100123:
	.loc	2 939
	.loc	2 939
	move	$16, $2
	beq	$16, 0, $100124
	lw	$25, 0($16)
	lw	$2, 0($25)
	bne	$17, $2, $100123
$100124:
	.loc	2 941
 # 940	        }
 # 941	        if(record->track==D_8014978C && record->mode==D_80152570 &&
	lb	$14, 8($3)
	bne	$5, $14, $100126
	.noalias	$21,$20
	.noalias	$21,$22
	.noalias	$21,$sp
	lb	$15, 0($21)
	lbu	$24, 9($3)
	bne	$15, $24, $100126
	.noalias	$19,$20
	.noalias	$19,$21
	.noalias	$19,$22
	.noalias	$19,$sp
	lw	$25, 4($19)
	beq	$17, $25, $100126
	lw	$14, 8($19)
	beq	$17, $14, $100126
	lw	$15, 12($19)
	beq	$17, $15, $100126
	lw	$24, 4($4)
	beq	$24, 0, $100125
	move	$4, $17
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100126
	lw	$4, 0($17)
$100125:
	lw	$25, 36($4)
	bne	$25, 0, $100133
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
$100126:
	.loc	2 944
 # 944	        if(!next) {
	bne	$16, 0, $100132
	.loc	2 944
	.loc	2 945
 # 945	            if(node==D_803BA010) {
	.noalias	$22,$sp
	lw	$2, 0($22)
	bne	$17, $2, $100128
	.loc	2 945
	.loc	2 946
 # 946	                if(direction>0)next=D_803BA010+1;
	ble	$18, 0, $100127
	.loc	2 946
	addu	$16, $2, 4
	b	$100132
$100127:
	.loc	2 947
 # 947	                else next=D_80152020.last;
	.noalias	$20,$22
	.noalias	$20,$sp
	lw	$16, 12($20)
	b	$100132
$100128:
	.loc	2 948
 # 948	            } else if(node==D_803BA010+1) {
	addu	$3, $2, 4
	bne	$17, $3, $100130
	.loc	2 948
	.loc	2 949
 # 949	                if(direction>0)next=D_803BA010+2;
	ble	$18, 0, $100129
	.loc	2 949
	addu	$16, $2, 8
	b	$100132
$100129:
	.loc	2 950
 # 950	                else next=D_803BA010;
	move	$16, $2
	b	$100132
$100130:
	.loc	2 951
 # 951	            } else if(node==D_803BA010+2) {
	addu	$14, $2, 8
	bne	$17, $14, $100131
	.loc	2 951
	.loc	2 952
 # 952	                if(direction<0)next=D_803BA010+1;
	bge	$18, 0, $100132
	.loc	2 952
	move	$16, $3
	b	$100132
$100131:
	.loc	2 953
 # 953	            } else if(direction>0)next=D_803BA010;
	ble	$18, 0, $100132
	.loc	2 953
	move	$16, $2
$100132:
	.loc	2 955
 # 954	        }
 # 955	        node=next;
	move	$17, $16
	bne	$16, 0, $100120
	.alias	$20,$22
	.alias	$20,$sp
	.alias	$22,$sp
$100133:
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
$100134:
	.loc	2 921
	.loc	2 922
 # 922	        if(func_8039D494(player,item))D_803BA060[player][D_803BA050[player]++]=item;
	move	$5, $12
	move	$4, $10
	.livereg	0x0C00000E,0x00000000
	jal	func_8039D494
	beq	$2, 0, $100135
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
$100135:
	.loc	2 921
 # 921	    D_803BA050[player]=0; for(item=0;item<19;item++) {
	addu	$10, $10, 1
	bne	$10, $16, $100134
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
	la	$14, $$100208
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
	bge	$24, 2, $100174
	li	$8, 2
	lw	$15, D_8014A110
	beq	$8, $15, $100174
	.alias	$17,$gp
	.loc	2 1345
	li	$3, 1
	.loc	2 1346
 #1346	    if (D_803BA028[p] == 1 || (D_803BA060[p][D_803B28B8[p]] != 1 && D_803BA060[p][D_803B28B8[p]] != 2 && D_803BA060[p][D_803B28B8[p]] != 3 && D_803BA060[p][D_803B28B8[p]] != 4 && D_803BA060[p][D_803B28B8[p]] != 5)) {
	lb	$14, D_803BA028($22)
	beq	$3, $14, $100136
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
	beq	$3, $2, $100139
	beq	$8, $2, $100139
	beq	$2, 3, $100139
	beq	$2, 4, $100139
	beq	$2, 5, $100139
	.alias	$23,$30
	.alias	$23,$sp
	.alias	$30,$sp
$100136:
	.loc	2 1346
	.loc	2 1347
 #1347	        for (i = 0; i < 11; i++) {
	move	$2, $0
	move	$17, $0
	mul	$14, $22, 572
	la	$25, D_803B2618
	addu	$16, $14, $25
	li	$18, 572
$100137:
	.loc	2 1347
	.loc	2 1348
 #1348	            if (gCarComponents[p][i].idx != -1) {
	.noalias	$16,$sp
	lh	$4, 0($16)
	beq	$4, -1, $100138
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
$100138:
	.loc	2 1347
 #1347	        for (i = 0; i < 11; i++) {
	addu	$17, $17, 52
	addu	$16, $16, 52
	bne	$17, $18, $100137
	.alias	$16,$sp
	.loc	2 1353
 #1348	            if (gCarComponents[p][i].idx != -1) {
 #1349	                model_data_load(gCarComponents[p][i].idx, 1, 15);
 #1350	                car_mass_set(gCarComponents[p][i].idx, -1);
 #1351	            }
 #1352	        }
 #1353	        return;
	b	$100174
$100139:
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
	bne	$14, -1, $100140
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
	b	$100141
$100140:
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
$100141:
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
	beq	$25, 0, $100176
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
	bne	$25, 0, $100177
	.set	 noat
	mfc1	$25, $f8
	li	$1, -2147483648
	or	$25, $25, $1
	.set	 at
	b	$100175
$100177:
	li	$25, -1
	b	$100175
$100176:
	mfc1	$25, $f8
	blt	$25, 0, $100177
$100175:
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
$100142:
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
	bgeu	$24, 5, $100155
	sll	$24, $24, 2
	lw	$24, $100143($24)
	j	$24
	.rdata	
$100143:
	.word	$100145
	.word	$100147
	.word	$100149
	.word	$100151
	.word	$100153
	.text	
$100145:
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
	bne	$25, 3, $100146
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
$100146:
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
	b	$100155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$100147:
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
	bne	$15, 5, $100148
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
$100148:
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
	b	$100155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$100149:
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
	bne	$24, 2, $100150
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
$100150:
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
	b	$100155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$100151:
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
	bne	$14, 4, $100152
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
$100152:
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
	b	$100155
	.alias	$17,$21
	.alias	$17,$23
	.alias	$17,$30
	.alias	$17,$sp
$100153:
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
	bne	$25, 1, $100154
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
$100154:
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
$100155:
	.loc	2 1411
 #1410	        }
 #1411	        if (currentLevel != gCarComponents[p][i].level) {
	.noalias	$16,$21
	.noalias	$16,$23
	.noalias	$16,$30
	.noalias	$16,$sp
	lh	$24, 2($16)
	beq	$18, $24, $100159
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
	beq	$4, -1, $100156
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
$100156:
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
	bne	$15, 1, $100157
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
	b	$100158
$100157:
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
$100158:
	.loc	2 1429
 #1428	            }
 #1429	            gCarComponents[p][i].level = currentLevel;
	sh	$18, 2($16)
$100159:
	.loc	2 1431
 #1430	        }
 #1431	        switch (i) {
	lw	$25, 140($sp)
	bgeu	$25, 11, $100173
	sll	$25, $25, 2
	lw	$25, $100160($25)
	j	$25
	.rdata	
$100160:
	.word	$100162
	.word	$100163
	.word	$100165
	.word	$100167
	.word	$100171
	.word	$100169
	.word	$100166
	.word	$100168
	.word	$100172
	.word	$100170
	.word	$100164
	.text	
$100162:
	.loc	2 1432
 #1432	        case 0: case 1: case 10:
$100163:
	.loc	2 1432
$100164:
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
	b	$100173
$100165:
	.loc	2 1439
 #1439	        case 2: case 6:
$100166:
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
	b	$100173
$100167:
	.loc	2 1446
 #1446	        case 3: case 7:
$100168:
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
	b	$100173
$100169:
	.loc	2 1452
 #1452	        case 5: case 9:
$100170:
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
	b	$100173
$100171:
	.loc	2 1459
 #1459	        case 4: case 8:
$100172:
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
$100173:
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
	bne	$25, 11, $100142
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
$100174:
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
	bne	$4, 1, $100178
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
	beq	$2, 0, $100179
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
	b	$100179
$100178:
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
	beq	$2, 0, $100179
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
$100179:
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
	ble	$2, 0, $100231
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
$100180:
	.loc	2 1526
	.loc	2 1527
 #1527	        for (i = 0; i < D_803BA020[p]; i++) {
	sw	$0, 208($sp)
	lw	$15, 92($sp)
	lh	$24, 0($15)
	ble	$24, 0, $100228
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
$100181:
	.loc	2 1527
	.loc	2 1528
 #1528	            if (((volatile s8 *)D_803B9FD4)[p] == i) {
	lw	$15, 208($sp)
	lw	$24, 100($sp)
	.set	 volatile
	lb	$25, 0($24)
	.set	 novolatile
	bne	$15, $25, $100182
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
	bc1f	$100183
	.loc	2 1530
	li.s	$f8, 1.0
	s.s	$f8, 0($19)
	b	$100183
$100182:
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
	bc1f	$100183
	.loc	2 1533
	s.s	$f30, 0($19)
$100183:
	.loc	2 1535
 #1534	            }
 #1535	            if (D_803BA028[p]) {
	lw	$15, 80($sp)
	lb	$25, 0($15)
	beq	$25, 0, $100189
	.loc	2 1535
	.loc	2 1536
 #1536	                framesLeft = 10 - D_803BA030;
	lw	$14, D_803BA030
	li	$24, 10
	subu	$2, $24, $14
	.loc	2 1537
 #1537	                if (framesLeft <= 1) D_803B9BC8[p][i].angle = 3.6651914f;
	bge	$2, 2, $100184
	.loc	2 1537
	li.s	$f8, 3.6651914
	s.s	$f8, 4($19)
	b	$100207
$100184:
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
	bc1f	$100186
$100185:
	.loc	2 1540
	sub.s	$f8, $f0, $f20
	s.s	$f8, 4($19)
	l.s	$f0, 4($19)
	c.lt.s	$f22, $f0
	bc1t	$100185
$100186:
	.loc	2 1541
 #1541	                    while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
	c.le.s	$f0, $f24
	bc1f	$100188
$100187:
	.loc	2 1541
	add.s	$f10, $f0, $f20
	s.s	$f10, 4($19)
	l.s	$f0, 4($19)
	c.le.s	$f0, $f24
	bc1t	$100187
$100188:
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
	b	$100207
$100189:
	.loc	2 1545
 #1544	                }
 #1545	            } else if (D_803BA060[p][D_803B28B8[p]] == 11) {
	mul	$25, $22, 76
	mul	$24, $22, 4
	lw	$14, D_803B28B8($24)
	mul	$15, $14, 4
	addu	$24, $25, $15
	lw	$14, D_803BA060($24)
	bne	$14, 11, $100206
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
	bc1f	$100191
$100190:
	.loc	2 1547
	sub.s	$f8, $f0, $f20
	s.s	$f8, 4($19)
	l.s	$f0, 4($19)
	c.lt.s	$f22, $f0
	bc1t	$100190
$100191:
	.loc	2 1548
 #1548	                while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
	c.le.s	$f0, $f24
	bc1f	$100193
$100192:
	.loc	2 1548
	add.s	$f10, $f0, $f20
	s.s	$f10, 4($19)
	l.s	$f0, 4($19)
	c.le.s	$f0, $f24
	bc1t	$100192
$100193:
	.loc	2 1549
 #1549	                if (D_803B9BC8[p][i].angle > 1.5707964f)
	li.s	$f4, 1.5707964
	c.lt.s	$f4, $f0
	bc1f	$100196
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
	bc1f	$100194
	li.s	$f8, 1.0471976
	.set	 volatile
	l.s	$f10, 0($25)
	.set	 novolatile
	mul.s	$f4, $f8, $f10
	li.s	$f6, 4.0
	mul.s	$f2, $f4, $f6
	b	$100195
$100194:
	mov.s	$f2, $f12
$100195:
	add.s	$f8, $f0, $f2
	s.s	$f8, 4($19)
	b	$100205
$100196:
	.loc	2 1551
 #1551	                else if (D_803B9BC8[p][i].angle < -1.5707964f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f + D_803B9BC8[p][i].angle);
	li.s	$f10, -1.5707963705062866e+00
	c.lt.s	$f0, $f10
	bc1f	$100199
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
	bc1f	$100197
	li.s	$f6, 1.0471976
	.set	 volatile
	l.s	$f8, 0($15)
	.set	 novolatile
	mul.s	$f10, $f6, $f8
	li.s	$f4, 4.0
	mul.s	$f2, $f10, $f4
	b	$100198
$100197:
	mov.s	$f2, $f12
$100198:
	sub.s	$f6, $f0, $f2
	s.s	$f6, 4($19)
	b	$100205
$100199:
	.loc	2 1552
 #1552	                else if (D_803B9BC8[p][i].angle > 0.f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, D_803B9BC8[p][i].angle);
	li.s	$f8, 0.
	c.lt.s	$f8, $f0
	bc1f	$100202
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
	bc1f	$100200
	li.s	$f4, 1.0471976
	.set	 volatile
	l.s	$f6, 0($24)
	.set	 novolatile
	mul.s	$f8, $f4, $f6
	li.s	$f10, 4.0
	mul.s	$f2, $f8, $f10
	b	$100201
$100200:
	mov.s	$f2, $f0
$100201:
	sub.s	$f4, $f0, $f2
	s.s	$f4, 4($19)
	b	$100205
$100202:
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
	bc1f	$100203
	li.s	$f8, 1.0471976
	.set	 volatile
	l.s	$f10, 0($14)
	.set	 novolatile
	mul.s	$f4, $f8, $f10
	li.s	$f6, 4.0
	mul.s	$f2, $f4, $f6
	b	$100204
$100203:
	mov.s	$f2, $f12
$100204:
	add.s	$f8, $f0, $f2
	s.s	$f8, 4($19)
$100205:
	.loc	2 1554
 #1554	                D_803B9BC8[p][i].angle += 1.5707964f;
	l.s	$f10, 4($19)
	li.s	$f4, 1.5707964
	add.s	$f6, $f10, $f4
	s.s	$f6, 4($19)
	b	$100207
$100206:
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
	bc1f	$100207
	.loc	2 1557
	sub.s	$f10, $f0, $f20
	s.s	$f10, 4($19)
$100207:
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
	bne	$2, 10, $100209
	.loc	2 1562
	.loc	2 1563
 #1563	                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 3.2f; carPos[2] = 40.5f; }
	lh	$15, D_80151AD0
	bne	$15, 1, $100208
	.loc	2 1563
	.loc	2 1563
	s.s	$f30, 192($sp)
	.loc	2 1563
	li.s	$f6, 3.2
	s.s	$f6, 196($sp)
	.loc	2 1563
	li.s	$f4, 40.5
	s.s	$f4, 200($sp)
	b	$100211
$100208:
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
	b	$100211
$100209:
	.loc	2 1565
 #1565	            } else {
	.loc	2 1566
 #1566	                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 2.6f; carPos[2] = 40.5f; }
	lh	$24, D_80151AD0
	bne	$24, 1, $100210
	.loc	2 1566
	.loc	2 1566
	s.s	$f30, 192($sp)
	.loc	2 1566
	li.s	$f6, 2.6
	s.s	$f6, 196($sp)
	.loc	2 1566
	li.s	$f4, 40.5
	s.s	$f4, 200($sp)
	b	$100211
$100210:
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
$100211:
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
	beq	$17, 0, $100233
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
	bne	$17, 0, $100234
	.set	 noat
	mfc1	$17, $f10
	li	$1, -2147483648
	or	$17, $17, $1
	.set	 at
	b	$100232
$100234:
	li	$17, -1
	b	$100232
$100233:
	mfc1	$17, $f10
	blt	$17, 0, $100234
$100232:
	ctc1	$14, $31
	and	$25, $17, 255
	move	$17, $25
	.loc	2 1570
 #1570	            if (D_8014A110 == 6 && D_801164BE == 0)
	lw	$15, D_8014A110
	bne	$15, 6, $100212
	lh	$24, D_801164BE
	bne	$24, 0, $100212
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
	b	$100213
$100212:
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
$100213:
	.loc	2 1573
 #1573	            if (D_8014A108 < 3 && D_8014A110 != 2) {
	lh	$14, D_8014A108
	bge	$14, 3, $100219
	lw	$25, D_8014A110
	beq	$25, 2, $100219
	.loc	2 1573
	.loc	2 1574
 #1574	                reflectIdx = p == 0 ? 2 : 3;
	bne	$22, 0, $100214
	li	$16, 2
	b	$100215
$100214:
	li	$16, 3
$100215:
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
	bne	$15, 10, $100216
	.loc	2 1580
	li.s	$f4, 4.0
	sub.s	$f0, $f0, $f4
	s.s	$f0, 136($sp)
	b	$100217
$100216:
	.loc	2 1581
 #1581	                else reflectPos[1] -= 2.5f;
	li.s	$f8, 2.5
	sub.s	$f0, $f0, $f8
	s.s	$f0, 136($sp)
$100217:
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
	bne	$24, 6, $100218
	lh	$14, D_801164BE
	bne	$14, 0, $100218
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
	bge	$17, $0, $100235
	li.s	$f4, 4294967296.0
	add.s	$f6, $f6, $f4
$100235:
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
	beq	$14, 0, $100237
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
	bne	$14, 0, $100238
	.set	 noat
	mfc1	$14, $f4
	li	$1, -2147483648
	or	$14, $14, $1
	.set	 at
	b	$100236
$100238:
	li	$14, -1
	b	$100236
$100237:
	mfc1	$14, $f4
	blt	$14, 0, $100238
$100236:
	ctc1	$24, $31
	sw	$14, 32($sp)
	.livereg	0x0F00000E,0x00000000
	jal	func_80390DCC
	.alias	$3,$sp
	b	$100219
$100218:
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
	bge	$17, $0, $100239
	li.s	$f10, 4294967296.0
	add.s	$f8, $f8, $f10
$100239:
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
	beq	$24, 0, $100241
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
	bne	$24, 0, $100242
	.set	 noat
	mfc1	$24, $f10
	li	$1, -2147483648
	or	$24, $24, $1
	.set	 at
	b	$100240
$100242:
	li	$24, -1
	b	$100240
$100241:
	mfc1	$24, $f10
	blt	$24, 0, $100242
$100240:
	ctc1	$15, $31
	sw	$24, 32($sp)
	.livereg	0x2F00000E,0x00000000
	jal	func_80390DCC
	.alias	$3,$sp
$100219:
	lw	$2, 208($sp)
	.loc	2 1587
 #1586	            }
 #1587	            if (((volatile s8 *)D_803B9FD4)[p] == i) {
	lw	$14, 100($sp)
	.set	 volatile
	lb	$25, 0($14)
	.set	 novolatile
	bne	$2, $25, $100227
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
	bne	$14, 1, $100220
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
	b	$100221
$100220:
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
$100221:
	.loc	2 1608
 #1607	                }
 #1608	                if (D_8014A110 == 2) {
	lw	$25, D_8014A110
	bne	$25, 2, $100227
	.loc	2 1608
	.loc	2 1609
 #1609	                    for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$16, 1
	lb	$15, D_803BA00C
	addu	$24, $15, 1
	blt	$24, 2, $100227
	la	$18, D_80152698+4
	la	$17, D_803BA018+2
$100222:
	.loc	2 1609
	.loc	2 1610
 #1610	                        if (!D_80152698[ghostIdx] || D_803BA018[ghostIdx] != (*(*D_80152698[ghostIdx])->active)->car) {
	.noalias	$18,$19
	.noalias	$18,$23
	.noalias	$18,$sp
	lw	$8, 0($18)
	beq	$8, 0, $100223
	.noalias	$17,$18
	.noalias	$17,$19
	.noalias	$17,$23
	.noalias	$17,$sp
	lh	$14, 0($17)
	lw	$25, 0($8)
	lw	$15, 40($25)
	lw	$24, 0($15)
	lbu	$25, 8($24)
	beq	$14, $25, $100224
$100223:
	.loc	2 1610
	.loc	2 1611
 #1611	                            if (D_803BA018[ghostIdx] != -1) func_80390DCC(D_803B9BC8[ghostIdx][D_803BA018[ghostIdx]].car, ghostIdx, 0, 0, 0, 0, carPos, uv, 0);
	lh	$2, 0($17)
	beq	$2, -1, $100224
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
$100224:
	.loc	2 1613
 #1612	                        }
 #1613	                        if (!D_80152698[ghostIdx]) continue;
	bne	$8, 0, $100225
	.loc	2 1613
	lb	$2, D_803BA00C
	addu	$2, $2, 1
	b	$100226
$100225:
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
$100226:
	addu	$16, $16, 1
	addu	$18, $18, 4
	addu	$17, $17, 2
	blt	$16, $2, $100222
	.alias	$17,$18
	.alias	$17,$19
	.alias	$17,$23
	.alias	$17,$sp
	.alias	$18,$19
	.alias	$18,$23
	.alias	$18,$sp
$100227:
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
	blt	$2, $15, $100181
	.alias	$19,$sp
	lh	$2, D_8014A108
$100228:
	.loc	2 1626
 #1626	        if (D_8014A108 == 1 && D_8014A110 != 2) {
	bne	$2, 1, $100230
	lw	$25, D_8014A110
	beq	$25, 2, $100230
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
	beq	$4, 0, $100229
	mul	$24, $22, 76
	mul	$15, $22, 4
	lw	$25, D_803B28B8($15)
	mul	$14, $25, 4
	addu	$15, $24, $14
	lw	$2, D_803BA060($15)
	seq	$4, $2, 1
	bne	$4, 0, $100229
	seq	$4, $2, 2
	bne	$4, 0, $100229
	seq	$4, $2, 3
	bne	$4, 0, $100229
	seq	$4, $2, 4
	bne	$4, 0, $100229
	seq	$4, $2, 5
$100229:
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
$100230:
	.loc	2 1526
 #1526	    for (p = 0; p < D_8014A108; p++) {
	addu	$22, $22, 1
	lw	$25, 96($sp)
	addu	$24, $25, 2
	sw	$24, 96($sp)
	lw	$14, 92($sp)
	addu	$15, $14, 2
	sw	$15, 92($sp)
	blt	$22, $2, $100180
	.alias	$23,$sp
	.loc	2 1634
 #1634	}
$100231:
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
	la	$14, $$100178
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
	bne	$24, 1, $100243
	.loc	2 1044
	li.s	$f4, 1.0000000000000000e+00
	s.s	$f4, zoom
$100243:
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
$100244:
	.loc	2 1045
	.loc	2 1046
 #1046	        if(D_803AF9A8[p][buttonIdx].handle==-1) {
	.noalias	$16,$30
	.noalias	$16,$sp
	lw	$24, 4($16)
	bne	$20, $24, $100248
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
	beq	$3, 0, $100245
	beq	$3, 1, $100245
	bne	$3, 2, $100246
$100245:
	move	$7, $22
	b	$100247
$100246:
	move	$7, $0
$100247:
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
$100248:
	.loc	2 1045
 #1045	    for(buttonIdx=0;buttonIdx<44;buttonIdx++) {
	addu	$18, $18, 64
	addu	$16, $16, 64
	bne	$18, $23, $100244
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
	blt	$14, 2, $100249
	li.s	$f8, 105.0
	s.s	$f8, 2484($16)
	b	$100250
$100249:
	li.s	$f10, 115.0
	s.s	$f10, 2484($16)
$100250:
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
	bne	$15, 2, $100251
	.loc	2 1067
	lw	$4, 2436($16)
	move	$5, $0
	li	$6, 15
	.livereg	0x0E00000E,0x00000000
	jal	model_transform_setup
$100251:
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
	blt	$25, 2, $100252
	li.s	$f24, 50.2
	s.s	$f24, 2612($16)
	li.s	$f26, 55.2
	b	$100253
$100252:
	li.s	$f26, 55.2
	s.s	$f26, 2612($16)
	li.s	$f24, 50.2
$100253:
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
	blt	$24, 2, $100254
	s.s	$f24, 2676($16)
	b	$100255
$100254:
	s.s	$f26, 2676($16)
$100255:
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
	blt	$14, 2, $100256
	s.s	$f24, 2740($16)
	b	$100257
$100256:
	s.s	$f26, 2740($16)
$100257:
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
	blt	$15, 2, $100258
	s.s	$f24, 2804($16)
	b	$100259
$100258:
	s.s	$f26, 2804($16)
$100259:
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
$100260:
	.loc	2 202
	.noalias	$17,$sp
	sw	$0, 0($17)
	.loc	2 202
	addu	$17, $17, 4
	bltu	$17, $2, $100260
	.alias	$17,$sp
	.loc	2 203
 # 203	    for (i = 0; i < 5; i++) nearGhosts[i] = 0;
	move	$16, $0
	addu	$3, $sp, 84
	addu	$2, $sp, 104
$100261:
	.loc	2 203
	.noalias	$3,$gp
	sw	$0, 0($3)
	.loc	2 203
	addu	$3, $3, 4
	bltu	$3, $2, $100261
	.alias	$3,$gp
	.loc	2 204
 # 204	    for (i = 1; i < 4; i++) D_803BA018[i] = -1;
	li	$16, 1
	la	$2, D_803BA018+2
	la	$4, D_803BA018+8
	li	$3, -1
$100262:
	.loc	2 204
	.noalias	$2,$sp
	sh	$3, 0($2)
	.loc	2 204
	addu	$2, $2, 2
	bne	$2, $4, $100262
	.alias	$2,$sp
	.loc	2 205
 # 205	    if (D_8014A110 != 2) return;
	lw	$15, D_8014A110
	bne	$15, 2, $100340
	.loc	2 205
	.loc	2 206
 # 206	    if (!D_801426EC) {
	lb	$24, D_801426EC
	bne	$24, 0, $100318
	.loc	2 206
	.loc	2 207
 # 207	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	lw	$19, D_80152020+8
	beq	$19, 0, $100266
	la	$30, D_8014978C
$100263:
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
	bne	$25, $14, $100265
	lb	$15, D_80152570
	lbu	$24, 9($18)
	bne	$15, $24, $100265
	addu	$4, $18, 10
	addu	$5, $20, 20
	.livereg	0x0C00000E,0x00000000
	jal	func_8008AD04
	bne	$2, 0, $100265
	lw	$25, 24($18)
	lw	$14, 12($20)
	bne	$25, $14, $100265
	lw	$15, 28($18)
	lw	$24, 16($20)
	bne	$15, $24, $100265
	.loc	2 212
 # 211	                !func_8008AD04(ghost->name, player->name) &&
 # 212	                player->id0 == ghost->id0 && player->id1 == ghost->id1) {
	.loc	2 213
 # 213	                if ((!best || ghost->time < (*best)->time) && menu_item_value_get(ghosth)) best = ghosth;
	beq	$22, 0, $100264
	l.s	$f4, 32($18)
	lw	$25, 0($22)
	l.s	$f6, 32($25)
	c.lt.s	$f4, $f6
	bc1f	$100265
$100264:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100265
	.loc	2 213
	move	$22, $19
$100265:
	.loc	2 207
 # 207	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $100263
$100266:
	la	$30, D_8014978C
	.loc	2 216
 # 216	        for (i = 0; i < 3; i++) {
	move	$16, $0
	li	$23, 3
$100267:
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
	beq	$24, 0, $100269
	lb	$25, 0($30)
	lb	$14, 8($18)
	bne	$25, $14, $100269
	lb	$15, D_80152570
	lbu	$24, 9($18)
	bne	$15, $24, $100269
	addu	$4, $18, 10
	addu	$5, $20, 20
	.livereg	0x0C00000E,0x00000000
	jal	func_8008AD04
	bne	$2, 0, $100269
	lw	$25, 24($18)
	lw	$14, 12($20)
	bne	$25, $14, $100269
	lw	$15, 28($18)
	lw	$24, 16($20)
	bne	$15, $24, $100269
	.loc	2 221
 # 220	                !func_8008AD04(ghost->name, player->name) &&
 # 221	                player->id0 == ghost->id0 && player->id1 == ghost->id1) {
	.loc	2 222
 # 222	                if ((!best || ghost->time < (*best)->time) && menu_item_value_get(ghosth)) best = ghosth;
	beq	$22, 0, $100268
	l.s	$f8, 32($18)
	lw	$25, 0($22)
	l.s	$f10, 32($25)
	c.lt.s	$f8, $f10
	bc1f	$100269
$100268:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100269
	.loc	2 222
	move	$22, $19
$100269:
	.loc	2 216
 # 216	        for (i = 0; i < 3; i++) {
	addu	$16, $16, 1
	bne	$16, $23, $100267
	lw	$2, D_80152020+8
	.loc	2 225
 # 225	        if (!best) {
	bne	$22, 0, $100292
	.loc	2 225
	.loc	2 226
 # 226	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $2
	beq	$19, 0, $100279
$100270:
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
	bne	$14, $15, $100278
	lb	$24, D_80152570
	lbu	$25, 9($18)
	bne	$24, $25, $100278
	.loc	2 229
	.loc	2 230
 # 230	                    for (i = 3; i >= 1; i--) {
	li	$16, 3
	la	$17, D_80152698+12
$100271:
	.loc	2 230
	.loc	2 231
 # 231	                        if ((!D_80152698[i] || ghost->time > (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $100272
	lw	$14, 0($2)
	l.s	$f4, 32($14)
	l.s	$f6, 32($18)
	c.lt.s	$f4, $f6
	bc1f	$100277
$100272:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100277
	.loc	2 231
	.loc	2 232
 # 232	                            for (j = 1; j < i; j++)
	li	$3, 1
	blt	$16, 2, $100276
	addu	$5, $16, -1
	and	$15, $5, 3
	move	$5, $15
	move	$4, $5
	beq	$4, 0, $100274
	addu	$4, $5, 1
	mul	$24, $3, 4
	la	$25, D_80152698
	addu	$2, $24, $25
$100273:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$14, 4($2)
	sw	$14, 0($2)
	addu	$3, $3, 1
	addu	$2, $2, 4
	bne	$4, $3, $100273
	.alias	$2,$30
	.alias	$2,$sp
	beq	$3, $16, $100276
$100274:
	mul	$15, $3, 4
	la	$24, D_80152698
	addu	$2, $15, $24
	mul	$25, $16, 4
	addu	$4, $25, $24
$100275:
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
	bne	$2, $4, $100275
	.alias	$2,$30
	.alias	$2,$sp
$100276:
	.loc	2 234
 # 233	                                D_80152698[j] = D_80152698[j+1];
 # 234	                            D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 235
 # 235	                            break;
	b	$100278
$100277:
	.loc	2 230
 # 230	                    for (i = 3; i >= 1; i--) {
	addu	$16, $16, -1
	addu	$17, $17, -4
	bne	$16, 0, $100271
	.alias	$17,$30
	.alias	$17,$sp
$100278:
	.loc	2 226
 # 226	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $100270
$100279:
	.loc	2 240
 # 240	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$3, D_803BA010
	move	$19, $3
	addu	$2, $3, 12
	bgeu	$3, $2, $100289
$100280:
	.loc	2 240
	.loc	2 241
 # 241	                ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 242
 # 242	                if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570) {
	lw	$14, 40($18)
	beq	$14, 0, $100288
	lb	$15, 0($30)
	lb	$25, 8($18)
	bne	$15, $25, $100288
	lb	$24, D_80152570
	lbu	$14, 9($18)
	bne	$24, $14, $100288
	.loc	2 242
	.loc	2 243
 # 243	                    for (i = 3; i >= 1; i--) {
	li	$16, 3
	la	$17, D_80152698+12
$100281:
	.loc	2 243
	.loc	2 244
 # 244	                        if ((!D_80152698[i] || ghost->time > (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $100282
	lw	$15, 0($2)
	l.s	$f8, 32($15)
	l.s	$f10, 32($18)
	c.lt.s	$f8, $f10
	bc1f	$100287
$100282:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100287
	.loc	2 244
	.loc	2 245
 # 245	                            for (j = 1; j < i; j++)
	li	$3, 1
	blt	$16, 2, $100286
	addu	$5, $16, -1
	and	$25, $5, 3
	move	$5, $25
	move	$4, $5
	beq	$4, 0, $100284
	addu	$4, $5, 1
	mul	$24, $3, 4
	la	$14, D_80152698
	addu	$2, $24, $14
$100283:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$15, 4($2)
	sw	$15, 0($2)
	addu	$3, $3, 1
	addu	$2, $2, 4
	bne	$4, $3, $100283
	.alias	$2,$30
	.alias	$2,$sp
	beq	$3, $16, $100286
$100284:
	mul	$25, $3, 4
	la	$24, D_80152698
	addu	$2, $25, $24
	mul	$14, $16, 4
	addu	$4, $14, $24
$100285:
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
	bne	$2, $4, $100285
	.alias	$2,$30
	.alias	$2,$sp
$100286:
	.loc	2 247
 # 246	                                D_80152698[j] = D_80152698[j+1];
 # 247	                            D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 248
 # 248	                            break;
	lw	$2, D_803BA010
	addu	$2, $2, 12
	b	$100288
$100287:
	.loc	2 243
 # 243	                    for (i = 3; i >= 1; i--) {
	addu	$16, $16, -1
	addu	$17, $17, -4
	bne	$16, 0, $100281
	.alias	$17,$30
	.alias	$17,$sp
	lw	$2, D_803BA010
	addu	$2, $2, 12
$100288:
	.loc	2 240
 # 240	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	addu	$19, $19, 4
	bltu	$19, $2, $100280
	.alias	$30,$sp
$100289:
	.loc	2 253
 # 253	            for (i = 1; i < 3; i++) {
	li	$16, 1
	la	$18, D_80152698
$100290:
	.loc	2 253
	.loc	2 254
 # 254	                if (!D_80152698[i] && D_80152698[i+1]) {
	.noalias	$18,$sp
	.noalias	$17,$sp
	mul	$15, $16, 4
	addu	$17, $18, $15
	lw	$25, 0($17)
	bne	$25, 0, $100291
	lw	$2, 4($17)
	beq	$2, 0, $100291
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
$100291:
	.loc	2 253
 # 253	            for (i = 1; i < 3; i++) {
	addu	$16, $16, 1
	blt	$16, 3, $100290
	b	$100338
	.alias	$18,$sp
$100292:
	.loc	2 260
 # 260	        } else {
	.loc	2 261
 # 261	            nearGhosts[2] = best;
	sw	$22, 92($sp)
	.loc	2 262
 # 262	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $2
	beq	$19, 0, $100302
$100293:
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
	bne	$14, $24, $100301
	lb	$15, D_80152570
	lbu	$25, 9($18)
	bne	$15, $25, $100301
	.loc	2 265
	.loc	2 266
 # 266	                    if (ghosth != best) {
	beq	$19, $22, $100301
	.loc	2 266
	.loc	2 267
 # 267	                        if (ghost->time < (*best)->time) {
	l.s	$f0, 32($18)
	lw	$14, 0($22)
	l.s	$f4, 32($14)
	c.lt.s	$f0, $f4
	bc1f	$100297
	.loc	2 267
	.loc	2 268
 # 268	                            if (!nearGhosts[1] || ghost->time > (*nearGhosts[1])->time) {
	lw	$24, 88($sp)
	beq	$24, $0, $100294
	lw	$15, 0($24)
	l.s	$f6, 32($15)
	c.lt.s	$f6, $f0
	bc1f	$100295
$100294:
	.loc	2 268
	.loc	2 269
 # 269	                                if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100301
	.loc	2 269
	.loc	2 270
 # 270	                                    nearGhosts[0] = nearGhosts[1];
	lw	$25, 88($sp)
	sw	$25, 84($sp)
	.loc	2 271
 # 271	                                    nearGhosts[1] = ghosth;
	sw	$19, 88($sp)
	b	$100301
$100295:
	.loc	2 273
 # 272	                                }
 # 273	                            } else if (!nearGhosts[0] || ghost->time > (*nearGhosts[0])->time) {
	lw	$14, 84($sp)
	beq	$14, $0, $100296
	lw	$24, 0($14)
	l.s	$f8, 32($24)
	c.lt.s	$f8, $f0
	bc1f	$100301
$100296:
	.loc	2 273
	.loc	2 274
 # 274	                                if (menu_item_value_get(ghosth)) nearGhosts[0] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100301
	.loc	2 274
	sw	$19, 84($sp)
	b	$100301
$100297:
	.loc	2 276
 # 275	                            }
 # 276	                        } else {
	.loc	2 277
 # 277	                            if (!nearGhosts[3] || ghost->time < (*nearGhosts[3])->time) {
	lw	$15, 96($sp)
	beq	$15, $0, $100298
	lw	$25, 0($15)
	l.s	$f10, 32($25)
	c.lt.s	$f0, $f10
	bc1f	$100299
$100298:
	.loc	2 277
	.loc	2 278
 # 278	                                if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100301
	.loc	2 278
	.loc	2 279
 # 279	                                    nearGhosts[4] = nearGhosts[3];
	lw	$14, 96($sp)
	sw	$14, 100($sp)
	.loc	2 280
 # 280	                                    nearGhosts[3] = ghosth;
	sw	$19, 96($sp)
	b	$100301
$100299:
	.loc	2 282
 # 281	                                }
 # 282	                            } else if (!nearGhosts[4] || ghost->time < (*nearGhosts[4])->time) {
	lw	$24, 100($sp)
	beq	$24, $0, $100300
	lw	$15, 0($24)
	l.s	$f4, 32($15)
	c.lt.s	$f0, $f4
	bc1f	$100301
$100300:
	.loc	2 282
	.loc	2 283
 # 283	                                if (menu_item_value_get(ghosth)) nearGhosts[4] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100301
	.loc	2 283
	sw	$19, 100($sp)
$100301:
	.loc	2 262
 # 262	            for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $100293
$100302:
	.loc	2 289
 # 289	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$3, D_803BA010
	move	$19, $3
	addu	$2, $3, 12
	bgeu	$3, $2, $100313
$100303:
	.loc	2 289
	.loc	2 290
 # 290	                ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 291
 # 291	                if (!ghost->active || ghost->track != D_8014978C || ghost->mode != D_80152570 || ghosth == best) continue;
	lw	$25, 40($18)
	beq	$25, 0, $100312
	lb	$14, 0($30)
	lb	$24, 8($18)
	bne	$14, $24, $100312
	lb	$15, D_80152570
	lbu	$25, 9($18)
	bne	$15, $25, $100312
	beq	$19, $22, $100312
	.loc	2 291
	.loc	2 292
 # 292	                if (ghost->time < (*best)->time) {
	l.s	$f0, 32($18)
	lw	$14, 0($22)
	l.s	$f6, 32($14)
	c.lt.s	$f0, $f6
	bc1f	$100307
	.loc	2 292
	.loc	2 293
 # 293	                    if (!nearGhosts[1] || ghost->time > (*nearGhosts[1])->time) {
	lw	$24, 88($sp)
	beq	$24, $0, $100304
	lw	$15, 0($24)
	l.s	$f8, 32($15)
	c.lt.s	$f8, $f0
	bc1f	$100305
$100304:
	.loc	2 293
	.loc	2 294
 # 294	                        if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100311
	.loc	2 294
	.loc	2 295
 # 295	                            nearGhosts[0] = nearGhosts[1];
	lw	$25, 88($sp)
	sw	$25, 84($sp)
	.loc	2 296
 # 296	                            nearGhosts[1] = ghosth;
	sw	$19, 88($sp)
	b	$100311
$100305:
	.loc	2 298
 # 297	                        }
 # 298	                    } else if (!nearGhosts[0] || ghost->time > (*nearGhosts[0])->time) {
	lw	$14, 84($sp)
	beq	$14, $0, $100306
	lw	$24, 0($14)
	l.s	$f10, 32($24)
	c.lt.s	$f10, $f0
	bc1f	$100311
$100306:
	.loc	2 298
	.loc	2 299
 # 299	                        if (menu_item_value_get(ghosth)) nearGhosts[0] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100311
	.loc	2 299
	sw	$19, 84($sp)
	b	$100311
$100307:
	.loc	2 301
 # 300	                    }
 # 301	                } else {
	.loc	2 302
 # 302	                    if (!nearGhosts[3] || ghost->time < (*nearGhosts[3])->time) {
	lw	$15, 96($sp)
	beq	$15, $0, $100308
	lw	$25, 0($15)
	l.s	$f4, 32($25)
	c.lt.s	$f0, $f4
	bc1f	$100309
$100308:
	.loc	2 302
	.loc	2 303
 # 303	                        if (menu_item_value_get(ghosth)) {
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100311
	.loc	2 303
	.loc	2 304
 # 304	                            nearGhosts[4] = nearGhosts[3];
	lw	$14, 96($sp)
	sw	$14, 100($sp)
	.loc	2 305
 # 305	                            nearGhosts[3] = ghosth;
	sw	$19, 96($sp)
	b	$100311
$100309:
	.loc	2 307
 # 306	                        }
 # 307	                    } else if (!nearGhosts[4] || ghost->time < (*nearGhosts[4])->time) {
	lw	$24, 100($sp)
	beq	$24, $0, $100310
	lw	$15, 0($24)
	l.s	$f6, 32($15)
	c.lt.s	$f0, $f6
	bc1f	$100311
$100310:
	.loc	2 307
	.loc	2 308
 # 308	                        if (menu_item_value_get(ghosth)) nearGhosts[4] = ghosth;
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100311
	.loc	2 308
	sw	$19, 100($sp)
$100311:
	.loc	2 289
 # 289	            for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$2, D_803BA010
	addu	$2, $2, 12
$100312:
	addu	$19, $19, 4
	bltu	$19, $2, $100303
	.alias	$30,$sp
$100313:
	.loc	2 312
 # 312	            ghostCount = 1;
	li	$2, 1
	.loc	2 313
 # 313	            for (i = 0; i < 5; i++) {
	lw	$25, 84($sp)
	beq	$25, $0, $100314
	sw	$25, D_80152698+4
	li	$2, 2
$100314:
	li	$16, 1
	.loc	2 313
	.loc	2 314
 # 314	                if (nearGhosts[i] && ghostCount < 4) { D_80152698[ghostCount] = nearGhosts[i]; ghostCount++; }
	.noalias	$3,$gp
	mul	$14, $16, 4
	addu	$24, $sp, 84
	addu	$3, $14, $24
	lw	$4, 0($3)
	beq	$4, 0, $100315
	bge	$2, 4, $100315
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
$100315:
	la	$18, D_80152698
	.loc	2 313
 # 313	            for (i = 0; i < 5; i++) {
	lw	$4, 4($3)
	beq	$4, 0, $100316
	bge	$2, 4, $100316
	mul	$14, $2, 4
	addu	$24, $18, $14
	.noalias	$24,$3
	.noalias	$24,$sp
	sw	$4, 0($24)
	.alias	$24,$3
	.alias	$24,$sp
	addu	$2, $2, 1
$100316:
	lw	$4, 8($3)
	beq	$4, 0, $100317
	bge	$2, 4, $100317
	mul	$15, $2, 4
	addu	$25, $18, $15
	.noalias	$25,$3
	.noalias	$25,$sp
	sw	$4, 0($25)
	.alias	$25,$3
	.alias	$25,$sp
	addu	$2, $2, 1
$100317:
	lw	$4, 12($3)
	beq	$4, 0, $100338
	.alias	$3,$18
	.alias	$3,$gp
	bge	$2, 4, $100338
	mul	$14, $2, 4
	addu	$24, $18, $14
	.noalias	$24,$sp
	sw	$4, 0($24)
	.alias	$24,$sp
	b	$100338
	.alias	$18,$sp
$100318:
	.loc	2 317
 # 314	                if (nearGhosts[i] && ghostCount < 4) { D_80152698[ghostCount] = nearGhosts[i]; ghostCount++; }
 # 315	            }
 # 316	        }
 # 317	    } else {
	.loc	2 318
 # 318	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	lw	$19, D_80152020+8
	beq	$19, 0, $100328
	la	$30, D_8014978C
	li	$23, 3
	li	$20, 4
$100319:
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
	bne	$15, $25, $100327
	lb	$14, D_80152570
	lbu	$24, 9($18)
	bne	$14, $24, $100327
	.loc	2 321
	.loc	2 322
 # 322	                for (i = 1; i < 4; i++) {
	li	$16, 1
	la	$17, D_80152698+4
$100320:
	.loc	2 322
	.loc	2 323
 # 323	                    if ((!D_80152698[i] || ghost->time < (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $100321
	l.s	$f8, 32($18)
	lw	$15, 0($2)
	l.s	$f10, 32($15)
	c.lt.s	$f8, $f10
	bc1f	$100326
$100321:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100326
	.loc	2 323
	.loc	2 324
 # 324	                        for (j = 3; j > i; j--)
	move	$3, $23
	bge	$16, 3, $100325
	subu	$5, $23, $16
	and	$25, $5, 3
	move	$5, $25
	negu	$5, $5
	move	$4, $5
	beq	$4, 0, $100323
	addu	$4, $5, 3
	mul	$14, $3, 4
	la	$24, D_80152698
	addu	$2, $14, $24
$100322:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$15, -4($2)
	sw	$15, 0($2)
	addu	$3, $3, -1
	addu	$2, $2, -4
	bne	$4, $3, $100322
	.alias	$2,$30
	.alias	$2,$sp
	beq	$16, $3, $100325
$100323:
	mul	$25, $3, 4
	la	$14, D_80152698
	addu	$2, $25, $14
	mul	$24, $16, 4
	addu	$4, $24, $14
$100324:
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
	bne	$4, $2, $100324
	.alias	$2,$30
	.alias	$2,$sp
$100325:
	.loc	2 326
 # 325	                            D_80152698[j] = D_80152698[j-1];
 # 326	                        D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 327
 # 327	                        break;
	b	$100327
$100326:
	.loc	2 322
 # 322	                for (i = 1; i < 4; i++) {
	addu	$16, $16, 1
	addu	$17, $17, 4
	bne	$16, $20, $100320
	.alias	$17,$30
	.alias	$17,$sp
$100327:
	.loc	2 318
 # 318	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	move	$19, $21
	bne	$21, 0, $100319
$100328:
	li	$20, 4
	li	$23, 3
	la	$30, D_8014978C
	.loc	2 332
 # 332	        for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	lw	$3, D_803BA010
	move	$19, $3
	addu	$2, $3, 12
	bgeu	$3, $2, $100338
$100329:
	.loc	2 332
	.loc	2 333
 # 333	            ghost = *ghosth;
	lw	$18, 0($19)
	.loc	2 334
 # 334	            if (ghost->active && ghost->track == D_8014978C && ghost->mode == D_80152570) {
	lw	$15, 40($18)
	beq	$15, 0, $100337
	lb	$25, 0($30)
	lb	$24, 8($18)
	bne	$25, $24, $100337
	lb	$14, D_80152570
	lbu	$15, 9($18)
	bne	$14, $15, $100337
	.loc	2 334
	.loc	2 335
 # 335	                for (i = 1; i < 4; i++) {
	li	$16, 1
	la	$17, D_80152698+4
$100330:
	.loc	2 335
	.loc	2 336
 # 336	                    if ((!D_80152698[i] || ghost->time < (*D_80152698[i])->time) && menu_item_value_get(ghosth)) {
	.noalias	$17,$30
	.noalias	$17,$sp
	lw	$2, 0($17)
	beq	$2, 0, $100331
	l.s	$f4, 32($18)
	lw	$25, 0($2)
	l.s	$f6, 32($25)
	c.lt.s	$f4, $f6
	bc1f	$100336
$100331:
	move	$4, $19
	.livereg	0x0800000E,0x00000000
	jal	menu_item_value_get
	beq	$2, 0, $100336
	.loc	2 336
	.loc	2 337
 # 337	                        for (j = 3; j > i; j--)
	move	$3, $23
	bge	$16, 3, $100335
	subu	$5, $23, $16
	and	$24, $5, 3
	move	$5, $24
	negu	$5, $5
	move	$4, $5
	beq	$4, 0, $100333
	addu	$4, $5, 3
	mul	$14, $3, 4
	la	$15, D_80152698
	addu	$2, $14, $15
$100332:
	.noalias	$2,$30
	.noalias	$2,$sp
	lw	$25, -4($2)
	sw	$25, 0($2)
	addu	$3, $3, -1
	addu	$2, $2, -4
	bne	$4, $3, $100332
	.alias	$2,$30
	.alias	$2,$sp
	beq	$16, $3, $100335
$100333:
	mul	$24, $3, 4
	la	$14, D_80152698
	addu	$2, $24, $14
	mul	$15, $16, 4
	addu	$4, $15, $14
$100334:
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
	bne	$4, $2, $100334
	.alias	$2,$30
	.alias	$2,$sp
$100335:
	.loc	2 339
 # 338	                            D_80152698[j] = D_80152698[j-1];
 # 339	                        D_80152698[i] = ghosth;
	sw	$19, 0($17)
	.loc	2 340
 # 340	                        break;
	lw	$2, D_803BA010
	addu	$2, $2, 12
	b	$100337
$100336:
	.loc	2 335
 # 335	                for (i = 1; i < 4; i++) {
	addu	$16, $16, 1
	addu	$17, $17, 4
	bne	$16, $20, $100330
	.alias	$17,$30
	.alias	$17,$sp
	lw	$2, D_803BA010
	addu	$2, $2, 12
$100337:
	.loc	2 332
 # 332	        for (ghosth = D_803BA010; ghosth < D_803BA010 + 3; ghosth++) {
	addu	$19, $19, 4
	bltu	$19, $2, $100329
	.alias	$30,$sp
$100338:
	la	$18, D_80152698
	.loc	2 346
 # 346	    if (!D_80156994) {
	lb	$25, D_80156994
	bne	$25, 0, $100340
	.loc	2 346
	.loc	2 347
 # 347	        if (D_80152698[2]) { menu_transition((void **)D_80152698[2]); D_80152698[2] = 0; }
	.noalias	$18,$sp
	lw	$4, 8($18)
	beq	$4, 0, $100339
	.loc	2 347
	.loc	2 347
	.livereg	0x0800000E,0x00000000
	jal	menu_transition
	.loc	2 347
	sw	$0, 8($18)
$100339:
	.loc	2 348
 # 348	        if (D_80152698[3]) { menu_transition((void **)D_80152698[3]); D_80152698[3] = 0; }
	lw	$4, 12($18)
	beq	$4, 0, $100340
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
$100340:
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
	beq	$14, 0, $100341
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
	b	$100343
$100341:
	.loc	2 386
 # 386	    } else if (D_8014978C >= 6) {
	lb	$15, D_8014978C
	blt	$15, 6, $100342
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
	b	$100343
$100342:
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
$100343:
	.loc	2 391
 # 390	    }
 # 391	    D_803BA02C = 0;
	sb	$0, D_803BA02C
	.loc	2 392
 # 392	    for (p = 0; p < D_8014A108; p++) {
	move	$20, $0
	lh	$6, D_8014A108
	ble	$6, 0, $100345
	la	$11, D_803B28B8
	la	$12, D_803BA048
	la	$13, D_803BA038
	la	$16, D_803BA028
	la	$17, D_803BA1D0
$100344:
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
	blt	$20, $6, $100344
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
$100345:
	.loc	2 401
 # 401	    for (p = 0; p < D_8014A108; p++) {
	ble	$6, 0, $100352
	la	$4, D_803BA020
	li	$7, 13
	la	$5, D_803B9FD8
	la	$2, D_803B3010
$100346:
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
$100347:
	.loc	2 402
	.loc	2 403
 # 403	            if (i < 13) D_803B9FD8[p][D_803BA020[p]++] = D_803B3010[i];
	bge	$21, 13, $100348
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
$100348:
	.loc	2 402
 # 402	        for (D_803BA020[p] = 0, i = 0; i < 13; i++) {
	bge	$21, 12, $100349
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
$100349:
	bge	$21, 11, $100350
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
$100350:
	bge	$21, 10, $100351
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
$100351:
	addu	$21, $21, 4
	bne	$21, $7, $100347
	.loc	2 401
 # 401	    for (p = 0; p < D_8014A108; p++) {
	addu	$20, $20, 1
	addu	$4, $4, 2
	blt	$20, $6, $100346
	.alias	$2,$4
	.alias	$2,$5
	.alias	$2,$18
	.alias	$2,$sp
	.alias	$4,$5
	.alias	$4,$18
	.alias	$4,$sp
	move	$20, $0
$100352:
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
$100353:
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
	bne	$16, $17, $100353
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
	bne	$2, $25, $100354
	.loc	2 415
	li	$15, 4
	sh	$15, 0($16)
$100354:
	li	$5, 2
	.loc	2 416
 # 416	    if (D_8014A110 == 2) {
	lw	$24, D_8014A110
	bne	$5, $24, $100355
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
	bne	$25, 0, $100355
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
$100355:
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
	bne	$24, 1, $100356
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
	b	$100357
$100356:
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
$100357:
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
$100358:
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
	bltu	$16, $17, $100358
	.alias	$16,$sp
	.loc	2 448
 # 448	    for (p = 0; p < D_8014A108; p++) {
	move	$20, $0
	lh	$25, D_8014A108
	ble	$25, 0, $100389
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
$100359:
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
	bne	$15, 2, $100360
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
	b	$100361
$100360:
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
$100361:
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
$100362:
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
	bne	$2, 0, $100365
$100363:
	.loc	2 473
	.loc	2 474
 # 474	                D_80111080[p + 1][i]--;
	lb	$25, 13($16)
	addu	$15, $25, -1
	sb	$15, 13($16)
	.loc	2 475
 # 475	                if (D_80111080[p + 1][i] < 0) { D_80111080[p + 1][i] = 0; break; }
	lb	$5, 13($16)
	bge	$5, 0, $100364
	.loc	2 475
	.loc	2 475
	sb	$0, 13($16)
	.loc	2 475
	b	$100365
$100364:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7604
	beq	$2, 0, $100363
$100365:
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
	bne	$2, 0, $100368
$100366:
	.loc	2 479
	.loc	2 480
 # 480	                D_8011119C[p + 1][i]--;
	lb	$24, 13($17)
	addu	$14, $24, -1
	sb	$14, 13($17)
	.loc	2 481
 # 481	                if (D_8011119C[p + 1][i] < 0) { D_8011119C[p + 1][i] = 0; break; }
	lb	$5, 13($17)
	bge	$5, 0, $100367
	.loc	2 481
	.loc	2 481
	sb	$0, 13($17)
	.loc	2 481
	b	$100368
$100367:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75D0
	beq	$2, 0, $100366
$100368:
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
	bne	$2, 0, $100371
$100369:
	.loc	2 485
	.loc	2 486
 # 486	                D_80111230[p + 1][i]--;
	lb	$25, 13($18)
	addu	$15, $25, -1
	sb	$15, 13($18)
	.loc	2 487
 # 487	                if (D_80111230[p + 1][i] < 0) { D_80111230[p + 1][i] = 0; break; }
	lb	$5, 13($18)
	bge	$5, 0, $100370
	.loc	2 487
	.loc	2 487
	sb	$0, 13($18)
	.loc	2 487
	b	$100371
$100370:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75B0
	beq	$2, 0, $100369
$100371:
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
	beq	$24, 4, $100372
	lb	$14, 13($23)
	blt	$14, 2, $100372
	.loc	2 491
	li	$25, 1
	sb	$25, 13($23)
$100372:
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
	bne	$25, 0, $100373
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
$100373:
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
	blt	$21, 13, $100362
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
	bne	$2, 0, $100377
$100374:
	.loc	2 505
	.loc	2 506
 # 506	            if (!D_803B9FD0[p]) D_803B9FD0[p] = 12;
	lw	$14, 60($sp)
	lb	$2, 0($14)
	bne	$2, 0, $100375
	.loc	2 506
	li	$25, 12
	sb	$25, 0($14)
	b	$100376
$100375:
	.loc	2 507
 # 507	            else D_803B9FD0[p]--;
	addu	$15, $2, -1
	lw	$24, 60($sp)
	sb	$15, 0($24)
$100376:
	move	$4, $20
	lw	$25, 60($sp)
	lb	$5, 0($25)
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7620
	beq	$2, 0, $100374
$100377:
	lw	$2, 56($sp)
	.loc	2 509
 # 508	        }
 # 509	        for (D_803B9FD4[p] = 0; D_803B9FD4[p] < D_803BA020[p]; D_803B9FD4[p]++) {
	.noalias	$2,$sp
	sb	$0, 0($2)
	lb	$4, 0($2)
	lw	$14, 180($sp)
	lh	$3, 0($14)
	bge	$4, $3, $100379
	.alias	$2,$sp
	mul	$15, $20, 13
	la	$24, D_803B9FD8
	addu	$5, $15, $24
	lw	$25, 60($sp)
	lb	$2, 0($25)
$100378:
	.loc	2 509
	.loc	2 510
 # 510	            if (D_803B9FD0[p] == D_803B9FD8[p][D_803B9FD4[p]]) break;
	.noalias	$5,$sp
	addu	$14, $5, $4
	.noalias	$14,$sp
	lb	$15, 0($14)
	.alias	$14,$sp
	beq	$2, $15, $100379
	.alias	$5,$sp
	.loc	2 510
	.loc	2 509
 # 509	        for (D_803B9FD4[p] = 0; D_803B9FD4[p] < D_803BA020[p]; D_803B9FD4[p]++) {
	addu	$24, $4, 1
	lw	$25, 56($sp)
	sb	$24, 0($25)
	lw	$14, 56($sp)
	lb	$4, 0($14)
	blt	$4, $3, $100378
$100379:
	.loc	2 512
 # 510	            if (D_803B9FD0[p] == D_803B9FD8[p][D_803B9FD4[p]]) break;
 # 511	        }
 # 512	        if (D_803B9FD4[p] >= D_803BA020[p]) {
	blt	$4, $3, $100380
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
$100380:
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
	ble	$3, 0, $100383
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
$100381:
	.loc	2 519
	.loc	2 520
 # 520	            D_803B9BC8[p][i].position = i - D_803B9FD4[p];
	.noalias	$2,$sp
	sb	$4, 8($2)
	.loc	2 521
 # 521	            if (D_803B9BC8[p][i].position < 0) D_803B9BC8[p][i].position += D_803BA020[p];
	lb	$6, 8($2)
	bge	$6, 0, $100382
	.loc	2 521
	addu	$14, $6, $3
	sb	$14, 8($2)
$100382:
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
	blt	$21, $3, $100381
	.alias	$2,$5
	.alias	$2,$sp
	.alias	$5,$sp
$100383:
	.loc	2 525
 # 520	            D_803B9BC8[p][i].position = i - D_803B9FD4[p];
 # 521	            if (D_803B9BC8[p][i].position < 0) D_803B9BC8[p][i].position += D_803BA020[p];
 # 522	            D_803B9BC8[p][i].car = D_803B9FD8[p][i] + p * 13;
 # 523	            D_803B9BC8[p][i].angle = 3.1415927f;
 # 524	        }
 # 525	        if (D_8014A110 == 6) D_8014A118[p].paint = 6;
	lw	$25, D_8014A110
	bne	$25, 6, $100384
	.loc	2 525
	li	$14, 6
	sb	$14, 66($22)
	b	$100387
$100384:
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
	bne	$2, 0, $100387
$100385:
	.loc	2 528
	.loc	2 529
 # 529	                D_8014A118[p].paint -= 2;
	lbu	$15, 66($22)
	addu	$24, $15, -2
	sb	$24, 66($22)
	.loc	2 530
 # 530	                if ((s32)D_8014A118[p].paint < 0) { D_8014A118[p].paint += 2; break; }
	lbu	$5, 66($22)
	bge	$5, 0, $100386
	.loc	2 530
	.loc	2 530
	addu	$25, $5, 2
	sb	$25, 66($22)
	.loc	2 530
	b	$100387
$100386:
	move	$4, $20
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75EC
	beq	$2, 0, $100385
$100387:
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
$100388:
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
	bne	$21, $3, $100388
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
	blt	$20, $24, $100359
	.alias	$6,$22
	.alias	$6,$sp
$100389:
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
	bne	$15, 0, $100390
	.loc	2 573
	lw	$24, D_8012E6F8
	sw	$24, D_803BA010
	b	$100393
$100390:
	.loc	2 574
 # 574	    else D_803BA010 = D_8012E6F8 + ((D_80152570 ? 6 : 0) + D_8014978C) * 3;
	lb	$25, D_80152570
	beq	$25, 0, $100391
	li	$3, 6
	b	$100392
$100391:
	move	$3, $0
$100392:
	lb	$14, D_8014978C
	addu	$15, $3, $14
	mul	$24, $15, 3
	sll	$25, $24, 2
	lw	$14, D_8012E6F8
	addu	$15, $25, $14
	sw	$15, D_803BA010
$100393:
	li	$3, 1
	la	$4, D_801174BC
	.loc	2 575
 # 575	    if (D_801174BC != 1) {
	.noalias	$4,$sp
	lw	$2, 0($4)
	beq	$3, $2, $100395
	.loc	2 575
	.loc	2 576
 # 576	        if (D_801174BC != D_801174B4) { func_803A0498(); func_800B5570(0x40000); return; }
	lw	$24, D_801174B4
	beq	$24, $2, $100394
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
	b	$100556
$100394:
	.loc	2 577
 # 577	        D_801174BC = 1;
	.noalias	$4,$sp
	sw	$3, 0($4)
	.alias	$4,$sp
$100395:
	.loc	2 579
 # 578	    }
 # 579	    for (i = 1; i < 4; i++) {
	li	$16, 1
$100396:
	.loc	2 579
	.loc	2 580
 # 580	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
	lw	$17, D_80152020+8
	beq	$17, 0, $100400
$100397:
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
	beq	$3, 0, $100398
	lw	$25, 0($3)
	lbu	$4, 16($25)
	.livereg	0x0800000E,0x00000000
	jal	func_800A1A3C
	beq	$2, 0, $100399
	.loc	2 583
$100398:
	.loc	2 584
 # 584	            if (ghosth == D_80152698[i]) break;
	mul	$14, $16, 4
	lw	$15, D_80152698($14)
	beq	$17, $15, $100400
	.loc	2 584
	.loc	2 580
 # 580	        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
$100399:
	move	$17, $18
	bne	$18, 0, $100397
$100400:
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
	bne	$17, 0, $100402
	.loc	2 586
	.loc	2 587
 # 587	            for (p = 0; p < 3; p++) {
	move	$23, $0
	.noalias	$4,$sp
	lw	$2, 0($4)
	lw	$3, D_803BA010
$100401:
	.loc	2 587
	.loc	2 588
 # 588	                ghosth = D_803BA010 + p;
	sll	$14, $23, 2
	addu	$17, $14, $3
	.loc	2 589
 # 589	                if (ghosth == D_80152698[i]) break;
	beq	$17, $2, $100402
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
	blt	$23, 3, $100401
$100402:
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
	blt	$16, 4, $100396
	.alias	$4,$sp
	.loc	2 595
 # 595	    if (!D_803B28A8) func_803A0FD8();
	lb	$15, D_803B28A8
	bne	$15, 0, $100403
	.loc	2 595
	.livereg	0x0000000E,0x00000000
	jal	func_803A0FD8
$100403:
	.loc	2 596
 # 596	    func_8039EF08();
	.livereg	0x0000000E,0x00000000
	jal	func_8039EF08
	.loc	2 597
 # 597	    for (i = 0; i < D_8014A108; i++) {
	move	$16, $0
	lh	$5, D_8014A108
	ble	$5, 0, $100406
$100404:
	.loc	2 597
	.loc	2 598
 # 598	        if ((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
	mul	$24, $16, 76
	lw	$25, D_8014A118+72($24)
	lw	$14, 0($25)
	lw	$2, 8($14)
	beq	$2, 0, $100405
	lw	$15, 0($2)
	lbu	$4, 16($15)
	.livereg	0x0800000E,0x00000000
	jal	func_800A361C
	beq	$2, 0, $100405
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
	b	$100556
$100405:
	.loc	2 597
 # 597	    for (i = 0; i < D_8014A108; i++) {
	addu	$16, $16, 1
	sll	$24, $16, 16
	move	$16, $24
	sra	$25, $16, 16
	move	$16, $25
	lh	$5, D_8014A108
	blt	$16, $5, $100404
$100406:
	.loc	2 602
 # 598	        if ((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
 # 599	            func_8039D05C(); func_800B5570(32); D_803BA030 = 0; return;
 # 600	        }
 # 601	    }
 # 602	    if (D_80157244) { func_8039D05C(); func_800B5570(4); D_803BA030 = 0; return; }
	lb	$14, D_80157244
	beq	$14, 0, $100407
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
	b	$100556
$100407:
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
	ble	$5, 0, $100543
	li.s	$f20, 5.0E-1
	.alias	$2,$sp
$100408:
	.loc	2 604
	.loc	2 615
 # 615	        if (!D_803BA02C && (D_8014A118[p].pressed & 4)) {
	lb	$25, D_803BA02C
	bne	$25, 0, $100410
	mul	$14, $23, 76
	lw	$15, D_8014A118+4($14)
	and	$24, $15, 4
	beq	$24, 0, $100410
	.loc	2 615
	.loc	2 616
 # 616	            if (D_803BA028[p] == 1) D_803BA028[p] = 0;
	.noalias	$17,$sp
	la	$25, D_803BA028
	addu	$17, $23, $25
	lb	$14, 0($17)
	bne	$14, 1, $100409
	.loc	2 616
	sb	$0, 0($17)
	b	$100410
	.alias	$17,$sp
$100409:
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
	b	$100556
$100410:
	.loc	2 619
 # 618	        }
 # 619	        if (D_803BA028[p]) {
	.noalias	$17,$sp
	la	$15, D_803BA028
	addu	$17, $23, $15
	lb	$24, 0($17)
	beq	$24, 0, $100421
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
	bne	$15, 1, $100411
	.loc	2 621
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$100412
$100411:
	.loc	2 621
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$100412:
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
	ble	$5, 0, $100415
$100413:
	.loc	2 626
	.loc	2 627
 # 627	                if (pIdx != p && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
	beq	$23, $16, $100414
	lb	$24, D_803BA028($16)
	bne	$24, 0, $100414
	.loc	2 627
	.loc	2 627
	sw	$16, 236($sp)
	.loc	2 627
	addu	$17, $17, 1
$100414:
	.loc	2 626
 # 626	            for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	addu	$16, $16, 1
	blt	$16, $5, $100413
$100415:
	.loc	2 629
 # 627	                if (pIdx != p && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
 # 628	            }
 # 629	            if (!notReadyCount) {
	bne	$17, 0, $100416
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
	b	$100420
$100416:
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
	bne	$17, 1, $100418
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
	bgeu	$2, $3, $100417
	.alias	$17,$sp
	sll	$16, $3, 16
	sra	$15, $16, 16
	move	$16, $15
	b	$100420
$100417:
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
	b	$100420
$100418:
	.loc	2 637
 # 637	                else w = (w > object_manager_update(countdown_state.text[31], -1) ? w : object_manager_update(countdown_state.text[31], -1));
	lw	$24, countdown_state+4
	lw	$4, 124($24)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	move	$3, $16
	bgeu	$2, $3, $100419
	sll	$16, $3, 16
	sra	$15, $16, 16
	move	$16, $15
	b	$100420
$100419:
	lw	$25, countdown_state+4
	lw	$4, 124($25)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
$100420:
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
	b	$100542
$100421:
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
	beq	$25, 0, $100424
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
	bne	$14, 6, $100422
	.alias	$17,$2
	.alias	$17,$22
	.alias	$17,$sp
	lh	$24, D_801164BE
	bne	$24, 0, $100422
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
	b	$100423
$100422:
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
$100423:
	.loc	2 659
 # 658	            }
 # 659	            D_803BA030 = 0;
	sw	$0, D_803BA030
$100424:
	.loc	2 661
 # 660	        }
 # 661	        if (D_8014A118[p].repeated & 0x400) {
	lw	$2, 12($22)
	la	$14, D_803B9FD0
	addu	$21, $23, $14
	and	$24, $2, 1024
	beq	$24, 0, $100425
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
	bge	$24, 0, $100525
	.loc	2 664
	lw	$15, D_803BA050($30)
	addu	$25, $15, -1
	sw	$25, 0($3)
	.alias	$3,$22
	.alias	$3,$sp
	b	$100525
$100425:
	.loc	2 665
 # 665	        } else if (D_8014A118[p].repeated & 0x800) {
	and	$14, $2, 2048
	beq	$14, 0, $100426
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
	blt	$14, $24, $100525
	.loc	2 668
	sw	$0, 0($3)
	.alias	$3,$22
	.alias	$3,$sp
	b	$100525
$100426:
	.loc	2 669
 # 669	        } else if (D_8014A118[p].repeated & 0x3080) {
	and	$15, $2, 12416
	beq	$15, 0, $100525
	.loc	2 669
	.loc	2 670
 # 670	            s32 d = (D_8014A118[p].repeated & 0x1000) ? -1 : ((D_8014A118[p].repeated & 0x2000) ? 1 : 0);
	and	$25, $2, 4096
	beq	$25, 0, $100427
	li	$19, -1
	b	$100430
$100427:
	and	$14, $2, 8192
	beq	$14, 0, $100428
	li	$3, 1
	b	$100429
$100428:
	move	$3, $0
$100429:
	move	$19, $3
$100430:
	.loc	2 671
 # 671	            if (!D_803BA048[p] || (!d && !D_803BA04C[p])) {
	lb	$24, D_803BA048($23)
	beq	$24, 0, $100431
	bne	$19, 0, $100432
	lb	$15, D_803BA04C($23)
	bne	$15, 0, $100432
$100431:
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
	b	$100530
$100432:
	.loc	2 675
 # 674	            }
 # 675	            switch (D_803BA060[p][D_803B28B8[p]]) {
	mul	$25, $23, 76
	lw	$14, D_803B28B8($30)
	mul	$24, $14, 4
	addu	$15, $25, $24
	lw	$14, D_803BA060($15)
	bgeu	$14, 19, $100524
	sll	$14, $14, 2
	lw	$14, $100433($14)
	j	$14
	.rdata	
$100433:
	.word	$100435
	.word	$100443
	.word	$100451
	.word	$100455
	.word	$100461
	.word	$100467
	.word	$100473
	.word	$100481
	.word	$100485
	.word	$100489
	.word	$100493
	.word	$100501
	.word	$100505
	.word	$100509
	.word	$100513
	.word	$100517
	.word	$100521
	.word	$100522
	.word	$100523
	.text	
$100435:
	.loc	2 677
 # 676	            case 0:
 # 677	                if (d) {
	move	$16, $0
	beq	$19, 0, $100439
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
$100436:
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
	bge	$2, 0, $100437
	.loc	2 680
	.noalias	$18,$17
	.noalias	$18,$21
	.noalias	$18,$22
	.noalias	$18,$sp
	lh	$14, 0($18)
	addu	$25, $14, -1
	sb	$25, 0($17)
	lb	$2, 0($17)
	b	$100438
$100437:
	.loc	2 681
 # 681	                        else if (D_803B9FD4[p] >= D_803BA020[p]) D_803B9FD4[p] = 0;
	lh	$24, 0($18)
	blt	$2, $24, $100438
	.loc	2 681
	sb	$0, 0($17)
	lb	$2, 0($17)
$100438:
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
	beq	$2, 0, $100436
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
$100439:
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
	bne	$14, 6, $100440
	.loc	2 685
	li	$25, 6
	sb	$25, 66($22)
	b	$100441
$100440:
	.loc	2 686
 # 686	                else D_8014A118[p].paint = func_800CDDE8((void **)D_8014A118[p].owner);
	lw	$4, 72($22)
	.livereg	0x0800000E,0x00000000
	jal	func_800CDDE8
	sb	$2, 66($22)
$100441:
	.loc	2 687
 # 687	                for (i = 0; i < 13; i++) D_80110E78[p + 1][i] = D_8014A118[p].paint;
	mul	$24, $23, 13
	la	$15, D_80110E78
	addu	$17, $24, $15
	lbu	$2, 66($22)
$100442:
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
	blt	$16, 13, $100442
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
	b	$100524
$100443:
	.loc	2 692
 # 691	            case 1:
 # 692	                if (D_8014A110 != 6) {
	lw	$14, D_8014A110
	beq	$14, 6, $100450
	.loc	2 692
	.loc	2 693
 # 693	                    if (d) {
	beq	$19, 0, $100448
	.loc	2 693
	.loc	2 694
 # 694	                        do {
	mul	$25, $23, 13
	la	$24, D_80110E78
	addu	$17, $25, $24
$100444:
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
	bge	$6, 0, $100445
	.loc	2 696
	li	$24, 7
	sb	$24, 13($2)
	lb	$6, 13($2)
	b	$100446
$100445:
	.loc	2 697
 # 697	                            else if (D_80110E78[p + 1][D_803B9FD0[p]] >= 8) D_80110E78[p + 1][D_803B9FD0[p]] = 0;
	blt	$6, 8, $100446
	.loc	2 697
	sb	$0, 13($2)
	lb	$6, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$100446:
	move	$4, $23
	move	$5, $6
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75EC
	beq	$2, 0, $100444
	lw	$15, D_8014A110
	bne	$15, 6, $100447
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
	bne	$15, 0, $100444
$100447:
	lb	$14, 0($21)
	addu	$25, $17, $14
	.noalias	$25,$21
	.noalias	$25,$22
	.noalias	$25,$sp
	lb	$6, 13($25)
	.alias	$25,$21
	.alias	$25,$22
	.alias	$25,$sp
	b	$100449
	.alias	$17,$21
	.alias	$17,$22
	.alias	$17,$sp
$100448:
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
$100449:
	.loc	2 700
 # 700	                    D_8014A118[p].paint = D_80110E78[p + 1][D_803B9FD0[p]];
	sb	$6, 66($22)
	.loc	2 701
 # 701	                    func_800CDCAC(D_8014A118[p].owner, D_8014A118[p].paint);
	lw	$4, 72($22)
	lbu	$5, 66($22)
	.livereg	0x0C00000E,0x00000000
	jal	func_800CDCAC
	b	$100524
$100450:
	.loc	2 702
 # 702	                } else sound = 0;
	sw	$0, 256($sp)
	.loc	2 703
 # 703	                break;
	b	$100524
$100451:
	.loc	2 705
 # 704	            case 2:
 # 705	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $100453
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
	bge	$7, 0, $100452
	.loc	2 707
	li	$15, 2
	sb	$15, 13($2)
	lb	$7, 13($2)
	b	$100454
$100452:
	.loc	2 708
 # 708	                    else if (D_8011103C[p + 1][D_803B9FD0[p]] > 2) D_8011103C[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 3, $100454
	.loc	2 708
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100454
$100453:
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
$100454:
	.loc	2 710
 # 710	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 0, D_8011103C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	move	$6, $0
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 711
 # 711	                break;
	b	$100524
$100455:
	.loc	2 713
 # 712	            case 3:
 # 713	                if (d) {
	beq	$19, 0, $100459
	.loc	2 713
	.loc	2 714
 # 714	                    do {
	mul	$25, $23, 13
	la	$24, D_80111080
	addu	$16, $25, $24
$100456:
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
	bge	$5, 0, $100457
	.loc	2 716
	li	$24, 8
	sb	$24, 13($2)
	lb	$5, 13($2)
	b	$100458
$100457:
	.loc	2 717
 # 717	                        else if (D_80111080[p + 1][D_803B9FD0[p]] > 8) D_80111080[p + 1][D_803B9FD0[p]] = 0;
	blt	$5, 9, $100458
	.loc	2 717
	sb	$0, 13($2)
	lb	$5, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$100458:
	move	$4, $23
	.livereg	0x0C00000E,0x00000000
	jal	func_800F7604
	beq	$2, 0, $100456
	lb	$8, 0($21)
	lb	$15, 0($21)
	addu	$2, $16, $15
	b	$100460
	.alias	$16,$21
	.alias	$16,$22
	.alias	$16,$sp
$100459:
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
$100460:
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
	b	$100524
$100461:
	.loc	2 723
 # 722	            case 4:
 # 723	                if (d) {
	beq	$19, 0, $100465
	.loc	2 723
	.loc	2 724
 # 724	                    do {
	mul	$14, $23, 13
	la	$25, D_8011119C
	addu	$16, $14, $25
$100462:
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
	bge	$5, 0, $100463
	.loc	2 726
	li	$25, 4
	sb	$25, 13($2)
	lb	$5, 13($2)
	b	$100464
$100463:
	.loc	2 727
 # 727	                        else if (D_8011119C[p + 1][D_803B9FD0[p]] > 4) D_8011119C[p + 1][D_803B9FD0[p]] = 0;
	blt	$5, 5, $100464
	.loc	2 727
	sb	$0, 13($2)
	lb	$5, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$100464:
	move	$4, $23
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75D0
	beq	$2, 0, $100462
	lb	$8, 0($21)
	lb	$24, 0($21)
	addu	$2, $16, $24
	b	$100466
	.alias	$16,$21
	.alias	$16,$22
	.alias	$16,$sp
$100465:
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
$100466:
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
	b	$100524
$100467:
	.loc	2 733
 # 732	            case 5:
 # 733	                if (d) {
	beq	$19, 0, $100471
	.loc	2 733
	.loc	2 734
 # 734	                    do {
	mul	$15, $23, 13
	la	$14, D_80111230
	addu	$16, $15, $14
$100468:
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
	bge	$5, 0, $100469
	.loc	2 736
	li	$14, 5
	sb	$14, 13($2)
	lb	$5, 13($2)
	b	$100470
$100469:
	.loc	2 737
 # 737	                        else if (D_80111230[p + 1][D_803B9FD0[p]] > 5) D_80111230[p + 1][D_803B9FD0[p]] = 0;
	blt	$5, 6, $100470
	.loc	2 737
	sb	$0, 13($2)
	lb	$5, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$100470:
	move	$4, $23
	.livereg	0x0C00000E,0x00000000
	jal	func_800F75B0
	beq	$2, 0, $100468
	lb	$8, 0($21)
	lb	$25, 0($21)
	addu	$2, $16, $25
	b	$100472
	.alias	$16,$21
	.alias	$16,$22
	.alias	$16,$sp
$100471:
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
$100472:
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
	b	$100524
$100473:
	.loc	2 743
 # 742	            case 6:
 # 743	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $100479
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
	bge	$7, 0, $100476
	.loc	2 745
	beq	$3, 4, $100474
	li	$15, 1
	sb	$15, 13($2)
	b	$100475
$100474:
	li	$14, 2
	sb	$14, 13($2)
$100475:
	lb	$7, 13($2)
	b	$100480
$100476:
	.loc	2 746
 # 746	                    else if (D_8011128C[p + 1][D_803B9FD0[p]] > (D_8014A110 != 4 ? 1 : 2)) D_8011128C[p + 1][D_803B9FD0[p]] = 0;
	beq	$3, 4, $100477
	li	$3, 1
	b	$100478
$100477:
	li	$3, 2
$100478:
	bge	$3, $7, $100480
	.loc	2 746
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100480
$100479:
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
$100480:
	.loc	2 748
 # 748	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 4, D_8011128C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 4
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 749
 # 749	                break;
	b	$100524
$100481:
	.loc	2 751
 # 750	            case 7:
 # 751	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	mul	$24, $23, 13
	addu	$15, $24, $16
	la	$14, D_80111604
	addu	$2, $15, $14
	beq	$19, 0, $100483
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
	bge	$7, 0, $100482
	.loc	2 753
	li	$15, 31
	sb	$15, 13($2)
	lb	$7, 13($2)
	b	$100484
$100482:
	.loc	2 754
 # 754	                    else if (D_80111604[p + 1][D_803B9FD0[p]] >= 32) D_80111604[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 32, $100484
	.loc	2 754
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100484
$100483:
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
$100484:
	.loc	2 756
 # 756	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 5, D_80111604[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 5
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 757
 # 757	                break;
	b	$100524
$100485:
	.loc	2 759
 # 758	            case 8:
 # 759	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	mul	$25, $23, 13
	addu	$24, $25, $16
	la	$15, D_80111648
	addu	$2, $24, $15
	beq	$19, 0, $100487
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
	bge	$7, 0, $100486
	.loc	2 761
	li	$24, 31
	sb	$24, 13($2)
	lb	$7, 13($2)
	b	$100488
$100486:
	.loc	2 762
 # 762	                    else if (D_80111648[p + 1][D_803B9FD0[p]] >= 32) D_80111648[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 32, $100488
	.loc	2 762
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100488
$100487:
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
$100488:
	.loc	2 764
 # 764	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 6, D_80111648[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 6
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 765
 # 765	                break;
	b	$100524
$100489:
	.loc	2 767
 # 766	            case 9:
 # 767	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	mul	$14, $23, 13
	addu	$25, $14, $16
	la	$24, D_8011168C
	addu	$2, $25, $24
	beq	$19, 0, $100491
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
	bge	$7, 0, $100490
	.loc	2 769
	li	$25, 31
	sb	$25, 13($2)
	lb	$7, 13($2)
	b	$100492
$100490:
	.loc	2 770
 # 770	                    else if (D_8011168C[p + 1][D_803B9FD0[p]] >= 32) D_8011168C[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 32, $100492
	.loc	2 770
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100492
$100491:
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
$100492:
	.loc	2 772
 # 772	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 7, D_8011168C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 7
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 773
 # 773	                break;
	b	$100524
$100493:
	.loc	2 775
 # 774	            case 10:
 # 775	                if (d) {
	la	$15, D_8012E67C
	addu	$2, $23, $15
	beq	$19, 0, $100500
	.loc	2 775
	.loc	2 776
 # 776	                    s32 teamFull = 1;
	li	$3, 1
	.loc	2 777
 # 777	                    do {
	lh	$5, D_8014A108
$100494:
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
	bge	$4, 0, $100495
	.loc	2 779
	li	$24, 3
	sb	$24, 0($2)
	b	$100496
$100495:
	.loc	2 780
 # 780	                        else if (D_8012E67C[p] > 3) D_8012E67C[p] = 0;
	blt	$4, 4, $100496
	.loc	2 780
	sb	$0, 0($2)
$100496:
	.loc	2 781
 # 781	                        for (i = 0; i < D_8014A108; i++) {
	ble	$5, 0, $100499
$100497:
	.loc	2 781
	.loc	2 782
 # 782	                            if (i != p && D_8012E67C[i] != D_8012E67C[p]) teamFull = 0;
	beq	$16, $23, $100498
	lb	$15, D_8012E67C($16)
	lb	$14, 0($2)
	beq	$15, $14, $100498
	.loc	2 782
	move	$3, $0
$100498:
	.loc	2 781
 # 781	                        for (i = 0; i < D_8014A108; i++) {
	addu	$16, $16, 1
	sll	$25, $16, 16
	move	$16, $25
	sra	$24, $16, 16
	move	$16, $24
	blt	$16, $5, $100497
$100499:
	beq	$3, 1, $100494
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100524
$100500:
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
	b	$100524
$100501:
	.loc	2 788
 # 787	            case 11:
 # 788	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $100503
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
	bge	$7, 0, $100502
	.loc	2 790
	li	$14, 20
	sb	$14, 13($2)
	lb	$7, 13($2)
	b	$100504
$100502:
	.loc	2 791
 # 791	                    else if (D_8011157C[p + 1][D_803B9FD0[p]] > 20) D_8011157C[p + 1][D_803B9FD0[p]] = 0;
	blt	$7, 21, $100504
	.loc	2 791
	sb	$0, 13($2)
	lb	$7, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100504
$100503:
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
$100504:
	.loc	2 793
 # 793	                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 8, D_8011157C[p + 1][D_803B9FD0[p]]);
	lw	$4, 72($22)
	and	$5, $8, 255
	li	$6, 8
	.livereg	0x0F00000E,0x00000000
	jal	func_800C7578
	.loc	2 794
 # 794	                break;
	b	$100524
$100505:
	.loc	2 796
 # 795	            case 12:
 # 796	                if (d) {
	lb	$16, 0($21)
	sw	$0, 256($sp)
	beq	$19, 0, $100507
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
	bge	$3, 0, $100506
	.loc	2 798
	li	$24, 9
	sb	$24, 13($2)
	b	$100508
$100506:
	.loc	2 799
 # 799	                    else if (D_801115C0[p + 1][D_803B9FD0[p]] > 9) D_801115C0[p + 1][D_803B9FD0[p]] = 0;
	blt	$3, 10, $100508
	.loc	2 799
	sb	$0, 13($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100508
$100507:
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
$100508:
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
	b	$100524
$100509:
	.loc	2 807
 # 806	            case 13:
 # 807	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $100511
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
	bc1f	$100510
	.loc	2 809
	li.s	$f4, 1.5
	s.s	$f4, 52($2)
	l.s	$f0, 52($2)
	b	$100512
$100510:
	.loc	2 810
 # 810	                    else if (D_801112DC[p + 1][D_803B9FD0[p]] > 1.6f) D_801112DC[p + 1][D_803B9FD0[p]] = 0.75f;
	li.s	$f10, 1.6
	c.lt.s	$f10, $f0
	bc1f	$100512
	.loc	2 810
	li.s	$f6, 0.75
	s.s	$f6, 52($2)
	l.s	$f0, 52($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100512
$100511:
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
$100512:
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
	b	$100524
$100513:
	.loc	2 815
 # 814	            case 14:
 # 815	                if (d) {
	lb	$16, 0($21)
	move	$8, $16
	beq	$19, 0, $100515
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
	bc1f	$100514
	.loc	2 817
	li.s	$f10, 1.5
	s.s	$f10, 52($2)
	l.s	$f0, 52($2)
	b	$100516
$100514:
	.loc	2 818
 # 818	                    else if (D_801113E0[p + 1][D_803B9FD0[p]] > 1.5f) D_801113E0[p + 1][D_803B9FD0[p]] = 0.75f;
	li.s	$f4, 1.5
	c.lt.s	$f4, $f0
	bc1f	$100516
	.loc	2 818
	li.s	$f6, 0.75
	s.s	$f6, 52($2)
	l.s	$f0, 52($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100516
$100515:
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
$100516:
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
	b	$100524
$100517:
	.loc	2 823
 # 822	            case 15:
 # 823	                if (d) {
	lb	$16, 0($21)
	beq	$19, 0, $100519
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
	bge	$6, 0, $100518
	.loc	2 825
	li	$24, 10
	sh	$24, 0($2)
	lh	$6, 0($2)
	b	$100520
$100518:
	.loc	2 826
 # 826	                    else if (D_803B28B0[p] > 10) D_803B28B0[p] = 0;
	blt	$6, 11, $100520
	.loc	2 826
	sh	$0, 0($2)
	lh	$6, 0($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
	b	$100520
$100519:
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
$100520:
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
	b	$100524
$100521:
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
	b	$100524
$100522:
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
	b	$100524
$100523:
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
$100524:
	.loc	2 834
 # 833	            }
 # 834	            if (sound) audio_doppler(D_8014A118[p].repeated);
	lw	$24, 256($sp)
	beq	$24, 0, $100525
	.loc	2 834
	lw	$4, 12($22)
	.livereg	0x0800000E,0x00000000
	jal	audio_doppler
$100525:
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
	bge	$4, $3, $100526
	.loc	2 836
	sw	$4, 0($2)
	lw	$3, 0($2)
$100526:
	.loc	2 837
 # 837	        if (D_803BA038[p] < D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
	bge	$3, $4, $100527
	.loc	2 837
	sw	$4, 0($2)
	lw	$3, 0($2)
$100527:
	.loc	2 838
 # 838	        if (D_803BA038[p] > D_803BA050[p] - 3) D_803BA038[p] = D_803BA050[p] - 3;
	addu	$4, $5, -3
	bge	$4, $3, $100528
	.loc	2 838
	sw	$4, 0($2)
	lw	$3, 0($2)
$100528:
	.loc	2 839
 # 839	        if (D_803BA038[p] < 1) D_803BA038[p] = 1;
	bgt	$3, 0, $100529
	.loc	2 839
	li	$25, 1
	sw	$25, 0($2)
	.alias	$2,$21
	.alias	$2,$22
	.alias	$2,$sp
$100529:
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
$100530:
	.loc	2 842
 # 842	        if (D_803BA018[p] != D_803B9FD0[p]) {
	.noalias	$17,$21
	.noalias	$17,$22
	.noalias	$17,$sp
	la	$15, D_803BA018
	addu	$17, $6, $15
	lh	$2, 0($17)
	beq	$2, $16, $100532
	.loc	2 842
	.loc	2 843
 # 843	            if (D_803BA018[p] >= 0) func_800CCA04(D_8014A118[p].owner, D_803B9FD0[p]);
	blt	$2, 0, $100531
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
$100531:
	.loc	2 844
 # 844	            D_803BA018[p] = D_803B9FD0[p];
	sh	$16, 0($17)
	.alias	$17,$sp
$100532:
	.loc	2 846
 # 845	        }
 # 846	        if (D_8014A110 == 2) {
	lw	$14, D_8014A110
	bne	$14, 2, $100541
	.loc	2 846
	.loc	2 847
 # 847	            for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$19, 1
	lb	$2, D_803BA00C
	addu	$2, $2, 1
	blt	$2, 2, $100541
	li	$22, 4
	la	$20, D_803B9FC0+4
$100533:
	.loc	2 847
	.loc	2 848
 # 848	                if (D_803B9FC0[ghostIdx]) {
	.noalias	$20,$sp
	lw	$25, 0($20)
	beq	$25, 0, $100540
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
	bne	$3, 0, $100536
	.alias	$16,$20
	.alias	$16,$sp
	.loc	2 857
	.loc	2 858
 # 858	                        if (!D_803AF980) gw = object_manager_update(countdown_state.text[225], -1);
	lw	$24, D_803AF980
	bne	$24, 0, $100534
	.loc	2 858
	lw	$15, countdown_state+4
	lw	$4, 900($15)
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$14, $16, 16
	move	$16, $14
	b	$100535
$100534:
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
$100535:
	.loc	2 860
 # 860	                        gh = object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	sll	$17, $2, 16
	sra	$15, $17, 16
	move	$17, $15
	b	$100539
$100536:
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
	bgeu	$2, $3, $100537
	sll	$16, $3, 16
	sra	$25, $16, 16
	move	$16, $25
	b	$100538
$100537:
	la	$4, D_803B85A0
	li	$5, -1
	.livereg	0x0C00000E,0x00000000
	jal	object_manager_update
	sll	$16, $2, 16
	sra	$24, $16, 16
	move	$16, $24
$100538:
	.loc	2 864
 # 864	                        gh = object_bytes_sum_global() * 2;
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	mul	$17, $2, 2
	sll	$15, $17, 16
	move	$17, $15
	sra	$14, $17, 16
	move	$17, $14
$100539:
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
$100540:
	.loc	2 847
 # 847	            for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	addu	$19, $19, 1
	addu	$22, $22, 4
	addu	$20, $20, 4
	blt	$19, $2, $100533
	.alias	$20,$sp
$100541:
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
$100542:
	addu	$23, $23, 1
	sll	$15, $23, 16
	move	$23, $15
	sra	$14, $23, 16
	move	$23, $14
	lh	$5, D_8014A108
	blt	$23, $5, $100408
	la	$2, D_803BA030
$100543:
	.loc	2 874
 # 874	    D_803BA02C = 1;
	li	$25, 1
	sb	$25, D_803BA02C
	.loc	2 875
 # 875	    for (i = 0; i < D_8014A108; i++) if (!D_803BA028[i]) D_803BA02C = 0;
	move	$16, $0
	ble	$5, 0, $100546
$100544:
	.loc	2 875
	lb	$24, D_803BA028($16)
	bne	$24, 0, $100545
	.loc	2 875
	sb	$0, D_803BA02C
$100545:
	.loc	2 875
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, $5, $100544
$100546:
	.loc	2 876
 # 876	    if (D_803BA02C && D_803BA030 >= 10) {
	lb	$25, D_803BA02C
	beq	$25, 0, $100556
	.noalias	$2,$sp
	lw	$24, 0($2)
	blt	$24, 10, $100556
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
	bne	$3, 3, $100547
	.loc	2 880
	.livereg	0x0000000E,0x00000000
	jal	net_session_update
	lw	$3, D_8014A110
$100547:
	.loc	2 881
 # 881	        if (D_8014A110 == 2) {
	bne	$3, 2, $100555
	.loc	2 881
	.loc	2 882
 # 882	            {
	.loc	2 884
 # 883	                Record **gh;
 # 884	                for (gh = D_80152020.first; gh; gh = (*gh)->next) menu_transition((void **)gh);
	lw	$16, D_80152020+8
	beq	$16, 0, $100549
$100548:
	.loc	2 884
	move	$4, $16
	.livereg	0x0800000E,0x00000000
	jal	menu_transition
	.loc	2 884
	lw	$15, 0($16)
	lw	$16, 0($15)
	bne	$16, 0, $100548
$100549:
	.loc	2 886
 # 885	            }
 # 886	            for (i = 1; i < D_803BA00C; i++) {
	li	$16, 1
	lb	$2, D_803BA00C
	ble	$2, 1, $100552
	la	$3, D_80152698
$100550:
	.loc	2 886
	.loc	2 887
 # 887	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
	.noalias	$3,$sp
	.noalias	$4,$sp
	mul	$14, $16, 4
	addu	$4, $3, $14
	lw	$25, 0($4)
	bne	$25, 0, $100551
	.loc	2 887
	.loc	2 887
	lw	$24, 4($4)
	sw	$24, 0($4)
	.loc	2 887
	sw	$0, 4($4)
	.alias	$4,$sp
$100551:
	.loc	2 886
 # 886	            for (i = 1; i < D_803BA00C; i++) {
	addu	$16, $16, 1
	sll	$15, $16, 16
	move	$16, $15
	sra	$14, $16, 16
	move	$16, $14
	blt	$16, $2, $100550
	li	$16, 1
$100552:
	la	$3, D_80152698
	.loc	2 889
 # 887	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
 # 888	            }
 # 889	            for (i = 1; i < D_803BA00C; i++) {
	ble	$2, 1, $100555
$100553:
	.loc	2 889
	.loc	2 890
 # 890	                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
	.noalias	$4,$sp
	mul	$25, $16, 4
	addu	$4, $3, $25
	lw	$24, 0($4)
	bne	$24, 0, $100554
	.loc	2 890
	.loc	2 890
	lw	$15, 4($4)
	sw	$15, 0($4)
	.loc	2 890
	sw	$0, 4($4)
	.alias	$4,$sp
$100554:
	.loc	2 889
 # 889	            for (i = 1; i < D_803BA00C; i++) {
	addu	$16, $16, 1
	sll	$14, $16, 16
	move	$16, $14
	sra	$25, $16, 16
	move	$16, $25
	blt	$16, $2, $100553
	.alias	$3,$sp
$100555:
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
$100556:
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

	.extern	D_8014A108 2
	.extern	D_803B97A0 4
	.extern	D_803B97A4 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A3A6C
	.loc	2 41
 #  41	{
	.ent	func_803A3A6C 2
func_803A3A6C:
	.option	O3
	subu	$sp, 72
	sw	$31, 28($sp)
	.mask	0x80000000, -44
	.frame	$sp, 72, $31
	.loc	2 41
	move	$9, $4
	.loc	2 41
	.loc	2 42
 #  42	    s32 player=blt->AnimID&15;
	lw	$2, 44($9)
	and	$8, $2, 15
	.loc	2 43
 #  43	    s32 marker=(blt->AnimID>>16)&255;
	srl	$11, $2, 16
	and	$14, $11, 255
	move	$11, $14
	.loc	2 49
 #  44	    MenuSlot *slot;
 #  45	    s32 palette;
 #  46	    s16 position[2];
 #  47	    s32 channel;
 #  48	
 #  49	    if (player>=D_8014A108) {
	lh	$15, D_8014A108
	blt	$8, $15, $200032
	.loc	2 49
	.loc	2 50
 #  50	        input_new_data_wrapper(blt,1);
	move	$4, $9
	li	$5, 1
	sw	$9, 72($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$9, 72($sp)
	.loc	2 51
 #  51	        blt->AnimFunc=0;
	sw	$0, 40($9)
	.loc	2 52
 #  52	        return 1;
	li	$2, 1
	b	$200040
$200032:
	.loc	2 54
 #  53	    }
 #  54	    if (D_803BA028[player]) {
	lb	$24, D_803BA028($8)
	beq	$24, 0, $200033
	.loc	2 54
	.loc	2 55
 #  55	        input_new_data_wrapper(blt,1);
	move	$4, $9
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	.loc	2 56
 #  56	        return 1;
	li	$2, 1
	b	$200040
$200033:
	.loc	2 58
 #  57	    }
 #  58	    slot=&D_803AF9A8[player][marker];
	.loc	2 59
 #  59	    if (input_new_data_wrapper(blt,
	mul	$10, $11, 64
	.noalias	$3,$sp
	mul	$25, $8, 2816
	addu	$12, $25, $10
	la	$13, D_803AF9A8
	addu	$3, $12, $13
	l.s	$f0, 8($3)
	l.s	$f4, D_803B97A0
	c.lt.s	$f4, $f0
	move	$5, $0
	bc1f	$200041
	li	$5, 1
$200041:
	beq	$5, 0, $200034
	l.s	$f6, D_803B97A4
	c.lt.s	$f0, $f6
	move	$5, $0
	bc1f	$200042
	li	$5, 1
$200042:
	bne	$5, 0, $200035
$200034:
	lbu	$5, 60($3)
	seq	$14, $5, 0
	move	$5, $14
$200035:
	move	$4, $9
	sw	$3, 36($sp)
	sw	$8, 68($sp)
	sw	$9, 72($sp)
	sw	$10, 40($sp)
	sw	$11, 64($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$3, 36($sp)
	lw	$8, 68($sp)
	lw	$9, 72($sp)
	lw	$10, 40($sp)
	lw	$11, 64($sp)
	beq	$2, 0, $200036
	.alias	$3,$sp
	.loc	2 61
 #  60	        (D_803B97A0<slot->state.depth && slot->state.depth<D_803B97A4) || !slot->state.alpha))
 #  61	        return 1;
	li	$2, 1
	b	$200040
$200036:
	.loc	2 62
 #  62	    brake_light_update(player,&slot[19].matrix[12],&D_80150B70[player],0,position);
	move	$4, $8
	addu	$5, $3, 1264
	mul	$15, $8, 152
	la	$24, D_80150B70
	addu	$6, $15, $24
	move	$7, $0
	addu	$25, $sp, 52
	sw	$25, 16($sp)
	sw	$3, 36($sp)
	sw	$8, 68($sp)
	sw	$9, 72($sp)
	sw	$10, 40($sp)
	sw	$11, 64($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	lw	$3, 36($sp)
	lw	$8, 68($sp)
	lw	$9, 72($sp)
	lw	$10, 40($sp)
	lw	$11, 64($sp)
	.loc	2 63
 #  63	    blt->X=position[0]-blt->Width/2;
	lh	$12, 52($sp)
	lh	$13, 20($9)
	div	$14, $13, 2
	subu	$15, $12, $14
	sh	$15, 14($9)
	.loc	2 64
 #  64	    blt->Y=position[1]-blt->Height/2;
	lh	$24, 54($sp)
	lh	$25, 22($9)
	div	$13, $25, 2
	subu	$12, $24, $13
	sh	$12, 16($9)
	.loc	2 65
 #  65	    blt->Alpha=slot->state.alpha;
	lbu	$14, 60($3)
	sb	$14, 24($9)
	.loc	2 66
 #  66	    if (marker==7) {
	bne	$11, 7, $200037
	.loc	2 66
	.loc	2 67
 #  67	        palette=D_80111611[player][D_803B9FD0[player]];
	mul	$15, $8, 13
	lb	$25, D_803B9FD0($8)
	addu	$24, $15, $25
	lb	$5, D_80111611($24)
	.loc	2 68
 #  68	        channel=0;
	sw	$0, 48($sp)
	sw	$5, 56($sp)
	b	$200039
$200037:
	.loc	2 69
 #  69	    } else if (marker==8) {
	bne	$10, 512, $200038
	.loc	2 69
	.loc	2 70
 #  70	        palette=D_80111655[player][D_803B9FD0[player]];
	mul	$13, $8, 13
	lb	$12, D_803B9FD0($8)
	addu	$14, $13, $12
	lb	$5, D_80111655($14)
	.loc	2 71
 #  71	        channel=1;
	li	$15, 1
	sw	$15, 48($sp)
	sw	$5, 56($sp)
	b	$200039
$200038:
	.loc	2 72
 #  72	    } else if (marker==9) {
	bne	$10, 576, $200039
	.loc	2 72
	.loc	2 73
 #  73	        palette=D_80111699[player][D_803B9FD0[player]];
	mul	$25, $8, 13
	lb	$24, D_803B9FD0($8)
	addu	$13, $25, $24
	lb	$5, D_80111699($13)
	.loc	2 74
 #  74	        channel=2;
	li	$12, 2
	sw	$12, 48($sp)
	sw	$5, 56($sp)
$200039:
	lw	$5, 56($sp)
	.loc	2 76
 #  75	    }
 #  76	    D_803B9B60[player][channel].r=D_801226C0[palette].r;
	.noalias	$2,$sp
	mul	$14, $8, 12
	lw	$15, 48($sp)
	mul	$25, $15, 4
	addu	$24, $14, $25
	la	$13, D_803B9B60
	addu	$2, $24, $13
	.noalias	$3,$2
	.noalias	$3,$sp
	mul	$12, $5, 4
	la	$15, D_801226C0
	addu	$3, $12, $15
	lbu	$14, 0($3)
	sb	$14, 0($2)
	.loc	2 77
 #  77	    D_803B9B60[player][channel].g=D_801226C0[palette].g;
	lbu	$25, 1($3)
	sb	$25, 1($2)
	.loc	2 78
 #  78	    D_803B9B60[player][channel].b=D_801226C0[palette].b;
	lbu	$24, 2($3)
	sb	$24, 2($2)
	.loc	2 79
 #  79	    D_803B9B60[player][channel].a=blt->Alpha;
	lbu	$13, 24($9)
	sb	$13, 3($2)
	.loc	2 80
 #  80	    blt->Image=&D_803B9B60[player][channel];
	sw	$2, 4($9)
	.loc	2 81
 #  81	    blt->AnimID=(blt->AnimID&0x0fffffff)|((u32)palette<<28);
	lw	$12, 44($9)
	and	$15, $12, 268435455
	sll	$14, $5, 28
	or	$25, $15, $14
	sw	$25, 44($9)
	.loc	2 82
 #  82	    Input_ApplyPadConfig(blt);
	move	$4, $9
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.alias	$2,$3
	.alias	$2,$sp
	.alias	$3,$sp
	.loc	2 83
 #  83	    return 1;
	li	$2, 1
$200040:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 72
	j	$31
	.end	func_803A3A6C

	.extern	D_8014A108 2
	.extern	D_803B97A8 4
	.extern	D_803B97AC 4
	.extern	D_803B97B0 4
	.extern	D_803B97B4 4
	.extern	D_803B97B8 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A3DA4
	.loc	2 40
 #  40	{
	.ent	func_803A3DA4 2
func_803A3DA4:
	.option	O3
	subu	$sp, 64
	sw	$31, 28($sp)
	.mask	0x80000000, -36
	.frame	$sp, 64, $31
	.loc	2 40
	move	$8, $4
	.loc	2 40
	.loc	2 41
 #  41	    s32 type=blt->AnimID&15;
	lw	$2, 44($8)
	and	$14, $2, 15
	sw	$14, 60($sp)
	.loc	2 42
 #  42	    s32 p=(blt->AnimID>>4)&15;
	srl	$9, $2, 4
	and	$15, $9, 15
	move	$9, $15
	.loc	2 43
 #  43	    s32 opt=blt->AnimID>>16;
	srl	$11, $2, 16
	.loc	2 45
 #  44	    s16 screenPos[2];
 #  45	    if (p>=D_8014A108) {
	lh	$24, D_8014A108
	blt	$9, $24, $300032
	.loc	2 45
	.loc	2 46
 #  46	        input_new_data_wrapper(blt,1);
	move	$4, $8
	li	$5, 1
	sw	$8, 64($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$8, 64($sp)
	.loc	2 47
 #  47	        blt->AnimFunc=0;
	sw	$0, 40($8)
	.loc	2 48
 #  48	        return 1;
	li	$2, 1
	b	$300041
$300032:
	.loc	2 50
 #  49	    }
 #  50	    if (D_803BA028[p]) {
	lb	$25, D_803BA028($9)
	beq	$25, 0, $300033
	.loc	2 50
	.loc	2 51
 #  51	        input_new_data_wrapper(blt,1);
	move	$4, $8
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	.loc	2 52
 #  52	        return 1;
	li	$2, 1
	b	$300041
$300033:
	.loc	2 54
 #  53	    }
 #  54	    if (input_new_data_wrapper(blt,
	.noalias	$10,$sp
	mul	$12, $9, 2816
	mul	$13, $11, 64
	addu	$14, $12, $13
	la	$15, D_803AF9A8
	addu	$10, $14, $15
	l.s	$f0, 8($10)
	l.s	$f4, D_803B97A8
	c.lt.s	$f4, $f0
	move	$5, $0
	bc1f	$300042
	li	$5, 1
$300042:
	beq	$5, 0, $300034
	l.s	$f6, D_803B97AC
	c.lt.s	$f0, $f6
	move	$5, $0
	bc1f	$300043
	li	$5, 1
$300043:
	bne	$5, 0, $300035
$300034:
	lbu	$5, 60($10)
	seq	$24, $5, 0
	move	$5, $24
$300035:
	move	$4, $8
	sw	$8, 64($sp)
	sw	$9, 56($sp)
	sw	$10, 40($sp)
	sw	$11, 52($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$8, 64($sp)
	lw	$9, 56($sp)
	lw	$10, 40($sp)
	lw	$11, 52($sp)
	beq	$2, 0, $300036
	.alias	$10,$sp
	.loc	2 56
 #  55	        (D_803B97A8<D_803AF9A8[p][opt].state.depth && D_803AF9A8[p][opt].state.depth<D_803B97AC) || !D_803AF9A8[p][opt].state.alpha))
 #  56	        return 1;
	li	$2, 1
	b	$300041
$300036:
	.loc	2 57
 #  57	    brake_light_update(p,&D_803AF9A8[p][opt+19].matrix[12],&D_80150B70[p],0,screenPos);
	move	$4, $9
	addu	$5, $10, 1264
	mul	$25, $9, 152
	la	$12, D_80150B70
	addu	$6, $25, $12
	move	$7, $0
	addu	$13, $sp, 48
	sw	$13, 16($sp)
	sw	$8, 64($sp)
	sw	$9, 56($sp)
	sw	$10, 40($sp)
	sw	$11, 52($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	lw	$8, 64($sp)
	lw	$9, 56($sp)
	lw	$10, 40($sp)
	lw	$11, 52($sp)
	.loc	2 58
 #  58	    blt->X=screenPos[0]-blt->Width/2;
	lh	$2, 20($8)
	lh	$14, 48($sp)
	div	$15, $2, 2
	subu	$24, $14, $15
	sh	$24, 14($8)
	.loc	2 59
 #  59	    blt->Y=screenPos[1]-(blt->Height/4)/2;
	lh	$4, 22($8)
	div	$3, $4, 4
	lh	$25, 50($sp)
	div	$12, $3, 2
	subu	$13, $25, $12
	sh	$13, 16($8)
	.loc	2 60
 #  60	    blt->Left=0;
	sh	$0, 32($8)
	.loc	2 61
 #  61	    blt->Right=blt->Width-1;
	addu	$14, $2, -1
	sh	$14, 34($8)
	.loc	2 62
 #  62	    blt->Top=0;
	sh	$0, 28($8)
	.loc	2 63
 #  63	    blt->Bot=(blt->Height/4)-1;
	addu	$15, $3, -1
	sh	$15, 30($8)
	.loc	2 64
 #  64	    blt->Alpha=D_803AF9A8[p][opt].state.alpha;
	.noalias	$10,$sp
	lbu	$24, 60($10)
	sb	$24, 24($8)
	.loc	2 65
 #  65	    if (D_803B97B0 < D_803AF9A8[p][opt].state.depth) {
	l.s	$f8, D_803B97B0
	l.s	$f10, 8($10)
	c.lt.s	$f8, $f10
	bc1f	$300037
	.alias	$10,$sp
	.loc	2 65
	.loc	2 66
 #  66	        blt->Top += blt->Height/2;
	div	$2, $4, 2
	lh	$25, 28($8)
	addu	$12, $25, $2
	sh	$12, 28($8)
	.loc	2 67
 #  67	        blt->Bot += blt->Height/2;
	lh	$13, 30($8)
	addu	$14, $13, $2
	sh	$14, 30($8)
$300037:
	.loc	2 69
 #  68	    }
 #  69	    if (type==1) {
	lw	$15, 60($sp)
	bne	$15, 1, $300040
	.loc	2 69
	.loc	2 70
 #  70	        blt->Top+=(blt->Height/4);
	lh	$24, 28($8)
	addu	$25, $24, $3
	sh	$25, 28($8)
	.loc	2 71
 #  71	        blt->Bot+=(blt->Height/4);
	lh	$12, 30($8)
	addu	$13, $12, $3
	sh	$13, 30($8)
	.loc	2 72
 #  72	        switch (opt) {
	beq	$11, 13, $300038
	beq	$11, 14, $300039
	b	$300040
$300038:
	.loc	2 74
 #  73	        case 13:
 #  74	            blt->Right=(D_80111310[p][D_803B9FD0[p]]-.75f)/D_803B97B4*(blt->Width-1)/15.0f;
	mul	$14, $9, 52
	lb	$15, D_803B9FD0($9)
	mul	$24, $15, 4
	addu	$25, $14, $24
	l.s	$f16, D_80111310($25)
	li.s	$f18, .75
	sub.s	$f4, $f16, $f18
	l.s	$f6, D_803B97B4
	div.s	$f8, $f4, $f6
	lh	$12, 20($8)
	addu	$13, $12, -1
	mtc1	$13, $f10
	cvt.s.w	$f16, $f10
	mul.s	$f18, $f8, $f16
	li.s	$f4, 15.0
	div.s	$f6, $f18, $f4
	trunc.w.s	$f10, $f6, $15
	mfc1	$14, $f10
	sh	$14, 34($8)
	.loc	2 75
 #  75	            break;
	b	$300040
$300039:
	.loc	2 77
 #  76	        case 14:
 #  77	            blt->Right=(D_80111414[p][D_803B9FD0[p]]-.75f)/D_803B97B8*(blt->Width-1)/15.0f;
	mul	$24, $9, 52
	lb	$25, D_803B9FD0($9)
	mul	$12, $25, 4
	addu	$13, $24, $12
	l.s	$f8, D_80111414($13)
	li.s	$f16, .75
	sub.s	$f18, $f8, $f16
	l.s	$f4, D_803B97B8
	div.s	$f6, $f18, $f4
	lh	$15, 20($8)
	addu	$14, $15, -1
	mtc1	$14, $f10
	cvt.s.w	$f8, $f10
	mul.s	$f16, $f6, $f8
	li.s	$f18, 15.0
	div.s	$f4, $f16, $f18
	trunc.w.s	$f10, $f4, $25
	mfc1	$24, $f10
	sh	$24, 34($8)
	.loc	2 78
 #  78	            break;
$300040:
	.loc	2 81
 #  79	        }
 #  80	    }
 #  81	    Input_ApplyPadConfig(blt);
	move	$4, $8
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.loc	2 82
 #  82	    return 1;
	li	$2, 1
$300041:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 28($sp)
	addu	$sp, 64
	j	$31
	.end	func_803A3DA4

	.extern	D_8014A108 2
	.extern	D_8014A110 4
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A4134
	.loc	2 47
 #  47	{
	.ent	func_803A4134 2
func_803A4134:
	.option	O3
	subu	$sp, 40
	sw	$31, 20($sp)
	.mask	0x80000000, -20
	.frame	$sp, 40, $31
	.loc	2 47
	.loc	2 47
	.loc	2 48
 #  48	    s32 segment = blt->AnimID & 15;
	lw	$2, 44($4)
	and	$8, $2, 15
	.loc	2 49
 #  49	    s32 bar = (blt->AnimID >> 4) & 15;
	srl	$9, $2, 4
	and	$14, $9, 15
	move	$9, $14
	.loc	2 50
 #  50	    s32 player = (blt->AnimID >> 8) & 15;
	srl	$7, $2, 8
	and	$15, $7, 15
	move	$7, $15
	.loc	2 52
 #  51	    s32 height;
 #  52	    s32 initialized = blt->AnimID >> 31;
	srl	$3, $2, 31
	.loc	2 53
 #  53	    if (!initialized) {
	bne	$3, 0, $400035
	.loc	2 53
	.loc	2 54
 #  54	        if (D_8014A108 >= 3 && player < 2)
	lh	$24, D_8014A108
	blt	$24, 3, $400032
	bge	$7, 2, $400032
	.loc	2 55
 #  55	            func_800EF5B0(blt, D_803B85D8, 0);
	la	$5, D_803B85D8
	move	$6, $0
	sw	$4, 40($sp)
	sw	$7, 28($sp)
	sw	$8, 36($sp)
	sw	$9, 32($sp)
	.livereg	0x0E00000E,0x00000000
	jal	func_800EF5B0
	lw	$4, 40($sp)
	lw	$7, 28($sp)
	lw	$8, 36($sp)
	lw	$9, 32($sp)
	lw	$2, 44($4)
$400032:
	.loc	2 56
 #  56	        blt->AnimID |= 0x80000000;
	or	$25, $2, -2147483648
	sw	$25, 44($4)
	.loc	2 57
 #  57	        if (player < D_8014A108 && D_8014A108 == 1 && D_8014A110 != 2) {
	lh	$3, D_8014A108
	bge	$7, $3, $400033
	bne	$3, 1, $400033
	lw	$10, D_8014A110
	beq	$10, 2, $400033
	.loc	2 57
	.loc	2 58
 #  58	            blt->X = 10;
	li	$11, 10
	sh	$11, 14($4)
	.loc	2 59
 #  59	            height = blt->Height / 8;
	lh	$2, 22($4)
	div	$12, $2, 8
	move	$2, $12
	.loc	2 60
 #  60	            blt->Y = 48 + bar * 2 * height;
	mul	$3, $9, 2
	mul	$13, $3, $2
	addu	$14, $13, 48
	sh	$14, 16($4)
	b	$400034
$400033:
	.loc	2 61
 #  61	        } else {
	.loc	2 62
 #  62	            input_new_data_wrapper(blt, 1);
	li	$5, 1
	sw	$4, 40($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$4, 40($sp)
	.loc	2 63
 #  63	            blt->AnimFunc = 0;
	sw	$0, 40($4)
	.loc	2 64
 #  64	            return 1;
	li	$2, 1
	b	$400040
$400034:
	.loc	2 66
 #  65	        }
 #  66	        blt->Left = 0;
	sh	$0, 32($4)
	.loc	2 67
 #  67	        blt->Top = (bar * 2 + (segment == 0)) * height;
	seq	$15, $8, 0
	addu	$24, $3, $15
	mul	$25, $24, $2
	sh	$25, 28($4)
	.loc	2 68
 #  68	        blt->Bot = blt->Top + height - 1;
	lh	$10, 28($4)
	addu	$11, $10, $2
	addu	$12, $11, -1
	sh	$12, 30($4)
$400035:
	.loc	2 70
 #  69	    }
 #  70	    if (input_new_data_wrapper(blt, D_803BA028[player] == 1))
	lb	$5, D_803BA028($7)
	seq	$13, $5, 1
	move	$5, $13
	sw	$4, 40($sp)
	sw	$7, 28($sp)
	sw	$8, 36($sp)
	sw	$9, 32($sp)
	.livereg	0x0C00000E,0x00000000
	jal	input_new_data_wrapper
	lw	$4, 40($sp)
	lw	$7, 28($sp)
	lw	$8, 36($sp)
	lw	$9, 32($sp)
	beq	$2, 0, $400036
	.loc	2 71
 #  71	        return 1;
	li	$2, 1
	b	$400040
$400036:
	.loc	2 72
 #  72	    if (segment == 1) {
	bne	$8, 1, $400038
	.loc	2 72
	.loc	2 73
 #  73	        blt->Right = D_803BA190[player][bar] * blt->Width - 1;
	mul	$14, $7, 16
	mul	$15, $9, 4
	addu	$24, $14, $15
	l.s	$f4, D_803BA190($24)
	lh	$25, 20($4)
	mtc1	$25, $f6
	cvt.s.w	$f8, $f6
	mul.s	$f10, $f4, $f8
	li.s	$f16, 1.0000000000000000e+00
	sub.s	$f18, $f10, $f16
	trunc.w.s	$f6, $f18, $10
	mfc1	$11, $f6
	sh	$11, 34($4)
	.loc	2 74
 #  74	        blt->Right = blt->Right < 2 ? 2 : blt->Right;
	lh	$2, 34($4)
	bge	$2, 2, $400037
	li	$12, 2
	sh	$12, 34($4)
	b	$400039
$400037:
	sh	$2, 34($4)
	b	$400039
$400038:
	.loc	2 76
 #  75	    } else
 #  76	        blt->Right = blt->Width - 1;
	lh	$13, 20($4)
	addu	$14, $13, -1
	sh	$14, 34($4)
$400039:
	.loc	2 77
 #  77	    Input_ApplyPadConfig(blt);
	.livereg	0x0800000E,0x00000000
	jal	Input_ApplyPadConfig
	.loc	2 78
 #  78	    return 1;
	li	$2, 1
$400040:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 40
	j	$31
	.end	func_803A4134

	.extern	D_8014A108 2
	.extern	D_8014A110 4
	.extern	D_80140BDC 1
	.extern	D_8017A4E0 20
	.text	
	.align	2
	.file	2 "g.c"
	.globl	func_803A4340
	.loc	2 40
 #  40	{
	.ent	func_803A4340 2
func_803A4340:
	.option	O3
	subu	$sp, 112
	sw	$31, 68($sp)
	sw	$30, 64($sp)
	sw	$23, 60($sp)
	sw	$22, 56($sp)
	sw	$21, 52($sp)
	sw	$20, 48($sp)
	sw	$19, 44($sp)
	sw	$18, 40($sp)
	sw	$17, 36($sp)
	sw	$16, 32($sp)
	.mask	0xC0FF0000, -44
	.frame	$sp, 112, $31
	.loc	2 40
	sw	$4, 112($sp)
	.loc	2 40
	.loc	2 47
 #  47	    render_helper(0.0f);
	li.s	$f12, 0.0
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 48
 #  48	    object_create(12);
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
	.loc	2 49
 #  49	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 50
 #  50	    for (i = 0; i < D_8014A108; i++) {
	move	$21, $0
	lh	$2, D_8014A108
	ble	$2, 0, $500036
	la	$14, D_803BA028
	sw	$14, 76($sp)
	la	$30, D_8014A110
	li	$23, 2
	li	$22, 1
	la	$20, D_8017A4E0
$500032:
	.loc	2 50
	.loc	2 51
 #  51	        if (D_803BA028[i] == 1) {
	lw	$15, 76($sp)
	lb	$24, 0($15)
	beq	$22, $24, $500035
	.loc	2 51
	.loc	2 52
 #  52	            continue;
	.loc	2 54
 #  53	        }
 #  54	        for (j = 0; j < 4; j++) {
	move	$16, $0
$500033:
	.loc	2 54
	.loc	2 55
 #  55	            if (i < D_8014A108 && D_8014A108 == 1 && D_8014A110 != 2) {
	bge	$21, $2, $500034
	bne	$22, $2, $500034
	.noalias	$30,$sp
	lw	$25, 0($30)
	beq	$23, $25, $500034
	.loc	2 55
	.loc	2 56
 #  56	                texture = func_800B24EC(D_803B85E8, &index, 0, D_80140BDC - 1, 1);
	la	$4, D_803B85E8
	addu	$5, $sp, 102
	move	$6, $0
	la	$8, D_80140BDC
	.set	 volatile
	lbu	$7, 0($8)
	.set	 novolatile
	addu	$7, $7, -1
	sll	$9, $7, 24
	move	$7, $9
	sra	$10, $7, 24
	move	$7, $10
	sw	$22, 16($sp)
	mul	$18, $16, 2
	mul	$19, $16, 4
	.livereg	0x0F00300E,0x00000000
	jal	func_800B24EC
	.loc	2 57
 #  57	                y = 48 + 2 * j * (texture->height / 8);
	lhu	$11, 18($2)
	div	$12, $11, 8
	mul	$17, $18, $12
	addu	$17, $17, 48
	.loc	2 58
 #  58	                state_utility(10, y - object_bytes_sum_global(), D_8017A4E0.text[D_8017A4E0.indices->first + j]);
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	li	$4, 10
	subu	$5, $17, $2
	sll	$13, $5, 16
	move	$5, $13
	sra	$14, $5, 16
	move	$5, $14
	.noalias	$20,$30
	.noalias	$20,$sp
	lw	$15, 16($20)
	lw	$24, 12($20)
	lhu	$25, 26($24)
	mul	$8, $25, 4
	addu	$9, $15, $8
	addu	$10, $9, $19
	lw	$6, 0($10)
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	lh	$2, D_8014A108
$500034:
	.loc	2 54
 #  54	        for (j = 0; j < 4; j++) {
	addu	$16, $16, 1
	bne	$16, 4, $500033
	.loc	2 50
 #  50	    for (i = 0; i < D_8014A108; i++) {
$500035:
	addu	$21, $21, 1
	lw	$11, 76($sp)
	addu	$12, $11, 1
	sw	$12, 76($sp)
	blt	$21, $2, $500032
	.alias	$20,$30
	.alias	$20,$sp
	.alias	$30,$sp
$500036:
	.loc	2 62
 #  62	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 63
 #  63	    return 1;
	li	$2, 1
	.livereg	0x2000FF0E,0x00000FFF
	lw	$16, 32($sp)
	lw	$17, 36($sp)
	lw	$18, 40($sp)
	lw	$19, 44($sp)
	lw	$20, 48($sp)
	lw	$21, 52($sp)
	lw	$22, 56($sp)
	lw	$23, 60($sp)
	lw	$31, 68($sp)
	lw	$30, 64($sp)
	addu	$sp, 112
	j	$31
	.end	func_803A4340

	.text
	.globl	func_803A44FC
	.ent	func_803A44FC 2
func_803A44FC:
	.option	O3
	subu	$sp, 312
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
	.mask	0xC0FF0000, -196
	.fmask	0xFFF00000, -240
	.frame	$sp, 312, $31
	.loc	2 84
	sw	$4, 312($sp)
	.loc	2 84
	.loc	2 86
 #  85	    s32 i;
 #  86	    for (i = 0; i < D_8014A108; i++) {
	move	$19, $0
	lh	$3, D_8014A108
	ble	$3, 0, $600088
	sw	$0, 152($sp)
	la	$2, D_803B9B90
	li.s	$f30, 1.5707964
	li.s	$f28, 2.443461
	li.s	$f26, 0.6981317
	li.s	$f24, 5.0E-1
	la	$21, countdown_state
$600032:
	.loc	2 86
	.loc	2 87
 #  87	        if (D_803B9B90[i] == 1) animButtonText(i);
	.noalias	$2,$sp
	lw	$14, 0($2)
	bne	$14, 1, $600087
	.loc	2 87
	sw	$19, 304($sp)
	.loc	2 94
 #  94	{
	.loc	2 102
 # 102	    f32 gTextPosScale = D_803B25AC;
	l.s	$f22, D_803B25AC
	.loc	2 103
 # 103	    if (D_80151AD0 > 1) gTextPosScale *= 0.5f;
	mul	$15, $19, 152
	la	$24, D_80150B70
	addu	$25, $15, $24
	sw	$25, 128($sp)
	la	$9, D_803BA028
	addu	$10, $19, $9
	sw	$10, 132($sp)
	mul	$11, $19, 76
	la	$12, D_8014A118
	addu	$22, $11, $12
	mul	$13, $19, 2816
	addu	$14, $13, 2480
	la	$15, D_803AF9A8
	addu	$16, $14, $15
	lh	$24, D_80151AD0
	blt	$24, 2, $600033
	.loc	2 103
	li.s	$f4, 0.5
	mul.s	$f22, $f22, $f4
$600033:
	.loc	2 104
 # 104	    render_helper(0.0f);
	li.s	$f12, 0.0
	sw	$2, 148($sp)
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.alias	$2,$sp
	.loc	2 105
 # 105	    func_800B669C(1, 1);
	li	$4, 1
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 106
 # 106	    if (D_80151AD0 == 1) { w = 140; object_create(13); }
	lh	$25, D_80151AD0
	bne	$25, 1, $600034
	.loc	2 106
	.loc	2 106
	li	$20, 140
	.loc	2 106
	li	$4, 13
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$600035
$600034:
	.loc	2 107
 # 107	    else { w = 69; object_create(12); }
	.loc	2 107
	li	$20, 69
	.loc	2 107
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$600035:
	.loc	2 108
 # 108	    brake_light_update(p, D_803AF9A8[p][38].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	move	$5, $16
	lw	$6, 128($sp)
	move	$7, $0
	addu	$9, $sp, 216
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 109
 # 109	    x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 110
 # 110	    y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 111
 # 111	    str = (*D_8014A118[p].owner)->name;
	.noalias	$22,$sp
	lw	$10, 72($22)
	lw	$16, 0($10)
	addu	$16, $16, 20
	.loc	2 112
 # 112	    dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.alias	$22,$sp
	.loc	2 113
 # 113	    music_fade(x, y, w, str);
	sll	$4, $17, 16
	sra	$11, $4, 16
	move	$4, $11
	sll	$5, $18, 16
	sra	$12, $5, 16
	move	$5, $12
	sll	$6, $20, 16
	sra	$13, $6, 16
	move	$6, $13
	move	$7, $16
	.livereg	0x0F00000E,0x00000000
	jal	music_fade
	.loc	2 114
 # 114	    func_800B669C(0, 1);
	move	$4, $0
	li	$5, 1
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 115
 # 115	    if (D_8014A110 == 2) {
	lw	$14, D_8014A110
	bne	$14, 2, $600041
	.loc	2 115
	.loc	2 117
 # 116	        s32 ghostIdx;
 # 117	        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	li	$20, 1
	lb	$15, D_803BA00C
	addu	$24, $15, 1
	blt	$24, 2, $600041
	la	$23, D_80150B70+152
	li	$25, 4
	sw	$25, 140($sp)
	la	$30, D_80152698+4
$600036:
	.loc	2 117
	.loc	2 119
 # 118	            f32 pos[3];
 # 119	            brake_light_update(ghostIdx, D_803AF9A8[0][38].position, &D_80150B70[ghostIdx], 0, screenPos);
	move	$4, $20
	la	$5, D_803AF9A8+2480
	move	$6, $23
	move	$7, $0
	addu	$9, $sp, 216
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 120
 # 120	            x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 121
 # 121	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 122
 # 122	            fcvt_wrapper(str1, "GHOST %d", ghostIdx);
	addu	$4, $sp, 220
	la	$5, $$600023
	move	$6, $20
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 123
 # 123	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 124
 # 124	            state_utility(sound_pitch_diff_halved(str1, x), y, str1);
	addu	$4, $sp, 220
	sll	$5, $17, 16
	sra	$10, $5, 16
	move	$5, $10
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$11, $4, 16
	move	$4, $11
	sll	$5, $18, 16
	sra	$12, $5, 16
	move	$5, $12
	addu	$6, $sp, 220
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	la	$2, D_803AF9A8
	.loc	2 125
 # 125	            pos[0] = D_803AF9A8[0][38].position[0];
	.noalias	$2,$sp
	l.s	$f6, 2480($2)
	s.s	$f6, 196($sp)
	.loc	2 126
 # 126	            pos[1] = D_803AF9A8[0][38].position[1];
	l.s	$f0, 2484($2)
	.loc	2 127
 # 127	            pos[2] = D_803AF9A8[0][38].position[2];
	l.s	$f8, 2488($2)
	s.s	$f8, 204($sp)
	.loc	2 128
 # 128	            pos[1] = -pos[1] / 2.0f;
	neg.s	$f10, $f0
	mul.s	$f0, $f10, $f24
	.loc	2 129
 # 129	            brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, screenPos);
	move	$4, $20
	addu	$5, $sp, 196
	move	$6, $23
	move	$7, $0
	addu	$13, $sp, 216
	sw	$13, 16($sp)
	s.s	$f0, 200($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.alias	$2,$sp
	.loc	2 130
 # 130	            x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 131
 # 131	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 132
 # 132	            if (!D_80152698[ghostIdx]) {
	.noalias	$30,$sp
	lw	$2, 0($30)
	bne	$2, 0, $600039
	.loc	2 132
	.loc	2 133
 # 133	                str = !D_803AF980 ? countdown_state.text[225] : countdown_state.text[224];
	lw	$14, D_803AF980
	bne	$14, 0, $600037
	.noalias	$21,$30
	.noalias	$21,$sp
	lw	$15, 4($21)
	lw	$16, 900($15)
	b	$600038
$600037:
	lw	$24, 4($21)
	lw	$16, 896($24)
$600038:
	.loc	2 134
 # 134	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 135
 # 135	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$5, $17, 16
	sra	$25, $5, 16
	move	$5, $25
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$9, $4, 16
	move	$4, $9
	sll	$5, $18, 16
	sra	$10, $5, 16
	move	$5, $10
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	b	$600040
$600039:
	.loc	2 136
 # 136	            } else {
	.loc	2 137
 # 137	                str = (*D_80152698[ghostIdx])->name;
	lw	$16, 0($2)
	addu	$16, $16, 10
	.loc	2 138
 # 138	                dispatch_handler(D_801146AC[ghostIdx]);
	lw	$11, 140($sp)
	lw	$4, D_801146AC($11)
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 139
 # 139	                state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$22, $17, 16
	sra	$12, $22, 16
	move	$22, $12
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 140
 # 140	                y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	li	$8, 60
	addu	$18, $18, $2
	.loc	2 141
 # 141	                fcvt_wrapper(str1, "TIME %01d:%02d.%03d", (s32)(*D_80152698[ghostIdx])->time / 60,
	addu	$4, $sp, 220
	la	$5, $$600031
	lw	$24, 0($30)
	lw	$25, 0($24)
	l.s	$f0, 32($25)
	trunc.w.s	$f16, $f0, $9
	mfc1	$3, $f16
	div	$6, $3, $8
	rem	$7, $3, $8
	li.s	$f18, 1000.0
	mul.s	$f4, $f0, $f18
	trunc.w.s	$f6, $f4, $10
	mfc1	$11, $f6
	rem	$12, $11, 1000
	sw	$12, 16($sp)
	.livereg	0x1F00000E,0x80000000
	jal	fcvt_wrapper
	.loc	2 143
 # 142	                             (s32)(*D_80152698[ghostIdx])->time % 60, (s32)(1000.0f * (*D_80152698[ghostIdx])->time) % 1000);
 # 143	                dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 144
 # 144	                state_utility(sound_pitch_diff_halved(str1, x), y, str1);
	addu	$4, $sp, 220
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$15, $5, 16
	move	$5, $15
	addu	$6, $sp, 220
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$600040:
	.loc	2 117
 # 117	        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
	addu	$20, $20, 1
	addu	$23, $23, 152
	lw	$24, 140($sp)
	addu	$25, $24, 4
	sw	$25, 140($sp)
	addu	$30, $30, 4
	lb	$9, D_803BA00C
	ble	$20, $9, $600036
	.alias	$30,$21
	.alias	$30,$sp
$600041:
	.loc	2 148
 # 148	    if (D_803BA028[p]) {
	lw	$10, 132($sp)
	lb	$11, 0($10)
	beq	$11, 0, $600048
	addu	$5, $sp, 172
	.loc	2 148
	.loc	2 152
 # 149	        s32 notReadyCount;
 # 150	        s32 notReadyIdx;
 # 151	        s32 pIdx;
 # 152	        f32 msgPos[3] = {0.0f, -120.0f, 300.0f};
	.noalias	$5,$21
	.noalias	$5,$gp
	la	$12, $$600033
	.set	 noat
	.set	 noat
	lw	$1, 0($12)
	sw	$1, 0($5)
	lw	$14, 4($12)
	sw	$14, 4($5)
	lw	$1, 8($12)
	sw	$1, 8($5)
	.set	 at
	.loc	2 153
 # 153	        brake_light_update(p, msgPos, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	lw	$6, 128($sp)
	move	$7, $0
	addu	$15, $sp, 216
	sw	$15, 16($sp)
	move	$20, $0
	move	$16, $0
	.livereg	0x0F00880E,0x00000000
	jal	brake_light_update
	.alias	$5,$21
	.alias	$5,$gp
	.loc	2 154
 # 154	        x = screenPos[0];
	lh	$17, 216($sp)
	.loc	2 155
 # 155	        y = screenPos[1] - object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	lh	$24, 218($sp)
	subu	$18, $24, $2
	.loc	2 156
 # 156	        notReadyCount = 0;
	.loc	2 157
 # 157	        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	lh	$3, D_8014A108
	ble	$3, 0, $600044
$600042:
	.loc	2 157
	.loc	2 158
 # 158	            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
	beq	$19, $16, $600043
	lb	$25, D_803BA028($16)
	bne	$25, 0, $600043
	.loc	2 158
	.loc	2 158
	sw	$16, 188($sp)
	.loc	2 158
	addu	$20, $20, 1
$600043:
	.loc	2 157
 # 157	        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
	addu	$16, $16, 1
	blt	$16, $3, $600042
$600044:
	.loc	2 160
 # 158	            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
 # 159	        }
 # 160	        if (!notReadyCount) {
	bne	$20, 0, $600045
	.loc	2 160
	.loc	2 161
 # 161	            y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 162
 # 162	            str = (u8 *)"GET READY TO RACE";
	la	$16, $$600034
	.loc	2 163
 # 163	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	sll	$22, $17, 16
	sra	$9, $22, 16
	move	$22, $9
	b	$600047
$600045:
	.loc	2 164
 # 164	        } else {
	.loc	2 165
 # 165	            str = countdown_state.text[151];
	lw	$10, 4($21)
	lw	$16, 604($10)
	.loc	2 166
 # 166	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	.loc	2 167
 # 167	            state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$22, $17, 16
	sra	$11, $22, 16
	move	$22, $11
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00020E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$12, $4, 16
	move	$4, $12
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 168
 # 168	            y += object_bytes_sum_global();
	.livereg	0x0000000E,0x00000000
	jal	object_bytes_sum_global
	addu	$18, $18, $2
	.loc	2 169
 # 169	            if (notReadyCount == 1) {
	bne	$20, 1, $600046
	.loc	2 169
	.loc	2 170
 # 170	                dispatch_handler(D_801146AC[notReadyIdx]);
	lw	$15, 188($sp)
	mul	$24, $15, 4
	lw	$4, D_801146AC($24)
	mul	$25, $15, 76
	la	$9, D_8014A118
	addu	$17, $25, $9
	.livereg	0x0800400E,0x00000000
	jal	dispatch_handler
	.loc	2 171
 # 171	                str = (*D_8014A118[notReadyIdx].owner)->name;
	.noalias	$17,$21
	.noalias	$17,$sp
	lw	$10, 72($17)
	lw	$16, 0($10)
	addu	$16, $16, 20
	b	$600047
	.alias	$17,$21
	.alias	$17,$sp
$600046:
	.loc	2 172
 # 172	            } else str = countdown_state.text[31];
	lw	$11, 4($21)
	lw	$16, 124($11)
$600047:
	.loc	2 174
 # 173	        }
 # 174	        state_utility(sound_pitch_diff_halved(str, x), y, str);
	move	$4, $16
	sll	$5, $22, 16
	sra	$13, $5, 16
	move	$5, $13
	.livereg	0x0C00000E,0x00000000
	jal	sound_pitch_diff_halved
	sll	$4, $2, 16
	sra	$12, $4, 16
	move	$4, $12
	sll	$5, $18, 16
	sra	$14, $5, 16
	move	$5, $14
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 175
 # 175	        func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 176
 # 176	        func_800ED66C(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 177
 # 177	        return;
	b	$600086
$600048:
	.loc	2 179
 # 178	    }
 # 179	    if (D_80151AD0 == 1) object_create(11); else object_create(12);
	move	$2, $0
	lw	$24, 152($sp)
	la	$15, D_803BA050
	addu	$16, $24, $15
	lh	$25, D_80151AD0
	bne	$25, 1, $600049
	.loc	2 179
	li	$4, 11
	.livereg	0x0800000E,0x00000000
	jal	object_create
	b	$600050
$600049:
	.loc	2 179
	li	$4, 12
	.livereg	0x0800000E,0x00000000
	jal	object_create
$600050:
	.loc	2 180
 # 180	    for (i = 0; i < D_803BA050[p]; i++) {
	.noalias	$16,$21
	.noalias	$16,$sp
	lw	$9, 0($16)
	ble	$9, 0, $600085
	.alias	$16,$21
	.alias	$16,$sp
	lw	$10, 304($sp)
	mul	$11, $10, 4
	la	$13, D_803BA050
	addu	$12, $11, $13
	sw	$12, 156($sp)
	move	$23, $0
	mul	$14, $19, 2816
	la	$24, D_803AF9A8
	addu	$15, $14, $24
	sw	$15, 132($sp)
	mul	$25, $19, 76
	la	$9, D_803BA060
	addu	$30, $25, $9
$600051:
	.loc	2 180
	.loc	2 181
 # 181	        s32 item = D_803BA060[p][i];
	.noalias	$30,$21
	.noalias	$30,$sp
	lw	$20, 0($30)
	.loc	2 182
 # 182	        if ((D_803AF9A8[p][item].angle > 0.6981317f && D_803AF9A8[p][item].angle < 2.443461f) || !D_803AF9A8[p][item].alpha) continue;
	.noalias	$22,$21
	.noalias	$22,$30
	.noalias	$22,$sp
	lw	$10, 132($sp)
	mul	$11, $20, 64
	addu	$22, $10, $11
	l.s	$f0, 8($22)
	c.lt.s	$f26, $f0
	bc1f	$600052
	c.lt.s	$f0, $f28
	bc1t	$600053
$600052:
	lbu	$2, 60($22)
	bne	$2, 0, $600054
	.alias	$22,$21
	.alias	$22,$30
	.alias	$22,$sp
$600053:
	.loc	2 182
	lw	$13, 156($sp)
	lw	$2, 0($13)
	mul	$3, $2, 4
	b	$600084
$600054:
	.loc	2 183
 # 183	        func_800ED66C(D_803AF9A8[p][item].alpha);
	mtc1	$2, $f8
	cvt.s.w	$f12, $f8
	bge	$2, $0, $600089
	li.s	$f10, 4294967296.0
	add.s	$f12, $f12, $f10
$600089:
	li.s	$f16, 45.0
	mul.s	$f18, $f16, $f22
	li.s	$f4, 300.0
	mul.s	$f20, $f18, $f4
	.livereg	0x0000000E,0x00080800
	jal	func_800ED66C
	.loc	2 184
 # 184	        if (D_803AF9A8[p][item].angle < 1.5707964f) {
	.noalias	$22,$21
	.noalias	$22,$30
	.noalias	$22,$sp
	l.s	$f6, 8($22)
	c.lt.s	$f6, $f30
	bc1f	$600055
	.loc	2 184
	.loc	2 185
 # 185	            func_800BEA3C(D_801146BC, D_801146C0);
	lw	$12, D_801146BC
	sw	$12, 0($sp)
	lw	$14, D_801146C0
	sw	$14, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 186
 # 186	            dispatch_handler(1);
	li	$4, 1
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
	b	$600056
$600055:
	.loc	2 187
 # 187	        } else {
	.loc	2 188
 # 188	            func_800BEA3C(D_801146BC, D_801146C0);
	lw	$24, D_801146BC
	sw	$24, 0($sp)
	lw	$15, D_801146C0
	sw	$15, 4($sp)
	lw	$4, 0($sp)
	lw	$5, 4($sp)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BEA3C
	.loc	2 189
 # 189	            dispatch_handler(22);
	li	$4, 22
	.livereg	0x0800000E,0x00000000
	jal	dispatch_handler
$600056:
	.loc	2 191
 # 190	        }
 # 191	        func_800F6928(D_803AF9A8[p][item].position[2] - 10.0f);
	l.s	$f8, 56($22)
	li.s	$f10, 10.0
	sub.s	$f12, $f8, $f10
	.livereg	0x0000000E,0x00080000
	jal	func_800F6928
	.loc	2 192
 # 192	        brake_light_update(p, D_803AF9A8[p][item].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	addu	$5, $22, 48
	lw	$6, 128($sp)
	move	$7, $0
	addu	$25, $sp, 216
	sw	$25, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	brake_light_update
	.loc	2 193
 # 193	        x = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_803B25C0;
	lh	$9, 216($sp)
	mtc1	$9, $f16
	cvt.s.w	$f18, $f16
	l.s	$f4, D_803B25C0
	div.s	$f6, $f20, $f4
	sub.s	$f8, $f18, $f6
	trunc.w.s	$f10, $f8, $10
	mfc1	$17, $f10
	.loc	2 194
 # 194	        y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 195
 # 195	        str = D_80151AD0 == 1 ? countdown_state.indexed[countdown_state.indices[2] + item]
	lh	$11, D_80151AD0
	bne	$11, 1, $600057
	lw	$13, 16($21)
	lw	$12, 12($21)
	lhu	$14, 4($12)
	mul	$24, $14, 4
	addu	$15, $13, $24
	mul	$25, $20, 4
	addu	$9, $15, $25
	lw	$16, 0($9)
	b	$600058
$600057:
	lw	$10, 16($21)
	lw	$11, 12($21)
	lhu	$12, 6($11)
	mul	$14, $12, 4
	addu	$13, $10, $14
	mul	$24, $20, 4
	addu	$15, $13, $24
	lw	$16, 0($15)
$600058:
	.loc	2 197
 # 196	                              : countdown_state.indexed[countdown_state.indices[3] + item];
 # 197	        state_utility(x, y, str);
	sll	$4, $17, 16
	sra	$25, $4, 16
	move	$4, $25
	sll	$5, $18, 16
	sra	$9, $5, 16
	move	$5, $9
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
	.loc	2 198
 # 198	        switch (item) {
	bgeu	$20, 19, $600081
	sll	$11, $20, 2
	lw	$11, $600059($11)
	j	$11
	.rdata	
$600059:
	.word	$600061
	.word	$600062
	.word	$600063
	.word	$600064
	.word	$600065
	.word	$600066
	.word	$600067
	.word	$600081
	.word	$600081
	.word	$600081
	.word	$600068
	.word	$600069
	.word	$600070
	.word	$600081
	.word	$600081
	.word	$600071
	.word	$600072
	.word	$600075
	.word	$600078
	.text	
$600061:
	.loc	2 199
 # 199	        case 0: str = D_80110D70[D_803B9FD0[p]]; break;
	lb	$12, D_803B9FD0($19)
	mul	$10, $12, 4
	lw	$16, D_80110D70($10)
	.loc	2 199
	b	$600082
$600062:
	.loc	2 200
 # 200	        case 1: str = countdown_state.indexed[countdown_state.indices[4] + D_80110E78[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$13, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$15, $13, $24
	lb	$25, D_80110E78+13($15)
	mul	$9, $25, 4
	addu	$11, $14, $9
	lw	$12, 12($21)
	lhu	$10, 8($12)
	mul	$13, $10, 4
	addu	$24, $11, $13
	lw	$16, 0($24)
	.loc	2 200
	b	$600082
$600063:
	.loc	2 201
 # 201	        case 2: str = countdown_state.indexed[countdown_state.indices[5] + D_8011103C[p+1][D_803B9FD0[p]]]; break;
	lw	$15, 16($21)
	mul	$25, $19, 13
	lb	$14, D_803B9FD0($19)
	addu	$9, $25, $14
	lb	$12, D_8011103C+13($9)
	mul	$10, $12, 4
	addu	$11, $15, $10
	lw	$13, 12($21)
	lhu	$24, 10($13)
	mul	$25, $24, 4
	addu	$14, $11, $25
	lw	$16, 0($14)
	.loc	2 201
	b	$600082
$600064:
	.loc	2 202
 # 202	        case 3: str = countdown_state.indexed[countdown_state.indices[6] + D_80111080[p+1][D_803B9FD0[p]]]; break;
	lw	$9, 16($21)
	mul	$12, $19, 13
	lb	$15, D_803B9FD0($19)
	addu	$10, $12, $15
	lb	$13, D_80111080+13($10)
	mul	$24, $13, 4
	addu	$11, $9, $24
	lw	$25, 12($21)
	lhu	$14, 12($25)
	mul	$12, $14, 4
	addu	$15, $11, $12
	lw	$16, 0($15)
	.loc	2 202
	b	$600082
$600065:
	.loc	2 203
 # 203	        case 4: str = countdown_state.indexed[countdown_state.indices[7] + D_8011119C[p+1][D_803B9FD0[p]]]; break;
	lw	$10, 16($21)
	mul	$13, $19, 13
	lb	$9, D_803B9FD0($19)
	addu	$24, $13, $9
	lb	$25, D_8011119C+13($24)
	mul	$14, $25, 4
	addu	$11, $10, $14
	lw	$12, 12($21)
	lhu	$15, 14($12)
	mul	$13, $15, 4
	addu	$9, $11, $13
	lw	$16, 0($9)
	.loc	2 203
	b	$600082
$600066:
	.loc	2 204
 # 204	        case 5: str = countdown_state.indexed[countdown_state.indices[8] + D_80111230[p+1][D_803B9FD0[p]]]; break;
	lw	$24, 16($21)
	mul	$25, $19, 13
	lb	$10, D_803B9FD0($19)
	addu	$14, $25, $10
	lb	$12, D_80111230+13($14)
	mul	$15, $12, 4
	addu	$11, $24, $15
	lw	$13, 12($21)
	lhu	$9, 16($13)
	mul	$25, $9, 4
	addu	$10, $11, $25
	lw	$16, 0($10)
	.loc	2 204
	b	$600082
$600067:
	.loc	2 205
 # 205	        case 6: str = countdown_state.indexed[countdown_state.indices[9] + D_8011128C[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$12, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$15, $12, $24
	lb	$13, D_8011128C+13($15)
	mul	$9, $13, 4
	addu	$11, $14, $9
	lw	$25, 12($21)
	lhu	$10, 18($25)
	mul	$12, $10, 4
	addu	$24, $11, $12
	lw	$16, 0($24)
	.loc	2 205
	b	$600082
$600068:
	.loc	2 206
 # 206	        case 10: str = countdown_state.indexed[countdown_state.indices[12] + D_8012E67C[p]]; break;
	lw	$15, 16($21)
	lb	$13, D_8012E67C($19)
	mul	$14, $13, 4
	addu	$9, $15, $14
	lw	$25, 12($21)
	lhu	$10, 24($25)
	mul	$11, $10, 4
	addu	$12, $9, $11
	lw	$16, 0($12)
	.loc	2 206
	b	$600082
$600069:
	.loc	2 207
 # 207	        case 11: str = countdown_state.indexed[countdown_state.indices[10] + D_8011157C[p+1][D_803B9FD0[p]]]; break;
	lw	$24, 16($21)
	mul	$13, $19, 13
	lb	$15, D_803B9FD0($19)
	addu	$14, $13, $15
	lb	$25, D_8011157C+13($14)
	mul	$10, $25, 4
	addu	$9, $24, $10
	lw	$11, 12($21)
	lhu	$12, 20($11)
	mul	$13, $12, 4
	addu	$15, $9, $13
	lw	$16, 0($15)
	.loc	2 207
	b	$600082
$600070:
	.loc	2 208
 # 208	        case 12: str = countdown_state.indexed[countdown_state.indices[11] + D_801115C0[p+1][D_803B9FD0[p]]]; break;
	lw	$14, 16($21)
	mul	$25, $19, 13
	lb	$24, D_803B9FD0($19)
	addu	$10, $25, $24
	lb	$11, D_801115C0+13($10)
	mul	$12, $11, 4
	addu	$9, $14, $12
	lw	$13, 12($21)
	lhu	$15, 22($13)
	mul	$25, $15, 4
	addu	$24, $9, $25
	lw	$16, 0($24)
	.loc	2 208
	b	$600082
$600071:
	.loc	2 209
 # 209	        case 15: fcvt_wrapper(str1, "%d", D_803B28B0[p] + 1); str = str1; break;
	addu	$4, $sp, 220
	la	$5, $$600054
	mul	$10, $19, 2
	lh	$6, D_803B28B0($10)
	addu	$6, $6, 1
	.livereg	0x0E00000E,0x00000000
	jal	fcvt_wrapper
	.loc	2 209
	addu	$16, $sp, 220
	.loc	2 209
	b	$600082
$600072:
	.loc	2 211
 # 210	        case 16:
 # 211	            if (D_80152698[1]) func_800BE6A4(str1, (*D_80152698[1])->name);
	lw	$2, D_80152698+4
	beq	$2, 0, $600073
	.loc	2 211
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$600074
$600073:
	.loc	2 212
 # 212	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$11, 16($21)
	lw	$14, 12($21)
	lhu	$12, 30($14)
	mul	$13, $12, 4
	addu	$15, $11, $13
	lw	$5, 0($15)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$600074:
	.loc	2 213
 # 213	            str = str1;
	addu	$16, $sp, 220
	.loc	2 214
 # 214	            break;
	b	$600082
$600075:
	.loc	2 216
 # 215	        case 17:
 # 216	            if (D_80152698[2]) func_800BE6A4(str1, (*D_80152698[2])->name);
	lw	$2, D_80152698+8
	beq	$2, 0, $600076
	.loc	2 216
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$600077
$600076:
	.loc	2 217
 # 217	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$9, 16($21)
	lw	$25, 12($21)
	lhu	$24, 30($25)
	mul	$10, $24, 4
	addu	$14, $9, $10
	lw	$5, 0($14)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$600077:
	.loc	2 218
 # 218	            str = str1;
	addu	$16, $sp, 220
	.loc	2 219
 # 219	            break;
	b	$600082
$600078:
	.loc	2 221
 # 220	        case 18:
 # 221	            if (D_80152698[3]) func_800BE6A4(str1, (*D_80152698[3])->name);
	lw	$2, D_80152698+12
	beq	$2, 0, $600079
	.loc	2 221
	addu	$4, $sp, 220
	lw	$5, 0($2)
	addu	$5, $5, 10
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
	b	$600080
$600079:
	.loc	2 222
 # 222	            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
	addu	$4, $sp, 220
	lw	$12, 16($21)
	lw	$11, 12($21)
	lhu	$13, 30($11)
	mul	$15, $13, 4
	addu	$25, $12, $15
	lw	$5, 0($25)
	.livereg	0x0C00000E,0x00000000
	jal	func_800BE6A4
$600080:
	.loc	2 223
 # 223	            str = str1;
	addu	$16, $sp, 220
	.loc	2 224
 # 224	            break;
	b	$600082
$600081:
	.loc	2 225
 # 225	        default: str = 0; break;
	move	$16, $0
	.loc	2 225
$600082:
	.loc	2 227
 # 226	        }
 # 227	        if (str) {
	beq	$16, 0, $600083
	.loc	2 227
	.loc	2 228
 # 228	            brake_light_update(p, D_803AF9A8[p][item+19].position, &D_80150B70[p], 0, screenPos);
	move	$4, $19
	addu	$5, $22, 1264
	lw	$6, 128($sp)
	move	$7, $0
	addu	$24, $sp, 216
	sw	$24, 16($sp)
	li.s	$f16, 50.0
	mul.s	$f4, $f16, $f22
	li.s	$f18, 300.0
	mul.s	$f20, $f4, $f18
	.livereg	0x0F00000E,0x00000800
	jal	brake_light_update
	.alias	$22,$21
	.alias	$22,$30
	.alias	$22,$sp
	.loc	2 229
 # 229	            x = screenPos[0] - 50.0f * gTextPosScale * 300.0f / D_803B25C0;
	lh	$9, 216($sp)
	mtc1	$9, $f6
	cvt.s.w	$f8, $f6
	l.s	$f10, D_803B25C0
	div.s	$f16, $f20, $f10
	sub.s	$f4, $f8, $f16
	trunc.w.s	$f18, $f4, $10
	mfc1	$17, $f18
	.loc	2 230
 # 230	            y = screenPos[1];
	lh	$18, 218($sp)
	.loc	2 231
 # 231	            state_utility(x, y, str);
	sll	$4, $17, 16
	sra	$14, $4, 16
	move	$4, $14
	sll	$5, $18, 16
	sra	$11, $5, 16
	move	$5, $11
	move	$6, $16
	.livereg	0x0E00000E,0x00000000
	jal	state_utility
$600083:
	.loc	2 180
 # 180	    for (i = 0; i < D_803BA050[p]; i++) {
	lw	$13, 156($sp)
	lw	$2, 0($13)
	mul	$3, $2, 4
$600084:
	addu	$23, $23, 4
	addu	$30, $30, 4
	blt	$23, $3, $600051
	.alias	$30,$21
	.alias	$30,$sp
$600085:
	.loc	2 234
 # 234	    func_800B669C(0, 3);
	move	$4, $0
	li	$5, 3
	.livereg	0x0C00000E,0x00000000
	jal	func_800B669C
	.loc	2 235
 # 235	    func_800ED66C(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	func_800ED66C
	.loc	2 236
 # 236	    render_helper(-1.0f);
	li.s	$f12, -1.0000000000000000e+00
	.livereg	0x0000000E,0x00080000
	jal	render_helper
	.loc	2 237
 # 237	}
$600086:
	lh	$3, D_8014A108
	lw	$2, 148($sp)
	.loc	2 88
 #  88	        else drawOptions(i);
	.loc	2 240
 # 240	{
	.loc	2 241
 # 241	}
$600087:
	.loc	2 86
 #  86	    for (i = 0; i < D_8014A108; i++) {
	addu	$19, $19, 1
	lw	$12, 152($sp)
	addu	$15, $12, 4
	sw	$15, 152($sp)
	addu	$2, $2, 4
	blt	$19, $3, $600032
	.alias	$21,$sp
$600088:
	.loc	2 90
 #  87	        if (D_803B9B90[i] == 1) animButtonText(i);
 #  88	        else drawOptions(i);
 #  89	    }
 #  90	    return 1;
	li	$2, 1
	.livereg	0x2000FF0E,0x00000FFF
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
	addu	$sp, 312
	j	$31
	.end	func_803A44FC
