"""Stock diagnostic replay; optional color oracle is never matching evidence."""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import re
import tempfile
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('availability_root_trace_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def replay(tracer=None):
    result={}
    with v.targets(),tempfile.TemporaryDirectory(prefix='availability-trace-') as tmp:
        for label,source,recipe,differing,size,frame in v.forms():
            work=Path(tmp)/label;work.mkdir();(work/'group.c').write_text(source);(work/'group.json').write_text(json.dumps(recipe))
            stock,trace=work/'stock.o',work/'trace.o';v.score.compile_group(work,stock);original=v.score._run;facts={}
            def run(command,**kwargs):
                command=list(command);stage=Path(command[0]).name
                if stage=='ugen':command+=['-e','trees','-l','listing']
                process=original(command,**kwargs)
                if stage=='as1' and process.returncode==0:
                    cwd=Path(kwargs['cwd']);listing=(cwd/'listing').read_text();names=re.findall(r'^\s*\.ent\s+(\w+)',listing,re.M)
                    blocks=(cwd/'trees').read_text().split('Tree dump after Build')[1:];v.require(len(names)==len(blocks),'procedure count')
                    block=blocks[names.index(v.NAME)].split('Tree dump',1)[0]
                    facts['memory_areas']={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',block)}
                    part=listing.split(v.NAME+':',1)[1].split('\t.end\t'+v.NAME,1)[0]
                    facts['frame_bytes']=int(re.search(r'\.frame\s+\$sp,\s*(\d+)',part)[1])
                return process
            try:v.score._run=run;v.score.compile_group(work,trace)
            finally:v.score._run=original
            v.require(stock.read_bytes()==trace.read_bytes(),'unforced stock diagnostics drift')
            facts.update(whole_object_identical=True,differing_words=v.score.compare(stock,v.NAME,show=0).differing)
            result[label]=facts
        if tracer:
            log=work/'colors.log';original=v.score._run
            def compile_trace(force=None):
                def run(command,**kwargs):
                    command=list(command)
                    if Path(command[0]).name=='uopt':
                        command[0]=str(tracer);env=dict(os.environ);env.pop('CDX_FORCE',None)
                        env.update(CDX_LOG='1',CDX_PROC='5',CDX_OUT=str(log))
                        if force:env['CDX_FORCE']=force
                        kwargs['env']=env
                    return original(command,**kwargs)
                try:v.score._run=run;v.score.compile_group(work,trace)
                finally:v.score._run=original
            compile_trace();v.require(stock.read_bytes()==trace.read_bytes(),'instrumented unforced object identity')
            records=log.read_text()
            for web,reg in [(46,'v0'),(44,'v1'),(79,'a0')]:v.require(re.search(r'p1color phase=p1 proc=5 web=%d sym=%d color=\d+ reg=%s forced=-2'%(web,web,reg),records),'source web census')
            force='p1:w46=c3,p1:w44=c2,p1:w79=c1';compile_trace(force)
            sc=v.score.compare(trace,v.NAME,show=0);v.require(sc.differing==6 and not(sc.unresolved or sc.unverified or sc.errors or sc.extra_words),'force-only diagnostic')
            proof=v.gnu(trace,work,v.PRESERVED+[v.NAME],6)
            v.require(proof['functions'][v.NAME]['differing_offsets']==[372,376,380,384,388,396],'six store/spill scheduling sites')
            result['color_oracle']={'unforced_stock_identical':True,'force':force,'differing_words':6,'acceptance_evidence':False,
                'tracer_sha256':v.sha(tracer.read_bytes()),'remaining_offsets':proof['functions'][v.NAME]['differing_offsets']}
    return result

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--tracer',type=Path);a=p.parse_args();print(json.dumps(replay(a.tracer),indent=2,sort_keys=True))
