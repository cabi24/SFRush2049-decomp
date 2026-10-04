#!/usr/bin/env python3
"""Build actual matching C with C89 layout assertions and ASan/UBSan test stubs."""
from pathlib import Path
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
SOURCES = [ROOT/'cloud/matches/boot_tail'/('func_'+a+'.c')
           for a in ('800259A8','80025C68')]
STREAM = SOURCES[0].read_text().split('extern StreamState')[0]
HOOKS = SOURCES[1].read_text().split('typedef struct ServiceHooks')[1].split('extern ServiceHooks')[0]
TYPES = STREAM + 'typedef struct ServiceHooks' + HOOKS
LAYOUT = r'''
#include <stddef.h>
#define CHECK(n,e) typedef char n[(e)?1:-1]
CHECK(stream_size,sizeof(StreamState)==4648);
CHECK(state_offset,offsetof(StreamState,state)==4573);
CHECK(scale_offset,offsetof(StreamState,scale)==4574);
CHECK(busy_offset,offsetof(StreamState,busy)==4608);
CHECK(rate_offset,offsetof(StreamState,rate)==4616);
CHECK(handle_offset,offsetof(StreamState,handle)==4620);
CHECK(buffer_offset,offsetof(StreamState,buffer)==4624);
CHECK(processed_offset,offsetof(StreamState,processed)==4636);
CHECK(float_offset,offsetof(StreamState,field_1220)==4640);
CHECK(token_offset,offsetof(StreamState,token)==4644);
CHECK(hooks_size,sizeof(ServiceHooks)==32);
CHECK(allocate_offset,offsetof(ServiceHooks,allocate)==24);
CHECK(release_offset,offsetof(ServiceHooks,release)==28);
'''
HARNESS = r'''
#include <assert.h>
#include <string.h>
#include <stdio.h>
typedef struct OSMesgQueue OSMesgQueue;
StreamState D_80056230[2];
ServiceHooks D_80038000;
volatile unsigned char D_8002D480[1];
unsigned int D_800586A0, D_80058680;
void *D_8005868C, *D_80058698[2];
void (*D_80038024)(void);
static int lookup_result, lookup_calls, lock_calls, unlock_calls;
static int init_calls, adjust_calls, queue_calls, size_calls, alloc_calls, inval_calls;
static int selected, null_mode, events[32], event_count;
static unsigned int lookup_token, expected_flags;
static int buffers[2];
static void event(int n) { events[event_count++]=n; }
float func_800259A8(unsigned int, float *);
void func_80025C68(unsigned int);
void func_8002574C(void) { assert(0); }
static void previous_callback(void) { assert(0); }
int func_80025264(unsigned int token)
{ assert(token==lookup_token); lookup_calls++; event(1); return lookup_result; }
int func_800250F0(void)
{ lock_calls++; event(2); return 37; }
void func_80025120(int token)
{ assert(token==37); unlock_calls++; event(3); }
void func_80025EB0(StreamState *stream, void *a, void *b, unsigned int c,
                  void (*callback)(void *,void *,unsigned int,OSMesgQueue *))
{
    assert(stream==&D_80056230[init_calls]);
    assert(a==0 && b==0 && c==0 && callback==0);
    assert(D_800586A0==expected_flags && D_8005868C==0);
    assert(D_8002D480[0]==0 && D_80038024==previous_callback);
    stream->state=1; stream->scale=2; stream->busy=7;
    event(10+init_calls*2); init_calls++;
}
void func_80024FB0(StreamState *stream)
{
    assert(stream==&D_80056230[adjust_calls]);
    assert(stream->state==1 && stream->scale==2 && stream->busy==7);
    stream->scale*=2; event(11+adjust_calls*2); adjust_calls++;
}
void func_800250AC(void)
{
    assert(init_calls==2 && adjust_calls==2);
    assert(D_80056230[0].busy==0 && D_80056230[1].busy==0);
    queue_calls++; event(20);
}
unsigned int func_8001C770(unsigned int count)
{ assert(count==4320); size_calls++; event(21); return count*2+8; }
static void *allocate_buffer(unsigned int size,int flag)
{
    void *result;
    assert(size==8648 && flag==0 && alloc_calls<2);
    assert(D_8002D480[0]==0 && D_80038024==previous_callback);
    result=(null_mode==alloc_calls+1)?0:&buffers[alloc_calls];
    event(22+alloc_calls*2); alloc_calls++; return result;
}
void osInvalDCache(void *pointer,int size)
{
    void *expected=(null_mode==inval_calls+1)?0:&buffers[inval_calls];
    assert(size==8648 && pointer==expected && D_80058698[inval_calls]==pointer);
    event(23+inval_calls*2); inval_calls++;
}
static void reset(void)
{
    memset(D_80056230,0,sizeof(D_80056230));
    lookup_result=selected=lookup_calls=lock_calls=unlock_calls=0;
    init_calls=adjust_calls=queue_calls=size_calls=alloc_calls=inval_calls=event_count=0;
    lookup_token=123; D_8002D480[0]=1;
    D_80038000.allocate=allocate_buffer;
}
static int same_float(float a,float b)
{
    unsigned int x,y;
    if (a!=a && b!=b) return 1;
    memcpy(&x,&a,4); memcpy(&y,&b,4); return x==y;
}
int main(void)
{
    unsigned int cases=0,a,b,f;
    unsigned int values[7]={0,1,16777217,0x7fffffffU,0x80000000U,0xfffffffeU,0xffffffffU};
    unsigned int flags[6]={0,1,2,3,0x80000000U,0xffffffffU};
    int index,busy,mode,i;
    float out,result,expected;
    StreamState saved[2];
    for (mode=0;mode<3;mode++) {
        reset();
        if (mode==0) lookup_token=(unsigned int)-1;
        if (mode==1) D_8002D480[0]=0;
        if (mode==2) lookup_result=-1;
        result=func_800259A8(lookup_token,0);
        assert(same_float(result,0.0f));
        assert(lookup_calls==(mode==2) && lock_calls==0 && unlock_calls==0);
        cases++;
    }
    for(index=0;index<2;index++) for(busy=0;busy<256;busy++) {
        reset(); lookup_result=index;
        D_80056230[index].busy=(unsigned char)busy;
        D_80056230[index].rate=16; D_80056230[index].processed=24;
        D_80056230[index].field_1220=-7.25f;
        memcpy(saved,D_80056230,sizeof(saved)); out=99.0f;
        result=func_800259A8(lookup_token,&out);
        assert(same_float(result,busy==2?1.5f:0.0f));
        assert(same_float(out,-7.25f));
        assert(lookup_calls==1 && lock_calls==1 && unlock_calls==1);
        assert(event_count==3 && events[0]==1 && events[1]==2 && events[2]==3);
        assert(memcmp(saved,D_80056230,sizeof(saved))==0); cases++;
    }
    for(index=0;index<2;index++) for(a=0;a<7;a++) for(b=0;b<7;b++) {
        reset(); lookup_result=index; D_80056230[index].busy=2;
        D_80056230[index].processed=values[a]; D_80056230[index].rate=values[b];
        D_80056230[index].field_1220=0.5f;
        result=func_800259A8(lookup_token,&out);
        expected=(float)values[a]/(float)values[b];
        assert(same_float(result,expected) && same_float(out,0.5f));
        assert(lock_calls==1 && unlock_calls==1); cases++;
    }
    for(f=0;f<6;f++) for(mode=0;mode<3;mode++) {
        reset(); expected_flags=flags[f]; null_mode=mode;
        D_8002D480[0]=0; D_80038024=previous_callback;
        D_80058680=12345; D_8005868C=&buffers[0];
        D_80058698[0]=&buffers[1]; D_80058698[1]=&buffers[0];
        memset(D_80056230,0xa5,sizeof(D_80056230));
        memcpy(saved,D_80056230,sizeof(saved));
        for(i=0;i<2;i++) { saved[i].state=1; saved[i].scale=4; saved[i].busy=0; }
        func_80025C68(expected_flags);
        assert(memcmp(saved,D_80056230,sizeof(saved))==0);
        assert(D_800586A0==expected_flags && D_8005868C==0 && D_80058680==0);
        assert(D_80038024==func_8002574C && D_8002D480[0]==1);
        assert(init_calls==2 && adjust_calls==2 && queue_calls==1);
        assert(events[0]==10 && events[1]==11 && events[2]==12 && events[3]==13 && events[4]==20);
        if (expected_flags&1) {
            assert(size_calls==1 && alloc_calls==2 && inval_calls==2 && event_count==10);
            for(i=5;i<10;i++) assert(events[i]==16+i);
        } else {
            assert(size_calls==0 && alloc_calls==0 && inval_calls==0 && event_count==5);
            assert(D_80058698[0]==&buffers[1] && D_80058698[1]==&buffers[0]);
        }
        cases++;
    }
    printf("{\"result\":\"PASS\",\"cases\":%u}\n",cases);
    return 0;
}
'''

def run():
    with tempfile.TemporaryDirectory(prefix='bt07-larger-test-') as tmp:
        tmp=Path(tmp)
        for source in SOURCES:
            stream=source.read_text().split('extern StreamState')[0]
            assert stream==STREAM
        layout=tmp/'layout.c'; layout.write_text(TYPES+LAYOUT)
        subprocess.run(['cc','-m32','-std=c89','-pedantic-errors','-Werror','-fsyntax-only',str(layout)],check=True)
        harness=tmp/'harness.c'; harness.write_text(TYPES+HARNESS)
        executable=tmp/'test'
        subprocess.run(['cc','-std=c89','-pedantic-errors','-O1','-g','-fsanitize=address,undefined',
                        '-fno-omit-frame-pointer','-fno-pie','-no-pie',str(harness),
                        *map(str,SOURCES),'-o',str(executable)],check=True)
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
        result=subprocess.run([str(executable)],env=env,capture_output=True,text=True,check=True)
        receipt=json.loads(result.stdout)
        receipt.update(c89_layout='PASS (32-bit size and field offsets)',sanitizers='ASan and UBSan',
                       scope='Actual source bodies with host test-only callee stubs; no ROM or target runtime')
        return receipt

if __name__=='__main__':
    print(json.dumps(run(),indent=2))
