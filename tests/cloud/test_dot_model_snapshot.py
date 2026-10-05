"""Complete-source regression for the donor-backed model snapshot candidate."""
import importlib.util
import json
from pathlib import Path
import shutil
import sys

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_model_snapshot_20261005'
spec=importlib.util.spec_from_file_location('model_snapshot_verify',PACKET/'verify.py')
proof=importlib.util.module_from_spec(spec);sys.modules[spec.name]=proof;spec.loader.exec_module(proof)


def toolchain():
    if not Path(proof.score.ido('cc')).exists():pytest.skip('IDO missing')
    if not shutil.which('mips-linux-gnu-ld'):pytest.skip('MIPS binutils missing')
    if not shutil.which('cc'):pytest.skip('C compiler missing')


@pytest.fixture(scope='module')
def receipt():return json.loads((PACKET/'verification.json').read_text())


@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    toolchain();work=tmp_path_factory.mktemp('snapshot-proof')
    return proof.verify(work,512)


def test_bound_source_and_proof(receipt):
    assert receipt['source_sha256']==proof.sha(proof.SOURCE.read_bytes())
    for name,digest in receipt['packet_sha256'].items():assert proof.sha((PACKET/name).read_bytes())==digest
    assert receipt['candidate_bytes']==248 and receipt['accepted_byte_gain']==0
    assert receipt['status']=='STRICT_MATCH_CANDIDATE'


def test_complete_symbol_relocations_and_owned_literal(replay,receipt):
    assert replay['object']==receipt['object']
    assert proof.complete(replay['object'])
    assert replay['object']['own_literal_bytes']==4
    assert len(replay['object']['relocations'])==3


def test_genuine_five_body_context(replay,receipt):
    assert replay['context']==receipt['context']
    assert len(replay['context']['bodies'])==5
    assert all(proof.complete(r) for r in replay['context']['bodies'].values())


def test_donor_macro_causal_controls(replay,receipt):
    assert replay['controls']==receipt['controls']
    assert replay['controls']['archived_a98']['differing']==4
    assert proof.complete(replay['controls']['authentic_final_vector_macro_only'])


def test_complete_native_linked_host_behavior(replay):
    result=replay['behavior']
    assert result['cases']==512
    assert result['native_target_instructions_executed']==62
    assert result['real_native_callee_instructions_executed']==19
    assert result['host_c89_ubsan']=='passed'
    assert len(result['negative_controls'])==5
    assert all(x['rejected'] for x in result['negative_controls'].values())


def test_wrong_literal_rejected(tmp_path):
    toolchain();bad=tmp_path/'bad.c';bad.write_text(proof.SOURCE.read_text().replace('2.72727275f);','2.0f);'))
    obj=tmp_path/'bad.o';proof.score.compile_single(bad,proof.FLAGS,obj)
    assert not proof.complete(proof.inspect(obj,proof.FN))


def test_native_unmodeled_instruction_fails_closed():
    words=list(proof.score.targets()[proof.FN]);words[0]=0xffffffff
    initial=next(proof.cases(1))
    machine=proof.native.Machine(words,proof.score.targets()['math_utility'],
                                  proof.score.own_data().read(proof.native.LITERAL,4),initial)
    with pytest.raises(AssertionError,match='opcode'):machine.run()


def test_real_direct_caller_is_recorded():
    names=proof.score.image_symbols();target=names[proof.FN];found=[]
    for name,words in proof.score.targets().items():
        for i,w in enumerate(words):
            if w>>26==3 and ((((names[name]+4*i+4)&0xf0000000)|((w&0x3ffffff)<<2))==target):
                found.append((name,hex(names[name]+4*i)))
    assert found==[('players_race_update','0x800d50b0')]
