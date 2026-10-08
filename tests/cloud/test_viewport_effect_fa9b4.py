"""Portable focused replay. No live lock or mutable production fingerprints."""
import dataclasses, hashlib, importlib.util, json, os, shutil, subprocess, sys
from contextlib import contextmanager
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_viewport_effect_fa9b4_20261006'

def root_for(env):
 if os.environ.get(env):return Path(os.environ[env]).resolve()
 for p in HERE.parents:
  if (p/'tools/cloud/score.py').is_file():return p
 pytest.skip('repository tool/history root required')

@pytest.fixture(scope='module')
def roots():return root_for('RUSH_REFERENCE_ROOT'),root_for('RUSH_TOOL_ROOT')

@pytest.fixture(scope='module')
def host(tmp_path_factory):
 if not shutil.which('cc'):pytest.skip('host C compiler required')
 tmp=tmp_path_factory.mktemp('viewport_host');so=tmp/'host.so'
 subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-shared','-fPIC',str(HERE/'host.c'),'-o',str(so)],check=True)
 return so

def test_source_and_receipts():
 source=(HERE/'candidate.c').read_bytes();base=json.loads((HERE/'baseline.json').read_text())
 assert source.splitlines()[0]==b'/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
 assert hashlib.sha256(source).hexdigest()==base['source_sha256']
 assert base['native_compiler_invocations']==1 and base['claims']==[]
 assert base['status']=='NONMATCH' and base['candidate_function_bytes']==912
 assert base['native_bytes']==924 and base['comparison']['differing']==227
 assert not any(base[k] for k in ('unresolved','unverified','errors','masked_offsets'))
 for name in ('semantics-before-build.json','semantics-after-build.json'):
  receipt=json.loads((HERE/name).read_text())
  assert receipt['source_sha256']==base['source_sha256']
  assert receipt['target_sha256']==base['target_sha256']
  assert receipt['native_instruction_coverage']==229 and receipt['external_calls_covered']==list(range(1,21))


def replay(roots,host,out,obj=None):
 ref,tools=roots
 cmd=[sys.executable,str(HERE/'verify_semantics.py'),'--reference-root',str(ref),'--tool-root',str(tools),'--host-library',str(host),'--output',str(out),'--cases','400']
 if obj:cmd+=['--native-object',str(obj)]
 subprocess.run(cmd,check=True,capture_output=True,text=True,cwd=out.parent)
 return json.loads(out.read_text())

def test_native_against_literal_host(roots,host,tmp_path):
 r=replay(roots,host,tmp_path/'native-host.json')
 assert r['cases']==400 and r['native_executions']==400


@contextmanager
def blob_score(tools):
 previous_path=sys.path[:]
 missing=object()
 previous_modules={name:sys.modules.get(name,missing) for name in ('score','owndata')}
 try:
  sys.path.insert(0,str(tools/'tools/cloud'))
  import score
  previous_state=(score.ASM_DIR,score._targets,score._target_fingerprint,score._own_data)
  try:
   score.ASM_DIR=tools/'asm/us/blob'
   score._targets=None
   score._target_fingerprint=None
   score._own_data={}
   yield score
  finally:
   score.ASM_DIR,score._targets,score._target_fingerprint,score._own_data=previous_state
 finally:
  sys.path[:]=previous_path
  for name,module in previous_modules.items():
   if module is missing:sys.modules.pop(name,None)
   else:sys.modules[name]=module


@pytest.mark.parametrize('fails',[False,True],ids=['success','failure'])
def test_blob_score_restores_foreign_image_and_caches(roots,monkeypatch,fails):
 tools=roots[1]
 with monkeypatch.context() as setup:
  setup.syspath_prepend(str(tools/'tools/cloud'))
  import score
  setup.setattr(score,'ASM_DIR',tools/'asm/us/ovl_a')
  setup.setattr(score,'_targets',None)
  setup.setattr(score,'_target_fingerprint',None)
  setup.setattr(score,'_own_data',{})
  prior_targets=score.targets()
  prior_own_data=score.own_data()
  prior=(score.ASM_DIR,score._targets,score._target_fingerprint,score._own_data)
  prior_path=sys.path[:]
  prior_modules={name:sys.modules.get(name) for name in ('score','owndata')}
  assert 'render_viewport_init' not in prior_targets

  def exercise():
   with blob_score(tools) as current:
    assert current is score
    assert current.ASM_DIR==tools/'asm/us/blob'
    assert len(current.targets()['render_viewport_init'])==924//4
    current.own_data()
    if fails:raise SystemExit('deliberate scorer image failure')

  if fails:
   with pytest.raises(SystemExit,match='deliberate scorer image failure'):exercise()
  else:exercise()
  assert score.ASM_DIR==prior[0]
  assert score._targets is prior[1]
  assert score._target_fingerprint is prior[2]
  assert score._own_data is prior[3]
  assert score.own_data() is prior_own_data
  assert sys.path==prior_path
  assert {name:sys.modules.get(name) for name in prior_modules}==prior_modules


def test_canonical_object_replay(roots,host,tmp_path):
 ref,tools=roots
 with blob_score(tools) as score:
  if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
   pytest.skip('pinned IDO and MIPS GNU linker required')
  if os.environ.get('RUSH_VIEWPORT_OBJECT'):
   obj=Path(os.environ['RUSH_VIEWPORT_OBJECT']).resolve()
  else:
   obj=tmp_path/'candidate.o'
   score.compile_single(HERE/'candidate.c','-g0 -O3 -mips2 -G 0 -non_shared',obj)
  expected=json.loads((HERE/'baseline.json').read_text())
  actual=dataclasses.asdict(score.compare(obj,'render_viewport_init',show=0))
  assert actual==expected['comparison']
  raw=score.text_words(obj);got,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,score.image_symbols())
  import struct
  assert hashlib.sha256(struct.pack('>'+str(len(got))+'I',*got)).hexdigest()==expected['resolved_candidate_sha256']
  assert not any((masks,unresolved,unverified,errors))
  r=replay(roots,host,tmp_path/'three-way.json',obj)
  assert r['native_executions']==800
