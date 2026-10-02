"""Compile the complete native BSD-derived formatter TU privately on Rocky C."""
from pathlib import Path
import subprocess,hashlib,json
p=Path('build/C85');src=Path('cloud/work/static_C85/lib_34a0.original_templates.full_native.c');obj=p/'lib_34a0.o'
subprocess.run(['scp',str(src),'Rocky:agents/C/scratch/static-C67/module/lib_34a0_C85.c'],check=True,stdout=subprocess.DEVNULL)
flags='-g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm -Iinclude -Iinclude/PR -Irom -D_LANGUAGE_C'
cmd='cd ~/agents/C/scratch/static-C67/module && ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c '+flags+' lib_34a0_C85.c -o lib_34a0_C85.o'
cp=subprocess.run(['ssh','Rocky',cmd],capture_output=True,text=True)
r={'module':'lib_34a0','flags':flags,'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'compile_exit':cp.returncode,'stderr':cp.stderr,'full_native':True}
if not cp.returncode:
 subprocess.run(['scp','Rocky:agents/C/scratch/static-C67/module/lib_34a0_C85.o',str(obj)],check=True,stdout=subprocess.DEVNULL)
 r['object_sha256']=hashlib.sha256(obj.read_bytes()).hexdigest()
Path('cloud/work/static_C85/full_module_compile.json').write_text(json.dumps(r,indent=2)+'\n')
print(json.dumps(r))
if cp.returncode:raise SystemExit(cp.returncode)
