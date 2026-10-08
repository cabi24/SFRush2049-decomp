"""Reproduce complete ELF, GNU, clock-contract, caller and bounded behavior proof."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('frame_dispatch_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='func_800B7FF8'
SOURCE=ROOT/'cloud/matches/func_800B7FF8.c'
CALLER='Effects_UpdateEmitters'
CALLER_PATH='src/blob/Effects_UpdateEmitters.c'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
BASELINE_PATH='cloud/work/game_C31/func_800B7FF8.bits.c'
WRITERS=['asm/us/nonmatchings/rom/lib_1050/viTickStart.s','asm/us/nonmatchings/rom/lib_1050/viUpdateTime.s']

BASE='e24b47d89a0c8ffade1e4c75ad76b9d390a1c232'
HISTORICAL_SOURCES=[CALLER_PATH,BASELINE_PATH,'tools/cloud/score.py']+WRITERS

def base_bytes(path):
    return subprocess.run(['git','show',BASE+':'+path],cwd=ROOT,check=True,capture_output=True).stdout

def sha(x):return hashlib.sha256(x).hexdigest()
def symbols(obj):
    data,secs=score._elf(obj)
    return [s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
def extent(obj,name):
    syms=[s for s in symbols(obj) if s['name']==name and s['type']==2 and s['section']!=0]
    assert len(syms)==1
    return syms[0]['value'],syms[0]['size']
def inspect(obj,name):
    off,size=extent(obj,name);result=asdict(score.compare(obj,name,show=0))
    result.update(elf_bytes=size,native_bytes=4*len(score.targets()[name]),offset=off)
    return result

def gnu(obj,name,work,expected_match=True):
    off,size=extent(obj,name);addresses=score.image_symbols();address=addresses[name]
    data,secs=score._elf(obj);ti=score._text_index(secs)
    assert not any(s['size'] for s in secs if s['name'] in ('.rodata','.data','.bss','.sdata','.sbss'))
    relocs=[]
    for sec in secs:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,secs,sec['link'])
            for pos in range(sec['off'],sec['off']+sec['size'],8):
                at,info=struct.unpack_from('>II',data,pos)
                r={'offset':at,'type':info&255,'symbol':syms[info>>8]['name']}
                if r['symbol'] not in addresses:
                    addresses[r['symbol']]=score.address_named(r['symbol'])
                assert addresses[r['symbol']] is not None and r['type'] in (4,5,6)
                relocs.append(r)
    script=work/(name+'.ld')
    names={r['symbol'] for r in relocs}-{name}
    script.write_text('SECTIONS { .text 0x%08X : SUBALIGN(4) { *(.text) } }\n'%(address-off)+
                      ''.join('%s = 0x%08X;\n'%(n,addresses[n]) for n in sorted(names)))
    elf=work/(name+'.elf')
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,s=score._elf(elf);t=s[score._text_index(s)]
    full=linked[t['off']+off:t['off']+off+size]
    assert size==4*len(score.targets()[name]) and extent(elf,name)==(address,size)
    words=list(struct.unpack('>%dI'%(size//4),full))
    differences=[i*4 for i,(a,b) in enumerate(zip(words,score.targets()[name])) if a!=b]
    if expected_match:assert not differences
    # GNU readelf provides an independent symbol/extent parser.
    rows=subprocess.run(['mips-linux-gnu-readelf','-sW',str(elf)],check=True,capture_output=True,text=True).stdout
    assert re.search(r'\b%08x\s+%d\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+%s\b'%(address,size,name),rows)
    own=[dict(r,offset=r['offset']-off) for r in relocs if off<=r['offset']<off+size]
    return {'elf_bytes':size,'full_body_equal':not differences,'differing_offsets':differences,'resolved_relocations':own,'linked_body_sha256':sha(full)},words

def clock_witness():
    results=[]
    for path,address in zip(WRITERS,(0x80001390,0x800014dc)):
        content=base_bytes(path).decode()
        pairs={int(a,16):int(w,16) for a,w in re.findall(r'/\*\s+[0-9A-F]+\s+([0-9A-F]{8})\s+([0-9A-F]{8})\s+\*/',content)}
        store=pairs[address]
        assert store>>26==57 and (store>>21&31)==1 and native.signed(store&65535,16)==-5228
        preceding=[a for a,w in pairs.items() if a<address and w>>26==15 and (w>>16&31)==1]
        high=pairs[max(preceding)]&65535
        assert (high*65536+native.signed(store&65535,16))&0xffffffff==native.DELTA
        results.append({'path':path,'writer_address':hex(address),'destination':hex(native.DELTA),'sha256':sha(content.encode())})
    w=score.targets()['func_8010E694']
    assert w[0x40//4]>>26==15 and w[0x40//4]&65535==0x8003
    assert w[0x44//4]>>26==9 and native.signed(w[0x44//4]&65535,16)==-5228
    reads=[]
    for off in range(0x4c,0x68,4):
        ins=w[off//4];op=ins>>26
        assert op not in (2,3,40,41,42,43,46,57),('intervening side effect',off)
        if op==49 and (ins>>21&31)==6 and ins&65535==0:reads.append(off)
    assert reads==[0x4c,0x50,0x64]
    static=base_bytes('symbol_addrs.us.txt').decode()
    assert re.search(r'__osScDeltaTime\s*=\s*0x8002EB94',static)
    return {'clock':'__osScDeltaTime','address':hex(native.DELTA),'distinct_from_elapsed_time':'0x8002eb90',
            'writers':results,'writer_callers':writer_callers(),'consumer':'func_8010E694','repeated_read_offsets':reads,
            'no_intervening_call_or_clock_store':True,
            'admission_limit':'Consistent independently witnessed observation contract; original header and asynchronous gameplay interleavings are not recovered.'}

def writer_callers():
    answer={}
    for address,name in [(0x80001350,'viTickStart'),(0x800013f4,'viUpdateTime')]:
        callers={fn:[4*i for i,w in enumerate(words) if w>>26==3 and ((w&0x3ffffff)*4|0x80000000)==address]
                 for fn,words in score.targets().items()}
        answer[name]={fn:offsets for fn,offsets in callers.items() if offsets}
    assert set(answer['viUpdateTime'])=={'game_mode_handler'}
    assert set(answer['viTickStart'])=={'game_late_init'}
    return answer

def native_bindings():
    """Bind selected native inputs after enforcing the current integrity gates.

    Whole-image manifests can change when unrelated bodies gain provenance
    comments. Their digests are historical provenance, not selected-body IDs.
    """
    targets=score.targets()
    addresses=score.image_symbols()
    names=(FN,CALLER,'func_8010E694','game_mode_handler','game_late_init','game_loop')
    functions={name:{'bytes':4*len(targets[name]),
        'sha256':sha(struct.pack('>%dI'%len(targets[name]),*targets[name]))} for name in names}
    selected=set(names)|{'state_word_a','D_801170FC','gameplay_mode','D_8002EB94',
        'D_8002EB90','D_80123DE0','D_80116178','particles_update','func_80391B00',
        'viTickStart','viUpdateTime','D_80150B70','D_80150BA0','D_80151AD0',
        'D_80152032','audio_doppler_full','math_utility','particles_spawn_emitter'}
    resolved={name:addresses[name] if name in addresses else score.address_named(name)
              for name in sorted(selected)}
    assert all(address is not None for address in resolved.values())
    static=base_bytes('symbol_addrs.us.txt').decode()
    clocks={name:int(re.search(r'\b%s\s*=\s*(0x[0-9A-Fa-f]+)'%name,static)[1],16)
            for name in ('__osScDeltaTime','__osScElapsedTime')}
    data=score.owndata.ImageData.from_artifact(score.owndata.artifact_dir(score.ASM_DIR))
    assert data is not None,'current native data artifact missing'
    return {'functions':functions,'symbols':resolved,
            'static_clock_symbols':clocks,
            'coefficient':{'address':native.PI,'bytes':4,'sha256':sha(data.read(native.PI,4))}}

def assert_native_bindings(expected):
    assert native_bindings()==expected,'selected native input changed'

def comparable_receipt(result):
    """Ignore only historical global-file digests; all live bindings remain."""
    result=json.loads(json.dumps(result))
    result.pop('historical_native_provenance', None)
    for name in HISTORICAL_SOURCES:result['source_bindings'].pop(name, None)
    return result

def strict(obj,name):
    row=inspect(obj,name)
    assert row['elf_bytes']==row['native_bytes']
    assert not any(row[k] for k in ('differing','unresolved','unverified','errors','extra_words'))
    return row

def host(work,source,cases,name):
    exe=work/name
    subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-ffp-contract=off',
                    '-fsanitize=undefined','-fno-sanitize-recover=all','-DCANDIDATE="%s"'%source,
                    str(HERE/'host.c'),'-o',str(exe)],check=True,capture_output=True)
    text=''.join(' '.join(map(str,c))+'\n' for c in cases)
    result=subprocess.run([str(exe)],input=text,check=True,capture_output=True,text=True)
    assert not result.stderr
    rows=[list(map(int,line.split())) for line in result.stdout.splitlines()]
    assert len(rows)==len(cases)
    return rows

def behavior(work,linked):
    cases=native.corpus();expected=[native.reference(c) for c in cases]
    rows=host(work,SOURCE,cases,'host')
    assert rows==expected
    visited=set();branches={};reads=0
    for words in (score.targets()[FN],linked):
        for case,want in zip(cases,expected):
            m=native.Machine(words,case);assert m.run()==want
            visited|=m.visited;reads+=m.reads.count(native.DELTA)
            for k,v in m.branches.items():branches.setdefault(k,set()).update(v)
    assert visited==set(range(0,native.SIZE,4)),sorted(set(range(0,native.SIZE,4))-visited)
    assert all(v=={False,True} for v in branches.values())
    src=SOURCE.read_text()
    mutants={
        'paused_inverted':src.replace('if (!D_801170FC)','if (D_801170FC)'),
        'wrong_flag':src.replace('0x200000','0x100000'),
        'wrong_overlay_mode':src.replace('gameplay_mode == 6','gameplay_mode == 5'),
        'wrong_coefficient':src.replace('D_80123DE0 * D_8002EB94','2.0f * D_8002EB94'),
        'cached_callback_mode':src.replace('void func_800B7FF8(void) {','void func_800B7FF8(void) {\n    s32 cached_mode = gameplay_mode;').replace('if (gameplay_mode == 6 || gameplay_mode == 4)','if (cached_mode == 6 || cached_mode == 4)'),
    }
    mutations={}
    for name,text in mutants.items():
        assert text!=src
        path=work/(name+'.c');path.write_text(text)
        actual=host(work,path,cases,name)
        differing=sum(a!=b for a,b in zip(actual,expected));assert differing>0
        mutations[name]={'compiled_host_cases':len(cases),'counterexamples':differing,'rejected':True}
    adverse={}
    for name,off,word in [('unknown_opcode',0,0xffffffff),('wrong_stack_home',0x14,0xafbf0010),('truncated_body',None,None)]:
        bad=list(linked)
        if off is None:bad=bad[:-1]
        else:bad[off//4]=word
        try:native.Machine(bad,cases[0]).run()
        except AssertionError:adverse[name]=True
        else:raise AssertionError(name+' accepted')
    return {'host_c89_ubsan_cases':len(cases),'native_executions':2*len(cases),'clock_reads':reads,
            'executed_instruction_offsets':sorted(visited),'conditional_branches':len(branches),
            'every_branch_both_outcomes':True,'source_mutants':mutations,'adverse_controls':adverse,
            'scope':'Finite binary32 inputs, including signed zero/subnormals. Helpers are explicit mutation-aware O32 hooks; no FCSR, concurrency, overlay internals or gameplay proof.'}

def verify(work):
    result={'base_revision':'e24b47d89a0c8ffade1e4c75ad76b9d390a1c232','status':'MATCHING_CANDIDATE_PENDING_INDEPENDENT_REVIEW',
            'target':FN,'range':['0x800B7FF8','0x800B80C8'],'candidate_bytes':208,'accepted_byte_gain':0,
            'flags':FLAGS,'clock_contract':clock_witness(),'native_bindings':native_bindings()}
    linked=None
    for level in ('O3','O2'):
        obj=work/(level+'.o');score.compile_single(SOURCE,FLAGS.replace('O3',level),obj)
        row=strict(obj,FN);assert row['elf_bytes']==208
        proof,words=gnu(obj,FN,work)
        data,sections=score._elf(obj);text=sections[score._text_index(sections)]
        tail=data[text['off']+row['offset']+208:text['off']+text['size']];assert not any(tail)
        row.update(gnu=proof,zero_text_alignment_bytes=len(tail),owned_data_bytes=0)
        result[level]=row
        if level=='O3':linked=words
    baseline=work/'baseline.c';baseline.write_bytes(base_bytes(BASELINE_PATH))
    old=work/'baseline.o';score.compile_single(baseline,FLAGS,old)
    result['ordinary_clock_baseline']=inspect(old,FN)
    assert result['ordinary_clock_baseline']['differing']==6
    oldproof,_=gnu(old,FN,work,expected_match=False)
    assert len(oldproof['differing_offsets'])==6
    result['ordinary_clock_baseline']['gnu']=oldproof
    group=work/'context';group.mkdir()
    shutil.copyfile(SOURCE,group/'candidate.c');(group/'caller.c').write_bytes(base_bytes(CALLER_PATH))
    (group/'group.json').write_text(json.dumps({'files':['candidate.c','caller.c'],'members':[FN],'context':[CALLER],
        'keep':[FN,CALLER],'flags':FLAGS}))
    obj=group/'group.o';score.compile_group(group,obj)
    result['genuine_context']={}
    for name in (FN,CALLER):
        row=strict(obj,name)
        proof,_=gnu(obj,name,group);row['gnu']=proof;result['genuine_context'][name]=row
    # Exact native call graph, not just names in a source header.
    address=score.image_symbols()[FN]
    callers={name:[i*4 for i,w in enumerate(words) if w>>26==3 and ((w&0x3ffffff)*4|0x80000000)==address]
             for name,words in score.targets().items()}
    callers={k:v for k,v in callers.items() if v};assert list(callers)==[CALLER]
    result['direct_callers']=callers
    result['external_coefficient']={'address':'0x80123de0','sha256':sha(score.own_data().read(native.PI,4)),
                                    'value':struct.unpack('>f',score.own_data().read(native.PI,4))[0]}
    assert score.own_data().read(native.PI,4)==struct.pack('>I',0x40490fdb)
    result['behavior']=behavior(work,linked)
    paths=[SOURCE,HERE/'verify.py',HERE/'native.py',HERE/'host.c']
    result['source_bindings']={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths}
    result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    result['limitations']=['No original N64 source/header recovery or arcade donor claim.',
                          'GNU context linking is per complete body, not original contiguous TU placement.',
                          'Only the dispatcher is behaviorally executed; actual caller is byte-checked, not executed.',
                          'No full-game shadow, source-built image, compression, ROM SHA-1, hardware or gameplay gate.']
    return result

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--write',action='store_true');args=p.parse_args()
    with tempfile.TemporaryDirectory(prefix='frame-dispatch-proof-') as tmp:result=verify(Path(tmp))
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','O3','ordinary_clock_baseline','behavior')},indent=2))
