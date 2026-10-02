"""Private builder proof; no coordinator DB/git state is touched."""
from pathlib import Path
import json,subprocess,hashlib,os
from tools.conveyor.pipeline import owned_existing_storage as E,owned_storage as S,owned_data as D,lock
repo=Path.cwd()
if repo.name!='B26-private-rom' or '/agents/B/' not in str(repo):raise SystemExit('private B26 builder only')
row=json.loads((repo/'cloud/work/integration_B26/storage_record.json').read_text())
targets={x['function']:(x['target_sha256'],x['words']) for x in json.loads((repo/row['strict_proof']).read_text())}
tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
env=dict(os.environ,LD_LIBRARY_PATH=str(tk/'lib'));proof=[]
reg0=D.load_registry(repo);unrelated_before={'rom_slots':reg0['rom_slots'],'storage_blocks':reg0['storage_blocks']}
prior_locks=dict(lock.load_lock(repo/'matched.lock.json'));baseline_source=(repo/row['tu']).read_bytes();yaml=(repo/'splat.us.yaml').read_bytes()
def gate(repo,paths,label='gate'):
 S.generate(repo)
 ld=repo/'rush2049.us.ld';ld.write_text(D.rewrite_linker(ld.read_text(),repo))
 for file in (repo/'src/rom').glob('*.c'):file.touch()
 # Actual mutable input must rebuild even when its basename persists on revert.
 for rel in ['build/us/assets/us/data.o','build/us/rush2049.us.elf','build/us/rush2049.us.z64']:(repo/rel).unlink(missing_ok=True)
 log=repo/(label+'.log')
 with log.open('w') as output:
  p=subprocess.run(['make','COMPILER=ido','CC='+str(tk/'ido/cc'),'-j2'],env=env,stdout=output,stderr=subprocess.STDOUT)
  code=p.returncode
  if code==0:code=subprocess.run(['make','test'],env=env,stdout=output,stderr=subprocess.STDOUT).returncode
 rom=repo/'build/us/rush2049.us.z64';digest=hashlib.sha1(rom.read_bytes()).hexdigest() if rom.exists() else None
 proof.append(dict(case=label,exit_code=code,sha1=digest,retail_gate=code==0 and digest=='3f99351d7bb61656614bdb2aa1a90cfe55d1922c'))
 return proof[-1]['retail_gate']

initial_locks=dict(lock.load_lock(repo/'matched.lock.json'));initial_reg=(repo/'rom_owned_data.json').read_bytes()

def bad_clock(r,paths):
 if not E._row(r,row['owner'])['active']:return gate(r,paths,'clock_rollback')
 assert gate(r,paths,'active_before_clock_corruption')
 obj=r/'build/us/src/rom/lib_8a80.o';payload=r/'private_bad_clock.bin'
 subprocess.run(['mips-linux-gnu-objcopy','--dump-section','.data='+str(payload),str(obj)],check=True)
 data=bytearray(payload.read_bytes());data[7]^=1;payload.write_bytes(data)
 subprocess.run(['mips-linux-gnu-objcopy','--update-section','.data='+str(payload),str(obj)],check=True)
 for rel in ['build/us/rush2049.us.elf','build/us/rush2049.us.z64']:(r/rel).unlink(missing_ok=True)
 with (r/'bad_clock.log').open('w') as output:
  process=subprocess.run(['make','COMPILER=ido','CC='+str(tk/'ido/cc'),'-j2'],env=env,stdout=output,stderr=subprocess.STDOUT)
 rom=r/'build/us/rush2049.us.z64';digest=hashlib.sha1(rom.read_bytes()).hexdigest() if rom.exists() else None
 proof.append(dict(case='actual compiled clock corruption',exit_code=process.returncode,sha1=digest,retail_gate=False))
 assert digest!='3f99351d7bb61656614bdb2aa1a90cfe55d1922c'
 return False

try:E.transition(repo,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=bad_clock)
except ValueError as error:assert 'original-ROM gate failed' in str(error)
else:raise AssertionError('actual clock corruption accepted')
assert lock.load_lock(repo/'matched.lock.json')==initial_locks
assert (repo/'rom_owned_data.json').read_bytes()==initial_reg

def bad_storage(r,paths):
 if not E._row(r,row['owner'])['active']:return gate(r,paths,'storage_rollback')
 assert gate(r,paths,'active_before_linker_corruption')
 ld=r/'rom_owned_storage.ld';text=ld.read_text()
 old='.sdk_initialize_bss 0x800367d0 (NOLOAD)'
 assert old in text
 ld.write_text(text.replace(old,'.sdk_initialize_bss 0x800367e0 (NOLOAD)',1))
 for rel in ['build/us/rush2049.us.elf','build/us/rush2049.us.z64']:(r/rel).unlink(missing_ok=True)
 with (r/'wrong_bss_link.log').open('w') as output:
  process=subprocess.run(['make','COMPILER=ido','CC='+str(tk/'ido/cc'),'-j2'],env=env,stdout=output,stderr=subprocess.STDOUT)
 assert process.returncode!=0
 assert 'owned storage alias moved: gSpTaskState' in (r/'wrong_bss_link.log').read_text()
 proof.append(dict(case='actual generated BSS placement corruption',exit_code=process.returncode,retail_gate=False,alias_assertion_refused=True))
 return False
try:E.transition(repo,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=bad_storage)
except ValueError as error:assert 'original-ROM gate failed' in str(error)
else:raise AssertionError('wrong BSS accepted')
assert lock.load_lock(repo/'matched.lock.json')==initial_locks
assert (repo/'rom_owned_data.json').read_bytes()==initial_reg
(repo/'negative_rom_proof.json').write_text(json.dumps(dict(scope='private actual compiler-data and storage-linker corruption refusal + complete-package rollback',results=proof,prior_locks_preserved=True,registry_restored_exactly=True,final_state='inactive baseline restored'),indent=2)+'\n')
print((repo/'negative_rom_proof.json').read_text())
