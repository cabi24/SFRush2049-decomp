#!/usr/bin/env python3
"""Strict canonical comparison plus GNU link replay. Requires pinned IDO/binutils.
The exit status is 0 when the documented NONMATCH is reproduced; never promotes.
"""
import dataclasses, hashlib, json, os, shutil, struct, subprocess, sys, tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
NAME='func_8010C448';FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
SOURCE=HERE/(NAME+'.c')
sha=lambda b:hashlib.sha256(b).hexdigest()
pack=lambda w:struct.pack('>%dI'%len(w),*w)
def run():
    addresses=score.image_symbols();want=score.targets()[NAME]
    with tempfile.TemporaryDirectory() as tmp:
        d=Path(tmp);(d/'candidate.c').write_bytes(SOURCE.read_bytes())
        subprocess.run([str(score.IDO.resolve()/'cc'),'-c',*FLAGS.split(),'-o','candidate.o','candidate.c'],cwd=d,check=True)
        obj=d/'candidate.o';raw=score.text_words(obj);data,secs=score._elf(obj)
        syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
        fn=next(s for s in syms if s['name']==NAME)
        undefined=[s['name'] for s in syms if s['section']==0 and s['name']]
        assert set(undefined)=={'D_8014AA3A','player_array'}
        ld=shutil.which('mips-linux-gnu-ld');copy=shutil.which('mips-linux-gnu-objcopy');assert ld and copy
        (d/'link.ld').write_text('SECTIONS { .text '+hex(addresses[NAME])+' : SUBALIGN(4) { *(.text) } }')
        subprocess.run([ld,'-T',str(d/'link.ld'),'-e',NAME,*['--defsym='+n+'='+hex(addresses[n]) for n in undefined],str(obj),'-o',str(d/'linked.elf')],check=True)
        subprocess.run([copy,'-O','binary','--only-section=.text',str(d/'linked.elf'),str(d/'linked.bin')],check=True)
        linked=(d/'linked.bin').read_bytes();resolved=list(struct.unpack('>%dI'%(len(linked)//4),linked))
        comparison=score.compare(obj,NAME,show=0)
        strict,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,addresses)
        assert strict==resolved and not masks and not unresolved and not unverified and not errors, (len(strict),len(resolved), [(i,hex(a),hex(b)) for i,(a,b) in enumerate(zip(strict,resolved)) if a!=b], masks, unresolved, unverified, errors)
        diffs=[{'offset':hex(i*4),'target':hex(w),'candidate':hex(resolved[i]) if i<len(resolved) else None} for i,w in enumerate(want) if i>=len(resolved) or resolved[i]!=w]
        assert fn['size']==304 and len(want)==80 and len(diffs)==45 and comparison.differing==45 and comparison.extra_words==0
        result={'verdict':'NONMATCH_CONFIRMED','claims':[],'source_sha256':sha(SOURCE.read_bytes()),'base_commit':subprocess.check_output(['git','rev-parse','origin/master'],cwd=ROOT,text=True).strip(),'flags':FLAGS,'target_start':hex(addresses[NAME]),'target_end_exclusive':hex(addresses[NAME]+4*len(want)),'target_bytes':len(want)*4,'candidate_function_bytes':fn['size'],'candidate_text_bytes':len(linked),'target_sha256':sha(pack(want)),'resolved_sha256':sha(linked),'object_sha256':sha(obj.read_bytes()),'compiler_sha256':sha((score.IDO/'cc').read_bytes()),'comparison':dataclasses.asdict(comparison),'differences':diffs,'missing_native_bytes':16,'nonzero_excess_words':0,'resolved_symbols':{n:hex(addresses[n]) for n in undefined},'replay_method':'Fresh direct IDO compile then GNU ld and objcopy; full canonical relocation replay agrees; no masks'}
    print(json.dumps(result,indent=2));return result
if __name__=='__main__':run()
