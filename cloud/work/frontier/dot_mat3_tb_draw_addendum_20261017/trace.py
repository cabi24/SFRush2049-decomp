"""Observation-only stock/instrumented ugen identity; logs stay outside the packet."""
from pathlib import Path
import argparse
import importlib.util
import json
import os
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('tb_replay', HERE/'replay.py')
replay = importlib.util.module_from_spec(spec)
spec.loader.exec_module(replay)


def source_bindings():
    return {'inherited': {n: replay.sha((replay.parent_group()/n).read_bytes())
                          for n in replay.inherited()['files']},
            'added': {n: replay.sha((HERE/n).read_bytes()) for n in
                      ['tb_draw.c', 'text_width.c', 'group.json', 'trace.py']}}


def observe(ugen, output):
    output.mkdir(parents=True, exist_ok=True)
    group = output/'group'
    replay.materialize(group, 'first')
    replay.score.compile_group(group, output/'stock.o')
    original = replay.score._run
    observed = []

    def invoke(command, **kwargs):
        command = list(command)
        if Path(command[0]).name == 'ugen':
            command[0] = str(ugen.resolve())
            env = {k: v for k, v in os.environ.items()
                   if not k.startswith(('CDX_', 'W5D_', 'DKWB_', '_DKWB_'))}
            env.update(DKWB_UGEN_TRACE='1', DKWB_UGEN_SCHED='1', _DKWB_UGEN_TRACE='1')
            kwargs['env'] = env
            result = original(command, **kwargs)
            log = result.stdout+result.stderr
            replay.require(bool(log), 'instrumented observation missing')
            (output/'ugen.log').write_text(log)
            observed.append(True)
            return result
        return original(command, **kwargs)

    replay.score._run = invoke
    try:
        replay.score.compile_group(group, output/'traced.o')
    finally:
        replay.score._run = original
    replay.require(observed == [True], 'instrumented stage inventory changed')
    replay.require((output/'stock.o').read_bytes() == (output/'traced.o').read_bytes(),
                   'observation changed complete object')
    return {'observation_only': True, 'forced_registers': False,
            'complete_objects_identical': True,
            'object_sha256': replay.sha((output/'stock.o').read_bytes()),
            'instrumented_ugen_sha256': replay.sha(ugen.read_bytes()),
            'sources': source_bindings(),
            'compiler': {n: replay.sha(Path(replay.score.ido(n)).read_bytes()) for n in
                         ['cc', 'cfe', 'ujoin', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']}}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ugen', required=True, type=Path)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--record', action='store_true')
    args = parser.parse_args()
    if args.output:
        result = observe(args.ugen, args.output)
    else:
        with tempfile.TemporaryDirectory(prefix='tb-observation-') as temp:
            result = observe(args.ugen, Path(temp))
    if args.record:
        (HERE/'trace.json').write_text(json.dumps(result, indent=2)+'\n')
    else:
        replay.require(result == replay.read_json(HERE/'trace.json'), 'trace receipt changed')
    print('PASS: complete stock/instrumented ugen objects agree')
