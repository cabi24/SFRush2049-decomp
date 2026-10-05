"""Full-source regression for the donor-backed tire polynomial initializer."""
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import sys

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_tire_constants_20261005'
spec=importlib.util.spec_from_file_location('tire_constants_verify',PACKET/'verify.py')
proof=importlib.util.module_from_spec(spec);sys.modules[spec.name]=proof;spec.loader.exec_module(proof)


def toolchain():
    if not (proof.score.IDO/'cc').exists():pytest.skip('IDO missing')
    if not shutil.which('mips-linux-gnu-ld'):pytest.skip('MIPS binutils missing')
    if not shutil.which('cc'):pytest.skip('C compiler missing')


@pytest.fixture(scope='module')
def receipt():return json.loads((PACKET/'verification.json').read_text())


@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    toolchain();return proof.verify(tmp_path_factory.mktemp('tire-proof'),512)


def test_source_and_packet_hashes(receipt):
    assert receipt['source_sha256']==proof.sha(proof.SOURCE.read_bytes())
    for name,digest in receipt['packet_sha256'].items():assert proof.sha((PACKET/name).read_bytes())==digest
    assert receipt['status']=='STRICT_MATCH_CANDIDATE'
    assert receipt['candidate_bytes']==260 and receipt['accepted_byte_gain']==0


def test_complete_elf_gnu_and_literal_pool(replay,receipt):
    assert replay['object']==receipt['object']
    assert proof.complete(replay['object'])
    assert replay['object']['symbol_bytes']==260
    assert replay['object']['literal_bytes']==16
    assert replay['object']['excluded_zero_text_alignment_bytes']==12
    assert len(replay['object']['relocations'])==8


def test_unchanged_genuine_caller_context(replay,receipt):
    assert replay['context']==receipt['context']
    assert set(replay['context']['bodies'])=={'track_preview_handler','track_info_display','func_800D0A1C'}
    assert all(proof.complete(v) for v in replay['context']['bodies'].values())


def test_source_controls_and_reciprocal_rounding(replay,receipt):
    assert replay['controls']==receipt['controls']
    assert proof.complete(replay['controls']['o2'])
    assert replay['controls']['donor_multiply_l2']['differing']>0
    r=replay['controls']['float_first_m2_reciprocal']
    assert r['differing']==0 and r['unverified'] and not proof.complete(r)


def test_all_instructions_host_native_and_caller_layouts(replay):
    r=replay['behavior']
    assert r['cases']==512 and r['native_executions']==1024
    assert r['native_target_instructions_executed']==r['linked_instructions_executed']==65
    assert r['host_c89_ubsan']=='passed'
    assert set(r['layouts'])=={str(i) for i in range(-1,4)} and all(r['layouts'].values())
    assert len(r['negative_controls'])==6
    assert all(v['rejected'] for v in r['negative_controls'].values())


def test_two_actual_caller_sites():
    assert proof.callers()==[['track_info_display','0x800d0e54'],['track_info_display','0x800d0e68']]


def test_decoder_fails_closed():
    words=list(proof.score.targets()[proof.FN]);words[0]=0xffffffff
    case=next(proof.cases(1));owned=proof.score.own_data().read(proof.native.LITERAL,16)
    with pytest.raises(AssertionError,match='unsupported opcode'):
        proof.native.Machine(words,owned,*case).run()


def test_actual_object_instruction_and_literal_mutations(tmp_path):
    toolchain();obj=tmp_path/'candidate.o';proof.score.compile_single(proof.SOURCE,proof.FLAGS,obj)
    original,sections=proof.score._elf(obj)
    text=sections[proof.score._text_index(sections)]
    ro,=[s for s in sections if s['name']=='.rodata']
    for kind,offset in [('instruction',text['off']+0x94),('literal',ro['off']+7)]:
        bad=bytearray(original);bad[offset]^=1;path=tmp_path/(kind+'.o');path.write_bytes(bad)
        assert not proof.complete(proof.inspect(path,proof.FN))


def test_actual_elf_extent_mutation(tmp_path):
    toolchain();obj=tmp_path/'candidate.o';proof.score.compile_single(proof.SOURCE,proof.FLAGS,obj)
    data,sections=proof.score._elf(obj);bad=bytearray(data);changed=0
    for i,sec in enumerate(sections):
        if sec['type']!=2:continue
        for index,sym in enumerate(proof.score._symbol_table(data,sections,i)):
            if sym['name']==proof.FN and sym['type']==2:
                struct.pack_into('>I',bad,sec['off']+index*16+8,sym['size']-4);changed+=1
    assert changed==1
    path=tmp_path/'short.o';path.write_bytes(bad)
    assert not proof.complete(proof.inspect(path,proof.FN))


def test_registered_single_submission():
    from tools.cloud.check_submissions import commands
    jobs=list(commands(ROOT,['cloud/matches/track_preview_handler.c']))
    assert len(jobs)==1 and jobs[0][-1]=='--flags='+proof.FLAGS
