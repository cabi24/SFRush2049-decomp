#!/usr/bin/env python3
"""Opt-in causal replay using the repository's existing IDO7.1 tracing patches.

Provide either --trace-ido DIR (with PRE/CDX uopt) or --recompiler DIR pointing
at decompals/ido-static-recomp v1.2 after make setup. All compiler-generated
C, binaries, listings and instruction-bearing diagnostics stay in --out.
Only the compact summary is suitable for publication. Oracle output is never
matching credit. The source candidate is always scored with stock IDO.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
NAME='func_8001558C'
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'


def run(cmd,**kw):
    r=subprocess.run([str(x) for x in cmd],capture_output=True,text=True,**kw)
    if r.returncode:raise ValueError((cmd,r.returncode,r.stdout,r.stderr))
    return r.stdout


def build(recompiler,out,stock):
    out.mkdir(parents=True,exist_ok=True)
    generated=out/'uopt.c'
    with generated.open('w') as f:
        subprocess.run([str(recompiler/'build/recomp.elf'),str(recompiler/'ido/7.1/usr/lib/uopt')],stdout=f,check=True)
    run([sys.executable,ROOT/'cloud/work/frontier/tools/trace71/patches/instrument71_uopt.py',generated,out/'uopt.trace.c'])
    run([sys.executable,ROOT/'cloud/work/frontier/w24c/tools/pre71.py',out/'uopt.trace.c',out/'uopt.pre.c'])
    for name in ('header.h','helpers.h','libc_impl.h','libc_impl.c','version_info.c'):
        shutil.copyfile(recompiler/name,out/name)
    libc=out/'libc_impl.c'
    if 'w5d_cvt' not in libc.read_text():
        run([sys.executable,ROOT/'cloud/work/frontier/tools/trace/patches/ecvt.patch.py',libc])
    cc=['gcc','-std=c11','-Os','-fno-strict-aliasing','-I.','-DIDO71','-DPACKAGE_VERSION="v1.2"','-DDATETIME="static1558c"']
    run(cc+['-c','libc_impl.c','version_info.c'],cwd=out)
    run(cc+['uopt.pre.c','libc_impl.o','version_info.o','-lm','-o','uopt'],cwd=out)
    ido=out/'ido';ido.mkdir(exist_ok=True)
    for p in stock.iterdir():
        if p.is_file() and p.name!='uopt':
            (ido/p.name).symlink_to(p.resolve())
    (ido/'uopt').symlink_to((out/'uopt').resolve())
    return ido


def summarize(path):
    bits={};vectors={}
    for line in path.read_text().splitlines():
        m=re.match(r'\[P71\] bit at=cm bit=(\d+) .*text=(\S+)',line)
        if m:bits[int(m[1])]=m[2]
        m=re.match(r'\[P71\] vec at=cm node=(\d+) (\w+)=(.*)',line)
        if m:vectors[(int(m[1]),m[2])]={int(v) for v in m[3].split(',') if v}
    wanted={n:t for n,t in bits.items() if re.fullmatch(r'cvt.J\(varP\(\d+\+2\)r\)',t)}
    if len(wanted)!=1:raise ValueError(('ID promotion not unique',wanted))
    bit=next(iter(wanted))
    table=[n for n,t in bits.items() if t.startswith('ixa.J(ldaS(')]
    if len(table)!=1:raise ValueError('table address not unique')
    return dict(bit=bit,table_address_bit=table[0],expression=wanted[bit],nodes={v:sorted(n for (n,k),bs in vectors.items() if k==v and bit in bs) for v in ('antlocs','avlocs','insert','delete')})


def main():
    p=argparse.ArgumentParser();g=p.add_mutually_exclusive_group(required=True)
    g.add_argument('--trace-ido',type=Path);g.add_argument('--recompiler',type=Path)
    p.add_argument('--out',type=Path,required=True);a=p.parse_args()
    a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
    cc=compilers.tu_cc('lib_15a10',ROOT/'tools/cloud/ido/cc')
    if compilers.identify_dir(cc.parent)!='ido7.1':raise ValueError('stock compiler identity')
    traced=a.trace_ido.resolve() if a.trace_ido else build(a.recompiler.resolve(),a.out/'compiler',cc.parent)
    text=(ROOT/'cloud/matches/boot_tail/func_8001558C.c').read_text()
    versions={'baseline':text.replace('        result = id;\n',''),'candidate':text,
              'early_read':text.replace('        i = 0;\n        result = id;\n        for (;','        if (id != 0) {}\n        for (i = 0;')}
    result={'compiler':'ido7.1','oracle_is_matching_evidence':False,'variants':{}}
    score.ASM_DIR=ROOT/'asm/us/boot_tail';score.IDO=cc.parent
    for label,src in versions.items():
        paths={}
        for mode in ('stock','off','on'):
            d=a.out/label/mode;d.mkdir(parents=True,exist_ok=True);(d/'input.c').write_text(src)
            env=dict(os.environ)
            env.pop('USR_LIB',None)
            if mode!='stock':env['USR_LIB']=str(traced)
            if mode=='on':env.update(P71_OUT=str(d/'p71.txt'),P71_FN=NAME,P71_PREDUMP='1',CDX_OUT=str(d/'cdx.txt'),CDX_LOG='1',CDX_FN=NAME)
            run([cc,'-c',*FLAGS.split(),'-keep','input.c'],cwd=d,env=env)
            paths[mode]=d
        for suffix in ('.O','.G','.o'):
            original=(paths['stock']/('input'+suffix)).read_bytes()
            for mode in ('off','on'):
                if original!=(paths[mode]/('input'+suffix)).read_bytes():raise ValueError(('trace fidelity',label,mode,suffix))
        comparison=score.compare(paths['stock']/'input.o',NAME,show=0)
        result['variants'][label]=dict(differing_words=comparison.differing,trace_fidelity=True,promotion=summarize(paths['on']/'p71.txt'))
    # One causal oracle only: relocate the baseline promoted-ID expression
    # from the positive-count preheader (node 2) into its count guard (node 1).
    base=result['variants']['baseline']['promotion'];bit=base['bit']
    if base['nodes']['insert']!=[2]:raise ValueError('baseline PRE location changed')
    d=a.out/'oracle';d.mkdir(exist_ok=True);(d/'input.c').write_text(versions['baseline'])
    env=dict(os.environ,USR_LIB=str(traced),P71_OUT=str(d/'p71.txt'),P71_FN=NAME,
             P71_CLR='insert:%d@2'%bit,P71_SET='insert:%d@1'%bit)
    run([cc,'-c',*FLAGS.split(),'-keep','input.c'],cwd=d,env=env)
    r=score.compare(d/'input.o',NAME,show=0)
    if r.differing or r.extra_words or r.errors or r.unresolved or r.unverified:raise ValueError('causal placement oracle not exact')
    result['oracle']=dict(differing_words=0,operation='move only promoted-ID PRE insertion from node 2 to node 1',credit=False)
    if [result['variants'][n]['differing_words'] for n in ('baseline','early_read','candidate')]!=[4,2,0]:raise ValueError('source control results changed')
    (a.out/'summary.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps(result,indent=2,sort_keys=True))


if __name__=='__main__':main()
