#!/usr/bin/env python3
"""One unchanged-body graphics-family control; emit scalar/hash evidence only."""
from pathlib import Path
import contextlib
import dataclasses
import hashlib
import io
import json
import re
import shutil
import struct
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

HERE = Path(__file__).resolve().parent
BASELINE = ROOT / 'cloud/work/frontier/w4a/func_80087110/best.c'
GROUP = ROOT / 'src/blob/groups/gfx_modes'
NAME = 'func_80087110'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def function_extent(obj, name):
    data, sections = score._elf(obj)
    text = score._text_index(sections)
    found = [symbol for i, section in enumerate(sections) if section['type'] == 2
             for symbol in score._symbol_table(data, sections, i)
             if symbol['section'] == text and symbol['type'] == 2 and symbol['name'] == name]
    assert len(found) == 1, (name, found)
    assert found[0]['size'] > 0, name
    return found[0]['value'], found[0]['size']


def inspect(obj, name):
    with contextlib.redirect_stdout(io.StringIO()):
        comparison = score.compare(obj, name, show=0)
    start, size = function_extent(obj, name)
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, score.image_symbols())
    body = resolved[start // 4:(start + size) // 4]
    target = score.targets()[name]
    proof = dataclasses.asdict(comparison)
    proof.update(elf_function_bytes=size, exact_extent=size == len(target) * 4,
                 strict_score_zero=comparison.accepted(), notes=list(comparison.notes))
    if not any((masks, unresolved, unverified, errors)):
        proof['fully_relocated_body_sha256'] = sha(struct.pack('>' + str(len(body)) + 'I', *body))
        proof['residual_offsets'] = [hex(i * 4) for i in range(max(len(body), len(target)))
                                     if i >= len(body) or i >= len(target) or body[i] != target[i]]
    else:
        # Never label a masked or unverified comparison as a fully relocated hash.
        proof['fully_relocated_body_sha256'] = None
        proof['residual_offsets'] = None
    return proof


def source_bindings(spec):
    paths = [BASELINE, GROUP / 'group.json'] + [GROUP / name for name in spec['files']]
    return {str(path.relative_to(ROOT)): sha(path.read_bytes()) for path in paths}


def native_inventory():
    addresses = score.image_symbols()
    targets = score.targets()
    target_address = addresses[NAME]
    callers = []
    for name, words in targets.items():
        for index, word in enumerate(words):
            site = addresses[name] + index * 4
            if word >> 26 == 3 and (((site + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)) == target_address:
                callers.append({'caller': name, 'entry': hex(addresses[name]),
                                'offset': hex(index * 4), 'address': hex(site)})
    caller_names = sorted({call['caller'] for call in callers})
    definitions = []
    patterns = {name: re.compile(r'(?m)^[ \t]*(?:static\s+)?(?:void|s32|int|u32)\s+' + name
                                + r'\s*\([^;{}]*\)\s*\{') for name in caller_names}
    for directory in ['src', 'cloud/work', 'work']:
        for path in sorted((ROOT / directory).rglob('*.c')):
            content = path.read_text(errors='replace')
            for name, pattern in patterns.items():
                for match in pattern.finditer(content):
                    index, depth = match.end(), 1
                    while depth and index < len(content):
                        depth += (content[index] == '{') - (content[index] == '}')
                        index += 1
                    body = content[match.end():index - 1]
                    uncommented = re.sub(r'/\*.*?\*/|//[^\n]*', '', body, flags=re.S).strip()
                    definitions.append({'name': name, 'source': str(path.relative_to(ROOT)),
                                        'source_sha256': sha(path.read_bytes()),
                                        'line': content[:match.start()].count('\n') + 1,
                                        'empty_body_after_comment_removal': uncommented == ''})
    locks = json.loads((ROOT / 'blob_matched.lock.json').read_text())
    neighbors = [{'name': name, 'address': hex(address), 'bytes': len(targets[name]) * 4,
                  'accepted_recipe': locks.get(name)} for name, address in sorted(addresses.items(), key=lambda pair: pair[1])
                 if 0x80086A50 <= address <= 0x80087A08 and name in targets]
    return {'direct_callers': callers, 'typed_definition_inventory': definitions,
            'definition_scan_limit': 'ordinary void/s32/int/u32 function definitions in src, cloud/work, work',
            'physical_neighbors': neighbors,
            'target_callee_count': sum(word >> 26 == 3 or (word >> 26 == 0 and word & 63 == 9)
                                       for word in targets[NAME])}


def main():
    build = ROOT / 'build/87110_compile_context'
    build.mkdir(parents=True, exist_ok=True)
    spec = json.loads((GROUP / 'group.json').read_text())
    bindings = source_bindings(spec)
    known = spec['members'] + spec.get('context', [])
    assert len(known) == len(set(known))
    assert NAME not in known and NAME not in spec['keep']
    objects = {}
    single = build / 'standalone.o'
    score.compile_single(BASELINE, FLAGS, single)
    objects['standalone'] = single
    unchanged = build / 'accepted_group.o'
    score.compile_group(GROUP, unchanged)
    objects['accepted_group'] = unchanged
    augmented = build / 'augmented_group'
    augmented.mkdir(exist_ok=True)
    for name in spec['files']:
        shutil.copyfile(GROUP / name, augmented / name)
    shutil.copyfile(BASELINE, augmented / 'texture_rectangle.c')
    new_spec = dict(spec)
    new_spec['files'] = spec['files'] + ['texture_rectangle.c']
    new_spec['members'] = spec['members'] + [NAME]
    new_spec['keep'] = spec['keep'] + [NAME]
    new_spec['claims'] = []
    (augmented / 'group.json').write_text(json.dumps(new_spec, indent=2) + '\n')
    combined = build / 'augmented_group.o'
    score.compile_group(augmented, combined)
    objects['augmented_group'] = combined
    results = {label: {name: inspect(obj, name) for name in
                     ([NAME] if label == 'standalone' else known + ([NAME] if label == 'augmented_group' else []))}
               for label, obj in objects.items()}
    assert bindings == source_bindings(spec), 'source input changed during verification'
    for name in spec['files']:
        assert (GROUP / name).read_bytes() == (augmented / name).read_bytes()
    assert BASELINE.read_bytes() == (augmented / 'texture_rectangle.c').read_bytes()
    regression = {name: results['accepted_group'][name] == results['augmented_group'][name] for name in known}
    receipt = {'accepted': False, 'eligible_for_promotion': False,
               'hypothesis': 'unchanged kept rectangle is affected by authentic graphics-group context',
               'source_bindings': bindings, 'baseline_flags': FLAGS,
               'native_inventory': native_inventory(),
               'existing_group_recipe': spec, 'augmented_group_recipe': new_spec,
               'compiler_components': {name: sha(Path(score.ido(name)).read_bytes()) for name in
                                       ['cc', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']},
               'protected_manifest_sha256': sha((score.ASM_DIR / 'SHA256SUMS').read_bytes()),
               'object_sha256': {name: sha(obj.read_bytes()) for name, obj in objects.items()},
               'results': results, 'neighbor_receipts_unchanged': regression,
               'all_original_sources_unchanged': True,
               'rectangle_receipt_unchanged': results['standalone'][NAME] == results['augmented_group'][NAME]}
    (build / 'verification.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps({'rectangle_receipt_unchanged': receipt['rectangle_receipt_unchanged'],
                      'neighbor_receipts_unchanged': regression,
                      'results': results}, indent=2))


if __name__ == '__main__':
    main()
