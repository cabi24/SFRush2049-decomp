"""Focused research-packet replay. Does not claim a broad suite or ROM gate."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_menu_loop_20261006'
spec=importlib.util.spec_from_file_location('menu_loop_verify',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def protected_repo():
 path=Path(os.environ.get('RUSH_PROTECTED_REPO',ROOT)).resolve()
 if not (path/'asm/us/ovl_a/extents.json').exists():
  if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('protected image-A inputs unavailable')
  pytest.skip('protected image-A inputs unavailable')
 # Match score.py: an explicit IDO_DIR overrides the tool checkout default.
 ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
 missing=[name for name in ('cc','cfe','uopt','ugen','as1') if not (ido/name).is_file()]
 if missing:
  reason='IDO toolchain unavailable at %s: missing %s'%(ido, ', '.join(missing))
  if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail(reason)
  pytest.skip(reason)
 return path

def test_scoped_no_draw_paths():
 for count,flag,mode in [(0,0,0),(-32768,0,0),(4,0,0),(1,1,0),(1,0,2)]:
  rows=v.oracle(count,flag,mode,65535,-128,255,0,1)
  assert [r[0] for r in rows]==[1,2,3,1]

def test_unsigned_height_and_signed_coordinate_boundary():
 rows=v.oracle(1,0,0,65535,-128,255,0,1)
 draws=[r for r in rows if r[0]==6]
 assert [r[2] for r in draws]==[176,16558,-32596,-16214]
 assert [r[3] for r in draws]==[102,103,104,105]

def test_signed_byte_table_boundary():
 for table,expected in [(0,-1),(1,0),(127,126),(128,127),(129,-128),(255,-2)]:
  rows=v.oracle(1,0,0,8,1,table,0,1)
  assert all(r[3]==expected for r in rows if r[0]==4)

def test_draw_mutation_changes_next_count_guard():
 rows=v.oracle(1,0,0,8,1,1,6,0)
 assert [r[0] for r in rows]==[1,2,3,4,5,6,1]
 assert rows[-1][6]==0

def test_frozen_producer_and_controls_from_other_cwd(tmp_path):
 repo=protected_repo();env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1')
 for name in ('verify.py','verify_controls.py'):
  result=subprocess.run([sys.executable,str(PACKET/name),'--repo',str(repo),'--check'],cwd=tmp_path,env=env,capture_output=True,text=True)
  assert result.returncode==0,(result.stdout,result.stderr)
 receipt=json.loads((PACKET/'verification.json').read_text())
 assert receipt['status']=='NONMATCH' and receipt['difference_offsets']==['0x70','0x7c','0x150','0x164']
 assert receipt['host_cases']==85476 and receipt['owned_data_bytes']==0

def test_optimized_python_fails_closed():
 result=subprocess.run([sys.executable,'-O',str(PACKET/'verify.py'),'--check'],capture_output=True,text=True)
 assert result.returncode!=0 and 'verification requires assertions' in result.stderr

@pytest.mark.parametrize('explicit',[False,True])
def test_configured_ido_discovery(tmp_path,monkeypatch,explicit):
 monkeypatch.setattr(sys.modules[__name__],'ROOT',tmp_path)
 monkeypatch.delenv('RUSH_PROTECTED_REPO',raising=False)
 monkeypatch.delenv('IDO_DIR',raising=False)
 monkeypatch.setenv('REQUIRE_TOOLCHAIN','1')
 (tmp_path/'asm/us/ovl_a').mkdir(parents=True)
 (tmp_path/'asm/us/ovl_a/extents.json').write_text('{}')
 ido=tmp_path/('explicit ido' if explicit else 'tools/cloud/ido')
 ido.mkdir(parents=True)
 if explicit:monkeypatch.setenv('IDO_DIR',str(ido))
 for name in ('cc','cfe','uopt','ugen','as1'):(ido/name).touch()
 assert protected_repo()==tmp_path
 (ido/'ugen').unlink()
 with pytest.raises(pytest.fail.Exception,match='missing ugen'):protected_repo()


def test_explicit_missing_ido_does_not_use_default(tmp_path,monkeypatch):
 monkeypatch.setattr(sys.modules[__name__],'ROOT',tmp_path)
 monkeypatch.delenv('RUSH_PROTECTED_REPO',raising=False)
 monkeypatch.setenv('REQUIRE_TOOLCHAIN','1')
 (tmp_path/'asm/us/ovl_a').mkdir(parents=True)
 (tmp_path/'asm/us/ovl_a/extents.json').write_text('{}')
 default=tmp_path/'tools/cloud/ido';default.mkdir(parents=True)
 for name in ('cc','cfe','uopt','ugen','as1'):(default/name).touch()
 monkeypatch.setenv('IDO_DIR',str(tmp_path/'missing-explicit'))
 with pytest.raises(pytest.fail.Exception,match='missing-explicit'):protected_repo()
