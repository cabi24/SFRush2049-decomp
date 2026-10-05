from pathlib import Path
import re,importlib.util,hashlib,json,subprocess,sys
root=Path.cwd();out=root/'cloud/work/integration_B24';tmp=root/'build/codex-B24'
spec=importlib.util.spec_from_file_location('tools.conveyor.pipeline.B24_targets',out/'targets.py');t=importlib.util.module_from_spec(spec);sys.modules[spec.name]=t;spec.loader.exec_module(t);t.REPO=root
regions=t.index_asm_regions();rows=[]
for fn in ['dll_remove','dll_init','dll_update','dll_reschedule','dll_insert','dll_get_priority']:
 path=root/f'asm/us/nonmatchings/rom/lib_cc50/{fn}.s';region=next(r for r in regions.values() if r.name==fn);lines=region.lines;words=region.words
 target=tmp/(fn+'.target.o');t.assemble_region(region,fn,target)
 aliases={}
 for name in re.findall(r'%(?:hi|lo)\(([A-Za-z_]\w*)','\n'.join(lines))+re.findall(r'\bjal\s+([A-Za-z_]\w*)','\n'.join(lines)):
  aliases[name]=t._resolve_symbol(name)
 ld=tmp/(fn+'.ld');ld.write_text('SECTIONS { .text '+hex(region.vaddr)+' : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) } }\n'+'\n'.join(name+' = '+hex(addr)+';' for name,addr in aliases.items() if addr is not None)+'\n')
 elf=tmp/(fn+'.elf');p=subprocess.run(['mips-linux-gnu-ld','-m','elf32btsmip','-T',str(ld),str(target),'-o',str(elf)],capture_output=True,text=True);assert p.returncode==0,p.stderr
 raw=tmp/(fn+'.bin');subprocess.run(['mips-linux-gnu-objcopy','--dump-section','.text='+str(raw),str(elf)],check=True)
 expected=b''.join(int(w,16).to_bytes(4,'big') for w in words);actual=raw.read_bytes();assert actual[:len(expected)]==expected,(fn,[(i//4,actual[i:i+4].hex(),expected[i:i+4].hex()) for i in range(0,len(expected),4) if actual[i:i+4]!=expected[i:i+4]]);assert not any(actual[len(expected):]);assert t.gate_target(words,target)==(True,None)
 rows.append(dict(function=fn,words=len(words),asm_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),target_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),linked_body_sha256=hashlib.sha256(expected).hexdigest(),linked_body_differences=0,tail_alignment_bytes=len(actual)-len(expected),target_gate=True))
(out/'linked_targets.json').write_text(json.dumps(rows,indent=2)+'\n')
p=subprocess.run(['diff','-u','--label','a/tools/conveyor/pipeline/targets.py','--label','b/tools/conveyor/pipeline/targets.py',str(root/'tools/conveyor/pipeline/targets.py'),str(out/'targets.py')],capture_output=True,text=True);(out/'targets.patch').write_text(p.stdout)
print(rows)
