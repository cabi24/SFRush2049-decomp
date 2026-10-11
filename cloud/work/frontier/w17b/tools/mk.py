#!/usr/bin/env python3
"""mk.py BASE.c E398C.inc OUT.c: replace func_800E398C's definition in BASE with the .inc body."""
import sys
s=open(sys.argv[1]).read(); body=open(sys.argv[2]).read()
a=s.index('void func_800E398C(s16 drone_index) {'); b=s.index('#define range_it')
open(sys.argv[3],'w').write(s[:a]+body.rstrip()+'\n\n'+s[b:])
