#!/usr/bin/env python3
"""Optional stock-stage diagnostics; retain metadata, never compiler listings."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('root_frame_verify', HERE / 'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)
score = v.score


def inspect(group, work):
    ordinary = work / 'ordinary.o'
    diagnostic = work / 'diagnostic.o'
    score.compile_group(group, ordinary)
    run = score._run
    info = {}

    def traced(command, **kwargs):
        command = list(command)
        stage = Path(command[0]).name
        if stage == 'umerge':
            command.insert(1, '-v')
        if stage == 'ugen':
            command += ['-e', 'trees', '-l', 'listing']
        result = run(command, **kwargs)
        if stage == 'umerge':
            # Procedure names and inline decisions are not raw assembly.
            info['inline_log'] = (result.stdout + result.stderr).strip().splitlines()
        if stage == 'as1' and result.returncode == 0:
            cwd = Path(kwargs['cwd'])
            names = re.findall(r'^\s*\.ent\s+(\w+)', (cwd / 'listing').read_text(), re.M)
            blocks = (cwd / 'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names) == len(blocks), 'stage procedure count mismatch')
            for name, block in zip(names, blocks):
                if name == 'func_80394058':
                    block = block.split('Tree dump', 1)[0]
                    info['root'] = {'external': int(re.search(r'\bent .* external=(\d+)', block)[1]),
                        'memory_areas': {kind: int(size) for kind, size in
                                         re.findall(r'\bdef mtype=(\w+) length=(\d+)', block)}}
        return result

    try:
        score._run = traced
        score.compile_group(group, diagnostic)
    finally:
        score._run = run
    v.require(ordinary.read_bytes() == diagnostic.read_bytes(), 'diagnostics alter stock object')
    info['whole_object_diagnostic_identical'] = True
    return info


def main():
    with tempfile.TemporaryDirectory(prefix='root-frame-trace-') as tmp:
        work = Path(tmp)
        kept = work / 'kept'
        kept.mkdir()
        info = {'with_slots': inspect(v.GROUP, kept)}
        control = work / 'control'
        control.mkdir()
        text = (v.GROUP / 'group.c').read_text().replace('    s32 residual_slot0; s32 residual_slot1;\n', '', 1)
        (control / 'group.c').write_text(text)
        (control / 'group.json').write_bytes((v.GROUP / 'group.json').read_bytes())
        info['without_slots'] = inspect(control, control)
        v.require(info['with_slots']['root']['memory_areas'] == {'T': 16, 'P': 4, 'M': 10}, 'slot area drift')
        v.require(info['without_slots']['root']['memory_areas'] == {'T': 16, 'P': 4, 'M': 2}, 'control area drift')
        print(json.dumps(info, indent=2))


if __name__ == '__main__':
    main()
