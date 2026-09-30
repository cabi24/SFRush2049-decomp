import sys, json, zlib, subprocess, os
sys.path.insert(0,'/home/user/SFRush2049-decomp')
from pathlib import Path
from tools.conveyor.pipeline import disasm, autodecomp as ad
S=Path(sys.argv[1]); 
data=Path('assets/us/data.bin').read_bytes()
img=zlib.decompressobj(-15).decompress(data[0xB0CB10-0x10000:0xB0CB10-0x10000+326180])
pass
syms={k:int(v,16) for k,v in json.load(open('asm/us/blob/symbols.json'))['symbols'].items()}
targets={v:k for k,v in syms.items() if 0x80086A50<=v<0x80086A50+len(img)}
sys.path.insert(0,'tools/cloud'); import score
ctx=ad._context(include_protos=False)
for fn in sys.argv[2:]:
    n=len(score.targets()[fn]); st=syms[fn]
    out=subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','--adjust-vma=0x80086A50','--start-address',hex(st),'--stop-address',hex(st+4*n),'/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad/img.bin'],capture_output=True,text=True).stdout
    asm=disasm.normalize_objdump(out,fn,targets)
    (S/f'{fn}.s').write_text(asm)
    cmd=[sys.executable,'/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad/m2c/m2c.py',str(S/f'{fn}.s'),'-f',fn,'--valid-syntax','--context',ctx[0]]
    p=subprocess.run(cmd,capture_output=True,text=True)
    (S/f'{fn}.m2c.c').write_text(p.stdout); print(fn,p.returncode,len(p.stdout.splitlines()),p.stderr[:300])
