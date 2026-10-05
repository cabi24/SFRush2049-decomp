/* Uses the same C record on the host; target offsets are checked separately. */
#include <assert.h>
#include <string.h>
#include "stream_contract.h"
typedef int (*SampleCallback_8002574C)(short *, unsigned int, short *, unsigned int, unsigned int);
typedef struct ServiceHooks_8002574C { unsigned char unknown_00[28]; void (*release)(void *); } ServiceHooks_8002574C;
extern void func_800254D4(int);
extern int func_80025670(unsigned int, unsigned char, unsigned char, unsigned char, unsigned char);
extern void func_8002574C(void);
extern float func_800259A8(unsigned int, float *);
StreamState_80025264 D_80056230[2];
volatile unsigned char D_8002D480[1];
unsigned int D_800586A0;
ServiceHooks_8002574C D_80038000;
void (*D_8003801C)(void *);
static short buffer[16];
static int acquired, released, freed, stopped, reset, serviced, started, changed, locked, unlocked;
static int lookup, next_handle;
static unsigned expected_index;
static unsigned cases;
int func_800250F0(void) { ++acquired; return 73; }
void func_80025120(int value) { assert(value==73); ++released; }
int func_80025264(unsigned int token) { assert(token==47); return lookup; }
void func_80025150(void) { ++locked; }
void func_8002517C(void) { ++unlocked; }
void func_80026348(StreamState_80025264 *stream) { assert(stream==&D_80056230[expected_index]); ++reset; stream->state=4; }
void func_80026328(StreamState_80025264 *stream) { assert(stream==&D_80056230[expected_index]); stream->state=3; }
void func_80025F74(StreamState_80025264 *stream) { assert(stream==&D_80056230[expected_index]); ++serviced; }
void func_8001C77C(int handle, unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    assert(handle==89 && a==2 && b==3 && c==4 && d==5); ++changed;
}
void func_8001C7F4(int handle) { assert(handle==89); ++stopped; }
int func_80024FD4(short *a, unsigned int b, short *c, unsigned int d, unsigned int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return 0;
}
int func_8001C580(unsigned char option, short *data, unsigned int count, unsigned int rate,
                 unsigned char volume, unsigned char pan, unsigned char span, unsigned char mode,
                 SampleCallback_8002574C callback, unsigned int selected) {
    assert(option==6 && data==buffer && count==16 && rate==400);
    assert(volume==7 && pan==8 && span==9 && mode==10);
    assert(callback==func_80024FD4 && selected==expected_index); ++started; return next_handle;
}
static void release_buffer(void *data) { assert(data==buffer); ++freed; }
static StreamState_80025264 *fresh(unsigned index) {
    StreamState_80025264 *stream;
    ++cases;
    memset(D_80056230,0,sizeof(D_80056230));
    D_8002D480[0]=1; D_800586A0=0;
    expected_index=index; stream=&D_80056230[index];
    stream->buffer=buffer; stream->buffer_count=16; stream->rate=400;
    stream->handle=89; stream->read_count=5;
    stream->option=6; stream->value1=7; stream->value2=8; stream->value3=9; stream->mode=10;
    acquired=released=freed=stopped=reset=serviced=started=changed=locked=unlocked=0;
    lookup=index; next_handle=89;
    return stream;
}
int main(void) {
    unsigned index, flag;
    StreamState_80025264 *stream;
    float value, ratio;
    D_80038000.release=release_buffer; D_8003801C=release_buffer;
    for (index=0; index<2; ++index) {
        stream=fresh(index); func_800254D4(index); assert(acquired==0 && freed==0 && stream->busy==0);
        for (flag=0; flag<2; ++flag) {
            stream=fresh(index); stream->busy=1; D_800586A0=flag;
            func_800254D4(index); assert(stream->busy==0 && freed==(int)!flag && acquired==0);
        }
        stream=fresh(index); stream->busy=2;
        func_800254D4(index); assert(acquired==1 && released==1 && reset==1 && stream->state==4 && stream->busy==3 && stream->request_state==4);
        stream=fresh(index); stream->busy=3; func_800254D4(index); assert(acquired==0 && freed==0 && stream->busy==3);
        stream=fresh(index); D_8002D480[0]=0;
        assert(func_80025670(47,2,3,4,5)==0 && acquired==0);
        stream=fresh(index); lookup=-1;
        assert(func_80025670(47,2,3,4,5)==0 && acquired==0);
        for (flag=0; flag<2; ++flag) {
            stream=fresh(index); stream->handle=flag ? 89 : -1;
            assert(func_80025670(47,2,3,4,5)==1 && acquired==1 && released==1 && changed==(int)flag);
            assert(stream->value1==2 && stream->value2==3 && stream->value3==4);
        }
        stream=fresh(index); D_8002D480[0]=0; func_8002574C(); assert(locked==0 && unlocked==0);
        stream=fresh(index); func_8002574C(); assert(locked==1 && unlocked==1 && serviced==0);
        stream=fresh(index); stream->busy=1; stream->state=-1;
        func_8002574C(); assert(serviced==1 && started==0 && stream->busy==1);
        stream=fresh(index); stream->busy=1; stream->state=2;
        func_8002574C(); assert(started==1 && stream->busy==2 && stream->state==3 && stream->handle==89 && stream->field_1220==2.0f && stream->processed==0);
        for (flag=0; flag<2; ++flag) {
            stream=fresh(index); stream->busy=1; stream->state=2; next_handle=-1; D_800586A0=flag;
            func_8002574C(); assert(started==1 && reset==1 && stream->busy==0 && freed==(int)!flag);
        }
        stream=fresh(index); stream->busy=2; stream->state=4;
        func_8002574C(); assert(stream->busy==3 && stream->request_state==4);
        for (flag=0; flag<2; ++flag) {
            stream=fresh(index); stream->busy=3; stream->request_state=0; D_800586A0=flag;
            func_8002574C(); assert(stream->busy==0 && freed==(int)!flag && stopped==0);
        }
        stream=fresh(index); stream->busy=3; stream->request_state=2;
        func_8002574C(); assert(stopped==1 && stream->handle==-1 && stream->request_state==1);
        stream=fresh(index); stream->busy=3; stream->request_state=1;
        func_8002574C(); assert(stopped==0 && stream->request_state==0);
        stream=fresh(index); value=-11;
        assert(func_800259A8((unsigned)-1,&value)==0 && value==-11 && acquired==0);
        stream=fresh(index); value=-11; D_8002D480[0]=0;
        assert(func_800259A8(47,&value)==0 && value==-11 && acquired==0);
        stream=fresh(index); value=-11; lookup=-1;
        assert(func_800259A8(47,&value)==0 && value==-11 && acquired==0);
        for (flag=0; flag<2; ++flag) {
            stream=fresh(index); stream->busy=flag ? 2 : 3; stream->processed=100; stream->field_1220=2.0f;
            ratio=func_800259A8(47,&value);
            assert(ratio==(flag ? 0.25f : 0.0f) && value==2.0f && acquired==1 && released==1);
        }
    }
    assert(cases==50);
    return 0;
}
