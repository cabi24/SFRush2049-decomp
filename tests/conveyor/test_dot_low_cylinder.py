"""Host and fail-closed emulator regression tests for C588 research."""
import importlib.util
import subprocess
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/dot_low_cylinder'
def machine():
    spec=importlib.util.spec_from_file_location('low_cylinder_machine',HERE/'native_machine.py')
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module

def test_host_semantics(tmp_path):
    binary=tmp_path/'host'
    subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-DSTANDALONE','-fsanitize=undefined',str(HERE/'host_semantics.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

def test_interpreter_rejects_unknown_instruction():
    with pytest.raises(AssertionError):machine().execute([0xffffffff],0x1000,[],[0,0,0,0])

def test_interpreter_rejects_unmapped_read():
    with pytest.raises(AssertionError):machine().execute([0x84820000],0x1000,[],[0,0,0,0])

def test_interpreter_branch_likely_annuls():
    # bnel zero,zero,+1 skips an invalid delay-slot instruction, then returns.
    code=[0x54000001,0xffffffff,0x03e00008,0]
    machine().execute(code,0x1000,[],[0,0,0,0])
