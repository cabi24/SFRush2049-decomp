"""Stock diagnostics only; entire objects must agree with diagnostics disabled."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('reset_trace_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def replay():
    result={}
    with v.targets(),tempfile.TemporaryDirectory(prefix='reset-trace-') as tmp:
        for label,source in [('final',(v.GROUP/'group.c').read_text())]+list(v.controls().items()):
            work=Path(tmp)/label;work.mkdir();(work/'group.c').write_text(source);(work/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            a,b=work/'stock.o',work/'traced.o';v.score.compile_group(work,a)
            original=v.score._run;facts={}
            def run(command,**kwargs):
                command=list(command);stage=Path(command[0]).name
                if stage=='ugen':command+=['-e','trees','-l','listing']
                process=original(command,**kwargs)
                if stage=='as1' and process.returncode==0:
                    cwd=Path(kwargs['cwd']);listing=(cwd/'listing').read_text();names=re.findall(r'^\s*\.ent\s+(\w+)',listing,re.M)
                    blocks=(cwd/'trees').read_text().split('Tree dump after Build')[1:]
                    v.require(len(names)==len(blocks),'procedure count')
                    block=blocks[names.index(v.NEW)].split('Tree dump',1)[0]
                    facts['memory_areas']={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',block)}
                    part=listing.split(v.NEW+':',1)[1].split('\t.end\t'+v.NEW,1)[0]
                    facts['frame_bytes']=int(re.search(r'\.frame\s+\$sp,\s*(\d+)',part)[1])
                return process
            try:v.score._run=run;v.score.compile_group(work,b)
            finally:v.score._run=original
            v.require(a.read_bytes()==b.read_bytes(),'stock trace object drift')
            facts.update(whole_object_identical=True,differing_words=v.score.compare(a,v.NEW,show=0).differing)
            result[label]=facts
    return result

if __name__=='__main__':print(json.dumps(replay(),indent=2))
