"""Create NEW scratch groups from this unscored handoff; never install source.

Run from an authorized private full N64 checkout. --out must not exist. No
network, build, compiler, claim, lock or promotion operation is performed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

HERE = Path(__file__).resolve().parent


def need(ok, message):
    if not ok:
        raise ValueError(message)


def materialize(repo, out):
    repo, out = repo.resolve(), out.resolve()
    packet = json.loads((HERE / 'packet.json').read_text(encoding='utf-8'))
    need(not out.exists(), 'output must be a new private scratch directory')
    need(out != repo, 'output cannot be the checkout root')
    # This fixed subtree check prevents the output being one of the source,
    # protected-input or canonical tool directories in the supplied checkout.
    if repo in out.parents:
        relative = out.relative_to(repo)
        need(relative.parts[0] in ('.scratch', 'scratch'), 'use scratch/ or .scratch/ inside the private checkout')
    sources = {}
    for filename, context in packet['blast_context'].items():
        src = repo / context['path']
        data = src.read_bytes()
        need(hashlib.sha256(data).hexdigest() == context['source_sha256'],
             'genuine context changed; rebase/review before use: ' + context['path'])
        sources[filename] = data
    # Verify ALL inputs before creating output; failed preflight leaves no group.
    for path, pin in packet['published_files'].items():
        need(hashlib.sha256((HERE/path).read_bytes()).hexdigest() == pin, 'handoff input changed: '+path)
    out.mkdir(parents=True)
    for name, row in packet['route_groups'].items():
        target = out/'routes'/name
        target.mkdir(parents=True)
        (target/'group.c').write_bytes((HERE/row['source']).read_bytes())
        (target/'group.json').write_text(json.dumps(row['spec'], indent=2)+'\n', encoding='utf-8')
    for variant in ('baseline', 'no_array'):
        target = out/'blast'/variant
        target.mkdir(parents=True)
        (target/'effects.c').write_bytes((HERE/'blast/effects.c').read_bytes())
        src = 'baseline_start_blast.c' if variant=='baseline' else 'start_blast.c'
        (target/'start_blast.c').write_bytes((HERE/'blast'/src).read_bytes())
        for name, data in sources.items():
            (target/name).write_bytes(data)
        (target/'group.json').write_text(json.dumps(packet['blast_group'], indent=2)+'\n', encoding='utf-8')
    for name in ('entity', 'c910'):
        shutil.copytree(HERE/name, out/name)
    return {'state':'SCRATCH_ONLY_UNSCORED', 'route_configurations':8,
            'blast_groups':2, 'claims':[], 'compiler_executed':False}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(materialize(args.repo, args.out), sort_keys=True))
