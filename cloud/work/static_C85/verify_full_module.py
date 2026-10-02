"""Independently verify native formatter placement, all body words and true data extents.
Logical table bytes and compiler alignment tail are reported separately; no
publication or canonical target mutation occurs.
"""
from pathlib import Path
import hashlib, json, re, struct, subprocess, sys
sys.path[:0] = ['.', 'tools/cloud']
import score
from tools.conveyor.pipeline import layout
p = Path('build/C85')
seg = next(s for s in layout.derive()['segments'] if s['rom_tu'] == 'rom/lib_34a0')
names = {f['name'] for f in seg['functions']}
text_base = int(seg['vram_start'], 16)
addresses = {n: int(a,16) for n,a in re.findall(r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)', Path('symbol_addrs.us.txt').read_text()+'\n'+Path('undefined_syms_auto.us.txt').read_text(),re.M)}
ld = f'SECTIONS {{ .text 0x{text_base:x} : {{ *(.text) }} .data 0x8002d4d0 : {{ *(.data) }} .rodata 0x8002d558 : SUBALIGN(4) {{ *(.rodata) }} /DISCARD/ : {{ *(.bss) *(.reginfo) *(.mdebug) *(.comment) *(.pdr) }} }}\n'
ld += '\n'.join(f'{n} = 0x{a:x};' for n,a in addresses.items() if n not in names)+'\n'
(p/'full_original_sections.ld').write_text(ld)
linked = p/'full_linked.elf'
subprocess.run(['mips-linux-gnu-ld','-T',str(p/'full_original_sections.ld'),'-o',str(linked),str(p/'lib_34a0.o')],check=True)
words = score.text_words(linked)
original_asm = Path('asm/us/34A0.s').read_text()
raw = {int(a,16):int(w,16) for a,w in re.findall(r'/\*\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+\*/', original_asm)}
data,secs = score._elf(linked)
# IDO symbol extents are object-relative; final ELF symbols establish actual linked placement.
linked_symbols = score.symbols(linked)
members=[]
for f in seg['functions']:
 addr=int(f['vaddr'],16);size=f['size'];start=(addr-text_base)//4
 expected=[raw[a] for a in range(addr,addr+size,4)]
 actual=words[start:start+size//4]
 r={'function':f['name'],'original_slot_words':size//4,'actual_vaddr':hex(linked_symbols[f['name']]),'slot_delta_bytes':linked_symbols[f['name']]-addr,'differing_full_words':sum(a!=b for a,b in zip(actual,expected))+abs(len(actual)-len(expected))}
 r['exact']=r['slot_delta_bytes']==r['differing_full_words']==0;members.append(r)
 assert r['exact'],r
rom=Path('baserom.us.z64').read_bytes();sections=[]
for name,base,extent in [('.rodata',0x8002d558,356)]:
 sec=next(s for s in secs if s['name']==name);payload=data[sec['off']:sec['off']+sec['size']];offset=base-0x7ffff400;original=rom[offset:offset+extent];tail=payload[extent:];neighbor=rom[offset+extent:offset+len(payload)]
 r={'section':name,'original_vaddr':hex(base),'original_logical_bytes':extent,'emitted_section_bytes':len(payload),'differing_logical_bytes':sum(a!=b for a,b in zip(original,payload))+max(0,extent-len(payload)),'compiler_alignment_tail_bytes':len(tail),'compiler_alignment_tail_nonzero':sum(bool(b) for b in tail),'following_original_interval_nonzero_bytes':sum(bool(b) for b in neighbor),'original_sha256':hashlib.sha256(original).hexdigest(),'following_original_interval_sha256':hashlib.sha256(neighbor).hexdigest()}
 assert r['differing_logical_bytes']==r['compiler_alignment_tail_nonzero']==0,r
 sections.append(r)
assert not any(s['name'] in ['.data','.bss','.rdata'] and s['size'] for s in secs)
r={'module':'lib_34a0','source_sha256':hashlib.sha256(Path('cloud/work/static_C85/lib_34a0.original_templates.full_native.c').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256((p/'lib_34a0.o').read_bytes()).hexdigest(),'original_slot_bytes':seg['size'],'object_text_bytes':len(words)*4,'full_native':True,'members':members,'all_members_exact':all(f['exact'] for f in members),'sections':sections,'publication_refusal':'4-byte logical table extent unsupported by current 16-byte owner-slot validator; 12-byte compiler tail must not replace following original nonzero bytes','claims':[]}
assert r['original_slot_bytes']==r['object_text_bytes']
Path('cloud/work/static_C85/full_module_proofs.json').write_text(json.dumps(r,indent=2)+'\n')
print(json.dumps({k:r[k] for k in ['module','original_slot_bytes','object_text_bytes','all_members_exact','sections']},indent=2))
