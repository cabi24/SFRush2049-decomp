#!/usr/bin/env python3
"""Reproduce the bounded NONMATCH, preserved helper and independent GNU proof."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
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
    paths += ['verify.py', 'trace_probes.py', 'frame_trace.json']
    with targets():
        manifest = score.target_manifest()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        wanted = {row['name']: row for row in extents['functions']}
        bindings = {}
        for name, size in SIZES.items():
            words = score.targets()[name]
            require(wanted[name]['size'] == size == len(words) * 4, 'extent mismatch')
            bindings[name] = {'address': wanted[name]['address'], 'size': size,
                              'words_sha256': sha(packed(words))}
    return {'sources': {p: sha((HERE / p).read_bytes()) for p in paths},
            'targets': bindings, 'asset_sha256': ASSET_HASH,
            'image_sha256': extents['image_sha256'],
            'metadata': {n: manifest[n] for n in ('extents.json', 'symbols.json')}}


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
        require(bad == ([0, 44, 952] if name == NAME else []), 'GNU residual changed')
        output[name] = {'bytes': size, 'relocations': counts[name], 'difference_offsets': bad,
                        'resolved_body_sha256': sha(raw)}
    output['owned_data'] = {'address': hex(address), 'bytes': 20, 'sha256': sha(literals)}
    return output


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    data, base, meta = authenticate()
    spec = json.loads((GROUP / 'group.json').read_text())
    require(spec['members'] == spec['claims'] == [HELPER], 'existing helper must stay claimed')
    require(spec['context'] == ['func_803993EC', 'func_80399D10', NAME], 'context set changed')
    with tempfile.TemporaryDirectory(prefix='projected-options-') as path, targets(owndata.ImageData.from_image(data, base)):
        temp = Path(path)
        obj = temp / 'group.o'
        score.compile_group(GROUP, obj)
        actual = {name: score.compare(obj, name, show=0) for name in SIZES}
        require(actual[HELPER].accepted(), 'existing helper regressed')
        candidate = actual[NAME]
        require(candidate.differing == 3 and not any((candidate.unverified, candidate.unresolved,
                                                     candidate.errors, candidate.extra_words)),
                'NONMATCH residual changed: ' + candidate.summary())
        gnu = gnu_relocate(obj, temp, data, base)
        # The canonical scorer must still reject absent or corrupted own-data evidence.
        key = owndata.artifact_dir(score.ASM_DIR)
        score._own_data[key] = None
        absent = score.compare(obj, NAME, show=0)
        require(len(absent.unverified) == 10, 'absent-data refusal changed')
        corrupt = bytearray(data)
        corrupt[0x803B9820-base] ^= 1
        score._own_data[key] = owndata.ImageData.from_image(corrupt, base)
        bad = score.compare(obj, NAME, show=0)
        require(bad.errors, 'corrupted literal was accepted')
        return {'inputs': binding, 'comparisons': {n: asdict(r) for n, r in actual.items()},
                'gnu': gnu, 'new_match_bytes': 0, 'preserved_match_bytes': 88,
                'accepted_image_bytes': 0, 'rom_bytes': 0,
                'controls': {'absent_data_unverified': 10, 'corrupt_literal_rejected': True}}


def verify():
    expected = json.loads((HERE / 'verification.json').read_text())
    actual = json.loads(json.dumps(replay()))
    require(actual == expected, 'input-bound receipt differs')
    return actual


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--score', action='store_true')
    args = parser.parse_args()
    if args.score:
        data, base, _ = authenticate()
        previous = sys.argv
        try:
            sys.argv = ['score.py', 'group', str(GROUP), '--targets', str(ROOT/'asm/us/ovl_a')]
            with targets(owndata.ImageData.from_image(data, base)):
                return score.main()
        finally:
            sys.argv = previous
    if args.record:
        result = replay()
        (HERE/'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    else:
        verify()
    print('PASS: A:803A6B50 remains NONMATCH (3/241); preserved helper MATCH; GNU/data proof agrees')
    return 0


if __name__ == '__main__':
    sys.exit(main())
