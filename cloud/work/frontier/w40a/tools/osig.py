#!/usr/bin/env python3
"""Pi side: osig.py OBJ [NAME]  -> rows/ops/len/sig for NAME (default entity_ai_pathfind) in OBJ (gbatch metrics)."""
import sys, os
sys.path.insert(0, '/home/cburnes/projects/rush2049-decomp/tools/cloud')
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
obj = os.path.abspath(sys.argv[1]); os.chdir('/home/cburnes/projects/rush2049-decomp')
import score, gbatch
n = sys.argv[2] if len(sys.argv) > 2 else 'entity_ai_pathfind'
print(n, 'rows %d ops %d len%+d %s' % gbatch.rows(obj, n))
