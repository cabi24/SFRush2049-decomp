#!/usr/bin/env python3
"""Optional stock-stage diagnostic replay. Emits metadata, never raw listings."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('radius_effect_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def inspect(source,directory):
    (directory/'group.c').write_text(source)
    (directory/'group.json').write_bytes((HERE/'group.json').read_bytes())
    ordinary,traced=directory/'ordinary.o',directory/'traced.o'
    v.score.compile_group(directory,ordinary)
    original=v.score._run
    metadata={}
    def run(command,**kwargs):
        command=list(command);stage=Path(command[0]).name
        if stage=='ugen':command+=['-e','trees','-l','listing']
        result=original(command,**kwargs)
        if stage=='as1' and result.returncode==0:
            work=Path(kwargs['cwd']);listing=(work/'listing').read_text();trees=(work/'trees').read_text()
            first=trees.split('Tree dump after Build',1)[1].split('Tree dump',1)[0]
            metadata.update(memory_areas={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',first)},
                            frame=int(re.search(r'\.frame\s+\$sp, (\d+),',listing)[1]),
                            direct_count_loads=len(re.findall(r'\blh\s+\$\d+, D_801543CA\b',listing)),
                            count_address_materializations=len(re.findall(r'\bla\s+\$\d+, D_801543CA\b',listing)))
        return result
    try:
        v.score._run=run;v.score.compile_group(directory,traced)
    finally:v.score._run=original
    v.require(ordinary.read_bytes()==traced.read_bytes(),'stock diagnostics changed object')
    metadata['whole_object_identical']=True
    return metadata


def main():
    results={}
    with tempfile.TemporaryDirectory(prefix='radius-effect-stock-trace-') as temp:
        for label,source,size,differing,extra in v.forms((HERE/'group.c').read_text()):
            directory=Path(temp)/label;directory.mkdir()
            results[label]=inspect(source,directory)
    for result in results.values():
        v.require(result['frame']==168 and result['memory_areas']=={'M':64,'T':16,'P':20},'stock native frame areas')
    v.require(results['plain_count']['direct_count_loads']==3 and results['plain_count']['count_address_materializations']==0,'ordinary count PRE read pattern')
    for label in ('diagnostic_volatile_count','diagnostic_old_homes'):
        v.require(results[label]['direct_count_loads']==0 and results[label]['count_address_materializations']==2,'diagnostic count barrier pattern')
    print(json.dumps(results,indent=2,sort_keys=True))


if __name__=='__main__':main()
