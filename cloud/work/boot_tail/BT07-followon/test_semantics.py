#!/usr/bin/env python3
"""Compile actual packet bodies into a temporary C89 ASan/UBSan behavior harness."""
import json
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
MATCHES = ('8002506C', '800254D4', '80025594', '80025670', '80025DC0')
STRUCT = (ROOT/'cloud/matches/boot_tail/func_800254D4.c').read_text().split('extern StreamState')[0]
HOOKS = (ROOT/'cloud/matches/boot_tail/func_80025DC0.c').read_text().split('extern ServiceHooks')[0]
LAYOUT = r'''
#include <stddef.h>
#define CHECK(n,e) typedef char n[(e)?1:-1]
CHECK(stream_size, sizeof(StreamState)==4648);
CHECK(busy_offset, offsetof(StreamState,busy)==4608);
CHECK(value1_offset, offsetof(StreamState,value1)==4609);
CHECK(value2_offset, offsetof(StreamState,value2)==4610);
CHECK(value3_offset, offsetof(StreamState,value3)==4611);
CHECK(handle_offset, offsetof(StreamState,handle)==4620);
CHECK(buffer_offset, offsetof(StreamState,buffer)==4624);
CHECK(request_offset, offsetof(StreamState,request_state)==4632);
CHECK(token_offset, offsetof(StreamState,token)==4644);
CHECK(hooks_size, sizeof(ServiceHooks)==32);
CHECK(allocate_offset, offsetof(ServiceHooks,allocate)==24);
CHECK(release_offset, offsetof(ServiceHooks,release)==28);
'''
HARNESS = r'''
#include <assert.h>
#include <string.h>
#include <stdio.h>
typedef struct OSMesgQueue_s { int marker; } OSMesgQueue;
StreamState D_80056230[2];
ServiceHooks D_80038000;
volatile unsigned char D_8002D480[1];
unsigned char D_8002D484;
void (*D_80038024)(void);
void *D_8005868C;
void *D_80058698[2];
unsigned int D_800586A0;
static unsigned int lookup_token;
static int lookup_result, lookup_calls, queue_calls, unlock_calls, handle_calls;
static int gate_calls, wait_calls, reset_calls, release_calls, events[64], event_count;
static void *released[16];
static unsigned char values[4];
static int expected_handle;
static void *copy_source, *copy_destination;
static unsigned int copy_count;
static OSMesgQueue *expected_queue;
static int scratch[8];
static void event(int value) { events[event_count++] = value; }
static void release_buffer(void *buffer) { released[release_calls++] = buffer; event(20); }
void (*D_8003801C)(void *) = release_buffer;
int func_80025264(unsigned int token) { assert(token==lookup_token); lookup_calls++; return lookup_result; }
int func_800250F0(void) { queue_calls++; event(1); return 0; }
void func_80025120(int token) { assert(token==0); unlock_calls++; event(3); }
void func_80026348(StreamState *stream) { assert(stream==&D_80056230[0] || stream==&D_80056230[1]); stream->state=4; event(2); }
void func_8001C77C(int handle, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{ assert(handle==expected_handle); assert(a==values[0] && b==values[1] && c==values[2] && d==values[3]); handle_calls++; event(4); }
void func_80025D84(void) { wait_calls++; event(10); }
void func_80014594(void) { assert(D_8002D480[0]==0); gate_calls++; event(11); }
void func_8001061C(void) { assert(D_80038024==0); reset_calls++; event(12); }
void func_800145DC(void) { gate_calls--; event(13); }
void func_80010714(void *destination, void *source, unsigned int count)
{ assert(destination==copy_destination && source==copy_source && count==copy_count); event(30); }
int osJamMesg(OSMesgQueue *queue, void *message, int flag)
{ assert(queue==expected_queue && message==0 && flag==1); event(31); return -7; }
void func_8002506C(void *, void *, unsigned int, OSMesgQueue *);
void func_800254D4(int);
int func_80025594(unsigned int);
int func_80025670(unsigned int, unsigned char, unsigned char, unsigned char, unsigned char);
void func_80025DC0(void);
static void unused_callback(void) { assert(0); }
static void reset(void)
{
    memset(D_80056230,0,sizeof(D_80056230));
    lookup_calls=queue_calls=unlock_calls=handle_calls=0;
    wait_calls=gate_calls=reset_calls=release_calls=event_count=0;
    D_8002D480[0]=1; D_800586A0=0; D_8002D484=0;
    D_80038000.release=release_buffer; D_80038024=unused_callback;
    D_8005868C=0; D_80058698[0]=&scratch[2]; D_80058698[1]=&scratch[3];
    lookup_token=123; lookup_result=0;
}
int main(void)
{
    int selected,busy,flags,enabled,result,handle,v,owned,present;
    unsigned int cases=0, counts[3]={0,1,4096};
    unsigned char samples[5]={0,1,127,128,255};
    StreamState expected[2];
    OSMesgQueue queue;
    for (v=0; v<3; v++) {
        reset(); copy_source=&scratch[0]; copy_destination=&scratch[1]; copy_count=counts[v]; expected_queue=&queue;
        func_8002506C(copy_source,copy_destination,copy_count,&queue);
        assert(event_count==2 && events[0]==30 && events[1]==31); cases++;
    }
    for (selected=0; selected<2; selected++) for (busy=0; busy<256; busy++) for (flags=0; flags<4; flags++) {
        reset(); D_800586A0=flags; D_80056230[selected].busy=busy; D_80056230[selected].buffer=&scratch[selected];
        memcpy(expected,D_80056230,sizeof(expected));
        func_800254D4(selected);
        if (busy==1) {
            expected[selected].busy=0;
            assert(release_calls==(!(flags&1)));
            if (release_calls) assert(released[0]==&scratch[selected]);
        } else if (busy==2) {
            expected[selected].busy=3; expected[selected].state=4; expected[selected].request_state=4;
            assert(queue_calls==1 && unlock_calls==1 && event_count==3);
            assert(events[0]==1 && events[1]==2 && events[2]==3);
        } else assert(event_count==0);
        assert(memcmp(expected,D_80056230,sizeof(expected))==0); cases++;
    }
    for (enabled=0; enabled<2; enabled++) for (selected=-1; selected<2; selected++) for (v=0; v<2; v++) {
        reset(); D_8002D480[0]=enabled; lookup_result=selected; lookup_token=v?0xffffffffU:123;
        D_80056230[0].busy=D_80056230[1].busy=1;
        result=func_80025594(lookup_token);
        assert(result==(enabled && !v && selected!=-1));
        assert(lookup_calls==(enabled && !v));
        assert(release_calls==result); cases++;
    }
    for (enabled=0; enabled<2; enabled++) for (selected=-1; selected<2; selected++) for (handle=-1; handle<2; handle++) for (v=0; v<5; v++) {
        reset(); D_8002D480[0]=enabled; lookup_result=selected; expected_handle=handle;
        values[0]=samples[v]; values[1]=samples[(v+1)%5]; values[2]=samples[(v+2)%5]; values[3]=samples[(v+3)%5];
        if (selected>=0) D_80056230[selected].handle=handle;
        memcpy(expected,D_80056230,sizeof(expected));
        result=func_80025670(123,values[0],values[1],values[2],values[3]);
        assert(result==(enabled && selected!=-1)); assert(lookup_calls==enabled);
        assert(queue_calls==result && unlock_calls==result);
        assert(handle_calls==(result && handle!=-1));
        if (result) { expected[selected].value1=values[0]; expected[selected].value2=values[1]; expected[selected].value3=values[2]; }
        assert(memcmp(expected,D_80056230,sizeof(expected))==0); cases++;
    }
    for (flags=0; flags<4; flags++) for (owned=0; owned<2; owned++) for (present=0; present<2; present++) {
        reset(); D_800586A0=flags; D_8002D484=owned; D_8005868C=present?&scratch[4]:0;
        D_80056230[0].busy=D_80056230[1].busy=1;
        D_80056230[0].buffer=&scratch[0]; D_80056230[1].buffer=&scratch[1];
        func_80025DC0();
        assert(wait_calls==1 && reset_calls==1 && gate_calls==0 && D_8002D480[0]==0 && D_80038024==0);
        assert(D_80056230[0].busy==0 && D_80056230[1].busy==0);
        v=(flags&1)?0:2;
        assert(events[v]==10 && events[v+1]==11 && events[v+2]==12 && events[v+3]==13);
        v=0; if (!(flags&1)) { assert(released[v++]==&scratch[0]); assert(released[v++]==&scratch[1]); }
        if (present && owned) assert(released[v++]==&scratch[4]);
        if (flags&1) { assert(released[v++]==&scratch[2]); assert(released[v++]==&scratch[3]); }
        assert(release_calls==v); cases++;
    }
    printf("%u behavior cases passed\n",cases);
    return 0;
}
'''

def run():
    with tempfile.TemporaryDirectory(prefix='bt07-followon-host-') as temporary:
        d=Path(temporary)
        layout=d/'layout.c'; layout.write_text(STRUCT+HOOKS+LAYOUT)
        subprocess.run(['gcc','-m32','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-c',str(layout),'-o',str(d/'layout.o')],check=True)
        harness=d/'harness.c'; harness.write_text(STRUCT+HOOKS+HARNESS)
        sources=[str(ROOT/'cloud/matches/boot_tail'/('func_'+a+'.c')) for a in MATCHES]
        command=['gcc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-O1','-g','-fno-omit-frame-pointer','-fsanitize=address,undefined',str(harness),*sources,'-o',str(d/'host')]
        subprocess.run(command,check=True)
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')
        result=subprocess.run([str(d/'host')],check=True,capture_output=True,text=True,env=env)
        return {'result':'PASS','layout':'32-bit C89 compile-only assertions', 'behavior':'native-width actual C sources with ASan/UBSan', 'stdout':result.stdout.strip(), 'rom_or_target_execution':False, 'leak_sanitizer':'disabled under ptrace'}

if __name__=='__main__':
    print(json.dumps(run(),indent=2))
