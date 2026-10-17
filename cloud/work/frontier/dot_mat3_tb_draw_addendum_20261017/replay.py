"""Additive real TB_DrawText boundary; partial research with complete accounting."""
from pathlib import Path
import argparse
import contextlib
import hashlib
import io
import json
import os
import re
import shutil
import shlex
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = Path(os.environ.get('N64_RESEARCH_REPO', HERE.parents[3])).resolve()
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(HERE))
from tools.cloud import score, owndata
import semantics

NAME = 'state_utility'
NEW = ['object_manager_update', 'sound_pitch_diff_calc', 'sound_pitch_diff_halved', NAME]
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
PACKET = ['README.md', 'replay.py', 'semantics.py', 'semantics.c', 'tb_draw.c',
          'text_width.c', 'group.json', 'scope.json', 'inheritance.json', 'trace.py', 'trace.json']
VARIANTS = ['baseline', 'first', 'repeat', 'flat_horizontal', 'old_header',
            'private_just', 'wrong_just_value']


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def read_json(path):
    return json.loads(path.read_text())


def scope():
    return read_json(HERE/'scope.json')


def inherited():
    return read_json(HERE/'inheritance.json')


def parent_group():
    return ROOT/inherited()['path']


def recipe():
    return read_json(HERE/'group.json')


def once(source, old, new):
    require(source.count(old) == 1, 'ambiguous source control')
    return source.replace(old, new)


def inputs():
    base = inherited()
    actual = {n: sha((parent_group()/n).read_bytes()) for n in base['files']}
    require(actual == base['files'], 'inherited PR415 source changed')
    spec = recipe()
    require(spec['members'] == base['native_members']+NEW and len(spec['members']) == 43,
            'native member scope changed')
    require(spec['claims'] == [] and spec['context'] == [] and spec['flags'] == FLAGS
            and not spec.get('allow_unverified') and not spec.get('unprototyped'),
            'recipe contract changed')
    old = read_json(parent_group()/'group.json')
    require(spec['files'] == old['files']+['text_width.c', 'tb_draw.c']
            and spec['keep'] == old['keep']+NEW, 'genuine context recipe changed')
    ts = score.targets()
    symbols = score.image_symbols()
    return {'packet': {n: sha((HERE/n).read_bytes()) for n in PACKET},
            'inherited_sources': actual,
            'focused_test_sha256': sha((ROOT/'tests/cloud/test_mat3_tb_draw_addendum.py').read_bytes()),
            'recipe': spec,
            'compiler': {n: sha(Path(score.ido(n)).read_bytes()) for n in
                         ['cc', 'cfe', 'ujoin', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']},
            'tools': {n: sha((ROOT/n).read_bytes()) for n in
                      ['tools/cloud/score.py', 'tools/cloud/owndata.py', 'blob_matched.lock.json']},
            'target_metadata': {n: sha((score.ASM_DIR/n).read_bytes()) for n in ['SHA256SUMS', 'symbols.json']},
            'owned_manifest': sha((owndata.artifact_dir(score.ASM_DIR)/'SHA256SUMS').read_bytes()),
            'targets': {n: {'address': hex(symbols[n]), 'bytes': 4*len(ts[n]),
                           'sha256': sha(struct.pack('>%dI' % len(ts[n]), *ts[n]))}
                        for n in spec['members']}}


def materialize(path, label):
    path.mkdir()
    base = read_json(parent_group()/'group.json')
    for name in base['files']:
        shutil.copyfile(parent_group()/name, path/name)
    spec = base if label == 'baseline' else recipe()
    if label != 'baseline':
        for name in ['tb_draw.c', 'text_width.c']:
            shutil.copyfile(HERE/name, path/name)
    if label == 'flat_horizontal':
        p = path/'tb_draw.c'
        source = once(p.read_text(), 'x=sound_pitch_diff_halved(text,x)',
                      'x=x-(object_manager_update(text,-1)>>1)')
        p.write_text(once(source, 'x=sound_pitch_diff_calc(text,x)',
                          'x=x-object_manager_update(text,-1)'))
    if label == 'old_header':
        p = path/'text_width.c'
        p.write_text(once(p.read_text(), '    u8 padD[3]; /* Complete native16-byte bank header. */\n', ''))
    if label == 'private_just':
        p = path/'ctx_gr3_b.c'
        text = once(p.read_text(), 'Just D_80118E20 = {0, 3};',
                    'static Just D_80118E20 = {0, 3};')
        body = (path/'tb_draw.c').read_text().split('u32 object_manager_update(', 1)[1]
        p.write_text(text+'\nu32 object_manager_update('+body)
        spec['files'].remove('tb_draw.c')
    if label == 'wrong_just_value':
        p = path/'ctx_gr3_b.c'
        p.write_text(once(p.read_text(), 'Just D_80118E20 = {0, 3};',
                          'Just D_80118E20 = {0, 4};'))
    (path/'group.json').write_text(json.dumps(spec))


def frames(words, start, size):
    return [65536-(w & 65535) for w in words[start//4:(start+size)//4]
            if w >> 16 == 0x27bd and w & 32768]


def sections(obj):
    elf = owndata._Object(obj)
    return {s['name']: sha(elf.data[s['off']:s['off']+s['size']])
            for s in elf.sections if s['type'] not in [8, 2, 3] and s['name'] != '.mdebug'}


def inspect(obj, names):
    elf = owndata._Object(obj)
    symbols = [s for i, section in enumerate(elf.sections) if section['type'] == owndata.SHT_SYMTAB
               for s in elf.symbols(i) if s['type'] == owndata.STT_FUNC and 0 < s['section'] < len(elf.sections)]
    expected = set(names) | set(scope()['residues'])
    require(len(symbols) == len(expected) and {s['name'] for s in symbols} == expected,
            'complete emitted inventory changed')
    words = score.text_words(obj)
    rows = {}
    cursor = 0
    for s in sorted(symbols, key=lambda x: x['value']):
        require(s['value'] == cursor, 'unaccounted function gap')
        cursor += s['size']
        name = s['name']
        row = {'bytes': s['size'], 'frames': frames(words, s['value'], s['size'])}
        if name in names:
            with contextlib.redirect_stdout(io.StringIO()):
                result = score.compare(obj, name, show=0)
            row.update(json.loads(json.dumps(vars(result))))
            row['errors'] = [re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+', 'retail [redacted], got [redacted]', error) for error in row['errors']]
            row['strict'] = result.accepted()
        else:
            require(s['size'] == 8, 'nonnative residue extent changed')
            a, b = words[s['value']//4:s['value']//4+2]
            require(a >> 26 == 0 and (a >> 21) & 31 == 31 and a & 63 == 8 and b == 0,
                    'unexpected nonnative residue')
        rows[name] = row
    require(not any(words[cursor//4:]), 'unaccounted nonzero text tail')
    return {'rows': rows, 'zero_alignment_bytes': len(words)*4-cursor,
            'sections': sections(obj), 'object_sha256': sha(obj.read_bytes())}


def just_data(obj, private=False):
    elf = owndata._Object(obj)
    symbols = [s for i, section in enumerate(elf.sections) if section['type'] == owndata.SHT_SYMTAB
               for s in elf.symbols(i) if s['name'] == 'D_80118E20']
    if private:
        require(not symbols, 'private Just escaped linkage')
        index = next(i for i, sec in enumerate(elf.sections) if sec['name'] == '.data')
        offset = 0  # The sole initialized writable object in this genuine ownership control.
    else:
        require(len(symbols) == 1, 'Just named symbol missing')
        index, offset = symbols[0]['section'], symbols[0]['value']
    data = elf.raw(index)
    require(offset == 0 and len(data) == 16 and not any(data[8:]), 'Just storage window changed')
    raw = data[offset:offset+8]
    native = score.own_data().read(0x80118E20, 8)
    require(len(raw) == 8 and raw == native, 'complete eight-byte Just data mismatch')
    return {'address': '0x80118E20', 'checked_bytes': 8, 'sha256': sha(raw),
            'object_offset': offset, 'native_equal': True,
            'basis': 'sole private initializer at data offset zero' if private else 'named symbol'}


def layout(path, wrong=False):
    text = (HERE/'tb_draw.c').read_text().split('extern Just', 1)[0]
    if wrong:
        text = once(text, 'JustEnum h,v;', 'JustEnum h,v,extra;')
    text += '\nu32 layout_values[]={sizeof(Just),(u32)&((Just*)0)->h,(u32)&((Just*)0)->v};\n'
    src, obj = path/'layout.c', path/'layout.o'
    src.write_text(text)
    command = [score.ido('cc'), '-c', *shlex.split(FLAGS), score.R4300_CC, '-o', str(obj), str(src)]
    result = score._run(command, cwd=path)
    require(result.returncode == 0 and obj.is_file(), 'IDO layout compilation failed: '+result.stderr)
    elf = owndata._Object(obj)
    sym = next(s for i, sec in enumerate(elf.sections) if sec['type'] == owndata.SHT_SYMTAB
               for s in elf.symbols(i) if s['name'] == 'layout_values')
    raw = elf.raw(sym['section'])[sym['value']:sym['value']+12]
    return list(struct.unpack('>3I', raw))


def core(row):
    return {key: value for key, value in row.items() if key != 'notes'}


def check_rows(build, expected_names, targets, root_override=None):
    rows = build['rows']
    s = scope()
    require(set(rows) == set(expected_names) | set(s['residues']), 'saved function inventory changed')
    for name in expected_names:
        row = rows[name]
        accepted = not any(row[k] for k in ['differing', 'extra_words', 'unresolved', 'unverified', 'errors'])
        require(row['strict'] == accepted, 'inconsistent strict result '+name)
        require(row['total']*4 == targets[name]['bytes'], 'protected extent changed '+name)
        if name in s['strict']:
            require(accepted and row['bytes'] == s['strict'][name] == row['total']*4,
                    'preserved strict extent changed '+name)
        else:
            expected = root_override if name == NAME and root_override is not None else s['partial'][name]
            require(all(row[k] == value for k, value in expected.items()),
                    'partial outcome changed '+name)
    for name, size in s['residues'].items():
        require(rows[name] == {'bytes': size, 'frames': []}, 'saved residue changed')


def verify_report(report):
    require(report['status'] == 'PARTIAL_RESEARCH' and report['claims'] == []
            and report['new_candidate_bytes'] == report['accepted_image_bytes'] == 0,
            'false research credit')
    require(report['inputs'] == inputs(), 'saved input binding changed')
    builds = report['builds']
    require(list(builds) == VARIANTS, 'build inventory changed')
    base_names = inherited()['native_members']
    check_rows(builds['baseline'], base_names, report['inputs']['targets'])
    for label in ['first', 'repeat', 'old_header', 'private_just']:
        check_rows(builds[label], recipe()['members'], report['inputs']['targets'])
    flat = dict(scope()['partial'][NAME])
    flat.update(bytes=388, differing=77, unverified=['.data+0x0 at +0x0', '.data+0x0 at +0x4', '.data+0x4 at +0x9c', '.data+0x4 at +0xa0'])
    check_rows(builds['flat_horizontal'], recipe()['members'], report['inputs']['targets'], flat)
    wrong = dict(scope()['partial'][NAME])
    wrong.update(unverified=['.data+0x0 at +0x0', '.data+0x0 at +0x4', '.data+0x4 at +0xa4', '.data+0x4 at +0xa8'], errors=['own .data+0x4 referenced at +0xa8 differs from retail 0x80118E24 (+0x0: retail [redacted], got [redacted]; 4 bytes compared)'])
    check_rows(builds['wrong_just_value'], recipe()['members'], report['inputs']['targets'], wrong)
    require(builds['first'] == builds['repeat'], 'complete repeated object changed')
    for name in base_names:
        require(core(builds['baseline']['rows'][name]) == core(builds['first']['rows'][name]),
                'inherited result changed '+name)
    require(builds['first']['sections'] == builds['old_header']['sections'],
            'header completion changed executable/data/relocations')
    require(builds['flat_horizontal']['rows'][NAME]['differing'] == 77
            and builds['flat_horizontal']['rows'][NAME]['bytes'] == 388,
            'flattened genuine helper control changed')
    require(report['layout'] == {'native': [8, 0, 4], 'wrong_capacity': [12, 0, 4]},
            'Just layout scope changed')
    require(report['wrong_data'] == 'rejected: complete eight-byte Just data mismatch',
            'complete data negative changed')
    require(set(report['just_data']) == set(VARIANTS)-{'wrong_just_value'}, 'Just proof inventory changed')
    for row in report['just_data'].values():
        require(row['native_equal'] is True and row['checked_bytes'] == 8
                and row['sha256'] == 'd5688a52d55a02ec4aea5ec1eadfffe1c9e0ee6a4ddbe2377f98326d42dfc975',
                'complete Just proof changed')
    trace = read_json(HERE/'trace.json')
    require(trace['observation_only'] is True and trace['forced_registers'] is False
            and trace['complete_objects_identical'] is True
            and trace['object_sha256'] == builds['first']['object_sha256']
            and trace['compiler'] == report['inputs']['compiler'], 'source-bound trace changed')
    require(trace['sources'] == {'inherited': report['inputs']['inherited_sources'],
            'added': {n: report['inputs']['packet'][n] for n in ['tb_draw.c', 'text_width.c', 'group.json', 'trace.py']}}, 'trace source changed')
    require(report['semantics']['fixtures'] == 37748736, 'semantic scope changed')
    require(report['semantics']['results']['positive'] == dict(returncode=0, stdout='37748736 bounded text placement fixtures passed\n', stderr=''),
            'semantic positive changed')
    require(set(report['semantics']['results']) == {'positive'} | set(semantics.CONTROLS),
            'semantic negative inventory changed')
    for name, control in semantics.CONTROLS.items():
        require(report['semantics']['results'][name] == dict(returncode=3, stdout='', stderr=f'text placement differential failure at {control[3]}\n'),
                'semantic negative result changed')


def verify(record=False):
    bound = inputs()
    saved = None if record else read_json(HERE/'proof.json')
    if saved is not None:
        verify_report(saved)
    report = {'status': 'PARTIAL_RESEARCH', 'claims': [], 'new_candidate_bytes': 0,
              'accepted_image_bytes': 0, 'inputs': bound, 'builds': {}, 'just_data': {}}
    with tempfile.TemporaryDirectory(prefix='tb-addendum-') as tmp:
        root = Path(tmp)
        for label in VARIANTS:
            path = root/label
            materialize(path, label)
            obj = path/'result.o'
            score.compile_group(path, obj)
            names = inherited()['native_members'] if label == 'baseline' else recipe()['members']
            report['builds'][label] = inspect(obj, names)
            if label == 'wrong_just_value':
                try:
                    just_data(obj)
                except RuntimeError as error:
                    require(str(error) == 'complete eight-byte Just data mismatch', 'wrong data refusal')
                    report['wrong_data'] = 'rejected: '+str(error)
                else:
                    raise RuntimeError('wrong Just initializer accepted')
            else:
                report['just_data'][label] = just_data(obj, label == 'private_just')
        report['layout'] = {'native': layout(root), 'wrong_capacity': layout(root, True)}
    report['semantics'] = semantics.run((parent_group()/'func_800B70EC.c').read_text())
    verify_report(report)
    if saved is not None:
        require(report == saved, 'complete fresh receipt changed')
    if record:
        (HERE/'proof.json').write_text(json.dumps(report, indent=2)+'\n')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    args = parser.parse_args()
    verify(args.record)
    print('PASS partial research: 43 native bodies, 37 strict, 14/99 root, full Just data; zero credit')
