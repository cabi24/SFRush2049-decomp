#!/usr/bin/env python3
# gv.py BASEDIR OUTDIR FUNCFILE : replace GetCarsIRColor in BASEDIR/rocket.c with FUNCFILE's text
import sys, shutil, os
base, out, ff = sys.argv[1:4]
if os.path.exists(out): shutil.rmtree(out)
shutil.copytree(base, out)
s = open(os.path.join(out, 'rocket.c')).read()
a = s.index('static Color GetCarsIRColor(Car *car, CarState *m)')
b = s.index('/* Accepted MAT3 AnimateFrame declaration')
s = s[:a] + open(ff).read().rstrip() + '\n\n' + s[b:]
open(os.path.join(out, 'rocket.c'), 'w').write(s)
