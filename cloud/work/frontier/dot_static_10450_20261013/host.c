/* Actual candidate against a logical field oracle; native layout is checked
 * separately with IDO. Hook checks include the entire pending batch and queues. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../matches/boot_tail/func_80010450.c"
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"host line %d case %d\n",__LINE__,case_no); exit(1); } } while(0)
TransferRequest D_80037FA8[4];
volatile unsigned char D_80037FA0;
unsigned char D_80037FD8,D_80037FE0[24];
void *D_8003802C;
unsigned int D_80038038,D_8003803C;
unsigned short D_80038028;
static DmaEntry_80010450 queues[2][65536];
static unsigned char logical[8][256];
static struct Record { unsigned int length; void *source; unsigned char *destination; } expected[2][65536];
static TransferRequest requests[4];
static int case_no,send_mode,write_mode,send_calls,write_calls,eq,index;
static unsigned char ep,ea;
static unsigned short ec;
static unsigned int cursor,command,answer;

static void check_state(void)
{
    int i,j,q;
    CHECK(D_80037FA0==ep && D_80037FD8==ea);
    CHECK(D_80038028==ec && D_8003802C==queues[eq]);
    CHECK(D_80038038==cursor && D_8003803C==command);
    for(i=0;i<4;i++) {
        CHECK(D_80037FA8[i].source==requests[i].source);
        CHECK(D_80037FA8[i].destination==requests[i].destination);
        CHECK(D_80037FA8[i].size==requests[i].size);
    }
    for(q=0;q<2;q++) for(j=0;j<13;j++) {
        i=j==12?65535:j;
        CHECK((unsigned int)queues[q][i].length==expected[q][i].length);
        CHECK(queues[q][i].source==expected[q][i].source);
        CHECK(queues[q][i].destination==expected[q][i].destination);
    }
}

int osSendMesg(OSMesgQueue *queue,void *message,int flag)
{
    ++send_calls;CHECK(send_calls==1 && write_calls==0);
    CHECK(queue==(OSMesgQueue *)D_80037FE0 && message==0 && flag==0);
    check_state();
    if(send_mode==1)D_80037FA0=ep=0;
    if(send_mode==2)D_80037FA0=ep=4;
    return (int)answer;
}

unsigned int func_8002C5E0(unsigned int arg_command,unsigned int arg_cursor)
{
    ++write_calls;CHECK(index<4);
    cursor+=12U;
    CHECK(arg_command==command && arg_cursor==cursor);
    check_state();
    if(write_mode&1)D_80038028=ec=3;
    if(write_mode&2) {eq=1;D_8003802C=queues[1];}
    if(write_mode&4) {
        D_80037FA0=ep=0;
        D_80037FA8[0].source=requests[0].source=logical[7];
    }
    expected[eq][ec].source=requests[index].source;
    expected[eq][ec].length=requests[index].size;
    expected[eq][ec].destination=requests[index].destination;
    ++index;ec=(unsigned short)(ec+1);command=answer;
    return answer;
}

static void one(int pending,int ack,int seed,int sm,int wm)
{
    static const unsigned int lengths[4]={0,16,0xffffffffU,0x80000000U};
    static const unsigned int answers[3]={0,0xffffffffU,0xabcdef00U};
    static const unsigned short counts[4]={0,1,7,65535};
    int i,j,q,batch,expected_calls;
    ++case_no;send_mode=sm;write_mode=wm;send_calls=write_calls=index=eq=0;
    ep=(unsigned char)pending;ea=(unsigned char)ack;ec=counts[seed%4];
    cursor=(seed%2)?0xfffffffcU:0x12345000U;command=(seed%2)?0x87654321U:0;
    answer=answers[seed%3];
    D_80037FA0=ep;D_80037FD8=ea;D_80038028=ec;D_8003802C=queues[0];
    D_80038038=cursor;D_8003803C=command;
    for(i=0;i<4;i++) {
        requests[i].source=logical[i+4];requests[i].destination=logical[i];
        requests[i].size=lengths[(i+seed)%4];D_80037FA8[i]=requests[i];
    }
    for(q=0;q<2;q++) for(j=0;j<13;j++) {
        i=j==12?65535:j;queues[q][i].length=0x12345678;
        queues[q][i].source=logical[7];queues[q][i].destination=logical[7];
        expected[q][i].length=0x12345678;expected[q][i].source=logical[7];expected[q][i].destination=logical[7];
    }
    batch=ack && sm? (sm==1?0:4):pending;
    expected_calls=batch && (wm&4)?1:batch;
    func_80010450();
    CHECK(write_calls==expected_calls && send_calls==(ack!=0));
    ep=0;ea=(unsigned char)(batch!=0);check_state();
}
int main(void)
{
    static const int acks[3]={0,1,255};
    static const int modes[5]={0,1,2,4,7};
    int p,a,s,m,w;
    for(p=0;p<5;p++)for(a=0;a<3;a++)for(s=0;s<6;s++)for(m=0;m<3;m++)for(w=0;w<5;w++)
        one(p,acks[a],s,m,modes[w]);
    printf("%d actual-C field-oracle cases passed\n",case_no);
    return 0;
}
