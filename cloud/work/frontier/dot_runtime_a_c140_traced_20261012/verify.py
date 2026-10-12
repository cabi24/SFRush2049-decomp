#!/usr/bin/env python3
"""Strict source-bound proof; cross-group migration is deliberately blocked."""
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
from unittest.mock import patch

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP_NAME = 'dot_runtime_a_text_helpers_20261006'
GROUP = HERE / 'groups' / GROUP_NAME
NEW = 'func_8039C140'
MEMBERS = ['func_8039B120', 'func_8039B214', 'func_8039BE48', 'func_8039A448',
           'func_8039B00C', NEW, 'func_8039B908', 'func_8039B988', 'func_8039BA28', 'func_8039BAE0']
CONTEXT = ['func_8039A4D4', 'func_8039BB80', 'func_803A5658', 'func_803A5948']
OLD_MEMBERS = ['func_8039B908', 'func_8039B988', 'func_8039BA28', 'func_8039BAE0']
# Observed lock ownership at the reserved master baseline, not lock-file hashes.
# The fixture remains historical if the independent checker later migrates it.
CROSS_GROUPS = {
    'func_8039B120': 'dot_runtime_a_config_caller_group_20261006',
    'func_8039B214': 'dot_runtime_a_config_parent_group_20261006',
    'func_8039BE48': 'dot_runtime_a_scene_init_group_20261006',
    'func_8039A448': 'dot_runtime_a_config_parent_group_20261006',
    'func_8039B00C': 'dot_runtime_a_cleanup_group_20261006',
}
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
from tools.conveyor.pipeline import ovl_splice

spec = importlib.util.spec_from_file_location('c140_elf_reader', HERE / 'elf.py')
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
        require(group['members'] == group['claims'] == MEMBERS, 'all verified members must remain claimed')
        require(group['context'] == CONTEXT, 'genuine context set')
        require(group['keep'] == [NEW, 'func_803A5948'], 'root ABI set')
        paths = [GROUP / 'group.c', GROUP / 'group.json', HERE / 'verify.py', HERE / 'elf.py', HERE / 'trace.py']
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


def controls(source):
    # Ordinary readable block replaces the line-sensitive macro invocation.
    start = source.index('#define C140_ANIMATION_ARM \\\n')
    end = source.index('\n\nvoid ' + NEW, start)
    macro = source[start:end].split('\n', 1)[1]
    arm = '\n'.join(line.rstrip().removesuffix('\\').rstrip() for line in macro.splitlines())
    ordinary = source[:start] + source[end:]
    ordinary = ordinary.replace('    if (D_803B342C) C140_ANIMATION_ARM else if (D_803B3414) {',
                                '    if (D_803B342C) ' + arm + ' else if (D_803B3414) {')
    def no_carriers(text):
        require(text.count('    f32 (*matrix)[3];\n') == 1, 'carrier declarations')
        text = text.replace('    f32 (*matrix)[3];\n', '').replace('    f32 angle;\n', '', 1)
        text = re.sub(r'^\s*matrix = \(f32 \(\*\)\[3\]\)D_803B9A50;[^\n]*\n', '', text, flags=re.M)
        text = re.sub(r'^\s*angle = -90\.0f \* D_8002EB94 \* D_803B95B8;[^\n]*\n', '', text, flags=re.M)
        return text.replace('func_80090E9C(angle, matrix);',
                            'func_80090E9C(-90.0f * D_8002EB94 * D_803B95B8, (f32 (*)[3])D_803B9A50);')
    return {'ordinary_lines': ordinary, 'without_carriers': no_carriers(source),
            'baseline': no_carriers(ordinary)}


def refusal_fixture(temporary):
    fixture = {'a': {name: {'group': group} for name, group in CROSS_GROUPS.items()}, 'b': {}}
    fixture['a'].update({name: {'group': GROUP_NAME} for name in OLD_MEMBERS})
    path = temporary / 'fixture-lock.json'
    path.write_text(json.dumps(fixture))
    before = path.read_bytes()
    def no_compile(*args, **kwargs):
        raise ValueError('unexpected compile after ownership guard')
    # Invoke the actual guard with historical ownership in a temporary fixture.
    # Suppress only unrelated layout generation; all output roots remain temp.
    with patch.object(ovl_splice.ovl_layout, 'derive', lambda image: None), \
         patch.object(ovl_splice, 'group_root', lambda image: temporary / 'groups'), \
         patch.object(ovl_splice.ovl_layout, 'work_dir', lambda image: temporary / 'work'):
        try:
            ovl_splice.splice_group('a', GROUP, GROUP_NAME, path, compile_fn=no_compile)
        except ovl_splice.SpliceError as error:
            message = str(error)
            require(message.startswith('already spliced outside group ' + GROUP_NAME), 'wrong refusal')
            require(all(name in message for name in CROSS_GROUPS), 'missing clashing member')
        else:
            raise ValueError('cross-group packet unexpectedly installed')
    require(path.read_bytes() == before and not (temporary / 'groups').exists(), 'fixture mutated on refusal')
    return dict(message=message, cross_group_ownership=CROSS_GROUPS, retained_old_members=OLD_MEMBERS,
                temporary_lock_unchanged=True, production_paths_untouched=True)


def root_abi(words):
    require(words[0] == 0x27BDFF78 and words[-1] == 0x27BD0088, 'root frame is not 136')
    stores, loads = {}, {}
    for word in words:
        op, base, register = word >> 26, word >> 21 & 31, word >> 16 & 31
        if base != 29 or op not in (43, 35, 61, 53):
            continue
        if op in (43, 35) and register not in (*range(16, 24), 30, 31):
            continue
        if op in (61, 53) and register not in range(20, 32, 2):
            continue
        key = ('f' if op in (61, 53) else 'r') + str(register)
        (stores if op in (43, 61) else loads)[key] = word & 65535
    expected = {'r' + str(r): 72 + 4 * i for i, r in enumerate((*range(16, 24), 30, 31))}
    expected.update({'f' + str(r): 24 + 4 * (r - 20) for r in range(20, 32, 2)})
    require(stores == loads == expected, 'root save/restore footprint')
    return dict(frame_bytes=136, preserved_slots=expected, preserved_bytes=88,
                helper_abi='genuine IPA context; not standalone o32 helpers')


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    with targets(), tempfile.TemporaryDirectory(prefix='c140-replay-') as tmp:
        temporary = Path(tmp)
        obj = temporary / 'candidate.o'
        score.compile_group(GROUP, obj)
        results = {name: score.compare(obj, name, show=0) for name in MEMBERS}
        require(all(result.accepted() for result in results.values()), 'one or more member mismatches')
        native = score.targets()
        linked, counts = gnu_link(obj, temporary, binding)
        for name in MEMBERS:
            require(linked[name] == native[name], 'GNU/native mismatch: ' + name)
        outcomes = {}
        expected = {'ordinary_lines': 3, 'without_carriers': 4, 'baseline': 7}
        for label, source in controls((GROUP / 'group.c').read_text()).items():
            folder = temporary / label
            folder.mkdir()
            (folder / 'group.c').write_text(source)
            (folder / 'group.json').write_bytes((GROUP / 'group.json').read_bytes())
            control_obj = folder / 'candidate.o'
            score.compile_group(folder, control_obj)
            value = score.compare(control_obj, NEW, show=0)
            require(value.differing == expected[label] and not any((value.errors, value.unresolved,
                    value.unverified, value.extra_words)), 'source control changed: ' + label)
            for name in MEMBERS:
                if name != NEW:
                    require(score.compare(control_obj, name, show=0).accepted(), 'old body changed in control')
            outcomes[label] = asdict(value)
        refusal = refusal_fixture(temporary)
        spec = ovl_splice.packet_spec(GROUP)
        require(spec['members'] == MEMBERS and set(OLD_MEMBERS) <= set(spec['members']), 'installer member loss')
        return dict(inputs=binding, results={n: asdict(r) for n, r in results.items()}, controls=outcomes,
                    gnu_relocations=counts, gnu_sha256={n: digest(packed(w)) for n, w in linked.items()},
                    root_abi=root_abi(native[NEW]), installer_refusal_fixture=refusal,
                    gross_claim_count=len(MEMBERS), gross_claim_bytes=sum(len(native[n]) * 4 for n in MEMBERS),
                    new_claim_count=1, new_claim_bytes=len(native[NEW]) * 4, accepted_image_bytes=0,
                    status='strict-match research; cross-group migration required', compiler='stock IDO 5.3')


def verify():
    receipt = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == receipt['inputs'], 'receipt input binding changed')
    result = replay()
    require(result == receipt, 'receipt replay changed')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
    else:
        verify()
    print('PASS: 10 strict members / 4864 B gross / 1984 B new; cross-group installation blocked')
