"""Reproduce compiler evidence; objects remain under ignored build/."""
import contextlib
import dataclasses
import hashlib
import io
import json
from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

def record(obj, fn, recipe):
    offsets = score.symbols(obj)
    start = offsets[fn]
    end = min((v for v in offsets.values() if v > start), default=len(score.text_words(obj))*4)
    with contextlib.redirect_stdout(io.StringIO()):
        result = score.compare(obj, fn, show=0)
    native = score.targets()[fn]
    import struct
    return dict(function=fn, recipe=recipe, native_bytes=len(native)*4,
                native_sha256=hashlib.sha256(b''.join(struct.pack('>I', w) for w in native)).hexdigest(),
                candidate_symbol_extent=end-start, comparison=dataclasses.asdict(result))

def main():
    build = ROOT / 'build/dot_graphics_init_closure'
    build.mkdir(parents=True, exist_ok=True)
    rows = []
    for name, fn in [('init.c','sound_init'), ('flags.c','func_800878E0')]:
        obj = build / (fn+'.o')
        score.compile_single(HERE/name, score.DEFAULT_FLAGS, obj)
        rows.append(record(obj, fn, 'standalone -g0 -O2 -mips2 -G 0 -non_shared; automatic -Wab,-r4300_mul'))
    obj = build / 'group.o'
    score.compile_group(HERE, obj)
    for fn in ['sound_init','func_800878E0','func_80086A50']:
        rows.append(record(obj, fn, 'genuine three-body O3 group per group.json; as1 -r4300_mul'))
    result = dict(status='NONMATCH; no accepted coverage',
                  source_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(HERE.glob('*.c'))},
                  recipes=rows)
    print(json.dumps(result, indent=2))

if __name__ == '__main__': main()
