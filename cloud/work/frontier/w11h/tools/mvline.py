# mvline.py in.s out.s SRCLINE(1-based) AFTER_SUBSTR  -- move one listing line to just after the first later line containing AFTER_SUBSTR
import sys
s=open(sys.argv[1]).read().split("\n")
i=int(sys.argv[3])-1; pat=sys.argv[4]
l=s.pop(i)
for k in range(i,len(s)):
    if pat in s[k] and not s[k].lstrip().startswith("#"): s.insert(k+1,l); break
open(sys.argv[2],"w").write("\n".join(s))
