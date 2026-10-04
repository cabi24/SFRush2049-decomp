#!/usr/bin/env python3
"""Basic recovered-source replay; does not grant peer/CI or runtime acceptance."""
import argparse,hashlib,json,os,sys,tempfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--repo',required=True);a=p.parse_args()
repo=Path(a.repo).resolve();packet=Path(__file__).resolve().parent
sys.path.insert(0,str(repo))
from tools.cloud import score
score.ASM_DIR=repo/'asm/us/boot_tail'
pins=json.loads((repo/'cloud/work/boot_tail/packet2/preflight.json').read_text())['compiler_files_sha256']
ido=Path(os.environ['IDO_DIR'])
for name,want in pins.items():
 assert hashlib.sha256((ido/name).read_bytes()).hexdigest()==want,name
rows=[('8002574C',604,packet/'BT07-decoder-facing-callers/func_8002574C.c',0,604),('80025F74',840,packet/'BT07-decoder-facing-callers/func_80025F74.c',2,840),('80017D38',872,packet/'BT03-high-sequence-events/func_80017D38.c',173,880),('80022CD4',592,packet.parent/'BT05-conditional-pitch/func_80022CD4_CONDITIONAL.c',125,592)]
results=[]
for address,size,source,expected_diff,expected_size in rows:
 name='func_'+address;native=score.targets()[name];assert len(native)*4==size
 for level in ('O2','O1'):
  flags='-g0 -'+level+' -mips2 -G 0 -non_shared'
  with tempfile.TemporaryDirectory() as tmp:
   obj=Path(tmp)/'candidate.o';score.compile_single(source,flags,obj);r=score.compare(obj,name,show=0)
   elf,secs=score._elf(obj);idx=score._text_index(secs)
   fs=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(elf,secs,i) if s['section']==idx and s['type']==2]
   assert len(fs)==1 and fs[0]['name']==name and fs[0]['value']==0
   words=score.text_words(obj);full,masks,u,v,e=score.relocate(obj,words,0,fs[0]['size'],score.image_symbols())
   if level=='O2':
    assert r.differing==expected_diff and fs[0]['size']==expected_size,(name,r.differing,fs)
    assert not(masks or u or v or e)
   results.append(dict(function=name,source=str(source.relative_to(packet.parent)),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),flags=flags,target_bytes=size,differing_words=r.differing,total_words=r.total,extra_nonzero_words=r.extra_words,elf_function=fs[0],text_bytes=secs[idx]['size'],target_window_unresolved=r.unresolved,target_window_unverified=r.unverified,target_window_errors=r.errors,full_function_mask_count=len(masks),full_function_unresolved=u,full_function_unverified=v,full_function_errors=e,local_strict_and_exact_extent=bool(r.accepted() and fs[0]['size']==size and not(masks or u or v or e))))
print(json.dumps(dict(result='Basic compile/extent recovery PASS; not independent acceptance',compiler_files_rechecked=len(pins),target_manifest=score.target_manifest(),results=results),indent=2))
