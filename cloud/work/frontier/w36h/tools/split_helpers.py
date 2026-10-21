#!/usr/bin/env python3
"""split_helpers.py IN OUT: move ShotUpdate's inlined AnimateCase / _InitNewShot bodies into functions
func_8038F560 (AnimateCase) and func_8038FCD8 (_InitNewShot) called from ShotUpdate."""
import sys
s = open(sys.argv[1]).read()
F = s.index("/* 0x8038FCE0 ShotUpdate */")
a0 = s.index("    for (casing = D_8039AE80.head; casing; casing = next) {")
a1 = s.index("    for (i = 0, car = D_80152818; i < D_801543CA; i++, car++) {", F)
old_anim = s[a0:a1]
s = s[:a0] + "    func_8038F560();\n" + s[a1:]
s = s[:F] + s[F:].replace("    ShotCase *casing, *next;\n    s32 idx;\n", "", 1)
b0 = s.index("            shot->slotDamaged = 0;")
b1 = s.index("            if (shot->type != 3)\n")
init_old = s[b0:b1]
s = s[:b0] + "            func_8038FCD8(shot, car);\n" + s[b1:]
anim = ("/* AnimateCase (MAT3 static, inlined into ShotUpdate; retail keeps the deleted-procedure stub) */\n"
        "void func_8038F560(void)\n{\n    ShotCase *casing, *next;\n    s32 idx;\n\n" + old_anim + "}\n\n")
init = ("/* _InitNewShot (MAT3 static, inlined into ShotUpdate; retail keeps the deleted-procedure stub) */\n"
        "void func_8038FCD8(Shot *shot, Car *car)\n{\n" + init_old.replace("\n        ", "\n").replace("            ", "    ", 1) + "}\n\n")
F = s.index("/* 0x8038FCE0 ShotUpdate */")
s = s[:F] + anim + init + s[F:]
s = s.replace("extern void func_8038E114(void);", "extern void func_8038E114(void);\nvoid func_8038F560(void);\nvoid func_8038FCD8(Shot *shot, Car *car);", 1)
open(sys.argv[2], 'w').write(s)
