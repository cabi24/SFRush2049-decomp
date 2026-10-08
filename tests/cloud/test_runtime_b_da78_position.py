"""Portable tests for DA78's complete source-only private-child packet."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

import pytest
from tools.cloud import score

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/runtime_b_da78_position_20261006'


def test_packet_source_bindings():
    receipt=json.loads((PACKET/'verification.json').read_text())
    for name,expected in receipt['sources_sha256'].items():
        assert hashlib.sha256((PACKET/name).read_bytes()).hexdigest()==expected
    assert (PACKET/'position.c').read_text().splitlines()[0]== \
        '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert 'RESEARCH-ONLY' in receipt['status']
    assert len(receipt['negative_controls'])==13
    assert receipt['behavior']['native_unexecuted_offsets']==['0x20']


def test_current_selected_native_words(monkeypatch):
    monkeypatch.setattr(score,'ASM_DIR',ROOT/'asm/us/ovl_b')
    words=score.targets()['func_8038DA78']
    native=struct.pack('>'+str(len(words))+'I',*words)
    receipt=json.loads((PACKET/'verification.json').read_text())
    assert len(native)==receipt['target']['size']
    assert hashlib.sha256(native).hexdigest()==receipt['target']['sha256']


def test_native_projection_contract(monkeypatch):
    monkeypatch.setattr(score,'ASM_DIR',ROOT/'asm/us/ovl_b')
    words=score.targets()['func_8038DA78']
    spec=importlib.util.spec_from_file_location('native_da78_test',PACKET/'native.py')
    native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
    code={native.ENTRY+4*i:w for i,w in enumerate(words)}
    data=[(0x80394DDC,struct.pack('>ff',0.001,1.35))]
    # Nearest ties replace the earlier result, with the full pointer returned.
    m=native.Machine(code,native.ENTRY,data,dict(count=4),True)
    result=m.run()
    assert result[0]==native.PLAYERS+3*952
    assert m.vec(native.RECORD+32)==tuple(native.bits(x) for x in (5.,0.,0.))
    # Kind3 never reads its previous-position input.
    m=native.Machine(code,native.ENTRY,data,dict(kind=3),True)
    m.r[18]=0xDEAD0000
    assert m.run()[0]==0
    # Nonpositive signed counts produce no hit after entry geometry.
    m=native.Machine(code,native.ENTRY,data,dict(count=-1),True)
    assert m.run()[0]==0
    # At the exact threshold, the strict angle comparison leaves flags alone.
    for angle,expected in [(1.350000023841858,0x80),(1.3499999046325684,0x81)]:
        m=native.Machine(code,native.ENTRY,data,
                         dict(flags=0x80,angle=angle,players=[dict(kind=5)]),True)
        assert m.run()[0]==native.PLAYERS
        assert m.get(native.RECORD+7,1)==expected


def test_complete_behavior_replay():
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    reference=Path(os.environ.get('RUSH_REFERENCE_ROOT',str(ROOT)))
    result=subprocess.run([sys.executable,str(PACKET/'verify.py'),
                           '--reference-root',str(reference),'--check'],
                          text=True,capture_output=True)
    assert result.returncode==0,result.stdout+result.stderr
