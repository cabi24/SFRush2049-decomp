"""Stock-only phase observation; no forced compiler or altered acceptance."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('callback_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def trace():
    with tempfile.TemporaryDirectory(prefix='callback-stock-trace-') as name,v.population('ovl_b'):
        work=Path(name);(work/'group.c').write_bytes((HERE/'candidate.c').read_bytes())
        (work/'group.json').write_text(json.dumps(dict(files=['group.c'],flags=v.FLAGS,keep=[v.NAME],members=[v.NAME],claims=[],context=[])))
        ordinary=work/'ordinary.o';v.score.compile_group(work,ordinary)
        original=v.score._run;facts={}
        def stage(command,**kwargs):
            command=list(command);tool=Path(command[0]).name
            if tool=='ugen':command+=['-e','trees','-l','listing']
            result=original(command,**kwargs)
            if tool=='as1' and result.returncode==0:
                directory=Path(kwargs['cwd']);listing=(directory/'listing').read_text();trees=(directory/'trees').read_text()
                first=trees.split('Tree dump after Build',1)[1].split('Tree dump',1)[0]
                facts['areas']={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',first)}
                facts['frame']=int(re.search(r'\.frame\s+\$sp, (\d+),',listing)[1])
                facts['UGEN_parameter_store_uses_t0']='sh\t$8, 8($3)' in listing
                facts['UGEN_index_scale_uses_t0']='mul\t$12, $8, 64' in listing
                facts['UGEN_argument_store_uses_t0']='sw\t$8, 16($sp)' in listing
            return result
        try:v.score._run=stage;v.score.compile_group(work,work/'diagnostic.o')
        finally:v.score._run=original
        v.require(ordinary.read_bytes()==(work/'diagnostic.o').read_bytes(),'diagnostics changed complete stock object')
        facts['whole_object_identical']=True
        proof,words=v.gnu(ordinary,work)
        v.require(len(proof['differing_offsets'])==13,'stock group differs from standalone')
        v.require(facts['frame']==64 and all(facts[k] for k in facts if k.startswith('UGEN_')),'phase observations changed')
        v.require(words[0x60//4]>>16&31==14,'as1 no longer substitutes t6 in callback store')
        v.require(words[0x64//4]>>16&31==14,'as1 no longer substitutes t6 in control scale')
        facts['post_as1_parameter_carrier']='t6 in callback field/control scale/argument setup, native uses t0'
        facts['full_word_differences']=proof['differing_offsets']
        return facts
if __name__=='__main__':print(json.dumps(trace(),indent=2,sort_keys=True))
