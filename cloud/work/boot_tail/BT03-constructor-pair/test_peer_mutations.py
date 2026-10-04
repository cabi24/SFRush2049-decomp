#!/usr/bin/env python3
"""Reproduce the independent reviewer's four defect-detection controls.

Source review is bound to d21bb30439a7fa4c2b4c826ead17657c5bb85c43.
Only temporary copies are mutated; retained sources are never edited.
"""
from pathlib import Path
import json,os,resource,shutil,subprocess,tempfile
P=Path(__file__).resolve().parent
R=P.parents[3]
mutations=[
('masked_raw_pan','func_8001A270.c','pan = keymap[key].panning - 64;','pan = keymap[index].panning - 64;'),
('reject_category_before_portamento','func_80019F48.c','current = func_80019ED0(note, channel, set);','if (layer->id & 0xC000) continue;\n            current = func_80019ED0(note, channel, set);'),
('root_failure_stops_retry','func_80019F48.c','if (previous != 0xFFFFFFFFU) result = func_8001ECE0(&D_8004BEB8[previous & 255]);','if (previous != 0xFFFFFFFFU) { result = func_8001ECE0(&D_8004BEB8[previous & 255]); if (result == 0xFFFFFFFFU) return result; }'),
('swap_start_and_group','func_8001A270.c','section, 1, group);','section, group, 1);')]
rows=[]
def no_core():resource.setrlimit(resource.RLIMIT_CORE,(0,0))
with tempfile.TemporaryDirectory(prefix='constructor-peer-') as td:
    d=Path(td)
    for label,name,old,new in mutations:
        q=d/label/'packet'/'work'/'unit';q.mkdir(parents=True);(q/'nonmatch').mkdir()
        for f in ['test_host.py','test_native.py','native_replay.py','host_behavior.c']:shutil.copy2(P/f,q/f)
        for f in (P/'nonmatch').glob('*.c'):shutil.copy2(f,q/'nonmatch'/f.name)
        source=q/'nonmatch'/name;text=source.read_text();assert old in text;source.write_text(text.replace(old,new))
        proc=subprocess.run(['python3',str(q/'test_host.py')],cwd=R,capture_output=True,text=True,env=dict(os.environ,PYTHONPATH=str(R)),preexec_fn=no_core)
        assert proc.returncode!=0 and 'AssertionError' in proc.stderr,(label,proc.returncode,proc.stderr)
        rows.append({'mutation':label,'test_rejects':True,'returncode':proc.returncode,'failure':'independent expected return/call/memory assertion'})
print(json.dumps({'result':'PASS','independent_mutations':rows},indent=2))
