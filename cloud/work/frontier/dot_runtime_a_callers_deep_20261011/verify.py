#!/usr/bin/env python3
"""Rebuild the authentic image-A callers and complete availability family.

Targets, instructions and transient ELF files are read or generated locally;
only identity hashes, counts and callsite metadata appear in this receipt.
"""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import time
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
BASELINE = ROOT / 'cloud/work/frontier/dot_runtime_a_availability_wave_20261011'
SIZES = {'func_80392F28': 220, 'func_80393004': 180, 'func_803930B8': 264,
         'func_803931C0': 276, 'func_803932E4': 168}
CLAIMS = [name for name in SIZES if name != 'func_80393004']
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def function(source, name):
    match = re.search(r'^void ' + re.escape(name) + r'\([^;{}]*\)\s*\{', source, re.M)
    require(match is not None, 'missing complete function ' + name)
    start = match.start()
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        require(end < len(source), 'incomplete function ' + name)
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def source_contract(source, config):
    require(config['members'] == list(SIZES) + ['func_8039338C', 'func_80394058'], 'member drift')
    require(config['claims'] == CLAIMS, 'claim drift')
    require(config['context'] == ['func_80393004'], 'duplicate-credit guard')
    require(config['keep'] == ['func_80394058'], 'root/ABI drift')
    require(config['flags'] == '-g0 -O3 -mips2 -G 0 -non_shared', 'flag drift')
    require(config['files'] == ['group.c'], 'source set drift')
    old = (BASELINE / 'group.c').read_text()
    for name in list(SIZES)[:4]:
        require(function(source, name) == function(old, name), 'changed archived body ' + name)
    old_cleanup = function((BASELINE / 'cleanup.c').read_text(), 'func_803932E4')
    expected = old_cleanup.replace('MenuRecord *record;\n    for (record = D_803B5F28; record != D_803B5F28 + 23; record++) {',
                                   's32 i;\n    for (i = 0; i < 23; i++) {').replace('record->fields[1]', 'D_803B5F28[i].fields[1]')
    require(function(source, 'func_803932E4') == expected, 'cleanup transformation drift')
    require(not re.search(r'\b(?:asm|__asm|volatile)\b', source), 'unexpected source construct')
    for name in ('func_8039338C', 'func_80394058'):
        require(len(function(source, name)) > 3000, 'truncated authentic caller')


def load_score(repo):
    sys.path.insert(0, str(repo / 'tools/cloud'))
    spec = importlib.util.spec_from_file_location('deep_callers_score', repo / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    score.ASM_DIR = repo / 'asm/us/ovl_a'
    return score


def verify(repo=ROOT):
    started = time.monotonic()
    score = load_score(repo)
    source = (HERE / 'group.c').read_text()
    config = json.loads((HERE / 'group.json').read_text())
    source_contract(source, config)
    manifest = score.target_manifest()
    meta = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    require((meta['image'], meta['base'], meta['rom_offset']) == ('A', '0x8038A400', '0xB5C534'), 'wrong image')
    targets = score.targets()
    asset = (repo / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8', 'asset drift')
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == 0xB5C534, 'loader pointer drift')
    image = zlib.decompress(asset[0xB5C534-0x283D0:], -15)
    require(len(image) == 194128 and sha(image) == IMAGE_HASH, 'image drift')
    native_calls = []
    for caller, words in targets.items():
        address = int(caller[-8:], 16)
        for offset, word in enumerate(words):
            if word >> 26 != 3:
                continue
            target = ((address + 4 * offset + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            callee = 'func_%08X' % target
            if callee in SIZES:
                native_calls.append({'caller': caller, 'offset': hex(4 * offset), 'callee': callee})
    with tempfile.TemporaryDirectory(prefix='runtime-a-callers-') as td:
        temp = Path(td)
        obj = temp / 'group.o'
        build_start = time.monotonic()
        score.compile_group(HERE, obj)
        build_seconds = round(time.monotonic() - build_start, 4)
        data, sections = score._elf(obj)
        ti = score._text_index(sections)
        symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
                   for s in score._symbol_table(data, sections, i)]
        functions = {s['name']: s for s in symbols if s['type'] == 2 and s['section'] == ti}
        words = score.text_words(obj)
        scores, bindings, generated_calls, caller_audit = {}, {}, [], {}
        for name in config['members']:
            result = score.compare(obj, name, show=0)
            scores[name] = dataclasses.asdict(result)
            sym = functions[name]
            offset, size = sym['value'], sym['size']
            resolved, masks, unresolved, unverified, errors = score.relocate(
                obj, words, offset, offset + size, score.image_symbols())
            require(not any((masks, unresolved, unverified, errors)), 'full-body relocation uncertainty: ' + name)
            body = resolved[offset//4:(offset+size)//4]
            extent = next(f for f in meta['functions'] if f['name'] == name)
            target_bytes = b''.join(w.to_bytes(4, 'big') for w in targets[name])
            bindings[name] = {'address': extent['address'], 'native_bytes': extent['size'],
                              'compiled_bytes': size, 'native_sha256': sha(target_bytes),
                              'compiled_sha256': sha(b''.join(w.to_bytes(4, 'big') for w in body)),
                              'entry_evidence': extent['evidence']}
            for index, word in enumerate(body):
                if word >> 26 != 3:
                    continue
                callee = 'func_%08X' % (0x80000000 | ((word & 0x3FFFFFF) << 2))
                if callee in SIZES:
                    generated_calls.append({'caller': name, 'offset': hex(4*index), 'callee': callee})
            if name not in SIZES:
                def calls(code):
                    return [0x80000000 | ((word & 0x3FFFFFF) << 2) for word in code if word >> 26 == 3]
                native_sequence, compiled_sequence = calls(targets[name]), calls(body)
                require(native_sequence == compiled_sequence, 'authentic caller call sequence differs: ' + name)
                caller_audit[name] = {'call_count': len(native_sequence), 'callee_sequence': [hex(x) for x in native_sequence], 'same_static_call_sequence': True}
                if name == 'func_80394058':
                    # Diagnose only: these are still different complete functions.
                    residual = [4*i for i in range(20, 305) if targets[name][i] != body[i-6]]
                    require(residual == [0xb0, 0xd4, 0xdc, 0xe4, 0x170, 0x184, 0x25c, 0x290], 'root interior changed')
                    caller_audit[name]['interior_native_interval'] = ['0x50', '0x4c4']
                    caller_audit[name]['compiled_shift_bytes'] = -24
                    caller_audit[name]['interior_differing_native_offsets'] = [hex(x) for x in residual]
            if name in SIZES:
                require(size == SIZES[name] == extent['size'] == len(target_bytes), 'complete extent differs: ' + name)
                require(result.accepted() and body == targets[name], 'not a complete exact match: ' + name)
                following = min((s['value'] for s in functions.values() if s['value'] > offset), default=len(words)*4)
                require(not any(words[(offset+size)//4:following//4]), 'nonzero inter-function excess: ' + name)
        controls = {}
        variants = {
            'without_authentic_9338C': (source.replace(function(source, 'func_8039338C'), ''), 'func_803932E4'),
            'cleanup_pointer_loop': (source.replace(function(source, 'func_803932E4'), function((BASELINE/'cleanup.c').read_text(), 'func_803932E4')), 'func_803932E4'),
            'producer_three_slots': (source.replace('slot < 4', 'slot < 3'), 'func_803930B8'),
            'cleanup_twenty_two_records': (source.replace('i < 23', 'i < 22'), 'func_803932E4'),
            'wrong_mode_dirty_flag': (source.replace('D_803BA7D4 = 1;', 'D_803BA7D4 = 2;'), 'func_803931C0'),
            'wrong_selector_fallback': (source.replace('D_803BA880 = 5;', 'D_803BA880 = 6;'), 'func_80392F28'),
            'inconsistent_flag_signedness': (source.replace('extern s8 D_803BA7F0[];', 'extern u8 D_803BA7F0[];'), 'func_80393004'),
        }
        for label, (variant, name) in variants.items():
            directory = temp / label
            directory.mkdir()
            (directory / 'group.c').write_text(variant)
            (directory / 'group.json').write_text(json.dumps(config))
            other = directory / 'group.o'
            score.compile_group(directory, other)
            result = score.compare(other, name, show=0)
            require(not result.accepted(), 'negative control accepted: ' + label)
            controls[label] = {'function': name, 'source_sha256': sha(variant.encode()), 'score': dataclasses.asdict(result)}
        # C89 compiler-derived layout probe, outside the matching translation unit.
        # Offset expressions have the standard offsetof implementation shape.
        probe = temp / 'layout.c'
        probe.write_text(source + '\n#define VIEW_OFFSET(T,M) ((unsigned int)&(((T *)0)->M))\n' +
            'unsigned int verified_view_layout[] = { sizeof(void *), sizeof(PlayerSlot), VIEW_OFFSET(ListOwner,slot), '
            'VIEW_OFFSET(ListNode,payload), VIEW_OFFSET(ListPayload,next), VIEW_OFFSET(ListPayload,prev), '
            'VIEW_OFFSET(ListPayload,object), VIEW_OFFSET(ListPayload,text), VIEW_OFFSET(ListPayload,flags2C), '
            'VIEW_OFFSET(ListRoot,head), VIEW_OFFSET(ListRoot,tail), sizeof(MenuRecord), VIEW_OFFSET(ResourceNames,label) };\n')
        probe_obj = temp / 'layout.o'
        score.compile_single(probe, '-g0 -O2 -mips2 -G 0 -non_shared', probe_obj)
        pdata, psections = score._elf(probe_obj)
        psym = next(s for i, sec in enumerate(psections) if sec['type'] == 2
                    for s in score._symbol_table(pdata, psections, i) if s['name'] == 'verified_view_layout')
        section = psections[psym['section']]
        start = section['off'] + psym['value']
        layout = [int.from_bytes(pdata[start+i:start+i+4], 'big') for i in range(0, psym['size'], 4)]
        require(layout == [4, 16, 16, 0, 0, 4, 8, 20, 44, 8, 12, 64, 708], 'N64 storage-view layout differs')
        p = subprocess.run(['gcc', '-std=c89', '-pedantic-errors', '-fsyntax-only', str(HERE/'group.c')], capture_output=True, text=True)
        require(p.returncode == 0, 'C89 validation failed: ' + p.stderr)
    hashes = {str(p.relative_to(repo)): sha(p.read_bytes()) for p in (
        HERE/'group.c', HERE/'group.json', HERE/'verify.py', repo/'tools/cloud/score.py', repo/'tools/cloud/owndata.py',
        score.ASM_DIR/'SHA256SUMS', *[score.ASM_DIR/n for n in manifest])}
    toolchain = {name: sha(Path(score.ido(name)).read_bytes()) for name in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    return {'image': 'A', 'image_sha256': IMAGE_HASH, 'baseline_commit': '4296906ea06f551540a29a2547ffdffe92e92104',
            'claims': CLAIMS, 'new_candidate_bytes': sum(SIZES[n] for n in CLAIMS), 'accepted_bytes': 0,
            'flags': config['flags']+' -Wab,-r4300_mul', 'bindings': bindings, 'scores': scores,
            'native_family_calls': native_calls, 'generated_family_calls': generated_calls,
            'caller_static_audit': caller_audit, 'verified_view_layout': layout,
            'negative_controls': controls, 'hashes': hashes, 'toolchain_sha256': toolchain,
            'compile_seconds': build_seconds, 'verification_seconds': round(time.monotonic()-started, 4)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = verify()
    text = json.dumps(result, indent=2, sort_keys=True)+'\n'
    if args.output:
        args.output.write_text(text)
    print(text)
