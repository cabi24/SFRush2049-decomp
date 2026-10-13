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
NAME = 'func_8001558C'
ADDRESS = 0x8001558c
SIZE = 348
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE = ROOT / 'cloud/matches/boot_tail/func_8001558C.c'

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
    require(size<=len(raw) and len(raw)-size<16 and not any(raw[size:]),'nonzero text suffix')
    require({n for n,(_,r,_) in sections.items() if r[2]&2 and r[5]}<={'.text','.reginfo'},'unexpected allocated source data')
    require(all(dest==ti and typ in (4,5,6) and site%4==0 and site<size for site,typ,_,dest in relocs),'unexpected original REL')
    addresses=score.image_symbols();bindings=[];expected_bindings={}
    for n,v in symbols.items():
        if v[3]==0:
            a=addresses.get(n,score.address_named(n));require(a is not None,'unresolved '+n);bindings.append('%s = 0x%x;'%(n,a));expected_bindings[n]=a
    script=work/(obj.stem+'.ld');linked=work/(obj.stem+'.elf')
    script.write_text('SECTIONS { .text 0x8001558c : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) } }\n'+'\n'.join(bindings))
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
                relocations=len(relocs),differing_offsets=bad,extra_words=extra,owned_data_bytes=0),list(struct.unpack('>%dI'%(size//4),body[:size]))

def inputs():
    files = [SOURCE, HERE/'verify.py', HERE/'host.c', HERE/'native.py', HERE/'trace.py', HERE/'trace_summary.json',
             ROOT/'tests/cloud/test_static_1558c_match.py']
    result = dict(protocol_version=2, population='static', claims=[NAME],
                  candidate_bytes=SIZE, cartridge_coverage_claimed=False,
                  files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in files}, targets={})
    with population():
        manifest = score.target_manifest()
        ts = score.targets()
        extents = json.loads(score.verified_bytes(score.ASM_DIR/'extents.json', manifest))
        row = next(r for r in extents['functions'] if r['name'] == NAME)
        require(int(row['address'],16) == ADDRESS and row['size'] == SIZE, 'native extent')
        require(min(int(r['address'],16) for r in extents['functions'] if int(r['address'],16)>ADDRESS) == ADDRESS+SIZE, 'next function boundary')
        for n in (NAME, 'func_800156E8', 'func_80019490', 'func_800178B0', 'func_80014594', 'func_800145DC'):
            result['targets'][n] = dict(bytes=len(ts[n])*4, words_sha256=sha(packed(ts[n])))
        names = set(re.findall(r'\b(?:func|D)_\w+\b', SOURCE.read_text()))
        addresses = score.image_symbols()
        result['symbols'] = {n:hex(addresses.get(n,score.address_named(n))) for n in sorted(names)}
    return result


def layout_check(work):
    checks = [('header_size','sizeof(ResourceHeader)',40), ('program_size','sizeof(Program)',132),
              ('id','(u32)&((ResourceHeader *)0)->id',4), ('type','(u32)&((ResourceHeader *)0)->type',6),
              ('bank_a','(u32)&((ResourceHeader *)0)->bank_a',28), ('bank_b','(u32)&((ResourceHeader *)0)->bank_b',32),
              ('programs','(u32)&((ResourceHeader *)0)->programs',36), ('u32','sizeof(u32)',4), ('int','sizeof(int)',4)]
    p = work/'layout.c'
    p.write_text(SOURCE.read_text()+'\n'+''.join('typedef char check_%s[(%s)==%d?1:-1];\n'%r for r in checks))
    score.compile_single(p, FLAGS, work/'layout.o')
    return {n:v for n,_,v in checks}


def tu_check(work):
    ctx = module(ROOT/'cloud/work/boot_tail_promotion/context_check.py', 'context_1558c')
    ctx.CC = compilers.tu_cc('lib_15a10', ROOT/'tools/cloud/ido/cc')
    ctx.COMPILER = 'ido7.1'
    row = ctx.check(NAME, work, str(SOURCE.relative_to(ROOT)))
    require(row['status']=='ok', row)
    p = module(ROOT/'cloud/work/boot_tail_promotion/promote_batch.py', 'splice_1558c')
    p.PREFIX_DEDUP = {'rom/lib_15a10'}
    candidate = dict(fn=NAME, seg=dict(rom_tu='rom/lib_15a10', flagset=FLAGS),
                     src=str(SOURCE.relative_to(ROOT)), row=row, slot=None)
    out = work/'tu'; out.mkdir()
    p.splice([candidate], out_dir=out)
    compiled = {}
    for label, source in [('before', ROOT/'src/rom/lib_15a10.c'), ('after', out/'lib_15a10.c')]:
        obj = work/('tu_'+label+'.o')
        run([ctx.CC, '-c', *ctx.cflags_for(FLAGS), '-o', obj, source])
        _, symbols, _ = elf(obj)
        names = sorted(n for n,v in symbols.items() if v[2]==2 and v[3])
        compiled[label] = {n:asdict(score.compare(obj,n,show=0)) for n in names if n in score.targets()}
    old, new = compiled['before'], compiled['after']
    require(set(new)==set(old)|{NAME}, 'TU function set changed')
    require(new[NAME]['differing']==0 and not new[NAME]['unresolved'] and not new[NAME]['errors'] and not new[NAME]['unverified'], 'actual TU new body not strict')
    require(all(old[n]==new[n] for n in old), 'preserved TU function score changed')
    require(all(not r['differing'] and not r['unresolved'] and not r['unverified'] and not r['errors'] and not r['extra_words'] for r in old.values()), 'existing TU body not strict')
    return dict(context=row, preserved_functions=sorted(old), promoted_shape_claims=sorted(new),
                temporary_only=True, prefix_dedup='rom/lib_15a10')


def verify(deep=False, write=False):
    observed = inputs()
    receipt = HERE/'verification.json'
    if not write:
        require(json.loads(receipt.read_text())['inputs']==observed, 'input receipt drift')
    cc = compilers.tu_cc('lib_15a10', ROOT/'tools/cloud/ido/cc')
    require(compilers.identify_dir(cc.parent)=='ido7.1', 'not stock IDO7.1')
    old_ido = score.IDO
    score.IDO = cc.parent
    output = dict(inputs=observed, compiler='ido7.1', results={})
    try:
        with tempfile.TemporaryDirectory(prefix='static1558c-') as tmp, population():
            work = Path(tmp)
            obj = work/'candidate.o'
            score.compile_single(SOURCE, FLAGS, obj)
            result = score.compare(obj, NAME, show=0)
            require(result.accepted(), 'strict candidate mismatch')
            sections, symbols, relocs = elf(obj)
            require(symbols[NAME][0:3] == (0,SIZE,2), 'STT_FUNC extent')
            require(len(sections['.text'][2])==352 and not any(sections['.text'][2][SIZE:]), 'alignment suffix')
            output['results']['strict'] = asdict(result)
            output['results']['geometry'] = dict(function_bytes=SIZE, text_bytes=352, zero_suffix_bytes=4, frame_bytes=56)
            require(struct.unpack_from('>I',sections['.text'][2])[0] & 0xffff == 0xffc8, 'frame allocation')
            if deep:
                for tool in ('mips-linux-gnu-ld','mips-linux-gnu-objdump','gcc'):
                    require(shutil.which(tool), 'tool absent: '+tool)
                proof, words = gnu(obj,work)
                require(not proof['differing_offsets'] and not proof['extra_words'], 'GNU mismatch')
                output['gnu'] = proof
                output['layout'] = layout_check(work)
                output['tu'] = tu_check(work)
                native = module(HERE/'native.py', 'native1558c')
                output['native'] = native.prove({'native':score.targets()[NAME], 'compiled':words})
                text = SOURCE.read_text()
                output['controls'] = {}
                for label,source,expected in [('remove_dead_init',text.replace('        result = id;\n',''),4),
                                              ('early_promoted_read',text.replace('        i = 0;\n        result = id;\n        for (;','        if (id != 0) {}\n        for (i = 0;'),2)]:
                    path = work/(label+'.c'); path.write_text(source)
                    co = work/(label+'.o'); score.compile_single(path,FLAGS,co)
                    cr = score.compare(co, NAME, show=0)
                    require(cr.differing==expected and cr.total==87 and not cr.extra_words and not cr.errors and not cr.unresolved and not cr.unverified, ('causal control',label))
                    cp,cw = gnu(co,work)
                    require(len(cp['differing_offsets'])==expected and cp['function_bytes']==SIZE, 'GNU causal control')
                    output['controls'][label] = dict(strict=asdict(cr),gnu=cp)
                mutant = work/'bad_bank.c'
                mutant.write_text(text.replace('resource->bank_a + 8','resource->bank_a + 12'))
                mo=work/'bad_bank.o';score.compile_single(mutant,FLAGS,mo)
                mr=score.compare(mo,NAME,show=0);require(not mr.accepted(),'bad bank strict false positive')
                _,mw=gnu(mo,work)
                try:native.prove({'bad_bank':mw})
                except ValueError as exc:require('behavior/ABI' in str(exc),'unexpected mutation rejection')
                else:raise ValueError('wrong bank offset accepted')
                try:native.execute([0xffffffff],*native.fixture(0,0,0),0)
                except ValueError as exc:require('unknown opcode' in str(exc),'unexpected unknown-opcode rejection')
                else:raise ValueError('unknown opcode accepted')
                output['negative_controls'] = ['wrong bank pointer rejected by strict score and machine ABI fixtures','unknown opcode rejected']
                output['host'] = {}
                for label,extra in [('O0',['-O0']),('O2',['-O2']),('sanitized',['-O2','-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all','-fno-omit-frame-pointer'])]:
                    exe=work/label
                    run(['gcc','-std=c89','-Wall','-Wextra','-Werror','-Wno-int-to-pointer-cast',*extra,HERE/'host.c','-o',exe])
                    output['host'][label]=run([exe],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')).strip()
    finally:
        score.IDO=old_ido
    if write:
        require(deep, 'only a full verification can write the receipt')
        receipt.write_text(json.dumps(output,indent=2,sort_keys=True)+'\n')
    else:
        saved=json.loads(receipt.read_text())
        require(output==saved if deep else all(saved[k]==v for k,v in output.items()),'verification receipt changed')
    return output


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--deep',action='store_true')
    parser.add_argument('--write',action='store_true')
    args=parser.parse_args()
    print(json.dumps(verify(args.deep,args.write),indent=2,sort_keys=True))
