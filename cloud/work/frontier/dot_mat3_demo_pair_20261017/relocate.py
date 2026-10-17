"""Explicit relocation of the pair and five source-defined static data slots.

No instruction is copied from the target. The five .data offsets are checked
against the compiler's data layout and native values before use. Main matching
verification separately requires all ordinary scorer relocations verified.
"""
import struct
DATA={0:0x80114154,4:0x80114158,8:0x8011414C,12:0x80114150,16:0x8011415C}
NAMES={'debug_collision':0x800FD024,'debug_stats':0x800FCDF8}
def require(ok,msg):
 if not ok:raise RuntimeError(msg)
def pair(score,obj):
 raw,secs=score._elf(obj);words=score.text_words(obj);symbols=score.symbols(obj);addresses=score.image_symbols();text=score._text_index(secs);sizes={s['name']:s['size'] for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i) if s['type']==2 and s['section']==text}
 ds=next(x for x in secs if x['name']=='.data');require(ds['size']==32,'static data extent')
 expected=bytes(4)+struct.pack('>h',-1)+bytes(26)
 require(raw[ds['off']:ds['off']+32]==expected,'static data values/order')
 for offset,address in DATA.items():require(raw[ds['off']+offset:ds['off']+offset+4]==score.own_data().read(address,4),'native static initializer')
 result={}
 for name,address in NAMES.items():
  start=symbols[name];end=start+sizes[name]
  got,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,addresses)
  require(not unresolved and not errors,'named pair relocation');patched=set()
  for sec in secs:
   if sec['type']!=9 or sec['info']!=text:continue
   syms=score._symbol_table(raw,secs,sec['link']);pending=[]
   def patch(off,kind,sym,addend):
    require(sym['type']==3 and sym['name']=='.data' and addend in DATA,'unsupported private data relocation')
    value=DATA[addend];got[off//4]=(words[off//4]&0xffff0000)|((((value+0x8000)>>16)&65535) if kind==5 else value&65535);patched.add(off)
   for k in range(sec['size']//8):
    off,info=struct.unpack_from('>II',raw,sec['off']+8*k)
    if not start<=off<end:continue
    index,kind=info>>8,info&255;sym=syms[index]
    if off not in masks:continue
    insn=words[off//4]
    if kind==5:pending.append((off,index,insn&65535))
    elif kind==6:
     lo=insn&65535;lo=lo-65536 if lo&32768 else lo;his=[x for x in pending if x[1]==index]
     for h,_,hi in his:patch(h,5,sym,(hi<<16)+lo)
     patch(off,6,sym,((his[0][2]<<16) if his else 0)+lo);pending=[x for x in pending if x[1]!=index]
    else:raise RuntimeError('unexpected private relocation')
   require(not pending,'unpaired private HI16')
  require(patched==set(masks),'private relocation coverage')
  result[address]=got[start//4:end//4]
 return result
