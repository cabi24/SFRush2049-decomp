E = """        } else {
            if (sc->flags & 0x20) {"""
V = {
 'q1': [(E, """        } else {
            s16 unused3;

            if (sc->flags & 0x20) {""")],
 'q2': [(E, """        } else {
            u8 unused3;

            if (sc->flags & 0x20) {""")],
 'q3': [("    s32 unused2;\n", "    s32 unused2;\n    u8 unused3;\n")],
 'q4': [("    s32 unused[11];\n", "    s32 unused[12];\n")],
}
