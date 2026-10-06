O = """tt = 0;
if (b < 0) {
 tt = 1;
 }
 if (tt) {}
"""
D = ("    s32 tt;\n", "")
V = {
 'g1': [D, (O, "                x = 0;\n                if (b < 0) {\n                    x = 1;\n                }\n                if (x) {\n                }\n")],
 'g2': [D, (O, "                y = 0;\n                if (b < 0) {\n                    y = 1;\n                }\n                if (y) {\n                }\n")],
 'g3': [(O, "                tt = 0;\n                if (b < 0) {\n                    tt = 1;\n                }\n                if (tt) {\n                }\n")],
}
