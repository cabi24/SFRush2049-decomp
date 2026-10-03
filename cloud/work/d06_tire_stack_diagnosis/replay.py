#!/usr/bin/env python3
"""Replay one rejected CFE spelling hypothesis; never an acceptance check."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
ARCHIVE = '433611408270ede7fe6acdb027f1b82179209511'
SOURCE = 'cloud/work/large_tire_forces/reconstruction/native_expression_scoped_tsc.c'
SOURCE_HASH = 'a78bf705049cb5d0db9fc8bcb09efa0ac819ba1e11c4d65c6daf377a3f68e8bb'
ORIGINAL = 'poortract=((vehicle->wheel_mode[2]==1 || vehicle->wheel_mode[2]==2) && (vehicle->wheel_mode[3]==1 || vehicle->wheel_mode[0]==2));'
EXPANDED = 'poortract=(vehicle->wheel_mode[2]==1 || vehicle->wheel_mode[2]==2);\n if(poortract) { poortract=(vehicle->wheel_mode[3]==1 || vehicle->wheel_mode[0]==2); }'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'


def candidate(source):
    assert hashlib.sha256(source).hexdigest() == SOURCE_HASH
    text = source.decode()
    assert text.count(ORIGINAL) == 1
    return text.replace(ORIGINAL, EXPANDED).encode()


def predicate_check():
    """Check result and short-circuit read order over all comparison classes."""
    from itertools import product
    count = 0
    for a, b, c in product((-2147483648, -1, 0, 1, 2, 3, 2147483647), repeat=3):
        values = {2: a, 3: b, 0: c}
        first, second = [], []
        def read(log, index):
            log.append(index)
            return values[index]
        x = ((read(first, 2) == 1 or read(first, 2) == 2) and
             (read(first, 3) == 1 or read(first, 0) == 2))
        y = (read(second, 2) == 1 or read(second, 2) == 2)
        if y:
            y = (read(second, 3) == 1 or read(second, 0) == 2)
        assert x == y and first == second
        count += 1
    return count


def replay():
    sys.path.insert(0, str(ROOT / 'tools/cloud'))
    import score
    source = subprocess.check_output(['git', 'show', ARCHIVE + ':' + SOURCE], cwd=ROOT)
    output = {'status': 'REJECTED_HYPOTHESIS_NONMATCH', 'claims': [],
              'archive_commit': ARCHIVE, 'source_sha256': SOURCE_HASH,
              'predicate_cases': predicate_check(), 'builds': {}}
    with tempfile.TemporaryDirectory(prefix='d06-replay-') as tmp:
        for label, content in [('baseline', source), ('expanded', candidate(source))]:
            group = Path(tmp) / label
            group.mkdir()
            (group / 'group.c').write_bytes(content)
            (group / 'group.json').write_text(json.dumps({
                'files': ['group.c'], 'keep': ['func_800E23A4'],
                'members': ['func_800E23A4'], 'context': [], 'flags': FLAGS}))
            for mode in ('O2', 'O3'):
                obj = group / (mode + '.o')
                if mode == 'O2':
                    score.compile_single(group / 'group.c', FLAGS.replace('-O3', '-O2'), obj)
                else:
                    score.compile_group(group, obj)
                with contextlib.redirect_stdout(io.StringIO()):
                    comparison = score.compare(obj, 'func_800E23A4')
                words = score.text_words(obj)
                spill = [(i, w & 65535) for i, w in enumerate(words)
                         if w >> 26 in (35, 43) and (w >> 21) & 31 == 29
                         and (w >> 16) & 31 == 3 and 120 <= (w & 65535) <= 124]
                assert comparison.differing == (2 if label == 'baseline' else 276)
                assert comparison.extra_words == (0 if label == 'baseline' else 1)
                assert not comparison.errors and not comparison.unresolved
                assert len(comparison.unverified) == 6
                assert spill == ([(179, 120), (190, 120)] if label == 'baseline'
                                 else [(181, 120), (192, 120)])
                assert not comparison.accepted()
                output['builds'][label + '_' + mode] = {
                    'summary': comparison.summary(), 'object_sha256': hashlib.sha256(obj.read_bytes()).hexdigest(),
                    'source_sha256': hashlib.sha256(content).hexdigest(),
                    'elf_text_bytes': len(words) * 4, 'frame_bytes': -(words[0] & 65535) % 65536,
                    'poortract_spill_and_reload': spill}
    return output


if __name__ == '__main__':
    print(json.dumps(replay(), indent=2))
