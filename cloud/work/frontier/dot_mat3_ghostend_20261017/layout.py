"""Read IDO-emitted native layout values rather than trusting typedef warnings."""
from pathlib import Path
import struct
FACTS=[('header_bytes','sizeof(GhostHeader)',88),('header_flag','OFF(GhostHeader,flag)',5),('header_finish','OFF(GhostHeader,finish)',56),('header_start','OFF(GhostHeader,start)',76),('header_offset','OFF(GhostHeader,offset)',80),('header_size','OFF(GhostHeader,size)',84),('ghost_bytes','sizeof(Ghost)',44),('ghost_data_handle','OFF(Ghost,datah)',36),('ghost_header_handle','OFF(Ghost,header)',40),('car_bytes','sizeof(GhostPlayer)',952),('car_locked','OFF(GhostPlayer,ready)',239),('car_score','OFF(GhostPlayer,value)',240),('model_bytes','sizeof(GhostObject)',2056),('model_run','OFF(GhostObject,reset)',1820),('model_run_bytes','sizeof(((GhostObject*)0)->reset)',2),('header_flag_signed','(s8)-1',0xffffffff)]
def verify(score,source,out,require):
 head=source[:source.index('extern Ghost **D_80152698')]
 mutations={'native':head,'run_byte':head.replace('s16 reset; u8 tail[234];','s8 reset; u8 tail[235];'),'car_stride':head.replace('u8 tail[708];','u8 tail[712];'),'header_shift':head.replace('u8 pad23[0x28 - 0x23];','u8 pad23[0x2C - 0x23];')};result={}
 for tag,s in mutations.items():
  require(tag=='native' or s!=head,'layout mutation anchor');p=out/tag;p.mkdir(parents=True,exist_ok=True)
  (p/'probe.c').write_text(s+'\n#define OFF(t,m) ((u32)&((t*)0)->m)\nu32 values[]={'+','.join(x[1] for x in FACTS)+'};\n')
  score.compile_single(p/'probe.c','-g0 -O3 -mips2 -G 0 -non_shared',p/'probe.o')
  data,secs=score._elf(p/'probe.o');es=[e for i,sec in enumerate(secs) if sec['type']==2 for e in score._symbol_table(data,secs,i) if e['name']=='values'];require(len(es)==1,'layout symbol');e=es[0];require(e['size']==4*len(FACTS),'layout extent');sec=secs[e['section']];values=struct.unpack('>%dI'%len(FACTS),data[sec['off']+e['value']:sec['off']+e['value']+e['size']]);bad=[name for (name,_,want),got in zip(FACTS,values) if want!=got];require(bool(bad)==(tag!='native'),'layout control');result[tag]={'values':dict(zip([x[0] for x in FACTS],values)),'mismatches':bad}
 return result
