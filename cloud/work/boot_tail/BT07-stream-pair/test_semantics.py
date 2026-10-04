#!/usr/bin/env python3
"""Actual-source host behavior checks; no native or cartridge execution."""
from pathlib import Path
from hashlib import sha256
import json, os, subprocess, tempfile
ROOT=Path(__file__).resolve().parents[4]
PACKET=Path(__file__).resolve().parent
COMMON='#define _GNU_SOURCE\n#include <assert.h>\n#include <stddef.h>\n#include <stdlib.h>\n#include <string.h>\n#include <sys/mman.h>\n'
HARNESS={
'80025AB4':r'''
ServiceHooks D_80038000;
volatile unsigned char D_8002D480[1];
unsigned char D_8002D484;
void *D_80058684, *D_80058688;
unsigned int *D_8005868C;
unsigned int D_80058690;
static unsigned int resource[4]={3, 0x1234, 0x5678, 0x9ABC};
static int allocations, transfers, releases, translations, fail_at, mutate;
static void *translation_input, *released[4];
static unsigned int allocation_sizes[3], transfer_sizes[3];
static void *translate(void *input)
{
    translation_input=input; translations++;
    return resource;
}
static void *allocate(unsigned int size, int flags)
{
    allocations++; assert(flags==0);
    allocation_sizes[allocations-1]=size;
    if (mutate && allocations==2) D_80058690=2;
    if (allocations==fail_at) return 0;
    return malloc(size ? size : 1);
}
static void release(void *pointer)
{
    assert(releases<4); released[releases++]=pointer;
    free(pointer);
}
void func_80010628(void *destination, void *source, unsigned int size)
{
    assert(transfers<3);
    transfer_sizes[transfers]=size;
    assert(source==(transfers ? (void *)(resource+1) : (void *)resource));
    memcpy(destination,source,size); transfers++;
}
static void reset(void)
{
    allocations=transfers=releases=translations=fail_at=mutate=0;
    translation_input=0;
    D_80038000.translate=translate;
    D_80038000.allocate=allocate;
    D_80038000.release=release;
    D_8002D480[0]=1; D_8002D484=0;
    D_8005868C=0; D_80058684=0; D_80058688=0; D_80058690=99;
}
int main(void)
{
    unsigned int *borrowed;
    void *old;
    int owns, failure;
    reset(); D_8002D480[0]=0;
    assert(func_80025AB4(0,0)==0);
    assert(!allocations && !releases && !translations && !transfers);
    for (owns=0;owns<2;owns++) for(failure=0;failure<3;failure++) {
        reset(); fail_at=failure;
        old=malloc(8); assert(old); D_8005868C=old; D_8002D484=owns;
        assert(func_80025AB4((void *)0x1000000UL,0)==(failure==0));
        assert(translations==1 && translation_input==(void *)0x1000000UL);
        assert(D_80058684==resource && D_80058688==resource);
        assert(allocations==(failure==1 ? 1 : 2));
        assert(allocation_sizes[0]==4);
        if(failure!=1) assert(allocation_sizes[1]==12 && D_80058690==3);
        assert(transfers==(failure==1 ? 0 : failure==2 ? 1 : 2));
        assert(releases==owns+(failure!=1));
        if(owns) assert(released[0]==old); else free(old);
        if(failure==0) {
            assert(D_8002D484==1 && !memcmp(D_8005868C,resource+1,12));
            assert(transfer_sizes[0]==4 && transfer_sizes[1]==12);
            free(D_8005868C);
        } else if(failure==2) assert(D_8005868C==0 && D_8002D484==owns);
    }
    reset(); mutate=1;
    assert(func_80025AB4((void *)0x1000000UL,0)==1);
    assert(allocation_sizes[1]==12 && transfer_sizes[1]==8 && D_80058690==2);
    free(D_8005868C);
    borrowed=mmap((void *)0x600080000000UL,4096,PROT_READ|PROT_WRITE,
                  MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
    assert(borrowed==(void *)0x600080000000UL);
    borrowed[0]=3; borrowed[1]=7; borrowed[2]=9; borrowed[3]=11;
    reset(); old=malloc(8); assert(old); D_8005868C=old; D_8002D484=1;
    assert(func_80025AB4(borrowed,(void *)0x222UL)==1);
    assert(releases==1 && released[0]==old && !allocations && !transfers);
    assert(translation_input==(void *)0x222UL && translations==1);
    assert(D_80058684==borrowed && D_80058688==resource);
    assert(D_8005868C==borrowed+1 && D_80058690==3 && D_8002D484==0);
    assert(munmap(borrowed,4096)==0);
    return 0;
}
''',
'800252AC':r'''
ServiceHooks D_80038000;
StreamState D_80056230[2];
volatile unsigned char D_8002D480[1];
void *D_80058688;
unsigned int *D_8005868C, D_80058690, D_800586A0;
void *D_80058698[2];
static unsigned int entries[2];
static unsigned char data[512], storage[2][256];
static int calls, selected, use_owned, fail_alloc;
static unsigned int expected_count, expected_rate, expected_index, expected_size;
static unsigned char expected_option, expected_values[3], expected_mode;
static void *allocated;
static void fields(void)
{
    StreamState *s=&D_80056230[selected];
    assert(s->rate==expected_rate && s->buffer_count==expected_count);
    assert(s->option==expected_option && s->mode==expected_mode);
    assert(s->value1==expected_values[0] && s->value2==expected_values[1] && s->value3==expected_values[2]);
}
void osCreateMesgQueue(OSMesgQueue *q,OSMesg *m,int n)
{
    assert(calls++==0 && q==&D_80056230[selected].request_queue);
    assert(m==D_80056230[selected].request_messages && n==2);
}
unsigned int func_8001C770(unsigned int count)
{
    assert(!use_owned && calls++==1 && count==expected_count); fields();
    return count*2+8;
}
static void *allocate(unsigned int bytes,int flags)
{
    assert(!use_owned && calls++==2 && bytes==expected_size && flags==0);
    allocated=fail_alloc ? 0 : (void *)storage[selected]; return allocated;
}
void osInvalDCache(void *p,int bytes)
{
    assert(!use_owned && calls++==3 && p==allocated && (unsigned int)bytes==expected_size);
}
int func_800250F0(void)
{
    assert(calls++==(use_owned?1:4)); fields(); return 19;
}
void func_8002506C(void *source,void *destination,unsigned int count,OSMesgQueue *queue)
{
    (void)source;(void)destination;(void)count;(void)queue; assert(0);
}
void func_80025EB0(StreamState *s,void *source,void *buffer,unsigned int count,
                  void (*callback)(void *,void *,unsigned int,OSMesgQueue *))
{
    assert(calls++==(use_owned?2:5)); fields();
    assert(s==&D_80056230[selected] && source==data+(entries[expected_index]&0xFFFFFF));
    assert(buffer==storage[selected] && count==expected_count && callback==func_8002506C);
}
void func_80024FB0(StreamState *s)
{
    assert(calls++==(use_owned?3:6) && s==&D_80056230[selected]);
}
void func_80025120(int token)
{
    assert(calls++==(use_owned?4:7) && token==19);
    assert(D_80056230[selected].handle==-1 && D_80056230[selected].busy==1);
}
unsigned int func_800251A8(int index)
{
    assert(calls++==(use_owned?5:8) && index==selected); return 0xFFABCD00U+index;
}
static void reset(void)
{
    memset(D_80056230,0xA5,sizeof(D_80056230));
    D_80056230[0].busy=D_80056230[1].busy=0;
    D_8002D480[0]=1; D_80058690=2; D_800586A0=use_owned;
    D_80058688=data; D_8005868C=entries;
    D_80058698[0]=storage[0]; D_80058698[1]=storage[1];
    D_80038000.allocate=allocate; calls=0;fail_alloc=0;
}
int main(void)
{
    unsigned int rates[]={0,1,30,8000,16000,44100,0x80000000U,0xFFFFFFFFU};
    unsigned int rate,i,j;
    StreamState saved;
    int high, value;
    use_owned=0;selected=0;reset();D_8002D480[0]=0;
    assert(func_800252AC(0,0,0,0,0,0,0)==~0U && !calls);
    reset();assert(func_800252AC(2,0,0,0,0,0,0)==~0U && !calls);
    reset();D_80056230[0].busy=1;D_80056230[1].busy=2;
    assert(func_800252AC(0,0,0,0,0,0,0)==~0U && !calls);
    for(use_owned=0;use_owned<2;use_owned++) for(selected=0;selected<2;selected++)
    for(high=0;high<2;high++) for(i=0;i<sizeof(rates)/sizeof(rates[0]);i++) {
        reset(); expected_index=(unsigned int)high;
        entries[expected_index]=(high ? 0x01000000U:0)|32;
        if(selected) D_80056230[0].busy=2;
        saved=D_80056230[1-selected];
        rate=rates[i];expected_rate=rate ? rate : (high?16000U:8000U);
        expected_count=((expected_rate*8U/30U+159U)/160U)*160U;
        expected_size=expected_count*2U+8U;
        expected_option=211;expected_mode=197;
        expected_values[0]=2;expected_values[1]=99;expected_values[2]=255;
        assert(func_800252AC(expected_index,rate,211,2,99,255,197)==0xFFABCD00U+(unsigned int)selected);
        assert(calls==(use_owned?6:9));fields();
        assert(!memcmp(&saved,&D_80056230[1-selected],sizeof(saved)));
    }
    use_owned=1;selected=0;expected_index=0;
    for(value=0;value<256;value++) {
        reset(); entries[0]=32;expected_rate=8000;expected_count=((8000U*8/30+159)/160)*160;
        expected_option=expected_mode=expected_values[0]=expected_values[1]=expected_values[2]=(unsigned char)value;
        assert(func_800252AC(0,8000,(unsigned char)value,(unsigned char)value,(unsigned char)value,(unsigned char)value,(unsigned char)value)==0xFFABCD00U);
        fields(); assert(calls==6);
    }
    for(j=0;j<2;j++) {
        use_owned=(int)j;selected=0;reset(); entries[0]=32;
        expected_rate=8000;expected_count=((8000U*8/30+159)/160)*160;expected_size=expected_count*2+8;
        expected_option=expected_mode=expected_values[0]=expected_values[1]=expected_values[2]=0;
        if(use_owned)D_80058698[0]=0;else fail_alloc=1;
        assert(func_800252AC(0,8000,0,0,0,0,0)==~0U);
        assert(calls==(use_owned?1:4) && !D_80056230[0].busy);fields();
    }
    return 0;
}
'''}
LAYOUT=r'''
#define CHECK(n,e) typedef char n[(e)?1:-1]
CHECK(word,sizeof(unsigned int)==4);
CHECK(queue,sizeof(OSMesgQueue)==24);
CHECK(stream,sizeof(StreamState)==4648);
CHECK(request_queue,offsetof(StreamState,request_queue)==4576);
CHECK(request_messages,offsetof(StreamState,request_messages)==4600);
CHECK(busy,offsetof(StreamState,busy)==4608);
CHECK(option,offsetof(StreamState,option)==4612);
CHECK(rate,offsetof(StreamState,rate)==4616);
CHECK(handle,offsetof(StreamState,handle)==4620);
CHECK(buffer,offsetof(StreamState,buffer)==4624);
CHECK(buffer_count,offsetof(StreamState,buffer_count)==4628);
CHECK(mode,offsetof(StreamState,mode)==4633);
CHECK(processed,offsetof(StreamState,processed)==4636);
CHECK(token,offsetof(StreamState,token)==4644);
CHECK(hooks,sizeof(ServiceHooks)==32);
CHECK(allocate,offsetof(ServiceHooks,allocate)==24);
'''
def run():
    results=[]
    with tempfile.TemporaryDirectory(prefix='bt07-stream-host-') as tmp:
        tmp=Path(tmp)
        for address,body in HARNESS.items():
            source=PACKET/('func_'+address+'.c'); harness=tmp/(address+'.c')
            harness.write_text(COMMON+'#include "'+str(source)+'"\n'+body)
            for mode,flags in [('c89-O2',['-O2']),('c89-asan-ubsan-O1',['-O1','-fsanitize=address,undefined','-fno-omit-frame-pointer'])]:
                exe=tmp/(address+'-'+mode)
                subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-Wno-pointer-to-int-cast']+flags+[str(harness),'-o',str(exe)],check=True,capture_output=True,text=True)
                subprocess.run([str(exe)],check=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'),capture_output=True,text=True)
            results.append({'name':'func_'+address,'source_sha256':sha256(source.read_bytes()).hexdigest(),'result':'PASS','modes':['c89-O2','c89-asan-ubsan-O1']})
        layout=tmp/'layout.c';layout.write_text('#include <stddef.h>\n#include "'+str(PACKET/'func_800252AC.c')+'"\n'+LAYOUT)
        subprocess.run(['cc','-m32','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-fsyntax-only',str(layout)],check=True,capture_output=True,text=True)
    return {'result':'PASS','32bit_c89_layout':'PASS','cases':334,'results':results,'limits':'Source behavior only; no native/ROM execution. ASan/UBSan enabled; LeakSanitizer disabled under ptrace. Native layout is checked separately from wider host pointer layout.'}
if __name__=='__main__':
    try:
        print(json.dumps(run(),indent=2))
    except subprocess.CalledProcessError as exc:
        print(exc.stdout or '')
        print(exc.stderr or '')
        raise
