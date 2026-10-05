#!/usr/bin/env python3
"""Source-bound NONMATCH receipt: exact extent, GNU relocations and bounded semantics."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('render_allocator_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='func_800A79F4'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE=HERE/'candidate.c'
CALLER=ROOT/'cloud/work/frontier/dot_new_multiblit_20261005/new_blit_nonmatch.c'
SETTER=ROOT/'src/blob/Input_InitPadHandlers.c'
FIELDS=[('word0',4),('word4',4),('half8',2),('halfA',2),('halfC',2),('padE',2),
        ('half10',2),('half12',2),('alpha',1),('flip',1),('state',1),('flags',1),
        ('top',2),('bot',2),('left',2),('right',2)]
class Record(ctypes.Structure):
    _fields_=[(name,{1:ctypes.c_uint8,2:ctypes.c_uint16,4:ctypes.c_uint32}[width]) for name,width in FIELDS]
assert ctypes.sizeof(Record)==32
MUTANTS={
 'reuse_zero':('D_80140BF0[i].state == 2','D_80140BF0[i].state == 0'),
 'capacity_199':('i >= 200','i >= 199'),
 'omit_high_water':('D_8013C234 = D_801613AC;','D_8013C234 = D_8013C234;'),
 'swap_crop_axes':('r->left = h12 - 1;','r->left = h10 - 1;'),
 'omit_flip_clear':('r->flip = 0;','/* wrong: preserve flip */'),
}

def sha(b):return hashlib.sha256(b).hexdigest()

def symbol(obj,name):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    found=[s for s in syms if s['name']==name and s['type']==2 and s['section']==ti]
    assert len(found)==1,(name,found)
    return found[0]

def inspect(obj,name):
    fn=symbol(obj,name);start=fn['value'];end=start+fn['size'];want=score.targets()[name]
    got,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),start,end,score.image_symbols())
    c=score.compare(obj,name,show=0)
    return dict(asdict(c),verdict=c.summary(),symbol_bytes=fn['size'],native_bytes=4*len(want),
                exact_extent=fn['size']==len(want)*4,
                differing_offsets=[i*4 for i,(a,b) in enumerate(zip(want,got[start//4:end//4])) if a!=b],
                all_relocations_resolved=not(masks or unresolved or unverified or errors))

def code_proof(work):
    obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
    out=inspect(obj,FN)
    assert out['symbol_bytes']==240 and out['differing']==23 and out['all_relocations_resolved']
    data,secs=score._elf(obj);ti=score._text_index(secs);txt=secs[ti]
    assert symbol(obj,FN)['value']==0 and txt['size']==240
    assert all(s['size']==0 for s in secs if s['name'] in ('.rodata','.data','.bss','.sdata','.sbss','.lit4','.lit8'))
    relocs=[]
    for sec in secs:
        if sec['type']==9:
            assert sec['info']==ti
            symbols=score._symbol_table(data,secs,sec['link'])
            for p in range(sec['off'],sec['off']+sec['size'],8):
                offset,info=struct.unpack_from('>II',data,p)
                relocs.append({'offset':offset,'type':info&255,'symbol':symbols[info>>8]['name']})
    assert len(relocs)==8 and {r['type'] for r in relocs}=={5,6}
    addresses=score.image_symbols()
    for r in relocs:
        n=r['symbol']
        if n not in addresses:
            assert n.startswith('D_') and len(n)==10
            addresses[n]=int(n[2:],16)
    script=work/'proof.ld'
    script.write_text('SECTIONS { .text 0x800A79F4 : SUBALIGN(4) { *(.text) } }\n'+
                      ''.join('%s = 0x%x;\n'%(n,addresses[n]) for n in sorted({r['symbol'] for r in relocs})))
    elf=work/'proof.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,lsecs=score._elf(elf);ltxt=lsecs[score._text_index(lsecs)]
    body=linked[ltxt['off']:ltxt['off']+240];words=list(struct.unpack('>60I',body))
    assert symbol(elf,FN)['value']==native.BASE and symbol(elf,FN)['size']==240
    want=score.targets()[FN]
    differences=[i*4 for i,(a,b) in enumerate(zip(want,words)) if a!=b]
    assert differences==out['differing_offsets'] and differences[0]==128
    out.update(relocations=relocs,own_data_bytes=0,text_alignment_bytes=0,
               native_body_sha256=sha(struct.pack('>60I',*want)),
               linked_body_sha256=sha(body),independent_gnu_differing_offsets=differences,
               full_body_equal=False,unchanged_prefix_bytes=128)
    # Fixed compiler/source controls. The genuine helper body is byte-for-byte source text.
    text=SOURCE.read_text();helper=SETTER.read_text().split('void Input_InitPadHandlers',1)[1]
    helper='void Input_InitPadHandlers'+helper
    treated=text.replace('s32 func_800A79F4(',helper+'\ns32 func_800A79F4(')
    for assignment in ['word4 = w4','word0 = w0','half8 = h8','halfA = hA','half10 = h10','half12 = h12','halfC = hC']:
        assert treated.count('    r->'+assignment+';\n')==1
        treated=treated.replace('    r->'+assignment+';\n','')
    call='    Input_InitPadHandlers(i, h8, w4, w0, hA, hC, h10, h12);\n'
    before=treated.replace('    r->padE = 0;',call+'    r->padE = 0;')
    after=treated.replace('    return i;',call+'    return i;')
    caller_types=text.replace('u32 h8, u32 w4, u32 w0, u32 hA, u32 hC, u32 h10, u32 h12',
                             'u16 h8, void *w4, void *w0, s32 hA, s32 hC, s32 h10, s32 h12')
    caller_types=caller_types.replace('r->word4 = w4','r->word4 = (u32)w4').replace('r->word0 = w0','r->word0 = (u32)w0')
    controls={}
    for label,body,flags in [('o2',text,FLAGS.replace('-O3','-O2')),('genuine_setter_before',before,FLAGS),
                             ('genuine_setter_after',after,FLAGS),('historical_caller_types',caller_types,FLAGS)]:
        src=work/(label+'.c');src.write_text(body);o=work/(label+'.o');score.compile_single(src,flags,o)
        controls[label]=inspect(o,FN)
        if label.startswith('genuine_setter'):
            controls[label]['accepted_setter']=inspect(o,'Input_InitPadHandlers')
            assert controls[label]['accepted_setter']['verdict']=='MATCH'
    assert controls['genuine_setter_before']['differing']==58
    assert controls['genuine_setter_after']['differing']==59
    assert controls['historical_caller_types']['differing']==48
    # Real caller body, ordinary exported visibility. Normalize only its old prototype
    # to this packet's bit-carrier interface; there is no claimed caller match.
    group=work/'context';group.mkdir()
    shutil.copy2(SOURCE,group/'candidate.c')
    caller=CALLER.read_text()
    old='extern s32 func_800A79F4(u16, void *, void *, s32, s32, s32, s32);'
    new='extern s32 func_800A79F4(u32, u32, u32, u32, u32, u32, u32);'
    assert caller.count(old)==1
    (group/'caller.c').write_text(caller.replace(old,new))
    (group/'group.json').write_text(json.dumps({'files':['candidate.c','caller.c'],'members':[FN],
         'context':['func_800B3704'],'keep':[FN,'func_800B3704'],'flags':FLAGS}))
    score.compile_group(group,group/'group.o')
    context={n:inspect(group/'group.o',n) for n in [FN,'func_800B3704']}
    assert context[FN]['differing']==23 and context['func_800B3704']['differing']==14
    context['caller_prototype_normalization']={'old':old,'new':new,'body_unchanged':True,
                                              'compiled_caller_sha256':sha((group/'caller.c').read_bytes())}
    return out,controls,context,words

def cases():
    rng=random.Random(0xA79F4)
    template=bytes(rng.randrange(256) for _ in range(6400))
    payloads=[0,1,0xffff,0x10000,0x7fffffff,0x80000000,0xffffffff,0x12345678]
    def make(count,free,high,args):
        pool=bytearray(template)
        for i in range(200):pool[32*i+22]=[0,1,3,127,128,255][i%6]
        for i in free:pool[32*i+22]=2
        return bytes(pool),count,high,tuple(x&0xffffffff for x in args)
    for free in range(200):
        yield make(200,[free],(-1,199,200,201)[free%4],[rng.getrandbits(32) for _ in range(7)])
    for count in [-3,-2,-1,0,1,2,50,199,200]:
        for high in [-2147483648,-1,0,199,200,2147483647]:
            yield make(count,[],high,[payloads[(i+count)%8] for i in range(7)])
    for k in range(8):
        yield make(2,[0,1],0,[payloads[(k+i)%8] for i in range(7)])
    for _ in range(128):
        count=rng.randrange(201);free=[i for i in range(count) if rng.randrange(4)==0]
        yield make(count,free,rng.randrange(-1,202),[rng.getrandbits(32) for _ in range(7)])

def host_library(work,source=SOURCE,label='host'):
    src=work/(label+'.c');so=work/(label+'.so')
    src.write_text('#include "'+str(source)+'"\n#include <stddef.h>\nRecord D_80140BF0[200];\ns32 D_801613AC,D_8013C234;\n'
                   'typedef char size_record[(sizeof(Record)==32)?1:-1];\n'
                   'typedef char state_offset[(offsetof(Record,state)==22)?1:-1];\n'
                   'typedef char right_offset[(offsetof(Record,right)==30)?1:-1];\n')
    subprocess.run(['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-O1','-fPIC','-shared',
                    '-fsanitize=undefined','-fno-sanitize-recover=all',str(src),'-o',str(so)],check=True,capture_output=True)
    lib=ctypes.CDLL(str(so));fn=getattr(lib,FN);fn.argtypes=[ctypes.c_uint32]*7;fn.restype=ctypes.c_int32
    return lib

def host_run(lib,pool,count,high,args):
    records=(Record*200).in_dll(lib,'D_80140BF0')
    for i,r in enumerate(records):
        off=i*32
        for name,width in FIELDS:
            setattr(r,name,int.from_bytes(pool[off:off+width],'big'));off+=width
    ctypes.c_int32.in_dll(lib,'D_801613AC').value=count
    ctypes.c_int32.in_dll(lib,'D_8013C234').value=high
    result=getattr(lib,FN)(*args)
    out=bytearray()
    for r in records:
        for name,width in FIELDS:out.extend(getattr(r,name).to_bytes(width,'big'))
    return result,bytes(out),ctypes.c_int32.in_dll(lib,'D_801613AC').value,ctypes.c_int32.in_dll(lib,'D_8013C234').value

def behavior(work,linked):
    lib=host_library(work);want=score.targets()[FN];native_seen=set();linked_seen=set();allcases=list(cases())
    for case in allcases:
        expected=native.reference(*case)
        a=native.Machine(want,*case);b=native.Machine(linked,*case)
        assert a.run()==b.run()==host_run(lib,*case)==expected
        native_seen.update(a.visited);linked_seen.update(b.visited)
    assert len(native_seen)==len(linked_seen)==60
    mutants={}
    text=SOURCE.read_text()
    for label,(old,new) in MUTANTS.items():
        assert text.count(old)==1
        path=work/(label+'_candidate.c');path.write_text(text.replace(old,new))
        altered=host_library(work,path,label)
        witness=next((i for i,c in enumerate(allcases) if host_run(altered,*c)!=native.reference(*c)),None)
        assert witness is not None,label
        mutants[label]={'rejected':True,'case_index':witness}
    bad=want[:];bad[0]=0xfc000000
    try:native.Machine(bad,*allcases[0]).run()
    except AssertionError:pass
    else:raise AssertionError('unknown opcode was not rejected')
    return {'cases':len(allcases),'routes':['protected_native','gnu_linked_nonmatch','unchanged_host_c89_ubsan','independent_record_oracle'],
            'native_executions':2*len(allcases),'native_instruction_offsets_executed':len(native_seen),
            'linked_instruction_offsets_executed':len(linked_seen),'all_state_equal':True,'wrong_contract_source_mutants':mutants,
            'unknown_instruction_rejected':True,'domain':'count -3..200, 200 accessible records; arbitrary 32-bit payloads and signed high-water values'}

def verify(work):
    obj,controls,context,linked=code_proof(work)
    return {'base_revision':'e0e734babdac3c6a79d2f87f7f895e34aa170148','function':FN,
            'range':['0x800A79F4','0x800A7AE4'],'status':'NONMATCH','accepted_byte_gain':0,'flags':FLAGS,
            'source_sha256':sha(SOURCE.read_bytes()),'native_executor_sha256':sha((HERE/'native.py').read_bytes()),
            'actual_caller_source_sha256':sha(CALLER.read_bytes()),'accepted_setter_source_sha256':sha(SETTER.read_bytes()),
            'object':obj,'controls':controls,'genuine_caller_context':context,'semantics':behavior(work,linked)}

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--record',action='store_true');ap.add_argument('--output',type=Path);args=ap.parse_args()
    with tempfile.TemporaryDirectory(prefix='rush-render-allocator-') as directory:result=verify(Path(directory))
    frozen=HERE/'verification.json'
    if args.record:frozen.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:assert result==json.loads(frozen.read_text()),'source or functional receipt changed; review required'
    if args.output:args.output.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps({'status':result['status'],'differing_words':result['object']['differing'],'native_words':60,
                      'symbol_bytes':result['object']['symbol_bytes'],'cases':result['semantics']['cases'],
                      'mutants_rejected':len(result['semantics']['wrong_contract_source_mutants'])}))
if __name__=='__main__':main()
