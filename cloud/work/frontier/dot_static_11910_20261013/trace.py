#!/usr/bin/env python3
"""Stock-fidelity uopt cause isolation. All compiler-oracle results get zero credit.

Raw compiler intermediates and logs stay under --out (use a git-ignored directory).
Build the tracer from the official v1.2 recompiler tree with --recompiler, or supply
an existing trace71 + w24c PRE-instrumented directory through --trace-ido.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
NAME='func_80011910'
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'


def run(cmd,**kw):
    p=subprocess.run([str(x) for x in cmd],capture_output=True,text=True,**kw)
    if p.returncode: raise ValueError((cmd,p.returncode,p.stdout,p.stderr))
    return p.stdout


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
    cc=['gcc','-std=c11','-Os','-fno-strict-aliasing','-I.','-DIDO71','-DPACKAGE_VERSION="v1.2"','-DDATETIME="static11910"']
    run(cc+['-c','libc_impl.c','version_info.c'],cwd=out)
    run(cc+['uopt.pre.c','libc_impl.o','version_info.o','-lm','-o','uopt'],cwd=out)
    ido=out/'ido';ido.mkdir(exist_ok=True)
    for p in stock.iterdir():
        if p.is_file() and p.name!='uopt': (ido/p.name).symlink_to(p.resolve())
    (ido/'uopt').symlink_to((out/'uopt').resolve())
    return ido


def main():
    p=argparse.ArgumentParser();g=p.add_mutually_exclusive_group(required=True)
    g.add_argument('--trace-ido',type=Path);g.add_argument('--recompiler',type=Path)
    p.add_argument('--out',type=Path,required=True);a=p.parse_args()
    a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
    cc=compilers.tu_cc('lib_11640',ROOT/'tools/cloud/ido/cc')
    if compilers.identify_dir(cc.parent)!='ido7.1':raise ValueError('not stock IDO7.1')
    traced=a.trace_ido.resolve() if a.trace_ido else build(a.recompiler.resolve(),a.out/'compiler',cc.parent)
    source=(HERE/'baseline.c').read_text()
    variants={'extern_task':source,'defined_task':source.replace('extern AudioTask D_800382F8;','AudioTask D_800382F8;')}
    score.ASM_DIR=ROOT/'asm/us/boot_tail';score.IDO=cc.parent
    result={'compiler':'ido7.1','oracle_is_matching_evidence':False,'claims':[],'variants':{}}
    sys.path.insert(0,str(ROOT/'third_party/n64-decomp-workbench/src'))
    from decomp_workbench.ucode import parse_ucode
    for label,text in variants.items():
        paths={}
        for mode in ('stock','off','on','oracle'):
            d=a.out/label/mode;d.mkdir(parents=True,exist_ok=True);(d/'input.c').write_text(text)
            env=dict(os.environ)
            for n in list(env):
                if n.startswith(('CDX_','P71_')) or n=='USR_LIB':env.pop(n)
            if mode!='stock':env['USR_LIB']=str(traced)
            if mode in ('on','oracle'):
                env.update(CDX_LOG='1',CDX_FN=NAME,CDX_OUT=str(d/'cdx.txt'),P71_OUT=str(d/'p71.txt'),P71_FN=NAME,P71_PREDUMP='1')
            if mode=='oracle':env['CDX_FORCE']='p1:w23=s,p2:w23=s'
            run([cc,'-c',*FLAGS.split(),'-keep','input.c'],cwd=d,env=env);paths[mode]=d
        for suffix in ('.B','.O','.G','.o'):
            original=(paths['stock']/('input'+suffix)).read_bytes()
            if any(original!=(paths[mode]/('input'+suffix)).read_bytes() for mode in ('off','on')):
                raise ValueError(('trace fidelity',label,suffix))
        log=(paths['on']/'cdx.txt').read_text()
        decision=next(line for line in log.splitlines() if 'p1dec ' in line and 'web=23 ' in line)
        for token in ('totalsave=10.000000','bestcost=2.100000','bestreg=a2','forced=-2'):
            if token not in decision:raise ValueError(('task-address decision changed',decision))
        if 'web=0 sym=0 color=6 reg=a3' not in log:raise ValueError('narrow count color changed')
        before=parse_ucode((paths['stock']/'input.B').read_bytes())
        after=parse_ucode((paths['stock']/'input.O').read_bytes())
        direct=sum(r.name=='str' and r.mtype==4 and r.words[2]==4 for r in before)
        indirect=sum(r.name=='istr' for r in after)
        if direct!=10 or indirect!=11:raise ValueError(('store transformation',direct,indirect))
        row={'trace_fidelity':True,'task_address_web':23,'task_address_register':'a2',
             'task_address_total_saving':10.0,'task_address_best_cost':2.1,'count_register':'a3',
             'cfe_direct_task_stores':direct,'uopt_indirect_stores_including_completion':indirect,'modes':{}}
        for mode in ('stock','oracle'):
            obj=paths[mode]/'input.o';r=score.compare(obj,NAME,show=0)
            data,secs=score._elf(obj)
            sy=[s for i,v in enumerate(secs) if v['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==NAME]
            row['modes'][mode]={'differing_words':r.differing,'extra_nonzero_words':r.extra_words,
                 'function_bytes':sy[0]['size'],'allocated_task_definition':label=='defined_task','credit':False}
            if any((r.unresolved,r.unverified,r.errors)):raise ValueError(('unexpected score error',mode,r.summary()))
        result['variants'][label]=row
    expected=[(47,196),(43,228),(47,196),(0,208)]
    actual=[(r['differing_words'],r['function_bytes']) for v in result['variants'].values() for r in v['modes'].values()]
    if actual!=expected:raise ValueError(('causal matrix changed',actual))
    d=a.out/'source_reachable';d.mkdir(exist_ok=True)
    (d/'input.c').write_text((HERE/'candidate.c').read_text())
    env=dict(os.environ)
    for n in list(env):
        if n.startswith(('CDX_','P71_')) or n=='USR_LIB':env.pop(n)
    run([cc,'-c',*FLAGS.split(),'-keep','input.c'],cwd=d,env=env)
    r=score.compare(d/'input.o',NAME,show=0)
    if not r.accepted():raise ValueError('source-reachable stock body changed')
    result['source_reachable']={'compiler':'stock ido7.1','differing_words':0,'function_bytes':208,
        'task_definition_bytes':64,'mechanism':'local task pointer initialized before count guard and reused for callback',
        'source_match_credit':False,'blocker':'unregistered task BSS ownership and actual-TU integration'}
    result['conclusion']='Stock candidate code is exact, but the 64-byte task definition is not owned in the ROM TU. Empty claims until storage placement and integration gates pass.'
    (a.out/'summary.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps(result,indent=2,sort_keys=True))


if __name__=='__main__':main()
