#!/usr/bin/env python3
"""Protocol-v2 input-bound stock compiler, independent GNU and behavior proof."""
import argparse
from contextlib import contextmanager, redirect_stdout
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
NAME = 'func_80012234'
ADDRESS = 0x80012234
SIZE = 372
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE = ROOT / 'cloud/matches/boot_tail/func_80012234.c'
BASELINE_TU = HERE/'baseline_lib_11640.c'  # w28p: pre-install TU snapshot

def require(value, message):
    if not value: raise ValueError(message)

def sha(raw): return hashlib.sha256(raw).hexdigest()
def packed(words): return struct.pack('>%dI' % len(words), *words)
def run(cmd, **kw):
    result = subprocess.run([str(x) for x in cmd], capture_output=True, text=True, **kw)
    require(result.returncode == 0, (cmd, result.returncode, result.stdout, result.stderr))
    return result.stdout

def module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result

@contextmanager
def population():
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    try: yield
    finally: score.ASM_DIR = old

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
    require(set(n for n,v in symbols.items() if v[2]==2 and v[3])=={NAME},'unexpected source function')
    start,size,kind,index=symbols[NAME];ti,row,raw=sections['.text']
    require(index==ti and start==0 and size%4==0,'full STT_FUNC starts at zero')
    require(size==SIZE,'full native function extent')
    require(size<=len(raw) and len(raw)-size<16 and not any(raw[size:]),'nonzero text suffix')
    require({n for n,(_,r,_) in sections.items() if r[2]&2 and r[5]}<={'.text','.reginfo'},'unexpected allocated source data')
    require(all(dest==ti and typ in (4,5,6) and site%4==0 and site<size for site,typ,_,dest in relocs),'unexpected original REL')
    addresses=score.image_symbols();bindings=[];expected_bindings={}
    for n,v in symbols.items():
        if v[3]==0:
            a=addresses.get(n,score.address_named(n));require(a is not None,'unresolved '+n);bindings.append('%s = 0x%x;'%(n,a));expected_bindings[n]=a
    script=work/(obj.stem+'.ld');linked=work/(obj.stem+'.elf')
    script.write_text('SECTIONS { .text 0x80012234 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) } }\n'+'\n'.join(bindings))
    run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    ss,sy,rr=elf(linked);i,r,body=ss['.text']
    require(not rr and all(v[3] for v in sy.values()),'unresolved GNU binding')
    require(all(sy[n][0]==a and sy[n][3]==0xfff1 for n,a in expected_bindings.items()),'GNU external binding drift')
    require({n for n,(_,r,_) in ss.items() if r[2]&2 and r[5]}=={'.text'},'unexpected GNU allocation')
    require(sy[NAME]==(ADDRESS,size,2,i) and r[3]==ADDRESS and len(body)==len(raw),'GNU extent/location')
    require(not any(body[size:]),'GNU nonzero suffix')
    native=packed(score.targets()[NAME]);bad=[i for i in range(0,SIZE,4) if native[i:i+4]!=body[i:i+4]]
    extra=sum(any(body[i:i+4]) for i in range(SIZE,size,4))
    return dict(function_bytes=size,complete_text_bytes=len(body),zero_suffix_bytes=len(body)-size,
                relocations=len(relocs),original_relocations=[[site,typ,n] for site,typ,n,_ in relocs],
                external_bindings={n:hex(a) for n,a in sorted(expected_bindings.items())},
                differing_offsets=bad,extra_words=extra,owned_data_bytes=0),list(struct.unpack('>%dI'%(size//4),body[:size]))

def tu_check(work):
    ctx = module(ROOT/'cloud/work/boot_tail_promotion/context_check.py', 'context_12234')
    ctx.CC = compilers.tu_cc('lib_11640', ROOT/'tools/cloud/ido/cc')
    ctx.COMPILER = 'ido7.1'
    row = ctx.check(NAME, work, str(SOURCE.relative_to(ROOT)))
    require(row['status']=='ok', row)
    p = module(ROOT/'cloud/work/boot_tail_promotion/promote_batch.py', 'splice_12234')
    p.PREFIX_DEDUP = {'rom/lib_11640'}
    candidate = dict(fn=NAME, seg=dict(rom_tu='rom/lib_11640', flagset=FLAGS),
                     src=str(SOURCE.relative_to(ROOT)), row=row, slot=None)
    # w28p: the slot is promoted on master, so replay the splice against the packet's pre-install
    # snapshot of the TU (byte-identical to src/rom/lib_11640.c at 55e17941/f45e3940), not the live TU.
    base = work/'baseline_repo'; (base/'src/rom').mkdir(parents=True)
    shutil.copyfile(BASELINE_TU, base/'src/rom/lib_11640.c')
    (base/SOURCE.relative_to(ROOT)).parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(SOURCE, base/SOURCE.relative_to(ROOT))
    p.REPO = base
    out = work/'tu'; out.mkdir()
    p.splice([candidate], out_dir=out)
    compiled = {}
    for label, source in [('before', BASELINE_TU), ('after', out/'lib_11640.c')]:
        obj = work/('tu_'+label+'.o')
        run([ctx.CC, '-c', *ctx.cflags_for(FLAGS), '-o', obj, source])
        _, symbols, _ = elf(obj)
        names = sorted(n for n,v in symbols.items() if v[2]==2 and v[3])
        compiled[label] = {n:asdict(score.compare(obj,n,show=0)) for n in names if n in score.targets()}
    old, new = compiled['before'], compiled['after']
    require(set(new)==set(old)|{NAME}, 'TU function set changed')
    require(new[NAME]['differing']==0 and not new[NAME]['unresolved'] and not new[NAME]['errors'] and not new[NAME]['unverified'] and not new[NAME]['extra_words'], 'actual TU new body not strict')
    require(all(old[n]==new[n] for n in old), 'preserved TU function score changed')
    require(all(not r['differing'] and not r['unresolved'] and not r['unverified'] and not r['errors'] and not r['extra_words'] for r in old.values()), 'existing TU body not strict')
    return dict(context=row, preserved_functions=sorted(old), promoted_shape_claims=sorted(new),
                temporary_only=True, prefix_dedup='rom/lib_11640')


def inputs():
    own = [SOURCE, BASELINE_TU, HERE/'verify.py', HERE/'native.py', HERE/'host.c',
           ROOT/'tests/cloud/test_static_12234_match.py']
    accepted = [ROOT/'cloud/work/frontier/w18c/sources/func_80011F60.c',
                ROOT/'cloud/matches/boot_tail/func_80012200.c',
                ROOT/'cloud/work/frontier/w21c/sources/func_80012730.c', ROOT/'include/types.h']
    result = dict(protocol_version=2, population='static', claims=[NAME],
                  candidate_bytes=SIZE, cartridge_coverage_claimed=False,
                  flags=FLAGS, files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in own+accepted})
    old=score.ASM_DIR
    census=[];census_inputs={}
    try:
        for pop in ('boot_tail','blob','ovl_a','ovl_b'):
            score.ASM_DIR=ROOT/'asm/us'/pop
            ts=score.targets()
            digest=hashlib.sha256()
            for n,ws in sorted(ts.items()):
                digest.update(n.encode()+b'\0'+packed(ws))
                for i,w in enumerate(ws):
                    if w>>26==3 and (0x80000000|((w&0x3ffffff)<<2))==ADDRESS:
                        census.append([pop,n,i*4])
            census_inputs[pop]=digest.hexdigest()
        result['caller_census']=census
        result['caller_census_parsed_inputs']=census_inputs
        require(census==[['boot_tail','func_80012D18',0x490]],'caller census changed')
    finally:score.ASM_DIR=old
    with population():
        ts=score.targets();manifest=score.target_manifest()
        ext=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))['functions']
        row=next(r for r in ext if r['name']==NAME)
        require(int(row['address'],16)==ADDRESS and row['size']==SIZE,'native extent')
        require(min(int(r['address'],16) for r in ext if int(r['address'],16)>ADDRESS)==ADDRESS+SIZE,'next native boundary')
        names=(NAME,'func_80011F60','func_80012200','func_80012D18','func_80012730','func_80010450')
        result['targets']={n:dict(bytes=len(ts[n])*4,words_sha256=sha(packed(ts[n]))) for n in names}
        symbols=score.image_symbols()
        result['symbols']={n:hex(symbols.get(n,score.address_named(n))) for n in sorted(set(re.findall(r'\b(?:func|D)_\w+\b',SOURCE.read_text())))}
        # Authentic single incoming word and ordinary pointer return in caller.
        word=ts['func_80012D18'][0x494//4]
        require((word>>26,word>>21&31,word>>16&31,word&65535)==(0x23,16,4,44),'caller key home changed')
    return result


def layout_check(work):
    checks=[('node_size','sizeof(AudioNode)',20),('node_next','(unsigned int)&((AudioNode*)0)->next',0),
            ('node_previous','(unsigned int)&((AudioNode*)0)->previous',4),
            ('node_buffer','(unsigned int)&((AudioNode*)0)->buffer',8),
            ('node_key','(unsigned int)&((AudioNode*)0)->unknown0C',12),
            ('node_age','(unsigned int)&((AudioNode*)0)->countdown',16),
            ('node_spare','(unsigned int)&((AudioNode*)0)->unknown12',18),
            ('record_size','sizeof(DmaEntry_80012234)',12),
            ('record_length','(unsigned int)&((DmaEntry_80012234*)0)->length',0),
            ('record_source','(unsigned int)&((DmaEntry_80012234*)0)->source',4),
            ('record_destination','(unsigned int)&((DmaEntry_80012234*)0)->destination',8),
            ('word','sizeof(unsigned int)',4),('half','sizeof(unsigned short)',2)]
    src=work/'layout.c'
    src.write_text(SOURCE.read_text()+'\n'+''.join('typedef char check_%s[(%s)==%d?1:-1];\n'%r for r in checks))
    score.compile_single(src,FLAGS,work/'layout.o')
    return {n:v for n,_,v in checks}


def verify(deep=False,write=False):
    observed=inputs();receipt=HERE/'verification.json'
    if not write:require(json.loads(receipt.read_text())['inputs']==observed,'input receipt drift')
    cc=compilers.tu_cc('lib_11640',ROOT/'tools/cloud/ido/cc')
    if not cc.exists():raise SystemExit('IDO absent: run tools/cloud/setup.sh')
    require(compilers.identify_dir(cc.parent)=='ido7.1','not verified stock IDO7.1')
    old_ido=score.IDO;score.IDO=cc.parent
    output=dict(inputs=observed,compiler='ido7.1',compiler_fingerprints={n:sha((cc.parent/n).read_bytes()) for n in compilers.FINGERPRINT_FILES},results={})
    try:
        with tempfile.TemporaryDirectory(prefix='static12234-') as tmp,population():
            work=Path(tmp);obj=work/'candidate.o'
            score.compile_single(SOURCE,FLAGS,obj)
            result=score.compare(obj,NAME,show=0)
            require(result.accepted(),'canonical mismatch')
            sections,symbols,relocs=elf(obj)
            require(symbols[NAME][0:3]==(0,SIZE,2),'full STT_FUNC extent')
            raw=sections['.text'][2]
            require(len(raw)==384 and not any(raw[SIZE:]),'separate zero alignment suffix')
            word=struct.unpack_from('>I',raw,12)[0]
            require((word>>26,word>>21&31,word>>16&31,word&65535)==(9,29,29,65504),'native frame allocation')
            require(len(relocs)==15,'original relocation count')
            output['results']=dict(strict=asdict(result),geometry=dict(function_bytes=SIZE,text_bytes=384,zero_suffix_bytes=12,frame_bytes=32,owned_data_bytes=0))
            if deep:
                for tool in ('mips-linux-gnu-ld','mips-linux-gnu-objdump','mips-linux-gnu-readelf','gcc'):
                    require(shutil.which(tool),'tool absent: '+tool)
                proof,words=gnu(obj,work)
                require(not proof['differing_offsets'] and not proof['extra_words'],'GNU mismatch')
                output['gnu']=proof;output['layout']=layout_check(work);output['tu']=tu_check(work)
                native=module(HERE/'native.py','native12234')
                output['native']=native.prove({'native':score.targets()[NAME],'compiled':words})
                require(all(not v for v in output['native']['unexecuted_offsets'].values()),'uncovered instruction')
                output['accepted_contract_controls']={}
                for source,name in [('cloud/work/frontier/w18c/sources/func_80011F60.c','func_80011F60'),
                                    ('cloud/matches/boot_tail/func_80012200.c','func_80012200'),
                                    ('cloud/work/frontier/w21c/sources/func_80012730.c','func_80012730')]:
                    control=work/(name+'.o');score.compile_single(ROOT/source,FLAGS+' -I'+str(ROOT/'include'),control)
                    cr=score.compare(control,name,show=0);require(cr.accepted(),'accepted contract control changed: '+name)
                    output['accepted_contract_controls'][name]=asdict(cr)
                # Freshly compiled semantic mutations must fail both static and dynamic proofs.
                output['negative_controls']=[]
                for label,before,after in [('wrong_countdown','node->countdown = 2;','node->countdown = 3;'),
                                           ('wrong_dma_length','.length = 0x100;','.length = 0x80;')]:
                    text=SOURCE.read_text();require(before in text,'missing mutation anchor')
                    src=work/(label+'.c');src.write_text(text.replace(before,after));mo=work/(label+'.o')
                    score.compile_single(src,FLAGS,mo);require(not score.compare(mo,NAME,show=0).accepted(),'semantic mutant accepted')
                    _,mw=gnu(mo,work)
                    try:native.prove({label:mw})
                    except ValueError as exc:require('behavior/ABI' in str(exc),'wrong mutant refusal')
                    else:raise ValueError('semantic mutant passed behavior')
                    output['negative_controls'].append(label+' rejected by strict and finite native oracle')
                # Preserve the whole object: an unexplained data allocation cannot be hidden.
                src=work/'owned.c';src.write_text(SOURCE.read_text()+'\nint unexpected_owned_data = 1;\n')
                extra=work/'owned.o';score.compile_single(src,FLAGS,extra)
                try:gnu(extra,work)
                except ValueError as exc:require('unexpected allocated source data' in str(exc),'wrong owned-data refusal')
                else:raise ValueError('unexpected owned data accepted')
                output['negative_controls'].append('unexpected owned allocation refused')
                # Canonical comparison tolerates section alignment. The explicit function-size
                # gate must still reject counting a zero suffix as native function extent.
                extended=bytearray(obj.read_bytes());shoff=struct.unpack_from('>I',extended,32)[0]
                stride,count=struct.unpack_from('>HH',extended,46)
                changed=False
                for i in range(count):
                    row=struct.unpack_from('>10I',extended,shoff+i*stride)
                    if row[1]!=2:continue
                    for at in range(row[4],row[4]+row[5],16):
                        no,value,size,info,other,section=struct.unpack_from('>IIIBBH',extended,at)
                        if info&15==2 and section and size==SIZE:
                            struct.pack_into('>I',extended,at+8,SIZE+4);changed=True
                require(changed,'extent mutant not constructed')
                altered=work/'extended.o';altered.write_bytes(extended)
                try:gnu(altered,work)
                except ValueError as exc:require('full native function extent' in str(exc),'wrong extended-extent refusal')
                else:raise ValueError('extended extent accepted')
                output['negative_controls'].append('full STT_FUNC extent gate rejects zero-suffix extension')
                m,s=native.fixture(0,1,-1,-1,0,0)
                for label,body,memory in [('unknown_opcode',[0xffffffff],m),('unmapped',words,native.Memory(m))]:
                    if label=='unmapped':del memory.banks[native.HEAD]
                    try:native.execute(body,memory,s)
                    except ValueError as exc:require(('unknown opcode' if label=='unknown_opcode' else 'unmapped') in str(exc),'wrong fail-closed refusal')
                    else:raise ValueError('invalid execution accepted')
                    output['negative_controls'].append(label+' refused')
                # Actual exhaustion is outside the valid domain and faults in retail too.
                m,s=native.fixture(0,0,-1,-1,0,0)
                try:native.execute(words,m,s)
                except ValueError as exc:require('unmapped' in str(exc),'wrong exhausted-pool refusal')
                else:raise ValueError('exhaustion unexpectedly survived')
                output['negative_controls'].append('empty exhausted pool dereferences null; explicit precondition')
                m,s=native.fixture(3,0,-1,-1,0,0)
                try:native.execute(words,m,s)
                except ValueError as exc:require('unmapped' in str(exc),'wrong nonempty exhausted-pool refusal')
                else:raise ValueError('nonempty exhaustion unexpectedly survived')
                output['negative_controls'].append('nonempty all-live exhausted pool dereferences null; explicit precondition')
                output['host']={}
                for label,extra in [('O0',['-O0']),('O2',['-O2']),('sanitized',['-O2','-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all','-fno-omit-frame-pointer'])]:
                    exe=work/label
                    run(['gcc','-std=c89','-Wall','-Wextra','-Werror',*extra,HERE/'host.c','-o',exe])
                    output['host'][label]=run([exe],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')).strip()
    finally:score.IDO=old_ido
    if write:
        require(deep,'receipt requires deep proof');receipt.write_text(json.dumps(output,indent=2,sort_keys=True)+'\n')
    else:
        saved=json.loads(receipt.read_text());require(output==saved if deep else all(saved[k]==v for k,v in output.items()),'verification receipt drift')
    return output


if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--deep',action='store_true');p.add_argument('--write',action='store_true')
    args=p.parse_args();result=verify(args.deep,args.write)
    print(json.dumps(result,indent=2,sort_keys=True))
