"""Compare complete private proposed modules with protected original words.
All actual function extents and canonical relocations are checked without masks.
"""
import hashlib,json,re,sys
from pathlib import Path
sys.path.insert(0,'tools/cloud');import score
addresses={n:int(a,16) for n,a in re.findall(r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)',Path('symbol_addrs.us.txt').read_text()+'\n'+Path('undefined_syms_auto.us.txt').read_text(),re.M)}
result=[]
for r in json.loads(Path('cloud/work/static_C75/full_module_compile.json').read_text()):
 seg=r['seg'];base=int(seg['vram_start'],16);asm=Path('asm/us/'+seg['yaml_name'][2:].upper()+'.s');raw={int(a,16):int(w,16) for a,w in re.findall(r'/\*\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+\*/',asm.read_text())};obj=Path('build/C75',r['module'],r['module']+'.o');words=score.text_words(obj);syms=score.symbols(obj);members=[]
 for f in seg['functions']:
  n=f['name'];addr=int(f['vaddr'],16);start=syms[n];size=f['size'];stop=min((v for v in syms.values() if v>start),default=len(words)*4)
  linked,masks,unresolved,unverified,errors=score.relocate(obj,words,start,stop,addresses)
  expected=[raw[a] for a in range(addr,addr+size,4)];actual=linked[start//4:min(start+size,stop)//4]
  # Last original zero section-alignment words are real protected padding;
  # track them explicitly instead of counting absent padding as source code.
  missing=expected[len(actual):];diff=sum(a!=b for a,b in zip(actual,expected))+sum(x!=0 for x in missing)
  extra=linked[(start+size)//4:stop//4] if start+size<=stop else [];extra_nonzero=sum(x!=0 for x in extra)
  placement=base+start-addr
  members.append({'function':n,'original_slot_words':size//4,'emitted_extent_words':(stop-start)//4,'original_zero_padding_words_missing':len(missing) if all(x==0 for x in missing) else None,'differing_full_words':diff,'extra_nonzero_words':extra_nonzero,'slot_delta_bytes':placement,'masks':len(masks),'unresolved':unresolved,'unverified':unverified,'errors':errors,'exact':diff==extra_nonzero==placement==0 and not any([masks,unresolved,unverified,errors])})
 result.append({'module':r['module'],'source_sha256':r['source_sha256'],'object_sha256':r['object_sha256'],'original_slot_bytes':seg['size'],'object_text_bytes':len(words)*4,'members':members,'all_members_exact':all(x['exact'] for x in members)})
Path('cloud/work/static_C75/full_module_proofs.json').write_text(json.dumps(result,indent=2)+'\n')
for r in result:print(r['module'],r['all_members_exact'],r['original_slot_bytes'],r['object_text_bytes'],[x for x in r['members'] if not x['exact']])
