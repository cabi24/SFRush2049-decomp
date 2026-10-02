import sys,subprocess,hashlib,json,re,shlex
from pathlib import Path
sys.path[:0]=['.','tools/asm-processor'];import asm_processor
from tools.conveyor.pipeline import layout
p=Path('build/C81');out=[];claims={'get_tv_offset','get_viewport_pos','get_viewport_offset','apply_display_mode','display_mode_tick','sprintf','modf','modff','__isinf','__isnan','__round_helper','__write_exponent','__ecvt_internal'}
prelude='''#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern s16 gViewportX, gViewportScaleX, gViewportScaleY, gViewportPendingFrames;
extern s16 gViewportOffsetX[], gViewportOffsetY[];
extern OSViMode *gViewportStruct, *gViewportDataPtr;
extern int fcvt(char *,const char *,char *);
extern s32 lzss_decode(void *,void *);
extern s32 inflate_entry(void *,void *,s32);
extern double gPerspFov, gPerspAspect;
extern u8 *__round_helper(double, s32 *, u8 *, u8 *, u8, u8 *);
extern u8 *__write_exponent(u8 *, s32, s32);
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
'''
for seg in layout.derive()['segments']:
 if '__ecvt_internal' not in {f['name'] for f in seg['functions']}:continue
 label=seg['rom_tu'].split('/')[-1];folder=p/label;folder.mkdir(exist_ok=True);asm=Path('asm/us/'+seg['yaml_name'][2:].upper()+'.s');text=asm.read_text();blocks=re.split(r'(?m)(?=^nonmatching\s+)',text)[1:];blockmap={re.search(r'^nonmatching\s+(\w+)',b).group(1):b for b in blocks}
 tu=Path('cloud/work/static_C81/lib_34a0.bsd.proposed.c').read_text()
 for n,block in blockmap.items():
  if re.search(r'#pragma GLOBAL_ASM\("build/C(?:79|81)/'+label+'/'+n+r'\.s"\)',tu):
   original=folder/(n+'.s');original.write_text(block)
   tu=re.sub(r'build/C(?:79|81)/'+label+'/'+n+r'\.s',str(original),tu)
 src=folder/(label+'.c');src.write_text(tu);pre=folder/'preprocessed.c';obj=folder/(label+'.o')
 with pre.open('wb') as stream:functions,deps=asm_processor.run(['-g',str(src)],outfile=stream)
 subprocess.run(['scp',str(pre),'Rocky:agents/C/scratch/static-C67/module/'+label+'.c'],check=True,stdout=subprocess.DEVNULL)
 flags='-g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm -Iinclude -Iinclude/PR -Irom -D_LANGUAGE_C'
 cmd='cd ~/agents/C/scratch/static-C67/module && ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c '+flags+' '+label+'.c -o '+label+'.o'
 cp=subprocess.run(['ssh','Rocky',cmd],capture_output=True,text=True);r={'module':label,'flags':flags,'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'original_asm_sha256':hashlib.sha256(asm.read_bytes()).hexdigest(),'existing_promoted':sum(f['state']=='promoted' for f in seg['functions']),'compile_exit':cp.returncode,'stderr':cp.stderr,'seg':seg}
 if not cp.returncode:
  subprocess.run(['scp','Rocky:agents/C/scratch/static-C67/module/'+label+'.o',str(obj)],check=True,stdout=subprocess.DEVNULL)
  asm_processor.run(['-g',str(src),'--post-process',str(obj),'--assembler','mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude','--asm-prelude','tools/asm-processor/prelude.inc'],functions=functions);r['object_sha256']=hashlib.sha256(obj.read_bytes()).hexdigest();r['postprocess']='ok'
 out.append(r);print(json.dumps({k:v for k,v in r.items() if k!='seg'}),flush=True)
Path('cloud/work/static_C81/full_module_compile.json').write_text(json.dumps(out,indent=2)+'\n')
