from pathlib import Path
import sys,json,hashlib
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
R=ROOT; score.ASM_DIR=R/'asm/us/boot_tail';p=Path(__file__).resolve().parent
import tempfile
work=tempfile.TemporaryDirectory(prefix='runtime-variants-');out=Path(work.name)
base=(p/'controls/func_80018634_initial.c').read_text(); variants=[]
def trial(name,src,reason):
 f=out/(name+'.c');f.write_text(src);o=f.with_suffix('.o');score.compile_single(f,score.DEFAULT_FLAGS,o);r=score.compare(o,'func_80018634',show=0);data,secs=score._elf(o);syms=[s for i,q in enumerate(secs) if q['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='func_80018634']; row=dict(name=name,reason=reason,differing=r.differing,extra=r.extra_words,bytes=syms[0]['size'],frame=65536-(score.text_words(o)[0]&65535),sha256=hashlib.sha256(src.encode()).hexdigest());variants.append(row);return src
s=trial('initial',base,'Whole-body initial reconstruction')
s=trial('decode_order',s.replace('    u8 active;\n    u8 first_loop;', '    u8 first_loop;\n    u8 active;').replace('    active = 0;\n    first_loop = 1;','    first_loop = 1;\n    active = 0;'),'Native initialization is first-loop flag before active byte, with active at stack+66')
s=trial('decoded_jump',s.replace('                    D_8004BE80->tracks[i].cursor = D_8004BE80->tracks[i].start + event->value.jump;','                    sum = event->value.jump;\n                    D_8004BE80->tracks[i].cursor = D_8004BE80->tracks[i].start + sum;').replace('            D_8004BE80->tracks[i].time += (sum >> 16) + D_8004BE80->step_time;','            sum >>= 16;\n            D_8004BE80->tracks[i].time += sum + D_8004BE80->step_time;'),'Native jump index and fixed-point carry share v1, so reuse the real scalar temporary')
s=trial('offset_first',s.replace('(u8 *)sequence + sequence->patterns','sequence->patterns + (u8 *)sequence').replace('(u8 *)D_8004BE80->sequence + pattern->optional04','pattern->optional04 + (u8 *)D_8004BE80->sequence').replace('(u8 *)D_8004BE80->sequence + pattern->optional08','pattern->optional08 + (u8 *)D_8004BE80->sequence').replace('(u8 *)sequence + sequence->channels','sequence->channels + (u8 *)sequence'),'Native addu operand order spells relative offset before byte base')
s=trial('decoded_code',s.replace('    u32 sum;','    u32 sum;\n    u16 code;').replace('                switch (event->code) {','                code = event->code;\n                switch (code) {').replace('patterns[event->code]','patterns[code]'),'Decode and reuse the genuine u16 opcode in dispatch and pattern table lookup')
s=trial('decoded_offsets',s.replace('    u32 sum;\n    u16 code;','    u32 sum;\n    u16 code;\n    u32 offset;').replace('                    if (pattern->optional04) {','                    offset = pattern->optional04;\n                    if (offset) {').replace('pattern->optional04 + (u8 *)D_8004BE80->sequence','offset + (u8 *)D_8004BE80->sequence').replace('                    if (pattern->optional08) {','                    offset = pattern->optional08;\n                    if (offset) {').replace('pattern->optional08 + (u8 *)D_8004BE80->sequence','offset + (u8 *)D_8004BE80->sequence'),'Explicitly decode optional resource offsets before zero testing and address conversion')
assert variants == json.loads((p/'variants.json').read_text())
print(json.dumps({'result':'PASS','variants':variants},indent=2))
work.cleanup()
