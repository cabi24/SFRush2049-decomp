#!/usr/bin/env python3
"""Sealed research replay: natural exact helper, unmatched caller, no new credit."""
from pathlib import Path
from contextlib import redirect_stdout
import argparse
import dataclasses
import hashlib
import io
import json
import shutil
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
MEMBERS = ['render_display_list', 'func_80099B30', 'func_8009C3F8',
           'camera_update_c', 'select_screen_update', 'particle_system',
           'track_collision_wall', 'Input_ProcessGameplayPad', 'func_800BFD8C',
           'func_800D3430', 'func_8008B2B4', 'func_8008B2E4']
CONTEXT = ['func_800D348C', 'stunt_combo_display', 'func_8009F058',
           'camera_follow_path', 'func_800C4F68']
FILES = ['resurrection.c', 'camera_pair.c', 'particle_context.c',
         'entry_context.c', 'func_8008B2B4.c']
KEEP = ['camera_update_c', 'select_screen_update', 'Input_ProcessGameplayPad',
        'func_800BFD8C', 'stunt_combo_display', 'func_800C4F68', 'func_8008B2E4',
        'func_800D3430', 'func_8008B2B4']
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def source_inputs():
    return {'group/' + name: digest(HERE / 'group' / name)
            for name in FILES + ['group.json']}


def proof_inputs():
    return {name: digest(HERE / name)
            for name in ['verify.py', 'trace_fidelity.py', 'README.md']}


def environment_inputs(score, repo):
    return {
        'compiler': {name: digest(Path(score.ido(name))) for name in
                     ['cc', 'cfe', 'ujoin', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']},
        'tools': {'score.py': digest(Path(score.__file__)),
                  'owndata.py': digest(Path(score.owndata.__file__))},
        'lock_sha256': digest(repo / 'blob_matched.lock.json'),
        'native_manifest_sha256': digest(score.ASM_DIR / 'SHA256SUMS'),
        'native_symbols_sha256': digest(score.ASM_DIR / 'symbols.json'),
        'owned_manifest_sha256': digest(score.owndata.artifact_dir(score.ASM_DIR) / 'SHA256SUMS')}


def verify_scope(group):
    require(group['members'] == MEMBERS and group['context'] == CONTEXT,
            'group membership changed')
    require(group['files'] == FILES and group['keep'] == KEEP and
            group['flags'] == FLAGS, 'group recipe changed')
    require(group['claims'] == [] and not group.get('unprototyped') and
            not group.get('allow_unverified'), 'uncertain group contract')


def verify_report(receipt):
    require(receipt['protocol_version'] == 3 and receipt['status'] == 'NONMATCH research'
            and receipt['claims'] == [] and receipt['new_accepted_bytes'] == 0
            and receipt['canonical_or_rom_verification'] is False,
            'false research credit')
    require(receipt['preserved_locked_members'] == MEMBERS and
            receipt['scored_context'] == CONTEXT and receipt['flags'] == FLAGS,
            'saved scope changed')
    controls = receipt['control_results']
    require(controls['natural']['camera_follow_path'] == 'MATCH' and
            controls['natural']['func_800C4F68'] == '17/354 words differ' and
            controls['natural']['extents'] == {
                'camera_follow_path': {'bytes': 1484, 'frame': 104},
                'func_800C4F68': {'bytes': 1416, 'frame': 80}},
            'natural research result changed')
    require(set(controls) == set(receipt['variants']) and
            all(row['preserved_locked_matches'] == 12 for row in controls.values()),
            'saved control inventory changed')
    expected = receipt['expected_results']
    require(set(expected) == set(controls), 'saved result inventory changed')
    native_sizes = dict(render_display_list=10236, func_80099B30=204,
                        func_8009C3F8=452, camera_update_c=36, select_screen_update=36,
                        particle_system=2900, track_collision_wall=2712,
                        Input_ProcessGameplayPad=2720, func_800BFD8C=852,
                        func_800D3430=92, func_8008B2B4=48, func_8008B2E4=72)
    for tag, result in expected.items():
        for field in ['scores', 'comparisons', 'extents']:
            require(set(result[field]) == set(MEMBERS + CONTEXT),
                    'saved native inventory changed')
        require(set(result['inventory']['functions']) == set(MEMBERS + CONTEXT),
                'saved emitted inventory changed')
        for name in MEMBERS:
            row = result['comparisons'][name]
            require(not any(row[key] for key in
                    ['differing', 'extra_words', 'unresolved', 'unverified', 'errors']),
                    'saved preserved match changed')
            require(result['extents'][name]['bytes'] == native_sizes[name] and
                    result['inventory']['functions'][name]['bytes'] == native_sizes[name],
                    'saved preserved extent changed')
        require(all(result['scores'][name] == controls[tag][name] and
                    result['extents'][name] == controls[tag]['extents'][name]
                    for name in CONTEXT[-2:]), 'saved control results disagree')


def inspect_object(score, obj):
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    symbols = [symbol for i, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(data, sections, i)
               if symbol['type'] == 2 and symbol['section'] == text_index]
    require(len(symbols) == len(MEMBERS + CONTEXT) and
            {symbol['name'] for symbol in symbols} == set(MEMBERS + CONTEXT),
            'complete emitted function inventory changed')
    words = score.text_words(obj)
    extents = {}
    for symbol in symbols:
        start = symbol['value'] // 4
        body = words[start:start + symbol['size'] // 4]
        frame = next((65536 - (word & 65535) for word in body[:40]
                      if word >> 16 == 10173 and word & 32768), 0)
        extents[symbol['name']] = {'bytes': symbol['size'], 'frame': frame}
    text_section = sections[text_index]
    text = data[text_section['off']:text_section['off'] + text_section['size']]
    gaps = []
    end = 0
    for symbol in sorted(symbols, key=lambda item: item['value']):
        start = symbol['value']
        require(start >= end, 'overlapping emitted functions')
        if start != end:
            require(not any(text[end:start]), 'unaccounted nonzero text')
            gaps.append({'offset': end, 'bytes': start - end})
        end = start + symbol['size']
    if end < len(text):
        require(not any(text[end:]), 'unaccounted nonzero tail')
        gaps.append({'offset': end, 'bytes': len(text) - end})
    inventory = {'functions': {symbol['name']: {'offset': symbol['value'], 'bytes': symbol['size']}
                               for symbol in sorted(symbols, key=lambda item: item['name'])},
                 'text_bytes': len(text), 'zero_alignment': gaps}
    section_hashes = {section['name']: hashlib.sha256(
        data[section['off']:section['off'] + section['size']]).hexdigest()
        for section in sections if section['type'] != 8}
    return extents, inventory, section_hashes


def clean_summary(summary):
    # Preserve the failure category and location without publishing native words.
    return summary.split(' (+0x')[0]


def verify(repo=None, compile=True, record=False):
    repo = Path(repo).resolve() if repo else HERE.parents[3]
    receipt = json.loads((HERE / 'receipt.json').read_text())
    group = json.loads((HERE / 'group/group.json').read_text())
    verify_scope(group)
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    sources = source_inputs()
    targets = {name: hashlib.sha256(struct.pack('>' + str(len(score.targets()[name])) + 'I',
                                                *score.targets()[name])).hexdigest()
               for name in MEMBERS + CONTEXT}
    environment = environment_inputs(score, repo)
    proofs = proof_inputs()
    require(sources == receipt['source_sha256'], 'source mutation')
    require(targets == receipt['target_words_sha256'], 'target row changed')
    if not record:
        verify_report(receipt)
        require(receipt['verify_sha256'] == digest(HERE / 'verify.py'), 'verify mutation')
        require(receipt['proof_inputs'] == proofs, 'proof input changed')
        require(receipt['environment_inputs'] == environment, 'tool or environment changed')
    if not compile:
        return {'input_integrity': True, 'compiler_run': False, 'claims': []}
    code = (HERE / 'group/camera_pair.c').read_text()
    variants = {'natural': code, 'inherited_six_unused_diagnostic': code.replace('    Player *player;', '    Player *player;\n    s32 xv, yv, zv, last_cp_index;\n    f32 *dp;\n    void *chkpnt;'), 'wrong_resurrection_point_stride': code.replace('typedef s16 Position[3];', 'typedef s16 Position[4];'), 'wrong_checkpoint_stride': code.replace('s16 pathPts[20];u8 tail[6];', 's16 pathPts[20];u8 tail[10];'), 'flatten_fasin_wrapper': code.replace('angle = camera_update_c(pt[0]);', 'angle = func_8009C3F8(pt[0],0);'), 'sequential_fallback_stores': code.replace('        matrix[5]=-sine;\n', '').replace('        matrix[4]=cosine;', '        matrix[4]=cosine;\n        matrix[5]=-sine;'), 'wrong_fasin_axis': code.replace('angle = camera_update_c(pt[0]);', 'angle = camera_update_c(pt[1]);')}
    require(list(variants) == receipt['variants'], 'control inventory changed')
    results = {}
    controls = {}
    natural_object = None
    with tempfile.TemporaryDirectory(prefix='resurrection-pair-') as tmp:
        for tag, source in variants.items():
            require(tag == 'natural' or source != code, 'unchanged control ' + tag)
            path = Path(tmp) / tag
            shutil.copytree(HERE / 'group', path)
            (path / 'camera_pair.c').write_text(source)
            obj = path / 'result.o'
            score.compile_group(path, obj)
            extents, inventory, sections = inspect_object(score, obj)
            rows, summaries = {}, {}
            for name in MEMBERS + CONTEXT:
                with redirect_stdout(io.StringIO()):
                    comparison = score.compare(obj, name, show=0)
                if name in MEMBERS:
                    require(comparison.accepted(), tag + ': lost preserved match ' + name)
                    require(extents[name]['bytes'] == len(score.targets()[name]) * 4,
                            tag + ': preserved extent changed ' + name)
                if name == 'camera_follow_path':
                    require(comparison.accepted() == (tag in ['natural', 'inherited_six_unused_diagnostic']),
                            tag + ': helper outcome changed')
                if name == 'func_800C4F68' and tag in ['natural', 'inherited_six_unused_diagnostic']:
                    require(comparison.differing == (17 if tag == 'natural' else 0) and
                            not any([comparison.extra_words, comparison.errors,
                                     comparison.unresolved, comparison.unverified]),
                            tag + ': caller outcome changed')
                row = dataclasses.asdict(comparison)
                row['errors'] = [error.split(' expected ')[0].split(' (+0x')[0]
                                 for error in row['errors']]
                rows[name] = row
                summaries[name] = clean_summary(comparison.summary())
            controls[tag] = {'camera_follow_path': summaries['camera_follow_path'],
                             'func_800C4F68': summaries['func_800C4F68'],
                             'extents': {name: extents[name] for name in CONTEXT[-2:]},
                             'preserved_locked_matches': len(MEMBERS)}
            results[tag] = {'scores': summaries, 'comparisons': rows, 'extents': extents,
                            'inventory': inventory, 'sections': sections, 'object_sha256': digest(obj)}
            if tag == 'natural':
                natural_object = obj.read_bytes()
        repeat = Path(tmp) / 'repeat'
        shutil.copytree(HERE / 'group', repeat)
        score.compile_group(repeat, repeat / 'result.o')
        require((repeat / 'result.o').read_bytes() == natural_object, 'complete repeat changed')
    require(controls == receipt['control_results'], 'saved control results changed')
    if record:
        receipt.update(protocol_version=3, new_accepted_bytes=0, canonical_or_rom_verification=False,
                       verify_sha256=digest(HERE / 'verify.py'), proof_inputs=proofs,
                       environment_inputs=environment, expected_results=results)
        verify_report(receipt)
        (HERE / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    else:
        require(results == receipt['expected_results'], 'complete sealed result changed')
        trace = json.loads((HERE / 'trace_fidelity.json').read_text())
        require(trace['source_sha256'] == sources and trace['proof_inputs'] == proofs and
                trace['environment_inputs'] == environment, 'trace input binding changed')
        require(trace['observation_only'] is True and trace['forced_colors'] is False and
                trace['complete_objects_identical'] is True and
                trace['object_sha256'] == results['natural']['object_sha256'], 'trace pair changed')
    return {'input_integrity': True, 'compiler_run': True, 'claims': [], 'results': results}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('repo', nargs='?', type=Path)
    parser.add_argument('--record', action='store_true')
    args = parser.parse_args()
    verify(args.repo, record=args.record)
    print('PASS: natural helper exact; caller 17/354; twelve preserved matches; zero credit')
