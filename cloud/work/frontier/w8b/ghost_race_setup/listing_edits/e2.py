exec(open('/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/e1.py').read())
a=s.index("\tsw\t$18, D_80140800\n")
b=s.index("$3427:")
seg=s[a:b]
seg2=seg.replace("\tlh\t$5, D_8014A108\n\tb\t$3428\n","\tb\t$3499\n")
s=s[:a]+seg2+s[b:]
k=s.rindex("\tlh\t$5, D_8014A108\n$3428:")
s=s[:k]+"$3499:\n"+s[k:]
