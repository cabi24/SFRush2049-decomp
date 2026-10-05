import copy,importlib.util,json,shutil,tempfile,unittest
from pathlib import Path
from unittest.mock import patch
ROOT=Path(__file__).resolve().parents[2]
import tools.conveyor.pipeline.promote as promote
import tools.conveyor.pipeline.lock as locks
from tools.conveyor.pipeline import owned_storage as storage
class Conn:
 def __init__(self):self.calls=[]
 def execute(self,*args):self.calls.append(args)
 def close(self):pass
class Tx:
 def __enter__(self):return self
 def __exit__(self,*args):return False
class StorageTests(unittest.TestCase):
 def setUp(self):
  self.temp=tempfile.TemporaryDirectory();self.repo=Path(self.temp.name);self.row=json.loads((ROOT/'cloud/work/integration_B18/storage_block.json').read_text())
  for rel in [self.row['source'],self.row['tu_before_source'],self.row['passthrough_asm'],*sum(([i['source'],i['before_source']] for i in self.row['context_files']),[])]:
   dst=self.repo/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/rel,dst)
  for f in ['Makefile','splat.us.yaml','rush2049.us.ld','include/PR/os_thread.h']:
   dst=self.repo/f;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/f,dst)
  for context in self.row['context_files']:
   shutil.copy(self.repo/context['before_source'],self.repo/context['path'])
  tu=self.repo/self.row['tu'];tu.parent.mkdir(parents=True,exist_ok=True);shutil.copy(self.repo/self.row['tu_before_source'],tu)
  self.rom=[{'owner':'pi_manager','immutable_value':'retain exact unrelated ROM metadata'}]
  (self.repo/storage.REGISTRY).write_text(json.dumps(dict(schema=1,rom_slots=self.rom,storage_blocks=[self.row])))
  storage.generate(self.repo)
  self.entries={self.row['source']+':'+fn:dict(body_sha256=locks.body_sha(self.repo/self.row['source'],fn),flagset=self.row['flags'],verified='score0',target_id=fn) for fn in self.row['members']}
  (self.repo/'matched.lock.json').write_text(json.dumps(self.entries))
  self.patches=[patch.object(promote,'REPO',self.repo),patch.object(promote,'_git_clean',lambda paths:True),patch.object(locks,'LOCKFILE',self.repo/'matched.lock.json'),patch.object(locks,'require_promotable_population',lambda *a:None)]
  for p in self.patches:p.start()
 def tearDown(self):
  for p in reversed(self.patches):p.stop()
  self.temp.cleanup()
 def package(self):return {str(p.relative_to(self.repo)):p.read_bytes() if p.exists() else None for p in storage.package_paths(self.repo,self.row['tu'])}
 def test_failed_gate_restores_full_tuple_and_other_owner(self):
  before=self.package();calls=[]
  def gate(repo,paths,builder):
   calls.append(copy.deepcopy(json.loads((repo/storage.REGISTRY).read_text())))
   return (len(calls)>1,'simulated ROM failure then restored baseline')
  with patch.object(storage,'_gate',gate):
   with self.assertRaises(promote.Refusal):storage.run_promotion('vi_manager',via_builder=True)
  self.assertEqual(before,self.package());self.assertEqual(calls[0]['rom_slots'],self.rom);self.assertFalse(calls[-1]['storage_blocks'][0]['active']);self.assertEqual(len(calls),2)
 def test_whole_module_activation_and_revert_migrate_both_locks(self):
  conn=Conn()
  with patch.object(storage,'_gate',lambda *a:(True,'ROM matches!')),patch.object(promote.dbmod,'connect',lambda *a:conn),patch.object(promote.dbmod,'tx',lambda *a:Tx()),patch.object(promote,'_run',lambda *a,**k:__import__('subprocess').CompletedProcess(a,0,'','')):
   storage.run_promotion('vi_manager',via_builder=True)
   self.assertEqual(storage.tu_states(self.repo,self.row['tu']),{n:'promoted' for n in self.row['members']})
   self.assertEqual(json.loads((self.repo/storage.REGISTRY).read_text())['rom_slots'],self.rom)
   entries=json.loads((self.repo/'matched.lock.json').read_text());self.assertTrue(all(self.row['tu']+':'+n in entries for n in self.row['members']));self.assertEqual(len(conn.calls),2)
   with self.assertRaises(SystemExit):storage.require_single_function_safe(self.repo,self.row['tu'])
   self.assertIn('INSERT AFTER .data;', (self.repo/storage.LINKER).read_text())
   self.assertIn('--rename-section .bss=.bss.vi_manager',(self.repo/storage.OVERRIDES).read_text())
   storage.run_revert('vi_manager',via_builder=True)
   self.assertEqual(storage.tu_states(self.repo,self.row['tu']),{})
   self.assertEqual((self.repo/self.row['tu']).read_bytes(),(self.repo/self.row['tu_before_source']).read_bytes())
   self.assertFalse(json.loads((self.repo/storage.REGISTRY).read_text())['storage_blocks'][0]['active']);self.assertEqual(len(conn.calls),4)
 def test_complete_context_pin_detects_storage_declaration_drift(self):
  reg=json.loads((self.repo/storage.REGISTRY).read_text());reg['storage_blocks'][0]['active']=True;(self.repo/storage.REGISTRY).write_text(json.dumps(reg));shutil.copy(self.repo/self.row['source'],self.repo/self.row['tu']);shutil.copy(self.repo/self.row['context_files'][0]['source'],self.repo/self.row['context_files'][0]['path'])
  storage.generate(self.repo)
  p=self.repo/self.row['tu'];p.write_text(p.read_text().replace('gViMgrMesgBuffer[5]','gViMgrMesgBuffer[6]'))
  with self.assertRaises(ValueError):storage.tu_states(self.repo,self.row['tu'])
 def test_overlapping_complete_storage_owners_refused(self):
  reg=json.loads((self.repo/storage.REGISTRY).read_text());other=copy.deepcopy(self.row);other['owner']='another';other['tu']='src/rom/another.c';reg['storage_blocks'].append(other);(self.repo/storage.REGISTRY).write_text(json.dumps(reg))
  with self.assertRaises(ValueError):storage.storage_blocks(self.repo)
if __name__=='__main__':unittest.main()
