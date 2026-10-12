#!/usr/bin/env python3
"""Optional stock-stage frame/scheduling metadata; no assembly is retained."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('c140_trace_verify', HERE / 'verify.py')
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
        elif stage == 'as1':
            command.insert(1, '-R')
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
            location = None
            locations = {}
            for line in function.splitlines():
                match = re.match(r'\s*\.loc\s+\d+\s+(\d+)', line)
                if match:
                    location = int(match[1])
                if re.search(r'^\s*li\.s\s+.*-9\.000', line):
                    locations['negative_ninety'] = location
                if re.search(r'^\s*la\s+.*D_8002EB94', line):
                    locations['clock_address'] = location
                if re.search(r'^\s*lb\s+.*D_803B3414', line):
                    locations['else_flag'] = location
            v.require(set(locations) == {'negative_ninety', 'clock_address', 'else_flag'}, 'source-line trace missing')
            metadata['source_lines'] = locations
            metadata['as1_scheduler_pick_records'] = len(re.findall(r'^Picking node ', result.stdout + result.stderr, re.M))
            v.require(metadata['as1_scheduler_pick_records'] > 0, 'stock scheduler trace missing')
        return result
    try:
        v.score._run = traced
        v.score.compile_group(group, diagnostic)
    finally:
        v.score._run = original
    v.require(ordinary.read_bytes() == diagnostic.read_bytes(), 'stock diagnostics changed object')
    metadata['whole_object_identical'] = True
    metadata['root_differing_words'] = v.score.compare(ordinary, v.NEW, show=0).differing
    return metadata


def main():
    controls = v.controls((v.GROUP / 'group.c').read_text())
    results = {}
    with v.targets(), tempfile.TemporaryDirectory(prefix='c140-stock-trace-') as tmp:
        temporary = Path(tmp)
        for label, source in [('matching', (v.GROUP / 'group.c').read_text()),
                              ('ordinary_lines', controls['ordinary_lines']),
                              ('without_carriers', controls['without_carriers'])]:
            folder = temporary / label
            folder.mkdir()
            (folder / 'group.c').write_text(source)
            (folder / 'group.json').write_bytes((v.GROUP / 'group.json').read_bytes())
            results[label] = inspect(folder, folder)
        v.require(results['matching']['memory_areas'] == {'T': 16, 'P': 4, 'M': 20}, 'matching area drift')
        v.require(results['without_carriers']['memory_areas'] == {'T': 16, 'P': 4, 'M': 12}, 'carrier control drift')
        v.require(len(set(results['matching']['source_lines'].values())) == 1, 'matching line tie missing')
        v.require(len(set(results['ordinary_lines']['source_lines'].values())) > 1, 'ordinary line control changed')
    print(json.dumps(results, indent=2))


if __name__ == '__main__':
    main()
