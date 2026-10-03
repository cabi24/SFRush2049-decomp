#!/usr/bin/env python3
"""Reproduce the small C11 packet scores without persisting objects/native bytes."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

WORK = ROOT / 'cloud/work/boot_tail/C11-small'
MATCHES = ['8001E740', '8001E768', '8001E930', '8001E9A0']


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    sources = [(ROOT / ('cloud/matches/boot_tail/func_' + a + '.c'), 'func_' + a,
                'submitted') for a in MATCHES]
    sources += [(WORK / 'func_8001E790_NONMATCH.c', 'func_8001E790', 'best-nonmatch')]
    sources += [(p, 'func_8001E790', 'rejected-control')
                for p in sorted((WORK / 'variants').glob('*.c'),
                                key=lambda p: int(p.stem.rsplit('v', 1)[1]))]
    results = []
    for source, name, purpose in sources:
        for level in ['O2', 'O1']:
            flags = '-g0 -' + level + ' -mips2 -G 0 -non_shared'
            with tempfile.TemporaryDirectory(prefix='boot-tail-c11-') as tmp:
                obj = Path(tmp) / 'candidate.o'
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                if purpose == 'submitted' and level == 'O2' and not result.accepted():
                    raise ValueError(name + ' submitted source no longer strictly matches')
                results.append(dict(name=name, purpose=purpose,
                    source_path=str(source.relative_to(ROOT)),
                    source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                    flags=flags, effective_flags=flags + ' -Wab,-r4300_mul',
                    native_bytes=len(score.targets()[name]) * 4,
                    object_text_bytes=len(score.text_words(obj)) * 4,
                    differing_words=result.differing, total_words=result.total,
                    extra_words=result.extra_words, unresolved=result.unresolved,
                    unverified=result.unverified, errors=result.errors,
                    strict_match=result.accepted()))
    return {'schema_version': 1, 'results': results}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.output:
        args.output.write_text(result)
    else:
        print(result, end='')
