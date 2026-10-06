#!/usr/bin/env python3
"""Source-bound whole-ELF, GNU relocation, genuine context and runtime evidence.
This packet is a 17-word NONMATCH. No acceptance or byte-coverage credit.
"""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
import math
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
spec=importlib.util.spec_from_file_location('cone_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='func_8010E4E4';START=0x8010E4E4;LITERAL=0x801249CC
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE=HERE/'candidate.c'
ARCHIVE=ROOT/'cloud/work/dot_entity_motion/func_8010E4E4.c'
CONTEXT=['sound_position_set','entity_transform_apply','entity_spawn_callback','func_800AFA84','func_8010E694']
EXPECTED_OFFSETS=[8,16,196,212,224,232,236,248,252,272,364,372,376,380,400,404,420]
S=score.image_symbols()
STATE,OBJECT,MODEL,REPLACEMENT=0x10000,0x20000,0x30000,0x40000
DT=0x3E19999A
sha=lambda b:hashlib.sha256(b).hexdigest()

def symbol(obj,name):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    syms=[s for i,x in enumerate(secs) if x['type']==2 for s in score._symbol_table(data,secs,i)
          if s['name']==name and s['type']==2 and s['section']==ti]
    assert len(syms)==1
    return syms[0]

def inspect(obj,name):
    result=asdict(score.compare(obj,name,show=0))
    result.update(verdict=score.compare(obj,name,show=0).summary(),symbol_bytes=symbol(obj,name)['size'],
                  native_bytes=4*len(score.targets()[name]))
    return result

def complete(result):
    return result['verdict']=='MATCH' and result['symbol_bytes']==result['native_bytes'] and not any(
        result[k] for k in ('differing','unresolved','unverified','errors','extra_words'))

def linked_proof(obj,work):
    data,secs=score._elf(obj);ti=score._text_index(secs);text=secs[ti]
    fn=symbol(obj,FN);assert fn['value']==0 and fn['size']==432
    assert text['size']==432
    ro,=[s for s in secs if s['name']=='.rodata']
    own=data[ro['off']:ro['off']+ro['size']]
    assert own[:4]==DT.to_bytes(4,'big')==score.own_data().read(LITERAL,4)
    assert len(own)==16 and not any(own[4:])
    assert not any(s['size'] for s in secs if s['name'] in ('.data','.bss'))
    relocs=[];undefined=set()
    for sec in secs:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,secs,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                at,info=struct.unpack_from('>II',data,off);sym=syms[info>>8]
                relocs.append({'offset':at,'type':info&255,'symbol':sym['name']})
                if sym['section']==0:undefined.add(sym['name'])
    assert len(relocs)==19
    script=work/'link.ld'
    script.write_text('SECTIONS { .text 0x8010E4E4 : SUBALIGN(4) { *(.text) }\n'
        '.rodata 0x801249CC : SUBALIGN(4) { *(.rodata) } }\n'+
        '\n'.join(n+' = '+hex(S[n])+';' for n in sorted(undefined)))
    elf=work/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,sections=score._elf(elf);txt=sections[score._text_index(sections)]
    body=linked[txt['off']:txt['off']+432];words=list(struct.unpack('>108I',body));want=score.targets()[FN]
    differences=[i*4 for i,(a,b) in enumerate(zip(want,words)) if a!=b]
    assert differences==EXPECTED_OFFSETS
    for i,(a,b) in enumerate(zip(want,words)):
        if a!=b:
            assert a>>16==b>>16, 'difference is not solely an immediate'
            assert (a>>21)&31==29 or (a>>26==9 and (a>>16)&31==29), 'non-stack residual'
    assert symbol(elf,FN)['size']==432 and symbol(elf,FN)['value']==START
    lr,=[s for s in sections if s['name']=='.rodata']
    assert linked[lr['off']:lr['off']+4]==own[:4]
    shoff=struct.unpack_from('>I',linked,0x20)[0];shsize=struct.unpack_from('>H',linked,0x2e)[0]
    assert struct.unpack_from('>I',linked,shoff+sections.index(lr)*shsize+12)[0]==LITERAL
    result=inspect(obj,FN)
    assert result['differing']==17 and not any(result[k] for k in ('unresolved','unverified','errors','extra_words'))
    result.update(body_sha256=sha(body),native_sha256=sha(struct.pack('>108I',*want)),
        all_gnu_differing_offsets=differences,all_differences_stack_immediates=True,relocations=relocs,
        own_literal_address=hex(LITERAL),own_literal_bytes=4,excluded_zero_rodata_alignment_bytes=12,
        excluded_text_alignment_bytes=0,native_frame=104,candidate_frame=80)
    return result,words

def relocated_body(obj,name):
    fn=symbol(obj,name);start=fn['value'];end=start+fn['size']
    words=score.text_words(obj)
    got,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,S)
    assert not unresolved and not errors
    data,secs=score._elf(obj);ti=score._text_index(secs)
    own=score.owndata.verify(obj,name,score.targets()[name],address=S[name],image=score.own_data(),
        start=start,addresses=lambda n:S.get(n,score.address_named(n)))
    assert own.ok and set(masks)<=own.sites
    bases=own.bases();fixed=set()
    for rel in secs:
        if rel['type']!=9 or rel['info']!=ti:continue
        syms=score._symbol_table(data,secs,rel['link']);pending=[]
        for off in range(rel['off'],rel['off']+rel['size'],8):
            at,info=struct.unpack_from('>II',data,off)
            if not start<=at<end:continue
            index,typ=info>>8,info&255;sym=syms[index]
            if sym['type']!=3 or sym['section']==ti:continue
            secname=secs[sym['section']]['name'];assert secname in bases
            if typ==5:pending.append((at,index,words[at//4]&65535))
            elif typ==6:
                lo=native.signed(words[at//4]&65535,16)
                his=[x for x in pending if x[1]==index]
                assert his
                for site,_,hi in his:
                    address=bases[secname]+(hi<<16)+lo
                    got[site//4]=(words[site//4]&0xffff0000)|(((address+0x8000)>>16)&65535)
                    fixed.add(site)
                address=bases[secname]+(his[0][2]<<16)+lo
                got[at//4]=(words[at//4]&0xffff0000)|(address&65535);fixed.add(at)
                pending=[x for x in pending if x[1]!=index]
            else:raise AssertionError('unsupported own relocation')
        assert not pending
    assert set(masks)==fixed
    return got[start//4:end//4]

def context_proof(work):
    group=work/'context';group.mkdir()
    names=[FN]+CONTEXT;paths=[SOURCE]+[ROOT/'src/blob'/(n+'.c') for n in CONTEXT]
    lock=json.loads((ROOT/'blob_matched.lock.json').read_text())
    for name,path in zip(CONTEXT,paths[1:]):assert sha(path.read_bytes())==lock[name]['source_sha256']
    for i,path in enumerate(paths):shutil.copyfile(path,group/('c%d.c'%i))
    (group/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(len(names))],
        'members':[FN],'context':CONTEXT,'keep':names,'flags':FLAGS}))
    obj=group/'context.o';score.compile_group(group,obj)
    results={n:inspect(obj,n) for n in names}
    assert all(complete(results[n]) for n in CONTEXT)
    assert results[FN]['differing']==17 and results[FN]['symbol_bytes']==432
    assert not any(results[FN][k] for k in ('unresolved','unverified','errors','extra_words'))
    full={n:relocated_body(obj,n) for n in names}
    assert full[FN]==relocated_body(work/'candidate.o',FN)
    assert all(full[n]==score.targets()[n] for n in CONTEXT)
    hashes={n:sha(struct.pack('>'+str(len(w))+'I',*w)) for n,w in full.items()}
    return {'full_relocated_body_sha256':hashes,'candidate_full_body_equals_standalone':True,'bodies':results,'source_sha256':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths[1:]},
        'existing_exported_roots_preserved':True,
        'limit':'Six real bodies with separate accepted type views; not unified full-game shadow or dependency runtime execution.'}

def controls(work):
    archive=ARCHIVE.read_text();original=SOURCE.read_text()
    variants={'archive':archive,
       'owned_literal_only':archive.replace('extern f32 D_8002EB94, D_801249CC, D_80121DDC[3];',
            'extern f32 D_8002EB94, D_80121DDC[3];').replace('acceleration_scale=D_801249CC;','acceleration_scale=0.15f;'),
       'repeated_clock_only':archive.replace('extern f32 D_8002EB94, D_801249CC, D_80121DDC[3];',
            'extern volatile f32 D_8002EB94;\nextern f32 D_801249CC, D_80121DDC[3];'),
       'without_descriptor_pointer':original.replace('    Kind *kind;\n','').replace(
            '            kind = &D_80117530[obj->kind];\n            if(kind->flags&0x2000)',
            '            if(D_80117530[obj->kind].flags&0x2000)'),
       'cached_clock':original.replace('extern volatile f32 D_8002EB94;','extern f32 D_8002EB94;')}
    result={}
    for name,source in variants.items():
        p=work/(name+'.c');p.write_text(source);o=work/(name+'.o');score.compile_single(p,FLAGS,o)
        result[name]=inspect(o,FN)
    p=work/'o2.o';score.compile_single(SOURCE,FLAGS.replace('-O3','-O2'),p);result['o2']=inspect(p,FN)
    return result

def corpus():
    base=list(map(native.to_bits,[1,-2,3,4,-5,6,7,8,-9,1,.5,.15,0,-.5,.25]))+[1,0,0x2000,0]
    result=[]
    for mode in [0,1,2,0xffff,0x8000,0x10000,0xffff0000,0x7fff]:
      for pause in [0,1,0xffffffff]:
       for flags in [0,0x2000,0xdfff,0xffff]:
        for mutation in [0,1]:
         for timer in [0,0x80000000,native.to_bits(.5),native.to_bits(.5)-1,native.to_bits(.5)+1,
                       native.to_bits(-1),0x7f800000,0xff800000,0x7fc12345]:
          x=base.copy();x[15:19]=[mode,pause,flags,mutation];x[9]=timer;result.append(x)
    special=[0,0x80000000,1,0x80000001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,native.to_bits(-1),native.to_bits(1)]
    for component in list(range(11))+[12,13,14]:
      for value in special:
       x=base.copy();x[component]=value;result.append(x)
    rng=random.Random(0xE4E420261005)
    for _ in range(4096):
      x=[rng.getrandbits(32) for _ in range(15)]+[rng.choice([0,1,0xffff]),rng.choice([0,0,0,1]),rng.randrange(65536),rng.randrange(2)]
      x[11]=DT;result.append(x)
    return result

def native_case(code,x,coverage=None,branches=None):
    regions=[]
    def add(base,size):
        b=bytearray(size);regions.append((base,b));return b
    def put(b,offset,value,n=4):b[offset:offset+n]=(value&((1<<(8*n))-1)).to_bytes(n,'big')
    state=add(STATE,20);obj=add(OBJECT,112);model=add(MODEL,24);add(REPLACEMENT,112)
    put(state,12,OBJECT);put(state,16,x[9]);put(obj,108,MODEL);put(obj,14,-7,2);put(obj,16,2,2)
    for i in range(3):put(model,4*i,x[i]);put(model,12+4*i,x[3+i]);put(obj,56+4*i,x[6+i])
    for name,value in [('D_801170FC',x[16]),('D_8002EB94',x[10]),('D_801249CC',DT)]:put(add(S[name],4),0,value)
    a=add(S['D_80121DDC'],12)
    for i in range(3):put(a,i*4,x[12+i])
    k=add(S['D_80117530'],48*4);put(k,2*48+18,x[17],2);put(k,3*48+18,0x2000,2)
    add(S['D_80143FC8'],4);events=[];output=[0]*20
    def call(dest,args,mem):
        if dest==S['sound_position_set']:
            assert args[1]==OBJECT+20;events.append(1)
            for i in range(3):
                value=mem(args[0]+4*i,4);output[9+i]=value;mem(OBJECT+20+4*i,4,value)
            if x[18]:
                mem(STATE+16,4,native.to_bits(2));mem(S['D_8002EB94'],4,native.to_bits(4))
                mem(OBJECT+16,2,3);mem(OBJECT+14,2,-12);mem(STATE+12,4,REPLACEMENT)
        elif dest==S['entity_transform_apply']:
            assert args[:2]==[STATE,1];events.append(4)
        elif dest==S['entity_spawn_callback']:
            assert args[1:3]==[0,0];events.append(2);output[18]=args[0]
        elif dest==S['func_800AFA84']:
            assert args[:2]==[S['D_80143FC8'],OBJECT];events.append(3)
        else:raise AssertionError(('unexpected callback',hex(dest)))
    _,after,reads,writes=native.execute(code,START,regions,[STATE,x[15],0,0],call,coverage,branches)
    def get(address):
        for base,data in after:
            if base<=address and address+4<=base+len(data):return int.from_bytes(data[address-base:address-base+4],'big')
        raise AssertionError(hex(address))
    for i in range(3):output[i]=get(MODEL+12+4*i);output[3+i]=get(OBJECT+56+4*i);output[6+i]=get(OBJECT+20+4*i)
    output[12]=get(STATE+16);output[13]=len(events);output[14:14+len(events)]=events;output[19]=get(S['D_8002EB94'])
    if (x[15]&65535)==0 or x[16]:
        assert not any(OBJECT<=a<OBJECT+112 or MODEL<=a<MODEL+24 for a,w in reads+writes),'early return accessed object'
    return output,[(base,bytes(data)) for base,data in after]

def canonical(a):
    return [0x7fc00000 if i in list(range(13))+[19] and math.isnan(native.to_float(v)) else v for i,v in enumerate(a)]

def oracle(x):
    f=native.to_float;b=native.to_bits
    op=lambda a,c,which:b({'add':lambda:f(a)+f(c),'sub':lambda:f(a)-f(c),'mul':lambda:f(a)*f(c)}[which]())
    out=x[3:6]+x[6:9]+[0]*6+[x[9],0,0,0,0,0,0,x[10]]
    if x[15]&0xffff==0:out[13]=1;out[14]=4;return canonical(out)
    if x[16]:return canonical(out)
    for i in range(3):
        step=op(x[12+i],DT,'mul');out[i]=op(x[3+i],step,'add')
        out[3+i]=op(x[6+i],op(out[i],step,'add'),'add')
        out[6+i]=out[9+i]=op(x[i],x[10],'mul')
    events=[1];timer=x[9];dt=x[10];flags=x[17];index=-7
    if x[18]:timer=b(2);dt=b(4);flags=0x2000;index=-12
    out[12]=op(timer,dt,'sub');out[19]=dt
    if f(out[12])<=0:
        if flags&0x2000:events.append(2);out[18]=index&0xffffffff
        events.extend([3,4])
    out[13]=len(events);out[14:14+len(events)]=events
    return canonical(out)

MUTANTS={
 'missing_second_gravity':('velocity[i]+D_80121DDC[i]*acceleration_scale','velocity[i]'),
 'wrong_position_accumulation':('obj->position[i] +=','obj->position[i] ='),
 'strict_expiry':('arg0->timer<=0.0f','arg0->timer<0.0f'),
 'wrong_descriptor_flag':('kind->flags&0x2000','kind->flags&0x1000'),
 'skip_cleanup_call':('            entity_transform_apply(arg0,1);','            /* deliberately omitted cleanup */')}

def host_library(work,source,label):
    where=work/label;where.mkdir();(where/'candidate.c').write_text(source)
    shutil.copyfile(HERE/'host.c',where/'host.c');libpath=where/'host.so'
    subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fPIC','-shared',
        '-fsanitize=undefined','-fno-sanitize-recover=all','-ffp-contract=off',str(where/'host.c'),'-o',str(libpath)],check=True,capture_output=True)
    lib=ctypes.CDLL(str(libpath));U=ctypes.c_uint32;lib.host_case.argtypes=[ctypes.POINTER(U),ctypes.POINTER(U)]
    def call(x):
        out=(U*20)();lib.host_case((U*19)(*x),out);return canonical(list(out))
    return call

def runtime_proof(work,linked):
    cases=corpus();host=host_library(work,SOURCE.read_text(),'host');target=score.targets()[FN]
    cov=[set(),set()];br=[set(),set()];expected=[]
    for x in cases:
        a,am=native_case(target,x,cov[0],br[0]);b,bm=native_case(linked,x,cov[1],br[1]);o=oracle(x)
        assert am==bm,'nonstack memory differs'
        assert canonical(a)==canonical(b)==host(x)==o,(x,a,b,o)
        expected.append(o)
    assert len(cov[0])==len(cov[1])==108
    mutants={}
    for name,(old,new) in MUTANTS.items():
        assert old in SOURCE.read_text();fun=host_library(work,SOURCE.read_text().replace(old,new),name)
        failures=sum(fun(x)!=want for x,want in zip(cases,expected));assert failures
        mutants[name]=failures
    bad=list(target);bad[0]=0xffffffff
    try:native_case(bad,cases[0])
    except AssertionError:pass
    else:raise AssertionError('unknown instruction accepted')
    bad=list(target);bad[4]=(bad[4]&0xffff0000)|0x70
    try:native_case(bad,cases[0])
    except AssertionError:pass
    else:raise AssertionError('redirected stack write accepted')
    return {'cases':len(cases),'mips_executions':2*len(cases),'instruction_coverage':list(map(len,cov)),
        'conditional_branch_outcomes':[sorted([list(x) for x in y]) for y in br],
        'source_mutants_rejected':mutants,'decoder_unknown_and_stack_write_controls_rejected':True,
        'models':['protected native','independently GNU-linked candidate','independent binary32 oracle','unchanged host C89 with UBSan'],
        'stack_canaries_saved_registers_return_address_and_complete_nonstack_memory_checked':True,
        'limitations':['Explicit O32 dependency hooks, not runtime execution of real helpers.',
            'Fixed 0.15f owned coefficient; gravity/clock modelled as scalar fixtures with selected callback mutations.',
            'Accessible nonoverlapping typed records and valid descriptor indices 2 and 3 only.',
            'NaNs compared by class; signaling payloads, FCSR flags, clock concurrency and gameplay unverified.']}

def verify():
    # The packet's no-claim boundary is about its base commit e24b47d8, where
    # FN was unlocked; it was matched later (wave 7), so the live lock is not
    # the question this receipt answers.
    import subprocess
    base_lock=subprocess.run(['git','show','e24b47d8:blob_matched.lock.json'],cwd=ROOT,
                             capture_output=True,text=True)
    if base_lock.returncode==0:
        assert FN not in json.loads(base_lock.stdout)
    with tempfile.TemporaryDirectory(prefix='cone-donor-proof-') as tmp:
        work=Path(tmp);obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
        code,linked=linked_proof(obj,work)
        result={'function':FN,'native_interval':['0x8010E4E4','0x8010E694'],'claims':[],
          'source_sha256':sha(SOURCE.read_bytes()),'archive_sha256':sha(ARCHIVE.read_bytes()),
          'support_source_sha256':{n:sha((HERE/n).read_bytes()) for n in ['native.py','host.c','verify.py','provenance.json']},
          'flags':FLAGS,'compiler_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ['cc','cfe','uopt','ugen','as1']},
          'standalone':code,'controls':controls(work),'context':context_proof(work),'runtime':runtime_proof(work,linked)}
        return result

def main():
    p=argparse.ArgumentParser();p.add_argument('--record',action='store_true');p.add_argument('--output',type=Path);args=p.parse_args()
    result=verify();text=json.dumps(result,indent=2,sort_keys=True)+'\n'
    if args.record:(HERE/'verification.json').write_text(text)
    else:assert result==json.loads((HERE/'verification.json').read_text()),'receipt drift'
    if args.output:args.output.write_text(text)
    print(text)
if __name__=='__main__':main()
