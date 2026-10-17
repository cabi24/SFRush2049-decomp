"""Verify native IDO-emitted layout constants, including the full visual array."""
from pathlib import Path
import struct
from environment_behavior import require,score
HERE=Path(__file__).resolve().parent
EXPECTED=[952,24,21,272,512,536,584,20,2056,1992,88,6]
def verify(out):
    head=(HERE/'group/group.c').read_text().split('extern Model952')[0]
    suffix='''
u32 environment_layout[]={sizeof(Model952),sizeof(Record24),
 sizeof(((Model952*)0)->visuals)/sizeof(Record24),
 (u32)&((Model952*)0)->visuals[0], (u32)&((Model952*)0)->visuals[10],
 (u32)&((Model952*)0)->visuals[11], (u32)&((Model952*)0)->visuals[13],
 (u32)&((Record24*)0)->callback, sizeof(Vehicle2056),
 (u32)&((Vehicle2056*)0)->active, sizeof(Record88), (u32)&((Record88*)0)->z};
'''
    observed={}
    for label,body in [('native',head),('wrong_visual_capacity',head.replace('visuals[21]','visuals[20]'))]:
        source=Path(out)/(label+'-layout.c');obj=source.with_suffix('.o');source.write_text(body+suffix)
        score.compile_single(source,'-g0 -O2 -mips2 -G 0 -non_shared',obj)
        data,sections=score._elf(obj)
        symbols=[symbol for i,section in enumerate(sections) if section['type']==2 for symbol in score._symbol_table(data,sections,i) if symbol['name']=='environment_layout']
        require(len(symbols)==1,'ambiguous layout symbol');symbol=symbols[0]
        require(symbol['size']==4*len(EXPECTED),'layout extent')
        offset=sections[symbol['section']]['off']+symbol['value'];observed[label]=list(struct.unpack('>'+str(len(EXPECTED))+'I',data[offset:offset+symbol['size']]))
    require(observed['native']==EXPECTED,'native layout mismatch')
    require(observed['wrong_visual_capacity']!=EXPECTED,'wrong-capacity negative escaped')
    return dict(status='PASS',native_layout=observed['native'],wrong_capacity_rejected=True)
