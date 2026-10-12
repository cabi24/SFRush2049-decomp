#!/usr/bin/env python3
"""Optional diagnostic replay with an existing CDX-instrumented uopt binary.

Acceptance always uses verify.py and the stock compiler. This script checks
unforced tracer fidelity before applying exactly one register-color oracle.
No assembly, native payload, or object is written outside its temporary dir.
"""
import argparse
from contextlib import contextmanager
import importlib.util
import os
from pathlib import Path
import subprocess
import tempfile
from unittest.mock import patch

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('selector_verify_trace', HERE / 'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


@contextmanager
def traced(tracer, log, force=None):
    original = v.score._run
    def run(command, **kwargs):
        if Path(command[0]).name == 'uopt':
            command = [str(tracer)] + command[1:]
            env = dict(os.environ)
            env.pop('CDX_FORCE', None)
            env.update(CDX_LOG='1', CDX_PROC='0', CDX_OUT=str(log))
            if force:
                env['CDX_FORCE'] = force
            kwargs['env'] = env
        return original(command, **kwargs)
    with patch.object(v.score, '_run', run):
        yield


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--tracer', type=Path, required=True)
    args = parser.parse_args()
    tracer = args.tracer.resolve()
    v.require(tracer.is_file(), 'instrumented uopt required')
    with tempfile.TemporaryDirectory(prefix='selector-trace-') as name, v.targets():
        temp = Path(name)
        for label, source, expected, register in (
            ('separate_index', v.baseline((v.GROUP / 'group.c').read_text()), 3, 'a0'),
            ('reused_selector', (v.GROUP / 'group.c').read_text(), 0, 'a1')):
            folder = temp / label
            folder.mkdir()
            (folder / 'group.c').write_text(source)
            (folder / 'group.json').write_bytes((v.GROUP / 'group.json').read_bytes())
            stock, trace, log = [folder / p for p in ('stock.o', 'trace.o', 'colors.log')]
            v.score.compile_group(folder, stock)
            with traced(tracer, log):
                v.score.compile_group(folder, trace)
            # The entire generated object, not just the selected member, agrees.
            v.require(stock.read_bytes() == trace.read_bytes(), 'unforced tracer fidelity')
            v.require(v.score.compare(stock, v.NEW, show=0).differing == expected, 'stock result')
            record = 'p2color phase=p2 proc=0 web=2 sym=2 color=%d reg=%s forced=-2' % (3 if register == 'a0' else 4, register)
            v.require(record in log.read_text(), 'selector trace record')
            print(label + ': faithful; selector p2:w2=' + register + '; differing=' + str(expected))
            if label == 'separate_index':
                with traced(tracer, log, 'p2:w2=c4'):
                    v.score.compile_group(folder, trace)
                v.require(v.score.compare(trace, v.NEW, show=0).accepted(), 'single-color oracle')
                v.require('reg=a1 forced=4' in log.read_text(), 'force was applied')
                print('single diagnostic force p2:w2=c4: MATCH; never acceptance evidence')


if __name__ == '__main__':
    main()
