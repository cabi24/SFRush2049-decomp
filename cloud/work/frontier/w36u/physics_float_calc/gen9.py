import itertools, sys
src = open('v/q2.c').read()
head = src[:src.index("void physics_float_calc")].replace("void ShowObject(ObjInstance *obj);", "void ShowCull();\nvoid ShowParts();")
pre = src[src.index("void physics_float_calc"):src.index("    parts = D_8011E748")]
pre = pre.replace("    s32 dbg;\n", "    ObjInstance *obj;\n    s32 dbg;\n")
objforms = {
 'objb': ("    obj = &D_8012E700[D_80149D90];\n", "obj[i]", "&obj[i]"),
 'idx':  ("", "D_8012E700[D_80149D90 + i]", "&D_8012E700[D_80149D90 + i]"),
}
preargs = ['ptr', 'ptr, ptr_no_clip', 'best, ptr', 'ptr, parts', 'best, ptr, ptr_no_clip']
prepos = ['afterswitch', 'afterparts', 'afterloop']
cargs = ['%s', 'ptr_no_clip, w, %s', 'i, %s']
man = []; n = 0
for (ok, (bl, lv, oa)), pa, pp, ca in itertools.product(objforms.items(), preargs, prepos, cargs):
    p = pre
    if ok == 'idx': p = p.replace("    ObjInstance *obj;\n", "")
    a = ca.replace('%s', oa)
    call = "        if (dbg) {\n            ShowCull(%s);\n        }\n" % a
    pcall = "    if (dbg) {\n        ShowParts(%s);\n    }\n" % pa
    L = (pcall if pp == 'afterswitch' else '') + "    parts = D_8011E748[D_80149B8C];\n" + (pcall if pp == 'afterparts' else '') + "    mask = 1;\n    w = 0;\n" + bl
    L += "    for (i = 0; i < parts; i++) {\n"
    L += "        if (i && !(i & 31)) {\n            mask = 1;\n            w++;\n        }\n"
    L += call
    L += "        if (((mask & ptr[w]) || (ptr_no_clip ? mask & ptr_no_clip[w] : 0) || show) && !D_8014061A) {\n"
    L += "            %s.flags &= ~0x80000000;\n        } else {\n            %s.flags |= 0x80000000;\n        }\n" % (lv, lv)
    L += "        mask <<= 1;\n"
    L += "    }\n" + (pcall if pp == 'afterloop' else '') + "}\n"
    fn = 'b9/v%04d.c' % n
    open(fn, 'w').write(head + p + L)
    man.append('%s\t%s\t%s\t%s\t%s' % (fn, ok, pa, pp, a)); n += 1
open('b9/manifest.tsv', 'w').write('\n'.join(man) + '\n')
print(n)
