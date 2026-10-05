#!/usr/bin/env python3
"""Fail-closed full-body, GNU relocation, source-context and behavior proof."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
SPEC = importlib.util.spec_from_file_location('heap_max_behavior', HERE / 'behavior.py')
behavior = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(behavior)
FN = 'func_800E79F8'
STUB = 'func_800E79F0'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE = HERE / 'group/heap_max.c'
CONTEXT = ['frontier_heap_free_total', 'audio_heap']


def sha(data):
    return hashlib.sha256(data).hexdigest()


def checked(args, **kw):
    return subprocess.run(args, check=True, capture_output=True, text=True, **kw)


def symbols(obj):
    data, sections = score._elf(obj)
    text = score._text_index(sections)
    found = {}
    for i, sec in enumerate(sections):
        if sec['type'] == 2:
            for s in score._symbol_table(data, sections, i):
                if s['section'] == text and s['type'] == 2:
                    assert s['name'] not in found, 'duplicate function'
                    found[s['name']] = s
    return found


def exact(obj, name, must_match=True):
    syms = symbols(obj); fn = syms[name]
    expected = len(score.targets()[name]) * 4
    assert fn['size'] == expected, 'ELF extent differs'
    lo, hi = fn['value'], fn['value'] + fn['size']
    for other, sym in syms.items():
        if other != name:
            assert not (sym['value'] < hi and sym['value'] + sym['size'] > lo), 'function overlap'
    words = score.text_words(obj)
    got, masks, unresolved, unverified, errors = score.relocate(obj, words, lo, hi, score.image_symbols())
    assert not (masks or unresolved or unverified or errors), 'incomplete relocation'
    got = got[lo // 4:hi // 4]
    want = score.targets()[name]
    c = score.compare(obj, name, show=0)
    if must_match:
        assert c.accepted(), c.summary()
        assert got == want, 'full-word mismatch'
    target = struct.pack('>%dI' % len(want), *want)
    raw = struct.pack('>%dI' % len(got), *got)
    relocation_count = 0
    data, secs = score._elf(obj)
    for sec in secs:
        if sec['type'] == 9 and sec['info'] == score._text_index(secs):
            for off in range(sec['off'], sec['off'] + sec['size'], 8):
                site, _ = struct.unpack_from('>II', data, off)
                relocation_count += lo <= site < hi
    return {'bytes': expected, 'elf_function_bytes': fn['size'], 'comparison': asdict(c),
            'target_sha256': sha(target), 'relocated_sha256': sha(raw),
            'relocations': relocation_count}, got


def independent_gnu(obj, work):
    syms = symbols(obj)
    start = score.image_symbols()[FN]
    base = start - syms[FN]['value']
    ld = work / 'native.ld'
    ld.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n' % base +
                  ''.join('%s = 0x%x;\n' % (name, addr) for name, addr in score.image_symbols().items()
                          if name not in syms))
    elf, raw = work / 'native.elf', work / 'native.bin'
    checked(['mips-linux-gnu-ld', '-EB', '-T', str(ld), '-o', str(elf), str(obj)])
    checked(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(elf), str(raw)])
    image = raw.read_bytes(); linked = symbols(elf)
    out = {}
    for name in (STUB, FN):
        f = linked[name]; length = len(score.targets()[name]) * 4
        assert f['value'] == score.image_symbols()[name] and f['size'] == length
        body = image[f['value'] - base:f['value'] - base + length]
        want = struct.pack('>%dI' % (length // 4), *score.targets()[name])
        assert body == want, 'GNU linked mismatch'
        out[name] = {'address': hex(f['value']), 'bytes': length, 'sha256': sha(body)}
    fn_off = start - base
    trailing = image[fn_off + 160:]
    assert not any(trailing)
    data, secs = score._elf(obj)
    assert not any(s['size'] for s in secs if s['name'] in ('.data', '.rodata', '.rdata', '.bss')), 'unexpected owned data'
    return {'functions': out, 'excluded_prefix_bytes': syms[STUB]['value'],
            'trailing_zero_alignment_bytes': len(trailing), 'own_data_bytes': 0}, list(struct.unpack('>40I', image[fn_off:fn_off + 160]))


def group(directory, source, members, keep):
    directory.mkdir(parents=True, exist_ok=True)
    (directory / 'group.c').write_text(source)
    (directory / 'group.json').write_text(json.dumps({'files': ['group.c'], 'members': members,
                                                   'keep': keep, 'flags': FLAGS}))
    obj = directory / 'out.o'; score.compile_group(directory, obj)
    return obj


def contexts(work):
    source = SOURCE.read_text()
    # Append the candidate's two unaltered bodies. Keep the accepted prefix
    # byte-for-byte, sharing its identical selector and struct contracts.
    bodies = source[source.index('u32 func_800E79F8(Heap *heap)'):]
    result = {}
    for name in CONTEXT:
        path = ROOT / 'src/blob/groups' / name
        spec = json.loads((path / 'group.json').read_text())
        accepted = (path / 'group.c').read_text()
        baseline = group(work / (name + '_baseline'), accepted, spec['members'], spec['keep'])
        full = accepted + '\nu32 func_800E79F0(Heap *heap);\n' + bodies
        assert full.startswith(accepted)
        combined = group(work / (name + '_combined'), full,
                         spec['members'] + [STUB, FN], spec['keep'] + [FN])
        rows = {}
        for member in spec['members']:
            old, old_words = exact(baseline, member)
            new, new_words = exact(combined, member)
            assert old == new and old_words == new_words, 'accepted context changed'
            rows[member] = new
        preexisting = {}
        for member in spec.get('context', []):
            old, old_words = exact(baseline, member, False)
            new, new_words = exact(combined, member, False)
            assert old_words == new_words, 'pre-existing nonmatch body changed'
            preexisting[member] = {'baseline': old, 'combined': new, 'complete_body_unchanged': True}
        result[name] = {'source_sha256': sha(accepted.encode()), 'accepted_prefix_unchanged': True,
                        'members': rows, 'candidate': exact(combined, FN)[0],
                        'stub': exact(combined, STUB)[0], 'preexisting_nonmatches': preexisting}
    return result


def controls(work):
    baseline = ROOT / 'cloud/work/tiny_A40/func_800E79F8.c'
    obj = work / 'old.o'; score.compile_single(baseline, FLAGS, obj)
    old = score.compare(obj, FN, show=0)
    assert old.differing == 21 and old.extra_words == 0
    source = SOURCE.read_text()
    wrapper_start = source.index('u32 func_800E79F8(Heap *heap)')
    worker_start = source.index('u32 func_800E79F0(Heap *heap)\n{')
    worker_first = source[:wrapper_start] + source[worker_start:] + '\n' + source[wrapper_start:worker_start]
    swapped = group(work / 'worker_before', worker_first, [STUB, FN], [FN])
    before = score.compare(swapped, FN, show=0)
    assert before.differing == 2 and before.extra_words == 0
    # Same accepted selector, scan directly in the wrapper; no helper added.
    scan = source[worker_start:]
    scan = scan.replace('u32 func_800E79F0(Heap *heap)', 'u32 func_800E79F8(Heap *heap)')
    scan = scan.replace('    block = heap_or_default(heap)->first;', '    osRecvMesg(&D_80152770, 0, 1);\n    block = heap_or_default(heap)->first;')
    scan = scan.replace('    return largest;', '    osJamMesg(&D_80152770, 0, 0);\n    return largest;')
    direct = work / 'direct.c'; direct.write_text(source[:wrapper_start] + scan)
    obj = work / 'direct.o'; score.compile_single(direct, FLAGS, obj)
    no_worker = score.compare(obj, FN, show=0)
    assert no_worker.differing == 2 and no_worker.extra_words == 0
    return {'archived_baseline': {'source': str(baseline.relative_to(ROOT)), 'sha256': sha(baseline.read_bytes()),
                                 'comparison': asdict(old)},
            'selector_only_direct_scan': asdict(no_worker), 'worker_defined_before_wrapper': asdict(before)}


def host_checks(work, corpus, returns):
    root = work / 'host'; (root / 'group').mkdir(parents=True)
    shutil.copy(HERE / 'host.c', root / 'host.c')
    shutil.copy(SOURCE, root / 'group/heap_max.c')
    command = ['cc', '-std=c89', '-pedantic-errors', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
               '-fsanitize=undefined', '-fno-sanitize-recover=all', str(root / 'host.c'), '-o', str(root / 'run')]
    checked(command)
    text = behavior.corpus_text(corpus)
    run = checked([str(root / 'run')], input=text)
    actual = [int(line) for line in run.stdout.splitlines()]
    assert actual == returns, 'host/native/oracle mismatch'
    mutations = {
        'signed_size_compare': ('largest < block->size', '(s32)largest < (s32)block->size'),
        'used_blocks_included': ('block->used == 0', 'block->used != 0'),
        'first_block_only': ('block = block->next;', 'block = 0;'),
        'default_cached_before_lock': ('    osRecvMesg(&D_80152770, 0, 1);',
                                       '    heap = heap_or_default(heap);\n    osRecvMesg(&D_80152770, 0, 1);'),
    }
    source = SOURCE.read_text(); results = {}
    for name, (old, new) in mutations.items():
        assert source.count(old) == 1
        (root / 'group/heap_max.c').write_text(source.replace(old, new))
        checked(command)
        run = subprocess.run([str(root / 'run')], input=text, text=True, capture_output=True)
        assert run.returncode == 0, 'mutation failed for an unrelated host error'
        bad = sum(int(line) != want for line, want in zip(run.stdout.splitlines(), returns))
        assert len(run.stdout.splitlines()) == len(returns) and bad > 0, 'mutation escaped'
        results[name] = {'discriminating_cases': bad}
    return {'cases': len(corpus), 'c89_ubsan': 'passed', 'complete_struct_and_queue_snapshots': 'passed',
            'mutations_rejected': results, 'host_pointer_width_limit': 'Host layout follows native C pointer width; native offset layout checked separately.'}


def abi(work):
    source = SOURCE.read_text()
    checks = '''
#define OFF(t,m) ((unsigned int)&(((t *)0)->m))
typedef char block_size[(sizeof(Block)==32)?1:-1];
typedef char block_next[(OFF(Block,next)==4)?1:-1];
typedef char block_size_field[(OFF(Block,size)==12)?1:-1];
typedef char block_used[(OFF(Block,used)==20)?1:-1];
typedef char heap_first[(OFF(Heap,first)==8)?1:-1];
'''
    p = work / 'abi.c'; p.write_text(source + checks)
    score.compile_single(p, FLAGS, work / 'abi.o')


def verify(work):
    score.targets(); abi(work)
    obj = work / 'candidate.o'; score.compile_group(HERE / 'group', obj)
    candidate, words = exact(obj, FN); stub, _ = exact(obj, STUB)
    linked, linked_words = independent_gnu(obj, work)
    native, corpus, returns = behavior.native_checks(words, linked_words)
    # Interpret canonical native bytes separately, not just relocated candidate.
    for case, expected in zip(corpus, returns):
        assert behavior.execute(score.targets()[FN], case)[0] == expected
    native['native_executions'] += len(corpus)
    return {'status': 'MATCH', 'new_candidate_bytes': 160, 'accepted_byte_gain': 0,
            'candidate': candidate, 'existing_stub': stub, 'gnu_link': linked,
            'context': contexts(work), 'controls': controls(work),
            'native_behavior': native, 'host_behavior': host_checks(work, corpus, returns),
            'source_sha256': sha(SOURCE.read_bytes()),
            'sources': {str(p.relative_to(HERE)): sha(p.read_bytes()) for p in
                        [SOURCE, HERE/'group/group.json', HERE/'host.c', HERE/'behavior.py', HERE/'verify.py']},
            'target_manifest_sha256': sha((score.ASM_DIR / 'SHA256SUMS').read_bytes()),
            'scorer_sha256': sha((ROOT / 'tools/cloud/score.py').read_bytes()),
            'compiler_sha256': {tool: sha(Path(score.ido(tool)).read_bytes()) for tool in
                                ['cc', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']},
            'limits': ['Stub source identity is inferred from native placement and accepted sibling structure.',
                       'No arcade original for this N64 heap subsystem is claimed.',
                       'Queue calls use explicit O32 hooks, not native OS scheduler execution.',
                       'Finite acyclic mapped heaps only; invalid pointers, cycles and asynchronous writes excluded.',
                       'No production splice, whole-program shadow, image, compression, ROM hash or gameplay verification.']}


def compare_receipt(actual, frozen):
    # Unrelated accepted-target annotations can change the global manifest.
    # The live manifest was independently revalidated; every per-body target,
    # source, ELF, relocation, context and behavior field must still agree.
    expected = dict(frozen)
    expected['target_manifest_sha256'] = actual['target_manifest_sha256']
    assert actual == expected, 'receipt differs'


def main():
    parser = argparse.ArgumentParser(); parser.add_argument('--record', action='store_true')
    parser.add_argument('--output', type=Path); args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='heap-max-proof-') as tmp:
        result = verify(Path(tmp))
    if not args.record:
        compare_receipt(result, json.loads((HERE/'verification.json').read_text()))
    text = json.dumps(result, indent=2, sort_keys=True) + '\n'
    if args.output: args.output.write_text(text)
    print(text)

if __name__ == '__main__':
    main()
