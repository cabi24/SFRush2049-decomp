"""Portable strict-MATCH checks; original projected-gauge NONMATCH stays frozen."""
import importlib.util,json,shutil,subprocess,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_projected_gauge_match_20261006'

def module():
 spec=importlib.util.spec_from_file_location('projected_gauge_match_verify',HERE/'verify.py')
 m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m

def toolchain():
 m=module();score=m.load_score(ROOT)
 if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
  pytest.skip('pinned IDO and MIPS GNU linker required')
 return m,score

def test_complete_replay_from_foreign_directory(tmp_path):
 toolchain();r=subprocess.run([sys.executable,str(HERE/'verify.py'),'--repo',str(ROOT),'--check'],cwd=tmp_path,capture_output=True,text=True)
 assert r.returncode==0,r.stdout+r.stderr

@pytest.mark.parametrize('offset,mask',[(0,8),(8,0x10000),(227,1)])
def test_full_match_rejects_word_mutations(offset,mask):
 previous_path=sys.path[:]
 missing=object()
 previous_modules={name:sys.modules.get(name,missing) for name in ('score','owndata')}
 try:
  # load_score creates a fresh scorer, so its image and caches stay private.
  m=module();score=m.load_score(ROOT);score.ASM_DIR=ROOT/'asm/us/ovl_a';native=score.targets()[m.NAME]
  m.check_match(native,list(native));changed=list(native);changed[offset]^=mask
  with pytest.raises(AssertionError):m.check_match(native,changed)
  with pytest.raises(AssertionError):m.check_match(native,native[:-1])
  with pytest.raises(AssertionError):m.check_match(native,native+[0])
 finally:
  sys.path[:]=previous_path
  for name,prior in previous_modules.items():
   if prior is missing:sys.modules.pop(name,None)
   else:sys.modules[name]=prior

def test_receipt_binds_final_source_and_own_packet_only():
 m=module();r=json.loads((HERE/'verification.json').read_text())
 assert r['status']=='MATCH' and r['base_commit']==m.BASE
 assert r['comparison']['differing']==0 and r['comparison']['total']==228
 assert not any(r['comparison'][k] for k in ('unresolved','unverified','errors','extra_words'))
 assert r['function_bytes']==912 and r['relocations']==31 and r['owned_data_bytes']==0
 assert r['gnu_full_relocation_equals_native'] and r['compiled_sha256']==r['native_sha256']
 assert r['behavior']['host_cases']==68306 and r['behavior']['native_execution'] is False
 assert str(m.SOURCE.relative_to(ROOT)) in r['packet_sha256']
 for p,h in r['packet_sha256'].items():
  assert p=='cloud/matches/ovl_a/func_803A3DA4.c' or p.startswith(str(HERE.relative_to(ROOT))+'/')
  assert m.sha((ROOT/p).read_bytes())==h
 assert not any(k in r for k in ('scorer_sha256','protected_targets','helper_protected_targets','test_sha256'))

def test_missing_ido_skips_without_compile(tmp_path,monkeypatch):
 m=module()
 class Score:
  IDO=tmp_path/'absent'
  def ido(self,*args):raise AssertionError('must skip before invoking IDO')
 monkeypatch.setattr(m,'load_score',lambda _:Score())
 assert m.verify(ROOT,tmp_path/'build')=={'status':'SKIP','reason':'pinned IDO and MIPS GNU linker required'}

def test_missing_linker_skips_without_compile(tmp_path,monkeypatch):
 m=module();(tmp_path/'cc').write_text('unused fixture')
 class Score:
  IDO=tmp_path
  def ido(self,*args):raise AssertionError('must skip before invoking IDO')
 monkeypatch.setattr(m,'load_score',lambda _:Score())
 monkeypatch.setattr(m.shutil,'which',lambda _:None)
 assert m.verify(ROOT,tmp_path/'build')=={'status':'SKIP','reason':'pinned IDO and MIPS GNU linker required'}
