a="\tbeq\t$2, 1, $53\n\tbeq\t$2, 2, $54\n"
assert a in s
s=s.replace(a,"\tbeq\t$2, 1, $53\n$99:\n\tbeq\t$2, 2, $54\n",1)
