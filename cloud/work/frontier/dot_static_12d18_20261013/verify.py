#!/usr/bin/env python3
"""Protocol-v2 full-object NONMATCH research; only prefix behavior is claimed."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shlex
import struct
import subprocess
import sys
import tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
def local_module(filename):
    spec=importlib.util.spec_from_file_location('static12d18_'+filename,HERE/(filename+'.py'))
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    return module
experiments=local_module('experiments')
native=local_module('native')
host=local_module('host')
NAME='func_80012D18'
ADDRESS=0x80012D18
SIZE=3148
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'

def require(value,message):
    if not value:raise ValueError(message)
def sha(raw):return hashlib.sha256(raw).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)
def run(command,**kw):
    p=subprocess.run([str(x) for x in command],capture_output=True,text=True,**kw)
    require(p.returncode==0,(command,p.returncode,p.stdout,p.stderr))
    return p.stdout
@contextmanager
def population():
    old=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us/boot_tail'
    try:yield
    finally:score.ASM_DIR=old

def elf(path):
    """Independent ELF32 big-endian reader; no project relocation application."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'ELF ABI')
    flags = struct.unpack_from('>I', data, 36)[0]
    require(flags & 0xf0000000 == 0x10000000 and not flags & 0x20, 'MIPS2 identity (legacy ELF has no o32 flag)' )
    base = struct.unpack_from('>I', data, 32)[0]
    stride, count, ni = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'ELF section stride')
    rows = [struct.unpack_from('>10I', data, base + i * stride) for i in range(count)]
    names = data[rows[ni][4]:rows[ni][4] + rows[ni][5]]
    sections, symbols, tabs, relocs = {}, {}, {}, []
    for i, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4] + row[5]]
        sections[name] = (i, row, raw)
        if row[1] == 2:
            ss = rows[row[6]]
            strings = data[ss[4]:ss[4] + ss[5]]
            tab = []
            for j in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, j)
                name = strings[no:].split(b'\0')[0].decode()
                tab.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tabs[i] = tab
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unexpected RELA')
            for at in range(row[4], row[4] + row[5], 8):
                site, info = struct.unpack_from('>II', data, at)
                relocs.append((site, info & 255, tabs[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs

def gnu(obj,work):
    sections,symbols,relocs=elf(obj)
    require({n for n,v in symbols.items() if v[2]==2 and v[3]}=={NAME},'single complete function')
    start,size,kind,index=symbols[NAME];ti,row,raw=sections['.text']
    require(index==ti and start==0 and size>0 and size%4==0,'STT_FUNC geometry')
    require(size<=len(raw) and len(raw)-size<16 and not any(raw[size:]),'nonzero text suffix')
    require({n for n,(_,r,_) in sections.items() if r[2]&2 and r[5]}<={'.text','.reginfo'},'unexpected allocated data')
    require(all(dest==ti and typ in (4,5,6) and site%4==0 and site<size for site,typ,n,dest in relocs),'invalid original relocations')
    addresses=score.image_symbols();bindings={}
    for n,v in symbols.items():
        if not v[3]:
            a=addresses.get(n,score.address_named(n));require(a is not None,'unresolved '+n);bindings[n]=a
    ld=work/(obj.stem+'.ld');linked=work/(obj.stem+'.elf')
    ld.write_text('SECTIONS { .text 0x80012D18 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) } }\n'+'\n'.join('%s = 0x%x;'%x for x in bindings.items()))
    run(['mips-linux-gnu-ld','-EB','-T',ld,'-o',linked,obj])
    ss,sy,rr=elf(linked);i,r,body=ss['.text']
    require(not rr and all(v[3] for v in sy.values()),'unresolved GNU binding')
    require(all(sy[n][0]==a and sy[n][3]==0xfff1 for n,a in bindings.items()),'external binding drift')
    require({n for n,(_,r,_) in ss.items() if r[2]&2 and r[5]}=={'.text'},'GNU owned allocation')
    require(sy[NAME]==(ADDRESS,size,2,i) and r[3]==ADDRESS and len(body)==len(raw),'GNU extent')
    require(not any(body[size:]),'GNU suffix')
    target=packed(score.targets()[NAME]);words=list(struct.unpack('>%dI'%(size//4),body[:size]))
    bad=sum(i>=size or target[i:i+4]!=body[i:i+4] for i in range(0,SIZE,4))
    frame=-native.signed(words[0]&65535,16)
    require(words[0]>>26==9 and words[0]>>21&31==29 and words[0]>>16&31==29,'frame prologue')
    stack=[native.signed(w&65535,16) for w in words if w>>26==9 and w>>21&31==29 and w>>16&31==29]
    require(stack==[-frame,frame],'balanced static frame')
    save=[(w>>16&31,w&65535) for w in words if w>>26==43 and w>>21&31==29 and w>>16&31 in (16,31)]
    restore=[(w>>16&31,w&65535) for w in words if w>>26==35 and w>>21&31==29 and w>>16&31 in (16,31)]
    require(sorted(save)==sorted(restore)==[(16,24),(31,28)],'callee-save frame contract')
    require(words[-2:]==[0x03e00008,0],'full return including delay slot')
    return dict(function_bytes=size,complete_text_bytes=len(body),zero_suffix_bytes=len(body)-size,
                target_bytes=SIZE,missing_body_words=max(0,(SIZE-size)//4),extra_body_words=max(0,(size-SIZE)//4),
                frame_bytes=frame,static_abi=dict(callee_saved=['s0','ra'],saved_offsets=[24,28],balanced_frame=True,ordinary_return=True),true_body_differing=bad,body_sha256=sha(body[:size]),relocations=len(relocs),
                original_relocations=[[site,typ,n] for site,typ,n,_ in relocs],
                external_bindings={n:hex(a) for n,a in sorted(bindings.items())},owned_data_bytes=0),words


def inputs():
    own=[HERE/x for x in ('verify.py','experiments.py','native.py','host.py','build_trace.sh','claim.json','trace.json')]+[ROOT/'tests/cloud/test_static_12d18_research.py']
    seeds=[experiments.BASE,experiments.NAMED,experiments.ORDER,ROOT/'include/types.h',ROOT/'include/boot_tail_audio_record.h']
    data=dict(protocol_version=2,population='static',claims=[],new_matching_bytes=0,flags=FLAGS,
              files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in own+seeds},
              generated_sources={n:sha(s.encode()) for n,s in experiments.sources().items()})
    with population():
        ts=score.targets();manifest=score.target_manifest();ext=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))['functions']
        row=next(r for r in ext if r['name']==NAME)
        require(int(row['address'],16)==ADDRESS and row['size']==SIZE,'target extent')
        require(min(int(r['address'],16) for r in ext if int(r['address'],16)>ADDRESS)==ADDRESS+SIZE,'next boundary')
        data['targets']={n:dict(bytes=len(ts[n])*4,words_sha256=sha(packed(ts[n]))) for n in (NAME,'func_800139D4','func_80011A64','func_80011D74','func_80012730','func_80012234','func_8001E688')}
        symbols=score.image_symbols();names=set(re.findall(r'\b(?:func|D)_\w+\b',experiments.BASE.read_text()))
        data['symbols']={n:hex(symbols.get(n,score.address_named(n))) for n in sorted(names)}
        data['static_callers']=[[n,i*4] for n,ws in sorted(ts.items()) for i,w in enumerate(ws) if w>>26==3 and (0x80000000|((w&0x3ffffff)<<2))==ADDRESS]
        require(data['static_callers']==[['func_800139D4',164]],'caller contract changed')
    require(json.loads((HERE/'claim.json').read_text())['claims']==[],'research claims')
    return data


def layout(name,geometry):
    frame=geometry['frame_bytes']
    offsets=dict(state=-9,address=-20,length=-28,padded=-44,cmd=-48) if name=='baseline' else dict(state=-13,address=-28,length=-36,padded=-52,cmd=-56)
    return dict(frame=frame,**{n:frame+x for n,x in offsets.items()})


def traces(cc,work,paths,objects):
    require(cc.is_file(),'missing trace compiler');out={}
    for name,path in paths.items():
        d=work/(name+'_trace');d.mkdir();log=d/'cdx.log'
        env=dict(os.environ,CDX_LOG='1',CDX_FN=NAME,CDX_DETAIL_WEB='all',CDX_OUT=str(log),SPLOG='1',CUPLOG='1')
        p=subprocess.run([str(cc),'-c',*shlex.split(FLAGS),'-Wab,-r4300_mul','-I',str(ROOT/'include'),'-I',str(ROOT/'include/PR'),'-o',str(d/'on.o'),str(path)],env=env,text=True,capture_output=True)
        require(p.returncode==0,p.stderr);require((d/'on.o').read_bytes()==objects[name].read_bytes(),'trace fidelity '+name)
        # Stock-to-traced whole-object identity is required with every switch on.
        raw=p.stderr;areas=[int(x) for x in re.findall(r'AREA \S+ proc='+NAME+r' area=(\d+)',raw)]
        homes=re.findall(r'HOME proc='+NAME+r' bit=(\d+) home=(\d+) reused=(\d+) size=(\d+)',raw)
        summary=run(['sh',ROOT/'cloud/work/frontier/tools/trace71/sum71.sh',log])
        out[name]=dict(whole_object_fidelity=True,
                       cfe_area=next(int(x) for x in re.findall(r'AREA f_readnxtinst:\d+ proc='+NAME+r' area=(\d+)',raw)),
                       final_area=areas[-1],spill_home_count=len({r[1] for r in homes}),
                       decisions=summary.splitlines())
    return dict(compiler_fingerprints={f:sha((cc.parent/f).read_bytes()) for f in compilers.FINGERPRINT_FILES},results=out)


def verify(deep=False,write=False,trace=None):
    observed=inputs();receipt=HERE/'verification.json'
    if not write:require(json.loads(receipt.read_text())['inputs']==observed,'input receipt drift')
    cc=compilers.tu_cc('lib_11640',ROOT/'tools/cloud/ido/cc')
    require(compilers.identify_dir(cc.parent)=='ido7.1','stock IDO7.1 identity')
    old=score.IDO;score.IDO=cc.parent
    output=dict(inputs=observed,compiler='ido7.1',compiler_fingerprints={n:sha((cc.parent/n).read_bytes()) for n in compilers.FINGERPRINT_FILES},results={})
    try:
        with tempfile.TemporaryDirectory(prefix='static12d18-') as tmp,population():
            work=Path(tmp);paths={};objects={};bodies={'native':(score.targets()[NAME],dict(frame=120,state=107,address=92,length=84,padded=68,cmd=64))}
            selected=experiments.sources() if deep else {'baseline':experiments.sources()['baseline']}
            for n,s in selected.items():
                p=work/(n+'.c');p.write_text(s);obj=work/(n+'.o');paths[n]=p;objects[n]=obj
                score.compile_single(p,FLAGS+' -I '+str(ROOT/'include')+' -I '+str(ROOT/'include/PR'),obj)
                r=score.compare(obj,NAME,show=0);require(not r.accepted(),'unexpected MATCH needs review')
                require(not r.unresolved and not r.unverified and not r.errors,'incomplete canonical comparison')
                output['results'][n]=dict(canonical=asdict(r))
                if deep:
                    geometry,words=gnu(obj,work);output['results'][n]['gnu']=geometry;bodies[n]=(words,layout(n,geometry))
            if deep:
                output['behavior']=native.prove(bodies)
                output['host_prefix']=host.prove(selected,work,ROOT,native)
                text=selected['baseline'];mutant=work/'wrong_margin.c';mutant.write_text(text.replace('length[0] = frames - 6;','length[0] = frames - 5;'))
                obj=work/'wrong_margin.o';score.compile_single(mutant,FLAGS+' -I '+str(ROOT/'include'),obj);geometry,words=gnu(obj,work)
                try:native.prove({'wrong_margin':(words,layout('baseline',geometry))})
                except ValueError as e:require('planner behavior' in str(e),'wrong mutant refusal')
                else:raise ValueError('semantic mutant accepted')
                output['negative_controls']=['fresh wrong filter-margin C mutant rejected by prefix behavior']
                src=work/'data.c';src.write_text(text+'\nint unexpected_owned_data = 1;\n');obj=work/'data.o';score.compile_single(src,FLAGS+' -I '+str(ROOT/'include'),obj)
                try:gnu(obj,work)
                except ValueError as e:require('unexpected allocated data' in str(e),'wrong allocation refusal')
                else:raise ValueError('owned-data mutant accepted')
                output['negative_controls'].append('complete-object owned-data gate refused fresh allocation')
                c=next(native.cases())
                bad=list(score.targets()[NAME]);bad[0]=0xffffffff
                try:native.execute(bad,c,dict(frame=120))
                except ValueError as e:require('unknown opcode' in str(e),'wrong unknown-body refusal')
                else:raise ValueError('invalid body accepted')
                try:native.Memory().get(0x10,4)
                except ValueError as e:require('unmapped' in str(e),'wrong memory refusal')
                else:raise ValueError('unmapped memory accepted')
                output['negative_controls']+=['unknown instruction refused','unmapped memory refused']
                if trace:
                    tracer=traces(Path(trace).resolve(),work,paths,objects)
                    if write:(HERE/'trace.json').write_text(json.dumps(tracer,indent=2,sort_keys=True)+'\n')
                    else:require(tracer['results']==json.loads((HERE/'trace.json').read_text())['results'],'trace receipt drift')
    finally:score.IDO=old
    if write:
        require(deep,'record requires deep');output['inputs']=inputs();receipt.write_text(json.dumps(output,indent=2,sort_keys=True)+'\n')
    else:
        saved=json.loads(receipt.read_text());require(saved['compiler_fingerprints']==output['compiler_fingerprints'],'compiler drift')
        if deep:require(output==saved,'deep receipt drift')
        else:require(output['results']['baseline']['canonical']==saved['results']['baseline']['canonical'],'baseline drift')
    return output

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--deep',action='store_true');p.add_argument('--write',action='store_true');p.add_argument('--trace',type=Path)
    a=p.parse_args();r=verify(a.deep,a.write,a.trace);print(json.dumps(r,indent=2,sort_keys=True))
