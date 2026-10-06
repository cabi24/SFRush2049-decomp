V = {
 'p1': [("    CamNode *node;\n    s32 unused1;\n    s32 unused2;\n", "    s32 unused1;\n    s32 unused2;\n    s32 unused3;\n"),
        ("""        } else {
            if (sc->flags & 0x20) {""", """        } else {
            CamNode *node;

            if (sc->flags & 0x20) {""")],
 'p2': [("""        } else {
            if (sc->flags & 0x20) {""", """        } else {
            s32 unused3;

            if (sc->flags & 0x20) {""")],
 'p3': [("    s32 unused2;\n", "    s32 unused2;\n    s32 unused3;\n")],
}
