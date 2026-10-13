#!/usr/bin/env python3
"""Input-bound code-equality research with a mandatory owned-storage refusal."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
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
from tools.conveyor import compilers
NAME='func_80011910'
ADDRESS=0x80011910
SIZE=208
TU='lib_11640'
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE=HERE/'candidate.c'
BASELINE=HERE/'baseline.c'

def require(value,message):
    if not value: raise ValueError(message)
def sha(raw): return hashlib.sha256(raw).hexdigest()
def packed(words): return struct.pack('>%dI'%len(words),*words)
def run(cmd,**kw):
    r=subprocess.run([str(x) for x in cmd],capture_output=True,text=True,**kw)
    require(r.returncode==0,(cmd,r.returncode,r.stdout,r.stderr))
    return r.stdout

def module(path,name):
    spec=importlib.util.spec_from_file_location(name,path)
    result=importlib.util.module_from_spec(spec);spec.loader.exec_module(result)
    return result

@contextmanager
def population():
    old=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us/boot_tail'
    try: yield
    finally: score.ASM_DIR=old

def elf(path):
    """Independent ELF32 big-endian reader; no project relocation application."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'ELF ABI')
    flags=struct.unpack_from('>I',data,36)[0]
    require(flags & 0xf0000000 == 0x10000000 and not flags & 0x20,'MIPS2/o32 ELF contract')
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

def gnu(obj,work,task_definition=False):
    sections,symbols,relocs=elf(obj)
    require(set(n for n,v in symbols.items() if v[2]==2 and v[3])=={NAME},'unexpected source function')
    start,size,kind,index=symbols[NAME];ti,row,raw=sections['.text']
    require(index==ti and start==0 and size%4==0,'full STT_FUNC starts at zero')
    require(size<=len(raw) and len(raw)-size<16 and not any(raw[size:]),'nonzero text suffix')
    require({n for n,(_,r,_) in sections.items() if r[2]&2 and r[5]}<=({'.text','.reginfo','.MIPS.abiflags','.bss'} if task_definition else {'.text','.reginfo','.MIPS.abiflags'}),'unexpected allocated source data')
    if task_definition:
        bi,br,bb=sections['.bss']
        require(br[1]==8 and br[5]==64 and br[8]==16 and symbols['D_800382F8']==(0,64,1,bi),'task definition contract')
    require(all(dest==ti and typ in (4,5,6) and site%4==0 and site<size for site,typ,_,dest in relocs),'unexpected original REL')
    addresses=score.image_symbols();bindings=[];expected_bindings={}
    for n,v in symbols.items():
        if v[3]==0:
            a=addresses.get(n,score.address_named(n));require(a is not None,'unresolved '+n);bindings.append('%s = 0x%x;'%(n,a));expected_bindings[n]=a
    script=work/(obj.stem+'.ld');linked=work/(obj.stem+'.elf')
    task_clause=' .task 0x800382f8 (NOLOAD) : SUBALIGN(4) { *(.bss) } ' if task_definition else ''
    script.write_text('SECTIONS { .text 0x80011910 : SUBALIGN(4) { *(.text) } '+task_clause+' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) } }\n'+'\n'.join(bindings))
    run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    ss,sy,rr=elf(linked);i,r,body=ss['.text']
    require(not rr and all(v[3] for v in sy.values()),'unresolved GNU binding')
    require(all(sy[n][0]==a and sy[n][3]==0xfff1 for n,a in expected_bindings.items()),'GNU external binding drift')
    require({n for n,(_,r,_) in ss.items() if r[2]&2 and r[5]}==({'.text','.task'} if task_definition else {'.text'}),'unexpected GNU allocation')
    if task_definition:
        require(sy['D_800382F8'][0]==0x800382f8 and ss['.task'][1][5]==64 and ss['.task'][1][1]==8,'hypothetical task placement')
    require(sy[NAME]==(ADDRESS,size,2,i) and r[3]==ADDRESS and len(body)==len(raw),'GNU extent/location')
    require(not any(body[size:]),'GNU nonzero suffix')
    native=packed(score.targets()[NAME]);bad=[i for i in range(0,SIZE,4) if native[i:i+4]!=body[i:i+4]]
    extra=sum(any(body[i:i+4]) for i in range(SIZE,size,4))
    return dict(function_bytes=size,complete_text_bytes=len(body),zero_suffix_bytes=len(body)-size,
                relocations=len(relocs),differing_offsets=bad,extra_words=extra,allocated_task_bytes=64 if task_definition else 0,owned_data_bytes=0,hypothetical_bss_subalign=4 if task_definition else None),list(struct.unpack('>%dI'%(size//4),body[:size]))

def inputs():
    files=[HERE/n for n in ('candidate.c','baseline.c','verify.py','native.py','probes.py','trace.py','trace_summary.json')]
    files += [ROOT/'tests/cloud/test_static_11910_research.py']
    with population():
        manifest=score.target_manifest();words=score.targets()[NAME]
        extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
        row=next(r for r in extents['functions'] if r['name']==NAME)
        require(int(row['address'],16)==ADDRESS and row['size']==SIZE,'extent changed')
        require(min(int(r['address'],16) for r in extents['functions'] if int(r['address'],16)>ADDRESS)==ADDRESS+SIZE,'next boundary changed')
        symbols=score.image_symbols()
        names=set(re.findall(r'\bD_\w+\b',SOURCE.read_text()))|{'osWritebackDCache',NAME,'func_800119E0','func_80011104','func_800118C0'}
        return {'protocol_version':2,'population':'static','claims':[],
                'files':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in files},
                'target':{'extent':row,'words_sha256':sha(packed(words))},
                'symbols':{n:hex(symbols.get(n,score.address_named(n))) for n in sorted(names)},
                'cartridge_coverage_claimed':False}


def layout_check(work):
    offsets={'type':0,'flags':4,'boot':8,'boot_size':12,'code':16,'code_size':20,
             'data':24,'data_size':28,'stack':32,'stack_size':36,'output':40,
             'output_size':44,'commands':48,'command_count':52,'yield_data':56,'yield_size':60}
    text=SOURCE.read_text()+'\n'
    for field,offset in offsets.items():
        text+='typedef char check_%s[((unsigned int)&((AudioTask *)0)->%s)==%d?1:-1];\n'%(field,field,offset)
    text+='typedef char check_size[sizeof(AudioTask)==64?1:-1];\ntypedef char check_pointer[sizeof(void *)==4?1:-1];\n'
    p=work/'layout.c';p.write_text(text);score.compile_single(p,FLAGS,work/'layout.o')
    return {'task_bytes':64,'pointer_bytes':4,'field_offsets':offsets}


def tu_check(work):
    ctx=module(ROOT/'cloud/work/boot_tail_promotion/context_check.py','context11910')
    ctx.CC=compilers.tu_cc(TU,ROOT/'tools/cloud/ido/cc');ctx.COMPILER='ido7.1'
    row=ctx.check(NAME,work,str(SOURCE.relative_to(ROOT)))
    require(row['status']=='refused' and '.bss:64' in row['reason'] and 'outside an owned slot' in row['reason'],row)
    original=ROOT/'src/rom'/(TU+'.c')
    needle='#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/'+TU+'/'+NAME+'.s")'
    text=original.read_text();require(text.count(needle)==1,'unpromoted TU slot required')
    scratch=work/'diagnostic_tu.c';scratch.write_text(text.replace(needle,SOURCE.read_text()))
    results={}
    for label,path in [('before',original),('diagnostic_only',scratch)]:
        obj=work/('tu_'+label+'.o')
        # C-only diagnostic. This deliberately does not claim to pass the refused integration gate.
        run([ctx.CC,'-c',*ctx.cflags_for(FLAGS),'-o',obj,path])
        _,symbols,_=elf(obj)
        names=sorted(n for n,v in symbols.items() if v[2]==2 and v[3] and n in score.targets())
        results[label]={n:asdict(score.compare(obj,n,show=0)) for n in names}
    before,after=results['before'],results['diagnostic_only']
    require(set(after)==set(before)|{NAME},'TU function membership changed')
    require(all(before[n]==after[n] for n in before),'preserved TU function score drift')
    require(all(not r['differing'] and not r['extra_words'] and not r['unresolved'] and not r['unverified'] and not r['errors'] for r in after.values()),'TU C body not exact')
    return {'context_gate':row,'preserved_exact_c_bodies':sorted(before),'candidate_in_tu':after[NAME],
            'temporary_only':True,'assembly_passthroughs_unchanged':True,'full_rom_not_run':True,
            'integration_permitted':False,'task_bss_owner_registered':False}


def native_gnu(work):
    src=ROOT/'asm/us/nonmatchings/rom'/TU/(NAME+'.s')
    text=src.read_text();text=text[text.index('glabel '+NAME):]
    text=text.replace('glabel '+NAME,'.globl '+NAME+'\n.type '+NAME+',@function\n'+NAME+':')
    text=text.replace('endlabel '+NAME,'.size '+NAME+',.-'+NAME)
    asm=work/'native.s';asm.write_text('.set noreorder\n.text\n'+text)
    obj=work/'native.o';run(['mips-linux-gnu-as','-32','-mips2','-o',obj,asm])
    proof,words=gnu(obj,work)
    require(proof['function_bytes']==SIZE and not proof['differing_offsets'] and not proof['extra_words'],'GNU native target mismatch')
    return proof,words


def verify(deep=False,write=False):
    pinned=inputs();receipt=HERE/'verification.json'
    if not write: require(json.loads(receipt.read_text())['inputs']==pinned,'input receipt drift')
    cc=compilers.tu_cc(TU,ROOT/'tools/cloud/ido/cc')
    if not cc.is_file():raise SystemExit('run tools/cloud/setup.sh (IDO7.1 missing)')
    require(compilers.identify_dir(cc.parent)=='ido7.1','stock per-TU compiler required')
    old=score.IDO;score.IDO=cc.parent
    result={'inputs':pinned,'compiler':'ido7.1','flags':FLAGS,'status':'CODE_EQUAL_STORAGE_BLOCKED','newly_matched_bytes':0,'landed_rom_bytes':0}
    try:
        with tempfile.TemporaryDirectory(prefix='static11910-') as tmp,population():
            work=Path(tmp);obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
            comparison=score.compare(obj,NAME,show=0)
            require(comparison.differing==0 and comparison.total==52 and not comparison.extra_words and not comparison.unresolved and not comparison.unverified and not comparison.errors,'stock candidate changed')
            sections,symbols,relocs=elf(obj)
            require(symbols[NAME][:3]==(0,208,2),'full symbol extent changed')
            require(len(sections['.text'][2])==208 and not any(sections['.text'][2][208:]),'zero padding changed')
            require(struct.unpack_from('>I',sections['.text'][2])[0]&65535==0xffe8,'frame changed')
            result['strict']=asdict(comparison)
            result['geometry']={'native_bytes':208,'candidate_bytes':208,'text_bytes':208,'zero_padding_bytes':0,'unowned_bss_bytes':64,'frame_bytes':24}
            if deep:
                for tool in ('mips-linux-gnu-as','mips-linux-gnu-ld','mips-linux-gnu-objdump'):
                    require(shutil.which(tool),'missing '+tool)
                result['gnu'],candidate=gnu(obj,work,task_definition=True)
                require(len(result['gnu']['differing_offsets'])==0,'GNU difference disagreement')
                result['native_gnu'],native=native_gnu(work)
                machine=module(HERE/'native.py','native11910')
                result['behavior']=machine.prove({'native':native,'stock_candidate':candidate})
                result['layout']=layout_check(work)
                result['tu']=tu_check(work)
                probe=module(HERE/'probes.py','probes11910')
                result['source_probes']={}
                for label,text in dict(probe.variants(BASELINE.read_text()),stock_equal=SOURCE.read_text()).items():
                    p=work/(label+'.c');p.write_text(text);o=p.with_suffix('.o');score.compile_single(p,FLAGS,o)
                    ss,sy,rr=elf(o);r=score.compare(o,NAME,show=0)
                    result['source_probes'][label]={'strict':asdict(r),'function_bytes':sy[NAME][1],
                        'allocated_data':{n:row[5] for n,(_,row,_) in ss.items() if row[2]&2 and row[5] and n not in ('.text','.reginfo')}}
                bad=work/'bad_scale.c';bad.write_text(SOURCE.read_text().replace('count * 2576','count * 2560'))
                bo=bad.with_suffix('.o');score.compile_single(bad,FLAGS,bo)
                require(not score.compare(bo,NAME,show=0).accepted(),'negative strict false positive')
                _,badwords=gnu(bo,work,task_definition=True)
                try: machine.prove({'bad_scale':badwords})
                except ValueError as exc: require('behavior/ABI mismatch' in str(exc),'wrong semantic rejection')
                else: raise ValueError('bad cache scale accepted')
                try: machine.execute([0xffffffff],0,0,0,0,False)
                except ValueError as exc: require('unknown opcode' in str(exc),'wrong decoder rejection')
                else: raise ValueError('unknown opcode accepted')
                stack_bad=list(candidate)
                sites=[i for i,w in enumerate(stack_bad) if w>>26==43 and w>>21&31==29 and w>>16&31==31]
                require(len(sites)==1,'one stack RA store')
                stack_bad[sites[0]]-=4
                try: machine.prove({'wrong_stack_slot':stack_bad})
                except ValueError as exc: require('stack write outside contract' in str(exc),'wrong stack rejection')
                else: raise ValueError('wrong stack store accepted')
                result['negative_controls']=['wrong cache scale rejected by independent behavior oracle','unknown opcode rejected','wrong stack store rejected before return']
    finally: score.IDO=old
    if write:
        require(deep,'receipt requires deep replay');receipt.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:
        stored=json.loads(receipt.read_text())
        require(result==stored if deep else all(stored[k]==v for k,v in result.items()),'verification result drift')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--deep',action='store_true');p.add_argument('--write',action='store_true');a=p.parse_args()
    print(json.dumps(verify(a.deep,a.write),indent=2,sort_keys=True))
