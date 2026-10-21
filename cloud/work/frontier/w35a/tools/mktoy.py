#!/usr/bin/env python3
"""mktoy.py SEG.c OUT.c : standalone toy (typedefs + externs) from a segment's D6A4 part"""
import sys
seg=open(sys.argv[1]).read()
i=seg.index('extern f32 fabsf')
pre=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35a/t/d6min.c').read()
pre=pre[:pre.index('extern f32 fabsf')]
open(sys.argv[2],'w').write(pre+seg[i:])
