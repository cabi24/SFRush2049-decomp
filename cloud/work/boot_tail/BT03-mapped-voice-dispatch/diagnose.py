#!/usr/bin/env python3
"""Keep only metadata from whole-word relocated workbench diagnosis."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
from verify import ROOT,P,SOURCE,FLAGS,NAME,score,compile_link,unpack,digest

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail'
    with tempfile.TemporaryDirectory(prefix='voice-diagnosis-') as tmp:
        tmp=Path(tmp);obj,body,table,rels=compile_link(SOURCE,FLAGS,tmp)
        for label,words in [('target',score.targets()[NAME]),('candidate',unpack(body))]:
            asm=tmp/(label+'.s');elf=tmp/(label+'.o')
            asm.write_text('.text\n.set noreorder\n.globl '+NAME+'\n.type '+NAME+',@function\n'+NAME+':\n'+''.join('.word 0x%08x\n'%w for w in words)+'.size '+NAME+',.-'+NAME+'\n')
            assembler=Path(os.environ['MIPS_OBJDUMP']).with_name('mips-linux-gnu-as')
            subprocess.run([str(assembler),'-mips2','-EB','-o',str(elf),str(asm)],check=True,capture_output=True)
        args=[sys.executable,str(ROOT/'tools/workbench.py'),'diagnose',str(tmp/'target.o'),str(tmp/'candidate.o'),'--function',NAME,'--objdump',os.environ['MIPS_OBJDUMP'],'--json']
        d=json.loads(subprocess.check_output(args,text=True));c=d['comparison']
        return {'source_sha256':digest(SOURCE),'routing':d['routing'],'owning_pass':d['owning_pass'],'ownership_basis':d['ownership_basis'],
                'lever_class':d['lever']['lever_class'],'lever_reason':d['lever']['reason'],
                'comparison':{k:c[k] for k in ['verdict','words','raw','target_frame_size','candidate_frame_size','target_instructions','candidate_instructions','aligned_insertions','aligned_deletions','aligned_register','aligned_constant','aligned_structural']},
                'scope':'Whole linked function words; temporary literal target/candidate objects are diagnostic only. No relocation masks or source edits. Independent table proof is recorded separately; canonical score.py remains unmodified.'}
if __name__=='__main__':print(json.dumps(run(),indent=2))
