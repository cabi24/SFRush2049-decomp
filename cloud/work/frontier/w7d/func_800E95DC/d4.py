a="\tbeq\t$2, 0, $52\n\tbeq\t$2, 1, $53\n"
assert a in s
s=s.replace(a,"\tbeq\t$2, 0, $52\n\t.loc\t2 3859\n\tbeq\t$2, 1, $53\n",1)
