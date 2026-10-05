import json,sys,struct,subprocess,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT))
from tools.conveyor.pipeline import blob_layout,blob_group,blob_splice
p=Path(__file__).parent; obj=ROOT/'build/C120'/sys.argv[1] if len(sys.argv)>1 else ROOT/'build/C120/group.o'
l=blob_layout.load();slots={e['target_id']:e for r in l['regions'] for e in r['entries'] if e['kind']=='function'}
names=['func_800D1AB0','func_800F8EC8','car_setup_confirm'];image=(ROOT/'build/game_code.bin').read_bytes();base=0x80086A50
symbols=subprocess.check_output(['mips-linux-gnu-nm','-S',str(obj)],text=True);sizes={a[-1]:int(a[1],16) for x in symbols.splitlines() if len(a:=x.split())==4}
slices,ndx=blob_group.member_slices(obj,names,slots)
rows=[]
for n in names:
 row={'name':n,'slot_bytes':slots[n]['size'],'true_symbol_bytes':sizes.get(n)}
 try:
  body=blob_group.relocate(obj,slices,ndx,blob_splice.image_symbols(l),members=[n],image=(image,base))[n]
  original=image[slots[n]['vaddr']-base:slots[n]['vaddr']-base+slots[n]['size']]
  diffs=[i for i in range(len(body)//4) if body[i*4:i*4+4]!=original[i*4:i*4+4]]
  row.update(word_differences=len(diffs),difference_indices=diffs,frame=-struct.unpack('>h',body[2:4])[0] if body[:2]==b'\x27\xbd' else None)
  (ROOT/'build/C120'/(obj.stem+'.'+n+'.bin')).write_bytes(body)
 except Exception as e: row['refusal']=str(e)
 rows.append(row)
proof={'object':str(obj.relative_to(ROOT)),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'results':rows}
(p/(obj.stem+'.proof.json')).write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(proof,indent=2))
