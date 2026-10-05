#!/usr/bin/env python3
"""Reproduce fixed donor controls, complete ELF/GNU accounting and bounded behavior."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import itertools
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
spec=importlib.util.spec_from_file_location('entity_boundary_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='entity_iterate'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
ARCHIVE=ROOT/'cloud/work/near_miss_B76/entity_iterate_native.c'
ACCEPTED=ROOT/'src/blob/groups/frontier_vector_normalize/group.c'
MEMBERS=[FN,'func_8008E098','func_8008E0B8']
def sha(data):return hashlib.sha256(data).hexdigest()
def symbol(obj,name):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    syms=[s for i,x in enumerate(secs) if x['type']==2 for s in score._symbol_table(data,secs,i)
          if s['name']==name and s['section']==ti and s['type']==2]
    assert len(syms)==1,(name,syms)
    return syms[0]
def sources():
    a=ARCHIVE.read_text();header=(HERE/'donor_vectors.h').read_text()
    helpers={}
    for name in ('vecsub','scalmul','vecadd'):
        first=header.index('static void '+name+'(')
        last=header.index('\n}',first)+2
        helpers[name]=header[first:last]
    start=a.index('  difference[0]=');end=a.index('\n }',start)
    scale_start=a.index('  first[0]=',start)
    sub='  vecsub(second,first,difference);\n'
    tail='  scalmul(difference,0.25f,difference);\n  vecadd(difference,first,first);'
    rows={'archived_direct':(a,()),
          'authentic_vecsub':(a[:start]+sub+a[a.index('  func_8008E0B8',start):],('vecsub',)),
          'authentic_scale_add':(a[:scale_start]+tail+a[end:],('scalmul','vecadd')),
          'authentic_all_functions':(a[:start]+sub+'  func_8008E0B8(difference);\n'+tail+a[end:],('vecsub','scalmul','vecadd')),
          'authentic_fmath_macros':((HERE/'candidate.c').read_text(),())}
    out={}
    for name,(source,used) in rows.items():
        if used:source=source.replace('int entity_iterate','\n\n'.join(helpers[n] for n in used)+'\n\nint entity_iterate')
        out[name]=source
    return out

def inspect(obj,name,work):
    work.mkdir(parents=True,exist_ok=True)
    data,secs=score._elf(obj);ti=score._text_index(secs);fn=symbol(obj,name)
    start,end=fn['value'],fn['value']+fn['size'];words=score.text_words(obj)
    assert fn['size']>0 and fn['size']%4==0 and end<=len(words)*4
    assert not any(s['size'] for s in secs if s['name'] in ('.rodata','.rdata','.data','.bss'))
    resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
    assert not any((masks,unresolved,unverified,errors))
    relocations=[];definitions={}
    addresses=score.image_symbols()
    for sec in secs:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,secs,sec['link'])
            for p in range(sec['off'],sec['off']+sec['size'],8):
                off,info=struct.unpack_from('>II',data,p);s=syms[info>>8]
                assert s['name'] in addresses,(s['name'],'unexpected section-relative relocation')
                definitions[s['name']]=addresses[s['name']]
                if start<=off<end:relocations.append({'offset':off-start,'type':info&255,'symbol':s['name']})
    # Each complete body gets its own native-address link. Absolute function
    # assignments resolve calls to real native addresses, regardless of object
    # source order. This does not assert contiguous original TU placement.
    definitions.pop(name,None)
    script=work/'link.ld';script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n'%(addresses[name]-start)+
         ''.join('%s = 0x%x;\n'%(n,a) for n,a in definitions.items()))
    elf=work/'linked.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,ls=score._elf(elf);ts=ls[score._text_index(ls)]
    body=linked[ts['off']+start:ts['off']+end]
    got=list(struct.unpack('>%dI'%(len(body)//4),body));want=score.targets()[name]
    assert got==resolved[start//4:end//4],'GNU/scorer relocation disagreement'
    assert symbol(elf,name)['value']==addresses[name] and symbol(elf,name)['size']==fn['size']
    diffs=[i*4 for i in range(max(len(got),len(want))) if i>=len(got) or i>=len(want) or got[i]!=want[i]]
    frames=[-native.signed(w&65535,16) for w in got if w>>16==0x27bd and w&32768]
    cmp=score.compare(obj,name,show=0)
    return {'canonical':dict(asdict(cmp),verdict=cmp.summary()),'elf_bytes':fn['size'],'native_bytes':4*len(want),
            'symbol_offset':start,'text_section_bytes':len(words)*4,
            'excluded_text_bytes_before_symbol':start,'excluded_text_bytes_after_symbol':len(words)*4-end,
            'excluded_text_nonzero_words_after_symbol':sum(w!=0 for w in words[end//4:]),
            'stack_frame':max(frames,default=0),'whole_body_differing_words':len(diffs),
            'differing_byte_offsets':diffs,'extra_elf_words':max(0,len(got)-len(want)),
            'missing_elf_words':max(0,len(want)-len(got)),
            'zero_words_inside_excess':sum(w==0 for w in got[len(want):]),
            'body_sha256':sha(body),'native_sha256':sha(struct.pack('>%dI'%len(want),*want)),
            'all_full_body_relocations_gnu_verified':True,'relocations':relocations,
            'owned_data_bytes':0,'full_symbol_equal':got==want},got

def build(work):
    result={};compiled={};srcs=sources()
    for name,text in srcs.items():
        d=work/name;d.mkdir();p=d/'candidate.c';p.write_text(text);obj=d/'candidate.o'
        score.compile_single(p,FLAGS,obj)
        row,words=inspect(obj,FN,d/'link');row.update(source_sha256=sha(text.encode()),mode='ordinary O3')
        result[name]={FN:row};compiled[name]=(p,words)
    for selected in ('archived_direct','authentic_all_functions','authentic_fmath_macros'):
        name=selected+'_accepted_context';d=work/name;d.mkdir()
        (d/'candidate.c').write_text(srcs[selected]);shutil.copyfile(ACCEPTED,d/'normalize.c')
        group={'files':['normalize.c','candidate.c'],'keep':MEMBERS,'members':MEMBERS,'claims':[],'flags':FLAGS}
        (d/'group.json').write_text(json.dumps(group));obj=d/'group.o';score.compile_group(d,obj)
        result[name]={}
        for member in MEMBERS:
            row,words=inspect(obj,member,d/member);result[name][member]=row
            if member==FN:compiled[name]=(d/'candidate.c',words)
            else:assert row['full_symbol_equal'],(name,member,row)
        result[name][FN].update(source_sha256=sha(srcs[selected].encode()),mode='genuine accepted normalization context')
    assert all(not row[FN]['full_symbol_equal'] for row in result.values())
    assert result['archived_direct'][FN]['canonical']['differing']==29
    assert result['authentic_fmath_macros'][FN]['canonical']['differing']==29
    return result,compiled

def cases():
    vectors=[(0.,0.,0.),(-0.,0.,-0.),(3.,4.,0.),(-3.,-4.,-12.),(1e-6,0.,0.),(1e-5,0.,0.),
             (1.000001e-5,0.,0.),(100.,-50.,25.)]
    for first,second,hit,mutate,alias in itertools.product(vectors[:2],vectors,range(2),range(2),range(2)):
        yield {'first':first,'second':second,'replacement':(1.25,-2.5,4.),'hit':hit,'mutate':mutate,'alias':alias}
    rng=random.Random(0xc69c0)
    for i in range(1792):
        v=lambda:tuple(rng.randint(-65535,65535)/128. for _ in range(3))
        yield {'first':v(),'second':v(),'replacement':v(),'hit':i%2,'mutate':(i//2)%2,'alias':(i//4)%2}

def host_lib(work,source,name):
    normalize=work/'accepted_normalize.o'
    common=['cc','-std=c89','-O1','-fPIC','-fno-fast-math','-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all']
    if not normalize.exists():
        subprocess.run(common+['-Dfunc_8008E0B8=accepted_normalizer','-Wno-unknown-pragmas','-c',str(ACCEPTED),'-o',str(normalize)],check=True,capture_output=True)
    out=work/(name+'.so')
    subprocess.run(common+['-shared','-DCANDIDATE_SOURCE="%s"'%source,str(HERE/'host.c'),str(normalize),'-lm','-o',str(out)],check=True,capture_output=True)
    dll=ctypes.CDLL(str(out));dll.run_case.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_float]
    dll.run_case.restype=ctypes.c_int
    return dll

def host_result(dll,case,threshold):
    inp=(ctypes.c_uint32*9)(*map(native.bits,case['first']+case['second']+case['replacement']))
    out=(ctypes.c_uint32*19)()
    assert dll.run_case(inp,out,case['hit'],case['mutate'],case['alias'],threshold)==0
    vals=list(out);events=[('collision',vals[6:9],vals[9:12])]
    assert vals[18]==(9 if case['hit'] else 6)
    if case['hit']:events.append(('normalize',vals[12:15]))
    return vals[:3],vals[3:6],events

def behavior(work,compiled):
    selected=list(cases());threshold_bytes=score.own_data().read(native.THRESHOLD,4)
    assert threshold_bytes is not None
    threshold=struct.unpack('>f',threshold_bytes)[0];assert threshold==native.f32(1e-5)
    normalizer=score.targets()['func_8008E0B8'];native_words=score.targets()[FN]
    expectations=[];vis=set();branches=set();digest=hashlib.sha256()
    for case in selected:
        expected=native.oracle(case,threshold);expectations.append(expected)
        machine=native.Machine(native_words,normalizer,threshold_bytes,case)
        assert machine.run()==expected,('protected native',case)
        vis.update(machine.visited);branches.update(machine.branches)
        digest.update(json.dumps(expected).encode())
    assert set(range(native.BASE,native.BASE+224,4))<=vis
    runs={}
    for name,(source,words) in compiled.items():
        host=host_lib(work,source,name);seen=set()
        for case,expected in zip(selected,expectations):
            assert host_result(host,case,threshold)==expected,('host',name,case)
            machine=native.Machine(words,normalizer,threshold_bytes,case)
            assert machine.run()==expected,('GNU-linked',name,case)
            seen.update(machine.visited)
        runs[name]={'cases':len(selected),'host_c89_ubsan':'passed','linked_native':'passed',
                    'linked_target_instructions_covered':sum(native.BASE<=pc<native.BASE+len(words)*4 for pc in seen),
                    'linked_target_instructions':len(words)}
    negative={}
    source=(HERE/'candidate.c').read_text()
    mutants={'wrong_correction':('ScaleAddVector(difference,0.25f','ScaleAddVector(difference,0.5f'),
             'reverse_delta':('SubVector(second,first,difference);','SubVector(first,second,difference);'),
             'skip_normalization':('  func_8008E0B8(difference);',''),
             'reverse_collision':('input_deadzone_apply(second,first','input_deadzone_apply(first,second')}
    # The arity/pointer boundary asserts via trap, so do not execute that mutant
    # in-process. It is exercised by the independently decoded wrong-JAL test.
    mutants.pop('reverse_collision')
    for name,(old,new) in mutants.items():
        assert source.count(old)==1;p=work/(name+'.c');p.write_text(source.replace(old,new));dll=host_lib(work,p,name)
        for i,(case,expected) in enumerate(zip(selected,expectations)):
            try: actual=host_result(dll,case,threshold)
            except AssertionError: actual=None
            if actual!=expected:
                negative[name]={'rejected':True,'case':i};break
        assert name in negative,name
    for name,changed in [('unknown_opcode',(native_words[0]&0x3ffffff)|(63<<26)),('wrong_call',0x0c000001)]:
        bad=list(native_words);bad[1 if name=='unknown_opcode' else 13]=changed
        try:native.Machine(bad,normalizer,threshold_bytes,selected[0]).run()
        except AssertionError:negative[name]={'rejected':True}
        else:raise AssertionError(name)
    return {'cases':len(selected),'protected_native_runs':len(selected),'gnu_linked_runs':len(selected)*len(compiled),
            'host_runs':len(selected)*len(compiled),'normalizer_native_instructions_covered':sum(native.NORMALIZE<=pc<native.NORMALIZE+140 for pc in vis),
            'target_native_instructions_covered':56,'target_branch_outcomes':len([b for b in branches if native.BASE<=b[0]<native.BASE+224]),
            'normalizer_branch_outcomes':len([b for b in branches if native.NORMALIZE<=b[0]<native.NORMALIZE+140]),
            'threshold_address':hex(native.THRESHOLD),'threshold_data_sha256':sha(threshold_bytes),
            'output_sha256':digest.hexdigest(),'controls':runs,'negative_controls':negative,
            'boundary':'input_deadzone_apply is a bounded callback model; actual native normalization executes.',
            'limits':'Finite arithmetic only; no FCSR, NaNs/infinities, partial pointer overlap, concurrency or whole-caller/gameplay proof.'}

def native_metadata():
    addresses=score.image_symbols();targets=score.targets();base=addresses[FN]
    locks=json.loads((ROOT/'blob_matched.lock.json').read_text())
    assert FN not in locks and len(targets[FN])*4==224
    assert -native.signed(targets[FN][0]&65535,16)==88
    callers=[]
    for name,words in targets.items():
        for i,w in enumerate(words):
            if w>>26 in (2,3):
                pc=addresses[name]+4*i
                if ((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)==base:
                    callers.append({'name':name,'call_site':hex(pc),'function_bytes':len(words)*4})
    calls=[]
    for i,w in enumerate(targets[FN]):
        if w>>26==3:
            target=((base+4*i+4)&0xf0000000)|((w&0x3ffffff)<<2)
            calls.append({'offset':4*i,'destination':hex(target)})
    assert {int(c['destination'],16) for c in calls}=={native.COLLISION,native.NORMALIZE}
    return {'native_frame_bytes':88,'direct_callers':callers,'direct_calls':calls,
            'locked':False,'native_bytes':224,'limits':'Direct protected-target census only; indirect callers are not excluded.'}

def verify(work):
    results,compiled=build(work)
    binds=[HERE/n for n in ('candidate.c','donor_vectors.h','native.py','host.c','verify.py','claim.json','provenance.json')]+[ARCHIVE,ACCEPTED,ACCEPTED.parent/'group.json']
    return {'status':'NONMATCH','claims':[],'accepted_byte_gain':0,'base_revision':'e24b47d89a0c8ffade1e4c75ad76b9d390a1c232',
            'function':FN,'range':['0x800C69C0','0x800C6AA0'],'flags':FLAGS,'mandatory_assembler_flag':score.R4300_CC,
            'native_metadata':native_metadata(),'controls':results,'behavior':behavior(work,compiled),
            'source_bindings':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in binds},
            'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
            'compiler_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','umerge','uopt','ugen','as1')}}
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--write',action='store_true');args=p.parse_args()
    with tempfile.TemporaryDirectory(prefix='entity-vector-proof-') as d:result=verify(Path(d))
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'controls':{n:{k:{x:y[x] for x in ('elf_bytes','stack_frame','whole_body_differing_words','full_symbol_equal')} for k,y in v.items()} for n,v in result['controls'].items()},'behavior':result['behavior']},indent=2))
