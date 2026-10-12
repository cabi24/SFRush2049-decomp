#!/usr/bin/env python3
"""Stock diagnostic-on/off replay; intermediate assembly stays temporary."""
import importlib.util,json,re,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('aggregate_trace_verify',HERE/'verify.py')
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
            v.require(len(names)==len(blocks),'diagnostic procedure census')
            block=blocks[names.index(v.NEW)].split('Tree dump',1)[0]
            function=listing.split('\t.ent\t'+v.NEW,1)[1].split('\t.end\t'+v.NEW,1)[0]
            info.update(memory_areas={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',block)},
                        external=int(re.search(r'\bent .* external=(\d+)',block)[1]),
                        frame_bytes=int(re.search(r'\.frame\s+\$sp, (\d+),',function)[1]))
        return result
    try:v.score._run=traced;v.score.compile_group(group,diagnostic)
    finally:v.score._run=run
    v.require(ordinary.read_bytes()==diagnostic.read_bytes(),'diagnostic changed whole object')
    with v.target_context():result=v.score.compare(ordinary,v.NEW,show=0)
    info.update(differing_words=result.differing,extra_words=result.extra_words,complete_object_diagnostic_identical=True)
    return info

def main():
    results={};source=(v.GROUP/'group.c').read_text()
    with tempfile.TemporaryDirectory(prefix='aggregate-trace-') as temp:
        work=Path(temp)
        for name,text in [('matching',source),*v.controls(source).items()]:
            group=work/name;group.mkdir();(group/'group.c').write_text(text)
            (group/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            results[name]=inspect(group,group)
    v.require(results['matching']['frame_bytes']==152 and results['matching']['external']==0,'native private frame')
    print(json.dumps(results,sort_keys=True,indent=2))
if __name__=='__main__':main()
