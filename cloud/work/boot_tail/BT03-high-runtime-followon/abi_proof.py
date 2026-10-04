"""Native compiler sizeof assertions, with no emitted object or target bytes."""
import hashlib,json,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
WORK=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
CHECKS={
'8001EB10':('cloud/matches/boot_tail/func_8001EB10.c',{'SequenceNode':16,'VoiceState':416}),
'80019194':('cloud/matches/boot_tail/func_80019194.c',{'SequenceContext':4088}),
'8001D764':('cloud/work/boot_tail/BT03-high-runtime-followon/nonmatch/func_8001D764.c',{'SpatialState':136,'Vector':12}),
'8001C580':('cloud/work/boot_tail/BT03-high-runtime-followon/nonmatch/func_8001C580.c',{'SampleBuffer':24,'SampleInfo':25}),
'8001824C':('cloud/work/boot_tail/BT03-high-runtime-followon/nonmatch/func_8001824C.c',{'Track':40,'Context':3944}),
'80018448':('cloud/work/boot_tail/BT03-high-runtime-followon/nonmatch/func_80018448.c',{'Track':40,'Context':3944})}
rows=[]
with tempfile.TemporaryDirectory(prefix='runtime-followon-widths-') as d:
 for name,(relative,types) in CHECKS.items():
  source=ROOT/relative
  check=' && '.join('sizeof('+t+')=='+str(n) for t,n in types.items())+' && sizeof(void*)==4 && sizeof(unsigned int)==4 && sizeof(short)==2'
  p=Path(d)/'probe.c';p.write_text('#include "'+str(source)+'"\ntypedef char actual_native_widths[('+check+')?1:-1];\n')
  score.compile_single(p,'-g0 -O2 -mips2 -G 0 -non_shared',Path(d)/'probe.o')
  rows.append({'name':'func_'+name,'source_path':relative,'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'native_type_sizes':types,'result':'PASS'})
print(json.dumps({'result':'PASS','scope':'Native O32 type widths only. Host pointer/function-pointer widths differ; partial Context extent does not claim full runtime object size. No raw object or data bytes exported.','results':rows},indent=2))
