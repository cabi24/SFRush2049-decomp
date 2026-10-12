#!/usr/bin/env python3
"""Optional stock-only retained UGEN/UMERGE trace; no diagnostic output is a match."""
import argparse
import importlib.util
from pathlib import Path
import shutil

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('kind_scene_verifier',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def main(out):
    out=Path(out).resolve();out.mkdir(parents=True,exist_ok=True)
    ordinary=out/'ordinary.o';diagnostic=out/'diagnostic.o'
    with v.targets(v.image_bytes()):
        v.score.compile_group(HERE,ordinary)
        original=v.score._run
        def run(cmd,**kwargs):
            cmd=list(cmd);stage=Path(cmd[0]).name
            if stage=='ugen':cmd+=['-e','trees','-l','listing']
            if stage=='umerge':cmd+=['-v']
            p=original(cmd,**kwargs)
            if stage in ('uopt','umerge'):(out/(stage+'.log')).write_text(p.stdout+p.stderr)
            if stage=='as1' and p.returncode==0:
                for name in ('merged','st','opt','gen','trees','listing'):shutil.copyfile(Path(kwargs['cwd'])/name,out/name)
            return p
        v.score._run=run
        try:v.score.compile_group(HERE,diagnostic)
        finally:v.score._run=original
        v.require(ordinary.read_bytes()==diagnostic.read_bytes(),'stock diagnostic altered object')
        for name in v.MEMBERS:v.require(v.score.compare(ordinary,name,show=0).accepted(),'stock member mismatch')
    print('PASS stock diagnostic-on/off whole-object identity and both complete members')


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--out',default='build/kind-scene-trace');main(parser.parse_args().out)
