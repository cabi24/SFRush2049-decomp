#!/usr/bin/env python3
"""Stock listing/tree diagnostics; retain only metadata, never native dumps."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('stats_selection_trace_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def inspect(group,work):
    ordinary,diagnostic=work/'ordinary.o',work/'diagnostic.o'
    v.score.compile_group(group,ordinary);run=v.score._run;info={}
    def traced(command,**kwargs):
        command=list(command);stage=Path(command[0]).name
        if stage=='ugen':command+=['-e','trees','-l','listing']
        result=run(command,**kwargs)
        if stage=='as1' and result.returncode==0:
            cwd=Path(kwargs['cwd']);listing=(cwd/'listing').read_text()
            names=re.findall(r'^\s*\.ent\s+(\w+)',listing,re.M)
            blocks=(cwd/'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names)==len(blocks),'diagnostic procedure count')
            for name,block in zip(names,blocks):
                if name!=v.NEW:continue
                first=block.split('Tree dump',1)[0]
                function=listing.split('\t.ent\t'+name,1)[1].split('\t.end\t'+name,1)[0]
                info[name]=dict(memory_areas={kind:int(size) for kind,size in re.findall(r'\bdef mtype=(\w+) length=(\d+)',first)},
                    external=int(re.search(r'\bent .* external=(\d+)',first)[1]),
                    frame=int(re.search(r'\.frame\s+\$sp, (\d+),',function)[1]))
        return result
    try:v.score._run=traced;v.score.compile_group(group,diagnostic)
    finally:v.score._run=run
    v.require(ordinary.read_bytes()==diagnostic.read_bytes(),'diagnostics changed entire object')
    return dict(procedures=info,whole_object_diagnostic_identical=True)


def main():
    results={};source=(v.GROUP/'group.c').read_text()
    with tempfile.TemporaryDirectory(prefix='stats-selection-stock-trace-') as temp:
        work=Path(temp)
        for name,text in [('research',source),*v.controls(source).items()]:
            group=work/name;group.mkdir();(group/'group.c').write_text(text)
            (group/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            results[name]=inspect(group,group)
    v.require(results['research']['procedures'][v.NEW]['frame']==192,'research frame changed')
    print(json.dumps(results,indent=2,sort_keys=True))


if __name__=='__main__':main()
