#!/usr/bin/env python3
"""Rebuild the complete bounded source experiment ledger, without raw bytes."""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import struct
import sys
import verify as v

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=v.ROOT);ap.add_argument('--tools-repo',type=Path,default=v.ROOT);ap.add_argument('--check',action='store_true');a=ap.parse_args()
 repo=a.repo.resolve();tr=a.tools_repo.resolve();build=v.ROOT/'build/runtime_a_menu_loop';build.mkdir(parents=True,exist_ok=True);tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
 import tempfile;tempfile.tempdir=str(tmp)
 sys.path.insert(0,str(tr/'tools/cloud'));spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';native=score.targets()[v.NAME];addresses=score.image_symbols();rows={}
 controls=[(s.stem,s,v.FLAGS) for s in sorted((v.HERE/'controls').glob('*.c'))]+[('final_O2',v.SOURCE,v.FLAGS),('final_O1',v.SOURCE,v.FLAGS.replace('-O2','-O1'))]
 for label,source,flags in controls:
  obj=build/'control.o';score.compile_single(source,flags,obj);sections,symbols,relocs=v.elf(obj);idx,row,raw=sections['.text'];f=symbols[v.NAME]
  assert f[0]==0 and f[2:]==(2,idx);size=f[1];assert size%4==0 and not any(raw[size:])
  assert {n for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}=={v.NAME}
  for n,(_,r,b) in sections.items():
   if r[2]&2 and n not in ('.text','.options','.reginfo'):assert r[5]==0,n
  assert all(off<size and off%4==0 and kind in (4,5,6) and name in v.ANCHORS and sid==idx for off,kind,name,sid in relocs)
  words,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),0,len(raw),addresses)
  assert not any((masks,unresolved,unverified,errors));resolved=struct.pack('>%dI'%len(words),*words)
  script=build/'control.ld';script.write_text('SECTIONS { .text 0x803A4340 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,x) for n,x in v.ANCHORS.items()))
  linked=build/'control.elf';v.shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj);ls,sy,lr=v.elf(linked)
  assert not lr and sy[v.NAME][0]==v.ENTRY and sy[v.NAME][1]==size and ls['.text'][1][3]==v.ENTRY and ls['.text'][2]==resolved
  actual=words[:size//4];diff=[i*4 for i in range(max(len(native),len(actual))) if (native[i] if i<len(native) else None)!=(actual[i] if i<len(actual) else None)]
  rows[label]={'source_sha256':v.sha(source.read_bytes()),'flags':flags,'function_bytes':size,'text_bytes':len(raw),'zero_alignment_bytes':len(raw)-size,'owned_data_bytes':0,'relocations':len(relocs),'full_body_differing_positions':len(diff),'difference_offsets':[hex(x) for x in diff],'gnu_linked_body_sha256':v.sha(resolved[:size]),'comparison':score.compare(obj,v.NAME,show=0).__dict__}
 result=json.loads(json.dumps({'native_sha256':v.NATIVE_HASH,'target_bytes':v.SIZE,'status':'All experiments NONMATCH; this replay does not resume source exploration.','experiments':rows}))
 out=v.HERE/'controls.json'
 if a.check:assert result==json.loads(out.read_text())
 else:out.write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({n:(r['function_bytes'],r['full_body_differing_positions']) for n,r in rows.items()},indent=2))
if __name__=='__main__':main()
