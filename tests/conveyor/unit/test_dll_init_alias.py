import importlib.util,sys,shutil
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[3]
from tools.conveyor.pipeline import targets as T

def test_dll_init_reuses_only_verified_time_family(monkeypatch):
 # w26a retired the label from symbol_addrs; the fold still serves pre-w26a asm text.
 monkeypatch.setattr(T, "_sdk_field_offsets", lambda kind: {"low_word": 4})
 monkeypatch.setattr(T,'_resolve_symbol',{'__osCurrentTime':0x80037c50,'gViTimeAccumLo':0x80037c54}.get)
 lines=['lui $at, %hi(gViTimeAccumLo)','sw $t7, %lo(gViTimeAccumLo)($at)','sw $t6, %lo(__osCurrentTime)($at)']
 assert T._typed_data_field_aliases(lines,'__osTimerServicesInit')==['lui $at, %hi(__osCurrentTime+0x4)','sw $t7, %lo(__osCurrentTime+0x4)($at)',lines[2]]
 assert T._typed_data_field_aliases(lines,'unrelated')==lines
 monkeypatch.setattr(T,'_resolve_symbol',{'__osCurrentTime':0x80037c50,'gViTimeAccumLo':0x80037c58}.get)
 assert T._typed_data_field_aliases(lines,'__osTimerServicesInit')==lines

def test_dll_init_requires_real_sdk_scalar_types(tmp_path,monkeypatch):
 paths=['include/types.h','include/PR/os_time.h','reference/repos/ultralib/include/PR/ultratypes.h','reference/repos/ultralib/include/PR/os_time.h']
 if not all((ROOT/rel).exists() for rel in paths):pytest.skip('canonical SDK checkout absent')
 for rel in paths:
  out=tmp_path/rel;out.parent.mkdir(parents=True,exist_ok=True);shutil.copy(ROOT/rel,out)
 monkeypatch.setattr(T,'REPO',tmp_path);monkeypatch.setattr(T,'_resolve_symbol',{'__osCurrentTime':0x80037c50,'gViTimeAccumLo':0x80037c54}.get)
 lines=['lui $at, %hi(gViTimeAccumLo)','sw $t7, %lo(gViTimeAccumLo)($at)']
 header=tmp_path/'include/PR/os_time.h';text=header.read_text();header.write_text(text.replace('typedef u64 OSTime;','typedef u32 OSTime;'))
 assert T._typed_data_field_aliases(lines,'__osTimerServicesInit')==lines
 header.write_text(text);(tmp_path/'reference/repos/ultralib/include/PR/os_time.h').unlink()
 assert T._typed_data_field_aliases(lines,'__osTimerServicesInit')==lines

def test_dll_init_original_sites_and_body_unchanged(monkeypatch):
 # w26a: the asm names the low word __osCurrentTime + 0x4 at exactly the original HI and
 # LO sites (reloc_addrs.us.txt), so the alias fold has nothing left to rewrite.
 monkeypatch.setattr(T, "_sdk_field_offsets", lambda kind: {"low_word": 4})
 region=T.index_asm_regions()[0x8000c090];normalized=T._typed_data_field_aliases(region.lines,'__osTimerServicesInit')
 assert normalized==region.lines
 sites=[i for i,line in enumerate(region.lines) if '__osCurrentTime + 0x4' in line]
 assert sites==[0,3]
 assert not any('gViTimeAccumLo' in line for line in region.lines)
 assert len(region.words)==35
