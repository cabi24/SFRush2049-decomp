#!/usr/bin/env python3
"""split2.py IN OUT [anim] [init]: like split_helpers.py but only the named helpers."""
import sys
s = open(sys.argv[1]).read(); which = sys.argv[3:]
F = s.index("/* 0x8038FCE0 ShotUpdate */")
add = ""
protos = ""
if 'anim' in which:
    a0 = s.index("    for (casing = D_8039AE80.head; casing; casing = next) {", F)
    a1 = s.index("    for (i = 0, car = D_80152818; i < D_801543CA; i++, car++) {", F)
    old_anim = s[a0:a1]
    s = s[:a0] + "    func_8038FCD8();\n" + s[a1:]
    s = s[:F] + s[F:].replace("    ShotCase *casing, *next;\n    s32 idx;\n", "", 1)
    add += ("/* AnimateCase (MAT3 static, inlined into ShotUpdate; retail keeps the deleted-procedure stub) */\n"
            "void func_8038FCD8(void)\n{\n    ShotCase *casing, *next;\n    s32 idx;\n\n" + old_anim + "}\n\n")
    protos += "void func_8038FCD8(void);\n"
if 'init' in which:
    b0 = s.index("            shot->slotDamaged = 0;", F)
    b1 = s.index("            if (shot->type != 3)\n", F)
    init_old = s[b0:b1]
    s = s[:b0] + "            func_8038F560(shot, car);\n" + s[b1:]
    add += ("/* _InitNewShot (MAT3 static, inlined into ShotUpdate; retail keeps the deleted-procedure stub) */\n"
            "void func_8038F560(Shot *shot, Car *car)\n{\n" + init_old.replace("\n        ", "\n").replace("            ", "    ", 1) + "}\n\n")
    protos += "void func_8038F560(Shot *shot, Car *car);\n"
F = s.index("/* 0x8038FCE0 ShotUpdate */")
s = s[:F] + add + s[F:]
s = s.replace("extern void func_8038E114(void);", "extern void func_8038E114(void);\n" + protos, 1)
open(sys.argv[2], 'w').write(s)
