"""Reproduce stock/traced object identity. Raw observation logs stay private."""
from pathlib import Path
import argparse
import importlib.util
import json
import os
import sys
import tempfile

HERE = Path(__file__).resolve().parent


def run(repo, traced_uopt, out=None, record=False):
    repo = Path(repo).resolve()
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    spec = importlib.util.spec_from_file_location('pair_proof', HERE / 'verify.py')
    proof = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(proof)
    out = Path(out).resolve() if out else Path(tempfile.mkdtemp(prefix='resurrection-trace-'))
    out.mkdir(parents=True, exist_ok=True)
    score.compile_group(HERE / 'group', out / 'stock.o')
    original = score._run

    def observed(command, **kwargs):
        command = list(command)
        if Path(command[0]).name == 'uopt':
            command[0] = str(Path(traced_uopt).resolve())
            command += ['-l', str(out / 'uopt.list')]
            environment = {key: value for key, value in os.environ.items()
                           if not key.startswith(('CDX_', 'W5D_'))}
            environment.update(CDX_LOG='1', CDX_DETAIL_WEB='all', CDX_SYMTAB='1',
                               CDX_OUT=str(out / 'colors.log'), W5D_PROC='func_800C4F68',
                               W5D_OUT=str(out / 'before.log'), W5D_LEVEL='3')
            kwargs['env'] = environment
        return original(command, **kwargs)

    score._run = observed
    try:
        score.compile_group(HERE / 'group', out / 'traced.o')
    finally:
        score._run = original
    proof.require((out / 'stock.o').read_bytes() == (out / 'traced.o').read_bytes(),
                  'observation changed object')
    proof.require((out / 'uopt.list').stat().st_size > 0 and
                  (out / 'colors.log').stat().st_size > 0, 'observation logs missing')
    result = {'observation_only': True, 'forced_colors': False, 'complete_objects_identical': True,
              'object_sha256': proof.digest(out / 'stock.o'),
              'source_sha256': proof.source_inputs(), 'proof_inputs': proof.proof_inputs(),
              'environment_inputs': proof.environment_inputs(score, repo),
              'traced_uopt_sha256': proof.digest(traced_uopt)}
    if record:
        (HERE / 'trace_fidelity.json').write_text(json.dumps(result, indent=2) + '\n')
    else:
        proof.require(result == json.loads((HERE / 'trace_fidelity.json').read_text()),
                      'trace receipt changed')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', required=True, type=Path)
    parser.add_argument('--traced-uopt', required=True, type=Path)
    parser.add_argument('--out', type=Path)
    parser.add_argument('--record', action='store_true')
    args = parser.parse_args()
    run(args.repo, args.traced_uopt, args.out, args.record)
    print('PASS: complete stock/traced object identity')
