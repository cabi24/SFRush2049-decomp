#!/usr/bin/env python3
"""Reproduce F1930 in a genuine, explicitly incomplete surrounding call group."""
import argparse
import hashlib
import importlib.util
import json
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
from tools.conveyor.pipeline import blob_group

prior = ROOT / 'cloud/work/frontier/dot_billboard_helpers_20261005/verify.py'
loader = importlib.util.spec_from_file_location('name_entry_proof', prior)
helper = importlib.util.module_from_spec(loader)
loader.loader.exec_module(helper)
FN = 'func_800F1930'
EXACT = [FN, 'func_800F207C', 'func_800F2718']


def sha(data):
    return hashlib.sha256(data).hexdigest()


def inspect_group(obj, spec):
    return {n: helper.inspect(obj, n) for n in spec['members'] + spec['context']}


def stage(directory):
    directory.mkdir(parents=True)
    spec = json.loads((HERE / 'group/group.json').read_text())
    for filename in spec['files']:
        shutil.copy2(HERE / 'group' / filename, directory / filename)
    (directory / 'group.json').write_text(json.dumps(spec))
    return spec


def verify(directory):
    directory.mkdir(parents=True, exist_ok=True)
    group = HERE / 'group'
    spec = json.loads((group / 'group.json').read_text())
    obj = directory / 'group.o'
    score.compile_group(group, obj)
    names = spec['members'] + spec['context']
    result = {
        'base_commit': 'f88dc3cb3807b8be3246b719a9b1e008210b477c',
        'status': 'MATCH_IN_GENUINE_INCOMPLETE_CONTEXT', 'claims': [FN],
        'new_matching_candidate_bytes': 980, 'accepted_byte_gain': 0,
        'production_integration_status': 'BLOCKED_ON_INHERITED_DECLARATIONS_AND_OPEN_CONTEXT',
        'remaining_type_contracts': [
            'Copied accepted F2718 passes an argument to viDeadlinePassed and declares s32(s32), while other group sources declare s32(void).',
            'Copied accepted F2718 and F207C/billboard retain noncanonical InputRecord layouts and resource_type_select signedness declarations.',
            'F0F44 Ref.pos begins at native offset 20; its original array extent is unknown and the inherited [4] is not proven.',
        ],
        'flags': spec['flags'],
        'source_sha256': {p: sha((HERE / p).read_bytes()) for p in
                          ['group/' + f for f in spec['files']] + ['group/group.json', 'semantic_test.c']},
        'tools_sha256': {p: sha((ROOT / p).read_bytes()) for p in
                         ['tools/cloud/score.py', 'tools/cloud/owndata.py',
                          'tools/conveyor/pipeline/blob_group.py',
                          'cloud/work/frontier/dot_billboard_helpers_20261005/verify.py']},
        'compiler_sha256': {n: sha(Path(score.ido(n)).read_bytes()) for n in
                            ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
        'comparison': inspect_group(obj, spec), 'controls': {},
    }
    # Independent production group reader and relocation path; no splice/write.
    native, addresses = score.targets(), score.image_symbols()
    extents = {n: {'vaddr': addresses[n], 'size': 4 * len(native[n])} for n in names}
    slices, text_ndx = blob_group.member_slices(obj, names, extents)
    relocated = blob_group.relocate(obj, slices, text_ndx, addresses, members=EXACT)
    result['independent_group_relocation_equal'] = {
        n: relocated[n] == struct.pack('>%dI' % len(native[n]), *native[n]) for n in EXACT}
    # Small natural spelling control: no new formal, read, keeper or type change.
    signed = directory / 'signed_mode_literal'
    signed_spec = stage(signed)
    source = (signed / 'func_800F0F44.c').read_text()
    assert source.count('D_8014A110 == 2U') == 1
    (signed / 'func_800F0F44.c').write_text(source.replace('D_8014A110 == 2U', 'D_8014A110 == 2'))
    signed_obj = signed / 'group.o'
    score.compile_group(signed, signed_obj)
    result['controls']['signed_mode_literal'] = inspect_group(signed_obj, signed_spec)
    # A further real callee of F1210, independently researched elsewhere.
    expanded = directory / 'genuine_slot_context'
    expanded_spec = stage(expanded)
    donor = ROOT / 'cloud/work/dot_menu_options_root_20261005'
    slot, header = donor / 'slot_state_setup.c', donor / 'menu_context.h'
    source = slot.read_text().replace('#include "menu_context.h"', header.read_text())
    (expanded / 'slot_state_setup.c').write_text(source)
    expanded_spec['files'].insert(0, 'slot_state_setup.c')
    (expanded / 'group.json').write_text(json.dumps(expanded_spec))
    expanded_obj = expanded / 'group.o'
    score.compile_group(expanded, expanded_obj)
    result['controls']['genuine_slot_context'] = inspect_group(expanded_obj, expanded_spec)
    result['controls']['genuine_slot_context']['slot_state_setup'] = helper.inspect(expanded_obj, 'slot_state_setup')
    result['slot_context_inputs_sha256'] = {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in [slot, header]}
    host = directory / 'host'
    subprocess.run(['cc','-std=c89','-O1','-Wall','-Wextra','-Werror','-fsanitize=undefined',
                    '-fno-sanitize-recover=all',str(HERE / 'semantic_test.c'),'-o',str(host)], check=True)
    result['host_test'] = subprocess.run([str(host)], check=True, capture_output=True, text=True).stdout.strip()
    for n in EXACT:
        assert result['comparison'][n]['full_extent_relocated_equal']
        assert result['independent_group_relocation_equal'][n]
        assert result['controls']['genuine_slot_context'][n]['full_extent_relocated_equal']
    assert result['controls']['signed_mode_literal'][FN]['differing'] == 1
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='billboard-context-proof-') as tmp:
        result = verify(Path(tmp))
    if args.write:
        (HERE / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({'status': result['status'], 'comparison': result['comparison'][FN],
                      'independent_group_relocation_equal': result['independent_group_relocation_equal'],
                      'host_test': result['host_test']}, indent=2))
