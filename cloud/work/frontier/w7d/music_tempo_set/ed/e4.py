s=s.replace("$1674:\n\tlw\t$5, 40($sp)\n","$1674:\n",1)
a="\tlbu\t$15, D_80153E88+7($14)\n"
assert a in s; s=s.replace(a,a+"\tlw\t$5, 40($sp)\n",1)
