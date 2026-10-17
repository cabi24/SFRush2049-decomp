"""Read IDO-emitted data constants for native views; compile-success is insufficient."""
from pathlib import Path
import re,struct
EXPECTED=[16,9,12,12,0,4,5,36,16,18,24,8,4,20,3,4,12,4,1,1]
def require(v,msg):
 if not v:raise RuntimeError(msg)
def run(group,out,score):
 out=Path(out);out.mkdir(parents=True,exist_ok=True)
 source=(group/'drawtext.c').read_text().split('void audio_doppler_calc(')[0]
 res=re.search(r'typedef struct ResSlot \{.*?\} ResSlot;', (group/'peer_slot_group.c').read_text(), re.S).group()
 color=re.search(r'typedef struct TextColor \{.*?\} TextColor;', (group/'text_storage.c').read_text(), re.S).group()
 source+='\n'+res+'\n'+color+'''
#define OFF(T,m) ((unsigned int)&(((T*)0)->m))
unsigned int facts[]={sizeof(Bank),OFF(Bank,f9),OFF(Bank,f12),sizeof(Entry),OFF(Entry,ptr),OFF(Entry,id),OFF(Entry,pad),sizeof(FontTexture36),OFF(FontTexture36,width),OFF(FontTexture36,height),OFF(FontTexture36,image),sizeof(NativeTextColor),sizeof(TextColor),sizeof(ResSlot),OFF(ResSlot,loaded),OFF(ResSlot,mode),OFF(ResSlot,data),sizeof(void*),sizeof(s8),(s8)-1<0};
'''
 variants={'positive':source,'short_color':source.replace('rgba[8]','rgba[4]'),'wide_nonprop':source.replace('u8 f9;','u16 f9;'),'wide_glyph':source.replace('u8 pad[7]','u8 pad[8]'),'wide_loaded':source.replace('s8 loaded;','s32 loaded;')}
 result={}
 for name,text in variants.items():
  d=out/name;d.mkdir(exist_ok=True);(d/'layout.c').write_text(text);obj=(d/'layout.o').resolve()
  p=score._run([score.ido('cc'),'-c','-g0','-O3','-mips2','-G','0','-non_shared',score.R4300_CC,'-o',str(obj),'layout.c'],cwd=d)
  require(p.returncode==0,'layout compile failed')
  data,sections=score._elf(obj);syms=[s for i,se in enumerate(sections) if se['type']==2 for s in score._symbol_table(data,sections,i) if s['name']=='facts'];require(len(syms)==1,'layout symbol')
  s=syms[0];require(s['size']==len(EXPECTED)*4,'layout fact extent');sec=sections[s['section']];values=list(struct.unpack('>%dI'%len(EXPECTED),data[sec['off']+s['value']:sec['off']+s['value']+s['size']]))
  require((values==EXPECTED)==(name=='positive'),'emitted layout control failed: '+name);result[name]=values
 return result
