"""Read-only complete static native/relocation/extent replay; private output only."""
import sys,json,hashlib,re,struct,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT))
from tools.conveyor.pipeline import targets
from tools.conveyor.jobs import scoring
from tools.cloud import score
p=Path(__file__).parent;work=ROOT/'build/C111';name='__scSchedule'
s=(ROOT/'asm/us/nonmatchings/rom/lib_1050'/f'{name}.s').read_text();original=[int(x,16) for x in re.findall(r'/\*\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+\*/',s)]
assert len(original)==64
addresses={n:int(a,16) for n,a in re.findall(r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)',(ROOT/'symbol_addrs.us.txt').read_text()+'\n'+(ROOT/'undefined_syms_auto.us.txt').read_text(),re.M)}
rows=[]
for stem,src in [('candidate','__scSchedule.c')]:
 obj=work/(stem+'.o');words=score.text_words(obj);linked,masks,unresolved,unverified,errors=score.relocate(obj,words,0,len(words)*4,addresses)
 size_line=next(x for x in subprocess.check_output(['mips-linux-gnu-nm','-S',str(obj)],text=True).splitlines() if x.split()[-1]==name);size=int(size_line.split()[1],16)
 actual=linked[:64];extra=linked[64:];diff=sum(a!=b for a,b in zip(actual,original))+abs(len(actual)-len(original));row={'source':str((p/src).relative_to(ROOT)),'source_sha256':hashlib.sha256((p/src).read_bytes()).hexdigest(),'flags':(p/src).read_text().splitlines()[0],'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'canonical_strict_including_stack':scoring.score(work/'target.o',obj,stack_differences=True),'original_bytes':256,'true_symbol_bytes':size,'emitted_text_bytes':len(words)*4,'frame_bytes':(-words[0])&65535,'full_word_differences':diff,'extra_nonzero_words':sum(bool(x) for x in extra),'masks':len(masks),'unresolved':unresolved,'unverified':unverified,'errors':errors,'full_native_match':diff==0 and not any(extra) and not any([masks,unresolved,unverified,errors])}
 rows.append(row)
 print(json.dumps(row))
proof={'target_id':name,'original_target_roundtrip':targets.gate_target([f'{w:08x}' for w in original],work/'target.o'),'accepted_credit':0,'results':rows}
(p/'proof.json').write_text(json.dumps(proof,indent=2)+'\n')
