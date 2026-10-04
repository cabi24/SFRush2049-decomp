#!/usr/bin/env python3
"""Reproduce the independent reviewer's three actual-source defect controls."""
from pathlib import Path
import hashlib
import json
import resource
import subprocess
import tempfile
P=Path(__file__).resolve().parent
MUTATIONS=[
    ('drop_tag_wrap','(u32)(index * sizeof(SequenceContext))','(index * sizeof(SequenceContext))'),
    ('wrong_guard','options.channelCount = 0;','options.channelCount = 1;'),
    ('wrong_service_mode','else if (state->flags & 0x40) func_80019194(0,state->duration,state->identifier,3);','else if (state->flags & 0x40) func_80019194(0,state->duration,state->identifier,1);')]
def no_core():resource.setrlimit(resource.RLIMIT_CORE,(0,0))
def run():
    original=(P/'nonmatch/func_80019490.c').read_text()
    digest=hashlib.sha256(original.encode()).hexdigest()
    assert digest=='9e892d39290a16a09691aa45b5c881d00f7ff46a808a26b08fdc95c63e10924f'
    rows=[]
    with tempfile.TemporaryDirectory(prefix='review-sequence-mutations-') as td:
        d=Path(td);(d/'nonmatch').mkdir();(d/'host_behavior.c').write_text((P/'host_behavior.c').read_text())
        for label,old,new in MUTATIONS:
            assert original.count(old)==1
            (d/'nonmatch/func_80019490.c').write_text(original.replace(old,new))
            subprocess.run(['gcc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-O2',str(d/'host_behavior.c'),'-o',str(d/'test')],check=True,capture_output=True)
            r=subprocess.run([str(d/'test')],capture_output=True,preexec_fn=no_core)
            assert r.returncode!=0,label
            rows.append({'mutation':label,'test_rejects':True,'returncode':r.returncode})
    return {'result':'PASS','source_sha256':digest,'controls':rows,'scope':'Independent-review defect injection only, not additional matching search or promoted source.'}
if __name__=='__main__':print(json.dumps(run(),indent=2))
