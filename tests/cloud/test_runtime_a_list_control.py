"""Focused list-control contract tests; full verifier checks native placement."""
import copy,hashlib,importlib.util,json,os,shutil,subprocess,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_list_control_20261006'
SOURCE=ROOT/'cloud/matches/ovl_a/func_803AE940.c'
spec=importlib.util.spec_from_file_location('list_control_semantics',HERE/'semantics.py')
s=importlib.util.module_from_spec(spec);spec.loader.exec_module(s)
def case(**kw):
 c=dict(pulse=1,item=0,group=0,texture=1,mode=0,first=1,total=12,active=0,selected=3,width=101,height=55,fade=s.fbits(.5),hook=0,hide=0)
 c.update(kw);return tuple(c.values())
def events(t):
 assert len(t)%31==0
 return [t[i:i+31] for i in range(0,len(t),31)]
def test_signed_division():
 assert [s.half(v) for v in (-32768,-3,-1,0,1,3,32767)]==[-16384,-1,0,0,0,1,16383]
def test_mode_gate_exactly_one():
 assert [e[0] for e in events(s.oracle(case(mode=1)))]==[1,2,7]
 assert any(e[0]==3 for e in events(s.oracle(case(mode=2))))
def test_scroll_boundary():
 assert [e[0] for e in events(s.oracle(case(item=3,total=5)))]==[1,1,2,7]
 assert any(e[0]==3 for e in events(s.oracle(case(item=3,total=6))))
def test_geometry_and_unsigned_alpha():
 end=events(s.oracle(case()))[-1]
 assert end[2:4]==[-220,-91] and end[6]==127
 assert events(s.oracle(case(fade=s.fbits(2.))))[-1][6]==254
 assert events(s.oracle(case(pulse=0)))[-1][6]==77

def test_texture_and_flip():
 end=events(s.oracle(case(item=1)))[-1];assert end[7]==1
 end=events(s.oracle(case(item=3)))[-1];assert end[15+5]==(55|8)
def test_callbacks_keep_descriptor_snapshot():
 end=events(s.oracle(case(item=3,hook=2)))[-1]
 assert end[9]==0x12345678 and end[10]==7 and end[15+7]==(77|8)
 assert end[2:4]==[-66,-48]
def test_width_callback_reload():
 end=events(s.oracle(case(hook=5)))[-1]
 assert end[2:4]==[-31,30]
def test_fixture_domain():
 samples=list(s.cases());assert len(samples)==5824
 assert {c[12] for c in samples}==set(range(6))
 assert all(0<=c[1]<4 and 0<=c[2]<16 and 0<=c[3]<16 and 0<=c[8]<32 for c in samples)
def test_receipt_source_binding():
 r=json.loads((HERE/'verification.json').read_text())
 assert r['status']=='MATCH' and r['kind']=='standalone' and r['bytes']==900
 assert r['source_sha256']==hashlib.sha256(SOURCE.read_bytes()).hexdigest()
 assert r['comparison']['differing']==r['comparison']['extra_words']==0
 assert not any(r['comparison'][k] for k in ('unresolved','unverified','errors'))
 assert r['relocations']==35 and r['alignment_bytes']==12 and r['owned_data_bytes']==0
 assert len(r['negative_controls'])==8 and all(x['host_rejected'] and not x['strict_match'] for x in r['negative_controls'].values())
 for n,h in r['support_sha256'].items():assert hashlib.sha256((HERE/n).read_bytes()).hexdigest()==h
@pytest.mark.skipif(not shutil.which('gcc'),reason='GCC is required for sanitized host checks')
def test_sanitized_host_oracle(tmp_path):
 result,_=s.verify(tmp_path,SOURCE)
 assert result==json.loads((HERE/'verification.json').read_text())['behavior']

def verifier_module():
 saved={k:sys.modules.pop(k,None) for k in ('semantics','elf_support')}
 sys.path.insert(0,str(HERE))
 try:
  spec=importlib.util.spec_from_file_location('list_control_verifier',HERE/'verify.py')
  v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v);return v
 finally:
  sys.path.remove(str(HERE))
  for k,old in saved.items():
   sys.modules.pop(k,None)
   if old is not None:sys.modules[k]=old
def test_provenance_drift_tolerated():
 v=verifier_module();r=json.loads((HERE/'verification.json').read_text());changed=copy.deepcopy(r)
 for field in ('protected_targets','scorer_sha256','tools'):changed[field]={'fresh':'different metadata'}
 v.check_receipt(r,changed)
@pytest.mark.parametrize('field',('source_sha256','native_sha256','gnu_linked_body_sha256','bytes','relocations','behavior','negative_controls'))
def test_substantive_drift_rejected(field):
 v=verifier_module();r=json.loads((HERE/'verification.json').read_text());changed=copy.deepcopy(r);changed[field]='incorrect'
 with pytest.raises(AssertionError,match='semantic receipt'):v.check_receipt(r,changed)
def test_optimized_python_refused():
 p=subprocess.run([sys.executable,'-O',str(HERE/'verify.py'),'--check'],capture_output=True,text=True)
 assert p.returncode!=0 and 'Python optimization is unsupported' in p.stderr

def test_full_native_replay_when_tools_available():
 ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
 needed=[ido/name for name in ('cc','cfe','uopt','ugen','as1')]
 missing=[str(p) for p in needed if not p.is_file() or not os.access(p,os.X_OK)]
 missing += [tool for tool in ('mips-linux-gnu-ld','gcc') if not shutil.which(tool)]
 if missing:pytest.skip('Native replay needs: '+', '.join(missing))
 p=subprocess.run([sys.executable,str(HERE/'verify.py'),'--check'],cwd=ROOT,capture_output=True,text=True)
 assert p.returncode==0,p.stdout+p.stderr
 assert '"status": "MATCH"' in p.stdout
