#!/usr/bin/env python3
"""Source-bound full ELF/GNU proof and bounded constructor contract tests."""
import argparse,ctypes,hashlib,importlib.util,json,shutil,struct,subprocess,sys,tempfile
from dataclasses import asdict
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('constructor_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='car_stats_display';FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=['func_800D18D8','func_80091FBC','func_8009211C']
SOURCE=HERE/'candidate.c'

def sha(data):return hashlib.sha256(data).hexdigest()
def run(args):
    p=subprocess.run(args,capture_output=True,text=True)
    assert p.returncode==0,(args,p.stdout,p.stderr)
    return p.stdout

def functions(obj):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    return {s['name']:s for i,sec in enumerate(secs) if sec['type']==2
            for s in score._symbol_table(data,secs,i) if s['section']==ti and s['type']==2}

def link_proof(obj,work):
    """Bind symbol definitions at their proven native addresses for GNU relocation.

    Instructions and relocation records are not altered. Defined function
    symbols become undefined in the temporary witness and the linker script
    supplies their proven addresses, so interfunction JALs
    resolve to native addresses despite the group object's concatenated layout.
    Complete original ELF extents supply extraction boundaries, independently
    of the native function length.
    """
    data,secs=score._elf(obj);ti=score._text_index(secs);fns=functions(obj)
    addresses=score.image_symbols();witness=bytearray(data)
    for sec in secs:
        if sec['type']!=2:continue
        syms=score._symbol_table(data,secs,secs.index(sec))
        for i,s in enumerate(syms):
            if s['name'] in fns:
                assert s['name'] in addresses
                struct.pack_into('>I',witness,sec['off']+i*16+4,0)
                struct.pack_into('>H',witness,sec['off']+i*16+14,0)
    rels=[];externs={}
    for sec in secs:
        if sec['type']!=9 or sec['info']!=ti:continue
        syms=score._symbol_table(data,secs,sec['link'])
        for at in range(sec['off'],sec['off']+sec['size'],8):
            off,info=struct.unpack_from('>II',data,at);s=syms[info>>8]
            assert s['type']!=3,'Unexpected section-relative relocation'
            n=s['name'];v=addresses.get(n,score.address_named(n));assert v is not None,n
            rels.append({'offset':off,'type':info&255,'symbol':n})
            externs[n]=v
    assert not any(s['size'] for s in secs if s['name'] in ('.data','.rodata','.bss'))
    wp=work/(obj.stem+'.gnu-input.o');wp.write_bytes(witness)
    ld=work/(obj.stem+'.ld');ld.write_text('SECTIONS { .text 0x80000000 : { *(.text) } }\n'+''.join('%s = 0x%X;\n'%x for x in sorted(externs.items())))
    elf=work/(obj.stem+'.elf');run(['mips-linux-gnu-ld','-EB','-T',str(ld),'-o',str(elf),str(wp)])
    linked,ls=score._elf(elf);txt=ls[score._text_index(ls)]
    raw=data[secs[ti]['off']:secs[ti]['off']+secs[ti]['size']]
    original=list(struct.unpack('>%dI'%(len(raw)//4),raw))
    reports={};bodies={};covered=set()
    for n,f in fns.items():
        start=f['value'];end=start+f['size'];assert f['size']>0 and f['size']%4==0
        body=linked[txt['off']+start:txt['off']+end];words=list(struct.unpack('>%dI'%(len(body)//4),body))
        got,masks,unknown,unverified,errors=score.relocate(obj,original,start,end,addresses)
        assert not any([masks,unknown,unverified,errors]);assert words==got[start//4:end//4]
        target=score.targets()[n]
        cmp=asdict(score.compare(obj,n,show=0));diff=[4*i for i in range(max(len(words),len(target))) if i>=len(words) or i>=len(target) or words[i]!=target[i]]
        assert len(words)==len(target),n
        assert cmp['differing']==len(diff) and not any(cmp[k] for k in ['extra_words','unresolved','unverified','errors'])
        rr=[dict(r,offset=r['offset']-start) for r in rels if start<=r['offset']<end]
        reports[n]={'symbol_bytes':f['size'],'native_bytes':len(target)*4,'differing_words':len(diff),
                    'differing_offsets':diff,'body_sha256':sha(body),'relocations':rr,
                    'gnu_equals_project_relocation':True,'canonical':cmp}
        bodies[n]=words;covered.update(range(start,end))
    outside=bytes(raw[i] for i in range(len(raw)) if i not in covered);assert not any(outside)
    return {'object_sha256':sha(data),'functions':reports,'excluded_zero_text_padding':len(outside),'owned_data_bytes':0},bodies

def compile_proof(work):
    objects={};source=SOURCE.read_text()
    for key,path in [('archived',ROOT/'cloud/work/game_C41/car_stats_display.floats.c'),('corrected',SOURCE)]:
        obj=work/(key+'.o');score.compile_single(path,FLAGS,obj);objects[key]=obj
    obj=work/'corrected_header_o2.o';score.compile_single(SOURCE,FLAGS.replace('-O3','-O2'),obj);objects['corrected_header_o2']=obj
    group=work/'group';group.mkdir();shutil.copyfile(SOURCE,group/'candidate.c')
    lock=json.loads((ROOT/'blob_matched.lock.json').read_text());hashes={}
    for n in CONTEXT:
        p=ROOT/lock[n]['source'];h=sha(p.read_bytes());assert h==lock[n]['source_sha256'];hashes[str(p.relative_to(ROOT))]=h
        shutil.copyfile(p,group/(n+'.c'))
    manifest={'files':['candidate.c']+[n+'.c' for n in CONTEXT],'members':[FN],
              'context':CONTEXT,'keep':[FN]+CONTEXT,'flags':FLAGS,'claims':[]}
    (group/'group.json').write_text(json.dumps(manifest))
    obj=work/'context.o';score.compile_group(group,obj);objects['context']=obj
    reports={};bodies={}
    for key,obj in objects.items():reports[key],bodies[key]=link_proof(obj,work)
    assert all(reports[k]['functions'][FN]['differing_words']==24 for k in reports)
    assert bodies['archived'][FN]==bodies['corrected'][FN]==bodies['context'][FN]
    assert all(reports['context']['functions'][n]['differing_words']==0 for n in CONTEXT)
    negative_object_checks(objects['corrected'],work)
    return reports,bodies['corrected'][FN],hashes

def negative_object_checks(obj,work):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    for sec in secs:
        if sec['type']==2:
            syms=score._symbol_table(data,secs,secs.index(sec))
            for i,s in enumerate(syms):
                if s['name']==FN and s['section']==ti:
                    bad=bytearray(data);struct.pack_into('>I',bad,sec['off']+16*i+8,s['size']-4)
                    p=work/'bad_extent.o';p.write_bytes(bad)
        elif sec['type']==9 and sec['info']==ti:
            bad=bytearray(data);info=struct.unpack_from('>I',bad,sec['off']+4)[0]
            struct.pack_into('>I',bad,sec['off']+4,(info&~255)|255)
            q=work/'bad_relocation.o';q.write_bytes(bad)
    for corrupted in (p,q):
        try:link_proof(corrupted,work)
        except (AssertionError,SystemExit):pass
        else:raise AssertionError('Corrupt ELF was accepted: '+corrupted.name)


def host(work,source=SOURCE,tag='host'):
    so=work/(tag+'.so')
    run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fPIC','-shared',
         '-fsanitize=undefined','-fno-sanitize-recover=all',str(source),str(HERE/'host.c'),'-o',str(so)])
    lib=ctypes.CDLL(str(so));f=lib.run_case
    f.argtypes=[ctypes.c_uint,ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p,ctypes.POINTER(ctypes.c_uint)]
    f.restype=ctypes.c_int
    return lib,f

def check_host(f,case,seed,count):
    out=(ctypes.c_ubyte*60)();at=(ctypes.c_ubyte*60)();result=ctypes.c_uint()
    assert f(seed,count,out,at,ctypes.byref(result))==0
    expected,ret,insert=native.oracle(case)
    assert bytes(out)==expected and result.value==ret and bytes(at)==insert

def behavior(words,work):
    lib,h=host(work);original=score.targets()[FN];cov=[set(),set()];branches=[{},{}]
    cases=[(s,n) for s in range(256) for n in [-2,-1,0,1,2,3,4]]
    for seed,count in cases:
        case=native.fixture(seed,count);check_host(h,case,seed,count)
        for k,code in enumerate([original,words]):
            seen,br=native.Machine(code,case).run();cov[k]|=seen
            for at,outcomes in br.items():branches[k].setdefault(at,set()).update(outcomes)
    mutants={
      'append_before_tail':('D_80149860.head','D_80149860.tail'),
      'wrong_padding_value':('obj->args[i] = -1;','obj->args[i] = 0;'),
      'inactive_publication':('obj->active = 1;','obj->active = 0;'),
      'wrong_return_handle':('result = obj->handle;','result = obj->handle + 1;'),
      'handle_cached_before_insert':('    obj->end = -1;', '    result = obj->handle;\n    obj->end = -1;'),
      'time_cached_before_lock':('    osRecvMesg(D_80142728, 0, 1);','    start = D_80152748;\n    osRecvMesg(D_80142728, 0, 1);')}
    rejected={}
    source=SOURCE.read_text()
    for name,(old,new) in mutants.items():
        assert old in source;p=work/(name+'.c');changed=source.replace(old,new)
        if name=='handle_cached_before_insert':changed=changed.replace('    result = obj->handle;\n    osJamMesg','    osJamMesg')
        if name=='time_cached_before_lock':changed=changed.replace('    start = D_80152748;\n    obj->start', '    obj->start')
        p.write_text(changed);ml,mh=host(work,p,name)
        for seed,count in cases:
            try:check_host(mh,native.fixture(seed,count),seed,count)
            except AssertionError:rejected[name]={'seed':seed,'count':count};break
        assert name in rejected
    adverse=[]
    bad=list(original);bad[0]=0xffffffff
    try:native.Machine(bad,native.fixture(1,2)).run()
    except AssertionError:adverse.append('unknown_opcode')
    else:raise AssertionError('accepted bad opcode')
    bad=list(original);bad[-3]=0 # Suppress restoration of the 40-byte frame.
    try:native.Machine(bad,native.fixture(1,2)).run()
    except AssertionError:adverse.append('stack_restore_corruption')
    else:raise AssertionError('accepted bad restore')
    assert cov[0]==set(range(0,308,4)) and cov[1]==set(range(0,308,4))
    assert all(len(x)==2 for b in branches for x in b.values())
    return {'cases':len(cases),'native_executions':2*len(cases),'unchanged_host_c89_ubsan_runs':len(cases),
            'native_covered_instructions':len(cov[0]),'candidate_covered_instructions':len(cov[1]),
            'all_conditional_outcomes_covered':True,'mutants_rejected':rejected,'adverse_controls':adverse,
            'external_calls':'Allocator, list insertion, receive and release are explicit contract hooks; native internals are not executed.'}

def verify():
    with tempfile.TemporaryDirectory(prefix='constructor-proof-') as t:
        work=Path(t);proof,words,hashes=compile_proof(work);runtime=behavior(words,work)
    addresses=score.image_symbols();target=addresses[FN];call=0x0c000000|((target>>2)&0x3ffffff)
    callers=[{'function':n,'call_site':hex(addresses[n]+4*i)} for n,w in score.targets().items() for i,v in enumerate(w) if v==call]
    return {'schema':1,'status':'NONMATCH','accepted_byte_gain':0,'function':FN,'start':hex(target),
            'end':hex(target+308),'source_sha256':sha(SOURCE.read_bytes()),'flags':FLAGS,
            'archived_source_sha256':sha((ROOT/'cloud/work/game_C41/car_stats_display.floats.c').read_bytes()),
            'target_sha256':sha(struct.pack('>77I',*score.targets()[FN])),
            'toolchain_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','uld','usplit','umerge','uopt','ugen','as1']},
            'gnu_linker_version':run(['mips-linux-gnu-ld','--version']).splitlines()[0],
            'host_compiler_version':run(['cc','--version']).splitlines()[0],
            'base':'cd22879d40b3de443cfde047b86e75e159b6cec6',
            'protected_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
            'context_source_sha256':hashes,'object_adverse_controls':['shortened_ELF_extent','unsupported_relocation'],'compile':proof,'behavior':runtime,'direct_callers':callers}

def main():
    p=argparse.ArgumentParser();p.add_argument('--write',action='store_true');args=p.parse_args()
    result=verify();dest=HERE/'verification.json'
    if args.write:dest.write_text(json.dumps(result,indent=2)+'\n')
    else:assert result==json.loads(dest.read_text()),'Frozen receipt differs'
    print('NONMATCH 24/77; complete GNU/context proof; %s bounded cases'%result['behavior']['cases'])
if __name__=='__main__':main()
