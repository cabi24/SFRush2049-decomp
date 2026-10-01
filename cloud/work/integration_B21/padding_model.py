"""Private B21 evidence model; not a production ownership implementation."""
import hashlib,json,re
from pathlib import Path

def validate(repo,record):
 rom=(Path(repo)/'baserom.us.z64').read_bytes()
 start=int(record['rom_start'],0);size=int(record['extent_bytes'])
 slot=rom[start:start+size]
 if len(slot)!=size or hashlib.sha256(slot).hexdigest()!=record['retail_extent_sha256']:
  raise ValueError('original full TU retail extent differs')
 pad=slot[record['logical_end_offset']:]
 if len(pad)!=record['zero_fill_bytes'] or any(pad) or hashlib.sha256(pad).hexdigest()!=record['zero_fill_sha256']:
  raise ValueError('original trailing area is not the proven zero fill')
 asm=(Path(repo)/record['original_asm']).read_text()
 if hashlib.sha256(asm.encode()).hexdigest()!=record['original_asm_sha256']:
  raise ValueError('original body/endlabel authority changed')
 if f'endlabel {record["function"]}' not in asm:
  raise ValueError('original logical body endpoint absent')
 return record

def rewrite(text,record):
 marker='B21_PRIVATE_BOUNDARY_'+record['owner']
 text=re.sub(r'        /\* '+marker+r'_BEGIN \*/.*?        /\* '+marker+r'_END \*/\n','        '+record['input']+'\n',text,flags=re.S)
 line='        '+record['input']+'\n'
 if text.count(line)!=1:raise ValueError('owner input is missing or ambiguous')
 name='__b21_'+record['owner'];extent=record['extent_bytes'];logical=record['logical_end_offset'];start=record['vram_start']
 replacement='\n'.join([
  '        /* '+marker+'_BEGIN */',
  '        '+name+'_start = .;',
  line.rstrip(),
  '        '+name+'_input_end = .;',
  '        ASSERT(ABSOLUTE('+name+'_start) == '+start+', "B21 original TU start moved");',
  '        ASSERT('+name+'_input_end == '+name+'_start + '+hex(logical)+' || '+name+'_input_end == '+name+'_start + '+hex(extent)+', "B21 unexpected real input extent");',
  '        FILL(0x00000000);',
  '        . = '+name+'_start + '+hex(extent)+';',
  '        '+name+'_end = .;',
  '        /* '+marker+'_END */',''])
 return text.replace(line,replacement,1)

if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--repo',type=Path,required=True);p.add_argument('--record',type=Path,required=True);p.add_argument('--linker',type=Path,required=True)
 a=p.parse_args();r=validate(a.repo,json.loads(a.record.read_text()));a.linker.write_text(rewrite(a.linker.read_text(),r))
