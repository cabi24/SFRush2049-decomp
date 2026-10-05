#!/usr/bin/env python3
"""poison.py LISTING.s : report .noalias regions that are still open at an unconditional branch
(the .alias closer is emitted after `b`/`j`), which stops as1's cross-block hoisting for the rest of
the function (measured on entity_process_main, 2026-10-04). Input: ugen listing from o3s.sh."""
import sys,re
L=[l.rstrip('\n') for l in open(sys.argv[1]) if not l.startswith(' #') and '.loc' not in l]
n=0
for i,l in enumerate(L):
    if re.match(r'\t\.alias\t',l):
        j=i-1
        while j>=0 and re.match(r'\t\.(no)?alias\t',L[j]): j-=1
        if re.match(r'\t(b|j)\t',L[j]):
            n+=1
            print('open at jump: line %d: %s   <- %s' % (i+1,l.strip(),L[j].strip()))
print('%d poisoned closers; %d .noalias total' % (n,sum(1 for l in L if '.noalias' in l)))
