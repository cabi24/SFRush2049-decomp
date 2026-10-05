"""Source-admissible camera match, complete bytes, live context and bounded semantics."""
import hashlib,importlib.util,json,os,shutil,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_steady_camera_20261005'
sys.path.insert(0,str(HERE))
spec=importlib.util.spec_from_file_location('steady_camera_packet',HERE/'verify.py')
packet=importlib.util.module_from_spec(spec);sys.modules[spec.name]=packet;spec.loader.exec_module(packet)

def saved():return json.loads((HERE/'verification.json').read_text())

def test_frozen_source_inputs():
 r=saved();assert r['status']=='MATCH' and r['claims']==[packet.FN] and r['accepted_byte_gain']==0
 for path,digest in r['inputs'].items():assert hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest
 for name,digest in r['source_sha256'].items():assert hashlib.sha256((HERE/name).read_bytes()).hexdigest()==digest

def test_whole_elf_gnu_and_literal_accounting():
 r=saved();p=r['group'][packet.FN]
 assert p['differing']==0 and p['elf_function_bytes']==p['native_bytes']==1684
 assert p['zero_alignment_bytes']==12 and len(p['relocations'])==54
 assert p['unresolved']==p['unverified']==p['errors']==[]
 assert p['own_placements']=={'.rodata':[[12,16,0x801244d0,'rodata']]}
 assert r['gnu'][packet.FN]['all_function_bytes_equal']
 assert all(c['owner']=='func_800E92C8' and c['relative_addend']==0 for c in r['gnu'][packet.FN]['equivalent_named_call_relocations'])

def test_accepted_context_and_fixed_causal_controls():
 r=saved()
 for n,size in zip(packet.CONTEXT,[788,468,280]):
  assert r['group'][n]['differing']==0 and r['group'][n]['elf_function_bytes']==size
  assert r['gnu'][n]['all_function_bytes_equal']
 for n,p in r['controls'].items():
  assert p[packet.FN]['differing']==(0 if n=='all_split' else 112)
  assert p[packet.FN]['elf_function_bytes']==1684
  assert all(p[c]['differing']==0 for c in packet.CONTEXT)

def test_no_added_matching_devices_or_declaration_changes(tmp_path):
 import steady_camera_experiments as experiments
 s=(HERE/'candidate.c').read_text();assert all(x not in s for x in ['volatile ','__asm','__inline','if (0)','pad[','__standin'])
 original=(packet.GROUP/'group.c').read_text();a,b=packet.body_bounds(original)
 assert s[s.index('void '+packet.FN):].strip()==experiments.split_body(original[a:b],{0,1,2}).strip()
 group=packet.make_group(tmp_path/'group');built=(group/'group.c').read_text()
 assert built==original[:a]+s+original[b:]
 assert (group/'group.json').read_bytes()==(packet.GROUP/'group.json').read_bytes()

def test_native_contract_coverage_and_wrong_contract_controls():
 r=saved()['semantics'];assert r['cases']==720 and r['native_and_GNU_runs']==1440
 assert r['native_covered_words']==420 and r['native_total_words']==421
 assert r['native_executed_offsets']==r['native_cfg_reachable_offsets']
 assert set(range(0,1684,4))-set(r['native_executed_offsets'])=={0x400}
 assert r['host_c89_ubsan']==r['independent_scalar_oracle']=='passed'
 assert len(r['real_native_helper_bodies'])==4 and len(r['rejected_mutants'])==5

def test_rejected_witnesses_agree_with_native():
 import steady_camera_semantics as semantics
 from steady_camera_native import Machine
 for case in saved()['semantics']['rejected_mutants'].values():
  assert Machine(packet.score.targets()[packet.FN],case).run()==semantics.oracle(case)

def test_fresh_compiler_gnu_and_semantic_replay(tmp_path):
 available=(packet.score.IDO/'cc').exists() and all(shutil.which(n) for n in ['cc','mips-linux-gnu-ld','mips-linux-gnu-objcopy'])
 if not available:
  if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required IDO/GNU/host tools missing')
  pytest.skip('IDO/GNU/host toolchain unavailable')
 fresh,_=packet.verify(tmp_path);expected=saved()
 for r in [fresh,expected]:r.pop('target_manifest_sha256')
 assert fresh==expected

def test_real_elf_literal_instruction_and_extent_tampering_is_rejected(tmp_path):
 import struct
 if not (packet.score.IDO/'cc').exists() or not shutil.which('mips-linux-gnu-ld'):
  pytest.skip('IDO/GNU toolchain unavailable')
 group=packet.make_group(tmp_path/'group');obj=tmp_path/'group.o';packet.score.compile_group(group,obj)
 original,secs=packet.score._elf(obj);fn=packet.symbol(obj,packet.FN)
 text=secs[packet.score._text_index(secs)];rodata=next(s for s in secs if s['name']=='.rodata')
 literal=bytearray(original);literal[rodata['off']+12]^=1
 insn=bytearray(original);insn[text['off']+fn['value']+3]^=4
 for label,data in [('literal',literal),('instruction',insn)]:
  bad=tmp_path/(label+'.o');bad.write_bytes(data)
  assert not packet.score.compare(bad,packet.FN,show=0).accepted()
  with pytest.raises(AssertionError):packet.gnu(bad,tmp_path/label,packet.FN)
 extent=bytearray(original)
 for si,sec in enumerate(secs):
  if sec['type']!=2:continue
  for index,s in enumerate(packet.score._symbol_table(original,secs,si)):
   if s['name']==packet.FN and s['type']==2:
    struct.pack_into('>I',extent,sec['off']+16*index+8,1680)
 bad=tmp_path/'extent.o';bad.write_bytes(extent)
 with pytest.raises(AssertionError):packet.gnu(bad,tmp_path/'extent',packet.FN)


def test_registered_submission_is_bound_and_discoverable(tmp_path):
 from tools.cloud import check_submissions
 derived=packet.make_group(tmp_path/'derived')
 registered=packet.registered_group(derived)
 paths=[str((registered/n).relative_to(ROOT)) for n in ('group.c','group.json')]
 jobs=list(check_submissions.commands(ROOT,paths))
 assert len(jobs)==1
 assert jobs[0][2:]==['group',str(registered),'--claims']
 assert all(p in saved()['inputs'] for p in paths)
