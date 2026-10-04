/* Actual reconstructed C exercised through semantic helper contracts. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "nonmatch/func_80019490.c"
SequenceContext D_80043EB8[8];
typedef struct Event { u32 address, a, b, c, d; } Event;
static Event expected[12];
static int count, position, translations, validFirst, validNext;
static u32 created;
static SequenceRequest *current;
static void expect(u32 a,u32 b,u32 c,u32 d,u32 e)
{
    Event *p;
    p=&expected[count++];p->address=a;p->a=b;p->b=c;p->c=d;p->d=e;
}
static void event(u32 a,u32 b,u32 c,u32 d,u32 e)
{
    Event *p;
    assert(position<count);p=&expected[position++];
    assert(p->address==a && p->a==b && p->b==c && p->c==d && p->d==e);
}
u32 func_80017644(u32 a)
{
    int valid;
    event(0x80017644,a,0,0,0);
    valid=translations++ == 0 ? validFirst : validNext;
    return valid ? ((a&0x80000000U)|(translations==1 ? 3 : 6)) : 0xFFFFFFFFU;
}
void func_80019194(u8 a,u16 b,u32 c,u8 d){event(0x80019194,a,b,c,d);}
void func_80019370(u8 a,u16 b,u32 c,u8 d){event(0x80019370,a,b,c,d);}
void func_80018FEC(u32 a){event(0x80018FEC,a,0,0,0);}
void func_8001906C(u32 a){event(0x8001906C,a,0,0,0);}
void func_800190AC(u32 a,u32 b,u32 c){event(0x800190AC,a,b,c,0);}
void func_80019144(u32 a,u32 b,u32 c){event(0x80019144,a,b,c,0);}
void func_80018F20(u32 a,u16 b){event(0x80018F20,a,b,0,0);}
void func_80018FA4(u32 a,u16 b){event(0x80018FA4,a,b,0,0);}
static void options_check(SequenceOptions *o)
{
    u32 flags;
    flags=4U|((current->flags&8)?16U:0U)|((current->flags&16)?1U:0U)|((current->flags&32)?2U:0U);
    assert(o->flags==flags);
    if (flags&1)assert(o->first==current->first && o->second==current->second);
    if (flags&2)assert(o->value==current->value);
    assert(o->duration==current->nextDuration && o->channel==current->channel);
    assert(o->channelCount==0 && !(flags&8));
}
u32 func_8001558C(u16 a,u16 b,void *c,SequenceOptions *d,u8 e)
{
    assert(c==current->stream && e==1);options_check(d);
    event(0x8001558C,a,b,1,0);return created;
}
u32 func_800156E8(u16 a,u16 b,void *c,SequenceOptions *d)
{
    assert(c==current->stream);options_check(d);
    event(0x800156E8,a,b,0,0);return created;
}
static void test(unsigned flags,u8 unlocked,u32 tag,int resultPresent,int firstOK,int nextOK,u32 createResult)
{
    SequenceRequest state,before,copy;
    u32 result,want,service,mode;
    unsigned i;
    memset(&state,0xA5,sizeof(state));memset(D_80043EB8,0x5A,sizeof(D_80043EB8));
    state.identifier=0x1234U|tag;state.duration=0xFFFF;
    state.nextIdentifier=0x5678U|tag;state.nextDuration=0xABCD;
    state.stream=&state;state.group=0x1234;state.program=0x5678;state.channel=255;
    state.first=0xDEAD1234U;state.second=0x87654321U;state.value=0xFEDC;state.flags=(u8)flags;
    before=state;current=&state;result=0xCCCCCCCCU;want=result;
    position=0;count=0;translations=0;validFirst=firstOK;validNext=nextOK;created=createResult;
    expect(0x80017644,state.identifier,0,0,0);
    if (firstOK) {
        if (flags&4) want=state.identifier|0x80000000U;
        else {
            service=unlocked?0x80019194U:0x80019370U;
            mode=(flags&1)?2U:((flags&64)?3U:1U);
            expect(service,0,state.duration,state.identifier,mode);
            if (resultPresent) {
                if (flags&2) {
                    expect(0x80017644,state.nextIdentifier,0,0,0);
                    if (!nextOK) want=0xFFFFFFFFU;
                    else {
                        expect(unlocked?0x80018FECU:0x8001906CU,state.nextIdentifier,0,0,0);
                        expect(service,state.channel,state.nextDuration,state.nextIdentifier,0);
                        if (flags&16)expect(unlocked?0x800190ACU:0x80019144U,state.nextIdentifier,state.first,state.second,0);
                        if (flags&32)expect(unlocked?0x80018F20U:0x80018FA4U,state.nextIdentifier,state.value,0,0);
                        want=state.nextIdentifier;
                    }
                } else {
                    expect(unlocked?0x8001558CU:0x800156E8U,state.group,state.program,unlocked?1U:0U,0);
                    want=createResult;
                    if (createResult!=0xFFFFFFFFU && (flags&128))expect(unlocked?0x800190ACU:0x80019144U,createResult,0,0,0);
                }
            }
        }
    }
    func_80019490(&state,resultPresent?&result:0,unlocked);
    assert(position==count && result==want);
    assert(memcmp(&state,&before,sizeof(state))==0);
    if (firstOK && (flags&4)) {
        copy=before;copy.flags&=(u8)~4;
        assert(memcmp(&copy,&D_80043EB8[3].request,sizeof(copy))==0);
        assert(D_80043EB8[3].pending==1 && D_80043EB8[3].pendingResult==&result);
    }
    for(i=0;i<8;i++)if (!(firstOK && (flags&4) && i==3)) {
        unsigned j;
        const unsigned char *p;
        p=(const unsigned char *)&D_80043EB8[i];
        for(j=0;j<sizeof(SequenceContext);j++)assert(p[j]==0x5A);
    }
}
int main(void)
{
    unsigned flags,mode,cases;
    u8 unlocked;
    cases=0;
    for(flags=0;flags<256;flags++)for(mode=0;mode<3;mode++) {
        unlocked=(u8)(mode==2?255:mode);
        test(flags,unlocked,0,1,1,1,0x7654321);cases++;
        test(flags,unlocked,0x80000000U,1,1,1,0x7654321);cases++;
        test(flags,unlocked,0,1,0,1,0x7654321);cases++;
        if (!(flags&4)){test(flags,unlocked,0,0,1,1,0x7654321);cases++;}
        if ((flags&2) && !(flags&4)){test(flags,unlocked,0,1,1,0,0x7654321);cases++;}
        if (!(flags&6)){test(flags,unlocked,0,1,1,1,0xFFFFFFFFU);cases++;}
    }
    printf("PASS: %u host semantic cases\n",cases);return 0;
}
