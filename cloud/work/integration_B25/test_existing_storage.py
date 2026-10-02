from pathlib import Path
import json,shutil,copy
import pytest
from tools.conveyor.pipeline import owned_existing_storage as E,owned_storage as S,owned_data as D,lock
ROOT=Path(__file__).resolve().parents[3]
@pytest.fixture
def repo(tmp_path):
 row=json.loads((ROOT/'cloud/work/integration_B25/storage_record.json').read_text())
 paths={row['source'],row['tu_before_source'],row['strict_proof'],row['context_proof'],row['linked_proof'],row['startup_proof'],'asm/us/1000.s','splat.us.yaml','Makefile','rush2049.us.ld'}|set(json.loads((ROOT/row['context_proof']).read_text()))
 for rel in paths:
  dst=tmp_path/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/rel,dst)
 # Sparse synthetic inputs keep tests runnable without ignored retail assets.
 # Actual original-ROM hashes/placement are tested by separate private gates.
 import hashlib
 slot=row['data_slot'];offset=int(slot['offset'],0);size=int(slot['size'],0)
 data=bytearray(offset+size);data[offset:offset+4]=int(row['vram_start'],0).to_bytes(4,'big')
 asset=tmp_path/slot['source'];asset.parent.mkdir(parents=True,exist_ok=True);asset.write_bytes(data)
 slot['sha256']=hashlib.sha256(data[offset:offset+size]).hexdigest()
 rom=bytearray(0x1038);(tmp_path/'baserom.us.z64').write_bytes(rom)
 startup_path=tmp_path/row['startup_proof'];startup=json.loads(startup_path.read_text())
 startup['entry_code_sha256']=hashlib.sha256(rom[0x1000:0x1038]).hexdigest()
 startup_path.write_text(json.dumps(startup,indent=2)+'\n');row['startup_proof_sha256']=E.sha(startup_path)
 dst=tmp_path/row['tu'];dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/row['tu_before_source'],dst)
 locks={row['tu']+':'+fn:entry for fn,entry in row['prior_locks'].items()};locks['unrelated:member']={'verified':'rom-sha1','body_sha256':'unrelated'};lock.save_lock(locks,tmp_path/'matched.lock.json')
 # Minimal unrelated sentinel rows are checked separately; genuine root rows
 # require their full source/header authority and are retained in ROM proof.
 (tmp_path/'rom_owned_data.json').write_text(json.dumps({'schema':1,'rom_slots':[],'storage_blocks':[],'future_metadata':{'preserve':True}}))
 targets={x['function']:(x['target_sha256'],x['words']) for x in json.loads((ROOT/row['strict_proof']).read_text())}
 return tmp_path,row,targets

def register(repo):
 root,row,targets=repo;E.register(root,row,row['strict_proof_sha256'],targets);return root,row,targets

def test_joint_activation_regeneration_revert_preserves_prior_locks_and_c_owner(repo):
 root,row,targets=register(repo);before=(root/row['tu']).read_bytes();locks=lock.load_lock(root/'matched.lock.json');yaml=(root/'splat.us.yaml').read_bytes();calls=[]
 def gate(r,paths):
  calls.append(list(paths));D.rom_slots(r);S.storage_blocks(r);return True
 E.transition(root,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=gate)
 assert (root/row['tu']).read_bytes()==(root/row['source']).read_bytes()
 assert lock.load_lock(root/'matched.lock.json')['src/rom/lib_cc50.c:dll_init']['verified']=='rom-sha1'
 generated=(root/'src/rom/storage_overrides.mk').read_text();assert row['flags'] in generated and ' -O2' not in generated
 assert 'include/PR/os_time.h' in generated and 'src/rom/rom_tu.h' in generated
 assert 'owned_existing_storage assert timer_services' in generated
 S.generate(root);assert (root/'src/rom/storage_overrides.mk').read_text()==generated
 assert D.companions_for_tu(root,row['tu'],{'timer_services_pointer':'passthrough'})==''
 E.transition(root,row['owner'],False,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=gate)
 assert (root/row['tu']).read_bytes()==before and (root/'splat.us.yaml').read_bytes()==yaml
 assert lock.load_lock(root/'matched.lock.json')==locks
 reg=json.loads((root/'rom_owned_data.json').read_text());assert not reg['rom_slots'] and reg['future_metadata']=={'preserve':True}
 assert len(calls)==2

def test_failed_gate_restores_complete_package_and_regates(repo):
 root,row,targets=register(repo);before={p:p.read_bytes() if p.exists() else None for p in E.package_paths(root,row)};calls=[]
 def gate(r,p):calls.append(bool(E._row(r,row['owner'])['active']));return not calls[-1]
 with pytest.raises(ValueError,match='original-ROM gate failed'):E.transition(root,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=gate)
 assert calls==[True,False]
 assert all((p.read_bytes() if p.exists() else None)==data for p,data in before.items())

@pytest.mark.parametrize('damage',['target','context','bodylock','proof','flags'])
def test_changed_authority_refuses_before_mutation(repo,damage):
 root,row,targets=repo
 if damage=='target':targets['dll_init']=('wrong',35)
 if damage=='context':(root/'src/rom/rom_tu.h').write_text('changed')
 if damage=='bodylock':
  entries=lock.load_lock(root/'matched.lock.json');entries[row['tu']+':dll_update']['body_sha256']='wrong';lock.save_lock(entries,root/'matched.lock.json')
 if damage=='proof':row['strict_proof_sha256']='wrong'
 if damage=='flags':row['flags']=row['flags'].replace('-O1','-O2')
 before=(root/'rom_owned_data.json').read_bytes()
 with pytest.raises(ValueError):E.register(root,row,row['strict_proof_sha256'],targets)
 assert (root/'rom_owned_data.json').read_bytes()==before

def test_real_slot_requires_active_complete_owner_and_no_single_splice(repo):
 root,row,targets=register(repo);reg=D.load_registry(root);reg['rom_slots'].append(row['data_slot']);(root/'rom_owned_data.json').write_text(json.dumps(reg))
 with pytest.raises(D.OwnershipError):D.rom_slots(root)
 with pytest.raises(SystemExit):S.require_single_function_safe(root,row['tu'])


def test_publication_failure_restores_locks_and_regates(repo):
 root,row,targets=register(repo);before=lock.load_lock(root/'matched.lock.json');calls=[]
 def gate(r,p):calls.append(bool(E._row(r,row['owner'])['active']));return True
 def publish(r,row,active,paths):
  assert lock.load_lock(r/'matched.lock.json')[row['tu']+':dll_init']['verified']=='rom-sha1'
  raise ValueError('standard publication rejected')
 with pytest.raises(ValueError,match='publication rejected'):
  E.transition(root,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=gate,publish=publish)
 assert calls==[True,False] and lock.load_lock(root/'matched.lock.json')==before


def test_baseline_pointer_payload_corruption_refused(repo):
 root,row,targets=repo;data=root/'assets/us/data.bin';value=bytearray(data.read_bytes());value[0x1cff0]^=1;data.write_bytes(value)
 with pytest.raises(ValueError,match='pointer slot changed'):E.register(root,row,row['strict_proof_sha256'],targets)


def test_startup_lifecycle_proof_requires_entire_original_block(repo):
 root,row,targets=repo;proof=root/row['startup_proof'];value=json.loads(proof.read_text());value['owned_end_exclusive']='0x80037c60';proof.write_text(json.dumps(value));row['startup_proof_sha256']=E.sha(proof)
 with pytest.raises(ValueError,match='startup clearing'):E.register(root,row,row['strict_proof_sha256'],targets)


@pytest.mark.parametrize('damage',['context','sdk_header','before_source','proof'])
def test_active_owner_pins_context_and_revert_prerequisites(repo,damage):
 root,row,targets=register(repo);E.transition(root,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=lambda r,p:True)
 rel={'context':'src/rom/rom_tu.h','sdk_header':'include/PR/os_time.h','before_source':row['tu_before_source'],'proof':row['strict_proof']}[damage]
 (root/rel).write_text('changed')
 with pytest.raises(ValueError):S.assert_context(root,E._row(root,row['owner']))


def test_readonly_active_context_without_rom_retains_acceptance_rom_gate(repo):
 root,row,targets=register(repo);E.transition(root,row['owner'],True,reviewed_digest=row['strict_proof_sha256'],trusted_targets=targets,gate=lambda r,p:True)
 (root/'baserom.us.z64').unlink()
 S.assert_context(root,E._row(root,row['owner']))
 problems=lock.check(lock.load_lock(root/'matched.lock.json'),repo=root)
 assert not any(spec=='rom_owned_data.json:storage' for spec,message in problems)
 with pytest.raises(FileNotFoundError):E.validate(root,E._row(root,row['owner']),row['strict_proof_sha256'],targets)


def test_existing_mode_never_accepts_o2_metadata_recipe(repo):
 root,row,targets=register(repo);reg=D.load_registry(root);reg['storage_blocks'][0]['flags']='-g0 -O2 -mips2 -G 0 -non_shared';(root/'rom_owned_data.json').write_text(json.dumps(reg))
 with pytest.raises(ValueError,match='unsupported compiler recipe'):S.storage_blocks(root)
