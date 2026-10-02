import importlib.util,sys,shutil
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[3]
from tools.conveyor.pipeline import targets as T

def test_dll_init_reuses_only_verified_time_family(monkeypatch):
 monkeypatch.setattr(T, "_sdk_field_offsets", lambda kind: {"low_word": 4})
 lines=['lui $at, %hi(gViTimeAccumLo)','sw $t7, %lo(gViTimeAccumLo)($at)','sw $t6, %lo(gViTimeAccumHi)($at)']
 assert T._typed_data_field_aliases(lines,'dll_init')==['lui $at, %hi(gViTimeAccumHi+0x4)','sw $t7, %lo(gViTimeAccumHi+0x4)($at)',lines[2]]
 assert T._typed_data_field_aliases(lines,'unrelated')==lines
 monkeypatch.setattr(T,'_resolve_symbol',{'gViTimeAccumHi':0x80037c50,'gViTimeAccumLo':0x80037c58}.get)
 assert T._typed_data_field_aliases(lines,'dll_init')==lines

def test_dll_init_requires_real_sdk_scalar_types(tmp_path,monkeypatch):
 paths=['include/types.h','include/PR/os_time.h','reference/repos/ultralib/include/PR/ultratypes.h','reference/repos/ultralib/include/PR/os_time.h']
 if not all((ROOT/rel).exists() for rel in paths):pytest.skip('canonical SDK checkout absent')
 for rel in paths:
  out=tmp_path/rel;out.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/rel,out)
 monkeypatch.setattr(T,'REPO',tmp_path);monkeypatch.setattr(T,'_resolve_symbol',{'gViTimeAccumHi':0x80037c50,'gViTimeAccumLo':0x80037c54}.get)
 lines=['lui $at, %hi(gViTimeAccumLo)','sw $t7, %lo(gViTimeAccumLo)($at)']
 header=tmp_path/'include/PR/os_time.h';text=header.read_text();header.write_text(text.replace('typedef u64 OSTime;','typedef u32 OSTime;'))
 assert T._typed_data_field_aliases(lines,'dll_init')==lines
 header.write_text(text);(tmp_path/'reference/repos/ultralib/include/PR/os_time.h').unlink()
 assert T._typed_data_field_aliases(lines,'dll_init')==lines

def test_dll_init_original_sites_and_body_unchanged(monkeypatch):
 monkeypatch.setattr(T, "_sdk_field_offsets", lambda kind: {"low_word": 4})
 region=T.index_asm_regions()[0x8000c090];normalized=T._typed_data_field_aliases(region.lines,'dll_init')
 changed=[i for i,(a,b) in enumerate(zip(region.lines,normalized)) if a!=b]
 assert changed==[0,3] # exactly original HI and LO instruction sites
 assert len(region.words)==35
 assert all('gViTimeAccumHi+0x4' in normalized[i] for i in changed)
