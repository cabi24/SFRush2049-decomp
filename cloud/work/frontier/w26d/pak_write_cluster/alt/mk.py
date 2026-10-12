# usage: python3 mk.py OUT.c [piece=FILE ...]   default pieces from p/<piece>.c
import sys, os
d=os.path.dirname(os.path.abspath(__file__))
order=['head','a1bb4','a1c6c','a1dd4','tdd','tcs','mpz','ssl']
over=dict(a.split('=',1) for a in sys.argv[2:])
out=''.join(open(over.get(n, os.path.join(d,'pieces',n+'.c'))).read() for n in order)
open(sys.argv[1],'w').write(out)
