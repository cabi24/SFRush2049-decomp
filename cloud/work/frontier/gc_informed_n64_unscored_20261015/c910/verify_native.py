"""Read-only native evidence. This does not compile or score candidate.c.

--repo REPO is required: use an authorized private current N64 checkout.
Never writes targets, locks, production sources, or verification receipts.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct

HERE = Path(__file__).resolve().parent


def need(condition, message):
    if not condition:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def words_hash(words):
    return sha(struct.pack('>%dI' % len(words), *words))


def functions(path):
    result = {}
    for part in path.read_text(encoding='utf-8').split('.section ')[1:]:
        match = re.match(r'\.text\.([^,]+)', part)
        if match:
            name = match[1]
            need(name not in result, 'duplicate target ' + name)
            result[name] = [int(x, 16) for x in re.findall(r'\.word\s+(0x[0-9A-Fa-f]+)', part)]
    return result


def validate_manifest(manifest, files):
    pins = {line.split()[1]: line.split()[0] for line in manifest.read_text().splitlines()}
    for path in files:
        need(sha(path.read_bytes()) == pins[path.name], 'target integrity: ' + path.name)


def instruction(word):
    return (word >> 26, word >> 21 & 31, word >> 16 & 31,
            word >> 11 & 31, word & 63, word & 65535)


def saved_gprs(words):
    return sorted({rt for op, rs, rt, rd, fn, imm in map(instruction, words)
                   if op == 43 and rs == 29 and (16 <= rt <= 23 or rt == 31)})


def native_contract(targets):
    wrapper = targets['save_slot_valid']
    producer = targets['func_8008E26C']
    calls = [i for i, word in enumerate(wrapper)
             if word >> 26 == 3 and 0x80000000 | ((word & 0x3ffffff) << 2) == 0x8008e26c]
    need(calls == [38], 'AF5E0 final producer call drift')
    tail = list(map(instruction, wrapper[39:]))
    need(len(tail) == 5, 'AF5E0 tail extent')
    need(tail[0][0] == 0 and tail[0][3] == 7 and tail[0][4] == 0x25,
         'AF5E0 delay slot must only form a3')
    need(tail[1][:3] == (0x23, 29, 31), 'AF5E0 restore ra')
    need(tail[2][:3] == (9, 29, 29), 'AF5E0 restore sp')
    need(tail[3][0] == 0 and tail[3][1] == 31 and tail[3][4] == 8,
         'AF5E0 return')
    need(wrapper[-1] == 0, 'AF5E0 final delay slot')
    need(instruction(producer[-4])[:3] == (0x23, 29, 2)
         and instruction(producer[-4])[-1] == 24, 'E26C restores result to v0')
    need(instruction(producer[0xac // 4])[:5] == (0, 0, 3, 4, 0)
         and producer[0xac // 4] >> 6 & 31 == 16, 'E26C narrows index left')
    need(instruction(producer[0xb8 // 4])[:5] == (0, 0, 4, 15, 3)
         and producer[0xb8 // 4] >> 6 & 31 == 16, 'E26C sign extends index')
    need(instruction(producer[0x100 // 4])[:3] == (43, 29, 15)
         and instruction(producer[0x100 // 4])[-1] == 24,
         'E26C saves sign-extended index in result spill')
    cleanup = targets['func_8038A95C']
    need(instruction(cleanup[2])[:5] == (0, 0, 20, 14, 0)
         and cleanup[2] >> 6 & 31 == 16, 'A95C consumes low s4')
    other = targets['func_8038AA14']
    need(instruction(other[0])[:5] == (0, 0, 4, 14, 0)
         and other[0] >> 6 & 31 == 16, 'AA14 consumes low a0')
    need(not (set(saved_gprs(other)) & {16, 17, 18, 19}), 'AA14 saves changed')
    callback = targets['func_8038AA8C']
    need(set(saved_gprs(callback)) == set(range(16, 24)) | {31}, 'AA8C save context')
    need(instruction(callback[12])[:5] == (0, 0, 5, 14, 0)
         and callback[12] >> 6 & 31 == 16, 'AA8C signed-low a1 operation')
    caller = targets['audio_frame_update']
    sites = [i * 4 for i, word in enumerate(caller)
             if word >> 26 == 3 and 0x80000000 | ((word & 0x3ffffff) << 2) == 0x8038c910]
    need(sites == [0xd4], 'genuine caller site drift')
    return {
        'AF5E0': 'tail forwards E26C v0; no instruction after its final call redefines v0',
        'E26C': 'narrows scene index through left16/arithmetic-right16, saves it at stack+24, then restores it to v0',
        'A95C': 'low-halfword slot input in s4; requires genuine private context',
        'AA14': 'low-halfword a0 slot; s0-s3 are not saved in this helper',
        'AA8C': 'saves s0-s7/ra and consumes operation in signed low a1',
        'C910_caller': {'function': 'audio_frame_update', 'call_offset': '0xd4'},
    }


def verify(repo):
    receipt = json.loads((HERE / 'inputs.json').read_text())
    for name, expected in receipt['files'].items():
        need(sha((HERE / name).read_bytes()) == expected, 'source input drift: ' + name)
    repo = Path(repo)
    overlay = repo / 'asm/us/ovl_b'
    blob = repo / 'asm/us/blob'
    paths = [overlay / 'ovl_b_8038a400.s']
    other = [blob / name for name in receipt['blob_regions']]
    validate_manifest(overlay / 'SHA256SUMS', paths + [overlay / 'extents.json', overlay / 'symbols.json'])
    validate_manifest(blob / 'SHA256SUMS', other)
    extent = json.loads((overlay / 'extents.json').read_text())
    for name, expected in receipt['image'].items():
        need(extent[name] == expected, 'image population drift: ' + name)
    rows = {row['name']: row for row in extent['functions']}
    need(int(rows['func_8038C910']['address'], 16) == 0x8038c910
         and rows['func_8038C910']['size'] == 276
         and int(rows['func_8038CA24']['address'], 16) == 0x8038ca24,
         'C910 full extent or next locked boundary drift')
    targets = {}
    for path in paths + other:
        targets.update(functions(path))
    observed = {}
    for name, expected in receipt['targets'].items():
        words = targets[name]
        actual = dict(size=len(words) * 4, words_sha256=words_hash(words))
        need(actual == expected, 'complete target words drift: ' + name)
        observed[name] = actual
    spec = importlib.util.spec_from_file_location('callback_native_reference', HERE / 'native_reference.py')
    native = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(native)
    positive = native.prove({'native_C910': targets['func_8038C910']})
    mutant = list(targets['func_8038C910'])
    flag_sites = [i for i, word in enumerate(mutant)
                  if instruction(word)[:2] == (9, 0) and word & 65535 == 0x2000]
    need(len(flag_sites) == 1, 'creation flag materialization drift')
    mutant[flag_sites[0]] = (mutant[flag_sites[0]] & 0xffff0000) | 0x1000
    try:
        native.prove({'wrong_creation_flag': mutant})
    except ValueError as error:
        need('creation ABI' in str(error), 'wrong negative-control failure')
    else:
        raise ValueError('wrong creation flag was accepted')
    memory, scenario = native.fixture(0, 0, True, False)
    try:
        native.execute([0xffffffff], memory, scenario, 0)
    except ValueError:
        pass
    else:
        raise ValueError('unknown instruction was accepted')
    return {
        'protocol_version': 2, 'population': 'ovl_b', 'claims': [],
        'new_matching_bytes': 0, 'candidate_compiled': False,
        'fresh_candidate_score': None, 'complete_target_hashes': observed,
        'native_contract': native_contract(targets),
        'native_only_fixtures': positive,
        'negative_controls': ['wrong native creation flag rejected', 'unknown instruction rejected'],
        'limits': 'native target versus finite semantic hooks only; candidate C, genuine helper execution, gameplay, relocation replay and regression builds are not verified',
    }


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(verify(args.repo), indent=2, sort_keys=True))
