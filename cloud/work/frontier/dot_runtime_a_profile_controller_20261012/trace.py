#!/usr/bin/env python3
"""Stock UGEN diagnostics with whole-object equality; no oracle acceptance."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('controller_trace_verify',HERE/'verify.py')
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
    source=(v.GROUP/'group.c').read_text();results={}
    # Diagnostic only: no residual scalar is retained or counted as a match.
    residual=source.replace('    Profile **owner;\n    while','    s32 residual0,residual1,residual2,residual3,residual4,residual5,residual6,residual7;\n    Profile **owner;\n    while',1)
    with v.target_context(),tempfile.TemporaryDirectory(prefix='controller-stock-trace-') as temp:
        work=Path(temp)
        for label,text in [('research',source),('diagnostic_32_byte_residual',residual)]:
            group=work/label;group.mkdir();(group/'group.c').write_text(text);(group/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            results[label]=inspect(group,group)
            obj=group/'ordinary.o';q=v.score.compare(obj,v.NEW,show=0)
            results[label]['differing_words']=q.differing
            for n in v.PRESERVED:v.require(v.score.compare(obj,n,show=0).accepted(),'diagnostic preservation')
    v.require(results['research']['procedures'][v.NEW]['frame']==56,'research frame changed')
    v.require(results['research']['differing_words']==31,'research residual changed')
    v.require(results['diagnostic_32_byte_residual']['procedures'][v.NEW]['frame']==88,'diagnostic frame changed')
    v.require(results['diagnostic_32_byte_residual']['differing_words']==5,'diagnostic five index words changed')
    print(json.dumps(results,indent=2,sort_keys=True))
if __name__=='__main__':main()
