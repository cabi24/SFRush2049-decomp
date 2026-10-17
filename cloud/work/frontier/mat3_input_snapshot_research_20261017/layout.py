"""Verify IDO-emitted type-layout constants, without warning-only typedef assertions."""
import re,struct
from behavior import HERE,ROOT,require,score
EXPECTED=[16,0,4,8,9,10]
def verify_layout(out):
 source=(HERE/'group/group.c').read_text();match=re.search(r'typedef struct NativePadState \{.*?\} NativePadState;',source,re.S);require(match,'missing native pad type')
 pre='typedef signed char s8; typedef unsigned char u8; typedef int s32; typedef unsigned int u32;\n'
 post='\nu32 input_layout[] = {sizeof(NativePadState), (u32)&((NativePadState*)0)->connected, (u32)&((NativePadState*)0)->buttons, (u32)&((NativePadState*)0)->stick_x, (u32)&((NativePadState*)0)->stick_y, (u32)&((NativePadState*)0)->auxiliary};\n'
 results={}
 for label,body in [('native',match[0]),('wrong_reserved_width',match[0].replace('reserved[3]','reserved[4]'))]:
  path=out/(label+'-layout.c');path.write_text(pre+body+post);obj=out/(label+'-layout.o');score.compile_single(path,'-g0 -O2 -mips2 -G 0 -non_shared',obj)
  data,secs=score._elf(obj);symbols=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='input_layout'];require(len(symbols)==1,'ambiguous layout object');sym=symbols[0];sec=secs[sym['section']];require(sym['size']==4*len(EXPECTED),'layout object extent');offset=sec['off']+sym['value'];actual=list(struct.unpack('>6I',data[offset:offset+24]));results[label]=actual
 require(results['native']==EXPECTED,'native type layout mismatch');require(results['wrong_reserved_width']!=EXPECTED,'layout negative escaped')
 return {'status':'PASS','emitted_layout':results['native'],'wrong_reserved_width_rejected':True}
