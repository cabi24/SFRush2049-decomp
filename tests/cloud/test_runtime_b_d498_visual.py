"""Portable tests for D498's complete source-only private-helper packet."""
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
PACKET=ROOT/'cloud/work/runtime_b_d498_visual_20261006'


def test_packet_source_bindings():
    receipt=json.loads((PACKET/'verification.json').read_text())
    for name,expected in receipt['sources_sha256'].items():
        assert hashlib.sha256((PACKET/name).read_bytes()).hexdigest()==expected
    assert (PACKET/'visual.c').read_text().splitlines()[0]== \
        '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert 'RESEARCH-ONLY' in receipt['status']
    assert len(receipt['negative_controls'])==17
    assert receipt['behavior']['paired_cases']==1658
    assert receipt['behavior']['native_unexecuted_offsets']==['0x88','0xb0']
    assert receipt['behavior']['candidate_unexecuted_offsets']==['0xf8']


def test_current_selected_native_words(monkeypatch):
    monkeypatch.setattr(score,'ASM_DIR',ROOT/'asm/us/ovl_b')
    words=score.targets()['func_8038D498']
    native=struct.pack('>'+str(len(words))+'I',*words)
    receipt=json.loads((PACKET/'verification.json').read_text())
    assert len(native)==receipt['target']['size']==768
    assert hashlib.sha256(native).hexdigest()==receipt['target']['sha256']


def test_native_tiers_mask_and_impulse_contract(monkeypatch):
    monkeypatch.setattr(score,'ASM_DIR',ROOT/'asm/us/ovl_b')
    words=score.targets()['func_8038D498']
    spec=importlib.util.spec_from_file_location('native_d498_test',PACKET/'native.py')
    native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
    code={native.ENTRY+4*i:w for i,w in enumerate(words)}
    data=[(0x80394DC8,struct.pack('>fff',0.6,66000.,330000.))]
    for scale,damage in ((8.,800),(8.000000953674316,400),(20.,400),(20.000001907348633,200)):
        m=native.Machine(code,native.ENTRY,data,dict(scale=scale),True)
        m.run()
        assert m.trace[0][:4]==('damage',native.PLAYERS+3*952,native.PLAYERS,damage)
        assert m.get(native.RECORD+5,1)==1
        # Addition of positive zero clears a pre-existing negative-zero y.
        assert m.get(native.VEHICLES+0x140)==0
    m=native.Machine(code,native.ENTRY,data,dict(mask=0x80,players=[dict(owner=31)]),True)
    m.run();assert not m.trace
    m=native.Machine(code,native.ENTRY,data,dict(count=4,players=[dict(owner=1)]*4),True)
    m.run();assert len(m.trace)==2
    # Both helpers precede a reloaded owner-indexed impulse destination.
    m=native.Machine(code,native.ENTRY,data,dict(mutate_transform=1,new_player_owner=2,new_count=0),True)
    m.run();assert m.get(native.VEHICLES+2*2056+0x124)!=native.bits(1.)
    assert m.get(native.VEHICLES+0x124)==native.bits(1.)


def test_complete_behavior_replay():
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    reference=Path(os.environ.get('RUSH_REFERENCE_ROOT',str(ROOT)))
    result=subprocess.run([sys.executable,str(PACKET/'verify.py'),
                           '--reference-root',str(reference),'--check'],
                          text=True,capture_output=True)
    assert result.returncode==0,result.stdout+result.stderr
