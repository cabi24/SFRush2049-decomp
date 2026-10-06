#!/usr/bin/env python3
"""Full-word research proof for image-B initializer; no published target bytes."""
import argparse
import contextlib
import hashlib
import io
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

if not __debug__:
    raise RuntimeError('Verification requires assertions enabled')
PACKET=Path(__file__).resolve().parent
PROJECT=PACKET.parents[2]
ROOT=Path(os.environ.get('RUSH_REPO',str(PROJECT))).resolve()
sys.path.insert(0,str(ROOT))
from tools.cloud import score
SOURCE=PACKET/'nonmatch/func_803908D0.c'
BASE='cd22879d40b3de443cfde047b86e75e159b6cec6'
NAME='func_803908D0';ADDRESS=0x803908D0;SIZE=560
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'
IMAGE='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
U32=0xffffffff;STACK=0x70000000;RETURN=0x60000000;COUNT=0x80140BDC
NAMES=[0x803942EC+i*4 for i in range(10)]+[0x80394314+i*4 for i in range(7)]+[0x80394330+i*4 for i in range(5)]+[0x80394344+i*4 for i in range(5)]
MODELS=[0x80399B18+i*4 for i in range(10)]+[0x80399B40+i*4 for i in range(7)]
TEXTURES=[0x80399AF8+i*2 for i in range(5)]+[0x80399B08+i*2 for i in range(5)]
POOLS=[(0x8039A520,0x80399B60,104,24,0),(0x8039A5F8,0x8039A538,8,24,0),(0x80394F70,0x8039AE98,60,24,0),(0x8039AE80,0x8039A610,72,30,0)]
HELPERS={'struct_fields_init':0x800B0550,'string_copy_format':0x80092E2C,'func_800B24EC':0x800B24EC,'func_8038D1A8':0x8038D1A8}

def sha_bytes(b):return hashlib.sha256(b).hexdigest()
def sha(p):return sha_bytes(Path(p).read_bytes())
def signed(n,bits):
    n&=(1<<bits)-1
    return n-(1<<bits) if n>>(bits-1) else n

def run(args,**kw):
    p=subprocess.run([str(a) for a in args],capture_output=True,text=True,**kw)
    assert p.returncode==0,(args,p.returncode,p.stdout,p.stderr)
    return p

def pinned(path):return subprocess.check_output(['git','-C',str(ROOT),'show',BASE+':'+path])

class Memory:
    def __init__(self,count,variant):
        self.parts=[]
        for base,size in [(0x80394F60,0x64F0),(0x803942E0,0x80),(COUNT-4,12),(STACK-80,112)]:
            self.parts.append((base,bytearray(((i*71+count*3+variant*19)^0xa5)&255 for i in range(size))))
    def region(self,a,n):
        for base,data in self.parts:
            if base<=a and a+n<=base+len(data):return data,a-base
        raise AssertionError(('unmapped memory',hex(a),n))
    def get(self,a,n):
        assert a%n==0
        d,i=self.region(a,n);return int.from_bytes(d[i:i+n],'big')
    def put(self,a,v,n):
        assert a%n==0
        d,i=self.region(a,n);d[i:i+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
    def fill(self,a,v,n):
        d,i=self.region(a,n);d[i:i+n]=bytes([v])*n
    def state(self):return b''.join(bytes(d) for a,d in self.parts if a!=STACK-80)

def fixture(count,variant):
    m=Memory(count,variant);m.put(COUNT,count,1);m.put(0x80394F88,123,2)
    for i,a in enumerate(NAMES):m.put(a,0x10000000+((i+count)%64)*16,4)
    for i,a in enumerate(MODELS):m.put(a,12345+i,4)
    for i,a in enumerate(TEXTURES):m.put(a,1000+i,2)
    for i in range(4):
        m.put(0x80394F90+i*52,200+i,4)
        for j in range(12):m.put(0x80394F94+i*52+j*4,int.from_bytes(struct.pack('>f',float(count+i+j)),'big'),4)
    return m

def effect(m,j,variant):
    if variant&1:m.put(COUNT,m.get(COUNT,1)+j*13+17,1)
    if variant&2:
        k=(j+3)%27;a=NAMES[k]
        token=(m.get(a,4)-0x10000000)//16
        m.put(a,0x10000000+((token+11)%64)*16,4)

def handle(m,j,variant):
    c=m.get(COUNT,1)
    return ((j*7+variant)&1023)|(((j+variant)%c)<<10) if c else 0

def hook_effect(m,k,variant):
    if k<4:
        head,pool,size,n,_=POOLS[k];m.fill(head,0x31+k,24);m.fill(pool,0x41+k,size*n);return 0
    if k<31:
        j=k-4;c=m.get(COUNT,1);result=handle(m,j,variant)
        if j<17 and not c:result=U32
        effect(m,j,variant)
        if j>=17:m.put(TEXTURES[j-17],result,2);return 0
        return result
    return 0

def expected(count,variant):
    m=fixture(count,variant);m.put(0x80394F88,0,2);trace=[]
    for k in range(32):
        if k<4:target=HELPERS['struct_fields_init'];args=list(POOLS[k])
        elif k<31:
            j=k-4;high=signed(m.get(COUNT,1)-1,8)&U32;name=m.get(NAMES[j],4)
            if j<17:target=HELPERS['string_copy_format'];args=[name,0,high,0]
            else:target=HELPERS['func_800B24EC'];args=[name,TEXTURES[j-17],0,high,1]
        else:
            for i in range(4):m.put(0x80394F90+i*52,U32,4)
            target=HELPERS['func_8038D1A8'];args=[]
        trace.append((target,args,sha_bytes(m.state())))
        result=hook_effect(m,k,variant)
        if 4<=k<21:m.put(MODELS[k-4],result,4)
    return m.state(),trace

def execute(words,count,variant):
    assert len(words)*4==SIZE
    final,trace=expected(count,variant);m=fixture(count,variant)
    regs=[((i+1)*0x01020304 ^ (variant*0x10205+count))&U32 for i in range(32)]
    regs[0]=0;regs[29]=STACK;regs[31]=RETURN;initial=regs[:]
    pc=ADDRESS;pending=None;coverage=set();branches=set();calls=0;count_reads=0
    stack_before=bytes(m.parts[-1][1])
    for steps in range(3000):
        assert ADDRESS<=pc<ADDRESS+SIZE and pc%4==0,('bad pc',hex(pc))
        old=pending;pending=None;w=words[(pc-ADDRESS)//4];coverage.add(pc-ADDRESS)
        op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63
        imm=signed(w,16);a=(regs[rs]+imm)&U32
        if op==0:
            if fn==0:regs[rd]=(regs[rt]<<sh)&U32
            elif fn==3:regs[rd]=(signed(regs[rt],32)>>sh)&U32
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==0x2b:regs[rd]=int(regs[rs]<regs[rt])
            elif fn==8:assert old is None and rs==31;pending=('jump',regs[rs])
            else:raise AssertionError(('unknown SPECIAL',fn))
        elif op==9:regs[rt]=a
        elif op==15:regs[rt]=(w&65535)<<16
        elif op in (0x24,0x23):
            n=1 if op==0x24 else 4;regs[rt]=m.get(a,n)
            if a==COUNT:assert n==1;count_reads+=1
        elif op in (0x29,0x2b):m.put(a,regs[rt],2 if op==0x29 else 4)
        elif op==5:
            assert old is None;taken=regs[rs]!=regs[rt];branches.add((pc-ADDRESS,taken));pending=('jump',pc+4+imm*4 if taken else pc+8)
        elif op==3:
            assert old is None;regs[31]=pc+8;pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2))
        else:raise AssertionError(('unknown opcode',op))
        regs[0]=0;pc+=4
        if old:
            assert pending is None,'branch in delay slot'
            kind,target=old
            if kind=='call':
                assert calls<len(trace)
                want,args,snapshot=trace[calls];assert target==want,(calls,hex(target),hex(want))
                actual=regs[4:4+min(4,len(args))]
                if len(args)==5:actual+=[m.get(regs[29]+16,4)]
                assert actual==args,(calls,actual,args)
                assert sha_bytes(m.state())==snapshot,('callback snapshot',calls)
                result=hook_effect(m,calls,variant);calls+=1;pc=regs[31]
                for r in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:regs[r]=(0xf0000000+r*0x12345+variant)&U32
                regs[2]=result
            elif target==RETURN:break
            else:pc=target
    else:raise AssertionError('did not return')
    assert calls==32 and count_reads==27
    assert m.state()==final,'complete final state'
    assert all(regs[r]==initial[r] for r in list(range(16,24))+[28,29,30,31]),'callee-save/stack'
    now=bytes(m.parts[-1][1]);assert now[:32]==stack_before[:32] and now[80:]==stack_before[80:],'stack canaries'
    return coverage,branches

def inspect(obj):
    data,secs=score._elf(obj);syms=[s for i,t in enumerate(secs) if t['type']==2 for s in score._symbol_table(data,secs,i)]
    f=[s for s in syms if s['name']==NAME and s['type']==2];assert len(f)==1 and f[0]['size']==SIZE
    shoff=struct.unpack_from('>I',data,0x20)[0];entsz=struct.unpack_from('>H',data,0x2e)[0]
    for i,t in enumerate(secs):
        flags=struct.unpack_from('>I',data,shoff+i*entsz+8)[0]
        assert not(flags&2 and t['size'] and t['name'] not in ('.text','.reginfo','.MIPS.abiflags'))
    ti=score._text_index(secs);t=secs[ti];raw=data[t['off']:t['off']+t['size']];assert len(raw)==SIZE
    textaddr=struct.unpack_from('>I',data,shoff+ti*entsz+12)[0]
    return data,secs,f[0],raw,textaddr

def prove():
    score.ASM_DIR=ROOT/'asm/us/ovl_b';native=score.targets()[NAME];assert len(native)*4==SIZE
    manifests=score.target_manifest();ext=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifests))
    entry=next(x for x in ext['functions'] if x['name']==NAME)
    assert (ext['image'],ext['image_sha256'],entry['address'],entry['size'])==('B',IMAGE,'0x803908D0',SIZE)
    addresses=score.image_symbols();helpers={};lock=json.loads(pinned('blob_matched.lock.json'))
    blob_asm=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us/blob';blob_targets=score.targets();score.ASM_DIR=blob_asm
    for helper in ['struct_fields_init','pool_linked_list_init','string_copy_format','func_800B24EC']:
        path='src/blob/'+helper+'.c';source=pinned(path);assert lock[helper]['source_sha256']==sha_bytes(source) and lock[helper]['verified']=='image_gate'
        assert addresses[helper]==dict(HELPERS,pool_linked_list_init=0x800B04D0)[helper]
        body=blob_targets[helper]
        helpers[helper]={'path':path,'source_sha256':sha_bytes(source),'native_address':hex(addresses[helper]),'native_bytes':len(body)*4,'native_sha256':sha_bytes(struct.pack('>%dI'%len(body),*body)),'execution':'abstract O32 boundary; implementation not executed'}
    native_wrapper=score.targets()['func_8038D1A8'];assert len(native_wrapper)*4==88
    helpers['func_8038D1A8']={'path':'cloud/matches/ovl_b/func_8038D1A8.c','source_sha256':sha_bytes(pinned('cloud/matches/ovl_b/func_8038D1A8.c')),'native_address':'0x8038d1a8','native_bytes':88,'native_sha256':sha_bytes(struct.pack('>22I',*native_wrapper)),'execution':'abstract O32 boundary; implementation not executed'}
    consumer=score.targets()['func_8038F938'];assert len(consumer)*4==928
    ce=next(x for x in ext['functions'] if x['name']=='func_8038F938');assert ce['address']=='0x8038F938' and ce['size']==928
    assert (consumer[0xcc//4]&65535)==0x4f90
    assert [(consumer[o//4]>>26,consumer[o//4]&65535) for o in [0xf4,0x104,0x114]]==[(0x39,0x28),(0x39,0x2c),(0x39,0x30)]
    receipt={'status':'NONMATCH','base':BASE,'image':'B','image_sha256':IMAGE,'address':hex(ADDRESS),'bytes':SIZE,'source_sha256':sha(SOURCE),'flags':FLAGS+' -Wab,-r4300_mul','accepted_or_coverage_bytes':0,'helpers':helpers,'consumer':{'name':'func_8038F938','address':'0x8038f938','bytes':928,'native_sha256':sha_bytes(struct.pack('>232I',*consumer)),'layout':'52-byte record; signed handle at +0, float matrix at +4, float position at +40/+44/+48'},'tool_sha256':tool_provenance(),
        'inputs_sha256':{str(p.relative_to(ROOT)):sha(p) for p in [score.ASM_DIR/'SHA256SUMS',score.ASM_DIR/'ovl_b_8038a400.s',score.ASM_DIR/'extents.json',score.ASM_DIR/'symbols.json',ROOT/'tools/cloud/score.py',ROOT/'tools/cloud/owndata.py',ROOT/'asm/us/blob/SHA256SUMS']},
        'packet_sha256':{str(p.relative_to(PACKET)):sha(p) for p in [PACKET/'verify.py',PACKET/'host_test.c',PACKET/'test_packet.py',PACKET/'controls/initial.c']}}
    with tempfile.TemporaryDirectory(prefix='b-908d0-') as td:
        tmp=Path(td);obj=tmp/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
        with contextlib.redirect_stdout(io.StringIO()):result=score.compare(obj,NAME)
        assert not result.accepted()
        data,secs,fn,raw,addr=inspect(obj);assert fn['value']==0 and addr==0
        relocated,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),0,SIZE,addresses)
        assert not any((masks,unresolved,unverified,errors))
        differences=[i*4 for i,(a,b) in enumerate(zip(native,relocated)) if a!=b]
        assert differences==[0x15c,0x160,0x170,0x1ac,0x1b0,0x1c0]
        relocs=[]
        for t in secs:
            if t['type']!=9:continue
            assert t['info']==score._text_index(secs)
            syms=score._symbol_table(data,secs,t['link'])
            for at in range(t['off'],t['off']+t['size'],8):
                off,info=struct.unpack_from('>II',data,at);symbol=syms[info>>8]['name'];typ=info&255
                assert off%4==0 and 0<=off<SIZE and typ in [4,5,6]
                relocs.append({'offset':off,'type':typ,'symbol':symbol})
        externs={r['symbol'] for r in relocs};bindings={k:addresses.get(k,score.address_named(k)) for k in externs};assert all(v is not None for v in bindings.values())
        script=tmp/'native.ld';script.write_text('SECTIONS { . = 0x803908D0; .text : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n'+'\n'.join('%s = 0x%08x;'%(k,v) for k,v in bindings.items()))
        linked=tmp/'linked.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj]);_,_,lf,lr,la=inspect(linked);assert lf['value']==ADDRESS and la==ADDRESS
        linkedwords=list(struct.unpack('>140I',lr));assert linkedwords==relocated
        coverage=set();branches=set()
        for count in range(256):
            for variant in range(4):
                for body in [native,relocated,linkedwords]:
                    c,b=execute(body,count,variant);coverage|=c;branches|=b
        assert coverage==set(range(0,SIZE,4))
        assert branches=={(off,choice) for off in [0xec,0x138,0x188,0x1d8] for choice in [True,False]}
        layout=tmp/'layout.c';layout.write_text('#include "'+str(SOURCE)+'"\n#define OFFSET(t,m) ((unsigned int)&((t*)0)->m)\ntypedef char p[(sizeof(void*)==4)?1:-1];\ntypedef char e[(sizeof(PlayerEffect)==52)?1:-1];\ntypedef char h[(OFFSET(PlayerEffect,handle)==0)?1:-1];\ntypedef char m[(OFFSET(PlayerEffect,transform)==4)?1:-1];\n');score.compile_single(layout,FLAGS,tmp/'layout.o');assert inspect(tmp/'layout.o')[3]==raw
        common=['gcc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fsanitize=undefined,bounds','-fno-sanitize-recover=all']
        exe=tmp/'host';run(common+['-DCANDIDATE_PATH="'+str(SOURCE)+'"',PACKET/'host_test.c','-o',exe]);host=run([exe]);assert host.stdout.strip()=='1024 unchanged-source C89 boundary cases passed' and not host.stderr
        source=SOURCE.read_text();controls={}
        score.compile_single(PACKET/'controls/initial.c',FLAGS,obj)
        with contextlib.redirect_stdout(io.StringIO()):initial=score.compare(obj,NAME)
        assert initial.differing==75 and initial.total==140 and not any((initial.unresolved,initial.unverified,initial.errors,initial.extra_words))
        initial_data,initial_sections=score._elf(obj)
        initial_symbol=next(s for i,t in enumerate(initial_sections) if t['type']==2 for s in score._symbol_table(initial_data,initial_sections,i) if s['name']==NAME and s['type']==2)
        assert initial_symbol['size']==556
        controls['initial']={'differing_words':75,'target_words':140,'function_bytes':556,'text_bytes':len(score.text_words(obj))*4,'source_sha256':sha(PACKET/'controls/initial.c'),'scope':'Historical initial isolated source control, not a complete-body acceptance candidate'}
        cases={'nonvolatile_count':(source.replace('extern volatile u8','extern u8'),FLAGS),'O3':(source,FLAGS.replace('-O2','-O3'))}
        for name,(s,flags) in cases.items():
            p=tmp/(name+'.c');p.write_text(s);score.compile_single(p,flags,obj);d,ss,f,r,a=inspect(obj);rw,ms,un,uv,er=score.relocate(obj,score.text_words(obj),0,SIZE,addresses);assert not any((ms,un,uv,er)) and rw==relocated;run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj]);assert inspect(linked)[3]==lr;controls[name]={'source_sha256':sha(p),'body_identical':True,'differing_words':6,'gnu_whole_object_equal':True}
        mutants={'wrong_high_bound':('D_80140BDC - 1','D_80140BDC'),'wrong_error_policy':('D_80140BDC - 1, 1','D_80140BDC - 1, 0'),'omit_last_model':('D_80399B18 + 10','D_80399B18 + 9'),'omit_last_handle':('i < 4','i < 3'),'wrong_pool_count':('104, 24, 0','104, 23, 0')}
        rejected=[]
        for name,(old,new) in mutants.items():
            assert old in source;p=tmp/(name+'.c');p.write_text(source.replace(old,new));run(common+['-DCANDIDATE_PATH="'+str(p)+'"',PACKET/'host_test.c','-o',exe]);bad=subprocess.run([str(exe)],capture_output=True,text=True);assert bad.returncode==1 and 'mismatch' in bad.stderr;rejected.append(name)
        try:execute([U32]+native[1:],0,0)
        except AssertionError:pass
        else:raise AssertionError('unknown instruction accepted')
        receipt['elf']={'function_bytes':SIZE,'text_bytes':SIZE,'alignment_bytes_outside_function':0,'owned_data_bytes':0,'relocations':relocs,'bindings':{k:hex(v) for k,v in sorted(bindings.items())},'differing_words':6,'differing_offsets':[hex(i) for i in differences],'gnu_agrees_with_project':True,'native_sha256':sha_bytes(struct.pack('>140I',*native)),'linked_sha256':sha_bytes(lr),'native_layout_assertions':4}
        receipt['controls']=controls
        receipt['behavior']={'fixtures':1024,'native_project_gnu_executions':3072,'host_c89_ubsan_bounds_cases':1024,'instruction_offsets':len(coverage),'branch_outcomes':len(branches),'dynamic_calls_per_case':32,'static_call_sites':9,'count_reads_per_case':27,'complete_mapped_state_and_callback_snapshots':True,'caller_save_poisoning':True,'saved_registers_and_stack_canaries':True,'compiled_host_mutants_rejected':rejected,'unknown_instruction_rejected':True,'scope':'Ordinary O32 helper-boundary model, not helper implementation execution. All count bytes; two independent count/name mutation modes; valid backed name/output arrays; four 52-byte player-effect records. Synthetic pool fills stress preservation, not actual pool construction; name contents, table searches, allocator behavior, aliases, concurrency, FCSR, hardware and gameplay are outside the proof.'}
    return receipt

def tool_provenance():
    names=[('ido_'+name,score.ido(name)) for name in ['cc','cfe','ugen','uopt','as1']]
    names += [(name,shutil.which(name)) for name in ['mips-linux-gnu-ld','mips-linux-gnu-readelf','gcc']]
    return {name:sha(path) for name,path in names}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('output',nargs='?');parser.add_argument('--check',action='store_true');args=parser.parse_args();receipt=prove()
    if args.check:assert receipt==json.loads((PACKET/'verification.json').read_text());print('Frozen source-bound proof reproduced')
    elif args.output:Path(args.output).write_text(json.dumps(receipt,indent=2,sort_keys=True)+'\n')
    else:print(json.dumps(receipt,indent=2,sort_keys=True))
