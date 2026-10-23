"""Independent referenced-window content checks. Never establishes whole-section layout or acceptance."""
import sys,struct,hashlib,json
from pathlib import Path
R=next(p for p in Path(__file__).resolve().parents if (p/'tools/cloud/score.py').is_file());sys.path.insert(0,str(R));from tools.cloud import score,owndata

def require(ok,message):
 if not ok:raise RuntimeError(message)
def check(path,name):
 obj=owndata._Object(path);want=score.targets()[name];addresses=score.image_symbols();text,start,end=owndata._locate(obj,name,None,len(want)*4);refs,stray=owndata._references(obj,text);out=[];rawtext=obj.raw(text);fns=sorted((value,n) for n,value,sec in obj.functions() if sec==text)
 require(not any(start<=site<end for site,*_ in stray),'Unpaired own reference')
 expected_sites={'highscore_animText':{0x58,0x60,0x3cc,0x3d8},'DrawGhost':{0x1ec,0x1f0,0x2e4,0x2e8,0x380,0x384,0x548,0x54c,0x640,0x648,0x6c4,0x6cc}}
 require(name in expected_sites,'Unknown content-proof target')
 observed={site-start for _,ref in refs if start<=ref.lo_site<end for site in ref.hi_sites+(ref.lo_site,)}
 require(observed==expected_sites[name],'Own-window reference inventory changed: '+name)
 starts={}
 for _,r in refs:starts.setdefault(r.section,set()).add(r.offset)
 def native_text(offset):
  owners=[(o,n) for o,n in fns if o<=offset]
  require(bool(owners),'No function owner for table entry');o,n=owners[-1]
  require(n in addresses,'Unknown table entry owner '+n)
  return addresses[n]+offset-o
 seen=set()
 for _,ref in refs:
  if not start<=ref.lo_site<end:continue
  for site in ref.hi_sites+(ref.lo_site,):
   require(start<=site<end,'Own reference crosses function boundary')
   actual,=struct.unpack_from('>I',rawtext,site)
   require(actual&0xffff0000==want[(site-start)//4]&0xffff0000,'Own reference operation/register changed')
  native={(((want[(site-start)//4]&0xffff)<<16)+owndata._sext16(want[(ref.lo_site-start)//4]&0xffff))&0xffffffff for site in ref.hi_sites}
  require(len(native)==1,'Inconsistent native reference pairing');address=native.pop();key=(ref.section,ref.offset,address)
  if key in seen:continue
  seen.add(key);raw=obj.raw(ref.section);require(raw is not None,'Uninitialized own section')
  stop=min((s for s in starts[ref.section] if s>ref.offset),default=len(raw));got=bytearray(raw[ref.offset:stop]);entries=0
  for off,kind,sym in obj.relocations(ref.section):
   if not ref.offset<=off<stop:continue
   require(kind==2 and off%4==0 and off+4<=stop,'Unsupported own-window relocation')
   addend,=struct.unpack_from('>I',got,off-ref.offset)
   if sym['section']==text:value=native_text(sym['value']+addend)
   else:
    require(sym['name'] in addresses,'Unknown own-window symbol');value=addresses[sym['name']]+addend
   struct.pack_into('>I',got,off-ref.offset,value&0xffffffff);entries+=1
  sizes={0x801248a0:20,0x80120674:4,0x80120654:16,0x80120664:16,0x80118e20:8,0x80124880:32}
  require(address in sizes,'Unexpected owned-window address')
  size=sizes[address];require(len(got)>=size,'Owned-window extent shrank')
  require(entries==({0x801248a0:5,0x80124880:8}.get(address,0)),'Own-window table extent changed')
  got=bytes(got[:size]);nativebytes=score.own_data().read(address,len(got))
  require(nativebytes==got,'Owned content differs: '+name+' '+obj.sections[ref.section]['name']+' offset '+str(ref.offset))
  out.append({'section':obj.sections[ref.section]['name'],'object_offset':ref.offset,'native_address':hex(address),'bytes':len(got),'sha256':hashlib.sha256(got).hexdigest(),'relocated_table_entries':entries})
 return out
if __name__=='__main__':
 print(json.dumps({n:check(Path(sys.argv[1]),n) for n in ['highscore_animText','DrawGhost']},indent=2))
