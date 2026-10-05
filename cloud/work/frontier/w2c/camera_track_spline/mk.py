#!/usr/bin/env python3
"""mk.py BODY.c OUT.c: splice a camera_track_spline definition into the camera_aspect_ratio group source."""
import sys, re
s = open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w2c/camera_track_spline/grp/base.c').read()
a = s.index('void camera_track_spline(Camera *cam) {')
b = s.index('void camera_process_input(Camera *cam) {')
body = open(sys.argv[1]).read()
open(sys.argv[2], 'w').write(s[:a] + body + '\n' + s[b:])
