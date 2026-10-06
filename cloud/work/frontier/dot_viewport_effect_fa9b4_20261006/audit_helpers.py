#!/usr/bin/env python3
"""Metadata-only read of additional FA9B4 service contracts at a fixed base.

Preservation scan records explicit register writes and matching stack pairs.
It supplements manual native signature review, not transitive whole-callee proof.
"""
import argparse,hashlib,json,re,struct,subprocess,sys
from pathlib import Path
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
NAMES=['race_init_helper','camera_scene_manager','func_800D169C','render_large_objects',
 'func_800F8EC8','hud_render','camera_position_update','func_800D5524','entity_flags_apply',
 'camera_target_track','func_800C3578','func_800F8E90','cpak_read','race_setup_1',
 'race_setup_2','menu_controller_remap','skid_mark_render']

def audit(root,tools):
 def git(*args):return subprocess.check_output(['git','-C',str(root),*args])
 paths=git('ls-tree','-r','--name-only',BASE,'asm/us/blob').decode().splitlines()
 historical={}
 for p in paths:
  if not p.endswith('.s'):continue
  text=git('show',BASE+':'+p).decode()
  for sec in text.split('.section .text.')[1:]:
   n=sec.split(',',1)[0]
   if n in NAMES:historical[n]=[int(x,16) for x in re.findall(r'\.word\s+0x([0-9A-Fa-f]+)',sec)]
 sys.path.insert(0,str(tools/'tools/cloud'));import score
 current=score.targets();result={}
 syms=json.loads(git('show',BASE+':asm/us/blob/symbols.json'))['symbols']
 for n in NAMES:
  ws=historical[n];assert ws==current[n],n
  defs=set();fds=set();saved={};restored={};fsaved={};frestored={}
  for w in ws:
   op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sa=w>>6&31;fn=w&63;imm=w&65535
   if op==0 and fn in (0,2,3,4,6,7,9,16,18,32,33,34,35,36,37,38,39,42,43):defs.add(rd)
   elif op in (8,9,10,11,12,13,14,15,32,33,34,35,36,37,38):defs.add(rt)
   elif op==17 and rs in (0,2):defs.add(rt)
   if op==17 and rs==4:fds.add(rd)
   elif op==17 and rs in (16,17,20) and fn<48:
    fds.add(sa)
    if rs==17:fds.add(sa+1)
   elif op in (49,53):
    fds.add(rt)
    if op==53:fds.add(rt+1)
   if rs==29:
    for opcode,dest in ((43,saved),(35,restored),(61,fsaved),(53,frestored),(57,fsaved),(49,frestored)):
     if op==opcode:
      dest.setdefault(rt,set()).add(imm)
      if op in (61,53):dest.setdefault(rt+1,set()).add(imm+4)
  paired=lambda a,b:{r for r in a if a[r]&b.get(r,set())}
  gpr=sorted(defs&set(range(16,24))|defs&{30});fpr=sorted(fds&set(range(20,32)))
  assert set(gpr)<=paired(saved,restored),(n,'gpr',gpr)
  assert set(fpr)<=paired(fsaved,frestored),(n,'fpr',fpr)
  raw=struct.pack('>'+str(len(ws))+'I',*ws)
  result[n]=dict(address=syms[n],bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
   explicit_nonvolatile_gpr_writes=gpr,explicit_nonvolatile_fpr_writes=fpr,
   paired_gpr_stack_saves=sorted(paired(saved,restored)&(set(range(16,24))|{30,31})),
   paired_fpr_stack_saves=sorted(paired(fsaved,frestored)&set(range(20,32))))
 return dict(status='ADDITIONAL NATIVE BOUNDARY AUDIT',base=BASE,compiler_invocations=0,
  verifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),targets=result,
  limits='Explicit-write preservation scan and identity check only. Consumed signatures and source field types are manual native review recorded in README; private descendants and callback bodies are not executed by this audit.')

def main():
 p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--tool-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 r=audit(a.reference_root.resolve(),a.tool_root.resolve());a.output.write_text(json.dumps(r,indent=2)+'\n');print(r['status']+': '+str(len(r['targets']))+' bodies verified')
if __name__=='__main__':main()
