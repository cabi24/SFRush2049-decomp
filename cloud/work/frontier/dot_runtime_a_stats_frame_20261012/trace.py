#!/usr/bin/env python3
"""Stock diagnostic-on/off proof; publish metadata rather than raw dumps."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('stats_trace_verify',HERE/'verify.py')
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
                if name not in v.NEW:continue
                first=block.split('Tree dump',1)[0]
                function=listing.split('\t.ent\t'+name,1)[1].split('\t.end\t'+name,1)[0]
                info[name]=dict(memory_areas={kind:int(size) for kind,size in re.findall(r'\bdef mtype=(\w+) length=(\d+)',first)},
                    external=int(re.search(r'\bent .* external=(\d+)',first)[1]),
                    frame=int(re.search(r'\.frame\s+\$sp, (\d+),',function)[1]),
                    text_base=int(re.search(r'\baddu\s+\$(?:17|18), \$sp, (\d+)',function)[1]))
        return result
    try:v.score._run=traced;v.score.compile_group(group,diagnostic)
    finally:v.score._run=run
    v.require(ordinary.read_bytes()==diagnostic.read_bytes(),'diagnostics changed object')
    return dict(procedures=info,whole_object_diagnostic_identical=True)


def main():
    results={};source=(v.GROUP/'group.c').read_text()
    with tempfile.TemporaryDirectory(prefix='stats-stock-trace-') as temp:
        work=Path(temp)
        for name,text in [('matching',source),*v.controls(source).items()]:
            group=work/name;group.mkdir();(group/'group.c').write_text(text)
            (group/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            results[name]=inspect(group,group)
    for name,frame,base,memory in zip(v.NEW,[112,104,112],[52,44,52],[76,72,80]):
        current=results['matching']['procedures'][name];old=results['baseline']['procedures'][name]
        v.require(current['frame']==frame and current['text_base']==base,'matching native stack layout')
        v.require(current['memory_areas']==dict(T=20,P=4,M=memory),'matching memory areas')
        v.require(old['frame']==frame+8 and old['text_base']==base,'baseline stack layout')
        v.require(old['memory_areas']==dict(T=20,P=4,M=memory+8),'exact memory delta')
        v.require(current['external']==old['external']==0,'private visibility')
    print(json.dumps(results,indent=2,sort_keys=True))


if __name__=='__main__':main()
