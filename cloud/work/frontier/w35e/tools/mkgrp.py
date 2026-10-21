#!/usr/bin/env python3
"""mkgrp.py NAME ROOT.c [BASEGROUP]: groups/NAME = BASEGROUP's group.c with func_803AD57C replaced by ROOT.c (default base groups/stats)"""
import sys, os, shutil
p = '/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35e/'
name, root = sys.argv[1], sys.argv[2]
base = sys.argv[3] if len(sys.argv) > 3 else p + 'groups/stats'
src = open(base + '/group.c').read()
j = src.index('s32 func_803AD57C(void *callback_context)')
os.makedirs(p + 'groups/' + name, exist_ok=True)
open(p + 'groups/' + name + '/group.c', 'w').write(src[:j] + open(root).read())
shutil.copy(base + '/group.json', p + 'groups/' + name + '/group.json')
