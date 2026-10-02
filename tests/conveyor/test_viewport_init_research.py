"""Semantic regressions for NONMATCH viewport initialization research."""
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess
import pytest

ROOT=Path(__file__).resolve().parents[2]
SOURCE=ROOT/'cloud/work/dot_viewport_init'

def module(path, name):
    spec=importlib.util.spec_from_file_location(name,path)
    value=importlib.util.module_from_spec(spec);spec.loader.exec_module(value);return value

def test_native_arguments_and_postcallback_reload():
    vm=module(SOURCE/'verify_semantics.py','viewport_vm')
    # Load words directly from the protected, integrity-checked repository targets.
    import sys
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    import score
    words=score.targets()[vm.NAME]
    values=[-2147483648,-2147483647,-65537,-3,-1,0,1,3,65535,16777217,2147483647]
    for width in values:
        for height in values:
            got=vm.execute(words,(width,height),(~width,~height))
            half=lambda x: -(abs(x)//2) if x<0 else x//2
            assert got==[0,vm.V,vm.B,0x12345678,vm.fbits(0),vm.fbits(width),vm.fbits(height),vm.fbits(half(width)),vm.fbits(half(height)),(~width)&65535,(~height)&65535]

def test_interpreter_rejects_unknown_instruction():
    vm=module(SOURCE/'verify_semantics.py','viewport_vm_negative')
    with pytest.raises(AssertionError,match='Unsupported'):
        vm.execute([0xFFFFFFFF],(0,0),(0,0))

def test_host_c89_and_undefined_behavior_sanitizer(tmp_path):
    cc=shutil.which('cc')
    if not cc:pytest.skip('C compiler unavailable')
    executable=tmp_path/'host'
    subprocess.run([cc,'-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O1','-fsanitize=undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(SOURCE/'host.c'),'-o',str(executable)],check=True)
    subprocess.run([str(executable)],check=True)
