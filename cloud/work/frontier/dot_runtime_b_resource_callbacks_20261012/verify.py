#!/usr/bin/env python3
"""Input-bound complete-object proof of the image-B selection helper group."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers
MEMBERS = {'func_80390B00': 8, 'func_80390B08': 8,
           'func_80390B10': 552, 'func_80390D38': 552}
CONTEXT = 'func_80390F60'
SIZES = dict(MEMBERS, func_80390F60=988)
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
IMAGE_HASH = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
DATA = 0x803942B8


def require(value, message):
    if not value:
        raise ValueError(message)


def sha(value):
    return hashlib.sha256(value).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def image_bytes():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == ASSET_HASH, 'current-tree asset identity')
    require(struct.unpack_from('>I', asset, 0x3854)[0] == 0xB6FEC4, 'loader identity')
    image = zlib.decompress(asset[0xB6FEC4 - 0x283D0:], -15)
    require(len(image) == 43888 and sha(image) == IMAGE_HASH, 'image-B identity')
    return image


@contextmanager
def bound_targets():
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    key = owndata.artifact_dir(score.ASM_DIR)
    existed, prior = key in score._own_data, score._own_data.get(key)
    image = image_bytes()
    score._own_data[key] = owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield image
    finally:
        score.ASM_DIR = old
        if existed:
            score._own_data[key] = prior
        else:
            score._own_data.pop(key, None)


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


def inputs():
    with bound_targets() as image:
        manifest = score.target_manifest()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        require((extents['image'], extents['base'], extents['rom_offset']) == ('B', '0x8038A400', '0xB6FEC4'), 'population identity')
        rows = {r['name']: r for r in extents['functions']}
        targets = score.targets()
        spec = json.loads((HERE / 'group.json').read_text())
        require(spec['members'] == spec['claims'] == list(MEMBERS), 'all exact members must be claimed')
        require(spec['context'] == spec['keep'] == [CONTEXT], 'authentic parent context')
        require(spec['flags'] == FLAGS and spec['files'] == ['group.c'], 'compiler recipe')
        source = (HERE / 'group.c').read_text()
        addresses = score.image_symbols()
        tokens = set(re.findall(r'\b[A-Za-z_]\w*\b', source))
        result = dict(protocol_version=2, population='ovl_b', targets={},
                      files={n: sha((HERE / n).read_bytes()) for n in ('group.c', 'group.json', 'verify.py', 'host.c')},
                      test_sha256=sha((ROOT / 'tests/cloud/test_runtime_b_resource_callbacks.py').read_bytes()),
                      asset_sha256=ASSET_HASH, image_sha256=IMAGE_HASH,
                      symbols={n: hex(addresses.get(n, score.address_named(n))) for n in sorted(tokens)
                               if addresses.get(n, score.address_named(n)) is not None})
        for name, size in SIZES.items():
            raw, address = packed(targets[name]), int(name[-8:], 16)
            require(rows[name]['size'] == size == len(raw) and int(rows[name]['address'], 16) == address, 'native full extent')
            require(raw == image[address - 0x8038A400:address - 0x8038A400 + size], 'target/image words')
            require(min(int(r['address'],16) for r in rows.values() if int(r['address'],16)>address)==address+size,'native next boundary')
            result['targets'][name] = dict(address=hex(address), size=size, words_sha256=sha(raw))
        result['direct_callers'] = {callee: [(name, hex(i * 4)) for name, words in sorted(targets.items())
                                           for i, word in enumerate(words)
                                           if word >> 26 == 3 and (word & 0x3ffffff) * 4 == int(callee[-8:],16) & 0xfffffff]
                                    for callee in MEMBERS}
        require(result['direct_callers'] == {'func_80390B00': [], 'func_80390B08': [],
                'func_80390B10': [(CONTEXT, '0x18')], 'func_80390D38': [(CONTEXT, '0x10')]}, 'native helper caller census')
        # JSON round-trip makes tuple-valued caller rows portable.
        result = json.loads(json.dumps(result))
        old = score.ASM_DIR
        try:
            score.ASM_DIR = ROOT / 'asm/us/blob'
            game = score.targets()
            result['game_caller'] = [(n, hex(i * 4)) for n, words in sorted(game.items()) for i, w in enumerate(words)
                                     if w >> 26 == 3 and (w & 0x3ffffff) * 4 == 0x0390F60]
            require(result['game_caller'] == [('render_viewport_init', '0x68')], 'outer native caller')
            result['helper_targets'] = {n: dict(size=len(game[n])*4, words_sha256=sha(packed(game[n])))
                                        for n in ('func_8008B2E4', 'func_80090770', 'model_transform_setup', 'render_viewport_init')}
        finally:
            score.ASM_DIR = old
        result['context_data'] = dict(address=hex(DATA), size=8, sha256=sha(image[DATA-0x8038A400:DATA-0x8038A400+8]), claimed=False)
        return json.loads(json.dumps(result))


def inspect_complete(obj):
    sections, symbols, relocs = elf(obj)
    ti, _, text = sections['.text']
    allocated = {n for n, r in sections.items() if r[1][2] & 2 and r[1][5]}
    require(allocated == {'.text', '.data', '.reginfo'}, 'unexpected allocated section')
    funcs = {n: v for n, v in symbols.items() if v[2] == 2 and v[3] == ti}
    require(set(funcs) == set(SIZES), 'unaccounted emitted function')
    order = sorted(funcs, key=lambda n: funcs[n][0])
    end = 0
    for name in order:
        start, size, kind, index = funcs[name]
        require(size == SIZES[name] and start == end, 'whole native STT_FUNC/order gap')
        end = start + size
    require(end <= len(text) and len(text)-end < 16 and not any(text[end:]), 'nonzero full-text suffix')
    _, _, data = sections['.data']
    require(len(data) == 16 and not any(data[8:]), 'context table complete data/alignment')
    return sections, symbols, relocs, dict(emitted_order=order, complete_text_bytes=len(text),
                                         meaningful_text_bytes=end, zero_text_suffix=len(text)-end,
                                         context_table_bytes=8, zero_data_suffix=8)


def gnu_proof(obj, work, image):
    sections, symbols, relocs, metadata = inspect_complete(obj)
    ti, _, raw = sections['.text']
    di, _, data = sections['.data']
    lines, externs, counts = ['.set noreorder', '.set noat'], set(), {}
    require(all(dest == ti for _,_,_,dest in relocs), 'unexpected nontext relocation')
    for name, size in SIZES.items():
        start = symbols[name][0]
        words = list(struct.unpack('>%dI' % (size//4), raw[start:start+size]))
        entries = []
        for site, typ, symbol, dest in relocs:
            if not start <= site < start+size:
                continue
            require(typ in (4,5,6) and site % 4 == 0, 'unsupported text relocation')
            off = site-start
            if symbol == '.text':
                require(typ == 4, 'unexpected local code reference')
                addend = (words[off//4]&0x3ffffff)*4
                owners = [(n,v) for n,v in symbols.items() if n in SIZES and v[0] <= addend < v[0]+v[1]]
                require(len(owners)==1, 'unowned code reference')
                symbol, owner = owners[0]
                words[off//4] = (words[off//4]&0xfc000000)|((addend-owner[0])//4)
            if symbol == '.data':
                symbol = 'context_alpha_table'
            require(not symbol.startswith('.'), 'unresolved section-relative reference')
            entries.append((off,typ,symbol));externs.add(symbol)
        lines += ['.section .review_'+name+',"ax",@progbits', '.balign 4', '.globl '+name,
                  '.type '+name+',@function', name+':']
        lines += ['.word 0x%08x' % word for word in words]+['.size %s,.-%s' % (name,name)]
        lines += ['.reloc %s+%d,%s,%s' % (name,off,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[typ],symbol)
                  for off,typ,symbol in entries]
        counts[name] = len(entries)
    lines += ['.section .review_data,"a",@progbits','.globl context_alpha_table','context_alpha_table:',
              '.byte '+','.join(str(b) for b in data[:8])]
    asm,reloc,script,linked = [work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    addresses=score.image_symbols();bindings=[]
    for n in sorted(externs-set(SIZES)-{'context_alpha_table'}):
        address=addresses.get(n,score.address_named(n));require(address is not None,'undefined binding '+n)
        bindings.append('%s = 0x%X;'%(n,address))
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in SIZES)
                     +'.review_data 0x803942B8 : SUBALIGN(4) { *(.review_data) }\n'
                     +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+'\n'.join(bindings))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],
                ['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]), 'GNU MIPS binutils unavailable')
        subprocess.run(cmd,check=True,capture_output=True)
    ss, sy, rr = elf(linked)
    require(not rr and all(v[3] for v in sy.values()), 'unresolved GNU symbol/relocation')
    require({n for n,v in ss.items() if v[1][2]&2 and v[1][5]} == {'.review_'+n for n in SIZES}|{'.review_data'}, 'GNU unexpected allocation')
    diffs={}
    for n,size in SIZES.items():
        index,row,body=ss['.review_'+n];address=int(n[-8:],16)
        require(row[3]==address and len(body)==size and sy[n]==(address,size,2,index), 'GNU complete native extent')
        target=packed(score.targets()[n]);offsets=[i for i in range(0,size,4) if body[i:i+4]!=target[i:i+4]]
        require(offsets==([0x34,0x5c,0x60,0x384,0x390] if n==CONTEXT else []),'GNU full-word comparison')
        diffs[n]=offsets
    _,row,table=ss['.review_data']
    require(row[3]==DATA and len(table)==8 and table==image[DATA-0x8038A400:DATA-0x8038A400+8], 'GNU context table')
    metadata.update(relocations=counts, complete_word_differences=diffs, context_table_sha256=sha(table),
                    claimed_code_bytes=1120, claimed_owned_data_bytes=0)
    return metadata


def installer_check(obj, work, image):
    """Run the installer's actual selective body path in disposable staging only."""
    from tools.conveyor.pipeline import blob_group, ovl_splice
    name = 'dot_runtime_b_resource_callbacks_20261012'
    spec = ovl_splice.packet_spec(HERE)
    require(spec['members'] == list(MEMBERS) and spec['context'] == [CONTEXT],
            'installer packet conversion changed')
    root, objects = work / 'install-groups', work / 'install-objects'
    group = root / name
    group.mkdir(parents=True)
    objects.mkdir()
    (group / 'group.json').write_text(json.dumps(spec))
    shutil.copyfile(HERE / 'group.c', group / 'group.c')
    shutil.copyfile(obj, objects / (name + '.o'))
    native = work / 'installer-image.bin'
    native.write_bytes(image)
    document = {'image': {'path': str(native), 'base': '0x8038A400'},
                'regions': [{'entries': [dict(target_id=n, kind='function',
                             vaddr=int(n[-8:], 16), size=size) for n, size in SIZES.items()]}]}
    bodies = blob_group.group_bodies(name, document=document, extern=score.image_symbols(),
                                     obj_dir=objects, root=root)
    require(set(bodies) == set(MEMBERS) and CONTEXT not in bodies,
            'installer leaked nonmatching parent into output')
    replacement = bytearray(image)
    for n, body in bodies.items():
        require(len(body) == MEMBERS[n] and ovl_splice.mismatch(document, n, body) is None,
                'installer full native body mismatch')
        start = int(n[-8:], 16) - 0x8038A400
        replacement[start:start + len(body)] = body
    require(bytes(replacement) == image, 'selective substitution changed image')
    return dict(output_members={n: len(body) for n, body in bodies.items()},
                unmatched_parent_excluded=True, native_parent_remains_unchanged=True,
                selective_image_sha256=sha(bytes(replacement)),
                production_image_build_performed=False, production_state_written=False,
                rom_verification_performed=False)


def compiler_trace(work):
    normal,traced=work/'normal.o',work/'traced.o'
    score.compile_group(HERE,normal)
    original=score._run; result={}
    def run(cmd, **kw):
        cmd=list(cmd);stage=Path(cmd[0]).name
        if stage=='umerge':cmd.insert(1,'-v')
        if stage=='ugen':cmd+=['-e','trees','-l','listing']
        proc=original(cmd,**kw)
        if stage=='umerge':result['inline_log']=(proc.stdout+proc.stderr).strip().splitlines()
        if stage=='as1' and proc.returncode==0:
            folder=Path(kw['cwd']);listing=(folder/'listing').read_text()
            names=re.findall(r'^\s*\.ent\s+(\w+)',listing,re.M)
            blocks=(folder/'trees').read_text().split('Tree dump after Build')[1:]
            require(len(names)==len(blocks),'stock trace procedures')
            result['memory_areas']={n:{k:int(v) for k,v in re.findall(r'\bdef mtype=(\w+) length=(\d+)',b.split('Tree dump')[0])}
                                    for n,b in zip(names,blocks)}
        return proc
    try:
        score._run=run;score.compile_group(HERE,traced)
    finally:score._run=original
    require(normal.read_bytes()==traced.read_bytes(),'trace changed stock object')
    result['diagnostic_object_identical']=True
    return result


def replay(deep=False):
    binding=inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO)=='ido5.3','recognized stock IDO 5.3 required')
    with bound_targets() as image,tempfile.TemporaryDirectory(prefix='b-resource-proof-') as tmp:
        work=Path(tmp);obj=work/'group.o';score.compile_group(HERE,obj)
        sections,symbols,relocs,whole=inspect_complete(obj)
        members={}
        for n in MEMBERS:
            value=score.compare(obj,n,show=0);require(value.accepted(),n+': '+value.summary());members[n]=asdict(value)
        context=score.compare(obj,CONTEXT,show=0)
        require(context.differing==5 and not context.extra_words and not context.unresolved and not context.unverified and not context.errors,'bounded parent residual')
        # Source-level causal control: direct scalar loses address materialization.
        folder=work/'direct';folder.mkdir();source=(HERE/'group.c').read_text()
        (folder/'group.c').write_text(source.replace('*(f32 *)(unsigned long)&D_8002EB94','D_8002EB94'))
        (folder/'group.json').write_bytes((HERE/'group.json').read_bytes())
        control=folder/'group.o';score.compile_group(folder,control)
        rollback={}
        for n in ('func_80390B10','func_80390D38'):
            value=score.compare(control,n,show=0);require(value.differing==131 and not value.unresolved,'address control changed');rollback[n]=asdict(value)
        key=owndata.artifact_dir(score.ASM_DIR);prior=score._own_data[key]
        try:
            score._own_data[key]=None;value=score.compare(obj,CONTEXT,show=0);require(value.unverified,'missing context data not rejected')
            bad=bytearray(image);bad[DATA-0x8038A400]^=1;score._own_data[key]=owndata.ImageData.from_image(bytes(bad),0x8038A400)
            value=score.compare(obj,CONTEXT,show=0);require(value.errors,'corrupt context data not rejected')
        finally:score._own_data[key]=prior
        result=dict(inputs=binding,compiler='stock IDO 5.3; no static ROM TU rebuilt',
                    compiler_binary_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')},
                    members=members,context=asdict(context),
                    complete_object=whole,causal_control=rollback,context_data_controls={'missing_rejected':True,'corrupt_rejected':True},
                    substantive_new_bytes=1104,deleted_stub_bytes=16,preserved_code_bytes=0,accepted_image_bytes=0,rom_verified_bytes=0)
        if deep:
            result['gnu']=gnu_proof(obj,work,image)
            result['installer']=installer_check(obj,work,image)
            result['stock_trace']=compiler_trace(work)
            exe=work/'host';subprocess.run(['cc','-std=c89','-O2','-Wall','-Wextra','-Werror','-fstrict-aliasing',str(HERE/'host.c'),'-o',str(exe)],check=True,capture_output=True)
            result['host']=json.loads(subprocess.check_output([str(exe)],text=True))
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input binding changed')
    actual=replay(deep)
    if not deep:
        for n in ('gnu','installer','stock_trace','host'):expected.pop(n,None)
    require(actual==expected,'receipt changed')


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');parser.add_argument('--inputs-only',action='store_true');args=parser.parse_args()
    if args.score:
        with bound_targets():
            sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(HERE),'--targets',str(ROOT/'asm/us/ovl_b')];code=score.main()
        raise SystemExit(code)
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    elif args.inputs_only:
        require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'input binding changed');print('PASS: input binding')
    else:
        verify(args.deep);print('PASS: 1104 new helper bytes + 16 deleted-stub bytes; parent NONMATCH; no image/ROM credit')
