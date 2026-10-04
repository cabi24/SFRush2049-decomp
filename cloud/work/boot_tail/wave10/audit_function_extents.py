import sys,json,tempfile,hashlib
from pathlib import Path
root=Path(__file__).resolve().parents[4];sys.path.insert(0,str(root))
from tools.cloud import score
score.ASM_DIR=root/'asm/us/boot_tail';es=[]
for n in range(1,11): es += [e for e in json.load(open(root/('cloud/work/boot_tail/wave%d/reviewed_sources.json'%n)))['entries'] if e['expected_match']]
es.append(dict(name='func_80010A00',source_path='cloud/matches/boot_tail/func_80010A00.c',size=12,flags='-g0 -O2 -mips2 -G 0 -non_shared'))
rows=[]
with tempfile.TemporaryDirectory(prefix='boot-tail-extent-audit-') as td:
 for e in es:
  obj=Path(td)/'candidate.o';source=root/e['source_path'];score.compile_single(source,e['flags'],obj)
  data,secs=score._elf(obj);ti=score._text_index(secs);ss=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['section']==ti and s['type']==2]
  good=len(ss)==1 and ss[0]['name']==e['name'] and ss[0]['value']==0 and ss[0]['size']==e['size'];words=score.text_words(obj)
  rows.append(dict(name=e['name'],source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),native_bytes=e['size'],function_symbols=ss,text_section_bytes=secs[ti]['size'],zero_text_alignment_bytes=secs[ti]['size']-ss[0]['size'] if len(ss)==1 else None,all_trailing_section_words_zero=not any(words[e['size']//4:]),exact_function_extent=good))
res=dict(schema_version=1,audited_source_head='b935b5c4f79b92f489636e6cc26c68cc81061c73',audited_source_tree='b8c2b544c1ac72177a7405b1f7e009d3866bc9e1',through_wave=10,result='PASS' if all(r['exact_function_extent'] and r['all_trailing_section_words_zero'] for r in rows) else 'FAIL',functions=len(rows),matching_native_bytes=sum(r['native_bytes'] for r in rows),failures=[r for r in rows if not r['exact_function_extent'] or not r['all_trailing_section_words_zero']],rows=rows)
text=json.dumps(res,indent=2)+'\n'
if '--check' in sys.argv:
 expected=Path(__file__).with_name('function_extent_audit.json').read_text()
 if text != expected: raise SystemExit('function extent audit drift')
 print('Function extent audit:221 exact ELF function bodies; only zero section alignment remains')
else:
 print(text,end='')
