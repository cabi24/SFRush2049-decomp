#!/usr/bin/env python3
"""Recompile all thirteen bounded source forms and both O2/O1 flags."""
import dataclasses,hashlib,json,sys,tempfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT));from tools.cloud import score
from verify import extent

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail';rows=[]
    expected={}
    for r in json.loads((P/'initial_controls.json').read_text()):expected[(r['name'][5:]+'_initial',r['flags'])]=r
    for r in json.loads((P/'directed_controls.json').read_text()):expected[(r['name'][5:]+'_'+r['variant'],r['flags'])]=r
    with tempfile.TemporaryDirectory(prefix='constructor-controls-') as td:
        for p in sorted((P/'controls').glob('*.c')):
            for opt in ('O2','O1'):
                flags=score.DEFAULT_FLAGS.replace('O2',opt);name='func_'+p.stem[:8];o=Path(td)/(p.stem+opt+'.o');score.compile_single(p,flags,o);r=score.compare(o,name,show=0);size=extent(o,name);want=expected[(p.stem,flags)]
                assert hashlib.sha256(p.read_bytes()).hexdigest()==want['source_sha256']
                assert dataclasses.asdict(r)=={k:want[k]for k in dataclasses.asdict(r)} and size==want['elf_size']
                rows.append(dict(source=p.name,flags=flags,source_sha256=want['source_sha256'],differing_words=r.differing,elf_function_bytes=size,extra_nonzero_words=r.extra_words))
    assert len(rows)==26
    return dict(result='PASS',source_forms=13,compile_rows=len(rows),rows=rows)
if __name__=='__main__':print(json.dumps(run(),indent=2))
