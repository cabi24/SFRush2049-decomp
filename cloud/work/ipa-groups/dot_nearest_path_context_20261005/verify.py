#!/usr/bin/env python3
"""Rebuild the source-bound path helper proof; output hashes and counts only."""
import argparse
import ctypes
import dataclasses
import hashlib
import importlib.util
import itertools
import json
import os
from pathlib import Path
import random
import struct
import subprocess
import sys
import tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score, owndata
spec=importlib.util.spec_from_file_location('path_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
NAME='func_800E4300'
CONTEXT=['func_800E451C','func_800E398C','func_800E4B58']
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'

def sha(data):return hashlib.sha256(data).hexdigest()
def pack(words):return struct.pack('>%dI'%len(words),*words)
def run(args,**kw):return subprocess.run(args,check=True,capture_output=True,**kw)

def object_records(obj):
    raw,sections=score._elf(obj)
    tab=next(i for i,s in enumerate(sections) if s['type']==2)
    syms=score._symbol_table(raw,sections,tab)
    return raw,sections,tab,syms

def object_proof(obj,work):
    raw,sections,tab,syms=object_records(obj);textidx=score._text_index(sections)
    functions={s['name']:s for s in syms if s['type']==2 and s['section']==textidx}
    assert set(functions)==set([NAME]+CONTEXT),functions
    assert functions[NAME]['size']==540
    tsec=sections[textidx];textwords=score.text_words(obj);addresses=score.image_symbols()
    normalized=bytearray(raw);internal_calls=[];all_relocs=[]
    # Express section-relative internal JAL relocations as named extern calls
    # solely in the independent GNU link fixture. Original ELF is untouched.
    byoffset={s['value']:(i,s) for i,s in enumerate(syms) if s['name'] in functions}
    for sec in sections:
        if sec['type']!=9 or sec['info']!=textidx:continue
        for k in range(sec['size']//8):
            loc=sec['off']+8*k;off,info=struct.unpack_from('>II',raw,loc)
            kind,idx=info&255,info>>8;s=syms[idx]
            all_relocs.append({'offset':off,'type':kind,'symbol':s['name']})
            if s['section']==textidx and s['type']==3:
                assert kind==4 and (textwords[off//4]>>26)==3
                addend=(textwords[off//4]&0x3ffffff)*4
                assert addend in byoffset,('internal call boundary',addend)
                calleeidx,callee=byoffset[addend]
                struct.pack_into('>I',normalized,tsec['off']+off,textwords[off//4]&0xfc000000)
                struct.pack_into('>I',normalized,loc+4,(calleeidx<<8)|kind)
                internal_calls.append({'site':off,'callee':callee['name']})
    for i,s in enumerate(syms):
        if s['name'] in functions:
            struct.pack_into('>I',normalized,sections[tab]['off']+16*i+4,0)
            struct.pack_into('>H',normalized,sections[tab]['off']+16*i+14,0)
    fixture=work/'gnu-fixture.o';fixture.write_bytes(normalized)
    base=addresses[NAME]-functions[NAME]['value']
    script='SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } .rodata 0x80124440 : SUBALIGN(4) { *(.rodata) } }\n'%base
    referenced=set(r['symbol'] for r in all_relocs if not r['symbol'].startswith('.')) | set(functions)
    for n in referenced:
        if n not in addresses and score.address_named(n) is not None: addresses[n]=score.address_named(n)
    assert referenced<=set(addresses),referenced-set(addresses)
    script+=''.join('%s = 0x%x;\n'%(n,addresses[n]) for n in sorted(referenced))
    ld=work/'gnu.ld';ld.write_text(script);elf=work/'gnu.elf';binary=work/'gnu.bin'
    run(['mips-linux-gnu-ld','-EB','-T',str(ld),'-o',str(elf),str(fixture)])
    run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)])
    blob=binary.read_bytes();assert len(blob)==tsec['size']
    ro=next(s for s in sections if s['name']=='.rodata')
    literals=raw[ro['off']:ro['off']+ro['size']]
    assert len(literals)==32 and literals[:20]==score.own_data().read(0x80124440,20) and not any(literals[20:])
    assert all(not s['size'] for s in sections if s['name'] in ('.data','.sdata','.bss','.sbss'))
    result={}
    for name,fn in functions.items():
        first,size=fn['value'],fn['size'];assert size>0 and size%4==0
        body=blob[first:first+size];words=list(struct.unpack('>%dI'%(size//4),body))
        # Independent project relocator with explicit actual own-literal placement.
        relocated,masks,unresolved,unverified,errors=score.relocate(obj,textwords,first,first+size,addresses)
        assert not(unresolved or errors)
        # For the unclaimed context, the compiler literal references occur at
        # shifted instructions; resolve their own section using fixed donor/data
        # verified placement, not same-position target inference.
        expected=list(relocated[first//4:(first+size)//4])
        pending=[]
        for r in all_relocs:
            if not first<=r['offset']<first+size or r['symbol']!='.rodata':continue
            i=(r['offset']-first)//4;original=textwords[r['offset']//4]
            if r['type']==5:pending.append((i,original&65535))
            elif r['type']==6:
                low=native.signed(original&65535,16)
                assert pending
                for hi,add in pending:
                    addr=0x80124440+(add<<16)+low
                    expected[hi]=(expected[hi]&0xffff0000)|((addr+0x8000)>>16)&65535
                expected[i]=(expected[i]&0xffff0000)|(addr&65535);pending=[]
            else:raise AssertionError('unexpected own-data relocation')
        assert not pending and words==expected,('GNU/project',name)
        want=score.targets()[name]
        differing=sum(a!=b for a,b in itertools.zip_longest(words,want))
        if name==NAME:
            assert differing==0 and score.compare(obj,name,show=0).accepted()
            assert not(masks or unverified)
        result[name]={'elf_bytes':size,'target_bytes':4*len(want),'object_offset':first,
                      'GNU_complete_word_differences':differing,'GNU_equals_project':True,
                      'body_sha256':sha(body),'relocations':[r for r in all_relocs if first<=r['offset']<first+size]}
    assert len(result[NAME]['relocations'])==8
    return result,list(struct.unpack('>135I',blob[functions[NAME]['value']:functions[NAME]['value']+540])),{
        'internal_calls':internal_calls,'verified_context_literal_bytes':20,'excluded_rodata_zero_alignment':12,
        'context_literal_sha256':sha(literals[:20]),'text_bytes':tsec['size'],'object_sha256':sha(raw),
        'own_data_bytes_for_claimed_helper':0,'external_threshold_sha256':sha(score.own_data().read(0x8012443c,4))}

def corpus():
    rng=random.Random(0xe4300)
    for k in range(1536):
        counts=[rng.choice([1,2,3,6,16,31,64]) for _ in range(4)]
        prev,cur,row=k%4,(k//4)%4,(k//16)%4
        starts=[0]*80
        for sec in range(5):
            for p in range(4):starts[sec*16+p]=rng.randrange(counts[p])
        last=rng.randrange(4);sections=4
        point=rng.choice([0,counts[cur]-1,-20,100,32767,-32768])
        xyz=[rng.randint(-32768,32767) for _ in range(4*64*3)]
        pos=[rng.randint(-32768,32767) for _ in range(3)]
        if k%8==0:xyz=[0]*len(xyz);pos=[0,0,0]
        if k%8==1:
            pos=[32767,-32768,0]
            for p in range(4):xyz[p*64*3:p*64*3+3]=pos
        # Legal integer metadata extremes exercise narrowing and skipped windows.
        if k%64 in (2,3,4,5) and prev!=cur:
            d=[-32768,-20000,16384,32767][k%64-2]
            starts[(row+1)*16+cur]=d
            starts[(row+1)*16+prev]=0
            last=0
        yield {'pos':pos,'prev':prev,'cur':cur,'row':row,'point':point,'counts':counts,
               'starts':starts,'last':last,'sections':sections,'xyz':xyz}

def oracle(c,threshold):
    S=native.signed;start=c['starts'];row=c['row'];cur=c['cur'];prev=c['prev'];n=c['counts'][cur]
    d=S(start[row*16+cur]-start[row*16+prev],16)
    other=(c['counts'][cur]-c['counts'][prev] if row+1==c['sections'] else
           start[(row+1)*16+cur]-start[(row+1)*16+prev])
    diff=S(other-d,16)
    if diff<0:diff=S(-diff,16)
    window=max(diff,5);idx=(c['point']+d-window)%n
    total=S(2*window+1,16);best=0;distance=threshold;F=native.f32
    for _ in range(max(total,0)):
        a=[c['xyz'][(cur*64+idx)*3+i]-c['pos'][i] for i in range(3)]
        squares=[F(float(x)*x) for x in a]
        candidate=F(F(squares[0]+squares[1])+squares[2])
        if candidate<distance:distance=candidate;best=idx
        idx=start[c['last']*16+cur] if idx==n-1 else idx+1
    return best

def native_case(words,c,threshold_bytes):
    data=bytearray([0xa5])*400
    struct.pack_into('>h',data,2,c['last']);struct.pack_into('>h',data,8,c['sections'])
    for row in range(5):
        for col in range(16):struct.pack_into('>h',data,row*80+48+col*2,c['starts'][row*16+col])
    tracks=bytearray();regions=[]
    for p,n in enumerate(c['counts']):
        address=0x200000+p*0x1000;tracks+=struct.pack('>HHI',n,0xa5a5,address)
        pts=b''.join(struct.pack('>hhhBB',*c['xyz'][(p*64+i)*3:(p*64+i)*3+3],0xa5,0xa5) for i in range(n))
        regions.append((address,pts))
    regions +=[(0x80151ce8,data),(0x8012e5e8,tracks),(0x100000,struct.pack('>3h',*c['pos'])),(0x8012443c,threshold_bytes)]
    return native.execute(words,0x800e4300,regions,[0x100000,c['prev'],c['point'],c['row'],c['cur']])

def host_build(work,source=None):
    directory=work/'host';directory.mkdir()
    for name in ('group.c','host.c'):(directory/name).write_bytes((HERE/name).read_bytes())
    if source is not None:(directory/'group.c').write_text(source)
    versions=directory/'exports';versions.write_text('{ global: check_case; local: *; };\n')
    out=directory/'host.so'
    run(['gcc','-std=c89','-O2','-shared','-fPIC','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-fno-sanitize-recover=all',
         '-Wl,--gc-sections','-Wl,--version-script='+str(versions),str(directory/'host.c'),'-o',str(out)])
    lib=ctypes.CDLL(str(out));fn=lib.check_case
    p16=ctypes.POINTER(ctypes.c_int16);u16=ctypes.POINTER(ctypes.c_uint16)
    fn.argtypes=[p16]+[ctypes.c_int16]*4+[p16,u16,p16,ctypes.c_int16,ctypes.c_int16,ctypes.c_uint32];fn.restype=ctypes.c_int
    return fn

def host_case(fn,c,literal):
    i16=ctypes.c_int16;u16=ctypes.c_uint16
    return fn((i16*3)(*c['pos']),c['prev'],c['point'],c['row'],c['cur'],(i16*80)(*c['starts']),
              (u16*4)(*c['counts']),(i16*len(c['xyz']))(*c['xyz']),c['last'],c['sections'],literal)



def caller_contract():
    addresses=score.image_symbols();targets=score.targets();graph={}
    for callee in [NAME]+CONTEXT:
        jump=0x0c000000|((addresses[callee]>>2)&0x3ffffff)
        graph[callee]={n:[hex(addresses[n]+4*i) for i,w in enumerate(words) if w==jump]
                       for n,words in targets.items() if jump in words}
    assert graph[NAME]=={'func_800E451C':['0x800e4748']}
    assert graph['func_800E451C']=={'func_800E4B58':['0x800e4bd4','0x800e4c1c','0x800e4c5c']}
    assert graph['func_800E398C']=={'func_800E4B58':['0x800e5064']}
    assert graph['func_800E4B58']=={'func_800E56F8':['0x800e5b3c']}
    w=targets['func_800E451C'];site=(0x800e4748-addresses['func_800E451C'])//4
    def decode(word):return word>>26,(word>>21)&31,(word>>16)&31,word&65535
    # The row/model halfword, point/Nav halfword and delay-slot path/Nav
    # halfword must reach the exact private argument registers.
    assert decode(w[site-2])==(33,21,5,0x7e2)
    assert decode(w[site-1])==(33,22,13,0x24)
    assert decode(w[site+1])==(33,22,10,0x28)
    near=w[site-30:site]
    assert any(decode(x)==(9,29,30,96) for x in w[:site])
    assert any((x>>26,(x>>21)&31,(x>>16)&31,(x>>11)&31,x&63)==(0,30,0,6,37) for x in near)
    assert any((x>>26,(x>>16)&31,(x>>11)&31,(x>>6)&31,x&63)==(0,8,14,16,3) for x in near)
    return {'native_direct_call_graph':graph,'private_arguments':['a2: pos[3] at caller sp+96',
             't2: Nav.sel at +40','t5: Nav.pt at +36','a1: model section at +2018','t0: current path index'],
             'verified_native_call_setup':True,'caller_end_to_end_runtime_tested':False}

def layout_proof(work):
    group=work/'layout';group.mkdir()
    spec=json.loads((HERE/'group.json').read_text())
    assertions="""
#define OFFSETOF(t,m) ((unsigned long)&((t *)0)->m)
typedef char size_short[(sizeof(s16)==2)?1:-1];
typedef char size_float[(sizeof(f32)==4)?1:-1];
typedef char size_section[(sizeof(Section)==80)?1:-1];
typedef char section_last[(OFFSETOF(Section,last)==2)?1:-1];
typedef char section_count[(OFFSETOF(Section,count)==8)?1:-1];
typedef char section_starts[(OFFSETOF(Section,start)==48)?1:-1];
typedef char size_point[(sizeof(TrackPt)==8)?1:-1];
typedef char point_flag[(OFFSETOF(TrackPt,flag)==6)?1:-1];
typedef char size_track[(sizeof(Track)==8)?1:-1];
typedef char track_points[(OFFSETOF(Track,points)==4)?1:-1];
typedef char size_model[(sizeof(D_8014A250_Record)==2056)?1:-1];
typedef char model_pos[(OFFSETOF(D_8014A250_Record,pos)==0x794)?1:-1];
typedef char model_node[(OFFSETOF(D_8014A250_Record,unk7C6)==0x7C6)?1:-1];
typedef char size_nav[(sizeof(Nav)==44)?1:-1];
typedef char size_car[(sizeof(GameCar)==952)?1:-1];
typedef char car_nav[(OFFSETOF(GameCar,nav)==0x314)?1:-1];
"""
    (group/'group.c').write_text((HERE/'group.c').read_text()+assertions)
    (group/'group.json').write_text(json.dumps(spec))
    obj=work/'layout.o';score.compile_group(group,obj)
    assert score.compare(obj,NAME,show=0).accepted()
    return 16

def verification(work):
    obj=work/'candidate.o';score.compile_group(HERE,obj)
    proof,linked,metadata=object_proof(obj,work)
    metadata['O32_layout_assertions']=layout_proof(work)
    threshold_bytes=score.own_data().read(0x8012443c,4);literal=int.from_bytes(threshold_bytes,'big')
    assert threshold_bytes==struct.pack('>f',1e20)
    threshold=native.value(literal);host=host_build(work);target=score.targets()[NAME]
    coverage=set();branches={};records=[];cases=list(corpus())
    for c in cases:
        expected=oracle(c,threshold);h=host_case(host,c,literal);assert h==expected,('host',c,h,expected)
        a=native_case(target,c,threshold_bytes);b=native_case(linked,c,threshold_bytes)
        assert a==b and a[0]==expected,('native',c,a[0],expected)
        coverage|=a[1]
        for pc,outcomes in a[2].items():branches.setdefault(pc,set()).update(outcomes)
        records.append([c['prev'],c['cur'],c['row'],c['point'],expected,len(a[3])])
    # These skipped native instructions are unconditional/likely delay-slot twins.
    missing=sorted(set(range(0,540,4))-coverage)
    mutation_tests={}
    source=(HERE/'group.c').read_text()
    mutations={
      'ignore_y':('dist = dx * dx + dy * dy + dz * dz;','dist = dx * dx + dz * dz;'),
      'accept_equal':('if (dist < bestd) {','if (dist <= bestd) {'),
      'omit_lag':('idx = ptIdx + d - win;','idx = ptIdx - win;'),
      'reset_to_zero':('idx = D_80151CE8[D_80151CE8[0].last].start[cur];','idx = 0;'),
      'too_small_window':('win = win * 2 + 1;','win = win + 1;')}
    for label,(before,after) in mutations.items():
        assert source.count(before)==1
        directory=work/label;directory.mkdir();mutant=host_build(directory,source.replace(before,after))
        rejected=sum(host_case(mutant,c,literal)!=oracle(c,threshold) for c in cases[:512])
        assert rejected,(label,'not caught');mutation_tests[label]=rejected
    # Decoder and exact-object checks must fail closed under real adverse inputs.
    try:native_case([0xffffffff]+target[1:],cases[0],threshold_bytes)
    except AssertionError:unknown=True
    else:raise AssertionError('unknown opcode accepted')
    assert coverage==set(range(0,540,4))-{520}
    assert all(v=={False,True} for k,v in branches.items() if k not in (164,252,512))
    return {'status':'MATCH','claimed_function':NAME,'base_commit':'e24b47d89a0c8ffade1e4c75ad76b9d390a1c232',
      'objects':proof,'object_metadata':metadata,'caller_contract':caller_contract(),'behavior':{'cases':len(cases),'native_executions':len(cases)*2,
      'host':'unchanged group.c, C89, UBSan','oracle':'independent bounded integer traversal and binary32 squared distance',
      'instruction_coverage':len(coverage),'total_instructions':135,'unexecuted_offsets':missing,
      'branches':{str(k):sorted(v) for k,v in sorted(branches.items())},'case_result_sha256':sha(json.dumps(records).encode()),
      'wrong_contracts_rejected':mutation_tests,'unknown_opcode_rejected':unknown},
      'protected_inputs_sha256':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in [ROOT/'asm/us/blob/SHA256SUMS',ROOT/'asm/us/blob/symbols.json',ROOT/'asm/us/blob_data/SHA256SUMS',ROOT/'tools/cloud/score.py',ROOT/'tools/cloud/owndata.py']},
      'source_sha256':{p.name:sha(p.read_bytes()) for p in [HERE/'group.c',HERE/'group.json',HERE/'native.py',HERE/'host.c',HERE/'verify.py']},
      'toolchain_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uopt','ugen','uld','as1']}}

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--output');args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='nearest-path-proof-') as temp:result=verification(Path(temp))
    text=json.dumps(result,indent=2,sort_keys=True)+'\n'
    if args.output:Path(args.output).write_text(text)
    print(text)
if __name__=='__main__':main()
