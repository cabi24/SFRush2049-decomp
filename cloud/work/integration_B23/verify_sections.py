from pathlib import Path
import hashlib,json,subprocess
from tools.conveyor.pipeline import lock
root=Path.cwd();out=root/'cloud/work/integration_B23';scratch=root/'build/codex-B23';m=json.loads((root/'cloud/work/static_C16/manifest.json').read_text());source=root/'cloud/work/static_C16/candidate.c';obj=scratch/'candidate.o'
assert hashlib.sha256(source.read_bytes()).hexdigest()==m['source_sha256'];assert hashlib.sha256(obj.read_bytes()).hexdigest()==m['object_sha256']
functions=['dll_remove','dll_update','dll_reschedule','dll_insert','dll_get_priority'];accepted=[]
entries=lock.load_lock(root/'matched.lock.json')
for fn in functions:
 a=lock.body_sha(root/'src/rom/lib_cc50.c',fn);b=lock.body_sha(source,fn);assert a==b
 row=entries.get('src/rom/lib_cc50.c:'+fn);assert row and row['body_sha256']==a
 accepted.append(dict(function=fn,body_sha256=a,lock_flags=row['flagset'],candidate_body_unchanged=True))
# Deliberately leave owned symbols out of absolute-address extern definitions:
# all six addresses must come from real compiler storage, not the AUTO script.
owned=['__osBaseTimer','__osTimerList','gViTimeAccumHi','gViLastCount','gViRetraceCount','__osTimerCounter']
defs=(root/'undefined_syms_auto.us.txt').read_text()
import re
defs='\n'.join(line for line in defs.splitlines() if not any(re.match(r'\s*'+re.escape(name)+r'\s*=',line) for name in owned))+'\n'
(scratch/'externs.ld').write_text(defs)
(scratch/'model.ld').write_text((root/'cloud/work/static_C16/model.ld').read_text());(scratch/'functions.ld').write_text((root/'cloud/work/static_C16/functions.ld').read_text()+'__osSetCompare = 0x8000FB90;\n')
cmd=['mips-linux-gnu-ld','-m','elf32btsmip','-T','model.ld','-T','externs.ld','-T','functions.ld','-o','model.elf','candidate.o']
p=subprocess.run(cmd,cwd=scratch,capture_output=True,text=True);assert p.returncode==0,p.stderr
symbols=subprocess.check_output(['mips-linux-gnu-readelf','-sW',str(scratch/'model.elf')],text=True)
sectionproof=[]
for name in owned:
 line=next(x for x in symbols.splitlines() if x.split()[-1:]==[name]);fields=line.split();assert fields[-2] not in ['ABS','COM','UND'];sectionproof.append(dict(symbol=name,address='0x'+fields[1],size=int(fields[2]),section_index=fields[-2]))
retail=(root/'baserom.us.z64').read_bytes();comparisons=[]
for sec,start,size in [('.text',0xcc50,0x460),('.data',0x2cff0,0x10)]:
 path=scratch/(sec[1:]+'.bin');subprocess.run(['mips-linux-gnu-objcopy','--dump-section',sec+'='+str(path),str(scratch/'model.elf')],check=True)
 compiled=path.read_bytes();expected=retail[start:start+size];assert compiled==expected
 comparisons.append(dict(section=sec,bytes=len(compiled),byte_differences=0,sha256=hashlib.sha256(compiled).hexdigest()))
startup=json.loads((root/'cloud/work/integration_B18/coordinator_D_startup.json').read_text());assert int(startup['clear_start'],0)<=0x80037c30 and int(startup['clear_end_exclusive'],0)>=0x80037c70;assert 0x8002c3f0+16<=int(startup['clear_start'],0)
result=dict(candidate_source_sha256=m['source_sha256'],candidate_object_sha256=m['object_sha256'],flags=m['flags'],accepted_members=accepted,linked_section_bound_symbols=sectionproof,linked_comparisons=comparisons,startup=dict(clear_start=startup['clear_start'],clear_end_exclusive=startup['clear_end_exclusive'],entry_sha256=startup['entry_code_sha256'],timer_bss_covered=True,initialized_pointer_below_clear_range=True),header_hashes={str(p):hashlib.sha256((root/p).read_bytes()).hexdigest() for p in ['src/rom/rom_tu.h','include/m2c_types.h','include/PR/os_time.h']},link_command=cmd,absolute_owned_symbol_definitions_excluded=owned,scope='Independent genuine-section local linker proof, no new full-ROM or strict-score claim')
(out/'audit_proof.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
