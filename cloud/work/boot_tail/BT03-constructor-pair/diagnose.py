#!/usr/bin/env python3
"""Produce metadata-only workbench diagnoses; native listings stay temporary."""
import hashlib,json,os,re,subprocess,sys,tempfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT));from tools.cloud import score

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail';objdump=os.environ.get('MIPS_OBJDUMP','mips-linux-gnu-objdump');assembler=objdump.replace('objdump','as');rows=[]
    with tempfile.TemporaryDirectory(prefix='constructor-diagnose-') as td:
        td=Path(td)
        for a in ('80019F48','8001A270'):
            name='func_'+a;s=td/(a+'.s');target=td/(a+'-target.o');source=P/'nonmatch'/(name+'.c');obj=td/(a+'.o')
            s.write_text('.set noreorder\n.text\n.globl '+name+'\n.ent '+name+'\n'+name+':\n'+''.join('.word 0x%08x\n'%w for w in score.targets()[name])+'.end '+name+'\n')
            subprocess.run([assembler,'-EB','-mips2',str(s),'-o',str(target)],check=True,capture_output=True)
            score.compile_single(source,score.DEFAULT_FLAGS,obj)
            result=subprocess.check_output([sys.executable,str(ROOT/'tools/workbench.py'),'diagnose',str(target),str(obj),'--function',name,'--objdump',objdump],text=True)
            lines=[]
            for line in result.splitlines():
                if line.startswith(('verdict=','frame mismatch:','aligned residual classes:','ownership:')):
                    line=line.replace(str(obj),'<candidate-object>');lines.append(line)
            rows.append(dict(function=name,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),summary=lines))
    return dict(result='PASS',rows=rows,limits='Workbench ownership and register alignment are heuristics. Raw canonical targets have no ELF relocations, so its relocation-symbol warnings are expected; strict score.py relocation evidence is authoritative.')
if __name__=='__main__':print(json.dumps(run(),indent=2))
