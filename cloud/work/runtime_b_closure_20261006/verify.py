#!/usr/bin/env python3
"""Portable exact-source research replay. Does not admit or splice a match."""
import argparse
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
BASELINE = HERE / 'baseline'
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def read(name):
    return json.loads((BASELINE / name).read_text())


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    sys.modules[name] = result
    spec.loader.exec_module(result)
    return result


def bindings():
    """Bind only research-owned files; never production/context/test hashes."""
    packet = json.loads((HERE / 'packet.json').read_text())
    assert packet['base_commit'] == BASE
    for relative, expected in packet['files_sha256'].items():
        assert relative.startswith('cloud/work/runtime_b_'), relative
        assert not relative.endswith('/test_packet.py'), relative
        assert sha((ROOT / relative).read_bytes()) == expected, ('packet drift', relative)
    final = read('final.json')
    for relative, expected in final['files_sha256'].items():
        assert sha((BASELINE / relative).read_bytes()) == expected, ('baseline drift', relative)
    assert final['D328']['canonical_name_based_scorer_admission'] is False
    assert final['D328']['accepted_coverage_bytes'] == 0
    assert sha((BASELINE / 'closure.c').read_bytes()) == final['source_sha256']
    assert (BASELINE / 'closure.c').read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
    assert read('group.json') == dict(files=['closure.c'], keep=['func_8038FCE0'], flags=FLAGS)
    # The historical receipts retain original invocation paths as provenance.
    # Resolve their source identities to repository-relative research paths.
    for entry in read('inventory.json')['sources']:
        relative = 'cloud/work/' + entry['path'].split('/cloud/work/', 1)[1]
        assert sha((ROOT / relative).read_bytes()) == entry['sha256'], relative
    return packet


def assembly():
    """Reconstruct all eight bodies without writing or retuning source."""
    asm = module('portable_closure_assemble', BASELINE / 'assemble.py')
    pieces = ['/* flags: ' + FLAGS + ' */',
              '/* One approved genuine-closure diagnostic. Research only, no original-TU claim. */',
              (BASELINE / 'shared_schema.proposed.h').read_text()]
    changes = read('assembly.json')['changes']
    for (_, packet, filename, key), change in zip(asm.ORDER, changes):
        assert change['member'] == key
        raw = (ROOT / 'cloud/work' / packet / filename).read_text()
        text = asm.body(raw, asm.SIGNATURES[key])
        assert sha(text.encode()) == change['original_body_sha256']
        for replacement in change['replacements']:
            before, after = replacement['before'], replacement['after']
            assert text.count(before) == replacement['count']
            text = text.replace(before, after)
        if key != 'FCE0':
            text = 'static ' + text
        assert sha(text.encode()) == change['reconciled_body_sha256']
        pieces.append('/* Complete ' + key + ' body; source-preserving approved reconciliation only. */\n' + text)
    assert ('\n\n'.join(pieces) + '\n').encode() == (BASELINE / 'closure.c').read_bytes()


def score_module():
    sys.path.insert(0, str(ROOT / 'tools/cloud'))
    return module('portable_closure_score', ROOT / 'tools/cloud/score.py')


def targets(score):
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    words = score.targets()
    for name, expected in read('inventory.json')['targets'].items():
        raw = struct.pack('>' + str(len(words[name])) + 'I', *words[name])
        assert len(raw) == expected['size'] and sha(raw) == expected['sha256'], name


def run(*args):
    result = subprocess.run([sys.executable, *map(str, args)], capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr


def compiled_proof(receipt):
    """Only compiled extents/allocated data/relocations, not ELF file metadata."""
    def image(value):
        return {key: value[key] for key in ('elf_type', 'allocated', 'functions', 'relocations')}
    return dict(source_sha256=receipt['source_sha256'], recipe=receipt['recipe'],
                mandatory_backend_flag=receipt['mandatory_backend_flag'],
                layout_facts=receipt['layout']['facts'],
                object=image(receipt['object']), linked=image(receipt['linked']),
                external_bindings=receipt['external_bindings'])


def boundary_proof(receipt):
    # Full ELF hashes include debug/symbol provenance. Every executable/data
    # byte remains covered by per-section/member hashes and relocation replay.
    return {key: value for key, value in receipt.items()
            if key not in ('object_sha256', 'linked_sha256')}


def semantic_proof(receipt):
    result = copy.deepcopy(receipt)
    result.pop('reproduction_arguments', None)
    # Each fresh replay authenticates its own source, ELF, and boundary input.
    # Their historical artifact identities are not cross-build invariants.
    for key in ('linked_elf_sha256', 'boundaries_sha256'):
        result['binding'].pop(key, None)
    return result


def negative_control_proof(receipt):
    """Keep the rejection contract, not the host's zero-division wording.

    The bound controls.py still executes every native mutant and checks its
    intended failure before emitting a receipt. Only the two spellings of the
    D498 floating-point zero-distance rejection are equivalent here. All other
    reasons, control identities/order/outcomes, and unknown fields stay bound.
    """
    result = copy.deepcopy(receipt)
    for control in result['controls']:
        if (control.get('name') == 'D498_zero_distance_domain'
                and control.get('result') == 'rejected'
                and control.get('reason') in ('float division by zero', 'division by zero')):
            control['reason'] = 'float division by zero'
    return result


def first_difference(expected, actual, path='$'):
    """Report the exact receipt field if a real negative-control drift remains."""
    if type(expected) is not type(actual):
        return path, expected, actual
    if isinstance(expected, dict):
        for key in expected:
            if key not in actual:
                return path + '.' + key, expected[key], '<missing>'
            difference = first_difference(expected[key], actual[key], path + '.' + key)
            if difference:
                return difference
        for key in actual:
            if key not in expected:
                return path + '.' + key, '<missing>', actual[key]
    elif isinstance(expected, list):
        if len(expected) != len(actual):
            return path + '.length', len(expected), len(actual)
        for index, (before, after) in enumerate(zip(expected, actual)):
            difference = first_difference(before, after, path + '[' + str(index) + ']')
            if difference:
                return difference
    elif expected != actual:
        return path, expected, actual
    return None


def check_negative_controls(fresh, historical, label):
    difference = first_difference(negative_control_proof(historical),
                                  negative_control_proof(fresh))
    assert difference is None, ('negative control drift', label, difference)


def replay(reference, output=None):
    bindings()
    assembly()
    score = score_module()
    targets(score)
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        raise SystemExit('pinned IDO and MIPS GNU linker required')
    # The original scripts use reference-root for both the current canonical
    # tool route and base-history reads. In a normal full checkout this is ROOT.
    with tempfile.TemporaryDirectory(prefix='runtime-b-closure-') as name:
        tmp = Path(name)
        build, bounds, behavior, controls = [tmp / (n + '.json') for n in ('build', 'bounds', 'behavior', 'controls')]
        run(BASELINE / 'build_once.py', '--reference-root', reference,
            '--work-dir', tmp, '--output', build)
        fresh_build = json.loads(build.read_text())
        assert compiled_proof(fresh_build) == compiled_proof(read('build.json')), 'compiled proof drift'
        run(BASELINE / 'inspect_baseline.py', '--reference-root', reference,
            '--build-receipt', build, '--output', bounds)
        fresh_bounds = json.loads(bounds.read_text())
        assert boundary_proof(fresh_bounds) == boundary_proof(read('boundaries.json')), 'boundary/relocation proof drift'
        run(BASELINE / 'semantics/verify.py', '--reference-root', reference,
            '--linked-elf', tmp / 'closure.elf', '--boundaries', bounds,
            '--source', BASELINE / 'closure.c', '--output', behavior)
        fresh_behavior = json.loads(behavior.read_text())
        assert semantic_proof(fresh_behavior) == semantic_proof(read('semantics/closure-verification.json')), 'behavior proof drift'
        assert semantic_proof(fresh_behavior) == semantic_proof(read('semantic-replay.json')), 'independent behavior receipt drift'
        run(BASELINE / 'semantics/controls.py', '--reference-root', reference, '--output', controls)
        fresh_controls = json.loads(controls.read_text())
        for receipt in ('semantics/controls.json', 'semantic-controls-replay.json'):
            check_negative_controls(fresh_controls, read(receipt), receipt)
        result = dict(status='PORTABLE RESEARCH REPLAY PASS; ZERO ACCEPTED COVERAGE',
                      base_commit=BASE, source_sha256=sha((BASELINE / 'closure.c').read_bytes()),
                      target_layout_facts=fresh_build['layout']['count'],
                      relocations=fresh_bounds['exact_link_relocation_validation']['relocation_count'],
                      paired_cases=fresh_behavior['paired_cases'],
                      root_invocations_per_image=fresh_behavior['root_invocations_per_image'],
                      behavior_sha256=fresh_behavior['behavior_sha256'],
                      negative_controls=len(fresh_controls['controls']),
                      private_D328=fresh_bounds['functions']['func_8038D328']['full_unmasked_research_placement'],
                      accepted_coverage_bytes=0, compiler_work_directory_removed=True)
    if output:
        Path(output).write_text(json.dumps(result, indent=2) + '\n')
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--reference-root', type=Path, default=ROOT)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--static-only', action='store_true')
    args = parser.parse_args()
    if args.static_only:
        bindings()
        assembly()
        targets(score_module())
        result = dict(status='STATIC PACKET AND NATIVE-TARGET BINDINGS PASS; NO COMPILATION')
    else:
        result = replay(args.reference_root.resolve(), args.output)
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
