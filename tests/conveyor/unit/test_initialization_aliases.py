import importlib.util
import shutil
import sys
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[3]
from tools.conveyor.pipeline import targets as T
PATHS = ['include/types.h', 'include/PR/os_time.h', 'src/rom/rom_tu.h',
         'reference/repos/ultralib/include/PR/ultratypes.h',
         'reference/repos/ultralib/include/PR/os_time.h',
         'reference/repos/ultralib/include/PR/R4300.h',
         'reference/repos/ultralib/src/os/initialize.c']
ADDRESSES = {'gAudioDmaCounter': 0x8002c360, 'gAudioDmaState': 0x8002c364,
             'g_tlb_exception_vector': 0x80000000, 'g_tlb_exception_instr1': 0x80000004,
             'g_tlb_exception_instr2': 0x80000008, 'g_tlb_exception_instr3': 0x8000000c}
LINES = ['lui $t0, %hi(g_tlb_exception_vector)',
         'sw $t1, %lo(g_tlb_exception_instr1)($t0)',
         'sw $t2, %lo(g_tlb_exception_instr2)($t0)',
         'sw $t3, %lo(g_tlb_exception_instr3)($t0)',
         'sw $v1, %lo(gAudioDmaState)($t0)',
         'sw $v0, %lo(gAudioDmaCounter)($t0)',
         'lw $a0, %lo(gAudioDmaStateOther)($t0)',
         'lw $a1, %lo(gAudioDmaState+4)($t0)']

@pytest.fixture
def setup(tmp_path, monkeypatch):
    if not all((ROOT/name).exists() for name in PATHS):
        pytest.skip("canonical SDK checkout absent")
    for name in PATHS:
        dest = tmp_path / name
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT / name, dest)
    monkeypatch.setattr(T, 'REPO', tmp_path)
    addresses = ADDRESSES.copy()
    monkeypatch.setattr(T, '_resolve_symbol', addresses.get)
    return tmp_path, addresses

def test_real_guards_and_operand_scope(setup):
    out = T._initialization_aliases(LINES, '__osInitialize_common')
    assert out[:5] == ['lui $t0, %hi(0x80000000)', 'sw $t1, %lo(0x80000004)($t0)',
                       'sw $t2, %lo(0x80000008)($t0)', 'sw $t3, %lo(0x8000000c)($t0)',
                       'sw $v1, %lo(gAudioDmaCounter+0x4)($t0)']
    assert out[5:] == LINES[5:]
    assert T._initialization_aliases(LINES, 'another_target') == LINES

@pytest.mark.parametrize('name', list(ADDRESSES))
@pytest.mark.parametrize('missing', [False, True])
def test_each_address_guard_is_atomic(setup, name, missing):
    _, addresses = setup
    if missing:
        del addresses[name]
    else:
        addresses[name] += 4
    assert T._initialization_aliases(LINES, '__osInitialize_common') == LINES

@pytest.mark.parametrize('path,old,new', [
    ('include/PR/os_time.h', 'typedef u64 OSTime;', 'typedef u32 OSTime;'),
    ('include/types.h', 'typedef unsigned int u32;', 'typedef unsigned short u32;'),
    ('reference/repos/ultralib/src/os/initialize.c', 'OSTime osClockRate', 'u32 osClockRate'),
    ('reference/repos/ultralib/src/os/initialize.c', 'unsigned int inst4;', 'unsigned short inst4;'),
    ('reference/repos/ultralib/include/PR/R4300.h', '#define\tUT_VEC\t\tK0BASE', '#define UT_VEC 0x80000004'),
    ('src/rom/rom_tu.h', '#define K0BASE 0x80000000', '#define K0BASE 0x80000004'),
])
def test_declaration_or_constant_drift_refuses(setup, path, old, new):
    repo, _ = setup
    p = repo / path
    source = p.read_text()
    assert old in source
    p.write_text(source.replace(old, new))
    assert T._initialization_aliases(LINES, '__osInitialize_common') == LINES

@pytest.mark.parametrize('path', PATHS)
def test_missing_authority_refuses(setup, path):
    repo, _ = setup
    (repo / path).unlink()
    assert T._initialization_aliases(LINES, '__osInitialize_common') == LINES
