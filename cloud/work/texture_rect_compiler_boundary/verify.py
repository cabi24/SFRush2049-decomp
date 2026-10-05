#!/usr/bin/env python3
"""Diagnostic-only alias-boundary isolation. No result from this script is a C match.

All compiled listings, objects, and raw disassembly remain below ignored build/.
The historical candidate and three complete C controls use the stock toolchain.
"""
import contextlib
import hashlib
import io
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

SOURCE = ROOT / 'cloud/work/frontier/w4a/func_80087110/best.c'
BUILD = ROOT / 'build/dot_87110_alias_causality_20261005'
FLAGS = ['-g0', '-O3', '-mips2', '-G', '0', '-non_shared', '-Wab,-r4300_mul']

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def run(command, cwd):
    result = subprocess.run([str(x) for x in command], cwd=cwd, capture_output=True)
    if result.returncode:
        raise RuntimeError(result.stderr.decode(errors='replace'))

def comparison(obj):
    with contextlib.redirect_stdout(io.StringIO()):
        result = score.compare(obj, 'func_80087110')
    return {key: getattr(result, key) for key in
            ('differing', 'total', 'unresolved', 'unverified', 'errors', 'extra_words')}

def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    ido = Path(os.environ.get('IDO_DIR', ROOT/'tools/cloud/ido')).resolve()
    baseline = BUILD/'baseline'
    baseline.mkdir(exist_ok=True)
    (baseline/'source.c').write_bytes(SOURCE.read_bytes())
    run([ido/'cc', '-c', '-K', *FLAGS, '-o', 'source.o', 'source.c'], baseline)
    listing = (baseline/'u.out.s').read_text()
    source_result = comparison(baseline/'source.o')
    if source_result != dict(differing=4, total=445, unresolved=[], unverified=[], errors=[], extra_words=0):
        raise RuntimeError(f'Frozen baseline changed: {source_result}')
    variants = {'identity': listing}
    # $46 is the x-flip-only test reached after the stretched both-flags arm.
    # $47 is the next y-flip-only test, a negative boundary control.
    for label in ('$46', '$47'):
        before = '\t.alias\t$3,$sp\n' + label + ':'
        if listing.count(before) != 1:
            raise RuntimeError(f'Expected alias boundary not unique: {label}')
        variants['remove_alias_before_'+label[1:]] = listing.replace(before, label+':', 1)
        variants['move_alias_after_'+label[1:]] = listing.replace(before, label+':\n\t.alias\t$3,$sp', 1)
    results = []
    for name, text in variants.items():
        folder = BUILD/name
        folder.mkdir(exist_ok=True)
        (folder/'diagnostic.s').write_text(text)
        run([ido/'as0', '-G', '0', '-mips2', '-EB', '-g0', '-O3',
             'diagnostic.s', '-o', 'diagnostic.G', '-t', 'diagnostic.T'], folder)
        run([ido/'as1', '-elf', '-G', '0', '-p0', '-mips2', '-EB', '-g0', '-O3',
             '-r4300_mul', '-Olimit', '5000', 'diagnostic.G', '-o', 'diagnostic.o',
             '-t', 'diagnostic.T'], folder)
        expected = 4 if name == 'identity' else (0 if name.endswith('_46') else 53)
        actual = comparison(folder/'diagnostic.o')
        if actual != dict(differing=expected, total=445, unresolved=[], unverified=[], errors=[], extra_words=0):
            raise RuntimeError(f'Diagnostic changed: {name}: {actual}')
        results.append(dict(name=name, diagnostic_only=True, eligible_for_promotion=False,
                            object_sha256=sha(folder/'diagnostic.o'),
                            comparison=actual))
    source_controls = []
    bindings = json.loads(Path(__file__).with_name('allocation_receipt.json').read_text())
    for name, expected in [('both_body_do', 8), ('height_before_do', 174), ('setup_do', 29)]:
        source = Path(__file__).with_name(name+'.c')
        if sha(source) != bindings[name]['source_sha256']:
            raise RuntimeError(f'Allocator receipt source binding changed: {name}')
        folder = BUILD/name
        folder.mkdir(exist_ok=True)
        (folder/'source.c').write_bytes(source.read_bytes())
        run([ido/'cc', '-c', '-K', *FLAGS, '-o', 'source.o', 'source.c'], folder)
        actual = comparison(folder/'source.o')
        if actual != dict(differing=expected, total=445, unresolved=[], unverified=[], errors=[], extra_words=0):
            raise RuntimeError(f'Source control changed: {name}: {actual}')
        source_controls.append(dict(source=str(source.relative_to(ROOT)), source_sha256=sha(source),
                                    accepted=False, eligible_for_promotion=False, input_kind='C source',
                                    comparison=actual))
    receipt = dict(function='func_80087110', source=str(SOURCE.relative_to(ROOT)),
                   source_sha256=sha(SOURCE), baseline=source_result, flags=FLAGS,
                   tool_sha256={name:sha(ido/name) for name in ('cc','uopt','ugen','as0','as1')},
                   accepted=False, diagnostic_only=True, controls=results,
                   source_controls=source_controls)
    (BUILD/'verification.json').write_text(json.dumps(receipt, indent=2)+'\n')
    print(json.dumps(receipt, indent=2))

if __name__ == '__main__':
    main()
