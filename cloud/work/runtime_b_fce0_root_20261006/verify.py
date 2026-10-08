#!/usr/bin/env python3
"""Portable, source/native-word-bound FCE0 research replay; no native dump output."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
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
FN='func_8038FCE0';ENTRY=0x8038FCE0;SIZE=3056

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
    for a,n in [(0x803942C0,9*4),(0x803943A4,8*13*12),(0x80394AFC,12+9*12),(0x80394EB8,0x50)]:
        tables.append((a,raw[a-0x8038A400:a-0x8038A400+n]))
    # Main-image vehicle bounds are explicit fixtures, not authenticated original data.
    bounds=b''.join(struct.pack('>4f',1.0+i*0.125,0.75+i*0.25,2.0,1.5) for i in range(13))
    tables.append((0x8011F844,bounds))
    return tables

def build(score,source,tmp,flags):
    obj=tmp/'root.o';score.compile_single(source,flags,obj)
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
    assert 'sqrtf' not in referenced, 'sqrtf must be a native intrinsic'
    undefined.discard('sqrtf')
    bindings={}
    for n in undefined:
        if n.startswith('D_') or n.startswith('func_'):bindings[n]=int(n.split('_')[-1],16)
        elif n=='math_utility':bindings[n]=0x8008D6B0
        elif n=='private_F938':bindings[n]=0x8038F938
        elif n=='private_E114':bindings[n]=0x8038E114
        else:raise AssertionError(('unexpected external symbol',n))
    script=tmp/'root.ld'
    script.write_text('\n'.join(f'{n} = 0x{a:08X};' for n,a in sorted(bindings.items()))+
      '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
      '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    linked=tmp/'root.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    lm,ss,code,ro=elf(score,linked)
    assert not [n for n,s in ss.items() if s['section']==0 and n!='sqrtf']
    for n,a in bindings.items():assert ss[n]['value']==a and ss[n]['section']==0xFFF1
    om['relocations']=relocations
    return om,lm,code,ss[FN]['value'],ro,bindings

def layout(score,tmp):
    obj=tmp/'layout.o';score.compile_single(HERE/'layout.c',FLAGS,obj)
    data,secs=score._elf(obj)
    sec=next(s for s in secs if s['name']=='.data')
    facts=list(struct.unpack('>25I',data[sec['off']:sec['off']+100]))
    expected=[104,72,952,2056,92,96,776,857,859,896,900,901,908,928,932,940,944,948,252,264,1600,1620,48,60,16]
    assert facts==expected
    return facts

def negatives(score,tmp,native,tables,code,start,ro):
    source=(HERE/'root.c').read_text()
    mutations={
      'wrong_gravity':(' * 2.5f',' * 2.0f',[dict(debris=1,debris_life=1.0)]),
      'wrong_ammo_exemption':("player->kind != 3)","player->kind != 4)",[dict(kind=3)]),
      'wrong_effect_mode':('effect_mode = 2;','effect_mode = 1;',[dict(kind=6)]),
      'drop_real_parent':('    private_E114();','    /* missing parent */',[dict(count=0)]),
      'wrong_position_offset':('offset[2] * player->uv[2][j]','offset[1] * player->uv[2][j]',[dict(kind=2)]),
      'wrong_expiry_boundary':('debris->lifetime <= 0.0f','debris->lifetime < 0.0f',[dict(debris=1)]),
    }
    rejected=[]
    for name,(old,new,cases) in mutations.items():
        assert old in source
        mutated=tmp/(name+'.c');mutated.write_text(source.replace(old,new))
        _,_,cc,st,rr,_=build(score,mutated,tmp,FLAGS)
        try:verify(native,tables,cc,st,tables+rr,cases)
        except AssertionError as exc:
            assert 'semantic mismatch' in str(exc),(name,str(exc));rejected.append(name)
        else:raise AssertionError(('wrong source accepted',name))
    from native import Machine
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
    spec=importlib.util.spec_from_file_location('score_fce0',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=ROOT/'asm/us/ovl_b';targets=score.targets();words=targets[FN]
    raw=image(args.reference_root);target=struct.pack('>'+str(len(words))+'I',*words)
    assert len(target)==SIZE and target==raw[ENTRY-0x8038A400:ENTRY-0x8038A400+SIZE]
    assert sha(target)=='3574383f8241f87f1aa201bc4a7265e49cd818b8b6f7e6564ecca4131b6682e2'
    native={ENTRY+4*i:w for i,w in enumerate(words)};tables=inputs(raw)
    with tempfile.TemporaryDirectory(prefix='fce0-root-') as td:
        om,lm,code,start,ro,bindings=build(score,HERE/'root.c',Path(td),FLAGS)
        cases=[{},dict(kind=1),dict(kind=3),dict(kind=5,ammo=0),dict(kind=8)] if args.quick else None
        behavior=verify(native,tables,code,start,tables+ro,cases)
        facts=layout(score,Path(td))
        controls=[] if args.quick else negatives(score,Path(td),native,tables,code,start,ro)
    result={'status':'COMPLETE-SEMANTIC-SOURCE / RESEARCH-ONLY; private closure incomplete',
      'base':BASE,'target':{'name':FN,'entry':hex(ENTRY),'size':SIZE,'sha256':sha(target)},
      'flags':FLAGS,'canonical_added_backend_flag':'-Wab,-r4300_mul',
      'sources_sha256':{n:sha((HERE/n).read_bytes()) for n in ['root.c','native.py','verify.py','layout.c']},
      'object':om,'linked':lm,'external_bindings':{n:hex(a) for n,a in sorted(bindings.items())},
      'behavior':behavior,'layout_facts':facts,'negative_controls':controls,
      'native_tables':[{'address':hex(a),'size':len(b),'sha256':sha(b)} for a,b in tables[:-1]],
      'fixture_bounds_table':'13 model rows; synthetic main-image data; does not prove real bounds values',
      'limits':'Semantic ordinary-ABI helper interfaces; actual private F938/E114 bodies not linked; no match or ROM claim'}
    if args.check:assert result==json.loads(args.output.read_text()),'receipt drift'
    else:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'candidate_bytes':lm['functions'][FN]['size'],
                     'cases':behavior['paired_cases'],'native_covered':len(behavior['native_executed_offsets']),
                     'native_unexecuted':behavior['native_unexecuted_offsets'],
                     'candidate_covered':len(behavior['candidate_executed_offsets'])},indent=2))
if __name__=='__main__':main()
