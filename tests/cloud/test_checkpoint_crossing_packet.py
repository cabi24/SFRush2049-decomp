"""Portable checks for the bounded D24C8 research packet."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/frontier/dot_checkpoint_crossing_20261006'
spec = importlib.util.spec_from_file_location('checkpoint_crossing_proof',PACKET/'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


def require_toolchain():
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')


@pytest.fixture(scope='module')
def replay():
    require_toolchain()
    return proof.verify()


def test_pinned_context_is_read_from_base():
    value=proof.context_at_base()
    assert value['base_commit']==proof.CONTEXT_BASE
    assert value['live_locks_not_assumed']


def test_native_extent_and_integrity():
    words=proof.score.targets()[proof.NAME]
    assert len(words)==280
    assert proof.sha(proof.words_bytes(words))==proof.EXPECTED


def test_receipt_exact_semantic_replay(replay):
    assert replay==json.loads((PACKET/'verification.json').read_text())
    assert replay['compiler']['comparison']['differing']==60
    assert replay['behavior']['native_words_covered']==280


def test_unknown_instruction_is_rejected():
    words=proof.score.targets()[proof.NAME][:]
    words[0]=0xFFFFFFFF
    case=proof.cases()[0]
    with pytest.raises(AssertionError):
        proof.native.execute(words,proof.native.fixture(case),case)


def test_bad_elf_extent_and_padding_rejected(tmp_path):
    require_toolchain()
    obj=tmp_path/'candidate.o'
    proof.score.compile_group(PACKET/'group',obj)
    data,sections=proof.score._elf(obj)
    text=sections[proof.score._text_index(sections)]
    altered=bytearray(data);altered[text['off']+text['size']-1]=1
    bad=tmp_path/'bad.o';bad.write_bytes(altered)
    with pytest.raises(AssertionError,match='alignment tail'):
        proof.elf(bad)
    symbols=next(s for s in sections if s['type']==2)
    names=sections[symbols['link']]
    import struct
    altered=bytearray(data)
    found=False
    for off in range(symbols['off'],symbols['off']+symbols['size'],16):
        nameoff,value,size,info,other,section=struct.unpack_from('>IIIBBH',data,off)
        name=data[names['off']+nameoff:].split(b'\0',1)[0].decode()
        if name==proof.NAME:
            struct.pack_into('>I',altered,off+8,size-4);found=True
    assert found
    bad.write_bytes(altered)
    with pytest.raises(AssertionError):proof.elf(bad)


def test_absent_toolchain_cli_skips(tmp_path):
    import os
    env=dict(os.environ,IDO_DIR=str(tmp_path/'missing'))
    result=subprocess.run([sys.executable,str(PACKET/'verify.py'),'--check'],env=env,
                          text=True,capture_output=True,check=True)
    assert 'SKIP: pinned IDO and MIPS GNU linker required' in result.stdout


def test_independent_unchanged_host_and_source_mutants():
    require_toolchain()
    if not shutil.which('cc'):
        pytest.skip('host C compiler required')
    spec=importlib.util.spec_from_file_location('checkpoint_crossing_independent',
                                              PACKET/'independent/verify_host.py')
    independent=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(independent)
    assert independent.verify()==json.loads((PACKET/'independent/verification.json').read_text())
