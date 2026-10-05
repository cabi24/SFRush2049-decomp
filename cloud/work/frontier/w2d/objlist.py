#!/usr/bin/env python3
"""objlist.py OBJ FN: plain listing of FN from an object (relocated against image symbols)."""
import sys, re, struct, subprocess, tempfile
sys.path.insert(0,'tools/cloud')
import score
from pathlib import Path
obj=Path(sys.argv[1]); fn=sys.argv[2]
words=score.text_words(obj); fns=score.symbols(obj); start=fns[fn]
end=min((o for o in fns.values() if o>start), default=len(words)*4)
res,masks,unres,unver,errs=score.relocate(obj,words,start,end,score.image_symbols())
got=res[start//4:end//4]
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack('>%dI'%len(got),*got)); f.flush()
    out=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:4300','-EB',f.name],capture_output=True,text=True).stdout
for line in out.splitlines():
    m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)',line)
    if m: print('%4x: %s'%(int(m.group(1),16), re.sub(r'\s+',' ',m.group(3))))
