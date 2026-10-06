from pathlib import Path
import sys,json
review=Path(__file__).parent/'review.py';sys.argv=['review',str(Path(sys.argv[1]).resolve())]
n={'__file__':str(review)};exec(review.read_text().split("m=manifest(ROOT/")[0],n)
R=n['ROOT'];O=n['OUT'];result={}
n['manifest'](R/'asm/us/ovl_a')
source_hashes={'func_8039156C':'28f105fb20180d731004251fa48c2cf18f85aac9295a3b0932d9470ba7051106','func_80390D2C':'6e85b1b1ba395ffcd1971caf8069dcfc613193baae452ab4fe4c183141cf91e7'}
for name,entry,end in [('func_8039156C',0x8039156c,0x803915e0),('func_80390D2C',0x80390d2c,0x80390dcc)]:
 source=R/'cloud/matches/ovl_a'/f'{name}.c';data=source.read_bytes();assert n['sha'](data)==source_hashes[name],('context source drift',name)
 flags=source.read_text().splitlines()[0][10:-3].split()+['-Wab,-r4300_mul'];n['shell'](Path(n['os'].environ['IDO_DIR'])/'cc',*flags,'-c','-o',O/'context.o',source)
 obj=n['elf'](O/'context.o');bindings={x['symbol']:int(x['symbol'].rsplit('_',1)[1],16) for x in obj['relocs']}
 (O/'context.ld').write_text(f'OUTPUT_ARCH(mips)\nENTRY({name})\nSECTIONS {{ .text 0x{entry:X} : SUBALIGN(4) {{ *(.text) }} }}\n'+'\n'.join(f'{k} = 0x{v:X};' for k,v in bindings.items())+'\n')
 n['shell']('mips-linux-gnu-ld','-EB','-T',O/'context.ld','-o',O/'context.elf',O/'context.o');linked=n['elf'](O/'context.elf');native=n['target'](R/'asm/us/ovl_a',name)
 assert linked['entry']==entry and linked['text_address']==entry and linked['symbols'][name]['size']==end-entry and len(native)==end-entry
 assert linked['text'][:len(native)]==native and linked['text'][len(native):]==bytes(len(linked['text'])-len(native))
 assert not linked['relocs'] and len(linked['text'])-len(native)<16
 assert not any(s[5] for k,s in linked['sections'].items() if s[2]&2 and k not in ('.text','.reginfo'))
 result[name]=dict(source_sha256=n['sha'](data),native_sha256=n['sha'](native),fresh_context_unchanged=True,whole_linked_native_match=True,size=len(native),alignment=len(linked['text'])-len(native),bindings={k:hex(v) for k,v in bindings.items()})
(O/'context.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
