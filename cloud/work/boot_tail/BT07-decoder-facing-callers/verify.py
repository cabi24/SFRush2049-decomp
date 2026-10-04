#!/usr/bin/env python3
"""Read-only caller packet replay: strict bytes plus actual ELF function extents."""
from hashlib import sha256
from pathlib import Path
import json
import os
import struct
import subprocess
import sys
import tempfile

ROOT = Path(os.environ["CALLER_REPO"]) if "CALLER_REPO" in os.environ else Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
score.ASM_DIR = ROOT / 'asm/us/boot_tail'
BASE = '496a72b0edd683bd7d46e8c21f8323ab43629494'
ORIGINAL_BASE = 'eae0295b40271c4758c7a3ecb890e48192487f8c'
CLAIM = 'bd452b11'
SOURCES = {
    '8002574C': ROOT / 'cloud/matches/boot_tail/func_8002574C.c',
    '80025F74': PACKET / 'func_80025F74.c',
}
EXPECTED = {'8002574C': [(0, 0), (150, 166)],
            '80025F74': [(2, 0), (209, 54)]}
INITIAL = {'8002574C': [(15, 0), (148, 9)],
           '80025F74': [(92, 0), (210, 53)]}
TARGET_HASH = {
    '8002574C': '19d7bc111c647cce08476953e530bcd26f2cb4ce36a54e7bcbc3f420ccc603d4',
    '80025F74': '7650ca0132961f8bcc14735c6797092a1b4fd0b4c40ff3031b2e512204aa5896',
}

def digest(path):
    return sha256(path.read_bytes()).hexdigest()

def function_extent(path, name):
    data = path.read_bytes()
    assert data[:6] == b'\x7fELF\x01\x02', 'expected big-endian ELF32'
    shoff = struct.unpack_from('>I', data, 32)[0]
    entsize, count = struct.unpack_from('>HH', data, 46)
    sections = [struct.unpack_from('>10I', data, shoff + i * entsize)
                for i in range(count)]
    matches = []
    for sec in sections:
        if sec[1] != 2:
            continue
        strings_sec = sections[sec[6]]
        strings = data[strings_sec[4]:strings_sec[4] + strings_sec[5]]
        for pos in range(sec[4], sec[4] + sec[5], sec[9]):
            offset, value, size, info, other, index = struct.unpack_from('>IIIBBH', data, pos)
            symbol = strings[offset:strings.find(b'\0', offset)].decode()
            if symbol == name and info & 15 == 2 and index != 0:
                matches.append({'start': value, 'function_bytes': size,
                                'section_bytes': sections[index][5]})
    assert len(matches) == 1, (name, matches)
    return matches[0]

def trial(source, address, level, obj):
    name = 'func_' + address
    flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
    score.compile_single(source, flags, obj)
    result = score.compare(obj, name, show=0)
    native = score.targets()[name]
    words = score.text_words(obj)
    extent = function_extent(obj, name)
    assert score.symbols(obj) == {name: 0}
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, 0, extent['function_bytes'], score.image_symbols())
    assert not masks and not unresolved and not unverified and not errors
    return {
        'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
        'source_sha256': digest(source), 'object_sha256': digest(obj),
        'differing_words': result.differing, 'total_words': result.total,
        'extra_words': result.extra_words, 'strict_match': result.accepted(),
        'scorer_window_relocation_errors': result.errors,
        'full_function_relocation_errors': errors, 'relocation_mask_count': len(masks),
        'unresolved': unresolved, 'unverified': unverified,
        'full_native_window_equal': relocated[:len(native)] == native,
        'elf': extent, 'exact_function_extent': extent['function_bytes'] == len(native) * 4,
        'section_padding_is_zero': not any(words[extent['function_bytes'] // 4:]),
    }

def run():
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR,
                                       text=True).splitlines()
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    assert len(inventory) == len(extents) == 439
    assert {r['address']: r['size'] for r in inventory} == {r['address']: r['size'] for r in extents}
    assert sum(r['size'] for r in inventory) == 99120
    selected = [r for r in inventory if r['address'][2:] in SOURCES]
    assert len(selected) == 2 and sum(r['size'] for r in selected) == 1444
    assert all(r['scope'] == 'in_scope' for r in selected)
    pins = json.loads((PACKET / 'input_pins.json').read_text())
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    for path, value in pins['source_hashes'].items():
        assert digest(ROOT / path) == value, path
    receipt = {
        'packet': 'BT07-decoder-facing-callers', 'base_commit': BASE,
        'original_pre_reset_base': ORIGINAL_BASE,
        'central_claim_commit': CLAIM, 'manifest': manifest,
        'compiler_files_sha256': compiler,
        'inventory_extents': {'count': 439, 'bytes': 99120, 'equal': True},
        'results': [], 'packet_hashes': {
            str(p.relative_to(ROOT)): digest(p) for p in sorted(PACKET.rglob('*'))
            if p.is_file() and p.name not in ['verification.json', 'semantics.json', 'independent_review.json']
        },
    }
    with tempfile.TemporaryDirectory(prefix='bt07-callers-verify-') as tmp:
        tmp = Path(tmp)
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, score.DEFAULT_FLAGS, tmp / 'getter.o')
        assert score.compare(tmp / 'getter.o', getter.stem, show=0).accepted()
        assert function_extent(tmp / 'getter.o', getter.stem)['function_bytes'] == 12
        receipt['getter_strict_match'] = True
        for address, source in SOURCES.items():
            assert source.read_text().splitlines()[0] == '/* flags: ' + score.DEFAULT_FLAGS + ' */'
            native = score.targets()['func_' + address]
            target_hash = sha256(struct.pack('>%dI' % len(native), *native)).hexdigest()
            assert target_hash == TARGET_HASH[address]
            row = {'name': 'func_' + address, 'native_bytes': len(native) * 4,
                   'target_sha256': target_hash, 'source_path': str(source.relative_to(ROOT)),
                   'source_sha256': digest(source), 'trials': [], 'initial_control': []}
            for k, level in enumerate((2, 1)):
                result = trial(source, address, level, tmp / (address + '-O%d.o' % level))
                assert (result['differing_words'], result['extra_words']) == EXPECTED[address][k]
                if level == 2:
                    assert result['exact_function_extent'] and result['section_padding_is_zero']
                    assert result['strict_match'] == (address == '8002574C')
                else:
                    assert not result['strict_match']
                row['trials'].append(result)
                control = trial(PACKET / 'controls' / ('initial_func_' + address + '.c'),
                                address, level, tmp / (address + '-initial-O%d.o' % level))
                assert (control['differing_words'], control['extra_words']) == INITIAL[address][k]
                row['initial_control'].append(control)
            receipt['results'].append(row)
    receipt.update(result='PASS', new_local_strict_matching_bodies=1,
                   new_local_strict_matching_bytes=604, complete_nonmatch_bodies=1,
                   complete_nonmatch_bytes=840)
    return receipt

if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
