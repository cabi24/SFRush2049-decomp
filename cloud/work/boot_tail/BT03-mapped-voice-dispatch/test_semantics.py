#!/usr/bin/env python3
"""Source-bound native/candidate MIPS replay and host-C sanitizer checks.
The 32-entry C array accepts direct selectors 0..31 and grouped selectors250..255.
Other selectors are native unchecked indexing and excluded from the C domain.
"""
import ctypes
import hashlib
import json
import os
from pathlib import Path
import random
import struct
import subprocess
import tempfile
from verify import ROOT,P,SOURCE,FLAGS,BASE,TABLE,compile_link,score,unpack,digest
from native_replay import execute,signed
GLOBAL=0x8004F300
MASK=0xFFFFFFFF

def put(b,o,n,v):b[o:o+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
def get(b,o,n=4):return int.from_bytes(b[o:o+n],'big')
def fixture(seed):
    r=random.Random(seed);b=bytearray(r.randbytes(1280))
    values=[0,1,0x7F0000,0xFF0000,0xFFFFFFFF,0x80000000,0x7FFFFFFF]
    for i in range(32):
        put(b,i*40,4,values[(i+seed)%len(values)])
        b[i*40+20]=[0,1,2,3,4,255][(i+seed)%6]
    return bytes(b)
def truncdiv(n,d):return (abs(n)//d)*(-1 if n<0 else 1)
def oracle(before,args,mutation=False):
    volume,time,group,mode,seq=args
    time=time or 1;b=bytearray(before)
    if mutation:
        put(b,0,4,0xFEDCBA98);b[20]=2
    indices=range(32) if group>=250 else [group]
    types={250:[2],251:[3],252:[2,3],253:[0],254:[1],255:[0,1]}
    target=volume<<16
    for i in indices:
        o=i*40
        if group>=250 and b[o+20] not in types[group]:continue
        delta=truncdiv(signed((target-get(b,o))&MASK),time)
        put(b,o+4,4,target);put(b,o+8,4,delta);put(b,o+12,4,time*256)
        put(b,o+16,4,MASK if group>=250 else seq)
        if group<250:b[o+21]=mode
    return bytes(b)

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail'
    targets=score.targets();native=targets['func_8001B9F8']
    proof=json.loads((P/'table_receipt.json').read_text())
    native_table=b''.join(int(x['target'],16).to_bytes(4,'big') for x in proof['selector_to_target'])
    assert hashlib.sha256(native_table).hexdigest()==proof['table_sha256']
    with tempfile.TemporaryDirectory(prefix='voice-semantics-') as tmp:
        tmp=Path(tmp)
        obj,body,table,rels=compile_link(SOURCE,FLAGS,tmp)
        candidate=unpack(body)
        cases=[]
        groups=list(range(32))+list(range(250,256))
        for group in groups:
            for volume in (0,1,64,127,128,255):
                for time in (0,1,2,255,256,65535):
                    for seed in (0,5):
                        cases.append((seed,(volume,time,group,(volume+time)&255,0xFEDCBA98 if seed else 0),False))
        for group in range(250,256):
            for volume in range(256):cases.append((7,(volume,7,group,255,0x12345678),False))
        for group in groups:cases.append((11,(31,0,group,0xA5,0xFFFFFFFF),True))
        steps=0;executions=0;coverage=[set(),set()]
        for seed,args,mutation in cases:
            before=fixture(seed);expected=oracle(before,args,mutation)
            for which,(words,tab) in enumerate(((native,native_table),(candidate,table))):
                calls=[]
                def helper(address,arguments,mem,ordinal):
                    assert address==0x8001E930 and ordinal==0
                    value=mem(arguments[0],4);assert value==(args[1] or 1)
                    mem(arguments[0],4,value*256)
                    if mutation:mem(GLOBAL,4,0xFEDCBA98);mem(GLOBAL+20,1,2)
                    return 0xBAD00002,{'callee':'func_8001E930','input_time':value}
                # Deliberately dirty high bits on narrow O32 inputs; the callee masks.
                polluted=(args[0]|0xC0DE0000,args[1]|0xBEEF0000,args[2]|0xFACE0000,args[3]|0xDEAD0000,args[4])
                result=execute(words,{GLOBAL:before,TABLE:tab},helper,polluted)
                assert result['regions'][GLOBAL]==expected,(args,seed,mutation)
                assert result['regions'][TABLE]==tab
                assert len(result['calls'])==1 and not result['uninitialized_stack_reads']
                steps+=result['steps'];executions+=1;coverage[which].update(result['visited_offsets'])
        # Host execution is an independent compiler, with real C89 source and ASan/UBSan.
        harness=tmp/'host.c'
        harness.write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include <limits.h>\n#include "'+str(SOURCE)+'"\n'+HOST)
        host=tmp/'host'
        cmd=['cc','-std=c89','-O2','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer',str(harness),'-o',str(host)]
        subprocess.run(cmd,check=True,capture_output=True,text=True)
        out=subprocess.run([str(host)],check=True,capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=0'})
        return {'result':'PASS','source_sha256':digest(SOURCE),'native_target_sha256':proof['canonical_function_sha256'],'native_cases':len(cases),'native_and_candidate_executions':executions,'instruction_coverage':[{'executed_words':len(v),'total_words':len(w),'unexecuted_offsets':[i*4 for i in range(len(w)) if i*4 not in v]} for v,w in zip(coverage,[native,candidate])],'interpreted_steps':steps,'helper_model':'Verified canonical pointer-to-u32 shift-left-eight; caller-clobber poisoning and one post-helper global mutation family','host':'C89 GCC with AddressSanitizer and UndefinedBehaviorSanitizer; '+out.stdout.strip(),'host_limitation':'LeakSanitizer is unavailable under this container ptrace environment; detect_leaks=0. Address/undefined sanitizers remain enabled. Harness has no dynamic allocation.','domain':'Direct selectors0..31 and grouped250..255. Selectors32..249 are unchecked out-of-bounds in native/C and are not executed as valid C. Arithmetic uses unsigned subtraction then implementation-defined two-complement signed conversion, verified for GCC and IDO.','coverage':['all valid selectors','all 256 volume values on six grouped arms','zero/one/2/255/256/65535 duration','mixed type0/1/2/3/4/255','signed wraparound and extrema','both unrolled loop positions and array endpoints','mode and identifier default-only writes','untouched byte-for-byte remainder','genuine fifth stack argument','callee-save/stack restoration','no uninitialized stack reads','complete native and separately linked C bodies']}

HOST=r'''
#include <stdio.h>
MasterFader D_8004F300[32];
static unsigned int calls;
void func_8001E930(u32 *time) { ++calls; *time *= 256U; }
static s32 signed32(u32 v) { return v<=2147483647U?(s32)v: -1-(s32)(0xFFFFFFFFU-v); }
int main(void) {
    MasterFader expected[32];
    static const unsigned int vals[]={0,1,0x7F0000,0xFF0000,0xFFFFFFFFU,0x80000000U,0x7FFFFFFFU};
    static const unsigned short times[]={0,1,2,255,256,65535};
    unsigned int g,v,t,i,cases=0;
    int selected;
    assert(sizeof(u32)==4 && sizeof(s32)==4 && sizeof(MasterFader)==40);
    assert(offsetof(MasterFader,type)==20 && offsetof(MasterFader,seqMode)==21 && offsetof(MasterFader,pauseVol)==24);
    for(g=0;g<256;++g) {
        if(g>=32 && g<250)continue;
        for(v=0;v<256;++v)for(t=0;t<6;++t) {
            memset(D_8004F300,0xA5,sizeof(D_8004F300));
            for(i=0;i<32;++i) { D_8004F300[i].type=(u8)(i%6==5?255:i%6); D_8004F300[i].volume=vals[(i+v)%7]; }
            memcpy(expected,D_8004F300,sizeof(expected));
            for(i=0;i<32;++i) {
                selected=g<32?i==g:g==250?expected[i].type==2:g==251?expected[i].type==3:g==252?(expected[i].type==2||expected[i].type==3):g==253?expected[i].type==0:g==254?expected[i].type==1:(expected[i].type==0||expected[i].type==1);
                if(selected) {
                    expected[i].target=v<<16;
                    expected[i].delta=signed32((v<<16)-expected[i].volume)/(times[t]?times[t]:1);
                    expected[i].time=(times[t]?times[t]:1)*256U;
                    expected[i].seqId=g<32?0xFEDCBA98U:0xFFFFFFFFU;
                    if(g<32)expected[i].seqMode=(u8)(v^0xA5);
                }
            }
            calls=0;func_8001B9F8((u8)v,times[t],(u8)g,(u8)(v^0xA5),0xFEDCBA98U);
            assert(calls==1 && memcmp(expected,D_8004F300,sizeof(expected))==0);++cases;
        }
    }
    printf("%u cases passed\n",cases);return 0;
}
'''
if __name__=='__main__': print(json.dumps(run(),indent=2))
