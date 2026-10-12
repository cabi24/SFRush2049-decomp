#!/usr/bin/env python3
"""Input-bound stock/GNU proof for the complete availability-producer group."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP_NAME = 'dot_runtime_a_availability_refresh_20261007'
GROUP = HERE / 'groups' / GROUP_NAME
NEW = 'func_80394564'
OLD = 'func_80394834'
MEMBERS = [NEW, OLD]
CONTEXT = ['func_80397FFC', 'func_80394B00']
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
spec = importlib.util.spec_from_file_location('selector_elf', HERE / 'elf.py')
elf = importlib.util.module_from_spec(spec)
spec.loader.exec_module(elf)
require = elf.require


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def targets():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    try:
        yield
    finally:
        score.ASM_DIR = previous


def inputs():
    with targets():
        manifest = score.target_manifest()
        words = score.targets()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        rows = {row['name']: row for row in extents['functions']}
        group = json.loads((GROUP / 'group.json').read_text())
        require(group['members'] == group['claims'] == MEMBERS, 'all strict members claimed')
        require(group['context'] == CONTEXT, 'genuine contexts')
        require(group['keep'] == ['func_80397FFC'], 'genuine root')
        require(group['flags'] == '-g0 -O3 -mips2 -G 0 -non_shared', 'stock group recipe')
        paths = [GROUP / 'group.c', GROUP / 'group.json', HERE / 'verify.py', HERE / 'elf.py', HERE / 'behavior.c', HERE / 'trace.py']
        result = dict(sources_sha256={str(p.relative_to(HERE)): digest(p.read_bytes()) for p in paths},
                      targets={}, claims=MEMBERS, context=CONTEXT, flags=group['flags'])
        for name in MEMBERS + CONTEXT:
            require(rows[name]['size'] == len(words[name]) * 4, 'target extent')
            result['targets'][name] = dict(image='A', address=rows[name]['address'], size=rows[name]['size'],
                                          words_sha256=digest(packed(words[name])))
        return result


def shell(*args):
    result = subprocess.run(list(map(str, args)), capture_output=True, text=True)
    require(result.returncode == 0, (args, result.stdout, result.stderr))
    return result.stdout


def gnu_link(obj, temp, binding):
    sections, symbols, relocations = elf.read(obj)
    text_index, _, raw = sections['.text']
    lines, external, counts = ['.set noreorder', '.set noat'], set(), {}
    for name in MEMBERS:
        size = binding['targets'][name]['size']
        start, extent, kind, index = symbols[name]
        require((extent, kind, index) == (size, 2, text_index), 'complete ELF extent: ' + name)
        require(not any(v[2] == 2 and v[3] == index and start < v[0] < start + extent
                        for v in symbols.values()), 'overlapping compiled functions')
        words = list(struct.unpack('>%dI' % (size // 4), raw[start:start + size]))
        records = []
        for offset, typ, symbol, dest in relocations:
            if dest != text_index or not start <= offset < start + size:
                continue
            require(typ in (4, 5, 6) and offset % 4 == 0, 'unsupported GNU relocation')
            local = offset - start
            if symbol == '.text':
                require(typ == 4, 'non-call text relocation')
                addend = (words[local // 4] & 0x3FFFFFF) * 4
                owners = [(n, v) for n, v in symbols.items()
                          if v[2] == 2 and v[3] == text_index and v[0] <= addend < v[0] + v[1]]
                require(len(owners) == 1, 'ambiguous local call owner')
                symbol, owner = owners[0]
                require(addend == owner[0], 'non-entry internal call')
                words[local // 4] &= 0xFC000000
            require(not symbol.startswith('.'), 'unowned local-data reference in claimed body')
            records.append((local, typ, symbol))
            external.add(symbol)
        lines += ['.section .review_' + name + ',"ax",@progbits', '.balign 4',
                  '.globl ' + name, '.type ' + name + ',@function', name + ':']
        lines += ['.word 0x%08x' % word for word in words]
        lines += ['.size %s,.-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, off, {4: 'R_MIPS_26', 5: 'R_MIPS_HI16', 6: 'R_MIPS_LO16'}[typ], symbol)
                  for off, typ, symbol in records]
        counts[name] = len(records)
    source, obj2, linked, script = [temp / n for n in ('gnu.s', 'gnu.o', 'gnu.elf', 'gnu.ld')]
    source.write_text('\n'.join(lines) + '\n')
    addresses = score.image_symbols()
    assignments = []
    for name in sorted(external - set(MEMBERS)):
        address = addresses.get(name, score.address_named(name))
        require(address is not None, 'unknown external: ' + name)
        assignments.append('%s = 0x%X;' % (name, address))
    script.write_text('SECTIONS {\n' + ''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n, n[-8:], n) for n in MEMBERS)
                      + '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n' + '\n'.join(assignments))
    shell('mips-linux-gnu-as', '-EB', '-mips2', '-o', obj2, source)
    shell('mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj2)
    ss, sy, rr = elf.read(linked)
    require(not rr, 'unresolved GNU records')
    result = {}
    for name in MEMBERS:
        index, row, data = ss['.review_' + name]
        size, address = binding['targets'][name]['size'], int(name[-8:], 16)
        require(row[3] == address and len(data) == size, 'GNU section extent')
        require(sy[name] == (address, size, 2, index), 'GNU symbol extent')
        result[name] = list(struct.unpack('>%dI' % (size // 4), data))
    return result, counts


def baseline(source):
    head, tail = source.split('typedef struct Owner Owner;', 1)
    start = head.index('    for (player = first;')
    head = head[:start] + re.sub(r'\bplayer\b', 'i', head[start:])
    require(head.count('    s32 j;') == 1, 'single index declaration')
    return head.replace('    s32 j;', '    s32 i,j;') + 'typedef struct Owner Owner;' + tail


def extent_evidence(obj, binding):
    sections, symbols, relocations = elf.read(obj)
    index, row, raw = sections['.text']
    found = sorted((value[0], name, value[1]) for name, value in symbols.items()
                   if value[2] == 2 and value[3] == index)
    require(set(name for _, name, _ in found) == set(MEMBERS + CONTEXT), 'unexpected function set')
    result = {}
    for pos, (start, name, size) in enumerate(found):
        end = found[pos + 1][0] if pos + 1 < len(found) else len(raw)
        require(start + size <= end and not any(raw[start + size:end]), 'nonzero inter-function/tail data')
        if name in MEMBERS:
            require(size == binding['targets'][name]['size'], 'member full STT_FUNC extent')
            require(all(not sym.startswith('.') for off, typ, sym, dest in relocations
                        if dest == index and start <= off < start + size), 'member owns unexpected local data')
        result[name] = dict(offset=start, size=size, next_boundary=end, zero_padding=end-start-size)
    return dict(text_bytes=len(raw), functions=result, member_owned_data_bytes=0)


def replay(deep=False):
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    with targets(), tempfile.TemporaryDirectory(prefix='selector-proof-') as tmp:
        temp = Path(tmp)
        obj = temp / 'candidate.o'
        score.compile_group(GROUP, obj)
        results = {n: score.compare(obj, n, show=0) for n in MEMBERS}
        require(all(r.accepted() for r in results.values()), 'member strict score failed')
        extent = extent_evidence(obj, binding)
        raw = obj.read_bytes()
        sections, symbols, _ = elf.read(obj)
        text_file_offset = sections['.text'][1][4]
        corrupt = bytearray(raw)
        corrupt[text_file_offset + symbols[NEW][0] + 3] ^= 1
        corrupt_obj = temp / 'corrupt-code.o'
        corrupt_obj.write_bytes(corrupt)
        require(not score.compare(corrupt_obj, NEW, show=0).accepted(), 'corrupt code must reject')
        # Independently check an overlong declared function, even if its words match.
        corrupt = bytearray(raw)
        _, symbol_row, table = sections['.symtab']
        changed = 0
        for at in range(0, len(table), 16):
            no, value, size, info, other, index = struct.unpack_from('>IIIBBH', table, at)
            if (value, size, info & 15, index) == symbols[NEW]:
                struct.pack_into('>I', corrupt, symbol_row[4] + at + 8, size + 4)
                changed += 1
        require(changed == 1, 'unique function extent control')
        corrupt_obj.write_bytes(corrupt)
        try:
            extent_evidence(corrupt_obj, binding)
        except ValueError:
            pass
        else:
            raise ValueError('overlong symbol must reject')
        native = score.targets()
        linked, counts = gnu_link(obj, temp, binding)
        require(all(linked[n] == native[n] for n in MEMBERS), 'GNU full-word mismatch')
        control = temp / 'separate_index'
        control.mkdir()
        (control / 'group.json').write_bytes((GROUP / 'group.json').read_bytes())
        (control / 'group.c').write_text(baseline((GROUP / 'group.c').read_text()))
        control_obj = control / 'candidate.o'
        score.compile_group(control, control_obj)
        old = score.compare(control_obj, NEW, show=0)
        require(old.differing == 3 and not any((old.errors, old.unresolved, old.unverified, old.extra_words)), 'baseline control')
        require(score.compare(control_obj, OLD, show=0).accepted(), 'preserved member control')
        # The private entry reads a1 in the exact three native instructions.
        require([(w >> 21) & 31 for w in native[NEW][1:4]] == [5, 5, 5], 'native selector is a1')
        # Both actual native callers invoke the producer and set a1 at the call.
        call = 0x0C000000 | ((int(NEW[-8:], 16) >> 2) & 0x03FFFFFF)
        sites = {}
        for name in CONTEXT:
            indexes = [i for i, word in enumerate(native[name]) if word == call]
            require(len(indexes) == 1, 'one real producer call in ' + name)
            i = indexes[0]
            delay = native[name][i + 1]
            if name == 'func_80394B00':
                require(delay >> 26 == 0 and delay & 63 == 37 and
                        (delay >> 21 & 31, delay >> 16 & 31, delay >> 11 & 31) == (16, 0, 5),
                        'state-change caller passes player s0 in a1')
                argument = 'a1 = s0 (player)'
            else:
                require(delay >> 26 == 9 and (delay >> 21 & 31, delay >> 16 & 31, delay & 65535) == (0, 5, 65535),
                        'setup caller passes -1 in a1')
                argument = 'a1 = -1 (all active players)'
            sites[name] = dict(call_offset=i*4, argument=argument,
                               window_sha256=digest(packed(native[name][max(0,i-4):i+2])))
        context = {n: asdict(score.compare(obj, n, show=0)) for n in CONTEXT}
        result = dict(inputs=binding, results={n: asdict(r) for n,r in results.items()},
                      contexts=context, separate_index_control=asdict(old), extents=extent,
                      gnu_relocations=counts, gnu_sha256={n: digest(packed(w)) for n,w in linked.items()},
                      native_private_selector_register='a1', native_call_sites=sites,
                      negative_controls=['instruction mutation rejected', 'overlong STT_FUNC rejected'],
                      gross_claim_count=2, gross_claim_bytes=1120, new_claim_bytes=720,
                      preserved_bytes=400, accepted_image_bytes=0, compiler='stock IDO 5.3')
        if deep:
            head = (GROUP / 'group.c').read_text().split('typedef struct Owner Owner;', 1)[0]
            source = temp / 'behavior.c'
            source.write_text(head + '\n' + (HERE / 'behavior.c').read_text())
            executable = temp / 'behavior'
            shell('cc', '-std=c89', '-O2', '-fstrict-aliasing', '-Wall', '-Wextra', '-Werror',
                  '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', source, '-o', executable)
            print(shell(executable).strip())
        return result


def verify(deep=False):
    receipt = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == receipt['inputs'], 'receipt input binding changed')
    result = replay(deep)
    require(result == receipt, 'receipt replay changed')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--deep', action='store_true')
    parser.add_argument('--score', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(args.deep), indent=2) + '\n')
    elif args.score:
        with targets():
            sys.argv = ['score.py', 'group', str(GROUP), '--targets', str(ROOT / 'asm/us/ovl_a')]
            raise SystemExit(score.main())
    else:
        verify(args.deep)
    print('PASS: 720 new exact bytes; 400 preserved bytes; full member extents and GNU linkage')
