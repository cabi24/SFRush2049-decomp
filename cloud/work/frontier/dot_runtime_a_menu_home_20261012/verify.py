#!/usr/bin/env python3
"""Input-bound complete-object checks; stock IDO and optional GNU/host replay."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
SOURCE = ROOT / 'cloud/matches/ovl_a/func_803A4340.c'
BASELINE = ROOT / 'cloud/work/frontier/dot_runtime_a_menu_loop_20261006/candidate.c'
NAME = 'func_803A4340'
ENTRY, SIZE = 0x803A4340, 428
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def require(ok, message):
    if not ok:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def shell(*args):
    run = subprocess.run([str(x) for x in args], capture_output=True, text=True)
    require(run.returncode == 0, (args, run.stdout, run.stderr))
    return run.stdout


def elf(path):
    """Independent ELF32 big-endian reader, including complete symbol sizes."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02', 'not ELF32 big-endian')
    require(struct.unpack_from('>H', data, 18)[0] == 8, 'not MIPS')
    at = struct.unpack_from('>I', data, 32)[0]
    stride, count, ni = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'section size')
    rows = [struct.unpack_from('>10I', data, at+i*stride) for i in range(count)]
    nr = rows[ni]; names = data[nr[4]:nr[4]+nr[5]]
    sections, symbols, tables, relocs = {}, {}, {}, []
    for index, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4]+row[5]]
        sections[name] = (index, row, raw)
        if row[1] == 2:
            strings_row = rows[row[6]]
            strings = data[strings_row[4]:strings_row[4]+strings_row[5]]
            table = []
            for offset in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, offset)
                name = strings[no:].split(b'\0')[0].decode(); table.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tables[index] = table
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unsupported RELA')
            for offset in range(row[4], row[4]+row[5], 8):
                loc, info = struct.unpack_from('>II', data, offset)
                relocs.append((loc, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs


def inspect(path, linked=False):
    sections, symbols, relocs = elf(path)
    ti, row, raw = sections['.text']
    functions = {name: sym for name, sym in symbols.items() if sym[2] == 2 and sym[3] == ti}
    require(functions == {NAME: (ENTRY if linked else 0, SIZE, 2, ti)}, 'complete function extent')
    require(len(raw) == 432 and raw[SIZE:] == bytes(4), 'nonzero excess text')
    for name, (_index, sec, _data) in sections.items():
        if sec[2] & 2 and name not in ('.text', '.options', '.reginfo'):
            require(sec[5] == 0, 'unexpected owned data: '+name)
    require(not relocs if linked else len(relocs) == 21, 'relocation count')
    require(all(off < SIZE and off % 4 == 0 and kind in (4, 5, 6) and dest == ti
                for off, kind, _symbol, dest in relocs), 'unsupported relocation')
    return raw[:SIZE], relocs


def inputs():
    manifest = score.target_manifest()
    metadata = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    extent = next(e for e in metadata['functions'] if e['name'] == NAME)
    require(extent['size'] == SIZE and int(extent['address'], 16) == ENTRY, 'target extent')
    words = score.targets()[NAME]
    paths = [SOURCE, BASELINE, HERE / 'verify.py']
    return {'sources_sha256': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in paths},
            'target': {'population': 'A', 'address': hex(ENTRY), 'bytes': SIZE,
                       'words_sha256': sha(struct.pack('>%dI' % len(words), *words))},
            'metadata_manifest_rows': {p: manifest[p] for p in ('extents.json', 'symbols.json')},
            'flags': FLAGS, 'compiler': 'stock IDO 5.3; overlay single, no static ROM TU'}


def run(deep=False, write=False):
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    try:
        binding = inputs()
        receipt_path = HERE / 'packet.json'
        if not write:
            require(json.loads(receipt_path.read_text())['inputs'] == binding, 'input receipt mismatch')
        source = SOURCE.read_text()
        baseline = BASELINE.read_text()
        reused = baseline.replace('s32 arg0)', 's32 i)').replace('    s32 i;\n', '')
        # Ignore only the explanatory comment; executable source must be the
        # previous complete reconstruction plus precisely the disclosed lever.
        import re
        strip = lambda text: re.sub(r'/\*.*?\*/', '', text, flags=re.S).strip()
        unshaped = source.replace('    s32 frame_slot0;\n', '').replace('    s32 frame_slot1;\n', '')
        require(strip(unshaped) == strip(reused), 'undisclosed executable source change')
        with tempfile.TemporaryDirectory(prefix='menu-home-') as tmp:
            work = Path(tmp); obj = work / 'match.o'
            score.compile_single(SOURCE, FLAGS, obj)
            result = score.compare(obj, NAME, show=0)
            require(result.accepted() and result.differing == 0, 'not a strict complete match')
            body, relocs = inspect(obj)
            controls = {}
            for label, text, expected in [('baseline', baseline, 4), ('parameter_reuse', reused, 4),
                                          ('one_slot_removed', source.replace('    s32 frame_slot1;\n', ''), 4)]:
                path = work / (label+'.c'); path.write_text(text)
                out = work / (label+'.o'); score.compile_single(path, FLAGS, out)
                comparison = score.compare(out, NAME, show=0)
                require(comparison.differing == expected and comparison.total == 107 and not comparison.extra_words
                        and not comparison.unresolved and not comparison.unverified and not comparison.errors,
                        'control residual changed: '+label)
                raw, _ = inspect(out)
                frame = -(struct.unpack_from('>I', raw)[0] & 0xffff) + 65536
                require(frame == (104 if label == 'parameter_reuse' else 112), 'frame control changed')
                controls[label] = {'differing_words': comparison.differing, 'complete_bytes': SIZE, 'frame': frame}
            details = {'complete_bytes': SIZE, 'alignment_bytes': 4, 'relocations': len(relocs),
                       'owned_bytes': 0, 'comparison': asdict(result), 'controls': controls,
                       'compiler_binary_provenance': {name: sha(Path(score.ido(name)).read_bytes())
                                                      for name in ('cc', 'cfe', 'uopt', 'ugen', 'as1')}}
            if deep:
                addresses = score.image_symbols()
                names = {name for _off, _kind, name, _dest in relocs}
                addresses = {name: addresses.get(name, score.address_named(name)) for name in names}
                require(all(value is not None for value in addresses.values()), 'unresolved GNU anchor')
                script = work / 'whole.ld'
                script.write_text('SECTIONS { .text 0x803A4340 : SUBALIGN(4) { *(.text) } '
                                  '/DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'
                                  + ''.join('%s = 0x%X;\n' % (name, addresses[name]) for name in sorted(names)))
                linked = work / 'match.elf'
                shell('mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj)
                linked_body, _ = inspect(linked, True)
                expected = struct.pack('>107I', *score.targets()[NAME])
                require(linked_body == expected, 'GNU full-extent mismatch')
                details['gnu'] = {'complete_words_exact': 107, 'remaining_relocations': 0}
            receipt = {'inputs': binding, 'verification': details}
            if write:
                receipt_path.write_text(json.dumps(receipt, indent=2, sort_keys=True)+'\n')
            return receipt
    finally:
        score.ASM_DIR = previous


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--deep', action='store_true')
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    print(json.dumps(run(args.deep, args.write), indent=2, sort_keys=True))
