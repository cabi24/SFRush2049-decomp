import copy,json,shutil,hashlib
from pathlib import Path
import pytest
from tools.conveyor.pipeline import owned_text,owned_data,layout
ROOT=Path(__file__).resolve().parents[3]
@pytest.fixture
def repo(tmp_path):
 row=json.loads((ROOT/'cloud/work/integration_B22_followup/text_boundary_record.json').read_text())
 paths=['splat.us.yaml','symbol_addrs.us.txt',row['source'],row['asm_authority'],row['tu']]
 for rel in paths:
  dest=tmp_path/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/rel,dest)
 # Synthetic sparse ROM fixture; real original-ROM gates are separate evidence.
 data=bytearray(0x8800)
 words=(0x27bdffe8,0xafbf0014,0x0c003590,0x24040400,0x8fbf0014,0x27bd0018,0x03e00008,0)
 data[0x8780:0x87a0]=b''.join(word.to_bytes(4,'big') for word in words)
 row['retail_extent_sha256']=hashlib.sha256(data[0x8700:0x8800]).hexdigest()
 (tmp_path/'baserom.us.z64').write_bytes(data)
 proof_path=tmp_path/row['asm_authority'];proof=json.loads(proof_path.read_text());proof['retail_extent_sha256']=row['retail_extent_sha256'];proof_path.write_text(json.dumps(proof,indent=2)+'\n');row['asm_authority_sha256']=hashlib.sha256(proof_path.read_bytes()).hexdigest()
 registry={'schema':1,'rom_slots':[],'storage_blocks':[],'text_boundaries':[row]};(tmp_path/'rom_owned_data.json').write_text(json.dumps(registry))
 return tmp_path,row

def test_zero_boundary_round_trip_and_absent_metadata_restore(repo):
 root,row=repo;text='SECTIONS { .main : {\n        build/us/src/rom/lib_8700.o(.text);\n    }\n}\n'
 rewritten=owned_data.rewrite_linker(text,root);assert rewritten==owned_data.rewrite_linker(rewritten,root)
 assert 'FILL(0x00000000);' in rewritten and ' + 0x100;' in rewritten
 reg=json.loads((root/'rom_owned_data.json').read_text());reg['text_boundaries']=[];(root/'rom_owned_data.json').write_text(json.dumps(reg))
 assert owned_data.rewrite_linker(rewritten,root)==text

def test_c_and_assembly_regeneration_use_real_owner(repo):
 root,row=repo;yaml=root/'splat.us.yaml';yaml.write_text(yaml.read_text().replace('[0x8700, c, rom/lib_8700]','[0x8700, asm]'))
 # Whole-segment revert deletes old nonmatching files; immutable authority survives.
 (root/row['tu']).unlink()
 rewritten=owned_text.rewrite_linker('        build/us/asm/us/8700.o(.text);\n',root)
 assert 'ROM_TEXT_BOUNDARY_BEGIN build/us/asm/us/8700.o(.text)' in rewritten
 assert 'build/us/src/rom/lib_8700.o' not in rewritten

def test_body_coverage_excludes_fill_and_keeps_static_denominator(repo,monkeypatch):
 root,row=repo;monkeypatch.setattr(layout,'REPO',root)
 mapping={'segments':[dict(yaml_name='0x8700',rom_tu='rom/lib_8700',converted=True,refusal=None,functions=[dict(name='osDpSetNextBuffer',size=128,state='promoted'),dict(name='osDpWait',size=128,state='passthrough')])]}
 before=layout.coverage(mapping);assert before['promoted_bytes']==128 and before['preserved_padding_bytes']==0
 shutil.copy(ROOT/'cloud/work/integration_B21/lib_8700.candidate.c',root/row['tu']);mapping['segments'][0]['functions'][1]['state']='promoted'
 after=layout.coverage(mapping);assert after['promoted_functions']-before['promoted_functions']==1
 assert after['promoted_bytes']-before['promoted_bytes']==32
 assert after['preserved_padding_bytes']==96 and after['promoted_slot_bytes']==256 and after['static_bytes']==before['static_bytes']==256

def test_alias_or_following_boundary_drift_refused(repo):
 root,row=repo;symbols=root/'symbol_addrs.us.txt';symbols.write_text(symbols.read_text().replace('osDpWait = 0x80007B80','osDpWait = 0x80007B84'))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)
 shutil.copy(ROOT/'symbol_addrs.us.txt',symbols);yaml=root/'splat.us.yaml';yaml.write_text(yaml.read_text().replace('[0x8800, c, rom/lib_8800]','[0x8810, c, rom/lib_8800]'))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)

def test_nonzero_padding_and_changed_canonical_body_refused(repo):
 root,row=repo;rom=root/'baserom.us.z64';data=bytearray(rom.read_bytes());data[0x87fc]=1;rom.write_bytes(data)
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)
 data[0x87fc]=0;rom.write_bytes(data);source=root/row['source'];source.write_text(source.read_text().replace('0x400','0x401'))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)

def test_lifecycle_package_preserves_unrelated_storage_and_rom_rows(repo):
 root,row=repo;original=json.loads((root/'rom_owned_data.json').read_text());paths=owned_data.promotion_paths(root,row['tu'])
 assert root/'rom_owned_data.json' in paths and root/row['asm_authority'] in paths and root/row['source'] in paths
 assert root/'tools/conveyor/pipeline/owned_text.py' in paths
 assert json.loads((root/'rom_owned_data.json').read_text())==original

def test_duplicate_boundaries_and_ambiguous_linker_input_refused(repo):
 root,row=repo;reg=json.loads((root/'rom_owned_data.json').read_text());reg['text_boundaries'].append(copy.deepcopy(row));(root/'rom_owned_data.json').write_text(json.dumps(reg))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)
 reg['text_boundaries'].pop();(root/'rom_owned_data.json').write_text(json.dumps(reg))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.rewrite_linker('build/us/src/rom/lib_8700.o(.text);\nbuild/us/src/rom/lib_8700.o(.text);\n',root)

def test_live_c_body_and_invented_padding_function_refused(repo):
 root,row=repo;tu=root/row['tu'];candidate=(ROOT/'cloud/work/integration_B21/lib_8700.candidate.c').read_text()
 tu.write_text(candidate.replace('0x400','0x401'))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)
 tu.write_text(candidate+'\nvoid invented_padding(void) {}\n')
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)

def test_invalid_body_stays_a_linker_dependency_and_inline_fill_is_refused(repo):
 root,row=repo;tu=root/row['tu'];tu.write_text((ROOT/'cloud/work/integration_B21/lib_8700.candidate.c').read_text()+'\n__asm__(".space 96");\n')
 assert row['tu'] in owned_text.dependency_paths(root)
 assert row['asm_authority'] in owned_text.dependency_paths(root)
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)


def test_reporting_without_rom_does_not_relax_linker_validation(repo,monkeypatch):
 root,row=repo;shutil.copy(ROOT/'cloud/work/integration_B21/lib_8700.candidate.c',root/row['tu']);(root/'baserom.us.z64').unlink()
 accounting=owned_text.accounting(root);assert accounting[(row['tu'],'osDpWait')]['logical_body_bytes']==32
 with pytest.raises(FileNotFoundError):owned_text.rewrite_linker('build/us/src/rom/lib_8700.o(.text);\n',root)


def test_standalone_mapping_and_body_hash_match_canonical_authority():
 from tools.conveyor.pipeline import lock
 assert owned_text.VRAM_DELTA==layout.VRAM_DELTA
 expected,_=layout.parse_subsegments(ROOT/'splat.us.yaml')
 assert owned_text._subsegments(ROOT/'splat.us.yaml')==[{k:r[k] for k in ('off','type','name')} for r in expected]
 for source,name in ((ROOT/'cloud/work/static_C14/osDpWait.c','osDpWait'),(ROOT/'src/rom/lib_cc50.c','dll_update')):
  assert owned_text.body_sha(source,name)==lock.body_sha(source,name)
 samples=['void f(){ /* comment */ return "a  b"; }', "void f(){char c='\\n';// comment\nreturn;}"]
 for sample in samples:assert owned_text.normalize_body(sample)==lock.normalize_body(sample)


def test_endpoint_proof_mutation_is_refused(repo):
 root,row=repo;proof=root/row['asm_authority'];text=proof.read_text();proof.write_text(text.replace('logical_body_bytes','wrong_field'))
 with pytest.raises(owned_text.TextBoundaryError):owned_text.boundaries(root)
