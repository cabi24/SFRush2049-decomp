import importlib.util,json,os,shutil,subprocess,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_mat3_createbg_research_20261017'
REPO=Path(os.environ.get('N64_RESEARCH_REPO',ROOT))
def ready():
    ido=Path(os.environ.get('IDO_DIR',REPO/'tools/cloud/ido'))
    if not (ido/'cc').is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('IDO required but absent')
        pytest.skip('IDO unavailable')
def run(packet,*args):
    return subprocess.run([sys.executable,'-O',str(packet/'verify.py'),'--repo',str(REPO),*args],text=True,capture_output=True)
def test_createbg_replay():
    ready();r=run(PACKET);assert r.returncode==0,r.stdout+r.stderr

def test_createbg_optimized_refusals(tmp_path):
    ready()
    mutations=[('claims','false credit',lambda x:x.update(claims=['particle_lifetime_set'])),
       ('credit','false credit',lambda x:x.update(new_accepted_bytes=732)),
       ('unverified','sealed result changed',lambda x:x['results']['natural']['functions']['particle_lifetime_set']['comparison']['unverified'].append('injected')),
       ('extent','sealed result changed',lambda x:x['results']['natural']['functions']['func_800A78BC'].update(symbol_bytes=248)),
       ('target','target or baseline changed',lambda x:x['target_sha256'].update(particle_lifetime_set='0'*64))]
    for name,message,edit in mutations:
        d=tmp_path/name;shutil.copytree(PACKET,d);p=d/'receipt.json';j=json.loads(p.read_text());edit(j);p.write_text(json.dumps(j));r=run(d);assert r.returncode!=0 and message in r.stderr,(name,r.stdout,r.stderr)
    d=tmp_path/'source';shutil.copytree(PACKET,d);p=d/'group/createbg.c';p.write_text(p.read_text()+'\n/* changed */\n');r=run(d);assert r.returncode!=0 and 'source or proof changed' in r.stderr,r.stdout+r.stderr
    d=tmp_path/'recipe';shutil.copytree(PACKET,d);p=d/'group/group.json';j=json.loads(p.read_text());j['claims']=['particle_lifetime_set'];p.write_text(json.dumps(j));r=run(d);assert r.returncode!=0 and 'false group claims' in r.stderr,r.stdout+r.stderr

def test_createbg_relocated_packet(tmp_path):
    ready();d=tmp_path/'packet';shutil.copytree(PACKET,d);r=run(d);assert r.returncode==0,r.stdout+r.stderr


def test_createbg_missing_ido_diagnostic(tmp_path):
    env=dict(os.environ,IDO_DIR=str(tmp_path/'missing_ido'))
    r=subprocess.run([sys.executable,'-O',str(PACKET/'verify.py'),'--repo',str(REPO)],env=env,text=True,capture_output=True)
    assert r.returncode!=0 and 'missing: run tools/cloud/setup.sh (or set IDO_DIR)' in r.stderr,r.stdout+r.stderr
    assert 'Traceback' not in r.stderr,r.stderr
