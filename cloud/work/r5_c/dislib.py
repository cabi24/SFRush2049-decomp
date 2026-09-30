import sys,difflib,subprocess,tempfile,struct
from pathlib import Path
import sc
def dis(words):
    with tempfile.TemporaryDirectory() as t:
        p=Path(t)/'a.bin'; p.write_bytes(b''.join(struct.pack('>I',w) for w in words))
        out=subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-mmips:isa32','-EB','-M','reg-names=o32',str(p)],capture_output=True,text=True).stdout
    return [ ' '.join(l.split('\t')[2:]).strip() for l in out.splitlines() if '\t' in l and len(l.split('\t'))>2]
