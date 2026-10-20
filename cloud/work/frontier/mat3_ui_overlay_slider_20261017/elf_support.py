"""ELF32 big-endian inspection helpers; does not embed native words."""
import struct

def require(ok,message):
 if not ok:raise RuntimeError(message)

def elf(path):
 """Independent ELF32 big-endian section/symbol/relocation reader."""
 b=path.read_bytes();require(b[:6]==b'\x7fELF\x01\x02','ELF format changed')
 require(struct.unpack_from('>H',b,18)[0]==8,'ELF machine changed')
 at=struct.unpack_from('>I',b,32)[0]
 stride,n,ni=struct.unpack_from('>HHH',b,46);require(stride==40,'Section entry size changed')
 rows=[struct.unpack_from('>10I',b,at+i*stride) for i in range(n)]
 nr=rows[ni];names=b[nr[4]:nr[4]+nr[5]];sections={};symbols={};tables={};relocs=[]
 for i,r in enumerate(rows):
  name=names[r[0]:].split(b'\0')[0].decode();raw=b[r[4]:r[4]+r[5]]
  sections[name]=(i,r,raw)
  if r[1]==2:
   st=rows[r[6]];strings=b[st[4]:st[4]+st[5]];table=[]
   for off in range(0,len(raw),16):
    no,v,size,info,other,index=struct.unpack_from('>IIIBBH',raw,off)
    name=strings[no:].split(b'\0')[0].decode();table.append(name)
    if name:symbols[name]=(v,size,info&15,index)
   tables[i]=table
 for r in rows:
  if r[1] in (4,9):
   require(r[1]==9,'Unsupported relocation section')
   for off in range(r[4],r[4]+r[5],8):
    loc,info=struct.unpack_from('>II',b,off)
    relocs.append((loc,info&255,tables[r[6]][info>>8],r[7]))
 return sections,symbols,relocs
