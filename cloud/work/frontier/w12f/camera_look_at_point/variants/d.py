O = """tt = 0;
if (b < 0) {
 tt = 1;
 }
 if (tt) {}
"""
V = {
 'd1': [(O, "                if (b < 0) do { } while (0);\n"), ("    s32 tt;\n", "")],
 'd2': [(O, "                if (b < 0) {\n                    goto skip;\n                }\nskip:\n"), ("    s32 tt;\n", "")],
 'd3': [(O, "                while (b < 0) {\n                    break;\n                }\n"), ("    s32 tt;\n", "")],
 'd4': [(O, "                if (b < 0) {\n                } else {\n                }\n"), ("    s32 tt;\n", "")],
}
