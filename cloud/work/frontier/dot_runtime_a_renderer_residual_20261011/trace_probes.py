#!/usr/bin/env python3
"""Optional bounded stock-stage probes; prints metadata, never raw compiler listings."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('renderer_residual_verify', HERE / 'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


def inspect(group, work):
    ordinary, traced = work / 'ordinary.o', work / 'traced.o'
    v.score.compile_group(group, ordinary)
    original_run = v.score._run
    metadata = {}

    def run(command, **kwargs):
        command = list(command)
        stage = Path(command[0]).name
        if stage == 'umerge':
            command.insert(1, '-v')
        elif stage == 'ugen':
            command += ['-e', 'trees', '-l', 'listing']
        result = original_run(command, **kwargs)
        if stage == 'umerge':
            metadata['inline_log'] = (result.stdout + result.stderr).strip().splitlines()
        elif stage == 'as1' and result.returncode == 0:
            cwd = Path(kwargs['cwd'])
            names = re.findall(r'^\s*\.ent\s+(\w+)', (cwd / 'listing').read_text(), re.M)
            blocks = (cwd / 'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names) == len(blocks), 'stage procedure count mismatch')
            for name, block in zip(names, blocks):
                if name == 'func_803A7180':
                    block = block.split('Tree dump', 1)[0]
                    metadata['memory_areas'] = {kind: int(size) for kind, size in
                                               re.findall(r'\bdef mtype=(\w+) length=(\d+)', block)}
                    metadata['external'] = int(re.search(r'\bent .* external=(\d+)', block)[1])
        return result

    try:
        v.score._run = run
        v.score.compile_group(group, traced)
    finally:
        v.score._run = original_run
    v.require(ordinary.read_bytes() == traced.read_bytes(), 'diagnostics changed the object')
    start = v.score.symbols(ordinary)['func_803A7180']
    words = v.score.text_words(ordinary)
    metadata['frame'] = 65536 - (words[start // 4] & 65535)
    comparison = v.score.compare(ordinary, 'func_803A7180', show=0)
    metadata['differing_words'] = comparison.differing
    metadata['complete_words'] = comparison.total
    metadata['whole_object_diagnostic_identical'] = True
    return metadata


def main():
    source = (v.GROUP / 'group.c').read_text()
    a = source.index('void func_803A7180(void)\n')
    b = source.index('\nvoid func_803A7620(void)', a)
    body = source[a:b]
    nested = body.replace('    s16 x;\n', '')
    nested = re.sub(r'(\s*)x=(func_800B71A0\([^\n]*\));\n(\s*)func_800B71D4\(x,',
                    r'\1func_800B71D4(\2,', nested)
    nested = nested.replace('        x=func_800B71A0(text,23);\n        row_y=y;\n'
                            '        func_800B71D4(x,row_y,text);',
                            '        row_y=y;\n        func_800B71D4(func_800B71A0(text,23),row_y,text);')
    derived = body.replace('    s32 y;\n', '').replace('    y=80;\n', '').replace(
        'row++,y+=12', 'row++').replace('row_y=y;', 'row_y=80+row*12;')
    direct = derived.replace('    s16 row_y;\n', '').replace('        row_y=80+row*12;\n', '').replace(
        'row_y', '(s16)(80+row*12)')
    variants = {'baseline': body, 'nested_x': nested, 'derive_y': derived, 'derive_y_direct': direct}
    data, base, _table = v.authenticate()
    results = {}
    with tempfile.TemporaryDirectory(prefix='renderer-frame-probes-') as temp, v.target_scope(
            v.owndata.ImageData.from_image(data, base)):
        for name, variant in variants.items():
            path = Path(temp) / name
            path.mkdir()
            (path / 'group.c').write_text(source[:a] + variant + source[b:])
            (path / 'group.json').write_bytes((v.GROUP / 'group.json').read_bytes())
            results[name] = inspect(path, path)
    print(json.dumps(results, indent=2))


if __name__ == '__main__':
    main()
