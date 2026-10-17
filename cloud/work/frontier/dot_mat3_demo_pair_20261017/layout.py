"""Read native declarations through IDO's emitted constant data."""
from pathlib import Path
import struct
FACTS={'model_stride':2056,'model_slot':1990,'model_slot_width':2,'player_stride':76,'player_slot':0,'player_slot_width':1,'player_capacity':4,'inited_width':1,'ready_width':1,'track_width':2,'windows_width':2,'cars_width':1,'blit_pointer_width':4}
def run(score,out,source):
 out=Path(out);out.mkdir(parents=True,exist_ok=True);p=out/'layout.c';p.write_text(source);o=out/'layout.o';score.compile_single(p,'-g0 -O2 -mips2 -G 0 -non_shared',o);data,ss=score._elf(o);e=next(x for i,sec in enumerate(ss) if sec['type']==2 for x in score._symbol_table(data,ss,i) if x['name']=='demo_layout');sec=ss[e['section']];return list(struct.unpack('>13I',data[sec['off']+e['value']:sec['off']+e['value']+52]))
def verify(score,out,source,require):
 source=source[:source.index('static void setGameState')]
 source+='\nconst u32 demo_layout[] = {sizeof(MODELDAT),(u32)&((MODELDAT*)0)->slot,sizeof(((MODELDAT*)0)->slot),sizeof(Player),(u32)&((Player*)0)->node,sizeof(((Player*)0)->node),sizeof(D_8014A118)/sizeof(Player),sizeof(D_8011414C),sizeof(D_80114154),sizeof(D_80114158),sizeof(D_80151AD0),sizeof(D_80152744),sizeof(D_80114150)};\n'
 values=run(score,Path(out)/'native',source);require(values==list(FACTS.values()),'native layout mismatch')
 negatives={}
 for name,old,new in [('model_slot','0x7C6','0x7C4'),('player_stride','0x4B','0x4F'),('player_capacity','D_8014A118[4]','D_8014A118[5]')]:
  require(old in source,'layout anchor');v=run(score,Path(out)/name,source.replace(old,new));require(v!=values,'layout negative passed');negatives[name]=v
 return {'facts':FACTS,'compiled_negative_values':negatives}
