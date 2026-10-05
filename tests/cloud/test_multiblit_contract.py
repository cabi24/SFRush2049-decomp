"""Source-bound NewMultiBlit proof and wrong-contract guards."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import tempfile
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_new_multiblit_20261005'
SOURCE=ROOT/'cloud/matches/sound_control.c'
def load(name,path):
 spec=importlib.util.spec_from_file_location(name,path);mod=importlib.util.module_from_spec(spec);spec.loader.exec_module(mod);return mod
V=load('multiblit_verifier',HERE/'verify.py')
R=json.loads((HERE/'verification.json').read_text())
def test_source_and_packet_binding():
 assert hashlib.sha256(SOURCE.read_bytes()).hexdigest()==R['source_sha256']
 for name,digest in R['packet_sha256'].items():
  assert hashlib.sha256((HERE/name).read_bytes()).hexdigest()==digest
 for name,digest in R['context_source_sha256'].items():
  assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest

def test_full_extent_and_link_receipt():
 assert R['object']==dict(differing=0,total=117,unresolved=[],unverified=[],errors=[],extra_words=0,
  canonical_verdict='MATCH',symbol_bytes=468,native_bytes=468,full_extent_equal=True,all_relocations_resolved=True)
 assert R['gnu_link']['full_symbol_equal'] and R['gnu_link']['symbol_bytes']==468
 assert R['text_alignment_bytes_outside_symbol']==12 and R['own_data_bytes']==0
 assert len(R['relocations'])==4 and all(r['type']==4 for r in R['relocations'])
 assert R['accepted_byte_gain']==0

def test_genuine_context_and_controls():
 assert set(R['real_context'])=={V.FN,*V.CONTEXT_NAMES}
 assert all(v['canonical_verdict']=='MATCH' and v['full_extent_equal'] and v['all_relocations_resolved'] for v in R['real_context'].values())
 assert R['controls']['archived_a78']['differing']==9
 assert R['controls']['o2']['differing']==115
 assert R['controls']['local_callback']['differing']==4
 assert R['controls']['cursor']['differing']==7
 assert R['scout']['differing']==14 and not R['scout']['full_extent_equal']

def test_native_oracle_full_corpus():
 native=V.sem.Native()
 for case in V.sem.cases():assert native.run(case)==V.sem.oracle(case)
 assert set(range(0,468,4))-native.coverage=={344}

def test_unreachable_load_has_no_direct_entry():
 proof=R['unreachable_instruction_proof']
 assert proof['offset']==344 and proof['preceding_unconditional_branch_offset']==336
 words=V.score.targets()[V.FN];branch=words[84]
 assert branch>>26==4 and ((branch>>16)&1023)==0
 destinations=[]
 for i,w in enumerate(words):
  if w>>26 in (1,4,5,6,7,20,21,22,23):destinations.append(i*4+4+4*V.sem.signed(w&65535,16))
 assert 344 not in destinations

def test_interpreter_fails_closed():
 with pytest.raises(AssertionError,match='unknown'):
  V.sem.Native([0xffffffff]*117).run(next(V.sem.cases()))

def test_actual_source_semantics_and_mutation_receipt():
 r=R['host_native_linked']
 assert r['cases']==3198 and r['native_runs']==6396 and r['host_ubsan']
 assert r['covered_native_instructions']==116 and r['unreachable_instruction_offsets']==[344]
 assert len(r['wrong_contract_rejections'])==6
 changed=SOURCE.read_text().replace('0x7fffffffU','0xffffffffU')
 assert hashlib.sha256(changed.encode()).hexdigest()!=R['source_sha256']

def test_no_qualifier_or_instruction_shaping():
 code='\n'.join(line for line in SOURCE.read_text().splitlines() if not line.lstrip().startswith('*'))
 assert 'volatile ' not in code and '__asm' not in code
 assert 'mblit[i]' in code and 'curblit->AnimFunc(curblit)' in code
 assert 'extern Blit *func_800B3704();' in code
 assert 's16 nblits' in code

def test_fresh_complete_compiler_replay():
 if not (Path(V.score.IDO)/'cc').exists() or not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
  pytest.skip('Pinned IDO, MIPS GNU linker and host compiler required')
 with tempfile.TemporaryDirectory(prefix='multiblit-test-') as d:r=V.verify(Path(d))
 for key in ['object','gnu_link','relocations','real_context','controls','host_native_linked','scout','packet_sha256']:
  assert r[key]==R[key]
