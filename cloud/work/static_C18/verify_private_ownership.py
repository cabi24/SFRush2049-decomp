"""Private physical-placement proof, not production activation or promotion.

Requires an intact C13 private builder baseline and the frozen compiler object.
The workspace must be a new directory inside this worker's scratch directory.
Uses the reviewed generic ROM-data splitter; only BSS linking is a private model.
"""
import argparse, hashlib, importlib.util, json, shlex, shutil, subprocess, sys
from pathlib import Path

p=argparse.ArgumentParser();p.add_argument('--baseline',type=Path,required=True);p.add_argument('--workspace',type=Path,required=True);p.add_argument('--candidate-object',type=Path,required=True);p.add_argument('--owned-data-module',type=Path,required=True);a=p.parse_args()
a.workspace=a.workspace.resolve();scratch=(Path.home()/'agents/C/scratch').resolve()
if not a.workspace.is_relative_to(scratch) or a.workspace.exists():raise SystemExit('requires a new private C scratch workspace')
manifest=json.loads((Path(__file__).parent/'init_ownership_manifest.json').read_text())
if hashlib.sha256(a.candidate_object.read_bytes()).hexdigest()!=manifest['object_sha256']:raise SystemExit('candidate object hash mismatch')
shutil.copytree(a.baseline,a.workspace,symlinks=True);r=a.workspace
sys.path.insert(0,str(r));spec=importlib.util.spec_from_file_location('tools.conveyor.pipeline.owned_data',a.owned_data_module);owned=importlib.util.module_from_spec(spec);sys.modules[spec.name]=owned;spec.loader.exec_module(owned)
# The current reviewed data module delegates optional text-boundary generation.
shutil.copyfile(a.owned_data_module.parent/'owned_text.py',r/'tools/conveyor/pipeline/owned_text.py')
shutil.copyfile(a.owned_data_module.parent/'extract_candidates.py',r/'tools/conveyor/seeds/extract_candidates.py')
registry=r/'rom_owned_data.json';ld=r/'rush2049.us.ld';obj=r/'build/us/src/rom/lib_8a80.o';data=r/'build/us/assets/us/data.o';syms=r/'build/us/syms.ld';saved={x:x.read_bytes() for x in [registry,ld,obj,data,syms]}
import re
owned_names=['gAudioDmaCounter','gAudioDmaBufferPtr','__osShutdown','__osGlobalIntMask','gSpTaskState']
syms.write_text('\n'.join(line for line in syms.read_text().splitlines() if not any(re.match(r'^\s*'+name+r'\s*=',line) for name in owned_names))+'\n')
rows=json.loads(registry.read_text());rows.setdefault('rom_slots',[]).append({'owner':'os_initialize_data','tu':'src/rom/lib_8a80.c','source':'assets/us/data.bin','offset':'0x1cf60','size':'0x20','container_vram':'0x8000f400','owner_section':'.data','sha256':'00eff670001b7953977665e4e87a8f811e3928fc8cce0f7ff5e8bf01d41559c6','passthrough_asm':'asm/us/nonmatchings/rom/lib_8a80/__osInitialize_common.data.s','composed_source':'build/us/assets/us/data.composed.bin'})
companion=r/rows['rom_slots'][-1]['passthrough_asm'];companion.parent.mkdir(parents=True,exist_ok=True);companion.write_text('.section .data\n.incbin "assets/us/data.bin", 0x1cf60, 0x20\n')
registry.write_text(json.dumps(rows,indent=2)+'\n');owned.split_container(r,'assets/us/data.bin',r/'build/us/assets/us/data.composed.bin',data);s=owned.rewrite_linker(ld.read_text(),r);ld.write_text(s)
if owned.rewrite_linker(s,r)!=s:raise SystemExit('data ownership regeneration is unstable')
shutil.copyfile(a.candidate_object,obj);subprocess.run(['mips-linux-gnu-objcopy','--rename-section','.bss=.bss.os_initialize',str(obj)],check=True)
script=r/'initialize_private_storage.ld'
script.write_text('''SECTIONS {
.os_initialize_bss 0x800367D0 (NOLOAD) : {
 initialize_bss_start = ABSOLUTE(.);
 build/us/src/rom/lib_8a80.o(.bss.os_initialize);
 initialize_bss_end = ABSOLUTE(.);
}
ASSERT(initialize_bss_end - initialize_bss_start == 0x10, "initialization BSS size changed")
ASSERT(gSpTaskState == 0x800367D0, "original final-ROM flag allocation moved")
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
valid=link('generic data ownership + genuine initialization storage')
original_script=script.read_text();script.write_text(original_script.replace('.os_initialize_bss 0x800367D0','.os_initialize_bss 0x800367E0'));bad_storage=link('wrong BSS address refuses');script.write_text(original_script)
original_obj=obj.read_bytes();pointer=r/'bad_pointer.bin';subprocess.run(['mips-linux-gnu-objcopy','--dump-section','.data='+str(pointer),str(obj)],check=True);b=bytearray(pointer.read_bytes());b[:4]=(8).to_bytes(4,'big');pointer.write_bytes(b);subprocess.run(['mips-linux-gnu-objcopy','--update-section','.data='+str(pointer),str(obj)],check=True);bad_pointer=link('wrong actual SDK clock initializer fails ROM gate');obj.write_bytes(original_obj)
for path,content in saved.items():path.write_bytes(content)
rollback=link('rollback original physical ownership',storage=False)
report={'scope':'private generic-data API and genuine SDK initialization BSS model; old C13 source-game baseline; no production lifecycle claim','regeneration_stable':True,'results':results}
(r/'ownership_proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if not(valid.get('retail_gate') and bad_storage['link_exit']!=0 and bad_pointer['link_exit']==0 and not bad_pointer.get('retail_gate') and rollback.get('retail_gate')):raise SystemExit(1)
