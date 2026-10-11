#!/usr/bin/env python3
"""Replay a strict D816C source match, two causal controls, and the blocked caller."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP = HERE / 'group'
MEMBERS = ['func_800D816C', 'func_800D91A0', 'render_replay_ui',
           'render_results_screen', 'func_800D8078', 'func_800D8154',
           'func_800DA0B4', 'func_8008B32C', 'func_800B5898', 'func_800B5940']
CONTEXT = ['drone_throttle_calc']
FILES = ['target.c', 'caller.c', 'rows.c', 'predicate.c', 'dtc.c', 'd8154.c', 'helpers.c']
KEEP = ['func_800D91A0', 'render_results_screen', 'func_8008B32C', 'func_800B5898', 'func_800B5940']
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
REFERENCES = [
    'D_8002EB94', 'D_8011196C', 'D_80111998', 'D_80112A98',
    'D_80112A9C', 'D_80112AA0', 'D_80112AA4', 'D_80112AA8',
    'D_80112AAC', 'D_80112AB0', 'D_80112AB4', 'D_80112AB8',
    'D_8011361C', 'D_80113910', 'D_80113ED4', 'D_80113ED8',
    'D_80113EDC', 'D_80113EE0', 'D_80114060', 'D_801140EC',
    'D_801140F0', 'D_8011418C', 'D_80120270', 'D_80120280',
    'D_8012E700', 'D_80139300', 'D_8013C068', 'D_80140BDC',
    'D_80144030', 'D_80146150', 'D_80149DA8', 'D_80149DD4',
    'D_8014A0F8', 'D_8014A100', 'D_8014A10A', 'D_8014A10C',
    'D_8014A118', 'D_80151AD0', 'D_8015418C', 'D_80154618',
    'D_80154630', 'D_80154648', 'D_80154FC0', 'D_80157244',
    'ambient_sound_set', 'cosf', 'drone_target_update', 'entity_flags_apply',
    'entity_spawn_callback', 'func_8008D870', 'func_8008E26C', 'func_800A1910',
    'func_800B24EC', 'func_800B4FB0', 'func_800C77C4', 'func_800D9058',
    'input_rec0', 'math_utility', 'memcpy', 'model_data_load',
    'model_transform_setup', 'particle_velocity_set', 'resource_type_select', 'sinf',
    'sound_control', 'sound_handles_clear', 'sound_stop', 'state_word_a',
    'string_copy_format',
]
_old_path = sys.path[:]
try:
    sys.path[:0] = [str(ROOT), str(ROOT / 'tools/cloud')]
    _spec = importlib.util.spec_from_file_location('d816c_stock_delay_score', ROOT / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(_spec)
    sys.modules[_spec.name] = score
    _spec.loader.exec_module(score)
    from tools.conveyor import compilers
finally:
    sys.path[:] = _old_path


def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


@contextmanager
def blob():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/blob'
    try:
        yield
    finally:
        score.ASM_DIR = previous


def inputs():
    spec = json.loads((GROUP / 'group.json').read_text())
    require(spec['members'] == MEMBERS and spec['context'] == CONTEXT,
            'member/context boundary changed')
    require(spec['claims'] == [] and spec['flags'] == FLAGS, 'research claims/flags changed')
    require(spec['files'] == FILES and spec['keep'] == KEEP, 'source/keep recipe changed')
    files = [GROUP / f for f in spec['files']] + [GROUP / 'group.json', HERE / 'verify.py',
             ROOT / 'tests/cloud/test_d816c_stock_delay.py']
    with blob():
        native = score.targets()  # current trusted manifest remains mandatory
        symbols = score.image_symbols()
        require(all(symbols.get(name) or score.address_named(name) for name in REFERENCES),
                'missing referenced image symbol')
        targets = {name: {'image': 'game', 'address': symbols.get(name) or score.address_named(name),
                         'size': len(native[name]) * 4,
                         'words_sha256': sha(struct.pack('>%dI' % len(native[name]), *native[name]))}
                   for name in MEMBERS + CONTEXT}
        return {'files_sha256': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in files},
                'referenced_symbol_rows': {name: symbols.get(name) or score.address_named(name)
                                           for name in REFERENCES},
                'targets': targets, 'owned_literals': {
                    'func_800D816C': {'address': 0x801241E8, 'size': 20,
                        'sha256': sha(score.own_data().read(0x801241E8, 20))}},
                'claims': [], 'flags': FLAGS}


def function_symbols(obj):
    raw, sections = score._elf(obj)
    return {s['name']: s for i, section in enumerate(sections) if section['type'] == 2
            for s in score._symbol_table(raw, sections, i) if s['type'] == 2}


def without_unused(source):
    for declaration in ('    f32 span;\n', '    f32 distance;\n',
                        '    f32 current_y;\n', '    s32 frame_residual3;\n'):
        require(source.count(declaration) == 1, 'disclosed frame declaration changed')
        source = source.replace(declaration, '', 1)
    return source



def causal_controls(work):
    """Both controls use only source changes and the stock compiler."""
    source = (GROUP / 'target.c').read_text()
    shaped = """                if (j < D_8014A0F8[player]) before++;
                count++;
                if (count) {}"""
    original = """                count++;
                if (j < D_8014A0F8[player]) before++;"""
    require(source.count(shaped) == 1, 'count source lever changed')
    variants = {
        'original_order': (source.replace(shaped, original), 2),
        'without_count_read': (source.replace(shaped, shaped.rsplit('\n', 1)[0]), 10),
    }
    results = {}
    for label, (content, expected) in variants.items():
        directory = work / label
        directory.mkdir()
        spec = json.loads((GROUP / 'group.json').read_text())
        for filename in spec['files'] + ['group.json']:
            (directory / filename).write_text(content if filename == 'target.c'
                                              else (GROUP / filename).read_text())
        obj = work / (label + '.o')
        score.compile_group(directory, obj)
        value = score.compare(obj, MEMBERS[0], show=0)
        require(value.differing == expected and value.total == 699 and
                not any((value.extra_words, value.unresolved, value.unverified, value.errors)),
                'causal source control changed: ' + label)
        results[label] = asdict(value)
    caller = (GROUP / 'caller.c').read_text()
    load = 'else if ((j = input_rec0[p].buttons) & 5)'
    call = 'resource_type_select(j);'
    require(caller.count(load) == caller.count(call) == 1, 'button carrier changed')
    restored = caller.replace(load, 'else if (input_rec0[p].buttons & 5)').replace(
        call, 'resource_type_select(input_rec0[p].buttons);')
    directory = work / 'without_button_carrier'
    directory.mkdir()
    spec = json.loads((GROUP / 'group.json').read_text())
    for filename in spec['files'] + ['group.json']:
        (directory / filename).write_text(restored if filename == 'caller.c'
                                          else (GROUP / filename).read_text())
    obj = work / 'without_button_carrier.o'
    score.compile_group(directory, obj)
    value = score.compare(obj, MEMBERS[1], show=0)
    require((value.differing, value.total, value.extra_words) == (239, 965, 1)
            and not any((value.unresolved, value.unverified, value.errors)),
            'button-carrier source control changed')
    require(score.compare(obj, MEMBERS[0], show=0).accepted(),
            'D816C changed with original caller spelling')
    results['without_button_carrier'] = asdict(value)
    return results


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'canonical IDO 5.3 required')
    with blob(), tempfile.TemporaryDirectory(prefix='d816c-stock-delay-') as tmp:
        work = Path(tmp)
        obj = work / 'group.o'
        spec = score.compile_group(GROUP, obj)
        symbols = function_symbols(obj)
        results = {}
        for name in MEMBERS + CONTEXT:
            value = score.compare(obj, name, show=0)
            require(not value.unresolved, 'unresolved references: ' + name)
            expected_size = binding['targets'][name]['size'] + 0
            require(symbols[name]['size'] == expected_size, 'compiled extent changed: ' + name)
            following = [s['value'] for s in symbols.values()
                         if s['section'] == symbols[name]['section'] and s['value'] > symbols[name]['value']]
            if following:
                require(min(following) == symbols[name]['value'] + expected_size,
                        'unexpected gap or overlapping next function: ' + name)
            if name == MEMBERS[0]:
                require((value.differing, value.total, value.extra_words) == (0, 699, 0)
                        and not value.unverified and not value.errors, 'D816C strict source match changed')
            elif name == MEMBERS[1]:
                require((value.differing, value.total, value.extra_words) == (166, 965, 0)
                        and not value.unverified and not value.errors, 'D91A0 research residual changed')
            elif name in CONTEXT:
                require(value.differing == value.extra_words == 0 and len(value.unverified) == 20
                        and len(value.errors) == 1 and not value.accepted(),
                        'split-rodata context status changed; independently reassess it')
            else:
                require(value.accepted(), 'existing preservation member failed: ' + name)
            results[name] = asdict(value)
        start = symbols[MEMBERS[0]]['value']
        got, masks, unresolved, unverified, errors = score.relocate(
            obj, score.text_words(obj), start, start + 2796, score.image_symbols())
        want = score.targets()[MEMBERS[0]]
        offsets = [4*i for i, word in enumerate(want)
                   if (got[start//4+i] & masks.get(start+4*i, 0xFFFFFFFF)) !=
                      (word & masks.get(start+4*i, 0xFFFFFFFF))]
        require(offsets == [], 'strict D816C unexpectedly differs')
        control = work / 'without_unused'
        control.mkdir()
        for filename in spec['files'] + ['group.json']:
            content = (GROUP / filename).read_text()
            (control / filename).write_text(without_unused(content) if filename == 'target.c' else content)
        control_obj = work / 'control.o'
        score.compile_group(control, control_obj)
        value = score.compare(control_obj, MEMBERS[0], show=0)
        require(value.differing == 20 and not any((value.extra_words, value.unresolved,
                value.unverified, value.errors)), 'four-slot control changed')
        control_start = function_symbols(control_obj)[MEMBERS[0]]['value']
        frame_word = score.text_words(control_obj)[control_start//4 + 6]
        require(frame_word >> 16 == 0x27BD and (frame_word & 65535) == 65536 - 216,
                'unshaped frame is no longer216')
        causal = causal_controls(work)
        return {'inputs': binding, 'results': results, 'causal_controls': causal, 'residual_byte_offsets': offsets,
                'unused_home_control': {'count': 4, 'bytes': 16, 'frame': 216, 'differing': 20},
                'new_claim_bytes': 0, 'accepted_image_bytes': 0,
                'compiler': {'identity': 'stock IDO 5.3', 'binary_sha256': {
                    name: sha(Path(score.ido(name)).read_bytes())
                    for name in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')}}}


def verify():
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'receipt input binding changed')
    actual = replay()
    for key in ('inputs', 'results', 'residual_byte_offsets', 'unused_home_control',
                'new_claim_bytes', 'accepted_image_bytes', 'causal_controls'):
        require(actual[key] == expected[key], 'receipt replay changed: ' + key)
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded stock D816C MATCH; caller166/965, exact extent; no install claims')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'],
                'receipt input binding changed')
        print('PASS: source, own verifier, native word and literal bindings')
    else:
        verify()
        print('PASS: stock D816C MATCH, causal controls, blocked caller, eight preservation members')
