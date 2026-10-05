#!/usr/bin/env python3
"""Fresh whole-body, GNU relocation, accepted-source and bounded behavior proof.

Source admissibility of the inherited volatile declaration is separate from
machine-code equality. No protected files, compiler flags, keep lists, source
locks, or production TUs are changed. Native words remain in temporary files.
"""
import argparse, ctypes, dataclasses, hashlib, itertools, json, os
from pathlib import Path
import random, struct, subprocess, sys, tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from native import Machine, reference, BASE, CARS
FN='func_80108DA8'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE=HERE/'candidate.c'

def sha(data):return hashlib.sha256(data).hexdigest()
def symbol(obj,name):
    data,sections=score._elf(obj)
    found=[s for i,sec in enumerate(sections) if sec['type']==2
           for s in score._symbol_table(data,sections,i) if s['name']==name and s['type']==2]
    assert len(found)==1,(name,found)
    return found[0]
def strict(obj,name):
    result=dataclasses.asdict(score.compare(obj,name,show=0))
    result['elf_function_bytes']=symbol(obj,name)['size']
    return result

def code_proof(work):
    obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
    sy=symbol(obj,FN);assert sy['size']==408
    start=sy['value'];end=start+sy['size'];want=score.targets()[FN]
    assert len(want)==102
    resolved,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),start,end,score.image_symbols())
    words=resolved[start//4:end//4]
    assert words==want and not any((masks,unresolved,unverified,errors))
    result=strict(obj,FN);assert score.compare(obj,FN,show=0).accepted()
    data,sections=score._elf(obj)
    assert all(sec['size']==0 for sec in sections if sec['name'] in ('.data','.rodata','.rdata','.bss'))
    names=score.image_symbols()
    undefined=[s for i,sec in enumerate(sections) if sec['type']==2
               for s in score._symbol_table(data,sections,i) if s['section']==0 and s['name']]
    script=work/'proof.ld'
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n'%(BASE-start)+
                      ''.join('%s = 0x%x;\n'%(s['name'],names.get(s['name'],score.address_named(s['name']))) for s in undefined))
    elf=work/'proof.elf';binary=work/'proof.bin'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)],check=True,capture_output=True)
    assert symbol(elf,FN)['value']==BASE and symbol(elf,FN)['size']==408
    b=binary.read_bytes();body=b[start:end]
    assert body==struct.pack('>102I',*want) and not any(b[end:])
    result.update(source_sha256=sha(SOURCE.read_bytes()),flags=FLAGS,start=hex(BASE),end_exclusive=hex(BASE+408),
                  full_extent_equal=True,gnu_link_equal=True,body_sha256=sha(body),
                  own_data_bytes=0,object_function_offset=start,alignment_bytes=len(b)-end,
                  preceding_bytes_excluded=start)
    return result,words

def cases():
    # Exhaustive branch/width combinations, then callback mutation adversaries.
    for args in itertools.product(range(5),[-1,0,1,2,3,4],[0,8],[1,2],[0,1],[-1,0,1],[-128,-1,0,1,2,127]):
        yield tuple(args)+(0,)
    for args in itertools.product(range(4),[1,2,3,4],[0,8],[1,2],[0,1],[0,1],[-1,0,1,2],range(1,5)):
        yield args

def host_library(work,source=None):
    directory=work if source is None else work/source
    directory.mkdir(exist_ok=True)
    if source:
        text=SOURCE.read_text()
        before,after={'constant_hidden_return':('return Hidden(blt, 1);','Hidden(blt, 1); return 1;'),
                      'wrong_object_mode':('D_80152818[slot].mode == 1','D_80152818[slot].mode == 0')}[source]
        assert before in text
        (directory/'candidate.c').write_text(text.replace(before,after))
        (directory/'host.c').write_text((HERE/'host.c').read_text())
        host=directory/'host.c'
    else:host=HERE/'host.c'
    lib=directory/'host.so'
    subprocess.run(['cc','-std=c99','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                    '-fsanitize=undefined','-fno-sanitize-recover=all',str(host),'-o',str(lib)],check=True,capture_output=True)
    dll=ctypes.CDLL(str(lib));dll.run_case.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)]
    return dll

def semantics(work,candidate_words):
    host=host_library(work);visited=set();count=0;digest=hashlib.sha256()
    target=score.targets()[FN]
    selected=list(cases())
    for args in selected:
        wanted=reference(args)
        inp=(ctypes.c_int*8)(*args);out=(ctypes.c_int*36)();host.run_case(inp,out)
        assert list(out)==wanted,('host',args,list(out),wanted)
        for label,words in [('retail',target),('candidate',candidate_words)]:
            m=Machine(words,args);got=m.run()
            assert got==wanted,(label,args,got,wanted)
            # The volatile car count is read once, only after earlier exclusion guards.
            expected_reads=int(args[0]<args[1] and not args[2]&8)
            assert m.global_reads.count(CARS)==expected_reads
            visited.update(m.visited)
        digest.update(struct.pack('>36i',*wanted));count+=1
    controls={}
    for mutation in ['constant_hidden_return','wrong_object_mode']:
        bad=host_library(work,mutation);rejected=False
        for args in [(0,0,0,2,1,0,0,2),(0,1,0,2,1,0,0,0)]:
            out=(ctypes.c_int*36)();bad.run_case((ctypes.c_int*8)(*args),out)
            if list(out)!=reference(args):rejected=True;break
        assert rejected,mutation
        controls[mutation]='rejected'
    return {'cases':count,'native_runs':2*count,'whole_target_instructions_visited':len(visited),
            'target_instructions':102,'host_ubsan':'passed','ordered_call_snapshots':'equal',
            'callee_save_and_stack':'preserved','output_sha256':digest.hexdigest(),
            'negative_controls':controls,'limits':'Bounded integer instruction interpreter and mocked audited O32 callees; no async-thread, real-callee, or cartridge execution.'}

def accepted_regressions(work):
    lock=json.loads((ROOT/'blob_matched.lock.json').read_text());result={}
    for fn in ['func_800EC914','func_800EF5B0','Input_InitPadHandlers']:
        entry=lock[fn];source=ROOT/entry['source'];assert sha(source.read_bytes())==entry['source_sha256']
        obj=work/(fn+'.o');score.compile_single(source,entry['flagset'],obj)
        assert score.compare(obj,fn,show=0).accepted()
        result[fn]={'source':entry['source'],'source_sha256':entry['source_sha256'],'comparison':strict(obj,fn)}
    group=ROOT/'src/blob/groups/frontier_pad_config';obj=work/'pad_config.o'
    score.compile_group(group,obj)
    spec=json.loads((group/'group.json').read_text())
    for fn in spec['members']:
        assert score.compare(obj,fn,show=0).accepted()
        result[fn]={'group':str(group.relative_to(ROOT)),'comparison':strict(obj,fn)}
    return result

def main():
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    with tempfile.TemporaryDirectory(prefix='blit-108da8-proof-') as tmp:
        work=Path(tmp);code,words=code_proof(work)
        result={'status':'EXACT-BODY; VOLATILE-CONTRACT REVIEW PENDING','accepted_byte_gain':0,
                'base':'cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5','code':code,
                'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
                'compiler_sha256':sha((score.IDO/'cc').read_bytes()),
                'sources':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in [SOURCE,HERE/'host.c',HERE/'native.py',HERE/'verify.py',HERE/'audit_contract.py']},
                'semantics':semantics(work,words),'accepted_regressions':accepted_regressions(work)}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
