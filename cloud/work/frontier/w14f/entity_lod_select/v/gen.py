import sys, os
src = open(sys.argv[1]).read()
out = sys.argv[2]
subs = {
 'A_noempty': [("                if (cmd[0] & 0x00FF0000) {\n                }\n", "")],
 'B_emptyafter': [("                if (cmd[0] & 0x00FF0000) {\n                }\n", ""), ("                first = 0;\n", "                first = 0;\n                if (cmd[0] & 0x00FF0000) {\n                }\n")],
 'C_wasint': [("was = first != 0;", "was = first;")],
 'E_segflip': [("if (seg < 0) {\n                val = (cmd[1] & 0x0F000000)", "if (seg >= 0) {\n                val = ((seg & 0xF) << 24) | 0;\n            } else {\n                val = (cmd[1] & 0x0F000000)")],
 'F_dec64': [("(op & 0xC0) == 0x40", "(op & 0xC0) == 64"), ("(op & 0xC0) == 0x80", "(op & 0xC0) == 128")],
 'G_s32op': [("u8 op;", "s32 op;")],
 'I_cmdfirst': [("    u32 *stk[10];\n    u32 *cmd;\n", "    u32 *cmd;\n    u32 *stk[10];\n")],
 'K_wasdrop': [("                was = first != 0;\n                first = 0;\n                if (was) {", "                was = first;\n                first = 0;\n                if (was != 0) {")],
 'L_typeorder': [("if (type == 1 && !((1 << idx) & mask))", "if (!((1 << idx) & mask) && type == 1)")],
 'M_u16idx': [("idx = cmd[0] & 0xFFFF;", "idx = (u16) cmd[0];")],
 'N_idxu32': [("    s32 idx;", "    u32 idx;")],
 'O_condswap': [("while (depth >= 0 && depth < 10)", "while (depth < 10 && depth >= 0)")],
 'P_gotoorder': [("(op >= 9 && op < 0x40) || (op >= 0xC0 && op < 0xD6)", "(op >= 0xC0 && op < 0xD6) || (op >= 9 && op < 0x40)")],
 'Q_valflip': [("val = ((cmd[1] + base) & 0x00FFFFFF) | ((seg & 0xF) << 24);", "val = (((seg & 0xF) << 24) | ((cmd[1] + base) & 0x00FFFFFF));")],
 'R_baseu': [("cmd[1] + base) & 0x00FFFFFF) | (w1", "cmd[1] + (u32) base) & 0x00FFFFFF) | (w1")],
 'S_savedfirst': [("            case 0xE1:\n                saved = cmd[1];", "            case 0xE1:\n                saved = cmd[1];\n                break;\n            case 0xE2:\n                saved = cmd[1];")],
 'T_opdecl': [("    u8 op;\n    u8 op2;", "    u8 op2;\n    u8 op;")],
 'U_w1decl': [("    s32 w1;\n    s32 depth;\n    s32 type;\n    s32 idx;", "    s32 depth;\n    s32 type;\n    s32 w1;\n    s32 idx;")],
 'V_typeu8': [("    s32 type;", "    u8 type;")],
 'W_nextdep': [("        if (op != 0xDE) {\n            if (depth >= 0) {\n                stk[depth] += 2;\n            }\n        }", "        if (op != 0xDE && depth >= 0) {\n            stk[depth] += 2;\n        }")],
 'X_firstzero': [("    first = 1;\n    depth = 0;", "    depth = 0;\n    first = 1;")],
 'Y_valinit': [("        cmd = stk[depth];", "        cmd = stk[depth];\n        val = val;")],
 'Z_unsigned_depth': [("    s32 depth;", "    u32 depth;")],
}
os.makedirs(out, exist_ok=True)
for name, reps in subs.items():
    s = src
    ok = True
    for a, b in reps:
        if a not in s:
            ok = False; print("MISS", name, repr(a[:40])); break
        s = s.replace(a, b, 1)
    if ok:
        open(os.path.join(out, name + ".c"), "w").write(s)
