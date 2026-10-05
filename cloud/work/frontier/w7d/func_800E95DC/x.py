import os
k=s.index(' #3860')
pre,post=s[:k],s[k:]
blk="\tmove\t$16, $11\n\tmul\t$8, $23, 4\n\tl.s\t$f22, D_801526F8($8)\n\tl.s\t$f24, D_801526E0($8)\n\taddu\t$20, $sp, 172\n\tsw\t$8, 140($sp)\n\tsw\t$10, 240($sp)\n\tsw\t$11, 236($sp)\n\tsw\t$13, 136($sp)\n"
assert blk in post
L=blk.strip('\n').split('\n')
mv=L.pop(0)
pos=int(os.environ.get('POS','0'))
L.insert(pos,mv)
post=post.replace(blk,'\n'.join(L)+'\n',1)
s=pre+post
