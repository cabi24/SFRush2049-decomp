"""Finite host differential: actual music/Random/writer/allocator C versus a field model."""
from pathlib import Path
import subprocess
HERE=Path(__file__).resolve().parent

def body(text, marker):
    start=text.index(marker); opening=text.index('{',start); depth=1; end=opening+1
    while depth:
        depth += (text[end]=='{')-(text[end]=='}');end+=1
    return text[start:end]+'\n'

def run(group,out,negative=False):
    out.mkdir(parents=True,exist_ok=True)
    selections=[('rand.c','int func_8008B2B4(void)'),('random.c','f32 func_8008B2E4(f32 max)'),
        ('func_80091B00.c','Slot18 *func_80091B00(void)'),
        ('codex_vsync_a145.c','void speed_set(f32 blend,f32 amount,s32 first,s32 second)'),
        ('codex_vsync_a145.c','void speed_mode0_wrapper(f32 blend, f32 amount)'),
        ('sync_entry_register.c','void sync_entry_register(s32 arg0, s32 arg1)'),
        ('func_800D6160.c','void func_800D6160(s32 resume)')]
    source='\n'.join(body((group/f).read_text(),m) for f,m in selections)
    (out/'candidate.inc').write_text(source)
    (out/'semantics.c').write_text((HERE/'semantics.c').read_text())
    exe=out/'host'
    subprocess.run(['cc','-std=c11','-O2','-fwrapv','-fno-strict-aliasing','-fsanitize=undefined',
                    '-fno-sanitize-recover=undefined',str(out/'semantics.c'),'-o',str(exe)],
                   check=True,capture_output=True,text=True)
    r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    if negative:
        if r.returncode!=3 or r.stderr!='music differential mismatch\n' or r.stdout:
            raise RuntimeError('semantic negative failed for wrong reason: '+repr((r.returncode,r.stdout,r.stderr)))
        return {'status':'rejected','reason':'music differential mismatch','exit':3}
    if r.returncode or r.stderr or r.stdout!='86184 finite music state-and-queue fixtures\n':
        raise RuntimeError('host model failed: '+repr((r.returncode,r.stdout,r.stderr)))
    return {'fixtures':86184,'real_C':['music root','Random','rand','music-volume wrapper','volume writer','song writer','slot allocator'],
            'boundaries':['queue receive/send capture'],'compiler_contract':['fwrapv','fno-strict-aliasing','fatal UBSan']}
