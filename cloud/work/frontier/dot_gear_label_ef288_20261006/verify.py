#!/usr/bin/env python3
"""Portable, fail-closed gear-label source and canonical-O3 diagnostic replay."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
import re
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
TOOL_ROOT=Path(os.environ.get('RUSH_TOOL_ROOT',ROOT)).resolve()
sys.path.insert(0,str(TOOL_ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('gear_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='func_800EF288'
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'

def check(ok,message):
    if not ok:raise AssertionError(message)
def sha(b):return hashlib.sha256(b).hexdigest()
def run(args):
    p=subprocess.run(args,capture_output=True,text=True)
    check(p.returncode==0,(args,p.stdout,p.stderr));return p.stdout

def cases():
    for count in [-32768,-1,0,1,2,3,4]:
        for enabled in [0,1,-1]:
            for hidden,kind in [(0,0),(15,0),(0,15),(5,10),(10,5)]:
                for gear in [-128,-1,0,1,9,10,79,80,127]:
                    yield [count,enabled,hidden,kind,32767,0,2,-1,0x7FFFFFFF,-40000,32768,*([gear]*4)]
    for mode in range(1,6):
        for count in [1,2,3,4]:
            for changed in [0,1,2,3,4]:
                for xy in [-0x80000000,-32769,-32768,-1,0,32767,32768,0x7FFFFFFF]:
                    yield [count,1,0,0,xy,mode,changed,-0x80000000,0x7FFFFFFF,-300,40000,-1,0,3,127]
    seed=0xEA288
    for _ in range(512):
        row=[]
        for i in range(15):
            seed=(seed*1664525+1013904223)&0xFFFFFFFF;row.append(native.signed(seed))
        row[0]=row[0]%5;row[1]=1;row[2]&=15;row[3]&=15;row[5]=0
        for i in range(11,15):row[i]=native.signed(row[i],8)
        yield row

def behavior(work,compiled=None,selector_reg=18):
    lib=work/'host.so'
    run(['gcc','-std=c99','-O2','-fPIC','-shared',str(HERE/'host.c'),'-o',str(lib)])
    f=ctypes.CDLL(str(lib)).run_case;f.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];f.restype=ctypes.c_int
    words=score.targets()[FN];cov=set();branches={};cases_run=0
    for row in cases():
        args=(ctypes.c_int*15)(*row);out=(ctypes.c_int*512)();n=f(args,out)
        check(0<n<=128,'host event extent');host=[list(out[i*4:i*4+4]) for i in range(n)]
        m=native.Machine(words,row);got=m.run();check(host==got,('host/native',row,host,got))
        if compiled is not None:
            c=native.Machine(compiled,row,selector_reg);check(c.run()==got,('compiled/native',row))
        cov|=m.visited
        for k,v in m.branches.items():branches.setdefault(k,set()).update(v)
        cases_run+=1
    return {'cases':cases_run,'host_equals_native':True,'compiled_equals_native':compiled is not None,
            'visited_native_words':len(cov),'native_words':len(words),
            'unvisited_offsets':sorted(set(range(0,len(words)*4,4))-cov),
            'branch_outcomes':{str(k):sorted(v) for k,v in sorted(branches.items())}}

def functions(obj):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    return {s['name']:s for i,sec in enumerate(secs) if sec['type']==2
            for s in score._symbol_table(data,secs,i) if s['section']==ti and s['type']==2}

def object_proof(obj,work):
    data,secs=score._elf(obj);ti=score._text_index(secs);t=secs[ti]
    raw=data[t['off']:t['off']+t['size']];words=list(struct.unpack('>%dI'%(len(raw)//4),raw))
    fns=functions(obj);addresses=score.image_symbols();rels=[];externs={}
    for sec in secs:
        if sec['type']!=9 or sec['info']!=ti:continue
        syms=score._symbol_table(data,secs,sec['link']);byoffset={s['value']:s for s in syms if s['type']==2 and s['section']==ti}
        for at in range(sec['off'],sec['off']+sec['size'],8):
            off,info=struct.unpack_from('>II',data,at);sym=syms[info>>8];kind=info&255;n=sym['name']
            check(kind in (4,5,6),('relocation type',kind))
            if sym['section']==ti:
                check(kind==4 and sym['type']==3,('internal relocation',sym))
                addend=(words[off//4]&0x3FFFFFF)<<2
                check(addend in byoffset,('internal addend',addend));n=byoffset[addend]['name'];check(n in addresses,('internal symbol',n))
            else:
                check(sym['section']==0 and sym['type']!=3,('unexpected own-data relocation',sym))
                v=addresses.get(n,score.address_named(n));check(v is not None,('unresolved',n));addresses[n]=v;externs[n]=v
            rels.append({'offset':off,'type':kind,'symbol':n})
    owned={s['name']:s['size'] for s in secs if s['name'] in ('.data','.sdata','.rodata','.rdata','.lit4','.lit8','.bss','.sbss') and s['size']}
    check(not owned,('owned data requires proof',owned))
    script=work/'group.ld';script.write_text('SECTIONS { .text 0x80000000 : { *(.text) } }\n'+''.join('%s = 0x%X;\n'%x for x in sorted(externs.items())))
    elf=work/'group.elf';run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)])
    linked,lsecs=score._elf(elf);lt=lsecs[score._text_index(lsecs)]
    placed=dict(addresses,**{n:0x80000000+s['value'] for n,s in fns.items()});reports={};bodies={};covered=set()
    for n,s in fns.items():
        start=s['value'];size=s['size'];end=start+size
        check(size>0 and size%4==0 and end<=len(raw) and not covered.intersection(range(start,end)),('bad extent',n,s))
        got,masks,unknown,unverified,errors=score.relocate(obj,words,start,end,placed)
        check(not any((masks,unknown,unverified,errors)),('placement relocations',n,masks,unknown,unverified,errors))
        exact=linked[lt['off']+start:lt['off']+end]
        check(exact==struct.pack('>%dI'%(size//4),*got[start//4:end//4]),('GNU/project mismatch',n))
        got,masks,unknown,unverified,errors=score.relocate(obj,words,start,end,addresses)
        check(not any((masks,unknown,unverified,errors)),('native-map relocations',n))
        got=got[start//4:end//4];want=score.targets()[n]
        diff=[i*4 for i in range(max(len(got),len(want))) if i>=len(got) or i>=len(want) or got[i]!=want[i]]
        reports[n]={'symbol_bytes':size,'native_bytes':len(want)*4,'complete_differing_words':len(diff),'differing_offsets':diff,
                    'compiled_words_sha256':sha(struct.pack('>%dI'%len(got),*got)),
                    'native_words_sha256':sha(struct.pack('>%dI'%len(want),*want)),
                    'canonical':asdict(score.compare(obj,n,show=0)),
                    'relocations':[dict(r,offset=r['offset']-start) for r in rels if start<=r['offset']<end]}
        check(not any(reports[n]['canonical'][k] for k in ['unresolved','unverified','errors']),('canonical incomplete',n))
        covered.update(range(start,end));bodies[n]=got
    outside=bytes(raw[i] for i in range(len(raw)) if i not in covered);check(not any(outside),'nonzero text outside ELF extents')
    return {'functions':reports,'owned_data_bytes':0,'zero_alignment_bytes':len(outside),'complete_text_bytes':len(raw),
            'GNU_project_placement_equal':True,'GNU_placement':'0x80000000','native_map_is_separate_diagnostic':True},bodies

def audit_base_context():
    repo=Path(os.environ.get('RUSH_REFERENCE_ROOT',os.environ.get('RUSH_GIT_REPO',ROOT)))
    def archived(path):return run(['git','-C',str(repo),'show',BASE+':'+path])
    context_path='cloud/work/dot_selector_d9058_20261006/context.c'
    original=archived(context_path)
    expected='/* flags: '+FLAGS+' */\n'+original.replace('/* flags: '+FLAGS+' */\n','')
    check((HERE/'context.c').read_text()==expected,'context differs from archived clean C104 source')
    symbols=json.loads(archived('asm/us/blob/symbols.json'))['symbols']
    aliases=sorted(n for n,a in symbols.items() if int(a,16)==native.BASE)
    check(aliases==[FN],('base aliases',aliases))
    locks=json.loads(archived('blob_matched.lock.json'))
    check(not any(n in locks for n in aliases),'target already accepted at base')
    result=subprocess.run(['git','-C',str(repo),'grep','-l',FN,BASE,'--','*.c','*.h'],capture_output=True,text=True)
    check(result.returncode in (0,1),'base definition search failed')
    prior=[]
    for entry in result.stdout.splitlines():
        path=entry.split(':',1)[1];text=archived(path)
        text=re.sub(r'/\*.*?\*/|//[^\n]*','',text,flags=re.S)
        if re.search(r'\b'+FN+r'\s*\([^;{}]*\)\s*{',text):prior.append(path)
    check(not prior,('prior source definitions',prior))
    claims=subprocess.run(['git','-C',str(repo),'grep','-l',FN,BASE,'--','*claim*.json'],capture_output=True,text=True)
    check(claims.returncode==1 and not claims.stdout,'target appears in a base claim file')
    return {'base':BASE,'aliases':aliases,'prior_c_or_h_definitions':prior,
            'base_claim_files':[],'base_locked':False,'context_path':context_path,
            'context_transform':'Move the unchanged canonical O3 header to line 1.',
            'rejected_prior_text':'ollama_analysis/overnight2_decompiled.txt contains an inline-assembly dump, not a C reconstruction.'}

def verify(compile_target=True):
    check((HERE/'candidate.c').read_text().splitlines()[0]=='/* flags: '+FLAGS+' */','literal flag header')
    with tempfile.TemporaryDirectory(prefix='gear-',dir=os.environ.get('TMPDIR')) as temp:
        work=Path(temp);sem=behavior(work)
        report={'status':'SEMANTIC_SOURCE_ONLY','base':BASE,'function':FN,'start':'0x800EF288','end':'0x800EF5B0',
                'native_bytes':808,'native_words_sha256':sha(struct.pack('>202I',*score.targets()[FN])),
                'source_hashes':{n:sha((HERE/n).read_bytes()) for n in ['candidate.c','context.c','group.json','host.c','native.py','verify.py']},
                'behavior':sem,'matching_bytes':0,'claims':[],'base_context':audit_base_context()}
        if compile_target:
            check((score.IDO/'cc').is_file() and shutil.which('mips-linux-gnu-ld'),'pinned IDO and MIPS GNU linker required')
            obj=work/'group.o';score.compile_group(HERE,obj);report['compile'],bodies=object_proof(obj,work)
            # Save only for local independent review, never as a packet artifact.
            if os.environ.get('GEAR_REVIEW_OBJECT'):shutil.copyfile(obj,os.environ['GEAR_REVIEW_OBJECT'])
            report['behavior']=behavior(work,bodies[FN],17)
            report['selector_ABI']={'native_input':'s2','compiled_input':'s1','matching_contract':False}
            report['status']='NONMATCH';report['flags']=FLAGS;report['mandatory_backend_flag']='-r4300_mul'
        return report

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--semantic-only',action='store_true');p.add_argument('--write',action='store_true');a=p.parse_args()
    result=verify(not a.semantic_only)
    if a.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result if a.semantic_only else {'status':result['status'],'behavior':result['behavior'],
          'compile':{n:{k:s[k] for k in ['symbol_bytes','native_bytes','complete_differing_words']} for n,s in result['compile']['functions'].items()}},indent=2))
