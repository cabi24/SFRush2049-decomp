#!/usr/bin/env python3
"""Replay NONMATCH research with pinned IDO and GNU MIPS binutils on PATH.
Checks the complete protected target and resolves relocations using GNU ld.
Expected result: NONMATCH, never a matching claim.
"""
from pathlib import Path
import sys,os,json,hashlib,subprocess,re,struct
R=Path(__file__).resolve().parents[3]; name='func_8010E4E4'; source=Path(__file__).with_name(name+'.c'); B=R/'build'/'dot_entity_motion_audit';B.mkdir(parents=True,exist_ok=True)
def run(args):return subprocess.check_output(list(map(str,args)),text=True)
sha=lambda b:hashlib.sha256(b).hexdigest()
A=R/'asm/us/blob'; manifest=(A/'SHA256SUMS').read_text().splitlines()
for line in manifest:
 h,f=line.split('  ');assert sha((A/f).read_bytes())==h
T={};cur=None
for p in A.glob('*.s'):
 for line in p.read_text().splitlines():
  m=re.match(r'\.section \.text\.(\w+),',line)
  if m:cur=m[1];T[cur]=[]
  elif line.startswith('.section'):cur=None
  m=re.match(r'\s*\.word (0x[\da-fA-F]+)',line)
  if m and cur:T[cur].append(int(m[1],16))
S={k:int(v,16) for k,v in json.loads((A/'symbols.json').read_text())['symbols'].items()}
flags=source.read_text().split('/* flags: ',1)[1].split(' */',1)[0] if '/* flags:' in source.read_text() else '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
run([Path(os.environ.get('IDO_DIR',R/'tools/cloud/ido'))/'cc','-c',*flags.split(),'-o',B/'fresh.o',source])
symbol_sizes=run(['mips-linux-gnu-nm','-S',B/'fresh.o'])
size_line=next(line.split() for line in symbol_sizes.splitlines() if line.split()[-1]==name)
function_bytes=int(size_line[1],16)
rel=run(['mips-linux-gnu-readelf','-r',B/'fresh.o']);(B/'relocations.txt').write_text(rel)
undefined=run(['mips-linux-gnu-nm','-u',B/'fresh.o']); names=[l.split()[-1] for l in undefined.splitlines()]
for n in names:
 if n not in S and re.fullmatch(r'D_[0-9A-Fa-f]{8}',n): S[n]=int(n[2:],16)
script='OUTPUT_ARCH(mips)\nENTRY('+name+')\nSECTIONS { . = '+hex(S[name])+'; .text : SUBALIGN(4) { *(.text) } }\n'+'\n'.join(n+' = '+hex(S[n])+';' for n in names)
(B/'link.ld').write_text(script);run(['mips-linux-gnu-ld','-T',B/'link.ld','-o',B/'linked.elf',B/'fresh.o']);run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',B/'linked.elf',B/'text.bin'])
raw=(B/'text.bin').read_bytes();got=list(struct.unpack('>'+str(len(raw)//4)+'I',raw));want=T[name];diff=[{'offset':hex(i*4),'target':hex(a),'candidate':hex(got[i]) if i<len(got) else None} for i,a in enumerate(want) if i>=len(got) or a!=got[i]]
(B/'target.bin').write_bytes(b''.join(w.to_bytes(4,'big') for w in want))
for f in ['target','text']:(B/(f+'.disasm')).write_text(run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB',B/(f+'.bin')]))
callers=[]
for n,ws in T.items():
 for i,w in enumerate(ws):
  if w>>26 in [2,3] and (((S[n]+i*4+4)&0xf0000000)|((w&0x3ffffff)<<2))==S[name]:callers.append({'function':n,'index':i,'kind':'jal' if w>>26==3 else 'j'})
proof={'function':name,'source_sha256':sha(source.read_bytes()),'flags':flags,'target_bytes':len(want)*4,'linked_text_bytes':len(raw),'candidate_function_bytes':function_bytes,'claims':[],'nonzero_extra_words':sum(w!=0 for w in got[len(want):]),'target_sha256':sha((B/'target.bin').read_bytes()),'linked_text_sha256':sha(raw),'differences':diff,'relocations':rel,'symbols':{n:hex(S[n]) for n in names},'direct_callers':callers,'protected_manifest_entries':len(manifest),'compiler_sha256':{n:sha((Path(os.environ.get('IDO_DIR',R/'tools/cloud/ido'))/n).read_bytes()) for n in ['cc','cfe','uopt','ugen','as1']},'method':'Fresh direct IDO compile; independent target parser and manifest hash; GNU ld/objcopy raw full-word comparison'}
(B/'proof.json').write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(proof,indent=2))

assert len(want)==108
assert len(diff)==72
assert function_bytes==412 and len(raw)==416 and got[-1]==0
assert not any(got[len(want):])
print("NONMATCH",len(diff),"/",len(want),"words; function",function_bytes,"text",len(raw))
