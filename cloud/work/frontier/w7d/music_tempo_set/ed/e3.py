s=s.replace("$1674:\n\tlw\t$5, 40($sp)\n","$1674:\n",1)
a="\tlb\t$2, D_80146180($3)\n"
assert a in s; s=s.replace(a,a+"\tlw\t$5, 40($sp)\n",1)
