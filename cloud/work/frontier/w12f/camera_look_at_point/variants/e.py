O = """tt = 0;
if (b < 0) {
 tt = 1;
 }
 if (tt) {}
"""
D = ("    s32 tt;\n", "")
V = {
 'e1': [D, (O, "                y = 0;\n                if (b < 0) {\n                    y = 1;\n                }\n")],
 'e2': [D, (O, "                x = 0;\n                if (b < 0) {\n                    x = 1;\n                }\n")],
 'e3': [D, (O, "                y = b;\n                if (b < 0) {\n                    y = -b;\n                }\n")],
 'e4': [D, (O, "                if (b < 0) {\n                    y = -b;\n                }\n")],
 'e5': [D, (O, "                if (b < 0) {\n                    x = 1;\n                }\n")],
}
