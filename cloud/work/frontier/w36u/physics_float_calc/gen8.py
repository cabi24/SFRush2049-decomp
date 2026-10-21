import itertools, sys
src = open('v/q2.c').read()
head = src[:src.index("void physics_float_calc")].replace("void ShowObject(ObjInstance *obj);", "void ShowCull();")
pre = src[src.index("void physics_float_calc"):src.index("    parts = D_8011E748")]
pre = pre.replace("    s32 dbg;\n", "    ObjInstance *obj;\n    s32 dbg;\n")
objforms = {
 'objb': ("    obj = &D_8012E700[D_80149D90];\n", "obj[i]", "&obj[i]"),
 'idx':  ("", "D_8012E700[D_80149D90 + i]", "&D_8012E700[D_80149D90 + i]"),
}
incs = {'w': "            w++;\n", 'nc': "            ptr_no_clip++;\n", 'wnc': "            w++;\n            ptr_no_clip++;\n", 'none': ""}
terns = {'none': "", 'w': "|| (ptr_no_clip ? mask & ptr_no_clip[w] : 0) ", 'p': "|| (ptr_no_clip ? mask & *ptr_no_clip : 0) "}
cargs = ['ptr_no_clip, w, ptr, %s', 'ptr, w, %s', 'ptr_no_clip, ptr, %s', '%s', 'ptr_no_clip, %s', '%s, ptr_no_clip', 'i, %s', 'w, %s', '%s, w', 'ptr_no_clip, w, %s', 'ptr, %s']
man = []; n = 0
for (ok, (bl, lv, oa)), (ik, inc), (tk, tern), ca, cp in itertools.product(objforms.items(), incs.items(), terns.items(), cargs, ('top', 'end')):
    p = pre
    if ok == 'idx': p = p.replace("    ObjInstance *obj;\n", "")
    a = ca.replace('%s', oa)
    call = "        if (dbg) {\n            ShowCull(%s);\n        }\n" % a
    L = "    parts = D_8011E748[D_80149B8C];\n    mask = 1;\n    w = 0;\n" + bl
    L += "    for (i = 0; i < parts; i++) {\n"
    L += "        if (i && !(i & 31)) {\n            mask = 1;\n" + inc + "        }\n"
    if cp == 'top': L += call
    L += "        if (((mask & ptr[w]) %s|| show) && !D_8014061A) {\n" % tern
    L += "            %s.flags &= ~0x80000000;\n        } else {\n            %s.flags |= 0x80000000;\n        }\n" % (lv, lv)
    L += "        mask <<= 1;\n"
    if cp == 'end': L += call
    L += "    }\n}\n"
    fn = 'b8/v%04d.c' % n
    open(fn, 'w').write(head + p + L)
    man.append('%s\t%s\tinc=%s\ttern=%s\t%s\t%s' % (fn, ok, ik, tk, a, cp)); n += 1
open('b8/manifest.tsv', 'w').write('\n'.join(man) + '\n')
print(n)
