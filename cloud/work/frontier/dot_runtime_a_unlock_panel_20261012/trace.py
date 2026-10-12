"""Stock input-bound frame diagnostics; native listings remain temporary."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('unlock_verify_trace',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def inspect(group, work):
    ordinary,traced=work/'ordinary.o',work/'traced.o'
    v.score.compile_group(group,ordinary)
    original=v.score._run;found={}
    def run(command,**kwargs):
        command=list(command);stage=Path(command[0]).name
        if stage=='ugen':command+=['-e','trees','-l','listing']
        result=original(command,**kwargs)
        if stage=='as1' and result.returncode==0:
            cwd=Path(kwargs['cwd']);listing=(cwd/'listing').read_text()
            names=re.findall(r'^\s*\.ent\s+(\w+)',listing,re.M)
            blocks=(cwd/'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names)==len(blocks),'diagnostic procedure counts disagree')
            block=blocks[names.index(v.NEW[0])].split('Tree dump',1)[0]
            found['memory_areas']={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',block)}
            found['external']=int(re.search(r'\bent .* external=(\d+)',block)[1])
            text=listing.split(v.NEW[0]+':',1)[1].split('\t.end\t'+v.NEW[0],1)[0]
            found['frame_bytes']=int(re.search(r'\.frame\s+\$sp,\s*(\d+)',text)[1])
            found['width_initial_register']=int(re.search(r'li\s+\$(\d+), 20\b',text)[1])
        return result
    try:
        v.score._run=run;v.score.compile_group(group,traced)
    finally:
        v.score._run=original
    v.require(ordinary.read_bytes()==traced.read_bytes(),'diagnostic changed entire object')
    result=v.score.compare(ordinary,v.NEW[0],show=0)
    found.update(differing_words=result.differing,total_words=result.total,complete_object_diagnostic_identical=True)
    return found


def replay():
    source=(v.GROUP/'group.c').read_text();controls=v.controls(source);output={}
    with tempfile.TemporaryDirectory(prefix='unlock-trace-') as temp,v.target_context():
        for label,text in [('matched',source)]+[(n,controls[n]) for n in ('baseline','nested_scan','old_capacity','old_order')]:
            work=Path(temp)/label;work.mkdir()
            (work/'group.json').write_bytes((v.GROUP/'group.json').read_bytes())
            (work/'group.c').write_text(text)
            output[label]=inspect(work,work)
    return output


if __name__=='__main__':
    print(json.dumps(replay(),indent=2,sort_keys=True))
