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
    source=(v.GROUP/'group.c').read_text()
    data,base,_=v.authenticate()
    result={}
    with tempfile.TemporaryDirectory(prefix='writer-stock-trace-') as temp,v.target_scope(v.owndata.ImageData.from_image(data,base)):
        for capacity in (100,76,72):
            path=Path(temp)/str(capacity);path.mkdir()
            (path/'group.c').write_text(source.replace('char text[76];','char text[%d];'%capacity))
            (path/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            result[str(capacity)]=inspect(path,path)
    v.require(result['100']['frame']==192 and result['76']['frame']==168 and result['72']['frame']==160,'stock frames changed')
    v.require(result['76']['memory_areas']=={'M':124,'T':28,'P':4},'stock area accounting changed')
    print(json.dumps(result,indent=2))


if __name__=='__main__':main()
