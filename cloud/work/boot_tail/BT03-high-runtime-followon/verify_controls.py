"""Reproduce archived, rejected natural compiler controls without match credit."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
score.ASM_DIR=ROOT/'asm/us/boot_tail'
rows=[]
for p in sorted((Path(__file__).resolve().parent/'controls').glob('*.c')):
    for level in (2,1):
        flags='-g0 -O%d -mips2 -G 0 -non_shared'%level
        with tempfile.TemporaryDirectory() as tmp:
            obj=Path(tmp)/'candidate.o';score.compile_single(p,flags,obj)
            r=score.compare(obj,p.stem[:13],show=0)
            elf,sections=score._elf(obj)
            symbol=next(sym for idx,sec in enumerate(sections) if sec['type']==2 for sym in score._symbol_table(elf,sections,idx) if sym['name']==p.stem[:13])
        rows.append(dict(name=p.stem[:13],source_path=str(p.relative_to(ROOT)),source_sha256=hashlib.sha256(p.read_bytes()).hexdigest(),flags=flags,differing_words=r.differing,total_words=r.total,extra_words=r.extra_words,unresolved=r.unresolved,unverified=r.unverified,errors=r.errors,strict_match=r.accepted(),elf_function_size=symbol["size"]))
print(json.dumps({'purpose':'Rejected compiler controls; zero match credit. All controls are rejected source forms.','results':rows},indent=2))
