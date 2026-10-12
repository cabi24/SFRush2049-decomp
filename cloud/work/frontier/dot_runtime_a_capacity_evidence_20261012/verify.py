#!/usr/bin/env python3
"""Input-bound stock/GNU proof with a separately authenticated writer bound."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import shutil
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP = HERE / 'groups/dot_runtime_a_seven_option_group_20261007'
NAME = 'func_803A6B50'
HELPER = 'func_80399394'
SIZES = {NAME: 964, HELPER: 88}
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers
from tools.conveyor.pipeline import blob_group, ovl_splice
spec_capacity = importlib.util.spec_from_file_location("capacity_a6b50", HERE / "capacity.py")
capacity = importlib.util.module_from_spec(spec_capacity)
spec_capacity.loader.exec_module(capacity)


def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def targets(image=None):
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(score.ASM_DIR)
    sentinel = object()
    saved = score._own_data.get(key, sentinel)
    score._own_data[key] = image
    try:
        yield
    finally:
        score.ASM_DIR = previous
        if saved is sentinel:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = saved


def authenticate():
    with targets():
        manifest = score.target_manifest()
        meta = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and sha(asset) == ASSET_HASH, 'asset identity mismatch')
    offset = int(meta['rom_offset'], 16)
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == offset == 0xB5C534,
            'loader pointer mismatch')
    decoder = zlib.decompressobj(-15)
    data = decoder.decompress(asset[offset - 0x283D0:]) + decoder.flush()
    require(decoder.eof and len(data) == meta['size'] and sha(data) == meta['image_sha256'],
            'image identity mismatch')
    return data, int(meta['base'], 16), meta


def inputs():
    paths = ['groups/dot_runtime_a_seven_option_group_20261007/' + n
             for n in ('renderer.c', 'eligibility.c', 'group.json')]
    paths += ['verify.py', 'capacity.py', 'trace.py', 'trace.json', 'host.c']
    bindings = {}
    with targets():
        for name, size in SIZES.items():
            words = score.targets()[name]
            require(size == len(words) * 4, 'extent mismatch')
            bindings[name] = {'size': size, 'words_sha256': sha(packed(words))}
    previous = score.ASM_DIR
    try:
        score.ASM_DIR = ROOT / 'asm/us/blob'
        for name in ('func_800DB1E0', 'func_800A4E58', 'func_800A5158', 'func_800A510C',
                     'audio_frame_sync', 'func_80097164', 'fcvt_wrapper'):
            words = score.targets()[name]
            bindings['game:' + name] = {'size': len(words)*4, 'words_sha256': sha(packed(words))}
    finally:
        score.ASM_DIR = previous
    return {'sources': {p: sha((HERE / p).read_bytes()) for p in paths}, 'targets': bindings}


def shell(*args):
    result = subprocess.run(list(map(str, args)), text=True, capture_output=True)
    require(result.returncode == 0, result.stdout + result.stderr)


def gnu_relocate(obj, temp, image, base):
    """Re-section both complete bodies and their original REL records, then GNU-link.

    All published material is source. Temporary binary/assembly objects stay local.
    Own literal placement is authenticated independently of the code comparison.
    """
    elf = owndata._Object(obj)
    ti = next(i for i, s in enumerate(elf.sections) if s['name'] == '.text')
    ri = next(i for i, s in enumerate(elf.sections) if s['name'] == '.rodata')
    symbols = [s for i, sec in enumerate(elf.sections) if sec['type'] == 2 for s in elf.symbols(i)]
    functions = {s['name']: s for s in symbols if s['type'] == 2 and s['section'] == ti}
    text = elf.raw(ti)
    literals = elf.raw(ri)[:20]
    address = 0x803B9820
    require(literals == image[address-base:address-base+20], 'five complete angle literals differ')
    lines = ['.set noreorder', '.set noat']
    externs = set()
    counts = {}
    kinds = {4: 'R_MIPS_26', 5: 'R_MIPS_HI16', 6: 'R_MIPS_LO16'}
    for name, size in SIZES.items():
        fn = functions[name]
        start = fn['value']
        require(fn['size'] == size, 'complete ELF extent differs: ' + name)
        following = min((s['value'] for s in functions.values() if s['value'] > start), default=len(text))
        require(following >= start + size and not any(text[start+size:following]),
                'nonzero bytes beyond complete function')
        words = list(struct.unpack('>%dI' % (size // 4), text[start:start+size]))
        rels = []
        for offset, kind, sym in elf.relocations(ti):
            if not start <= offset < start + size:
                continue
            require(kind in kinds and offset % 4 == 0, 'unsupported relocation')
            offset -= start
            symbol = sym['name']
            if sym['type'] == 3 and sym['section'] == ti:
                require(kind == 4, 'non-call local text relocation')
                addend = (words[offset//4] & 0x3ffffff) * 4
                owners = [f for f in functions.values() if f['value'] <= addend < f['value'] + f['size']]
                require(len(owners) == 1, 'unowned local call')
                owner = owners[0]
                symbol = owner['name']
                words[offset//4] = (words[offset//4] & 0xfc000000) | ((addend-owner['value'])//4)
            elif sym['section'] == ri:
                require(sym['type'] == 3 and kind in (5, 6), 'unexpected literal relocation')
                symbol = 'packet_angles'
                if kind == 6:
                    require(0 <= (words[offset//4] & 0xffff) < 20, 'literal leaves owned range')
            require(not symbol.startswith('.'), 'unhandled section-relative relocation')
            rels.append((offset, kind, symbol))
            externs.add(symbol)
        section = '.proof_' + name
        lines += ['.section ' + section + ',"ax",@progbits', '.balign 4',
                  '.globl ' + name, '.type ' + name + ',@function', name + ':']
        lines += ['.word 0x%08x' % w for w in words]
        lines += ['.size %s,.-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, o, kinds[k], s) for o, k, s in rels]
        counts[name] = len(rels)
    lines += ['.section .proof_angles,"a",@progbits', '.globl packet_angles', 'packet_angles:']
    lines += ['.byte ' + ','.join(str(n) for n in literals)]
    source, reloc, linked, script = [temp / ('gnu.' + e) for e in ('s', 'o', 'elf', 'ld')]
    source.write_text('\n'.join(lines) + '\n')
    sections = ''.join('.proof_%s 0x%s : SUBALIGN(4) { *(.proof_%s) }\n' %
                       (n, n[-8:], n) for n in SIZES)
    binding = score.image_symbols()
    assignments = []
    for n in sorted(externs - set(SIZES) - {'packet_angles'}):
        value = binding.get(n, score.address_named(n))
        require(value is not None, 'unknown GNU external: ' + n)
        assignments.append('%s = 0x%X;' % (n, value))
    script.write_text('SECTIONS {\n' + sections +
                      '.proof_angles 0x803B9820 : SUBALIGN(4) { *(.proof_angles) }\n' +
                      '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n' +
                      '\n'.join(assignments) + '\n')
    shell('mips-linux-gnu-as', '-EB', '-mips2', '-o', reloc, source)
    shell('mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, reloc)
    result = owndata._Object(linked)
    require(not any(s['type'] in (4, 9) for s in result.sections), 'GNU left relocations')
    linked_symbols = [s for i, section in enumerate(result.sections) if section['type'] == 2
                      for s in result.symbols(i)]
    shoff = struct.unpack_from('>I', result.data, 0x20)[0]
    stride = struct.unpack_from('>H', result.data, 0x2E)[0]

    def section_address(index):
        return struct.unpack_from('>I', result.data, shoff + stride*index + 12)[0]

    data_index = next(i for i, s in enumerate(result.sections) if s['name'] == '.proof_angles')
    require(section_address(data_index) == address and result.raw(data_index) == literals,
            'GNU owned literal placement/content changed')
    output = {}
    for name, size in SIZES.items():
        idx = next(i for i, s in enumerate(result.sections) if s['name'] == '.proof_' + name)
        raw = result.raw(idx)
        require(len(raw) == size and section_address(idx) == int(name[-8:], 16),
                'GNU section address/extent changed')
        symbol = next(s for s in linked_symbols if s['name'] == name)
        require((symbol['value'], symbol['size'], symbol['type'], symbol['section']) ==
                (int(name[-8:], 16), size, 2, idx), 'GNU STT_FUNC address/extent changed')
        words = list(struct.unpack('>%dI' % (size//4), raw))
        bad = [i*4 for i, (a, b) in enumerate(zip(words, score.targets()[name])) if a != b]
        require(bad == [], 'GNU complete-body mismatch')
        output[name] = {'bytes': size, 'relocations': counts[name], 'difference_offsets': bad,
                        'resolved_body_sha256': sha(raw)}
    output['owned_data'] = {'address': hex(address), 'bytes': 20, 'sha256': sha(literals)}
    return output


def stack_census(words):
    accesses, addresses = [], set()
    for index, word in enumerate(words):
        op, base, reg, off = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 65535
        off = off if off < 32768 else off - 65536
        if base == 29 and op in (32,33,35,36,37,40,41,43,49,53,57,61):
            accesses.append([index*4, op, reg, off])
        if base == 29 and op == 9 and reg != 29:
            addresses.add(off)
    require(addresses == {88,92}, 'native address-taken stack set changed')
    require((words[0] & 65535) == 65536-144, 'native frame changed')
    require({a[3] for a in accesses} == {0,4,16,32,36,40,44,48,52,56,60,88,90,144},
            'native stack access domain changed')
    return {'frame': 144, 'address_taken': sorted(addresses), 'accesses': accesses,
            'point': [88,92], 'text': [92,128], 'slot_home': [128,132],
            'title_home': [132,136], 'i_home': [136,140], 'y_home': [140,142],
            'x_home': [142,144], 'callback_home': [144,148],
            'scalar_homes': 'reconstruction from declared layout; not live native accesses'}


def host_check(strings, work):
    # Authentic resource strings are passed only through temporary local data.
    rows = ['{' + ','.join('"' + ''.join('\\%03o' % ch for ch in value) + '"' for value in pair) + '}'
            for pair in strings]
    (work/'language_data.h').write_text('static char *native_strings[6][2] = {' + ','.join(rows) + '};\n')
    cc = shutil.which('cc') or shutil.which('gcc')
    require(cc, 'C compiler missing')
    shell(cc, '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter',
          '-I', work, '-I', HERE, HERE/'host.c', '-o', work/'host')
    shell(work/'host')
    return {'cases': 393216, 'domain': 'all signed16 values x all six authentic language resources',
            'actual_candidate_writer_calls_checked': True, 'max_output_bytes_including_nul': 17,
            'helpers': 'hooks; eligibility false; projection mode disabled',
            'native_behavior_equivalence': False}


def replay(deep=False):
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    data, base, meta = authenticate()
    bound, strings = capacity.audit(data, base)
    spec = json.loads((GROUP/'group.json').read_text())
    require(spec['members'] == spec['claims'] == [HELPER,NAME], 'every matched member must stay claimed')
    require(spec['context'] == ['func_803993EC','func_80399D10'], 'genuine context set changed')
    require(spec['keep'] == ['func_80399D10', NAME], 'genuine kept roots changed')
    # Do not pin production inputs, but ensure this packet has not changed the
    # installed helper and both complete real context bodies.
    # w25p: before installation production has group.c (the helper); after, this packet's eligibility.c.
    installed = ROOT/'src/ovl_a/groups/dot_runtime_a_seven_option_group_20261007'
    installed = installed/('eligibility.c' if (installed/'eligibility.c').exists() else 'group.c')
    require((GROUP/'eligibility.c').read_bytes() == installed.read_bytes(),
            'inherited genuine group source differs from production')
    with tempfile.TemporaryDirectory(prefix='capacity-a6b50-') as path, targets(owndata.ImageData.from_image(data,base)):
        temp=Path(path);obj=temp/'group.o'
        score.compile_group(GROUP,obj)
        actual={name:score.compare(obj,name,show=0) for name in SIZES}
        require(all(r.accepted() for r in actual.values()), 'canonical member is not exact')
        output={'inputs':binding,'capacity':bound,'stack':stack_census(score.targets()[NAME]),
                'comparisons':{n:asdict(r) for n,r in actual.items()},
                'new_match_bytes':964,'preserved_match_bytes':88,'accepted_image_bytes':0,'rom_bytes':0}
        if deep:
            output['gnu']=gnu_relocate(obj,temp,data,base)
            traced=json.loads(subprocess.check_output([sys.executable,str(HERE/'trace.py')],text=True))
            require(traced==json.loads((HERE/'trace.json').read_text()),'stock diagnostic metadata differs')
            output['stock_trace']=traced
            output['host']=host_check(strings,temp)
            native={n:packed(score.targets()[n]) for n in SIZES}
            installed=ovl_splice.packet_spec(GROUP)
            require(set(installed['members'])==set(SIZES),'installer member set changed')
            slices,ti=blob_group.member_slices(obj,list(SIZES),
                {n:{'vaddr':int(n[-8:],16),'size':size} for n,size in SIZES.items()})
            require(blob_group.relocate(obj,slices,ti,score.image_symbols(),members=list(SIZES),
                    image=(data,base)) == native, 'installer full-body relocation mismatch')
            key=owndata.artifact_dir(score.ASM_DIR)
            original=score._own_data[key]
            try:
                score._own_data[key]=None
                absent=score.compare(obj,NAME,show=0)
                require(not absent.accepted() and len(absent.unverified)==10,'missing data accepted')
                for offset in range(0,20,4):
                    corrupt=bytearray(data);corrupt[0x803B9820-base+offset]^=1
                    score._own_data[key]=owndata.ImageData.from_image(corrupt,base)
                    require(not score.compare(obj,NAME,show=0).accepted(),'corrupt owned literal accepted')
                    try:
                        blob_group.relocate(obj,slices,ti,score.image_symbols(),members=list(SIZES),image=(bytes(corrupt),base))
                    except blob_group.GroupError:
                        pass
                    else:
                        raise ValueError('installer accepted corrupt owned literal')
            finally:
                score._own_data[key]=original
            controls={}
            for length,expected in ((50,3),(32,11)):
                folder=temp/('text%d'%length);folder.mkdir()
                for filename in ('eligibility.c','group.json'):
                    (folder/filename).write_bytes((GROUP/filename).read_bytes())
                (folder/'renderer.c').write_text((GROUP/'renderer.c').read_text().replace('char text[36];','char text[%d];'%length))
                score.compile_group(folder,folder/'group.o')
                result=score.compare(folder/'group.o',NAME,show=0)
                require(result.differing==expected and not any((result.unverified,result.unresolved,result.errors,result.extra_words)),
                        'capacity diagnostic changed')
                require(score.compare(folder/'group.o',HELPER,show=0).accepted(),'diagnostic regressed helper')
                controls[str(length)]=asdict(result)
            output['controls']={'capacity':controls,'missing_data_unverified':10,'all_five_corrupt_literals_rejected':True,
                                'actual_installer_full_bodies_exact':True}
            abi=temp/'abi.c'
            abi.write_text('#include "'+str(GROUP/'renderer.c')+'"\n'
                           '#define CHECK(n,e) typedef char n[(e)?1:-1]\n'
                           'CHECK(word,sizeof(s32)==4);CHECK(pointer,sizeof(void*)==4);CHECK(short16,sizeof(s16)==2);\n'
                           'CHECK(color,sizeof(Color4)==4);CHECK(slot,sizeof(Slot)==64);CHECK(position,(u32)&((Slot*)0)->position==48);\n')
            # Only this new overlay source is compiled; no static ROM TU is rebuilt.
            score.compile_single(abi,'-g0 -O2 -mips2 -G 0 -non_shared',temp/'abi.o')
            output['abi']='IDO32 scalar, pointer, signed16, Color4, Slot64 and position offset checked'
        return output


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text())
    actual=json.loads(json.dumps(replay(deep)))
    require(all(expected[k]==v for k,v in actual.items()),'input-bound receipt differs')
    return actual


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record',action='store_true');parser.add_argument('--score',action='store_true')
    parser.add_argument('--deep',action='store_true');args=parser.parse_args()
    if args.score:
        data,base,_=authenticate();previous=sys.argv
        try:
            sys.argv=['score.py','group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')]
            with targets(owndata.ImageData.from_image(data,base)):
                return score.main()
        finally:
            sys.argv=previous
    if args.record:
        (HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    else:
        verify(args.deep)
    print('PASS: A6B50 964 fresh exact bytes; helper88 preserved; writer bound17; zero new ROM coverage')
    return 0


if __name__=='__main__':
    sys.exit(main())
