"""Private physical-placement proof, not production activation or promotion.

Requires an intact C13 private builder baseline and the frozen compiler object.
The workspace must be a new directory inside this worker's scratch directory.
Uses the reviewed generic ROM-data splitter; only BSS linking is a private model.
"""
import argparse, hashlib, importlib.util, json, shlex, shutil, subprocess
from pathlib import Path

p=argparse.ArgumentParser();p.add_argument('--baseline',type=Path,required=True);p.add_argument('--workspace',type=Path,required=True);p.add_argument('--candidate-object',type=Path,required=True);p.add_argument('--owned-data-module',type=Path,required=True);a=p.parse_args()
a.workspace=a.workspace.resolve();scratch=(Path.home()/'agents/C/scratch').resolve()
if not a.workspace.is_relative_to(scratch) or a.workspace.exists():raise SystemExit('requires a new private C scratch workspace')
manifest=json.loads((Path(__file__).parent/'manifest.json').read_text())
if hashlib.sha256(a.candidate_object.read_bytes()).hexdigest()!=manifest['object_sha256']:raise SystemExit('candidate object hash mismatch')
shutil.copytree(a.baseline,a.workspace,symlinks=True);r=a.workspace
spec=importlib.util.spec_from_file_location('private_owned_data',a.owned_data_module);owned=importlib.util.module_from_spec(spec);spec.loader.exec_module(owned)
registry=r/'rom_owned_data.json';ld=r/'rush2049.us.ld';obj=r/'build/us/src/rom/lib_cc50.o';data=r/'build/us/assets/us/data.o';saved={x:x.read_bytes() for x in [registry,ld,obj,data]}
rows=json.loads(registry.read_text());rows.setdefault('rom_slots',[]).append({'owner':'timer_services_pointer','tu':'src/rom/lib_cc50.c','source':'assets/us/data.bin','offset':'0x1cff0','size':'0x10','container_vram':'0x8000f400','owner_section':'.data','sha256':'3c1a6f5433a5490b4c6df69ff14fe6cd7df420acdf36f18e0491bc2e3aab4206','passthrough_asm':'asm/us/nonmatchings/rom/lib_cc50/dll_init.pointer.s','composed_source':'build/us/assets/us/data.composed.bin'})
companion=r/rows['rom_slots'][-1]['passthrough_asm'];companion.parent.mkdir(parents=True,exist_ok=True);companion.write_text('.section .data\n.incbin "assets/us/data.bin", 0x1cff0, 0x10\n')
registry.write_text(json.dumps(rows,indent=2)+'\n');owned.split_container(r,'assets/us/data.bin',r/'build/us/assets/us/data.composed.bin',data);s=owned.rewrite_linker(ld.read_text(),r);ld.write_text(s)
if owned.rewrite_linker(s,r)!=s:raise SystemExit('data ownership regeneration is unstable')
shutil.copyfile(a.candidate_object,obj);subprocess.run(['mips-linux-gnu-objcopy','--rename-section','.bss=.bss.timer_services',str(obj)],check=True)
script=r/'timer_private_storage.ld'
script.write_text('''SECTIONS {
.timer_services_bss 0x80037C30 (NOLOAD) : {
 timer_bss_start = ABSOLUTE(.);
 build/us/src/rom/lib_cc50.o(.bss.timer_services);
 timer_bss_end = ABSOLUTE(.);
}
ASSERT(timer_bss_end - timer_bss_start == 0x40, "timer BSS size changed")
ASSERT(__osBaseTimer == timer_bss_start, "base timer allocation moved")
ASSERT(gViTimeAccumHi == timer_bss_start + 0x20, "clock allocation moved")
ASSERT(gViLastCount == timer_bss_start + 0x28, "base counter moved")
ASSERT(gViRetraceCount == timer_bss_start + 0x2c, "VI count moved")
ASSERT(__osTimerCounter == timer_bss_start + 0x30, "timer counter moved")
} INSERT AFTER .data;
''')
elf=r/'build/us/rush2049.us.elf';elf.unlink(missing_ok=True)
lines=subprocess.check_output(['make','-n','build/us/rush2049.us.elf'],cwd=r,text=True).splitlines();base_cmd=next(shlex.split(x) for x in lines if x.startswith('mips-linux-gnu-ld '));results=[]
def link(label,storage=True):
 cmd=base_cmd[:]
 if storage:cmd[1:1]=['-T',str(script)]
 proc=subprocess.run(cmd,cwd=r,capture_output=True,text=True)
 result={'case':label,'link_exit':proc.returncode,'link_diagnostic':proc.stderr.strip()}
 if proc.returncode==0:
  rom=r/'build/us/rush2049.us.z64';subprocess.run(['mips-linux-gnu-objcopy','-O','binary',str(elf),str(rom)],check=True)
  sha=hashlib.sha1(rom.read_bytes()).hexdigest();result.update(sha1=sha,retail_gate=sha=='3f99351d7bb61656614bdb2aa1a90cfe55d1922c')
 results.append(result);return result
valid=link('generic data ownership + genuine timer storage')
original_script=script.read_text();script.write_text(original_script.replace('0x80037C30','0x80037C40'));bad_storage=link('wrong BSS address refuses');script.write_text(original_script)
original_obj=obj.read_bytes();pointer=r/'bad_pointer.bin';subprocess.run(['mips-linux-gnu-objcopy','--dump-section','.data='+str(pointer),str(obj)],check=True);b=bytearray(pointer.read_bytes());b[:4]=(8).to_bytes(4,'big');pointer.write_bytes(b);subprocess.run(['mips-linux-gnu-objcopy','--update-section','.data='+str(pointer),str(obj)],check=True);bad_pointer=link('wrong allocated timer pointer fails ROM gate');obj.write_bytes(original_obj)
for path,content in saved.items():path.write_bytes(content)
rollback=link('rollback original physical ownership',storage=False)
report={'scope':'private generic-data API and genuine BSS model; old C13 source-game baseline; no production lifecycle claim','regeneration_stable':True,'results':results}
(r/'ownership_proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if not(valid.get('retail_gate') and bad_storage['link_exit']!=0 and bad_pointer['link_exit']==0 and not bad_pointer.get('retail_gate') and rollback.get('retail_gate')):raise SystemExit(1)
