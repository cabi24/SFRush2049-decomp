s=s.replace("\tlh\t$4, 30($18)\n\tmul\t$12, $10, 2056\n","\tlh\t$4, 30($18)\n",1)
a="\tlb\t$25, D_80156994\n"
assert a in s; s=s.replace(a,a+"\tmul\t$12, $10, 2056\n",1)
