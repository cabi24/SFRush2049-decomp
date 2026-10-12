#!/usr/bin/env python3
"""Input-bound image-A BB80 proof; transient native and ELF data never published."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
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

GROUP_NAME = 'dot_runtime_a_text_helpers_20261006'
GROUP = HERE / 'groups' / GROUP_NAME
NEW = 'func_8039BB80'
MEMBERS = {'func_8039B120':244, 'func_8039B214':836, 'func_8039BE48':760,
           'func_8039A448':140, 'func_8039B00C':276, 'func_8039C140':1984,
           'func_8039B908':128, 'func_8039B988':152, 'func_8039BA28':184,
           'func_8039BAE0':160, 'func_8039B560':936, NEW:712}
CONTEXT = ['func_8039A4D4', 'func_803A5658', 'func_803A5948']
KEEP = ['func_8039C140', 'func_803A5948']
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
RODATA = 0x803B9594

def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def image_data():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == ASSET_HASH, 'current-tree asset identity changed')
    require(struct.unpack_from('>I', asset, 0x3850)[0] == 0xB5C534, 'loader pointer changed')
    image = zlib.decompress(asset[0xB5C534 - 0x283D0:], -15)
    require(len(image) == 194128 and sha(image) == IMAGE_HASH, 'image-A identity changed')
    return image


@contextmanager
def target_context():
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(score.ASM_DIR)
    prior = score._own_data.get(key)
    image = image_data()
    score._own_data[key] = owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield image
    finally:
        score.ASM_DIR = old
        if prior is None:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = prior



def elf(path):
    """Independent ELF32 BE reader for GNU and complete symbol/relocation checks."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'not BE ELF32 MIPS')
    base = struct.unpack_from('>I', data, 32)[0]
    stride, count, names_index = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'section stride changed')
    rows = [struct.unpack_from('>10I', data, base + i * stride) for i in range(count)]
    names_row = rows[names_index]
    names = data[names_row[4]:names_row[4] + names_row[5]]
    sections, symbols, tables, relocs = {}, {}, {}, []
    for index, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4] + row[5]]
        sections[name] = (index, row, raw)
        if row[1] == 2:
            string_row = rows[row[6]]
            strings = data[string_row[4]:string_row[4] + string_row[5]]
            table = []
            for offset in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, offset)
                name = strings[no:].split(b'\0')[0].decode()
                table.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tables[index] = table
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unexpected RELA')
            for offset in range(row[4], row[4] + row[5], 8):
                at, info = struct.unpack_from('>II', data, offset)
                relocs.append((at, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs



def gnu_verify(obj, work, image):
    sections, symbols, relocs = elf(obj)
    ti, _, raw = sections['.text']
    lines = ['.set noreorder', '.set noat']
    externs, counts = set(), {}
    for name, size in MEMBERS.items():
        start, actual, kind, section = symbols[name]
        require((actual, kind, section) == (size, 2, ti), 'incomplete ELF body: ' + name)
        following = min((v[0] for v in symbols.values() if v[2] == 2 and v[3] == ti and v[0] > start), default=len(raw))
        require(following >= start + size and raw[start + size:following] == b'\0' * (following - start - size), 'extra or overlapping function: ' + name)
        words = list(struct.unpack('>%dI' % (size // 4), raw[start:start + size]))
        entries = []
        for loc, typ, symbol, dest in relocs:
            if dest != ti or not start <= loc < start + size:
                continue
            require(typ in (4, 5, 6) and loc % 4 == 0, 'unsupported relocation')
            off = loc - start
            if symbol == '.text':
                require(typ == 4, 'unexpected text address relocation')
                addend = (words[off // 4] & 0x3FFFFFF) * 4
                owners = [(n, v) for n, v in symbols.items()
                          if v[2] == 2 and v[3] == ti and v[0] <= addend < v[0] + v[1]]
                require(len(owners) == 1, 'ambiguous call target')
                symbol, owner = owners[0]
                words[off // 4] = (words[off // 4] & 0xFC000000) | ((addend - owner[0]) // 4)
            if symbol == '.rodata':
                symbol = 'packet_owned_rodata'
            require(not symbol.startswith('.'), 'unowned data relocation')
            entries.append((off, typ, symbol))
            externs.add(symbol)
        lines += ['.section .review_' + name + ',"ax",@progbits', '.balign 4', '.globl ' + name,
                  '.type ' + name + ',@function', name + ':']
        lines += ['.word 0x%08x' % word for word in words]
        lines += ['.size %s,.-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, off, {4: 'R_MIPS_26', 5: 'R_MIPS_HI16', 6: 'R_MIPS_LO16'}[typ], symbol)
                  for off, typ, symbol in entries]
        counts[name] = len(entries)
    owned = sections['.rodata'][2]
    require(len(owned) == 48 and owned[44:] == b'\0' * 4, 'owned-data extent/padding changed')
    owned = owned[12:44]
    require(owned == image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 32], 'all owned floats differ')
    lines += ['.section .review_owned,"a",@progbits', '.globl packet_owned_rodata', 'packet_owned_rodata = . - 12']
    lines += ['.word 0x%08x' % word for word in struct.unpack('>8I', owned)]
    asm, reloc, script, linked = [work / name for name in ('review.s', 'review.o', 'review.ld', 'review.elf')]
    asm.write_text('\n'.join(lines) + '\n')
    addresses = score.image_symbols()
    assignments = []
    for name in sorted(externs - set(MEMBERS) - {'packet_owned_rodata'}):
        address = addresses.get(name, score.address_named(name))
        require(address is not None, 'unresolved GNU symbol: ' + name)
        assignments.append('%s = 0x%X;' % (name, address))
    script.write_text('SECTIONS {\n' + ''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n, n[-8:], n) for n in MEMBERS)
                      + '.review_owned 0x803B9594 : SUBALIGN(4) { *(.review_owned) }\n'
                      + '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n' + '\n'.join(assignments))
    for command in (['mips-linux-gnu-as', '-EB', '-mips2', '-o', str(reloc), str(asm)],
                    ['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(reloc)]):
        require(shutil.which(command[0]), 'GNU MIPS binutils missing: ' + command[0])
        subprocess.run(command, check=True, capture_output=True)
    ss, sy, rr = elf(linked)
    require(not rr, 'GNU relocation left unresolved')
    for name, size in MEMBERS.items():
        index, row, body = ss['.review_' + name]
        address = int(name[-8:], 16)
        require(row[3] == address and len(body) == size and sy[name] == (address, size, 2, index), 'GNU extent changed')
        require(body == packed(score.targets()[name]), 'GNU full words differ: ' + name)
    owned_index, owned_row, owned_body = ss['.review_owned']
    require(owned_row[3] == RODATA and owned_row[5] == 32 and len(owned_body) == 32, 'GNU owned section placement/extent')
    require(sy['packet_owned_rodata'] == (RODATA - 12, 0, 0, owned_index), 'GNU owned anchor symbol')
    require(owned_body == owned, 'GNU owned section differs')
    return {'relocations': counts, 'owned_bytes': 32, 'owned_start': hex(RODATA),
            'owned_sha256': sha(owned), 'full_extent_words_exact': True}



def inputs():
    with target_context() as image:
        manifest = score.target_manifest()
        meta = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        require((meta['image'], meta['base'], meta['rom_offset']) ==
                ('A', '0x8038A400', '0xB5C534'), 'population identity')
        rows = {r['name']: r for r in meta['functions']}
        target_words = score.targets()
        spec = json.loads((GROUP / 'group.json').read_text())
        require(spec['members'] == spec['claims'] == list(MEMBERS), 'all exact members/claims required')
        require(spec['context'] == CONTEXT and spec['keep'] == KEEP, 'genuine context or keep set')
        require(spec['files'] == ['group.c'] and spec['flags'] == '-g0 -O3 -mips2 -G 0 -non_shared', 'recipe')
        files = [GROUP / 'group.c', GROUP / 'group.json', HERE / 'verify.py',
                 ROOT / 'tests/cloud/test_runtime_a_bb80.py']
        result = dict(sources={str(f.relative_to(ROOT)):sha(f.read_bytes()) for f in files},
                      targets={}, symbols={}, call_sites={}, members=list(MEMBERS),
                      claims=spec['claims'], context=CONTEXT, keep=KEEP, flags=spec['flags'],
                      asset_sha256=ASSET_HASH, image_sha256=IMAGE_HASH)
        for name in list(MEMBERS) + CONTEXT:
            raw = packed(target_words[name]); row = rows[name]; address = int(row['address'],16)
            require(row['size'] == len(raw) and address == int(name[-8:],16), 'native extent')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)] == raw, 'image/target words')
            require(name not in MEMBERS or len(raw) == MEMBERS[name], 'member size')
            result['targets'][name] = dict(address=row['address'], size=len(raw), words_sha256=sha(raw))
        addresses = score.image_symbols()
        # Pin only symbol rows named by this source. Do not bind mutable whole manifests.
        import re
        tokens = set(re.findall(r'\b[A-Za-z_]\w*\b', (GROUP/'group.c').read_text()))
        result['symbols'] = {name:hex(addresses[name]) for name in sorted(tokens & addresses.keys())}
        # All native direct callers of the private ABI members stay explicit.
        for callee in (NEW, 'func_8039B908', 'func_8039B988'):
            address = int(callee[-8:],16)
            result['call_sites'][callee] = [dict(caller=name, offset=hex(i*4))
                for name, words in sorted(target_words.items()) for i,w in enumerate(words)
                if w >> 26 == 3 and (w & 0x3FFFFFF)*4 == address & 0x0FFFFFFF]
        return result


def without_homes(source):
    return source.replace('    s32 frame_home0, frame_home1, frame_home2, frame_home3;\n','').replace(
        '    s32 frame_home4, frame_home5, frame_home6, frame_home7;\n','')


def reversed_formals(source):
    import re
    for suffix in ('908','988'):
        source = source.replace('void func_8039B%s(s32 player,u8 *destination)' % suffix,
                                'void func_8039B%s(u8 *destination,s32 player)' % suffix)
    return re.sub(r'func_8039B(908|988)\((\w+),(\w+)\)',r'func_8039B\1(\3,\2)',source)


def replay(deep=False):
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    with target_context() as image, tempfile.TemporaryDirectory(prefix='bb80-packet-') as temp:
        work = Path(temp); obj = work/'group.o'
        score.compile_group(GROUP,obj)
        sections, symbols, relocs = elf(obj)
        ti,_,text = sections['.text']
        results = {}
        for name,size in MEMBERS.items():
            value = score.compare(obj,name,show=0)
            require(value.accepted(),name + ': ' + value.summary())
            start,extent,kind,index = symbols[name]
            require((extent,kind,index)==(size,2,ti),'whole STT_FUNC extent')
            following=min((s[0] for s in symbols.values() if s[2]==2 and s[3]==ti and s[0]>start),default=len(text))
            require(following>=start+size and text[start+size:following]==b'\0'*(following-start-size),'extra body words')
            results[name] = dict(comparison=asdict(value), notes=list(value.notes))
        contexts = {name:asdict(score.compare(obj,name,show=0)) for name in CONTEXT}
        require(all(v['differing']>0 for v in contexts.values()),'context classification changed')
        source=(GROUP/'group.c').read_text()
        controls={}
        for label,changed,expected in (('without_homes',without_homes(source),5),
                                      ('reversed_formals',reversed_formals(source),4),
                                      ('baseline',without_homes(reversed_formals(source)),9)):
            folder=work/label;folder.mkdir()
            (folder/'group.c').write_text(changed)
            (folder/'group.json').write_bytes((GROUP/'group.json').read_bytes())
            score.compile_group(folder,folder/'group.o')
            value=score.compare(folder/'group.o',NEW,show=0)
            require(value.differing==expected and not value.accepted(),'causal control: '+label)
            require(not value.extra_words and not value.errors and not value.unresolved and not value.unverified,'control extent/relocations')
            controls[label]=asdict(value)
        key=owndata.artifact_dir(score.ASM_DIR)
        bound=score._own_data[key]
        try:
            score._own_data[key]=None
            absent=score.compare(obj,'func_8039B560',show=0)
            require(not absent.accepted() and bool(absent.unverified),'absent own-data accepted')
            bad=bytearray(image);bad[RODATA-0x8038A400]^=1
            score._own_data[key]=owndata.ImageData.from_image(bytes(bad),0x8038A400)
            corrupt=score.compare(obj,'func_8039B560',show=0)
            require(not corrupt.accepted() and bool(corrupt.errors),'corrupt owned-data accepted')
        finally:
            score._own_data[key]=bound
        result=dict(inputs=binding,compiler='stock IDO 5.3',members=results,contexts=contexts,
                    controls=controls,owned_data_controls=dict(absent_rejected=True,corrupt_rejected=True),
                    new_exact_code_bytes=712,preserved_exact_code_bytes=sum(MEMBERS.values())-712,
                    new_landed_image_bytes=0,new_rom_verified_bytes=0,
                    installation='same-name group extension; independent checker owns installation')
        if deep:result['gnu']=gnu_verify(obj,work,image)
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text())
    require(inputs()==expected['inputs'],'input binding changed')
    actual=replay(deep)
    require(all(expected[k]==v for k,v in actual.items()),'verification receipt differs')
    return actual


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--record',action='store_true')
    parser.add_argument('--deep',action='store_true')
    parser.add_argument('--inputs-only',action='store_true')
    args=parser.parse_args()
    if args.record:
        result=replay(args.deep)
        (HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    elif args.inputs_only:
        require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'input binding changed')
        print('PASS: source/input identity and complete native targets')
    else:
        result=verify(args.deep)
        for name,row in result['members'].items():
            print(name+': MATCH')
            for note in row['notes']:print('  '+note)
        print('PASS: 712 new exact bytes; eleven previous members preserved')
