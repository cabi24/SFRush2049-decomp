#!/usr/bin/env python3
"""Replay the existing bounded C oracle on the new complete callback source."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import tempfile

if not __debug__:
    raise RuntimeError('the inherited oracle requires assertions; use ordinary Python')
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
OLD = ROOT / 'cloud/work/frontier/dot_runtime_a_menu_loop_20261006'
SOURCE = ROOT / 'cloud/matches/ovl_a/func_803A4340.c'


def run():
    spec = importlib.util.spec_from_file_location('menu_original_bounded_oracle', OLD / 'verify.py')
    oracle = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(oracle)
    with tempfile.TemporaryDirectory(prefix='menu-home-host-') as tmp:
        work = Path(tmp)
        host = work / 'host.c'
        host.write_text((OLD / 'host.c').read_text().replace('#include "candidate.c"', '#include "'+str(SOURCE)+'"'))
        library = work / 'host.so'
        flags = ['gcc', '-std=c89', '-O2', '-fstrict-aliasing', '-shared', '-fPIC',
                 '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-fstack-protector-all']
        subprocess.run(flags+[str(host), '-o', str(library)], check=True)
        count, digest = oracle.host_check(library, oracle.cases())
        layout = work / 'layout.c'
        layout.write_text('#include "'+str(SOURCE)+'"\n'
                          'typedef char ptr[sizeof(void*)==4?1:-1];\n'
                          'typedef char size[sizeof(Texture)==36?1:-1];\n'
                          + ''.join('typedef char f%d[__builtin_offsetof(%s,%s)==%d?1:-1];\n' % (n,t,f,n)
                                    for t,f,n in [('Texture','height',18), ('MenuIndices','first',26),
                                                  ('MenuData','indices',12), ('MenuData','text',16)]))
        subprocess.run(['gcc', '-m32', '-std=c89', '-fsyntax-only', str(layout)], check=True)
    paths = [SOURCE, OLD / 'host.c', OLD / 'verify.py', HERE / 'host_verify.py']
    return {'inputs': {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
            'cases': count, 'trace_sha256': digest, 'layout32': 'PASS',
            'scope': 'bounded hosted C89 UBSan/bounds cases against inherited oracle and helper doubles'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(); parser.add_argument('--write', action='store_true')
    args = parser.parse_args(); result = run(); receipt = HERE / 'host_receipt.json'
    if args.write:
        receipt.write_text(json.dumps(result, indent=2, sort_keys=True)+'\n')
    elif json.loads(receipt.read_text()) != result:
        raise ValueError('host replay receipt mismatch')
    print(json.dumps(result, indent=2, sort_keys=True))
