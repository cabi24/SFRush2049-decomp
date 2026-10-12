#!/usr/bin/env python3
"""Optional stock/intermediate diagnostics; no forced object is acceptance evidence."""
import argparse
import importlib.util
import os
from pathlib import Path
import shutil

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('hud_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def trace(out,uopt=None):
    out=Path(out).resolve();out.mkdir(parents=True,exist_ok=True)
    ordinary=out/'ordinary.o';diagnostic=out/'diagnostic.o'
    v.score.compile_single(HERE/'candidate.c',v.FLAGS,ordinary)
    v.run([v.score.ido('cc'),'-K','-c',*v.FLAGS.split(),v.score.R4300_CC,'-o',diagnostic,HERE/'candidate.c'],cwd=out)
    v.require(ordinary.read_bytes()==diagnostic.read_bytes(),'stock retained-intermediate object changed')
    if uopt:
        env=dict(os.environ,CDX_LOG='1',CDX_PROC='0',CDX_OUT=str(out/'colors.log'),
                 W5D_PROC=v.NAME,W5D_LEVEL='3',W5D_OUT=str(out/'expressions.log'))
        v.run([Path(uopt).resolve(),'-G','0','-mips2','-EB','-g0','-O2',out/'candidate.B',out/'traced.O',
               '-t',out/'candidate.T',out/'uopt.log','-l',out/'uopt.list'],env=env)
        v.require((out/'traced.O').read_bytes()==(out/'candidate.O').read_bytes(),'traced uopt differs from stock')
        v.run([v.score.ido('ugen'),'-G','0','-mips2','-EB','-g0','-O2',out/'traced.O','-o',out/'traced.G',
               '-t',out/'candidate.T','-temp',out/'ugen.tmp','-l',out/'traced.s'])
        v.run([v.score.ido('as1'),'-elf','-G','0','-p0','-mips2','-EB','-g0','-O2','-r4300_mul',out/'traced.G',
               '-o',out/'traced.o','-t',out/'candidate.T'])
        v.require((out/'traced.o').read_bytes()==ordinary.read_bytes(),'traced complete object differs from stock')
    v.inspect_object(ordinary,v.DIFFS)
    print('PASS stock diagnostic-on/off whole-object identity; NONMATCH 4/95 remains')


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--out',default='build/hud-teardown-trace');parser.add_argument('--uopt')
    args=parser.parse_args();trace(args.out,args.uopt)
