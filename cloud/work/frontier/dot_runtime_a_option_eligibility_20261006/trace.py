#!/usr/bin/env python3
"""Optional stock-stage diagnostics; emit metadata only, never raw listings."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('position_callback_verify', HERE / 'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


def inspect(group, work):
    ordinary, diagnostic = work / 'ordinary.o', work / 'diagnostic.o'
    v.score.compile_group(group, ordinary)
    run, info = v.score._run, {}
    def traced(command, **kwargs):
        command = list(command)
        stage = Path(command[0]).name
        if stage == 'ugen':
            command += ['-e', 'trees', '-l', 'listing']
        result = run(command, **kwargs)
        if stage == 'as1' and result.returncode == 0:
            cwd = Path(kwargs['cwd'])
            listing = (cwd / 'listing').read_text()
            names = re.findall(r'^\s*\.ent\s+(\w+)', listing, re.M)
            blocks = (cwd / 'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names) == len(blocks), 'stage procedure count')
            for name, block in zip(names, blocks):
                if name == 'func_803ADDA8':
                    block = block.split('Tree dump', 1)[0]
                    info['memory_areas'] = {kind: int(size) for kind, size in re.findall(r'\bdef mtype=(\w+) length=(\d+)', block)}
            body = listing.split('func_803ADDA8:', 1)[1].split('.end\tfunc_803ADDA8', 1)[0]
            info['frame_bytes'] = int(re.search(r'\.frame\s+\$sp,\s*(\d+)', body)[1])
        return result
    try:
        v.score._run = traced
        v.score.compile_group(group, diagnostic)
    finally:
        v.score._run = run
    v.require(ordinary.read_bytes() == diagnostic.read_bytes(), 'stock diagnostics changed object')
    info['whole_object_diagnostic_identical'] = True
    return info


def main():
    with tempfile.TemporaryDirectory(prefix='dda8-stock-trace-') as tmp:
        root = Path(tmp)
        result = {'candidate': inspect(HERE, root)}
        for name, (source, _) in v.controls((HERE / 'group.c').read_text()).items():
            work = root / name
            work.mkdir()
            (work / 'group.c').write_text(source)
            (work / 'group.json').write_bytes((HERE / 'group.json').read_bytes())
            result[name] = inspect(work, work)
        v.require(result['candidate']['memory_areas'] == {'T': 20, 'P': 4, 'M': 76}, 'candidate areas changed')
        v.require(result['candidate']['frame_bytes'] == 112, 'candidate frame changed')
        for name in ('baseline', 'position_text48'):
            v.require(result[name]['memory_areas'] == {'T': 20, 'P': 4, 'M': 92}, 'control areas changed')
            v.require(result[name]['frame_bytes'] == 128, 'control frame changed')
        print(json.dumps(result, indent=2, sort_keys=True))


if __name__ == '__main__':
    main()
