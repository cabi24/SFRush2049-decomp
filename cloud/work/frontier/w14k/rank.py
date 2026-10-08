import re,sys
# usage: rank.py all.txt -> best strict first (parses label/value pairs, any order)
rows=[]
for line in open(sys.argv[1]):
    if 'COMPILE' in line or 'mnem' not in line and 'strict' not in line: continue
    d=dict(re.findall(r'(mnem-missing|aligned-missing|strict|size)\s+([+-]?\d+)',line))
    f=re.search(r'(v\d+\.c)',line)
    if 'strict' in d and f: rows.append((int(d['strict']),int(d['aligned-missing']),d['size'],f.group(1)))
rows.sort()
for r in rows[:3]: print('strict',r[0],'aligned',r[1],'size',r[2],r[3])
print('n=',len(rows))
