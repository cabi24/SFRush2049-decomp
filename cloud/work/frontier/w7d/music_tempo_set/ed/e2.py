s=s.replace("$1674:\n\tlw\t$5, 40($sp)\n","$1674:\n",1)
a="\tlh\t$3, 90($sp)\n\t.loc\t4045 60\n"
assert a in s; s=s.replace(a,"\tlh\t$3, 90($sp)\n\tlw\t$5, 40($sp)\n\t.loc\t4045 60\n",1)
