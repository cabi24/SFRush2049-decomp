#!/usr/bin/env python3
"""Complete compiler evidence and bounded callback-contract differential checks."""
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
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
spec=importlib.util.spec_from_file_location('cone_start_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='func_8010DCFC'; START=0x8010DCFC
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=['func_80090284','vector_normalize_length','math_utility']
SYMS={k:int(v,16) for k,v in json.loads((ROOT/'asm/us/blob/symbols.json').read_text())['symbols'].items()}
T=0x100000; M=0x200000; REPLACEMENT=0x200100; V=0x300000; OLD=0x310000; CHANGED=0x310100
CAR=0x80152818; MODEL=0x8014A250; DESC=0x80117530; HEAD=0x801391F0; FLAGS_ADDR=0x8013FECC
MUTANTS={
 'wrong_velocity_scale':('* 0.125f','* 0.25f'),
 'wrong_hit_flags':('t->flags &= ~6;','t->flags &= ~2;'),
 'wrong_launch_axis':('velocity[1] +=','velocity[0] +='),
 'cached_sound_descriptor':('D_80117530[t->descriptor].sound','descriptor->sound'),
}
def sha(b):return hashlib.sha256(b).hexdigest()
def symbols(obj):
 data,secs=score._elf(obj)
 return [s for i,x in enumerate(secs) if x['type']==2 for s in score._symbol_table(data,secs,i)]
def symbol(obj,name):
 values=[s for s in symbols(obj) if s['name']==name and s['type']==2]
 assert len(values)==1;return values[0]
def inspect(obj,name):
 c=score.compare(obj,name,show=0);sym=symbol(obj,name)
 return dict(asdict(c),verdict=c.summary(),symbol_bytes=sym['size'],native_bytes=4*len(score.targets()[name]))
def link(work,obj,name,tag,own=False):
 data,secs=score._elf(obj);ti=score._text_index(secs);sym=symbol(obj,name)
 base=SYMS[name]-sym['value']; script=work/(tag+'.ld')
 defs={}
 for s in symbols(obj):
  n=s['name']
  if n!=name and n in SYMS:defs[n]=SYMS[n]
  elif re.fullmatch(r'(?:D_|func_)[0-9A-Fa-f]{8}',n):defs[n]=int(n.rsplit('_',1)[1],16)
 script.write_text('SECTIONS { .text 0x%X : SUBALIGN(4) { *(.text) } .rodata 0x8012388C : SUBALIGN(4) { *(.rodata) } }\n'%base+''.join('%s = 0x%X;\n'%(n,a) for n,a in defs.items()))
 elf=work/(tag+'.elf');subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
 linked,sections=score._elf(elf);text=sections[score._text_index(sections)]
 body=linked[text['off']+sym['value']:text['off']+sym['value']+sym['size']]
 words=list(struct.unpack('>%dI'%(len(body)//4),body));want=score.targets()[name]
 relocations=[]
 for sec in secs:
  if sec['type']==9 and sec['info']==ti:
   names=score._symbol_table(data,secs,sec['link'])
   for offset in range(sec['off'],sec['off']+sec['size'],8):
    at,info=struct.unpack_from('>II',data,offset)
    if sym['value']<=at<sym['value']+sym['size']:relocations.append({'offset':at-sym['value'],'type':info&255,'symbol':names[info>>8]['name']})
 row=inspect(obj,name);diff=[4*i for i in range(max(len(words),len(want))) if i>=len(words) or i>=len(want) or words[i]!=want[i]]
 row['frame_bytes']=-native.signed(words[0]&65535,16) if words[0]>>16==0x27BD else 0
 row.update(complete_differing_positions=len(diff),complete_difference_offsets=diff,gnu_body_sha256=sha(body),relocations=relocations)
 assert row['differing']==sum(words[i]!=w for i,w in enumerate(want) if i<len(words))+max(0,len(want)-len(words))
 assert not any(row[x] for x in ('unresolved','unverified','errors'))
 # GNU readelf provides a second, external extent check of the original object.
 listing=subprocess.run(['mips-linux-gnu-readelf','-Ws',str(obj)],check=True,capture_output=True,text=True).stdout
 assert any(name==line.split()[-1] and str(sym['size'])==line.split()[2] for line in listing.splitlines() if len(line.split())>=8)
 if own:
  ro,=[x for x in secs if x['name']=='.rodata'];pool=data[ro['off']:ro['off']+ro['size']]
  assert len(pool)==16 and pool[:4]==score.own_data().read(0x8012388C,4) and not any(pool[4:])
  row['context_literal_bytes']=4;row['context_literal_sha256']=sha(pool[:4])
 return row,words

def compiler_proof(work):
 source=HERE/'candidate.c';obj=work/'candidate.o';score.compile_single(source,FLAGS,obj)
 row,words=link(work,obj,FN,'candidate')
 assert row['symbol_bytes']==664 and row['differing']==148 and row['complete_differing_positions']==149
 data,secs=score._elf(obj);text=secs[score._text_index(secs)]
 assert text['size']==672 and not any(data[text['off']+664:text['off']+672])
 assert not any(x['size'] for x in secs if x['name'] in ('.data','.bss','.rodata'))
 row['excluded_zero_text_alignment_bytes']=8
 controls={}
 sources={
  'o2':(source.read_text(),FLAGS.replace('-O3','-O2')),
  'authentic_scale_macro':(source.read_text().replace('void func_8010DCFC(Target *t)', '#define ScaleVector(v1,s,r) (r[0] = v1[0]*(s), r[1] = v1[1]*(s), r[2] = v1[2]*(s))\nvoid func_8010DCFC(Target *t)').replace('    velocity[0] = car->velocity[0] * 0.125f;\n    velocity[1] = car->velocity[1] * 0.125f;\n    velocity[2] = car->velocity[2] * 0.125f;', '    ScaleVector((car->velocity),0.125f,velocity);'),FLAGS),
  'genuine_basis_36':(source.read_text().replace('    MATRIX tmat;','    struct { f32 uvs[3][3]; } tmat;').replace('tmat.mat3.uvs','tmat.uvs'),FLAGS),
  'archived_singleformal':((ROOT/'cloud/work/heads_B14/func_8010DCFC_singleformal.c').read_text(),FLAGS.replace('-O3','-O2')),
 }
 for label,(s,flags) in sources.items():
  p=work/(label+'.c');p.write_text(s);o=work/(label+'.o');score.compile_single(p,flags,o)
  result,_=link(work,o,FN,label);result['source_sha256']=sha(s.encode());result['flags']=flags;controls[label]=result
 context=work/'context';context.mkdir();paths=[source]+[ROOT/'src/blob'/(n+'.c') for n in CONTEXT]
 for i,p in enumerate(paths):shutil.copyfile(p,context/('c%d.c'%i))
 (context/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(4)],'members':[FN],'context':CONTEXT,'keep':[FN]+CONTEXT,'flags':FLAGS}))
 context_obj=context/'group.o';score.compile_group(context,context_obj)
 ctx={}
 for n in [FN]+CONTEXT:
  result,linked=link(work,context_obj,n,'context_'+n,True)
  if n==FN:assert linked==words
  else:assert result['verdict']=='MATCH' and result['complete_differing_positions']==0
  ctx[n]=result
 return {'candidate':row,'controls':controls,'context':ctx,'source_sha256':sha(source.read_bytes()),'context_source_sha256':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths[1:]}},words

def memory(regions):
 def access(a,width,v=None):
  for base,data in regions:
   off=a-base
   if 0<=off and off+width<=len(data):
    if v is None:return int.from_bytes(data[off:off+width],'big')
    data[off:off+width]=(v&((1<<(8*width))-1)).to_bytes(width,'big');return
  raise AssertionError(('oracle unmapped',hex(a),width))
 return access

def initial(case):
 regions=[(T,bytearray([0xA5]*112)),(M,bytearray(24)),(REPLACEMENT,bytearray(24)),(V,bytearray(24)),(CAR,bytearray([0xA5]*1904)),(MODEL,bytearray([0xA5]*4112)),(DESC,bytearray([0xA5]*144)),(HEAD,bytearray(4)),(FLAGS_ADDR,bytearray(2))]
 m=memory(regions);m(T+4,1,case[4]);m(T+12,4,-17);m(T+16,2,1);m(T+80,2,case[3]);m(T+92,1,0);m(T+108,4,M)
 m(FLAGS_ADDR,1,case[1]);m(FLAGS_ADDR+1,1,case[0]);m(DESC+48+20,2,case[2]);m(DESC+48+12,4,0x400000);m(DESC+96+12,4,0x400100)
 m(DESC+48+28,4,123);m(DESC+96+28,4,456);m(CAR+863,1,case[5]);m(CAR+952+863,1,0);m(MODEL+1600,1,case[6]);m(MODEL+2056+1600,1,0);m(HEAD,4,OLD)
 for i in range(3):m(CAR+20+4*i,4,case[9+i]);m(M+4*i,4,case[12+i]);m(M+12+4*i,4,case[15+i])
 for i in range(9):m(T+20+4*i,4,case[18+i])
 return regions

def handler(case,events):
 def call(dst,args,m):
  if dst==SYMS['model_data_load']:
   assert args[:3]==[0xFFFFFFEF,0,15];events.append((1,))
  elif dst==SYMS['save_write_data']:
   assert args==[T+68,0,native.to_bits(0.4),1];events.append((2,))
  elif dst==SYMS['func_80090284']:
   events.append((3,))
   if case[8]&1:m(T+92,1,1);m(T+16,2,2)
   return 0 if case[7] else V
  elif dst==SYMS['vector_normalize_length']:
   assert args[0]==M+12;events.append((4,))
   for i in range(9):m(args[1]+4*i,4,native.to_bits((i+1)+native.to_float(m(M+12+4*(i%3),4))))
   if case[8]&2:m(T+80,2,339);m(T+92,1,1);m(T+16,2,2);m(T+108,4,REPLACEMENT);m(HEAD,4,CHANGED)
  elif dst==SYMS['math_utility']:
   assert args[1]==T+20;events.append((5,))
   for i in range(9):m(T+20+4*i,4,m(args[0]+4*i,4))
  elif dst==SYMS['stat_lap_split']:
   assert args[2:]==[T+56,2];events.append((6,args[0],args[1]))
  else:raise AssertionError(('unknown callback',hex(dst)))
 return call

def oracle(case):
 regions=initial(case);m=memory(regions);events=[]
 def clear():m(T+4,1,m(T+4,1)&~6)
 if case[0]&255 and (case[2]&65535)==236:
  m(CAR+863,1,1);events.append((1,));clear();return regions,events
 if case[1]&255 and (case[2]&65535)==236:
  events.extend([(1,),(2,)]);m(MODEL+1600,1,m(MODEL+1600,1) or 1);clear();return regions,events
 events.append((3,))
 if case[8]&1:m(T+92,1,1);m(T+16,2,2)
 if case[7]:return regions,events
 m(V+4,2,0);m(V+12,4,T);m(V+16,4,native.to_bits(5));m(V+20,4,0x400000);clear()
 values=[native.to_bits(native.to_float(case[9+i])*0.125) for i in range(3)]
 for i in range(3):m(M+12+4*i,4,values[i])
 basis=[native.to_bits((i+1)+native.to_float(values[i%3])) for i in range(9)]
 events.append((4,))
 if case[8]&2:m(T+80,2,339);m(T+92,1,1);m(T+16,2,2);m(T+108,4,REPLACEMENT);m(HEAD,4,CHANGED)
 rise=1 if m(T+80,2) in (339,237) else 2
 m(M+16,4,native.to_bits(native.to_float(values[1])+rise));m(M,4,0);m(M+4,4,native.to_bits(12));m(M+8,4,native.to_bits(15))
 events.append((5,))
 for i,w in enumerate(basis):m(T+20+4*i,4,w)
 m(V,4,m(HEAD,4));m(HEAD,4,V);events.append((6,123 if m(T+16,2)==1 else 456,m(T+92,1)))
 return regions,events

def normalized(regions,events):
 m=memory(regions);out=[0]*64
 out[:11]=[m(T+4,1),m(CAR+863,1),m(CAR+952+863,1),m(MODEL+1600,1),m(MODEL+2056+1600,1),{0:0,OLD:1,CHANGED:2}[m(V,4)],m(V+4,2),int(m(V+12,4)==T),m(V+16,4),{0:0,0x400000:1,0x400100:2}[m(V+20,4)],{OLD:1,CHANGED:2,V:3}[m(HEAD,4)]]
 for i in range(3):out[11+i]=m(M+4*i,4);out[14+i]=m(M+12+4*i,4)
 for i in range(9):out[17+i]=m(T+20+4*i,4)
 out[26]=len(events)
 for i,e in enumerate(events):out[27+i]=e[0]
 out[37:40]=[m(T+92,1),m(T+16,2),m(T+80,2)]
 for e in events:
  if e[0]==6:out[40:42]=e[1:]
 out[42]=int(m(T+108,4)==REPLACEMENT)
 return out

def cases():
 rng=random.Random(0xDCFC)
 for k in range(2048):
  a=[rng.choice([0,1,255]),rng.choice([0,1,128]),rng.choice([236,237,0,65535]),rng.choice([339,237,0,350]),rng.randrange(256),rng.randrange(256),rng.choice([0,1,255]),rng.randrange(2),rng.randrange(4)]
  a += [native.to_bits(rng.randint(-1000,1000)/16) for i in range(18)]
  yield a

def host_library(work,source=None,label='host'):
 source=source or HERE/'candidate.c';so=work/(label+'.so')
 command=['cc','-std=c89','-pedantic-errors','-O1','-shared','-fPIC','-Wall','-Wextra','-Werror','-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all','-DCANDIDATE_SOURCE="%s"'%source,str(HERE/'host.c'),'-o',str(so)]
 subprocess.run(command,check=True,capture_output=True);lib=ctypes.CDLL(str(so));lib.host_case.argtypes=[ctypes.POINTER(ctypes.c_uint),ctypes.POINTER(ctypes.c_uint)];return lib

def behavior(work,words):
 selected=list(cases());lib=host_library(work);coverage=set();branches=set();digest=hashlib.sha256()
 for case in selected:
  expected,events=oracle(case);out=normalized(expected,events)
  host=(ctypes.c_uint*64)();lib.host_case((ctypes.c_uint*27)(*case),host);assert list(host)==out
  for stream in (score.targets()[FN],words):
   seen=[];_,result,reads,writes=native.execute(stream,START,initial(case),[T,0xCAFE,0xBEEF,0xBABE],handler(case,seen),coverage if stream is not words else None,branches if stream is not words else None)
   assert result==expected and seen==events,(case,result,expected)
  digest.update(struct.pack('>64I',*out))
 mutants={}
 for label,(before,after) in MUTANTS.items():
  text=(HERE/'candidate.c').read_text();assert before in text
  p=work/(label+'.c');p.write_text(text.replace(before,after));mutant=host_library(work,p,label)
  rejected=0
  for case in selected:
   host=(ctypes.c_uint*64)();mutant.host_case((ctypes.c_uint*27)(*case),host)
   rejected += list(host)!=normalized(*oracle(case))
  assert rejected;mutants[label]=rejected
 expected_branches={(4*i,take) for i,w in enumerate(score.targets()[FN]) if w>>26 in (4,5,20,21) and ((w>>21)&31)!=((w>>16)&31) for take in (False,True)}
 assert expected_branches <= branches
 assert coverage==set(range(0,660,4))
 return {'conditional_branch_outcomes':len(expected_branches),'cases':len(selected),'native_linked_executions':2*len(selected),'native_covered_offsets':sorted(coverage),'native_branch_outcomes':sorted(branches),'output_sha256':digest.hexdigest(),'compiled_semantic_mutants_rejected':mutants}


def buffer_contract(work):
    # Execute the unchanged accepted 36-byte basis writer against both capacities.
    harness = r"""
#include <assert.h>
#include <math.h>
#include <string.h>
#include "ACCEPTED_SOURCE"
float sqrtf(float);
f32 D_801141C8[3] = {1.0f,0.0f,0.0f};
f32 func_8008B3C8(f32 *v) { return sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); }
void vector_copy_scale(f32 *a,f32 *b) {
    float n=func_8008B3C8(a);int i;
    for(i=0;i<3;i++)b[i]=a[i]/n;
}
int main(void) {
    float v[3]={1.0f,2.0f,3.0f};
#ifdef SHORT_BUFFER
    float output[3];
    vector_normalize_length(v,(float (*)[3])output);
#else
    struct {float basis[3][3];unsigned pos[3];} output;
    int i; memset(&output,0xA5,sizeof(output));
    vector_normalize_length(v,output.basis);
    for(i=0;i<3;i++)assert(output.pos[i]==0xA5A5A5A5U);
#endif
    return 0;
}
""".replace('ACCEPTED_SOURCE',str(ROOT/'src/blob/vector_normalize_length.c'))
    path=work/'bounds.c';path.write_text(harness)
    commands=['cc','-std=c89','-O1','-g','-fno-omit-frame-pointer','-fsanitize=address,undefined','-fno-sanitize-recover=all']
    results={}
    for label,define in [('matrix48',[]),('legacy_array12',['-DSHORT_BUFFER'])]:
        out=work/label
        subprocess.run(commands+define+[str(path),'-lm','-o',str(out)],check=True,capture_output=True)
        r=subprocess.run([str(out)],capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS="detect_leaks=0"))
        if label=='matrix48':assert r.returncode==0,r.stderr
        else:assert r.returncode!=0 and 'AddressSanitizer: stack-buffer-overflow' in r.stderr,r.stderr
        results[label]={'exit_code':r.returncode,'stack_buffer_overflow_rejected':label=='legacy_array12'}
    results['accepted_writer_source_sha256']=sha((ROOT/'src/blob/vector_normalize_length.c').read_bytes())
    return results

def caller_audit():
    calls=[]
    for name,words in score.targets().items():
        for i,w in enumerate(words):
            if w>>26==3 and (((SYMS[name]+4*i+4)&0xF0000000)|((w&0x3FFFFFF)<<2))==START:
                calls.append({'caller':name,'offset':4*i})
    needle=struct.pack('>I',START);pointers=[]
    for base,data in score.own_data().runs:
        for i in range((-base)%4,len(data)-3,4):
            if data[i:i+4]==needle:pointers.append(hex(base+i))
    assert not calls
    return {'direct_calls':calls,'aligned_protected_data_references':pointers,'limit':'Data pointers support callback registration, not runtime dispatcher execution or an original C signature.'}

def main(output=None):
 with tempfile.TemporaryDirectory(prefix='cone-start-proof-') as tmp:
  work=Path(tmp);compiler,words=compiler_proof(work);runtime=behavior(work,words)
  result={'status':'NONMATCH','claims':[],'accepted_byte_gain':0,'compiler':compiler,'behavior':runtime,'buffer_contract':buffer_contract(work),'caller_audit':caller_audit(),'source_hashes':{p.name:sha(p.read_bytes()) for p in (HERE/'candidate.c',HERE/'host.c',HERE/'native.py',HERE/'verify.py')},'toolchain_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},'flags':FLAGS,'native_target_sha256':sha(struct.pack('>165I',*score.targets()[FN]))}
  if output:Path(output).write_text(json.dumps(result,indent=2)+'\n')
  return result
if __name__=='__main__':
 result=main(sys.argv[1] if len(sys.argv)>1 else None);print(json.dumps({'status':result['status'],'candidate':result['compiler']['candidate'],'behavior':result['behavior']},indent=2))
