"""Research-only ReadPathData exact-body/open-helper proof and refusal controls."""
import importlib.util, json, os, shutil, subprocess, sys
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_mat3_readpath_20261017'
REPO=Path(os.environ.get('N64_RESEARCH_REPO',str(ROOT))).resolve()

def test_readpath_research():
    spec=importlib.util.spec_from_file_location('readpath_verify',PACKET/'verify.py')
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);result=m.verify(REPO)
    assert result['claims']==[] and result['new_accepted_bytes']==0
    assert result['preserved_locked_matches']==10 and not result['natural_dependency_closure']
    assert result['results']['natural']['behavior']['instruction_coverage']==[174,174]

def test_readpath_optimized_refusals(tmp_path):
    runner=tmp_path/'run.py'
    runner.write_text('''import dataclasses,importlib.util,sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3]
sys.path.insert(0,str(repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('proof',p/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
if mode=='target':
    rows=dict(score.targets());rows['audio_effect_apply']=list(rows['audio_effect_apply']);rows['audio_effect_apply'][0]^=1;score.targets=lambda:rows
if mode=='unverified':
    original=score.compare
    def changed(*a,**kw):
        r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate unknown relocation'])
    score.compare=changed
try:m.verify(repo,compile=mode=='unverified')
except RuntimeError:print('rejected '+mode)
else:raise SystemExit('accepted '+mode)
''')
    for mode in ['claims','source','script','model','target','unverified']:
        p=tmp_path/mode;shutil.copytree(PACKET,p)
        if mode=='claims':
            r=json.loads((p/'receipt.json').read_text());r['claims']=['audio_effect_apply'];(p/'receipt.json').write_text(json.dumps(r))
        elif mode in ('source','script','model'):
            f=p/({'source':'group/loader.c','script':'verify.py','model':'behavior.py'}[mode])
            f.write_text(f.read_text()+ ('\n/* changed */\n' if mode=='source' else '\n# changed\n'))
        run=subprocess.run([sys.executable,'-O',str(runner),str(p),str(REPO),mode],capture_output=True,text=True)
        if run.returncode or run.stdout.strip()!='rejected '+mode:raise RuntimeError(mode+': '+run.stdout+run.stderr)
