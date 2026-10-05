#!/usr/bin/env python3
"""Reproduce complete-object NONMATCH and native/linked/host behavior evidence."""
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
spec=importlib.util.spec_from_file_location('radar_semantics',HERE/'verify_semantics.py')
semantics=importlib.util.module_from_spec(spec);spec.loader.exec_module(semantics)
FN='func_80108F40'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=['src/blob/func_800CF604.c','src/blob/func_800A61B0.c','src/blob/groups/frontier_pad_config/group.c']
MEMBERS=['func_800CF604','func_800A61B0','Input_ApplyPadConfig','Input_InitPadHandlers']
def sha(b):return hashlib.sha256(b).hexdigest()

# Public evidence retains the scorer's failure, location and compared length,
# but never republishes a native byte excerpt from an own-data diagnostic.
_DIAGNOSTIC_BYTES=re.compile(
    r"(\(\+0x[0-9a-fA-F]+: retail )[0-9a-fA-F]+"
    r"(, got )[0-9a-fA-F]+(; \d+ bytes compared\))")

def public_evidence(value):
    if isinstance(value,dict):
        return {key:public_evidence(item) for key,item in value.items()}
    if isinstance(value,list):
        return [public_evidence(item) for item in value]
    if isinstance(value,str):
        return _DIAGNOSTIC_BYTES.sub(r"\1[redacted]\2[redacted]\3",value)
    return value
def inspect(obj,name):
    data,sections=score._elf(obj);ti=score._text_index(sections)
    syms=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
    fn,=[s for s in syms if s['name']==name and s['type']==2 and s['section']==ti]
    text=sections[ti];words=list(struct.unpack('>%dI'%(text['size']//4),data[text['off']:text['off']+text['size']]))
    native=score.targets()[name];start=fn['value'];comp=score.compare(obj,name,show=0)
    got,masks,unresolved,unverified,errors=score.relocate(obj,words,start,start+fn['size'],score.image_symbols())
    bounded=got[start//4:(start+fn['size'])//4]
    return dict(asdict(comp),full_symbol_equal=(bounded==native and not(masks or unresolved or unverified or errors)),canonical_verdict=comp.summary(),own_data_notes=list(comp.notes),
                symbol_offset=start,symbol_bytes=fn['size'],native_bytes=len(native)*4,
                stack_frame=(-semantics.signed(words[start//4]&65535,16) if words[start//4]>>16==0x27bd else 0)),fn,words

def verify(directory):
    directory.mkdir(parents=True,exist_ok=True)
    source=HERE/'candidate.c';obj=directory/'candidate.o';score.compile_single(source,FLAGS,obj)
    result,fn,words=inspect(obj,FN)
    assert result['differing']==307 and result['symbol_bytes']==1316 and result['stack_frame']==152
    assert not result['unresolved'] and result['extra_words']==0
    assert all(x.startswith('own .rodata') for x in result['errors'])
    out={'base_revision':'cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5','function':FN,
         'range':['0x80108F40','0x80109468'],'status':'NONMATCH','accepted_byte_gain':0,
         'flags':FLAGS,'assembler_erratum_flag_added_by_scorer':score.R4300_CC,
         'source_sha256':sha(source.read_bytes()),'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
         'object':result}
    data,sections=score._elf(obj);ti=score._text_index(sections);text=sections[ti]
    assert all(x==0 for x in words[(fn['value']+fn['size'])//4:])
    out['trailing_zero_alignment_bytes']=text['size']-fn['value']-fn['size']
    own=[s for s in sections if s['name']=='.rodata'][0]
    literal_bytes=struct.pack('>ff',semantics.f32(1/480),semantics.f32(1/480))
    assert data[own['off']:own['off']+8]==literal_bytes==score.own_data().read(0x801248cc,8)
    assert not any(data[own['off']+8:own['off']+own['size']])
    out['own_literals']={'address':'0x801248CC','bytes_verified':8,'values':[1/480,1/480],
                         'remaining_section_bytes_zero':own['size']-8}
    addresses=score.image_symbols()
    names={}
    relocs=[]
    for sec in sections:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,sections,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                address,info=struct.unpack_from('>II',data,off);sym=syms[info>>8]
                relocs.append({'offset':address-fn['value'],'type':info&255,'symbol':sym['name']})
                n=sym['name']
                if sym['section']==0:
                    names[n]=addresses.get(n,int(n[2:],16) if n.startswith('D_') else None)
                    assert names[n] is not None,n
    out['relocations']=relocs
    start=addresses[FN]
    script=directory/'link.ld'
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } .rodata 0x801248CC : SUBALIGN(4) { *(.rodata) } }\n'%(start-fn['value'])+
                      ''.join('%s = 0x%x;\n'%(n,a) for n,a in names.items()))
    elf=directory/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    ld,secs=score._elf(elf);txt=secs[score._text_index(secs)]
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(ld,secs,i)]
    linked_fn,=[s for s in syms if s['name']==FN]
    assert linked_fn['value']==start and linked_fn['size']==fn['size']
    body=ld[txt['off']+fn['value']:txt['off']+fn['value']+fn['size']]
    linked_words=list(struct.unpack('>%dI'%(len(body)//4),body))
    native=score.targets()[FN]
    differences=[i*4 for i in range(max(len(native),len(linked_words))) if i>=len(linked_words) or i>=len(native) or native[i]!=linked_words[i]]
    out['gnu_link']={'all_undefined_relocations_resolved':True,'symbol_address':hex(start),'symbol_bytes':len(body),
                     'code_sha256':sha(body),'differing_words':len(differences),'differing_word_offsets':differences,
                     'full_symbol_equal':False}
    assert len(differences)>0
    controls={}
    o2=directory/'o2.o';score.compile_single(source,FLAGS.replace('-O3','-O2'),o2);controls['o2']=inspect(o2,FN)[0]
    s=source.read_text()
    a=s.index('    lpos[0]=');b=s.index('    func_800A61B0',a)
    vec='static void vecsub(register f32 *ap,register f32 *bp,register f32 *rp)\n{\n    *rp++=*ap++-*bp++;\n    *rp++=*ap++-*bp++;\n    *rp++=*ap++-*bp++;\n}\n\n'
    s=s[:a]+'    vecsub(gc->dr_pos,view_car->dr_pos,lpos);\n'+s[b:]
    s=s.replace('s32 func_80108F40(Blit *blt)',vec+'s32 func_80108F40(Blit *blt)')
    v=directory/'vecsub.c';v.write_text(s);vo=directory/'vecsub.o';score.compile_single(v,FLAGS,vo)
    controls['authentic_vecsub_function']=inspect(vo,FN)[0]
    group=directory/'context';group.mkdir()
    shutil.copy(source,group/'target.c')
    files=[]
    for i,p in enumerate(CONTEXT):
        name='context_%d.c'%i;shutil.copy(ROOT/p,group/name);files.append(name)
    files.append('target.c')
    (group/'group.json').write_text(json.dumps({'files':files,'flags':FLAGS,'keep':[FN]+MEMBERS,'members':[FN]+MEMBERS}))
    go=directory/'group.o';score.compile_group(group,go)
    controls['genuine_context']={n:inspect(go,n)[0] for n in [FN]+MEMBERS}
    out['accepted_context_sha256']={p:sha((ROOT/p).read_bytes()) for p in CONTEXT}
    assert all(controls['genuine_context'][n]['full_symbol_equal'] for n in MEMBERS)
    out['controls']=controls
    out['semantics']=semantics.verify(directory,linked_words)
    out['packet_sha256']={n:sha((HERE/n).read_bytes()) for n in ('candidate.c','semantic_test.c','verify_semantics.py','verify.py','claim.json')}
    out['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')}
    return public_evidence(out)
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--write',action='store_true');args=p.parse_args()
    with tempfile.TemporaryDirectory(prefix='radar-traffic-proof-') as d:result=verify(Path(d))
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'object':result['object'],'gnu_link':{k:v for k,v in result['gnu_link'].items() if k!='differing_word_offsets'},
                      'controls':{k:({n:v['canonical_verdict'] for n,v in x.items()} if k=='genuine_context' else x['canonical_verdict']) for k,x in result['controls'].items()},
                      'semantics':result['semantics']},indent=2))
