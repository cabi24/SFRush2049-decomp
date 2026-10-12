/* Bounded actual-reset-source fixtures; selector is a synchronous test double.
 * Host logical pointer layout is not MIPS record layout or native execution. */
#include <stdio.h>
#include <stdlib.h>

Owner *D_8012E6E0, *D_803BAC80[4];
Owner D_80146150[4];
Player D_8014A118[4];
s8 D_803BADBC[4],D_803BADB8[4],D_803BADC8[4],D_803BADCC[4],D_803BADD0[4],D_803BADD4[4];
s32 D_803BAD80[4],D_803BACE0[4],D_803BA888[4];
u8 D_803BAC98[4][13];
s16 D_803BACD0[4],D_803BACD8[4];
static Owner nodes[14];
static OwnerBody bodies[14];
static Selection selections[14];
static Item items[14];
static s32 calls, observed_player, expected_mode;

void func_80394564(s32 player)
{
    if (D_803BACE0[player] != expected_mode || D_803BA888[player] != 1) abort();
    calls++;
    observed_player = player;
}

static void check(int condition)
{
    if (!condition) abort();
}

int main(void)
{
    int player, mode, oldmode, selected, count, level, i, j, h, fixture;
    int id, choice, expected_level, expected_id, expected_calls, ncases = 0;
    unsigned int rng = 94300U;
    Owner *expected_owner;
    check(sizeof(s32)==4);
    for (fixture = 0; fixture < 25000; fixture++) {
        player=fixture%4;
        mode=(fixture/4)%9-1;
        oldmode=(fixture/36)%8-1;
        count=(fixture/288)%15;
        selected=(fixture/4320)%4;
        level=(fixture/7)%16-3;
        id=(fixture*31)%256;
        rng=rng*1664525U+1013904223U;
        for (i=0;i<14;i++) {
            nodes[i].body=&bodies[i];
            bodies[i].next=i+1<count?&nodes[i+1]:0;
            bodies[i].previous=i?&nodes[i-1]:0;
            bodies[i].selection=&selections[i];
            selections[i].item=&items[i];
            items[i].player=(rng>>(i%16))%4;
        }
        D_8012E6E0=count?nodes:0;
        choice=count?((fixture/13)%count):0;
        for (i=0;i<4;i++) {
            D_8014A118[i].id=(u8)id;
            D_8014A118[i].owner=count?&nodes[choice]:0;
            if (selected==1) D_8014A118[i].owner=0;
            if (selected==2) D_8014A118[i].owner=&D_80146150[3];
            D_803BAC80[i]=&D_80146150[2];
            D_803BADBC[i]=91;D_803BADB8[i]=92;
            D_803BAD80[i]=level;D_803BACE0[i]=oldmode;D_803BA888[i]=93;
            D_803BADC8[i]=94;D_803BADCC[i]=95;D_803BADD0[i]=96;D_803BADD4[i]=97;
            D_803BACD0[i]=98;D_803BACD8[i]=99;
            for(j=0;j<13;j++)D_803BAC98[i][j]=(u8)(i*16+j+100);
        }
        if(count && selected==3)bodies[choice].selection=0;
        expected_owner=&D_80146150[2];expected_level=level;expected_id=91;
        if(mode==0) {
            if(count && selected==0) {
                expected_owner=&nodes[choice];expected_id=items[choice].player;
                if(level>=3 && level<11) {
                    expected_level=3;
                    for(i=0;i<choice;i++)if(items[i].player==items[choice].player && expected_level<10)expected_level++;
                }
            } else {
                expected_owner=&D_80146150[player];
                if(oldmode!=3 && oldmode!=4)expected_id=(s8)id;
                if(level>=3 && level<11)expected_level=0;
            }
        }
        expected_mode=mode;calls=0;observed_player=-99;
        func_80394B00(player,mode);
        check(calls==1 && observed_player==player);
        for(i=0;i<4;i++) {
            h=i==player;
            check(D_803BAC80[i]==(h?expected_owner:&D_80146150[2]));
            check(D_803BADBC[i]==(h?expected_id:91));
            check(D_803BAD80[i]==(h?expected_level:level));
            check(D_803BACE0[i]==(h?mode:oldmode));
            check(D_803BA888[i]==(h?1:93));
            check(D_803BADB8[i]==(h && mode==0?0:92));
            check(D_803BACD0[i]==(h && mode==3?0:98));
            check(D_803BACD8[i]==(h && mode==3?0:99));
            check(D_803BADC8[i]==(h && mode==4?1:94));
            check(D_803BADCC[i]==(h && mode==4?0:95));
            check(D_803BADD0[i]==(h && mode==4?0:96));
            check(D_803BADD4[i]==(h && mode==4?0:97));
            for(j=0;j<13;j++)check(D_803BAC98[i][j]==(h && mode==3?0:(u8)(i*16+j+100)));
            expected_owner=count?&nodes[choice]:0;
            if(selected==1)expected_owner=0;
            if(selected==2)expected_owner=&D_80146150[3];
            check(D_8014A118[i].owner==(h && (mode==0 || mode==3 || mode==4)?0:expected_owner));
            /* Restore the selected expected cache owner after the independent owner check. */
            if(mode==0)expected_owner=count && selected==0?&nodes[choice]:&D_80146150[player];
            else expected_owner=&D_80146150[2];
            check(D_8014A118[i].id==(u8)id);
        }
        ncases++;
    }
    for(i=0;i<256;i++) {
        u8 value=(u8)i;
        check(((s32)(value * sizeof(s32))) / (s32)sizeof(s32)==i);
    }
    printf("PASS: %d actual-reset cases; all 256 byte-index identities\n",ncases);
    return 0;
}
