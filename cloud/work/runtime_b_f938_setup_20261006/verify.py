#!/usr/bin/env python3
"""Portable, source/native-word-bound F938 research replay; no native dump output."""
import argparse
from dataclasses import asdict
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
FN='func_8038F938';ENTRY=0x8038F938;SIZE=928

def sha(x):return hashlib.sha256(x).hexdigest()
def run(args):
    p=subprocess.run([str(x) for x in args],text=True,capture_output=True)
    assert p.returncode==0,(args,p.stdout,p.stderr)
    return p.stdout

def image(reference):
    asset=subprocess.check_output(['git','-C',str(reference),'show',BASE+':assets/us/data.bin'])
    assert sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
    main=zlib.decompress(asset[0xB0CB10-0x283D0:],-15)
    assert len(main)==647072 and sha(main)=='bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
    contracts=json.loads((HERE/'contracts.json').read_text())
    assert contracts['base']==BASE
    for helper in contracts['helpers']:
        start=int(helper['address'],16)-0x80086A50
        assert sha(main[start:start+helper['size']])==helper['native_sha256']
        # Historical production context is read from the stated commit; no
        # mutable production-file hash or live lock assertion is pinned.
        context=subprocess.check_output(['git','-C',str(reference),'show',BASE+':'+helper['historical_source']])
        assert context,helper['historical_source']
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
    return [(a,raw[a-0x8038A400:a-0x8038A400+n]) for a,n in
            [(0x80394358,13*4),(0x8039438C,4),(0x80394E98,32)]]


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
    bindings={}
    for n in undefined:
        if n.startswith('D_') or n.startswith('func_'):bindings[n]=int(n.split('_')[-1],16)
        elif n=='math_utility':bindings[n]=0x8008D6B0
        else:raise AssertionError(('unexpected external symbol',n))
    script=tmp/'root.ld'
    script.write_text('\n'.join(f'{n} = 0x{a:08X};' for n,a in sorted(bindings.items()))+
      '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
      '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    linked=tmp/'root.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    lm,ss,code,ro=elf(score,linked)
    assert not [n for n,s in ss.items() if s['section']==0]
    for n,a in bindings.items():assert ss[n]['value']==a and ss[n]['section']==0xFFF1
    om['relocations']=relocations
    return om,lm,code,ss[FN]['value'],ro,bindings

def layout(score,tmp):
    obj=tmp/'layout.o';score.compile_single(HERE/'layout.c',FLAGS,obj)
    data,secs=score._elf(obj)
    sec=next(s for s in secs if s['name']=='.data')
    facts=list(struct.unpack('>15I',data[sec['off']:sec['off']+60]))
    expected=[20,952,2056,52,4,8,44,908,929,930,936,8,1600,4,40]
    assert facts==expected
    return facts

def negatives(score,tmp,native,tables,code,start,ro):
    source=(HERE/'setup.c').read_text()
    mutations={
      'wrong_transition_boundary':('status->transition_timer <= 0.0f','status->transition_timer < 0.0f',[dict(flags=1,transition_timer=0.125)]),
      'wrong_blocked_clear_mask':('status->flags &= ~0x1E;','status->flags &= ~0x1C;',[dict(flags=3,blocked=1)]),
      'wrong_grow_increment':('status->scale += 0.05f;','status->scale += 0.04f;',[dict(flags=10)]),
      'wrong_hold_duration':('status->scale_timer = 30.0f;','status->scale_timer = 3.0f;',[dict(flags=10,scale=2.0)]),
      'wrong_shrink_boundary':('status->scale <= 0.06f','status->scale < 0.06f',[dict(flags=18,scale=0.06)]),
      'wrong_alpha_signedness':('if (color.rgba[3] < 48)', 'if ((s8) color.rgba[3] < 48)', [dict(alpha=255)]),
      'wrong_alpha_floor':('color.rgba[3] < 48','color.rgba[3] < 47',[dict(alpha=47)]),
      'missing_full_scene_clear':('effect->scene_index = -1;','/* omitted clear */',[dict(blocked=1,scene=5)]),
      'wrong_color_handle_narrowing':('func_8008E06C((s16) effect->scene_index','func_8008E06C((s16) (effect->scene_index + 1)',[dict(scene=5)]),
    }
    rejected=[]
    for name,(old,new,cases) in mutations.items():
        assert old in source
        mutated=tmp/(name+'.c');mutated.write_text(source.replace(old,new))
        _,_,cc,st,rr,_=build(score,mutated,tmp,FLAGS)
        try:verify(native,tables,cc,st,tables+rr,cases)
        except AssertionError as exc:
            assert ('semantic mismatch' in str(exc) or name=='wrong_color_handle_narrowing' and 'unmapped/readonly write' in str(exc)),(name,str(exc));rejected.append(name)
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
    spec=importlib.util.spec_from_file_location('score_f938',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        raise SystemExit('pinned IDO and MIPS GNU linker required')
    score.ASM_DIR=ROOT/'asm/us/ovl_b';targets=score.targets();words=targets[FN]
    raw=image(args.reference_root);target=struct.pack('>'+str(len(words))+'I',*words)
    assert len(target)==SIZE and target==raw[ENTRY-0x8038A400:ENTRY-0x8038A400+SIZE]
    assert sha(target)=='1b5eb1e66549eb1cc0fba50a3dc3e432e295fde04bf1777be2a5bcdfccd9e106'
    native={ENTRY+4*i:w for i,w in enumerate(words)};tables=inputs(raw)
    with tempfile.TemporaryDirectory(prefix='f938-root-') as td:
        om,lm,code,start,ro,bindings=build(score,HERE/'setup.c',Path(td),FLAGS)
        cases=[{},dict(flags=1),dict(blocked=1),dict(flags=18,scale=0.06),dict(flags=10,scale=2.0)] if args.quick else None
        behavior=verify(native,tables,code,start,tables+ro,cases)
        facts=layout(score,Path(td))
        controls=[] if args.quick else negatives(score,Path(td),native,tables,code,start,ro)
    result={'status':'COMPLETE-SEMANTIC-SOURCE / RESEARCH-ONLY; native private ABI not reproduced',
      'provenance':{'compiler_tools_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ['cc','cfe','uopt','ugen','as1']},
        'gnu_ld_sha256':sha(Path(shutil.which('mips-linux-gnu-ld')).read_bytes()),
        'gnu_ld_version':run(['mips-linux-gnu-ld','--version']).splitlines()[0],
        'build_route':'unchanged canonical score.compile_single; whole object linked; no group or keep list'},
      'base':BASE,'target':{'name':FN,'entry':hex(ENTRY),'size':SIZE,'sha256':sha(target)},
      'flags':FLAGS,'canonical_added_backend_flag':'-Wab,-r4300_mul',
      'sources_sha256':{n:sha((HERE/n).read_bytes()) for n in ['setup.c','native.py','verify.py','layout.c','contracts.json']},
      'object':om,'linked':lm,'external_bindings':{n:hex(a) for n,a in sorted(bindings.items())},
      'behavior':behavior,'layout_facts':facts,'negative_controls':controls,
      'nonmatch':{'different_word_positions':sum(code.get(start+i*4)!=w for i,w in enumerate(words)),
        'native_words':len(words),'candidate_words':len(code),
        'extra_nonzero_words':sum(w!=0 for a,w in code.items() if a>=start+SIZE),
        'scope':'full linked research-placement words; no native owned-data placement or strict match claim'},
      'native_tables':[{'address':hex(a),'size':len(b),'sha256':sha(b)} for a,b in tables],
      'limits':'Ordinary four-input semantic adapter; actual native entry remains private s0/s2/s3/s5. External service bodies are bounded models; no match or ROM claim'}
    if args.check:
        old=json.loads(args.output.read_text())
        # Compiler/tool provenance is reported honestly but is not itself a
        # portable proof invariant; fresh words/layout/effects must agree.
        stable=lambda x:{k:v for k,v in x.items() if k!='provenance'}
        assert stable(result)==stable(old),'receipt drift'
    else:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'candidate_bytes':lm['functions'][FN]['size'],
                     'cases':behavior['paired_cases'],'native_covered':len(behavior['native_executed_offsets']),
                     'native_unexecuted':behavior['native_unexecuted_offsets'],
                     'candidate_covered':len(behavior['candidate_executed_offsets'])},indent=2))
if __name__=='__main__':main()
