"""Complete masked RNG source unit, independently linked and behavior checked."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/ipa-groups/dot_masked_random_b23e0_20261006'
spec=importlib.util.spec_from_file_location('masked_random_proof',PACKET/'verify.py')
proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)

@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
        pytest.skip('IDO, GNU MIPS tools and host compiler required')
    return proof.verify(tmp_path_factory.mktemp('masked-rng'))

def test_source_bound_receipt():
    saved=json.loads((PACKET/'verification.json').read_text())
    for name,digest in saved['source_sha256'].items():assert hashlib.sha256((PACKET/name).read_bytes()).hexdigest()==digest
    group=json.loads((PACKET/'group.json').read_text())
    assert group['claims']==group['members']==[proof.FN]
    assert group['context']==['func_8008B2E4','func_8008B2B4']
    assert set(group['keep'])==set(proof.NAMES)
    assert saved['new_candidate_bytes']==268 and saved['accepted_byte_gain']==0
    assert (PACKET/'rand.c').read_bytes()==proof.base_bytes('src/blob/func_8008B2B4.c')

def test_complete_portable_replay(replay):
    assert proof.portable(replay)==proof.portable(json.loads((PACKET/'verification.json').read_text()))

def test_exact_complete_functions_and_links(replay):
    assert [replay['bodies'][n]['symbol_bytes'] for n in proof.NAMES]==[48,72,268]
    assert all(b['full_extent_equal'] and b['all_references_resolved'] and b['full_differing_positions']==0 for b in replay['bodies'].values())
    assert replay['gnu_complete_bodies_equal']==proof.NAMES
    assert replay['production_reader_complete_bodies_equal']==proof.NAMES
    assert len(replay['relocations'])==10
    assert replay['zero_section_alignment_outside_functions']==12 and replay['no_owned_literals_or_data']

def test_causal_source_boundaries(replay):
    assert replay['archived_control']['differing']==13
    assert replay['causal_controls']['pre_cached_table'][proof.FN]['full_differing_positions']==14
    assert replay['causal_controls']['without_range_mask_effect']==11

def test_behavior_coverage_and_mutants(replay):
    b=replay['behavior']
    assert b['cases']==38032 and b['native_executions']==76064
    assert b['retry_cases']==4775 and b['maximum_iterations']==248
    assert len(b['executed_instruction_offsets'])==49
    assert b['conditional_outcomes']==[[144,True],[228,False],[252,False],[252,True]]
    assert b['all_32768_first_random_outputs'] and b['host_c89_ubsan']
    assert set(b['compiled_mutants_rejected'])=={'ignore_mask','wrong_table_index','skip_high_bit','wrong_denominator'}

def test_zero_mask_keeps_advancing_seed():
    sem=proof.sem
    trace=sem.Native(proof.score.targets()[proof.FN]).run(0xffffffff,0xabcd00ff,0,stop_after=128)
    seed=0xffffffff;want=[]
    for _ in range(128):seed=(seed*1103515245+12345)&0xffffffff;want.append(seed)
    assert trace==want

def test_u8_input_alias_and_bit31():
    sem=proof.sem
    native=sem.Native(proof.score.targets()[proof.FN])
    for arg in (0,255,0xffffffff,0xdeadbe00,0xabcdef1f):
        assert native.run(1,arg,0x80000000)==sem.oracle(1,arg,0x80000000)

def test_reject_unknown_instruction():
    words=list(proof.score.targets()[proof.FN]);words[0]=0xffffffff
    with pytest.raises(AssertionError,match='unsupported instruction'):proof.sem.Native(words).run(1,0,0xffffffff)

def test_reject_truncated_complete_body():
    with pytest.raises(AssertionError,match='complete 268'):proof.sem.Native(proof.score.targets()[proof.FN][:-1])

def test_reject_redirected_seed_write():
    words=list(proof.score.targets()[proof.FN]);words[0x64//4]^=4
    with pytest.raises(AssertionError,match='unexpected write'):proof.sem.Native(words).run(1,0,0xffffffff)

def test_reject_redirected_table_read():
    words=list(proof.score.targets()[proof.FN]);words[0x3c//4]^=4
    with pytest.raises(AssertionError,match='unexpected read'):proof.sem.Native(words).run(1,0,0xffffffff)

def test_reject_stack_home_corruption():
    words=list(proof.score.targets()[proof.FN]);words[0x2c//4]^=4
    with pytest.raises(AssertionError,match='unexpected write'):proof.sem.Native(words).run(1,0,0xffffffff)

def test_reject_missing_fcsr_restore():
    words=list(proof.score.targets()[proof.FN]);words[0xf8//4]=0
    with pytest.raises(AssertionError):proof.sem.Native(words).run(1,0,0xffffffff)

def test_fresh_data_manifest_required_after_warm_read(tmp_path,monkeypatch):
    original=proof.score.ASM_DIR.with_name('blob_data');data=tmp_path/'blob_data';shutil.copytree(original,data)
    monkeypatch.setattr(proof.score,'ASM_DIR',tmp_path/'blob')
    proof.image_window()
    artifact=data/proof.owndata.ARTIFACT_NAME;artifact.write_bytes(artifact.read_bytes()+b'\n')
    with pytest.raises(SystemExit,match='SHA-256 mismatch'):proof.image_window()

def test_wrong_function_extent_is_not_full_match(tmp_path):
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    obj=tmp_path/'candidate.o';proof.score.compile_group(PACKET,obj)
    raw,sections=proof.score._elf(obj);changed=bytearray(raw)
    found=False
    for sec in sections:
        if sec['type']!=2:continue
        names=sections[sec['link']]
        for pos in range(sec['off'],sec['off']+sec['size'],16):
            nameoff=struct.unpack_from('>I',raw,pos)[0]
            name=raw[names['off']+nameoff:].split(b'\0',1)[0]
            if name==proof.FN.encode():
                struct.pack_into('>I',changed,pos+8,264);found=True
    assert found;obj.write_bytes(changed)
    report=proof.inspect(obj,proof.FN)
    assert not report['full_extent_equal'] and report['full_differing_positions']==1


def test_live_source_and_lock_churn_does_not_change_base_context(monkeypatch):
    old_read=Path.read_bytes
    def changed(path):
        if path==ROOT/'src/blob/func_8008B2B4.c':return b'changed integration source'
        return old_read(path)
    monkeypatch.setattr(Path,'read_bytes',changed)
    assert (PACKET/'rand.c').read_bytes()==proof.base_bytes('src/blob/func_8008B2B4.c')


def test_matching_source_header_has_exact_group_recipe():
    assert (PACKET/'selector.c').read_text().splitlines()[0]=='/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert json.loads((PACKET/'group.json').read_text())['flags']==proof.FLAGS


def test_portable_receipt_keeps_packet_and_native_proof():
    import copy
    saved=json.loads((PACKET/'verification.json').read_text())
    changed=copy.deepcopy(saved)
    changed['target_manifest_historical_sha256']='0'*64
    changed['data_manifest_historical_sha256']='0'*64
    changed['tools_sha256']={}
    assert proof.portable(changed)==proof.portable(saved)
    changed['selected_native_body_sha256'][proof.FN]='0'*64
    assert proof.portable(changed)!=proof.portable(saved)
