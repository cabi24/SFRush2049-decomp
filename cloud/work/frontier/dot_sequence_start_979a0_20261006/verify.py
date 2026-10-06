#!/usr/bin/env python3
"""Rebuild the complete sequence-start caller and unchanged accepted context."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
BASE = 'dea99f09ab19b1d3b324ed7097162f7b378e7096'
NAME = 'func_800979A0'
EXPECTED_DIFFERENCES = [0x18, 0x3c] + list(range(0xfc, 0x124, 4))


def require(condition, detail):
    if not condition:
        raise ValueError(detail)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def words_bytes(words):
    return struct.pack('>%dI' % len(words), *words)


def comparable(receipt):
    """Exclude enumerated historical integration provenance, never proof inputs."""
    result = json.loads(json.dumps(receipt))
    for key in ('accepted_group_spec_sha256', 'scorer_sha256', 'own_data_tool_sha256', 'native_manifest_sha256'):
        result.pop(key, None)
    return result


def pinned(repo, path):
    return subprocess.check_output(["git", "-C", str(repo), "show", BASE + ":" + path])


def context_config(repo, group):
    """Compare our explicit packet recipe/copies only to immutable BASE context."""
    config = json.loads((group / 'group.json').read_text())
    accepted_dir = 'src/blob/groups/slot_sound'
    accepted_bytes = pinned(repo, accepted_dir + '/group.json')
    accepted = json.loads(accepted_bytes)
    require(config['claims'] == [], 'Research packet must not claim matching bytes')
    require(config['members'] == accepted['members'] + [NAME], 'Context membership changed')
    require(config['keep'] == accepted['keep'] + [NAME], 'Context kept roots changed')
    require(config['files'] == accepted['files'] + [NAME + '.c'], 'Context file order changed')
    require(config['flags'] == accepted['flags'], 'Context flags changed')
    for filename in accepted['files']:
        require((group / filename).read_bytes() == pinned(repo, accepted_dir + '/' + filename),
                'Accepted source copy changed: ' + filename)
    return config, accepted_bytes


def verify(repo):
    path = repo / 'tools/cloud/score.py'
    spec = importlib.util.spec_from_file_location('sequence_start_score', path)
    score = importlib.util.module_from_spec(spec)
    import sys
    sys.path.insert(0, str(path.parent))
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    group = HERE / 'group'
    config, accepted_bytes = context_config(repo, group)
    native = score.targets()
    addresses = score.image_symbols()
    require(addresses[NAME] == 0x800979a0 and len(native[NAME]) == 73, 'Native identity changed')
    with tempfile.TemporaryDirectory(prefix='sequence-start-') as tmp:
        obj = Path(tmp) / 'group.o'
        score.compile_group(group, obj)
        data, sections = score._elf(obj)
        ti = score._text_index(sections)
        functions = {sym['name']: sym for i, sec in enumerate(sections) if sec['type'] == 2
                     for sym in score._symbol_table(data, sections, i)
                     if sym['section'] == ti and sym['type'] == 2}
        require(set(functions) == set(config['members']), 'Unexpected emitted function set')
        own_data = {s['name']: s['size'] for s in sections
                    if s['name'] in ('.data', '.rodata', '.sdata', '.bss', '.sbss') and s['size']}
        require(not own_data, 'Unexpected owned data/storage')
        text = score.text_words(obj)
        relocated, masks, unresolved, unverified, errors = score.relocate(
            obj, text, 0, len(text) * 4, addresses)
        require(not any((masks, unresolved, unverified, errors)), 'Incomplete group relocations')
        results = {}
        for name in config['members']:
            symbol = functions[name]
            emitted = relocated[symbol['value'] // 4:(symbol['value'] + symbol['size']) // 4]
            result = score.compare(obj, name, show=0)
            if name == NAME:
                require(symbol['size'] == 288, 'Caller extent changed')
                require(result.differing == 12 and result.total == 73 and result.extra_words == 0,
                        'Caller comparison changed')
                require(not any((result.unresolved, result.unverified, result.errors)), 'Unverified caller')
                offsets = [i * 4 for i, word in enumerate(native[name])
                           if i >= len(emitted) or emitted[i] != word]
                require(offsets == EXPECTED_DIFFERENCES, 'Caller residual changed')
            else:
                require(result.accepted(), 'Accepted context no longer strict: ' + name)
                require(emitted == native[name], 'Accepted complete function extent/body changed: ' + name)
            results[name] = {
                'native_address': hex(addresses[name]), 'native_bytes': len(native[name]) * 4,
                'emitted_function_bytes': symbol['size'],
                'native_sha256': sha(words_bytes(native[name])),
                'relocated_function_sha256': sha(words_bytes(emitted)),
                'comparison': asdict(result),
            }
        relocations = sum(sec['size'] // 8 for sec in sections if sec['type'] == 9 and sec['info'] == ti)
        return {
            'base_commit': BASE, 'status': 'NONMATCH', 'claims': [], 'accepted_bytes': 0,
            'target': NAME, 'interval': ['0x800979A0', '0x80097AC4'],
            'residual_offsets': [hex(x) for x in offsets], 'owned_data': own_data,
            'group_text_bytes': len(text) * 4, 'group_text_relocations': relocations,
            'flags': config['flags'], 'as1_extra_flag': score.R4300_AS1,
            'source_sha256': {filename: sha((group / filename).read_bytes()) for filename in config['files']},
            'group_spec_sha256': sha((group / 'group.json').read_bytes()),

            'tool_sha256': {name: sha((score.IDO / name).read_bytes())
                            for name in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')},



            'verification_script_sha256': sha(Path(__file__).read_bytes()),
            'functions': results,
            'limits': [
                'Complete strict compile/relocation/body comparison only; behavior reviewed separately.',
                'External services are real declared call boundaries, not reconstructed implementations.',
                'Actual array capacities, unrestricted aliases, concurrency, and original TU are not proved.',
                'No image, compressed-stream, ROM, gameplay, hardware, or accepted-coverage claim.',
            ],
        }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=HERE.parents[3])
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    receipt = verify(args.repo.resolve())
    output = HERE / 'verification.json'
    if args.check:
        require(comparable(json.loads(output.read_text())) == comparable(receipt), 'Frozen receipt differs')
    else:
        output.write_text(json.dumps(receipt, indent=2) + '\n')
    print('NONMATCH 12/73, 288/292 bytes; 12/12 accepted context bodies strict; receipt ' +
          ('replayed' if args.check else 'written'))


if __name__ == '__main__':
    main()
