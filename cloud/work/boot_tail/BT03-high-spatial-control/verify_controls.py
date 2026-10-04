#!/usr/bin/env python3
"""Replay every retained source control, including honest rejected HI16 results."""
import hashlib,json,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4];WORK=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail';count=0
    with tempfile.TemporaryDirectory() as tmp:
        for receipt in ['initial_controls.json','directed_controls.json']:
            for row in json.loads((WORK/receipt).read_text()):
                source=ROOT/row['source_path'];data=source.read_bytes()
                assert hashlib.sha256(data).hexdigest()==row['source_sha256']
                obj=Path(tmp)/'control.o';score.compile_single(source,row['flags'],obj)
                r=score.compare(obj,row['name'],show=0);elf,secs=score._elf(obj)
                sym=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(elf,secs,i) if s['name']==row['name'])
                actual=dict(differing_words=r.differing,total_words=r.total,extra_words=r.extra_words,unresolved=r.unresolved,unverified=r.unverified,errors=r.errors,strict_match=r.accepted(),elf_function_size=sym['size'])
                assert all(row[k]==v for k,v in actual.items()),row['source_path']
                assert data==source.read_bytes();count+=1
    return dict(result='PASS',control_rows=count,matching_credit=0)
if __name__=='__main__':print(json.dumps(run(),indent=2))
