"""One unchanged canonical O3 compile, with complete emitted-body accounting."""
import argparse
import contextlib
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
from inputs import load,BASE
HERE=Path(__file__).resolve().parent
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'

def sha(raw):return hashlib.sha256(raw).hexdigest()
def inspect(score,path):
    raw,sections=score._elf(path)
    header=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
    symbols=[s for i,sec in enumerate(sections) if sec['type']==2
             for s in score._symbol_table(raw,sections,i) if s['name']]
    functions={s['name']:dict(address=hex(s['value']),size=s['size'],section=s['section'])
               for s in symbols if s['type']==2 and s['section'] not in (0,0xFFF1)}
    relocs=[]
    for sec in sections:
        if sec['type']!=9:continue
        table=score._symbol_table(raw,sections,sec['link'])
        for offset,info in struct.iter_unpack('>II',raw[sec['off']:sec['off']+sec['size']]):
            relocs.append(dict(section=sec['name'],offset=hex(offset),type=info&255,symbol=table[info>>8]['name']))
    allocated=[]
    for i,sec in enumerate(sections):
        h=struct.unpack_from('>10I',raw,header[6]+i*header[11])
        if h[2]&2 and h[5]:
            payload=raw[h[4]:h[4]+h[5]] if h[1]!=8 else bytes(h[5])
            allocated.append(dict(name=sec['name'],bytes=h[5],flags=h[2],sha256=sha(payload)))
    return dict(sha256=sha(raw),functions=functions,relocations=relocs,
                symbols=symbols,allocated_sections=allocated)

def run(reference,toolroot,work):
    source=HERE/'candidate.c';raw=source.read_bytes()
    assert raw.splitlines()[0]==b'/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    sys.path.insert(0,str(toolroot/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('af06c_canonical_score',toolroot/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        return dict(status='SKIP: pinned IDO and MIPS GNU linker required',target_compiler_invocations=0)
    native,image,identities=load(reference)
    targets=score.targets()
    for name,meta in identities.items():
        assert sha(struct.pack('>'+str(len(targets[name]))+'I',*targets[name]))==meta['sha256']
    semantic=json.loads((HERE/'semantics.json').read_text())
    assert semantic['packet_sha256']['candidate.c']==sha(raw),'source changed after semantic checks'
    work.mkdir(parents=True,exist_ok=True)
    marker=work/'target_compile_started'
    assert not marker.exists(),'the one target baseline has already been consumed'
    obj=work/'candidate.o';marker.write_text('One approved canonical O3 source baseline. Do not source-sweep.\n')
    score.compile_single(source,FLAGS,obj)
    object_meta=inspect(score,obj)
    compares={};own_image=score.owndata.ImageData.from_image(image,0x80086A50)
    for name,meta in identities.items():
        if name not in object_meta['functions']:
            compares[name]=dict(status='NATURALLY ABSENT',native_size=meta['size']);continue
        with contextlib.redirect_stdout(io.StringIO()):result=score.compare(obj,name,show=0)
        own=score.owndata.verify(obj,name,targets[name],address=int(meta['address'],16),
                                image=own_image,addresses=lambda n:score.image_symbols().get(n,score.address_named(n)))
        compares[name]=dict(status=result.summary(),canonical=asdict(result),
                            own_data_pinned_image=asdict(own),native_size=meta['size'],
                            emitted_size=object_meta['functions'][name]['size'])
    undefined={s['name'] for s in object_meta['symbols'] if s['section']==0}
    referenced={r['symbol'] for r in object_meta['relocations']}
    bindings={}
    for name in sorted(undefined):
        if name not in referenced:continue
        address=score.image_symbols().get(name,score.address_named(name))
        assert address is not None,('unmapped real external',name)
        bindings[name]=address
    assert 'func_80090308' not in bindings,'private helper body is missing'
    linker=work/'candidate.ld'
    linker.write_text('\n'.join(f'{name} = 0x{address:08X};' for name,address in bindings.items())+
      '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) *(.rdata) *(.lit4) *(.lit8) } '+
      '.data 0x81020000 : { *(.data) *(.sdata) } .bss 0x81030000 : { *(.bss) *(.sbss) } '+
      '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    linked=work/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(linker),'-o',str(linked),str(obj)],check=True,capture_output=True)
    linked_meta=inspect(score,linked);assert not linked_meta['relocations']
    return dict(status='ONE CANONICAL O3 BASELINE; no source-shaping follow-ups',base=BASE,
        source_sha256=sha(raw),flags=FLAGS,mandatory_backend_flag=score.R4300_CC,
        target_compiler_invocations=1,targets=identities,comparisons=compares,
        object=object_meta,linked=linked_meta,external_bindings={n:hex(a) for n,a in bindings.items()},
        object_local_path=str(obj),elf_local_path=str(linked))

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True)
    p.add_argument('--tool-root',type=Path);p.add_argument('--work-dir',type=Path,required=True)
    p.add_argument('--output',type=Path,default=HERE/'baseline.json');args=p.parse_args()
    result=run(args.reference_root.resolve(),(args.tool_root or args.reference_root).resolve(),args.work_dir.resolve())
    args.output.write_text(json.dumps(result,indent=2,default=lambda x:sorted(x) if isinstance(x,set) else str(x))+'\n')
    print(json.dumps({k:v for k,v in result.items() if k in ('status','comparisons','target_compiler_invocations')},indent=2,default=str))
