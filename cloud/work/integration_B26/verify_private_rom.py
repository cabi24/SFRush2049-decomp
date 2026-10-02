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
reg0=D.load_registry(repo);unrelated_before={'rom_slots':[r for r in reg0['rom_slots'] if r['owner']!=row['data_slot']['owner']],
                  'storage_blocks':[r for r in reg0['storage_blocks'] if r['owner']!=row['owner']]}
assert any(r['owner']=='timer_services' and r.get('active') for r in reg0['storage_blocks']), 'active timer regression prerequisite'
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
assert gate(repo,[],'baseline')
if not any(r['owner']==row['owner'] for r in D.load_registry(repo)['storage_blocks']):
 E.register(repo,row,row['strict_proof_sha256'],targets)
else:
 E.validate(repo,row,row['strict_proof_sha256'],targets)
assert gate(repo,[],'inactive_registered')
E.transition(repo,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=lambda r,p:gate(r,p,'active'))
reg=D.load_registry(repo);assert [x for x in reg['storage_blocks'] if x['owner']!=row['owner']]==unrelated_before['storage_blocks'];assert [x for x in reg['rom_slots'] if x['owner']!=row['data_slot']['owner']]==unrelated_before['rom_slots']
assert gate(repo,[],'regenerated_active')
E.transition(repo,row['owner'],False,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=lambda r,p:gate(r,p,'reverted'))
assert (repo/row['tu']).read_bytes()==baseline_source and (repo/'splat.us.yaml').read_bytes()==yaml
assert lock.load_lock(repo/'matched.lock.json')==prior_locks
reg=D.load_registry(repo);assert reg['rom_slots']==unrelated_before['rom_slots'];assert [x for x in reg['storage_blocks'] if x['owner']!=row['owner']]==unrelated_before['storage_blocks']
result=dict(scope='isolated B26 source-built older game snapshot; actual generated O1 recipe, joint API and full-ROM gates, no production acceptance claim',results=proof,prior_locks_preserved=True,c_splat_unchanged=True,unrelated_vi_pi_timer_metadata_preserved=True,final_state='inactive registered baseline source/storage/container restored')
(repo/'private_rom_proof.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
