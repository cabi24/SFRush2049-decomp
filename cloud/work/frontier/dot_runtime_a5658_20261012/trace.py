#!/usr/bin/env python3
"""Optional stock-stage metadata replay. No raw compiler dump is persisted."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('a5658_trace_verify', HERE / 'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


def inspect(group, temporary):
    ordinary, diagnostic = temporary / 'ordinary.o', temporary / 'diagnostic.o'
    v.score.compile_group(group, ordinary)
    original = v.score._run
    metadata = {}
    def traced(command, **kwargs):
        command = list(command)
        stage = Path(command[0]).name
        if stage == 'ugen':
            command += ['-e', 'trees', '-l', 'listing']
        result = original(command, **kwargs)
        if stage == 'as1' and result.returncode == 0:
            work = Path(kwargs['cwd'])
            listing = (work / 'listing').read_text()
            names = re.findall(r'^\s*\.ent\s+(\w+)', listing, re.M)
            blocks = (work / 'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names) == len(blocks), 'UGEN procedure metadata mismatch')
            for name, block in zip(names, blocks):
                if name == v.NEW:
                    block = block.split('Tree dump', 1)[0]
                    metadata['memory_areas'] = {kind: int(size) for kind, size in re.findall(r'\bdef mtype=(\w+) length=(\d+)', block)}
            function = listing.split('\t.ent\t' + v.NEW, 1)[1].split('\t.end\t' + v.NEW, 1)[0]
            metadata['frame'] = int(re.search(r'\.frame\s+\$sp, (\d+),', function)[1])
            metadata['s8_address'] = re.search(r'\bla\s+\$30, (D_\w+)', function)[1]
            loc = None
            source_lines = {}
            for line in function.splitlines():
                match = re.match(r'\s*\.loc\s+\d+\s+(\d+)', line)
                if match:
                    loc = int(match[1])
                if re.search(r'\bmove\s+\$17, \$0', line):
                    source_lines['index_init'] = loc
                if re.search(r'\bla\s+\$30, D_', line):
                    source_lines['address_setup'] = loc
            v.require(set(source_lines) == {'index_init', 'address_setup'}, 'missing source-line locations')
            metadata['initializer_and_address_same_line'] = len(set(source_lines.values())) == 1
        return result
    try:
        v.score._run = traced
        v.score.compile_group(group, diagnostic)
    finally:
        v.score._run = original
    v.require(ordinary.read_bytes() == diagnostic.read_bytes(), 'stock diagnostic changed object')
    metadata['whole_object_identical'] = True
    metadata['differing_words'] = v.score.compare(ordinary, v.NEW, show=0).differing
    return metadata


def main():
    source = (v.GROUP / 'group.c').read_text()
    controls = v.controls(source)
    results = {}
    with v.target_context(), tempfile.TemporaryDirectory(prefix='a5658-stock-trace-') as tmp:
        for label, body in [('matching', source), *controls.items()]:
            folder = Path(tmp) / label
            folder.mkdir()
            (folder / 'group.c').write_text(body)
            (folder / 'group.json').write_bytes((v.GROUP / 'group.json').read_bytes())
            results[label] = inspect(folder, folder)
    v.require(results['matching']['memory_areas'] == {'T':16,'P':4,'M':164}, 'matching frame areas')
    v.require(results['without_homes']['memory_areas'] == {'T':16,'P':4,'M':140}, 'exact home residual')
    v.require(results['matching']['frame'] == 192 and results['without_homes']['frame'] == 168, 'frame arithmetic')
    v.require(results['matching']['s8_address'] == 'D_803BAD90' and results['baseline']['s8_address'] == 'D_803B9A84', 'address web identity')
    v.require(results['matching']['initializer_and_address_same_line'], 'for-loop header lost source-line identity')
    v.require(not results['original_loop']['initializer_and_address_same_line'], 'loop negative control')
    print(json.dumps(results, indent=2, sort_keys=True))


if __name__ == '__main__':
    main()
