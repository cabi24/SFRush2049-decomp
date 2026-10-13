"""Compile the unchanged range-planning statements against logical host records.

Only the prelude command-pointer/write helper and everything from flags/format
onward are excluded; this is explicitly not a full-source behavior proof.
"""
import os
from pathlib import Path
import subprocess

def require(x,why):
    if not x:raise ValueError(why)

def translation(source):
    begin=source.index('void func_80012D18(')
    head=source[:begin]
    body=source[begin:source.index('    if (V->flags & 1)',begin)]
    start=body.index('    cmd = ');end=body.index('    frames = ',start)
    body=body[:start]+body[end:]
    body=body.replace('void func_80012D18(AudioState *audio, unsigned short samples, unsigned char index)','static void plan(Voice_80012D18 *audio)')
    finish='''    printf("%u %u %u %u %u %u %u %d %u\\n", address[0], length[0], length[1], length[1] ? address[1] : 0U, padded[0], length[1] ? padded[1] : 0U, (unsigned int)offset, mode, (unsigned int)state);
}
double func_8001E688(double value) { return floor(value); }
int main(void) {
    Voice_80012D18 v;
    unsigned int rate, pending, tail;
    memset(&v, 0, sizeof(v));
    while (scanf("%u %lf %u %u %u %u %u", &rate, &v.position, &v.length, &v.loop_start, &v.loop_length, &pending, &tail) == 7) {
        v.rate = (unsigned short)rate; v.pending = (unsigned char)pending; v.tail = tail;
        plan(&v);
    }
    return 0;
}
'''
    return '#include <stdio.h>\n#include <string.h>\n#include <math.h>\n'+head+body+finish

def prove(sources,work,root,native):
    payload=''.join('%u %.17g %u %u %u %u %u\n'%c for c in native.cases())
    expected=[]
    for c in native.cases():
        v,_=native.oracle(c)
        expected.append([v[x] or 0 for x in ('address','count','second','second_addr','padded','second_padded','offset','mode','state')])
    result={}
    for name,source in sources.items():
        file=work/(name+'_host.c');file.write_text(translation(source));result[name]={}
        for mode,extra in [('O0',['-O0']),('O2',['-O2']),('sanitized',['-O2','-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all','-fno-omit-frame-pointer'])]:
            exe=work/(name+'_'+mode)
            p=subprocess.run(['gcc','-std=c89','-Wall','-Wextra','-Wno-unused-variable',*extra,'-I',str(root/'include'),str(file),'-lm','-o',str(exe)],text=True,capture_output=True)
            require(p.returncode==0,(name,mode,p.stderr))
            p=subprocess.run([str(exe)],input=payload,text=True,capture_output=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
            require(p.returncode==0,(name,mode,p.stderr))
            observed=[[int(v) for v in line.split()] for line in p.stdout.splitlines()]
            require(observed==expected,('host prefix behavior',name,mode))
            result[name][mode]=len(expected)
    return result
