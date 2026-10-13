#!/usr/bin/env python3
"""Optional stock UGEN listing identity; raw outputs stay in temporary storage."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
s=importlib.util.spec_from_file_location('radial_verify',HERE/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)

def inspect():
    with tempfile.TemporaryDirectory(prefix='radial-stock-trace-') as t:
        d=Path(t);(d/'candidate.c').write_bytes((HERE/'candidate.c').read_bytes())
        (d/'group.json').write_text(json.dumps(dict(flags=v.FLAGS,files=['candidate.c'],keep=[v.NAME])))
        plain,traced=d/'plain.o',d/'traced.o';v.score.compile_group(d,plain)
        original=v.score._run;result={}
        def run(command,**kw):
            command=list(command)
            if Path(command[0]).name=='ugen':command+=['-e','trees','-l','listing']
            out=original(command,**kw)
            if Path(command[0]).name=='as1' and out.returncode==0:
                work=Path(kw['cwd']);listing=(work/'listing').read_text();trees=(work/'trees').read_text().split('Tree dump after Build',1)[1].split('Tree dump',1)[0]
                result['frame']=int(re.search(r'\.frame\s+\$sp, (\d+),',listing)[1])
                result['memory_areas']={a:int(b) for a,b in re.findall(r'\bdef mtype=(\w+) length=(\d+)',trees)}
            return out
        try:v.score._run=run;v.score.compile_group(d,traced)
        finally:v.score._run=original
        v.require(plain.read_bytes()==traced.read_bytes(),'stock diagnostic identity')
        single=d/'single.o';v.score.compile_single(d/'candidate.c',v.FLAGS,single)
        a,_,_=v.elf(single);b,_,_=v.elf(plain)
        v.require(all(a[n][2]==b[n][2] for n in ('.text','.rodata')),'group/single code-data identity')
        result.update(whole_object_stock_diagnostic_identity=True,group_vs_single_code_data_identity=True,group_vs_single_whole_object_identity=single.read_bytes()==plain.read_bytes())
        return result
if __name__=='__main__':print(json.dumps(inspect(),sort_keys=True,indent=2))
