"""Actual-source O32 layout and external-address proof; no native data exported."""
import hashlib,json,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
WORK=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
CHECKS={
'80019C8C':('cloud/matches/boot_tail/func_80019C8C.c',{'VoiceState':416},{'VoiceState':{'child':16,'parent':20,'flags':36,'channel':74,'set':75,'base_key':78,'key':80,'identifier':96,'glide':140,'pitch':148,'cents':192,'original_key':193}}),
'8001D1F4':('cloud/work/boot_tail/BT03-high-spatial-control/nonmatch/func_8001D1F4.c',{'Emitter':68,'Vector':12},{'Emitter':{'next':0,'previous':4,'flags':8,'position':12,'velocity':24,'range':36,'gain':40,'minimum':44,'curve':48,'handle':52,'context':56,'identifier':60,'counter':62,'fade':64}}),
'8001DC08':('cloud/work/boot_tail/BT03-high-spatial-control/nonmatch/func_8001DC08.c',{'Emitter':68,'Vector':12,'SpatialEntry':28,'SpatialGroup':12},{'SpatialEntry':{'next':0,'volume':4,'pan':8,'span':12,'send':16,'pitch':20,'emitter':24},'SpatialGroup':{'key':0,'pending':4,'active':8}})}
def run():
 score.ASM_DIR=ROOT/'asm/us/boot_tail';rows=[]
 with tempfile.TemporaryDirectory(prefix='spatial-layouts-') as d:
  for name,(relative,sizes,offsets) in CHECKS.items():
   source=ROOT/relative
   terms=['sizeof('+t+')=='+str(n) for t,n in sizes.items()]
   terms+=['((unsigned int)&(('+t+'*)0)->'+f+')=='+str(n) for t,fields in offsets.items() for f,n in fields.items()]
   terms+=['sizeof(void*)==4','sizeof(unsigned int)==4','sizeof(float)==4','sizeof(short)==2']
   p=Path(d)/'probe.c';p.write_text('#include "'+str(source)+'"\ntypedef char actual_native_layout[('+' && '.join(terms)+')?1:-1];\n')
   score.compile_single(p,'-g0 -O2 -mips2 -G 0 -non_shared',Path(d)/'probe.o')
   rows.append(dict(name='func_'+name,source_path=relative,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),native_sizes=sizes,native_offsets=offsets,result='PASS'))
  source=ROOT/CHECKS['8001DC08'][0];obj=Path(d)/'thresholds.o';score.compile_single(source,score.DEFAULT_FLAGS,obj)
  w=score.text_words(obj);rel,m,u,uv,e=score.relocate(obj,w,0,len(w)*4,score.image_symbols());assert not m and not u and not uv and not e
  target=score.targets()['func_8001DC08'];proof=[]
  for name,address in [('D_8002D904',0x8002D904),('D_8002D908',0x8002D908)]:
   native=[];candidate=[]
   for words,hits in [(target,native),(rel,candidate)]:
    gp=[None]*32
    for index,word in enumerate(words):
     op=word>>26;rs=(word>>21)&31;rt=(word>>16)&31;imm=word&65535;imm=imm if imm<32768 else imm-65536
     if op==15:gp[rt]=(word&65535)<<16
     elif op==49 and gp[rs] is not None and (gp[rs]+imm)&0xffffffff==address:hits.append(index*4)
   assert len(native)==1 and len(candidate)==1
   proof.append(dict(symbol=name,address=hex(address),native_lwc1_offsets=native,candidate_lwc1_offsets=candidate,contents_known=False,source_contract='one cached external float value per nonempty invocation'))
 return dict(result='PASS',scope='Native O32 sizes/offsets and exact external load addresses. Host LP64 layouts differ. Threshold values and original data bytes are neither supplied nor inferred.',results=rows,external_thresholds=proof)
if __name__=='__main__':print(json.dumps(run(),indent=2))
