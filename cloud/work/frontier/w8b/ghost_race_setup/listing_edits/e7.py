a = "\tmove\t$2, $4\n\tbeq\t$2, 0, $3525\n"
assert a in s
s = s.replace(a, "\tmove\t$2, $4\n\tmove\t$16, $11\n\tbeq\t$2, 0, $3525\n", 1)
b = "$3525:\n\t.loc\t1254 3845\n\tmove\t$16, $11\n"
assert b in s
s = s.replace(b, "$3525:\n\t.loc\t1254 3845\n", 1)
