"""IDO-emitted facts, with genuine wrong-type controls; warnings are not proofs."""
import struct
from pathlib import Path
HERE=Path(__file__).resolve().parent
FACTS=[('target_bytes','sizeof(Target)',112),('target_flags','OFF(Target,flags)',4),('target_model','OFF(Target,model)',12),('target_matrix','OFF(Target,mat)',20),('target_position','OFF(Target,position)',68),('target_owner','OFF(Target,owner)',101),('target_age','OFF(Target,age)',104),('target_data','OFF(Target,data)',108),('mine_bytes','sizeof(MineData)',136),('mine_matrix','OFF(MineData,mat)',16),('mine_elapsed','OFF(MineData,elapsed)',112),('mine_step','OFF(MineData,step)',120),('mine_alpha','OFF(MineData,alpha)',122),('mine_scale','OFF(MineData,scale)',124),('mine_phase','OFF(MineData,phase)',132),('mine_active','OFF(MineData,active)',134),('visual_bytes','sizeof(Visual)',24),('visual_data','OFF(Visual,data)',12),('visual_callback','OFF(Visual,callback)',20),('object_bytes','sizeof(Object68)',68),('object_color','OFF(Object68,body.view.color)',60),('slots_bytes','sizeof(MineSlots)',20),('slots_count','OFF(MineSlots,count)',16),('pool_bytes','sizeof(Pool)',24),('pool_head','OFF(Pool,active)',16),('texture_bytes','sizeof(Texture36)',36),('signed_step','(s8)-1',0xffffffff),('alpha_width','sizeof(((MineData*)0)->alpha[0])',1)]
def verify(score,out,require):
 source=(HERE/'group/group.c').read_text();head=source[source.index('typedef signed char'):source.index('extern s32')]
 variants={'native':head,'wide_alpha':head.replace('u8 alpha[2]','u16 alpha[2]'),'wrong_slots':head.replace('Target *target[4]','Target *target[3]'),'wrong_object_color':head.replace('s32 prefix[8]','s32 prefix[7]')}
 result={}
 for name,src in variants.items():
  require(name=='native' or src!=head,'layout mutation failed');d=out/name;d.mkdir(parents=True,exist_ok=True)
  (d/'probe.c').write_text(src+'\n#define OFF(t,m) ((u32)&((t*)0)->m)\nu32 values[]={'+','.join(x[1] for x in FACTS)+'};\n');score.compile_single(d/'probe.c','-g0 -O3 -mips2 -G 0 -non_shared',d/'probe.o')
  data,secs=score._elf(d/'probe.o');entries=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='values'];require(len(entries)==1 and entries[0]['size']==len(FACTS)*4,'layout symbol extent');e=entries[0];sec=secs[e['section']];got=struct.unpack('>%dI'%len(FACTS),data[sec['off']+e['value']:sec['off']+e['value']+e['size']]);bad=[n for (n,_,v),g in zip(FACTS,got) if v!=g];require(bool(bad)==(name!='native'),'layout control not rejected');result[name]={'values':dict(zip([x[0] for x in FACTS],got)),'mismatches':bad}
 return result
