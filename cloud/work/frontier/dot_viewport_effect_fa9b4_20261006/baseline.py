#!/usr/bin/env python3
"""One canonical O3 build and metadata-only strict comparison receipt."""
import argparse,dataclasses,hashlib,json,shlex,shutil,struct,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
NAME='render_viewport_init'
def main():
 p=argparse.ArgumentParser();p.add_argument('--tool-root',type=Path,required=True);p.add_argument('--object',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 sys.path.insert(0,str(a.tool_root/'tools/cloud'));import score
 if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
  print('SKIP: pinned IDO and MIPS GNU linker required');return
 source=HERE/'candidate.c';assert source.read_text().splitlines()[0]=='/* flags: '+FLAGS+' */'
 score.compile_single(source,FLAGS,a.object)
 comparison=score.compare(a.object,NAME,show=0)
 raw=score.text_words(a.object);resolved,masks,unresolved,unverified,errors=score.relocate(a.object,raw,0,len(raw)*4,score.image_symbols())
 target=score.targets()[NAME];data,sections=score._elf(a.object)
 functions=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['name']==NAME]
 assert len(functions)==1;f=functions[0]
 result=dict(status='MATCH' if comparison.accepted() else 'NONMATCH',claims=[],base=BASE,requested_flags=FLAGS,mandatory_backend_flag=score.R4300_CC,
  native_compiler_invocations=1,native_bytes=len(target)*4,candidate_function_bytes=f['size'],candidate_text_bytes=len(raw)*4,comparison=dataclasses.asdict(comparison),
  source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),object_sha256=hashlib.sha256(a.object.read_bytes()).hexdigest(),
  target_sha256=hashlib.sha256(struct.pack('>'+str(len(target))+'I',*target)).hexdigest(),
  resolved_candidate_sha256=hashlib.sha256(struct.pack('>'+str(len(resolved))+'I',*resolved)).hexdigest(),
  differing_offsets=[hex(i*4) for i,v in enumerate(target) if i>=len(resolved) or resolved[i]!=v],
  unresolved=unresolved,unverified=unverified,errors=errors,masked_offsets=sorted(masks),
  source_contract='Complete ordinary caller. No context helpers, fake arguments, pressure locals, padding locals, dead reads, asm or volatile.')
 a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='differing_offsets'},indent=2))
if __name__=='__main__':main()
