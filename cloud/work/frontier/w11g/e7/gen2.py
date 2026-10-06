src=open('base.c').read()
anchors={
 'a_mag':"            magnitude = sample;\n",
 'a_if6':"            if (D_8014A110 == 6) {\n",
 'a_cmp':"            if (curve->level[0] * scale <= magnitude) {\n",
 'a_lo':"                lo = curve->level;\n",
 'a_hi':"                hi = lo[4];\n",
 'a_sw':"            switch (i) {\n",
}
for k,a in anchors.items():
    assert src.count(a)==1,k
    ind=a[:len(a)-len(a.lstrip())]
    open('v2/%s.c'%k,'w').write(src.replace(a, ind+"segment = 0;\n"+a))
