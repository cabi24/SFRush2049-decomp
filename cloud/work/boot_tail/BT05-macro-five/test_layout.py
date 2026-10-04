#!/usr/bin/env python3
"""Independently compile native C89 layout assertions with pinned IDO."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT=Path(__file__).resolve().parents[4]
WORK=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
CHECKS=[('pointer_size','sizeof(void *) == 4'),('command_size','sizeof(MacroCommand) == 8'),
        ('voice_size','sizeof(VoiceState) == 416')]
for field,offset in [('program',0),('current',4),('flags24',36),('pan38',56),
    ('panDelta3C',60),('keyGroup4E',78),('id60',96),('panTimeA8',168),
    ('panTargetAC',172),('variables180',384)]:
    CHECKS.append((field,'(unsigned int)&((VoiceState *)0)->%s == %d'%(field,offset)))

def run():
    rows=[]
    with tempfile.TemporaryDirectory(prefix='bt05-macro-five-layout-') as tmp:
        for source in sorted((WORK/'nonmatch').glob('*.c')):
            translation=Path(tmp)/source.name
            translation.write_text(source.read_text()+'\n'+''.join(
                'typedef char check_%s[(%s) ? 1 : -1];\n'%item for item in CHECKS))
            score.compile_single(translation,score.DEFAULT_FLAGS,Path(tmp)/'layout.o')
            rows.append({'name':source.stem,'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
                'checks':[expr for name,expr in CHECKS],'result':'PASS'})
    return {'schema_version':1,'result':'PASS','compiler':'pinned IDO 5.3','results':rows}

if __name__=='__main__':print(json.dumps(run(),indent=2))
