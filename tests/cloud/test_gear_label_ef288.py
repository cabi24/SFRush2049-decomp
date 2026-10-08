"""Portable source, behavioral and complete-ELF checks for the gear callback."""
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import tempfile
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_gear_label_ef288_20261006'
spec=importlib.util.spec_from_file_location('gear_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def proof_view(r):
    """Compare only packet promises, never changing manifests or live locks."""
    return {k:r[k] for k in ['status','base','function','start','end','native_bytes',
            'native_words_sha256','source_hashes','behavior','matching_bytes','claims',
            'base_context','compile','selector_ABI','flags','mandatory_backend_flag']}


def test_base_context_and_owned_source_pins():
    receipt=json.loads((HERE/'verification.json').read_text())
    assert v.audit_base_context()==receipt['base_context']
    for name,digest in receipt['source_hashes'].items():
        assert v.sha((HERE/name).read_bytes())==digest
    assert not any(name.startswith('test_') for name in receipt['source_hashes'])
    assert (HERE/'candidate.c').read_text().splitlines()[0]=='/* flags: '+v.FLAGS+' */'


def test_host_native_behavior(tmp_path):
    if not shutil.which('gcc'):pytest.skip('host C compiler required')
    result=v.behavior(tmp_path)
    assert result['cases']==2257
    assert result['host_equals_native']
    assert result['visited_native_words']==202 and result['unvisited_offsets']==[]


@pytest.fixture(scope='module')
def compiled_packet():
    if not (v.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    if not shutil.which('gcc'):pytest.skip('host C compiler required')
    with tempfile.TemporaryDirectory(prefix='gear-test-') as temp:
        work=Path(temp);obj=work/'group.o'
        v.score.compile_group(HERE,obj)
        yield obj,work


def test_canonical_replay(compiled_packet):
    obj,work=compiled_packet
    receipt=json.loads((HERE/'verification.json').read_text())
    proof,bodies=v.object_proof(obj,work)
    assert proof==receipt['compile']
    assert v.behavior(work,bodies[v.FN],17)==receipt['behavior']
    root=proof['functions'][v.FN]
    assert (root['symbol_bytes'],root['native_bytes'],root['complete_differing_words'])==(788,808,179)
    assert proof['owned_data_bytes']==0 and proof['GNU_project_placement_equal']
    assert sum(len(f['relocations']) for f in proof['functions'].values())==139


def test_reject_truncated_extent(compiled_packet):
    obj,work=compiled_packet;data,secs=v.score._elf(obj);bad=bytearray(data)
    for si,sec in enumerate(secs):
        if sec['type']==2:
            for i,s in enumerate(v.score._symbol_table(data,secs,si)):
                if s['name']==v.FN:struct.pack_into('>I',bad,sec['off']+i*16+8,s['size']-8)
    path=work/'short.o';path.write_bytes(bad)
    with pytest.raises((AssertionError,SystemExit)):v.object_proof(path,work)


def test_reject_unknown_relocation(compiled_packet):
    obj,work=compiled_packet;data,secs=v.score._elf(obj);bad=bytearray(data);ti=v.score._text_index(secs)
    sec=next(s for s in secs if s['type']==9 and s['info']==ti)
    info=struct.unpack_from('>I',data,sec['off']+4)[0]
    struct.pack_into('>I',bad,sec['off']+4,(info&~255)|255)
    path=work/'badrel.o';path.write_bytes(bad)
    with pytest.raises((AssertionError,SystemExit)):v.object_proof(path,work)


def test_reject_unproved_owned_data(compiled_packet):
    obj,work=compiled_packet;data,secs=v.score._elf(obj);bad=bytearray(data)
    # Rename an existing nonempty metadata section in the ELF string table;
    # this deliberately introduces unproved owned data without touching text.
    index=next(i for i,s in enumerate(secs) if s['name']=='.mdebug')
    assert secs[index]['size']>0
    at=bad.index(b'.mdebug\x00')
    bad[at:at+8]=b'.data\x00\x00\x00'
    path=work/'owndata.o';path.write_bytes(bad)
    with pytest.raises((AssertionError,SystemExit)):v.object_proof(path,work)


def test_native_return_and_private_abi_drills():
    words=v.score.targets()[v.FN]
    args=[1,1,0,0,42,0,1,12,13,50,60,-1,0,1,2]
    native=v.native.Machine(words,args).run()
    bad=list(words);bad[-1]=(bad[-1]&0xFFFF0000)|0
    assert v.native.Machine(bad,args).run()!=native
    assert v.native.Machine(words,args,4).run()!=native
