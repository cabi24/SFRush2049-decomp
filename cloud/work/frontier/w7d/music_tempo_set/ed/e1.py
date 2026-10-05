a="$1674:\n\tlw\t$5, 40($sp)\n\t.loc\t4045 63\n\t.noalias\t$16,$sp\n\tmul\t$24, $3, 2056\n\tla\t$25, D_8014A250\n\taddu\t$16, $24, $25\n"
b="$1674:\n\t.loc\t4045 63\n\t.noalias\t$16,$sp\n\tmul\t$24, $3, 2056\n\tla\t$25, D_8014A250\n\taddu\t$16, $24, $25\n\tlw\t$5, 40($sp)\n"
assert a in s; s=s.replace(a,b)
