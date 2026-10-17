"""Music source-backed complete closure and optimized refusal tests."""
from pathlib import Path
import importlib.util,json,subprocess,sys,shutil
PACKET=Path(__file__).resolve().parents[2]/'cloud/work/frontier/mat3_game_music_exact_20261017'
def module():
    spec=importlib.util.spec_from_file_location('music_verify',PACKET/'verify.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m

def test_music_complete_replay(tmp_path):
    r=module().verify(tmp_path)
    assert r['new_candidate_bytes']==304 and len(r['positive'])==13 and len(r['leaves'])==3

def test_music_guards_under_optimized_python(tmp_path):
    program=r'''
from pathlib import Path
import copy,importlib.util,json,sys
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('v',p/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'verification.json').read_text());count=0
def refusal(fn,reason):
    global count
    try:fn()
    except RuntimeError as e:
        if reason not in str(e):raise RuntimeError('wrong refusal: '+str(e))
        count+=1;return
    raise RuntimeError('false proof accepted: '+reason)
for field,value,reason in [('differing',1,'non-strict'),('extra_words',1,'non-strict'),('unverified',['bad'],'non-strict'),('unresolved',['bad'],'non-strict'),('errors',['bad'],'non-strict'),('total',75,'incomplete extent')]:
    z=copy.deepcopy(r);z['positive'][m.ROOT_NAME]['comparison'][field]=value;refusal(lambda:m.verify_report(z),reason)
for change,reason in [
    (lambda z:z.update(new_candidate_bytes=308),'false credit'),
    (lambda z:z.update(accepted_image_bytes=304),'false credit'),
    (lambda z:z['group_claims'].pop(),'claim inventory'),
    (lambda z:z['leaves'].pop('func_8009211C'),'claim inventory'),
    (lambda z:z['positive'][m.ROOT_NAME].update(symbol_bytes=300),'incomplete extent'),
    (lambda z:z['positive'][m.ROOT_NAME].update(frame=32),'root frame'),
    (lambda z:z['inventory']['functions'].pop(m.ROOT_NAME),'emitted group inventory'),
    (lambda z:z['leaves']['func_800D63EC']['inventory'].update(private_preentry_stub_bytes=0),'leaf inventory'),
    (lambda z:z['behavior'].update(fixtures=1),'semantic fixture'),
    (lambda z:z['layout'].update(short_options=z['layout']['positive']),'layout control'),
    (lambda z:z['negatives']['wrong_exclusion']['behavior'].update(exit=1),'semantic refusal'),
    (lambda z:z['causal_private_rng']['func_800D6530']['comparison'].update(extra_words=0),'private RNG boundary'),
]:
    z=copy.deepcopy(r);change(z);refusal(lambda:m.verify_report(z),reason)
original=Path.read_text
for mode,reason in [('input','source or target binding'),('target','source or target binding'),('trace_input','trace input binding'),('trace_pair','trace pair changed'),('trace_object','trace object changed'),('sealed','fresh sealed receipt'),('group_claims','complete group claims')]:
    def read(path,*args,**kwargs):
        text=original(path,*args,**kwargs)
        if path==p/'verification.json':
            z=json.loads(text)
            if mode=='input':z['inputs']['files']['verify.py']='0'*64
            if mode=='target':z['inputs']['targets'][m.ROOT_NAME]='0'*64
            if mode=='sealed':z['object_sha256']='0'*64
            return json.dumps(z)
        if path==p/'trace_fidelity.json':
            z=json.loads(text)
            if mode=='trace_input':z['inputs']['files']['verify.py']='0'*64
            if mode=='trace_pair':z['trace_object_sha256']='0'*64
            if mode=='trace_object':z['stock_object_sha256']=z['trace_object_sha256']='0'*64
            return json.dumps(z)
        if path==p/'group/group.json' and mode=='group_claims':
            z=json.loads(text);z['claims']=[];return json.dumps(z)
        return text
    Path.read_text=read
    try:refusal(lambda:m.verify(Path(sys.argv[2])/mode),reason)
    finally:Path.read_text=original
if count!=25:raise RuntimeError('refusal scope changed')
print('25 optimized integrity refusals pass')
'''
    r=subprocess.run([sys.executable,'-O','-c',program,str(PACKET),str(tmp_path)],capture_output=True,text=True)
    assert r.returncode==0,r.stdout+r.stderr
    assert r.stdout=='25 optimized integrity refusals pass\n'

def test_music_relocated_packet(tmp_path):
    moved=tmp_path/'different_checkout'/'music';shutil.copytree(PACKET,moved,ignore=shutil.ignore_patterns('__pycache__'))
    m=module();m.HERE=moved
    assert m.verify(tmp_path/'objects')['new_candidate_bytes']==304


def test_music_missing_ido_diagnostic(tmp_path):
    program=r'''
from pathlib import Path
import importlib.util,json,sys
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('music_availability',p/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
m.score.IDO=Path(sys.argv[2])
try:m.inputs(json.loads((p/'group/group.json').read_text()),json.loads((p/'closure.json').read_text()))
except SystemExit as e:
    if 'run tools/cloud/setup.sh' not in str(e):raise
else:raise RuntimeError('missing IDO did not use the standard diagnostic')
print('standard missing-IDO diagnostic passes')
'''
    r=subprocess.run([sys.executable,'-O','-c',program,str(PACKET),str(tmp_path/'missing_ido')],capture_output=True,text=True)
    assert r.returncode==0,r.stdout+r.stderr
    assert r.stdout=='standard missing-IDO diagnostic passes\n'
