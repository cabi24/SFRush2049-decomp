"""Read protected local targets and compare complete private compiler objects.
Never emits target instructions. No relocation masks or unknown placements accepted.
"""
import hashlib,json,re,sys
from pathlib import Path
sys.path.insert(0,'tools/cloud')
import score
P=Path('build/C67');OUT=Path('cloud/work/static_C67')
addresses={n:int(a,16) for n,a in re.findall(r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)',Path('symbol_addrs.us.txt').read_text()+'\n'+Path('undefined_syms_auto.us.txt').read_text(),re.M)}
def original(n):
 s=Path('asm/us/nonmatchings/rom/lib_1050',n+'.s').read_text();size=int(re.search(r'nonmatching\s+\w+,\s*(0x[0-9a-fA-F]+)',s).group(1),16)
 w=[int(x,16) for x in re.findall(r'/\*\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+\*/',s)]
 assert len(w)*4>=size;return w[:size//4]
def compare(obj,n,full_module=True):
 words=score.text_words(obj);syms=score.symbols(obj);start=syms[n];stop=min((x for x in syms.values() if x>start),default=len(words)*4)
 expected=original(n);end=start+len(expected)*4
 linked,masks,unresolved,unverified,errors=score.relocate(obj,words,start,stop,addresses)
 actual=linked[start//4:min(end,stop)//4];extra=linked[end//4:stop//4] if end<=stop else []
 diff=sum(a!=b for a,b in zip(actual,expected))+abs(len(actual)-len(expected));extra_nonzero=sum(w!=0 for w in extra)
 placement=(0x80000450+start)-addresses[n] if full_module else None
 return {'function':n,'original_words':len(expected),'candidate_extent_words':(stop-start)//4,'differing_full_words':diff,'extra_nonzero_words':extra_nonzero,'candidate_slot_delta_bytes':placement,'masks':len(masks),'unresolved':unresolved,'unverified':unverified,'errors':errors,'exact_body_and_extent':diff==0 and extra_nonzero==0 and not any([masks,unresolved,unverified,errors]),'slot_placement_exact':placement==0 if full_module else None}
results=[]
for label in ['baseline','debug1_O1','debug1_O1_modelg','natural_stub_debug1_O1']:
 obj=P/('lib_1050.'+label+'.o');syms=score.symbols(obj)
 members=[compare(obj,n) for n,_ in sorted(syms.items(),key=lambda t:t[1])]
 results.append({'label':label,'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'text_bytes':len(score.text_words(obj))*4,'members':members,'all_bodies_exact':all(x['exact_body_and_extent'] for x in members),'all_slots_exact':all(x['slot_placement_exact'] for x in members)})
obj=P/'osScAddClient.current_header.o';r=compare(obj,'osScAddClient',False);r.update(object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),source_sha256=hashlib.sha256((OUT/'osScAddClient.current_header.c').read_bytes()).hexdigest());results.append({'label':'current_header_singleton','members':[r]})
(OUT/'full_word_proofs.json').write_text(json.dumps(results,indent=2)+'\n')
for r in results:
 print(r['label'],[(x['function'],x['differing_full_words'],x['extra_nonzero_words'],x['candidate_slot_delta_bytes']) for x in r['members'] if not x['exact_body_and_extent'] or x['slot_placement_exact'] is False])
