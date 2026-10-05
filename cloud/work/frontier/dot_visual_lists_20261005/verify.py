#!/usr/bin/env python3
"""Reproduce exact source/ELF/GNU/behavior proofs; never publish target words."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location("visual_lists_native", HERE / "native.py")
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
execute, oracle = native.execute, native.oracle

NAME = 'visual_objects_update'
SOURCE = ROOT / 'cloud/matches/visual_objects_update.c'
BASELINE = ROOT / 'cloud/work/tiny_A42/visual_objects_update.c'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT = ['func_80091FBC', 'func_8009211C']

def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def digest(data): return hashlib.sha256(data).hexdigest()
def run(cmd, **kw): return subprocess.run(cmd, check=True, capture_output=True, **kw)

def symbols(obj):
    data, sections = score._elf(obj)
    return data, sections, [s for i, section in enumerate(sections) if section['type'] == 2
                            for s in score._symbol_table(data, sections, i)]

def extent(obj, name):
    _, _, syms = symbols(obj)
    rows = [s for s in syms if s['name'] == name and s['type'] == 2]
    assert len(rows) == 1
    return rows[0]['size']

def comparison(obj, name):
    value = dataclasses.asdict(score.compare(obj, name, show=0))
    value['elf_function_bytes'] = extent(obj, name)
    return value


def portable_elf(data):
    """All semantic ELF bytes; only physical offsets and nonallocated .mdebug payload vary by source path."""
    assert data[:6] == b"\x7fELF\x01\x02"
    header = struct.unpack_from(">HHIIIIIHHHHHH", data, 16)
    assert header[0] == 1 and header[1] == 8 and header[9] == 0
    shoff, shsize, shnum, names_index = header[5], header[10], header[11], header[12]
    assert shsize == 40
    sections = [struct.unpack_from(">10I", data, shoff + shsize * i) for i in range(shnum)]
    names = sections[names_index]
    records = []
    for row in sections:
        index, typ, flags, addr, offset, size, link, info, align, entry_size = row
        start = names[4] + index
        name = data[start:data.index(b"\0", start)].decode()
        record = dict(name=name, type=typ, flags=flags, address=addr, link=link,
                      info=info, alignment=align, entry_size=entry_size)
        if name == '.mdebug':
            assert typ == 0x70000005 and flags == 0 and addr == 0
            record['scope'] = 'nonallocated ECOFF debug metadata; fingerprinted separately'
        else:
            record['size'] = size
            record['sha256'] = digest(data[offset:offset + size]) if typ != 8 else None
        records.append(record)
    return {'ident_sha256': digest(data[:16]),
            'header': list(header[:5]) + list(header[6:]), 'sections': records}


def debug_payload(obj):
    data, sections = score._elf(obj)
    rows = [s for s in sections if s['name'] == '.mdebug']
    assert len(rows) == 1
    sec = rows[0]
    return data[sec['off']:sec['off'] + sec['size']]


def portability_controls(obj, work):
    reference = portable_elf(obj.read_bytes())
    controls = []
    for label in ['path_a', 'different_length_path_b']:
        directory = work / label
        directory.mkdir()
        source = directory / SOURCE.name
        source.write_bytes(SOURCE.read_bytes())
        out = directory / 'candidate.o'
        score.compile_single(source, FLAGS, out)
        assert portable_elf(out.read_bytes()) == reference
        debug = debug_payload(out)
        assert debug.count(str(source).encode() + b'\0') == 1
        controls.append((sha(out), digest(debug)))
    assert controls[0][0] != controls[1][0] and controls[0][1] != controls[1][1]
    data, sections = score._elf(obj)
    mutations = {}
    for name in ['.text', '.rel.text', '.symtab', '.reginfo', '.options']:
        sec = next(s for s in sections if s['name'] == name)
        altered = bytearray(data)
        altered[sec['off']] ^= 1
        assert portable_elf(altered) != reference
        mutations[name] = 'rejected'
    altered = bytearray(data)
    altered[39] ^= 1  # ELF e_flags, never debug metadata
    assert portable_elf(altered) != reference
    mutations['ELF_ABI_flags'] = 'rejected'
    return {'identical_source_at_two_absolute_paths': True,
            'full_object_and_mdebug_hashes_differ': True,
            'all_non_debug_sections_and_ABI_metadata_equal': True,
            'each_mdebug_contains_exactly_one_source_path': True,
            'semantic_mutations': mutations}

def cases():
    actions = [0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0xABCDE123]
    seeds = [0, 1, 0xFFFF, 0x5555, 0xAAAA] + [1 << i for i in range(16)]
    rng = random.Random(0xB55FC)
    seeds += [rng.getrandbits(32) for _ in range(171)]
    return [(seed, mode, action) for seed in seeds for mode in range(7) for action in actions]

def behavior(words, linked, work):
    corpus = cases()
    expectations, coverage = [], set()
    for seed, mode, action in corpus:
        expected, expected_memory = oracle(seed, mode, action)
        for stream in [words, linked]:
            got, mem, seen = execute(stream, 0x800B55FC, seed, mode, action)
            assert got == expected and mem == expected_memory
            coverage |= seen
        expectations.append(expected)
    assert coverage == set(range(0, 140, 4))
    ccflags = ['cc', '-std=c99', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
               '-fsanitize=address,undefined', '-fno-sanitize-recover=all']
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
    text = ''.join('%d %d %d\n' % case for case in corpus)
    exe = work / 'host'
    run(ccflags + [str(HERE / 'host.c'), '-o', str(exe)])
    output = run([str(exe)], input=text, text=True, env=env).stdout
    assert [list(map(int, row.split())) for row in output.splitlines()] == expectations
    source = SOURCE.read_text()
    controls = {
        'omit_fourth_list': source.replace('i < 4', 'i < 3'),
        'zero_action': source.replace('MaxPathZeroControls(node, action)', 'MaxPathZeroControls(node, 0)'),
        'invert_predicate': source.replace('if (node->body->enabled)', 'if (!node->body->enabled)'),
        'prefetch_next_before_operation': source.replace('    VisualNode *node;', '    VisualNode *node;\n    VisualNode *next;')
            .replace('node = node->body->next)', 'node = next)')
            .replace('            if (node->body->enabled)', '            next = node->body->next;\n            if (node->body->enabled)'),
    }
    negative = {}
    for name, altered in controls.items():
        assert altered != source
        mutant = work / (name + '.c')
        mutant.write_text(altered)
        host = work / (name + '_host.c')
        host.write_text((HERE / 'host.c').read_text().replace('#include "../../../matches/visual_objects_update.c"',
                                                          '#include "' + str(mutant) + '"'))
        binary = work / (name + '_host')
        flags = [f for f in ccflags if f != '-Werror']
        run(flags + [str(host), '-o', str(binary)])
        actual = run([str(binary)], input=text, text=True, env=env).stdout
        rows = [list(map(int, row.split())) for row in actual.splitlines()]
        differing = sum(a != b for a, b in zip(rows, expectations))
        assert len(rows) == len(expectations) and differing > 0
        negative[name] = {'rejected': True, 'differing_cases': differing}
    return {'cases': len(corpus), 'native_runs': 2 * len(corpus),
            'all_instruction_offsets_executed': len(coverage), 'host_asan_ubsan': 'passed',
            'native_memory_and_call_snapshots': 'equal_to_oracle',
            'caller_save_clobber_and_callee_save_checks': 'passed', 'mutants': negative}

def build(work):
    obj = work / 'candidate.o'
    score.compile_single(SOURCE, FLAGS, obj)
    result = comparison(obj, NAME)
    assert score.compare(obj, NAME, show=0).accepted() and extent(obj, NAME) == 140
    data, sections, _ = symbols(obj)
    own = {s['name']: s['size'] for s in sections if s['name'] in ('.rodata', '.data', '.bss') and s['size']}
    assert not own
    address = score.image_symbols()
    script = work / 'link.ld'
    script.write_text('SECTIONS { .text 0x800B55FC : SUBALIGN(4) { *(.text) } }\n' +
                      ''.join('%s = 0x%x;\n' % (k, v) for k, v in address.items() if k != NAME))
    linked, binary = work / 'linked.elf', work / 'linked.bin'
    run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(obj)])
    run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(binary)])
    blob = binary.read_bytes()
    words = score.targets()[NAME]
    target = struct.pack('>%dI' % len(words), *words)
    assert blob[:140] == target and not any(blob[140:]) and extent(linked, NAME) == 140
    relocs = [line for line in run(["mips-linux-gnu-readelf", "-r", str(obj)], text=True).stdout.splitlines() if "R_MIPS_" in line and "R_MIPS_32" not in line]
    # A separate layout-only compilation verifies O32 facts in the same unmodified source.
    layout = work / 'layout.c'
    layout.write_text('#define offsetof(t, m) ((unsigned long)&((t *)0)->m)\n#include "' + str(SOURCE) + '"\n' +
        'typedef char a[(sizeof(VisualList)==16)?1:-1];\n' +
        'typedef char b[(offsetof(VisualList,head)==8)?1:-1];\n' +
        'typedef char c[(offsetof(VisualBody,enabled)==72)?1:-1];\n' +
        'typedef char d[(offsetof(VisualNode,body)==0)?1:-1];\n' +
        'typedef char e[(offsetof(VisualBody,next)==0)?1:-1];\n')
    layout_obj = work / 'layout.o'
    score.compile_single(layout, FLAGS, layout_obj)
    assert score.compare(layout_obj, NAME, show=0).accepted()
    controls = {}
    base = BASELINE.read_text()
    typed_pointer = SOURCE.read_text().replace('    s32 i;', '    VisualList *list;')
    typed_pointer = typed_pointer.replace('for (i = 0; i < 4; i++)',
                                        'for (list = D_80144D60; list != D_80144D60 + 4; list++)')
    typed_pointer = typed_pointer.replace('D_80144D60[i].head', 'list->head')
    raw_counted = base.replace('List *list;Node *node;', 's32 i;Node *node;')
    raw_counted = raw_counted.replace('for(list=D_80144D60;list!=D_80144D60+4;list++)', 'for(i=0;i<4;i++)')
    raw_counted = raw_counted.replace('list->head', 'D_80144D60[i].head')
    for name, contents in [('raw_pointer_prior', base), ('typed_pointer', typed_pointer),
                           ('raw_counted', raw_counted), ('typed_counted', SOURCE.read_text())]:
        path, control_obj = work / (name + '.c'), work / (name + '.o')
        path.write_text(contents)
        score.compile_single(path, FLAGS, control_obj)
        controls[name] = comparison(control_obj, NAME)
    assert controls['raw_pointer_prior']['differing'] == controls['typed_pointer']['differing'] == 22
    assert controls['raw_counted']['differing'] == controls['typed_counted']['differing'] == 0
    o2 = work / 'o2.o'
    score.compile_single(SOURCE, FLAGS.replace('-O3', '-O2'), o2)
    assert score.compare(o2, NAME, show=0).accepted()
    group = work / 'context'
    group.mkdir()
    files = ['candidate.c']
    shutil.copyfile(SOURCE, group / files[0])
    locks = json.loads((ROOT / 'blob_matched.lock.json').read_text())
    sources = {}
    for name in CONTEXT:
        path = ROOT / locks[name]['source']
        assert sha(path) == locks[name]['source_sha256']
        shutil.copyfile(path, group / (name + '.c'))
        sources[str(path.relative_to(ROOT))] = sha(path)
        files.append(name + '.c')
    (group / 'group.json').write_text(json.dumps({'files': files, 'flags': FLAGS,
        'members': [NAME], 'context': CONTEXT, 'keep': [NAME] + CONTEXT, 'claims': [NAME]}))
    group_obj = work / 'context.o'
    score.compile_group(group, group_obj)
    context = {name: comparison(group_obj, name) for name in [NAME] + CONTEXT}
    assert all(score.compare(group_obj, name, show=0).accepted() for name in context)
    inputs = {str(p.relative_to(ROOT)): sha(p) for p in [SOURCE, HERE/'verify.py', HERE/'host.c', HERE/'native.py', BASELINE]}
    return {'schema': 1, 'base': 'cc4d5fdd', 'status': 'MATCH', 'accepted_byte_gain': 0,
        'target': {'name': NAME, 'start': '0x800B55FC', 'end_exclusive': '0x800B5688',
                   'bytes': 140, 'words': 35, 'sha256': digest(target)},
        'inputs': inputs, 'flags': FLAGS, 'comparison': result,
        'o2_comparison': comparison(o2, NAME), 'object_sha256': sha(obj),
        'mdebug_sha256': digest(debug_payload(obj)), 'portable_elf': portable_elf(obj.read_bytes()),
        'portability_controls': portability_controls(obj, work),
        'target_manifest_sha256': sha(score.ASM_DIR/'SHA256SUMS'),
        'compiler_sha256': {name: sha(score.ido(name)) for name in ['cc','cfe','uld','umerge','uopt','ugen','as1']},
        'gnu_link': {'equal_bytes': 140, 'code_sha256': digest(blob[:140]),
                     'zero_alignment_bytes': len(blob)-140, 'relocation_count': len(relocs)},
        'owned_data_sections': own, 'o32_layout': 'verified_by_compile_time_assertions',
        'causal_controls': controls, 'context_sources': sources, 'context': context,
        'context_scope': 'unchanged accepted list primitives, not a complete caller/callee closure',
        'behavior': behavior(words, struct.unpack('>35I', blob[:140]), work)}

def binding():
    receipt = json.loads((HERE/'verification.json').read_text())
    for field in ['inputs', 'context_sources']:
        for path, expected in receipt[field].items(): assert sha(ROOT/path) == expected, path
    score.targets()  # validate the current protected manifest
    assert digest(struct.pack('>35I', *score.targets()[NAME])) == receipt['target']['sha256']
    return receipt

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', action='store_true')
    parser.add_argument('--record', action='store_true')
    args = parser.parse_args()
    if args.compiler or args.record:
        with tempfile.TemporaryDirectory(prefix='visual-list-proof-') as tmp: receipt = build(Path(tmp))
        if args.record: (HERE/'verification.json').write_text(json.dumps(receipt, indent=2, sort_keys=True)+'\n')
        else:
            expected = binding()
            # Full-object/debug hashes remain original provenance. Actual replay still
            # checks every semantic ELF section and both source-path controls.
            for field in ['target_manifest_sha256', 'object_sha256', 'mdebug_sha256']:
                expected.pop(field, None)
                receipt.pop(field, None)
            assert receipt == expected, 'fresh evidence differs from the source-bound receipt'
    else: receipt = binding()
    print('%s: %s, %d words; %d behavior cases' % (NAME, receipt['status'],
          receipt['target']['words'], receipt['behavior']['cases']))
if __name__ == '__main__': main()
