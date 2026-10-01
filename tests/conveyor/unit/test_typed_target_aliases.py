import shutil,subprocess
from pathlib import Path
import pytest
from tools.conveyor.pipeline import targets as T

def test_time_alias_requires_sdk_types_and_both_authoritative_addresses(monkeypatch):
 lines=['lui $t1, %hi(gViTimeAccumLo)','lw $t1, %lo(gViTimeAccumLo)($t1)','lui $t0, %hi(gViTimeAccumHi)','lui $a0, %hi(gViTimeAccumLoOther)']
 addresses={'gViTimeAccumHi':0x80037c50,'gViTimeAccumLo':0x80037c54}
 monkeypatch.setattr(T,'_resolve_symbol',addresses.get)
 monkeypatch.setattr(T,'_sdk_field_offsets',lambda kind:{'low_word':4})
 result=T._typed_data_field_aliases(lines,'osGetTime')
 assert result[:2]==['lui $t1, %hi(gViTimeAccumHi+0x4)','lw $t1, %lo(gViTimeAccumHi+0x4)($t1)']
 assert result[2:]==lines[2:]
 addresses['gViTimeAccumLo']+=4
 assert T._typed_data_field_aliases(lines,'osGetTime')==lines
 addresses.pop('gViTimeAccumHi')
 assert T._typed_data_field_aliases(lines,'osGetTime')==lines
 monkeypatch.setattr(T,'_sdk_field_offsets',lambda kind:None)
 assert T._typed_data_field_aliases(lines,'osGetTime')==lines
 assert T._typed_data_field_aliases(lines,'another_target')==lines

def test_task_alias_family_is_atomic_and_retains_other_operands(monkeypatch):
 fields=('ucode','ucode_data','dram_stack','output_buff','output_buff_size','data_ptr','yield_data_ptr')
 offsets=(0x10,0x18,0x20,0x28,0x2c,0x30,0x38)
 addresses={'gViModeTempBuffer':0x80036790,**{'gViModePtr'+str(i):0x80036790+v for i,v in enumerate(offsets)}}
 lines=[f'lui $a0, %hi(gViModePtr{i})' for i in range(7)]+['lw $a0, %lo(other_symbol)($a0)','lw $a0, %lo(gViModePtr0+4)($a0)']
 monkeypatch.setattr(T,'_resolve_symbol',addresses.get);monkeypatch.setattr(T,'_sdk_field_offsets',lambda kind:dict(zip(fields,offsets)))
 result=T._typed_data_field_aliases(lines,'osViModeTableGet')
 assert result[:7]==[f'lui $a0, %hi(gViModeTempBuffer+{hex(v)})' for v in offsets]
 assert result[7:]==lines[7:]
 addresses['gViModePtr6']+=4
 assert T._typed_data_field_aliases(lines,'osViModeTableGet')==lines
 del addresses['gViModePtr6']
 assert T._typed_data_field_aliases(lines,'osViModeTableGet')==lines

def test_real_sdk_type_and_field_order_guard_rejects_drift(tmp_path,monkeypatch):
 repo=Path(__file__).resolve().parents[3]
 paths=['include/types.h','include/PR/os_time.h','reference/repos/ultralib/include/PR/ultratypes.h','reference/repos/ultralib/include/PR/os_time.h','reference/repos/ultralib/include/PR/sptask.h']
 if not all((repo/p).exists() for p in paths):pytest.skip('canonical SDK checkout absent')
 for rel in paths:
  dest=tmp_path/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy(repo/rel,dest)
 monkeypatch.setattr(T,'REPO',tmp_path)
 assert T._sdk_field_offsets('time')=={'low_word':4}
 task=T._sdk_field_offsets('task');assert [task[f] for f in ('ucode','ucode_data','dram_stack','output_buff','output_buff_size','data_ptr','yield_data_ptr')]==[0x10,0x18,0x20,0x28,0x2c,0x30,0x38]
 header=tmp_path/'include/types.h';original=header.read_text();header.write_text(original.replace('u64 *ucode;', 'u32 ucode;'))
 assert T._sdk_field_offsets('task') is None
 header.write_text(original.replace('    OSTask_t t;', '    u32 prefix; OSTask_t t;'))
 assert T._sdk_field_offsets('task') is None
 header.write_text(original)
 time=tmp_path/'include/PR/os_time.h';time.write_text(time.read_text().replace('typedef u64 OSTime;','typedef u32 OSTime;'))
 assert T._sdk_field_offsets('time') is None
 (tmp_path/'reference/repos/ultralib/include/PR/sptask.h').unlink()
 assert T._sdk_field_offsets('task') is None

@pytest.mark.skipif(not all(shutil.which('mips-linux-gnu-'+tool) for tool in ('as','ld','objcopy')),reason='MIPS binutils required')
@pytest.mark.parametrize('target,base,aliases', [('osGetTime','gViTimeAccumHi',[('gViTimeAccumLo',4)]),('osViModeTableGet','gViModeTempBuffer',[(f'gViModePtr{i}',v) for i,v in enumerate((0x10,0x18,0x20,0x28,0x2c,0x30,0x38))])])
def test_field_alias_link_preserves_every_unmasked_word(tmp_path,monkeypatch,target,base,aliases):
 address=0x80037c50 if target=='osGetTime' else 0x80036790
 mapping={base:address,**{alias:address+offset for alias,offset in aliases}}
 monkeypatch.setattr(T,'_resolve_symbol',mapping.get)
 monkeypatch.setattr(T,'_sdk_field_offsets',lambda kind: {'low_word':4} if kind=='time' else dict(zip(('ucode','ucode_data','dram_stack','output_buff','output_buff_size','data_ptr','yield_data_ptr'),[v for _,v in aliases])))
 lines=[];words=[]
 for alias,offset in aliases:
  lines += [f'lui $t0, %hi({alias})', f'lw $t1, %lo({alias})($t0)']
  words += [f'{0x3c080000|(((address+offset+0x8000)>>16)&0xffff):08X}',f'{0x8d090000|((address+offset)&0xffff):08X}']
 lines+=['jr $ra','nop'];words+=['03E00008','00000000']
 obj=tmp_path/'field.o';linked=tmp_path/'field.elf';raw=tmp_path/'text.bin'
 T.assemble_region(T.Region(target,0x80001000,lines,words),target,obj)
 subprocess.run(['mips-linux-gnu-ld','-Ttext=0x80001000','-e',target,'--defsym='+base+'='+hex(address),str(obj),'-o',str(linked)],check=True,capture_output=True)
 subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(linked),str(raw)],check=True)
 expected=b''.join(int(word,16).to_bytes(4,'big') for word in words)
 assert raw.read_bytes()[:len(expected)]==expected
 assert not any(raw.read_bytes()[len(expected):])
 assert T.gate_target(words,obj)==(True,None)
