#!/usr/bin/env python3
"""Protocol-v2 input and strict-score controls. Run on a current checkout."""
from contextlib import redirect_stdout
import hashlib, importlib.util, io, json, os
from pathlib import Path
import struct, sys, tempfile
HERE = Path(__file__).resolve().parent

def verify(repo=None, compile=True, source=None):
    repo = Path(repo).resolve() if repo else HERE.parents[3]
    source = Path(source) if source else repo / 'cloud/matches/func_800B9740.c'
    receipt = json.loads((HERE / 'receipt.json').read_text())
    assert hashlib.sha256(Path(__file__).read_bytes()).hexdigest() == receipt['verify_sha256'], 'replay script mutation'
    data = source.read_bytes()
    assert hashlib.sha256(data).hexdigest() == receipt['source_sha256'], 'source mutation'
    assert receipt['claims'] == ['func_800B9740'] and receipt['native_bytes'] == 408
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    words = score.targets()['func_800B9740']
    assert len(words) == 102
    assert hashlib.sha256(struct.pack('>102I', *words)).hexdigest() == receipt['target_words_sha256'], 'target row changed'
    if not compile:
        return {'input_integrity': True, 'compiler_run': False}
    code = data.decode()
    variants = {
        'candidate': code,
        'endpoint_pointer_arithmetic': code.replace('((s16 *)((u8 *)D_801407F0.ranges[j].points + D_801407F0.ranges[j].count * sizeof(Vertex)))', 'D_801407F0.ranges[j].points[D_801407F0.ranges[j].count]'),
        'wrong_kind': code.replace('ranges[j].kind == 1', 'ranges[j].kind == 2'),
        'early_break': code.replace('                    validPt = 1;', '                    { validPt = 1; break; }'),
        'conditional_extrema_stores': code.replace('D_801407D4[j] = D_801407D4[j] < D_801409E8[i][j] ? D_801407D4[j] : D_801409E8[i][j];', 'if (D_801409E8[i][j] < D_801407D4[j]) D_801407D4[j] = D_801409E8[i][j];').replace('D_801407B4[j] = D_801407B4[j] > D_801409E8[i][j] ? D_801407B4[j] : D_801409E8[i][j];', 'if (D_801409E8[i][j] > D_801407B4[j]) D_801407B4[j] = D_801409E8[i][j];'),
    }
    results = {}
    with tempfile.TemporaryDirectory(prefix='res-path-bounds-') as tmp:
        old = Path.cwd()
        try:
            os.chdir(tmp)
            for name, text in variants.items():
                assert name == 'candidate' or text != code, 'control did not mutate source'
                path = Path(tmp) / (name + '.c')
                obj = path.with_suffix('.o')
                path.write_text(text)
                score.compile_single(path, receipt['flags'], obj)
                with redirect_stdout(io.StringIO()):
                    comparison = score.compare(obj, 'func_800B9740', show=0)
                assert comparison.accepted() == (name == 'candidate'), name + ': ' + comparison.summary()
                results[name] = comparison.summary()
        finally:
            os.chdir(old)
    return {'input_integrity': True, 'compiler_run': True, 'results': results}

if __name__ == '__main__':
    print(json.dumps(verify(sys.argv[1] if len(sys.argv) > 1 else None), indent=2))
