#!/usr/bin/env python3
"""One canonical group diagnostic, preceded by target-layout verification."""
import argparse
import contextlib
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import zlib

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
MEMBERS = ['func_8038'+k for k in ('D200','D328','D498','DA78','E088','E114','F938','FCE0')]

def sha(data): return hashlib.sha256(data).hexdigest()

def inspect(score, path):
    data, sections = score._elf(path)
    header = struct.unpack_from('>16sHHIIIIIHHHHHH',data)
    symbols = [s for i,sec in enumerate(sections) if sec['type']==2
               for s in score._symbol_table(data,sections,i) if s['name']]
    allocated = []
    for i,sec in enumerate(sections):
        raw = struct.unpack_from('>10I',data,header[6]+i*header[11])
        if raw[2]&2 and raw[5]:
            payload = data[raw[4]:raw[4]+raw[5]] if raw[1]!=8 else bytes(raw[5])
            allocated.append(dict(name=sec['name'],type=raw[1],flags=raw[2],address=hex(raw[3]),
                size=raw[5],alignment=raw[8],sha256=sha(payload),nonzero_bytes=sum(x!=0 for x in payload)))
    relocations = []
    for sec in sections:
        if sec['type']==9:
            table = score._symbol_table(data,sections,sec['link'])
            for offset,info in struct.iter_unpack('>II',data[sec['off']:sec['off']+sec['size']]):
                sym = table[info>>8]
                relocations.append(dict(section=sec['name'],offset=hex(offset),type=info&255,
                    symbol=sym['name'],symbol_type=sym['type'],symbol_section=sym['section']))
    functions = {s['name']:dict(address=hex(s['value']),size=s['size'],section=s['section'])
                 for s in symbols if s['type']==2 and s['section'] not in (0,0xFFF1)}
    return dict(sha256=sha(data),elf_type=header[1],allocated=allocated,symbols=symbols,
                functions=functions,relocations=relocations)

def layout(score, temp):
    facts = [('sizeof(void *)',4),('sizeof(s8)',1),('sizeof(s16)',2),('sizeof(s32)',4),('sizeof(f32)',4)]
    sizes = dict(BObject=60,BAttachedEffect=28,BStatusEffect=52,BRecord=104,BDebris=72,
                 BInput=64,BStatus=20,BPlayer=952,BVehicle=2056,BRelease=8,BPool=20,
                 BScene=68,BTexture=36,BTextureBank=8,BColor=4)
    facts += [('sizeof('+name+')',size) for name,size in sizes.items()]
    fields = {
      'BObject':dict(scene_index=4,uv=8,position=44),
      'BAttachedEffect':dict(flags=8,position=16),
      'BStatusEffect':dict(scene_index=0,uv=4,position=40),
      'BRecord':dict(next=0,owner=4,hit_mask=5,kind=6,flags=7,lifetime=8,timer=12,
        collision_extent=16,velocity=20,position=32,previous_position=44,uv=56,
        attached_effect=92,primary=96,secondary=100),
      'BDebris':dict(scene_index=4,uv=8,position=44,lifetime=56,velocity=60),
      'BInput':dict(pressed=4,held=8,reset_mask=48,fire_mask=60),
      'BStatus':dict(flags=0,transition_timer=4,scale_timer=8,scale=12,pitch=16),
      'BPlayer':dict(position=8,velocity=20,uv=44,active=0x308,color=0x34C,blocked=0x359,
        owner=0x35B,input=0x380,kind=0x384,ammo=0x385,status=0x38C,action=0x3A0,
        alpha=0x3A1,transition=0x3A2,latched=0x3A4,transition_time=0x3A8,
        cooldown=0x3AC,pitch=0x3B0,yaw=0x3B4),
      'BVehicle':dict(model=8,extent_fc=0xFC,extent_108=0x108,impulse=0x124,
        horizontal_impulse=0x13C,blocked=0x640,radius=0x654,state=0x6C4),
      'BRelease':dict(next=0,quad=4),'BPool':dict(head=16),
      'BScene':dict(scale=12),'BTextureBank':dict(textures=0),
    }
    facts += [('OFF('+typ+','+name+')',offset) for typ,fields in fields.items() for name,offset in fields.items()]
    source = temp/'layout_probe.c'
    source.write_text('#include "'+str(HERE/'shared_schema.proposed.h')+'"\n'+
      '#define OFF(t,m) ((u32)&((t *)0)->m)\n'+
      'u32 closure_layout[] = {'+','.join(x[0] for x in facts)+'};\n')
    obj = temp/'layout_probe.o'
    score.compile_single(source,FLAGS,obj)
    data,sections = score._elf(obj)
    syms = [s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
    symbol = next(s for s in syms if s['name']=='closure_layout')
    sec = sections[symbol['section']]
    values = struct.unpack_from('>'+str(len(facts))+'I',data,sec['off']+symbol['value'])
    assert values == tuple(x[1] for x in facts), [('layout mismatch',x,value) for x,value in zip(facts,values) if x[1]!=value]
    return dict(status='target IDO layout passed before group compile',count=len(facts),
                facts=[dict(expression=e,value=v) for e,v in facts],object=inspect(score,obj))

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--reference-root',type=Path,required=True)
    p.add_argument('--work-dir',type=Path,required=True)
    p.add_argument('--output',type=Path,default=HERE/'build.json')
    args = p.parse_args()
    root,temp = args.reference_root.resolve(),args.work_dir.resolve()
    temp.mkdir(parents=True,exist_ok=True)
    # Fail rather than silently running a second combined compile.
    marker = temp/'combined_compile_started'
    assert not marker.exists(), 'this work directory already consumed its one group compile'
    sys.path.insert(0,str(root/'tools/cloud'))
    spec = importlib.util.spec_from_file_location('closure_once_score',root/'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        raise SystemExit('pinned IDO and MIPS GNU linker required')
    score.ASM_DIR = root/'asm/us/ovl_b'
    source = (HERE/'closure.c').read_bytes()
    assembly = json.loads((HERE/'assembly.json').read_text())
    assert sha(source)==assembly['combined_sha256']
    layout_result = layout(score,temp)
    obj = temp/'closure.o'
    marker.write_text('One approved combined source baseline; do not repeat as a source sweep.\n')
    group = score.compile_group(HERE,obj)
    object_meta = inspect(score,obj)
    target_words = score.targets()
    asset = subprocess.check_output(['git','-C',str(root),'show',BASE+':assets/us/data.bin'])
    dec = zlib.decompressobj(-15); native_image = dec.decompress(asset[0xB6FEC4-0x283D0:])
    assert dec.eof and sha(native_image)=='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
    own_image = score.owndata.ImageData.from_image(native_image,0x8038A400)
    comparisons = {}
    for name in MEMBERS:
        size = len(target_words[name])*4
        if name not in object_meta['functions']:
            comparisons[name] = dict(status='ABSENT; natural inline/elimination/name outcome; no fake keeper',
                                     native_size=size,candidate_size=None)
            continue
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj,name,show=0)
        own = score.owndata.verify(obj,name,target_words[name],address=int(name[-8:],16),
          image=own_image,addresses=lambda n: score.image_symbols().get(n,score.address_named(n)))
        comparisons[name] = dict(status=result.summary(),canonical=asdict(result),
            canonical_notes=list(result.notes),native_size=size,
            candidate_size=object_meta['functions'][name]['size'],
            pinned_image_own_data=asdict(own))
    referenced = {r['symbol'] for r in object_meta['relocations']}
    undefined = {s['name'] for s in object_meta['symbols'] if s['section']==0}
    assert not (undefined & set(MEMBERS)), ('would borrow missing executable child',undefined & set(MEMBERS))
    bindings = {}
    for name in undefined:
        if name in ('sqrtf','fabsf') and name not in referenced: continue
        address = 0x8008D6B0 if name=='math_utility' else score.address_named(name)
        assert address is not None, ('unmapped external',name)
        bindings[name] = address
    script = temp/'closure.ld'
    script.write_text('\n'.join(f'{name} = 0x{address:08X};' for name,address in sorted(bindings.items()))+
      '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) *(.rdata) *(.lit4) *(.lit8) } '+
      '.data 0x81020000 : { *(.data) *(.sdata) } .bss 0x81030000 : { *(.bss) *(.sbss) } '+
      '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    linked = temp/'closure.elf'
    proc = subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(obj)],capture_output=True,text=True)
    assert proc.returncode==0, proc.stderr
    linked_meta = inspect(score,linked)
    assert not linked_meta['relocations'], 'linked relocations remain'
    remaining = {s['name'] for s in linked_meta['symbols'] if s['section']==0}
    assert not (remaining-{'sqrtf','fabsf'}), remaining
    linked_symbols = {s['name']:s for s in linked_meta['symbols']}
    for name,address in bindings.items():
        assert linked_symbols[name]['value']==address and linked_symbols[name]['section']==0xFFF1
    result = dict(status='ONE-COMBINED-CONTEXT DIAGNOSTIC; semantic proof pending; no match/acceptance',
        base=BASE,source_sha256=sha(source),recipe=group,mandatory_backend_flag='as1 -r4300_mul',
        group_compiler_invocations=1,layout=layout_result,object=object_meta,linked=linked_meta,
        comparisons=comparisons,external_bindings={n:hex(a) for n,a in bindings.items()},
        discarded_allocated_metadata=['.reginfo','.options'],
        semantic_elf_local_path=str(linked),object_local_path=str(obj),
        limits='Canonical scorer has no checked-in B own-data artifact; pinned-image own-data checks are separately reported using unmodified owndata API. Research-address link is not native-placement equality.')
    args.output.write_text(json.dumps(result,indent=2,default=lambda x:sorted(x) if isinstance(x,set) else str(x))+'\n')
    print(json.dumps(dict(status=result['status'],layout_facts=layout_result['count'],
      functions={n:f['size'] for n,f in object_meta['functions'].items()},
      absent=[n for n in MEMBERS if n not in object_meta['functions']],
      elf=str(linked),output=str(args.output)),indent=2))

if __name__=='__main__':main()
