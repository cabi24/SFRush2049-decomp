import sys,subprocess,re
L=open('listing/s.s').read().split('\n')
def run(lines,name):
    p='listing/b_%s.s'%name; open(p,'w').write('\n'.join(lines))
    out=subprocess.run(['sh','../tools/asmdis.sh','rcd',p],capture_output=True,text=True).stdout.split('\n')
    # find last 'lb v0,0(a1)'
    idx=[i for i,l in enumerate(out) if re.search(r'lb\tv0,0\(a1\)',l)]
    if not idx: return 'nolb '+out[-1] if out else 'fail'
    i=idx[-1]
    return ' / '.join(x.split(':',1)[1].strip() for x in out[i:i+4])
def isinst(l):
    s=l.strip()
    return s and not s.startswith('.') and not s.endswith(':') and l.startswith('\t')
for arg in sys.argv[1:]:
    rs=[tuple(map(int,x.split('-'))) for x in arg.split(',')]
    lines=[l for i,l in enumerate(L,1) if not (any(a<=i<=b for a,b in rs) and isinst(l))]
    print(arg,run(lines,arg.replace(',','_')),flush=True)
