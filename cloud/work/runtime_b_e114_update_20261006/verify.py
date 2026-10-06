#!/usr/bin/env python3
"""Portable, source/native-word-bound E114 research replay; no native dump output."""
import argparse
import hashlib
import importlib.util
import json
import shutil
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib
from native import verify, bits

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
FN='func_8038E114';ENTRY=0x8038E114;SIZE=5196

def sha(x):return hashlib.sha256(x).hexdigest()
def run(args):
    p=subprocess.run([str(x) for x in args],text=True,capture_output=True)
    assert p.returncode==0,(args,p.stdout,p.stderr)
    return p.stdout

def image(reference):
    asset=subprocess.check_output(['git','-C',str(reference),'show',BASE+':assets/us/data.bin'])
    assert sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
    dec=zlib.decompressobj(-15);data=dec.decompress(asset[0xB6FEC4-0x283D0:])
    assert dec.eof and len(data)==43888
    assert sha(data)=='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
    return data

def elf(score,path):
    data,sections=score._elf(path);header=struct.unpack_from('>16sHHIIIIIHHHHHH',data)
    assert header[2]==8
    syms={s['name']:s for i,s in enumerate(sections) if s['type']==2
          for s in score._symbol_table(data,sections,i) if s['name']}
    funcs={n:s for n,s in syms.items() if s['type']==2 and s['section'] not in (0,0xFFF1)}
    assert set(funcs)=={FN}
    allocated=[];code={};rodata=[]
    for i,s in enumerate(sections):
        h=struct.unpack_from('>IIIIIIIIII',data,header[6]+i*header[11])
        if h[2]&2 and h[5]:
            assert s['name'] in ('.text','.rodata','.reginfo','.options'),s['name']
            raw=data[h[4]:h[4]+h[5]]
            allocated.append({'name':s['name'],'size':h[5],'address':hex(h[3]),'sha256':sha(raw)})
            if s['name']=='.text':
                f=funcs[FN];off=f['value']-h[3]
                assert off==0 and f['size']<=h[5]
                assert not any(raw[f['size']:]),'nonzero text outside full function'
                code={f['value']+j:w for j,(w,) in zip(range(0,f['size'],4),struct.iter_unpack('>I',raw[:f['size']]))}
            elif s['name']=='.rodata':rodata.append((h[3],raw))
        if header[1]==2:assert s['type'] not in (4,9) or s['size']==0
    return {'functions':{n:{'address':hex(s['value']),'size':s['size']} for n,s in funcs.items()},
            'allocated':allocated},syms,code,rodata

def inputs(raw):
    tables=[]
    for a,n in [(0x80394390,4),(0x803943A4,8*13*12),(0x80394B08,9*12),
                (0x80394E18,0x74),(0x803942C0,9*4)]:
        tables.append((a,raw[a-0x8038A400:a-0x8038A400+n]))
    return tables

def build(score,source,tmp,flags):
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        raise RuntimeError('pinned IDO and MIPS GNU linker required')
    obj=tmp/'update.o';score.compile_single(source,flags,obj)
    om,syms,_,_=elf(score,obj)
    undefined={n for n,s in syms.items() if s['section']==0}
    data,sections=score._elf(obj)
    referenced=set();relocations=[]
    for sec in sections:
        if sec['type']==9:
            table=score._symbol_table(data,sections,sec['link'])
            for off,info in struct.iter_unpack('>II',data[sec['off']:sec['off']+sec['size']]):
                name=table[info>>8]['name'];referenced.add(name)
                relocations.append({'section':sec['name'],'offset':off,'type':info&255,'symbol':name})
    assert 'fabsf' not in referenced, 'fabsf must be a native intrinsic'
    undefined.discard('fabsf')
    bindings={}
    for n in undefined:
        if n.startswith('D_') or n.startswith('func_'):bindings[n]=int(n.split('_')[-1],16)
        elif n=='math_utility':bindings[n]=0x8008D6B0
        elif n.startswith('private_'):bindings[n]=0x80380000+int(n.split('_')[1],16)
        else:raise AssertionError(('unexpected external symbol',n))
    script=tmp/'update.ld'
    script.write_text('\n'.join(f'{n} = 0x{a:08X};' for n,a in sorted(bindings.items()))+
      '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
      '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    linked=tmp/'update.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    lm,ss,code,ro=elf(score,linked)
    assert not [n for n,s in ss.items() if s['section']==0 and n!='fabsf']
    for n,a in bindings.items():assert ss[n]['value']==a and ss[n]['section']==0xFFF1
    om['relocations']=relocations
    return om,lm,code,ss[FN]['value'],ro,bindings


def layout(score,tmp):
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        raise RuntimeError('pinned IDO and MIPS GNU linker required')
    obj=tmp/'layout.o';score.compile_single(HERE/'layout.c',FLAGS,obj)
    data,secs=score._elf(obj);sec=next(x for x in secs if x['name']=='.data')
    values=list(struct.unpack('>42I',data[sec['off']:sec['off']+168]))
    assert values==[60,28,104,952,2056,68,36,8,4,8,44,4,6,7,8,12,16,20,32,44,56,92,96,100,
                   844,776,857,896,900,901,908,928,929,932,940,8,1600,12,0,4,60,16]
    return values

def negatives(score,tmp,native,tables):
    source=(HERE/'update.c').read_text()
    mutations={
      'wrong_gravity':(' * 2.5f',' * 2.0f',[dict(kind=2)]),
      'wrong_expiry_boundary':('record->lifetime <= 0.0f','record->lifetime < 0.0f',[dict(life=0.125)]),
      'wrong_quad_corner':('corners[3][i] = record->position[i] - side[i]',
        'corners[3][i] = record->position[i] + side[i]',[dict(collision=1)]),
      'wrong_previous_collision_input':('hit = private_DA78(record, record->previous_position)',
        'hit = private_DA78(record, previous)',[dict(kind=4)]),
      'wrong_tick_constant':('0.0333333015f','0.0333333333f',[dict(kind=7)]),
      'wrong_phase_priority':('if (record->flags & 0x10)',
        'if (record->flags & 0x20)',[dict(kind=7,flags=0x10)]),
      'wrong_color_minimum':('player->alpha < 48','player->alpha < 16',
        [dict(kind=3,timer=0.0,latched=1,primary=0,status=1,alpha=20)]),
      'wrong_saved_next':('record = next;', 'record = record->next;', [dict(count=2,mutation=1)]),
      'invented_destroy_on_state1':('player->latched = 0;\n                    func_800AFA84',
        'player->latched = 0;\n                    private_E088(record);\n                    func_800AFA84',
        [dict(kind=3,timer=0.0,latched=1,active=0)]),
    }
    rejected=[]
    for name,(old,new,cases) in mutations.items():
        if not cases:continue
        assert old in source
        mutated=tmp/(name+'.c');mutated.write_text(source.replace(old,new))
        _,_,code,start,ro,_=build(score,mutated,tmp,FLAGS)
        try:verify(native,tables,code,start,tables+ro,cases)
        except AssertionError as exc:
            assert 'semantic mismatch' in str(exc) or (name=='wrong_saved_next' and 'unmapped' in str(exc)),(name,str(exc))
            rejected.append(name)
        else:raise AssertionError(('wrong source accepted',name))
    from native import Machine
    for name,case in [('null_release',dict(collision=1,null_release=1)),
                      ('null_object',dict(kind=0,primary=0,null_object=1))]:
        try:Machine(native,ENTRY,tables,case,True).run()
        except AssertionError as exc:assert 'unmapped' in str(exc)
        else:raise AssertionError((name,'failure domain accepted'))
        rejected.append(name+' refused')
    try:Machine({ENTRY:0xFFFFFFFF},ENTRY,tables,{},True).run()
    except AssertionError as exc:assert 'opcode' in str(exc)
    else:raise AssertionError('unknown opcode accepted')
    return rejected+['unknown opcode refused']

def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path,default=HERE/'verification.json')
    p.add_argument('--check',action='store_true');p.add_argument('--quick',action='store_true')
    args=p.parse_args()
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('score_e114',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=ROOT/'asm/us/ovl_b';targets=score.targets();words=targets[FN]
    raw=image(args.reference_root);target=struct.pack('>'+str(len(words))+'I',*words)
    assert len(target)==SIZE and target==raw[ENTRY-0x8038A400:ENTRY-0x8038A400+SIZE]
    assert sha(target)=='d1dc2c70973b1780b81b37730a974fc4921b14ab4d368437d899a6fe9f7ef8e0'
    native={ENTRY+4*i:w for i,w in enumerate(words)};tables=inputs(raw)
    with tempfile.TemporaryDirectory(prefix='e114-root-') as td:
        om,lm,code,start,ro,bindings=build(score,HERE/'update.c',Path(td),FLAGS)
        cases=[{},dict(kind=1),dict(kind=3),dict(kind=7),dict(kind=8)] if args.quick else None
        behavior=verify(native,tables,code,start,tables+ro,cases)
        facts=layout(score,Path(td));controls=[] if args.quick else negatives(score,Path(td),native,tables)
    result={'status':'COMPLETE-SEMANTIC-SOURCE / RESEARCH-ONLY; private closure incomplete',
      'base':BASE,'target':{'name':FN,'entry':hex(ENTRY),'size':SIZE,'sha256':sha(target)},
      'flags':FLAGS,'canonical_added_backend_flag':'-Wab,-r4300_mul',
      'sources_sha256':{n:sha((HERE/n).read_bytes()) for n in ['update.c','native.py','verify.py','layout.c']},
      'object':om,'linked':lm,'external_bindings':{n:hex(a) for n,a in sorted(bindings.items())},
      'behavior':behavior,'layout_facts':facts,'negative_controls':controls,
      'native_tables':[{'address':hex(a),'size':len(b),'sha256':sha(b)} for a,b in tables],
      'fixture_tables':'main-image textures, scene data and identity matrix are synthetic bounded fixtures',
      'limits':'Semantic ordinary-ABI helper interfaces; actual private DA78/D498 and child bodies not linked; no match or ROM claim'}
    if args.check:assert result==json.loads(args.output.read_text()),'receipt drift'
    else:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'candidate_bytes':lm['functions'][FN]['size'],
                     'cases':behavior['paired_cases'],'native_covered':len(behavior['native_executed_offsets']),
                     'native_unexecuted':behavior['native_unexecuted_offsets'],
                     'candidate_covered':len(behavior['candidate_executed_offsets'])},indent=2))
if __name__=='__main__':main()
