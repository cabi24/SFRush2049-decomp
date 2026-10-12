#!/usr/bin/env python3
"""Optional W5D diagnostic replay; every trace object must equal stock exactly.

Pass a locally built W5D diagnostic uopt with --uopt. The acceptance verifier
never uses it. Dumps and object bytes remain in a temporary directory.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tempfile

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('d300_trace_proof',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def replay(traced):
    v.inputs();v.require(v.compilers.identify_dir(v.score.IDO)=='ido5.3','stock IDO5.3 required')
    source=(v.GROUP/'group.c').read_text();variants={'final':source}
    variants.update({n:s for n,(s,count) in v.causal_sources(source).items() if n!='opposite_pair_order'})
    facts={}
    common=['-mips2','-EB','-g0','-O3']
    clean={k:x for k,x in os.environ.items() if not k.startswith(('CDX_','W5D_'))}
    with tempfile.TemporaryDirectory(prefix='d300-fidelity-') as temporary:
        for tag,text in variants.items():
            work=Path(temporary)/tag;work.mkdir();(work/'group.c').write_text(text);(work/'keep.txt').write_text('func_803A1EAC\n')
            steps=[['cc','-j','-g0','-O3','-mips2','-G','0','-non_shared','group.c'],
                   ['uld','-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','group.u','-ko','linked'],
                   ['usplit','-mips2','-o','split','-t','st','linked'],
                   ['umerge','-Olimit','5000',*common,'split','-o','merged','-t','st']]
            for step in steps:subprocess.run([v.score.ido(step[0]),*step[1:]],cwd=work,env=clean,check=True,capture_output=True)
            for mode,tool in [('stock',v.score.ido('uopt')),('trace',str(traced))]:
                env=dict(clean)
                if mode=='trace':env.update(W5D_LEVEL='0',W5D_PROC=v.NEW,W5D_OUT=str(work/'w5d'),CDX_LOG='1',CDX_OUT=str(work/'cdx'))
                commands=[[tool,'-G','0','-Olimit','5000',*common,'merged',mode+'.opt','-t','st',mode+'.log'],
                          [v.score.ido('ugen'),'-G','0',*common,mode+'.opt','-o',mode+'.gen','-t','st','-temp',mode+'.tmp'],
                          [v.score.ido('as1'),'-elf','-G','0','-p0',*common,'-r4300_mul','-Olimit','5000',mode+'.gen','-o',mode+'.o','-t','st']]
                for command in commands:subprocess.run(command,cwd=work,env=env,check=True,capture_output=True)
            v.require((work/'stock.o').read_bytes()==(work/'trace.o').read_bytes(),'trace compiler changed complete object: '+tag)
            log=(work/'w5d').read_text();v.require('proc='+v.NEW in log,'selected trace absent')
            facts[tag]={'whole_object_identical':True,'shape':v.body_shape(work/'stock.o',v.NEW),'selected_trace_lines':sum('proc='+v.NEW in line for line in log.splitlines())}
    return {'diagnostic_uopt_sha256':hashlib.sha256(traced.read_bytes()).hexdigest(),'facts':facts,'acceptance_compiler':'unchanged stock IDO5.3'}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--uopt',required=True,type=Path);args=parser.parse_args()
    print(json.dumps(replay(args.uopt.resolve()),indent=2))
