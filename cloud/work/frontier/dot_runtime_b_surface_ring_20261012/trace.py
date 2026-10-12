#!/usr/bin/env python3
"""Optional instrumented-uopt diagnostic. Forced results are never acceptance."""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import re
import tempfile

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('surface_research_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def run(tracer=None,force=None):
    source=(HERE/'group.c').read_text();metadata={}
    with tempfile.TemporaryDirectory(prefix='surface-stock-trace-') as tmp,v.targets():
        d=Path(tmp);(d/'group.c').write_text(source);(d/'group.json').write_bytes((HERE/'group.json').read_bytes())
        ordinary=d/'ordinary.o';v.score.compile_group(d,ordinary)
        original=v.score._run
        def stage(command,**kwargs):
            command=list(command);name=Path(command[0]).name
            if name=='uopt' and tracer:
                command[0]=str(tracer)
                env=dict(os.environ,CDX_LOG='1',CDX_DETAIL_WEB='all',CDX_PROC='0',CDX_OUT=str(d/'color.log'),W5D_PROC=v.NAME,W5D_OUT=str(d/'pre.log'),W5D_LEVEL='0')
                env.pop('CDX_FORCE',None)
                if force:env['CDX_FORCE']=force
                kwargs['env']=env
            if name=='ugen':command+=['-e','trees','-l','listing']
            result=original(command,**kwargs)
            if name=='as1' and result.returncode==0:
                work=Path(kwargs['cwd']);listing=(work/'listing').read_text();trees=(work/'trees').read_text()
                first=trees.split('Tree dump after Build',1)[1].split('Tree dump',1)[0]
                metadata.update(memory_areas={k:int(n) for k,n in re.findall(r'\bdef mtype=(\w+) length=(\d+)',first)},frame=int(re.search(r'\.frame\s+\$sp, (\d+),',listing)[1]))
            return result
        try:v.score._run=stage;v.score.compile_group(d,d/'diagnostic.o')
        finally:v.score._run=original
        metadata['force']=force
        metadata['whole_object_identical']=ordinary.read_bytes()==(d/'diagnostic.o').read_bytes()
        if not force:v.require(metadata['whole_object_identical'],'diagnostics changed complete stock object')
        result=v.score.compare(d/'diagnostic.o',v.NAME)
        metadata['comparison']=v.asdict(result)
        metadata['gnu']=v.gnu(d/'diagnostic.o',d)
        if tracer:
            color=(d/'color.log').read_text()
            selected=[line for line in color.splitlines() if 'p1dec ' in line and 'web=158 ' in line]
            v.require(selected,'index web disappeared; re-inspect before forcing')
            metadata['index_decisions']=selected
            if force:v.require('forced=-1' in selected[0],'index split was not applied')
        if force:
            v.require(result.differing==7 and metadata['gnu']['function_bytes']==1220,'force-only causal residual')
            v.require(metadata['gnu']['differing_offsets']==[0x310,0x318,0x31c,0x324,0x3a0,0x3e4,0x480],'force-only sites changed')
        else:v.require(result.differing==105,'stock source score')
        return metadata


def main():
    p=argparse.ArgumentParser();p.add_argument('--uopt-trace',type=Path,default=os.environ.get('IDO_UOPT_TRACE'));a=p.parse_args()
    results={'stock_diagnostics':run()}
    if a.uopt_trace:
        results['instrumented_unforced']=run(a.uopt_trace)
        results['forced_index_split_DIAGNOSTIC_ONLY']=run(a.uopt_trace,'p1:w158=s')
    print(json.dumps(results,indent=2,sort_keys=True))


if __name__=='__main__':main()
