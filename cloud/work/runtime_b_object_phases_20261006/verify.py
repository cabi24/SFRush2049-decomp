#!/usr/bin/env python3
"""Full ELF/GNU/native/C89 proof for image B's object phase initializer."""
import contextlib
import ctypes
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
PACKET=Path(__file__).resolve().parent
PROJECT=PACKET.parents[2]
ROOT=Path(os.environ.get('RUSH_REPO',str(PROJECT))).resolve()
sys.path.insert(0,str(ROOT))
from tools.cloud import score
SOURCE=PROJECT/'cloud/matches/ovl_b/func_8039133C.c'
NAME,ADDRESS,SIZE='func_8039133C',0x8039133C,340
BASE='cd22879d40b3de443cfde047b86e75e159b6cec6'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
IMAGE_SHA='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
STATE,HEAD,SEED=0x80399AE0,0x80143FD8,0x8011735C
OBJECTS,SLOTS,STACK,RETURN=0x81000000,0x82000000,0x70000000,0x60000000
ALLOC,RNG=0x80097470,0x8008B2E4
GLOBALS={'D_80399AE0':STATE,'D_80143FD8':HEAD,'audio_dma_sync':ALLOC,'func_8008B2E4':RNG}
U32=0xffffffff


def portable_receipt(receipt):
    """Compare packet proof; base-context and whole-tree digests are provenance."""
    result = json.loads(json.dumps(receipt))
    for path in (
        'asm/us/ovl_b/SHA256SUMS',
        'asm/us/ovl_b/extents.json',
        'asm/us/ovl_b/ovl_b_8038a400.s',
        'asm/us/ovl_b/symbols.json',
        'tools/cloud/score.py',
    ):
        result.get('inputs_sha256', {}).pop(path, None)
    for key in ('allocator_sha256', 'list_witness_sha256'):
        result.get('contexts', {}).pop(key, None)
    result.get('packet_sha256', {}).pop('test_packet.py', None)
    if not result.get('inputs_sha256'):
        result.pop('inputs_sha256', None)
    return result


def sha_bytes(b):return hashlib.sha256(b).hexdigest()
def sha(p):return sha_bytes(Path(p).read_bytes())
def run(args,**kw):
    p=subprocess.run([str(a) for a in args],capture_output=True,text=True,**kw)
    assert p.returncode==0,(args,p.returncode,p.stdout,p.stderr)
    return p

def pinned(path):return subprocess.check_output(['git','-C',str(ROOT),'show',BASE+':'+path])
def signed(x,n):
    x &= (1<<n)-1
    return x-(1<<n) if x>>(n-1) else x

def bits(f):return struct.unpack('>I',struct.pack('>f',f))[0]
def value(b):return struct.unpack('>f',struct.pack('>I',b))[0]
def f32(f):return value(bits(f))
def put(mem,a,v,n):
    assert a%n==0 and all(a+i in mem for i in range(n)),('unmapped write',hex(a),n)
    for i in range(n):mem[a+i]=(v>>(8*(n-i-1)))&255

def get(mem,a,n):
    assert a%n==0 and all(a+i in mem for i in range(n)),('unmapped read',hex(a),n)
    return int.from_bytes(bytes(mem[a+i] for i in range(n)),'big')

def visible(mem):return {a:v for a,v in mem.items() if not STACK-64<=a<STACK+32}
def fixture(count,var,seed,reuse):
    mem={}
    for base,length in [(STATE-8,36),(HEAD-4,12),(SEED-4,12),(OBJECTS-8,144),(SLOTS-8,80),(STACK-64,96)]:
        for i in range(length):mem[base+i]=(i*17+var*41+count*3)&255
    mem[STATE]=count
    for off,v in [(4,-17),(8,23),(12,-31)]:put(mem,STATE+off,bits(v),4)
    put(mem,STATE+16,SLOTS if reuse else 0,4)
    n=var%9
    put(mem,HEAD,OBJECTS if reuse and n else 0,4);put(mem,SEED,seed,4)
    for i in range(8):
        a=OBJECTS+16*i;put(mem,a,a+16 if i+1<n else 0,4);mem[a+4]=(count+i*43+var*11)&255
        a=SLOTS+8*i;put(mem,a,0,4);put(mem,a+4,-123,2);put(mem,a+6,234,2)
    return mem

def snap(mem,scale=0):
    h=0
    for i in range(8):
        a=SLOTS+8*i
        for v in [get(mem,a,4),get(mem,a+4,2),get(mem,a+6,2)]:h=(h*33+v)&U32
    return [get(mem,STATE+o,4) for o in [4,8,12]]+[get(mem,SEED,4),scale,h,mem[STATE],get(mem,HEAD,4)]

def allocation_effect(mem,count,var):
    put(mem,HEAD,OBJECTS if var%9 else 0,4)
    mem[STATE]=(count+73)&255

def oracle(count,var,seed,reuse):
    mem=fixture(count,var,seed,reuse);traces=[];snapshots=[[0]*8 for _ in range(5)]
    if reuse==0:
        snapshots[0]=snap(mem);traces.append((ALLOC,[0,(signed(count,8)*8)&U32],visible(mem)))
        allocation_effect(mem,count,var);put(mem,STATE+16,SLOTS,4)
    k=0
    for i in range(var%9):
        a=OBJECTS+16*i
        if mem[a+4]&32:
            put(mem,SLOTS+k*8,a,4);put(mem,SLOTS+k*8+4,1,2);put(mem,SLOTS+k*8+6,1,2);k+=1
    for i,scale in enumerate([10.,45.,45.,1.]):
        snapshots[i+1]=snap(mem,bits(scale));traces.append((RNG,[bits(scale)],visible(mem)))
        seed=(seed*0x41c64e6d+12345)&U32;put(mem,SEED,seed,4)
        r=f32(f32(float((seed>>16)&32767)*scale)/32768.)
        if i<3:put(mem,STATE+[4,8,12][i],bits(f32(r+[5.,15.,15.][i])),4)
        else:
            off=12 if r>0.5 else 8
            put(mem,STATE+off,bits(f32(value(get(mem,STATE+off,4))+45.)),4)
    return mem,traces,snapshots

def execute(words,rng_words,count,var,seed,reuse,poison=False):
    assert len(words)*4==SIZE and len(rng_words)==18
    mem=fixture(count,var,seed,reuse);initial_mem=mem.copy();expect,traces,snapshots=oracle(count,var,seed,reuse)
    regs=[(0x1234567*(i+1)+var*31)&U32 for i in range(32)];regs[0]=0;regs[4]=reuse&U32;regs[29]=STACK;regs[31]=RETURN
    initial=regs[:];fp=[0x3f800000+i*77 for i in range(32)];initial_fp=fp[:]
    pc=ADDRESS;pending=None;covered=set();rng_covered=set();branches=set();calls=[];lo=0;fcc=False;inside_rng=False
    for steps in range(4000):
        inmain=ADDRESS<=pc<ADDRESS+SIZE
        assert pc%4==0 and (inmain or RNG<=pc<RNG+72),('bad pc',hex(pc))
        if inmain:w=words[(pc-ADDRESS)//4];covered.add(pc-ADDRESS)
        else:w=rng_words[(pc-RNG)//4];rng_covered.add(pc-RNG)
        old=pending;pending=None;nextpc=pc+4
        op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16);a=(regs[rs]+imm)&U32
        if op==0:
            if fn==0:regs[rd]=(regs[rt]<<sh)&U32
            elif fn==3:regs[rd]=(signed(regs[rt],32)>>sh)&U32
            elif fn==0x21:regs[rd]=(regs[rs]+regs[rt])&U32
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==0x19:lo=(regs[rs]*regs[rt])&U32
            elif fn==0x12:regs[rd]=lo
            elif fn==8:pending=('jump',regs[rs])
            else:raise AssertionError(('unknown SPECIAL',fn))
        elif op==9:regs[rt]=a
        elif op==12:regs[rt]=regs[rs]&(w&65535)
        elif op==13:regs[rt]=regs[rs]|(w&65535)
        elif op==15:regs[rt]=(w&65535)<<16
        elif op in [0x20,0x24,0x23]:
            n=4 if op==0x23 else 1;v=get(mem,a,n);regs[rt]=(signed(v,8) if op==0x20 else v)&U32
        elif op in [0x29,0x2b]:put(mem,a,regs[rt],2 if op==0x29 else 4)
        elif op in [4,5,0x14,0x15]:
            take=regs[rs]==regs[rt] if op in [4,0x14] else regs[rs]!=regs[rt]
            branches.add((pc-ADDRESS,take));likely=op in [0x14,0x15]
            if likely and not take:nextpc=pc+8
            else:pending=('jump',pc+4+imm*4 if take else pc+8)
        elif op==3:
            regs[31]=pc+8;pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2))
        elif op==0x31:fp[rt]=get(mem,a,4)
        elif op==0x39:put(mem,a,fp[rt],4)
        elif op==0x11:
            fs=rd;ft=rt;fd=sh
            if rs==4:fp[fs]=regs[rt]
            elif rs==8:
                take=fcc if rt&1 else not fcc;branches.add((pc-ADDRESS,take))
                if rt&2 and not take:nextpc=pc+8
                else:pending=('jump',pc+4+imm*4 if take else pc+8)
            elif rs==0x14 and fn==0x20:fp[fd]=bits(float(signed(fp[fs],32)))
            elif rs==0x10:
                x,y=value(fp[fs]),value(fp[ft])
                if fn==0:fp[fd]=bits(x+y)
                elif fn==2:fp[fd]=bits(x*y)
                elif fn==3:fp[fd]=bits(x/y)
                elif fn==0x3c:fcc=x<y
                else:raise AssertionError(('unknown FP',fn))
            else:raise AssertionError(('unknown COP1',rs,fn))
        else:raise AssertionError(('unknown opcode',op))
        regs[0]=0;pc=nextpc
        if old:
            assert pending is None,'branch in delay slot'
            kind,target=old
            if kind=='call':
                args=regs[4:6] if target==ALLOC else [fp[12]]
                assert len(calls)<len(traces) and (target,args,visible(mem))==traces[len(calls)],('call mismatch',len(calls),target,args)
                calls.append((target,args))
                if target==ALLOC:
                    allocation_effect(mem,count,var);pc=regs[31]
                    for r in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:regs[r]=(0xab000000+r*999)&U32
                    for r in range(20):fp[r]=0x4a000000+r*99
                    regs[2]=SLOTS
                else:
                    assert target==RNG;inside_rng=True;pc=target
            elif target==RETURN:break
            else:
                if inside_rng and ADDRESS<=target<ADDRESS+SIZE:
                    inside_rng=False
                    if poison:
                        for r in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:regs[r]=(0xab000000+r*999)&U32
                        for r in range(1,20):fp[r]=0x4a000000+r*99
                pc=target
    else:raise AssertionError('did not return')
    assert visible(mem)==visible(expect)
    assert len(calls)==len(traces)
    for r in list(range(16,24))+[28,29,30,31]:assert regs[r]==initial[r],r
    assert fp[20:]==initial_fp[20:]
    for a in range(STACK-64,STACK+32):
        if not STACK-32<=a<STACK:assert mem[a]==initial_mem[a]
    return covered,rng_covered,branches

def inspect(obj):
    data,sections=score._elf(obj)
    symbols=[s for i,t in enumerate(sections) if t['type']==2 for s in score._symbol_table(data,sections,i)]
    funcs=[s for s in symbols if s['type']==2 and s['section']==score._text_index(sections)]
    assert len(funcs)==1 and funcs[0]['name']==NAME
    fn=funcs[0];assert fn['size']==SIZE
    assert not [s for s in symbols if s['type']==1 and 0<s['section']<0xff00]
    shoff=struct.unpack_from('>I',data,0x20)[0];entsize=struct.unpack_from('>H',data,0x2e)[0]
    for i,t in enumerate(sections):
        flags=struct.unpack_from('>I',data,shoff+i*entsize+8)[0]
        assert not(flags&2 and t['size'] and t['name'] not in ['.text','.reginfo'])
    text=sections[score._text_index(sections)];raw=data[text['off']:text['off']+text['size']]
    assert len(raw)>=SIZE and len(raw)-SIZE<16 and not any(raw[SIZE:])
    return data,sections,fn,raw

def cases():
    # Invert the four-step LCG to force fourth outputs immediately below,
    # exactly at, and immediately above the 0.5 decision boundary.
    inverse=pow(0x41c64e6d,-1,1<<32)
    for count in range(256):
        for var in range(18):
            seed=(count*0x9e3779b9+var*0x1234567)&U32
            if var in [0,1,2]:
                seed=((16383+var)<<16)|0x1234
                for _ in range(4):seed=((seed-12345)*inverse)&U32
            for reuse in [0,1,-1,0x12345678]:yield count,var,seed,reuse

def host_expected(count,var,seed,reuse):
    mem,_,snaps=oracle(count,var,seed,reuse)
    out=[mem[STATE]]+[get(mem,STATE+o,4) for o in [4,8,12]]+[get(mem,SEED,4),int(not reuse),(signed(count,8)*8)&U32 if not reuse else 0]
    for i in range(8):
        a=SLOTS+8*i;out += [get(mem,a,4),get(mem,a+4,2),get(mem,a+6,2)]
    return out+[v for s in snaps for v in s]

def native_link_script():
    # Both output placement and input subalignment are explicit: ADDRESS is
    # word-aligned but GNU 2.42 otherwise rounds this section up to 16 bytes.
    return ('SECTIONS { .text 0x%08X : SUBALIGN(4) { *(.text) } '
            '/DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n' % ADDRESS +
            '\n'.join('%s = 0x%08x;' % (k, v) for k, v in GLOBALS.items()))


def prove():
    score.ASM_DIR=ROOT/'asm/us/blob';rng_words=score.targets()['func_8008B2E4']
    list_words=score.targets()['func_800BEA6C']
    caller_words=score.targets()['engine_sound_update']
    assert len(caller_words)==357 and caller_words[0x558//4]>>26==3
    assert ((caller_words[0x558//4]&0x3ffffff)<<2)==(ADDRESS&0x0fffffff)
    score.ASM_DIR=ROOT/'asm/us/ovl_b';native=score.targets()[NAME]
    manifests=score.target_manifest();extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifests))
    entry=next(f for f in extents['functions'] if f['name']==NAME)
    assert (extents['image'],extents['image_sha256'],entry['address'],entry['size'])==('B',IMAGE_SHA,hex(ADDRESS).upper().replace('0X','0x'),SIZE)
    assert extents['rom_offset']=='0xB6FEC4'
    addresses=score.image_symbols()
    for k,v in dict(GLOBALS,**{NAME:ADDRESS}).items():assert addresses.get(k,score.address_named(k))==v
    locks=json.loads(pinned('blob_matched.lock.json'))
    assert NAME not in locks and 'func_8008B2E4' not in locks
    helper_path='src/blob/groups/audio_heap/group.c';helper=pinned(helper_path)
    assert sha_bytes(helper)=='21357517675e9bfefe1695b7cb97d3425fb6fbb7d680d18cf8b1272f8b86b041'
    assert locks['audio_dma_sync']['verified']=='image_gate' and locks['audio_dma_sync']['group']=='audio_heap'
    assert b'void *audio_dma_sync(Heap *heap, u32 size)' in helper
    helper_manifest=pinned('src/blob/groups/audio_heap/group.json')
    assert json.loads(helper_manifest)['files']==['group.c']
    assert 'audio_dma_sync' in json.loads(helper_manifest)['members']
    assert locks['audio_dma_sync']['source']=='src/blob/groups/audio_heap/group.json'
    assert locks['func_800BEA6C']['verified']=='image_gate'
    assert sha_bytes(pinned('src/blob/func_800BEA6C.c'))==locks['func_800BEA6C']['source_sha256']
    with tempfile.TemporaryDirectory(prefix='b-9133c-') as dirname:
        tmp=Path(dirname);obj=tmp/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
        with contextlib.redirect_stdout(io.StringIO()):result=score.compare(obj,NAME)
        assert result.accepted(),vars(result)
        data,sections,fn,raw=inspect(obj);assert fn['value']==0
        symrows=[s for i,t in enumerate(sections) if t['type']==2 for s in score._symbol_table(data,sections,i)]
        assert {s['name'] for s in symrows if s['name'] and s['section']==0}==set(GLOBALS)
        words=score.text_words(obj)
        relocated,masks,unresolved,unverified,errors=score.relocate(obj,words,0,SIZE,addresses)
        assert not any([masks,unresolved,unverified,errors])
        assert relocated[:SIZE//4]==native and not any(relocated[SIZE//4:])
        relocs=[]
        for sec in sections:
            if sec['type']!=9:continue
            assert sec['info']==score._text_index(sections)
            syms=score._symbol_table(data,sections,sec['link'])
            for pos in range(sec['off'],sec['off']+sec['size'],8):
                off,info=struct.unpack_from('>II',data,pos)
                relocs.append({'offset':off,'type':info&255,'symbol':syms[info>>8]['name']})
        assert sorted((r['offset'],r['type'],r['symbol']) for r in relocs)==sorted([
            (0x10,5,'D_80399AE0'),(0x14,6,'D_80399AE0'),(0x24,4,'audio_dma_sync'),
            (0x38,5,'D_80399AE0'),(0x3c,6,'D_80399AE0'),(0x30,5,'D_80143FD8'),(0x34,6,'D_80143FD8'),
            (0x9c,4,'func_8008B2E4'),(0xb8,4,'func_8008B2E4'),(0xd4,4,'func_8008B2E4'),(0xf0,4,'func_8008B2E4')])
        assert all(0<=r['offset']<SIZE and r['type'] in [4,5,6] for r in relocs)
        script=tmp/'native.ld';script.write_text(native_link_script())
        linked=tmp/'linked.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
        ld,ls,lf,lr=inspect(linked);assert lf['value']==ADDRESS
        linked_syms={s['name']:s for i,t in enumerate(ls) if t['type']==2 for s in score._symbol_table(ld,ls,i)}
        assert all(linked_syms[k]['value']==v and linked_syms[k]['section']!=0 for k,v in GLOBALS.items())
        assert not [t for t in ls if t['type'] in [4,9] and t['size']]
        sho=struct.unpack_from('>I',ld,0x20)[0];she=struct.unpack_from('>H',ld,0x2e)[0]
        assert struct.unpack_from('>I',ld,sho+score._text_index(ls)*she+12)[0]==ADDRESS
        linkedwords=list(struct.unpack('>85I',lr[:SIZE]));assert linkedwords==native
        nm=run(['mips-linux-gnu-nm','-S','--defined-only',linked]).stdout
        assert any(line.split()==['8039133c','00000154','T',NAME] for line in nm.splitlines())
        layout=tmp/'layout.c';layout.write_text('#include "'+str(SOURCE)+'"\n#define OFF(t,m) ((unsigned int)&((t *)0)->m)\n'+
            '\n'.join('typedef char test%d[(%s)?1:-1];'%(i,e) for i,e in enumerate([
                'sizeof(void*)==4','sizeof(Slot)==8','sizeof(State)==20','OFF(Object,flags)==4',
                'OFF(State,delay)==4','OFF(State,phase_a)==8','OFF(State,phase_b)==12','OFF(State,slots)==16',
                'OFF(Slot,active)==4','OFF(Slot,count)==6'])))
        score.compile_single(layout,FLAGS,tmp/'layout.o');assert inspect(tmp/'layout.o')[3]==raw
        common=['gcc','-shared','-fPIC','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fsanitize=undefined','-fno-sanitize-recover=all']
        so=tmp/'host.so';run(common+['-DCANDIDATE_PATH="'+str(SOURCE)+'"',PACKET/'host_test.c','-o',so])
        host=ctypes.CDLL(str(so)).run_host;host.argtypes=[ctypes.c_uint,ctypes.c_uint,ctypes.c_uint,ctypes.c_int,ctypes.POINTER(ctypes.c_uint)];host.restype=ctypes.c_int
        coverage=set();rng_cov=set();branches=set();count_cases=0
        for count,var,seed,reuse in cases():
            for body,poison in [(native,False),(relocated[:85],True),(linkedwords,False)]:
                c,r,b=execute(body,rng_words,count,var,seed,reuse,poison);coverage|=c;rng_cov|=r;branches|=b
            out=(ctypes.c_uint*71)();status=host(count,var,seed,reuse,out)
            assert status==0,('host status',status,count,var,reuse)
            assert list(out)==host_expected(count,var,seed,reuse),('host mismatch',count,var,reuse,list(out),host_expected(count,var,seed,reuse))
            count_cases+=1
        # One duplicate load after a branch-likely is unreachable, as in native.
        assert coverage==set(range(0,SIZE,4))-{0x12c},sorted(set(range(0,SIZE,4))-coverage)
        assert rng_cov==set(range(0,72,4))
        bad=native[:];bad[0]=0xffffffff
        try:execute(bad,rng_words,0,0,0,0)
        except AssertionError:pass
        else:raise AssertionError('unknown opcode accepted')
        source=SOURCE.read_text();mutants={
            'inverted_filter':('object->flags & 0x20','object->flags & 0x10'),
            'wrong_allocation_count':('D_80399AE0.count * 8','(u8)D_80399AE0.count * 8'),
            'tie_selects_wrong_phase':('> 0.5f','>= 0.5f'),
            'wrong_entry_tag':('.active = 1;','.active = 2;'),
            'wrong_rng_range':('func_8008B2E4(10.0f)','func_8008B2E4(11.0f)')}
        rejected=[]
        drills=list(cases())[:216]+[(255,8,0,0)]
        for name,(old,new) in mutants.items():
            assert source.count(old)==1
            p=tmp/'mutant.c';p.write_text(source.replace(old,new));score.compile_single(p,FLAGS,obj)
            with contextlib.redirect_stdout(io.StringIO()):m=score.compare(obj,NAME)
            assert not m.accepted()
            mutant_so=tmp/(name+'.so');run(common+['-DCANDIDATE_PATH="'+str(p)+'"',PACKET/'host_test.c','-o',mutant_so])
            mh=ctypes.CDLL(str(mutant_so)).run_host;mh.argtypes=host.argtypes;mh.restype=ctypes.c_int
            found=False
            for c,v,s,r in drills:
                out=(ctypes.c_uint*71)();status=mh(c,v,s,r,out)
                if status!=0 or list(out)!=host_expected(c,v,s,r):found=True;break
            assert found,name
            rejected.append(name)
        receipt={'status':'MATCH','base':BASE,'image':'B','image_sha256':IMAGE_SHA,'address':hex(ADDRESS),'bytes':SIZE,
            'source_sha256':sha(SOURCE),'flags':FLAGS+' -Wab,-r4300_mul',

            'packet_sha256':{p.name:sha(p) for p in [PACKET/'verify.py',PACKET/'host_test.c']},
            'contexts':{'allocator_source':helper_path,'allocator_native_execution':False,
                'list_witness':'src/blob/func_800BEA6C.c',
                'direct_caller':{'name':'engine_sound_update','offset':'0x558','bytes':1428,'sha256':sha_bytes(struct.pack('>357I',*caller_words))},
                'rng_native_sha256':sha_bytes(struct.pack('>18I',*rng_words)),'rng_accepted':False,'rng_native_executed':True},
            'elf':{'function_size':SIZE,'text_size':len(raw),'alignment_bytes_outside_function':len(raw)-SIZE,'owned_data_bytes':0,
                'relocations':relocs,'strict_differing_words':0,'gnu_differing_words':0,'body_sha256':sha_bytes(lr[:SIZE]),'native_layout_assertions':10},
            'behavior':{'cases':count_cases,'native_project_gnu_executions':count_cases*3,'unchanged_host_c89_ubsan_cases':count_cases,
                'native_instruction_offsets':len(coverage),'rng_instruction_offsets':len(rng_cov),'unreachable_duplicate_load_offset':'0x12c',
                'branch_outcomes':sorted([list(b) for b in branches]),'full_callback_snapshots_and_final_memory':True,
                'saved_gpr_fpr_stack_canaries':True,'source_mutants_rejected':rejected,'unknown_opcode_rejected':True,
                'scope':'Acyclic lists of 0..8 nodes, all raw count bytes, four reuse modes, successful allocator boundary returning sufficient mapped slots, finite RNG results and exact fourth-call threshold. Actual native RNG executes; allocator remains a contract hook. LP64 host layout is not native ABI.'}}
    return receipt

def tool_provenance():
    names=[('ido_'+n,score.ido(n)) for n in ['cc','cfe','ugen','uopt','as1']]
    names += [(n,shutil.which(n)) for n in ['mips-linux-gnu-ld','mips-linux-gnu-nm','gcc']]
    return {n:sha(p) for n,p in names}

if __name__=='__main__':
    import argparse
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('output',nargs='?');parser.add_argument('--check',action='store_true');parser.add_argument('--tool-provenance',type=Path)
    args=parser.parse_args();result=prove()
    if args.tool_provenance:args.tool_provenance.write_text(json.dumps(tool_provenance(),indent=2,sort_keys=True)+'\n')
    if args.check:assert portable_receipt(result)==portable_receipt(json.loads((PACKET/'verification.json').read_text()));print('Frozen proof reproduced')
    elif args.output:Path(args.output).write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:print(json.dumps(result,indent=2,sort_keys=True))
