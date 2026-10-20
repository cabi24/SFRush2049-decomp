/* Host-only bounded behavior and layout regression; never linked into N64. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "func_800DE45C.c"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "line %d: %s\n", __LINE__, #c); return 1; } } while (0)
#define OFF(t,f,n) _Static_assert(offsetof(t,f)==n,#f)
_Static_assert(sizeof(Player)==76,"player stride");
_Static_assert(sizeof input_rec0==304,"four player records");
_Static_assert(sizeof D_80146180==4,"four per-player flags");
OFF(Player,node,0); OFF(Player,unk1,1); OFF(Player,unk42,66); OFF(Player,unk44,68);
OFF(Rec164A4,unk06,6); OFF(Rec164A4,unk1A,26); OFF(Rec164A4,unk1C,28);
OFF(Rec164A4,unk1E,30); OFF(Rec164A4,unk20,32);
s8 D_80146108[41];
Rec164A4 D_801164A4;
s16 D_80151AD0;
s16 D_8014A108;
s8 D_8013F1D8;
s8 D_8013F1D9;
s8 D_8013F2FC;
s16 D_8013FEC8;
s8 D_8013FECC;
s8 D_8013FECD;
s8 D_801403D0;
s8 D_80140418;
s8 D_8014061A;
s8 D_801407B0;
s32 D_801407BC;
s8 D_801407D0;
s32 D_801407DC;
s32 D_80140804;
s32 D_80140A00;
s8 D_80140A04;
s32 D_80140AD8;
s32 D_80140B08;
s32 D_80140BD8;
s32 D_80142510;
s8 D_801426EC;
s16 D_80142724;
s8 D_80142726;
s8 D_80142760;
s8 D_801427A0;
s8 D_801427A1;
s8 D_80142B00;
s8 D_80142DB0;
s8 D_80143F1C;
s8 D_80150EFC;
s8 D_80151AD8;
s8 D_80152030;
s8 D_80152570;
s16 D_80152734;
s8 D_801543C8;
s8 D_80156BDC;
s8 D_80156CE8;
s8 D_8015723C;
s8 D_8015B24C;
s8 D_8015B25C;
s8 D_8015F72C;
s8 D_8015F734;
s8 D_8016137C;
s8 D_80161394;
s8 D_801613A0;
s8 D_801613A8;
s8 D_801613A9;
s8 D_801613AA;
s8 D_801613AB;
s8 D_8017A4D8;
s8 D_8017A634;
static int seed, event, bad;
static s8 option(int i) { return (s8)((seed+i*17)&255); }
void drone_target_update(s32 mode) {
    int i;
    if (mode!=1 || event!=0) bad=1;
    for (i=0;i<4;i++) {
        if (input_rec0[i].node!=i || input_rec0[i].unk1!=5 ||
            input_rec0[i].unk42!=0 || input_rec0[i].unk44!=-1) bad=1;
    }
    event=1;
}
s8 func_800CCB40(s32 i) {
    if (event!=i+1 || i<0 || i>=41) bad=1;
    event++;
    return option(i);
}
void speed_set(f32 blend,f32 amount,s32 first,s32 second) {
    int i;
    if (event!=42 && event!=43) bad=1;
    if (blend!=option(event==42?13:12)/10.0f || amount!=0.05f ||
        first!=(event==43) || second!=(event==42)) bad=1;
    for (i=0;i<41;i++) if (D_80146108[i]!=option(i)) bad=1;
    event++;
}
void func_800D6E00(s8 value) {
    if (event!=44 || value!=option(15)) bad=1;
    event++;
}
int main(void) {
    int i,j,cases=0;
    for (seed=0;seed<256;seed++) {
        event=bad=0;
        memset(input_rec0,0x55,sizeof input_rec0);
        memset(&D_801164A4,0x55,sizeof D_801164A4);
        memset(D_80146180,0x55,sizeof D_80146180);
        memset(D_80146108,0x55,sizeof D_80146108);
        D_80151AD0=0x55;
        D_8014A108=0x55;
        D_8013F1D8=0x55;
        D_8013F1D9=0x55;
        D_8013F2FC=0x55;
        D_8013FEC8=0x55;
        D_8013FECC=0x55;
        D_8013FECD=0x55;
        D_801403D0=0x55;
        D_80140418=0x55;
        D_8014061A=0x55;
        D_801407B0=0x55;
        D_801407BC=0x55;
        D_801407D0=0x55;
        D_801407DC=0x55;
        D_80140804=0x55;
        D_80140A00=0x55;
        D_80140A04=0x55;
        D_80140AD8=0x55;
        D_80140B08=0x55;
        D_80140BD8=0x55;
        D_80142510=0x55;
        D_801426EC=0x55;
        D_80142724=0x55;
        D_80142726=0x55;
        D_80142760=0x55;
        D_801427A0=0x55;
        D_801427A1=0x55;
        D_80142B00=0x55;
        D_80142DB0=0x55;
        D_80143F1C=0x55;
        D_80150EFC=0x55;
        D_80151AD8=0x55;
        D_80152030=0x55;
        D_80152570=0x55;
        D_80152734=0x55;
        D_801543C8=0x55;
        D_80156BDC=0x55;
        D_80156CE8=0x55;
        D_8015723C=0x55;
        D_8015B24C=0x55;
        D_8015B25C=0x55;
        D_8015F72C=0x55;
        D_8015F734=0x55;
        D_8016137C=0x55;
        D_80161394=0x55;
        D_801613A0=0x55;
        D_801613A8=0x55;
        D_801613A9=0x55;
        D_801613AA=0x55;
        D_801613AB=0x55;
        D_8017A4D8=0x55;
        D_8017A634=0x55;
        func_800DE45C();
        CHECK(!bad && event==45);
        for(i=0;i<4;i++) {
            unsigned char *p=(unsigned char *)&input_rec0[i];
            CHECK(input_rec0[i].node==i && input_rec0[i].unk1==5);
            CHECK(input_rec0[i].unk42==0 && input_rec0[i].unk44==-1);
            for(j=0;j<76;j++) if(j!=0 && j!=1 && j!=66 && !(j>=68 && j<72)) CHECK(p[j]==0x55);
            CHECK(D_80146180[i]==0);
        }
        for(i=0;i<41;i++) CHECK(D_80146108[i]==option(i));
        CHECK(D_80152734==option(21));
        CHECK(D_80142724==option(22));
        CHECK(D_80156BDC==option(0));
        CHECK(D_801543C8==option(23));
        CHECK(D_80156CE8==option(1));
        CHECK(D_8015723C==option(2));
        CHECK(D_8015B24C==option(3));
        CHECK(D_8015B25C==option(4));
        CHECK(D_8015F72C==option(5));
        CHECK(D_8015F734==option(6));
        CHECK(D_8016137C==option(7));
        CHECK(D_80161394==option(8));
        CHECK(D_80152030==option(24));
        CHECK(D_801426EC==option(25));
        CHECK(D_80150EFC==option(26));
        CHECK(D_80152570==option(27));
        CHECK(D_8013F1D8==option(29));
        CHECK(D_80142726==option(30));
        CHECK(D_801613AA==option(9));
        CHECK(D_80142760==option(31));
        CHECK(D_801613A8==option(10));
        CHECK(D_801613A9==option(11));
        CHECK(D_8013FEC8==option(16));
        CHECK(D_801613A0==option(18));
        CHECK(D_801407BC==option(32));
        CHECK(D_801407DC==option(33));
        CHECK(D_80140804==option(35));
        CHECK(D_80140A00==option(34));
        CHECK(D_80140AD8==option(36));
        CHECK(D_80140B08==option(37));
        CHECK(D_80140BD8==option(39));
        CHECK(D_80142510==option(38));
        CHECK(D_80140A04==(option(28)>0));
        CHECK(D_8013F1D9==(option(28)>=2));
        CHECK(D_8017A4D8==0);
        CHECK(D_801427A0==0);
        CHECK(D_8013FECC==0);
        CHECK(D_8013FECD==0);
        CHECK(D_801164A4.unk06==0);
        CHECK(D_80151AD8==0);
        CHECK(D_801403D0==0);
        CHECK(D_80142B00==0);
        CHECK(D_80140418==0);
        CHECK(D_8014061A==0);
        CHECK(D_8013F2FC==0);
        CHECK(D_80142DB0==0);
        CHECK(D_80143F1C==0);
        CHECK(D_8017A634==0);
        CHECK(D_801164A4.unk1A==0);
        CHECK(D_801164A4.unk1C==0);
        CHECK(D_801164A4.unk1E==0);
        CHECK(D_801164A4.unk20==0);
        CHECK(D_801427A1==0);
        CHECK(D_801407B0==0);
        CHECK(D_801613AB==0);
        CHECK(D_801407D0==0);
        CHECK(D_80151AD0==1 && D_8014A108==1);
        cases++;
    }
    printf("%d bounded host cases passed\n",cases);
    return 0;
}
