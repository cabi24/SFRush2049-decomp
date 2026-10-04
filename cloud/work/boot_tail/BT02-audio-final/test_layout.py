#!/usr/bin/env python3
"""Pin the native O32 layouts independently of the 64-bit behavioral host."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
P = Path(__file__).resolve().parent
ROOT = P.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
CHECKS = {'80011104': [('sizeof(AudioState)',104), ('(unsigned int)&((AudioState*)0)->active',0), ('(unsigned int)&((AudioState*)0)->bit_index',96), ('(unsigned int)&((AudioState*)0)->changed',97)],
'800114C0':[('sizeof(void*)',4),('sizeof(unsigned int)',4),('sizeof(unsigned short)',2)],
'800139D4':[('sizeof(AudioState)',104),('sizeof(AudioCommand)',80),('sizeof(AudioBlock)',2576),('sizeof(AudioLoop)',8),('(unsigned int)&((AudioCommand*)0)->status',44),('(unsigned int)&((AudioCommand*)0)->output',68),('(unsigned int)&((AudioBlock*)0)->sample',2),('(unsigned int)&((AudioBlock*)0)->changed',4),('(unsigned int)&((AudioBlock*)0)->work',8),('(unsigned int)&((AudioBlock*)0)->loop',12),('(unsigned int)&((AudioBlock*)0)->commands',16),('(unsigned int)&((AudioLoop*)0)->length',4)]}
def run():
    rows=[]
    with tempfile.TemporaryDirectory(prefix='audio-final-layout-') as folder:
        for a, checks in CHECKS.items():
            source=P/'nonmatch'/('func_'+a+'.c')
            text=source.read_text()+'\n'+'\n'.join('typedef char check_%d[(%s == %d) ? 1 : -1];' % (i,e,v) for i,(e,v) in enumerate(checks))+'\n'
            path=Path(folder)/(a+'.c');path.write_text(text);score.compile_single(path,score.DEFAULT_FLAGS,path.with_suffix('.o'))
            rows.append({'function':'func_'+a,'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'checks':[{'expression':e,'value':v} for e,v in checks],'result':'PASS'})
    return {'result':'PASS','compiler':'pinned IDO5.3','results':rows}
if __name__=='__main__':
    print(json.dumps(run(),indent=2))
