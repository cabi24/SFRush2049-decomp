#!/usr/bin/env python3
"""Portable, source/native-word-bound D498 research replay; no native dump output."""
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
from native import verify

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
FN='func_8038D498';ENTRY=0x8038D498;SIZE=768

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
    extents=json.loads(subprocess.check_output(['git','-C',str(reference),'show',BASE+':asm/us/ovl_b/extents.json']))
    extent=next(x for x in extents['functions'] if x['name']==FN)
    assert int(extent['address'],16)==ENTRY and extent['size']==SIZE
    for helper in contracts['helpers']:
        off=int(helper['address'],16)-0x80086A50
        assert sha(main[off:off+helper['size']])==helper['native_sha256']
        context=subprocess.check_output(['git','-C',str(reference),'show',BASE+':'+helper['historical_source']])
        assert helper['name'].encode() in context
    dec=zlib.decompressobj(-15);data=dec.decompress(asset[0xB6FEC4-0x283D0:])
    assert dec.eof and len(data)==43888
    assert sha(data)=='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
    damage=contracts['damage_helper']
    off=int(damage['address'],16)-0x8038A400
    assert sha(data[off:off+damage['bytes']])==damage['native_sha256']
    sites=[]
    for off in range(0,0x80393B34-0x8038A400,4):
        w=struct.unpack_from('>I',data,off)[0]
        if w>>26==3 and ((w&0x3FFFFFF)<<2 | 0x80000000)==ENTRY:
            sites.append(0x8038A400+off)
    assert sites==[int(x['address'],16) for x in contracts['callers']]
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
    return [(0x80394DC8,raw[0xA9C8:0xA9D4])]


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
    assert not ({'sqrtf'} & referenced), 'math primitives must compile intrinsically'
    undefined -= {'sqrtf'}
    assert undefined=={'D_80152818','D_8014A250','D_8012E700','D_801543CA','func_8038D3A4','func_800A61B0'},undefined
    bindings={}
    for n in undefined:
        if n.startswith('D_') or n.startswith('func_'):bindings[n]=int(n.split('_')[-1],16)
        else:raise AssertionError(('unexpected external symbol',n))
    script=tmp/'root.ld'
    script.write_text('\n'.join(f'{n} = 0x{a:08X};' for n,a in sorted(bindings.items()))+
      '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
      '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    linked=tmp/'root.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    lm,ss,code,ro=elf(score,linked)
    assert not [n for n,s in ss.items() if s['section']==0 and n not in ('sqrtf',)]
    for n,a in bindings.items():assert ss[n]['value']==a and ss[n]['section']==0xFFF1
    om['relocations']=relocations
    return om,lm,code,ss[FN]['value'],ro,bindings

def layout(score,tmp):
    obj=tmp/'layout.o';score.compile_single(HERE/'layout.c',FLAGS,obj)
    data,secs=score._elf(obj);sec=next(s for s in secs if s['name']=='.data')
    facts=list(struct.unpack('>18I',data[sec['off']:sec['off']+72]))
    assert facts==[104,60,952,2056,68,4,5,96,4,44,8,44,776,859,292,316,1732,12],facts
    return facts

def negatives(score,tmp,native,tables):
    source=(HERE/'visual.c').read_text()
    mutations={
      'wrong_first_tier_boundary':('scale <= 8.0f','scale < 8.0f',[dict(scale=8.)]),
      'wrong_second_tier_boundary':('scale <= 20.0f','scale < 20.0f',[dict(scale=20.)]),
      'wrong_mask_signedness':('(u32)(s32)record->hit_mask','(u32)(u8)record->hit_mask',[dict(mask=0x80,players=[dict(owner=31)])]),
      'truncated_scene_index':('[record->primary->scene_index]','[(s16)record->primary->scene_index]',[dict(scene_index=65536,players=[dict(position=(10.,0.,0.))])]),
      'wrong_vehicle_eligibility_index':('D_8014A250[i].state','D_8014A250[player->owner].state',[dict(players=[dict(owner=2,state=0)])]),
      'wrong_impulse_output_index':('vehicle = &D_8014A250[player->owner]','vehicle = &D_8014A250[i]',[dict(players=[dict(owner=2)])]),
      'wrong_radius_boundary':('distance_squared <= radius * radius','distance_squared < radius * radius',[dict(players=[dict(position=(8.,0.,0.))])]),
      'lost_existing_mask_bits':('record->hit_mask |=','record->hit_mask =',[dict(mask=0x80)]),
      'late_mask_write':('record->hit_mask |= 1u << (player->owner & 31);\n            func_8038D3A4(owner, player, damage);','func_8038D3A4(owner, player, damage);\n            record->hit_mask |= 1u << (player->owner & 31);',[{}]),
      'uncaptured_source_owner':('func_8038D3A4(owner, player, damage)','func_8038D3A4(&D_80152818[record->owner], player, damage)',[dict(mutate_damage=1,new_count=3)]),
      'uncaptured_center':('player->position[j] - center[j]','player->position[j] - record->primary->position[j]',[dict(mutate_damage=1,new_count=3)]),
      'lost_vertical_bias':('+ 66000.0f','+ 0.0f',[{}]),
      'uncleared_horizontal_y':('delta[1] = 0.0f;','delta[1] = delta[1];',[{}]),
      'wrong_count_signedness':('i < D_801543CA','i < (unsigned short) D_801543CA',[dict(count=-1)]),
    }
    rejected=[]
    for name,(old,new,cases) in mutations.items():
        assert old in source,name
        mutated=tmp/(name+'.c');mutated.write_text(source.replace(old,new))
        _,_,code,start,ro,_=build(score,mutated,tmp,FLAGS)
        try:verify(native,tables,code,start,tables+ro,cases)
        except AssertionError as exc:
            assert 'semantic mismatch' in str(exc) or (name=='wrong_count_signedness' and 'unmapped' in str(exc)),(name,str(exc))
            rejected.append(name)
        else:raise AssertionError(('wrong source accepted',name))
    from native import Machine
    try:Machine({ENTRY:0xFFFFFFFF},ENTRY,tables,{},True).run()
    except AssertionError as exc:assert 'opcode' in str(exc)
    else:raise AssertionError('unknown opcode accepted')
    _,_,code,start,ro,_=build(score,HERE/'visual.c',tmp,FLAGS)
    for stream,entry,data,private in [(native,ENTRY,tables,True),(code,start,tables+ro,False)]:
        try:Machine(stream,entry,data,dict(players=[dict(position=(0.,0.,0.))]),private).run()
        except AssertionError as exc:assert 'positive distance domain' in str(exc)
        else:raise AssertionError('zero distance silently admitted')
        m=Machine(stream,entry,data,dict(count=0),private);m.put(0x100000+96,0)
        try:m.run()
        except AssertionError as exc:assert 'unmapped' in str(exc)
        else:raise AssertionError('null primary silently admitted')
    return rejected+['unknown opcode refused','zero-distance domain excluded for native and candidate','null primary rejected even with nonpositive count']

def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path,default=HERE/'verification.json')
    p.add_argument('--check',action='store_true');p.add_argument('--quick',action='store_true')
    args=p.parse_args()
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('score_d498',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        print('SKIP: pinned IDO and MIPS GNU linker required');return
    score.ASM_DIR=ROOT/'asm/us/ovl_b';words=score.targets()[FN]
    raw=image(args.reference_root);target=struct.pack('>'+str(len(words))+'I',*words)
    assert len(target)==SIZE and target==raw[ENTRY-0x8038A400:ENTRY-0x8038A400+SIZE]
    assert sha(target)==json.loads((HERE/'contracts.json').read_text())['entry']['native_sha256']
    native={ENTRY+4*i:w for i,w in enumerate(words)};tables=inputs(raw)
    with tempfile.TemporaryDirectory(prefix='d498-visual-') as td:
        om,lm,code,start,ro,bindings=build(score,HERE/'visual.c',Path(td),FLAGS)
        cases=[{},dict(count=0),dict(count=4),dict(scale=12.),dict(scale=24.)] if args.quick else None
        behavior=verify(native,tables,code,start,tables+ro,cases)
        if not args.quick:
            assert behavior['native_unexecuted_offsets']==['0x88','0xb0']
            assert behavior['candidate_unexecuted_offsets']==['0xf8']
        facts=layout(score,Path(td));controls=[] if args.quick else negatives(score,Path(td),native,tables)
    result={'status':'COMPLETE-SEMANTIC-SOURCE / RESEARCH-ONLY; native private ABI not reproduced',
      'provenance':{'compiler_tools_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ['cc','cfe','uopt','ugen','as1']},
        'gnu_ld_sha256':sha(Path(shutil.which('mips-linux-gnu-ld')).read_bytes()),
        'gnu_ld_version':run(['mips-linux-gnu-ld','--version']).splitlines()[0],
        'build_route':'unchanged canonical score.compile_single; whole object linked; no group or keep list'},
      'base':BASE,'target':{'name':FN,'entry':hex(ENTRY),'size':SIZE,'sha256':sha(target)},
      'flags':FLAGS,'canonical_added_backend_flag':'-Wab,-r4300_mul',
      'sources_sha256':{n:sha((HERE/n).read_bytes()) for n in ['visual.c','native.py','verify.py','layout.c','contracts.json']},
      'object':om,'linked':lm,'external_bindings':{n:hex(a) for n,a in sorted(bindings.items())},
      'coverage_exclusions':{'native +0x88/+0xB0':'Unreachable duplicate mtc1 landing operations: each follows an unconditional branch with a live delay slot; preceding likely branch targets skip this duplicate.', 'candidate +0xF8':'Same duplicated mtc1 pattern after unconditional branch+0xF0 with delay slot+0xF4; likely branch+0xE4 targets+0xFC.'},
      'behavior':behavior,'layout_facts':facts,'negative_controls':controls,
      'nonmatch':{'different_word_positions':sum(code.get(start+i*4)!=w for i,w in enumerate(words)),
        'native_words':len(words),'candidate_words':len(code),
        'extra_nonzero_words':sum(w!=0 for a,w in code.items() if a>=start+SIZE),
        'scope':'full linked research-placement words; no native owned-data placement or strict match claim'},
      'native_tables':[{'address':hex(a),'size':len(b),'sha256':sha(b)} for a,b in tables],
      'limits':'Ordinary one-input semantic adapter; actual native entry remains private s2. External transform is rounded row-dot contract; damage is bounded model; no match or ROM claim'}
    if args.check:
        old=json.loads(args.output.read_text())
        stable=lambda x:{k:v for k,v in x.items() if k!='provenance'}
        assert stable(result)==stable(old),'receipt drift'
    else:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'candidate_bytes':lm['functions'][FN]['size'],
                     'cases':behavior['paired_cases'],'native_covered':len(behavior['native_executed_offsets']),
                     'native_unexecuted':behavior['native_unexecuted_offsets'],
                     'candidate_covered':len(behavior['candidate_executed_offsets'])},indent=2))
if __name__=='__main__':main()
