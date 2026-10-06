"""Bounded native-versus-unchanged-host-C proof; no target compiler invoked."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
from inputs import load,BASE
from fixture import Fixture
from native import Machine
from host_adapter import Host
HERE=Path(__file__).resolve().parent

def compare(code,image,host,options,coverage=None,branches=None):
    native,expected=Fixture(image,**options),Fixture(image,**options)
    machine=Machine(code,native);machine.run();host.run(expected)
    if native.trace!=expected.trace:
        for i,(a,b) in enumerate(zip(native.trace,expected.trace)):
            if a!=b:raise AssertionError(('ordered external boundary mismatch',options,i,a,b))
        raise AssertionError(('boundary count mismatch',options,native.trace,expected.trace))
    actual,wanted=native.memory.snapshot(),expected.memory.snapshot()
    assert len(actual)==len(wanted)
    for (a,x),(b,y) in zip(actual,wanted):
        assert a==b
        if x!=y:
            off=next(i for i in range(len(x)) if x[i]!=y[i])
            raise AssertionError(('state mismatch',options,hex(a+off),x[off:off+16].hex(),y[off:off+16].hex()))
    assert native.alloc_count==expected.alloc_count and native.created==expected.created
    if coverage is not None:coverage.update(machine.coverage)
    if branches is not None:branches.update(machine.branches)
    return native

def build_host(output):
    cc=shutil.which('cc')
    if not cc:raise RuntimeError('host C compiler required')
    cmd=[cc,'-std=c99','-O0','-ffp-contract=off','-fPIC','-shared','-Wall','-Wextra',
         '-Werror',str(HERE/'host.c'),'-o',str(output)]
    subprocess.run(cmd,check=True)
    return cmd

def run(root):
    code,image,identities=load(root)
    coverage,branches=set(),set();count=0
    with tempfile.TemporaryDirectory(prefix='af06c-host-',dir=os.environ.get('TMPDIR')) as td:
        output=Path(td)/'host.so';command=build_host(output);host=Host(output)
        def check(**options):
            nonlocal count
            compare(code,image,host,options,coverage,branches);count+=1
        for mode in (0,1,-1,2):
            for index in range(6):
                for player_count in (-1,0,1,3,4,6):
                    for alloc in ((False,False),(False,True),(True,False),(True,True)):
                        check(mode=mode,index=index,count=player_count,alloc=alloc)
        for index in range(6):
            for player_count in (0,3,4,6):
                for alloc in ((False,),(True,)):
                    for old_scene in (-1,0,23,0x12340002):
                        check(shortcut=True,index=index,count=player_count,alloc=alloc,old_scene=old_scene)
        for ring in (0,1,48,49):
            for mode in (0,1):
                for enabled in (0,1,-1):
                    for scale in (-2.,0.,.25,.49999997,.5,.50000006,.99999994,1.,1.00000012,5.):
                        check(mode=mode,count=4,ring=ring,enabled=enabled,scale=scale,sound=1)
    return {'status':'BOUNDED HOST/NATIVE SEMANTIC AGREEMENT; NOT A TARGET MATCH',
            'base':BASE,'targets':identities,'fixtures':count,'covered_instructions':len(coverage),
            'executed_branch_outcomes':len(branches),'target_compiler_invocations':0,
            'host_command':command[:-1]+['<temporary-host-library>'],
            'packet_sha256':{name:hashlib.sha256((HERE/name).read_bytes()).hexdigest() for name in ('candidate.c','host.c','host_adapter.py','native.py','fixture.py','inputs.py','verify.py','check_layout.c')}}

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--reference-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=HERE/'semantics.json');args=parser.parse_args()
    result=run(args.reference_root);args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
