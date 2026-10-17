"""No coverage claim: replay the genuine LoadTrack helper boundary and negatives."""
import importlib.util
from pathlib import Path


def test_mat3_loadtrack_helper():
    root=Path(__file__).resolve().parents[2]
    path=root/'cloud/work/frontier/dot_mat3_loadtrack_helper_20261017/verify.py'
    spec=importlib.util.spec_from_file_location('loadtrack_helper_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    result=module.verify(root)
    assert result['claims']==[] and result['preserved_locked_matches']==11
    assert result['resolved_body_identical']


def test_mat3_loadtrack_optimized_guards(tmp_path):
    """Proof refusals must survive Python's removal of assert statements."""
    import json
    import shutil
    import subprocess
    import sys
    root=Path(__file__).resolve().parents[2]
    packet=root/'cloud/work/frontier/dot_mat3_loadtrack_helper_20261017'
    runner=tmp_path/'check_optimized.py'
    runner.write_text('''import dataclasses, importlib.util, sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3]
sys.path.insert(0,str(repo/'tools/cloud'));import score
spec=importlib.util.spec_from_file_location('optimized_packet',p/'verify.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
if mode=='target':
    original=score.targets;rows=original();rows=dict(rows);rows['LoadTrack']=list(rows['LoadTrack']);rows['LoadTrack'][0]^=1
    score.targets=lambda:rows
if mode=='unverified':
    original=score.compare
    def changed(*args,**kwargs):
        v=original(*args,**kwargs)
        return dataclasses.replace(v,unverified=v.unverified+['deliberate unsupported relocation'])
    score.compare=changed
try:
    m.verify(repo,compile=mode=='unverified')
except RuntimeError:
    print('guard rejected '+mode)
else:
    raise SystemExit('guard accepted '+mode)
''')
    for mode in ['claims','source','script','target','unverified']:
        p=tmp_path/mode;shutil.copytree(packet,p)
        if mode=='claims':
            r=json.loads((p/'receipt.json').read_text());r['claims']=['LoadTrack'];(p/'receipt.json').write_text(json.dumps(r))
        elif mode=='source':
            with (p/'group/LoadTrack.c').open('a') as f:f.write('\n/* changed source */\n')
        elif mode=='script':
            with (p/'verify.py').open('a') as f:f.write('\n# changed verifier\n')
        run=subprocess.run([sys.executable,'-O',str(runner),str(p),str(root),mode],text=True,capture_output=True)
        if run.returncode or run.stdout.strip()!='guard rejected '+mode:
            raise RuntimeError(mode+' optimized guard failed: '+run.stdout+run.stderr)
