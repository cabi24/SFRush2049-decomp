"""Three hypothesis-bound stock diagnostics; raw listings stay temporary."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('capacity_verify',HERE/'verify.py')
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
            cwd=Path(kwargs['cwd'])
            names=re.findall(r'^\s*\.ent\s+(\w+)',(cwd/'listing').read_text(),re.M)
            blocks=(cwd/'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names)==len(blocks),'diagnostic procedure counts disagree')
            block=blocks[names.index(v.NAME)].split('Tree dump',1)[0]
            found['memory_areas']={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',block)}
            found['external']=int(re.search(r'\bent .* external=(\d+)',block)[1])
        return result
    try:
        v.score._run=run;v.score.compile_group(group,traced)
    finally:
        v.score._run=original
    v.require(ordinary.read_bytes()==traced.read_bytes(),'diagnostic flags changed complete object')
    start=v.score.symbols(ordinary)[v.NAME]
    found['frame_bytes']=65536-(v.score.text_words(ordinary)[start//4]&65535)
    result=v.score.compare(ordinary,v.NAME,show=0)
    found.update(differing_words=result.differing,total_words=result.total,complete_object_diagnostic_identical=True)
    return found


def replay():
    data,base,_=v.authenticate();output={}
    with tempfile.TemporaryDirectory(prefix='capacity-trace-') as temp,v.targets(v.owndata.ImageData.from_image(data,base)):
        for length in (50,36,32):
            work=Path(temp)/('text%d'%length);work.mkdir()
            for file in ('eligibility.c','group.json'):(work/file).write_bytes((v.GROUP/file).read_bytes())
            (work/'renderer.c').write_text((v.GROUP/'renderer.c').read_text().replace('char text[36];','char text[%d];'%length))
            output[str(length)]=inspect(work,work)
    return output


if __name__=='__main__':
    print(json.dumps(replay(),indent=2))
