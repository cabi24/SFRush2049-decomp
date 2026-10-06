#include <assert.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifndef CANDIDATE_PATH
#define CANDIDATE_PATH "../../../matches/ovl_b/func_80393518.c"
#endif
#include CANDIDATE_PATH
s16 D_80151AD0;
Player952 player_array[4];
TextPosition D_803941D0[4][4];
char D_80394AB0[] = "%d";
typedef struct Event { int type, a, b; char text[12]; } Event;
static Event events[40];
static int used, scenario, draws;
static void event(int type, int a, int b, const char *text)
{
    Event *e;
    assert(used < 40);
    e = &events[used++];
    memset(e, 0, sizeof(*e)); e->type=type; e->a=a; e->b=b;
    if (text) { assert(strlen(text)<sizeof(e->text)); strcpy(e->text,text); }
}
void render_helper(float f) { assert(f==0.0f || f==-1.0f); event(1,(int)f,0,0); }
int object_create(int f) { event(2,f,0,0); return 123; }
void func_800B669C(unsigned int a,unsigned int b) { event(3,(int)a,(int)b,0); }
void fcvt_wrapper(char *buf,char *fmt,...)
{
    int n; va_list ap;
    assert(fmt==D_80394AB0 && strcmp(fmt,"%d")==0);
    va_start(ap,fmt); n=va_arg(ap,int); va_end(ap);
    assert(n>=0 && n<=127); sprintf(buf,"%d",n); event(4,n,0,buf);
    if(scenario==1) { D_80151AD0=0; D_803941D0[1][0].x=999; player_array[0].lives=99; }
}
void dispatch_handler(int color) { event(5,color,0,0); }
void state_utility(s16 x,s16 y,char *text)
{
    event(6,x,y,text); draws++;
    if(scenario==2 && draws==2) {
        D_80151AD0=4; player_array[1].kind=8; player_array[2].lives=-1; player_array[3].lives=127;
    }
}
static void reset(int count)
{
    int row,i; used=0; scenario=0; draws=0; memset(events,0,sizeof(events));
    memset(player_array,0,sizeof(player_array)); D_80151AD0=(s16)count;
    for(row=0;row<4;row++) for(i=0;i<4;i++) {
        D_803941D0[row][i].x=(s16)(row*100+i*10);
        D_803941D0[row][i].y=(s16)(row*100+i*10+2);
    }
}
static void eq(int k,int t,int a,int b,const char *s)
{
    assert(k<used && events[k].type==t && events[k].a==a && events[k].b==b);
    if(s) assert(strcmp(events[k].text,s)==0);
}
static void finish(int n) { eq(n,3,0,3,0); eq(n+1,1,-1,0,0); assert(used==n+2); }
int main(void)
{
    int c,v,i,k,fixtures=0; char expected[12]; Player952 original[4];
    assert(sizeof(Player952)==952 && offsetof(Player952,kind)==0x384 && offsetof(Player952,lives)==0x385);
    assert(sizeof(TextPosition)==4 && offsetof(TextPosition,y)==2);
    for(c=-2;c<=4;c++) for(v=-128;v<=127;v++) {
        reset(c);
        for(i=0;i<4;i++) { player_array[i].kind=(s8)(i==1?8:-1); player_array[i].lives=(s8)v; }
        memcpy(original,player_array,sizeof(original));
        assert(func_80393518((void*)0)==1); eq(0,1,0,0,0); eq(1,2,10,0,0); eq(2,3,1,1,0); k=3;
        if(c>0) for(i=0;i<c;i++) if(i!=1 && v>=0) {
            sprintf(expected,"%d",v);
            eq(k++,4,v,0,expected); eq(k++,5,0,0,0);
            eq(k++,6,(c-1)*100+i*10+1,(c-1)*100+i*10+3,expected);
            eq(k++,5,1,0,0); eq(k++,6,(c-1)*100+i*10,(c-1)*100+i*10+2,expected);
        }
        finish(k); assert(memcmp(original,player_array,sizeof(original))==0); fixtures++;
    }
    reset(-32768); assert(func_80393518((void*)1)==1); finish(3); fixtures++;
    reset(2); scenario=1; player_array[0].lives=7;
    assert(func_80393518((void*)1)==1); eq(5,6,101,103,"7"); eq(7,6,100,102,"7"); finish(8); fixtures++;
    reset(1); scenario=2; player_array[0].lives=0;
    assert(func_80393518((void*)1)==1); eq(5,6,1,3,"0"); eq(7,6,0,2,"0");
    eq(8,4,127,0,"127"); eq(10,6,331,333,"127"); eq(12,6,330,332,"127"); finish(13); fixtures++;
    reset(1); player_array[0].lives=127; D_803941D0[0][0].x=32767; D_803941D0[0][0].y=-32768;
    assert(func_80393518((void*)1)==1); eq(5,6,-32768,-32767,"127"); eq(7,6,32767,-32768,"127"); finish(8); fixtures++;
    printf("%d host fixtures passed\n",fixtures); return 0;
}
