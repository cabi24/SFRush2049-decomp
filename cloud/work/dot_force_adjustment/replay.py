#!/usr/bin/env python3
"""Canonical strict score plus GNU-linked raw-word and O32 layout replay."""
import dataclasses, hashlib, json, os, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];HERE=Path(__file__).resolve().parent
NAME='func_800E1AA0';FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
def main():
 sys.path.insert(0,str(ROOT/'tools/cloud'));import score
 with tempfile.TemporaryDirectory() as tmp:
  d=Path(tmp);obj=d/'candidate.o'
  subprocess.run([str(score.ido('cc')),*FLAGS.split(),'-c',str(HERE/'candidate.c'),'-o',str(obj)],check=True)
  canonical=dataclasses.asdict(score.compare(obj,NAME,show=0));native=score.targets()[NAME]
  script='SECTIONS { . = 0x800E1AA0; .text : { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) } }\nD_801243C0 = 0x801243C0;\n'
  (d/'link.ld').write_text(script)
  subprocess.run(['mips-linux-gnu-ld','-T',str(d/'link.ld'),'-o',str(d/'linked.elf'),str(obj)],check=True)
  subprocess.run(['mips-linux-gnu-objcopy','-O','binary','--only-section=.text',str(d/'linked.elf'),str(d/'text.bin')],check=True)
  raw=(d/'text.bin').read_bytes();got=list(struct.unpack('>%dI'%(len(raw)//4),raw));diff=[{'offset':i*4,'target':'%08x'%w,'candidate':'%08x'%got[i] if i<len(got) else None} for i,w in enumerate(native) if i>=len(got) or got[i]!=w]
  data,secs=score._elf(obj);symbols=[s for i,v in enumerate(secs) if v['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==NAME]
  layouts={'config':4,'direction':64,'height':72,'velocity':80,'force':320,'blend':980,'speed':1008,'bias':1452,'mode_a':1548,'mode_b':1552,'magnitude':1824,'direct':1994,'flags':2004}
  layout='#define offsetof(t,m) ((unsigned long) &(((t *)0)->m))\n#include "'+str(HERE/'candidate.c')+'"\n'+''.join('typedef char offset_%s[(offsetof(ForceState,%s)==%d)?1:-1];\n'%(k,k,v) for k,v in layouts.items())+'typedef char config_scale[(offsetof(ForceConfig,scale)==36)?1:-1];\n'
  (d/'layout.c').write_text(layout);subprocess.run([str(score.ido('cc')),*FLAGS.split(),'-c',str(d/'layout.c'),'-o',str(d/'layout.o')],check=True)
  assert len(diff)==canonical['differing']==40 and len(native)==100 and symbols[0]['size']==392
  result={'status':'NONMATCH','function':NAME,'flags':FLAGS,'source_sha256':hashlib.sha256((HERE/'candidate.c').read_bytes()).hexdigest(),'target_sha256':hashlib.sha256(struct.pack('>100I',*native)).hexdigest(),'target_bytes':400,'candidate_function_bytes':symbols[0]['size'],'candidate_text_bytes':len(raw),'canonical':canonical,'gnu_linked_differences':diff,'o32_layout_assertions':layouts,'relocations':'GNU ld resolves sole global D_801243C0 via linker script; no masks','claims':[]}
  print(json.dumps(result,indent=2))
  if len(sys.argv)>1:Path(sys.argv[1]).write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
