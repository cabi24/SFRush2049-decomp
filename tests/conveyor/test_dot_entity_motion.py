"""Portable behavioral regressions for the E4E4 research contribution."""
from pathlib import Path
import importlib.util
import subprocess
import shutil
import pytest

ROOT=Path(__file__).resolve().parents[2]
WORK=ROOT/'cloud/work/dot_entity_motion'

def machine():
    spec=importlib.util.spec_from_file_location('entity_motion_machine',WORK/'native_machine.py')
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module

def test_host_callback_contract(tmp_path):
    cc=shutil.which('cc')
    if not cc:pytest.skip('host C compiler unavailable')
    binary=tmp_path/'motion'
    subprocess.run([cc,'-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-DSTANDALONE',str(WORK/'host_semantics.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

def test_unknown_opcode_fails_closed():
    with pytest.raises(AssertionError,match='unsupported'):
        machine().execute([0xffffffff],0,[],[0,0,0,0])

def test_callback_delay_and_caller_clobber():
    # Save return address, call0x100 with a0=7 in delay slot, restore and return.
    words=[0x27bdfff8,0xafbf0004,0x0c000040,0x24040007,0x8fbf0004,0x27bd0008,0x03e00008,0]
    calls=[]
    def callback(destination,args,memory):calls.append((destination,args[0]))
    value,_,_,_=machine().execute(words,0,[],[0,0,0,0],callback)
    assert calls==[(0x100,7)]
    assert value==0xbad00002

def test_likely_annuls_unmapped_access():
    # bnel zero,zero,+1: false, skip load from unmapped zero.
    machine().execute([0x54000001,0x8c020000,0x03e00008,0],0,[],[0,0,0,0])

def test_missing_callback_handler_rejected():
    with pytest.raises(AssertionError,match='missing callback'):
        machine().execute([0x0c000040,0],0,[],[0,0,0,0])

def test_full_linked_native_host_replay():
    import os
    import sys
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').is_file() or not all(shutil.which(x) for x in ['cc','mips-linux-gnu-ld','mips-linux-gnu-objcopy','mips-linux-gnu-nm','mips-linux-gnu-readelf','mips-linux-gnu-objdump']):
        pytest.skip('pinned IDO and GNU MIPS tools required')
    subprocess.run([sys.executable,str(WORK/'verify.py')],check=True,capture_output=True,text=True)
    run=subprocess.run([sys.executable,str(WORK/'differential.py')],check=True,capture_output=True,text=True)
    assert '"cases": 5893' in run.stdout and '"result": "PASS"' in run.stdout
